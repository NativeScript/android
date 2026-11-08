#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "ArgsWrapper.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "LRUCache.h"
#include "v8.h"

namespace tns {
class ObjectManager {
 public:
  ObjectManager(jobject javaRuntimeObject);

  /*
   * Frees the JS side of every still-linked object. Must run on the runtime's
   * own thread while the isolate is alive -- it resets v8::Persistents and
   * clears internal fields. V8 does not run weak callbacks at isolate
   * disposal, so without this every linked wrapper's handle, JSInstanceInfo
   * and callback state is abandoned.
   */
  void ReleaseAllRegistered();

  /*
   * JNI-only teardown; runs from ~Runtime, after the isolate is disposed and
   * while the thread is still attached. Must not touch any v8 handle.
   */
  ~ObjectManager();

  void Init(v8::Isolate* isolate);

  JniLocalRef GetJavaObjectByJsObject(const v8::Local<v8::Object>& object);

  void UpdateCache(int objectID, jobject obj);

  jclass GetJavaClass(const v8::Local<v8::Object>& instance);

  void SetJavaClass(const v8::Local<v8::Object>& instance, jclass clazz);
  int GetOrCreateObjectId(jobject object);

  v8::Local<v8::Object> GetJsObjectByJavaObject(int javaObjectID);

  v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID,
                                        const std::string& typeName);

  v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID,
                                        const std::string& typeName,
                                        jobject instance);

  void Link(const v8::Local<v8::Object>& object, uint32_t javaObjectID,
            jclass clazz);

  void ReleaseNativeCounterpart(v8::Local<v8::Object>& object);

  bool CloneLink(const v8::Local<v8::Object>& src,
                 const v8::Local<v8::Object>& dest);

  /*
   * Whether GetInternalField is safe to call on this exact object. False for
   * a Proxy even when its target qualifies; unwrap first where a Proxy should
   * be accepted.
   */
  static bool IsJsRuntimeObject(const v8::Local<v8::Object>& object);

  /*
   * The runtime object holding the JsInfo External for `object`: the object
   * itself, or its prototype when the TypeScript layout keeps the registered
   * instance one level up. Empty when neither holds one. Reads and clears of
   * the link must go through this object, never through a Proxy or a child.
   */
  static v8::Local<v8::Object> GetJSInstanceInfoOwner(const v8::Local<v8::Object>& object);

  /*
   * The innermost target of a Proxy chain; any other value is returned as is.
   * A revoked Proxy anywhere in the chain yields null. Internal fields and
   * private symbols live on the target, never on the Proxy.
   */
  static v8::Local<v8::Value> UnwrapProxy(v8::Local<v8::Value> value);

  /*
   * Copies an array-like Proxy into a fresh Array by reading `length` and each
   * index through the Proxy, so its traps decide what Java receives. Empty when
   * a trap throws; the exception is left for the caller's TryCatch.
   */
  static v8::MaybeLocal<v8::Array> ReadArrayThroughProxy(
      v8::Local<v8::Context> context, v8::Local<v8::Object> proxy);

  static std::string GetClassName(jobject javaObject);

  static std::string GetClassName(jclass clazz);

  int GenerateNewObjectID();

  void SetInstanceIsolate(v8::Isolate* isolate);

  v8::Local<v8::Object> GetEmptyObject(v8::Isolate* isolate);

  enum class MetadataNodeKeys { JsInfo, CallSuper, END };

 private:
  static void InitializeJNI();
  struct JSInstanceInfo {
   public:
    JSInstanceInfo(uint32_t javaObjectID, jclass claz)
        : JavaObjectID(javaObjectID), ObjectClazz(claz) {}

    uint32_t JavaObjectID;
    jclass ObjectClazz;
  };

  struct ObjectWeakCallbackState {
    ObjectWeakCallbackState(ObjectManager* _thisPtr, JSInstanceInfo* _jsInfo,
                            v8::Persistent<v8::Object>* _target)
        : thisPtr(_thisPtr),
          jsInfo(_jsInfo),
          target(_target),
          javaObjectID(_jsInfo->JavaObjectID) {}

    ObjectManager* thisPtr;
    JSInstanceInfo* jsInfo;
    v8::Persistent<v8::Object>* target;
    // Duplicated from jsInfo: the finalizer has to unregister itself even when
    // the JsInfo field was already cleared and jsInfo freed.
    uint32_t javaObjectID;
  };

  JSInstanceInfo* GetJSInstanceInfo(const v8::Local<v8::Object>& object);

  JSInstanceInfo* GetJSInstanceInfoFromRuntimeObject(
      const v8::Local<v8::Object>& object);

  v8::Local<v8::Object> CreateJSWrapperHelper(jint javaObjectID,
                                              const std::string& typeName,
                                              jclass clazz);

  static void JSObjectFinalizerStatic(
      const v8::WeakCallbackInfo<ObjectWeakCallbackState>& data);

  void JSObjectFinalizer(v8::Isolate* isolate,
                         ObjectWeakCallbackState* callbackState);

  jweak GetJavaObjectByID(uint32_t javaObjectID);

  jobject GetJavaObjectByIDImpl(uint32_t javaObjectID);

  static jweak NewWeakGlobalRefCallback(const int& javaObjectID, void* state);

  static void DeleteWeakGlobalRefCallback(const jweak& object, void* state);

  static bool ValidateWeakGlobalRefCallback(const int& javaObjectID,
                                            const jweak& object, void* state);

  static void JSWrapperConstructorCallback(
      const v8::FunctionCallbackInfo<v8::Value>& info);

  jobject m_javaRuntimeObject;

  v8::Isolate* m_isolate;

  std::unordered_map<int, ObjectWeakCallbackState*> m_idToObject;

  LRUCache<int, jweak> m_cache;

  volatile int m_currentObjectId;

  bool m_useGlobalRefs;

  static jclass JAVA_LANG_CLASS;

  static jmethodID GET_NAME_METHOD_ID;

  static jmethodID GET_JAVAOBJECT_BY_ID_METHOD_ID;

  static jmethodID GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID;

  static jmethodID MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID;

  static jmethodID RELEASE_NATIVE_INSTANCE_METHOD_ID;

  v8::Persistent<v8::Function>* m_poJsWrapperFunc;
};
}  // namespace tns

#endif /* OBJECTMANAGER_H_ */
