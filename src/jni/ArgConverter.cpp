#include "ArgConverter.h"
#include "ObjectManager.h"
#include "JniLocalRef.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "NativeScriptAssert.h"
#include "JType.h"
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
	args.This()->SetHiddenValue(V8StringConstants::GetMarkedAsLong(), args[0]);
	args.This()->Set(V8StringConstants::GetValue(), args[0]);

	args.This()->SetPrototype(Local<NumberObject>::New(Isolate::GetCurrent(), *NAN_NUMBER_OBJECT));
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
		Type argTypeID = (Type)JType::IntValue(env, argTypeIDObj);

		Handle<Value> jsArg;
		Handle<String> v8String;
		switch (argTypeID)
		{
			case Type::Boolean:
				jsArg = Boolean::New(isolate, JType::BooleanValue(env, arg));
				break;
			case Type::Char:
				v8String = jcharToV8String(JType::CharValue(env, arg));
				jsArg = v8String;
				break;
			case Type::Byte:
				jsArg = Number::New(isolate, JType::ByteValue(env, arg));
				break;
			case Type::Short:
				jsArg = Number::New(isolate, JType::ShortValue(env, arg));
				break;
			case Type::Int:
				jsArg = Number::New(isolate, JType::IntValue(env, arg));
				break;
			case Type::Long:
				jsArg = Number::New(isolate, JType::LongValue(env, arg));
				break;
			case Type::Float:
				jsArg = Number::New(isolate, JType::FloatValue(env, arg));
				break;
			case Type::Double:
				jsArg = Number::New(isolate, JType::DoubleValue(env, arg));
				break;
			case Type::String:
				v8String = jstringToV8String((jstring)arg);
				jsArg = v8String;
				break;
			case Type::JsObject:
			{
				jint javaObjectID = JType::IntValue(env, arg);
				jsArg = ObjectManager::GetJsObjectByJavaObjectStatic(javaObjectID);

				if (jsArg.IsEmpty())
				{
					string argClassName = jstringToString(ObjectToString(argJavaClassPath));
					argClassName = Util::ConvertFromCanonicalToJniName(argClassName);
					jsArg = ObjectManager::CreateJSWrapperStatic(javaObjectID, argClassName);
				}
				break;
			}
			case Type::Null:
				jsArg = Null(isolate);
				break;
		}

		arr->Set(i, jsArg);
	}

	return arr;
}

std::string ArgConverter::jstringToString(jstring value)
{
	if (value == nullptr) {
		return string();
	}

	jsize utfLength;
	bool readInBuffer = ReadJStringInBuffer(value, utfLength);
	if(readInBuffer) {
		string s(charBuffer, utfLength);
		return s;
	}

	JEnv env;

	jboolean f = false;
	const char* chars = env.GetStringUTFChars(value, &f);
	string s(chars);
	env.ReleaseStringUTFChars(value, chars);

	return s;
}

Local<String> ArgConverter::jstringToV8String(jstring value)
{
	if	(value == nullptr)
	{
		return Handle<String>();
	}

	JEnv env;
	auto chars = env.GetStringChars(value, NULL);
	auto length = env.GetStringLength(value);
	auto v8String = ConvertToV8String(chars, length);
	env.ReleaseStringChars(value, chars);

	return v8String;
}

bool ArgConverter::ReadJStringInBuffer(jstring value, jsize& utfLength) {
	if (value == nullptr) {
		return false;
	}

	JEnv env;
	utfLength = env.GetStringUTFLength(value);
	if(utfLength > BUFFER_SIZE) {
		return false;
	}

	jsize strLength = env.GetStringLength(value);
	// use existing buffer to prevent extensive memory allocation
	env.GetStringUTFRegion(value, (jsize)0, strLength, charBuffer);

	return true;
}

Handle<String> ArgConverter::jcharToV8String(jchar value)
{
	auto v8String = ConvertToV8String(&value, 1);
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
char* ArgConverter::charBuffer = new char[ArgConverter::BUFFER_SIZE];
