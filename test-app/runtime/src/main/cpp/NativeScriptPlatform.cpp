#include "NativeScriptPlatform.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctime>
#include <sstream>
#include <vector>

#include "JEnv.h"
#include "JniLocalRef.h"
#include "NativeScriptException.h"

using namespace v8;

namespace {

// same clock as android.os.SystemClock.uptimeMillis()
double now_ms() {
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}

}  // namespace

namespace tns {

jclass ForegroundTaskRunner::EVENT_LOOP_HANDLER_CLASS = nullptr;
jmethodID ForegroundTaskRunner::EVENT_LOOP_HANDLER_CTOR = nullptr;
jmethodID ForegroundTaskRunner::EVENT_LOOP_HANDLER_POST = nullptr;
jmethodID ForegroundTaskRunner::EVENT_LOOP_HANDLER_RELEASE = nullptr;

void ForegroundTaskRunner::BindToCurrentThread() {
    JEnv env;
    std::vector<jlong> tokens;
    jobject handler = nullptr;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (handler_ != nullptr || stopped_) {
            return;
        }
        if (EVENT_LOOP_HANDLER_CLASS == nullptr) {
            // JEnv::FindClass caches a global ref to the class. The first bind
            // happens on the main runtime's thread before any worker exists,
            // so the one-time write is not racy.
            EVENT_LOOP_HANDLER_CLASS = env.FindClass("com/tns/EventLoopHandler");
            assert(EVENT_LOOP_HANDLER_CLASS != nullptr);
            EVENT_LOOP_HANDLER_CTOR = env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "<init>", "(J)V");
            EVENT_LOOP_HANDLER_POST = env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "post", "(J)V");
            EVENT_LOOP_HANDLER_RELEASE = env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "release", "()V");
        }
        JniLocalRef localHandler(env.NewObject(EVENT_LOOP_HANDLER_CLASS, EVENT_LOOP_HANDLER_CTOR,
                                               reinterpret_cast<jlong>(this)));
        handler = env.NewGlobalRef(localHandler);
        handler_ = handler;
        // tasks posted before the home thread was known get their tokens now
        for (auto& entry : immediate_) {
            tokens.push_back((jlong) entry.time);
        }
        for (auto& pair : delayed_) {
            tokens.push_back((jlong) std::ceil(pair.first));
        }
    }
    for (auto when : tokens) {
        env.CallVoidMethod(handler, EVENT_LOOP_HANDLER_POST, when);
    }
}

void ForegroundTaskRunner::Shutdown() {
    jobject handler;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) {
            return;
        }
        stopped_ = true;
        immediate_.clear();
        delayed_.clear();
        handler = handler_;
    }
    if (handler != nullptr) {
        // the global ref stays alive until the runner is destroyed: an
        // off-thread post may have read handler_ just before stopped_ was set
        // and still be calling post() on it - released handlers ignore tokens
        JEnv env;
        env.CallVoidMethod(handler, EVENT_LOOP_HANDLER_RELEASE);
    }
}

ForegroundTaskRunner::~ForegroundTaskRunner() {
    // normally a no-op: RuntimeDestroyed already shut the runner down
    Shutdown();
    if (handler_ != nullptr) {
        JEnv env;
        env.DeleteGlobalRef(handler_);
        handler_ = nullptr;
    }
}

void ForegroundTaskRunner::PostToken(jobject handler, jlong uptimeMillis) {
    JEnv env;
    env.CallVoidMethod(handler, EVENT_LOOP_HANDLER_POST, uptimeMillis);
}

void ForegroundTaskRunner::PostImmediate(std::unique_ptr<Task> task, bool nestable) {
    auto now = now_ms();
    jobject handler;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) {
            return;
        }
        immediate_.push_back(Entry{std::move(task), nestable, now});
        handler = handler_;
    }
    if (handler != nullptr) {
        PostToken(handler, (jlong) now);
    }
}

void ForegroundTaskRunner::PostDelayed(std::unique_ptr<Task> task, bool nestable,
                                       double delay_in_seconds) {
    auto now = now_ms();
    auto due = now + std::max(delay_in_seconds, 0.0) * 1000.0;
    jobject handler;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) {
            return;
        }
        delayed_.emplace(due, Entry{std::move(task), nestable, due});
        handler = handler_;
    }
    if (handler != nullptr) {
        // ceil so the token never arrives before the due time
        PostToken(handler, (jlong) std::ceil(due));
    }
}

void ForegroundTaskRunner::PostTaskImpl(std::unique_ptr<Task> task,
                                        const SourceLocation& location) {
    PostImmediate(std::move(task), true);
}

void ForegroundTaskRunner::PostNonNestableTaskImpl(std::unique_ptr<Task> task,
                                                   const SourceLocation& location) {
    PostImmediate(std::move(task), false);
}

void ForegroundTaskRunner::PostDelayedTaskImpl(std::unique_ptr<Task> task,
                                               double delay_in_seconds,
                                               const SourceLocation& location) {
    PostDelayed(std::move(task), true, delay_in_seconds);
}

void ForegroundTaskRunner::PostNonNestableDelayedTaskImpl(std::unique_ptr<Task> task,
                                                          double delay_in_seconds,
                                                          const SourceLocation& location) {
    PostDelayed(std::move(task), false, delay_in_seconds);
}

std::unique_ptr<Task> ForegroundTaskRunner::TakeDueTaskLocked(bool nestableOnly, double now) {
    auto imIt = immediate_.begin();
    if (nestableOnly) {
        while (imIt != immediate_.end() && !imIt->nestable) {
            ++imIt;
        }
    }
    auto delIt = delayed_.begin();
    if (nestableOnly) {
        while (delIt != delayed_.end() && !delIt->second.nestable) {
            ++delIt;
        }
    }
    bool hasImmediate = imIt != immediate_.end();
    bool hasDelayed = delIt != delayed_.end() && delIt->first <= now;
    if (hasImmediate && (!hasDelayed || imIt->time <= delIt->first)) {
        auto task = std::move(imIt->task);
        immediate_.erase(imIt);
        return task;
    }
    if (hasDelayed) {
        auto task = std::move(delIt->second.task);
        delayed_.erase(delIt);
        return task;
    }
    return nullptr;
}

void ForegroundTaskRunner::RunTask() {
    std::unique_ptr<Task> task;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) {
            return;
        }
        task = TakeDueTaskLocked(false, now_ms());
    }
    if (task == nullptr) {
        // leftover token: the task ran early from a nested loop drain, or the
        // earliest delayed task isn't due yet
        return;
    }
    auto isolate = isolate_;
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    task->Run();
    // a task may enqueue microtasks without entering JS (e.g. resolving the
    // Atomics.waitAsync promise), which never reaches kAuto's depth-0 drain
    isolate->PerformMicrotaskCheckpoint();
}

void ForegroundTaskRunner::RunNestableTasks() {
    while (true) {
        std::unique_ptr<Task> task;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stopped_) {
                return;
            }
            task = TakeDueTaskLocked(true, now_ms());
        }
        if (task == nullptr) {
            return;
        }
        v8::Locker locker(isolate_);
        v8::Isolate::Scope isolate_scope(isolate_);
        v8::HandleScope handleScope(isolate_);
        task->Run();
    }
}

NativeScriptPlatform* NativeScriptPlatform::instance_ = nullptr;

NativeScriptPlatform::NativeScriptPlatform(std::unique_ptr<Platform> defaultPlatform)
        : default_(std::move(defaultPlatform)) {
    instance_ = this;
}

std::shared_ptr<ForegroundTaskRunner> NativeScriptPlatform::GetForegroundRunner(Isolate* isolate) {
    std::lock_guard<std::mutex> lock(runnersMutex_);
    auto it = runners_.find(isolate);
    if (it != runners_.end()) {
        return it->second;
    }
    auto runner = std::make_shared<ForegroundTaskRunner>(isolate);
    runners_.emplace(isolate, runner);
    return runner;
}

void NativeScriptPlatform::RuntimeDestroyed(Isolate* isolate) {
    std::shared_ptr<ForegroundTaskRunner> runner;
    {
        std::lock_guard<std::mutex> lock(runnersMutex_);
        auto it = runners_.find(isolate);
        if (it == runners_.end()) {
            return;
        }
        runner = it->second;
    }
    runner->Shutdown();
}

void NativeScriptPlatform::IsolateDisposed(Isolate* isolate) {
    std::lock_guard<std::mutex> lock(runnersMutex_);
    runners_.erase(isolate);
}

PageAllocator* NativeScriptPlatform::GetPageAllocator() {
    return default_->GetPageAllocator();
}

ThreadIsolatedAllocator* NativeScriptPlatform::GetThreadIsolatedAllocator() {
    return default_->GetThreadIsolatedAllocator();
}

size_t NativeScriptPlatform::GetZeroSegmentSize() {
    return default_->GetZeroSegmentSize();
}

void NativeScriptPlatform::OnCriticalMemoryPressure() {
    default_->OnCriticalMemoryPressure();
}

int NativeScriptPlatform::NumberOfWorkerThreads() {
    return default_->NumberOfWorkerThreads();
}

std::shared_ptr<TaskRunner> NativeScriptPlatform::GetForegroundTaskRunner(
        Isolate* isolate, TaskPriority priority) {
    // one runner regardless of priority: the Java MessageQueue's FIFO order is
    // the priority model of the runtime thread
    return GetForegroundRunner(isolate);
}

bool NativeScriptPlatform::IdleTasksEnabled(Isolate* isolate) {
    return false;
}

std::unique_ptr<ScopedBoostablePriority> NativeScriptPlatform::CreateBoostablePriorityScope() {
    return default_->CreateBoostablePriorityScope();
}

std::unique_ptr<ScopedBlockingCall> NativeScriptPlatform::CreateBlockingScope(
        BlockingType blocking_type) {
    return default_->CreateBlockingScope(blocking_type);
}

double NativeScriptPlatform::MonotonicallyIncreasingTime() {
    return default_->MonotonicallyIncreasingTime();
}

int64_t NativeScriptPlatform::CurrentClockTimeMilliseconds() {
    return default_->CurrentClockTimeMilliseconds();
}

double NativeScriptPlatform::CurrentClockTimeMillis() {
    return default_->CurrentClockTimeMillis();
}

double NativeScriptPlatform::CurrentClockTimeMillisecondsHighResolution() {
    return default_->CurrentClockTimeMillisecondsHighResolution();
}

Platform::StackTracePrinter NativeScriptPlatform::GetStackTracePrinter() {
    return default_->GetStackTracePrinter();
}

TracingController* NativeScriptPlatform::GetTracingController() {
    return default_->GetTracingController();
}

void NativeScriptPlatform::DumpWithoutCrashing() {
    default_->DumpWithoutCrashing();
}

HighAllocationThroughputObserver* NativeScriptPlatform::GetHighAllocationThroughputObserver() {
    return default_->GetHighAllocationThroughputObserver();
}

std::unique_ptr<JobHandle> NativeScriptPlatform::CreateJobImpl(
        TaskPriority priority, std::unique_ptr<JobTask> job_task,
        const SourceLocation& location) {
    return default_->CreateJob(priority, std::move(job_task), location);
}

void NativeScriptPlatform::PostTaskOnWorkerThreadImpl(TaskPriority priority,
                                                      std::unique_ptr<Task> task,
                                                      const SourceLocation& location) {
    default_->PostTaskOnWorkerThread(priority, std::move(task), location);
}

void NativeScriptPlatform::PostDelayedTaskOnWorkerThreadImpl(
        TaskPriority priority, std::unique_ptr<Task> task, double delay_in_seconds,
        const SourceLocation& location) {
    default_->PostDelayedTaskOnWorkerThread(priority, std::move(task), delay_in_seconds,
                                            location);
}

}  // namespace tns

extern "C" JNIEXPORT void JNICALL Java_com_tns_EventLoopHandler_nativeRunTask(
        JNIEnv* env, jclass clazz, jlong nativeRunnerPtr) {
    try {
        reinterpret_cast<tns::ForegroundTaskRunner*>(nativeRunnerPtr)->RunTask();
    } catch (tns::NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception& e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        tns::NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        tns::NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}
