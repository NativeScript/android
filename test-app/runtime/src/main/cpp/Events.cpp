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
}
