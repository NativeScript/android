#include "EventLoop.h"

#include <android/api-level.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <chrono>
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

// Depth, not a flag: a pumped callback can start a nested pump (a drained
// timer calling a pumping require).
thread_local int t_pumpDepth = 0;

struct PumpScope {
    PumpScope() { ++t_pumpDepth; }
    ~PumpScope() { --t_pumpDepth; }
};

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

bool EventLoop::claimGateRegistered_ = false;
jclass EventLoop::EVENT_LOOP_HANDLER_CLASS = nullptr;
jmethodID EventLoop::EVENT_LOOP_HANDLER_CTOR = nullptr;
jmethodID EventLoop::EVENT_LOOP_HANDLER_POST = nullptr;
jmethodID EventLoop::EVENT_LOOP_HANDLER_POST_TOKEN = nullptr;
jmethodID EventLoop::EVENT_LOOP_HANDLER_POST_IDENTIFIED = nullptr;
jmethodID EventLoop::EVENT_LOOP_HANDLER_CANCEL_IDENTIFIED = nullptr;
jmethodID EventLoop::EVENT_LOOP_HANDLER_RELEASE = nullptr;

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
        NS_CHECK(EVENT_LOOP_HANDLER_CLASS != nullptr);
        EVENT_LOOP_HANDLER_CTOR = env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "<init>", "(J)V");
        EVENT_LOOP_HANDLER_POST = env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "post", "(J)V");
        EVENT_LOOP_HANDLER_POST_TOKEN =
                env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "postToken", "(JII)V");
        EVENT_LOOP_HANDLER_POST_IDENTIFIED = env.GetMethodID(
                EVENT_LOOP_HANDLER_CLASS, "postIdentified", "(J)Ljava/lang/Object;");
        EVENT_LOOP_HANDLER_CANCEL_IDENTIFIED = env.GetMethodID(
                EVENT_LOOP_HANDLER_CLASS, "cancelIdentified", "(Ljava/lang/Object;)Z");
        EVENT_LOOP_HANDLER_RELEASE = env.GetMethodID(EVENT_LOOP_HANDLER_CLASS, "release", "()V");
        // The @CriticalNative gate must be bound explicitly (name resolution
        // doesn't apply to the critical calling convention on older ART).
        // Below API 26 ART ignores the annotation and calls through the
        // normal JNI ABI, so bind the standard-convention twin there.
        const bool criticalAbi = android_get_device_api_level() >= 26;
        const JNINativeMethod claimMethod = {
                const_cast<char*>("nativeClaimToken"), const_cast<char*>("(JJ)Z"),
                criticalAbi ? reinterpret_cast<void*>(EventLoop::ClaimTokenCritical)
                            : reinterpret_cast<void*>(EventLoop::ClaimTokenLegacy)};
        JNIEnv* rawEnv = env;
        if (rawEnv->RegisterNatives(EVENT_LOOP_HANDLER_CLASS, &claimMethod, 1) == 0) {
            claimGateRegistered_ = true;
        } else {
            rawEnv->ExceptionClear();
            DEBUG_WRITE_FORCE(
                    "EventLoop: claim gate registration failed; timer tokens stay plain");
        }
    }
    JniLocalRef handler(env.NewObject(EVENT_LOOP_HANDLER_CLASS, EVENT_LOOP_HANDLER_CTOR,
                                      reinterpret_cast<jlong>(this)));
    NS_CHECK(!handler.IsNull());
    handler_ = env.NewGlobalRef(handler);

    // flush work buffered before the home thread was known
    auto now = now_ms();
    for (auto& entry : internal_.immediate) {
        uint64_t value = 1;
        write(eventFd_, &value, sizeof(value));
        entry.unitIssued = true;
    }
    ArmTimerLocked(now);
    for (auto& entry : ordered_.immediate) {
        env.CallVoidMethod(handler_, EVENT_LOOP_HANDLER_POST, (jlong) entry.time);
    }
    for (auto& pair : ordered_.delayed) {
        env.CallVoidMethod(handler_, EVENT_LOOP_HANDLER_POST, (jlong) std::ceil(pair.first));
    }
    for (auto when : pendingTokens_) {
        env.CallVoidMethod(handler_, EVENT_LOOP_HANDLER_POST, when);
    }
    pendingTokens_.clear();
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
    deferredJavaThrows_.clear();
    pumpDrainHook_ = nullptr;
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
    // Normally a no-op: DestroyRuntime already shut the loop down and this
    // runs on the home thread via ~Runtime. In the pathological case where a
    // transient shared_ptr taken on a v8 pool thread is the last reference,
    // the JEnv below permanently attaches that thread to ART (which aborts if
    // it later exits attached) - accepted, since those pool threads live for
    // the process lifetime.
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
        entry.unitIssued = eventFd_ != -1;
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
    PostInternalLocked(Entry{nullptr, std::move(fn), true, false, 0}, 0);
}

void EventLoop::PostInternalDelayed(std::function<void()> fn, double delayMs) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostInternalLocked(Entry{nullptr, std::move(fn), true, false, 0}, delayMs);
}

void EventLoop::PostInternalBare(std::function<void()> fn) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostInternalLocked(Entry{nullptr, std::move(fn), true, true, 0}, 0);
}

void EventLoop::PostOrdered(std::function<void()> fn) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostOrderedLocked(Entry{nullptr, std::move(fn), true, false, 0}, 0);
}

void EventLoop::PostOrderedDelayed(std::function<void()> fn, double delayMs) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostOrderedLocked(Entry{nullptr, std::move(fn), true, false, 0}, delayMs);
}

uint64_t EventLoop::PostTimerToken(jlong uptimeMillis, int timerId) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return 0;
    }
    if (handler_ == nullptr) {
        pendingTokens_.push_back(uptimeMillis);
        return 0;
    }
    uint64_t word = 0;
    auto& cell = claimCells_[((uint32_t) timerId) & (kClaimCells - 1)];
    if (claimGateRegistered_) {
        uint64_t expected = 0;
        uint64_t candidate = (((uint64_t) (uint32_t) timerId) << 2) | kCellActive;
        if (cell.compare_exchange_strong(expected, candidate, std::memory_order_acq_rel)) {
            word = candidate;
        }
        // a busy slot (previous token of the same interval still in flight,
        // or an id collision) downgrades this token to plain; clear then uses
        // tombstones
    }
    JEnv env;
    try {
        env.CallVoidMethod(handler_, EVENT_LOOP_HANDLER_POST_TOKEN, uptimeMillis,
                           (jint) (word >> 32), (jint) (word & 0xffffffffull));
    } catch (...) {
        // no token reached the queue, so no dispatch gate will ever retire
        // the cell - release it here or the slot is burned for the process
        if (word != 0) {
            cell.store(0, std::memory_order_release);
        }
        throw;
    }
    return word;
}

jobject EventLoop::PostIdentifiedTimerToken(jlong uptimeMillis) {
    JEnv env;
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return nullptr;
    }
    if (handler_ == nullptr) {
        pendingTokens_.push_back(uptimeMillis);
        return nullptr;
    }
    JniLocalRef peer(env.CallObjectMethod(handler_, EVENT_LOOP_HANDLER_POST_IDENTIFIED,
                                          uptimeMillis));
    if (peer.IsNull()) {
        return nullptr;
    }
    return env.NewGlobalRef(peer);
}

bool EventLoop::CancelClaimCell(uint64_t cellWord) {
    auto& cell = claimCells_[(cellWord >> 2) & (kClaimCells - 1)];
    uint64_t expected = cellWord;  // id|ACTIVE
    return cell.compare_exchange_strong(expected, (cellWord & ~3ull) | kCellCancelled,
                                        std::memory_order_acq_rel);
}

bool EventLoop::CancelIdentifiedToken(jobject peer) {
    JEnv env;
    bool won = false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!stopped_ && handler_ != nullptr) {
            won = env.CallBooleanMethod(handler_, EVENT_LOOP_HANDLER_CANCEL_IDENTIFIED, peer) ==
                  JNI_TRUE;
        }
    }
    env.DeleteGlobalRef(peer);
    return won;
}

void EventLoop::ReleaseIdentifiedToken(jobject peer) {
    JEnv env;
    env.DeleteGlobalRef(peer);
}

jboolean EventLoop::ClaimTokenLegacy(JNIEnv* env, jclass clazz, jlong loopPtr, jlong cellWord) {
    return ClaimTokenCritical(loopPtr, cellWord);
}

jboolean EventLoop::ClaimTokenCritical(jlong loopPtr, jlong cellWord) {
    // @CriticalNative: no JNIEnv, thread stays runnable - a single CAS, no
    // locks, no allocation, no exceptions. The loop pointer is valid for the
    // same reason nativeRunTask's is: the handler is released before the loop
    // is destroyed, and released handlers never reach this gate.
    auto* loop = reinterpret_cast<EventLoop*>(loopPtr);
    auto word = (uint64_t) cellWord;
    auto& cell = loop->claimCells_[(word >> 2) & (kClaimCells - 1)];
    uint64_t expected = word;  // id|ACTIVE
    if (cell.compare_exchange_strong(expected, 0, std::memory_order_acq_rel)) {
        // claimed and retired in one step: proceed to the fat path
        return JNI_TRUE;
    }
    if (expected == ((word & ~3ull) | kCellCancelled)) {
        // the timer was cleared; retire the cell and drop the token here
        cell.store(0, std::memory_order_release);
        return JNI_FALSE;
    }
    // defensive: a mismatched word can't occur while this token is in flight
    // (only the gate retires cells), but the fat path is always safe
    return JNI_TRUE;
}

void EventLoop::SetTimerSource(OrderedTaskSource* source) {
    // home thread only, like every consumer of timerSource_
    timerSource_ = source;
}

void EventLoop::PostV8Task(std::unique_ptr<Task> task, bool nestable, double delaySeconds) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    PostInternalLocked(Entry{std::move(task), nullptr, nestable, false, 0},
                       delaySeconds * 1000.0);
}

bool EventLoop::IsStopped() {
    std::lock_guard<std::mutex> lock(mutex_);
    return stopped_;
}

bool EventLoop::MatchesFilter(const Entry& e, DrainFilter filter) {
    switch (filter) {
        case DrainFilter::kAny:
            return true;
        case DrainFilter::kNestableV8:
            return e.nestable && e.task != nullptr;
        case DrainFilter::kPumpDeliverable:
            return e.nestable && !e.bare;
    }
    return false;
}

std::unique_ptr<EventLoop::Entry> EventLoop::TakeDueLocked(Lane& lane, DrainFilter filter,
                                                           bool requireSignaledDelayed,
                                                           double now) {
    auto matches = [&](const Entry& e) { return MatchesFilter(e, filter); };
    auto imIt = lane.immediate.begin();
    while (imIt != lane.immediate.end() && !matches(*imIt)) {
        ++imIt;
    }
    auto delIt = lane.delayed.begin();
    while (delIt != lane.delayed.end() &&
           (!matches(delIt->second) || (requireSignaledDelayed && !delIt->second.signaled))) {
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

double EventLoop::PeekDueLocked(Lane& lane, double now) {
    // immediate entries are enqueued with monotonically increasing times, so
    // the front is the earliest
    double due = lane.immediate.empty() ? -1 : lane.immediate.front().time;
    if (!lane.delayed.empty() && lane.delayed.begin()->first <= now &&
        (due < 0 || lane.delayed.begin()->first < due)) {
        due = lane.delayed.begin()->first;
    }
    return due;
}

double EventLoop::PeekDueFilteredLocked(Lane& lane, DrainFilter filter, double now) {
    auto matches = [&](const Entry& e) { return MatchesFilter(e, filter); };
    double due = -1;
    for (const auto& e : lane.immediate) {
        if (matches(e)) {
            due = e.time;
            break;
        }
    }
    for (const auto& pair : lane.delayed) {
        if (pair.first > now) {
            break;
        }
        if (matches(pair.second) && (due < 0 || pair.first < due)) {
            due = pair.first;
        }
    }
    return due;
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
    if (entry.bare) {
        // the fn locks its own (possibly different) isolate; taking this
        // loop's Locker here would nest Lockers across isolates
        entry.fn();
        return;
    }
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
        entry = TakeDueLocked(internal_, DrainFilter::kAny, true, now_ms());
        if (entry == nullptr) {
            // leftover unit: the work it represented ran early from a direct
            // drain - this dispatch just consumed it, so it is no longer
            // WaitForInternalWork's to swallow
            if (leftoverUnits_ > 0) {
                leftoverUnits_--;
            }
            return;
        }
    }
    RunEntry(*entry);
}

bool EventLoop::IsPumping() { return t_pumpDepth > 0; }

void EventLoop::DeferJavaThrow(std::shared_ptr<NativeScriptException> ex) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stopped_) {
        return;
    }
    deferredJavaThrows_.push_back(std::move(ex));
    // the wakeup: an empty ordered entry whose token forces a nativeRunTask
    // visit once the looper resumes, even if a drain consumes the entry first
    PostOrderedLocked(Entry{nullptr, []() {}, true, false, 0}, 0);
}

void EventLoop::ReportDeferredJavaError() {
    std::shared_ptr<NativeScriptException> ex;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (deferredJavaThrows_.empty()) {
            return;
        }
        ex = std::move(deferredJavaThrows_.front());
        deferredJavaThrows_.pop_front();
    }
    ex->ReThrowToJava();
}

void EventLoop::SetPumpDrainHook(std::function<int()> hook) {
    // home thread only, like every consumer of pumpDrainHook_
    pumpDrainHook_ = std::move(hook);
}

// Runs `body` without letting a C++ exception escape, deferring the Java-side
// report while a pump is on the stack (RunGuarded's direct ReThrowToJava arms
// a pending JNI exception, which is only legal when returning to Java is the
// next act).
void EventLoop::GuardEntryRun(const std::function<void()>& body) {
    if (!IsPumping()) {
        RunGuarded(body);
        return;
    }
    try {
        body();
    } catch (NativeScriptException& ex) {
        // what() may read a JNI local ref that cannot outlive this dispatch,
        // so only its text is carried
        DeferJavaThrow(std::make_shared<NativeScriptException>(std::string(ex.what())));
    } catch (std::exception& ex) {
        DEBUG_WRITE_FORCE("Error: c++ exception in event loop task: %s", ex.what());
    } catch (...) {
        DEBUG_WRITE_FORCE("Error: unknown c++ exception in event loop task!");
    }
}

void EventLoop::RunNestableV8Tasks() {
    RunDueInternalWork(DrainFilter::kNestableV8);
}

void EventLoop::RunDueInternalWork(DrainFilter filter) {
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
            const size_t delayedBefore = internal_.delayed.size();
            entry = TakeDueLocked(internal_, filter, false, now_ms());
            if (entry == nullptr) {
                return;
            }
            if (entry->unitIssued) {
                leftoverUnits_++;
            }
            if (internal_.delayed.size() != delayedBefore) {
                // a drained delayed entry may leave the timerfd armed (or
                // expired unread) for it; rearming to the queue's new
                // earliest also discards the stale expiration
                ArmTimerLocked(now_ms());
            }
        }
        // the pause loops call this from inside v8 inspector frames - a C++
        // exception must not unwind through them
        GuardEntryRun([&] { RunEntry(*entry); });
    }
}

bool EventLoop::RunOneOrderedDue() {
    // one due slot across the whole ordered domain: pick the earliest due
    // item among the ordered entries and the timer source, whichever it is.
    // Timers and entries only ever run on this thread, so the peeked winner
    // can't be taken by anyone else before we re-lock (a concurrent post can
    // only add later work).
    auto now = now_ms();
    double entryDue;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) {
            return false;
        }
        entryDue = PeekDueLocked(ordered_, now);
    }
    if (timerSource_ != nullptr && timerSource_->RunIfEarliest(now, entryDue)) {
        // fn entries get their checkpoint in RunEntry; a timer callback runs
        // under kAuto, which skips the depth-0 drain whenever a pump's JS
        // frames are on the stack, so drain here or a microtask enqueued by
        // one timer runs after the next timer instead of before it
        v8::Locker locker(isolate_);
        v8::Isolate::Scope isolateScope(isolate_);
        v8::HandleScope handleScope(isolate_);
        isolate_->PerformMicrotaskCheckpoint();
        return true;
    }
    if (entryDue < 0) {
        // leftover token, or an idle drain: nothing in the domain is due yet
        return false;
    }
    std::unique_ptr<Entry> entry;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) {
            return false;
        }
        entry = TakeDueLocked(ordered_, DrainFilter::kAny, false, now_ms());
    }
    if (entry == nullptr) {
        return false;
    }
    if (IsPumping()) {
        // an ordered entry's failure is its own report, never the pumping
        // require's; on the token path the throw belongs to nativeRunTask
        GuardEntryRun([&] { RunEntry(*entry); });
    } else {
        RunEntry(*entry);
    }
    return true;
}

void EventLoop::RunOrderedTask() {
    // one anonymous token = one due slot; a token whose item a pump drained
    // early finds nothing due and dies here
    RunOneOrderedDue();
}

int EventLoop::RunDueOrderedEntries() {
    // Bounded slice: a callback that keeps minting due-now work (a
    // setTimeout(0) chain) must not pin the calling pump past its own
    // deadline checks, so the drain yields after a few milliseconds and the
    // pump comes back for the rest on its next iteration.
    constexpr double kSliceMs = 8.0;
    const double start = now_ms();
    int ran = 0;
    while (!isolate_->IsExecutionTerminating() && RunOneOrderedDue()) {
        ran++;
        if (now_ms() - start >= kSliceMs) {
            break;
        }
    }
    return ran;
}

EventLoop::PumpResult EventLoop::PumpUntil(double deadlineSeconds,
                                           const std::function<bool()>& settled,
                                           bool drainLooperWork) {
    // home thread only: the drains below take ordered/timer slots and eventfd
    // units that the looper's own dispatch owns on that thread
    NS_DCHECK(looper_ == nullptr || ALooper_forThread() == looper_);
    PumpScope pumpScope;
    const auto deadline = std::chrono::steady_clock::now() +
                          std::chrono::duration<double>(deadlineSeconds);
    for (;;) {
        if (settled()) {
            return PumpResult::kSettled;
        }
        // Both probes: IsExecutionTerminating is true only while JS frames
        // unwind with the termination exception active, so a pump parked with
        // nothing queued would never observe TerminateExecution through it.
        if (terminationRequested_.load(std::memory_order_acquire) ||
            isolate_->IsExecutionTerminating()) {
            return PumpResult::kTerminated;
        }
        if (IsStopped()) {
            // a stopped loop drops every post, so nothing can settle anymore
            return PumpResult::kTerminated;
        }
        if (std::chrono::steady_clock::now() >= deadline) {
            return PumpResult::kDeadline;
        }
        RunDueInternalWork(drainLooperWork ? DrainFilter::kPumpDeliverable
                                           : DrainFilter::kNestableV8);
        {
            // work may enqueue microtasks without entering JS; scopes are
            // re-entrant, so callers already holding them pay nothing
            v8::Locker locker(isolate_);
            v8::Isolate::Scope isolateScope(isolate_);
            v8::HandleScope handleScope(isolate_);
            isolate_->PerformMicrotaskCheckpoint();
        }
        int ranLooperWork = 0;
        if (drainLooperWork) {
            ranLooperWork = RunDueOrderedEntries();
            if (pumpDrainHook_ != nullptr) {
                ranLooperWork += pumpDrainHook_();
            }
        }
        if (settled()) {
            return PumpResult::kSettled;
        }
        if (ranLooperWork == 0) {
            WaitForInternalWork(10, /*pumpDeliverable=*/drainLooperWork);
        }
    }
}

namespace {
// Depth, not a flag: an fd callback can dispatch JS that lands back in
// another callback through a nested drain.
thread_local int t_looperCallbackDepth = 0;

struct LooperCallbackScope {
    LooperCallbackScope() { ++t_looperCallbackDepth; }
    ~LooperCallbackScope() { --t_looperCallbackDepth; }
};
}  // namespace

bool EventLoop::IsInLooperCallback() { return t_looperCallbackDepth > 0; }

void EventLoop::WaitForInternalWork(int timeoutMs, bool pumpDeliverable) {
    const DrainFilter filter =
            pumpDeliverable ? DrainFilter::kPumpDeliverable : DrainFilter::kNestableV8;
    struct pollfd fds[2];
    nfds_t count = 0;
    bool sleepOnly = false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopped_) {
            sleepOnly = true;
        } else {
            const double now = now_ms();
            // Drainable work already due: the caller's drain runs it, waiting
            // would only add latency. The filter must match the drain mode of
            // the pump idling here — a due entry the drain cannot take must
            // not turn the wait into a no-op.
            if (PeekDueFilteredLocked(internal_, filter, now) >= 0) {
                return;
            }
            // units whose entries a direct drain already consumed keep the
            // eventfd readable; swallow them or the poll below returns
            // immediately on every call
            while (leftoverUnits_ > 0 && eventFd_ != -1) {
                uint64_t value;
                if (read(eventFd_, &value, sizeof(value)) != sizeof(value)) {
                    break;
                }
                leftoverUnits_--;
            }
            // A due entry the drain cannot take (non-nestable task, plain fn
            // post) pins its unread unit in the eventfd, so the fds cannot go
            // quiet — polling them would spin. Plain sleep is the only honest
            // wait until the looper resumes and runs it.
            if (PeekDueLocked(internal_, now) >= 0) {
                sleepOnly = true;
            } else {
                if (eventFd_ != -1) fds[count++] = {eventFd_, POLLIN, 0};
                if (timerFd_ != -1) fds[count++] = {timerFd_, POLLIN, 0};
            }
        }
    }
    if (sleepOnly || count == 0) {
        usleep(static_cast<useconds_t>(timeoutMs) * 1000);
        return;
    }
    poll(fds, count, timeoutMs);
}

int EventLoop::EventFdCallback(int fd, int events, void* data) {
    LooperCallbackScope callbackScope;
    uint64_t value;
    // EFD_SEMAPHORE: consumes exactly one unit; while more remain the fd stays
    // readable and ALooper calls back next poll, interleaving with Java
    // messages instead of draining in one go. A spurious wakeup with nothing
    // to read must not consume an entry, or its real unit becomes a leftover.
    if (read(fd, &value, sizeof(value)) != sizeof(value)) {
        return 1;
    }
    RunGuarded([&] { static_cast<EventLoop*>(data)->RunOneInternal(); });
    return 1;
}

int EventLoop::TimerFdCallback(int fd, int events, void* data) {
    LooperCallbackScope callbackScope;
    uint64_t expirations;
    if (read(fd, &expirations, sizeof(expirations)) != sizeof(expirations)) {
        return 1;
    }
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
                pair.second.unitIssued = true;
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
        auto* loop = reinterpret_cast<tns::EventLoop*>(nativeLoopPtr);
        loop->RunOrderedTask();
        // returning to Java is the next act, so a report a pump had to defer
        // is safe to arm here
        loop->ReportDeferredJavaError();
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
