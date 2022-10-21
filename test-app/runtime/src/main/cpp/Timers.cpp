#include "Timers.h"
#include "ArgConverter.h"
#include "Runtime.h"
#include "NativeScriptException.h"
#include <android/looper.h>
#include <unistd.h>
#include <thread>


/**
 * Overall rules when modifying this file:
 * `sortedTimers_` must always be sorted by dueTime
 * `sortedTimers_`. `deletedTimers_` and `stopped` modifications MUST be done while locked with the mutex
 * `threadLoop` must not access anything that is not `sortedTimers_` or `stopped` or any atomic var
 * ALL changes and scheduling of a TimerTask MUST be done when locked in an isolate to ensure consistency
 */

using namespace tns;
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


void Timers::Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &globalObjectTemplate) {
    isolate_ = isolate;
    // TODO: remove the __ns__ prefix once this is validated
    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__ns__setTimeout"),
                              FunctionTemplate::New(isolate, SetTimeoutCallback,
                                                    External::New(isolate, this)));
    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__ns__setInterval"),
                              FunctionTemplate::New(isolate, SetIntervalCallback,
                                                    External::New(isolate, this)));
    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__ns__clearTimeout"),
                              FunctionTemplate::New(isolate, ClearTimer,
                                                    External::New(isolate, this)));
    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__ns__clearInterval"),
                              FunctionTemplate::New(isolate, ClearTimer,
                                                    External::New(isolate, this)));
    auto res = pipe(fd_);
    assert(res != -1);
    res = fcntl(fd_[1], F_SETFL, O_NONBLOCK);
    assert(res != -1);
    // TODO: check success of fd
    looper_ = ALooper_prepare(0);
    ALooper_acquire(looper_);
    ALooper_addFd(looper_, fd_[0], ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT,
                  PumpTimerLoopCallback, this);
    ALooper_wake(looper_);
    watcher_ = std::thread(&Timers::threadLoop, this);
    stopped = false;
}

void Timers::addTask(std::shared_ptr<TimerTask> task) {
    if (task->queued_) {
        return;
    }
    auto now = now_ms();
    task->nestingLevel_ = nesting + 1;
    task->queued_ = true;
    // theoretically this should be >5 on the spec, but we're following chromium behavior here again
    if (task->nestingLevel_ >= 5 && task->frequency_ < 4) {
        task->frequency_ = 4;
        task->startTime_ = now;
    }
    timerMap_.emplace(task->id_, task);
    auto newTime = task->NextTime(now);
    task->dueTime_ = newTime;
    bool needsScheduling = true;
    if (!isBufferFull.load() && task->dueTime_ <= now) {
        auto result = write(fd_[1], &task->id_, sizeof(int));
        if (result != -1 || errno != EAGAIN) {
            needsScheduling = false;
        } else {
            isBufferFull = true;
        }
    }
    if (needsScheduling) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            auto it = sortedTimers_.begin();
            auto dueTime = task->dueTime_;
            it = std::upper_bound(sortedTimers_.begin(), sortedTimers_.end(), dueTime,
                                  [](const double &value,
                                     const std::shared_ptr<TimerReference> &ref) {
                                      return ref->dueTime > value;
                                  });
            auto ref = std::make_shared<TimerReference>();
            ref->dueTime = task->dueTime_;
            ref->id = task->id_;
            sortedTimers_.insert(it, ref);
        }
        taskReady.notify_one();
    }
}

void Timers::removeTask(const std::shared_ptr<TimerTask> &task) {
    removeTask(task->id_);
}

void Timers::removeTask(const int &taskId) {
    auto it = timerMap_.find(taskId);
    if (it != timerMap_.end()) {
        auto wasScheduled = it->second->queued_;
        it->second->Unschedule();
        timerMap_.erase(it);
        // we could safely remove this block to avoid locking the main thread
        // the only issue is that removing this will trigger more small locks on the main thread
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (wasScheduled) {
                // was scheduled, notify the thread so it doesn't trigger again
                this->deletedTimers_.insert(taskId);
            } else {
                // was not scheduled, remove it to reduce memory footprint
                this->deletedTimers_.erase(taskId);
            }
        }
    }
}

void Timers::threadLoop() {
    std::unique_lock<std::mutex> lk(mutex);
    while (!stopped) {
        if (!sortedTimers_.empty()) {
            auto timer = sortedTimers_.at(0);
            if (deletedTimers_.find(timer->id) != deletedTimers_.end()) {
                sortedTimers_.erase(sortedTimers_.begin());
                deletedTimers_.erase(timer->id);
                continue;
            }
            auto now = now_ms();
            // timer has reached its time, fire it and keep going
            if (timer->dueTime <= now) {
                sortedTimers_.erase(sortedTimers_.begin());
                auto result = write(fd_[1], &timer->id, sizeof(int));
                if (result == -1 && errno == EAGAIN) {
                    isBufferFull = true;
                    while (!stopped && deletedTimers_.find(timer->id) != deletedTimers_.end() &&
                           write(fd_[1], &timer->id, sizeof(int)) == -1 && errno == EAGAIN) {
                        bufferFull.wait(lk);
                    }
                } else if (isBufferFull.load() &&
                           (sortedTimers_.empty() || sortedTimers_.at(0)->dueTime > now)) {
                    // we had a successful write and the next timer is not due
                    // mark the buffer as free to re-enable the setTimeout with 0 optimization
                    isBufferFull = false;
                }
            } else {
                taskReady.wait_for(lk, std::chrono::milliseconds((int) (timer->dueTime - now)));
            }
        } else {
            taskReady.wait(lk);
        }
    }
}

void Timers::Destroy() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (stopped) {
            return;
        }
        stopped = true;
    }
    bufferFull.notify_one();
    taskReady.notify_all();
    watcher_.join();
    auto mainLooper = Runtime::GetMainLooper();
    ALooper_removeFd(mainLooper, fd_[0]);
    close(fd_[0]);
    timerMap_.clear();
    ALooper_release(looper_);
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
 * ALooper callback.
 * Responsible for checking if the callback is still scheduled and entering the isolate to trigger it
 */
int Timers::PumpTimerLoopCallback(int fd, int events, void *data) {
    int timerId;
    read(fd, &timerId, sizeof(int));

    auto thiz = static_cast<Timers *>(data);
    auto isolate = thiz->isolate_;
    if (thiz->stopped || isolate == nullptr || isolate->IsDead()) {
        return 0;
    }
    // thread safety is important!
    v8::Locker locker(thiz->isolate_);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto it = thiz->timerMap_.find(timerId);
    if (it != thiz->timerMap_.end()) {
        auto task = it->second;
        // task is no longer in queue to be executed
        task->queued_ = false;
        thiz->nesting = task->nestingLevel_;
        if (task->repeats_) {
            // the reason we're doing this in kind of a convoluted way is to follow more closely the chromium implementation than the node implementation
            // imagine an interval of 1000ms
            // node's setInterval drifts slightly (1000, 2001, 3001, 4002, some busy work 5050, 6050)
            // chromium will be consistent: (1000, 2001, 3000, 4000, some busy work 5050, 6000)
            task->startTime_ = task->dueTime_;
            thiz->addTask(task);
        }
        auto context = task->context_.Get(isolate);
        Context::Scope context_scope(context);
        TryCatch tc(isolate);
        auto argc = task->args_.get() == nullptr ? 0 : task->args_->size();
        if (argc > 0) {
            Local<Value> argv[argc];
            for (int i = 0; i < argc; i++) {
                argv[i] = task->args_->at(i)->Get(isolate);
            }
            task->callback_.Get(isolate)->Call(context, context->Global(), argc, argv);
        } else {
            task->callback_.Get(isolate)->Call(context, context->Global(), 0, nullptr);
        }
        // task is not queued, so it's either a setTimeout or a cleared setInterval
        // ensure we remove it
        if (!task->queued_) {
            thiz->removeTask(task);
        }

        thiz->nesting = 0;

        if (tc.HasCaught()) {
            NativeScriptException(tc).ReThrowToJava();
        }


    }
    thiz->bufferFull.notify_one();
    return 1;
}