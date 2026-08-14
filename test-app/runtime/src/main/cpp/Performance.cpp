#include "Performance.h"

#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "NativeScriptException.h"
#include "Runtime.h"

using namespace v8;

namespace tns {

namespace {

}  // namespace

void Performance::Init(Local<Context> context) {
    Isolate* isolate = Isolate::GetCurrent();

    /*
     * kThrow so `new performance.now()` throws, as WebIDL operations must;
     * kHasNoSideEffect so the debugger can call it during side-effect-free
     * evaluation.
     */
    Local<v8::Function> now;
    if (!v8::Function::New(context, NowCallback, Local<Value>(), 0,
                           ConstructorBehavior::kThrow,
                           SideEffectType::kHasNoSideEffect)
                 .ToLocal(&now)) {
        throw NativeScriptException("Performance::Init: failed to create now");
    }

    Local<Object> binding = Object::New(isolate);
    binding->Set(context, ArgConverter::ConvertToV8String(isolate, "now"), now)
            .Check();
    binding->Set(context,
                 ArgConverter::ConvertToV8String(isolate, "timeOrigin"),
                 Number::New(isolate, TimeOriginMillis(isolate)))
            .Check();

    Local<Value> result;
    if (!BuiltinLoader::RunBuiltin(context, BuiltinId::kPerformance, binding)
                 .ToLocal(&result)) {
        throw NativeScriptException(
                "Performance::Init: the performance bootstrap failed");
    }
}

double Performance::NowMillis(Isolate* isolate) {
    Runtime* runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        return 0.0;
    }

    return runtime->PerformanceNowMillis();
}

double Performance::TimeOriginMillis(Isolate* isolate) {
    Runtime* runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        return 0.0;
    }

    return runtime->TimeOriginMillis();
}

double Performance::MonotonicNanosToTimelineMillis(Isolate* isolate,
                                                   int64_t nanos) {
    Runtime* runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        return 0.0;
    }

    return (double) nanos / 1e6 - runtime->TimeOriginMonotonicMillis();
}

void Performance::NowCallback(const FunctionCallbackInfo<Value>& info) {
    info.GetReturnValue().Set(NowMillis(info.GetIsolate()));
}

}  // namespace tns
