#include "ArgConverter.h"
#include "ObjectManager.h"
#include "Util.h"
#include "V8StringConstants.h"
#include "NativeScriptException.h"
#include "NumericCasts.h"
#include "Runtime.h"
#include "V8GlobalHelpers.h"
#include <sstream>

using namespace v8;
using namespace std;
using namespace tns;

void ArgConverter::Init(Isolate* isolate) {
    auto cache = GetTypeLongCache(isolate);

    auto ft = FunctionTemplate::New(isolate, ArgConverter::NativeScriptLongFunctionCallback);
    ft->SetClassName(V8StringConstants::GetLongNumber(isolate));
    ft->InstanceTemplate()->Set(V8StringConstants::GetValueOf(isolate), FunctionTemplate::New(isolate, ArgConverter::NativeScriptLongValueOfFunctionCallback));
    ft->InstanceTemplate()->Set(V8StringConstants::GetToString(isolate), FunctionTemplate::New(isolate, ArgConverter::NativeScriptLongToStringFunctionCallback));
    cache->LongNumberCtorFunc = new Persistent<Function>(isolate, ft->GetFunction());

    auto nanObject = Number::New(isolate, numeric_limits<double>::quiet_NaN()).As<NumberObject>();
    cache->NanNumberObject = new Persistent<NumberObject>(isolate, nanObject);
}

void ArgConverter::NativeScriptLongValueOfFunctionCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        args.GetReturnValue().Set(Number::New(isolate, numeric_limits<double>::quiet_NaN()));
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void ArgConverter::NativeScriptLongToStringFunctionCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        args.GetReturnValue().Set(args.This()->Get(V8StringConstants::GetValue(isolate)));
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void ArgConverter::NativeScriptLongFunctionCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        auto thiz = args.This();
        auto cache = GetTypeLongCache(isolate);
        V8SetPrivateValue(isolate, thiz, V8StringConstants::GetJavaLong(isolate), Boolean::New(isolate, true));
        NumericCasts::MarkAsLong(isolate, thiz, args[0]);
        thiz->SetPrototype(Local<NumberObject>::New(isolate, *cache->NanNumberObject));
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

jstring ArgConverter::ObjectToString(jobject object) {
    return (jstring) object;
}

Local<Array> ArgConverter::ConvertJavaArgsToJsArgs(Isolate* isolate, jobjectArray args) {
    JEnv env;

    int argc = env.GetArrayLength(args) / 3;
    Local<Array> arr(Array::New(isolate, argc));

    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    int jArrayIndex = 0;
    for (int i = 0; i < argc; i++) {
        JniLocalRef argTypeIDObj(env.GetObjectArrayElement(args, jArrayIndex++));
        JniLocalRef arg(env.GetObjectArrayElement(args, jArrayIndex++));
        JniLocalRef argJavaClassPath(env.GetObjectArrayElement(args, jArrayIndex++));

        Type argTypeID = (Type) JType::IntValue(env, argTypeIDObj);

        Local<Value> jsArg;
        switch (argTypeID) {
        case Type::Boolean:
            jsArg = Boolean::New(isolate, JType::BooleanValue(env, arg));
            break;
        case Type::Char:
            jsArg = jcharToV8String(isolate, JType::CharValue(env, arg));
            break;
        case Type::Byte:
            jsArg = Number::New(isolate, JType::ByteValue(env, arg));
            break;
        case Type::Short:
            jsArg = Number::New(isolate, JType::ShortValue(env, arg));
            break;
        case Type::Int:
            jsArg = Number::New(isolate, JType::IntValue(env, arg));
            break;
        case Type::Long:
            jsArg = Number::New(isolate, JType::LongValue(env, arg));
            break;
        case Type::Float:
            jsArg = Number::New(isolate, JType::FloatValue(env, arg));
            break;
        case Type::Double:
            jsArg = Number::New(isolate, JType::DoubleValue(env, arg));
            break;
        case Type::String:
            jsArg = jstringToV8String(isolate, (jstring) arg);
            break;
        case Type::JsObject: {
            jint javaObjectID = JType::IntValue(env, arg);
            jsArg = objectManager->GetJsObjectByJavaObject(javaObjectID);

            if (jsArg.IsEmpty()) {
                string argClassName = jstringToString(ObjectToString(argJavaClassPath));
                argClassName = Util::ConvertFromCanonicalToJniName(argClassName);
                jsArg = objectManager->CreateJSWrapper(javaObjectID, argClassName);
            }
            break;
        }
        case Type::Null:
            jsArg = Null(isolate);
            break;
        }

        arr->Set(i, jsArg);
    }

    return arr;
}

std::string ArgConverter::jstringToString(jstring value) {
    if (value == nullptr) {
        return string();
    }

    JEnv env;

    jboolean f = JNI_FALSE;
    auto chars = env.GetStringUTFChars(value, &f);
    string s(chars);
    env.ReleaseStringUTFChars(value, chars);

    return s;
}

Local<Value> ArgConverter::jstringToV8String(Isolate* isolate, jstring value) {
    if (value == nullptr) {
        return Null(isolate);
    }

    JEnv env;
    auto chars = env.GetStringChars(value, NULL);
    auto length = env.GetStringLength(value);
    auto v8String = ConvertToV8String(isolate, chars, length);
    env.ReleaseStringChars(value, chars);

    return v8String;
}

Local<String> ArgConverter::jcharToV8String(Isolate* isolate, jchar value) {
    auto v8String = ConvertToV8String(isolate, &value, 1);
    return v8String;
}

Local<Value> ArgConverter::ConvertFromJavaLong(Isolate* isolate, jlong value) {
    Local<Value> convertedValue;
    long long longValue = value;

    if ((-JS_LONG_LIMIT < longValue) && (longValue < JS_LONG_LIMIT)) {
        convertedValue = Number::New(isolate, longValue);
    } else {
        auto cache = GetTypeLongCache(isolate);
        char strNumber[24];
        sprintf(strNumber, "%lld", longValue);
        Local<Value> strValue = ConvertToV8String(isolate, strNumber);
        convertedValue = Local<Function>::New(isolate, *cache->LongNumberCtorFunc)->CallAsConstructor(1, &strValue);
    }

    return convertedValue;
}

int64_t ArgConverter::ConvertToJavaLong(Isolate* isolate, const Local<Value>& value) {
    assert(!value.IsEmpty());

    auto obj = Local<Object>::Cast(value);

    assert(!obj.IsEmpty());

    auto valueProp = obj->Get(V8StringConstants::GetValue(isolate));

    assert(!valueProp.IsEmpty());

    string num = ConvertToString(valueProp->ToString());

    int64_t longValue = atoll(num.c_str());

    return longValue;
}

ArgConverter::TypeLongOperationsCache* ArgConverter::GetTypeLongCache(v8::Isolate* isolate) {
    TypeLongOperationsCache* cache;
    auto itFound = s_type_long_operations_cache.find(isolate);
    if (itFound == s_type_long_operations_cache.end()) {
        cache = new TypeLongOperationsCache;
        s_type_long_operations_cache.insert(make_pair(isolate, cache));
    } else {
        cache = itFound->second;
    }

    return cache;
}


string ArgConverter::ConvertToString(const v8::Local<String>& s) {
    if (s.IsEmpty()) {
        return string();
    } else {
        String::Utf8Value str(s);
        return string(*str);
    }
}

u16string ArgConverter::ConvertToUtf16String(const v8::Local<String>& s) {
    if (s.IsEmpty()) {
        return u16string();
    } else {
        auto str = ConvertToString(s);
        auto utf16str =  Util::ConvertFromUtf8ToUtf16(str);

        return utf16str;
    }
}

jstring ArgConverter::ConvertToJavaString(const Local<Value>& value) {
    JEnv env;
    String::Value stringValue(value);
    return env.NewString((const jchar*) *stringValue, stringValue.length());
}

Local<String> ArgConverter::ConvertToV8String(Isolate* isolate, const jchar* data, int length) {
    return String::NewFromTwoByte(isolate, (const uint16_t*) data, String::kNormalString, length);
}

Local<String> ArgConverter::ConvertToV8String(Isolate* isolate, const string& s) {
    Local<String> str =	String::NewFromUtf8(isolate, s.c_str(), NewStringType::kNormal, s.length()).ToLocalChecked();
    return str;
}

Local<String> ArgConverter::ConvertToV8String(Isolate* isolate, const char* data, int length) {
    return String::NewFromUtf8(isolate, (const char*) data, String::kNormalString, length);
}

Local<String> ArgConverter::ConvertToV8UTF16String(Isolate* isolate, const string& string) {
    auto utf16str = Util::ConvertFromUtf8ToUtf16(string);

    return ConvertToV8UTF16String(isolate, utf16str);
}

Local<String> ArgConverter::ConvertToV8UTF16String(Isolate* isolate, const u16string& utf16string) {
    return String::NewFromTwoByte(isolate, ((const uint16_t*) utf16string.data()));
}

Local<String> ArgConverter::ConvertToV8UTF16String(v8::Isolate* isolate, const uint16_t* utf16string, int size) {
    return String::NewFromTwoByte(isolate, utf16string, NewStringType::kNormal, size).ToLocalChecked();
}

std::map<Isolate*, ArgConverter::TypeLongOperationsCache*> ArgConverter::s_type_long_operations_cache;