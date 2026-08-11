#include "EventLoop.h"

#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <ctime>

#include "JEnv.h"
#include "JniLocalRef.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"

using namespace v8;

namespace {

// same clock as android.os.SystemClock.uptimeMillis() and the timerfd below
double now_ms() {
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}

// runs one unit of work without letting a C++ exception escape into an
// ALooper callback frame
template <typename F>
void RunGuarded(F&& body) {
    try {
        body();
    } catch (tns::NativeScriptException& ex) {
        ex.ReThrowToJava();
    } catch (std::exception& ex) {
        DEBUG_WRITE_FORCE("Error: c++ exception in event loop task: %s", ex.what());
    } catch (...) {
        DEBUG_WRITE_FORCE("Error: unknown c++ exception in event loop task!");
    }
}

}  // namespace

namespace tns {

jclass EventLoop::EVENT_LOOP_HANDLER_CLASS = nullptr;
jmethodID EventLoop::EVENT_LOOP_HANDLER_CTOR = nullptr;
jmethodID EventLoop::EVENT_LOOP_HANDLER_POST = nullptr;
jmethodID EventLoop::EVENT_LOOP_HANDLER_RELEASE = nullptr;

/**
 * Adapter v8 holds (via shared_ptr) as the isolate's foreground task runner.
 * Holds the EventLoop weakly: the loop is owned by the platform's registry and
 * the Runtime; once it shuts down and is released, late posts from v8 lock()
 * to nullptr and drop, matching the loop's own post-after-Shutdown behavior.
 */
class V8TaskRunnerAdapter : public v8::TaskRunner {
public:
    explicit V8TaskRunnerAdapter(std::weak_ptr<EventLoop> loop) : loop_(std::move(loop)) {}

    bool IdleTasksEnabled() override {
        return false;
    }

    bool NonNestableTasksEnabled() const override {
        return true;
    }

    bool NonNestableDelayedTasksEnabled() const override {
        return true;
    }

protected:
    void PostTaskImpl(std::unique_ptr<Task> task, const SourceLocation& location) override {
        if (auto loop = loop_.lock()) {
            loop->PostV8Task(std::move(task), true, 0);
        }
    }

    void PostNonNestableTaskImpl(std::unique_ptr<Task> task,
                                 const SourceLocation& location) override {
        if (auto loop = loop_.lock()) {
            loop->PostV8Task(std::move(task), false, 0);
        }
    }

    void PostDelayedTaskImpl(std::unique_ptr<Task> task, double delay_in_seconds,
                             const SourceLocation& location) override {
        if (auto loop = loop_.lock()) {
            loop->PostV8Task(std::move(task), true, delay_in_seconds);
        }
    }

    void PostNonNestableDelayedTaskImpl(std::unique_ptr<Task> task, double delay_in_seconds,
                                        const SourceLocation& location) override {
        if (auto loop = loop_.lock()) {
            loop->PostV8Task(std::move(task), false, delay_in_seconds);
        }
    }

private:
    std::weak_ptr<EventLoop> loop_;
};

void EventLoop::BindToCurrentThread() {
    JEnv env;
    std::lock_guard<std::mutex> lock(mutex_);
    if (looper_ != nullptr || stopped_) {
        return;
    }

    auto looper = ALooper_forThread();
    if (looper == nullptr) {
        DEBUG_WRITE_FORCE("EventLoop: no ALooper on the binding thread");
        return;
    }

    int eventFd = eventfd(0, EFD_SEMAPHORE | EFD_NONBLOCK | EFD_CLOEXEC);
    if (eventFd == -1) {
        DEBUG_WRITE_FORCE("EventLoop: eventfd failed: %s", strerror(errno));
        return;
    }
    int timerFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerFd == -1) {
        DEBUG_WRITE_FORCE("EventLoop: timerfd failed: %s", strerror(errno));
        close(eventFd);
        return;
    }
    if (ALooper_addFd(looper, eventFd, ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT,
                      EventLoop::EventFdCallback, this) != 1 ||
        ALooper_addFd(looper, timerFd, ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT,
                      EventLoop::TimerFdCallback, this) != 1) {
        DEBUG_WRITE_FORCE("EventLoop: ALooper_addFd failed");
        ALooper_removeFd(looper, eventFd);
        close(eventFd);
        close(timerFd);
        return;
    }
    looper_ = looper;
    ALooper_acquire(looper_);
    eventFd_ = eventFd;
    timerFd_ = timerFd;

    if (EVENT_LOOP_HANDLER_CLASS == nullptr) {
        // JEnv::FindClass caches a global ref to the class
        EVENT_LOOP_HANDLER_CLASS = env.FindClass("com/tns/EventLoopHandler");
        assert(EVENT_LOOP_HANDLER_CLASS != nullptr);
        EVENT_LOOP_HANDLER_CTOR = env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "<init>", "(J)V");
        EVENT_LOOP_HANDLER_POST = env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "post", "(J)V");
        EVENT_LOOP_HANDLER_RELEASE = env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "release", "()V");
    }
    JniLocalRef handler(env.NewObject(EVENT_LOOP_HANDLER_CLASS, EVENT_LOOP_HANDLER_CTOR,
                                      reinterpret_cast<jlong>(this)));
    assert(!handler.IsNull());
    handler_ = env.NewGlobalRef(handler);

    // flush work buffered before the home thread was known
    auto now = now_ms();
    for (size_t i = 0; i < internal_.immediate.size(); i++) {
        uint64_t value = 1;
        write(eventFd_, &value, sizeof(value));
    }
    ArmTimerLocked(now);
    for (auto& entry : ordered_.immediate) {
        env.CallVoidMethod(handler_, EVENT_LOOP_HANDLER_POST, (jlong) entry.time);
    }
    for (auto& pair : ordered_.delayed) {
        env.CallVoidMethod(handler_, EVENT_LOOP_HANDLER_POST, (jlong) std::ceil(pair.first));
    }
}

void EventLoop::Shutdown() {
    // must run on the home thread: removing an fd concurrently with an
    // in-flight ALooper callback dispatch is racy
    JEnv env;
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    stopped_ = true;
    internal_.immediate.clear();
    internal_.delayed.clear();
    ordered_.immediate.clear();
    ordered_.delayed.clear();
    if (eventFd_ != -1) {
        ALooper_removeFd(looper_, eventFd_);
        close(eventFd_);
        eventFd_ = -1;
    }
    if (timerFd_ != -1) {
        ALooper_removeFd(looper_, timerFd_);
        close(timerFd_);
        timerFd_ = -1;
    }
    if (looper_ != nullptr) {
        ALooper_release(looper_);
        looper_ = nullptr;
    }
    if (handler_ != nullptr) {
        // the global ref stays alive until the destructor, but the released
        // handler ignores any token already in (or racing into) its queue
        env.CallVoidMethod(handler_, EVENT_LOOP_HANDLER_RELEASE);
    }
}

EventLoop::~EventLoop() {
    // normally a no-op: DestroyRuntime already shut the loop down
    Shutdown();
    std::lock_guard<std::mutex> lock(mutex_);
    if (handler_ != nullptr) {
        JEnv env;
        env.DeleteGlobalRef(handler_);
        handler_ = nullptr;
    }
}

void EventLoop::PostInternalLocked(Entry entry, double delayMs) {
    auto now = now_ms();
    if (delayMs <= 0) {
        entry.time = now;
        internal_.immediate.push_back(std::move(entry));
        if (eventFd_ != -1) {
            uint64_t value = 1;
            write(eventFd_, &value, sizeof(value));
        }
    } else {
        auto due = now + delayMs;
        entry.time = due;
        internal_.delayed.emplace(due, std::move(entry));
        if (timerFd_ != -1) {
            ArmTimerLocked(now);
        }
    }
}

void EventLoop::PostOrderedLocked(Entry entry, double delayMs) {
    auto now = now_ms();
    if (delayMs <= 0) {
        entry.time = now;
        ordered_.immediate.push_back(std::move(entry));
        if (handler_ != nullptr) {
            // Handler.sendMessageAtTime only enqueues, so the JNI call is
            // cheap enough to keep under the lock, which in turn keeps posts
            // from overlapping Shutdown/destruction
            JEnv env;
            env.CallVoidMethod(handler_, EVENT_LOOP_HANDLER_POST, (jlong) now);
        }
    } else {
        auto due = now + delayMs;
        entry.time = due;
        ordered_.delayed.emplace(due, std::move(entry));
        if (handler_ != nullptr) {
            // ceil so the token never arrives before the due time
            JEnv env;
            env.CallVoidMethod(handler_, EVENT_LOOP_HANDLER_POST, (jlong) std::ceil(due));
        }
    }
}

void EventLoop::PostInternal(std::function<void()> fn) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostInternalLocked(Entry{nullptr, std::move(fn), true, 0}, 0);
}

void EventLoop::PostInternalDelayed(std::function<void()> fn, double delayMs) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostInternalLocked(Entry{nullptr, std::move(fn), true, 0}, delayMs);
}

void EventLoop::PostOrdered(std::function<void()> fn) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostOrderedLocked(Entry{nullptr, std::move(fn), true, 0}, 0);
}

void EventLoop::PostOrderedDelayed(std::function<void()> fn, double delayMs) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostOrderedLocked(Entry{nullptr, std::move(fn), true, 0}, delayMs);
}

void EventLoop::PostV8Task(std::unique_ptr<Task> task, bool nestable, double delaySeconds) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostInternalLocked(Entry{std::move(task), nullptr, nestable, 0},
                       delaySeconds * 1000.0);
}

std::shared_ptr<v8::TaskRunner> EventLoop::V8TaskRunner() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (v8Runner_ == nullptr) {
        v8Runner_ = std::make_shared<V8TaskRunnerAdapter>(weak_from_this());
    }
    return v8Runner_;
}

std::unique_ptr<EventLoop::Entry> EventLoop::TakeDueLocked(Lane& lane, bool nestableOnly,
                                                           bool v8Only, double now) {
    auto matches = [&](const Entry& e) {
        return (!nestableOnly || e.nestable) && (!v8Only || e.task != nullptr);
    };
    auto imIt = lane.immediate.begin();
    while (imIt != lane.immediate.end() && !matches(*imIt)) {
        ++imIt;
    }
    auto delIt = lane.delayed.begin();
    while (delIt != lane.delayed.end() && !matches(delIt->second)) {
        ++delIt;
    }
    bool hasImmediate = imIt != lane.immediate.end();
    bool hasDelayed = delIt != lane.delayed.end() && delIt->first <= now;
    if (hasImmediate && (!hasDelayed || imIt->time <= delIt->first)) {
        auto entry = std::make_unique<Entry>(std::move(*imIt));
        lane.immediate.erase(imIt);
        return entry;
    }
    if (hasDelayed) {
        auto entry = std::make_unique<Entry>(std::move(delIt->second));
        lane.delayed.erase(delIt);
        return entry;
    }
    return nullptr;
}

void EventLoop::ArmTimerLocked(double now) {
    if (timerFd_ == -1) {
        return;
    }
    // earliest delayed entry that hasn't had its eventfd unit issued yet;
    // signaled entries are just waiting to be consumed
    double due = -1;
    for (auto& pair : internal_.delayed) {
        if (!pair.second.signaled) {
            due = pair.first;
            break;
        }
    }
    struct itimerspec spec = {};
    if (due >= 0) {
        // a due time already in the past fires immediately, except an exact 0
        // would disarm - clamp to 1ns
        due = std::max(due, now - 1);
        spec.it_value.tv_sec = (time_t) (due / 1000.0);
        spec.it_value.tv_nsec = std::max(1L, (long) (std::fmod(due, 1000.0) * 1e6));
    }
    timerfd_settime(timerFd_, TFD_TIMER_ABSTIME, &spec, nullptr);
}

void EventLoop::RunEntry(Entry& entry) {
    v8::Locker locker(isolate_);
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handleScope(isolate_);
    if (entry.task != nullptr) {
        entry.task->Run();
    } else {
        entry.fn();
    }
    // work may enqueue microtasks without entering JS (e.g. resolving the
    // Atomics.waitAsync promise), which never reaches kAuto's depth-0 drain
    isolate_->PerformMicrotaskCheckpoint();
}

void EventLoop::RunOneInternal() {
    std::unique_ptr<Entry> entry;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) {
            return;
        }
        entry = TakeDueLocked(internal_, false, false, now_ms());
    }
    if (entry == nullptr) {
        // leftover unit: the work it represented ran early from a nested loop
        // drain
        return;
    }
    RunEntry(*entry);
}

void EventLoop::RunNestableV8Tasks() {
    // bounded to the entries present at call time so a task that reposts
    // can't wedge the inspector pause loop that called us
    size_t budget;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        budget = internal_.immediate.size() + internal_.delayed.size();
    }
    while (budget-- > 0) {
        std::unique_ptr<Entry> entry;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stopped_) {
                return;
            }
            entry = TakeDueLocked(internal_, true, true, now_ms());
        }
        if (entry == nullptr) {
            return;
        }
        RunEntry(*entry);
    }
}

void EventLoop::RunOrderedTask() {
    std::unique_ptr<Entry> entry;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) {
            return;
        }
        entry = TakeDueLocked(ordered_, false, false, now_ms());
    }
    if (entry == nullptr) {
        // leftover token: the earliest delayed entry isn't due yet
        return;
    }
    RunEntry(*entry);
}

int EventLoop::EventFdCallback(int fd, int events, void* data) {
    uint64_t value;
    // EFD_SEMAPHORE: consumes exactly one unit; while more remain the fd stays
    // readable and ALooper calls back next poll, interleaving with Java
    // messages instead of draining in one go
    read(fd, &value, sizeof(value));
    RunGuarded([&] { static_cast<EventLoop*>(data)->RunOneInternal(); });
    return 1;
}

int EventLoop::TimerFdCallback(int fd, int events, void* data) {
    uint64_t expirations;
    read(fd, &expirations, sizeof(expirations));
    auto self = static_cast<EventLoop*>(data);
    {
        std::lock_guard<std::mutex> lock(self->mutex_);
        if (self->stopped_) {
            return 0;
        }
        auto now = now_ms();
        uint64_t due = 0;
        for (auto& pair : self->internal_.delayed) {
            if (pair.first > now) {
                break;
            }
            if (!pair.second.signaled) {
                pair.second.signaled = true;
                due++;
            }
        }
        if (due > 0 && self->eventFd_ != -1) {
            // plain (non-semaphore) write of N adds N one-unit reads
            write(self->eventFd_, &due, sizeof(due));
        }
        self->ArmTimerLocked(now);
    }
    return 1;
}

}  // namespace tns

extern "C" JNIEXPORT void JNICALL Java_com_tns_EventLoopHandler_nativeRunTask(
        JNIEnv* env, jclass clazz, jlong nativeLoopPtr) {
    try {
        reinterpret_cast<tns::EventLoop*>(nativeLoopPtr)->RunOrderedTask();
    } catch (tns::NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception& e) {
        std::string msg = std::string("Error: c++ exception: ") + e.what();
        tns::NativeScriptException nsEx(msg);
        nsEx.ReThrowToJava();
    } catch (...) {
        tns::NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}
