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
#include "NumericCasts.h"
#include "NativeScriptException.h"

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
		auto javaObject = JType::NewInt(env, value);
		SetConvertedObject(env, index, javaObject);

		success = true;
	}
	else if (arg->IsNumber() || arg->IsNumberObject())
	{
		double d = arg->NumberValue();
		int64_t i = (int64_t) d;
		bool isInteger = d == i;

		if (isInteger)
		{
			jobject obj;

			//if returnType is long it will cast to long
			//if there is no return type specified it will cast to int
			//because default return type is null (ref type)
			if ((INT_MIN <= i) && (i <= INT_MAX) && (returnType == Type::Int || returnType == Type::Null))
			{
				obj = JType::NewInt(env, (jint) d);
			}
			else /*isLong*/
			{
				obj = JType::NewLong(env, (jlong) d);
			}

			SetConvertedObject(env, index, obj);

			success = true;
		}
		else
		{
			jobject obj;

			//if returnType is double it will cast to double
			//if there is no return type specified it will cast to float
			//because default return type is null (ref type)
			if ((FLT_MIN <= d) && (d <= FLT_MAX) && (returnType == Type::Float || returnType == Type::Null))
			{
				obj = JType::NewFloat(env, (jfloat) d);
			}
			else
			{/*isDouble*/
				obj = JType::NewDouble(env, (jdouble) d);
			}

			SetConvertedObject(env, index, obj);

			success = true;
		}
	}
	else if (arg->IsBoolean())
	{
		jboolean value = arg->BooleanValue();
		auto javaObject = JType::NewBoolean(env, value);
		SetConvertedObject(env, index, javaObject);
		success = true;
	}
	else if (arg->IsBooleanObject())
	{
		auto boolObj = Local<BooleanObject>::Cast(arg);
		jboolean value = boolObj->BooleanValue() ? JNI_TRUE : JNI_FALSE;
		auto javaObject = JType::NewBoolean(env, value);
		SetConvertedObject(env, index, javaObject);

		success = true;
	}
	else if (arg->IsString() || arg->IsStringObject())
	{
		auto stringObject = ConvertToJavaString(arg);
		SetConvertedObject(env, index, stringObject);

		success = true;
	}
	else if (arg->IsObject())
	{
		auto jsObj = arg->ToObject();

		auto castType = NumericCasts::GetCastType(jsObj);
		Local<Value> castValue;
		jchar charValue;
		jbyte byteValue;
		jshort shortValue;
		jlong longValue;
		jfloat floatValue;
		jdouble doubleValue;
		jobject javaObject;
		JniLocalRef obj;

		switch (castType)
		{
			case CastType::Char:
				castValue = NumericCasts::GetCastValue(jsObj);
				charValue = '\0';
				if (castValue->IsString())
				{
					string str = ConvertToString(castValue->ToString());
					charValue = (jchar) str[0];
				}
				javaObject = JType::NewChar(env, charValue);
				SetConvertedObject(env, index, javaObject);
				success = true;
				break;

			case CastType::Byte:
				castValue = NumericCasts::GetCastValue(jsObj);
				byteValue = 0;
				if (castValue->IsString())
				{
					string value = ConvertToString(castValue->ToString());
					int byteArg = atoi(value.c_str());
					byteValue = (jbyte) byteArg;
				}
				else if (castValue->IsInt32())
				{
					int byteArg = castValue->ToInt32()->Int32Value();
					byteValue = (jbyte) byteArg;
				}
				javaObject = JType::NewByte(env, byteValue);
				SetConvertedObject(env, index, javaObject);
				success = true;
				break;

			case CastType::Short:
				castValue = NumericCasts::GetCastValue(jsObj);
				shortValue = 0;
				if (castValue->IsString())
				{
					string value = ConvertToString(castValue->ToString());
					int shortArg = atoi(value.c_str());
					shortValue = (jshort) shortArg;
				}
				else if (castValue->IsInt32())
				{
					jlong shortArg = castValue->ToInt32()->Int32Value();
					shortValue = (jshort) shortArg;
				}
				javaObject = JType::NewShort(env, shortValue);
				SetConvertedObject(env, index, javaObject);
				success = true;
				break;

			case CastType::Long:
				castValue = NumericCasts::GetCastValue(jsObj);
				longValue = 0;
				if (castValue->IsString())
				{
					auto strValue = ConvertToString(castValue->ToString());
					longValue = atoll(strValue.c_str());
				}
				else if (castValue->IsInt32())
				{
					longValue = castValue->ToInt32()->Int32Value();
				}
				javaObject = JType::NewLong(env, longValue);
				SetConvertedObject(env, index, javaObject);
				success = true;
				break;

			case CastType::Float:
				castValue = NumericCasts::GetCastValue(jsObj);
				floatValue = 0;
				if (castValue->IsNumber())
				{
					double floatArg = castValue->ToNumber()->NumberValue();
					floatValue = (jfloat) floatArg;
				}
				javaObject = JType::NewFloat(env, floatValue);
				SetConvertedObject(env, index, javaObject);
				success = true;
				break;

			case CastType::Double:
				castValue = NumericCasts::GetCastValue(jsObj);
				doubleValue = 0;
				if (castValue->IsNumber())
				{
					double doubleArg = castValue->ToNumber()->NumberValue();
					doubleValue = (jdouble) doubleArg;
				}
				javaObject = JType::NewDouble(env, doubleValue);
				SetConvertedObject(env, index, javaObject);
				success = true;
				break;

			case CastType::None:
				obj = ObjectManager::GetJavaObjectByJsObjectStatic(jsObj);
				success = !obj.IsNull();
				if (success)
				{
					SetConvertedObject(env, index, obj.Move(), obj.IsGlobal());
				}
				else
				{
					s << "Cannot convert JavaScript object with id " << jsObj->GetIdentityHash() << " at index " << index;
				}
				break;

			default:
				throw NativeScriptException("Unsupported cast type");

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

void JsArgToArrayConverter::SetConvertedObject(JEnv& env, int index, jobject obj, bool isGlobal)
{
	m_argsAsObject[index] = obj;
	if ((obj != nullptr) && !isGlobal)
	{
		m_storedIndexes.push_back(index);
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
