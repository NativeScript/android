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

	auto charClass = env.FindClass("java/lang/Character");
	assert(charClass != nullptr);
	CHAR_VALUE_METHOD_ID = env.GetMethodID(charClass, "charValue", "()C");
	assert(CHAR_VALUE_METHOD_ID != nullptr);

	auto booleanClass = env.FindClass("java/lang/Boolean");
	assert(booleanClass != nullptr);
	BOOLEAN_VALUE_METHOD_ID = env.GetMethodID(booleanClass, "booleanValue", "()Z");
	assert(BOOLEAN_VALUE_METHOD_ID != nullptr);

	auto byteClass = env.FindClass("java/lang/Byte");
	assert(byteClass != nullptr);
	BYTE_VALUE_METHOD_ID = env.GetMethodID(byteClass, "byteValue", "()B");
	assert(BYTE_VALUE_METHOD_ID != nullptr);

	auto shortClass = env.FindClass("java/lang/Short");
	assert(shortClass != nullptr);
	SHORT_VALUE_METHOD_ID = env.GetMethodID(shortClass, "shortValue", "()S");
	assert(SHORT_VALUE_METHOD_ID != nullptr);

	auto integerClass = env.FindClass("java/lang/Integer");
	assert(integerClass != nullptr);
	INT_VALUE_METHOD_ID = env.GetMethodID(integerClass, "intValue", "()I");
	assert(INT_VALUE_METHOD_ID != nullptr);

	auto longClass = env.FindClass("java/lang/Long");
	assert(longClass != nullptr);
	LONG_VALUE_METHOD_ID = env.GetMethodID(longClass, "longValue", "()J");
	assert(LONG_VALUE_METHOD_ID != nullptr);

	auto floatClass = env.FindClass("java/lang/Float");
	assert(floatClass != nullptr);
	FLOAT_VALUE_METHOD_ID = env.GetMethodID(floatClass, "floatValue", "()F");
	assert(FLOAT_VALUE_METHOD_ID != nullptr);

	auto doubleClass = env.FindClass("java/lang/Double");
	assert(doubleClass != nullptr);
	DOUBLE_VALUE_METHOD_ID = env.GetMethodID(doubleClass, "doubleValue", "()D");
	assert(DOUBLE_VALUE_METHOD_ID != nullptr);
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
	args.This()->Set(V8StringConstants::GetValue(), args[0]);

	args.This()->SetPrototype(Local<NumberObject>::New(Isolate::GetCurrent(), *NAN_NUMBER_OBJECT));
}


jlong ArgConverter::ObjectToLong(jobject object)
{
	JEnv env;

	return env.CallLongMethod(object, LONG_VALUE_METHOD_ID);
}

jboolean ArgConverter::ObjectToBoolean(jobject object)
{
	JEnv env;

	return env.CallBooleanMethod(object, BOOLEAN_VALUE_METHOD_ID);
}

jchar ArgConverter::ObjectToChar(jobject object)
{
	JEnv env;

	return env.CallCharMethod(object, CHAR_VALUE_METHOD_ID);
}


jbyte ArgConverter::ObjectToByte(jobject object)
{
	JEnv env;

	return env.CallByteMethod(object, BYTE_VALUE_METHOD_ID);
}

jshort ArgConverter::ObjectToShort(jobject object)
{
	JEnv env;

	return env.CallShortMethod(object, SHORT_VALUE_METHOD_ID);
}

jint ArgConverter::ObjectToInt(jobject object)
{
	JEnv env;

	return env.CallIntMethod(object, INT_VALUE_METHOD_ID);
}


jfloat ArgConverter::ObjectToFloat(jobject object)
{
	JEnv env;

	return env.CallFloatMethod(object, FLOAT_VALUE_METHOD_ID);
}

jdouble ArgConverter::ObjectToDouble(jobject object)
{
	JEnv env;

	return env.CallDoubleMethod(object, DOUBLE_VALUE_METHOD_ID);
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
		JniLocalRef argTypeIDObj(env.GetObjectArrayElement(args, jArrayIndex++));
		JniLocalRef arg(env.GetObjectArrayElement(args, jArrayIndex++));
		JniLocalRef argJavaClassPath(env.GetObjectArrayElement(args, jArrayIndex++));

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
	env.DeleteLocalRef(value);

	return s;
}

Handle<String> ArgConverter::jcharToV8String(jchar value)
{
	JEnv env;

	JniLocalRef str(env.NewString(&value, 1));
	jboolean bol = true;
	const char* resP = env.GetStringUTFChars(str, &bol);
	auto v8String = ConvertToV8String(resP, 1);
	env.ReleaseStringUTFChars(str, resP);
	env.DeleteLocalRef(str);
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
	env.DeleteLocalRef(arr);
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
jmethodID ArgConverter::CHAR_VALUE_METHOD_ID = nullptr;
jmethodID ArgConverter::BOOLEAN_VALUE_METHOD_ID = nullptr;
jmethodID ArgConverter::BYTE_VALUE_METHOD_ID = nullptr;
jmethodID ArgConverter::SHORT_VALUE_METHOD_ID = nullptr;
jmethodID ArgConverter::INT_VALUE_METHOD_ID = nullptr;
jmethodID ArgConverter::LONG_VALUE_METHOD_ID = nullptr;
jmethodID ArgConverter::FLOAT_VALUE_METHOD_ID = nullptr;
jmethodID ArgConverter::DOUBLE_VALUE_METHOD_ID = nullptr;
jstring ArgConverter::UTF_8_ENCODING = nullptr;
