#include "WorkerInspectorClient.h"

#include <sys/eventfd.h>
#include <unistd.h>

#include <chrono>

#include <include/libplatform/libplatform.h>
#include <src/inspector/string-util.h>
#include <src/inspector/v8-inspector-impl.h>
#include <src/inspector/v8-stack-trace-impl.h>

#include "JsV8InspectorClient.h"
#include "Runtime.h"

using namespace v8;
using namespace v8_inspector;

namespace tns {

namespace {

StringView Make8BitStringView(const std::string& value) {
    return StringView(reinterpret_cast<const uint8_t*>(value.data()), value.size());
}

// Pure C++ (no V8 handles): legal from the Channel callbacks regardless of
// what scopes the inspector entered before calling them.
std::string ToUtf8String(const StringView& view) {
    return v8_inspector::toString16(view).utf8();
}

}  // namespace

WorkerInspectorClient::WorkerInspectorClient(int workerId, Isolate* isolate, ALooper* workerLooper,
                                             const std::string& url)
    : workerId_(workerId),
      sessionId_("NS_WORKER_" + std::to_string(workerId)),
      targetId_("ns-worker-" + std::to_string(workerId)),
      url_(url),
      isolate_(isolate),
      workerLooper_(workerLooper) {
    // Wakes the worker looper when CDP messages arrive on the socket thread;
    // same mechanism as the worker's message inbox (ConcurrentQueue).
    eventFd_ = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (eventFd_ != -1 && workerLooper_ != nullptr &&
            ALooper_addFd(workerLooper_, eventFd_, ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT,
                          WorkerInspectorClient::InspectorMessagesCallback, this) == 1) {
        ALooper_acquire(workerLooper_);
    } else {
        if (eventFd_ != -1) {
            close(eventFd_);
            eventFd_ = -1;
        }
        workerLooper_ = nullptr;
    }

    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    HandleScope handle_scope(isolate_);
    Local<Context> context = Runtime::GetRuntime(isolate_)->GetContext();

    inspector_ = V8Inspector::create(isolate_, this);
    // Name the context after the worker script: the DevTools console context
    // selector labels entries with the context's name (or its origin as a
    // fallback) — with neither set the dropdown rows are blank and
    // unselectable.
    V8ContextInfo contextInfo(context, contextGroupId, Make8BitStringView(url_));
    contextInfo.origin = Make8BitStringView(url_);
    inspector_->contextCreated(contextInfo);
    context_.Reset(isolate_, context);
    session_ = inspector_->connect(contextGroupId, this, {});
}

WorkerInspectorClient::~WorkerInspectorClient() {
    dying_ = true;

    if (eventFd_ != -1) {
        // Runs on the worker thread, so unregistering from its own looper is
        // safe (same rule as ConcurrentQueue::Terminate).
        ALooper_removeFd(workerLooper_, eventFd_);
        close(eventFd_);
        eventFd_ = -1;
        ALooper_release(workerLooper_);
    }

    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    HandleScope handle_scope(isolate_);
    if (session_ != nullptr) {
        session_->resume();
        session_.reset();
    }
    inspector_.reset();
    context_.Reset();
}

int WorkerInspectorClient::InspectorMessagesCallback(int fd, int events, void* data) {
    // Clear the eventfd counter first or the level-triggered looper spins.
    uint64_t value;
    read(fd, &value, sizeof(value));

    static_cast<WorkerInspectorClient*>(data)->DrainIncoming();
    return 1;
}

void WorkerInspectorClient::PushMessage(const std::string& message) {
    if (dying_) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(incomingMutex_);
        incoming_.push(message);
    }

    if (eventFd_ != -1) {
        // Wakes an idle worker pumping its looper. While paused the looper is
        // not pumping — the condition variable below wakes the nested pause
        // loop instead, and the still-readable fd fires one (possibly empty)
        // drain after resume.
        uint64_t value = 1;
        write(eventFd_, &value, sizeof(value));
    }
    messageArrived_.notify_all();
}

std::string WorkerInspectorClient::PopMessage() {
    std::lock_guard<std::mutex> lock(incomingMutex_);
    if (incoming_.empty()) {
        return "";
    }
    std::string message = incoming_.front();
    incoming_.pop();
    return message;
}

void WorkerInspectorClient::DrainIncoming() {
    std::string message;
    while (!dying_ && !(message = this->PopMessage()).empty()) {
        this->DispatchOne(message);
    }
    this->MaybeResetSession();
}

void WorkerInspectorClient::DispatchOne(const std::string& message) {
    if (message == kResetSessionMessage) {
        this->HandleResetRequest();
        return;
    }

    if (session_ == nullptr) {
        return;
    }

    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    HandleScope handle_scope(isolate_);
    session_->dispatchProtocolMessage(Make8BitStringView(message));
    isolate_->PerformMicrotaskCheckpoint();
}

void WorkerInspectorClient::HandleResetRequest() {
    if (runningPauseLoop_.load(std::memory_order_acquire)) {
        // We're inside session_->dispatchProtocolMessage somewhere up the stack —
        // resume now (which exits the nested pause loop) and swap the session
        // only once that stack has fully unwound, from DrainIncoming.
        pendingReset_ = true;
        {
            v8::Locker locker(isolate_);
            Isolate::Scope isolate_scope(isolate_);
            HandleScope handle_scope(isolate_);
            if (session_ != nullptr) {
                session_->resume();
            }
        }
        if (eventFd_ != -1) {
            uint64_t value = 1;
            write(eventFd_, &value, sizeof(value));
        }
        return;
    }

    this->DoResetSession();
}

void WorkerInspectorClient::DoResetSession() {
    v8::Locker locker(isolate_);
    Isolate::Scope isolate_scope(isolate_);
    HandleScope handle_scope(isolate_);
    if (session_ != nullptr) {
        session_->resume();
        session_.reset();
    }
    session_ = inspector_->connect(contextGroupId, this, {});
}

void WorkerInspectorClient::MaybeResetSession() {
    if (pendingReset_ && !runningPauseLoop_.load(std::memory_order_acquire) && !dying_) {
        pendingReset_ = false;
        this->DoResetSession();
    }
}

void WorkerInspectorClient::runMessageLoopOnPause(int contextGroupId) {
    if (runningPauseLoop_.load(std::memory_order_acquire) || dying_) {
        return;
    }
    runningPauseLoop_.store(true, std::memory_order_release);
    pauseTerminated_ = false;

    while (!pauseTerminated_ && !dying_) {
        std::string message = this->PopMessage();
        bool shouldWait = message.empty();
        if (!shouldWait) {
            this->DispatchOne(message);
        }

        while (v8::platform::PumpMessageLoop(Runtime::platform, isolate_)) {
        }

        if (shouldWait && !pauseTerminated_ && !dying_) {
            std::unique_lock<std::mutex> lock(messageArrivedMutex_);
            messageArrived_.wait_for(lock, std::chrono::milliseconds(1));
        }
    }

    runningPauseLoop_.store(false, std::memory_order_release);
}

void WorkerInspectorClient::quitMessageLoopOnPause() {
    pauseTerminated_ = true;
}

void WorkerInspectorClient::NotifyTerminating() {
    dying_ = true;
    pauseTerminated_ = true;
    messageArrived_.notify_all();
}

void WorkerInspectorClient::RequestPauseInterrupt() {
    isolate_->RequestInterrupt(
        [](Isolate* isolate, void* data) {
            // Runs on the worker thread mid-JS. Teardown also happens on the
            // worker thread, so the client either still exists or this resolves
            // to nothing — re-resolve through the registry instead of capturing
            // the pointer.
            int workerId = static_cast<int>(reinterpret_cast<intptr_t>(data));
            JsV8InspectorClient* root = JsV8InspectorClient::GetInstanceIfCreated();
            if (root != nullptr) {
                root->SchedulePauseInWorker(workerId);
            }
        },
        reinterpret_cast<void*>(static_cast<intptr_t>(workerId_)));
}

void WorkerInspectorClient::SchedulePauseFromInterrupt() {
    if (session_ != nullptr) {
        session_->schedulePauseOnNextStatement({}, {});
    }
}

void WorkerInspectorClient::sendResponse(int callId, std::unique_ptr<StringBuffer> message) {
    this->SendWrapped(ToUtf8String(message->string()));
}

void WorkerInspectorClient::sendNotification(std::unique_ptr<StringBuffer> message) {
    this->SendWrapped(ToUtf8String(message->string()));
}

void WorkerInspectorClient::flushProtocolNotifications() {
}

void WorkerInspectorClient::SendWrapped(const std::string& message) {
    if (message.empty() || message[0] != '{') {
        return;
    }

    // Flat-session protocol: tag everything this session emits with its
    // sessionId so the frontend routes it to the right child target.
    std::string wrapped;
    wrapped.reserve(message.size() + sessionId_.size() + 16);
    wrapped += "{\"sessionId\":\"";
    wrapped += sessionId_;
    wrapped += "\"";
    if (message.size() > 2) {
        wrapped += ",";
    }
    wrapped.append(message, 1, std::string::npos);

    JsV8InspectorClient* root = JsV8InspectorClient::GetInstanceIfCreated();
    if (root != nullptr) {
        root->SendToFrontend(wrapped);
    }
}

void WorkerInspectorClient::consoleLog(ConsoleAPIType method,
                                       const std::vector<Local<Value>>& args) {
    if (inspector_ == nullptr) {
        return;
    }

    // Note, here we access private V8 API (mirrors
    // JsV8InspectorClient::consoleLogCallback).
    auto* impl = reinterpret_cast<V8InspectorImpl*>(inspector_.get());

    Local<StackTrace> stack =
        StackTrace::CurrentStackTrace(isolate_, 1, StackTrace::StackTraceOptions::kDetailed);
    std::unique_ptr<V8StackTraceImpl> stackImpl = impl->debugger()->createStackTrace(stack);

    Local<Context> context = context_.Get(isolate_);
    const int contextId = V8ContextInfo::executionContextId(context);

    std::unique_ptr<V8ConsoleMessage> msg = V8ConsoleMessage::createForConsoleAPI(
        context, contextId, contextGroupId, impl, this->currentTimeMS(), method, args, String16{},
        std::move(stackImpl));

    // Going through the message storage both reports to the session when the
    // frontend has enabled the Runtime agent AND keeps the message for replay
    // on Runtime.enable. Workers log most of their output (module top-level,
    // early onmessage work) before DevTools attaches and enables the session;
    // delivering straight to the runtime agent would silently drop all of it.
    impl->ensureConsoleMessageStorage(contextGroupId)->addMessage(std::move(msg));
}

Local<Context> WorkerInspectorClient::ensureDefaultContextInGroup(int contextGroupId) {
    return context_.Get(isolate_);
}

}  // namespace tns
