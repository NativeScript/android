#include "JsArgToArrayConverter.h"
#include <limits>
#include <sstream>
#include <assert.h>
#include "ObjectManager.h"
#include "NativeScriptAssert.h"
#include "ArgConverter.h"
#include "JniLocalRef.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "JavaObjectArrayCache.h"

using namespace v8;
using namespace std;
using namespace tns;

JsArgToArrayConverter::JsArgToArrayConverter(const v8::Handle<Value>& arg, bool isImplementationObject) :
		m_arr(nullptr), m_argsAsObject(nullptr), m_argsLen(0), m_isValid(false), m_error(Error())
{
	if (!isImplementationObject)
	{
		m_argsLen = 1;
		m_argsAsObject = new jobject[m_argsLen];
		memset(m_argsAsObject, 0, m_argsLen * sizeof(jobject));

		m_isValid = ConvertArg(arg, 0);
	}
}


JsArgToArrayConverter::JsArgToArrayConverter(const v8::FunctionCallbackInfo<Value>& args, bool hasImplementationObject, const Handle<Object>& outerThis) :
		m_arr(nullptr), m_argsAsObject(nullptr), m_argsLen(0), m_isValid(false), m_error(Error())
{
	auto isInnerClass = !outerThis.IsEmpty();
	if (isInnerClass)
	{
		m_argsLen = args.Length() + 1;
	}
	else
	{
		m_argsLen = !hasImplementationObject ? args.Length() : args.Length() - 2;
	}

	bool success = true;

	if (m_argsLen > 0)
	{
		m_argsAsObject = new jobject[m_argsLen];
		memset(m_argsAsObject, 0, m_argsLen * sizeof(jobject));

		for (int i = 0; i < m_argsLen; i++)
		{
			if (isInnerClass)
			{
				if (i == 0)
				{
					success = ConvertArg(outerThis, i);
				}
				else
				{
					success = ConvertArg(args[i - 1], i);
				}
			}
			else
			{
				success = ConvertArg(args[i], i);
			}

			if (!success)
			{
				break;
			}
		}
	}

	m_isValid = success;
}


bool JsArgToArrayConverter::ConvertArg(const Handle<Value>& arg, int index)
{
	bool success = false;
	stringstream s;

	JEnv env;

	if (arg.IsEmpty())
	{
		s << "Cannot convert empty JavaScript object";
		success = false;
	}
	else if (arg->IsInt32())
	{
		jint value = arg->Int32Value();
		jclass clazz = env.FindClass("java/lang/Integer");
		jmethodID ctor = env.GetMethodID(clazz, "<init>", "(I)V");
		JniLocalRef javaObject(env.NewObject(clazz, ctor, value));
		SetConvertedObject(env, index, javaObject);

		success = true;
	}
	else if (arg->IsNumber() || arg->IsNumberObject())
	{
		double d = arg->NumberValue();
		int64_t i = (int64_t)d;
		bool isInteger = d == i;

		if (isInteger)
		{
			string className;
			string initSignature;
			if ((INT_MIN <= i) && (i < INT_MAX))
			{
				className = "java/lang/Integer";
				initSignature = "(I)V";
			}
			else
			{
				className = "java/lang/Long";
				initSignature = "(J)V";
			}

			jclass clazz = env.FindClass(className);
			jmethodID ctor = env.GetMethodID(clazz, "<init>", initSignature.c_str());
			JniLocalRef javaObject(env.NewObject(clazz, ctor, (jint)d));
			SetConvertedObject(env, index, javaObject);

			success = true;
		}
		else
		{
			jclass clazz = env.FindClass("java/lang/Double");
			jmethodID ctor = env.GetMethodID(clazz, "<init>", "(D)V");
			JniLocalRef javaObject(env.NewObject(clazz, ctor, (jdouble)d));
			SetConvertedObject(env, index, javaObject);

			success = true;
		}
	}
	else if (arg->IsBoolean())
	{
		jboolean value = arg->BooleanValue();
		jclass clazz = env.FindClass("java/lang/Boolean");
		jmethodID ctor = env.GetMethodID(clazz, "<init>", "(Z)V");
		JniLocalRef javaObject(env.NewObject(clazz, ctor, value));
		SetConvertedObject(env, index, javaObject);

		success = true;
	}
	else if (arg->IsBooleanObject())
	{
		auto boolObj = Handle<BooleanObject>::Cast(arg);
		jboolean value = boolObj->BooleanValue() ? JNI_TRUE : JNI_FALSE;
		jclass clazz = env.FindClass("java/lang/Boolean");
		jmethodID ctor = env.GetMethodID(clazz, "<init>", "(Z)V");
		JniLocalRef javaObject(env.NewObject(clazz, ctor, value));
		SetConvertedObject(env, index, javaObject);

		success = true;
	}
	else if (arg->IsString() || arg->IsStringObject())
	{
		Handle<String> argAsString;
		if (arg->IsStringObject())
		{
			auto stringObject = *reinterpret_cast<v8::Handle<StringObject>*>(const_cast<v8::Handle<Value>*>(&arg));
			argAsString = stringObject->ValueOf();
		}
		else
		{
			argAsString = arg->ToString();
		}

		String::Utf8Value stringValue(argAsString);
		int strLength = stringValue.length();

		JniLocalRef strData(env.NewByteArray(strLength));
		env.SetByteArrayRegion((jbyteArray)strData, 0, strLength, (jbyte*)*stringValue);
		JniLocalRef stringObject(env.NewObject(STRING_CLASS, STRING_CTOR, (jbyteArray)strData, UTF_8_ENCODING));
		SetConvertedObject(env, index, stringObject);

		success = true;
	}
	else if (arg->IsObject())
	{
		Local<Object> objectWithHiddenID = arg->ToObject();

		auto hidden = objectWithHiddenID->GetHiddenValue(V8StringConstants::GetMarkedAsLong());
		if (!hidden.IsEmpty())
		{
			jlong value = 0;
			if (hidden->IsString())
			{
				auto strValue = ConvertToString(hidden->ToString());
				value = atoll(strValue.c_str());
			}
			else if (hidden->IsInt32())
			{
				value = hidden->ToInt32()->Int32Value();
			}

			jclass clazz = env.FindClass("java/lang/Long");
			jmethodID ctor = env.GetMethodID(clazz, "<init>", "(J)V");
			JniLocalRef javaObject(env.NewObject(clazz, ctor, value));
			SetConvertedObject(env, index, javaObject);
			return true;
		}

		hidden = objectWithHiddenID->GetHiddenValue(V8StringConstants::GetMarkedAsByte());
		if (!hidden.IsEmpty())
		{
			jbyte value = 0;
			if (hidden->IsString())
			{
				string value = ConvertToString(hidden->ToString());
				int byteArg = atoi(value.c_str());
				value = (jbyte)byteArg;
			}
			else if (hidden->IsInt32())
			{
				int byteArg = hidden->ToInt32()->Int32Value();
				value = (jbyte)byteArg;
			}

			jclass clazz = env.FindClass("java/lang/Byte");
			jmethodID ctor = env.GetMethodID(clazz, "<init>", "(B)V");
			JniLocalRef javaObject(env.NewObject(clazz, ctor, value));
			SetConvertedObject(env, index, javaObject);
			return true;
		}

		hidden = objectWithHiddenID->GetHiddenValue(V8StringConstants::GetMarkedAsShort());
		if (!hidden.IsEmpty())
		{
			jshort value = 0;
			if (hidden->IsString())
			{
				string value = ConvertToString(hidden->ToString());
				int shortArg = atoi(value.c_str());
				value = (jshort)shortArg;
			}
			else if (hidden->IsInt32())
			{
				jlong shortArg = hidden->ToInt32()->Int32Value();
				value = (jshort)shortArg;
			}

			jclass clazz = env.FindClass("java/lang/Short");
			jmethodID ctor = env.GetMethodID(clazz, "<init>", "(S)V");
			JniLocalRef javaObject(env.NewObject(clazz, ctor, value));
			SetConvertedObject(env, index, javaObject);
			return true;
		}

		hidden = objectWithHiddenID->GetHiddenValue(V8StringConstants::GetMarkedAsDouble());
		if (!hidden.IsEmpty())
		{
			jdouble value = 0;
			if (hidden->IsNumber())
			{
				double doubleArg = hidden->ToNumber()->NumberValue();
				value = (jdouble)doubleArg;
			}

			jclass clazz = env.FindClass("java/lang/Double");
			jmethodID ctor = env.GetMethodID(clazz, "<init>", "(D)V");
			JniLocalRef javaObject(env.NewObject(clazz, ctor, value));
			SetConvertedObject(env, index, javaObject);
			return true;
		}

		hidden = objectWithHiddenID->GetHiddenValue(V8StringConstants::GetMarkedAsFloat());
		if (!hidden.IsEmpty())
		{
			jfloat value = 0;
			if (hidden->IsNumber())
			{
				double floatArg = hidden->ToNumber()->NumberValue();
				value = (jfloat)floatArg;
			}

			jclass clazz = env.FindClass("java/lang/Float");
			jmethodID ctor = env.GetMethodID(clazz, "<init>", "(F)V");
			JniLocalRef javaObject(env.NewObject(clazz, ctor, value));
			SetConvertedObject(env, index, javaObject);
			return true;
		}

		hidden = objectWithHiddenID->GetHiddenValue(V8StringConstants::GetMarkedAsChar());
		if (!hidden.IsEmpty())
		{
			jchar value = '\0';
			if (hidden->IsString())
			{
				string value = ConvertToString(hidden->ToString());
				value = (jchar)value[0];
			}

			jclass clazz = env.FindClass("java/lang/Character");
			jmethodID ctor = env.GetMethodID(clazz, "<init>", "(C)V");
			JniLocalRef javaObject(env.NewObject(clazz, ctor, value));
			SetConvertedObject(env, index, javaObject);
			return true;
		}

		jweak obj = ObjectManager::GetJavaObjectByJsObjectStatic(objectWithHiddenID);
		success = obj != nullptr;
		if (success)
		{
			SetConvertedObject(env, index, obj, true);
		}
		else
		{
			s << "Cannot convert JavaScript object with id " << objectWithHiddenID->GetIdentityHash() << " at index " << index;
		}
	}
	else if (arg->IsUndefined() || arg->IsNull())
	{
		SetConvertedObject(env, index, nullptr);
		success = true;
	}
	else
	{
		s << "Cannot convert JavaScript object at index " << index;
		success = false;
	}

	if (!success)
	{
		m_error.index = index;
		m_error.msg = s.str();
	}

	return success;
}

jobject JsArgToArrayConverter::GetConvertedArg()
{
	return (m_argsLen > 0) ? m_argsAsObject[0] : nullptr;
}

void JsArgToArrayConverter::SetConvertedObject(JEnv& env, int index, jobject obj, bool isGlobalRef)
{
	if (obj == nullptr)
	{
		m_argsAsObject[index] = obj;
	}
	else if (isGlobalRef)
	{
		m_argsAsObject[index] = obj;
	}
	else
	{
		m_storedIndexes.push_back(index);
		m_argsAsObject[index] = env.NewLocalRef(obj);
	}
}

int JsArgToArrayConverter::Length() const
{
	return m_argsLen;
}

bool JsArgToArrayConverter::IsValid() const
{
	return m_isValid;
}

JsArgToArrayConverter::Error JsArgToArrayConverter::GetError() const
{
	return m_error;
}

jobjectArray JsArgToArrayConverter::ToJavaArray()
{
	if ((m_arr == nullptr) && (m_argsLen > 0))
	{
		m_arr = JavaObjectArrayCache::GetJavaObjectArray(m_argsLen);
	}

	if (m_argsLen > 0)
	{
		JEnv env;

		for (int i = 0; i < m_argsLen; i++)
		{
			env.SetObjectArrayElement(m_arr, i, m_argsAsObject[i]);
		}
	}

	return m_arr;
}

JsArgToArrayConverter::~JsArgToArrayConverter()
{
	if (m_argsLen > 0)
	{
		JEnv env;

		int length = m_storedIndexes.size();
		for (int i = 0; i < length; i++)
		{
			int index = m_storedIndexes[i];
			env.DeleteLocalRef(m_argsAsObject[index]);
		}

		delete[] m_argsAsObject;
	}
}

void JsArgToArrayConverter::Init(JavaVM *jvm)
{
	m_jvm = jvm;

	JEnv env;

	STRING_CLASS = env.FindClass("java/lang/String");
	assert(STRING_CLASS != nullptr);

	STRING_CTOR = env.GetMethodID(STRING_CLASS, "<init>", "([BLjava/lang/String;)V");
	assert(STRING_CTOR != 0);

	JniLocalRef encoding(env.NewStringUTF("UTF-8"));
	UTF_8_ENCODING = (jstring)env.NewGlobalRef(encoding);
	assert(UTF_8_ENCODING != nullptr);
}

JavaVM* JsArgToArrayConverter::m_jvm = nullptr;
jclass JsArgToArrayConverter::STRING_CLASS = nullptr;
jmethodID JsArgToArrayConverter::STRING_CTOR = nullptr;
jstring JsArgToArrayConverter::UTF_8_ENCODING = nullptr;
