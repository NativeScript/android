#include "V8GlobalHelpers.h"
#include "ArgConverter.h"
#include "CallbackHandlers.h"
#include "include/v8.h"
#include "JEnv.h"
#include "NativeScriptException.h"
#include <sstream>

using namespace v8;
using namespace std;

static std::map<v8::Isolate*, v8::Persistent<v8::Function>*> isolateToPersistentSmartJSONStringify = std::map<v8::Isolate*, v8::Persistent<v8::Function>*>();

string tns::ConvertToString(const Local<v8::String>& s) {
    if (s.IsEmpty()) {
        return string();
    } else {
        String::Utf8Value str(s);
        return string(*str);
    }
}

Local<Function> GetSmartJSONStringifyFunction(Isolate* isolate) {
    auto it = isolateToPersistentSmartJSONStringify.find(isolate);
    if (it != isolateToPersistentSmartJSONStringify.end()) {
        auto smartStringifyPersistentFunction = it->second;

        return smartStringifyPersistentFunction->Get(isolate);
    }

    string smartStringifyFunctionScript =
        "(function () {\n"
        "    function smartStringify(object) {\n"
        "        seen = [];\n"
        "        var replacer = function (key, value) {\n"
        "            if (value != null && typeof value == \"object\") {\n"
        "                if (seen.indexOf(value) >= 0) {\n"
        "                    if (key) {\n"
        "                        return \"[Circular]\";\n"
        "                    }\n"
        "                    return;\n"
        "                }\n"
        "                seen.push(value);\n"
        "            }\n"
        "            return value;\n"
        "        };\n"
        "        return JSON.stringify(object, replacer, 2);\n"
        "    }\n"
        "    return smartStringify;\n"
        "})();";

    auto source = tns::ArgConverter::ConvertToV8String(isolate, smartStringifyFunctionScript);
    auto context = isolate->GetCurrentContext();

    Local<Script> script;
    auto maybeScript = Script::Compile(context, source).ToLocal(&script);

    if (script.IsEmpty()) {
        return Local<Function>();
    }

    Local<Value> result;
    auto maybeResult = script->Run(context).ToLocal(&result);

    if (result.IsEmpty() && !result->IsFunction()) {
        return Local<Function>();
    }

    auto smartStringifyFunction = result.As<Function>();

    auto smartStringifyPersistentFunction = new Persistent<Function>(isolate, smartStringifyFunction);

    isolateToPersistentSmartJSONStringify.insert(std::make_pair(isolate, smartStringifyPersistentFunction));

    return smartStringifyPersistentFunction->Get(isolate);
}

Local<String> tns::JsonStringifyObject(Isolate* isolate, Handle<v8::Value> value) {
    if (value.IsEmpty()) {
        return String::Empty(isolate);
    }

    Local<Function> smartJSONStringifyFunction = GetSmartJSONStringifyFunction(isolate);

    if (!smartJSONStringifyFunction.IsEmpty()) {
        if (value->IsObject()) {
            v8::Local<v8::Value> resultValue;
            v8::TryCatch tc(isolate);

            Local<Value> args[] = { value->ToObject() };
            auto success = smartJSONStringifyFunction->Call(isolate->GetCurrentContext(), Undefined(isolate), 1, args).ToLocal(&resultValue);

            if (success && !tc.HasCaught()) {
                return resultValue->ToString();
            }
        }
    }

    v8::Local<v8::String> resultString;
    v8::TryCatch tc(isolate);
    auto success = v8::JSON::Stringify(isolate->GetCurrentContext(), value->ToObject(isolate), ArgConverter::ConvertToV8String(isolate, "2")).ToLocal(&resultString);

    if (!success && tc.HasCaught()) {
        auto message = tc.Message()->Get();
        resultString = v8::String::Concat(ArgConverter::ConvertToV8String(isolate, "Couldn't convert object to a JSON string: "), message);
    }

    return resultString;
}

jstring tns::ConvertToJavaString(const Local<Value>& value) {
    JEnv env;
    String::Value stringValue(value);
    return env.NewString((const jchar*) *stringValue, stringValue.length());
}

Local<String> tns::ConvertToV8String(const jchar* data, int length) {
    auto isolate = Isolate::GetCurrent();
    return String::NewFromTwoByte(isolate, (const uint16_t*) data, String::kNormalString, length);
}

Local<String> tns::ConvertToV8String(const string& s) {
    auto isolate = Isolate::GetCurrent();
    Local<v8::String> str = String::NewFromUtf8(isolate, s.c_str(), NewStringType::kNormal, s.length()).ToLocalChecked();
    return str;
}

Local<String> tns::ConvertToV8String(const char* data, int length) {
    auto isolate = Isolate::GetCurrent();
    return String::NewFromUtf8(isolate, (const char*) data, String::kNormalString, length);
}

bool tns::V8HasPrivateValue(Isolate* isolate, const Local<Object>& obj, const Local<String>& propName) {

    if (obj.IsEmpty()) {
        return false;
    }

    auto privateKey = Private::ForApi(isolate, propName);

    auto hasPrivate = obj->HasPrivate(isolate->GetCurrentContext(), privateKey);

    if (hasPrivate.IsNothing()) {
        return false;
    }

    if (!hasPrivate.FromMaybe(false)) {
        return false;
    }

    auto res = obj->GetPrivate(isolate->GetCurrentContext(), privateKey);
    if (res.IsEmpty()) {
        return false;
    }

    return true;
}

bool tns::V8GetPrivateValue(Isolate* isolate, const Local<Object>& obj, const Local<String>& propName, Local<Value>& out) {
    auto privateKey = Private::ForApi(isolate, propName);

    auto hasPrivate = obj->HasPrivate(isolate->GetCurrentContext(), privateKey);

    if (hasPrivate.IsNothing()) {
        stringstream ss;
        ss << "Failed to Get Private Value for prop: " << tns::ConvertToString(propName).c_str() << endl;
        throw tns::NativeScriptException(ss.str());
    }

    if (!hasPrivate.FromMaybe(false)) {
        return false;
    }

    auto res = obj->GetPrivate(isolate->GetCurrentContext(), privateKey);

    if (res.IsEmpty()) {
        stringstream ss;
        ss << "Failed to Get Private Value for prop: " << tns::ConvertToString(propName).c_str() << endl;
        throw tns::NativeScriptException(ss.str());
    }

    return res.ToLocal(&out);
}

bool tns::V8SetPrivateValue(Isolate* isolate, const Local<Object>& obj, const Local<String>& propName, const Local<Value>& value) {
    auto privateKey = Private::ForApi(isolate, propName);
    auto res = obj->SetPrivate(isolate->GetCurrentContext(), privateKey, value);

    if (res.IsNothing()) {
        stringstream ss;
        ss << "Failed to Set Private Value for prop: " << tns::ConvertToString(propName).c_str() << endl;
        throw tns::NativeScriptException(ss.str());
    }

    return res.FromMaybe(false);
}
