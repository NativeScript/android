#ifndef NATIVESCRIPTEXCEPTION_H_
#define NATIVESCRIPTEXCEPTION_H_

#include "v8.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "ObjectManager.h"

namespace tns
{
	class NativeScriptException
	{
	public:
		NativeScriptException(JEnv& env);
		NativeScriptException(const std::string& message);
		NativeScriptException(const v8::Local<v8::Value>& jsException, const std::string& message = "");
		NativeScriptException(v8::TryCatch& tc, const std::string& message = "");

		void ReThrowToV8();
		void ReThrowToJava();

		static void Init(JavaVM *jvm, ObjectManager *objectManager);
		static void OnUncaughtError(v8::Local<v8::Message> message, v8::Local<v8::Value> error);
		static void CallJsFuncWithErr(v8::Local<v8::Value> errObj);

	private:
		jweak TryGetJavaThrowableObject(JEnv& env, const v8::Local<v8::Object>& jsObj);
		std::string GetExceptionMessage(JEnv& env, jthrowable exception);
		v8::Local<v8::Value> WrapJavaException();
		void ThrowExceptionToV8(const std::string& exceptionMessage);
		v8::Local<v8::Value> GetJavaExceptionFromEnv(const JniLocalRef& exc, JEnv& env);
		static std::string PrintErrorMessage(const v8::Local<v8::Message>& message, const v8::Local<v8::Value>& error);
		static std::string GetErrorMessage(const v8::Local<v8::Message>& message, const v8::Local<v8::Value>& error);
		static std::string GetErrorStackTrace(const v8::Local<v8::StackTrace>& stackTrace);

		std::string GetFullMessage(const v8::TryCatch& tc, bool isExceptionEmpty, bool isMessageEmpty, const std::string& prependMessage = "");

		JniLocalRef GetJavaException(const v8::TryCatch& tc, bool isExceptionEmpty, bool isMessageEmpty, const std::string& prependMessage = "");
		v8::Local<v8::Value> m_javascriptException;
		JniLocalRef m_javaException;
		std::string m_message;


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
