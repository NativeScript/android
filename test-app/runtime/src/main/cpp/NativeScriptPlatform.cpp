#include "NativeScriptPlatform.h"

using namespace v8;

namespace tns {

NativeScriptPlatform* NativeScriptPlatform::instance_ = nullptr;

namespace {

/**
 * The v8::TaskRunner handed to v8 for one isolate. Stateless beyond the
 * isolate pointer: every post resolves the current EventLoop through the
 * platform registry, so a stale loop replaced by RefreshEventLoop is
 * redirected transparently, and posts for a disposed isolate (registry entry
 * gone) drop instead of reviving a dead pointer's entry.
 */
class V8TaskRunnerAdapter : public v8::TaskRunner {
public:
    explicit V8TaskRunnerAdapter(Isolate* isolate) : isolate_(isolate) {}

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
        Post(std::move(task), true, 0);
    }

    void PostNonNestableTaskImpl(std::unique_ptr<Task> task,
                                 const SourceLocation& location) override {
        Post(std::move(task), false, 0);
    }

    void PostDelayedTaskImpl(std::unique_ptr<Task> task, double delay_in_seconds,
                             const SourceLocation& location) override {
        Post(std::move(task), true, delay_in_seconds);
    }

    void PostNonNestableDelayedTaskImpl(std::unique_ptr<Task> task, double delay_in_seconds,
                                        const SourceLocation& location) override {
        Post(std::move(task), false, delay_in_seconds);
    }

private:
    void Post(std::unique_ptr<Task> task, bool nestable, double delaySeconds) {
        auto loop = NativeScriptPlatform::Instance()->LookupEventLoop(isolate_);
        if (loop != nullptr) {
            loop->PostV8Task(std::move(task), nestable, delaySeconds);
        }
    }

    Isolate* isolate_;
};

}  // namespace

NativeScriptPlatform::NativeScriptPlatform(std::unique_ptr<Platform> defaultPlatform)
        : default_(std::move(defaultPlatform)) {
    instance_ = this;
}

NativeScriptPlatform::IsolateEntry& NativeScriptPlatform::GetEntryLocked(Isolate* isolate) {
    auto it = loops_.find(isolate);
    if (it != loops_.end()) {
        return it->second;
    }
    auto emplaced = loops_.emplace(
            isolate, IsolateEntry{std::make_shared<EventLoop>(isolate),
                                  std::make_shared<V8TaskRunnerAdapter>(isolate)});
    return emplaced.first->second;
}

std::shared_ptr<EventLoop> NativeScriptPlatform::GetEventLoop(Isolate* isolate) {
    std::lock_guard<std::mutex> lock(loopsMutex_);
    return GetEntryLocked(isolate).loop;
}

std::shared_ptr<EventLoop> NativeScriptPlatform::RefreshEventLoop(Isolate* isolate) {
    std::lock_guard<std::mutex> lock(loopsMutex_);
    auto& entry = GetEntryLocked(isolate);
    if (entry.loop->IsStopped()) {
        entry.loop = std::make_shared<EventLoop>(isolate);
    }
    return entry.loop;
}

std::shared_ptr<EventLoop> NativeScriptPlatform::LookupEventLoop(Isolate* isolate) {
    std::lock_guard<std::mutex> lock(loopsMutex_);
    auto it = loops_.find(isolate);
    return it != loops_.end() ? it->second.loop : nullptr;
}

void NativeScriptPlatform::IsolateDisposed(Isolate* isolate,
                                           const std::shared_ptr<EventLoop>& loop) {
    std::lock_guard<std::mutex> lock(loopsMutex_);
    auto it = loops_.find(isolate);
    if (it != loops_.end() && it->second.loop == loop) {
        loops_.erase(it);
    }
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
    std::lock_guard<std::mutex> lock(loopsMutex_);
    return GetEntryLocked(isolate).runner;
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
