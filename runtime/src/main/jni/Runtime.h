#ifndef RUNTIME_H_
#define RUNTIME_H_

#include "v8.h"
#include "JniLocalRef.h"
#include "ObjectManager.h"
#include "SimpleAllocator.h"
#include "WeakRef.h"
#include "ArrayBufferHelper.h"
#include "Profiler.h"
#include "ModuleInternal.h"
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

			static Runtime* GetRuntime(int runtimeId);

			static Runtime* GetRuntime(v8::Isolate *isolate);

			static ObjectManager* GetObjectManager(v8::Isolate *isolate);

			static void Init(JavaVM *vm, void *reserved);

			static void Init(JNIEnv *_env, jobject obj, int runtimeId, jstring filesPath, jstring nativeLibsDir, jstring packageName, jobjectArray args, jstring callingDir, jobject jsDebugger);

			void Init(jstring filesPath, jstring nativeLibsDir, jstring packageName, jobjectArray args, jstring callingDir, jobject jsDebugger);

			v8::Isolate* GetIsolate() const;

			jobject GetJavaRuntime() const;

			ObjectManager* GetObjectManager() const;

			void RunModule(JNIEnv *_env, jobject obj, jstring scriptFile);
			void RunWorker(jstring scriptFile);
			jobject RunScript(JNIEnv *_env, jobject obj, jstring scriptFile);
			jobject CallJSMethodNative(JNIEnv *_env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs);
			void CreateJSInstanceNative(JNIEnv *_env, jobject obj, jobject javaObject, jint javaObjectID, jstring className);
			jint GenerateNewObjectId(JNIEnv *env, jobject obj);
			void AdjustAmountOfExternalAllocatedMemory();
			bool NotifyGC(JNIEnv *env, jobject obj);
			bool TryCallGC();
			void PassUncaughtExceptionToJsNative(JNIEnv *env, jobject obj, jthrowable exception, jstring stackTrace);
			void PassUncaughtExceptionFromWorkerToMainHandler(v8::Local<v8::String> message, v8::Local<v8::String> stackTrace, v8::Local<v8::String> filename, int lineno);
			void ClearStartupData(JNIEnv *env, jobject obj);
			void DestroyRuntime();

		private:
			Runtime(JNIEnv *env, jobject runtime, int id);

			JEnv m_env;
			int m_id;
			jobject m_runtime;
			v8::Isolate *m_isolate;

			ObjectManager *m_objectManager;

			ModuleInternal m_module;

			ArrayBufferHelper m_arrayBufferHelper;

			WeakRef m_weakRef;

			Profiler m_profiler;

			v8::StartupData *m_startupData = nullptr;
			MemoryMappedFile *m_heapSnapshotBlob = nullptr;

			int64_t m_lastUsedMemory;

			v8::Persistent<v8::Function> *m_gcFunc;
			volatile bool m_runGC;

			v8::Isolate* PrepareV8Runtime(const std::string& filesPath, jstring nativeLibsDir, jstring packageName, jstring callingDir, jobject jsDebugger, jstring profilerOutputDir);
			jobject ConvertJsValueToJavaObject(JEnv& env, const v8::Local<v8::Value>& value, int classReturnType);

			static std::map<int, Runtime*> s_id2RuntimeCache;

			static std::map<v8::Isolate*, Runtime*> s_isolate2RuntimesCache;

			static JavaVM *s_jvm;

			static jmethodID GET_USED_MEMORY_METHOD_ID;

			static bool s_mainThreadInitialized;
	};
}

#endif /*#ifndef RUNTIME_H_*/
