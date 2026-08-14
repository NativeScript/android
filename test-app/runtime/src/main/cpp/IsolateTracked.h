#ifndef TEST_APP_ISOLATETRACKED_H
#define TEST_APP_ISOLATETRACKED_H

#include "v8.h"

namespace tns {

/*
 * Base for self-owned native objects whose lifetime is bound to a single JS
 * object through a weak handle (URL, URLSearchParams, URLPattern).
 *
 * An instance dies in exactly two places: the GC finalizer, or SweepAll at
 * runtime teardown. The sweep is the point of this class -- V8 does not run
 * weak callbacks when an isolate is disposed, so every instance still alive
 * when a runtime goes away (a worker shutting down, say) would otherwise leak
 * its native state, and URLPattern would leak its compiled v8::Global handles
 * with it.
 *
 * Never delete a bound instance directly: both deletion paths own the registry
 * bookkeeping. Subclasses only need a destructor that releases their own
 * state; it runs while the isolate is still alive, so it may reset v8::Global
 * handles, but it must not create new ones.
 */
class IsolateTracked {
 public:
    virtual ~IsolateTracked() = default;

    void BindFinalizer(v8::Isolate* isolate, const v8::Local<v8::Object>& object);

    /*
     * Deletes every instance the GC never got to. Runs on the runtime's own
     * thread while the isolate is alive.
     */
    static void SweepAll(v8::Isolate* isolate);

 private:
    static void Finalizer(const v8::WeakCallbackInfo<IsolateTracked>& data);

    v8::Global<v8::Object> weakHandle_;
};

}  // namespace tns

#endif  // TEST_APP_ISOLATETRACKED_H
