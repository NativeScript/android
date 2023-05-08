#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "v8.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "ArgsWrapper.h"
#include "LRUCache.h"
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <string>

namespace tns {
class ObjectManager {
    public:
        ObjectManager(jobject javaRuntimeObject);

        void Init(v8::Isolate* isolate);

        JniLocalRef GetJavaObjectByJsObject(const v8::Local<v8::Object>& object);

        jint GetOrCreateObjectId(jobject object);

        v8::Local<v8::Object> GetJsObjectByJavaObject(jint javaObjectID);

        v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID, const std::string& typeName);

        v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID, const std::string& typeName, jobject instance);

        void Link(const v8::Local<v8::Object>& object, jint javaObjectID);

        void ReleaseNativeCounterpart(v8::Local<v8::Object>& object);

        bool CloneLink(const v8::Local<v8::Object>& src, const v8::Local<v8::Object>& dest);

        bool IsJsRuntimeObject(const v8::Local<v8::Object>& object);

        std::string GetClassName(jobject javaObject);

        std::string GetClassName(jclass clazz);

        jint GenerateNewObjectID();

        void SetInstanceIsolate(v8::Isolate* isolate);

        v8::Local<v8::Object> GetEmptyObject(v8::Isolate* isolate);

        enum class MetadataNodeKeys {
            JsInfo,
            CallSuper,
            END
        };

        /**
         * Memory management modes. Keep the members in sync with the java/com/tns/MarkingMode.
         */
        enum JavaScriptMarkingMode {
            /**
             * For JavaScript instances with implementation objects that were marked for collection,
             * MarkReachableObjects will scan the whole graph of reachable objects and keep strong reference to
             * the Java instances of implementation objects.
             */
            Full,
            /**
             * Fully suppress the MarkReachableObjects.
             */
            None
        };

        JavaScriptMarkingMode GetMarkingMode();

    private:

        struct JSInstanceInfo {
            public:
                JSInstanceInfo(bool isJavaObjectWeak, jint javaObjectID)
                    : IsJavaObjectWeak(isJavaObjectWeak), JavaObjectID(javaObjectID) {
                }

                bool IsJavaObjectWeak;
                jint JavaObjectID;
        };

        struct ObjectWeakCallbackState {
            ObjectWeakCallbackState(ObjectManager* _thisPtr, v8::Persistent<v8::Object>* _target)
                : thisPtr(_thisPtr), target(_target) {
            }

            ObjectManager* thisPtr;
            v8::Persistent<v8::Object>* target;
        };

        struct GarbageCollectionInfo {
            std::vector<v8::Persistent<v8::Object>*> markedForGC;
        };

        JSInstanceInfo* GetJSInstanceInfo(const v8::Local<v8::Object>& object);

        JSInstanceInfo* GetJSInstanceInfoFromRuntimeObject(const v8::Local<v8::Object>& object);

        v8::Local<v8::Object> CreateJSWrapperHelper(jint javaObjectID, const std::string& typeName, jclass clazz);

        static void JSObjectWeakCallbackStatic(const v8::WeakCallbackInfo<ObjectWeakCallbackState>& data);

        static void JSObjectFinalizerStatic(const v8::WeakCallbackInfo<ObjectWeakCallbackState>& data);

        void JSObjectWeakCallback(v8::Isolate* isolate, ObjectWeakCallbackState* callbackState);

        void JSObjectFinalizer(v8::Isolate* isolate, ObjectWeakCallbackState* callbackState);

        bool HasImplObject(v8::Isolate* isolate, const v8::Local<v8::Object>& obj);

        jweak GetJavaObjectByID(jint javaObjectID);

        jobject GetJavaObjectByIDImpl(jint javaObjectID);

        static jweak NewWeakGlobalRefCallback(const int& javaObjectID, void* state);

        static void DeleteWeakGlobalRefCallback(const jweak& object, void* state);

        static void JSWrapperConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

        jobject m_javaRuntimeObject;

        v8::Isolate* m_isolate;

        std::stack<GarbageCollectionInfo> m_markedForGC;

        std::unordered_map<jint, v8::Persistent<v8::Object>*> m_idToObject;

        LRUCache<jint, jweak> m_cache;

        std::set<v8::Persistent<v8::Object>*> m_visitedPOs;
        std::unordered_map<jint, v8::Persistent<v8::Object>*> m_implObjStrong;

        volatile jint m_currentObjectId;

        bool m_useGlobalRefs;

        JavaScriptMarkingMode m_markingMode;

        jmethodID GET_NAME_METHOD_ID;

        jmethodID GET_JAVAOBJECT_BY_ID_METHOD_ID;

        jmethodID GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID;

        jmethodID MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID;

        jmethodID RELEASE_NATIVE_INSTANCE_METHOD_ID;

        v8::Persistent<v8::Function>* m_poJsWrapperFunc;
};
}

#endif /* OBJECTMANAGER_H_ */
