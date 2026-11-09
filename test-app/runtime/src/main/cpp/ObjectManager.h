#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include <map>
#include <memory>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "ArgsWrapper.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "LRUCache.h"
#include "NativeScriptAssert.h"
#include "v8.h"

namespace tns {
struct JavaCallArgs;

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

  // The id object is registered under, or -1.
  int FindObjectId(jobject object);

  void RegisterStrong(jobject object, int javaObjectID);

  v8::Local<v8::Object> GetJsObjectByJavaObject(int javaObjectID);

  // Settles the wrappers collected since the last flush. Called on every JS<->Java
  // crossing: JS cannot create or receive a Java object without one, so the Java objects
  // still held strongly for collected wrappers cannot grow between a GC and the flush.
  void FlushFinalizedBatchIfPending() {
    if (!m_flushBackingOff &&
        (!m_pendingWeaken.empty() || !m_pendingCheck.empty())) {
      FlushFinalizedBatch();
    }
  }

  // Whether a wrapper was ever created for this id and not yet collected.
  bool HasJsObject(int javaObjectID) const {
    return m_idToObject.find(javaObjectID) != m_idToObject.end();
  }

  // After a failed Java->JS call: drops the Java registration of every object Java
  // registered for the call (JavaCallArgs::FreshArg) that never got a wrapper.
  void ReleaseUnwrappedArgs(const JavaCallArgs& args);

  // As GetJsObjectByJavaObject, but empty once the wrapper's link to the Java object was
  // released.
  v8::Local<v8::Object> GetLinkedJsObjectByJavaObject(int javaObjectID);

  v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID,
                                        const std::string& typeName);

  // The wrapper of object, registering it and creating the wrapper if needed.
  v8::Local<v8::Object> GetOrCreateJSWrapper(jobject object);

  // classIndex is Runtime.classIndexOf(instance's class), or 0 when unknown.
  v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID, int classIndex,
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
    // The Java side holds the object weakly; a finalization now only asks
    // whether it is still alive.
    bool javaWeakened = false;
    // Queued in a finalizer batch that has not been flushed yet.
    bool queued = false;
    // Handed back to JS while queued: the wrapper is reachable again, so the flush must
    // leave the Java side alone and let a later collection queue it anew.
    bool reacquired = false;
  };

  JSInstanceInfo* GetJSInstanceInfo(const v8::Local<v8::Object>& object);

  JSInstanceInfo* GetJSInstanceInfoFromRuntimeObject(
      const v8::Local<v8::Object>& object);

  // clazz must be a global ref, or null to look className up.
  v8::Local<v8::Object> CreateJSWrapperHelper(jint javaObjectID,
                                              const std::string& className,
                                              jclass clazz);

  struct WrapperClass {
    std::string name;
    jclass clazz;  // global ref
  };

  // The JNI name and a global ref of instance's class, cached by class index.
  const std::string& WrapperClassFor(int classIndex, jobject instance,
                                     jclass& clazz, std::string& scratch);

  // Indexed by class index. Entries are never replaced, so references to them
  // survive a nested wrap growing the vector.
  std::vector<std::unique_ptr<WrapperClass>> m_wrapperClasses;

  static void JSObjectFinalizerStatic(
      const v8::WeakCallbackInfo<ObjectWeakCallbackState>& data);

  void JSObjectFinalizer(v8::Isolate* isolate,
                         ObjectWeakCallbackState* callbackState);

  // The Java object table may only be touched by the thread holding the
  // isolate lock; every upcall into it checks that in debug builds.
  void AssertIsolateLocked() const {
    NS_DCHECK(m_isolate == nullptr || v8::Locker::IsLocked(m_isolate));
  }

  void FlushFinalizedBatch();

  // Whether a flush task is pending afterwards; false on a thread with no event loop.
  bool ScheduleFinalizedBatchFlush(double delayMs = 0);

  void ReleaseCollectedWrapper(int javaObjectID);

  // Ids whose JS wrapper was collected, waiting for one JNI call that weakens
  // (first collection) or liveness-checks (later ones) their Java objects.
  std::vector<jint> m_pendingWeaken;
  std::vector<jint> m_pendingCheck;
  bool m_flushScheduled = false;
  // A flush failed; crossings leave the retry to the delayed task.
  bool m_flushBackingOff = false;

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

  static jmethodID GET_OR_CREATE_JAVA_OBJECT_ID_AND_CLASS_METHOD_ID;

  static jmethodID FIND_JAVA_OBJECT_ID_METHOD_ID;

  static jmethodID MAKE_INSTANCE_STRONG_METHOD_ID;

  static jmethodID PROCESS_FINALIZED_BATCH_METHOD_ID;

  static jmethodID RELEASE_NATIVE_INSTANCE_METHOD_ID;

  v8::Persistent<v8::Function>* m_poJsWrapperFunc;
  v8::Persistent<v8::ObjectTemplate> m_jsWrapperTemplate;
};
}  // namespace tns

#endif /* OBJECTMANAGER_H_ */
