#include "ArgConverter.h"
#include "ObjectManager.h"
#include "Util.h"
#include "V8StringConstants.h"
#include "NativeScriptException.h"
#include "NumericCasts.h"
#include "Runtime.h"
#include "V8GlobalHelpers.h"
#include "NativeScriptAssert.h"
#include <sstream>

using namespace v8;
using namespace std;
using namespace tns;

void ArgConverter::Init(Local<Context> context) {
    Isolate* isolate = context->GetIsolate();
    auto cache = GetTypeLongCache(isolate);

    auto ft = FunctionTemplate::New(isolate, ArgConverter::NativeScriptLongFunctionCallback);
    ft->SetClassName(V8StringConstants::GetLongNumber(isolate));
    ft->InstanceTemplate()->Set(V8StringConstants::GetValueOf(isolate), FunctionTemplate::New(isolate, ArgConverter::NativeScriptLongValueOfFunctionCallback));
    ft->InstanceTemplate()->Set(V8StringConstants::GetToString(isolate), FunctionTemplate::New(isolate, ArgConverter::NativeScriptLongToStringFunctionCallback));
    cache->LongNumberCtorFunc = new Persistent<Function>(isolate, ft->GetFunction(context).ToLocalChecked());

    auto nanObject = Number::New(isolate, numeric_limits<double>::quiet_NaN()).As<NumberObject>();
    cache->NanNumberObject = new Persistent<NumberObject>(isolate, nanObject);
}

void ArgConverter::NativeScriptLongValueOfFunctionCallback(const v8::FunctionCallbackInfo<Value>& args) {
    try {
        args.GetReturnValue().Set(numeric_limits<double>::quiet_NaN());
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
        auto context = isolate->GetCurrentContext();
        Local<Value> result;
        if (args.This()->Get(context, V8StringConstants::GetValue(isolate)).ToLocal(&result)) {
            args.GetReturnValue().Set(result);
        } else {
            args.GetReturnValue().SetUndefined();
        }
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
        auto context = isolate->GetCurrentContext();
        thiz->SetPrototype(context, Local<NumberObject>::New(isolate, *cache->NanNumberObject));
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

Local<Array> ArgConverter::ConvertJavaArgsToJsArgs(Local<Context> context, jobjectArray args) {
    JEnv env;

    int argc = env.GetArrayLength(args) / 3;
    auto isolate = context->GetIsolate();
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

        arr->Set(context, i, jsArg);
    }

    return arr;
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
        auto context = isolate->GetCurrentContext();
        convertedValue = Local<Function>::New(isolate, *cache->LongNumberCtorFunc)->CallAsConstructor(context, 1, &strValue).ToLocalChecked();
    }

    return convertedValue;
}

int64_t ArgConverter::ConvertToJavaLong(Isolate* isolate, const Local<Value>& value) {
    assert(!value.IsEmpty());

    auto obj = Local<Object>::Cast(value);

    assert(!obj.IsEmpty());

    auto context = isolate->GetCurrentContext();
    Local<Value> temp;
    bool success = obj->Get(context, V8StringConstants::GetValue(isolate)).ToLocal(&temp);
    assert(success && !temp.IsEmpty());
    auto valueProp = temp.As<Object>();

    string num = ConvertToString(valueProp->ToString(context).ToLocalChecked());

    int64_t longValue = atoll(num.c_str());

    return longValue;
}

ArgConverter::TypeLongOperationsCache* ArgConverter::GetTypeLongCache(v8::Isolate* isolate) {
    TypeLongOperationsCache* cache;
    auto itFound = s_type_long_operations_cache.find(isolate);
    if (itFound == s_type_long_operations_cache.end()) {
        cache = new TypeLongOperationsCache;
        s_type_long_operations_cache.emplace(isolate, cache);
    } else {
        cache = itFound->second;
    }

    return cache;
}


u16string ArgConverter::ConvertToUtf16String(const v8::Local<String>& s) {
    if (s.IsEmpty()) {
        return {};
    } else {
        auto str = ConvertToString(s);
        auto utf16str =  Util::ConvertFromUtf8ToUtf16(str);

        return utf16str;
    }
}



void ArgConverter::onDisposeIsolate(Isolate* isolate) {
    auto itFound = s_type_long_operations_cache.find(isolate);
    if (itFound != s_type_long_operations_cache.end()) {
        delete itFound->second;
        s_type_long_operations_cache.erase(itFound);
    }
}

robin_hood::unordered_map<Isolate*, ArgConverter::TypeLongOperationsCache*> ArgConverter::s_type_long_operations_cache;