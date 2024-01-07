#include "V8GlobalHelpers.h"
#include "ArgConverter.h"
#include "CallbackHandlers.h"
#include "include/v8.h"
#include "JEnv.h"
#include "NativeScriptException.h"
#include <sstream>
#include "robin_hood.h"

using namespace v8;
using namespace std;

static robin_hood::unordered_map<v8::Isolate*, v8::Persistent<v8::Function>*> isolateToPersistentSmartJSONStringify = robin_hood::unordered_map<v8::Isolate*, v8::Persistent<v8::Function>*>();

Local<Function> GetSmartJSONStringifyFunction(Isolate* isolate) {
    auto it = isolateToPersistentSmartJSONStringify.find(isolate);
    if (it != isolateToPersistentSmartJSONStringify.end()) {
        auto smartStringifyPersistentFunction = it->second;

        return smartStringifyPersistentFunction->Get(isolate);
    }

    string smartStringifyFunctionScript =
        "(function () {\n"
        "    function smartStringify(object) {\n"
        "        const seen = [];\n"
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

    isolateToPersistentSmartJSONStringify.emplace(isolate, smartStringifyPersistentFunction);

    return smartStringifyPersistentFunction->Get(isolate);
}

std::string tns::JsonStringifyObject(Isolate* isolate, v8::Local<v8::Object> value, bool handleCircularReferences) {
    if (value.IsEmpty()) {
        return "";
    }

    auto context = isolate->GetCurrentContext();
    if (handleCircularReferences) {
        Local<Function> smartJSONStringifyFunction = GetSmartJSONStringifyFunction(isolate);

        if (!smartJSONStringifyFunction.IsEmpty()) {
            if (value->IsObject()) {
                v8::Local<v8::Value> resultValue;
                v8::TryCatch tc(isolate);

                Local<Value> args[] = { value };
                auto success = smartJSONStringifyFunction
                        ->Call(context, Undefined(isolate), 1, args)
                        .ToLocal(&resultValue);

                if (success && !tc.HasCaught()) {
                    auto res = resultValue.As<v8::String>();
                    return ArgConverter::ConvertToString(res);
                }
            }
        }
    }

    v8::Local<v8::String> resultString;
    v8::TryCatch tc(isolate);
    auto success = v8::JSON::Stringify(context, value).ToLocal(&resultString);

    if (!success && tc.HasCaught()) {
        throw NativeScriptException(tc);
    }

    return ArgConverter::ConvertToString(resultString);
}

bool tns::V8GetPrivateValue(Isolate* isolate, const Local<Object>& obj, const Local<String>& propName, Local<Value>& out) {
    auto privateKey = Private::ForApi(isolate, propName);

    auto context = obj->CreationContext();
    auto hasPrivate = obj->HasPrivate(context, privateKey);

    if (hasPrivate.IsNothing()) {
        stringstream ss;
        ss << "Failed to Get Private Value for prop: " << ArgConverter::ConvertToString(propName).c_str() << endl;
        throw tns::NativeScriptException(ss.str());
    }

    if (!hasPrivate.FromMaybe(false)) {
        return false;
    }

    auto res = obj->GetPrivate(context, privateKey);

    if (res.IsEmpty()) {
        stringstream ss;
        ss << "Failed to Get Private Value for prop: " << ArgConverter::ConvertToString(propName).c_str() << endl;
        throw tns::NativeScriptException(ss.str());
    }

    return res.ToLocal(&out);
}

bool tns::V8SetPrivateValue(Isolate* isolate, const Local<Object>& obj, const Local<String>& propName, const Local<Value>& value) {
    auto privateKey = Private::ForApi(isolate, propName);
    auto context = obj->CreationContext();
    auto res = obj->SetPrivate(context, privateKey, value);

    if (res.IsNothing()) {
        stringstream ss;
        ss << "Failed to Set Private Value for prop: " << ArgConverter::ConvertToString(propName).c_str() << endl;
        throw tns::NativeScriptException(ss.str());
    }

    return res.FromMaybe(false);
}

void tns::V8GlobalHelpers::onDisposeIsolate(Isolate* isolate) {
    isolateToPersistentSmartJSONStringify.erase(isolate);
}
