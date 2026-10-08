#ifndef WORKEREVENTS_H_
#define WORKEREVENTS_H_

#include <memory>
#include <string>

#include "WorkerMessage.h"
#include "v8.h"

namespace tns {

/*
 * The Worker object and the worker global scope as EventTargets. The JS tier
 * (internal/worker-events.js) grafts Worker.prototype onto EventTarget's,
 * defines the handler attributes on both it and the target backing the global
 * scope, and exports the two callouts native delivery goes through.
 */
class WorkerEvents {
public:
    /*
     * Runs the worker-events builtin and caches its callouts for this isolate.
     * Evaluated once per isolate during PrepareV8Runtime, after Events::Init
     * has installed the event primitives it builds on and before
     * ErrorEvents::Init - the ErrorEvent constructor it needs is taken lazily,
     * on the first error delivery.
     */
    static void Init(v8::Local<v8::Context> context);

    /*
     * Builds a MessageEvent out of `message` and dispatches it on `receiver` -
     * the Worker object for worker-to-parent traffic, the global scope's
     * EventTarget for parent-to-worker. A message that cannot be read arrives
     * as a `messageerror` event carrying nothing. A handler that throws leaves
     * the exception pending for the caller's TryCatch, which owns the worker's
     * error chain. No-op before Init has run.
     */
    static void EmitMessage(v8::Isolate* isolate, v8::Local<v8::Object> receiver,
                            const std::shared_ptr<worker::Message>& message);

    /*
     * Dispatches a cancelable `error` ErrorEvent on `receiver` (the Worker
     * object, on the parent isolate). Only primitives cross the isolate
     * boundary, so the event carries no error object; the worker's error is
     * rebuilt from `errorName`, `errorMessage` and `stackTrace`. Returns that
     * error when no handler took ownership of the event, for the caller to
     * report on the parent's global scope, and undefined when one did - either
     * by returning truthy from the `onerror` attribute or by calling
     * preventDefault(). Empty when a listener threw, which leaves the
     * exception pending for the caller's TryCatch, and before Init has run.
     */
    static v8::MaybeLocal<v8::Value> EmitError(v8::Isolate* isolate,
                                               v8::Local<v8::Object> receiver,
                                               const std::string& message,
                                               const std::string& source,
                                               const std::string& stackTrace, int lineNumber,
                                               v8::Local<v8::String> errorName,
                                               v8::Local<v8::String> errorMessage);
};

}  // namespace tns

#endif /* WORKEREVENTS_H_ */
