#include "ArrayElementAccessor.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "JniLocalRef.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "NativeScriptAssert.h"
#include <assert.h>

using namespace v8;
using namespace std;
using namespace tns;

void ArrayElementAccessor::Init(JavaVM *jvm, ObjectManager *objectManager)
{
	this->jvm = jvm;
	this->objectManager = objectManager;
}

Handle<Value> ArrayElementAccessor::GetArrayElement(const Handle<Object>& array, uint32_t index, const string& arraySignature)
{
	JEnv env;

	Isolate* isolate = Isolate::GetCurrent();
	EscapableHandleScope handleScope(isolate);

	jweak arr = objectManager->GetJavaObjectByJsObject(array);

	Local<Value> value;
	jsize startIndex = index;
	const jsize length = 1;

	const string elementSignature = arraySignature.substr(1);
	jboolean isCopy = false;

	if (elementSignature == "Z")
	{
		jbooleanArray boolArr = reinterpret_cast<jbooleanArray>(arr);
		jboolean boolArrValue;
		env.GetBooleanArrayRegion(boolArr, startIndex, length, &boolArrValue);
		value = CheckForArrayAccessException(env, elementSignature, &boolArrValue);
	}
	else if (elementSignature == "B")
	{
		jbyteArray byteArr = reinterpret_cast<jbyteArray>(arr);
		jbyte byteArrValue;
		env.GetByteArrayRegion(byteArr, startIndex, length, &byteArrValue);
		value = CheckForArrayAccessException(env, elementSignature, &byteArrValue);
	}
	else if (elementSignature == "C")
	{
		jcharArray charArr = reinterpret_cast<jcharArray>(arr);
		jchar charArrValue;
		env.GetCharArrayRegion(charArr, startIndex, length, &charArrValue);
		JniLocalRef s(env.NewString(&charArrValue, 1));
		const char* singleChar = env.GetStringUTFChars(s, &isCopy);
		value = CheckForArrayAccessException(env, elementSignature, singleChar);
		env.ReleaseStringUTFChars(s, singleChar);
	}
	else if (elementSignature == "S")
	{
		jshortArray shortArr = reinterpret_cast<jshortArray>(arr);
		jshort shortArrValue;
		env.GetShortArrayRegion(shortArr, startIndex, length, &shortArrValue);
		value = CheckForArrayAccessException(env, elementSignature, &shortArrValue);
	}
	else if (elementSignature == "I")
	{
		jintArray intArr = reinterpret_cast<jintArray>(arr);
		jint intArrValue;
		env.GetIntArrayRegion(intArr, startIndex, length, &intArrValue);
		value = CheckForArrayAccessException(env, elementSignature, &intArrValue);
	}
	else if (elementSignature == "J")
	{
		jlongArray longArr = reinterpret_cast<jlongArray>(arr);
		jlong longArrValue;
		env.GetLongArrayRegion(longArr, startIndex, length, &longArrValue);
		value = CheckForArrayAccessException(env, elementSignature, &longArrValue);
	}
	else if (elementSignature == "F")
	{
		jfloatArray floatArr = reinterpret_cast<jfloatArray>(arr);
		jfloat floatArrValue;
		env.GetFloatArrayRegion(floatArr, startIndex, length, &floatArrValue);
		value = CheckForArrayAccessException(env, elementSignature, &floatArrValue);
	}
	else if (elementSignature == "D")
	{
		jdoubleArray doubleArr = reinterpret_cast<jdoubleArray>(arr);
		jdouble doubleArrValue;
		env.GetDoubleArrayRegion(doubleArr, startIndex, length, &doubleArrValue);
		value = CheckForArrayAccessException(env, elementSignature, &doubleArrValue);
	}
	else
	{
		jobject result = env.GetObjectArrayElement(reinterpret_cast<jobjectArray>(arr), index);
		value = CheckForArrayAccessException(env, elementSignature, &result);
		env.DeleteLocalRef(result);
	}

	return handleScope.Escape(value);
}


void ArrayElementAccessor::SetArrayElement(const Handle<Object>& array, uint32_t index, const string& arraySignature, Handle<Value>& value)
{
	JEnv env;

	Isolate* isolate = Isolate::GetCurrent();
	HandleScope handleScope(isolate);

	jweak arr = objectManager->GetJavaObjectByJsObject(array);

	const string elementSignature = arraySignature.substr(1);
	jboolean isCopy = false;

	if (elementSignature == "Z")
	{
		jboolean boolElementValue = (jboolean) value->BooleanValue();
		jbooleanArray boolArr = reinterpret_cast<jbooleanArray>(arr);
		env.SetBooleanArrayRegion(boolArr, index, 1, &boolElementValue);
	}
	else if (elementSignature == "B")
	{
		jbyte byteElementValue = (jbyte) value->Int32Value();
		jbyteArray byteArr = reinterpret_cast<jbyteArray>(arr);
		env.SetByteArrayRegion(byteArr, index, 1, &byteElementValue);
	}
	else if (elementSignature == "C")
	{
		String::Utf8Value utf8(value->ToString());
		JniLocalRef s(env.NewString((jchar*) *utf8, 1));
		const char* singleChar = env.GetStringUTFChars(s, &isCopy);
		jchar charElementValue = *singleChar;
		env.ReleaseStringUTFChars(s, singleChar);
		jcharArray charArr = reinterpret_cast<jcharArray>(arr);
		env.SetCharArrayRegion(charArr, index, 1, &charElementValue);
	}
	else if (elementSignature == "S")
	{
		jshort shortElementValue = (jshort) value->Int32Value();
		jshortArray shortArr = reinterpret_cast<jshortArray>(arr);
		env.SetShortArrayRegion(shortArr, index, 1, &shortElementValue);
	}
	else if (elementSignature == "I")
	{
		jint intElementValue = value->Int32Value();
		jintArray intArr = reinterpret_cast<jintArray>(arr);
		env.SetIntArrayRegion(intArr, index, 1, &intElementValue);
	}
	else if (elementSignature == "J")
	{
		jlong longElementValue;
		if (value->IsObject())
		{
			longElementValue = (jlong) ArgConverter::ConvertToJavaLong(value);
		}
		else
		{
			longElementValue = (jlong) value->IntegerValue();
		}
		jlongArray longArr = reinterpret_cast<jlongArray>(arr);
		env.SetLongArrayRegion(longArr, index, 1, &longElementValue);
	}
	else if (elementSignature == "F")
	{
		jfloat floatElementValue = (jfloat) value->NumberValue();
		jfloatArray floatArr = reinterpret_cast<jfloatArray>(arr);
		env.SetFloatArrayRegion(floatArr, index, 1, &floatElementValue);
	}
	else if (elementSignature == "D")
	{
		jdouble doubleElementValue = (jdouble) value->NumberValue();
		jdoubleArray doubleArr = reinterpret_cast<jdoubleArray>(arr);
		env.SetDoubleArrayRegion(doubleArr, index, 1, &doubleElementValue);
	}
	else
	{
		bool isReferenceType = value->IsObject() || value->IsString();
		if (isReferenceType)
		{
			auto object = value.As<Object>();

			JsArgToArrayConverter argConverter(value, false);
			if (argConverter.IsValid())
			{
				jobjectArray objArr = reinterpret_cast<jobjectArray>(arr);
				jobject objectElementValue = argConverter.GetConvertedArg();
				env.SetObjectArrayElement(objArr, index, objectElementValue);
			}
			else
			{
				JsArgToArrayConverter::Error err = argConverter.GetError();
				ExceptionUtil::GetInstance()->ThrowExceptionToJs(err.msg);
				return;
			}
		}
		else
		{
			ExceptionUtil::GetInstance()->ThrowExceptionToJs("Cannot assign primitive value to array of objects.");
			return;
		}
	}
}

Handle<Value> ArrayElementAccessor::CheckForArrayAccessException(JEnv& env, const string& elementSignature, const void *value)
{
	Handle<Value> jsValue;

	auto isolate = Isolate::GetCurrent();

	JniLocalRef exc(env.ExceptionOccurred());
	if (nullptr != (jthrowable) exc)
	{
		DEBUG_WRITE("Error during getting array element");
		// env.ExceptionDescribe(); We will print this manually in the ExceptionUtil
		env.ExceptionClear();
		string errMsg;
		ExceptionUtil::GetInstance()->GetExceptionMessage(env, exc, errMsg);
		ExceptionUtil::GetInstance()->ThrowExceptionToJs(errMsg);
	}
	else if (elementSignature == "Z")
	{
		jsValue = Boolean::New(isolate, *(jboolean*) value);
	}
	else if (elementSignature == "B")
	{
		jsValue = Integer::New(isolate, *(jbyte*) value);
	}
	else if (elementSignature == "C")
	{
		jsValue = ConvertToV8String((const char*) value, 1);
	}
	else if (elementSignature == "S")
	{
		jsValue = Integer::New(isolate, *(jshort*) value);
	}
	else if (elementSignature == "I")
	{
		jsValue = Integer::New(isolate, *(jint*) value);
	}
	else if (elementSignature == "J")
	{
		jsValue = ArgConverter::ConvertFromJavaLong(*(jlong*) value);
	}
	else if (elementSignature == "F")
	{
		jsValue = Number::New(isolate, *(jfloat*) value);
	}
	else if (elementSignature == "D")
	{
		jsValue = Number::New(isolate, *(jdouble*) value);
	}
	else
	{
		if (nullptr != (*(jobject*) value))
		{
			bool isString = elementSignature == "Ljava/lang/String;";

			if (isString)
			{
				jsValue = ArgConverter::jstringToV8String(*(jstring*) value);
			}
			else
			{
				jint javaObjectID = objectManager->GetOrCreateObjectId(*(jobject*) value);
				jsValue = objectManager->GetJsObjectByJavaObject(javaObjectID);

				if (jsValue.IsEmpty())
				{
					string className;
					if (elementSignature[0] == '[')
					{
						className = Util::JniClassPathToCanonicalName(elementSignature);
					}
					else
					{
						className = objectManager->GetClassName(*(jobject*) value);
					}

					jsValue = objectManager->CreateJSWrapper(javaObjectID, className);
				}
			}
		}
		else
		{
			jsValue = Null(isolate);
		}
	}

	return jsValue;
}
