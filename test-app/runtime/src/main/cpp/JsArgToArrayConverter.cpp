#include "JsArgToArrayConverter.h"
#include <sstream>
#include "ObjectManager.h"
#include "V8StringConstants.h"
#include "ArgConverter.h"
#include "NumericCasts.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "MetadataNode.h"
#include "V8GlobalHelpers.h"
#include "JsArgConverter.h"

using namespace v8;
using namespace std;
using namespace tns;

/*
 * Converts a single JavaScript (V8) object to its respective Java representation
 */
JsArgToArrayConverter::JsArgToArrayConverter(Local<Context> context, const v8::Local<Value>& arg, bool isImplementationObject, int classReturnType)
    : m_arr(nullptr), m_argsAsObject(nullptr), m_argsLen(0), m_isValid(false), m_error(Error()), m_return_type(classReturnType) {
    if (!isImplementationObject) {
        m_argsLen = 1;
        m_argsAsObject = new jobject[m_argsLen];
        memset(m_argsAsObject, 0, m_argsLen * sizeof(jobject));

        m_isValid = ConvertArg(context, arg, 0);
    }
}

/*
 * Converts an array of JavaScript (V8) objects to a Java array of objects
 */
JsArgToArrayConverter::JsArgToArrayConverter(const v8::FunctionCallbackInfo<Value>& args, bool hasImplementationObject)
    : m_arr(nullptr), m_argsAsObject(nullptr), m_argsLen(0), m_isValid(false), m_error(Error()), m_return_type(static_cast<int>(Type::Null)) {
    m_argsLen = !hasImplementationObject ? args.Length() : args.Length() - 2;

    bool success = true;

    if (m_argsLen > 0) {
        m_argsAsObject = new jobject[m_argsLen];
        memset(m_argsAsObject, 0, m_argsLen * sizeof(jobject));

        auto context = args.GetIsolate()->GetCurrentContext();
        for (int i = 0; i < m_argsLen; i++) {
            success = ConvertArg(context, args[i], i);

            if (!success) {
                break;
            }
        }
    }

    m_isValid = success;
}

bool JsArgToArrayConverter::ConvertArg(Local<Context> context, const Local<Value>& arg, int index) {
    bool success = false;
    stringstream s;

    JEnv env;

    Type returnType = JType::getClassType(m_return_type);
    auto isolate = context->GetIsolate();

    if (arg.IsEmpty()) {
        s << "Cannot convert empty JavaScript object";
        success = false;
    }else if (arg->IsInt32() && (returnType == Type::Int || returnType == Type::Null)) {
        jint value = arg->Int32Value(context).ToChecked();
        auto javaObject = JType::NewInt(env, value);
        SetConvertedObject(env, index, javaObject);

        success = true;
    } else if (arg->IsNumber() || arg->IsNumberObject()) {
        double d = arg->NumberValue(context).ToChecked();
        int64_t i = (int64_t) d;

        //if returnType isNumber and this check is true, the number we'll try to convert is whole(integer)
        bool isWholeNumber = d == i;

        if (isWholeNumber) {
            jobject obj;

            //if returnType is long it will cast to long
            //if there is no return type specified it will cast to int
            //because default return type is null (ref type)
            if ((INT_MIN <= i) && (i <= INT_MAX) && (returnType == Type::Int || returnType == Type::Null)) {
                obj = JType::NewInt(env, (jint) d);
            } else { /*isLong*/
                obj = JType::NewLong(env, (jlong) d);
            }

            SetConvertedObject(env, index, obj);

            success = true;
        } else {
            jobject obj;

            //if returnType is double it will cast to double
            //if there is no return type specified it will cast to float
            //because default return type is null (ref type)
            if ((FLT_MIN <= d) && (d <= FLT_MAX) && (returnType == Type::Float || returnType == Type::Null)) {
                obj = JType::NewFloat(env, (jfloat) d);
            } else {
                /*isDouble*/
                obj = JType::NewDouble(env, (jdouble) d);
            }

            SetConvertedObject(env, index, obj);

            success = true;
        }
    } else if (arg->IsBoolean()) {
        jboolean value = arg->BooleanValue(isolate);
        auto javaObject = JType::NewBoolean(env, value);
        SetConvertedObject(env, index, javaObject);
        success = true;
    } else if (arg->IsBooleanObject()) {
        auto boolObj = Local<BooleanObject>::Cast(arg);
        jboolean value = boolObj->BooleanValue(isolate) ? JNI_TRUE : JNI_FALSE;
        auto javaObject = JType::NewBoolean(env, value);
        SetConvertedObject(env, index, javaObject);

        success = true;
    } else if (arg->IsString() || arg->IsStringObject()) {
        auto stringObject = ArgConverter::ConvertToJavaString(arg);
        SetConvertedObject(env, index, stringObject);

        success = true;
    } else if (arg->IsObject()) {
        auto jsObj = arg->ToObject(context).ToLocalChecked();

        auto castType = NumericCasts::GetCastType(isolate, jsObj);

        Local<Value> castValue;
        jchar charValue;
        jbyte byteValue;
        jshort shortValue;
        jlong longValue;
        jfloat floatValue;
        jdouble doubleValue;
        jobject javaObject;
        JniLocalRef obj;

        auto runtime = Runtime::GetRuntime(isolate);
        auto objectManager = runtime->GetObjectManager();

        switch (castType) {
        case CastType::Char:
            castValue = NumericCasts::GetCastValue(jsObj);
            charValue = '\0';
            if (castValue->IsString()) {
                string str = ArgConverter::ConvertToString(castValue->ToString(context).ToLocalChecked());
                charValue = (jchar) str[0];
            }
            javaObject = JType::NewChar(env, charValue);
            SetConvertedObject(env, index, javaObject);
            success = true;
            break;

        case CastType::Byte:
            castValue = NumericCasts::GetCastValue(jsObj);
            byteValue = 0;
            if (castValue->IsString()) {
                string value = ArgConverter::ConvertToString(castValue->ToString(context).ToLocalChecked());
                int byteArg = atoi(value.c_str());
                byteValue = (jbyte) byteArg;
            } else if (castValue->IsInt32()) {
                int byteArg = castValue->ToInt32(context).ToLocalChecked()->Int32Value(context).ToChecked();
                byteValue = (jbyte) byteArg;
            }
            javaObject = JType::NewByte(env, byteValue);
            SetConvertedObject(env, index, javaObject);
            success = true;
            break;

        case CastType::Short:
            castValue = NumericCasts::GetCastValue(jsObj);
            shortValue = 0;
            if (castValue->IsString()) {
                string value = ArgConverter::ConvertToString(castValue->ToString(context).ToLocalChecked());
                int shortArg = atoi(value.c_str());
                shortValue = (jshort) shortArg;
            } else if (castValue->IsInt32()) {
                jlong shortArg = castValue->ToInt32(context).ToLocalChecked()->Int32Value(context).ToChecked();
                shortValue = (jshort) shortArg;
            }
            javaObject = JType::NewShort(env, shortValue);
            SetConvertedObject(env, index, javaObject);
            success = true;
            break;

        case CastType::Long:
            castValue = NumericCasts::GetCastValue(jsObj);
            longValue = 0;
            if (castValue->IsString()) {
                auto strValue = ArgConverter::ConvertToString(castValue->ToString(context).ToLocalChecked());
                longValue = atoll(strValue.c_str());
            } else if (castValue->IsInt32()) {
                longValue = castValue->ToInt32(context).ToLocalChecked()->Int32Value(context).ToChecked();
            }
            javaObject = JType::NewLong(env, longValue);
            SetConvertedObject(env, index, javaObject);
            success = true;
            break;

        case CastType::Float:
            castValue = NumericCasts::GetCastValue(jsObj);
            floatValue = 0;
            if (castValue->IsNumber()) {
                double floatArg = castValue->ToNumber(context).ToLocalChecked()->NumberValue(context).ToChecked();
                floatValue = (jfloat) floatArg;
            }
            javaObject = JType::NewFloat(env, floatValue);
            SetConvertedObject(env, index, javaObject);
            success = true;
            break;

        case CastType::Double:
            castValue = NumericCasts::GetCastValue(jsObj);
            doubleValue = 0;
            if (castValue->IsNumber()) {
                double doubleArg = castValue->ToNumber(context).ToLocalChecked()->NumberValue(context).ToChecked();
                doubleValue = (jdouble) doubleArg;
            }
            javaObject = JType::NewDouble(env, doubleValue);
            SetConvertedObject(env, index, javaObject);
            success = true;
            break;

        case CastType::None:
            obj = objectManager->GetJavaObjectByJsObject(jsObj);


            if (obj.IsNull() && (jsObj->IsTypedArray() || jsObj->IsArrayBuffer() || jsObj->IsArrayBufferView()))
                {

                    BufferCastType bufferCastType = tns::BufferCastType::Byte;
                    shared_ptr<BackingStore> store;
                    size_t offset = 0;
                    size_t length;
                    if (jsObj->IsArrayBuffer())
                    {
                        auto array = jsObj.As<v8::ArrayBuffer>();
                        store = array->GetBackingStore();
                        length = array->ByteLength();
                    }
                    else if (jsObj->IsArrayBufferView())
                    {
                        auto array = jsObj.As<v8::ArrayBufferView>();

                        if (!array->HasBuffer())
                        {

                            length = array->ByteLength();
                            void *data = malloc(length);
                            array->CopyContents(data, length);
                        }
                        else
                        {
                            length = array->ByteLength();
                        }
                        offset = array->ByteOffset();
                        store = array->Buffer()->GetBackingStore();
                        bufferCastType = JsArgConverter::GetCastType(array);
                    }
                    else
                    {
                        auto array = jsObj.As<v8::TypedArray>();
                        offset = array->ByteOffset();
                        store = array->Buffer()->GetBackingStore();
                        length = array->ByteLength();
                        bufferCastType = JsArgConverter::GetCastType(array);
                    }

                    auto data = static_cast<uint8_t *>(store->Data()) + offset;

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
                    objectManager->Link(jsObj, id, clazz);

                    obj = objectManager->GetJavaObjectByJsObject(jsObj);
                }


            V8GetPrivateValue(isolate, jsObj, V8StringConstants::GetNullNodeName(isolate), castValue);

            if (!castValue.IsEmpty()) {
                auto node = reinterpret_cast<MetadataNode*>(castValue.As<External>()->Value());

                if (node == nullptr) {
                    s << "Cannot get type of the null argument at index " << index;
                    success = false;
                    break;
                }

                auto type = node->GetName();
                auto nullObjName = "com/tns/NullObject";
                auto nullObjCtorSig = "(Ljava/lang/Class;)V";

                jclass nullClazz = env.FindClass(nullObjName);
                jmethodID ctor = env.GetMethodID(nullClazz, "<init>", nullObjCtorSig);
                jclass clazzToNull = env.FindClass(type.c_str());
                jobject nullObjType = env.NewObject(nullClazz, ctor, clazzToNull);

                if (nullObjType != nullptr) {
                    SetConvertedObject(env, index, nullObjType, false);
                } else {
                    SetConvertedObject(env, index, nullptr);
                }

                success = true;
                return success;
            }

            success = !obj.IsNull();
            if (success) {
                SetConvertedObject(env, index, obj.Move(), obj.IsGlobal());
            } else {
                String::Utf8Value jsObjStr(isolate, jsObj);
                s << "Cannot marshal JavaScript argument " << (*jsObjStr ? *jsObjStr : "<failed-to-string>") << " at index " << index << " to Java type.";
            }
            break;

        default:
            throw NativeScriptException("Unsupported cast type");
        }
    } else if (arg->IsUndefined() || arg->IsNull()) {
        SetConvertedObject(env, index, nullptr);
        success = true;
    } else {
        s << "Cannot marshal JavaScript argument at index " << index << " to Java type.";
        success = false;
    }

    if (!success) {
        m_error.index = index;
        m_error.msg = s.str();
    }

    return success;
}

jobject JsArgToArrayConverter::GetConvertedArg() {
    return (m_argsLen > 0) ? m_argsAsObject[0] : nullptr;
}

void JsArgToArrayConverter::SetConvertedObject(JEnv& env, int index, jobject obj, bool isGlobal) {
    m_argsAsObject[index] = obj;
    if ((obj != nullptr) && !isGlobal) {
        m_storedIndexes.push_back(index);
    }
}

int JsArgToArrayConverter::Length() const {
    return m_argsLen;
}

bool JsArgToArrayConverter::IsValid() const {
    return m_isValid;
}

JsArgToArrayConverter::Error JsArgToArrayConverter::GetError() const {
    return m_error;
}

jobjectArray JsArgToArrayConverter::ToJavaArray() {
    if ((m_arr == nullptr) && (m_argsLen > 0)) {
        if (m_argsLen >= JsArgToArrayConverter::MAX_JAVA_PARAMS_COUNT) {
            stringstream ss;
            ss << "You are trying to override more than the MAX_JAVA_PARAMS_COUNT: " << JsArgToArrayConverter::MAX_JAVA_PARAMS_COUNT;
            throw NativeScriptException(ss.str());
        }

        JEnv env;

        if (JsArgToArrayConverter::JAVA_LANG_OBJECT_CLASS == nullptr) {
            JsArgToArrayConverter::JAVA_LANG_OBJECT_CLASS = env.FindClass("java/lang/Object");
        }

        JniLocalRef tmpArr(env.NewObjectArray(m_argsLen, JsArgToArrayConverter::JAVA_LANG_OBJECT_CLASS, nullptr));
        m_arr = (jobjectArray) env.NewGlobalRef(tmpArr);

        for (int i = 0; i < m_argsLen; i++) {
            env.SetObjectArrayElement(m_arr, i, m_argsAsObject[i]);
        }
    }

    return m_arr;
}

JsArgToArrayConverter::~JsArgToArrayConverter() {
    if (m_argsLen > 0) {
        JEnv env;

        env.DeleteGlobalRef(m_arr);

        int length = m_storedIndexes.size();
        for (int i = 0; i < length; i++) {
            int index = m_storedIndexes[i];
            env.DeleteLocalRef(m_argsAsObject[index]);
        }

        delete[] m_argsAsObject;
    }
}

jclass JsArgToArrayConverter::JAVA_LANG_OBJECT_CLASS = nullptr;