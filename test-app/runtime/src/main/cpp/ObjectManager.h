#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include <map>
#include <optional>
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
struct FieldCallbackData;
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

  // Drops the jweak cached for objectID, after Java may have registered a different object
  // under it.
  void ForgetCachedJavaObject(int objectID) { m_cache.evictKey(objectID); }

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

  // Ids JS released explicitly (ReleaseNativeCounterpart), and canary settles that
  // released ids. While this does not move, an id Java registered for a call is still
  // registered.
  uint64_t ReleaseCount() const { return m_releaseCount; }

  // enableMultithreadedJavascript: Java may call in from threads other than
  // the runtime's own.
  void SetMultithreadedJavascript(bool value) { m_multithreadedJavascript = value; }
  bool MultithreadedJavascript() const { return m_multithreadedJavascript; }

  // Whether a wrapper was ever created for this id and not yet collected.
  bool HasJsObject(int javaObjectID) const {
    return m_idToObject.Find(javaObjectID) != nullptr;
  }

  // After a failed Java->JS call: drops the Java registration of every object Java
  // registered for the call (JavaCallArgs::FreshArg) that never got a wrapper.
  void ReleaseUnwrappedArgs(const JavaCallArgs& args);

  // As GetJsObjectByJavaObject, but empty once the wrapper's link to the Java object was
  // released. Ids are never reused, so an id read without the isolate lock that names a
  // dead canary's record still names that record's object.
  v8::Local<v8::Object> GetLinkedJsObjectByJavaObject(int javaObjectID);

  v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID,
                                        const std::string& typeName);

  // The wrapper of object, registering it and creating the wrapper if needed.
  v8::Local<v8::Object> GetOrCreateJSWrapper(jobject object);

  // GetJsObjectByJavaObject, or CreateJSWrapper when that is empty.
  v8::Local<v8::Object> GetOrCreateJSWrapper(jint javaObjectID, int classIndex,
                                             jobject instance);

  // classIndex is Runtime.classIndexOf(instance's class), or 0 when unknown.
  v8::Local<v8::Object> CreateJSWrapper(jint javaObjectID, int classIndex,
                                        jobject instance);

  // What a link's object is: Java calls into JS through an extended one (a
  // JS-extended class's or an interface implementation's instance).
  enum class LinkKind { kPlain, kExtended };

  // instanceTemplate is the template object was made from, alive as long as
  // the runtime's metadata cache. Canary mode needs it for an extended link:
  // a canary rebuilt later is an instance of it.
  void Link(const v8::Local<v8::Object>& object, uint32_t javaObjectID,
            jclass clazz, LinkKind kind = LinkKind::kPlain,
            const v8::Persistent<v8::ObjectTemplate>* instanceTemplate = nullptr);

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
  // live canary in scope: an object this manager linked as one, not yet
  // released.
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
  inline CanaryHandle* CanaryOfInstance(const v8::Local<v8::Object>& holder);
  // The canary's state object, empty if it has none yet.
  v8::Local<v8::Object> StateOf(CanaryHandle* canary);
  // Whether every property of the canary's state is a writable, enumerable,
  // configurable data property: only a define can make one otherwise.
  bool StateIsPlain(CanaryHandle* canary);
  void SetStateNotPlain(CanaryHandle* canary);
  // Before the handlers change the canary's state: a shell copied from an
  // earlier state can no longer stand in for it.
  inline void StateChanging(CanaryHandle* canary);
  // The canary's state object, created on its first write.
  v8::Local<v8::Object> StateForWrite(CanaryHandle* canary);
  // A new state for a canary that has none, holding `property` as a plain data
  // property.
  v8::Local<v8::Object> NewStateWith(CanaryHandle* canary, v8::Local<v8::Name> property,
                                     v8::Local<v8::Value> value);
  // The canary's state when its plain state is known to have `property` as an
  // own data property; empty when that is not known.
  inline v8::Local<v8::Object> CachedPlainState(CanaryHandle* canary,
                                                const v8::Local<v8::Name>& property);
  // Whether the canary's state may have `property`; false only for a plain
  // state that the handlers never added the name to as it is now.
  inline bool StateMayHave(CanaryHandle* canary, const v8::Local<v8::Name>& property);
  // After the handlers added `property` to the canary's state.
  inline void NoteStateName(CanaryHandle* canary, const v8::Local<v8::Name>& property);
  // After the handlers rewrote `property`, an own data property of the canary's
  // plain state. An entry holds a global handle, so a name added to the state
  // waits for its first rewrite.
  void CacheStateName(CanaryHandle* canary, const v8::Local<v8::Name>& property);
  // Before the handlers delete or redefine `property` on the canary's state.
  void ForgetStateName(CanaryHandle* canary, const v8::Local<v8::Name>& property);
  // The Java instance field a write of `property` on the canary `holder` runs
  // the setter of, when a cached earlier write through an object with holder's
  // prototype still applies; null otherwise.
  inline FieldCallbackData* CachedInstanceField(CanaryHandle* canary,
                                                const v8::Local<v8::Object>& holder,
                                                const v8::Local<v8::Name>& property);
  // After a write of `property` on the canary `holder` found `field`'s
  // accessor on `owner`, `depth` prototypes above holder's prototype.
  void CacheInstanceField(const v8::Local<v8::Object>& holder, const v8::Local<v8::Object>& owner,
                          int depth, const v8::Local<v8::Name>& property, FieldCallbackData* field);

  // Set while the runtime copies properties between a canary and its state, so
  // the handlers let those defines through. The copy runs no JS, and only the
  // thread holding the isolate's lock touches this.
  // Canary mode: the private key under which a class prototype keeps its
  // instance field accessors' data (MetadataNode::InstanceFieldOf); empty
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

  // Canary mode: adds `unwrap` (a canary -> its Java object's weak-collection
  // key) and `wrap` (key -> live or new canary, or undefined once Java
  // collected the object) to binding, for js/weak-collections.js.
  bool AddWeakCollectionFunctions(v8::Local<v8::Context> context,
                                  v8::Local<v8::Object> binding);

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
    // The Java side may hold the object weakly; a finalization now only asks
    // whether it is still alive. While false, Java holds the object strongly under
    // javaObjectID, so a jweak cached for that id still names it.
    bool javaWeakened = false;
    // Queued in a finalizer batch that has not been flushed yet.
    bool queued = false;
    // Handed back to JS while queued: the wrapper is reachable again, so the flush must
    // leave the Java side alone and let a later collection queue it anew.
    bool reacquired = false;
    // A canary: a phantom handle, and no resurrection.
    bool canary = false;
    // A canary in scope: the write handlers mirror its JS properties. Its
    // template can carry them while it is not, e.g. an interface proxy's.
    bool keepsState = false;
    // A canary's record, cached; EraseCanaryRecord clears it.
    CanaryRecord* record = nullptr;
  };

  // Canary mode: what a canary built later for a JS-extended instance starts
  // from, as the first canary had it once registered. The records of instances
  // that registered with the same values share one; the last of them frees it.
  struct CanaryClass {
    v8::Global<v8::Value> prototype;
    v8::Global<v8::Value> callSuper;
    v8::Global<v8::Value> metadata;
    v8::Global<v8::Value> implementationObject;
    const v8::Persistent<v8::ObjectTemplate>* instanceTemplate = nullptr;
    uint32_t records = 0;
    // The m_canaryClasses slot offering it to new records, or null.
    CanaryClass** slot = nullptr;
  };

  // Canary mode: what outlives the canaries of one Java object id, until Java
  // collects the object. Its handles are reset only on the runtime's thread.
  struct CanaryRecord {
    CanaryRecord() = default;
    CanaryRecord(const CanaryRecord&) = delete;
    CanaryRecord& operator=(const CanaryRecord&) = delete;
    ~CanaryRecord();
    int id = -1;
    // The object's JS properties; empty until the first write.
    v8::Global<v8::Object> state;
    // JS-extended instances only.
    CanaryClass* cls = nullptr;
    // A copy of the last rebuilt canary, taken once it was hydrated and before
    // JS saw it, unlinked; a later rebuild clones it instead of hydrating.
    // Present only while it matches the state.
    v8::Global<v8::Object> shell;
    // The link's class, taken from the last canary (a global ref the class caches own).
    jclass clazz = nullptr;
    // The Java object table holds the object through a watched weak reference.
    bool javaWeak = false;
    // A define reached the state with an accessor or non-default attributes.
    bool stateNotPlain = false;
    // Tags (FieldNameTag) of the names the handlers added to a plain state, so a
    // write of a name without one skips looking the state up. A name that moved
    // since has another tag; its write takes the path for a name the state lacks,
    // which also handles one the canary has. Past kStateTags, every write looks.
    static constexpr uint8_t kStateTags = 6;
    uint8_t stateTagCount = 0;
    uint32_t stateTags[kStateTags];
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

  // record is FindCanaryRecord(javaObjectID), or nullopt to look it up.
  // hydrate false: object already carries the state's properties.
  void Link(const v8::Local<v8::Object>& object, uint32_t javaObjectID, jclass clazz,
            LinkKind kind, const v8::Persistent<v8::ObjectTemplate>* instanceTemplate,
            std::optional<CanaryRecord*> record, bool hydrate = true);

  // Registers a new canary: creates an extended instance's record, and
  // hydrates a canary for an object that already has state. record is
  // FindCanaryRecord(javaObjectID); returns the canary's record, or null.
  CanaryRecord* AdoptCanary(const v8::Local<v8::Object>& object, uint32_t javaObjectID,
                            LinkKind kind,
                            const v8::Persistent<v8::ObjectTemplate>* instanceTemplate,
                            CanaryRecord* record, bool hydrate);

  // The live canary state of holder, or null.
  ObjectWeakCallbackState* CanaryOf(const v8::Local<v8::Object>& holder);

  CanaryRecord* FindCanaryRecord(int javaObjectID);

  // A new canary for a JS-extended instance whose canary died; empty when id
  // has no extended record or Java collected the object meanwhile.
  v8::Local<v8::Object> RebuildCanary(int javaObjectID);
  // record is FindCanaryRecord(javaObjectID).
  v8::Local<v8::Object> RebuildCanary(int javaObjectID, CanaryRecord* record);

  // Has the Java object table hold record's object strongly again; false (and
  // the record dropped) when Java collected it.
  bool EnsureJavaStrong(int javaObjectID, CanaryRecord* record);

  void EraseCanaryRecord(int javaObjectID);

  // Makes fresh the state of canary's record, creating the record if needed.
  v8::Local<v8::Object> AttachState(CanaryHandle* canary, const v8::Local<v8::Object>& fresh);

  // Names that are own data properties of a plain state, the record's set
  // chosen by its address: an entry is cleared before its name leaves the
  // state and before its record is freed.
  struct StateName {
    CanaryRecord* record = nullptr;
    v8::Global<v8::Name> name;
  };
  static constexpr size_t kStateNameSets = 64;
  static constexpr size_t kStateNameWays = 4;
  static size_t StateNameSet(const CanaryRecord* record) {
    auto address = reinterpret_cast<uintptr_t>(record);
    return ((address >> 4) ^ (address >> 10)) & (kStateNameSets - 1);
  }
  StateName m_stateNames[kStateNameSets][kStateNameWays];
  uint8_t m_stateNameVictims[kStateNameSets] = {};
  void ForgetStateNames(CanaryRecord* record);

  // Java instance fields the write handler ran the setter of, by the name
  // written and the canary's prototype. Field accessors are non-configurable,
  // so a hit rechecks only what lies below the accessor's owner: the canary's
  // state and the prototypes in between. Sets are picked by addresses, which a
  // moving collection may change; the entry then just misses.
  struct FieldWrite {
    v8::Global<v8::Name> name;
    // Weak, as is owner: the cache keeps no prototype alive.
    v8::Global<v8::Object> prototype;
    // Empty when the owner is the prototype itself.
    v8::Global<v8::Object> owner;
    FieldCallbackData* field = nullptr;
    int depth = 0;
  };
  // The low half of a name's tagged address: never 0, and only a filter, as a
  // name can move or its address be reused.
  static uint32_t FieldNameTag(const v8::Local<v8::Name>& name) {
    return static_cast<uint32_t>(v8::internal::ValueHelper::ValueAsAddress(*name));
  }
  static constexpr size_t kFieldNameSets = 32;
  static constexpr size_t kFieldNameWays = 4;
  static size_t FieldNameSet(uint32_t tag) {
    return ((tag >> 3) ^ (tag >> 9)) & (kFieldNameSets - 1);
  }
  // Tags of the names entries were made for, so a write of any other name
  // reads one line and makes no V8 call. One name has entries for many
  // prototypes (a field every LayoutParams subclass inherits).
  uint32_t m_fieldNameTags[kFieldNameSets][kFieldNameWays] = {};
  uint8_t m_fieldNameVictims[kFieldNameSets] = {};
  static constexpr size_t kFieldWriteSets = 64;
  static constexpr size_t kFieldWriteWays = 4;
  static size_t FieldWriteSet(uint32_t tag, const v8::Local<v8::Value>& prototype) {
    auto address = static_cast<uint32_t>(v8::internal::ValueHelper::ValueAsAddress(*prototype));
    uint32_t mixed = (address ^ tag) * 0x9E3779B1u;
    return (mixed >> 26) & (kFieldWriteSets - 1);
  }
  FieldWrite m_fieldWrites[kFieldWriteSets][kFieldWriteWays];
  uint8_t m_fieldWriteVictims[kFieldWriteSets] = {};
  FieldCallbackData* CheckedInstanceField(CanaryHandle* canary, const v8::Local<v8::Object>& holder,
                                          const v8::Local<v8::Name>& property, uint32_t tag);

  // Settles canaries collected since the last flush: Java holds the objects
  // with records weakly and releases the others. False if the JNI call failed;
  // the ids are queued again.
  bool FlushCanaryDeaths();

  // The last class registered per instance template, by template address.
  static constexpr size_t kCanaryClassSlots = 16;
  CanaryClass* m_canaryClasses[kCanaryClassSlots] = {};
  // A class for a new extended record with these values, shared when they match.
  CanaryClass* CanaryClassFor(const v8::Persistent<v8::ObjectTemplate>* instanceTemplate,
                              const v8::Local<v8::Value>& prototype,
                              const v8::Local<v8::Value>& callSuper,
                              const v8::Local<v8::Value>& metadata,
                              const v8::Local<v8::Value>& implementationObject);
  // Owns the records.
  ObjectIdIndex<CanaryRecord> m_canaryRecords;
  size_t m_canaryRecordCount = 0;
  CanaryRecord* NewCanaryRecord(int javaObjectID);
  std::vector<jint> m_pendingCanaryDeaths;
  bool m_copyingCanaryProperties = false;
  bool m_deathInboxOpen = false;
  bool m_canaryEpilogueAdded = false;
  int m_deathInboxRuntimeId = -1;
  v8::Persistent<v8::Private> m_instanceFieldTableKey;
  // Cloned for each new state; never handed to JS.
  v8::Global<v8::Object> m_emptyState;
  v8::Local<v8::Object> NewEmptyState();
  // The weak-collection key of a canary's Java object hangs off its state
  // object, and names the object's id. Once the canary's link is released,
  // the canary holds the key and the key leads back to the canary.
  v8::Persistent<v8::Private> m_keyOfState;
  v8::Persistent<v8::Private> m_objectOfKey;
  v8::Persistent<v8::Private> m_releasedOfKey;

  // Moves id's weak-collection key from its state to its live canary, whose
  // link is being released.
  void KeepWeakCollectionKey(int javaObjectID);

  static void UnwrapForWeakCollection(const v8::FunctionCallbackInfo<v8::Value>& info);
  static void WrapForWeakCollection(const v8::FunctionCallbackInfo<v8::Value>& info);

  // id's Java object, or null once Java collected it.
  jobject JavaObjectOrNull(int javaObjectID);

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
  uint64_t m_releaseCount = 0;

  // validate=false only while the Java side holds the object strongly: its jweak
  // cannot have been cleared then.
  jweak GetJavaObjectByID(uint32_t javaObjectID, bool validate);

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

  static jclass JAVA_LANG_CLASS;

  static jmethodID GET_NAME_METHOD_ID;

  static jmethodID GET_JAVAOBJECT_BY_ID_METHOD_ID;

  static jmethodID GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID;

  static jmethodID GET_OR_CREATE_JAVA_OBJECT_ID_AND_CLASS_METHOD_ID;

  static jmethodID GET_CLASS_INDEX_METHOD_ID;

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

inline ObjectManager::CanaryHandle* ObjectManager::CanaryOfInstance(
    const v8::Local<v8::Object>& holder) {
  // The JsInfo field read as Object::GetInternalField does, without the handle
  // and the current-isolate lookup it costs; an id past the Smi range takes
  // LinkedIdOf.
  using A = v8::internal::Address;
  using I = v8::internal::Internals;
  A object = v8::internal::ValueHelper::ValueAsAddress(*holder);
  int id = -1;
  if (V8_LIKELY(I::CanHaveInternalField(I::GetInstanceType(object)))) {
    A field = I::ReadRawField<A>(object, I::kJSAPIObjectWithEmbedderSlotsHeaderSize +
                                             I::kEmbedderDataSlotSize *
                                                 static_cast<int>(MetadataNodeKeys::JsInfo));
#ifdef V8_COMPRESS_POINTERS
    field = static_cast<uint32_t>(field);
#endif
    if (V8_LIKELY(!I::HasHeapObjectTag(field))) {
      id = I::SmiValue(field) - 1;
    }
  }
  if (V8_UNLIKELY(id == -1)) {
    id = LinkedIdOf(holder);
  }
  auto state = m_idToObject.Find(id);
  // A `super` object shares its owner's id but is not the canary.
  if (state == nullptr || !state->keepsState || !state->linked || *state->target != holder) {
    return nullptr;
  }
  return state;
}

inline void ObjectManager::StateChanging(CanaryHandle* canary) {
  if (canary->record != nullptr && !canary->record->shell.IsEmpty()) {
    canary->record->shell.Reset();
  }
}

inline v8::Local<v8::Object> ObjectManager::CachedPlainState(
    CanaryHandle* canary, const v8::Local<v8::Name>& property) {
  CanaryRecord* record = canary->record;
  if (record == nullptr || record->stateNotPlain) {
    return v8::Local<v8::Object>();
  }
  const StateName* set = m_stateNames[StateNameSet(record)];
  for (size_t way = 0; way < kStateNameWays; way++) {
    if (set[way].record == record && set[way].name == property) {
      m_lifetimeStats.canaryCachedWrites++;
      return record->state.Get(m_isolate);
    }
  }
  return v8::Local<v8::Object>();
}

inline bool ObjectManager::StateMayHave(CanaryHandle* canary,
                                        const v8::Local<v8::Name>& property) {
  CanaryRecord* record = canary->record;
  if (record == nullptr || record->stateNotPlain ||
      record->stateTagCount > CanaryRecord::kStateTags) {
    return true;
  }
  uint32_t tag = FieldNameTag(property);
  for (uint8_t i = 0; i < record->stateTagCount; i++) {
    if (record->stateTags[i] == tag) {
      return true;
    }
  }
  return false;
}

inline void ObjectManager::NoteStateName(CanaryHandle* canary,
                                         const v8::Local<v8::Name>& property) {
  CanaryRecord* record = canary->record;
  if (record == nullptr || record->stateTagCount > CanaryRecord::kStateTags) {
    return;
  }
  uint32_t tag = FieldNameTag(property);
  for (uint8_t i = 0; i < record->stateTagCount; i++) {
    if (record->stateTags[i] == tag) {
      return;
    }
  }
  if (record->stateTagCount == CanaryRecord::kStateTags) {
    record->stateTagCount++;
    return;
  }
  record->stateTags[record->stateTagCount++] = tag;
}

inline FieldCallbackData* ObjectManager::CachedInstanceField(CanaryHandle* canary,
                                                             const v8::Local<v8::Object>& holder,
                                                             const v8::Local<v8::Name>& property) {
  uint32_t tag = FieldNameTag(property);
  const uint32_t* tags = m_fieldNameTags[FieldNameSet(tag)];
  for (size_t way = 0; way < kFieldNameWays; way++) {
    if (tags[way] == tag) {
      return CheckedInstanceField(canary, holder, property, tag);
    }
  }
  return nullptr;
}
}  // namespace tns

#endif /* OBJECTMANAGER_H_ */
