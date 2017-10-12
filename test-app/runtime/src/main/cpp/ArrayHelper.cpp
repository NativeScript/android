#include "ArrayHelper.h"
#include "ArgConverter.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include <sstream>

using namespace v8;
using namespace std;
using namespace tns;

ArrayHelper::ArrayHelper() {
}

void ArrayHelper::Init(const Local<Context>& context) {
    JEnv env;

    RUNTIME_CLASS = env.FindClass("com/tns/Runtime");
    assert(RUNTIME_CLASS != nullptr);

    CREATE_ARRAY_HELPER = env.GetStaticMethodID(RUNTIME_CLASS, "createArrayHelper", "(Ljava/lang/String;I)Ljava/lang/Object;");
    assert(CREATE_ARRAY_HELPER != nullptr);

    auto isolate = context->GetIsolate();
    auto global = context->Global();
    auto arr = global->Get(context, ArgConverter::ConvertToV8String(isolate, "Array"));

    if (!arr.IsEmpty()) {
        Local<Value> arrVal;
        auto success = arr.ToLocal(&arrVal);
        if (success) {
            auto arrayObj = arrVal.As<Object>();
            arrayObj->Set(context, ArgConverter::ConvertToV8String(isolate, "create"), FunctionTemplate::New(isolate, CreateJavaArrayCallback)->GetFunction());
        }
    }
}

void ArrayHelper::CreateJavaArrayCallback(const FunctionCallbackInfo<Value>& info) {
    try {
        CreateJavaArray(info);
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

void ArrayHelper::CreateJavaArray(const v8::FunctionCallbackInfo<v8::Value>& info) {
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();

    if (info.Length() != 2) {
        Throw(isolate, "Expect two parameters.");
        return;
    }

    auto type = info[0];
    auto length = info[1];

    JniLocalRef array;

    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    if (type->IsString()) {
        if (!length->IsInt32()) {
            Throw(isolate, "Expect integer value as a second argument.");
            return;
        }

        jint len = length->Int32Value(context).FromJust();
        if (len < 0) {
            Throw(isolate, "Expect non-negative integer value as a second argument.");
            return;
        }

        auto typeName = ArgConverter::ConvertToString(type.As<String>());
        array = JniLocalRef(CreateArrayByClassName(typeName, len));
    } else if (type->IsFunction()) {
        if (!length->IsInt32()) {
            Throw(isolate, "Expect integer value as a second argument.");
            return;
        }

        jint len = length->Int32Value(context).FromJust();
        if (len < 0) {
            Throw(isolate, "Expect non-negative integer value as a second argument.");
            return;
        }

        auto func = type.As<Function>();

        auto clazz = func->Get(ArgConverter::ConvertToV8String(isolate, "class"));

        if (clazz.IsEmpty()) {
            Throw(isolate, "Expect known class as a second argument.");
            return;
        }

        auto c = objectManager->GetJavaObjectByJsObject(clazz.As<Object>());

        JEnv env;
        array = env.NewObjectArray(len, static_cast<jclass>(c), nullptr);
    } else {
        Throw(isolate, "Expect primitive type name or class function as a first argument");
        return;
    }

    jint javaObjectID = objectManager->GetOrCreateObjectId(array);
    auto jsWrapper = objectManager->CreateJSWrapper(javaObjectID, "" /* ignored */, array);
    info.GetReturnValue().Set(jsWrapper);
}

void ArrayHelper::Throw(Isolate* isolate, const std::string& errorMessage) {
    auto errMsg = ArgConverter::ConvertToV8String(isolate, errorMessage.c_str());
    auto err = Exception::Error(errMsg);
    isolate->ThrowException(err);
}

jobject ArrayHelper::CreateArrayByClassName(const string& typeName, int length) {
    JEnv env;
    jobject array;

    if (typeName == "char") {
        array = env.NewCharArray(length);
    } else if (typeName == "boolean") {
        array = env.NewBooleanArray(length);
    } else if (typeName == "byte") {
        array = env.NewByteArray(length);
    } else if (typeName == "short") {
        array = env.NewShortArray(length);
    } else if (typeName == "int") {
        array = env.NewIntArray(length);
    } else if (typeName == "long") {
        array = env.NewLongArray(length);
    } else if (typeName == "float") {
        array = env.NewFloatArray(length);
    } else if (typeName == "double") {
        array = env.NewDoubleArray(length);
    } else {
        JniLocalRef s(env.NewStringUTF(typeName.c_str()));
        array = env.CallStaticObjectMethod(RUNTIME_CLASS, CREATE_ARRAY_HELPER, (jstring)s, length);
    }

    return array;
}

jclass ArrayHelper::RUNTIME_CLASS = nullptr;
jmethodID ArrayHelper::CREATE_ARRAY_HELPER = nullptr;
