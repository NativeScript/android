#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include <map>
#include <unordered_map>
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
#include "ObjectIdIndex.h"
#include "WrapperLifetime.h"
#include "v8.h"

namespace tns {
struct JavaCallArgs;
class MetadataNode;

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

  // As GetJavaObjectByJsObject, but a link whose Java object was already collected is
  // released and reported as no link, so the JS object can be linked again.
  JniLocalRef GetLiveJavaObjectByJsObject(const v8::Local<v8::Object>& object);

  void UpdateCache(int objectID, jobject obj);

  jclass GetJavaClass(const v8::Local<v8::Object>& instance);

  void SetJavaClass(const v8::Local<v8::Object>& instance, jclass clazz);
  int GetOrCreateObjectId(jobject object);

  // The id object is registered under, or -1.
  int FindObjectId(jobject object);

  void RegisterStrong(jobject object, int javaObjectID);

  v8::Local<v8::Object> GetJsObjectByJavaObject(int javaObjectID);

  // Settles the wrappers collected since the last flush. Called on JS<->Java calls and
  // constructions; field and array-element reads that hand JS a Java object skip it and
  // rely on the delayed flush task.
  void FlushFinalizedBatchIfPending() {
    if (!m_flushBackingOff &&
        (!m_pendingWeaken.empty() || !m_pendingCheck.empty() ||
         !m_pendingCanaryDeaths.empty())) {
      FlushFinalizedBatch();
    }
  }

  // enableMultithreadedJavascript: Java may call in from threads other than
  // the runtime's own.
  void SetMultithreadedJavascript(bool value) { m_multithreadedJavascript = value; }
  bool MultithreadedJavascript() const { return m_multithreadedJavascript; }

  // Canary settles that released Java ids. A Java object registered for a
  // call keeps its id while this does not move, on a single JS thread.
  uint64_t CanaryReleaseCount() const { return m_canaryReleaseCount; }

  // Whether a wrapper was ever created for this id and not yet collected.
  bool HasJsObject(int javaObjectID) const {
    return m_idToObject.Find(javaObjectID) != nullptr;
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

  // What a link's object is: Java calls into JS through an extended one (a
  // JS-extended class's or an interface implementation's instance).
  enum class LinkKind { kPlain, kExtended };

  void Link(const v8::Local<v8::Object>& object, uint32_t javaObjectID,
            jclass clazz, LinkKind kind = LinkKind::kPlain);

  void ReleaseNativeCounterpart(v8::Local<v8::Object>& object);

  bool CloneLink(const v8::Local<v8::Object>& src,
                 const v8::Local<v8::Object>& dest);

  /*
   * Whether GetInternalField is safe to call on this exact object. False for
   * a Proxy even when its target qualifies; unwrap first where a Proxy should
   * be accepted. Any object with this many internal fields is read as a runtime
   * object, so embedder code must never store an aligned pointer in field 0 of
   * an object with 2 or 3 internal fields.
   */
  static bool IsJsRuntimeObject(const v8::Local<v8::Object>& object);

  // Whether object is a runtime object with a Metadata field: not an array buffer.
  static bool HasMetadataField(const v8::Local<v8::Object>& object) {
    return object->InternalFieldCount() == static_cast<int>(MetadataNodeKeys::END);
  }

  /*
   * The runtime object holding the JsInfo External for `object`: the object
   * itself, or its prototype when the TypeScript layout keeps the registered
   * instance one level up. Empty when neither holds one. Reads and clears of
   * the link must go through this object, never through a Proxy or a child.
   */
  static v8::Local<v8::Object> GetJSInstanceInfoOwner(const v8::Local<v8::Object>& object);

  // Whether a runtime object's JsInfo field holds a link. The field holds id + 1, so
  // the zero V8 stores in a typed array's embedder fields never reads as a link.
  static bool HasJsInfo(const v8::Local<v8::Object>& object) {
    return LinkedIdOf(object) != -1;
  }

  // Whether object, seen through any Proxy and the TypeScript layout, is a wrapper whose
  // link was released or whose Java object was collected.
  static bool IsReleasedWrapper(const v8::Local<v8::Object>& object);

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

  enum class MetadataNodeKeys { JsInfo, CallSuper, Metadata, END };

  const WrapperLifetimeStats& GetLifetimeStats() const { return m_lifetimeStats; }

  // Canary mode (WrapperLifetimeCanary), for its write handlers. holder is a
  // live canary: an object this manager linked as one, not yet released.
  bool IsCanary(const v8::Local<v8::Object>& holder);
  // The state object of a canary that has one.
  bool ExistingCanaryState(const v8::Local<v8::Object>& holder, v8::Local<v8::Object>* out);
  // The state object of a canary, created on its first write.
  bool CanaryStateForWrite(const v8::Local<v8::Object>& holder, v8::Local<v8::Object>* out);
  void CountCanaryWrite() { m_lifetimeStats.canaryInterceptedWrites++; }

  // What the write handlers need of one live canary.
 private:
  struct ObjectWeakCallbackState;

 public:
  using CanaryHandle = ObjectWeakCallbackState;
  // holder's live canary registration, or null. holder must be an instance of
  // a template with the canary handlers (it has the Metadata field).
  CanaryHandle* CanaryOfInstance(const v8::Local<v8::Object>& holder);
  // The canary's state object, empty if it has none yet.
  v8::Local<v8::Object> StateOf(CanaryHandle* canary);
  // Whether every property of the canary's state is a writable, enumerable,
  // configurable data property: only a define can make one otherwise.
  bool StateIsPlain(CanaryHandle* canary);
  void SetStateNotPlain(CanaryHandle* canary);
  // The canary's state object, created on its first write.
  v8::Local<v8::Object> StateForWrite(CanaryHandle* canary);

  // Set while the runtime copies properties between a canary and its state, so
  // the handlers let those defines through. The copy runs no JS, and only the
  // thread holding the isolate's lock touches this.
  // Canary mode: the private key under which a class prototype keeps its
  // instance field accessors' data (MetadataNode::TrySetInstanceField); empty
  // in other modes.
  v8::Local<v8::Private> InstanceFieldTableKey() {
    return m_instanceFieldTableKey.Get(m_isolate);
  }

  bool CopyingCanaryProperties() const { return m_copyingCanaryProperties; }
  void SetCopyingCanaryProperties(bool copying) { m_copyingCanaryProperties = copying; }
  // Lets go of the state of objects Java reported collected. Under the
  // isolate's Locker: the runtime's thread, or a thread calling in under
  // enableMultithreadedJavascript.
  void ReleaseCollectedCanaryStates();

  struct ClassCounts {
    // Linked wrappers, canaries included.
    uint64_t wrappers = 0;
    // Live canaries.
    uint64_t canaries = 0;
    // Canary state objects, alive until Java collects their object.
    uint64_t states = 0;
  };

  // Per Java class name (dotted), for ns:runtime's getWrapperClassCounts().
  std::map<std::string, ClassCounts> GetWrapperClassCounts();

 private:
  static void InitializeJNI();

  struct CanaryRecord;

  // The registration of one linked Java object id, reached from the wrapper's JsInfo
  // field through m_idToObject.
  struct ObjectWeakCallbackState {
    ObjectWeakCallbackState(ObjectManager* _thisPtr, uint32_t _javaObjectID,
                            jclass clazz, v8::Isolate* isolate,
                            v8::Local<v8::Object> object)
        : thisPtr(_thisPtr),
          handle(isolate, object),
          target(&handle),
          javaObjectID(_javaObjectID),
          objectClazz(clazz) {}

    ObjectManager* thisPtr;
    // Persistent, not Global: its destructor never calls into V8, so a state may be
    // freed after its isolate is gone.
    v8::Persistent<v8::Object> handle;
    v8::Persistent<v8::Object>* target;
    uint32_t javaObjectID;
    jclass objectClazz;
    // Cleared by ReleaseNativeCounterpart; the registration itself lives until the
    // wrapper is collected.
    bool linked = true;
    // The Java side holds the object weakly; a finalization now only asks
    // whether it is still alive.
    bool javaWeakened = false;
    // Queued in a finalizer batch that has not been flushed yet.
    bool queued = false;
    // Handed back to JS while queued: the wrapper is reachable again, so the flush must
    // leave the Java side alone and let a later collection queue it anew.
    bool reacquired = false;
    // A canary: a phantom handle, and no resurrection.
    bool canary = false;
    // A canary's record, cached; EraseCanaryRecord clears it.
    CanaryRecord* record = nullptr;
  };

  // Canary mode: what outlives the canaries of one Java object id, until Java
  // collects the object. Its handles are reset only on the runtime's thread.
  struct CanaryRecord {
    // The object's JS properties; empty until the first write.
    v8::Global<v8::Object> state;
    // JS-extended instances: what a canary built later starts from, as the
    // first canary had it once registered.
    bool extended = false;
    v8::Global<v8::Value> prototype;
    v8::Global<v8::Value> callSuper;
    v8::Global<v8::Value> metadata;
    v8::Global<v8::Value> implementationObject;
    // The link's class, taken from the last canary (a global ref the class caches own).
    jclass clazz = nullptr;
    // The Java object table holds the object through a watched weak reference.
    bool javaWeak = false;
    // A define reached the state with an accessor or non-default attributes.
    bool stateNotPlain = false;
  };

  using JSInstanceInfo = ObjectWeakCallbackState;

  // The linked state of object, or null.
  JSInstanceInfo* GetJSInstanceInfo(const v8::Local<v8::Object>& object);

  // The linked Java object of jsInstanceInfo; throws if Java has collected it.
  JniLocalRef JavaObjectOf(JSInstanceInfo* jsInstanceInfo);

  WrapperLifetimeStats m_lifetimeStats;

  JSInstanceInfo* GetJSInstanceInfoFromRuntimeObject(
      const v8::Local<v8::Object>& object);

  // The id in a runtime object's JsInfo field (an unsigned 32-bit id carried in an int),
  // or -1.
  static int LinkedIdOf(const v8::Local<v8::Object>& object);

  void SetJsInfo(const v8::Local<v8::Object>& object, uint32_t javaObjectID);

  // clazz must be a global ref, or null to look className up.
  v8::Local<v8::Object> CreateJSWrapperHelper(jint javaObjectID,
                                              const std::string& className,
                                              jclass clazz,
                                              MetadataNode* node = nullptr);

  struct WrapperClass {
    std::string name;
    jclass clazz;  // global ref
    MetadataNode* node = nullptr;  // process-wide, never freed
  };

  // The JNI name and a global ref of instance's class, cached by class index.
  // node is set only for a cached class.
  const std::string& WrapperClassFor(int classIndex, jobject instance,
                                     jclass& clazz, std::string& scratch,
                                     MetadataNode*& node);

  // Indexed by class index. Entries are never replaced, so references to them
  // survive a nested wrap growing the vector.
  std::vector<std::unique_ptr<WrapperClass>> m_wrapperClasses;

  // state's wrapper, marking a queued one as handed back to JS.
  v8::Local<v8::Object> JsObjectFor(ObjectWeakCallbackState* state);

  static void JSObjectFinalizerStatic(
      const v8::WeakCallbackInfo<ObjectWeakCallbackState>& data);

  // A canary died: first-pass phantom callback, C++ bookkeeping only.
  static void CanaryDiedStatic(
      const v8::WeakCallbackInfo<ObjectWeakCallbackState>& data);

  static void CanaryGCEpilogue(v8::Isolate* isolate, v8::GCType type,
                               v8::GCCallbackFlags flags, void* data);

  // Registers a new canary: creates an extended instance's record, and
  // hydrates a canary for an object that already has state.
  void AdoptCanary(const v8::Local<v8::Object>& object, uint32_t javaObjectID,
                   LinkKind kind);

  // The live canary state of holder, or null.
  ObjectWeakCallbackState* CanaryOf(const v8::Local<v8::Object>& holder);

  CanaryRecord* FindCanaryRecord(int javaObjectID);

  // A new canary for a JS-extended instance whose canary died; empty when id
  // has no extended record or Java collected the object meanwhile.
  v8::Local<v8::Object> RebuildCanary(int javaObjectID);

  // Has the Java object table hold record's object strongly again; false (and
  // the record dropped) when Java collected it.
  bool EnsureJavaStrong(int javaObjectID, CanaryRecord* record);

  void EraseCanaryRecord(int javaObjectID);

  // Settles canaries collected since the last flush: Java holds the objects
  // with records weakly and releases the others. False if the JNI call failed;
  // the ids are queued again.
  bool FlushCanaryDeaths();

  std::unordered_map<int, std::unique_ptr<CanaryRecord>> m_canaryRecords;
  std::vector<jint> m_pendingCanaryDeaths;
  // Template of a canary rebuilt for a JS-extended instance.
  v8::Persistent<v8::ObjectTemplate> m_canaryTemplate;
  bool m_copyingCanaryProperties = false;
  bool m_deathInboxOpen = false;
  bool m_canaryEpilogueAdded = false;
  int m_deathInboxRuntimeId = -1;
  v8::Persistent<v8::Private> m_instanceFieldTableKey;

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

  // A link that is gone leaves null in JsInfo; a link never made leaves undefined.
  void MarkLinkReleased(const v8::Local<v8::Object>& object);

  // Drops id's registrations on both sides if Java has collected its object.
  bool ReleaseIfCollected(int javaObjectID);

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

  ObjectIdIndex<ObjectWeakCallbackState> m_idToObject;

  LRUCache<int, jweak> m_cache;

  volatile int m_currentObjectId;

  bool m_useGlobalRefs;

  bool m_multithreadedJavascript = false;

  uint64_t m_canaryReleaseCount = 0;

  static jclass JAVA_LANG_CLASS;

  static jmethodID GET_NAME_METHOD_ID;

  static jmethodID GET_JAVAOBJECT_BY_ID_METHOD_ID;

  static jmethodID GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID;

  static jmethodID GET_OR_CREATE_JAVA_OBJECT_ID_AND_CLASS_METHOD_ID;

  static jmethodID FIND_JAVA_OBJECT_ID_METHOD_ID;

  static jmethodID MAKE_INSTANCE_STRONG_METHOD_ID;

  static jmethodID PROCESS_FINALIZED_BATCH_METHOD_ID;

  static jmethodID RELEASE_NATIVE_INSTANCE_METHOD_ID;
  static jmethodID RELEASE_IF_COLLECTED_METHOD_ID;
  static jmethodID PROCESS_CANARY_BATCH_METHOD_ID;
  static jmethodID STRENGTHEN_INSTANCE_METHOD_ID;

  v8::Persistent<v8::Function>* m_poJsWrapperFunc;
  v8::Persistent<v8::ObjectTemplate> m_jsWrapperTemplate;
};
}  // namespace tns

#endif /* OBJECTMANAGER_H_ */
