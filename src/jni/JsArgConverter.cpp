#include "JsArgConverter.h"
#include <sstream>
#include "ObjectManager.h"
#include "NativeScriptAssert.h"
#include "ArgConverter.h"
#include "JniLocalRef.h"
#include "JniSignatureParser.h"
#include "JsArgToArrayConverter.h"
#include "JType.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"

using namespace v8;
using namespace std;
using namespace tns;

JsArgConverter::JsArgConverter(const v8::FunctionCallbackInfo<Value>& args, bool hasImplementationObject, const string& methodSignature, MetadataEntry *entry) :
		m_env(JEnv()), m_methodSignature(methodSignature), m_isValid(true), m_error(Error()), m_tokens(nullptr)
{
	m_argsLen = !hasImplementationObject ? args.Length() : args.Length() - 1;

	if (m_argsLen > 0)
	{
		if ((entry != nullptr) && (entry->isResolved))
		{
			if (entry->parsedSig.empty())
			{
				JniSignatureParser parser(m_methodSignature);
				entry->parsedSig = parser.Parse();
			}
			m_tokens = &entry->parsedSig;
		}
		else
		{
			JniSignatureParser parser(m_methodSignature);
			m_tokens2 = parser.Parse();
			m_tokens = &m_tokens2;
		}

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

bool JsArgConverter::ConvertArg(const Local<Value>& arg, int index)
{
	bool success = false;

	char buff[1024];

	const auto& typeSignature = m_tokens->at(index);

	if (arg.IsEmpty())
	{
		SetConvertedObject(index, nullptr);
		success = false;
	}
	else if (arg->IsArray())
	{
		success = typeSignature[0] == '[';

		if (success)
		{
			auto jsArr = Local<Array>::Cast(arg);

			success = ConvertJavaScriptArray(jsArr, index);
		}

		if (!success)
		{
			sprintf(buff, "Cannot convert array to %s at index %d", typeSignature.c_str(), index);
		}
	}
	else if (arg->IsNumber() || arg->IsNumberObject())
	{
		success = ConvertJavaScriptNumber(arg, index);

		if (!success)
		{
			sprintf(buff, "Cannot convert number to %s at index %d", typeSignature.c_str(), index);
		}
	}
	else if (arg->IsBoolean() || arg->IsBooleanObject())
	{
		success = ConvertJavaScriptBoolean(arg, index);

		if (!success)
		{
			sprintf(buff, "Cannot convert boolean to %s at index %d", typeSignature.c_str(), index);
		}
	}
	else if (arg->IsString() || arg->IsStringObject())
	{
		success = ConvertJavaScriptString(arg, index);

		if (!success)
		{
			sprintf(buff, "Cannot convert string to %s at index %d", typeSignature.c_str(), index);
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
		SetConvertedObject(index, obj, true);
		success = obj != nullptr;

		if (!success)
		{
			sprintf(buff, "Cannot convert object to %s at index %d", typeSignature.c_str(), index);
		}
	}
	else if (arg->IsUndefined() || arg->IsNull())
	{
		SetConvertedObject(index, nullptr);
		success = true;
	}
	else
	{
		SetConvertedObject(index, nullptr);
		success = false;
	}

	if (!success)
	{
		m_error.index = index;
		m_error.msg = string(buff);
	}

	return success;
}

void JsArgConverter::SetConvertedObject(int index, jobject obj, bool isGlobalRef)
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
		m_args[index].l = obj;
	}
}

bool JsArgConverter::ConvertJavaScriptNumber(const Local<Value>& jsValue, int index)
{
	bool success = true;

	jvalue value =  { 0 };

	const auto& typeSignature = m_tokens->at(index);

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
			// TODO: refactor the whole method
			if (jsValue->IsNumberObject())
			{
				auto numObj = Local<NumberObject>::Cast(jsValue);
				value.j = (jlong) numObj->ValueOf();
			}
			else
			{
				value.j = (jlong) jsValue->IntegerValue();
			}
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

bool JsArgConverter::ConvertJavaScriptBoolean(const Local<Value>& jsValue, int index)
{
	bool success;

	const auto& typeSignature = m_tokens->at(index);

	if (typeSignature == "Z")
	{
		bool argValue;
		if (jsValue->IsBoolean())
		{
			argValue = jsValue->BooleanValue();
		}
		else
		{
			auto boolObj = Local<BooleanObject>::Cast(jsValue);
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

bool JsArgConverter::ConvertJavaScriptString(const Local<Value>& jsValue, int index)
{
	jstring stringObject = ConvertToJavaString(jsValue);
	SetConvertedObject(index, stringObject);

	return true;
}

bool JsArgConverter::ConvertJavaScriptArray(const Local<Array>& jsArr, int index)
{
	bool success = true;

	jarray arr = nullptr;

	jsize arrLength = jsArr->Length();

	const auto& arraySignature = m_tokens->at(index);

	string elementType = arraySignature.substr(1);

	const char elementTypePrefix = elementType[0];

	jclass elementClass;
	string strippedClassName;

	switch (elementTypePrefix)
	{
		case 'Z':
			arr = m_env.NewBooleanArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jboolean value = jsArr->Get(i)->BooleanValue();
				m_env.SetBooleanArrayRegion((jbooleanArray)arr, i, 1, &value);
			}
			break;
		case 'B':
			arr = m_env.NewByteArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jbyte value = jsArr->Get(i)->Int32Value();
				m_env.SetByteArrayRegion((jbyteArray)arr, i, 1, &value);
			}
			break;
		case 'C':
			arr = m_env.NewCharArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				String::Utf8Value utf8(jsArr->Get(i)->ToString());
				JniLocalRef s(m_env.NewString((jchar*) *utf8, 1));
				const char* singleChar = m_env.GetStringUTFChars(s, nullptr);
				jchar value = *singleChar;
				m_env.ReleaseStringUTFChars(s, singleChar);
				m_env.SetCharArrayRegion((jcharArray)arr, i, 1, &value);
			}
			break;
		case 'S':
			arr = m_env.NewShortArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jshort value = jsArr->Get(i)->Int32Value();
				m_env.SetShortArrayRegion((jshortArray)arr, i, 1, &value);
			}
			break;
		case 'I':
			arr = m_env.NewIntArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jint value = jsArr->Get(i)->Int32Value();
				m_env.SetIntArrayRegion((jintArray)arr, i, 1, &value);
			}
			break;
		case 'J':
			arr = m_env.NewLongArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jlong value = jsArr->Get(i)->Int32Value();
				m_env.SetLongArrayRegion((jlongArray)arr, i, 1, &value);
			}
			break;
		case 'F':
			arr = m_env.NewFloatArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jfloat value = jsArr->Get(i)->NumberValue();
				m_env.SetFloatArrayRegion((jfloatArray)arr, i, 1, &value);
			}
			break;
		case 'D':
			arr = m_env.NewDoubleArray(arrLength);
			for (jsize i=0; i<arrLength; i++)
			{
				jdouble value = jsArr->Get(i)->NumberValue();
				m_env.SetDoubleArrayRegion((jdoubleArray)arr, i, 1, &value);
			}
			break;
		case 'L':
			strippedClassName = elementType.substr(1, elementType.length() - 2);
			elementClass = m_env.FindClass(strippedClassName);
			arr = m_env.NewObjectArray(arrLength, elementClass, nullptr);
			for (int i=0; i<arrLength; i++)
			{
				auto v = jsArr->Get(i);
				JsArgToArrayConverter c(v, false, (int)Type::Null);
				jobject o = c.GetConvertedArg();
				m_env.SetObjectArrayElement((jobjectArray)arr, i, o);
			}
			break;
		default:
			success = false;
			break;
	}

	if (success)
	{
		SetConvertedObject(index, arr);
	}

	return success;
}


template<typename T>
bool JsArgConverter::ConvertFromCastFunctionObject(T value, int index)
{
	bool success = false;

	const auto& typeSignature = m_tokens->at(index);

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

jvalue* JsArgConverter::ToArgs()
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
	}
}
