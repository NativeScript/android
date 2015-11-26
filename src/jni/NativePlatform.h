#ifndef NATIVEPLATFORM_H_
#define NATIVEPLATFORM_H_

#include "JniLocalRef.h"
#include "ObjectManager.h"
#include "SimpleAllocator.h"

using namespace v8;
using namespace std;
using namespace tns;

void AppInitCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

jobject ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value, int classReturnType);

namespace tns {
	class NativePlatform {
		public:
			jint static JNI_ON_LOAD(JavaVM *vm, void *reserved);
			void static InitNativeScript(JNIEnv *_env, jobject obj, jstring filesPath, jint appJavaObjectId, jboolean verboseLoggingEnabled, jstring packageName, jobjectArray args);
			void static RunModule(JNIEnv *_env, jobject obj, jstring scriptFile);
			jobject static RunScript(JNIEnv *_env, jobject obj, jstring scriptFile);
			jobject static CallJSMethodNative(JNIEnv *_env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs);
			void static CreateJSInstanceNative(JNIEnv *_env, jobject obj, jobject javaObject, jint javaObjectID, jstring className);
			jint static GenerateNewObjectId(JNIEnv *env, jobject obj);
			void static AdjustAmountOfExternalAllocatedMemoryNative(JNIEnv *env, jobject obj, jlong usedMemory);
			void static PassUncaughtExceptionToJsNative(JNIEnv *env, jobject obj, jthrowable exception, jstring stackTrace);

			bool LogEnabled = true;
		private:

			void static PrepareExtendFunction(Isolate *isolate, jstring filesPath);
			void static PrepareV8Runtime(JEnv& env, string filesPath, jstring packageName);
			void static AppInitCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
			jobject static ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value, int classReturnType);
	};
}

#endif /*#ifndef NATIVEPLATFORM_H_*/
