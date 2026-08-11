#ifndef TEST_APP_NATIVESCRIPTPLATFORM_H
#define TEST_APP_NATIVESCRIPTPLATFORM_H

#include <memory>
#include <mutex>
#include "v8.h"
#include "v8-platform.h"
#include "EventLoop.h"
#include "robin_hood.h"

namespace tns {

/**
 * v8::Platform that delegates worker-thread scheduling, time and tracing to
 * the default libplatform implementation but serves per-isolate foreground
 * task runners backed by each runtime's EventLoop. This is what makes v8's
 * own foreground tasks (WASM async compilation, Atomics.waitAsync wakeups,
 * GC finalization) actually run - nothing pumps the default platform's
 * internal queues.
 */
class NativeScriptPlatform : public v8::Platform {
public:
    explicit NativeScriptPlatform(std::unique_ptr<v8::Platform> defaultPlatform);

    static NativeScriptPlatform* Instance() {
        return instance_;
    }

    /**
     * Returns the isolate's event loop, creating an unbound one if v8 asks
     * before Runtime::PrepareV8Runtime binds it to the isolate's home thread.
     */
    std::shared_ptr<EventLoop> GetEventLoop(v8::Isolate* isolate);

    /**
     * Drops the loop registry entry. Call after the isolate can no longer
     * post (EventLoop::Shutdown ran), and before the isolate pointer may be
     * reused for a future isolate.
     */
    void IsolateDisposed(v8::Isolate* isolate);

    // v8::Platform
    v8::PageAllocator* GetPageAllocator() override;
    v8::ThreadIsolatedAllocator* GetThreadIsolatedAllocator() override;
    size_t GetZeroSegmentSize() override;
    void OnCriticalMemoryPressure() override;
    int NumberOfWorkerThreads() override;
    std::shared_ptr<v8::TaskRunner> GetForegroundTaskRunner(
            v8::Isolate* isolate, v8::TaskPriority priority) override;
    bool IdleTasksEnabled(v8::Isolate* isolate) override;
    std::unique_ptr<v8::ScopedBoostablePriority> CreateBoostablePriorityScope() override;
    std::unique_ptr<v8::ScopedBlockingCall> CreateBlockingScope(
            v8::BlockingType blocking_type) override;
    double MonotonicallyIncreasingTime() override;
    int64_t CurrentClockTimeMilliseconds() override;
    double CurrentClockTimeMillis() override;
    double CurrentClockTimeMillisecondsHighResolution() override;
    StackTracePrinter GetStackTracePrinter() override;
    v8::TracingController* GetTracingController() override;
    void DumpWithoutCrashing() override;
    v8::HighAllocationThroughputObserver* GetHighAllocationThroughputObserver() override;

protected:
    std::unique_ptr<v8::JobHandle> CreateJobImpl(
            v8::TaskPriority priority, std::unique_ptr<v8::JobTask> job_task,
            const v8::SourceLocation& location) override;
    void PostTaskOnWorkerThreadImpl(v8::TaskPriority priority,
                                    std::unique_ptr<v8::Task> task,
                                    const v8::SourceLocation& location) override;
    void PostDelayedTaskOnWorkerThreadImpl(v8::TaskPriority priority,
                                           std::unique_ptr<v8::Task> task,
                                           double delay_in_seconds,
                                           const v8::SourceLocation& location) override;

private:
    std::unique_ptr<v8::Platform> default_;
    std::mutex loopsMutex_;
    robin_hood::unordered_map<v8::Isolate*, std::shared_ptr<EventLoop>> loops_;

    static NativeScriptPlatform* instance_;
};

}  // namespace tns

#endif  // TEST_APP_NATIVESCRIPTPLATFORM_H
