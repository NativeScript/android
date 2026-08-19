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
#include "Timers.h"
#include "EventLoop.h"
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <android/looper.h>
#include <fcntl.h>

// Declared rather than included: js_native_api_types.h pins NAPI_VERSION for
// the whole translation unit, and Runtime.h reaches nearly all of them.
typedef struct napi_env__* napi_env;

namespace tns {
class PromiseRejectionTracker;
class RuntimeState;

class Runtime {
    public:
        enum IsolateData {
            RUNTIME = 0,
            CONSTANTS = 1,
            WORKER_WRAPPER = 2
        };

        /*
         * What happens to an unprevented uncaught JS error (sync throw in a
         * native-initiated callback, or an unhandled promise rejection):
         * Report (default) - report and continue, never crash; Throw - hand
         * it to the native layer as a real Java exception (pre-9.1 behavior).
         * Configured via `uncaughtErrorPolicy` in the app's package.json.
         */
        enum class UncaughtErrorPolicy {
            Report,
            Throw
        };

        ~Runtime();

        static Runtime* GetRuntime(int runtimeId);

        static Runtime* GetRuntime(v8::Isolate* isolate);

        static Runtime* GetRuntimeFromIsolateData(v8::Isolate* isolate);

        /*
         * The runtime for this isolate, or null. Unlike the accessors above it
         * neither throws nor locks -- it only reads the isolate's own data
         * slot -- which is what makes it usable from the places that must not
         * throw: GC weak callbacks, teardown paths, and the error handlers
         * that run while a runtime is going away.
         */
        static Runtime* TryGetRuntime(v8::Isolate* isolate) {
            return static_cast<Runtime*>(
                isolate->GetData((uint32_t)IsolateData::RUNTIME));
        }

        /*
         * The runtime whose home thread is the calling thread, or null. Set at
         * the end of PrepareV8Runtime; may be stale after a Runtime destroyed
         * on another thread, so consumers must validate through
         * GetNapiEnvIfAlive rather than dereference it.
         */
        static Runtime* GetCurrentRuntime() {
            return s_currentRuntime;
        }

        /*
         * The Node-API environment for this runtime's context. Null before
         * PrepareV8Runtime creates it and after DestroyRuntime destroys it.
         */
        napi_env GetNapiEnv() const {
            return m_napiEnv;
        }

        /*
         * Resolves the env while holding the registry lock, so a possibly-
         * stale pointer (e.g. the thread-local left behind when a Runtime was
         * destroyed on another thread) is never dereferenced outside it. The
         * returned env's validity is governed by the Node-API threading
         * contract: it is only safe to use on the runtime's own thread, where
         * teardown cannot race it.
         */
        static napi_env GetNapiEnvIfAlive(const Runtime* runtime);

        static ObjectManager* GetObjectManager(v8::Isolate* isolate);

        static void Init(JavaVM* vm, void* reserved);

        static void Init(JNIEnv* _env, jobject obj, int runtimeId, jstring filesPath, jstring nativeLibsDir, jboolean verboseLoggingEnabled, jboolean isDebuggable, jstring packageName, jobjectArray args, jstring callingDir, int maxLogcatObjectSize,
                         bool forceLog);

        static void SetManualInstrumentationMode(jstring mode);

        void Init(JNIEnv* env, jstring filesPath, jstring nativeLibsDir, bool verboseLoggingEnabled, bool isDebuggable, jstring packageName, jobjectArray args, jstring callingDir, int maxLogcatObjectSize, bool forceLog);

        v8::Isolate* GetIsolate() const;

        /*
         * Per-runtime storage for subsystem state bound to this isolate; see
         * RuntimeState.h. Released in DestroyRuntime while the isolate is
         * still alive, so state holding v8::Persistents can be torn down.
         */
        RuntimeState* GetState() const {
            return m_state.get();
        }

        bool IsMainThread() const {
            return m_isMainThread;
        }

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

        /*
         * The main runtime's event loop, set once when the main runtime
         * initializes. __runOnMainThread posts its (own-isolate) closures
         * here from any runtime's thread.
         */
        static std::shared_ptr<EventLoop> GetMainEventLoop() {
            return s_mainEventLoop;
        }
        static JavaVM* GetJVM() {
            return s_jvm;
        }

        /*
         * Scheduler bound to this runtime's looper. Child workers hold a
         * weak_ptr to their parent runtime's loop for worker -> parent
         * delivery (messages, errors, cleanup notifications).
         */
        std::shared_ptr<EventLoop> GetEventLoop() const {
            return m_eventLoop;
        }

        /*
         * This runtime's CommonJS loader. `ns:module`'s createRequire mints its
         * requires through it, so the require it hands out is the very one the
         * loader builds for every module.
         */
        ModuleInternal* GetModuleInternal() {
            return &m_module;
        }

        /*
         * Milliseconds since this runtime's time origin, on the monotonic
         * clock. Not inline: v8::Platform is only forward-declared through
         * v8.h here.
         */
        double PerformanceNowMillis();

        /*
         * Wall-clock milliseconds since the Unix epoch at the moment the time
         * origin was captured, on the same base as Date.now(); this is
         * performance.timeOrigin.
         */
        double TimeOriginMillis() const {
            return m_timeOriginRealtimeMs;
        }

        /*
         * The time origin as a CLOCK_MONOTONIC reading in milliseconds -- the
         * same clock and epoch Choreographer stamps frames with. Subtracting
         * it from such a timestamp maps it onto this isolate's performance
         * timeline.
         */
        double TimeOriginMonotonicMillis() const {
            return m_timeOriginMonotonic * 1000.0;
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
        v8::Global<v8::Function>& DispatchNativeUncaughtErrorFunc() {
            return m_dispatchNativeUncaughtErrorFunc;
        }
        PromiseRejectionTracker* PromiseRejections() const {
            return m_promiseRejections.get();
        }

        UncaughtErrorPolicy GetUncaughtErrorPolicy() const {
            return m_uncaughtErrorPolicy;
        }
        bool GetDiscardUncaughtJsExceptions() const {
            return m_discardUncaughtJsExceptions;
        }

        /*
         * Depth of in-flight JS->Java calls on this runtime's thread. When a
         * JS callback invoked from Java throws while the depth is non-zero,
         * the whole chain is JS-initiated (JS -> Java -> JS), so the throw
         * must propagate back to the outer JS catch instead of being
         * contained at the boundary. Maintained by CallbackHandlers around
         * the JS->Java invocation sites.
         */
        int JavaCallDepth() const {
            return m_javaCallDepth;
        }
        void EnterJavaCall() {
            ++m_javaCallDepth;
        }
        void LeaveJavaCall() {
            --m_javaCallDepth;
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

        std::shared_ptr<EventLoop> m_eventLoop;

        std::unique_ptr<RuntimeState> m_state;

        napi_env m_napiEnv = nullptr;

        v8::Global<v8::Object> m_globalEventTarget;
        v8::Global<v8::Function> m_dispatchErrorEventFunc;
        v8::Global<v8::Function> m_dispatchUnhandledRejectionFunc;
        v8::Global<v8::Function> m_dispatchRejectionHandledFunc;
        v8::Global<v8::Function> m_dispatchNativeUncaughtErrorFunc;
        std::unique_ptr<PromiseRejectionTracker> m_promiseRejections;

        UncaughtErrorPolicy m_uncaughtErrorPolicy = UncaughtErrorPolicy::Report;
        bool m_discardUncaughtJsExceptions = false;
        int m_javaCallDepth = 0;

        int64_t m_lastUsedMemory;

        v8::Persistent<v8::Function>* m_gcFunc;
        volatile bool m_runGC;

        v8::Persistent<v8::Context>* m_context;

        // Decided by ElectMainRuntime, before anything can read it.
        bool m_isMainThread = false;

        // This isolate's performance time origin, captured at isolate
        // creation: the monotonic clock reading now() is relative to, and the
        // wall-clock milliseconds that reading corresponds to.
        double m_timeOriginMonotonic {0};
        double m_timeOriginRealtimeMs {0};

        v8::Isolate* PrepareV8Runtime(const std::string& filesPath, const std::string& nativeLibsDir, const std::string& packageName, bool isDebuggable, const std::string& callingDir, const std::string& profilerOutputDir, const int maxLogcatObjectSize, const bool forceLog);

        /*
         * Decides whether this runtime is the main one and, if so, performs the
         * once-per-process V8 initialization -- both inside one critical
         * section, so two concurrent bootstraps cannot each elect themselves
         * and overwrite Runtime::platform and s_mainEventLoop.
         *
         * A runtime that loses the election blocks here until the main runtime
         * is ready, because everything after this point depends on state only
         * the main runtime publishes -- most of all the metadata tree, which it
         * alone builds and every other runtime reads. Throws if the main
         * runtime failed to initialize.
         */
        void ElectMainRuntime();

        // Publishes (or, on failure, withdraws) the main runtime's readiness
        // and wakes anything blocked in ElectMainRuntime.
        static void SignalMainRuntimeReady(bool failed);

        /*
         * Unwinds an initialization that threw after the isolate existed. The
         * Java-side rollback only unwinds Java state, which would otherwise
         * leave the isolate in the runtime caches and the half-built Runtime
         * holding everything it had allocated.
         */
        void UnwindFailedInit();
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

        /*
         * The main runtime has finished initializing. Distinct from the
         * election below: the elected runtime is not usable by others until it
         * has built the metadata they all read.
         */
        static std::atomic<bool> s_mainThreadInitialized;

        // Guards main-runtime election and the V8 initialization that goes
        // with it; s_mainInitReady signals the flags above and below.
        static std::mutex s_mainInitMutex;
        static std::condition_variable s_mainInitReady;
        static bool s_mainRuntimeElected;
        static bool s_mainRuntimeFailed;

        static std::shared_ptr<EventLoop> s_mainEventLoop;

        static thread_local Runtime* s_currentRuntime;

#ifdef APPLICATION_IN_DEBUG
        std::mutex m_fileWriteMutex;
#endif
};
}

#endif /*#ifndef RUNTIME_H_*/
