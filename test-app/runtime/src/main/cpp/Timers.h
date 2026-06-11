#ifndef TEST_APP_TIMERS_H
#define TEST_APP_TIMERS_H

#include <jni.h>
#include "v8.h"
#include "ObjectManager.h"
#include "robin_hood.h"

// Define NS_TIMERS_NESTING_CLAMP to enable the HTML-spec/Chromium nesting
// clamp (>=5 nested timer levels -> 4ms minimum delay). Off by default: the
// clamp exists to keep browser pages from spinning the event loop, which is
// rarely the desired behavior in a native app.

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

#ifdef NS_TIMERS_NESTING_CLAMP
        int nestingLevel_ = 0;
#endif
        v8::Isolate *isolate_;
        v8::Persistent<v8::Function> callback_;
        std::shared_ptr<std::vector<std::shared_ptr<v8::Persistent<v8::Value>>>> args_;
        bool repeats_ = false;
        /**
         * this helper parameter is used in the following way:
         * task scheduled means queued_ = true
         * this is set to false right before the callback is executed
         * if this is false then it's not on the message queue to be executed
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
         * also creates the dedicated Java Handler bound to the executing thread's Looper
         * @param isolate target isolate
         * @param globalObjectTemplate global template
         */
        void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &globalObjectTemplate);

        static void InitStatic(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> globalObjectTemplate);

        /**
         * Disposes the timers. This will clear all references and remove all
         * pending timer messages from the Java message queue.
         * MUST be called in the same thread Init was called
         * This method doesn't need to be called most of the time as it's called on object destruction
         * Reusing this class is not advised
         */
        void Destroy();

        /**
         * Calls Destroy
         */
        ~Timers();

        /**
         * Fires the earliest due timer, if any (invoked by Java
         * TimerHandler.handleMessage once per scheduled "due token")
         */
        void FireTimer();

    private:
        static void SetTimeoutCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void SetIntervalCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void SetTimer(const v8::FunctionCallbackInfo<v8::Value> &args, bool repeatable);

        static void ClearTimer(const v8::FunctionCallbackInfo<v8::Value> &args);

        void addTask(std::shared_ptr<TimerTask> task);

        void removeTask(const std::shared_ptr<TimerTask> &task);

        void removeTask(const int &taskId);

        void postTimer(const std::shared_ptr<TimerTask> &task, double now);

        v8::Isolate *isolate_ = nullptr;
        int currentTimerId = 0;
#ifdef NS_TIMERS_NESTING_CLAMP
        int nesting = 0;
#endif
        // stores the map of timer tasks
        robin_hood::unordered_map<int, std::shared_ptr<TimerTask>> timerMap_;
        // scheduled timers sorted by exact (sub-millisecond) dueTime, stable for
        // equal dueTimes. Only ever touched under the isolate lock, no mutex.
        // The Java message queue is millisecond-quantized, so this preserves the
        // relative order of JS timers; each Java message is an anonymous token
        // that fires the front of this list.
        std::vector<TimerReference> sortedTimers_;
        // global ref to the dedicated com.tns.TimerHandler for this isolate's thread
        jobject handler_ = nullptr;
        bool stopped_ = false;

        // process-wide JNI cache, written once on the first Timers::Init (main
        // runtime), which happens-before any worker thread is spawned
        static jclass TIMER_HANDLER_CLASS;
        static jmethodID TIMER_HANDLER_CTOR;
        static jmethodID TIMER_HANDLER_POST;
        static jmethodID TIMER_HANDLER_RELEASE;
    };

}

#endif //TEST_APP_TIMERS_H
