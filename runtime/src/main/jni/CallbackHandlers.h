#ifndef CALLBACKHANDLERS_H_
#define CALLBACKHANDLERS_H_

#include <string>
#include <map>
#include <vector>
#include "v8-debug.h"
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

namespace tns {
class CallbackHandlers {
    public:

        /*
         * Stores persistent handles of all 'Worker' objects initialized on the main thread
         * Note: No isolates different than that of the main thread should access this map
         */
        static std::map<int, v8::Persistent<v8::Object>*> id2WorkerMap;

        static std::map<v8::Isolate*, v8::Persistent<v8::Function>*> isolateToJsonStringify;

        static int nextWorkerId;

        static void Init(v8::Isolate* isolate);

        static v8::Local<v8::Object> CreateJSWrapper(v8::Isolate* isolate, jint javaObjectID, const std::string& typeName);

        static bool RegisterInstance(v8::Isolate* isolate, const v8::Local<v8::Object>& jsObject, const std::string& fullClassName, const ArgsWrapper& argWrapper, const v8::Local<v8::Object>& implementationObject, bool isInterface);

        static jclass ResolveClass(v8::Isolate* isolate, const std::string& fullClassname, const v8::Local<v8::Object>& implementationObject, bool isInterface);

        static std::string ResolveClassName(v8::Isolate* isolate, jclass& clazz);

        static v8::Local<v8::Value> GetArrayElement(v8::Isolate* isolate, const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature);

        static void SetArrayElement(v8::Isolate* isolate, const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature, v8::Local<v8::Value>& value);

        static int GetArrayLength(v8::Isolate* isolate, const v8::Local<v8::Object>& arr);

        static void CallJavaMethod(const v8::Local<v8::Object>& caller, const std::string& className, const std::string& methodName, MetadataEntry* entry, bool isStatic, bool isSuper, const v8::FunctionCallbackInfo<v8::Value>& args);

        static v8::Local<v8::Value> CallJSMethod(v8::Isolate* isolate, JNIEnv* _env, const v8::Local<v8::Object>& jsObject, const std::string& methodName, jobjectArray args);

        static v8::Local<v8::Value> GetJavaField(v8::Isolate* isolate, const v8::Local<v8::Object>& caller, FieldCallbackData* fieldData);

        static void SetJavaField(v8::Isolate* isolate, const v8::Local<v8::Object>& target, const v8::Local<v8::Value>& value, FieldCallbackData* fieldData);

        static void LogMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void TimeCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void DumpReferenceTablesMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void DumpReferenceTablesMethod();

        static void ExitMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void CreateGlobalCastFunctions(v8::Isolate* isolate, const v8::Local<v8::ObjectTemplate>& globalTemplate);

        static std::vector<std::string> GetTypeMetadata(const std::string& name, int index);

        /*
         * Gets all methods in the implementation object, and packs them in a jobjectArray
         * to pass them to Java Land, so that their corresponding Java callbacks are written when
         * the dexFactory generates the class
         */
        static jobjectArray GetMethodOverrides(JEnv& env, const v8::Local<v8::Object>& implementationObject);

        /*
         * Gets all interfaces declared in the 'interfaces' array inside the implementation object,
         * and packs them in a jobjectArray to pass them to Java Land, so that they may be
         * implemented when the dexFactory generates the corresponding class
         */
        static jobjectArray GetImplementedInterfaces(JEnv& env, const v8::Local<v8::Object>& implementationObject);

        static void EnableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void DisableVerboseLoggingMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static v8::Local<v8::Object> FindClass(v8::Isolate* isolate, const std::string& className);

        static void NewThreadCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * main -> worker messaging
         * Fired when a Worker instance's postMessage is called
         */
        static void WorkerObjectPostMessageCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * main -> worker messaging
         * Fired when worker object has "postMessage" and the worker has implemented "onMessage" handler
         * In case "onMessage" handler isn't implemented no exception is thrown
         */
        static void WorkerGlobalOnMessageCallback(v8::Isolate* isolate, jstring message);

        /*
         * worker -> main thread messaging
         * Fired when a Worker script's "postMessage" is called
         */
        static void WorkerGlobalPostMessageCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * worker -> main messaging
         * Fired when worker has sent a message to main and the worker object has implemented "onMessage" handler
         * In case "onMessage" handler isn't implemented no exception is thrown
         */
        static void WorkerObjectOnMessageCallback(v8::Isolate* isolate, jint workerId, jstring message);

        /*
         * Fired when a Worker instance's terminate is called (immediately stops execution of the thread)
         */
        static void WorkerObjectTerminateCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * Fired when a Worker script's close is called
         */
        static void WorkerGlobalCloseCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * Clears the persistent Worker object handle associated with a workerId
         * Occurs when calling a worker object's `terminate` or a worker thread's global scope `close`
         */
        static void ClearWorkerPersistent(int workerId);

        /*
         * Terminates the currently executing Isolate. No scripts can be executed after this call
         */
        static void TerminateWorkerThread(v8::Isolate* isolate);

        /*
         * Is called when an unhandled exception is thrown inside the worker
         * Will execute 'onerror' if one is provided inside the Worker Scope
         * Will make the exception "bubble up" through to main, to be handled by the Worker Object
         * if 'onerror' isn't implemented or returns false
         */
        static void CallWorkerScopeOnErrorHandle(v8::Isolate* isolate, v8::TryCatch& tc);

        /*
         * Is called when an unhandled exception bubbles up from the worker scope to the main thread Worker Object
         * Will execute `onerror` if one is implemented for the Worker Object instance
         * Will throw a NativeScript Exception if 'onerror' isn't implemented or returns false
         */
        static void CallWorkerObjectOnErrorHandle(v8::Isolate* isolate, jint workerId, jstring message, jstring stackTrace, jstring filename, jint lineno, jstring threadName);

    private:
        CallbackHandlers() {
        }

        static void AdjustAmountOfExternalAllocatedMemory(JEnv& env, v8::Isolate* isolate);

        /*
         * Helper method that creates a java string array for sending strings over JNI
         */
        static jobjectArray GetJavaStringArray(JEnv& env, int length);

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
                JavaObjectIdScope(JEnv& env, jfieldID fieldId, jobject runtime, int javaObjectId)
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
};
}

#endif /* CALLBACKHANDLERS_H_ */
