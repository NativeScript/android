#ifndef ERROREVENTS_H_
#define ERROREVENTS_H_

#include <string>

#include "v8.h"

namespace tns {

class ErrorEvents {
public:
    /*
     * Installs the WHATWG error-events layer on top of the generic event
     * primitives: the ErrorEvent and PromiseRejectionEvent constructors
     * (subclassing the Event installed by Events::Init, captured off
     * globalThis at init time) and global reportError. Evaluated once per
     * isolate during PrepareV8Runtime, right after Events::Init, for both the
     * main and worker isolates. The bootstrap IIFE is invoked with the backing
     * event target (Runtime::GlobalEventTarget()) and the native
     * nativeReportFatal(error, stack) function, and returns three closures
     * bound to the internal listener store; they are stashed on the Runtime
     * so native dispatch survives app code overwriting
     * globalThis.dispatchEvent.
     */
    static void Init(v8::Local<v8::Context> context);

    /*
     * Dispatches the cancelable `error` ErrorEvent through the JS listener
     * store. Returns true when a listener called preventDefault(). A dispatch
     * that itself throws is logged and treated as unprevented so an error is
     * never lost.
     */
    static bool DispatchError(v8::Isolate* isolate, v8::Local<v8::Value> error,
                              const std::string& messageString,
                              const std::string& stack);

    /*
     * Dispatches the cancelable `unhandledrejection` PromiseRejectionEvent
     * through the JS listener store. Returns true when a listener called
     * preventDefault(). Never re-dispatches on failure: a dispatch that itself
     * throws is logged and treated as unprevented so the error is never lost.
     */
    static bool DispatchUnhandledRejection(v8::Isolate* isolate,
                                           v8::Local<v8::Promise> promise,
                                           v8::Local<v8::Value> reason);

    /*
     * Fires the non-cancelable `rejectionhandled` PromiseRejectionEvent for a
     * late-attached handler, carrying the original rejection reason.
     */
    static void DispatchRejectionHandled(v8::Isolate* isolate,
                                         v8::Local<v8::Promise> promise,
                                         v8::Local<v8::Value> reason);
};

}  // namespace tns

#endif /* ERROREVENTS_H_ */
