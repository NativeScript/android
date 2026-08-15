#include "NativeScriptException.h"
#include "NativeScriptAssert.h"
#include "JSONObjectHelper.h"
#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "RuntimeState.h"
#include "robin_hood.h"
#include <sstream>
#include <string>

using namespace v8;
using namespace tns;

namespace {
// The compiled JS->org.json serializer, per runtime; see RuntimeState.h.
struct SerializeFuncState {
    Persistent<Function>* func = nullptr;

    ~SerializeFuncState() {
        delete func;
    }
};
}  // namespace

void JSONObjectHelper::RegisterFromFunction(Isolate *isolate, Local<Value>& jsonObject) {
    if (!jsonObject->IsFunction()) {
        return;
    }

    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);

    Local<Context> context = isolate->GetCurrentContext();
    Context::Scope context_scope(context);

    Local<Function> jsonObjectFunc =  jsonObject.As<Function>();
    auto fromKey = ArgConverter::ConvertToV8String(isolate, "from");
    if (jsonObjectFunc->Has(context, fromKey).FromMaybe(false)) {
        return;
    }

    Persistent<Function>* serializeFunc = GetSerializeFunc(context);
    if (serializeFunc == nullptr) {
        return;
    }

    Local<External> extData = External::New(isolate, serializeFunc, v8::kExternalPointerTypeTagDefault);
    Local<Function> fromFunc;
    bool ok = FunctionTemplate::New(isolate, ConvertCallbackStatic, extData)->GetFunction(context).ToLocal(&fromFunc);
    NS_DCHECK(ok);
    jsonObjectFunc->Set(context, fromKey, fromFunc);
}

void JSONObjectHelper::ConvertCallbackStatic(const FunctionCallbackInfo<Value>& info) {
    try {
        Local<External> extData = info.Data().As<External>();
        auto poSerializeFunc = reinterpret_cast<Persistent<Function>*>(extData->Value(v8::kExternalPointerTypeTagDefault));
        Isolate* isolate = info.GetIsolate();
        Local<Function> serializeFunc = poSerializeFunc->Get(isolate);

        if (info.Length() < 1) {
            NativeScriptException nsEx(std::string("The \"from\" function expects one parameter"));
            nsEx.ReThrowToV8();
            return;
        }

        Local<Context> context = isolate->GetCurrentContext();

        Local<Value> args[] = { info[0] };
        Local<Value> result;
        TryCatch tc(isolate);
        if (!serializeFunc->Call(context, Undefined(isolate), 1, args).ToLocal(&result)) {
            throw NativeScriptException(tc, "Error serializing to JSONObject");
        }

        info.GetReturnValue().Set(result);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        std::stringstream ss;
        ss << "Error: c++ exception: " << e.what() << std::endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

/*
 * The JS->org.json serializer is shared by every `from` function created on
 * this isolate; it holds no per-call state, so it is compiled once and kept
 * alive for the isolate's lifetime.
 */
Persistent<Function>* JSONObjectHelper::GetSerializeFunc(Local<Context> context) {
    Isolate* isolate = v8::Isolate::GetCurrent();

    auto* state = RuntimeState::For<SerializeFuncState>(isolate);
    if (state == nullptr) {
        return nullptr;
    }
    if (state->func != nullptr) {
        return state->func;
    }

    Local<Value> result;
    if (!BuiltinLoader::RunBuiltin(context, BuiltinId::kJsonHelper).ToLocal(&result) ||
            !result->IsFunction()) {
        return nullptr;
    }

    state->func = new Persistent<Function>(isolate, result.As<Function>());

    return state->func;
}

