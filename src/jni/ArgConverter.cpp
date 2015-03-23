#include "ArgConverter.h"
#include "ObjectManager.h"
#include "JniLocalRef.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "NativeScriptAssert.h"
#include <assert.h>
#include <sstream>
#include <cstdlib>

using namespace v8;
using namespace std;
using namespace tns;

void ArgConverter::Init(JavaVM *jvm)
{
	ArgConverter::jvm = jvm;

	auto isolate = Isolate::GetCurrent();

	auto ft = FunctionTemplate::New(isolate, ArgConverter::NativeScriptLongFunctionCallback);
	ft->SetClassName(V8StringConstants::GetLongNumber());
	ft->InstanceTemplate()->Set(V8StringConstants::GetValueOf(), FunctionTemplate::New(isolate, ArgConverter::NativeScriptLongValueOfFunctionCallback));
	ft->InstanceTemplate()->Set(V8StringConstants::GetToString(), FunctionTemplate::New(isolate, ArgConverter::NativeScriptLongToStringFunctionCallback));
	NATIVESCRIPT_NUMERA_CTOR_FUNC = new Persistent<Function>(isolate, ft->GetFunction());

	auto nanObject = Number::New(isolate, numeric_limits<double>::quiet_NaN()).As<NumberObject>();
	NAN_NUMBER_OBJECT = new Persistent<NumberObject>(isolate, nanObject);

	JEnv env;

	STRING_CLASS = env.FindClass("java/lang/String");
	assert(STRING_CLASS != nullptr);

	GET_BYTES_METHOD_ID = env.GetMethodID(STRING_CLASS, "getBytes", "(Ljava/lang/String;)[B");
	assert(GET_BYTES_METHOD_ID != nullptr);

	JniLocalRef encoding(env.NewStringUTF("UTF-8"));
	UTF_8_ENCODING = (jstring)env.NewGlobalRef(encoding);
	assert(UTF_8_ENCODING != nullptr);
}

void ArgConverter::NativeScriptLongValueOfFunctionCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	auto isolate = Isolate::GetCurrent();
	args.GetReturnValue().Set(Number::New(isolate, numeric_limits<double>::quiet_NaN()));
}

void ArgConverter::NativeScriptLongToStringFunctionCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	args.GetReturnValue().Set(args.This()->Get(V8StringConstants::GetValue()));
}

void ArgConverter::NativeScriptLongFunctionCallback(const v8::FunctionCallbackInfo<Value>& args)
{
	auto isolate = Isolate::GetCurrent();
	args.This()->SetHiddenValue(V8StringConstants::GetJavaLong(), Boolean::New(isolate, true));
	args.This()->SetHiddenValue(ConvertToV8String(V8StringConstants::MARKED_AS_LONG), args[0]);
	args.This()->Set(V8StringConstants::GetValue(), args[0], (PropertyAttribute)(ReadOnly | DontDelete));

	args.This()->SetPrototype(Handle<NumberObject>::New(Isolate::GetCurrent(), *NAN_NUMBER_OBJECT));
}


jlong ArgConverter::ObjectToLong(jobject object)
{
	JEnv env;

	JniLocalRef argClass(env.GetObjectClass(object));

	//TODO: Lubo: cache this methodId on startup
	jmethodID methodID = env.GetMethodID(argClass, "longValue", "()J");

	return env.CallLongMethod(object, methodID);
}

jboolean ArgConverter::ObjectToBoolean(jobject object)
{
	JEnv env;

	JniLocalRef argClass(env.GetObjectClass(object));

	//TODO: Lubo: cache this methodId on startup
	jmethodID methodID = env.GetMethodID(argClass, "booleanValue", "()Z");

	return env.CallBooleanMethod(object, methodID);
}

jchar ArgConverter::ObjectToChar(jobject object)
{
	JEnv env;

	JniLocalRef argClass(env.GetObjectClass(object));

	//TODO: Lubo: cache this methodId on startup
	jmethodID methodID = env.GetMethodID(argClass, "characterValue", "()C");

	return env.CallCharMethod(object, methodID);
}


jbyte ArgConverter::ObjectToByte(jobject object)
{
	JEnv env;

	JniLocalRef argClass(env.GetObjectClass(object));

	//TODO: Lubo: cache this methodId on startup
	jmethodID methodID = env.GetMethodID(argClass, "byteValue", "()B");

	return env.CallByteMethod(object, methodID);
}

jshort ArgConverter::ObjectToShort(jobject object)
{
	JEnv env;

	JniLocalRef argClass(env.GetObjectClass(object));

	//TODO: Lubo: cache this methodId on startup
	jmethodID methodID = env.GetMethodID(argClass, "shortValue", "()S");

	return env.CallShortMethod(object, methodID);
}

jint ArgConverter::ObjectToInt(jobject object)
{
	JEnv env;

	JniLocalRef argClass(env.GetObjectClass(object));

	//TODO: Lubo: cache this methodId on startup
	jmethodID methodID = env.GetMethodID(argClass, "intValue", "()I");

	return env.CallIntMethod(object, methodID);
}


jfloat ArgConverter::ObjectToFloat(jobject object)
{
	JEnv env;

	JniLocalRef argClass(env.GetObjectClass(object));

	//TODO: Lubo: cache this methodId on startup
	jmethodID methodID = env.GetMethodID(argClass, "floatValue", "()F");

	return env.CallFloatMethod(object, methodID);
}

jdouble ArgConverter::ObjectToDouble(jobject object)
{
	JEnv env;

	JniLocalRef argClass(env.GetObjectClass(object));

	//TODO: Lubo: cache this methodId on startup
	jmethodID methodID = env.GetMethodID(argClass, "doubleValue", "()D");

	return env.CallDoubleMethod(object, methodID);
}


jstring ArgConverter::ObjectToString(jobject object)
{
	return (jstring)object;
}

Handle<Array> ArgConverter::ConvertJavaArgsToJsArgs(jobjectArray args)
{
	JEnv env;

	auto isolate = Isolate::GetCurrent();

	int argc = env.GetArrayLength(args) / 3;
	Handle<Array> arr(Array::New(isolate, argc));

	int jArrayIndex = 0;
	for (int i = 0; i < argc; i++)
	{
		jobject argTypeIDObj = env.GetObjectArrayElement(args, jArrayIndex++);
		jobject arg = env.GetObjectArrayElement(args, jArrayIndex++);
		jobject argJavaClassPath = env.GetObjectArrayElement(args, jArrayIndex++);

		jint length;
		jint argTypeID = ObjectToInt(argTypeIDObj);

		Handle<Value> jsArg;
		Handle<String> v8String;
		switch (argTypeID)
		{
			case TypeID_Boolean : jsArg = Boolean::New(isolate, ObjectToBoolean(arg)); break;
			case TypeID_Char:
				v8String = jcharToV8String(ObjectToChar(arg));
				jsArg = v8String;
				break;
			case TypeID_Byte: jsArg = Number::New(isolate, ObjectToByte(arg)); break;
			case TypeID_Short: jsArg = Number::New(isolate, ObjectToShort(arg)); break;
			case TypeID_Int: jsArg = Number::New(isolate, ObjectToInt(arg)); break;
			case TypeID_Long: jsArg = Number::New(isolate, ObjectToLong(arg)); break;
			case TypeID_Float: jsArg = Number::New(isolate, ObjectToFloat(arg)); break;
			case TypeID_Double: jsArg = Number::New(isolate, ObjectToDouble(arg)); break;
			case TypeID_String:
				v8String = jstringToV8String(ObjectToString(arg));
				jsArg = v8String;
				break;
			case TypeID_JsObject:
			{
				jint javaObjectID = ObjectToInt(arg);
				jsArg = ObjectManager::GetJsObjectByJavaObjectStatic(javaObjectID);

				if (jsArg.IsEmpty())
				{
					string argClassName = jstringToString(ObjectToString(argJavaClassPath));
					argClassName = Util::ConvertFromCanonicalToJniName(argClassName);
					jsArg = ObjectManager::CreateJSWrapperStatic(javaObjectID, argClassName);
				}
				break;
			}
			case TypeID_Null: jsArg = Null(isolate); break;
		}

		arr->Set(i, jsArg);
	}

	return arr;
}

std::string ArgConverter::jstringToString(jstring value)
{
	if (value == nullptr)
		return string();

	JEnv env;

	jboolean f = false;
	const char* chars = env.GetStringUTFChars(value, &f);
	string s(chars);
	env.ReleaseStringUTFChars(value, chars);

	return s;
}

Handle<String> ArgConverter::jcharToV8String(jchar value)
{
	JEnv env;

	jstring str = env.NewString(&value, 1);
	jboolean bol = true;
	const char* resP = env.GetStringUTFChars(str, &bol);
	auto v8String = ConvertToV8String(resP, 1);
	env.ReleaseStringUTFChars(str, resP);
	return v8String;
}

Local<String> ArgConverter::jstringToV8String(jstring value)
{
	if	(value == nullptr)
	{
		return Handle<String>();
	}

	JEnv env;

	JniLocalRef arr(env.CallObjectMethod(value, GET_BYTES_METHOD_ID, UTF_8_ENCODING));
	int length = env.GetArrayLength(arr);
	jbyte *data = env.GetByteArrayElements(arr, nullptr);
	auto v8String = ConvertToV8String((const char *)data, length);
	env.ReleaseByteArrayElements(arr, data, JNI_ABORT);

	return v8String;
}

Local<Value> ArgConverter::ConvertFromJavaLong(jlong value)
{
	Local<Value> convertedValue;
	long long longValue = value;

	auto isolate = Isolate::GetCurrent();

	if ((-JS_LONG_LIMIT < longValue) && (longValue < JS_LONG_LIMIT))
	{
		convertedValue = Number::New(isolate, longValue);
	}
	else
	{
		char strNumber[24];
		sprintf(strNumber, "%lld", longValue);
		Handle<Value> strValue = ConvertToV8String(strNumber);
		convertedValue = Local<Function>::New(isolate, *NATIVESCRIPT_NUMERA_CTOR_FUNC)->CallAsConstructor(1, &strValue);
	}

	return convertedValue;
}


int64_t ArgConverter::ConvertToJavaLong(const Handle<Value>& value)
{
	assert(!value.IsEmpty());

	auto obj = Handle<Object>::Cast(value);

	assert(!obj.IsEmpty());

	auto valueProp = obj->Get(V8StringConstants::GetValue());

	assert(!valueProp.IsEmpty());

	string num = ConvertToString(valueProp->ToString());

	int64_t longValue = atoll(num.c_str());

	return longValue;
}

bool ArgConverter::TryConvertToJavaLong(const Handle<Value>& value, jlong& javaLong)
{
	bool success = false;

	if (!value.IsEmpty())
	{
		if (value->IsNumber() || value->IsNumberObject())
		{
			javaLong = (jlong)value->IntegerValue();
			success = true;
		}
		else if (value->IsObject())
		{
			auto obj = Handle<Object>::Cast(value);
			auto isJavaLongValue = obj->GetHiddenValue(V8StringConstants::GetJavaLong());
			if (!isJavaLongValue.IsEmpty() && isJavaLongValue->BooleanValue())
			{
				javaLong = (jlong)ConvertToJavaLong(value);
				success = true;
			}
		}
	}

	return success;
}



JavaVM* ArgConverter::jvm = nullptr;
Persistent<Function>* ArgConverter::NATIVESCRIPT_NUMERA_CTOR_FUNC = nullptr;
Persistent<NumberObject>* ArgConverter::NAN_NUMBER_OBJECT = nullptr;
jclass ArgConverter::STRING_CLASS = nullptr;
jmethodID ArgConverter::GET_BYTES_METHOD_ID = nullptr;
jstring ArgConverter::UTF_8_ENCODING = nullptr;
