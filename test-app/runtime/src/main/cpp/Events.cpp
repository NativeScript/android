#include "Events.h"

#include "BuiltinLoader.h"
#include "NativeScriptException.h"
#include "Runtime.h"

using namespace std;
using namespace tns;
using namespace v8;

void Events::Init(Local<Context> context) {
    auto isolate = v8::Isolate::GetCurrent();
    auto runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        throw NativeScriptException("Events::Init: no runtime for isolate");
    }

    Local<Value> result;
    if (!BuiltinLoader::RunBuiltin(context, BuiltinId::kEvents).ToLocal(&result) ||
        !result->IsObject()) {
        throw NativeScriptException("Events::Init: the event-primitives bootstrap did not return the backing target");
    }

    runtime->GlobalEventTarget().Reset(isolate, result.As<Object>());

    // AbortController/AbortSignal (internal/abort-signal.js) build directly on
    // the event primitives installed above; no native binding, no export.
    Local<Value> abortResult;
    if (!BuiltinLoader::RunBuiltin(context, BuiltinId::kAbortSignal).ToLocal(&abortResult)) {
        throw NativeScriptException("Events::Init: the abort-signal bootstrap failed");
    }
}
