#include "JsArgConverter.h"
#include <sstream>
#include <assert.h>
#include "ObjectManager.h"
#include "NativeScriptAssert.h"
#include "ArgConverter.h"
#include "JniLocalRef.h"
#include "JniSignatureParser.h"
#include "JsArgToArrayConverter.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"

using namespace v8;
using namespace std;
using namespace tns;

JsArgConverter::JsArgConverter(const v8::FunctionCallbackInfo<Value>& args, bool hasImplementationObject, const string& methodSignature) :
		m_args(nullptr), m_methodSignature(methodSignature), m_isValid(true), m_error(Error())
{
	m_argsLen = !hasImplementationObject ? args.Length() : args.Length() - 1;

	if (m_argsLen > 0)
	{
		m_args = new jvalue[m_argsLen];
		memset(m_args, 0, m_argsLen * sizeof(jvalue));

		JniSignatureParser parser(m_methodSignature);
		m_tokens = parser.Parse();

		for (int i = 0; i < m_argsLen; i++)
		{
			m_isValid = ConvertArg(args[i], i);

			if (!m_isValid)
			{
				break;
			}
		}
	}
}

bool JsArgConverter::ConvertArg(const Handle<Value>& arg, int index)
{
	JEnv env;

	bool success = false;
	stringstream s;

	string typeSignature = m_tokens[index];

	if (arg.IsEmpty())
	{
		SetConvertedObject(env, index, nullptr);
		success = false;
	}
	else if (arg->IsArray())
	{
		ASSERT_MESSAGE(typeSignature[0] == '[', "Expected array signature, actual signature=%s", typeSignature.c_str());

		auto jsArr = Handle<Array>::Cast(arg);

		success = ConvertJavaScriptArray(env, jsArr, index);

		if (!success)
		{
			s << "Cannot convert array to " << typeSignature << " at index " << index;
		}
	}
	else if (arg->IsNumber() || arg->IsNumberObject())
	{
		success = ConvertJavaScriptNumber(env, arg, index);

		if (!success)
		{
			s << "Cannot convert number to " << typeSignature << " at index " << index;
		}
	}
	else if (arg->IsBoolean() || arg->IsBooleanObject())
	{
		success = ConvertJavaScriptBoolean(env, arg, index);

		if (!success)
		{
			s << "Cannot convert boolean to " << typeSignature << " at index " << index;
		}
	}
	else if (arg->IsString() || arg->IsStringObject())
	{
		success = ConvertJavaScriptString(env, arg, index);

		if (!success)
		{
			s << "Cannot convert string to " << typeSignature << " at index " << index;
		}
	}
	else if (arg->IsObject())
	{
		jlong javaLongValue;
		auto jsObject = arg->ToObject();

		auto hidden = jsObject->GetHiddenValue(V8StringConstants::GetMarkedAsLong());
		if (!hidden.IsEmpty())
		{
			if (hidden->IsString())
			{
				string strValue = ConvertToString(hidden->ToString());
				int64_t longArg = atoll(strValue.c_str());
				jlong value = (jlong)longArg;
				success = ConvertFromCastFunctionObject(value, index);
			}
			else if (hidden->IsInt32())
			{
				jlong value = (jlong)hidden->ToInt32()->IntegerValue();
				success = ConvertFromCastFunctionObject(value, index);
			}
			return success;
		}
		else if (ArgConverter::TryConvertToJavaLong(jsObject, javaLongValue))
		{
			m_args[index].j = javaLongValue;
			success = true;
			return success;
		}

		hidden = jsObject->GetHiddenValue(V8StringConstants::GetMarkedAsByte());
		if (!hidden.IsEmpty())
		{
			if (hidden->IsString())
			{
				string strValue = ConvertToString(hidden->ToString());
				int byteArg = atoi(strValue.c_str());
				jbyte value = (jbyte)byteArg;
				success = ConvertFromCastFunctionObject(value, index);
			}
			else if (hidden->IsInt32())
			{
				jbyte value = (jbyte)hidden->ToInt32()->Int32Value();
				success = ConvertFromCastFunctionObject(value, index);
			}
			return success;
		}

		hidden = jsObject->GetHiddenValue(V8StringConstants::GetMarkedAsShort());
		if (!hidden.IsEmpty())
		{
			if (hidden->IsString())
			{
				string strValue = ConvertToString(hidden->ToString());
				int shortArg = atoi(strValue.c_str());
				jshort value = (jshort)shortArg;
				success = ConvertFromCastFunctionObject(value, index);
			}
			else if (hidden->IsInt32())
			{
				jshort value = (jshort)hidden->ToInt32()->Int32Value();
				success = ConvertFromCastFunctionObject(value, index);
			}
			return success;
		}

		hidden = jsObject->GetHiddenValue(V8StringConstants::GetMarkedAsDouble());
		if (!hidden.IsEmpty())
		{
			if (hidden->IsNumber())
			{
				double doubleArg = hidden->ToNumber()->NumberValue();
				jdouble value = (jdouble)doubleArg;
				success = ConvertFromCastFunctionObject(value, index);
			}
			return success;
		}

		hidden = jsObject->GetHiddenValue(V8StringConstants::GetMarkedAsFloat());
		if (!hidden.IsEmpty())
		{
			if (hidden->IsNumber())
			{
				double floatArg = hidden->ToNumber()->NumberValue();
				jfloat value = (jfloat)floatArg;
				success = ConvertFromCastFunctionObject(value, index);
			}
			return success;
		}

		hidden = jsObject->GetHiddenValue(V8StringConstants::GetMarkedAsChar());
		if (!hidden.IsEmpty())
		{
			if (hidden->IsString())
			{
				string value = ConvertToString(hidden->ToString());
				m_args[index].c = (jchar)value[0];
				success = true;
			}
			return success;
		}

		jweak obj = ObjectManager::GetJavaObjectByJsObjectStatic(jsObject);
		SetConvertedObject(env, index, obj, true);
		success = obj != nullptr;

		if (!success)
		{
			s << "Cannot convert object to " << typeSignature << " at index " << index;
		}
	}
	else if (arg->IsUndefined() || arg->IsNull())
	{
		SetConvertedObject(env, index, nullptr);
		success = true;
	}
	else
	{
		SetConvertedObject(env, index, nullptr);
		success = false;
	}

	if (!success)
	{
		m_error.index = index;
		m_error.msg = s.str();
	}

	return success;
}

void JsArgConverter::SetConvertedObject(JEnv& env, int index, jobject obj, bool isGlobalRef)
{
	if (obj == nullptr)
	{
		m_args[index].l = obj;
	}
	else if (isGlobalRef)
	{
		m_args[index].l = obj;
	}
	else
	{
		m_storedObjects.push_back(index);
		m_args[index].l = env.NewLocalRef(obj);
	}
}

bool JsArgConverter::ConvertJavaScriptNumber(JEnv& env, const Handle<Value>& jsValue, int index)
{
	bool success = true;

	jvalue value =  { 0 };

	string typeSignature = m_tokens[index];

	const char typePrefix = typeSignature[0];

	switch (typePrefix)
	{
		case 'B': // byte
			value.b = (jbyte) jsValue->Int32Value();
			break;

		case 'S': // short
			value.s = (jshort) jsValue->Int32Value();
			break;

		case 'I': // int
			value.i = (jint) jsValue->Int32Value();
			break;

		case 'J': // long
			value.j = (jlong) jsValue->IntegerValue();
			break;

		case 'F': // float
			value.f = (jfloat) jsValue->NumberValue();
			break;

		case 'D': // double
			value.d = (jdouble) jsValue->NumberValue();
			break;

		default:
			success = false;
			break;
	}

	if (success)
	{
		m_args[index] = value;
	}

	return success;
}

bool JsArgConverter::ConvertJavaScriptBoolean(JEnv& env, const Handle<Value>& jsValue, int index)
{
	bool success;

	string typeSignature = m_tokens[index];

	if (typeSignature == "Z")
	{
		bool argValue;
		if (jsValue->IsBoolean())
		{
			argValue = jsValue->BooleanValue();
		}
		else
		{
			auto boolObj = Handle<BooleanObject>::Cast(jsValue);
			auto val = boolObj->Get(V8StringConstants::GetValueOf());
			if (!val.IsEmpty() && val->IsFunction())
			{
				argValue = val.As<Function>()->Call(boolObj, 0, nullptr)->BooleanValue();
			}
			else
			{
				argValue = false;
			}
		}
		jboolean value = argValue ? JNI_TRUE : JNI_FALSE;
		m_args[index].z = value;
		success = true;
	}
	else
	{
		success = false;
	}

	return success;
}

bool JsArgConverter::ConvertJavaScriptString(JEnv& env, const Handle<Value>& jsValue, int index)
{
	JniLocalRef stringObject(ConvertToJavaString(jsValue));
	SetConvertedObject(env, index, stringObject);

	return true;
}

bool JsArgConverter::ConvertJavaScriptArray(JEnv& env, const Handle<Array>& jsArr, int index)
{
	bool success = true;

	jarray arr = nullptr;

	jsize arrLength = jsArr->Length();

	string arraySignature = m_tokens[index];

	string elementType = arraySignature.substr(1);

	const char elementTypePrefix = elementType[0];

	jclass elementClass;
	string strippedClassName;

	switch (elementTypePrefix)
	{
		case 'Z':
			arr = env.NewBooleanArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jboolean value = jsArr->Get(i)->BooleanValue();
				env.SetBooleanArrayRegion((jbooleanArray)arr, i, 1, &value);
			}
			break;
		case 'B':
			arr = env.NewByteArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jbyte value = jsArr->Get(i)->Int32Value();
				env.SetByteArrayRegion((jbyteArray)arr, i, 1, &value);
			}
			break;
		case 'C':
			arr = env.NewCharArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				String::Utf8Value utf8(jsArr->Get(i)->ToString());
				JniLocalRef s(env.NewString((jchar*) *utf8, 1));
				const char* singleChar = env.GetStringUTFChars(s, nullptr);
				jchar value = *singleChar;
				env.ReleaseStringUTFChars(s, singleChar);
				env.SetCharArrayRegion((jcharArray)arr, i, 1, &value);
			}
			break;
		case 'S':
			arr = env.NewShortArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jshort value = jsArr->Get(i)->Int32Value();
				env.SetShortArrayRegion((jshortArray)arr, i, 1, &value);
			}
			break;
		case 'I':
			arr = env.NewIntArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jint value = jsArr->Get(i)->Int32Value();
				env.SetIntArrayRegion((jintArray)arr, i, 1, &value);
			}
			break;
		case 'J':
			arr = env.NewLongArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jlong value = jsArr->Get(i)->Int32Value();
				env.SetLongArrayRegion((jlongArray)arr, i, 1, &value);
			}
			break;
		case 'F':
			arr = env.NewFloatArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jfloat value = jsArr->Get(i)->NumberValue();
				env.SetFloatArrayRegion((jfloatArray)arr, i, 1, &value);
			}
			break;
		case 'D':
			arr = env.NewDoubleArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jdouble value = jsArr->Get(i)->NumberValue();
				env.SetDoubleArrayRegion((jdoubleArray)arr, i, 1, &value);
			}
			break;
		case 'L':
			strippedClassName = elementType.substr(1, elementType.length() - 2);
			elementClass = env.FindClass(strippedClassName);
			arr = env.NewObjectArray(arrLength, elementClass, nullptr);
			for (int i=0; i<arrLength; i++)
			{
				auto v = jsArr->Get(i);
				JsArgToArrayConverter c(v, false);
				jobject o = c.GetConvertedArg();
				env.SetObjectArrayElement((jobjectArray)arr, i, o);
			}
			break;
		default:
			success = false;
			break;
	}

	if (success)
	{
		SetConvertedObject(env, index, arr);
	}

	return success;
}


template<typename T>
bool JsArgConverter::ConvertFromCastFunctionObject(T value, int index)
{
	bool success = false;

	string typeSignature = m_tokens[index];

	const char typeSignaturePrefix = typeSignature[0];

	switch (typeSignaturePrefix)
	{
		case 'B':
			m_args[index].b = (jbyte)value;
			success = true;
			break;

		case 'S':
			m_args[index].s = (jshort)value;
			success = true;
			break;

		case 'I':
			m_args[index].i = (jint)value;
			success = true;
			break;

		case 'J':
			m_args[index].j = (jlong)value;
			success = true;
			break;

		case 'F':
			m_args[index].f = (jfloat)value;
			success = true;
			break;

		case 'D':
			m_args[index].d = (jdouble)value;
			success = true;
			break;

		default:
			success = false;
			break;
	}

	return success;
}

int JsArgConverter::Length() const
{
	return m_argsLen;
}

bool JsArgConverter::IsValid() const
{
	return m_isValid;
}

jvalue* JsArgConverter::ToArgs() const
{
	return m_args;
}

JsArgConverter::Error JsArgConverter::GetError() const
{
	return m_error;
}

JsArgConverter::~JsArgConverter()
{
	if (m_argsLen > 0)
	{
		JEnv env;

		int length = m_storedObjects.size();
		for (int i = 0; i < length; i++)
		{
			int index = m_storedObjects[i];
			env.DeleteLocalRef(m_args[index].l);
		}

		delete[] m_args;
	}
}
