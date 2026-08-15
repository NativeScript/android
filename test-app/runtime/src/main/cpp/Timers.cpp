#include "Timers.h"
#include "ArgConverter.h"
#include "Runtime.h"
#include "NativeScriptException.h"
#include "ModuleBinding.h"
#include "RuntimeState.h"
#include "NativeScriptPlatform.h"
#include "Util.h"
#include <cmath>
#include <sstream>


/**
 * Overall rules when modifying this file:
 * Everything runs on the isolate's thread (or under its v8::Locker): there are
 * no background threads and no locking. `sortedTimers_` must always be sorted
 * by dueTime (stable for equal dueTimes) and in sync with `timerMap_` (except
 * tombstones, which only live in `sortedTimers_`).
 *
 * Scheduling model: every scheduled timer posts one anonymous "due token"
 * through the runtime EventLoop's ordered lane (a Java Handler bound to this
 * thread's Looper), at uptimeMillis >= the timer's due time. Timers therefore
 * share one queue with Handler.post/postDelayed and ordered macrotasks, and
 * interleave with Java messages in exact MessageQueue order. Because the Java
 * queue is millisecond-quantized, the token does not name a timer: the
 * EventLoop drain consumes the earliest due item across this list and its own
 * ordered entries. Cancelled timers leave a tombstone so their token consumes
 * a slot as a no-op instead of lending its position to a later item.
 * ALL changes and scheduling of a TimerTask MUST be done when locked in an isolate to ensure consistency
 */

using namespace v8;

// Takes a value and transform into a positive number
// returns a negative number if the number is negative or invalid
inline static double
ToMaybePositiveValue(const v8::Local<v8::Value> &v, const v8::Local<v8::Context> &ctx) {
    double value = -1;
    if (v->IsNullOrUndefined()) {
        return -1;
    }
    Local<Number> numberValue;
    auto success = v->ToNumber(ctx).ToLocal(&numberValue);
    if (success) {
        value = numberValue->Value();
        if (isnan(value)) {
            value = -1;
        }
    }
    return value;
}

static double now_ms() {
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}

namespace tns {

void Timers::Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &globalObjectTemplate) {
    isolate_ = isolate;
    // TODO: remove the __ns__ prefix once this is validated
    SetMethod(isolate, globalObjectTemplate, "__ns__setTimeout", SetTimeoutCallback, External::New(isolate, this, v8::kExternalPointerTypeTagDefault));
    SetMethod(isolate, globalObjectTemplate, "__ns__setInterval", SetIntervalCallback, External::New(isolate, this, v8::kExternalPointerTypeTagDefault));
    SetMethod(isolate, globalObjectTemplate, "__ns__clearTimeout", ClearTimer, External::New(isolate, this, v8::kExternalPointerTypeTagDefault));
    SetMethod(isolate, globalObjectTemplate, "__ns__clearInterval", ClearTimer, External::New(isolate, this, v8::kExternalPointerTypeTagDefault));

    // PrepareV8Runtime bound the loop to this thread's looper before any
    // builtin initialization runs
    eventLoop_ = NativeScriptPlatform::Instance()->GetEventLoop(isolate);
    eventLoop_->SetTimerSource(this);
    stopped_ = false;
}

void Timers::addTask(std::shared_ptr<TimerTask> task) {
    if (task->queued_) {
        return;
    }
    auto now = now_ms();
    task->queued_ = true;
#ifdef NS_TIMERS_NESTING_CLAMP
    task->nestingLevel_ = nesting + 1;
    // theoretically this should be >5 on the spec, but we're following chromium behavior here again
    if (task->nestingLevel_ >= 5 && task->frequency_ < 4) {
        task->frequency_ = 4;
        task->startTime_ = now;
    }
#endif
    timerMap_.emplace(task->id_, task);
    task->dueTime_ = task->NextTime(now);
    auto it = std::upper_bound(sortedTimers_.begin(), sortedTimers_.end(), task->dueTime_,
                               [](const double &value, const TimerReference &ref) {
                                   return ref.dueTime > value;
                               });
    sortedTimers_.insert(it, TimerReference{task->id_, task->dueTime_});
    try {
        postTimer(task, now);
    } catch (...) {
        // No token reached the queue: the slot must not linger, tokenless -
        // as a live entry it would consume some other token's slot, and as a
        // tombstone it would starve the item behind it. Erase it outright.
        auto sit = std::lower_bound(sortedTimers_.begin(), sortedTimers_.end(), task->dueTime_,
                                    [](const TimerReference &ref, const double &value) {
                                        return ref.dueTime < value;
                                    });
        while (sit != sortedTimers_.end() && sit->dueTime == task->dueTime_) {
            if (sit->id == task->id_) {
                sortedTimers_.erase(sit);
                break;
            }
            ++sit;
        }
        timerMap_.erase(task->id_);
        task->Unschedule();
        throw;
    }
}

// Above this remaining delay a timer's token gets an identified peer so a
// clear removes the queued message outright: the stale wakeup is the cost
// worth paying JNI to avoid (debounce-style long timers on a possibly idle
// device). Below it the wakeup lands within two frames of the interaction
// that scheduled it - the app is provably awake - so the timer takes the
// zero-overhead claim-cell path instead.
static constexpr double kIdentifiedCutoffMs = 32;

void Timers::postTimer(const std::shared_ptr<TimerTask> &task, double now) {
    // uptimeMillis is the integer part of the same CLOCK_MONOTONIC clock as
    // now_ms(). Due-now timers post at (jlong) now so they tie (and FIFO) with
    // a Handler.postDelayed(0) made in the same millisecond; future timers
    // post at ceil(dueTime) so the token never arrives before the due time.
    auto when = task->dueTime_ <= now ? (jlong) now : (jlong) std::ceil(task->dueTime_);
    // an interval re-arm orphans the previous token's carriers: the old token
    // stays valid anonymously, only the newest one is cancellable
    releaseTokenCarriers(task);
    if (task->dueTime_ - now >= kIdentifiedCutoffMs) {
        task->tokenPeer_ = eventLoop_->PostIdentifiedTimerToken(when);
        if (task->tokenPeer_ != nullptr) {
            return;
        }
    }
    task->tokenCell_ = eventLoop_->PostTimerToken(when, task->id_);
}

void Timers::releaseTokenCarriers(const std::shared_ptr<TimerTask> &task) {
    if (task->tokenPeer_ != nullptr) {
        eventLoop_->ReleaseIdentifiedToken(task->tokenPeer_);
        task->tokenPeer_ = nullptr;
    }
    task->tokenCell_ = 0;
}

void Timers::removeTask(const std::shared_ptr<TimerTask> &task) {
    removeTask(task->id_);
}

void Timers::removeTask(const int &taskId) {
    auto it = timerMap_.find(taskId);
    if (it != timerMap_.end()) {
        if (it->second->queued_) {
            // First try to neutralize the pending token itself. Winning the
            // claim means the token is guaranteed dead wherever it is, so the
            // sorted entry can be erased outright - token and slot leave
            // together and no wakeup work remains. Losing means dispatch
            // already owns the token (in flight past the claim gate), so
            // leave a tombstone: the owned token consumes it as a no-op
            // instead of running whatever item happens to be due next, which
            // could jump foreign Java messages queued between the two tokens'
            // positions.
            bool tokenNeutralized = false;
            if (it->second->tokenPeer_ != nullptr) {
                tokenNeutralized = eventLoop_->CancelIdentifiedToken(it->second->tokenPeer_);
                it->second->tokenPeer_ = nullptr;  // ref released by the cancel
            } else if (it->second->tokenCell_ != 0) {
                tokenNeutralized = eventLoop_->CancelClaimCell(it->second->tokenCell_);
                it->second->tokenCell_ = 0;
            }
            auto dueTime = it->second->dueTime_;
            auto sit = std::lower_bound(sortedTimers_.begin(), sortedTimers_.end(), dueTime,
                                        [](const TimerReference &ref, const double &value) {
                                            return ref.dueTime < value;
                                        });
            while (sit != sortedTimers_.end() && sit->dueTime == dueTime) {
                if (sit->id == taskId) {
                    if (tokenNeutralized) {
                        sortedTimers_.erase(sit);
                    } else {
                        sit->cancelled = true;
                    }
                    break;
                }
                ++sit;
            }
        }
        releaseTokenCarriers(it->second);
        it->second->Unschedule();
        timerMap_.erase(it);
    }
}

void Timers::Destroy() {
    if (stopped_) {
        return;
    }
    stopped_ = true;
    if (eventLoop_ != nullptr) {
        for (auto &pair : timerMap_) {
            releaseTokenCarriers(pair.second);
        }
        // the loop is already shut down by DestroyRuntime at this point (its
        // handler dropped every pending token), but the source pointer must
        // not outlive this object
        eventLoop_->SetTimerSource(nullptr);
        eventLoop_.reset();
    }
    timerMap_.clear();
    sortedTimers_.clear();
    isolate_ = nullptr;
}

Timers::~Timers() {
    Destroy();
}

void Timers::SetTimeoutCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Timers::SetTimer(args, false);
}

void Timers::SetIntervalCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Timers::SetTimer(args, true);
}

void Timers::ClearTimer(const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto argLength = args.Length();
    auto extData = args.Data().As<External>();
    auto thiz = reinterpret_cast<Timers *>(extData->Value(v8::kExternalPointerTypeTagDefault));
    int id = -1;
    if (argLength > 0) {
        auto isolate = args.GetIsolate();
        auto ctx = isolate->GetCurrentContext();
        id = (int) ToMaybePositiveValue(args[0], ctx);
    }
    // ids start at 1
    if (id > 0) {
        thiz->removeTask(id);
    }
}

void Timers::SetTimer(const v8::FunctionCallbackInfo<v8::Value> &args, bool repeatable) {
    auto argLength = args.Length();
    auto extData = args.Data().As<External>();
    auto thiz = reinterpret_cast<Timers *>(extData->Value(v8::kExternalPointerTypeTagDefault));
    int id = ++thiz->currentTimerId;
    if (argLength >= 1) {
        if (!args[0]->IsFunction()) {
            args.GetReturnValue().Set(-1);
            return;
        }
        auto handler = args[0].As<Function>();
        auto isolate = args.GetIsolate();
        auto ctx = isolate->GetCurrentContext();
        long timeout = 0;
        if (argLength >= 2) {
            timeout = (long) ToMaybePositiveValue(args[1], ctx);
            if (timeout < 0) {
                timeout = 0;
            }
        }
        std::shared_ptr<std::vector<std::shared_ptr<Persistent<Value>>>> argArray;
        if (argLength >= 3) {
            auto otherArgLength = argLength - 2;
            argArray = std::make_shared<std::vector<std::shared_ptr<Persistent<Value>>>>(
                    otherArgLength);
            for (int i = 0; i < otherArgLength; i++) {
                (*argArray)[i] = std::make_shared<Persistent<Value>>(isolate, args[i + 2]);
            }
        }


        auto task = std::make_shared<TimerTask>(isolate, ctx, handler, timeout, repeatable,
                                                argArray, id, now_ms());
        try {
            thiz->addTask(task);
        } catch (NativeScriptException &e) {
            // a failed JNI token post must surface as a JS exception, not
            // unwind through the V8 callback frame
            e.ReThrowToV8();
            return;
        }
    }
    args.GetReturnValue().Set(id);

}

/**
 * Invoked by the EventLoop's ordered-lane token drain on the isolate's
 * thread. Under one Locker acquisition (sortedTimers_ is mutated through
 * setTimeout from background threads under multithreaded JS): if the front
 * slot is due and earlier-or-equal to the loop's own earliest entry, consume
 * it - firing the earliest due timer (exact sub-millisecond order, not
 * necessarily the timer that enqueued the token) or swallowing a tombstone
 * left by clearTimeout/clearInterval.
 */
bool Timers::RunIfEarliest(double now, double otherDue) {
    auto isolate = isolate_;
    if (stopped_ || isolate == nullptr || isolate->IsDead()) {
        return false;
    }
    // thread safety is important!
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    if (sortedTimers_.empty()) {
        return false;
    }
    auto ref = sortedTimers_.front();
    if (ref.dueTime > now_ms() || (otherDue >= 0 && ref.dueTime > otherDue)) {
        // not due, or the loop's own entry is earlier - not this source's slot
        return false;
    }
    sortedTimers_.erase(sortedTimers_.begin());
    if (ref.cancelled) {
        // tombstone: this slot's token is spent doing nothing, keeping tokens
        // and slots 1:1 so later items can't jump foreign Java messages
        return true;
    }
    auto it = timerMap_.find(ref.id);
    if (it != timerMap_.end()) {
        auto task = it->second;
        // task is no longer in queue to be executed
        task->queued_ = false;
#ifdef NS_TIMERS_NESTING_CLAMP
        nesting = task->nestingLevel_;
#endif
        if (task->repeats_) {
            // the reason we're doing this in kind of a convoluted way is to follow more closely the chromium implementation than the node implementation
            // imagine an interval of 1000ms
            // node's setInterval drifts slightly (1000, 2001, 3001, 4002, some busy work 5050, 6050)
            // chromium will be consistent: (1000, 2001, 3000, 4000, some busy work 5050, 6000)
            task->startTime_ = task->dueTime_;
            addTask(task);
        }
        v8::Local<v8::Function> cb = task->callback_.Get(isolate);
        Runtime* runtime = Runtime::GetRuntime(isolate);
        v8::Local<v8::Context> context = runtime->GetContext();
        Context::Scope context_scope(context);
        TryCatch tc(isolate);
        auto argc = task->args_.get() == nullptr ? 0 : task->args_->size();
        if (argc > 0) {
            Local<Value> argv[argc];
            for (int i = 0; i < argc; i++) {
                argv[i] = task->args_->at(i)->Get(isolate);
            }
            cb->Call(context, context->Global(), argc, argv);
        } else {
            cb->Call(context, context->Global(), 0, nullptr);
        }
        // task is not queued, so it's either a setTimeout or a cleared setInterval
        // ensure we remove it
        if (!task->queued_) {
            removeTask(task);
        }

#ifdef NS_TIMERS_NESTING_CLAMP
        nesting = 0;
#endif

        if (tc.HasCaught() &&
            !NativeScriptException::ContainUncaughtCallbackException(isolate, tc)) {
            NativeScriptException(tc).ReThrowToJava();
        }


    }
    // the slot was consumed (front popped) even if the map had no task
    return true;
}

void Timers::InitStatic(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> globalObjectTemplate) {
   // Owned by the runtime, which destroys it while the isolate is still alive
   // -- what releasing the tasks' handles and their Java token peers requires.
   auto* timers = RuntimeState::For<Timers>(isolate);
   if (timers == nullptr) {
       return;
   }
   timers->Init(isolate, globalObjectTemplate);
}

};

NODE_BINDING_PER_ISOLATE_INIT_OBJ(timers, tns::Timers::InitStatic);
