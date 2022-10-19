#include "Timers.h"
#include "ArgConverter.h"
#include "Runtime.h"
#include <android/looper.h>
#include <unistd.h>
#include <thread>

using namespace tns;
using namespace v8;

static double now_ms(void) {
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}


void Timers::Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate>& globalObjectTemplate) {
    isolate_ = isolate;
    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__setTimeout"), FunctionTemplate::New(isolate, SetTimeoutCallback, External::New(isolate, this)));
    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__setInterval"), FunctionTemplate::New(isolate, SetIntervalCallback, External::New(isolate, this)));
    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__clearTimeout"), FunctionTemplate::New(isolate, ClearTimer, External::New(isolate, this)));
    globalObjectTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__clearInterval"), FunctionTemplate::New(isolate, ClearTimer, External::New(isolate, this)));
    pipe(fd_);
    fcntl(fd_[1], F_SETFL, O_NONBLOCK);
    // TODO: check success of fd
    looper_ = ALooper_prepare(0);
    ALooper_acquire(looper_);
    ALooper_addFd(looper_, fd_[0], ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT, PumpTimerLoopCallback, this);
    ALooper_wake(looper_);
    watcher_ = std::thread(&Timers::threadLoop, this);
}

void Timers::addTask(std::shared_ptr<TimerTask> task) {
    if (task->scheduled_) {
        return;
    }
    auto now = now_ms();
    task->nestingLevel_ = nesting + 1;
    task->scheduled_ = true;
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
            it = std::upper_bound(sortedTimers_.begin(), sortedTimers_.end(), dueTime, [](const double& value, const std::shared_ptr<TimerReference>& ref) {
                return ref->dueTime > value;
            });
//            while (it != sortedTimers_.end()) {
//                if (it->get()->dueTime > task->dueTime_) {
//                    break;
//                }
//                it++;
//            }
            auto ref= std::make_shared<TimerReference>();
            ref->dueTime = task->dueTime_;
            ref->id = task->id_;
            sortedTimers_.insert(it, ref);
        }
        cv.notify_one();
    }
}

void Timers::removeTask(std::shared_ptr<TimerTask> task) {
    timerMap_.erase(task->id_);
    task->Unschedule();
}
void Timers::removeTask(const int& taskId) {
    auto it = timerMap_.find(taskId);
    if (it != timerMap_.end()) {
        it->second->Unschedule();
        timerMap_.erase(it);
    }
}

void Timers::threadLoop() {
    std::unique_lock<std::mutex> lk(mutex);
    while (!stopped) {
        if (!sortedTimers_.empty()) {
            auto timer = sortedTimers_.at(0);
            auto now = now_ms();
            // timer has reached its time, fire it and keep going
            if (timer->dueTime <= now) {
                sortedTimers_.erase(sortedTimers_.begin());
                auto result = write(fd_[1], &timer->id, sizeof(int));
                if (result == -1 && errno == EAGAIN) {
                    isBufferFull = true;
                    while (!stopped && (write(fd_[1], &timer->id, sizeof(int)) == -1 && errno == EAGAIN)) {
                        bufferFull.wait(lk);
                    }
                } else if (isBufferFull.load() && (sortedTimers_.empty() || sortedTimers_.at(0)->dueTime > now)) {
                    // we had a successful write and the next timer is not due
                    // mark the buffer as free to re-enable the setTimeout with 0 optimization
                    isBufferFull = false;
                }
            } else {
                cv.wait_for(lk, std::chrono::milliseconds((int)(timer->dueTime - now)));
            }
        } else {
            cv.wait(lk);
        }
    }
}
Timers::~Timers() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        stopped = true;
    }
    bufferFull.notify_one();
    cv.notify_all();
    watcher_.join();
    auto mainLooper = Runtime::GetMainLooper();
    ALooper_removeFd(mainLooper, fd_[0]);
    close(fd_[0]);
    timerMap_.clear();
    ALooper_release(looper_);
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
        if (!args[0]->IsNullOrUndefined()) {
            Local<Number> numberValue;
            auto success = args[0]->ToNumber(ctx).ToLocal(&numberValue);
            if (success) {
                id = numberValue->Value();
                if (isnan(id)) {
                    id = -1;
                }
            }
        }
    }
    if (id > 0) {
        thiz->removeTask(id);
    }
}

void Timers::SetTimer(const v8::FunctionCallbackInfo<v8::Value> &args, bool repeatable) {
    auto argLength = args.Length();
    auto extData = args.Data().As<External>();
    auto thiz = reinterpret_cast<Timers*>(extData->Value());
    int id = ++thiz->currentTimerId;
    if (argLength >= 1) {
        auto handler = args[0].As<Function>();
        auto isolate = args.GetIsolate();
        auto ctx = isolate->GetCurrentContext();
        long timeout = 0;
        if (argLength >= 2) {
            if (!args[1]->IsNullOrUndefined()) {
                Local<Number> numberValue;
                auto success = args[1]->ToNumber(ctx).ToLocal(&numberValue);
                if (success) {
                    timeout = numberValue->Value();
                    if (isnan(timeout)) {
                        timeout = 0;
                    }
                }
            }
        }
        std::shared_ptr<std::vector<std::shared_ptr<Persistent<Value>>>> argArray;
        if (argLength >= 3) {
            auto otherArgLength = argLength - 2;
            argArray = std::make_shared<std::vector<std::shared_ptr<Persistent<Value>>>>(otherArgLength);
            for (int i = 0; i < otherArgLength; i++) {
                (*argArray)[i] = std::make_shared<Persistent<Value>>(isolate, args[i + 2]);
            }
        }


        auto task = std::make_shared<TimerTask>(isolate, ctx, handler, timeout, repeatable, argArray, id, now_ms());
        thiz->addTask(task);
    }
    args.GetReturnValue().Set(id);

}

int Timers::PumpTimerLoopCallback(int fd, int events, void* data) {
    int timerId;
    read(fd, &timerId, sizeof(int));

    auto thiz = static_cast<Timers*>(data);
    auto isolate = thiz->isolate_;
    if (thiz->stopped || isolate->IsDead()) {
        return 0;
    }
    // thread safety is important!
    v8::Locker locker(thiz->isolate_);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto it = thiz->timerMap_.find(timerId);
    if (it != thiz->timerMap_.end()) {
        auto task = it->second;
        task->scheduled_ = false;
        thiz->nesting = task->nestingLevel_;
        if (task->repeats_) {
            task->startTime_ = task->dueTime_;
            thiz->addTask(task);
        }
        auto context = task->context_.Get(isolate);
        auto argc = task->args_.get() == nullptr ? 0 : task->args_->size();
        if(argc > 0) {
            Local<Value> argv[argc];
            for(int i = 0 ; i < argc; i++) {
                argv[i] = task->args_->at(i)->Get(isolate);
            }
            task->callback_.Get(isolate)->Call(context, context->Global(), argc, argv);
        } else {
            task->callback_.Get(isolate)->Call(context, context->Global(), 0, nullptr);
        }
        thiz->nesting = 0;


    }
    thiz->bufferFull.notify_one();
    return 1;
}