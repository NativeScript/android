#include "FieldAccessor.h"
#include "ArgConverter.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include <sstream>

using namespace v8;
using namespace std;
using namespace tns;

Local<Value> FieldAccessor::GetJavaField(Isolate* isolate, const Local<Object>& target, FieldCallbackData* fieldData) {
    JEnv env;

    EscapableHandleScope handleScope(isolate);
    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    Local<Value> fieldResult;

    JniLocalRef targetJavaObject;

    const auto& fieldTypeName = fieldData->signature;
    auto isStatic = fieldData->isStatic;

    auto isPrimitiveType = fieldTypeName.size() == 1;
    auto isFieldArray = fieldTypeName[0] == '[';

    if (fieldData->fid == nullptr) {
        auto fieldJniSig = isPrimitiveType
                           ? fieldTypeName
                           :
                           (isFieldArray
                            ? fieldTypeName
                            :
                            ("L" + fieldTypeName + ";"));

        if (isStatic) {
            fieldData->clazz = env.FindClass(fieldData->declaringType);
            fieldData->fid = env.GetStaticFieldID(fieldData->clazz, fieldData->name, fieldJniSig);
        } else {
            fieldData->clazz = env.FindClass(fieldData->declaringType);
            fieldData->fid = env.GetFieldID(fieldData->clazz, fieldData->name, fieldJniSig);
        }
    }

    if (!isStatic) {
        targetJavaObject = objectManager->GetJavaObjectByJsObject(target);

        if (targetJavaObject.IsNull()) {
            stringstream ss;
            ss << "Cannot access property '" << fieldData->name.c_str() << "' because there is no corresponding Java object";
            throw NativeScriptException(ss.str());
        }
    }

    auto fieldId = fieldData->fid;
    auto clazz = fieldData->clazz;

    if (isPrimitiveType) {
        switch (fieldTypeName[0]) {
        case 'Z': { //bool
            jboolean result;
            if (isStatic) {
                result = env.GetStaticBooleanField(clazz, fieldId);
            } else {
                result = env.GetBooleanField(targetJavaObject, fieldId);
            }
            fieldResult = Boolean::New(isolate, (result == JNI_TRUE));
            break;
        }
        case 'B': { //byte
            jbyte result;
            if (isStatic) {
                result = env.GetStaticByteField(clazz, fieldId);
            } else {
                result = env.GetByteField(targetJavaObject, fieldId);
            }
            fieldResult = handleScope.Escape(Int32::New(isolate, result));
            break;
        }
        case 'C': { //char
            jchar result;
            if (isStatic) {
                result = env.GetStaticCharField(clazz, fieldId);
            } else {
                result = env.GetCharField(targetJavaObject, fieldId);
            }

            JniLocalRef str(env.NewString(&result, 1));
            jboolean bol = true;
            const char* resP = env.GetStringUTFChars(str, &bol);
            fieldResult = handleScope.Escape(ArgConverter::ConvertToV8String(isolate, resP, 1));
            env.ReleaseStringUTFChars(str, resP);
            break;
        }
        case 'S': { //short
            jshort result;
            if (isStatic) {
                result = env.GetStaticShortField(clazz, fieldId);
            } else {
                result = env.GetShortField(targetJavaObject, fieldId);
            }
            fieldResult = handleScope.Escape(Int32::New(isolate, result));
            break;
        }
        case 'I': { //int
            jint result;
            if (isStatic) {
                result = env.GetStaticIntField(clazz, fieldId);
            } else {
                result = env.GetIntField(targetJavaObject, fieldId);
            }
            fieldResult = handleScope.Escape(Int32::New(isolate, result));
            break;

        }
        case 'J': { //long
            jlong result;
            if (isStatic) {
                result = env.GetStaticLongField(clazz, fieldId);
            } else {
                result = env.GetLongField(targetJavaObject, fieldId);
            }
            fieldResult = handleScope.Escape(ArgConverter::ConvertFromJavaLong(isolate, result));
            break;
        }
        case 'F': { //float
            jfloat result;
            if (isStatic) {
                result = env.GetStaticFloatField(clazz, fieldId);
            } else {
                result = env.GetFloatField(targetJavaObject, fieldId);
            }
            fieldResult = handleScope.Escape(Number::New(isolate, (double) result));
            break;
        }
        case 'D': { //double
            jdouble result;
            if (isStatic) {
                result = env.GetStaticDoubleField(clazz, fieldId);
            } else {
                result = env.GetDoubleField(targetJavaObject, fieldId);
            }
            fieldResult = handleScope.Escape(Number::New(isolate, result));
            break;
        }
        default: {
            stringstream ss;
            ss << "(InternalError): in FieldAccessor::GetJavaField: Unknown field type: '" << fieldTypeName[0] << "'";
            throw NativeScriptException(ss.str());
        }
        }
    } else {
        jobject result;

        if (isStatic) {
            result = env.GetStaticObjectField(clazz, fieldId);
        } else {
            result = env.GetObjectField(targetJavaObject, fieldId);
        }

        if (result != nullptr) {

            bool isString = fieldTypeName == "java/lang/String";
            if (isString) {
                auto resultV8Value = ArgConverter::jstringToV8String(isolate, (jstring) result);
                fieldResult = handleScope.Escape(resultV8Value);
            } else {
                int javaObjectID = objectManager->GetOrCreateObjectId(result);
                auto objectResult = objectManager->GetJsObjectByJavaObject(javaObjectID);

                if (objectResult.IsEmpty()) {
                    objectResult = objectManager->CreateJSWrapper(javaObjectID, fieldTypeName, result);
                }

                fieldResult = handleScope.Escape(objectResult);
            }
            env.DeleteLocalRef(result);
        } else {
            fieldResult = handleScope.Escape(Null(isolate));
        }
    }
    return fieldResult;
}

void FieldAccessor::SetJavaField(Isolate* isolate, const Local<Object>& target, const Local<Value>& value, FieldCallbackData* fieldData) {
    JEnv env;

    HandleScope handleScope(isolate);
    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    JniLocalRef targetJavaObject;

    const auto& fieldTypeName = fieldData->signature;
    auto isStatic = fieldData->isStatic;

    auto isPrimitiveType = fieldTypeName.size() == 1;
    auto isFieldArray = fieldTypeName[0] == '[';

    if (fieldData->fid == nullptr) {
        auto fieldJniSig = isPrimitiveType
                           ? fieldTypeName
                           :
                           (isFieldArray
                            ? fieldTypeName
                            :
                            ("L" + fieldTypeName + ";"));

        if (isStatic) {
            fieldData->clazz = env.FindClass(fieldData->declaringType);
            assert(fieldData->clazz != nullptr);
            fieldData->fid = env.GetStaticFieldID(fieldData->clazz, fieldData->name, fieldJniSig);
            assert(fieldData->fid != nullptr);
        } else {
            fieldData->clazz = env.FindClass(fieldData->declaringType);
            assert(fieldData->clazz != nullptr);
            fieldData->fid = env.GetFieldID(fieldData->clazz, fieldData->name, fieldJniSig);
            assert(fieldData->fid != nullptr);
        }
    }

    if (!isStatic) {
        targetJavaObject = objectManager->GetJavaObjectByJsObject(target);

        if (targetJavaObject.IsNull()) {
            stringstream ss;
            ss << "Cannot access property '" << fieldData->name.c_str() << "' because there is no corresponding Java object";
            throw NativeScriptException(ss.str());
        }
    }

    auto fieldId = fieldData->fid;
    auto clazz = fieldData->clazz;

    if (isPrimitiveType) {
        auto context = isolate->GetCurrentContext();
        switch (fieldTypeName[0]) {
        case 'Z': { //bool
            //TODO: validate value is a boolean before calling
            if (isStatic) {
                env.SetStaticBooleanField(clazz, fieldId, value->BooleanValue(context).ToChecked());
            } else {
                env.SetBooleanField(targetJavaObject, fieldId, value->BooleanValue(context).ToChecked());
            }
            break;
        }
        case 'B': { //byte
            //TODO: validate value is a byte before calling
            if (isStatic) {
                env.SetStaticByteField(clazz, fieldId, value->Int32Value(context).ToChecked());
            } else {
                env.SetByteField(targetJavaObject, fieldId, value->Int32Value(context).ToChecked());
            }
            break;
        }
        case 'C': { //char
            //TODO: validate value is a single char
            String::Utf8Value stringValue(isolate, value->ToString(isolate));
            JniLocalRef strValue(env.NewStringUTF(*stringValue));
            const char* chars = env.GetStringUTFChars(strValue, 0);

            if (isStatic) {
                env.SetStaticCharField(clazz, fieldId, chars[0]);
            } else {
                env.SetCharField(targetJavaObject, fieldId, chars[0]);
            }
            env.ReleaseStringUTFChars(strValue, chars);
            break;
        }
        case 'S': { //short
            //TODO: validate value is a short before calling
            if (isStatic) {
                env.SetStaticShortField(clazz, fieldId, value->Int32Value(context).ToChecked());
            } else {
                env.SetShortField(targetJavaObject, fieldId, value->Int32Value(context).ToChecked());
            }
            break;
        }
        case 'I': { //int
            //TODO: validate value is a int before calling
            if (isStatic) {
                env.SetStaticIntField(clazz, fieldId, value->Int32Value(context).ToChecked());
            } else {
                env.SetIntField(targetJavaObject, fieldId, value->Int32Value(context).ToChecked());
            }
            break;

        }
        case 'J': { //long
            jlong longValue = static_cast<jlong>(ArgConverter::ConvertToJavaLong(isolate, value));
            if (isStatic) {
                env.SetStaticLongField(clazz, fieldId, longValue);
            } else {
                env.SetLongField(targetJavaObject, fieldId, longValue);
            }
            break;
        }
        case 'F': { //float
            if (isStatic) {
                env.SetStaticFloatField(clazz, fieldId, static_cast<jfloat>(value->NumberValue(context).ToChecked()));
            } else {
                env.SetFloatField(targetJavaObject, fieldId, static_cast<jfloat>(value->NumberValue(context).ToChecked()));
            }
            break;
        }
        case 'D': { //double
            if (isStatic) {
                env.SetStaticDoubleField(clazz, fieldId, value->NumberValue(context).ToChecked());
            } else {
                env.SetDoubleField(targetJavaObject, fieldId, value->NumberValue(context).ToChecked());
            }
            break;
        }
        default: {
            stringstream ss;
            ss << "(InternalError): in FieldAccessor::SetJavaField: Unknown field type: '" << fieldTypeName[0] << "'";
            throw NativeScriptException(ss.str());
        }
        }
    } else {
        bool isString = fieldTypeName == "java/lang/String";
        JniLocalRef result;

        if (!value->IsNull() && !value->IsUndefined()) {
            if (isString) {
                //TODO: validate valie is a string;
                result = ArgConverter::ConvertToJavaString(value);
            } else {
                auto objectWithHiddenID = value->ToObject(isolate);
                result = objectManager->GetJavaObjectByJsObject(objectWithHiddenID);
            }
        }

        if (isStatic) {
            env.SetStaticObjectField(clazz, fieldId, result);
        } else {
            env.SetObjectField(targetJavaObject, fieldId, result);
        }
    }
}
