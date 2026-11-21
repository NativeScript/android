#include "ArrayElementAccessor.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "Util.h"
#include "NativeScriptException.h"
#include "Runtime.h"

using namespace v8;
using namespace std;
using namespace tns;

namespace {
    // The element type of a primitive array's signature ("[I" -> 'I'), else '\0'.
    char PrimitiveElementType(const string& arraySignature) {
        return arraySignature.size() == 2 ? arraySignature[1] : '\0';
    }
}

Local<Value> ArrayElementAccessor::GetArrayElement(Isolate* isolate, Local<Context> context, const Local<Object>& array, uint32_t index, const string& arraySignature) {
    JEnv env;

    EscapableHandleScope handleScope(isolate);
    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    auto arr = objectManager->GetJavaObjectByJsObject(array);

    assertNonNullNativeArray(arr);

    Local<Value> value;
    jsize startIndex = index;
    const jsize length = 1;

    switch (PrimitiveElementType(arraySignature)) {
        case 'Z': {
            jboolean boolArrValue;
            env.GetBooleanArrayRegion(static_cast<jbooleanArray>(arr), startIndex, length, &boolArrValue);
            value = Boolean::New(isolate, boolArrValue);
            break;
        }
        case 'B': {
            jbyte byteArrValue;
            env.GetByteArrayRegion(static_cast<jbyteArray>(arr), startIndex, length, &byteArrValue);
            value = Integer::New(isolate, byteArrValue);
            break;
        }
        case 'C': {
            jchar charArrValue;
            env.GetCharArrayRegion(static_cast<jcharArray>(arr), startIndex, length, &charArrValue);
            value = ArgConverter::jcharToV8String(isolate, charArrValue);
            break;
        }
        case 'S': {
            jshort shortArrValue;
            env.GetShortArrayRegion(static_cast<jshortArray>(arr), startIndex, length, &shortArrValue);
            value = Integer::New(isolate, shortArrValue);
            break;
        }
        case 'I': {
            jint intArrValue;
            env.GetIntArrayRegion(static_cast<jintArray>(arr), startIndex, length, &intArrValue);
            value = Integer::New(isolate, intArrValue);
            break;
        }
        case 'J': {
            jlong longArrValue;
            env.GetLongArrayRegion(static_cast<jlongArray>(arr), startIndex, length, &longArrValue);
            value = ArgConverter::ConvertFromJavaLong(isolate, longArrValue);
            break;
        }
        case 'F': {
            jfloat floatArrValue;
            env.GetFloatArrayRegion(static_cast<jfloatArray>(arr), startIndex, length, &floatArrValue);
            value = Number::New(isolate, floatArrValue);
            break;
        }
        case 'D': {
            jdouble doubleArrValue;
            env.GetDoubleArrayRegion(static_cast<jdoubleArray>(arr), startIndex, length, &doubleArrValue);
            value = Number::New(isolate, doubleArrValue);
            break;
        }
        default: {
            jobject result = env.GetObjectArrayElement(static_cast<jobjectArray>(arr), index);
            if (result == nullptr) {
                value = Null(isolate);
            } else if (arraySignature.compare(1, string::npos, "Ljava/lang/String;") == 0) {
                value = ArgConverter::jstringToV8String(isolate, static_cast<jstring>(result));
            } else {
                value = objectManager->GetOrCreateJSWrapper(result);
            }
            env.DeleteLocalRef(result);
            break;
        }
    }

    return handleScope.Escape(value);
}

void ArrayElementAccessor::SetArrayElement(Isolate* isolate, Local<Context> context, const Local<Object>& array, uint32_t index, const string& arraySignature, Local<Value>& value) {
    JEnv env;

    HandleScope handleScope(isolate);
    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    tns::JniLocalRef arr = objectManager->GetJavaObjectByJsObject(array);

    assertNonNullNativeArray(arr);

    const char elementType = PrimitiveElementType(arraySignature);

    if (elementType == 'Z') { //bool
        jboolean boolElementValue = (jboolean) value->BooleanValue(isolate);
        jbooleanArray boolArr = static_cast<jbooleanArray>(arr);
        env.SetBooleanArrayRegion(boolArr, index, 1, &boolElementValue);
    } else if (elementType == 'B') { //byte
        jbyte byteElementValue = (jbyte) value->Int32Value(context).ToChecked();
        jbyteArray byteArr = static_cast<jbyteArray>(arr);
        env.SetByteArrayRegion(byteArr, index, 1, &byteElementValue);
    } else if (elementType == 'C') { //char
        jchar charElementValue = ArgConverter::ToJavaChar(isolate, value->ToString(context).ToLocalChecked());
        jcharArray charArr = static_cast<jcharArray>(arr);
        env.SetCharArrayRegion(charArr, index, 1, &charElementValue);
    } else if (elementType == 'S') { //short
        jshort shortElementValue = (jshort) value->Int32Value(context).ToChecked();
        jshortArray shortArr = static_cast<jshortArray>(arr);
        env.SetShortArrayRegion(shortArr, index, 1, &shortElementValue);
    } else if (elementType == 'I') { //int
        jint intElementValue = value->Int32Value(context).ToChecked();
        jintArray intArr = static_cast<jintArray>(arr);
        env.SetIntArrayRegion(intArr, index, 1, &intElementValue);
    } else if (elementType == 'J') { //long
        jlong longElementValue;
        if (value->IsObject()) {
            longElementValue = (jlong) ArgConverter::ConvertToJavaLong(isolate, value);
        } else {
            longElementValue = (jlong) value->IntegerValue(context).ToChecked();
        }
        jlongArray longArr = static_cast<jlongArray>(arr);
        env.SetLongArrayRegion(longArr, index, 1, &longElementValue);
    } else if (elementType == 'F') { //float
        jfloat floatElementValue = (jfloat) value->NumberValue(context).ToChecked();
        jfloatArray floatArr = static_cast<jfloatArray>(arr);
        env.SetFloatArrayRegion(floatArr, index, 1, &floatElementValue);
    } else if (elementType == 'D') { //double
        jdouble doubleElementValue = (jdouble) value->NumberValue(context).ToChecked();
        jdoubleArray doubleArr = static_cast<jdoubleArray>(arr);
        env.SetDoubleArrayRegion(doubleArr, index, 1, &doubleElementValue);
    } else { //string or object
        bool isReferenceType = value->IsObject() || value->IsString();
        if (isReferenceType) {
            auto object = value.As<Object>();

            JsArgToArrayConverter argConverter(isolate, context, value, false, (int) Type::Null);
            if (argConverter.IsValid()) {
                jobjectArray objArr = static_cast<jobjectArray>(arr);
                jobject objectElementValue = argConverter.GetConvertedArg();
                env.SetObjectArrayElement(objArr, index, objectElementValue);
            } else {
                JsArgToArrayConverter::Error err = argConverter.GetError();
                throw NativeScriptException(string(err.msg));
            }
        } else {
            throw NativeScriptException(string("Cannot assign primitive value to array of objects."));
        }
    }
}

void ArrayElementAccessor::assertNonNullNativeArray(tns::JniLocalRef& arrayReference) {
    if(arrayReference.IsNull()){
        throw NativeScriptException("Failed calling indexer operator on native array. The JavaScript instance no longer has available Java instance counterpart.");
    }
}
