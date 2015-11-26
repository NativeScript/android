#include "JsArgToArrayConverter.h"
#include <limits>
#include <sstream>
#include "ObjectManager.h"
#include "NativeScriptAssert.h"
#include "ArgConverter.h"
#include "JniLocalRef.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "JavaObjectArrayCache.h"
#include "JType.h"

using namespace v8;
using namespace std;
using namespace tns;

JsArgToArrayConverter::JsArgToArrayConverter(const v8::Local<Value>& arg, bool isImplementationObject, int classReturnType) :
		m_arr(nullptr), m_argsAsObject(nullptr), m_argsLen(0), m_isValid(false), m_error(Error()), m_return_type(classReturnType)
{
	if (!isImplementationObject)
	{
		m_argsLen = 1;
		m_argsAsObject = new jobject[m_argsLen];
		memset(m_argsAsObject, 0, m_argsLen * sizeof(jobject));

		m_isValid = ConvertArg(arg, 0);
	}
}


JsArgToArrayConverter::JsArgToArrayConverter(const v8::FunctionCallbackInfo<Value>& args, bool hasImplementationObject, const Local<Object>& outerThis) :
		m_arr(nullptr), m_argsAsObject(nullptr), m_argsLen(0), m_isValid(false), m_error(Error()), m_return_type(static_cast<int>(Type::Null))
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


bool JsArgToArrayConverter::ConvertArg(const Local<Value>& arg, int index)
{
	bool success = false;
	stringstream s;

	JEnv env;

	Type returnType = JType::getClassType(m_return_type);

	if (arg.IsEmpty())
	{
		s << "Cannot convert empty JavaScript object";
		success = false;
	}
	else if (arg->IsInt32() && (returnType == Type::Int || returnType == Type::Null))
	{
		jint value = arg->Int32Value();
		JniLocalRef javaObject(JType::NewInt(env, value));
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
			jobject obj;

			//if returnType is long it will cast to long
			//if there is no return type specified it will cast to int
			//because default return type is null (ref type)
			if ((INT_MIN <= i) && (i <= INT_MAX) && (returnType == Type::Int || returnType == Type::Null))
			{
				obj = JType::NewInt(env, (jint)d);
			}
			else /*isLong*/
			{
				obj = JType::NewLong(env, (jlong)d);
			}

			JniLocalRef javaObject(obj);
			SetConvertedObject(env, index, javaObject);

			success = true;
		}
		else
		{
			jobject obj;

			//if returnType is double it will cast to double
			//if there is no return type specified it will cast to float
			//because default return type is null (ref type)
			if((FLT_MIN <= d) && (d <= FLT_MAX) && (returnType == Type::Float || returnType == Type::Null)) {
				obj = JType::NewFloat(env, (jfloat)d);
			}
			else {/*isDouble*/
				obj = JType::NewDouble(env, (jdouble)d);
			}

			JniLocalRef javaObject(obj);
			SetConvertedObject(env, index, javaObject);

			success = true;
		}
	}
	else if (arg->IsBoolean())
	{
		jboolean value = arg->BooleanValue();
		JniLocalRef javaObject(JType::NewBoolean(env, value));
		SetConvertedObject(env, index, javaObject);
		success = true;
	}
	else if (arg->IsBooleanObject())
	{
		auto boolObj = Local<BooleanObject>::Cast(arg);
		jboolean value = boolObj->BooleanValue() ? JNI_TRUE : JNI_FALSE;
		JniLocalRef javaObject(JType::NewBoolean(env, value));
		SetConvertedObject(env, index, javaObject);

		success = true;
	}
	else if (arg->IsString() || arg->IsStringObject())
	{
		JniLocalRef stringObject(ConvertToJavaString(arg));
		SetConvertedObject(env, index, stringObject);

		success = true;
	}
	else if (arg->IsObject())
	{
		auto objectWithHiddenID = arg->ToObject();

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

			JniLocalRef javaObject(JType::NewLong(env, value));
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

			JniLocalRef javaObject(JType::NewByte(env, value));
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

			JniLocalRef javaObject(JType::NewShort(env, value));
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

			JniLocalRef javaObject(JType::NewDouble(env, value));
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

			JniLocalRef javaObject(JType::NewFloat(env, value));
			SetConvertedObject(env, index, javaObject);
			return true;
		}

		hidden = objectWithHiddenID->GetHiddenValue(V8StringConstants::GetMarkedAsChar());
		if (!hidden.IsEmpty())
		{
			jchar value = '\0';
			if (hidden->IsString())
			{
				string str = ConvertToString(hidden->ToString());
				value = (jchar)str[0];
			}

			JniLocalRef javaObject(JType::NewChar(env, value));
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
