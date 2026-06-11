#include "Timers.h"
#include "ArgConverter.h"
#include "Runtime.h"
#include "NativeScriptException.h"
#include "ModuleBinding.h"
#include "IsolateDisposer.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "Util.h"
#include <cmath>
#include <sstream>


/**
 * Overall rules when modifying this file:
 * Everything runs on the isolate's thread (or under its v8::Locker): there are
 * no background threads and no locking. `sortedTimers_` must always be sorted
 * by dueTime (stable for equal dueTimes) and in sync with `timerMap_`.
 *
 * Scheduling model: every scheduled timer enqueues one anonymous "due token"
 * message on a dedicated Java Handler bound to this thread's Looper, at
 * uptimeMillis >= the timer's due time. Timers therefore share one queue with
 * Handler.post/postDelayed and interleave with Java messages in exact
 * MessageQueue order. Because the Java queue is millisecond-quantized, the
 * token does not name a timer: each token fires the front of `sortedTimers_`
 * (the earliest due timer by exact sub-millisecond time). A token whose front
 * timer is not yet due is a leftover from a cleared timer and is dropped.
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

jclass Timers::TIMER_HANDLER_CLASS = nullptr;
jmethodID Timers::TIMER_HANDLER_CTOR = nullptr;
jmethodID Timers::TIMER_HANDLER_POST = nullptr;
jmethodID Timers::TIMER_HANDLER_RELEASE = nullptr;

void Timers::Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &globalObjectTemplate) {
    isolate_ = isolate;
    // TODO: remove the __ns__ prefix once this is validated
    SetMethod(isolate, globalObjectTemplate, "__ns__setTimeout", SetTimeoutCallback, External::New(isolate, this));
    SetMethod(isolate, globalObjectTemplate, "__ns__setInterval", SetIntervalCallback, External::New(isolate, this));
    SetMethod(isolate, globalObjectTemplate, "__ns__clearTimeout", ClearTimer, External::New(isolate, this));
    SetMethod(isolate, globalObjectTemplate, "__ns__clearInterval", ClearTimer, External::New(isolate, this));

    JEnv env;
    if (TIMER_HANDLER_CLASS == nullptr) {
        // JEnv::FindClass caches a global ref to the class
        TIMER_HANDLER_CLASS = env.FindClass("com/tns/TimerHandler");
        assert(TIMER_HANDLER_CLASS != nullptr);
        TIMER_HANDLER_CTOR = env.GetMethodID(TIMER_HANDLER_CLASS, "<init>", "(J)V");
        TIMER_HANDLER_POST = env.GetMethodID(TIMER_HANDLER_CLASS, "post", "(J)V");
        TIMER_HANDLER_RELEASE = env.GetMethodID(TIMER_HANDLER_CLASS, "release", "()V");
    }
    // the handler binds to the current thread's Looper, which Runtime.java
    // prepares before initNativeScript on both the main and worker threads
    JniLocalRef handler(env.NewObject(TIMER_HANDLER_CLASS, TIMER_HANDLER_CTOR,
                                      reinterpret_cast<jlong>(this)));
    handler_ = env.NewGlobalRef(handler);
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
    postTimer(task, now);
}

void Timers::postTimer(const std::shared_ptr<TimerTask> &task, double now) {
    // uptimeMillis is the integer part of the same CLOCK_MONOTONIC clock as
    // now_ms(). Due-now timers post at (jlong) now so they tie (and FIFO) with
    // a Handler.postDelayed(0) made in the same millisecond; future timers
    // post at ceil(dueTime) so the token never arrives before the due time.
    auto when = task->dueTime_ <= now ? (jlong) now : (jlong) std::ceil(task->dueTime_);
    JEnv env;
    env.CallVoidMethod(handler_, TIMER_HANDLER_POST, when);
}

void Timers::removeTask(const std::shared_ptr<TimerTask> &task) {
    removeTask(task->id_);
}

void Timers::removeTask(const int &taskId) {
    auto it = timerMap_.find(taskId);
    if (it != timerMap_.end()) {
        // if still scheduled, drop the sorted entry; the token already in the
        // java queue will find a not-yet-due (or no) front timer and no-op
        if (it->second->queued_) {
            auto dueTime = it->second->dueTime_;
            auto sit = std::lower_bound(sortedTimers_.begin(), sortedTimers_.end(), dueTime,
                                        [](const TimerReference &ref, const double &value) {
                                            return ref.dueTime < value;
                                        });
            while (sit != sortedTimers_.end() && sit->dueTime == dueTime) {
                if (sit->id == taskId) {
                    sortedTimers_.erase(sit);
                    break;
                }
                ++sit;
            }
        }
        it->second->Unschedule();
        timerMap_.erase(it);
    }
}

void Timers::Destroy() {
    if (stopped_) {
        return;
    }
    stopped_ = true;
    if (handler_ != nullptr) {
        JEnv env;
        env.CallVoidMethod(handler_, TIMER_HANDLER_RELEASE);
        env.DeleteGlobalRef(handler_);
        handler_ = nullptr;
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
    auto thiz = reinterpret_cast<Timers *>(extData->Value());
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
    auto thiz = reinterpret_cast<Timers *>(extData->Value());
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
        thiz->addTask(task);
    }
    args.GetReturnValue().Set(id);

}

/**
 * Invoked by Java TimerHandler.handleMessage on the isolate's thread, once per
 * scheduled "due token". Fires the earliest due timer (exact sub-millisecond
 * order), which is not necessarily the timer that enqueued this token.
 */
void Timers::FireTimer() {
    auto isolate = isolate_;
    if (stopped_ || isolate == nullptr || isolate->IsDead()) {
        return;
    }
    // thread safety is important!
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    if (sortedTimers_.empty()) {
        // leftover token of a cleared timer
        return;
    }
    auto ref = sortedTimers_.front();
    if (ref.dueTime > now_ms()) {
        // the earliest timer isn't due yet, so this token is a leftover from a
        // cleared timer; the front timer's own token will arrive at its due time
        return;
    }
    sortedTimers_.erase(sortedTimers_.begin());
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

        if (tc.HasCaught()) {
            NativeScriptException(tc).ReThrowToJava();
        }


    }
}

void Timers::InitStatic(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> globalObjectTemplate) {
   auto timers = new Timers();
   timers->Init(isolate, globalObjectTemplate);
   registerIsolateBoundObject(isolate, timers);
}

};

NODE_BINDING_PER_ISOLATE_INIT_OBJ(timers, tns::Timers::InitStatic);

extern "C" JNIEXPORT void JNICALL Java_com_tns_TimerHandler_nativeFireTimer(
        JNIEnv *env, jclass clazz, jlong timersPtr) {
    try {
        reinterpret_cast<tns::Timers *>(timersPtr)->FireTimer();
    } catch (tns::NativeScriptException &e) {
        e.ReThrowToJava();
    } catch (std::exception &e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        tns::NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        tns::NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}
