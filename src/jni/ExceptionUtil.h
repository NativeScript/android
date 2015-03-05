#ifndef EXCEPTIONUTIL_H_
#define EXCEPTIONUTIL_H_

#include "v8.h"
#include "JEnv.h"
#include "ObjectManager.h"
#include <string>

namespace tns
{
	class ExceptionUtil
	{
	public:

		void Init(JavaVM *jvm, ObjectManager *objectManager);

		bool CheckForException(v8::Isolate *isolate, const std::string& methodName, v8::TryCatch& tc);

		bool CheckForJavaException(JEnv& env);

		bool ThrowExceptionToJava(v8::TryCatch& tc);

		void GetExceptionMessage(JEnv& env, jthrowable exception, std::string& errMsg);

		/**
		 * Inspects a TryCatch block for pending exceptions.
		 * If an exception exists there are two execution paths:
		 *  - The flow may continue. In this case a check for nested TryCatch blocks will be made and if such exist the error will be re-thrown.
		 *  - The flow may not continue. In this case a call the NativeScriptRuntime::APP_FAIL will be made. This will go to Java where the Java VM will be shut down.
		 */
		bool HandleTryCatch(v8::TryCatch& tc);

		/**
		 * Provides an entry point to handle states considered invalid for the NativeScript runtime.
		 * There are two invalid states:
		 *  - A state which does not break the whole runtime. In this case a JavaScript error will be raised.
		 *  - A state which breaks the runtime flow. In this case a call the NativeScriptRuntime::APP_FAIL will be made. This will go to Java where the Java VM will be shut down.
		 */
		void HandleInvalidState(const std::string& message, bool fail);

		/**
		 * A callback to the V8's AddMessageListener method.
		 * Reports any uncaught non-critical errors.
		 */
		static void OnUncaughtError(v8::Handle<v8::Message> message, v8::Handle<v8::Value> error);
		static void CallJsFuncWithErr(v8::Handle<v8::Value> errObj);

		static ExceptionUtil* GetInstance();

	private:
		ExceptionUtil();

		std::string GetErrorStackTrace(const v8::Handle<v8::StackTrace>& stackTrace);
		std::string GetErrorMessage(const v8::Handle<v8::Message>& message, const v8::Handle<v8::Value>& error);
		std::string PrintErrorMessage(const v8::Handle<v8::Message>& message, const v8::Handle<v8::Value>& error);

		jweak TryGetJavaThrowableObject(JEnv& env, const v8::Handle<v8::Object>& jsObj);

		static ExceptionUtil* instance;

		bool initialized;

		JavaVM *jvm;

		ObjectManager *objectManager;

		jclass PlatformClass;

		jclass THROWABLE_CLASS;

		jmethodID THROWABLE_GET_CAUSE_METHOD_ID;

		jmethodID THROWABLE_GET_STACK_TRACE_METHOD_ID;

		jmethodID THROWABLE_TO_STRING_METHOD_ID;

		jclass STACK_TRACE_ELEMENT_CLASS;

		jmethodID FRAME_TO_STRING_METHOD_ID;

		jmethodID IS_JAVA_THROWABLE_METHOD_ID;
	};
}

#endif /* EXCEPTIONUTIL_H_ */
