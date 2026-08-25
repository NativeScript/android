#include "Events.h"

#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "NativeScriptException.h"
#include "Runtime.h"

using namespace std;
using namespace tns;
using namespace v8;

void Events::Init(Local<Context> context) {
    // The builtin installs Event/EventTarget and the global EventTarget
    // methods; its exports carry the internal EventTarget instance backing the
    // global (cached here so native dispatch survives app code overwriting
    // globalThis.dispatchEvent) and the CustomEvent interface the lazy-global
    // tier places. Run through GetExports so that tier's read shares this run.
    auto isolate = v8::Isolate::GetCurrent();
    auto runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        throw NativeScriptException("Events::Init: no runtime for isolate");
    }

    Local<Object> exports;
    if (!BuiltinLoader::GetExports(context, BuiltinId::kEvents, nullptr).ToLocal(&exports)) {
        throw NativeScriptException("Events::Init: the event-primitives bootstrap failed");
    }

    Local<Value> globalEventTarget;
    if (!exports->Get(context, ArgConverter::ConvertToV8String(isolate, "globalEventTarget"))
                 .ToLocal(&globalEventTarget) ||
        !globalEventTarget->IsObject()) {
        throw NativeScriptException("Events::Init: the event-primitives bootstrap did not return the backing target");
    }

    runtime->GlobalEventTarget().Reset(isolate, globalEventTarget.As<Object>());

    // AbortController/AbortSignal (internal/abort-signal.js) build directly on
    // the event primitives installed above; the listener-mutation hook key for
    // its GC-liveness accounting comes from the events builtin's exports, via
    // require("internal/events").
    Local<Value> abortResult;
    if (!BuiltinLoader::RunBuiltin(context, BuiltinId::kAbortSignal).ToLocal(&abortResult)) {
        throw NativeScriptException("Events::Init: the abort-signal bootstrap failed");
    }
}
