//
// Created by Vladimir Mutafov on 16.11.22.
//

#include "JSToJavaPrimitivesConverter.h"
#include "ObjectManager.h"
#include "JniSignatureParser.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "V8StringConstants.h"
#include "NumericCasts.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "V8GlobalHelpers.h"
#include <cstdlib>
#include "../CastTypes.h"
#include "string"

bool tns::ConvertJavaScriptBoolean(
        v8::Isolate *isolate,
        const v8::Local<v8::Value> &jsValue,
        int index,
        std::vector<std::string> *tokens,
        JNIArgRefsState &jniArgRefsState
) {
    bool success;

    const auto &typeSignature = tokens->at(index);
    auto context = isolate->GetCurrentContext();

    if (typeSignature == "Z") {
        bool argValue;
        if (jsValue->IsBoolean()) {
            argValue = jsValue->BooleanValue(isolate);
        } else {
            auto boolObj = Local<BooleanObject>::Cast(jsValue);
            Local<Value> val;
            boolObj->Get(context, V8StringConstants::GetValueOf(isolate)).ToLocal(&val);
            if (!val.IsEmpty() && val->IsFunction()) {
                argValue = val.As<Function>()->Call(context, boolObj, 0,
                                                    nullptr).ToLocalChecked()->BooleanValue(
                        isolate);
            } else {
                argValue = false;
            }
        }

        jboolean value = argValue ? JNI_TRUE : JNI_FALSE;
        jniArgRefsState.m_args[index].z = value;
        success = true;
    } else {
        success = false;
    }

    return success;
}

bool tns::ConvertJavaScriptNumber(
        v8::Isolate *isolate,
        const v8::Local<v8::Value> &jsValue,
        int index,
        std::vector<std::string> *tokens,
        JNIArgRefsState &jniArgRefsState
) {
    bool success = true;

    jvalue value = {
            0
    };

    const auto &typeSignature = tokens->at(index);
    auto context = isolate->GetCurrentContext();

    const char typePrefix = typeSignature[0];

    switch (typePrefix) {
        case 'B': // byte
            value.b = (jbyte) jsValue->Int32Value(context).ToChecked();
            break;

        case 'S': // short
            value.s = (jshort) jsValue->Int32Value(context).ToChecked();
            break;

        case 'I': // int
            value.i = (jint) jsValue->Int32Value(context).ToChecked();
            break;

        case 'J': // long
            if (jsValue->IsNumberObject()) {
                auto numObj = Local<NumberObject>::Cast(jsValue);
                value.j = (jlong) numObj->ValueOf();
            } else {
                value.j = (jlong) jsValue->IntegerValue(context).ToChecked();
            }
            break;

        case 'F': // float
            value.f = (jfloat) jsValue->NumberValue(context).ToChecked();
            break;

        case 'D': // double
            value.d = (jdouble) jsValue->NumberValue(context).ToChecked();
            break;

        default:
            success = false;
            break;
    }

    if (success) {
        jniArgRefsState.m_args[index] = value;
    }

    return success;
}

bool tns::ConvertJavaScriptString(
        const Local<v8::Value> &jsValue,
        int index,
        tns::JNIArgRefsState &jniArgRefsState
) {
    jstring stringObject = ArgConverter::ConvertToJavaString(jsValue);
    jniArgRefsState.SetConvertedObject(index, stringObject);

    return true;
}

