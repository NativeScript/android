#ifndef TEST_APP_TIMERS_H
#define TEST_APP_TIMERS_H

#include <android/looper.h>
#include "v8.h"
#include "ObjectManager.h"
#include "condition_variable"
#include "thread"

namespace tns {
    /**
     * A Timer Task
     * this class is used to store the persistent values and context
     * once Unschedule is called everything is released
     */
    class TimerTask {
    public:
        inline TimerTask(v8::Isolate *isolate, const v8::Local<v8::Context> &context,
                         const v8::Local<v8::Function> &callback, double frequency,
                         bool repeats,
                         const std::shared_ptr<std::vector<std::shared_ptr<v8::Persistent<v8::Value>>>> &args,
                         int id, double startTime)
                : isolate_(isolate), callback_(isolate, callback),
                  frequency_(frequency), repeats_(repeats), args_(args), id_(id),
                  startTime_(startTime) {

        }

        inline double NextTime(double targetTime) {
            if (frequency_ <= 0) {
                return targetTime;
            }
            auto timeDiff = targetTime - startTime_;
            auto div = std::div((long) timeDiff, (long) frequency_);
            return startTime_ + frequency_ * (div.quot + 1);
        }

        inline void Unschedule() {
            callback_.Reset();
            args_.reset();
            isolate_ = nullptr;
            queued_ = false;
        }

        int nestingLevel_ = 0;
        v8::Isolate *isolate_;
        v8::Persistent<v8::Function> callback_;
        std::shared_ptr<std::vector<std::shared_ptr<v8::Persistent<v8::Value>>>> args_;
        bool repeats_ = false;
        /**
         * this helper parameter is used in the following way:
         * task scheduled means queued_ = true
         * this is set to false right before the callback is executed
         * if this is false then it's not on the background thread queue
         */
        bool queued_ = false;
        double frequency_ = 0;
        double dueTime_ = -1;
        double startTime_ = -1;
        int id_;
    };

    struct TimerReference {
        int id;
        double dueTime;
    };

    class Timers {
    public:
        /**
         * Initializes the global functions setTimeout, setInterval, clearTimeout and clearInterval
         * also creates helper threads and binds the timers to the executing thread
         * @param isolate target isolate
         * @param globalObjectTemplate global template
         */
        void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &globalObjectTemplate);

        /**
         * Disposes the timers. This will clear all references and stop all thread.
         * MUST be called in the same thread Init was called
         * This methods blocks until the threads are stopped.
         * This method doesn't need to be called most of the time as it's called on object destruction
         * Reusing this class is not advised
         */
        void Destroy();

        /**
         * Calls Destruct
         */
        ~Timers();

    private:
        static void SetTimeoutCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void SetIntervalCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void SetTimer(const v8::FunctionCallbackInfo<v8::Value> &args, bool repeatable);

        static void ClearTimer(const v8::FunctionCallbackInfo<v8::Value> &args);

        void threadLoop();

        static int PumpTimerLoopCallback(int fd, int events, void *data);

        void addTask(std::shared_ptr<TimerTask> task);

        void removeTask(const std::shared_ptr<TimerTask> &task);

        void removeTask(const int &taskId);

        v8::Isolate *isolate_ = nullptr;
        ALooper *looper_;
        int currentTimerId = 0;
        int nesting = 0;
        // stores the map of timer tasks
        std::map<int, std::shared_ptr<TimerTask>> timerMap_;
        std::vector<std::shared_ptr<TimerReference>> sortedTimers_;
        // sets are faster than vector iteration
        // so we use this to avoid redundant isolate locks and we don't care about the
        // background thread lost cycles
        std::set<int> deletedTimers_;
        int fd_[2];
        std::atomic_bool isBufferFull = ATOMIC_VAR_INIT(false);
        std::condition_variable taskReady;
        std::condition_variable bufferFull;
        std::mutex mutex;
        std::thread watcher_;
        bool stopped = false;
    };

}

#endif //TEST_APP_TIMERS_H
