#include "NativeScriptException.h"
#include "JSONObjectHelper.h"
#include "ArgConverter.h"
#include <sstream>
#include <string>

using namespace v8;
using namespace tns;

JSONObjectHelper::JSONObjectHelper()
    : m_objectManager(nullptr), m_serializeFunc(nullptr) {
}

void JSONObjectHelper::CreateConvertFunctions(Isolate *isolate, const Local<Object> &global, ObjectManager* objectManager) {
    m_objectManager = objectManager;

    m_serializeFunc = new Persistent<Function>(isolate, CreateSerializeFunc(isolate));

    Local<Context> context = isolate->GetCurrentContext();

    Local<External> extData = External::New(isolate, this);
    Local<Function> fromFunc = FunctionTemplate::New(isolate, ConvertCallbackStatic, extData)->GetFunction(context).ToLocalChecked();

    Local<Function> jsonObjectFunc = global->Get(context, ArgConverter::ConvertToV8String(isolate, "org"))
            .ToLocalChecked().As<Object>()->Get(context, ArgConverter::ConvertToV8String(isolate, "json"))
            .ToLocalChecked().As<Object>()->Get(context, ArgConverter::ConvertToV8String(isolate, "JSONObject"))
            .ToLocalChecked().As<Function>();

    jsonObjectFunc->Set(context, ArgConverter::ConvertToV8String(isolate, "from"), fromFunc);
}

void JSONObjectHelper::ConvertCallbackStatic(const FunctionCallbackInfo<Value>& info) {
    try {
        Local<External> extData = info.Data().As<External>();
        auto thiz = reinterpret_cast<JSONObjectHelper*>(extData->Value());
        thiz->ConvertCallback(info);
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

void JSONObjectHelper::ConvertCallback(const FunctionCallbackInfo<Value>& info) {
    if (info.Length() < 1) {
        NativeScriptException nsEx(std::string("The \"from\" function expects one parameter"));
        nsEx.ReThrowToV8();
        return;
    }

    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    Local<Function> serializeFunc = m_serializeFunc->Get(isolate);
    Local<Value> args[] = { info[0] };
    Local<Value> result;
    TryCatch tc(isolate);
    if (!serializeFunc->Call(context, Undefined(isolate), 1, args).ToLocal(&result)) {
        throw NativeScriptException(tc, "Error serializing to JSONObject");
    }

    info.GetReturnValue().Set(result);
}

Local<Function> JSONObjectHelper::CreateSerializeFunc(Isolate* isolate) {
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

    Local<Context> context = isolate->GetCurrentContext();

    Local<Script> script = Script::Compile(context, ArgConverter::ConvertToV8String(isolate, source)).ToLocalChecked();

    Local<Value> result = script->Run(context).ToLocalChecked();

    Local<Function> serializeFunc = result.As<Function>();

    return serializeFunc;
}
