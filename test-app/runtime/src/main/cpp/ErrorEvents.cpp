#include "ErrorEvents.h"

#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "Runtime.h"

using namespace std;
using namespace tns;
using namespace v8;

/*
 * Native function handed to internal/error-events.js as `nativeReportFatal(error,
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
    auto isolate = v8::Isolate::GetCurrent();
    auto runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        throw NativeScriptException("ErrorEvents::Init: no runtime for isolate");
    }
    if (runtime->GlobalEventTarget().IsEmpty()) {
        throw NativeScriptException("ErrorEvents::Init: Events::Init must run first");
    }

    Local<Function> nativeReportFatal;
    if (!Function::New(context, NativeReportFatalCallback).ToLocal(&nativeReportFatal)) {
        throw NativeScriptException("ErrorEvents::Init: failed to create nativeReportFatal");
    }

    Local<Object> binding = Object::New(isolate);
    binding->Set(context, ArgConverter::ConvertToV8String(isolate, "globalTarget"),
                 runtime->GlobalEventTarget().Get(isolate));
    binding->Set(context, ArgConverter::ConvertToV8String(isolate, "nativeReportFatal"),
                 nativeReportFatal);

    Local<Value> result;
    if (!BuiltinLoader::RunBuiltin(context, BuiltinId::kErrorEvents, binding).ToLocal(&result) ||
        !result->IsArray()) {
        throw NativeScriptException("ErrorEvents::Init: the error-events bootstrap did not return the dispatch closures");
    }

    auto closures = result.As<Array>();
    Local<Value> errorFn, rejectionFn, handledFn, nativeUncaughtFn;
    if (!closures->Get(context, 0).ToLocal(&errorFn) || !errorFn->IsFunction() ||
        !closures->Get(context, 1).ToLocal(&rejectionFn) || !rejectionFn->IsFunction() ||
        !closures->Get(context, 2).ToLocal(&handledFn) || !handledFn->IsFunction() ||
        !closures->Get(context, 3).ToLocal(&nativeUncaughtFn) || !nativeUncaughtFn->IsFunction()) {
        throw NativeScriptException("ErrorEvents::Init: unexpected dispatch closures");
    }

    runtime->DispatchErrorEventFunc().Reset(isolate, errorFn.As<Function>());
    runtime->DispatchUnhandledRejectionFunc().Reset(isolate, rejectionFn.As<Function>());
    runtime->DispatchRejectionHandledFunc().Reset(isolate, handledFn.As<Function>());
    runtime->DispatchNativeUncaughtErrorFunc().Reset(isolate, nativeUncaughtFn.As<Function>());
}

bool ErrorEvents::DispatchError(Isolate* isolate, Local<Value> error,
                                const string& messageString,
                                const string& stack) {
    auto runtime = Runtime::TryGetRuntime(isolate);
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
    auto runtime = Runtime::TryGetRuntime(isolate);
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

bool ErrorEvents::DispatchNativeUncaughtError(Isolate* isolate,
                                              Local<Value> error,
                                              const string& messageString,
                                              const string& stack) {
    auto runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr || runtime->DispatchNativeUncaughtErrorFunc().IsEmpty()) {
        return false;
    }

    auto context = isolate->GetCurrentContext();
    auto dispatch = runtime->DispatchNativeUncaughtErrorFunc().Get(isolate);
    Local<Value> args[] = {error,
                           ArgConverter::ConvertToV8String(isolate, messageString),
                           ArgConverter::ConvertToV8String(isolate, stack)};
    Local<Value> result;
    TryCatch tc(isolate);
    bool success = dispatch->Call(context, context->Global(), 3, args).ToLocal(&result);
    if (tc.HasCaught()) {
        DEBUG_WRITE_FORCE("ErrorEvents: exception while dispatching `nativeuncaughterror` event");
        return false;
    }
    return success && !result.IsEmpty() && result->BooleanValue(isolate);
}

void ErrorEvents::DispatchRejectionHandled(Isolate* isolate,
                                           Local<Promise> promise,
                                           Local<Value> reason) {
    auto runtime = Runtime::TryGetRuntime(isolate);
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
