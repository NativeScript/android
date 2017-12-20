#include "V8GlobalHelpers.h"
#include "ArgConverter.h"
#include "CallbackHandlers.h"
#include "include/v8.h"
#include "JEnv.h"
#include "NativeScriptException.h"
#include <sstream>

using namespace v8;
using namespace std;

string tns::ConvertToString(const Local<v8::String>& s) {
    if (s.IsEmpty()) {
        return string();
    } else {
        String::Utf8Value str(s);
        return string(*str);
    }
}

Local<String> tns::JsonStringifyObject(Isolate* isolate, Handle<v8::Value> value) {
    v8::HandleScope scope(isolate);

    if (value.IsEmpty()) {
        return String::Empty(isolate);
    }

    v8::Local<v8::String> resultString;
    v8::TryCatch tc;
    auto success = v8::JSON::Stringify(isolate->GetCurrentContext(), value->ToObject(isolate)).ToLocal(&resultString);

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

