#include "ArrayElementAccessor.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "Util.h"
#include "NativeScriptException.h"
#include "Runtime.h"

using namespace v8;
using namespace std;
using namespace tns;

Local<Value> ArrayElementAccessor::GetArrayElement(Isolate* isolate, const Local<Object>& array, uint32_t index, const string& arraySignature) {
    JEnv env;

    EscapableHandleScope handleScope(isolate);
    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();

    auto arr = objectManager->GetJavaObjectByJsObject(array);

    Local<Value> value;
    jsize startIndex = index;
    const jsize length = 1;

    const string elementSignature = arraySignature.substr(1);
    jboolean isCopy = false;

    if (elementSignature == "Z") {
        jbooleanArray boolArr = static_cast<jbooleanArray>(arr);
        jboolean boolArrValue;
        env.GetBooleanArrayRegion(boolArr, startIndex, length, &boolArrValue);
        value = ConvertToJsValue(isolate, objectManager, env, elementSignature, &boolArrValue);
    } else if (elementSignature == "B") {
        jbyteArray byteArr = static_cast<jbyteArray>(arr);
        jbyte byteArrValue;
        env.GetByteArrayRegion(byteArr, startIndex, length, &byteArrValue);
        value = ConvertToJsValue(isolate, objectManager, env, elementSignature, &byteArrValue);
    } else if (elementSignature == "C") {
        jcharArray charArr = static_cast<jcharArray>(arr);
        jchar charArrValue;
        env.GetCharArrayRegion(charArr, startIndex, length, &charArrValue);
        JniLocalRef s(env.NewString(&charArrValue, 1));
        const char* singleChar = env.GetStringUTFChars(s, &isCopy);
        value = ConvertToJsValue(isolate, objectManager, env, elementSignature, singleChar);
        env.ReleaseStringUTFChars(s, singleChar);
    } else if (elementSignature == "S") {
        jshortArray shortArr = static_cast<jshortArray>(arr);
        jshort shortArrValue;
        env.GetShortArrayRegion(shortArr, startIndex, length, &shortArrValue);
        value = ConvertToJsValue(isolate, objectManager, env, elementSignature, &shortArrValue);
    } else if (elementSignature == "I") {
        jintArray intArr = static_cast<jintArray>(arr);
        jint intArrValue;
        env.GetIntArrayRegion(intArr, startIndex, length, &intArrValue);
        value = ConvertToJsValue(isolate, objectManager, env, elementSignature, &intArrValue);
    } else if (elementSignature == "J") {
        jlongArray longArr = static_cast<jlongArray>(arr);
        jlong longArrValue;
        env.GetLongArrayRegion(longArr, startIndex, length, &longArrValue);
        value = ConvertToJsValue(isolate, objectManager, env, elementSignature, &longArrValue);
    } else if (elementSignature == "F") {
        jfloatArray floatArr = static_cast<jfloatArray>(arr);
        jfloat floatArrValue;
        env.GetFloatArrayRegion(floatArr, startIndex, length, &floatArrValue);
        value = ConvertToJsValue(isolate, objectManager, env, elementSignature, &floatArrValue);
    } else if (elementSignature == "D") {
        jdoubleArray doubleArr = static_cast<jdoubleArray>(arr);
        jdouble doubleArrValue;
        env.GetDoubleArrayRegion(doubleArr, startIndex, length, &doubleArrValue);
        value = ConvertToJsValue(isolate, objectManager, env, elementSignature, &doubleArrValue);
    } else {
        jobject result = env.GetObjectArrayElement(static_cast<jobjectArray>(arr), index);
        value = ConvertToJsValue(isolate, objectManager, env, elementSignature, &result);
        env.DeleteLocalRef(result);
    }

    return handleScope.Escape(value);
}

void ArrayElementAccessor::SetArrayElement(Isolate* isolate, const Local<Object>& array, uint32_t index, const string& arraySignature, Local<Value>& value) {
    JEnv env;

    HandleScope handleScope(isolate);
    auto runtime = Runtime::GetRuntime(isolate);
    auto objectManager = runtime->GetObjectManager();
    auto context = isolate->GetCurrentContext();

    auto arr = objectManager->GetJavaObjectByJsObject(array);

    const string elementSignature = arraySignature.substr(1);
    jboolean isCopy = false;

    if (elementSignature == "Z") { //bool
        jboolean boolElementValue = (jboolean) value->BooleanValue(context).ToChecked();
        jbooleanArray boolArr = static_cast<jbooleanArray>(arr);
        env.SetBooleanArrayRegion(boolArr, index, 1, &boolElementValue);
    } else if (elementSignature == "B") { //byte
        jbyte byteElementValue = (jbyte) value->Int32Value(context).ToChecked();
        jbyteArray byteArr = static_cast<jbyteArray>(arr);
        env.SetByteArrayRegion(byteArr, index, 1, &byteElementValue);
    } else if (elementSignature == "C") { //char
        String::Utf8Value utf8(isolate, value->ToString(isolate));
        JniLocalRef s(env.NewString((jchar*) *utf8, 1));
        const char* singleChar = env.GetStringUTFChars(s, &isCopy);
        jchar charElementValue = *singleChar;
        env.ReleaseStringUTFChars(s, singleChar);
        jcharArray charArr = static_cast<jcharArray>(arr);
        env.SetCharArrayRegion(charArr, index, 1, &charElementValue);
    } else if (elementSignature == "S") { //short
        jshort shortElementValue = (jshort) value->Int32Value(context).ToChecked();
        jshortArray shortArr = static_cast<jshortArray>(arr);
        env.SetShortArrayRegion(shortArr, index, 1, &shortElementValue);
    } else if (elementSignature == "I") { //int
        jint intElementValue = value->Int32Value(context).ToChecked();
        jintArray intArr = static_cast<jintArray>(arr);
        env.SetIntArrayRegion(intArr, index, 1, &intElementValue);
    } else if (elementSignature == "J") { //long
        jlong longElementValue;
        if (value->IsObject()) {
            longElementValue = (jlong) ArgConverter::ConvertToJavaLong(isolate, value);
        } else {
            longElementValue = (jlong) value->IntegerValue(context).ToChecked();
        }
        jlongArray longArr = static_cast<jlongArray>(arr);
        env.SetLongArrayRegion(longArr, index, 1, &longElementValue);
    } else if (elementSignature == "F") { //float
        jfloat floatElementValue = (jfloat) value->NumberValue(context).ToChecked();
        jfloatArray floatArr = static_cast<jfloatArray>(arr);
        env.SetFloatArrayRegion(floatArr, index, 1, &floatElementValue);
    } else if (elementSignature == "D") { //double
        jdouble doubleElementValue = (jdouble) value->NumberValue(context).ToChecked();
        jdoubleArray doubleArr = static_cast<jdoubleArray>(arr);
        env.SetDoubleArrayRegion(doubleArr, index, 1, &doubleElementValue);
    } else { //string or object
        bool isReferenceType = value->IsObject() || value->IsString();
        if (isReferenceType) {
            auto object = value.As<Object>();

            JsArgToArrayConverter argConverter(isolate, value, false, (int) Type::Null);
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

Local<Value> ArrayElementAccessor::ConvertToJsValue(Isolate* isolate, ObjectManager* objectManager, JEnv& env, const string& elementSignature, const void* value) {
    Local<Value> jsValue;

    if (elementSignature == "Z") {
        jsValue = Boolean::New(isolate, *(jboolean*) value);
    } else if (elementSignature == "B") {
        jsValue = Integer::New(isolate, *(jbyte*) value);
    } else if (elementSignature == "C") {
        jsValue = ArgConverter::ConvertToV8String(isolate, (const char*) value, 1);
    } else if (elementSignature == "S") {
        jsValue = Integer::New(isolate, *(jshort*) value);
    } else if (elementSignature == "I") {
        jsValue = Integer::New(isolate, *(jint*) value);
    } else if (elementSignature == "J") {
        jsValue = ArgConverter::ConvertFromJavaLong(isolate, *(jlong*) value);
    } else if (elementSignature == "F") {
        jsValue = Number::New(isolate, *(jfloat*) value);
    } else if (elementSignature == "D") {
        jsValue = Number::New(isolate, *(jdouble*) value);
    } else {
        if (nullptr != (*(jobject*) value)) {
            bool isString = elementSignature == "Ljava/lang/String;";

            if (isString) {
                jsValue = ArgConverter::jstringToV8String(isolate, *(jstring*) value);
            } else {
                jint javaObjectID = objectManager->GetOrCreateObjectId(*(jobject*) value);
                jsValue = objectManager->GetJsObjectByJavaObject(javaObjectID);

                if (jsValue.IsEmpty()) {
                    string className;
                    if (elementSignature[0] == '[') {
                        className = Util::JniClassPathToCanonicalName(elementSignature);
                    } else {
                        className = objectManager->GetClassName(*(jobject*) value);
                    }

                    jsValue = objectManager->CreateJSWrapper(javaObjectID, className);
                }
            }
        } else {
            jsValue = Null(isolate);
        }
    }

    return jsValue;
}
