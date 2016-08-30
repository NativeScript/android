#ifndef RUNTIME_H_
#define RUNTIME_H_

#include "v8.h"
#include "JniLocalRef.h"
#include "ObjectManager.h"
#include "SimpleAllocator.h"
#include "WeakRef.h"
#include "ArrayBufferHelper.h"
#include "Profiler.h"
#include "Module.h"
#include "File.h"

jobject ConvertJsValueToJavaObject(tns::JEnv& env, const v8::Local<v8::Value>& value, int classReturnType);

namespace tns
{
	class Runtime
	{
		public:
		    enum IsolateData {
		    	RUNTIME = 0,
		    	CONSTANTS = 1
		    };

			static Runtime *GetRuntime(int runtimeId);

			static Runtime *GetRuntime(v8::Isolate *isolate);

			static void Init(JavaVM *vm, void *reserved);

			static void Init(JNIEnv *_env, jobject obj, int runtimeId, jstring filesPath, jboolean verboseLoggingEnabled, jstring packageName, jobjectArray args, jobject jsDebugger);

			void Init(jstring filesPath, bool verboseLoggingEnabled, jstring packageName, jobjectArray args, jobject jsDebugger);

			v8::Isolate* GetIsolate() const;

			jobject GetJavaRuntime() const;

			ObjectManager* GetObjectManager() const;

			void RunModule(JNIEnv *_env, jobject obj, jstring scriptFile);
			jobject RunScript(JNIEnv *_env, jobject obj, jstring scriptFile);
			jobject CallJSMethodNative(JNIEnv *_env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs);
			void CreateJSInstanceNative(JNIEnv *_env, jobject obj, jobject javaObject, jint javaObjectID, jstring className);
			jint GenerateNewObjectId(JNIEnv *env, jobject obj);
			void AdjustAmountOfExternalAllocatedMemoryNative(JNIEnv *env, jobject obj, jlong usedMemory);
			void PassUncaughtExceptionToJsNative(JNIEnv *env, jobject obj, jthrowable exception, jstring stackTrace);
			void ClearStartupData(JNIEnv *env, jobject obj);

		private:
			Runtime(JNIEnv *env, jobject runtime, int id);

			JEnv m_env;
			int m_id;
			jobject m_runtime;
			v8::Isolate *m_isolate;

			ObjectManager *m_objectManager;

			Module m_module;

			ArrayBufferHelper m_arrayBufferHelper;

			WeakRef m_weakRef;

			Profiler m_profiler;

			v8::StartupData *m_startupData = nullptr;
			MemoryMappedFile *m_heapSnapshotBlob = nullptr;

			v8::Isolate* PrepareV8Runtime(const std::string& filesPath, jstring packageName, jobject jsDebugger, jstring profilerOutputDir);
			jobject ConvertJsValueToJavaObject(JEnv& env, const v8::Local<v8::Value>& value, int classReturnType);

			static std::map<int, Runtime*> s_id2RuntimeCache;

			static std::map<v8::Isolate*, Runtime*> s_isolate2RuntimesCache;

			static JavaVM *s_jvm;

			static bool s_mainThreadInitialized;
	};
}

#endif /*#ifndef RUNTIME_H_*/
