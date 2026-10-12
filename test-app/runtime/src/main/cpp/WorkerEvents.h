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
     * object, on the parent isolate) and returns whether a handler took
     * ownership of it - either by returning truthy from the `onerror`
     * attribute or by calling preventDefault(). Only primitives cross the
     * isolate boundary, so the event carries no error object. A listener that
     * throws leaves the exception pending for the caller's TryCatch and
     * reports as unhandled. False before Init has run.
     */
    static bool EmitError(v8::Isolate* isolate, v8::Local<v8::Object> receiver,
                          const std::string& message, const std::string& source,
                          const std::string& stackTrace, int lineNumber);
};

}  // namespace tns

#endif /* WORKEREVENTS_H_ */
