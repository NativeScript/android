//
// Created by Vladimir Mutafov on 16.11.22.
//

#include "JSToJavaObjectsConverter.h"
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

bool tns::ConvertJavaScriptObject(
        v8::Isolate *const isolate,
        const v8::Local<v8::Value> &arg,
        int index,
        std::string &methodSignature,
        std::vector<std::string> *tokens,
        JNIArgRefsState &jniArgRefsState
) {
    auto context = isolate->GetCurrentContext();
    auto jsObject = arg->ToObject(context).ToLocalChecked();

    auto castType = NumericCasts::GetCastType(isolate, jsObject);

    Local<Value> castValue;
    JniLocalRef obj;

    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    JEnv env;

    switch (castType) {
        case CastType::Char:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsString()) {
                std::string value = ArgConverter::ConvertToString(
                        castValue->ToString(context).ToLocalChecked());
                jniArgRefsState.m_args[index].c = (jchar) value[0];
                return true;
            }
            break;

        case CastType::Byte:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsString()) {
                std::string strValue = ArgConverter::ConvertToString(
                        castValue->ToString(context).ToLocalChecked());
                int byteArg = atoi(strValue.c_str());
                jbyte value = (jbyte) byteArg;
                ConvertFromCastFunctionObject(value, index, methodSignature, tokens, jniArgRefsState);
            } else if (castValue->IsInt32()) {
                jbyte value = (jbyte) castValue->ToInt32(context).ToLocalChecked()->Int32Value(
                        context).ToChecked();
                ConvertFromCastFunctionObject(value, index, methodSignature, tokens, jniArgRefsState);
            }
            break;

        case CastType::Short:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsString()) {
                std::string strValue = ArgConverter::ConvertToString(
                        castValue->ToString(context).ToLocalChecked());
                int shortArg = atoi(strValue.c_str());
                jshort value = (jshort) shortArg;
                ConvertFromCastFunctionObject(value, index, methodSignature, tokens, jniArgRefsState);
            } else if (castValue->IsInt32()) {
                jshort value = (jshort) castValue->ToInt32(context).ToLocalChecked()->Int32Value(
                        context).ToChecked();
                ConvertFromCastFunctionObject(value, index, methodSignature, tokens, jniArgRefsState);
            }
            break;

        case CastType::Long:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsString()) {
                std::string strValue = ArgConverter::ConvertToString(
                        castValue->ToString(context).ToLocalChecked());
                int64_t longArg = atoll(strValue.c_str());
                jlong value = (jlong) longArg;
                ConvertFromCastFunctionObject(value, index, methodSignature, tokens, jniArgRefsState);
            } else if (castValue->IsInt32()) {
                jlong value = (jlong) castValue->ToInt32(context).ToLocalChecked()->IntegerValue(
                        context).ToChecked();
                ConvertFromCastFunctionObject(value, index, methodSignature, tokens, jniArgRefsState);
            }
            break;

        case CastType::Float:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsNumber()) {
                double floatArg = castValue->ToNumber(context).ToLocalChecked()->NumberValue(
                        context).ToChecked();
                jfloat value = (jfloat) floatArg;
                ConvertFromCastFunctionObject(value, index, methodSignature, tokens, jniArgRefsState);
            }
            break;

        case CastType::Double:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsNumber()) {
                double doubleArg = castValue->ToNumber(context).ToLocalChecked()->NumberValue(
                        context).ToChecked();
                jdouble value = (jdouble) doubleArg;
                ConvertFromCastFunctionObject(value, index, methodSignature, tokens, jniArgRefsState);
            }
            break;


        case CastType::None:
            obj = objectManager->GetJavaObjectByJsObject(jsObject);

            if (obj.IsNull() && (jsObject->IsTypedArray() || jsObject->IsArrayBuffer() ||
                                 jsObject->IsArrayBufferView())) {

                JavaObjectCastType bufferCastType = tns::JavaObjectCastType::Byte;
                std::shared_ptr<BackingStore> store;
                size_t offset = 0;
                size_t length;
                uint8_t *data = nullptr;
                auto link_with_data = false;
                if (jsObject->IsArrayBuffer()) {
                    auto array = jsObject.As<v8::ArrayBuffer>();
                    store = array->GetBackingStore();
                    length = array->ByteLength();
                } else if (jsObject->IsArrayBufferView()) {
                    auto array = jsObject.As<v8::ArrayBufferView>();

                    if (!array->HasBuffer()) {

                        length = array->ByteLength();
                        void *data_ = malloc(length);
                        array->CopyContents(data_, length);
                        data = (uint8_t *) data_;
                        link_with_data = true;
                    } else {
                        length = array->ByteLength();
                    }
                    offset = array->ByteOffset();
                    store = array->Buffer()->GetBackingStore();
                    bufferCastType = tns::GetCastType(array);
                } else {
                    auto array = jsObject.As<v8::TypedArray>();
                    offset = array->ByteOffset();
                    store = array->Buffer()->GetBackingStore();
                    length = array->ByteLength();
                    bufferCastType = tns::GetCastType(array);
                }

                if (data == nullptr) {
                    data = static_cast<uint8_t *>(store->Data()) + offset;
                }

                auto directBuffer = env.NewDirectByteBuffer(
                        data,
                        length);

                auto directBufferClazz = env.GetObjectClass(directBuffer);

                auto byteOrderId = env.GetMethodID(directBufferClazz, "order",
                                                   "(Ljava/nio/ByteOrder;)Ljava/nio/ByteBuffer;");

                auto byteOrderClazz = env.FindClass("java/nio/ByteOrder");

                auto byteOrderEnumId = env.GetStaticMethodID(byteOrderClazz, "nativeOrder",
                                                             "()Ljava/nio/ByteOrder;");

                auto nativeByteOrder = env.CallStaticObjectMethodA(byteOrderClazz, byteOrderEnumId,
                                                                   nullptr);

                directBuffer = env.CallObjectMethod(directBuffer, byteOrderId, nativeByteOrder);

                jobject buffer;

                if (bufferCastType == JavaObjectCastType::Short) {

                    auto id = env.GetMethodID(directBufferClazz, "asShortBuffer",
                                              "()Ljava/nio/ShortBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                } else if (bufferCastType == JavaObjectCastType::Int) {

                    auto id = env.GetMethodID(directBufferClazz, "asIntBuffer",
                                              "()Ljava/nio/IntBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                } else if (bufferCastType == JavaObjectCastType::Long) {

                    auto id = env.GetMethodID(directBufferClazz, "asLongBuffer",
                                              "()Ljava/nio/LongBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                } else if (bufferCastType == JavaObjectCastType::Float) {

                    auto id = env.GetMethodID(directBufferClazz, "asFloatBuffer",
                                              "()Ljava/nio/FloatBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                } else if (bufferCastType == JavaObjectCastType::Double) {

                    auto id = env.GetMethodID(directBufferClazz, "asDoubleBuffer",
                                              "()Ljava/nio/DoubleBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                } else {
                    buffer = directBuffer;
                }

                buffer = env.NewGlobalRef(buffer);

                jint id = objectManager->GetOrCreateObjectId(buffer);
                objectManager->Link(jsObject, id);

                obj = objectManager->GetJavaObjectByJsObject(jsObject);
            }

            V8GetPrivateValue(isolate, jsObject, ArgConverter::ConvertToV8String(isolate,
                                                                                 V8StringConstants::NULL_NODE_NAME),
                              castValue);

            if (!castValue.IsEmpty()) {
                jniArgRefsState.SetConvertedObject(index, nullptr);
                break;
            }

            if (!obj.IsNull()) {
                jniArgRefsState.SetConvertedObject(index, obj.Move(), obj.IsGlobal());
            }
            break;

        default:
            throw NativeScriptException("Unsupported cast type");
    }

    return true;
}


template<typename T>
bool tns::ConvertFromCastFunctionObject(
        T value,
        int index,
        std::string &methodSignature,
        std::vector<std::string> *tokens,
        JNIArgRefsState &jniArgRefsState
) {
    const auto &typeSignature = tokens->at(index);
    const char typeSignaturePrefix = typeSignature[0];

    switch (typeSignaturePrefix) {
        case 'B':
            jniArgRefsState.m_args[index].b = (jbyte) value;
            break;
        case 'S':
            jniArgRefsState.m_args[index].s = (jshort) value;
            break;
        case 'I':
            jniArgRefsState.m_args[index].i = (jint) value;
            break;
        case 'J':
            jniArgRefsState.m_args[index].j = (jlong) value;
            break;
        case 'F':
            jniArgRefsState.m_args[index].f = (jfloat) value;
            break;
        case 'D':
            jniArgRefsState.m_args[index].d = (jdouble) value;
            break;
        default:
            return false;
    }

    return true;
}