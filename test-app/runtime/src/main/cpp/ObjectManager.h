#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "v8.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "ArgsWrapper.h"
#include "DirectBuffer.h"
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

        void UpdateCache(int objectID, jobject obj);

        jclass GetJavaClass(const v8::Local<v8::Object>& instance);

        void SetJavaClass(const v8::Local<v8::Object>& instance, jclass clazz);
        int GetOrCreateObjectId(jobject object);

        v8::Local<v8::Object> GetJsObjectByJavaObject(int javaObjectID);

        v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID, const std::string& typeName);

        v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID, const std::string& typeName, jobject instance);

        void Link(const v8::Local<v8::Object>& object, uint32_t javaObjectID, jclass clazz);

        bool CloneLink(const v8::Local<v8::Object>& src, const v8::Local<v8::Object>& dest);

        std::string GetClassName(jobject javaObject);

        std::string GetClassName(jclass clazz);

        int GenerateNewObjectID();

        void SetInstanceIsolate(v8::Isolate* isolate);

        v8::Local<v8::Object> GetEmptyObject(v8::Isolate* isolate);

        static void MarkReachableArrayElements(v8::Local<v8::Object>& o, std::stack<v8::Local<v8::Value>>& s);

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

    private:

        struct JSInstanceInfo {
            public:
                JSInstanceInfo(bool isJavaObjectWeak, uint32_t javaObjectID, jclass claz)
                    :IsJavaObjectWeak(isJavaObjectWeak), JavaObjectID(javaObjectID), ObjectClazz(claz) {
                }

                bool IsJavaObjectWeak;
                uint32_t JavaObjectID;
                jclass ObjectClazz;
        };

        struct ObjectWeakCallbackState {
            ObjectWeakCallbackState(ObjectManager* _thisPtr, JSInstanceInfo* _jsInfo, v8::Persistent<v8::Object>* _target)
                :
                thisPtr(_thisPtr), jsInfo(_jsInfo), target(_target) {
            }

            ObjectManager* thisPtr;
            JSInstanceInfo* jsInfo;
            v8::Persistent<v8::Object>* target;
        };

        struct GarbageCollectionInfo {
            GarbageCollectionInfo(int _numberOfGC)
                :
                numberOfGC(_numberOfGC) {
            }
            std::vector<v8::Persistent<v8::Object>*> markedForGC;
            int numberOfGC;
        };

        class PersistentObjectIdSet {
            public:
                PersistentObjectIdSet() {
                    /* TODO: use functors */
                }

                void clear() {
                    m_POs.clear();
                    m_IDs.clear();
                }

                void insert(v8::Persistent<v8::Object>* po, int javaObjectId) {
                    m_POs.insert(po);
                    m_IDs.insert(javaObjectId);
                }

                bool contains(v8::Persistent<v8::Object>* po) {
                    return m_POs.find(po) != m_POs.end();
                }

                std::set<v8::Persistent<v8::Object>*> m_POs;
                std::set<int> m_IDs;
        };

        struct PersistentObjectIdPair {
            PersistentObjectIdPair(v8::Persistent<v8::Object>* _po, int _javaObjectId)
                :
                po(_po), javaObjectId(_javaObjectId) {
            }
            v8::Persistent<v8::Object>* po;
            int javaObjectId;
        };

        bool IsJsRuntimeObject(const v8::Local<v8::Object>& object);

        JSInstanceInfo* GetJSInstanceInfo(const v8::Local<v8::Object>& object);

        JSInstanceInfo* GetJSInstanceInfoFromRuntimeObject(const v8::Local<v8::Object>& object);

        void ReleaseJSInstance(v8::Persistent<v8::Object>* po, JSInstanceInfo* jsInstanceInfo);

        void ReleaseRegularObjects();

        void MakeRegularObjectsWeak(const std::set<int>& instances, DirectBuffer& inputBuff);

        void MakeImplObjectsWeak(const std::map<int, v8::Persistent<v8::Object>*>& instances, DirectBuffer& inputBuff);

        void CheckWeakObjectsAreAlive(const std::vector<PersistentObjectIdPair>& instances, DirectBuffer& inputBuff, DirectBuffer& outputBuff);

        v8::Local<v8::Object> CreateJSWrapperHelper(jint javaObjectID, const std::string& typeName, jclass clazz);

        static void JSObjectWeakCallbackStatic(const v8::WeakCallbackInfo<ObjectWeakCallbackState>& data);

        static void JSObjectFinalizerStatic(const v8::WeakCallbackInfo<ObjectWeakCallbackState>& data);

        void JSObjectWeakCallback(v8::Isolate* isolate, ObjectWeakCallbackState* callbackState);

        void JSObjectFinalizer(v8::Isolate* isolate, ObjectWeakCallbackState* callbackState);

        bool HasImplObject(v8::Isolate* isolate, const v8::Local<v8::Object>& obj);

        void MarkReachableObjects(v8::Isolate* isolate, const v8::Local<v8::Object>& obj);

        void OnGcStarted(v8::GCType type, v8::GCCallbackFlags flags);

        void OnGcFinished(v8::GCType type, v8::GCCallbackFlags flags);

        static void OnGcStartedStatic(v8::Isolate* isolate, v8::GCType type, v8::GCCallbackFlags flags);

        static void OnGcFinishedStatic(v8::Isolate* isolate, v8::GCType type, v8::GCCallbackFlags flags);

        jweak GetJavaObjectByID(uint32_t javaObjectID);

        jobject GetJavaObjectByIDImpl(uint32_t javaObjectID);

        static jweak NewWeakGlobalRefCallback(const int& javaObjectID, void* state);

        static void DeleteWeakGlobalRefCallback(const jweak& object, void* state);

        static void JSWrapperConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

        jobject m_javaRuntimeObject;

        int m_numberOfGC;

        JEnv m_env;

        v8::Isolate* m_isolate;

        std::stack<GarbageCollectionInfo> m_markedForGC;

        std::map<int, v8::Persistent<v8::Object>*> m_idToObject;

        PersistentObjectIdSet m_released;

        std::set<unsigned long> m_visited;

        LRUCache<int, jweak> m_cache;

        std::set<v8::Persistent<v8::Object>*> m_visitedPOs;
        std::vector<PersistentObjectIdPair> m_implObjWeak;
        std::map<int, v8::Persistent<v8::Object>*> m_implObjStrong;

        volatile int m_currentObjectId;

        DirectBuffer m_buff;

        DirectBuffer m_outBuff;

        bool m_useGlobalRefs;

        JavaScriptMarkingMode m_markingMode;

        jclass JAVA_LANG_CLASS;

        jmethodID GET_NAME_METHOD_ID;

        jmethodID GET_JAVAOBJECT_BY_ID_METHOD_ID;

        jmethodID GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID;

        jmethodID MAKE_INSTANCE_WEAK_BATCH_METHOD_ID;

        jmethodID MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID;

        jmethodID CHECK_WEAK_OBJECTS_ARE_ALIVE_METHOD_ID;

        v8::Persistent<v8::Function>* m_poJsWrapperFunc;
};
}

#endif /* OBJECTMANAGER_H_ */
