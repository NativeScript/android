#include "ArgConverter.h"
#include "JniLocalRef.h"
#include "ObjectManager.h"
#include "Util.h"
#include "V8StringConstants.h"
#include "NativeScriptException.h"
#include "NumericCasts.h"
#include "Runtime.h"
#include "V8GlobalHelpers.h"
#include "NativeScriptAssert.h"
#include "RuntimeState.h"
#include <cstring>
#include <sstream>
#include <vector>

using namespace v8;
using namespace std;
using namespace tns;

void ArgConverter::Init(Local<Context> context) {
    Isolate* isolate = v8::Isolate::GetCurrent();
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

int ArgConverter::GetJavaArgsLength(JNIEnv* env, const JavaCallArgs& args) {
    return args.types == nullptr ? 0 : env->GetArrayLength(args.types);
}

void ArgConverter::ConvertJavaArgsToJsArgs(Isolate* isolate, JNIEnv* env, const JavaCallArgs& args, int argc, Local<Value>* out) {
    if (argc == 0) {
        return;
    }

    constexpr int kInlineArgs = 16;
    jint inlineTypes[kInlineArgs];
    jlong inlineValues[kInlineArgs];
    std::vector<jint> heapTypes;
    std::vector<jlong> heapValues;
    jint* types = inlineTypes;
    jlong* values = inlineValues;
    if (argc > kInlineArgs) {
        heapTypes.resize(argc);
        heapValues.resize(argc);
        types = heapTypes.data();
        values = heapValues.data();
    }

    env->GetIntArrayRegion(args.types, 0, argc, types);
    env->GetLongArrayRegion(args.values, 0, argc, values);

    auto objectManager = Runtime::GetRuntime(isolate)->GetObjectManager();

    for (int i = 0; i < argc; i++) {
        jlong bits = values[i];
        Local<Value> jsArg;
        switch (static_cast<Type>(types[i])) {
        case Type::Boolean:
            jsArg = Boolean::New(isolate, bits != 0);
            break;
        case Type::Char:
            jsArg = jcharToV8String(isolate, static_cast<jchar>(bits));
            break;
        case Type::Byte:
        case Type::Short:
        case Type::Int:
        case Type::Long:
            jsArg = Number::New(isolate, static_cast<double>(bits));
            break;
        case Type::Float: {
            auto raw = static_cast<int32_t>(bits);
            float value;
            memcpy(&value, &raw, sizeof(value));
            jsArg = Number::New(isolate, value);
            break;
        }
        case Type::Double: {
            double value;
            memcpy(&value, &bits, sizeof(value));
            jsArg = Number::New(isolate, value);
            break;
        }
        case Type::String: {
            JniLocalRef str(env->GetObjectArrayElement(args.refs, i));
            jsArg = jstringToV8String(isolate, (jstring) str);
            break;
        }
        case Type::JsObject: {
            // values[i] is an id Java read without the isolate lock, or -1.
            auto hintedObjectID = static_cast<jint>(bits);
            if (hintedObjectID >= 0) {
                jsArg = objectManager->GetLinkedJsObjectByJavaObject(hintedObjectID);
            }
            if (jsArg.IsEmpty()) {
                JniLocalRef arg(env->GetObjectArrayElement(args.refs, i));
                auto javaObjectID = objectManager->GetOrCreateObjectId(arg);
                jsArg = objectManager->GetJsObjectByJavaObject(javaObjectID);
                if (jsArg.IsEmpty()) {
                    jsArg = objectManager->CreateJSWrapper(javaObjectID, "", arg);
                }
            }
            break;
        }
        case Type::Null:
            jsArg = Null(isolate);
            break;
        }

        out[i] = jsArg;
    }
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
    NS_DCHECK(!value.IsEmpty());

    auto obj = Local<Object>::Cast(value);

    NS_DCHECK(!obj.IsEmpty());

    auto context = isolate->GetCurrentContext();
    Local<Value> temp;
    bool success = obj->Get(context, V8StringConstants::GetValue(isolate)).ToLocal(&temp);
    NS_DCHECK(success && !temp.IsEmpty());
    auto valueProp = temp.As<Object>();

    string num = ConvertToString(valueProp->ToString(context).ToLocalChecked());

    int64_t longValue = atoll(num.c_str());

    return longValue;
}

ArgConverter::TypeLongOperationsCache* ArgConverter::GetTypeLongCache(v8::Isolate* isolate) {
    // Per runtime, so there is no shared table to race on; see RuntimeState.h.
    auto* cache = RuntimeState::For<TypeLongOperationsCache>(isolate);
    if (cache == nullptr) {
        throw NativeScriptException("Long conversion cache requested after the runtime was torn down");
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



