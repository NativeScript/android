#ifndef TEST_APP_EVENTLOOP_H
#define TEST_APP_EVENTLOOP_H

#include <jni.h>
#include <android/looper.h>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include "v8.h"
#include "v8-platform.h"

namespace tns {

/**
 * Per-runtime scheduler for work that must run on the runtime's home thread -
 * the Android analogue of the iOS runtime's ExecuteOnRunLoop. Two lanes, split
 * by ordering contract:
 *
 * Ordered lane - work whose ordering is observable against app-level Java
 * messages (future spec'd macrotasks such as performance-observer callbacks).
 * Rides the Java MessageQueue via a dedicated com.tns.EventLoopHandler using
 * anonymous "task due" tokens (the Timers scheme), so it is strictly FIFO with
 * Handler.post runnables and JS timers on the same looper.
 *
 * Internal lane - work in its own ordering domain: v8 platform foreground
 * tasks (WASM finalization, Atomics.waitAsync wakeups, GC tasks), worker
 * channel messages and exception drains. Rides an eventfd (+ one timerfd for
 * delayed work) on the thread's ALooper: no JNI on the post path, so v8's
 * non-JVM worker threads can post without attaching to the JVM. The eventfd is
 * EFD_SEMAPHORE and carries one unit per runnable entry, so each ALooper
 * callback runs exactly one unit of work and the loop stays fair with Java
 * messages. fd callbacks are serviced between Java messages with no mutual
 * ordering - acceptable here precisely because this lane has no ordering
 * contract with them.
 *
 * Posts are accepted from any thread. The loop starts unbound and buffers
 * (v8 requests its task runner during Isolate::New, before the home thread is
 * committed); BindToCurrentThread attaches both lanes and flushes. Posts after
 * Shutdown are silently dropped, preserving the old LooperTasks "message to a
 * terminated runtime" semantics; leftover wakeups (tokens or eventfd units
 * whose work was drained early) are no-ops.
 */
class EventLoop {
public:
    explicit EventLoop(v8::Isolate* isolate) : isolate_(isolate) {}

    ~EventLoop();

    /**
     * Attaches both lanes to the calling thread (Java handler on the thread's
     * Looper, eventfd/timerfd on its ALooper) and flushes work buffered before
     * the bind. Must run on the runtime's home thread, before its looper
     * starts dispatching.
     */
    void BindToCurrentThread();

    /**
     * Drops all queued work and detaches both lanes; posts after this are
     * silently dropped. Must run on the home thread (removing ALooper fds
     * concurrently with a callback dispatch is racy), before the isolate is
     * disposed.
     */
    void Shutdown();

    // ordered lane: strictly FIFO with Java messages on the home looper
    void PostOrdered(std::function<void()> fn);
    void PostOrderedDelayed(std::function<void()> fn, double delayMs);

    // internal lane: runs on the home thread as soon as the looper polls
    void PostInternal(std::function<void()> fn);
    void PostInternalDelayed(std::function<void()> fn, double delayMs);

    /**
     * Posts a v8 foreground task into the internal lane. Called by the
     * platform's per-isolate v8::TaskRunner adapter, from any thread.
     */
    void PostV8Task(std::unique_ptr<v8::Task> task, bool nestable, double delaySeconds);

    /**
     * True once Shutdown ran. A stopped loop found in the platform registry
     * for a (reused) isolate pointer is stale and must be replaced.
     */
    bool IsStopped();

    /**
     * Runs the internal-lane v8 tasks that are due and nestable, bounded to
     * the entries present at call time. For nested message loops (inspector
     * pause) where the looper isn't polling: JS is on the stack, so
     * non-nestable tasks and plain function posts stay queued and run from
     * their own wakeups after the loop unwinds.
     */
    void RunNestableV8Tasks();

    /**
     * Runs at most one due ordered-lane entry, then performs a microtask
     * checkpoint. Invoked by Java EventLoopHandler.handleMessage once per
     * token, on the home thread.
     */
    void RunOrderedTask();

private:
    struct Entry {
        // exactly one of task/fn is set; fn entries are never drained by
        // RunNestableV8Tasks (plain posts didn't run during debugger pauses
        // under LooperTasks either)
        std::unique_ptr<v8::Task> task;
        std::function<void()> fn;
        bool nestable;
        // enqueue time for immediate entries, due time for delayed ones (both
        // CLOCK_MONOTONIC ms) so one comparison orders both queues
        double time;
        // delayed internal entries only: an eventfd unit has been issued for
        // this entry (written when its timerfd deadline fired), so a later
        // timer fire must not issue a second one
        bool signaled = false;
    };
    struct Lane {
        std::deque<Entry> immediate;
        std::multimap<double, Entry> delayed;
    };

    // all *Locked members require mutex_ to be held.
    // requireSignaledDelayed must be true on the eventfd unit-consuming path:
    // a due delayed entry whose timerfd unit hasn't been issued yet is not
    // this unit's work - consuming it would strand the entry the unit was
    // written for (the timerfd fire then finds nothing due and issues no
    // replacement unit). Ordered-lane and nested (unit-free) drains pass
    // false: ordered entries carry their token from post time, and nested
    // drains consume no units at all.
    void PostInternalLocked(Entry entry, double delayMs);
    void PostOrderedLocked(Entry entry, double delayMs);
    static std::unique_ptr<Entry> TakeDueLocked(Lane& lane, bool nestableOnly, bool v8Only,
                                                bool requireSignaledDelayed, double now);
    void ArmTimerLocked(double now);
    void RunEntry(Entry& entry);
    void RunOneInternal();

    static int EventFdCallback(int fd, int events, void* data);
    static int TimerFdCallback(int fd, int events, void* data);

    v8::Isolate* isolate_;
    std::mutex mutex_;
    Lane internal_;
    Lane ordered_;
    // ordered lane: global ref to this thread's com.tns.EventLoopHandler
    jobject handler_ = nullptr;
    // internal lane: EFD_SEMAPHORE eventfd (one unit = run one due entry) and
    // a timerfd armed to the earliest delayed due time
    ALooper* looper_ = nullptr;
    int eventFd_ = -1;
    int timerFd_ = -1;
    bool stopped_ = false;

    // process-wide JNI cache, written once under the first bind's lock (the
    // main runtime binds before any worker thread exists)
    static jclass EVENT_LOOP_HANDLER_CLASS;
    static jmethodID EVENT_LOOP_HANDLER_CTOR;
    static jmethodID EVENT_LOOP_HANDLER_POST;
    static jmethodID EVENT_LOOP_HANDLER_RELEASE;
};

}  // namespace tns

#endif  // TEST_APP_EVENTLOOP_H
