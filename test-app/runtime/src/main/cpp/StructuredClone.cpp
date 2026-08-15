#include "StructuredClone.h"

#include "NativeScriptAssert.h"

#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "StructuredSerialization.h"

using namespace v8;

namespace tns {

namespace {

/*
 * binding.clone(value, transferArrayOrUndefined): serialize and deserialize in
 * this one isolate, which is what StructuredDeserialize(
 * StructuredSerializeWithTransfer(...)) amounts to when there is no second
 * agent involved.
 */
void CloneCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    Local<Value> value =
            info.Length() > 0 ? info[0] : v8::Undefined(isolate).As<Value>();
    Local<Value> transferList =
            info.Length() > 1 ? info[1] : v8::Undefined(isolate).As<Value>();

    serialization::SerializedValue serialized;
    if (serialized
                .Serialize(isolate, context, value, transferList,
                           serialization::HostObjectPolicy::kReject)
                .IsNothing()) {
        return;
    }

    Local<Value> result;
    if (!serialized.Deserialize(isolate, context).ToLocal(&result)) {
        return;
    }
    info.GetReturnValue().Set(result);
}

}  // namespace

void StructuredClone::Init(Local<Context> context) {
    Isolate* isolate = Isolate::GetCurrent();

    Local<v8::Function> clone;
    bool success = v8::Function::New(context, CloneCallback).ToLocal(&clone);
    NS_DCHECK(success);

    Local<Object> binding = Object::New(isolate);
    success = binding->Set(context,
                           ArgConverter::ConvertToV8String(isolate, "clone"),
                           clone)
                      .FromMaybe(false);
    NS_DCHECK(success);

    Local<Value> result;
    success = BuiltinLoader::RunBuiltin(context, BuiltinId::kStructuredClone,
                                        binding)
                      .ToLocal(&result);
    NS_DCHECK(success);
}

}  // namespace tns
