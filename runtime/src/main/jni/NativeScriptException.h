#ifndef NATIVESCRIPTEXCEPTION_H_
#define NATIVESCRIPTEXCEPTION_H_

#include "v8.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "ObjectManager.h"
#include "include/v8.h"

namespace tns {
class NativeScriptException {
    public:
        /*
         * Generates a NativeScriptException with java error from environment
         */
        NativeScriptException(JEnv& env);

        /*
         * Generates a NativeScriptException with given message
         */
        NativeScriptException(const std::string& message);

        /*
         * Generates a NativeScriptException with javascript error from TryCatch and a prepend message if any
         */
        NativeScriptException(v8::TryCatch& tc, const std::string& message = "");

        void ReThrowToV8();
        void ReThrowToJava();

        static void Init(ObjectManager* objectManager);

        /*
         * This handler is attached to v8 to handle uncaught javascript exceptions.
         */
        static void OnUncaughtError(v8::Local<v8::Message> message, v8::Local<v8::Value> error);

        /*
         * Calls the global "__onUncaughtError" if such is provided
         */
        static void CallJsFuncWithErr(v8::Local<v8::Value> errObj);

    private:
        /*
         * Try to get native exception or NativeScriptException from js object
         */
        JniLocalRef TryGetJavaThrowableObject(JEnv& env, const v8::Local<v8::Object>& jsObj);

        /*
         * Gets java exception stack message from jthrowable
         */
        std::string GetExceptionMessage(JEnv& env, jthrowable exception);

        /*
         * Gets the member m_javaException, wraps it and creates a javascript error object from it
         */
        v8::Local<v8::Value> WrapJavaToJsException();

        /*
         * Gets all the information from a java exception and puts it in a javascript errror object
         */
        v8::Local<v8::Value> GetJavaExceptionFromEnv(const JniLocalRef& exc, JEnv& env);

        /*
         * Gets all the information from a js message and an js error object and puts it in a string
         */
        static std::string GetErrorMessage(const v8::Local<v8::Message>& message, v8::Local<v8::Value>& error);

        /*
         * Generates string stack trace from js StackTrace
         */
        static std::string GetErrorStackTrace(const v8::Local<v8::StackTrace>& stackTrace);

        /*
         *	Adds a prepend message to the normal message process
         */
        std::string GetFullMessage(const v8::TryCatch& tc, bool isExceptionEmpty, bool isMessageEmpty, const std::string& prependMessage = "");

        v8::Persistent<v8::Value>* m_javascriptException;
        JniLocalRef m_javaException;
        std::string m_message;

        static jclass RUNTIME_CLASS;
        static jclass THROWABLE_CLASS;
        static jclass NATIVESCRIPTEXCEPTION_CLASS;
        static jmethodID NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID;
        static jmethodID NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID;
        static jmethodID NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID;

        static void PrintErrorMessage(const std::string& errorMessage);
};
}

#endif /* NATIVESCRIPTEXCEPTION_H_ */
