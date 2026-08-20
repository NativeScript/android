#ifndef TEST_APP_EVENTLOOP_H
#define TEST_APP_EVENTLOOP_H

#include <jni.h>
#include <android/looper.h>
#include <atomic>
#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include "v8.h"
#include "v8-platform.h"

namespace tns {

class NativeScriptException;

/**
 * A producer of ordered-lane work that keeps its own bookkeeping (Timers).
 * The EventLoop's token drain consults it so timers and ordered entries form
 * ONE due-ordered domain: each anonymous token runs the earliest due item
 * across both. Home-thread only.
 */
class OrderedTaskSource {
public:
    /**
     * If the source's earliest item is due at `now` and is earlier-or-equal
     * to `otherDue` (a negative otherDue means no competitor), consumes that
     * slot - running the item, or nothing if the slot is a tombstone
     * (cancelled item) - and returns true. Consumes exactly one slot per call
     * so tokens and slots stay 1:1. Check and run form a single call so the
     * source can do both under one acquisition of whatever guards its state
     * (Timers' bookkeeping is guarded by the isolate Locker: background
     * threads mutate it through setTimeout under multithreaded JS).
     */
    virtual bool RunIfEarliest(double now, double otherDue) = 0;

    virtual ~OrderedTaskSource() = default;
};

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

    /**
     * Posts a bare ordered token at an absolute uptime for an item the
     * OrderedTaskSource keeps in its own bookkeeping (Timers). One token per
     * item; the drain picks the earliest due item across the source and the
     * ordered entries, so the token needn't name what it will run.
     *
     * The token carries a claim cell when one is free for `timerId` (returns
     * the non-zero cell word to cancel with): EventLoopHandler claims the
     * cell through a @CriticalNative CAS before entering the runtime, so a
     * token whose timer was cancelled dies in Java without acquiring the
     * isolate Locker, and CancelClaimCell neutralizes a queued token with no
     * JNI at all. Returns 0 when the token is plain (cell slot busy, or the
     * loop is unbound/stopped) - the caller must then use tombstones.
     */
    uint64_t PostTimerToken(jlong uptimeMillis, int timerId);

    /**
     * Posts an ordered token carrying a Java AtomicBoolean claim peer, for
     * timers long enough that the stale wakeup itself is worth removing.
     * Returns a global ref to the peer (null when unbound/stopped - caller
     * falls back to tombstones). CancelIdentifiedToken later CASes the peer
     * and, on winning, removeMessages()es the queued token: a cleared long
     * timer produces no wakeup. The peer and its Message are GC-owned, which
     * is what makes the remove-vs-in-flight-dispatch race harmless.
     */
    jobject PostIdentifiedTimerToken(jlong uptimeMillis);

    /**
     * Neutralizes a cell-carrying token (no JNI, single CAS). True = the
     * token is guaranteed dead wherever it is, so the caller may delete the
     * item outright; false = dispatch already claimed it, so the caller must
     * leave a tombstone for it to consume.
     */
    bool CancelClaimCell(uint64_t cellWord);

    /**
     * Neutralizes an identified token (one JNI crossing; releases the peer
     * ref). Same true/false contract as CancelClaimCell.
     */
    bool CancelIdentifiedToken(jobject peer);

    /**
     * Releases an identified token's peer ref without cancelling (the timer
     * fired or is being torn down; the Java Message keeps the peer alive for
     * its own dispatch).
     */
    void ReleaseIdentifiedToken(jobject peer);

    /**
     * Registers the ordered lane's external source. Home thread only; pass
     * nullptr to unregister (the source is being destroyed).
     */
    void SetTimerSource(OrderedTaskSource* source);

    // internal lane: runs on the home thread as soon as the looper polls
    void PostInternal(std::function<void()> fn);
    void PostInternalDelayed(std::function<void()> fn, double delayMs);

    /**
     * Internal-lane post whose fn does its OWN isolate ceremony: RunEntry
     * skips the loop's Locker/scopes/microtask checkpoint. Required when the
     * fn locks a different isolate than this loop's (__runOnMainThread
     * closures lock the caller's isolate) - taking this loop's Locker first
     * would nest Lockers across isolates and can deadlock against
     * multithreaded-JS entry paths.
     */
    void PostInternalBare(std::function<void()> fn);

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
     * True while the calling thread is inside one of this process's ALooper
     * fd callbacks. Android's Looper::pollInner holds a Response& into its
     * response vector across each callback; a nested ALooper_pollOnce on the
     * same looper clears and reallocates that vector, so the outer poll
     * resumes over freed memory. Any code that pumps the looper (module
     * evaluation, the boot backstop, the fetch yield) must consult this and
     * drain queues directly instead of polling when it is set.
     */
    static bool IsInLooperCallback();

    /**
     * Marks this loop's isolate as termination-requested. Callable from any
     * thread. Pumps consult it alongside Isolate::IsExecutionTerminating,
     * which per its contract is true only while JS frames are unwinding with
     * the termination exception active - a pump parked with nothing queued
     * never runs JS, so TerminateExecution alone cannot end it before the
     * deadline.
     */
    void NoteTerminationRequested() {
        terminationRequested_.store(true, std::memory_order_release);
    }

    /**
     * Blocks the calling thread until this loop's internal lane has work (the
     * eventfd or timerfd is readable) or `timeoutMs` elapses, whichever comes
     * first, without entering the looper - so it is safe where
     * IsInLooperCallback forbids polling. Returns immediately when work the
     * caller's drain can take is already due; `pumpDeliverable` selects which
     * filter that is, and must match the drain mode of the pump that idles
     * here - a due entry the drain cannot take must not no-op the wait.
     * Eventfd units left over from entries a direct drain consumed are
     * swallowed first, so the wait only wakes for new work instead of
     * spinning on stale readability.
     */
    void WaitForInternalWork(int timeoutMs, bool pumpDeliverable = false);

    /**
     * Runs every ordered-lane item that is due NOW - Java-token entries and
     * timer-source items alike, in due order - directly from the calling
     * (home) thread, without waiting for their Handler messages. The messages
     * still arrive later and die as leftover tokens: a token whose item was
     * drained early finds nothing due and no-ops, and its claim cell is
     * retired by the dispatch gate as usual. Bounded to a short slice so a
     * callback minting due-now work (a setTimeout(0) chain) cannot pin the
     * caller past its own deadline checks. Returns the number of items run.
     */
    int RunDueOrderedEntries();

    enum class PumpResult { kSettled, kDeadline, kTerminated };

    /**
     * Drives this loop in place on the home thread until `settled()` returns
     * true or `deadlineSeconds` elapses, idling in WaitForInternalWork
     * between slices. The one pump primitive behind module evaluation, the
     * graph walk, and the boot backstop; `settled` may throw and the
     * exception propagates. Returns kTerminated when the isolate is
     * terminating or the loop has been shut down.
     *
     * `drainLooperWork` picks what a pump iteration runs, mirroring the iOS
     * pump's pumpRunLoop split:
     *  - false: nestable v8 tasks and a microtask checkpoint only, exactly
     *    like the inspector pause loops (iOS's default pump body).
     *  - true (the looper-equivalent drain, standing in for iOS's runloop
     *    slice): additionally runs due ordered-lane work (JS timers included)
     *    and plain internal-lane posts (worker->parent messages, Node-API
     *    completions), plus the registered pump drain hook. Non-nestable v8
     *    tasks stay queued in both modes (the v8 nestability contract: JS
     *    frames are on the stack throughout), and so do bare posts - their
     *    fns lock a DIFFERENT isolate, and running one under a caller
     *    holding this isolate's Locker nests Lockers across isolates.
     */
    PumpResult PumpUntil(double deadlineSeconds, const std::function<bool()>& settled,
                         bool drainLooperWork);

    /**
     * True while the calling thread is inside PumpUntil. Callback code that
     * would arm a pending Java exception (env.Throw) must defer it through
     * DeferJavaThrow instead while this holds: the pump keeps making JNI
     * calls after the callback returns.
     */
    static bool IsPumping();

    /**
     * Queues an exception to be raised on the Java side from the next
     * ordered-token dispatch - the point where returning to Java is the next
     * act - and posts the wakeup that guarantees such a dispatch happens.
     * The exception must not hold JNI local refs (capture the message text
     * instead when it might).
     */
    void DeferJavaThrow(std::shared_ptr<NativeScriptException> ex);

    /**
     * Registers extra home-thread work for looper-equivalent pump drains
     * (the worker inbox, which rides its own fd the pump never polls).
     * Returns the number of items it ran. Home thread only; cleared by
     * Shutdown; pass nullptr to unregister early.
     */
    void SetPumpDrainHook(std::function<int()> hook);

    /**
     * Runs at most one due ordered-lane entry, then performs a microtask
     * checkpoint. Invoked by Java EventLoopHandler.handleMessage once per
     * token, on the home thread.
     */
    void RunOrderedTask();

    /**
     * Raises one deferred exception (DeferJavaThrow) as a pending Java
     * exception. Called by nativeRunTask after each token dispatch, where
     * returning to Java is the next act; one exception per dispatch, each
     * defer having posted its own wakeup token.
     */
    void ReportDeferredJavaError();

private:
    // which internal-lane entries a drain may take
    enum class DrainFilter {
        kAny,           // the looper's own dispatch (RunOneInternal)
        kNestableV8,    // inspector pause loops and drain-off pumps
        // looper-equivalent pumps: nestable v8 tasks and plain fn posts;
        // never bare posts (they lock a different isolate) and never
        // non-nestable tasks
        kPumpDeliverable,
    };
    struct Entry {
        // exactly one of task/fn is set; fn entries are never drained by
        // RunNestableV8Tasks (plain posts didn't run during debugger pauses
        // under LooperTasks either)
        std::unique_ptr<v8::Task> task;
        std::function<void()> fn;
        bool nestable;
        // bare entries run without the loop's Locker/scopes/checkpoint (see
        // PostInternalBare)
        bool bare = false;
        // enqueue time for immediate entries, due time for delayed ones (both
        // CLOCK_MONOTONIC ms) so one comparison orders both queues
        double time;
        // delayed internal entries only: an eventfd unit has been issued for
        // this entry (written when its timerfd deadline fired), so a later
        // timer fire must not issue a second one
        bool signaled = false;
        // internal entries only: an eventfd unit backs this entry, so a
        // direct (unit-free) drain that consumes it must count the unit as
        // leftover for WaitForInternalWork to swallow
        bool unitIssued = false;
    };
    struct Lane {
        std::deque<Entry> immediate;
        std::multimap<double, Entry> delayed;
    };

    static bool MatchesFilter(const Entry& e, DrainFilter filter);

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
    static std::unique_ptr<Entry> TakeDueLocked(Lane& lane, DrainFilter filter,
                                                bool requireSignaledDelayed, double now);
    // earliest due entry time in the lane, or a negative value if none is due
    static double PeekDueLocked(Lane& lane, double now);
    // same, but only over entries matching TakeDueLocked's filter
    static double PeekDueFilteredLocked(Lane& lane, DrainFilter filter, double now);
    void ArmTimerLocked(double now);
    void RunEntry(Entry& entry);
    // RunGuarded, but pump-aware: defers the Java-side report while a pump is
    // on the stack instead of arming a pending JNI exception mid-pump
    void GuardEntryRun(const std::function<void()>& body);
    // one bounded pass over the internal lane's due entries under `filter`;
    // the body behind RunNestableV8Tasks and the pumps' internal drains
    void RunDueInternalWork(DrainFilter filter);
    void RunOneInternal();
    // one due slot across the ordered domain (entries + timer source); true
    // when a slot was consumed. The body behind both RunOrderedTask (one call
    // per Java token) and RunDueOrderedEntries (looped by the pumps).
    bool RunOneOrderedDue();

    static int EventFdCallback(int fd, int events, void* data);
    static int TimerFdCallback(int fd, int events, void* data);

    /**
     * Claim cells for cell-carrying timer tokens, indexed by timer id. A cell
     * word is (id << 2) | state so a token can prove the cell is still its
     * own; a busy slot (>kClaimCells timers in flight, or an interval's
     * previous token still pending) just downgrades the new token to plain.
     * Lifecycle: 0 (free) -> id|ACTIVE (posted, under mutex_) ->
     * id|CANCELLED (by CancelClaimCell, any thread) -> 0 (retired by the
     * dispatch gate, which runs exactly once per cell token since cell tokens
     * are never removeMessages()ed). Only the gate stores 0, so a cell is
     * never reused while its token is in flight, and cancellation can never
     * hit a recycled cell.
     */
    static constexpr int kClaimCells = 1024;
    static constexpr uint64_t kCellActive = 1;
    static constexpr uint64_t kCellCancelled = 2;

    /**
     * The @CriticalNative body behind EventLoopHandler.nativeClaimToken: one
     * CAS, no JNIEnv, and the thread stays runnable - it must never block,
     * allocate or throw. Returns false when the token's timer was cancelled
     * (token dies in Java); true otherwise (proceed to nativeRunTask).
     */
    static jboolean ClaimTokenCritical(jlong loopPtr, jlong cellWord);

    /**
     * Standard-ABI twin registered on devices below API 26, where ART ignores
     * @CriticalNative and calls through the normal JNI convention - binding
     * the critical-convention function there would misread its arguments.
     */
    static jboolean ClaimTokenLegacy(JNIEnv* env, jclass clazz, jlong loopPtr, jlong cellWord);

    // false when the claim gate couldn't be registered: PostTimerToken then
    // never emits cell words, so nativeClaimToken is never invoked
    static bool claimGateRegistered_;

    v8::Isolate* isolate_;
    std::mutex mutex_;
    Lane internal_;
    Lane ordered_;
    std::atomic<uint64_t> claimCells_[kClaimCells] = {};
    // Set by NoteTerminationRequested (any thread), read by PumpUntil.
    std::atomic_bool terminationRequested_{false};
    // ordered-lane source with its own bookkeeping (Timers); home-thread only
    OrderedTaskSource* timerSource_ = nullptr;
    // bare ordered tokens posted before the bind; flushed by Bind
    std::vector<jlong> pendingTokens_;
    // ordered lane: global ref to this thread's com.tns.EventLoopHandler
    jobject handler_ = nullptr;
    // internal lane: EFD_SEMAPHORE eventfd (one unit = run one due entry) and
    // a timerfd armed to the earliest delayed due time
    ALooper* looper_ = nullptr;
    int eventFd_ = -1;
    int timerFd_ = -1;
    bool stopped_ = false;
    // units written to eventFd_ whose entries a direct drain already ran;
    // consumed by WaitForInternalWork (or by an EventFdCallback that finds
    // nothing due). Guarded by mutex_.
    uint64_t leftoverUnits_ = 0;
    // exceptions deferred by pump drains, raised one per token dispatch by
    // ReportDeferredJavaError. Guarded by mutex_.
    std::deque<std::shared_ptr<NativeScriptException>> deferredJavaThrows_;
    // extra looper-equivalent pump work (the worker inbox); home-thread only
    std::function<int()> pumpDrainHook_;

    // process-wide JNI cache, written once under the first bind's lock (the
    // main runtime binds before any worker thread exists)
    static jclass EVENT_LOOP_HANDLER_CLASS;
    static jmethodID EVENT_LOOP_HANDLER_CTOR;
    static jmethodID EVENT_LOOP_HANDLER_POST;
    static jmethodID EVENT_LOOP_HANDLER_POST_TOKEN;
    static jmethodID EVENT_LOOP_HANDLER_POST_IDENTIFIED;
    static jmethodID EVENT_LOOP_HANDLER_CANCEL_IDENTIFIED;
    static jmethodID EVENT_LOOP_HANDLER_RELEASE;
};

}  // namespace tns

#endif  // TEST_APP_EVENTLOOP_H
