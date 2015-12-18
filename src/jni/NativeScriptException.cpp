#include "Util.h"
#include "NativeScriptException.h"
#include "V8GlobalHelpers.h"
#include "NativeScriptRuntime.h"
#include "NativeScriptAssert.h"
#include "V8StringConstants.h"
#include <sstream>

using namespace std;
using namespace tns;
using namespace v8;

NativeScriptException::NativeScriptException(JEnv& env)
:
		m_javascriptException(nullptr)
{
	JniLocalRef ex(env.ExceptionOccurred());
	env.ExceptionClear();
	m_javaException = ex;
}

NativeScriptException::NativeScriptException(const string& message)
:
		m_javascriptException(nullptr), m_javaException(JniLocalRef()), m_message(message)
{
}

NativeScriptException::NativeScriptException(TryCatch& tc, const string& message)
:
		m_javaException(JniLocalRef())
{
	auto isolate = Isolate::GetCurrent();
	m_javascriptException = new Persistent<Value>(isolate, tc.Exception());
	bool isMessageEmpty = tc.Message().IsEmpty();
	bool isExceptionEmpty = tc.Exception().IsEmpty();
	m_message = GetFullMessage(tc, isExceptionEmpty, isMessageEmpty, message);
	tc.Reset();
}

void NativeScriptException::ReThrowToV8()
{
	auto isolate = Isolate::GetCurrent();
	Local<Value> errObj;

	if (m_javascriptException != nullptr)
	{
		errObj = Local<Value>::New(isolate, *m_javascriptException);
	}
	else if (!m_message.empty())
	{
		errObj = Exception::Error(ConvertToV8String(m_message));
	}
	else if (!m_javaException.IsNull())
	{
		errObj = WrapJavaToJsException();
	}
	else
	{
		errObj = Exception::Error(ConvertToV8String("No javascript exception or message provided."));
	}

	isolate->ThrowException(errObj);
}

void NativeScriptException::ReThrowToJava()
{
	jthrowable ex = nullptr;
	JEnv env;

	if (!m_javaException.IsNull())
	{
		auto excClassName = objectManager->GetClassName((jobject) m_javaException);
		if (excClassName == "com/tns/NativeScriptException")
		{
			ex = m_javaException;
		}
		else
		{
			JniLocalRef msg(env.NewStringUTF("Java Error!"));
			ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID, (jstring) msg, (jobject) m_javaException));
		}
	}
	else if (m_javascriptException != nullptr)
	{
		/*
		 * If exception is an object, then check if it has 'nativeException' property and it if does then use it.
		 * If the found 'nativeException' is of type different than com.tns.NativeScript then we should wrap it and keep
		 * the JavaScript callstack as a message.
		 * Otherwise create we have to create new exception object.
		 */
		auto isolate = Isolate::GetCurrent();
		auto errObj = Local<Value>::New(isolate, *m_javascriptException);
		if (errObj->IsObject())
		{
			ex = (jthrowable) TryGetJavaThrowableObject(env, errObj.As<Object>());
		}

		if (ex == nullptr)
		{
			JniLocalRef msg(env.NewStringUTF(m_message.c_str()));
			ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID, (jstring) msg, reinterpret_cast<jlong>(m_javascriptException)));
		}
		else
		{
			auto excClassName = objectManager->GetClassName(ex);
			if (excClassName != "com/tns/NativeScriptException")
			{
				JniLocalRef msg(env.NewStringUTF(m_message.c_str()));
				ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID, (jstring) msg, ex));
			}
		}
	}
	else if (!m_message.empty())
	{
		JniLocalRef msg(env.NewStringUTF(m_message.c_str()));
		ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID, (jstring) msg, (jlong) 0));
	}
	else
	{
		JniLocalRef msg(env.NewStringUTF("No java exception or message provided."));
		ex = static_cast<jthrowable>(env.NewObject(NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID, (jstring) msg, (jlong) 0));
	}

	env.Throw(ex);
}

void NativeScriptException::Init(JavaVM *jvm, ObjectManager *objectManager)
{
	NativeScriptException::objectManager = objectManager;
	assert(NativeScriptException::objectManager != nullptr);

	JEnv env;

	PlatformClass = env.FindClass("com/tns/Platform");
	assert(PlatformClass != nullptr);

	THROWABLE_CLASS = env.FindClass("java/lang/Throwable");
	assert(THROWABLE_CLASS != nullptr);

	THROWABLE_GET_CAUSE_METHOD_ID = env.GetMethodID(THROWABLE_CLASS, "getCause", "()Ljava/lang/Throwable;");
	assert(THROWABLE_GET_CAUSE_METHOD_ID != nullptr);

	THROWABLE_GET_STACK_TRACE_METHOD_ID = env.GetMethodID(THROWABLE_CLASS, "getStackTrace", "()[Ljava/lang/StackTraceElement;");
	assert(THROWABLE_GET_STACK_TRACE_METHOD_ID != nullptr);

	THROWABLE_TO_STRING_METHOD_ID = env.GetMethodID(THROWABLE_CLASS, V8StringConstants::TO_STRING, "()Ljava/lang/String;");
	assert(THROWABLE_TO_STRING_METHOD_ID != nullptr);

	STACK_TRACE_ELEMENT_CLASS = env.FindClass("java/lang/StackTraceElement");
	assert(STACK_TRACE_ELEMENT_CLASS != nullptr);

	FRAME_TO_STRING_METHOD_ID = env.GetMethodID(STACK_TRACE_ELEMENT_CLASS, V8StringConstants::TO_STRING, "()Ljava/lang/String;");
	assert(FRAME_TO_STRING_METHOD_ID != nullptr);

	NATIVESCRIPTEXCEPTION_CLASS = env.FindClass("com/tns/NativeScriptException");
	assert(NATIVESCRIPTEXCEPTION_CLASS != nullptr);

	NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID = env.GetMethodID(NATIVESCRIPTEXCEPTION_CLASS, "<init>", "(Ljava/lang/String;J)V");
	assert(NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID != nullptr);

	NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID = env.GetMethodID(NATIVESCRIPTEXCEPTION_CLASS, "<init>", "(Ljava/lang/String;Ljava/lang/Throwable;)V");
	assert(NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID != nullptr);
}

// ON V8 UNCAUGHT EXCEPTION
void NativeScriptException::OnUncaughtError(Local<Message> message, Local<Value> error)
{
	auto errorMessage = PrintErrorMessage(message, error);

	Isolate *isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	Local<Object> errorObject;
	if (error->IsObject())
	{
		errorObject = error.As<Object>();
		errorObject->Set(ConvertToV8String("message"), ConvertToV8String(errorMessage));
	}
	else
	{
		errorObject = Exception::Error(ConvertToV8String(errorMessage)).As<Object>();
	}

	CallJsFuncWithErr(errorObject);

	// check whether the developer marked the error as "Caught"
	// As per discussion, it is safer to ALWAYS kill the application due to uncaught error(s)
	// TODO: We may think for some per-thread specific behavior and allow execution to continue for background thread exceptions
//		if(!returnValue.IsEmpty() && (returnValue->IsBoolean() || returnValue->IsBooleanObject())){
//			handled = returnValue->BooleanValue();
//		}
}

void NativeScriptException::CallJsFuncWithErr(Local<Value> errObj)
{
	auto isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	auto context = isolate->GetCurrentContext();
	auto globalHandle = context->Global();

	auto handler = globalHandle->Get(V8StringConstants::GetUncaughtError());
	auto isEmpty = handler.IsEmpty();
	auto isFunction = handler->IsFunction();

	if (!isEmpty && isFunction)
	{
		auto thiz = Object::New(isolate);
		auto func = handler.As<Function>();

		func->Call(thiz, 1, &errObj);
	}
}

Local<Value> NativeScriptException::WrapJavaToJsException()
{
	Local<Value> errObj;

	JEnv env;
	auto isolate = Isolate::GetCurrent();

	string excClassName = objectManager->GetClassName((jobject) m_javaException);
	if (excClassName == "com/tns/NativeScriptException")
	{
		jfieldID fieldID = env.GetFieldID(env.GetObjectClass(m_javaException), "jsValueAddress", "J");
		jlong addr = env.GetLongField(m_javaException, fieldID);

		jmethodID gmId = env.GetMethodID(env.GetObjectClass(m_javaException), "getMessage", "()Ljava/lang/String;");
		jobject javaMessage = env.CallObjectMethod(m_javaException, gmId);

		if (addr != 0)
		{
			auto pv = (Persistent<Value> *) addr;
			errObj = Local<Value>::New(isolate, *pv);
			pv->Reset();
		}
		else
		{
			errObj = GetJavaExceptionFromEnv(m_javaException, env);
		}
	}
	else
	{
		errObj = GetJavaExceptionFromEnv(m_javaException, env);
	}

	return errObj;
}

Local<Value> NativeScriptException::GetJavaExceptionFromEnv(const JniLocalRef& exc, JEnv& env)
{
	auto errMsg = GetExceptionMessage(env, exc);
	DEBUG_WRITE("Error during java interop errorMessage %s", errMsg.c_str());

	auto msg = ConvertToV8String(errMsg);
	auto errObj = Exception::Error(msg).As<Object>();

	jint javaObjectID = objectManager->GetOrCreateObjectId((jobject) exc);
	auto nativeExceptionObject = objectManager->GetJsObjectByJavaObject(javaObjectID);

	if (nativeExceptionObject.IsEmpty())
	{
		string className = objectManager->GetClassName((jobject) exc);
		nativeExceptionObject = objectManager->CreateJSWrapper(javaObjectID, className);
	}

	errObj->Set(V8StringConstants::GetNativeException(), nativeExceptionObject);

	return errObj;
}

string NativeScriptException::GetFullMessage(const TryCatch& tc, bool isExceptionEmpty, bool isMessageEmpty, const string& prependMessage)
{
	auto ex = tc.Exception();

	string message;
	if (!isExceptionEmpty && !isMessageEmpty)
	{
		message = PrintErrorMessage(tc.Message(), ex);
	}

	stringstream ss;
	ss << endl << prependMessage << message;
	string loggedMessage = ss.str();

	DEBUG_WRITE("Error: %s", loggedMessage.c_str());

	if (!tc.CanContinue())
	{
		stringstream errM;
		errM << endl << "An uncaught error has occurred and V8's TryCatch block CAN'T be continued. ";
		loggedMessage = errM.str() + loggedMessage;
	}

	return loggedMessage;
}

jweak NativeScriptException::TryGetJavaThrowableObject(JEnv& env, const Local<Object>& jsObj)
{
	jweak javaThrowableObject = nullptr;

	jweak javaObj = objectManager->GetJavaObjectByJsObject(jsObj);
	JniLocalRef objClass;

	if (javaObj != nullptr)
	{
		objClass = JniLocalRef(env.GetObjectClass(javaObj));
	}
	else
	{
		auto nativeEx = jsObj->Get(V8StringConstants::GetNativeException());
		if (!nativeEx.IsEmpty() && nativeEx->IsObject())
		{
			javaObj = objectManager->GetJavaObjectByJsObject(nativeEx.As<Object>());
			objClass = JniLocalRef(env.GetObjectClass(javaObj));
		}
	}

	auto isThrowable = !objClass.IsNull() ? env.IsAssignableFrom(objClass, THROWABLE_CLASS) : JNI_FALSE;

	if (isThrowable == JNI_TRUE)
	{
		javaThrowableObject = javaObj;
	}

	return javaThrowableObject;
}

string NativeScriptException::PrintErrorMessage(const Local<Message>& message, const Local<Value>& error)
{
	string errorMessage = GetErrorMessage(message, error);

	// split the message by new lines to workaround the LogCat's maximum characters in a single message
	stringstream ss(errorMessage);
	string line;
	while (getline(ss, line, '\n'))
	{
		// TODO: Log in the V8's Console as well?
		DEBUG_WRITE("%s", line.c_str());
	}

	return errorMessage;
}

string NativeScriptException::GetErrorMessage(const Local<Message>& message, const Local<Value>& error)
{
	stringstream ss;

	auto str = error->ToDetailString();
	if (str.IsEmpty())
	{
		str = String::NewFromUtf8(Isolate::GetCurrent(), "");
	}
	String::Utf8Value utfError(str);
	ss << endl << endl << *utfError << endl;
	auto scriptResName = message->GetScriptResourceName();
	if (!scriptResName.IsEmpty() && scriptResName->IsString())
	{
		ss << "File: \"" << ConvertToString(scriptResName.As<String>());
	}
	else
	{
		ss << "File: \"<unknown>";
	}
	ss << ", line: " << message->GetLineNumber() << ", column: " << message->GetStartColumn() << endl << endl;

	string stackTraceMessage = GetErrorStackTrace(message->GetStackTrace());
	ss << "StackTrace: " << endl << stackTraceMessage << endl;

	return ss.str();
}

string NativeScriptException::GetErrorStackTrace(const Local<StackTrace>& stackTrace)
{
	stringstream ss;

	auto isolate = Isolate::GetCurrent();
	HandleScope handleScope(isolate);

	int frameCount = stackTrace->GetFrameCount();

	for (int i = 0; i < frameCount; i++)
	{
		auto frame = stackTrace->GetFrame(i);
		auto funcName = ConvertToString(frame->GetFunctionName());
		auto srcName = ConvertToString(frame->GetScriptName());
		auto lineNumber = frame->GetLineNumber();
		auto column = frame->GetColumn();

		ss << "\tFrame: function:'" << funcName.c_str() << "', file:'" << srcName.c_str() << "', line: " << lineNumber << ", column: " << column << endl;
	}

	return ss.str();
}

string NativeScriptException::GetExceptionMessage(JEnv& env, jthrowable exception)
{
	string errMsg;
	JniLocalRef frames(env.CallObjectMethod(exception, THROWABLE_GET_STACK_TRACE_METHOD_ID));

	jsize framesLength = env.GetArrayLength(frames);

	if (nullptr != (jobjectArray) frames)
	{
		JniLocalRef msg(env.CallObjectMethod(exception, THROWABLE_TO_STRING_METHOD_ID));

		const char* msgStr = env.GetStringUTFChars(msg, nullptr);

		if (!errMsg.empty())
		{
			errMsg.append("\nCaused by: ");
			errMsg.append(msgStr);
		}
		else
		{
			errMsg = msgStr;
		}

		env.ReleaseStringUTFChars(msg, msgStr);
	}

	for (jsize i = 0; i < framesLength; i++)
	{
		JniLocalRef frame(env.GetObjectArrayElement(frames, i));
		JniLocalRef msg(env.CallObjectMethod(frame, FRAME_TO_STRING_METHOD_ID));

		const char* msgStr = env.GetStringUTFChars(msg, nullptr);

		errMsg.append("\n    ");
		errMsg.append(msgStr);

		env.ReleaseStringUTFChars(msg, msgStr);
	}

	if (nullptr != (jobjectArray) frames)
	{
		JniLocalRef cause(env.CallObjectMethod(exception, THROWABLE_GET_CAUSE_METHOD_ID));

		if (nullptr != (jthrowable) cause)
		{
			errMsg = GetExceptionMessage(env, cause);
		}
	}
	return errMsg;
}

ObjectManager* NativeScriptException::objectManager = nullptr;
jclass NativeScriptException::PlatformClass = nullptr;
jclass NativeScriptException::THROWABLE_CLASS = nullptr;
jmethodID NativeScriptException::THROWABLE_GET_CAUSE_METHOD_ID = nullptr;
jmethodID NativeScriptException::THROWABLE_GET_STACK_TRACE_METHOD_ID = nullptr;
jmethodID NativeScriptException::THROWABLE_TO_STRING_METHOD_ID = nullptr;
jclass NativeScriptException::STACK_TRACE_ELEMENT_CLASS = nullptr;
jmethodID NativeScriptException::FRAME_TO_STRING_METHOD_ID = nullptr;
jclass NativeScriptException::NATIVESCRIPTEXCEPTION_CLASS = nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID = nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID = nullptr;
