#ifndef NATIVESCRIPTEXCEPTION_H_
#define NATIVESCRIPTEXCEPTION_H_

#include "v8.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "ObjectManager.h"

using namespace v8;
using namespace std;

namespace tns
{
	class NativeScriptException
	{
	public:
		NativeScriptException(JEnv& env);
		NativeScriptException(const string& message);
		NativeScriptException(const Local<Value>& jsException, const string& message = "");
		NativeScriptException(TryCatch& tc, const string& message = "");

		void ReThrowToV8();
		void ReThrowToJava();

		static void Init(JavaVM *jvm, ObjectManager *objectManager);
		static void OnUncaughtError(Local<Message> message, Local<Value> error);
		static void CallJsFuncWithErr(Local<Value> errObj);

	private:
		static jweak TryGetJavaThrowableObject(JEnv& env, const Local<Object>& jsObj);
		static string PrintErrorMessage(const Local<Message>& message, const Local<Value>& error);
		static string GetErrorMessage(const Local<Message>& message, const Local<Value>& error);
		static string GetErrorStackTrace(const Local<StackTrace>& stackTrace);
		static string GetExceptionMessage(JEnv& env, jthrowable exception);
		static Local<Value> WrapJavaException(JEnv& env);
		static void ThrowExceptionToV8(const string& exceptionMessage);
		static Local<Value> GetJavaExceptionFromEnv(const JniLocalRef& exc, JEnv& env);

		string GetFullMessage(const TryCatch& tc, bool isExceptionEmpty, bool isMessageEmpty, const string& prependMessage = "");

		JniLocalRef GetJavaException(const TryCatch& tc, bool isExceptionEmpty, bool isMessageEmpty, const string& prependMessage = "");
		Local<Value> m_javascriptException;
		JniLocalRef m_javaException;
		string m_message;


		static ObjectManager *objectManager;
		static jclass PlatformClass;
		static jclass THROWABLE_CLASS;
		static jmethodID THROWABLE_GET_CAUSE_METHOD_ID;
		static jmethodID THROWABLE_GET_STACK_TRACE_METHOD_ID;
		static jmethodID THROWABLE_TO_STRING_METHOD_ID;
		static jclass STACK_TRACE_ELEMENT_CLASS;
		static jmethodID FRAME_TO_STRING_METHOD_ID;
	};
}

#endif /* NATIVESCRIPTEXCEPTION_H_ */
