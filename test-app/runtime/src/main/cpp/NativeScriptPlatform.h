#ifndef TEST_APP_NATIVESCRIPTPLATFORM_H
#define TEST_APP_NATIVESCRIPTPLATFORM_H

#include <jni.h>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include "v8.h"
#include "v8-platform.h"
#include "robin_hood.h"

namespace tns {

/**
 * Foreground v8::TaskRunner for one isolate, delivering tasks on the
 * isolate's home thread via a dedicated Java Handler (com.tns.EventLoopHandler)
 * bound to that thread's Looper.
 *
 * Scheduling model (same as Timers): every posted task enqueues one anonymous
 * "task due" token on the Java MessageQueue, so platform tasks are strictly
 * FIFO-ordered with Handler.post runnables and JS timers on the same looper.
 * The token doesn't name a task: each token runs the front of the native
 * queue (or the earliest due delayed task); a token whose work was already
 * drained (see RunNestableTasks) is a no-op.
 *
 * V8 may request this runner (and post to it) before the isolate's home
 * thread is known - e.g. during Isolate::New - so the runner starts unbound
 * and buffers tasks; BindToCurrentThread attaches the Java handler and flushes
 * one token per buffered task. Posts are accepted from any thread.
 */
class ForegroundTaskRunner : public v8::TaskRunner {
public:
    explicit ForegroundTaskRunner(v8::Isolate* isolate) : isolate_(isolate) {}

    ~ForegroundTaskRunner() override;

    /**
     * Creates the Java handler bound to the calling thread's Looper and posts
     * tokens for tasks buffered before the bind. Must be called on the
     * isolate's home thread, before that thread's looper starts dispatching.
     */
    void BindToCurrentThread();

    /**
     * Releases the Java handler (removing all pending tokens) and drops all
     * queued tasks; posts after this are silently dropped. Must be called on
     * the home thread, before the isolate is disposed.
     */
    void Shutdown();

    /**
     * Runs at most one due task, then performs a microtask checkpoint.
     * Invoked by Java EventLoopHandler.handleMessage once per token, on the
     * home thread.
     */
    void RunTask();

    /**
     * Runs all currently due nestable tasks without a microtask checkpoint.
     * For nested message loops (inspector pause) where the Java looper isn't
     * spinning: JS is on the stack, so non-nestable tasks stay queued and run
     * from their tokens after the loop unwinds.
     */
    void RunNestableTasks();

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
    void PostTaskImpl(std::unique_ptr<v8::Task> task,
                      const v8::SourceLocation& location) override;

    void PostNonNestableTaskImpl(std::unique_ptr<v8::Task> task,
                                 const v8::SourceLocation& location) override;

    void PostDelayedTaskImpl(std::unique_ptr<v8::Task> task,
                             double delay_in_seconds,
                             const v8::SourceLocation& location) override;

    void PostNonNestableDelayedTaskImpl(std::unique_ptr<v8::Task> task,
                                        double delay_in_seconds,
                                        const v8::SourceLocation& location) override;

private:
    struct Entry {
        std::unique_ptr<v8::Task> task;
        bool nestable;
        // enqueue time for immediate tasks, due time for delayed ones (both
        // CLOCK_MONOTONIC ms) so TakeDueTaskLocked has one comparable order
        double time;
    };

    void PostImmediate(std::unique_ptr<v8::Task> task, bool nestable);
    void PostDelayed(std::unique_ptr<v8::Task> task, bool nestable,
                     double delay_in_seconds);
    // returns the earliest due task, or nullptr; caller must hold mutex_
    std::unique_ptr<v8::Task> TakeDueTaskLocked(bool nestableOnly, double now);
    static void PostToken(jobject handler, jlong uptimeMillis);

    v8::Isolate* isolate_;
    std::mutex mutex_;
    std::deque<Entry> immediate_;
    // delayed tasks keyed by absolute due time (CLOCK_MONOTONIC ms, the same
    // clock as uptimeMillis); each posted its token at ceil(dueTime)
    std::multimap<double, Entry> delayed_;
    // global ref to the com.tns.EventLoopHandler for the isolate's home thread
    jobject handler_ = nullptr;
    bool stopped_ = false;

    // process-wide JNI cache, written once under the first bind's lock
    static jclass EVENT_LOOP_HANDLER_CLASS;
    static jmethodID EVENT_LOOP_HANDLER_CTOR;
    static jmethodID EVENT_LOOP_HANDLER_POST;
    static jmethodID EVENT_LOOP_HANDLER_RELEASE;
};

/**
 * v8::Platform that delegates worker-thread scheduling, time and tracing to
 * the default libplatform implementation but serves per-isolate foreground
 * task runners riding each runtime thread's Java Looper. This is what makes
 * v8's own foreground tasks (WASM async compilation, Atomics.waitAsync
 * wakeups, GC finalization) actually run - nothing pumps the default
 * platform's internal queues.
 */
class NativeScriptPlatform : public v8::Platform {
public:
    explicit NativeScriptPlatform(std::unique_ptr<v8::Platform> defaultPlatform);

    static NativeScriptPlatform* Instance() {
        return instance_;
    }

    /**
     * Returns the isolate's runner, creating an unbound one if v8 asks before
     * Runtime::PrepareV8Runtime binds it to the isolate's home thread.
     */
    std::shared_ptr<ForegroundTaskRunner> GetForegroundRunner(v8::Isolate* isolate);

    /**
     * Shuts down the isolate's runner (home thread only). The map entry
     * survives until IsolateDisposed so late GetForegroundTaskRunner calls
     * during teardown see the stopped runner instead of a fresh one.
     */
    void RuntimeDestroyed(v8::Isolate* isolate);

    /**
     * Drops the runner map entry. Call after v8::Isolate::Dispose, when the
     * isolate pointer may be reused for a future isolate.
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
    std::mutex runnersMutex_;
    robin_hood::unordered_map<v8::Isolate*, std::shared_ptr<ForegroundTaskRunner>> runners_;

    static NativeScriptPlatform* instance_;
};

}  // namespace tns

#endif  // TEST_APP_NATIVESCRIPTPLATFORM_H
