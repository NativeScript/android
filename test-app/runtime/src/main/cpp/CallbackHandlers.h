#ifndef CALLBACKHANDLERS_H_
#define CALLBACKHANDLERS_H_

#include <string>
#include <map>
#include <vector>
#include "JEnv.h"
#include "ArgsWrapper.h"
#include "MetadataEntry.h"
#include "FieldCallbackData.h"
#include "MetadataTreeNode.h"
#include "NumericCasts.h"
#include "FieldAccessor.h"
#include "ArrayElementAccessor.h"
#include "ObjectManager.h"
#include "include/v8.h"
#include "robin_hood.h"

namespace tns {
    class CallbackHandlers {
    public:

        /*
         * Stores persistent handles of all 'Worker' objects initialized on the main thread
         * Note: No isolates different than that of the main thread should access this map
         */
        static std::map<int, v8::Persistent<v8::Object> *> id2WorkerMap;

        static int nextWorkerId;

        static void Init(v8::Isolate *isolate);

        static v8::Local<v8::Object>
        CreateJSWrapper(v8::Isolate *isolate, jint javaObjectID, const std::string &typeName);

        static bool RegisterInstance(v8::Isolate *isolate, const v8::Local<v8::Object> &jsObject,
                                     const std::string &fullClassName,
                                     const ArgsWrapper &argWrapper,
                                     const v8::Local<v8::Object> &implementationObject,
                                     bool isInterface,
                                     const std::string &baseClassName = std::string());

        static jclass ResolveClass(v8::Isolate *isolate, const std::string &baseClassName,
                                   const std::string &fullClassName,
                                   const v8::Local<v8::Object> &implementationObject,
                                   bool isInterface);

        static std::string ResolveClassName(v8::Isolate *isolate, jclass &clazz);

        static v8::Local<v8::Value>
        GetArrayElement(v8::Local<v8::Context> context, const v8::Local<v8::Object> &array, uint32_t index,
                        const std::string &arraySignature);

        static void
        SetArrayElement(v8::Local<v8::Context> context, const v8::Local<v8::Object> &array, uint32_t index,
                        const std::string &arraySignature, v8::Local<v8::Value> &value);

        static int GetArrayLength(v8::Isolate *isolate, const v8::Local<v8::Object> &arr);

        static void
        CallJavaMethod(const v8::Local<v8::Object> &caller, const std::string &className,
                       const std::string &methodName, MetadataEntry *entry, bool isFromInterface,
                       bool isStatic, bool isSuper,
                       const v8::FunctionCallbackInfo<v8::Value> &args);

        static v8::Local<v8::Value>
        CallJSMethod(v8::Isolate *isolate, JNIEnv *_env, const v8::Local<v8::Object> &jsObject,
                     const std::string &methodName, jobjectArray args);

        static v8::Local<v8::Value>
        GetJavaField(v8::Isolate *isolate, const v8::Local<v8::Object> &caller,
                     FieldCallbackData *fieldData);

        static void SetJavaField(v8::Isolate *isolate, const v8::Local<v8::Object> &target,
                                 const v8::Local<v8::Value> &value, FieldCallbackData *fieldData);


        static void RunOnMainThreadCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static int RunOnMainThreadFdCallback(int fd, int events, void* data);

        static void LogMethodCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void TimeCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void
        DumpReferenceTablesMethodCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void DrainMicrotaskCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void DumpReferenceTablesMethod();

        static void ExitMethodCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void CreateGlobalCastFunctions(v8::Isolate *isolate,
                                              const v8::Local<v8::ObjectTemplate> &globalTemplate);

        static std::vector<std::string> GetTypeMetadata(const std::string &name, int index);

        /*
         * Gets all methods in the implementation object, and packs them in a jobjectArray
         * to pass them to Java Land, so that their corresponding Java callbacks are written when
         * the dexFactory generates the class
         */
        static jobjectArray
        GetMethodOverrides(JEnv &env, const v8::Local<v8::Object> &implementationObject);

        /*
         * Gets all interfaces declared in the 'interfaces' array inside the implementation object,
         * and packs them in a jobjectArray to pass them to Java Land, so that they may be
         * implemented when the dexFactory generates the corresponding class
         */
        static jobjectArray
        GetImplementedInterfaces(JEnv &env, const v8::Local<v8::Object> &implementationObject);

        static void
        EnableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void
        DisableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void ReleaseNativeCounterpartCallback(const v8::FunctionCallbackInfo<v8::Value> &info);

        static v8::Local<v8::Object> FindClass(v8::Isolate *isolate, const std::string &className);

        static void NewThreadCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        /*
         * main -> worker messaging
         * Fired when a Worker instance's postMessage is called
         */
        static void
        WorkerObjectPostMessageCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        /*
         * main -> worker messaging
         * Fired when worker object has "postMessage" and the worker has implemented "onMessage" handler
         * In case "onMessage" handler isn't implemented no exception is thrown
         */
        static void WorkerGlobalOnMessageCallback(v8::Isolate *isolate, jstring message);

        /*
         * worker -> main thread messaging
         * Fired when a Worker script's "postMessage" is called
         */
        static void
        WorkerGlobalPostMessageCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        /*
         * worker -> main messaging
         * Fired when worker has sent a message to main and the worker object has implemented "onMessage" handler
         * In case "onMessage" handler isn't implemented no exception is thrown
         */
        static void
        WorkerObjectOnMessageCallback(v8::Isolate *isolate, jint workerId, jstring message);

        /*
         * Fired when a Worker instance's terminate is called (immediately stops execution of the thread)
         */
        static void WorkerObjectTerminateCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        /*
         * Fired when a Worker script's close is called
         */
        static void WorkerGlobalCloseCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        /*
         * Clears the persistent Worker object handle associated with a workerId
         * Occurs when calling a worker object's `terminate` or a worker thread's global scope `close`
         */
        static void ClearWorkerPersistent(int workerId);

        /*
         * Terminates the currently executing Isolate. No scripts can be executed after this call
         */
        static void TerminateWorkerThread(v8::Isolate *isolate);

        /*
         * Is called when an unhandled exception is thrown inside the worker
         * Will execute 'onerror' if one is provided inside the Worker Scope
         * Will make the exception "bubble up" through to main, to be handled by the Worker Object
         * if 'onerror' isn't implemented or returns false
         */
        static void CallWorkerScopeOnErrorHandle(v8::Isolate *isolate, v8::TryCatch &tc);

        /*
         * Is called when an unhandled exception bubbles up from the worker scope to the main thread Worker Object
         * Will execute `onerror` if one is implemented for the Worker Object instance
         * Will throw a NativeScript Exception if 'onerror' isn't implemented or returns false
         */
        static void
        CallWorkerObjectOnErrorHandle(v8::Isolate *isolate, jint workerId, jstring message,
                                      jstring stackTrace, jstring filename, jint lineno,
                                      jstring threadName);

        static void RemoveIsolateEntries(v8::Isolate *isolate);


        static void PostFrameCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        static void RemoveFrameCallback(const v8::FunctionCallbackInfo<v8::Value> &args);

        struct AChoreographer;


        typedef void (*AChoreographer_frameCallback)(long frameTimeNanos, void* data);

        typedef void (*AChoreographer_frameCallback64)(int64_t frameTimeNanos, void* data);

        typedef AChoreographer* (*func_AChoreographer_getInstance)();

        typedef void (*func_AChoreographer_postFrameCallback)(
                AChoreographer* choreographer, AChoreographer_frameCallback callback,
                void* data);

        typedef void (*func_AChoreographer_postFrameCallback64)(
                AChoreographer* choreographer, AChoreographer_frameCallback64 callback,
                void* data);

        typedef void (*func_AChoreographer_postFrameCallbackDelayed)(
                AChoreographer* choreographer, AChoreographer_frameCallback callback,
                void* data, long delayMillis);

        typedef void (*func_AChoreographer_postFrameCallbackDelayed64)(
                AChoreographer* choreographer, AChoreographer_frameCallback64 callback,
                void* data, uint32_t delayMillis);


    private:
        CallbackHandlers() {
        }

        static void AdjustAmountOfExternalAllocatedMemory(JEnv &env, v8::Isolate *isolate);

        /*
         * Helper method that creates a java string array for sending strings over JNI
         */
        static jobjectArray GetJavaStringArray(JEnv &env, int length);

        static void validateProvidedArgumentsLength(const v8::FunctionCallbackInfo<v8::Value> &args, int expectedSize);

        static short MAX_JAVA_STRING_ARRAY_LENGTH;

        static jclass RUNTIME_CLASS;

        static jclass JAVA_LANG_STRING;

        static jmethodID RESOLVE_CLASS_METHOD_ID;

        static jfieldID CURRENT_OBJECTID_FIELD_ID;

        static jmethodID MAKE_INSTANCE_STRONG_ID;

        static jmethodID GET_TYPE_METADATA;

        static jmethodID ENABLE_VERBOSE_LOGGING_METHOD_ID;

        static jmethodID DISABLE_VERBOSE_LOGGING_METHOD_ID;

        static jmethodID INIT_WORKER_METHOD_ID;

        static NumericCasts castFunctions;

        static ArrayElementAccessor arrayElementAccessor;

        static FieldAccessor fieldAccessor;

        struct JavaObjectIdScope {
            JavaObjectIdScope(JEnv &env, jfieldID fieldId, jobject runtime, int javaObjectId)
                    : _env(env), _fieldID(fieldId), _runtime(runtime) {
                _env.SetIntField(_runtime, _fieldID, javaObjectId);
            }

            ~JavaObjectIdScope() {
                _env.SetIntField(_runtime, _fieldID, -1);
            }

        private:
            JEnv _env;
            jfieldID _fieldID;
            jobject _runtime;
        };

        static void RemoveKey(const uint32_t key);

        static _Atomic uint32_t count_;

        struct CacheEntry {
            CacheEntry(v8::Isolate* isolate, v8::Persistent<v8::Function>* callback)
                    : isolate_(isolate),
                      callback_(callback) {
            }

            v8::Isolate* isolate_;
            v8::Persistent<v8::Function>* callback_;

        };

        static robin_hood::unordered_map<uint32_t, CacheEntry> cache_;

        static _Atomic uint64_t frameCallbackCount_;


        struct FrameCallbackCacheEntry {
            FrameCallbackCacheEntry(v8::Isolate *isolate, v8::Local<v8::Context> context ,v8::Local<v8::Function> callback)
                    : isolate_(isolate),
                      callback_(isolate,callback),
                      context_(isolate,context){
            }

            v8::Isolate *isolate_;
            v8::Global<v8::Function> callback_;
            v8::Global<v8::Context> context_;
            bool removed;
            uint64_t id;

            AChoreographer_frameCallback frameCallback_ = [](long ts, void *data) {
                execute((double)ts, data);
            };

            AChoreographer_frameCallback64 frameCallback64_ = [](int64_t ts, void *data) {
                execute((double)ts, data);
            };

            static void execute(double ts, void *data) {
                if (data != nullptr) {
                    auto entry = static_cast<FrameCallbackCacheEntry *>(data);


                    if (entry == nullptr || entry->removed) {
                        return;
                    }
                    v8::Isolate *isolate = entry->isolate_;

                    if (isolate == nullptr) {
                        return;
                    }
                    v8::Locker locker(isolate);
                    v8::Isolate::Scope isolate_scope(isolate);
                    v8::HandleScope handle_scope(isolate);
                    auto context = entry->context_.Get(isolate);
                    v8::Context::Scope context_scope(context);

                    auto cb = entry->callback_.Get(isolate);
                    v8::Local<v8::Value> result;

                    v8::Local<v8::Value> args[1] = {v8::Number::New(isolate, ts)};

                    if (!cb.IsEmpty() &&
                        !cb->Call(context, context->Global(), 1, args).ToLocal(&result)) {
                        assert(false);
                    }

                }
            }

        };

        static robin_hood::unordered_map<uint64_t, FrameCallbackCacheEntry*> frameCallbackCache_;

        static void InitChoreographer();

        static void PostCallback(const v8::FunctionCallbackInfo<v8::Value> &args,
                          FrameCallbackCacheEntry *entry, v8::Local<v8::Context> context);
    };
}

#endif /* CALLBACKHANDLERS_H_ */
