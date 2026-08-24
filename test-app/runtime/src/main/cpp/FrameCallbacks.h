#ifndef FRAMECALLBACKS_H_
#define FRAMECALLBACKS_H_

#include <jni.h>

#include <cstdint>

#include "v8.h"

namespace tns {

/*
 * requestAnimationFrame(fn) / cancelAnimationFrame(handle): the standard
 * animation-frame surface. Each request registers its own one-shot entry and
 * returns a handle; fn receives a single DOMHighResTimeStamp on the isolate's
 * performance timeline.
 *
 * __postFrameCallback(fn[, delayMillis]) / __removeFrameCallback(fn): the
 * compatibility surface predating requestAnimationFrame. Schedules a JS
 * function for the next display frame, deduping by function: rescheduling a
 * callback that is already pending is a no-op.
 *
 * fn receives two arguments:
 *   fn(frameTimeNanos, performanceMillis)
 * frameTimeNanos is the platform's raw frame time -- CLOCK_MONOTONIC
 * nanoseconds, the System.nanoTime() base -- and performanceMillis is the same
 * instant on this isolate's performance timeline, so it compares directly with
 * performance.now(). Both are exact on either implementation below.
 *
 * Two implementations sit behind the one JS surface:
 *   - AChoreographer (NDK, API 24+), reached through dlsym.
 *   - android.view.Choreographer (com.tns.FrameCallbacks), for API 21-23,
 *     where the NDK API does not exist.
 * Scheduling is per calling thread, so a worker schedules against its own
 * looper. Cancellation on either surface only marks the entry: a posted frame
 * callback cannot be recalled, so the dispatch drops it instead.
 */
class FrameCallbacks {
public:
    static void Init(v8::Isolate* isolate,
                     v8::Local<v8::ObjectTemplate> globalTemplate);

    /*
     * Drops every entry belonging to this isolate. Called during runtime
     * teardown, before the isolate is disposed, so a frame that arrives
     * afterwards finds nothing to run.
     */
    static void RemoveIsolateEntries(v8::Isolate* isolate);

private:
    static void RequestAnimationFrame(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void CancelAnimationFrame(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void PostFrameCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void RemoveFrameCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}  // namespace tns

#endif /* FRAMECALLBACKS_H_ */
