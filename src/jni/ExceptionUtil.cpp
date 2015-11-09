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

bool ExceptionUtil::HandleTryCatch(TryCatch& tc, const string& prependMessage){
	if(!tc.HasCaught()){
		return false;
	}

	if(tc.CanContinue()){
		ThrowExceptionToJava(tc, prependMessage);
	}
	else {
		stringstream ss;
		ss << endl << "An uncaught error has occurred and V8's TryCatch block may not be continued. Error is: ";

		ExceptionUtil::GetInstance()->ThrowExceptionToJava(tc, ss.str());
	}

	return true;
}

void ExceptionUtil::OnUncaughtError(Local<Message> message, Local<Value> error){
	auto errorMessage = instance->PrintErrorMessage(message, error);

	Isolate *isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	Local<Object> errorObject;
	if(error->IsObject()){
		errorObject = error.As<Object>();
		errorObject->Set(ConvertToV8String("message"), ConvertToV8String(errorMessage));
	}
	else{
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

void ExceptionUtil::CallJsFuncWithErr(Local<Value> errObj)
{
	//create handle scope
	Isolate *isolate = Isolate::GetCurrent();
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

string ExceptionUtil::GetErrorMessage(const Local<Message>& message, const Local<Value>& error){
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
	ss << ", line: " << message->GetLineNumber() - Constants::MODULE_LINES_OFFSET << ", column: " << message->GetStartColumn() << endl << endl;

	string stackTraceMessage = GetErrorStackTrace(message->GetStackTrace());
	ss << "StackTrace: " << endl << stackTraceMessage << endl;

	return ss.str();
}

string ExceptionUtil::PrintErrorMessage(const Local<Message>& message, const Local<Value>& error){
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

string ExceptionUtil::GetErrorStackTrace(const Local<StackTrace>& stackTrace)
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

void ExceptionUtil::ThrowExceptionToJava(TryCatch& tc, const string& prependMessage)
{
	if(!tc.HasCaught()) {
		NativeScriptRuntime::AppFail(nullptr, prependMessage.c_str());
		return;
	}

	Isolate *isolate = Isolate::GetCurrent();
	auto ex = tc.Exception();

	bool isExceptionEmpty = ex.IsEmpty();
	bool isMessageEmpty = tc.Message().IsEmpty();

	string message;
	if(!isExceptionEmpty && !isMessageEmpty){
		message = PrintErrorMessage(tc.Message(), ex);
	}

	stringstream ss;
	ss << endl << prependMessage << message;
	string loggedMessage = ss.str();

	DEBUG_WRITE("Error: %s", loggedMessage.c_str());

	JEnv env;
	env.ExceptionClear();

	if(!tc.CanContinue()) {
		NativeScriptRuntime::AppFail(nullptr, loggedMessage.c_str());
		return;
	}

	jweak javaThrowable = nullptr;
	if (!ex.IsEmpty() && ex->IsObject())
	{
		javaThrowable = TryGetJavaThrowableObject(env, ex->ToObject());
	}

	if (javaThrowable == nullptr)
	{
		//make it an object
		jclass nativeScriptExceptionClass = env.FindClass("com/tns/NativeScriptException");
		jmethodID ctor = env.GetMethodID(nativeScriptExceptionClass, "<init>", "(Ljava/lang/String;J)V");
		jstring errMessage = env.NewStringUTF(loggedMessage.c_str());
		auto pv = new Persistent<Value>(isolate, ex);
		javaThrowable = env.NewObject(nativeScriptExceptionClass, ctor, errMessage, (jlong)pv);
	}

	NativeScriptRuntime::AppFail(reinterpret_cast<jthrowable>(javaThrowable), loggedMessage.c_str());
}

void ExceptionUtil::ThrowExceptionToJs(const string& exceptionMessage)
{
	Isolate *isolate(Isolate::GetCurrent());
	Local<Value> exception = v8::Exception::Error(ConvertToV8String(exceptionMessage));
	isolate->ThrowException(exception);
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
			Local<Value> v;
			if (addr != 0)
			{
				auto pv = (Persistent<Value> *) addr;
				v = Local<Value>::New(Isolate::GetCurrent(), *pv);
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
				nativeExceptionObject = objectManager->CreateJSWrapper(javaObjectID, className);
			}

			errObj->Set(V8StringConstants::GetNativeException(), nativeExceptionObject);
			isolate->ThrowException(errObj);
		}
	}

	return exceptionOccurred;
}

jweak ExceptionUtil::TryGetJavaThrowableObject(JEnv& env, const Local<v8::Object>& jsObj)
{
	jweak javaThrowableObject = nullptr;

	jweak javaObj = objectManager->GetJavaObjectByJsObject(jsObj);

	if (javaObj != nullptr)
	{
		jboolean ret = env.CallStaticBooleanMethod(PlatformClass, IS_JAVA_THROWABLE_METHOD_ID, javaObj);

		if (ret == JNI_TRUE)
		{
			javaThrowableObject = javaObj;
		}
	}

	return javaThrowableObject;
}

ExceptionUtil* ExceptionUtil::instance = nullptr;
