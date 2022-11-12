#include "JsArgConverter.h"
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

using namespace v8;
using namespace std;
using namespace tns;

JsArgConverter::JsArgConverter(const Local<Object>& caller, const v8::FunctionCallbackInfo<Value>& args, const string& methodSignature, MetadataEntry* entry)
        : m_isolate(args.GetIsolate()), m_methodSignature(methodSignature), m_isValid(true), m_error(Error()) {
    int v8ProvidedArgumentsLength = args.Length();
    m_argsLen = 1 + v8ProvidedArgumentsLength;

    if (m_argsLen > 0) {
        if ((entry != nullptr) && (entry->isResolved)) {
            if (entry->parsedSig.empty()) {
                JniSignatureParser parser(m_methodSignature);
                entry->parsedSig = parser.Parse();
            }
            m_tokens = entry->parsedSig;
        } else {
            JniSignatureParser parser(m_methodSignature);
            m_tokens = parser.Parse();
        }

        m_isValid = ConvertArg(caller, 0);

        if (!m_isValid) {
            throw NativeScriptException("Error while converting argument!");
        }

        for (int i = 0; i < v8ProvidedArgumentsLength; i++) {
            m_isValid = ConvertArg(args[i], i + 1);

            if (!m_isValid) {
                break;
            }
        }
    }
}

JsArgConverter::JsArgConverter(const v8::FunctionCallbackInfo<Value>& args, bool hasImplementationObject, const string& methodSignature, MetadataEntry* entry)
    : m_isolate(args.GetIsolate()), m_methodSignature(methodSignature), m_isValid(true), m_error(Error()) {
    m_argsLen = !hasImplementationObject ? args.Length() : args.Length() - 1;

    if (m_argsLen > 0) {
        m_storedObjects.reserve(m_argsLen);
        if ((entry != nullptr) && (entry->isResolved)) {
            if (entry->parsedSig.empty()) {
                JniSignatureParser parser(m_methodSignature);
                entry->parsedSig = parser.Parse();
            }
            m_tokens = entry->parsedSig;
        } else {
            JniSignatureParser parser(m_methodSignature);
            m_tokens = parser.Parse();
        }

        for (int i = 0; i < m_argsLen; i++) {
            m_isValid = ConvertArg(args[i], i);

            if (!m_isValid) {
                break;
            }
        }
    }
}

JsArgConverter::JsArgConverter(const v8::FunctionCallbackInfo<Value>& args, const string& methodSignature)
    : m_isolate(args.GetIsolate()), m_methodSignature(methodSignature), m_isValid(true), m_error(Error()) {
    m_argsLen = args.Length();

    JniSignatureParser parser(m_methodSignature);
    m_tokens = parser.Parse();

    if(m_argsLen > 0){
        m_storedObjects.reserve(m_argsLen);
    }

    for (int i = 0; i < m_argsLen; i++) {
        m_isValid = ConvertArg(args[i], i);


        if (!m_isValid) {
            break;
        }
    }
}

tns::BufferCastType JsArgConverter::GetCastType(const v8::Local<v8::ArrayBufferView>& view) {
    if (view->IsUint16Array() || view->IsInt16Array()){
        return tns::BufferCastType::Short;
    }else if (view->IsUint32Array() || view->IsInt32Array()){
        return tns::BufferCastType::Int;
    }else if (view->IsFloat32Array()){
        return tns::BufferCastType::Float;
    }else if (view->IsFloat64Array()){
        return tns::BufferCastType::Double;
    }else if (view->IsBigUint64Array() || view->IsBigInt64Array()){
        return tns::BufferCastType::Long;
    }
    return tns::BufferCastType::Byte;
}

bool JsArgConverter::ConvertArg(const Local<Value>& arg, int index) {
    bool success = false;

    char buff[1024];

    const auto& typeSignature = m_tokens.at(index);

    if (arg.IsEmpty()) {
        SetConvertedObject(index, nullptr);
        success = false;
    } else if (arg->IsArray()) {
        success = typeSignature[0] == '[';

        if (success) {
            auto jsArr = Local<Array>::Cast(arg);
            success = ConvertJavaScriptArray(jsArr, index);
        }

        if (!success) {
            sprintf(buff, "Cannot convert array to %s at index %d", typeSignature.c_str(), index);
        }
    } else if (arg->IsNumber() || arg->IsNumberObject()) {
        success = ConvertJavaScriptNumber(arg, index);

        if (!success) {
            sprintf(buff, "Cannot convert number to %s at index %d", typeSignature.c_str(), index);
        }
    } else if (arg->IsBoolean() || arg->IsBooleanObject()) {
        success = ConvertJavaScriptBoolean(arg, index);

        if (!success) {
            sprintf(buff, "Cannot convert boolean to %s at index %d", typeSignature.c_str(), index);
        }
    } else if (arg->IsString() || arg->IsStringObject()) {
        success = ConvertJavaScriptString(arg, index);

        if (!success) {
            sprintf(buff, "Cannot convert string to %s at index %d", typeSignature.c_str(), index);
        }
    } else if (arg->IsObject()) {
        auto context = m_isolate->GetCurrentContext();
        auto jsObject = arg->ToObject(context).ToLocalChecked();

        auto castType = NumericCasts::GetCastType(m_isolate, jsObject);

        Local<Value> castValue;
        JniLocalRef obj;

        auto runtime = Runtime::GetRuntime(m_isolate);
        auto objectManager = runtime->GetObjectManager();

        JEnv env;


        auto isSupportSig = typeSignature == "Ljava/nio/ByteBuffer;" ||
                            typeSignature == "Ljava/nio/ShortBuffer;" ||
                            typeSignature == "Ljava/nio/IntBuffer;" ||
                            typeSignature == "Ljava/nio/LongBuffer;" ||
                            typeSignature == "Ljava/nio/FloatBuffer;" ||
                            typeSignature == "Ljava/nio/DoubleBuffer;";

        switch (castType) {
        case CastType::Char:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsString()) {
                string value = ArgConverter::ConvertToString(castValue->ToString(context).ToLocalChecked());
                m_args[index].c = (jchar) value[0];
                success = true;
            }
            break;

        case CastType::Byte:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsString()) {
                string strValue = ArgConverter::ConvertToString(castValue->ToString(context).ToLocalChecked());
                int byteArg = atoi(strValue.c_str());
                jbyte value = (jbyte) byteArg;
                success = ConvertFromCastFunctionObject(value, index);
            } else if (castValue->IsInt32()) {
                jbyte value = (jbyte) castValue->ToInt32(context).ToLocalChecked()->Int32Value(context).ToChecked();
                success = ConvertFromCastFunctionObject(value, index);
            }
            break;

        case CastType::Short:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsString()) {
                string strValue = ArgConverter::ConvertToString(castValue->ToString(context).ToLocalChecked());
                int shortArg = atoi(strValue.c_str());
                jshort value = (jshort) shortArg;
                success = ConvertFromCastFunctionObject(value, index);
            } else if (castValue->IsInt32()) {
                jshort value = (jshort) castValue->ToInt32(context).ToLocalChecked()->Int32Value(context).ToChecked();
                success = ConvertFromCastFunctionObject(value, index);
            }
            break;

        case CastType::Long:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsString()) {
                string strValue = ArgConverter::ConvertToString(castValue->ToString(context).ToLocalChecked());
                int64_t longArg = atoll(strValue.c_str());
                jlong value = (jlong) longArg;
                success = ConvertFromCastFunctionObject(value, index);
            } else if (castValue->IsInt32()) {
                jlong value = (jlong) castValue->ToInt32(context).ToLocalChecked()->IntegerValue(context).ToChecked();
                success = ConvertFromCastFunctionObject(value, index);
            }
            break;

        case CastType::Float:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsNumber()) {
                double floatArg = castValue->ToNumber(context).ToLocalChecked()->NumberValue(context).ToChecked();
                jfloat value = (jfloat) floatArg;
                success = ConvertFromCastFunctionObject(value, index);
            }
            break;

        case CastType::Double:
            castValue = NumericCasts::GetCastValue(jsObject);
            if (castValue->IsNumber()) {
                double doubleArg = castValue->ToNumber(context).ToLocalChecked()->NumberValue(context).ToChecked();
                jdouble value = (jdouble) doubleArg;
                success = ConvertFromCastFunctionObject(value, index);
            }
            break;


        case CastType::None:
            obj = objectManager->GetJavaObjectByJsObject(jsObject);

            if (obj.IsNull() && (jsObject->IsTypedArray() || jsObject->IsArrayBuffer() || jsObject->IsArrayBufferView()))
            {

                BufferCastType bufferCastType = tns::BufferCastType::Byte;
                shared_ptr<BackingStore> store;
                size_t offset = 0;
                size_t length;
                if (jsObject->IsArrayBuffer())
                {
                    auto array = jsObject.As<v8::ArrayBuffer>();
                    store = array->GetBackingStore();
                    length = array->ByteLength();
                }
                else if (jsObject->IsArrayBufferView())
                {
                    auto array = jsObject.As<v8::ArrayBufferView>();
                    offset = array->ByteOffset();
                    length = array->ByteLength();
                    store = array->Buffer()->GetBackingStore();
                    bufferCastType = JsArgConverter::GetCastType(array);
                }
                else {
                    auto array = jsObject.As<v8::TypedArray>();
                    offset = array->ByteOffset();
                    store = array->Buffer()->GetBackingStore();
                    length = array->ByteLength();
                    bufferCastType = JsArgConverter::GetCastType(array);
                }

                uint8_t * data = static_cast<uint8_t *>(store->Data()) + offset;

                auto directBuffer = env.NewDirectByteBuffer(
                    data,
                    length);

                auto directBufferClazz = env.GetObjectClass(directBuffer);

                auto byteOrderId = env.GetMethodID(directBufferClazz, "order",
                                                   "(Ljava/nio/ByteOrder;)Ljava/nio/ByteBuffer;");

                auto byteOrderClazz = env.FindClass("java/nio/ByteOrder");

                auto byteOrderEnumId = env.GetStaticMethodID(byteOrderClazz, "nativeOrder", "()Ljava/nio/ByteOrder;");

                auto nativeByteOrder = env.CallStaticObjectMethodA(byteOrderClazz, byteOrderEnumId,
                                                                   nullptr);

                directBuffer = env.CallObjectMethod(directBuffer, byteOrderId, nativeByteOrder);

                jobject buffer;

                if (bufferCastType == BufferCastType::Short)
                {

                    auto id = env.GetMethodID(directBufferClazz, "asShortBuffer",
                                              "()Ljava/nio/ShortBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                }
                else if (bufferCastType == BufferCastType::Int)
                {

                    auto id = env.GetMethodID(directBufferClazz, "asIntBuffer",
                                              "()Ljava/nio/IntBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                }
                else if (bufferCastType == BufferCastType::Long)
                {

                    auto id = env.GetMethodID(directBufferClazz, "asLongBuffer",
                                              "()Ljava/nio/LongBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                }
                else if (bufferCastType == BufferCastType::Float)
                {

                    auto id = env.GetMethodID(directBufferClazz, "asFloatBuffer",
                                              "()Ljava/nio/FloatBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                }
                else if (bufferCastType == BufferCastType::Double)
                {

                    auto id = env.GetMethodID(directBufferClazz, "asDoubleBuffer",
                                              "()Ljava/nio/DoubleBuffer;");
                    buffer = env.CallObjectMethodA(directBuffer, id, nullptr);
                }
                else
                {
                    buffer = directBuffer;
                }

                buffer = env.NewGlobalRef(buffer);

                int id = objectManager->GetOrCreateObjectId(buffer);
                auto clazz = env.GetObjectClass(buffer);

                objectManager->Link(jsObject, id, clazz);

                obj = objectManager->GetJavaObjectByJsObject(jsObject);
            }

            V8GetPrivateValue(m_isolate, jsObject, ArgConverter::ConvertToV8String(m_isolate, V8StringConstants::NULL_NODE_NAME), castValue);

            if (!castValue.IsEmpty()) {
                SetConvertedObject(index, nullptr);
                success = true;
                break;
            }

            success = !obj.IsNull();

            if (success) {
                SetConvertedObject(index, obj.Move(), obj.IsGlobal());
            } else {
                sprintf(buff, "Cannot convert object to %s at index %d", typeSignature.c_str(), index);
            }
            break;

        default:
            throw NativeScriptException("Unsupported cast type");
        }
    } else if (arg->IsUndefined() || arg->IsNull()) {
        SetConvertedObject(index, nullptr);
        success = true;
    } else {
        SetConvertedObject(index, nullptr);
        success = false;
    }

    if (!success) {
        m_error.index = index;
        m_error.msg = string(buff);
    }

    return success;
}

void JsArgConverter::SetConvertedObject(int index, jobject obj, bool isGlobal) {
    m_args[index].l = obj;
    if ((obj != nullptr) && !isGlobal) {
        m_storedObjects.push_back(index);
    }
}

bool JsArgConverter::ConvertJavaScriptNumber(const Local<Value>& jsValue, int index) {
    bool success = true;

    jvalue value = {
        0
    };

    const auto& typeSignature = m_tokens.at(index);
    auto context = m_isolate->GetCurrentContext();

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
        // TODO: refactor the whole method
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
        m_args[index] = value;
    }

    return success;
}

bool JsArgConverter::ConvertJavaScriptBoolean(const Local<Value>& jsValue, int index) {
    bool success;

    const auto& typeSignature = m_tokens.at(index);
    auto context = m_isolate->GetCurrentContext();

    if (typeSignature == "Z") {
        bool argValue;
        if (jsValue->IsBoolean()) {
            argValue = jsValue->BooleanValue(m_isolate);
        } else {
            auto boolObj = Local<BooleanObject>::Cast(jsValue);
            Local<Value> val;
            boolObj->Get(context, V8StringConstants::GetValueOf(m_isolate)).ToLocal(&val);
            if (!val.IsEmpty() && val->IsFunction()) {
                argValue = val.As<Function>()->Call(context, boolObj, 0, nullptr).ToLocalChecked()->BooleanValue(m_isolate);
            } else {
                argValue = false;
            }
        }

        jboolean value = argValue ? JNI_TRUE : JNI_FALSE;
        m_args[index].z = value;
        success = true;
    } else {
        success = false;
    }

    return success;
}

bool JsArgConverter::ConvertJavaScriptString(const Local<Value>& jsValue, int index) {
    jstring stringObject = ArgConverter::ConvertToJavaString(jsValue);
    SetConvertedObject(index, stringObject);

    return true;
}

bool JsArgConverter::ConvertJavaScriptArray(const Local<Array>& jsArr, int index) {
    bool success = true;

    jarray arr = nullptr;

    jsize arrLength = jsArr->Length();

    const auto& arraySignature = m_tokens.at(index);
    auto context = m_isolate->GetCurrentContext();

    string elementType = arraySignature.substr(1);

    const char elementTypePrefix = elementType[0];

    jclass elementClass;
    string strippedClassName;

    JEnv env;
    switch (elementTypePrefix) {
    case 'Z':
        arr = env.NewBooleanArray(arrLength);
        for (jsize i = 0; i < arrLength; i++) {
            jboolean value = jsArr->Get(context, i).ToLocalChecked()->BooleanValue(m_isolate);
            env.SetBooleanArrayRegion((jbooleanArray) arr, i, 1, &value);
        }
        break;
    case 'B':
        arr = env.NewByteArray(arrLength);
        for (jsize i = 0; i < arrLength; i++) {
            jbyte value = jsArr->Get(context, i).ToLocalChecked()->Int32Value(context).ToChecked();
            env.SetByteArrayRegion((jbyteArray) arr, i, 1, &value);
        }
        break;
    case 'C':
        arr = env.NewCharArray(arrLength);
        for (jsize i = 0; i < arrLength; i++) {
            String::Utf8Value utf8(m_isolate, jsArr->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked());
            JniLocalRef s(env.NewString((jchar*) *utf8, 1));
            const char* singleChar = env.GetStringUTFChars(s, nullptr);
            jchar value = *singleChar;
            env.ReleaseStringUTFChars(s, singleChar);
            env.SetCharArrayRegion((jcharArray) arr, i, 1, &value);
        }
        break;
    case 'S':
        arr = env.NewShortArray(arrLength);
        for (jsize i = 0; i < arrLength; i++) {
            jshort value = jsArr->Get(context, i).ToLocalChecked()->Int32Value(context).ToChecked();
            env.SetShortArrayRegion((jshortArray) arr, i, 1, &value);
        }
        break;
    case 'I':
        arr = env.NewIntArray(arrLength);
        for (jsize i = 0; i < arrLength; i++) {
            jint value = jsArr->Get(context, i).ToLocalChecked()->Int32Value(context).ToChecked();
            env.SetIntArrayRegion((jintArray) arr, i, 1, &value);
        }
        break;
    case 'J':
        arr = env.NewLongArray(arrLength);
        for (jsize i = 0; i < arrLength; i++) {
            jlong value = jsArr->Get(context, i).ToLocalChecked()->NumberValue(context).ToChecked();
            env.SetLongArrayRegion((jlongArray) arr, i, 1, &value);
        }
        break;
    case 'F':
        arr = env.NewFloatArray(arrLength);
        for (jsize i = 0; i < arrLength; i++) {
            jfloat value = jsArr->Get(context, i).ToLocalChecked()->NumberValue(context).ToChecked();
            env.SetFloatArrayRegion((jfloatArray) arr, i, 1, &value);
        }
        break;
    case 'D':
        arr = env.NewDoubleArray(arrLength);
        for (jsize i = 0; i < arrLength; i++) {
            jdouble value = jsArr->Get(context, i).ToLocalChecked()->NumberValue(context).ToChecked();
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
        SetConvertedObject(index, arr);
    }

    return success;
}

template<typename T>
bool JsArgConverter::ConvertFromCastFunctionObject(T value, int index) {
    bool success = false;

    const auto& typeSignature = m_tokens.at(index);

    const char typeSignaturePrefix = typeSignature[0];

    switch (typeSignaturePrefix) {
    case 'B':
        m_args[index].b = (jbyte) value;
        success = true;
        break;

    case 'S':
        m_args[index].s = (jshort) value;
        success = true;
        break;

    case 'I':
        m_args[index].i = (jint) value;
        success = true;
        break;

    case 'J':
        m_args[index].j = (jlong) value;
        success = true;
        break;

    case 'F':
        m_args[index].f = (jfloat) value;
        success = true;
        break;

    case 'D':
        m_args[index].d = (jdouble) value;
        success = true;
        break;

    default:
        success = false;
        break;
    }

    return success;
}

int JsArgConverter::Length() const {
    return m_argsLen;
}

bool JsArgConverter::IsValid() const {
    return m_isValid;
}

jvalue* JsArgConverter::ToArgs() {
    return m_args;
}

JsArgConverter::Error JsArgConverter::GetError() const {
    return m_error;
}

JsArgConverter::~JsArgConverter() {
    if (m_argsLen > 0) {
        JEnv env;
        int length = m_storedObjects.size();
        for (int i = 0; i < length; i++) {
            int index = m_storedObjects[i];
            env.DeleteLocalRef(m_args[index].l);
        }
    }
}
