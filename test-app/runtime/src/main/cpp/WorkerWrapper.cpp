#include "WorkerWrapper.h"

#include <android/looper.h>
#include <pthread.h>

#include <thread>

#include "ArgConverter.h"
#include "CallbackHandlers.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "Runtime.h"

using namespace v8;

namespace tns {

WorkerWrapper::WorkerWrapper(Isolate* parentIsolate, int workerId, std::string workerPath,
                             std::string callingDir, int priority,
                             Local<Object> workerObject)
        : parentIsolate_(parentIsolate),
          // runs on the parent's thread, where the parent runtime is alive
          parentTasks_(Runtime::GetRuntime(parentIsolate)->GetLooperTasks()),
          workerIsolate_(nullptr),
          runtime_(nullptr),
          workerId_(workerId),
          workerPath_(std::move(workerPath)),
          callingDir_(std::move(callingDir)),
          // workerPath_ (not workerPath) - the parameter was just moved from
          threadName_("W" + std::to_string(workerId) + ": " + workerPath_),
          priority_(priority),
          poWorker_(new Persistent<Object>(parentIsolate, workerObject)),
          isClosing_(false),
          isTerminating_(false),
          isDisposed_(false),
          javaLooperRef_(nullptr) {}

void WorkerWrapper::Start() {
    auto self = shared_from_this();
    std::thread thread([self]() {
        self->BackgroundLooper(self);
    });
    thread.detach();
}

void WorkerWrapper::PostMessage(std::shared_ptr<worker::Message> message) {
    if (!isTerminating_ && !isClosing_) {
        queue_.Push(message);
    }
}

void WorkerWrapper::PostMessageToParent(std::shared_ptr<worker::Message> message) {
    if (isTerminating_) {
        return;
    }

    auto parentTasks = parentTasks_.lock();
    if (parentTasks == nullptr) {
        // the parent runtime is gone (e.g. a parent worker that shut down)
        return;
    }

    int workerId = workerId_;
    parentTasks->Post([workerId, message]() {
        WorkerWrapper::FireMessageOnParentWorkerObject(workerId, message);
    });
}

void WorkerWrapper::Terminate() {
    if (isClosing_ || isDisposed_) {
        // The worker is already shutting down on its own; nothing to do.
        return;
    }

    bool wasTerminating = isTerminating_.exchange(true);
    if (wasTerminating) {
        return;
    }

    Isolate* isolate = workerIsolate_.load();
    if (isolate != nullptr) {
        // The only v8 call that is legal from another thread - interrupts any
        // JS currently running on the worker (e.g. a busy loop).
        isolate->TerminateExecution();
    }

    QuitLooper();
}

void WorkerWrapper::Close() {
    bool wasClosing = isClosing_.exchange(true);
    if (wasClosing) {
        return;
    }

    // Once the current callback unwinds, Looper.loop() returns and the
    // thread proceeds to cleanup. Pending messages are dropped, matching the
    // previous front-of-queue TerminateAndCloseThread behavior.
    QuitLooper();
}

void WorkerWrapper::QuitLooper() {
    std::lock_guard<std::mutex> lock(looperMutex_);
    if (javaLooperRef_ != nullptr) {
        JEnv env;
        env.CallVoidMethod(javaLooperRef_, LOOPER_QUIT_METHOD_ID);
    }
}

int WorkerWrapper::DrainCallback(int fd, int events, void* data) {
    uint64_t value;
    read(fd, &value, sizeof(value));

    auto wrapper = static_cast<WorkerWrapper*>(data);
    wrapper->DrainPendingTasks();
    return 1;
}

void WorkerWrapper::DrainPendingTasks() {
    Isolate* isolate = workerIsolate_.load();
    if (isolate == nullptr || isTerminating_) {
        return;
    }

    auto messages = queue_.PopAll();
    if (messages.empty()) {
        return;
    }

    v8::Locker locker(isolate);
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);
    auto context = runtime_->GetContext();
    Context::Scope context_scope(context);
    auto globalObject = context->Global();

    for (auto& message : messages) {
        if (isTerminating_ || isClosing_) {
            break;
        }

        TryCatch tc(isolate);

        Local<Value> callback;
        if (!globalObject->Get(context, ArgConverter::ConvertToV8String(isolate, "onmessage"))
                     .ToLocal(&callback) ||
            !callback->IsFunction()) {
            DEBUG_WRITE(
                    "WORKER: couldn't fire a worker's `onmessage` callback because it isn't implemented!");
            continue;
        }

        Local<Value> data;
        if (message->Deserialize(isolate, context).ToLocal(&data)) {
            auto event = Object::New(isolate);
            event->DefineOwnProperty(context, ArgConverter::ConvertToV8String(isolate, "data"),
                                     data, PropertyAttribute::ReadOnly);
            Local<Value> args[] = {event};
            callback.As<Function>()->Call(context, Undefined(isolate), 1, args);
        }

        if (tc.HasCaught() && !isTerminating_) {
            CallbackHandlers::CallWorkerScopeOnErrorHandle(isolate, tc);
        }
    }
}

void WorkerWrapper::FireMessageOnParentWorkerObject(int workerId,
                                                  std::shared_ptr<worker::Message> message) {
    auto wrapper = WorkerWrapper::GetById(workerId);
    if (wrapper == nullptr) {
        DEBUG_WRITE("MAIN: no worker instance was found with workerId=%d.", workerId);
        return;
    }

    Isolate* isolate = wrapper->parentIsolate_;
    v8::Locker locker(isolate);
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);

    if (wrapper->poWorker_ == nullptr || wrapper->poWorker_->IsEmpty()) {
        DEBUG_WRITE(
                "MAIN: couldn't fire a worker(id=%d) object's `onmessage` callback because the worker has been cleared.",
                workerId);
        return;
    }

    auto worker = Local<Object>::New(isolate, *wrapper->poWorker_);
    auto context = Runtime::GetRuntime(isolate)->GetContext();
    Context::Scope context_scope(context);

    try {
        Local<Value> callback;
        if (!worker->Get(context, ArgConverter::ConvertToV8String(isolate, "onmessage"))
                     .ToLocal(&callback) ||
            !callback->IsFunction()) {
            DEBUG_WRITE(
                    "MAIN: couldn't fire a worker(id=%d) object's `onmessage` callback because it isn't implemented.",
                    workerId);
            return;
        }

        Local<Value> data;
        if (message->Deserialize(isolate, context).ToLocal(&data)) {
            auto event = Object::New(isolate);
            event->DefineOwnProperty(context, ArgConverter::ConvertToV8String(isolate, "data"),
                                     data, PropertyAttribute::ReadOnly);
            Local<Value> args[] = {event};
            callback.As<Function>()->Call(context, Undefined(isolate), 1, args);
        }
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
    }
}

void WorkerWrapper::PassUncaughtExceptionFromWorkerToParent(const std::string& message,
                                                          const std::string& filename,
                                                          const std::string& stackTrace,
                                                          int lineno) {
    auto parentTasks = parentTasks_.lock();
    if (parentTasks == nullptr) {
        // the parent runtime is gone (e.g. a parent worker that shut down)
        return;
    }

    int workerId = workerId_;
    std::string threadName = threadName_;
    Isolate* parentIsolate = parentIsolate_;

    parentTasks->Post([workerId, message, filename, stackTrace, lineno, threadName,
                       parentIsolate]() {
        v8::Locker locker(parentIsolate);
        Isolate::Scope isolate_scope(parentIsolate);
        HandleScope handle_scope(parentIsolate);
        auto context = Runtime::GetRuntime(parentIsolate)->GetContext();
        Context::Scope context_scope(context);

        WorkerWrapper::FireErrorOnParentWorkerObject(workerId, message, stackTrace, filename,
                                                     lineno, threadName);
    });
}

void WorkerWrapper::FireErrorOnParentWorkerObject(int workerId, const std::string& message,
                                                const std::string& stackTrace,
                                                const std::string& filename, int lineno,
                                                const std::string& threadName) {
    auto wrapper = WorkerWrapper::GetById(workerId);
    if (wrapper == nullptr) {
        DEBUG_WRITE("MAIN: no worker instance was found with workerId=%d.", workerId);
        return;
    }

    Isolate* isolate = wrapper->parentIsolate_;

    try {
        if (wrapper->poWorker_ == nullptr || wrapper->poWorker_->IsEmpty()) {
            DEBUG_WRITE(
                    "MAIN: couldn't fire a worker(id=%d) object's `onerror` callback because the worker has been cleared.",
                    workerId);
            return;
        }

        auto worker = Local<Object>::New(isolate, *wrapper->poWorker_);
        auto context = Runtime::GetRuntime(isolate)->GetContext();

        Local<Value> callback;
        bool hasOnError =
                worker->Get(context, ArgConverter::ConvertToV8String(isolate, "onerror"))
                        .ToLocal(&callback) &&
                callback->IsFunction();

        if (hasOnError) {
            auto errEvent = Object::New(isolate);
            errEvent->Set(context, ArgConverter::ConvertToV8String(isolate, "message"),
                          ArgConverter::ConvertToV8String(isolate, message));
            errEvent->Set(context, ArgConverter::ConvertToV8String(isolate, "stackTrace"),
                          ArgConverter::ConvertToV8String(isolate, stackTrace));
            errEvent->Set(context, ArgConverter::ConvertToV8String(isolate, "filename"),
                          ArgConverter::ConvertToV8String(isolate, filename));
            errEvent->Set(context, ArgConverter::ConvertToV8String(isolate, "lineno"),
                          Number::New(isolate, lineno));

            Local<Value> args[] = {errEvent};

            // If the handler returns a truthy value the exception is handled
            // and must not be raised to application level
            Local<Value> result;
            callback.As<Function>()->Call(context, Undefined(isolate), 1, args).ToLocal(&result);
            if (!result.IsEmpty() && result->BooleanValue(isolate)) {
                return;
            }
        }

        DEBUG_WRITE(
                "Unhandled exception in '%s' thread. file: %s, line %d, message: %s\nStackTrace: %s",
                threadName.c_str(), filename.c_str(), lineno, message.c_str(),
                stackTrace.c_str());
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
    }
}

void WorkerWrapper::BackgroundLooper(std::shared_ptr<WorkerWrapper> self) {
    JavaVM* jvm = Runtime::GetJVM();
    JNIEnv* jniEnv = nullptr;

    JavaVMAttachArgs attachArgs;
    attachArgs.version = JNI_VERSION_1_6;
    attachArgs.name = const_cast<char*>(threadName_.c_str());
    attachArgs.group = nullptr;
    jvm->AttachCurrentThread(&jniEnv, &attachArgs);

    // pthread names are limited to 15 chars
    pthread_setname_np(pthread_self(), threadName_.substr(0, 15).c_str());

    int runtimeId = -1;

    try {
        JEnv env;

        // Performs the cgroup/scheduling-policy move in addition to the nice
        // value, exactly like the previous Java-side
        // Process.setThreadPriority(THREAD_PRIORITY_BACKGROUND) call.
        env.CallStaticVoidMethod(PROCESS_CLASS, SET_THREAD_PRIORITY_METHOD_ID, priority_);

        if (!isTerminating_ && !isClosing_) {
            // Prepares the Java Looper for this thread and creates the
            // per-worker com.tns.Runtime (which creates the worker isolate on
            // this thread via initNativeScript -> PrepareV8Runtime).
            JniLocalRef callingDir(env.NewStringUTF(callingDir_.c_str()));
            runtimeId = env.CallStaticIntMethod(RUNTIME_CLASS, INIT_WORKER_RUNTIME_METHOD_ID,
                                                workerId_, (jstring) callingDir);
            runtime_ = Runtime::GetRuntime(runtimeId);

            {
                std::lock_guard<std::mutex> lock(looperMutex_);
                JniLocalRef looper(env.CallStaticObjectMethod(LOOPER_CLASS, MY_LOOPER_METHOD_ID));
                javaLooperRef_ = env.NewGlobalRef(looper);
            }

            // Looper.prepare() ran above, so ALooper_forThread() returns the
            // native looper backing the Java one - fds added here are pumped
            // by Looper.loop().
            queue_.Initialize(ALooper_forThread(), WorkerWrapper::DrainCallback, this);

            Isolate* isolate = runtime_->GetIsolate();

            {
                v8::Locker locker(isolate);
                Isolate::Scope isolate_scope(isolate);
                HandleScope handle_scope(isolate);
                isolate->SetData((uint32_t) Runtime::IsolateData::WORKER_WRAPPER, this);
                workerIsolate_.store(isolate);

                auto context = runtime_->GetContext();
                Context::Scope context_scope(context);

                // (A future worker inspector would be created here, before the
                // script runs, mirroring the iOS runtime.)

                if (!isTerminating_) {
                    runtime_->RunWorker(workerPath_);
                }
            }

            // Deliver messages that were posted before the worker was ready
            // (replaces the old Java Handshake + pendingWorkerMessages).
            DrainPendingTasks();

            if (!isTerminating_ && !isClosing_) {
                // Blocks, pumping Java Handler messages (cross-thread Java->JS
                // calls), timers and the worker inbox until quit() is called.
                env.CallStaticVoidMethod(RUNTIME_CLASS, RUN_WORKER_LOOP_METHOD_ID);
            }
        }
    } catch (NativeScriptException& ex) {
        if (jniEnv->ExceptionCheck()) {
            jniEnv->ExceptionClear();
        }
        if (!isTerminating_) {
            PassUncaughtExceptionFromWorkerToParent(ex.GetErrorMessage(), workerPath_, "", 0);
        }
    } catch (std::exception& ex) {
        DEBUG_WRITE_FORCE("Worker(id=%d) error: c++ exception: %s", workerId_, ex.what());
    } catch (...) {
        DEBUG_WRITE_FORCE("Worker(id=%d) error: unknown c++ exception!", workerId_);
    }

    // ----- Shutdown (close, terminate or bootstrap failure) -----

    isTerminating_ = true;

    // Terminate any workers this worker created (nested workers). Their
    // Worker object persistents live in this isolate, so they must be
    // released before the isolate is disposed below. Each child cascades to
    // its own children during its shutdown.
    {
        Isolate* isolate = workerIsolate_.load();
        if (isolate != nullptr) {
            TerminateChildren(isolate);
        }
    }

    // On this thread: safe to unregister the inbox fd from the looper.
    queue_.Terminate();

    if (runtime_ != nullptr) {
        try {
            // Java-side detach (GcListener.unsubscribe + runtimeCache.remove)
            // must happen before the isolate is disposed, preserving the old
            // WorkerThreadHandler -> TerminateWorkerCallback ordering.
            JEnv env;
            env.CallStaticVoidMethod(RUNTIME_CLASS, DETACH_WORKER_RUNTIME_METHOD_ID, runtimeId);
        } catch (NativeScriptException& ex) {
            if (jniEnv->ExceptionCheck()) {
                jniEnv->ExceptionClear();
            }
            DEBUG_WRITE_FORCE("Worker(id=%d) error while detaching Java runtime: %s", workerId_,
                              ex.GetErrorMessage().c_str());
        }

        // Take the isolate from the runtime, not from workerIsolate_: if the
        // bootstrap failed between initWorkerRuntime and the workerIsolate_
        // publish (e.g. a JNI error while resolving the looper), the atomic is
        // still null while the isolate very much needs disposing.
        workerIsolate_.store(nullptr);
        Isolate* isolate = runtime_->GetIsolate();
        {
            v8::Locker locker(isolate);
            Isolate::Scope isolate_scope(isolate);
            HandleScope handle_scope(isolate);
            runtime_->DestroyRuntime();
        }
        isolate->Dispose();

        // The Runtime destructor still makes JNI calls - it must run before
        // DetachCurrentThread below.
        delete runtime_;
        runtime_ = nullptr;
    }

    {
        std::lock_guard<std::mutex> lock(looperMutex_);
        if (javaLooperRef_ != nullptr) {
            jniEnv->DeleteGlobalRef(javaLooperRef_);
            javaLooperRef_ = nullptr;
        }
    }

    isDisposed_ = true;

    // Notify the parent thread so the Worker object's persistent handle and
    // the registry entry are released (no-op if terminate() or the parent's
    // own shutdown already cleared them).
    if (auto parentTasks = parentTasks_.lock()) {
        int workerId = workerId_;
        parentTasks->Post([workerId]() {
            WorkerWrapper::ClearWorkerOnParent(workerId);
        });
    }

    // ART aborts if a native thread exits while still attached. This must be
    // the very last JNI-touching action on this thread.
    jvm->DetachCurrentThread();
}

int WorkerWrapper::NextWorkerId() {
    return nextWorkerId_.fetch_add(1, std::memory_order_relaxed) + 1;
}

std::shared_ptr<WorkerWrapper> WorkerWrapper::GetById(int workerId) {
    std::lock_guard<std::mutex> lock(registryMutex_);
    auto it = registry_.find(workerId);
    return it != registry_.end() ? it->second : nullptr;
}

void WorkerWrapper::Insert(int workerId, std::shared_ptr<WorkerWrapper> wrapper) {
    std::lock_guard<std::mutex> lock(registryMutex_);
    registry_.emplace(workerId, std::move(wrapper));
}

void WorkerWrapper::ClearWorkerOnParent(int workerId) {
    std::shared_ptr<WorkerWrapper> wrapper;
    {
        std::lock_guard<std::mutex> lock(registryMutex_);
        auto it = registry_.find(workerId);
        if (it == registry_.end()) {
            return;
        }
        wrapper = it->second;
        registry_.erase(it);
    }

    if (wrapper->poWorker_ != nullptr) {
        v8::Locker locker(wrapper->parentIsolate_);
        wrapper->poWorker_->Reset();
        delete wrapper->poWorker_;
        wrapper->poWorker_ = nullptr;
    }
}

void WorkerWrapper::TerminateChildren(Isolate* parentIsolate) {
    std::vector<std::shared_ptr<WorkerWrapper>> children;
    {
        std::lock_guard<std::mutex> lock(registryMutex_);
        for (auto& entry : registry_) {
            if (entry.second->parentIsolate_ == parentIsolate) {
                children.push_back(entry.second);
            }
        }
    }

    for (auto& child : children) {
        DEBUG_WRITE(
                "Terminating nested worker(id=%d) because its parent is shutting down",
                child->workerId_);
        child->Terminate();
        ClearWorkerOnParent(child->workerId_);
    }
}

WorkerWrapper* WorkerWrapper::FromIsolate(Isolate* isolate) {
    return static_cast<WorkerWrapper*>(
            isolate->GetData((uint32_t) Runtime::IsolateData::WORKER_WRAPPER));
}

void WorkerWrapper::EnsureJniCached() {
    if (RUNTIME_CLASS != nullptr) {
        return;
    }

    JEnv env;

    RUNTIME_CLASS = env.FindClass("com/tns/Runtime");
    assert(RUNTIME_CLASS != nullptr);
    INIT_WORKER_RUNTIME_METHOD_ID =
            env.GetStaticMethodID(RUNTIME_CLASS, "initWorkerRuntime", "(ILjava/lang/String;)I");
    RUN_WORKER_LOOP_METHOD_ID = env.GetStaticMethodID(RUNTIME_CLASS, "runWorkerLoop", "()V");
    DETACH_WORKER_RUNTIME_METHOD_ID =
            env.GetStaticMethodID(RUNTIME_CLASS, "detachWorkerRuntime", "(I)V");

    LOOPER_CLASS = env.FindClass("android/os/Looper");
    assert(LOOPER_CLASS != nullptr);
    MY_LOOPER_METHOD_ID =
            env.GetStaticMethodID(LOOPER_CLASS, "myLooper", "()Landroid/os/Looper;");
    LOOPER_QUIT_METHOD_ID = env.GetMethodID(LOOPER_CLASS, "quit", "()V");

    PROCESS_CLASS = env.FindClass("android/os/Process");
    assert(PROCESS_CLASS != nullptr);
    SET_THREAD_PRIORITY_METHOD_ID =
            env.GetStaticMethodID(PROCESS_CLASS, "setThreadPriority", "(I)V");
}

std::mutex WorkerWrapper::registryMutex_;
std::map<int, std::shared_ptr<WorkerWrapper>> WorkerWrapper::registry_;
std::atomic_int WorkerWrapper::nextWorkerId_(0);

jclass WorkerWrapper::RUNTIME_CLASS = nullptr;
jclass WorkerWrapper::LOOPER_CLASS = nullptr;
jclass WorkerWrapper::PROCESS_CLASS = nullptr;
jmethodID WorkerWrapper::INIT_WORKER_RUNTIME_METHOD_ID = nullptr;
jmethodID WorkerWrapper::RUN_WORKER_LOOP_METHOD_ID = nullptr;
jmethodID WorkerWrapper::DETACH_WORKER_RUNTIME_METHOD_ID = nullptr;
jmethodID WorkerWrapper::MY_LOOPER_METHOD_ID = nullptr;
jmethodID WorkerWrapper::LOOPER_QUIT_METHOD_ID = nullptr;
jmethodID WorkerWrapper::SET_THREAD_PRIORITY_METHOD_ID = nullptr;

}  // namespace tns
