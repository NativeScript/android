#include "NativeScriptException.h"
#include "JSONObjectHelper.h"
#include "ArgConverter.h"
#include <sstream>
#include <string>

using namespace v8;
using namespace tns;

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

    Persistent<Function>* serializeFunc = new Persistent<Function>(isolate, CreateSerializeFunc(context));
    Local<External> extData = External::New(isolate, serializeFunc);
    Local<Function> fromFunc;
    bool ok = FunctionTemplate::New(isolate, ConvertCallbackStatic, extData)->GetFunction(context).ToLocal(&fromFunc);
    assert(ok);
    jsonObjectFunc->Set(context, fromKey, fromFunc);
}

void JSONObjectHelper::ConvertCallbackStatic(const FunctionCallbackInfo<Value>& info) {
    try {
        Local<External> extData = info.Data().As<External>();
        auto poSerializeFunc = reinterpret_cast<Persistent<Function>*>(extData->Value());
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

Local<Function> JSONObjectHelper::CreateSerializeFunc(Local<Context> context) {
    std::string source =
        "(() => function serialize(data) {"
        "    let store;"
        "    switch (typeof data) {"
        "        case \"string\":"
        "        case \"boolean\":"
        "        case \"number\": {"
        "            return data;"
        "        }"
        "        case \"object\": {"
        "            if (!data) {"
        "                return null;"
        "            }"
        ""
        "            if (data instanceof Date) {"
        "                return data.toJSON();"
        "            }"
        ""
        "            if (Array.isArray(data)) {"
        "                store = new org.json.JSONArray();"
        "                data.forEach((item) => store.put(serialize(item)));"
        "                return store;"
        "            }"
        ""
        "            store = new org.json.JSONObject();"
        "            Object.keys(data).forEach((key) => store.put(key, serialize(data[key])));"
        "            return store;"
        "        }"
        "        default:"
        "            return null;"
        "    }"
        "})()";

    Isolate* isolate = context->GetIsolate();

    Local<Script> script = Script::Compile(context, ArgConverter::ConvertToV8String(isolate, source)).ToLocalChecked();

    Local<Value> result = script->Run(context).ToLocalChecked();

    Local<Function> serializeFunc = result.As<Function>();

    return serializeFunc;
}
