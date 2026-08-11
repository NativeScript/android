#include "NativeScriptPlatform.h"

using namespace v8;

namespace tns {

NativeScriptPlatform* NativeScriptPlatform::instance_ = nullptr;

NativeScriptPlatform::NativeScriptPlatform(std::unique_ptr<Platform> defaultPlatform)
        : default_(std::move(defaultPlatform)) {
    instance_ = this;
}

std::shared_ptr<EventLoop> NativeScriptPlatform::GetEventLoop(Isolate* isolate) {
    std::lock_guard<std::mutex> lock(loopsMutex_);
    auto it = loops_.find(isolate);
    if (it != loops_.end()) {
        return it->second;
    }
    auto loop = std::make_shared<EventLoop>(isolate);
    loops_.emplace(isolate, loop);
    return loop;
}

void NativeScriptPlatform::IsolateDisposed(Isolate* isolate) {
    std::lock_guard<std::mutex> lock(loopsMutex_);
    loops_.erase(isolate);
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
    // one runner regardless of priority: the home looper's FIFO order is the
    // priority model of the runtime thread
    return GetEventLoop(isolate)->V8TaskRunner();
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
