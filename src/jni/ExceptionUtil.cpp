#include "ExceptionUtil.h"
#include "JniLocalRef.h"
#include "NativeScriptAssert.h"
#include "NativeScriptRuntime.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "Constants.h"
#include <assert.h>
#include <sstream>

using namespace v8;
using namespace std;
using namespace tns;

ExceptionUtil::ExceptionUtil()
	: jvm(nullptr), objectManager(nullptr)
{
	this->initialized = false;
}

ExceptionUtil* ExceptionUtil::GetInstance(){
	if(!instance){
		// TODO: Possible concurrency issues
		instance = new ExceptionUtil();
	}

	return instance;
}

void ExceptionUtil::Init(JavaVM *jvm, ObjectManager *objectManager)
{
	if(this->initialized){
		return;
	}

	this->jvm = jvm;
	this->objectManager = objectManager;

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

	IS_JAVA_THROWABLE_METHOD_ID = env.GetStaticMethodID(PlatformClass, "isJavaThrowable", "(Ljava/lang/Object;)Z");
	assert(IS_JAVA_THROWABLE_METHOD_ID != 0);

	this->initialized = true;
}

void ExceptionUtil::GetExceptionMessage(JEnv& env, jthrowable exception, string& errMsg)
{
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
			GetExceptionMessage(env, cause, errMsg);
		}
	}
}

void ExceptionUtil::HandleInvalidState(const string& message, bool fail){
	if(fail){
		NativeScriptRuntime::APP_FAIL(message.c_str());
	}
	else {
		auto error = Exception::Error(ConvertToV8String(message));
		Isolate::GetCurrent()->ThrowException(error);
	}
}

bool ExceptionUtil::HandleTryCatch(TryCatch& tc){
	if(!tc.HasCaught()){
		return false;
	}

	if(tc.CanContinue()){
		auto message = tc.Message();
		auto error = tc.Exception();
		OnUncaughtError(message, error);
	}
	else {
		auto errorMessage = PrintErrorMessage(tc.Message(), tc.Exception());

		stringstream ss;
		ss << "An uncaught error has occurred and V8's TryCatch block may not be continued. Error is: " << errorMessage;

		HandleInvalidState(ss.str(), true);
	}

	return true;
}

void ExceptionUtil::OnUncaughtError(Handle<Message> message, Handle<Value> error){
	auto errorMessage = instance->PrintErrorMessage(message, error);

	auto isolate = Isolate::GetCurrent();
	auto context = isolate->GetCurrentContext();
	auto globalHandle = context->Global();

	auto handler = globalHandle->Get(V8StringConstants::GetUncaughtError());
	auto isEmpty = handler.IsEmpty();
	auto isFunction = handler->IsFunction();

	if(!isEmpty && isFunction){
		// call the global exception handler
		Handle<Value> errorObject;

		if(error->IsObject()){
			errorObject = error.As<Object>();
			error.As<Object>()->Set(ConvertToV8String("message"), ConvertToV8String(errorMessage));
		}
		else{
			errorObject = Exception::Error(ConvertToV8String(errorMessage));
		}

		auto thiz = Object::New(isolate);
		auto func = handler.As<Function>();

		// Notify the developer for the exception and let him do some additional job here
		// TODO: Launching UI at this point may be erroneous, think how to prevent it
		func->Call(thiz, 1, &errorObject);

		// check whether the developer marked the error as "Caught"
		// As per discussion, it is safer to ALWAYS kill the application due to uncaught error(s)
		// TODO: We may think for some per-thread specific behavior and allow execution to continue for background thread exceptions
//		if(!returnValue.IsEmpty() && (returnValue->IsBoolean() || returnValue->IsBooleanObject())){
//			handled = returnValue->BooleanValue();
//		}
	}

	// go to Java through the Thread.setUncaughtExceptionHandler routine
	//NativeScriptRuntime::APP_FAIL(errorMessage.c_str());
}

string ExceptionUtil::GetErrorMessage(const Handle<Message>& message, const Handle<Value>& error){
	stringstream ss;

	auto str = error->ToDetailString();
	if (str.IsEmpty())
	{
		str = String::NewFromUtf8(Isolate::GetCurrent(), "");
	}
	String::Utf8Value utfError(str);
	ss << *utfError << endl;
	ss << "File: \"" << ConvertToString(message->GetScriptResourceName().As<String>());
	ss << ", line: " << message->GetLineNumber() << ", column: " << message->GetStartColumn() << endl;

	string stackTraceMessage = GetErrorStackTrace(message->GetStackTrace());
	ss << "StackTrace: " << endl << stackTraceMessage;

	return ss.str();
}

string ExceptionUtil::PrintErrorMessage(const Handle<Message>& message, const Handle<Value>& error){
	string errorMessage = GetErrorMessage(message, error);

	// split the message by new lines to workaround the LogCat's maximum characters in a single message
	stringstream ss(errorMessage);
	string line;
	while (std::getline(ss, line, '\n')) {
		// TODO: Log in the V8's Console as well?
		__android_log_print(ANDROID_LOG_ERROR, "TNS.Native", "%s", line.c_str());
	}

	return errorMessage;
}

string ExceptionUtil::GetErrorStackTrace(const Handle<StackTrace>& stackTrace)
{
	stringstream ss;

	auto isolate = Isolate::GetCurrent();
	HandleScope handleScope(isolate);

	int frameCount = stackTrace->GetFrameCount();

	for (int i = 0; i < frameCount; i++)
	{
		auto frame = stackTrace->GetFrame(i);
		string funcName = ConvertToString(frame->GetFunctionName());
		string srcName = ConvertToString(frame->GetScriptName());

		int lineNumber = frame->GetLineNumber();
		if (lineNumber > Constants::MODULE_LINES_OFFSET)
		{
			lineNumber -= Constants::MODULE_LINES_OFFSET;
		}

		int column = frame->GetColumn();
		ss << "\tFrame: function:'" << funcName.c_str() << "', file:'" << srcName.c_str() << "', line: " << lineNumber << ", column: " << column << endl;
	}

	return ss.str();
}

bool ExceptionUtil::CheckForException(Isolate *isolate, const string& methodName, TryCatch& tc)
{
	bool exceptionFound = tc.HasCaught();

	JEnv env;

	if (exceptionFound)
	{
		DEBUG_WRITE("Calling js method %s failed", methodName.c_str());

		auto ex = tc.Exception();
		string loggedMessage = GetErrorMessage(tc.Message(), ex);

		String::Utf8Value error(tc.Exception());
		String::Utf8Value file(tc.Message()->GetScriptResourceName());
		int line = tc.Message()->GetLineNumber();
		int column = tc.Message()->GetStartColumn();
		DEBUG_WRITE("Error: %s @line: %d, column: %d", *error, line, column);

		auto pv = new Persistent<Value>(isolate, ex);

		if (tc.CanContinue())
		{
			jweak javaThrowable = nullptr;
			if (ex->IsObject())
			{
				javaThrowable = TryGetJavaThrowableObject(env, ex->ToObject());
			}

			if (javaThrowable != nullptr)
			{
				jint ret = env.Throw(reinterpret_cast<jthrowable>(javaThrowable));

				DEBUG_WRITE("Error: Throw (1)=%d", (int)ret);
			}
			else
			{
				jclass nativeScriptExceptionClass = env.FindClass("com/tns/NativeScriptException");

				jmethodID ctor = env.GetMethodID(nativeScriptExceptionClass, "<init>", "(Ljava/lang/String;J)V");
				jstring s = env.NewStringUTF(loggedMessage.c_str());
				jobject exObject = env.NewObject(nativeScriptExceptionClass, ctor, s, (jlong)pv);
				jint ret = env.Throw((jthrowable)exObject);

				DEBUG_WRITE("Error: Throw (2)=%d", (int)ret);
			}
		}
		else
		{
			NativeScriptRuntime::APP_FAIL(*error);
		}
	}

	return exceptionFound;
}

bool ExceptionUtil::CheckForJavaException(JEnv& env)
{
	bool exceptionOccurred = env.ExceptionCheck() == JNI_TRUE;

	if (exceptionOccurred)
	{
		auto isolate = Isolate::GetCurrent();
		JniLocalRef exc(env.ExceptionOccurred());

		DEBUG_WRITE("Error during java interop");
		// env.ExceptionDescribe(); We will print this manually in the ExceptionUtil
		env.ExceptionClear();
		string excClassName = objectManager->GetClassName((jobject)exc);
		if (excClassName == "com/tns/NativeScriptException")
		{
			jfieldID fieldID = env.GetFieldID(env.GetObjectClass(exc), "jsValueAddress", "J");
			jlong addr = env.GetLongField(exc, fieldID);
			Handle<Value> v;
			if (addr != 0)
			{
				auto pv = (Persistent<Value> *) addr;
				v = Handle<Value>::New(Isolate::GetCurrent(), *pv);
				pv->Reset();
			}
			else
			{
				v = Null(isolate);
			}
			isolate->ThrowException(v);
		}
		else
		{
			string errMsg;
			ExceptionUtil::GetInstance()->GetExceptionMessage(env, exc, errMsg);
			DEBUG_WRITE("Error during java interop errorMessage %s", errMsg.c_str());

			auto msg = ConvertToV8String(errMsg);
			auto errObj = Exception::Error(msg).As<Object>();

			jint javaObjectID = objectManager->GetOrCreateObjectId((jobject) exc);
			auto nativeExceptionObject = objectManager->GetJsObjectByJavaObject(javaObjectID);

			if (nativeExceptionObject.IsEmpty())
			{
				string className = objectManager->GetClassName((jobject)exc);
				nativeExceptionObject = objectManager->CreateJSProxyInstance(javaObjectID, className);
			}

			errObj->Set(V8StringConstants::GetNativeException(), nativeExceptionObject);
			isolate->ThrowException(errObj);
		}
	}

	return exceptionOccurred;
}

jweak ExceptionUtil::TryGetJavaThrowableObject(JEnv& env, const Handle<v8::Object>& jsObj)
{
	jweak javaThrowableObject = nullptr;

	auto jsInstanceInfo = jsObj->GetHiddenValue(ConvertToV8String("t::HiddenJSInstanceInfo"));
	if (!jsInstanceInfo.IsEmpty())
	{
		jweak javaObj = objectManager->GetJavaObjectByJsObject(jsObj);

		jboolean ret = env.CallStaticBooleanMethod(PlatformClass, IS_JAVA_THROWABLE_METHOD_ID, javaObj);

		if (ret == JNI_TRUE)
		{
			javaThrowableObject = javaObj;
		}
	}

	return javaThrowableObject;
}

ExceptionUtil* ExceptionUtil::instance = nullptr;
