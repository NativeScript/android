//
// Created by Vladimir Mutafov on 16.11.22.
//

#include "JSToJavaArraysConverter.h"
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

bool tns::ConvertJavaScriptArray(
        v8::Isolate *isolate,
        const Local<Value> &arg,
        int index,
        std::vector<std::string> &tokens,
        JNIArgRefsState &jniArgRefsState
) {
    bool success = true;

    jarray arr = nullptr;

    const Local<Array> &jsArr = Local<Array>::Cast(arg);

    jsize arrLength = jsArr->Length(); // java arrays size is max int32

    const auto &arraySignature = tokens.at(index);
    auto context = isolate->GetCurrentContext();

    std::string elementType = arraySignature.substr(1);

    const char elementTypePrefix = elementType[0];

    jclass elementClass;
    std::string strippedClassName;

    JEnv env;
    switch (elementTypePrefix) {
        case 'Z':
            arr = env.NewBooleanArray(arrLength);
            for (jsize i = 0; i < arrLength; i++) {
                jboolean value = jsArr->Get(context, i).ToLocalChecked()->BooleanValue(isolate);
                env.SetBooleanArrayRegion((jbooleanArray) arr, i, 1, &value);
            }
            break;
        case 'B':
            arr = env.NewByteArray(arrLength);
            for (jsize i = 0; i < arrLength; i++) {
                jbyte value = jsArr->Get(context, i).ToLocalChecked()->Int32Value(
                        context).ToChecked();
                env.SetByteArrayRegion((jbyteArray) arr, i, 1, &value);
            }
            break;
        case 'C':
            arr = env.NewCharArray(arrLength);
            for (jsize i = 0; i < arrLength; i++) {
                String::Utf8Value utf8(isolate, jsArr->Get(context, i).ToLocalChecked()->ToString(
                        context).ToLocalChecked());
                JniLocalRef s(env.NewString((jchar *) *utf8, 1));
                const char *singleChar = env.GetStringUTFChars(s, nullptr);
                jchar value = *singleChar;
                env.ReleaseStringUTFChars(s, singleChar);
                env.SetCharArrayRegion((jcharArray) arr, i, 1, &value);
            }
            break;
        case 'S':
            arr = env.NewShortArray(arrLength);
            for (jsize i = 0; i < arrLength; i++) {
                jshort value = jsArr->Get(context, i).ToLocalChecked()->Int32Value(
                        context).ToChecked();
                env.SetShortArrayRegion((jshortArray) arr, i, 1, &value);
            }
            break;
        case 'I':
            arr = env.NewIntArray(arrLength);
            for (jsize i = 0; i < arrLength; i++) {
                jint value = jsArr->Get(context, i).ToLocalChecked()->Int32Value(
                        context).ToChecked();
                env.SetIntArrayRegion((jintArray) arr, i, 1, &value);
            }
            break;
        case 'J':
            arr = env.NewLongArray(arrLength);
            for (jsize i = 0; i < arrLength; i++) {
                jlong value = jsArr->Get(context, i).ToLocalChecked()->NumberValue(
                        context).ToChecked();
                env.SetLongArrayRegion((jlongArray) arr, i, 1, &value);
            }
            break;
        case 'F':
            arr = env.NewFloatArray(arrLength);
            for (jsize i = 0; i < arrLength; i++) {
                jfloat value = jsArr->Get(context, i).ToLocalChecked()->NumberValue(
                        context).ToChecked();
                env.SetFloatArrayRegion((jfloatArray) arr, i, 1, &value);
            }
            break;
        case 'D':
            arr = env.NewDoubleArray(arrLength);
            for (jsize i = 0; i < arrLength; i++) {
                jdouble value = jsArr->Get(context, i).ToLocalChecked()->NumberValue(
                        context).ToChecked();
                env.SetDoubleArrayRegion((jdoubleArray) arr, i, 1, &value);
            }
            break;
        case 'L':
            strippedClassName = elementType.substr(1, elementType.length() - 2);
            elementClass = env.FindClass(strippedClassName);
            arr = env.NewObjectArray(arrLength, elementClass, nullptr);
            for (int i = 0; i < arrLength; i++) {
                auto v = jsArr->Get(context, i).ToLocalChecked();
                JsArgToArrayConverter c(context, v, false, (int) Type::Null);
                jobject o = c.GetConvertedArg();
                env.SetObjectArrayElement((jobjectArray) arr, i, o);
            }
            break;
        default:
            success = false;
            break;
    }

    if (success) {
        jniArgRefsState.SetConvertedObject(index, arr);
    }

    return success;
}