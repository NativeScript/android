#include "ErrorEvents.h"

#include "ArgConverter.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "Runtime.h"

using namespace std;
using namespace tns;
using namespace v8;

/*
 * Non-throwing runtime lookup, safe from V8 callbacks that may fire while a
 * runtime is being torn down (Runtime::GetRuntime throws in that window).
 */
static Runtime* GetRuntimeOrNull(Isolate* isolate) {
    return static_cast<Runtime*>(
            isolate->GetData((uint32_t) Runtime::IsolateData::RUNTIME));
}

/*
 * Native function handed to the bootstrap IIFE as `nativeReportFatal(error,
 * stackString)`. It runs the terminal tail (shim + log) WITHOUT re-dispatching
 * an event: reportError and listener-thrown errors have already gone through
 * JS dispatch, so dispatching again here would recurse.
 */
static void NativeReportFatalCallback(const FunctionCallbackInfo<Value>& info) {
    auto isolate = info.GetIsolate();
    Local<Value> error = info.Length() > 0 ? info[0]
                                           : Undefined(isolate).As<Value>();
    string stack;
    if (info.Length() > 1 && info[1]->IsString()) {
        stack = ArgConverter::ConvertToString(info[1].As<String>());
    }
    NativeScriptException::ReportFatalTail(isolate, error, stack);
}

void ErrorEvents::Init(Local<Context> context) {
    /*
     * WHATWG error-events layer, layered on top of the generic event
     * primitives installed by Events::Init and ported from the iOS runtime.
     * Plain (module-free) script, strict inside the IIFE, ES5-ish so it never
     * depends on other runtime extensions. The IIFE is invoked with two
     * arguments - the internal EventTarget backing the global (so native
     * dispatch survives app code overwriting globalThis.dispatchEvent) and
     * the native nativeReportFatal(error, stack) function that runs the
     * terminal tail - and returns three closures bound to that backing store.
     * ErrorEvent/PromiseRejectionEvent subclass the Event captured off
     * globalThis at init time, which runs before any user code.
     */
    auto source = R"js(
    (function (globalTarget, nativeReportFatal) {
      "use strict";
      var g = globalThis;
      var Event = g.Event;

      function ErrorEvent(type, opts) {
        opts = opts || {};
        Event.call(this, type, opts);
        this.message = opts.message !== undefined ? String(opts.message) : "";
        this.filename = opts.filename !== undefined ? String(opts.filename) : "";
        this.lineno = opts.lineno !== undefined ? (opts.lineno | 0) : 0;
        this.colno = opts.colno !== undefined ? (opts.colno | 0) : 0;
        this.error = opts.error !== undefined ? opts.error : null;
      }
      ErrorEvent.prototype = Object.create(Event.prototype);
      ErrorEvent.prototype.constructor = ErrorEvent;

      function PromiseRejectionEvent(type, opts) {
        opts = opts || {};
        Event.call(this, type, opts);
        this.promise = opts.promise;
        this.reason = opts.reason;
      }
      PromiseRejectionEvent.prototype = Object.create(Event.prototype);
      PromiseRejectionEvent.prototype.constructor = PromiseRejectionEvent;

      // A listener that throws must not stop other listeners: route the thrown
      // value to the native fatal tail instead of ever recursively dispatching
      // another `error` event from inside dispatch.
      globalTarget._installListenerErrorReporter(function (e) {
        try { nativeReportFatal(e, (e && e.stack) || ""); } catch (ignored) {}
      });

      g.reportError = function (e) {
        if (arguments.length === 0) {
          throw new TypeError("Failed to execute 'reportError': 1 argument required, but only 0 present.");
        }
        var ev = new ErrorEvent("error", {
          message: (e && e.message !== undefined && e.message !== null) ? String(e.message) : String(e),
          error: e,
          cancelable: true
        });
        if (globalTarget.dispatchEvent(ev)) {
          nativeReportFatal(e, (e && e.stack) || "");
        }
      };

      g.ErrorEvent = ErrorEvent;
      g.PromiseRejectionEvent = PromiseRejectionEvent;

      // Closures called by C++. They never look up globalThis.dispatchEvent,
      // so they keep working even if app code overwrites it.
      function dispatchErrorEvent(error, message, stack) {
        var ev = new ErrorEvent("error", {
          message: message !== undefined && message !== null ? String(message) : "",
          error: error,
          cancelable: true
        });
        globalTarget.dispatchEvent(ev);
        return ev.defaultPrevented;
      }
      function dispatchUnhandledRejection(promise, reason) {
        var ev = new PromiseRejectionEvent("unhandledrejection", {
          promise: promise,
          reason: reason,
          cancelable: true
        });
        globalTarget.dispatchEvent(ev);
        return ev.defaultPrevented;
      }
      function dispatchRejectionHandled(promise, reason) {
        var ev = new PromiseRejectionEvent("rejectionhandled", {
          promise: promise,
          reason: reason,
          cancelable: false
        });
        globalTarget.dispatchEvent(ev);
      }

      return [dispatchErrorEvent, dispatchUnhandledRejection, dispatchRejectionHandled];
    })
    )js";

    auto isolate = context->GetIsolate();
    auto runtime = GetRuntimeOrNull(isolate);
    if (runtime == nullptr) {
        throw NativeScriptException("ErrorEvents::Init: no runtime for isolate");
    }
    if (runtime->GlobalEventTarget().IsEmpty()) {
        throw NativeScriptException("ErrorEvents::Init: Events::Init must run first");
    }
    Local<Object> globalTarget = runtime->GlobalEventTarget().Get(isolate);

    Local<Script> script;
    if (!Script::Compile(context, ArgConverter::ConvertToV8String(isolate, source))
                 .ToLocal(&script)) {
        throw NativeScriptException("ErrorEvents::Init: failed to compile the error-events bootstrap");
    }

    Local<Value> result;
    if (!script->Run(context).ToLocal(&result) || !result->IsFunction()) {
        throw NativeScriptException("ErrorEvents::Init: failed to evaluate the error-events bootstrap");
    }

    Local<Function> nativeReportFatal;
    if (!Function::New(context, NativeReportFatalCallback).ToLocal(&nativeReportFatal)) {
        throw NativeScriptException("ErrorEvents::Init: failed to create nativeReportFatal");
    }

    Local<Value> installArgs[] = {globalTarget, nativeReportFatal};
    Local<Value> iifeResult;
    if (!result.As<Function>()
                 ->Call(context, context->Global(), 2, installArgs)
                 .ToLocal(&iifeResult) ||
        !iifeResult->IsArray()) {
        throw NativeScriptException("ErrorEvents::Init: the error-events bootstrap did not return the dispatch closures");
    }

    auto closures = iifeResult.As<Array>();
    Local<Value> errorFn, rejectionFn, handledFn;
    if (!closures->Get(context, 0).ToLocal(&errorFn) || !errorFn->IsFunction() ||
        !closures->Get(context, 1).ToLocal(&rejectionFn) || !rejectionFn->IsFunction() ||
        !closures->Get(context, 2).ToLocal(&handledFn) || !handledFn->IsFunction()) {
        throw NativeScriptException("ErrorEvents::Init: unexpected dispatch closures");
    }

    runtime->DispatchErrorEventFunc().Reset(isolate, errorFn.As<Function>());
    runtime->DispatchUnhandledRejectionFunc().Reset(isolate, rejectionFn.As<Function>());
    runtime->DispatchRejectionHandledFunc().Reset(isolate, handledFn.As<Function>());
}

bool ErrorEvents::DispatchError(Isolate* isolate, Local<Value> error,
                                const string& messageString,
                                const string& stack) {
    auto runtime = GetRuntimeOrNull(isolate);
    if (runtime == nullptr || runtime->DispatchErrorEventFunc().IsEmpty()) {
        return false;
    }

    auto context = isolate->GetCurrentContext();
    auto dispatch = runtime->DispatchErrorEventFunc().Get(isolate);
    Local<Value> args[] = {error,
                           ArgConverter::ConvertToV8String(isolate, messageString),
                           ArgConverter::ConvertToV8String(isolate, stack)};
    Local<Value> result;
    TryCatch tc(isolate);
    bool success = dispatch->Call(context, context->Global(), 3, args).ToLocal(&result);
    if (tc.HasCaught()) {
        DEBUG_WRITE_FORCE("ErrorEvents: exception while dispatching `error` event");
        return false;
    }
    return success && !result.IsEmpty() && result->BooleanValue(isolate);
}

bool ErrorEvents::DispatchUnhandledRejection(Isolate* isolate,
                                             Local<Promise> promise,
                                             Local<Value> reason) {
    auto runtime = GetRuntimeOrNull(isolate);
    if (runtime == nullptr || runtime->DispatchUnhandledRejectionFunc().IsEmpty()) {
        return false;
    }

    auto context = isolate->GetCurrentContext();
    auto dispatch = runtime->DispatchUnhandledRejectionFunc().Get(isolate);
    Local<Value> args[] = {promise, reason};
    Local<Value> result;
    TryCatch tc(isolate);
    bool success = dispatch->Call(context, context->Global(), 2, args).ToLocal(&result);
    if (tc.HasCaught()) {
        DEBUG_WRITE_FORCE("ErrorEvents: exception while dispatching `unhandledrejection` event");
        return false;
    }
    return success && !result.IsEmpty() && result->BooleanValue(isolate);
}

void ErrorEvents::DispatchRejectionHandled(Isolate* isolate,
                                           Local<Promise> promise,
                                           Local<Value> reason) {
    auto runtime = GetRuntimeOrNull(isolate);
    if (runtime == nullptr || runtime->DispatchRejectionHandledFunc().IsEmpty()) {
        return;
    }

    auto context = isolate->GetCurrentContext();
    auto dispatch = runtime->DispatchRejectionHandledFunc().Get(isolate);
    Local<Value> args[] = {promise, reason};
    Local<Value> result;
    TryCatch tc(isolate);
    if (!dispatch->Call(context, context->Global(), 2, args).ToLocal(&result) &&
        tc.HasCaught()) {
        DEBUG_WRITE_FORCE("ErrorEvents: exception while dispatching `rejectionhandled` event");
    }
}
