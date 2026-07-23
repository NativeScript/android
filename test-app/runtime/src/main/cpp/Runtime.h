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
#include "MessageLoopTimer.h"
#include "File.h"
#include "Timers.h"
#include "LooperTasks.h"
#include <memory>
#include <mutex>
#include <android/looper.h>
#include <fcntl.h>

namespace tns {
class PromiseRejectionTracker;

class Runtime {
    public:
        enum IsolateData {
            RUNTIME = 0,
            CONSTANTS = 1,
            WORKER_WRAPPER = 2
        };

        ~Runtime();

        static Runtime* GetRuntime(int runtimeId);

        static Runtime* GetRuntime(v8::Isolate* isolate);

        static Runtime* GetRuntimeFromIsolateData(v8::Isolate* isolate);

        static ObjectManager* GetObjectManager(v8::Isolate* isolate);

        static void Init(JavaVM* vm, void* reserved);

        static void Init(JNIEnv* _env, jobject obj, int runtimeId, jstring filesPath, jstring nativeLibsDir, jboolean verboseLoggingEnabled, jboolean isDebuggable, jstring packageName, jobjectArray args, jstring callingDir, int maxLogcatObjectSize,
                         bool forceLog);

        static void SetManualInstrumentationMode(jstring mode);

        void Init(JNIEnv* env, jstring filesPath, jstring nativeLibsDir, bool verboseLoggingEnabled, bool isDebuggable, jstring packageName, jobjectArray args, jstring callingDir, int maxLogcatObjectSize, bool forceLog);

        v8::Isolate* GetIsolate() const;

        jobject GetJavaRuntime() const;

        ObjectManager* GetObjectManager() const;

        void RunModule(JNIEnv* _env, jobject obj, jstring scriptFile);
        void RunModule(const char *moduleName);
        void RunWorker(const std::string& filePath);
        jobject RunScript(JNIEnv* _env, jobject obj, jstring scriptFile);
        jobject CallJSMethodNative(JNIEnv* _env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs);
        void CreateJSInstanceNative(JNIEnv* _env, jobject obj, jobject javaObject, jint javaObjectID, jstring className);
        jint GenerateNewObjectId(JNIEnv* env, jobject obj);
        void AdjustAmountOfExternalAllocatedMemory();
        bool NotifyGC(JNIEnv* env, jobject obj);
        bool TryCallGC();
        /*
         * Reports a Java-side exception to JS: dispatches the WHATWG `error`
         * event first and, unless a listener called preventDefault(), calls
         * the __onUncaughtError/__onDiscardedError shim. Returns JNI_TRUE
         * when a listener prevented the default, so the Java caller can treat
         * the exception as fully handled (e.g. skip crashing the process).
         */
        jboolean PassExceptionToJsNative(JNIEnv* env, jobject obj, jthrowable exception, jstring message, jstring fullStackTrace, jstring jsStackTrace, jboolean isDiscarded);
        void DestroyRuntime();

        void Lock();
        void Unlock();

        int GetId();

        v8::Local<v8::Context> GetContext();

        static v8::Platform* platform;

        std::string ReadFileText(const std::string& filePath);

        static int GetWriter();
        static int GetReader();
        static ALooper* GetMainLooper() {
            return m_mainLooper;
        }
        static JavaVM* GetJVM() {
            return s_jvm;
        }

        /*
         * Task queue bound to this runtime's looper. Child workers hold a
         * weak_ptr to their parent runtime's queue for worker -> parent
         * delivery (messages, errors, cleanup notifications).
         */
        std::shared_ptr<LooperTasks> GetLooperTasks() const {
            return m_looperTasks;
        }

        /*
         * WHATWG events state, the Android analogue of the iOS runtime's
         * Caches members of the same names. The backing event target is set
         * by Events::Init, the three dispatch closures by ErrorEvents::Init,
         * and the rejection tracker is created in PrepareV8Runtime. All are
         * released in DestroyRuntime before the isolate is disposed.
         *
         * Internal EventTarget instance backing the global. Holds the real
         * listener store, so native layers dispatch through it without going
         * through overwritable globals (future native consumers like
         * AbortSignal dispatch through it too).
         */
        v8::Global<v8::Object>& GlobalEventTarget() {
            return m_globalEventTarget;
        }
        /*
         * Error-events dispatch closures returned by the bootstrap IIFE
         * (ErrorEvents::Init). They close over the internal listener store,
         * so native dispatch keeps working even if app code overwrites
         * globalThis.dispatchEvent.
         */
        v8::Global<v8::Function>& DispatchErrorEventFunc() {
            return m_dispatchErrorEventFunc;
        }
        v8::Global<v8::Function>& DispatchUnhandledRejectionFunc() {
            return m_dispatchUnhandledRejectionFunc;
        }
        v8::Global<v8::Function>& DispatchRejectionHandledFunc() {
            return m_dispatchRejectionHandledFunc;
        }
        PromiseRejectionTracker* PromiseRejections() const {
            return m_promiseRejections.get();
        }

    private:
        Runtime(JNIEnv* env, jobject runtime, int id);

        int m_id;
        jobject m_runtime;
        v8::Isolate* m_isolate;

        ObjectManager* m_objectManager;

        ModuleInternal m_module;

        ArrayBufferHelper m_arrayBufferHelper;

        WeakRef m_weakRef;

        Profiler m_profiler;

        MessageLoopTimer* m_loopTimer;

        std::shared_ptr<LooperTasks> m_looperTasks;

        v8::Global<v8::Object> m_globalEventTarget;
        v8::Global<v8::Function> m_dispatchErrorEventFunc;
        v8::Global<v8::Function> m_dispatchUnhandledRejectionFunc;
        v8::Global<v8::Function> m_dispatchRejectionHandledFunc;
        std::unique_ptr<PromiseRejectionTracker> m_promiseRejections;

        int64_t m_lastUsedMemory;

        v8::Persistent<v8::Function>* m_gcFunc;
        volatile bool m_runGC;

        v8::Persistent<v8::Context>* m_context;

        bool m_isMainThread;

        // High resolution timing origin values
        // m_startTime: monotonic clock time captured at isolate creation
        // m_realtimeOrigin: wall-clock time origin (milliseconds) captured at isolate creation
        double m_startTime {0};
        double m_realtimeOrigin {0};

        // performance.now() callback
        static void PerformanceNowCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        v8::Isolate* PrepareV8Runtime(const std::string& filesPath, const std::string& nativeLibsDir, const std::string& packageName, bool isDebuggable, const std::string& callingDir, const std::string& profilerOutputDir, const int maxLogcatObjectSize, const bool forceLog);
        jobject ConvertJsValueToJavaObject(JEnv& env, const v8::Local<v8::Value>& value, int classReturnType);
        static int GetAndroidVersion();
        static int m_androidVersion;

        static robin_hood::unordered_map<int, Runtime*> s_id2RuntimeCache;

        static robin_hood::unordered_map<v8::Isolate*, Runtime*> s_isolate2RuntimesCache;

        /*
         * Guards the two caches above: runtimes are now constructed and
         * destroyed concurrently on native-spawned worker threads.
         */
        static std::mutex s_runtimeCacheMutex;

        static JavaVM* s_jvm;

        static jmethodID GET_USED_MEMORY_METHOD_ID;

        static bool s_mainThreadInitialized;

        static ALooper* m_mainLooper;

        static int m_mainLooper_fd[2];

#ifdef APPLICATION_IN_DEBUG
        std::mutex m_fileWriteMutex;
#endif
};
}

#endif /*#ifndef RUNTIME_H_*/
