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
{
	m_javascriptException = WrapJavaException(env);
}


NativeScriptException::NativeScriptException(const std::string& message)
	:m_javascriptException(Local<Value>()), m_message(message)
{
}

NativeScriptException::NativeScriptException(const v8::Local<v8::Value>& jsException, const std::string& message)
	:m_javascriptException(jsException), m_message(message)
{
}

NativeScriptException::NativeScriptException(TryCatch& tc, const string& message)
	:m_javascriptException(Local<Value>())
{
	bool isMessageEmpty = tc.Message().IsEmpty();
	bool isExceptionEmpty = tc.Exception().IsEmpty();
	m_message = GetFullMessage(tc, isExceptionEmpty, isMessageEmpty, message);
	m_javaException = GetJavaException(tc, isExceptionEmpty, isMessageEmpty, message);
}

void NativeScriptException::ReThrowToV8() {
	auto isolate = Isolate::GetCurrent();

	if (!m_message.empty()){
		ThrowExceptionToV8(m_message);
	}
	else if(!m_javascriptException.IsEmpty()) {
		isolate->ThrowException(m_javascriptException);
	}
	else if (m_javaException.IsNull()){
		isolate->ThrowException(Exception::Error(ConvertToV8String("Error: You are trying to throw java exception to V8.")));
	}
	else {
		isolate->ThrowException(Exception::Error(ConvertToV8String("No javascript exception or message provided.")));
	}
}

void NativeScriptException::ReThrowToJava() {

	jthrowable ex;
	JEnv env;

	jclass nativeScriptExceptionClass = env.FindClass("com/tns/NativeScriptException");
	jmethodID ctor = env.GetMethodID(nativeScriptExceptionClass, "<init>", "(Ljava/lang/String;J)V");

	if (!m_javaException.IsNull()){
		ex = m_javaException;
	}
	else if (!m_message.empty()){
		JniLocalRef msg(env.NewStringUTF(m_message.c_str()));
		ex = static_cast<jthrowable>(env.NewObject(nativeScriptExceptionClass, ctor, (jstring)msg, (jlong)0));
	}
	else if(!m_javascriptException.IsEmpty()) {
		auto isolate = Isolate::GetCurrent();
		auto pv = new Persistent<Value>(isolate, m_javascriptException);
		JniLocalRef msg(env.NewStringUTF("Error: You are trying to throw javascript exception to java."));
		ex = static_cast<jthrowable>(env.NewObject(nativeScriptExceptionClass, ctor, (jstring)msg, reinterpret_cast<jlong>(pv)));
	}
	else {
		JniLocalRef msg(env.NewStringUTF("No java exception or message provided."));
		ex = static_cast<jthrowable>(env.NewObject(nativeScriptExceptionClass, ctor, (jstring)msg, (jlong)0));
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
}

// ON V8 UNCAUGHT EXCEPTION
void NativeScriptException::OnUncaughtError(Local<Message> message, Local<Value> error)
{
	auto errorMessage = PrintErrorMessage(message, error);

	Isolate *isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	Local<Object> errorObject;
	if(error->IsObject()){
		errorObject = error.As<Object>();
		errorObject->Set(ConvertToV8String("message"), ConvertToV8String(errorMessage));
	}
	else {
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

	if(!isEmpty && isFunction)
	{
		auto thiz = Object::New(isolate);
		auto func = handler.As<Function>();

		func->Call(thiz, 1, &errObj);
	}
}


// private
//throw to v8 after JNI call caused JAVA exception
Local<Value> NativeScriptException::WrapJavaException(JEnv& env)
{
	auto isolate = Isolate::GetCurrent();
	JniLocalRef exc(env.ExceptionOccurred());

	env.ExceptionClear();
	string excClassName = objectManager->GetClassName((jobject)exc);
	if (excClassName == "com/tns/NativeScriptException")
	{
		jfieldID fieldID = env.GetFieldID(env.GetObjectClass(exc), "jsValueAddress", "J");
		jlong addr = env.GetLongField(exc, fieldID);

		jmethodID gmId = env.GetMethodID(env.GetObjectClass(exc), "getMessage", "()Ljava/lang/String;");
		jobject javaMessage = env.CallObjectMethod(exc, gmId);

		Local<Value> v;
		if (addr != 0)
		{
			auto pv = (Persistent<Value> *) addr;
			v = Local<Value>::New(isolate, *pv);
			pv->Reset();
		}
		else if(javaMessage != nullptr) {
			v = GetJavaExceptionFromEnv(exc, env);
		}
		else
		{
			v = Null(isolate);
		}

		return v;
	}
	else
	{
		auto errObj = GetJavaExceptionFromEnv(exc, env);
		return errObj;
	}
}

Local<Value> NativeScriptException::GetJavaExceptionFromEnv(const JniLocalRef& exc, JEnv& env) {
	auto errMsg = GetExceptionMessage(env, exc);
	DEBUG_WRITE("Error during java interop errorMessage %s", errMsg.c_str());

	auto msg = ConvertToV8String(errMsg);
	auto errObj = Exception::Error(msg).As<Object>();

	jint javaObjectID = objectManager->GetOrCreateObjectId((jobject) exc);
	auto nativeExceptionObject = objectManager->GetJsObjectByJavaObject(javaObjectID);

	if (nativeExceptionObject.IsEmpty())
	{
		string className = objectManager->GetClassName((jobject)exc);
		nativeExceptionObject = objectManager->CreateJSWrapper(javaObjectID, className);
	}

	errObj->Set(V8StringConstants::GetNativeException(), nativeExceptionObject);

	return errObj;
}

void NativeScriptException::ThrowExceptionToV8(const string& exceptionMessage)
{
	auto isolate = Isolate::GetCurrent();
	auto errObj = Exception::Error(ConvertToV8String(exceptionMessage)).As<Object>();

	isolate->ThrowException(errObj);
}

string NativeScriptException::GetFullMessage(const TryCatch& tc, bool isExceptionEmpty, bool isMessageEmpty, const string& prependMessage)
{
	auto ex = tc.Exception();

	string message;
	if(!isExceptionEmpty && !isMessageEmpty){
		message = PrintErrorMessage(tc.Message(), ex);
	}

	stringstream ss;
	ss << endl << prependMessage << message;
	string loggedMessage = ss.str();

	DEBUG_WRITE("Error: %s", loggedMessage.c_str());

	if(!tc.CanContinue()) {
		stringstream errM;
		errM << endl << "An uncaught error has occurred and V8's TryCatch block CAN'T be continued. ";
		loggedMessage = errM.str() + loggedMessage;
	}

	return loggedMessage;
}

JniLocalRef NativeScriptException::GetJavaException(const TryCatch& tc, bool isExceptionEmpty, bool isMessageEmpty, const string& prependMessage)
{
	assert(tc.HasCaught());
	JniLocalRef javaThrowable;

	auto ex = tc.Exception();

	if(!tc.CanContinue()) {
		return javaThrowable;
	}

	JEnv env;
	env.ExceptionClear();

	//try to get the wrapped java exception from the js exception
	if (!isExceptionEmpty && ex->IsObject())
	{
		javaThrowable = JniLocalRef(TryGetJavaThrowableObject(env, ex->ToObject()), true /* isWeak */);
	}

	//if the js exception is not just a wrapper to the java exception
	if (javaThrowable.IsNull())
	{
		//create java nativescript exception
		jclass nativeScriptExceptionClass = env.FindClass("com/tns/NativeScriptException");
		jmethodID ctor = env.GetMethodID(nativeScriptExceptionClass, "<init>", "(Ljava/lang/String;J)V");

		//persist the js exception
		auto isolate = Isolate::GetCurrent();
		auto pv = new Persistent<Value>(isolate, ex);

		// put the js exception in a java throwable exception
		jstring errMessage = env.NewStringUTF(m_message.c_str());
		javaThrowable = JniLocalRef(env.NewObject(nativeScriptExceptionClass, ctor, errMessage, (jlong)pv));
	}

	return javaThrowable;
}

jweak NativeScriptException::TryGetJavaThrowableObject(JEnv& env, const Local<v8::Object>& jsObj)
{
	jweak javaThrowableObject = nullptr;

	jweak javaObj = objectManager->GetJavaObjectByJsObject(jsObj);

	if (javaObj != nullptr)
	{
		JniLocalRef obj(env.GetObjectClass(javaObj));
		jboolean ret = env.IsAssignableFrom(obj, THROWABLE_CLASS);

		if (ret == JNI_TRUE)
		{
			javaThrowableObject = javaObj;
		}
	}

	return javaThrowableObject;
}

string NativeScriptException::PrintErrorMessage(const Local<Message>& message, const Local<Value>& error)
{
	string errorMessage = GetErrorMessage(message, error);

	// split the message by new lines to workaround the LogCat's maximum characters in a single message
	stringstream ss(errorMessage);
	string line;
	while (std::getline(ss, line, '\n')) {
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
