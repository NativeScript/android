#ifndef TEST_APP_TIMERS_H
#define TEST_APP_TIMERS_H

#include <android/looper.h>
#include "v8.h"
#include "ObjectManager.h"
#include "condition_variable"
#include "thread"

namespace tns {
    class TimerTask {
        public:
        TimerTask(v8::Isolate* isolate,const v8::Local<v8::Context>& context,const v8::Local<v8::Function>& callback, double frequency,
                  bool repeats, const std::shared_ptr<std::vector<std::shared_ptr<v8::Persistent<v8::Value>>>>& args, int id, double startTime)
                : isolate_(isolate), context_(isolate, context), callback_(isolate, callback), frequency_(frequency), repeats_(repeats), args_(args), id_(id), startTime_(startTime) {

        }

        double NextTime(double targetTime) {
            if (frequency_ <= 0) {
                return targetTime;
            }
            auto timeDiff = targetTime - startTime_;
            auto div = std::div((long)timeDiff, (long) frequency_);
            return startTime_ + frequency_ * (div.quot + 1);
        }

        void Unschedule() {
            context_.Reset();
            callback_.Reset();
            args_.reset();
            isolate_ = nullptr;
            scheduled_ = false;
            disposed = true;
        }

        int nestingLevel_ = 0;
        v8::Isolate* isolate_;
        v8::Persistent<v8::Context> context_;
        v8::Persistent<v8::Function> callback_;
        std::shared_ptr<std::vector<std::shared_ptr<v8::Persistent<v8::Value>>>> args_;
        bool repeats_;
        bool scheduled_ = false;
        double frequency_;
        double dueTime_;
        double startTime_;
        int id_;
        bool disposed = false;
    };

    struct TimerReference {
        int id;
        double dueTime;
    };

    class Timers {
    public:
        void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &globalObjectTemplate);
        void Destruct(v8::Isolate *isolate);
        ~Timers();

    private:
        static void SetTimeoutCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void SetIntervalCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void SetTimer(const v8::FunctionCallbackInfo<v8::Value> &args, bool repeatable);
        static void ClearTimer(const v8::FunctionCallbackInfo<v8::Value> &args);
        void threadLoop();
        static int PumpTimerLoopCallback(int fd, int events, void* data);
        void addTask(std::shared_ptr<TimerTask> task);
        void removeTask(std::shared_ptr<TimerTask> task);
        void removeTask(const int& taskId);
        bool IsScheduled(const int& id) {
            auto it = timerMap_.find(id);
            return it != timerMap_.end();
        }

        v8::Isolate* isolate_ = nullptr;
        ALooper* looper_;
        int currentTimerId = 0;
        int nesting = 0;
        std::map<int, std::shared_ptr<TimerTask>> timerMap_;
        std::vector<std::shared_ptr<TimerReference>> sortedTimers_;
        int fd_[2];
        std::atomic_bool isBufferFull = ATOMIC_VAR_INIT(false);
        std::condition_variable cv;
        std::condition_variable bufferFull;
        std::mutex mutex;
        std::thread watcher_;
        bool stopped = false;
    };

}

#endif //TEST_APP_TIMERS_H
