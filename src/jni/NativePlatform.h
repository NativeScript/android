#ifndef NATIVEPLATFORM_H_
#define NATIVEPLATFORM_H_

#include "v8.h"
#include "JniLocalRef.h"
#include "ObjectManager.h"
#include "SimpleAllocator.h"

void AppInitCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

jobject ConvertJsValueToJavaObject(tns::JEnv& env, const v8::Local<v8::Value>& value, int classReturnType);

namespace tns
{
	class NativePlatform
	{
		public:
			static jint JNI_ON_LOAD(JavaVM *vm, void *reserved);
			static v8::Isolate* InitNativeScript(JNIEnv *_env, jobject obj, jstring filesPath, jint appJavaObjectId, jboolean verboseLoggingEnabled, jstring packageName, jobjectArray args);
			static void RunModule(JNIEnv *_env, jobject obj, jstring scriptFile);
			static jobject RunScript(JNIEnv *_env, jobject obj, jstring scriptFile);
			static jobject CallJSMethodNative(JNIEnv *_env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs);
			static void CreateJSInstanceNative(JNIEnv *_env, jobject obj, jobject javaObject, jint javaObjectID, jstring className);
			static jint GenerateNewObjectId(JNIEnv *env, jobject obj);
			static void AdjustAmountOfExternalAllocatedMemoryNative(JNIEnv *env, jobject obj, jlong usedMemory);
			static void PassUncaughtExceptionToJsNative(JNIEnv *env, jobject obj, jthrowable exception, jstring stackTrace);

			bool LogEnabled = true;
			private:

			static void PrepareExtendFunction(v8::Isolate *isolate, jstring filesPath);
			static v8::Isolate* PrepareV8Runtime(JEnv& env, const std::string& filesPath, jstring packageName);
			static void AppInitCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
			static jobject ConvertJsValueToJavaObject(JEnv& env, const v8::Local<v8::Value>& value, int classReturnType);

			static v8::Isolate *s_isolate;
	};
}

#endif /*#ifndef NATIVEPLATFORM_H_*/
