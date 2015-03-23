#include "FieldAccessor.h"
#include "JniLocalRef.h"
#include "ArgConverter.h"
#include "NativeScriptAssert.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include <assert.h>

using namespace v8;
using namespace std;
using namespace tns;

void FieldAccessor::Init(JavaVM *jvm, ObjectManager *objectManager)
{
	this->jvm = jvm;
	this->objectManager = objectManager;
}

Handle<Value> FieldAccessor::GetJavaField(const Handle<Object>& caller, const string& declaringClassName, const string& fieldName, const string& fieldTypeName, const bool isStatic)
{
	JEnv env;

	auto isolate = Isolate::GetCurrent();
	EscapableHandleScope handleScope(isolate);

	Handle<Value> fieldResult;

	jweak targetJavaObject;
	jclass clazz;
	jfieldID fieldId;

	bool isPrimitiveType = fieldTypeName.size() == 1;

	bool isFieldArray = fieldTypeName[0] == '[';

	string fieldJniSig = isPrimitiveType
							? fieldTypeName
							: (isFieldArray
								? fieldTypeName
								: ("L" + fieldTypeName + ";"));

	if (isStatic)
	{
		clazz = env.FindClass(declaringClassName);
		fieldId = env.GetStaticFieldID(clazz, fieldName, fieldJniSig);
	}
	else
	{
		targetJavaObject = objectManager->GetJavaObjectByJsObject(caller);
		clazz = objectManager->GetJavaClass(caller);
		fieldId = env.GetFieldID(clazz, fieldName, fieldJniSig);
	}

	if (isPrimitiveType)
	{
		switch (fieldTypeName[0])
		{
			case 'Z': //bool
			{
				jboolean result;
				if (isStatic)
				{
					result = env.GetStaticBooleanField(clazz, fieldId);
				}
				else
				{
					result = env.GetBooleanField(targetJavaObject, fieldId);
				}
				fieldResult = Boolean::New(isolate, (result == JNI_TRUE));
				break;
			}
			case 'B': //byte
			{
				jbyte result;
				if (isStatic)
				{
					result = env.GetStaticByteField(clazz, fieldId);
				}
				else
				{
					result = env.GetByteField(targetJavaObject, fieldId);
				}
				fieldResult = handleScope.Escape(Int32::New(isolate, result));
				break;
			}
			case 'C': //char
			{
				jchar result;
				if (isStatic)
				{
					result = env.GetStaticCharField(clazz, fieldId);
				}
				else
				{
					result = env.GetCharField(targetJavaObject, fieldId);
				}

				jstring str = env.NewString(&result, 1);
				jboolean bol = true;
				const char* resP = env.GetStringUTFChars(str, &bol);
				env.ReleaseStringUTFChars(str, resP);
				fieldResult = handleScope.Escape(ConvertToV8String(resP, 1));
				break;
			}
			case 'S': //short
			{
				jshort result;
				if (isStatic)
				{
					result = env.GetStaticShortField(clazz, fieldId);
				}
				else
				{
					result = env.GetShortField(targetJavaObject, fieldId);
				}
				fieldResult = handleScope.Escape(Int32::New(isolate, result));
				break;
			}
			case 'I': //int
			{
				jint result;
				if (isStatic)
				{
					result = env.GetStaticIntField(clazz, fieldId);
				}
				else
				{
					result = env.GetIntField(targetJavaObject, fieldId);
				}
				fieldResult = handleScope.Escape(Int32::New(isolate, result));
				break;

			}
			case 'J': //long
			{
				jlong result;
				if (isStatic)
				{
					result = env.GetStaticLongField(clazz, fieldId);
				}
				else
				{
					result = env.GetLongField(targetJavaObject, fieldId);
				}
				fieldResult = handleScope.Escape(ArgConverter::ConvertFromJavaLong(result));
				break;
			}
			case 'F': //float
			{
				jfloat result;
				if (isStatic)
				{
					result = env.GetStaticFloatField(clazz, fieldId);
				}
				else
				{
					result = env.GetFloatField(targetJavaObject, fieldId);
				}
				fieldResult = handleScope.Escape(Number::New(isolate, (double) result));
				break;
			}
			case 'D': //double
			{
				jdouble result;
				if (isStatic)
				{
					result = env.GetStaticDoubleField(clazz, fieldId);
				}
				else
				{
					result = env.GetDoubleField(targetJavaObject, fieldId);
				}
				fieldResult = handleScope.Escape(Number::New(isolate, result));
				break;
			}
			default:
			{
				// TODO:
				ASSERT_FAIL("Unknown field type");
				break;
			}
		}
	}
	else
	{
		bool isString = fieldTypeName == "java/lang/String";

		if (isString)
		{
			JniLocalRef result;
			if (isStatic)
			{
				result = env.GetStaticObjectField(clazz, fieldId);
			}
			else
			{
				result = env.GetObjectField(targetJavaObject, fieldId);
			}

			auto resultV8String = ArgConverter::jstringToV8String(result);
			fieldResult = handleScope.Escape(resultV8String);
		}
		else
		{
			jobject result;
			if (isStatic)
			{
				result = env.GetStaticObjectField(clazz, fieldId);
			}
			else
			{
				result = env.GetObjectField(targetJavaObject, fieldId);
			}

			if (result != nullptr)
			{
				int javaObjectID = objectManager->GetOrCreateObjectId(result);
				auto objectResult = objectManager->GetJsObjectByJavaObject(javaObjectID);

				if (objectResult.IsEmpty())
				{
					objectResult = objectManager->CreateJSWrapper(javaObjectID, fieldTypeName, result);
				}

				env.DeleteLocalRef(result);

				fieldResult = handleScope.Escape(objectResult);
			}
			else
			{
				fieldResult = Null(isolate);
			}
		}
	}

	return fieldResult;
}

void FieldAccessor::SetJavaField(const Handle<Object>& target, const Handle<Value>& value, const string& declaringTypeName, const string& fieldName, const std::string& fieldTypeName, bool isStatic)
{
	JEnv env;

	auto isolate = Isolate::GetCurrent();
	HandleScope handleScope(isolate);

	jweak targetJavaObject;
	jclass clazz;
	jfieldID fieldId;

	bool isPrimitiveType = fieldTypeName.size() == 1;

	bool isFieldArray = fieldTypeName[0] == '[';

	string fieldJniSig = isPrimitiveType
							? fieldTypeName
							: (isFieldArray
								? fieldTypeName
								: ("L" + fieldTypeName + ";"));

	if (isStatic)
	{
		clazz = env.FindClass(declaringTypeName);
		fieldId = env.GetStaticFieldID(clazz, fieldName, fieldJniSig);
	}
	else
	{
		targetJavaObject = objectManager->GetJavaObjectByJsObject(target);
		clazz = objectManager->GetJavaClass(target);
		fieldId = env.GetFieldID(clazz, fieldName, fieldJniSig);
	}

	if (isPrimitiveType)
	{
		switch (fieldTypeName[0])
		{
			case 'Z': //bool
			{
				//TODO: validate value is a boolean before calling
				if (isStatic)
				{
					env.SetStaticBooleanField(clazz, fieldId, value->BooleanValue());
				}
				else
				{
					env.SetBooleanField(targetJavaObject, fieldId, value->BooleanValue());
				}
				break;
			}
			case 'B': //byte
			{
				//TODO: validate value is a byte before calling
				if (isStatic)
				{
					env.SetStaticByteField(clazz, fieldId, value->Int32Value());
				}
				else
				{
					env.SetByteField(targetJavaObject, fieldId, value->Int32Value());
				}
				break;
			}
			case 'C': //char
			{
				//TODO: validate value is a single char
				String::Utf8Value stringValue(value->ToString());
				JniLocalRef value(env.NewStringUTF(*stringValue));
				const char* chars = env.GetStringUTFChars(value, 0);

				if (isStatic)
				{
					env.SetStaticCharField(clazz, fieldId, chars[0]);
				}
				else
				{
					env.SetCharField(targetJavaObject, fieldId, chars[0]);
				}
				break;
			}
			case 'S': //short
			{
				//TODO: validate value is a short before calling
				if (isStatic)
				{
					env.SetStaticShortField(clazz, fieldId, value->Int32Value());
				}
				else
				{
					env.SetShortField(targetJavaObject, fieldId, value->Int32Value());
				}
				break;
			}
			case 'I': //int
			{
				//TODO: validate value is a int before calling
				if (isStatic)
				{
					env.SetStaticIntField(clazz, fieldId, value->Int32Value());
				}
				else
				{
					env.SetIntField(targetJavaObject, fieldId, value->Int32Value());
				}
				break;

			}
			case 'J': //long
			{
				jlong longValue = (jlong) ArgConverter::ConvertToJavaLong(value);
				if (isStatic)
				{
					env.SetStaticLongField(clazz, fieldId, longValue);
				}
				else
				{
					env.SetLongField(targetJavaObject, fieldId, longValue);
				}
				break;
			}
			case 'F': //float
			{
				if (isStatic)
				{
					env.SetStaticFloatField(clazz, fieldId, (jfloat) value->NumberValue());
				}
				else
				{
					env.SetLongField(targetJavaObject, fieldId, (jfloat) value->NumberValue());
				}
				break;
			}
			case 'D': //double
			{
				if (isStatic)
				{
					env.SetStaticFloatField(clazz, fieldId, value->NumberValue());
				}
				else
				{
					env.SetLongField(targetJavaObject, fieldId, value->NumberValue());
				}
				break;
			}
			default:
			{
				// TODO:
				ASSERT_FAIL("Unknown field type");
				break;
			}
		}
	}
	else
	{
		bool isString = fieldTypeName == "java/lang/String";
		if (isString)
		{
			//TODO: validate valie is a string;
			String::Utf8Value stringValue(value->ToString());
			JniLocalRef value(env.NewStringUTF(*stringValue));

			if (isStatic)
			{
				env.SetStaticObjectField(clazz, fieldId, value);
			}
			else
			{
				env.SetObjectField(targetJavaObject, fieldId, value);
			}
		}
		else
		{
			Local<Object> objectWithHiddenID = value->ToObject();
			jweak javaObject = objectManager->GetJavaObjectByJsObject(objectWithHiddenID);

			if (isStatic)
			{
				env.SetStaticObjectField(clazz, fieldId, javaObject);
			}
			else
			{
				env.SetObjectField(targetJavaObject, fieldId, javaObject);
			}
		}
	}
}
