#include "ObjectManager.h"

#include <algorithm>
#include <sstream>

#include "ArgConverter.h"
#include "JType.h"
#include "ManualInstrumentation.h"
#include "MetadataNode.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "WrapperLifetimeCanary.h"
#include "include/v8.h"

using namespace v8;
using namespace std;
using namespace tns;

ObjectManager::ObjectManager(jobject javaRuntimeObject)
    : m_javaRuntimeObject(javaRuntimeObject),
      m_isolate(nullptr),
      m_currentObjectId(0),
      m_cache(NewWeakGlobalRefCallback, DeleteWeakGlobalRefCallback,
              ValidateWeakGlobalRefCallback, 1000, this) {
  JEnv env;
  InitializeJNI();

  auto runtimeClass = env.FindClass("com/tns/Runtime");
  NS_CHECK(runtimeClass != nullptr);

  auto useGlobalRefsMethodID =
      env.GetStaticMethodID(runtimeClass, "useGlobalRefs", "()Z");
  NS_CHECK(useGlobalRefsMethodID != nullptr);

  auto useGlobalRefs =
      env.CallStaticBooleanMethod(runtimeClass, useGlobalRefsMethodID);
  m_useGlobalRefs = useGlobalRefs == JNI_TRUE;
}

void ObjectManager::InitializeJNI() {
  if (GET_JAVAOBJECT_BY_ID_METHOD_ID != nullptr) {
    return;
  }
  JEnv env;
  auto runtimeClass = env.FindClass("com/tns/Runtime");
  NS_CHECK(runtimeClass != nullptr);
  GET_JAVAOBJECT_BY_ID_METHOD_ID = env.GetMethodID(
      runtimeClass, "getJavaObjectByID", "(I)Ljava/lang/Object;");
  NS_CHECK(GET_JAVAOBJECT_BY_ID_METHOD_ID != nullptr);

  GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID = env.GetMethodID(
      runtimeClass, "getOrCreateJavaObjectID", "(Ljava/lang/Object;)I");
  NS_CHECK(GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID != nullptr);

  GET_OR_CREATE_JAVA_OBJECT_ID_AND_CLASS_METHOD_ID = env.GetMethodID(
      runtimeClass, "getOrCreateJavaObjectIDAndClass", "(Ljava/lang/Object;)J");
  NS_CHECK(GET_OR_CREATE_JAVA_OBJECT_ID_AND_CLASS_METHOD_ID != nullptr);

  FIND_JAVA_OBJECT_ID_METHOD_ID = env.GetMethodID(
      runtimeClass, "findJavaObjectID", "(Ljava/lang/Object;)I");
  NS_CHECK(FIND_JAVA_OBJECT_ID_METHOD_ID != nullptr);

  MAKE_INSTANCE_STRONG_METHOD_ID = env.GetMethodID(
      runtimeClass, "makeInstanceStrong", "(Ljava/lang/Object;I)V");
  NS_CHECK(MAKE_INSTANCE_STRONG_METHOD_ID != nullptr);

  PROCESS_FINALIZED_BATCH_METHOD_ID =
      env.GetMethodID(runtimeClass, "processFinalizedBatch", "([II[II)V");
  NS_CHECK(PROCESS_FINALIZED_BATCH_METHOD_ID != nullptr);

  RELEASE_NATIVE_INSTANCE_METHOD_ID =
      env.GetMethodID(runtimeClass, "releaseNativeCounterpart", "(I)V");
  NS_CHECK(RELEASE_NATIVE_INSTANCE_METHOD_ID != nullptr);

  RELEASE_IF_COLLECTED_METHOD_ID =
      env.GetMethodID(runtimeClass, "releaseIfCollected", "(I)Z");
  NS_CHECK(RELEASE_IF_COLLECTED_METHOD_ID != nullptr);

  PROCESS_CANARY_BATCH_METHOD_ID =
      env.GetMethodID(runtimeClass, "processCanaryBatch", "([II[II)V");
  NS_CHECK(PROCESS_CANARY_BATCH_METHOD_ID != nullptr);

  STRENGTHEN_INSTANCE_METHOD_ID =
      env.GetMethodID(runtimeClass, "strengthenInstance", "(I)Z");
  NS_CHECK(STRENGTHEN_INSTANCE_METHOD_ID != nullptr);

  JAVA_LANG_CLASS = env.FindClass("java/lang/Class");
  NS_CHECK(JAVA_LANG_CLASS != nullptr);

  GET_NAME_METHOD_ID =
      env.GetMethodID(JAVA_LANG_CLASS, "getName", "()Ljava/lang/String;");
  NS_CHECK(GET_NAME_METHOD_ID != nullptr);
}

void ObjectManager::SetInstanceIsolate(Isolate* isolate) {
  m_isolate = isolate;
}

void ObjectManager::Init(Isolate* isolate) {
  auto jsWrapperFuncTemplate =
      FunctionTemplate::New(isolate, JSWrapperConstructorCallback);
  jsWrapperFuncTemplate->InstanceTemplate()->SetInternalFieldCount(
      static_cast<int>(MetadataNodeKeys::END));
  auto context = isolate->GetCurrentContext();
  auto jsWrapperFunc =
      jsWrapperFuncTemplate->GetFunction(context).ToLocalChecked();
  m_poJsWrapperFunc = new Persistent<Function>(isolate, jsWrapperFunc);
  m_jsWrapperTemplate.Reset(isolate, jsWrapperFuncTemplate->InstanceTemplate());

  if (WrapperLifetimeCanary::Enabled()) {
    m_instanceFieldTableKey.Reset(isolate, Private::New(isolate));
    m_keyOfState.Reset(isolate, Private::New(isolate));
    m_objectOfKey.Reset(isolate, Private::New(isolate));
    m_releasedOfKey.Reset(isolate, Private::New(isolate));
    isolate->AddGCEpilogueCallback(CanaryGCEpilogue, this);
    m_canaryEpilogueAdded = true;
  }
}

JniLocalRef ObjectManager::GetJavaObjectByJsObject(
    const Local<Object>& object) {
  JSInstanceInfo* jsInstanceInfo = GetJSInstanceInfo(object);
  if (jsInstanceInfo == nullptr) {
    if (IsReleasedWrapper(object)) {
      m_lifetimeStats.releasedAccesses++;
    }
    return JniLocalRef();
  }
  try {
    return JavaObjectOf(jsInstanceInfo);
  } catch (const NativeScriptException&) {
    m_lifetimeStats.releasedAccesses++;
    throw;
  }
}

JniLocalRef ObjectManager::JavaObjectOf(JSInstanceInfo* jsInstanceInfo) {
  jweak existingObject;
  try {
    existingObject = GetJavaObjectByID(jsInstanceInfo->javaObjectID);
  } catch (const NativeScriptException& e) {
    // this captures usually the error
    auto className = GetClassName(jsInstanceInfo->objectClazz);
    throw NativeScriptException("Failed to get Java object by ID. id=" +
                                std::to_string(jsInstanceInfo->javaObjectID) +
                                ", class=" + className + ". " +
                                e.GetErrorMessage());
  }
  if (m_useGlobalRefs) {
    JniLocalRef javaObject(existingObject, true);
    return javaObject;
  }
  JEnv env;
  JniLocalRef javaObject(env.NewLocalRef(existingObject));
  return javaObject;
}

JniLocalRef ObjectManager::GetLiveJavaObjectByJsObject(
    const Local<Object>& object) {
  JSInstanceInfo* jsInstanceInfo = GetJSInstanceInfo(object);
  if (jsInstanceInfo == nullptr) {
    return JniLocalRef();
  }
  auto javaObjectID = jsInstanceInfo->javaObjectID;

  // Only a collected object may be relinked: releasing the link of a live one would
  // leave it pointing at memory nothing keeps alive any more.
  JniLocalRef javaObject;
  try {
    javaObject = JavaObjectOf(jsInstanceInfo);
  } catch (const NativeScriptException&) {
    if (!ReleaseIfCollected(javaObjectID)) {
      throw;
    }
    return JniLocalRef();
  }
  if (javaObject.IsNull() && !ReleaseIfCollected(javaObjectID)) {
    throw NativeScriptException("Failed to get Java object by ID. id=" +
                                std::to_string(javaObjectID));
  }
  return javaObject;
}

bool ObjectManager::ReleaseIfCollected(int javaObjectID) {
  JEnv env;
  AssertIsolateLocked();
  if (env.CallBooleanMethod(m_javaRuntimeObject, RELEASE_IF_COLLECTED_METHOD_ID,
                            javaObjectID) != JNI_TRUE) {
    return false;
  }
  ReleaseCollectedWrapper(javaObjectID);
  return true;
}

ObjectManager::JSInstanceInfo* ObjectManager::GetJSInstanceInfo(
    const Local<Object>& object) {
  if (object->IsProxy()) {
    auto target = UnwrapProxy(object);
    return target->IsObject() ? GetJSInstanceInfo(target.As<Object>())
                              : nullptr;
  }
  if (IsJsRuntimeObject(object)) {
    return GetJSInstanceInfoFromRuntimeObject(object);
  }
  return nullptr;
}

Local<Value> ObjectManager::UnwrapProxy(Local<Value> value) {
  while (value->IsProxy()) {
    value = value.As<Proxy>()->GetTarget();
  }
  return value;
}

MaybeLocal<Array> ObjectManager::ReadArrayThroughProxy(Local<Context> context,
                                                       Local<Object> proxy) {
  auto isolate = Isolate::GetCurrent();
  EscapableHandleScope scope(isolate);

  Local<Value> lengthValue;
  if (!proxy->Get(context, ArgConverter::ConvertToV8String(isolate, "length"))
           .ToLocal(&lengthValue)) {
    return {};
  }
  double length;
  if (!lengthValue->NumberValue(context).To(&length)) {
    return {};
  }
  if (!(length > 0)) {
    length = 0;
  }
  // Java arrays are indexed by jint.
  if (length > INT32_MAX) {
    isolate->ThrowException(Exception::RangeError(ArgConverter::ConvertToV8String(
        isolate, "Array length exceeds the maximum Java array size")));
    return {};
  }

  auto count = static_cast<uint32_t>(length);
  auto snapshot = Array::New(isolate, static_cast<int>(count));
  for (uint32_t i = 0; i < count; i++) {
    Local<Value> element;
    if (!proxy->Get(context, i).ToLocal(&element) ||
        snapshot->CreateDataProperty(context, i, element).IsNothing()) {
      return {};
    }
  }
  return scope.Escape(snapshot);
}

int ObjectManager::LinkedIdOf(const Local<Object>& object) {
  auto field = object->GetInternalField(static_cast<int>(MetadataNodeKeys::JsInfo)).As<Value>();
  if (!field->IsUint32()) {
    return -1;
  }
  return static_cast<int>(field.As<Uint32>()->Value() - 1u);
}

void ObjectManager::SetJsInfo(const Local<Object>& object, uint32_t javaObjectID) {
  object->SetInternalField(static_cast<int>(MetadataNodeKeys::JsInfo),
                           Integer::NewFromUnsigned(m_isolate, javaObjectID + 1));
}

Local<Object> ObjectManager::GetJSInstanceInfoOwner(const Local<Object>& object) {
  if (HasJsInfo(object)) {
    return object;
  }

  // Typescript object layout has an object instance as child of the actual
  // registered instance. checking for that
  auto prototypeObject = object->GetPrototype().As<Object>();
  if (!prototypeObject.IsEmpty() && prototypeObject->IsObject()) {
    DEBUG_WRITE("GetJSInstanceInfo: need to check prototype :%d",
                prototypeObject->GetIdentityHash());
    if (IsJsRuntimeObject(prototypeObject) && HasJsInfo(prototypeObject)) {
      return prototypeObject;
    }
  }

  return Local<Object>();
}

ObjectManager::JSInstanceInfo*
ObjectManager::GetJSInstanceInfoFromRuntimeObject(const Local<Object>& object) {
  int id = LinkedIdOf(object);
  if (id == -1) {
    HandleScope handleScope(m_isolate);
    auto owner = GetJSInstanceInfoOwner(object);
    if (owner.IsEmpty()) {
      return nullptr;
    }
    id = LinkedIdOf(owner);
  }

  auto state = m_idToObject.Find(id);
  return state != nullptr && state->linked ? state : nullptr;
}

// Array buffers and their views are linked to Java buffers through V8's own embedder
// fields, which must cover JsInfo and CallSuper; they have no Metadata field.
static_assert(V8_ARRAY_BUFFER_INTERNAL_FIELD_COUNT ==
              static_cast<int>(ObjectManager::MetadataNodeKeys::Metadata));
static_assert(V8_ARRAY_BUFFER_VIEW_INTERNAL_FIELD_COUNT ==
              static_cast<int>(ObjectManager::MetadataNodeKeys::Metadata));

bool ObjectManager::IsJsRuntimeObject(const v8::Local<v8::Object>& object) {
  int internalFieldCount = object->InternalFieldCount();
  return internalFieldCount == static_cast<int>(MetadataNodeKeys::END) ||
         internalFieldCount == V8_ARRAY_BUFFER_INTERNAL_FIELD_COUNT;
}

void ObjectManager::MarkLinkReleased(const Local<Object>& object) {
  object->SetInternalField(static_cast<int>(MetadataNodeKeys::JsInfo), Null(m_isolate));
}

bool ObjectManager::IsReleasedWrapper(const Local<Object>& object) {
  auto target = UnwrapProxy(object);
  if (!target->IsObject() || !HasMetadataField(target.As<Object>())) {
    return false;
  }
  auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
  auto field = target.As<Object>()->GetInternalField(jsInfoIdx).As<Value>();
  if (!field->IsUndefined()) {
    return field->IsNull();
  }
  auto prototype = target.As<Object>()->GetPrototype();
  return prototype->IsObject() && HasMetadataField(prototype.As<Object>()) &&
         prototype.As<Object>()->GetInternalField(jsInfoIdx).As<Value>()->IsNull();
}

jweak ObjectManager::GetJavaObjectByID(uint32_t javaObjectID) {
  jweak obj = m_cache(javaObjectID);

  return obj;
}

jobject ObjectManager::GetJavaObjectByIDImpl(uint32_t javaObjectID) {
  JEnv env;
  AssertIsolateLocked();
  jobject object = env.CallObjectMethod(
      m_javaRuntimeObject, GET_JAVAOBJECT_BY_ID_METHOD_ID, javaObjectID);
  return object;
}

void ObjectManager::UpdateCache(int objectID, jobject obj) {
  m_cache.update(objectID, obj);
}

jclass ObjectManager::GetJavaClass(const Local<Object>& instance) {
  DEBUG_WRITE("GetClass called");

  JSInstanceInfo* jsInfo = GetJSInstanceInfo(instance);
  jclass clazz = jsInfo->objectClazz;

  return clazz;
}

void ObjectManager::SetJavaClass(const Local<Object>& instance, jclass clazz) {
  JSInstanceInfo* jsInfo = GetJSInstanceInfo(instance);
  jsInfo->objectClazz = clazz;
}

int ObjectManager::GetOrCreateObjectId(jobject object) {
  JEnv env;
  AssertIsolateLocked();
  jint javaObjectID = env.CallIntMethod(
      m_javaRuntimeObject, GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID, object);

  return javaObjectID;
}

int ObjectManager::FindObjectId(jobject object) {
  JEnv env;
  AssertIsolateLocked();
  return env.CallIntMethod(m_javaRuntimeObject, FIND_JAVA_OBJECT_ID_METHOD_ID,
                           object);
}

void ObjectManager::RegisterStrong(jobject object, int javaObjectID) {
  JEnv env;
  AssertIsolateLocked();
  env.CallVoidMethod(m_javaRuntimeObject, MAKE_INSTANCE_STRONG_METHOD_ID,
                     object, javaObjectID);
}

Local<Object> ObjectManager::GetJsObjectByJavaObject(int javaObjectID) {
  auto isolate = m_isolate;
  EscapableHandleScope handleScope(isolate);

  auto state = m_idToObject.Find(javaObjectID);
  if (state == nullptr) {
    return handleScope.Escape(RebuildCanary(javaObjectID));
  }

  return handleScope.Escape(JsObjectFor(state));
}

Local<Object> ObjectManager::JsObjectFor(ObjectWeakCallbackState* state) {
  if (state->queued) {
    state->reacquired = true;
  }
  return Local<Object>::New(m_isolate, *state->target);
}

void ObjectManager::ReleaseUnwrappedArgs(const JavaCallArgs& args) {
  if (args.converted || args.count == 0) {
    return;
  }
  JEnv env;
  JNIEnv* raw = env;
  // Runs after the failure was rethrown to Java; keep that exception for the caller.
  jthrowable pending = raw->ExceptionOccurred();
  raw->ExceptionClear();

  jsize argc = args.count;
  std::vector<jlong> values(JavaCallArgs::PackedLength(argc));
  raw->GetLongArrayRegion(args.values, 0, values.size(), values.data());
  AssertIsolateLocked();
  for (jsize i = 0; i < argc; i++) {
    if (JavaCallArgs::TypeAt(values.data(), argc, i) != Type::JsObject ||
        (values[i] & JavaCallArgs::FreshArg) == 0) {
      continue;
    }
    auto javaObjectID = static_cast<jint>(values[i]);
    // A canary record means JS wrapped the id meanwhile (another thread, or
    // the method lookup); its settle owns the Java entry, which may hold state.
    if (HasJsObject(javaObjectID) || FindCanaryRecord(javaObjectID) != nullptr) {
      continue;
    }
    raw->CallVoidMethod(m_javaRuntimeObject, RELEASE_NATIVE_INSTANCE_METHOD_ID,
                        javaObjectID);
    if (raw->ExceptionCheck()) {
      raw->ExceptionClear();
    }
  }

  if (pending != nullptr) {
    raw->Throw(pending);
    raw->DeleteLocalRef(pending);
  }
}

Local<Object> ObjectManager::GetLinkedJsObjectByJavaObject(int javaObjectID) {
  auto state = m_idToObject.Find(javaObjectID);
  if (state == nullptr) {
    return RebuildCanary(javaObjectID);
  }
  if (!state->linked) {
    return Local<Object>();
  }
  return JsObjectFor(state);
}

Local<Object> ObjectManager::CreateJSWrapper(jint javaObjectID,
                                             const string& typeName) {
  return CreateJSWrapperHelper(javaObjectID, typeName, nullptr);
}

Local<Object> ObjectManager::GetOrCreateJSWrapper(jobject object) {
  JEnv env;
  AssertIsolateLocked();
  jlong packed = env.CallLongMethod(
      m_javaRuntimeObject, GET_OR_CREATE_JAVA_OBJECT_ID_AND_CLASS_METHOD_ID,
      object);
  return GetOrCreateJSWrapper(static_cast<jint>(packed), static_cast<int>(packed >> 32), object);
}

Local<Object> ObjectManager::GetOrCreateJSWrapper(jint javaObjectID, int classIndex,
                                                  jobject instance) {
  // As GetJsObjectByJavaObject, then CreateJSWrapper, which rebuilds a canary itself.
  if (auto state = m_idToObject.Find(javaObjectID)) {
    return JsObjectFor(state);
  }
  return CreateJSWrapper(javaObjectID, classIndex, instance);
}

Local<Object> ObjectManager::CreateJSWrapper(jint javaObjectID, int classIndex,
                                             jobject instance) {
  jclass clazz;
  std::string scratch;
  MetadataNode* node = nullptr;
  const auto& className = WrapperClassFor(classIndex, instance, clazz, scratch, node);
  return CreateJSWrapperHelper(javaObjectID, className, clazz, node);
}

const std::string& ObjectManager::WrapperClassFor(int classIndex,
                                                  jobject instance,
                                                  jclass& clazz,
                                                  std::string& scratch,
                                                  MetadataNode*& node) {
  if (classIndex > 0 &&
      static_cast<size_t>(classIndex) < m_wrapperClasses.size() &&
      m_wrapperClasses[classIndex] != nullptr) {
    auto& entry = *m_wrapperClasses[classIndex];
    if (entry.node == nullptr) {
      entry.node = MetadataNode::GetOrCreate(entry.name);
    }
    clazz = entry.clazz;
    node = entry.node;
    return entry.name;
  }

  JEnv env;
  JniLocalRef localClass(env.GetObjectClass(instance));
  auto name = GetClassName(static_cast<jclass>(localClass));
  if (classIndex <= 0) {
    scratch = std::move(name);
    clazz = nullptr;
    return scratch;
  }

  // The object's own class, not whatever its name resolves to: they differ
  // when two class loaders define the same name.
  auto entry = std::make_unique<WrapperClass>();
  entry->name = std::move(name);
  entry->clazz = static_cast<jclass>(env.NewGlobalRef(localClass));
  if (static_cast<size_t>(classIndex) >= m_wrapperClasses.size()) {
    m_wrapperClasses.resize(classIndex + 1);
  }
  m_wrapperClasses[classIndex] = std::move(entry);
  clazz = m_wrapperClasses[classIndex]->clazz;
  return m_wrapperClasses[classIndex]->name;
}

Local<Object> ObjectManager::CreateJSWrapperHelper(jint javaObjectID,
                                                   const string& className,
                                                   jclass clazz,
                                                   MetadataNode* node) {
  auto isolate = m_isolate;

  // Nothing from here to the link runs JS or settles canaries, so the record
  // stays valid.
  CanaryRecord* record = FindCanaryRecord(javaObjectID);
  if (record != nullptr && record->extended) {
    // A JS-extended instance's canary starts from its record, not from its class.
    auto rebuilt = RebuildCanary(javaObjectID, record);
    if (!rebuilt.IsEmpty()) {
      return rebuilt;
    }
    record = FindCanaryRecord(javaObjectID);
  }

  if (node == nullptr) {
    node = MetadataNode::GetOrCreate(className);
  }

  auto jsWrapper = node->CreateJSWrapper(isolate, this);

  if (!jsWrapper.IsEmpty()) {
    if (clazz == nullptr) {
      JEnv env;
      clazz = env.FindClass(className);
    }
    Link(jsWrapper, javaObjectID, clazz, LinkKind::kPlain, nullptr, record);
  }
  return jsWrapper;
}

/* *
 * Link the JavaScript object and it's java counterpart with an ID
 */
void ObjectManager::Link(const Local<Object>& object, uint32_t javaObjectID,
                         jclass clazz, LinkKind kind,
                         const Persistent<ObjectTemplate>* instanceTemplate) {
  Link(object, javaObjectID, clazz, kind, instanceTemplate, std::nullopt);
}

void ObjectManager::Link(const Local<Object>& object, uint32_t javaObjectID, jclass clazz,
                         LinkKind kind, const Persistent<ObjectTemplate>* instanceTemplate,
                         std::optional<CanaryRecord*> record, bool hydrate) {
  if (!IsJsRuntimeObject(object)) {
    string errMsg("Trying to link invalid 'this' to a Java object");
    throw NativeScriptException(errMsg);
  }

  auto isolate = m_isolate;

  DEBUG_WRITE("Linking js object: %d and java instance id: %d",
              object->GetIdentityHash(), javaObjectID);

  auto state = new ObjectWeakCallbackState(this, javaObjectID, clazz, isolate, object);

  // Array buffers carry no Metadata field and cannot hold the canary handlers.
  state->canary = WrapperLifetimeCanary::InScope(kind == LinkKind::kExtended) &&
                  HasMetadataField(object);
  if (state->canary) {
    state->target->SetWeak(state, CanaryDiedStatic, WeakCallbackType::kParameter);
  } else {
    // subscribe for JS GC event
    state->target->SetWeak(state, JSObjectFinalizerStatic,
                           WeakCallbackType::kFinalizer);
  }

  SetJsInfo(object, javaObjectID);

  bool inserted = m_idToObject.Insert(javaObjectID, state);
  // A second link for one id would leave this state's finalizer weakening the first
  // wrapper's Java object.
  NS_DCHECK(inserted);
  (void) inserted;

  if (state->canary) {
    state->record = AdoptCanary(object, javaObjectID, kind, instanceTemplate,
                                record.has_value() ? *record : FindCanaryRecord(javaObjectID),
                                hydrate);
  }
}

ObjectManager::CanaryRecord* ObjectManager::AdoptCanary(
    const Local<Object>& object, uint32_t javaObjectID, LinkKind kind,
    const Persistent<ObjectTemplate>* instanceTemplate, CanaryRecord* record, bool hydrate) {
  if (record == nullptr) {
    if (kind != LinkKind::kExtended) {
      return nullptr;
    }
    // Java calls into JS through this object for as long as it lives, so the
    // record is made now, not at the first write: a canary built later needs
    // the prototype and fields this one was registered with.
    NS_DCHECK(instanceTemplate != nullptr);
    auto isolate = m_isolate;
    auto created = std::make_unique<CanaryRecord>();
    created->extended = true;
    created->instanceTemplate = instanceTemplate;
    created->prototype.Reset(isolate, object->GetPrototype());
    created->callSuper.Reset(
        isolate, object->GetInternalField(static_cast<int>(MetadataNodeKeys::CallSuper)).As<Value>());
    created->metadata.Reset(
        isolate, object->GetInternalField(static_cast<int>(MetadataNodeKeys::Metadata)).As<Value>());
    Local<Value> implementationObject;
    if (V8GetPrivateValue(isolate, object, V8StringConstants::GetImplementationObject(isolate),
                          implementationObject) &&
        !implementationObject.IsEmpty()) {
      created->implementationObject.Reset(isolate, implementationObject);
    }
    record = created.get();
    m_canaryRecords.emplace(javaObjectID, std::move(created));
    return record;
  }
  if (!EnsureJavaStrong(javaObjectID, record)) {
    return nullptr;
  }
  if (hydrate && !record->state.IsEmpty()) {
    auto context = m_isolate->GetCurrentContext();
    if (context.IsEmpty()) {
      context = Runtime::GetRuntime(m_isolate)->GetContext();
    }
    WrapperLifetimeCanary::CopyOwnProperties(this, context, record->state.Get(m_isolate), object,
                                             !record->stateNotPlain);
  }
  return record;
}

ObjectManager::CanaryRecord* ObjectManager::FindCanaryRecord(int javaObjectID) {
  if (m_canaryRecords.empty()) {
    return nullptr;
  }
  auto it = m_canaryRecords.find(javaObjectID);
  return it != m_canaryRecords.end() ? it->second.get() : nullptr;
}

void ObjectManager::EraseCanaryRecord(int javaObjectID) {
  if (auto live = m_idToObject.Find(javaObjectID)) {
    live->record = nullptr;
  }
  auto it = m_canaryRecords.find(javaObjectID);
  if (it != m_canaryRecords.end()) {
    ForgetStateNames(it->second.get());
    m_canaryRecords.erase(it);
  }
}

void ObjectManager::CacheStateName(CanaryHandle* canary, const Local<Name>& property) {
  CanaryRecord* record = canary->record;
  if (record == nullptr || record->stateNotPlain || record->state.IsEmpty()) {
    return;
  }
  size_t index = StateNameSet(record);
  StateName* set = m_stateNames[index];
  for (size_t way = 0; way < kStateNameWays; way++) {
    if (set[way].record == record && set[way].name == property) {
      return;
    }
  }
  StateName& entry = set[m_stateNameVictims[index]];
  m_stateNameVictims[index] = (m_stateNameVictims[index] + 1) % kStateNameWays;
  entry.record = record;
  entry.name.Reset(m_isolate, property);
}

void ObjectManager::ForgetStateName(CanaryHandle* canary, const Local<Name>& property) {
  CanaryRecord* record = canary->record;
  if (record == nullptr) {
    return;
  }
  StateName* set = m_stateNames[StateNameSet(record)];
  for (size_t way = 0; way < kStateNameWays; way++) {
    if (set[way].record == record && set[way].name == property) {
      set[way].record = nullptr;
      set[way].name.Reset();
    }
  }
}

void ObjectManager::ForgetStateNames(CanaryRecord* record) {
  StateName* set = m_stateNames[StateNameSet(record)];
  for (size_t way = 0; way < kStateNameWays; way++) {
    if (set[way].record == record) {
      set[way].record = nullptr;
      set[way].name.Reset();
    }
  }
}

bool ObjectManager::EnsureJavaStrong(int javaObjectID, CanaryRecord* record) {
  if (!record->javaWeak) {
    return true;
  }
  JEnv env;
  AssertIsolateLocked();
  bool alive = env.CallBooleanMethod(m_javaRuntimeObject, STRENGTHEN_INSTANCE_METHOD_ID,
                                     javaObjectID) == JNI_TRUE;
  if (!alive) {
    // Collected; the watcher's report has not arrived yet.
    EraseCanaryRecord(javaObjectID);
    return false;
  }
  record->javaWeak = false;
  return true;
}

Local<Object> ObjectManager::RebuildCanary(int javaObjectID) {
  return RebuildCanary(javaObjectID, FindCanaryRecord(javaObjectID));
}

Local<Object> ObjectManager::RebuildCanary(int javaObjectID, CanaryRecord* record) {
  if (record == nullptr || !record->extended) {
    return Local<Object>();
  }
  auto isolate = m_isolate;
  EscapableHandleScope handleScope(isolate);
  if (!EnsureJavaStrong(javaObjectID, record)) {
    return handleScope.Escape(Local<Object>());
  }
  if (!record->shell.IsEmpty()) {
    Local<Object> canary = record->shell.Get(isolate)->Clone(isolate);
    m_lifetimeStats.canaryRebuilds++;
    Link(canary, javaObjectID, record->clazz, LinkKind::kExtended, nullptr, record, false);
    return handleScope.Escape(canary);
  }

  NS_DCHECK(record->instanceTemplate != nullptr);
  if (record->instanceTemplate == nullptr) {
    return handleScope.Escape(Local<Object>());
  }
  auto context = isolate->GetCurrentContext();
  Local<Object> canary;
  if (!record->instanceTemplate->Get(isolate)->NewInstance(context).ToLocal(&canary)) {
    return handleScope.Escape(Local<Object>());
  }
  Local<Value> prototype = record->prototype.Get(isolate);
  if (!prototype.IsEmpty() && (prototype->IsObject() || prototype->IsNull()) &&
      !prototype->StrictEquals(canary->GetPrototype())) {
    (void) canary->SetPrototype(context, prototype);
  }
  canary->SetInternalField(static_cast<int>(MetadataNodeKeys::CallSuper),
                           record->callSuper.Get(isolate));
  canary->SetInternalField(static_cast<int>(MetadataNodeKeys::Metadata),
                           record->metadata.Get(isolate));
  if (!record->implementationObject.IsEmpty()) {
    V8SetPrivateValue(isolate, canary, V8StringConstants::GetImplementationObject(isolate),
                      record->implementationObject.Get(isolate));
  }
  m_lifetimeStats.canaryRebuilds++;
  Link(canary, javaObjectID, record->clazz, LinkKind::kExtended, nullptr, record);
  // Taken before JS can reach the canary, so it holds nothing JS adds later
  // (#private brands, weak-collection keys); Link sets each clone's JsInfo.
  Local<Object> shell = canary->Clone(isolate);
  shell->SetInternalField(static_cast<int>(MetadataNodeKeys::JsInfo), Undefined(isolate));
  record->shell.Reset(isolate, shell);
  return handleScope.Escape(canary);
}

ObjectManager::ObjectWeakCallbackState* ObjectManager::CanaryOf(const Local<Object>& holder) {
  if (!HasMetadataField(holder)) {
    return nullptr;
  }
  int id = LinkedIdOf(holder);
  if (id == -1) {
    return nullptr;
  }
  auto state = m_idToObject.Find(id);
  // A `super` object shares its owner's id but is not the canary.
  if (state == nullptr || !state->canary || !state->linked || *state->target != holder) {
    return nullptr;
  }
  return state;
}

bool ObjectManager::IsCanary(const Local<Object>& holder) {
  return CanaryOf(holder) != nullptr;
}

bool ObjectManager::ExistingCanaryState(const Local<Object>& holder, Local<Object>* out) {
  auto state = CanaryOf(holder);
  if (state == nullptr) {
    return false;
  }
  CanaryRecord* record = FindCanaryRecord(state->javaObjectID);
  if (record == nullptr || record->state.IsEmpty()) {
    return false;
  }
  *out = record->state.Get(m_isolate);
  return true;
}

bool ObjectManager::CanaryStateForWrite(const Local<Object>& holder, Local<Object>* out) {
  if (ExistingCanaryState(holder, out)) {
    return true;
  }
  auto state = CanaryOf(holder);
  if (state == nullptr) {
    return false;
  }
  int id = state->javaObjectID;
  // Allocated before the record is looked up: a collection it starts runs the
  // canary callbacks, which read the records.
  Local<Object> fresh = Object::New(m_isolate, Null(m_isolate), nullptr, nullptr, 0);
  CanaryRecord* record = FindCanaryRecord(id);
  if (record == nullptr) {
    auto created = std::make_unique<CanaryRecord>();
    record = created.get();
    m_canaryRecords.emplace(id, std::move(created));
  }
  state->record = record;
  record->state.Reset(m_isolate, fresh);
  m_lifetimeStats.canaryStates++;
  *out = fresh;
  return true;
}

Local<Object> ObjectManager::StateOf(CanaryHandle* canary) {
  CanaryRecord* record = canary->record;
  return record != nullptr && !record->state.IsEmpty() ? record->state.Get(m_isolate)
                                                       : Local<Object>();
}

bool ObjectManager::StateIsPlain(CanaryHandle* canary) {
  return canary->record == nullptr || !canary->record->stateNotPlain;
}

void ObjectManager::SetStateNotPlain(CanaryHandle* canary) {
  if (canary->record != nullptr) {
    canary->record->stateNotPlain = true;
  }
}

Local<Object> ObjectManager::StateForWrite(CanaryHandle* canary) {
  Local<Object> existing = StateOf(canary);
  if (!existing.IsEmpty()) {
    return existing;
  }
  Local<Object> state;
  return CanaryStateForWrite(Local<Object>::New(m_isolate, *canary->target), &state) ? state
                                                                                     : Local<Object>();
}

void ObjectManager::CanaryDiedStatic(
    const WeakCallbackInfo<ObjectWeakCallbackState>& data) {
  ObjectWeakCallbackState* state = data.GetParameter();
  ObjectManager* thisPtr = state->thisPtr;
  state->handle.Reset();
  uint32_t id = state->javaObjectID;
  if (thisPtr->m_idToObject.Find(id) == state) {
    thisPtr->m_idToObject.Erase(id);
  }
  if (state->linked) {
    if (CanaryRecord* record = thisPtr->FindCanaryRecord(id)) {
      if (state->objectClazz != nullptr) {
        record->clazz = state->objectClazz;
      }
    }
    thisPtr->m_pendingCanaryDeaths.push_back(static_cast<jint>(id));
  }
  delete state;
}

void ObjectManager::CanaryGCEpilogue(Isolate* isolate, GCType type, GCCallbackFlags flags,
                                     void* data) {
  auto thisPtr = static_cast<ObjectManager*>(data);
  if (!thisPtr->m_pendingCanaryDeaths.empty()) {
    // Same as the finalizers': the next JS<->Java crossing flushes; this task
    // covers a thread that goes idle first.
    constexpr double kIdleFlushDelayMs = 4;
    thisPtr->ScheduleFinalizedBatchFlush(kIdleFlushDelayMs);
  }
}

bool ObjectManager::FlushCanaryDeaths() {
  ReleaseCollectedCanaryStates();
  if (m_pendingCanaryDeaths.empty()) {
    return true;
  }
  if (!m_deathInboxOpen) {
    auto runtime = Runtime::GetRuntime(m_isolate);
    m_deathInboxRuntimeId = runtime->GetId();
    WrapperLifetimeCanary::OpenDeathInbox(m_deathInboxRuntimeId, runtime->GetEventLoop());
    m_deathInboxOpen = true;
  }
  auto deaths = std::move(m_pendingCanaryDeaths);
  m_pendingCanaryDeaths.clear();
  std::sort(deaths.begin(), deaths.end());
  deaths.erase(std::unique(deaths.begin(), deaths.end()), deaths.end());

  std::vector<jint> weaken;
  std::vector<jint> release;
  for (jint id : deaths) {
    if (m_idToObject.Find(id) != nullptr) {
      // A new canary was built since: the Java side stays strong.
      continue;
    }
    // Weakening is idempotent: an id a failed batch already weakened is sent again.
    if (FindCanaryRecord(id) == nullptr) {
      release.push_back(id);
    } else {
      weaken.push_back(id);
    }
  }
  if (weaken.empty() && release.empty()) {
    return true;
  }

  JEnv env;
  JNIEnv* raw = env;
  auto toJava = [raw](const std::vector<jint>& ids) -> jintArray {
    if (ids.empty()) {
      return nullptr;
    }
    auto count = static_cast<jsize>(ids.size());
    jintArray array = raw->NewIntArray(count);
    if (array != nullptr) {
      raw->SetIntArrayRegion(array, 0, count, ids.data());
    }
    return array;
  };
  JniLocalRef weakenArray(toJava(weaken));
  JniLocalRef releaseArray(toJava(release));
  bool failed = (!weaken.empty() && weakenArray.IsNull()) ||
                (!release.empty() && releaseArray.IsNull());
  if (!release.empty()) {
    m_canaryReleaseCount++;
  }
  if (!failed) {
    AssertIsolateLocked();
    raw->CallVoidMethod(m_javaRuntimeObject, PROCESS_CANARY_BATCH_METHOD_ID,
                        (jintArray) weakenArray, static_cast<jint>(weaken.size()),
                        (jintArray) releaseArray, static_cast<jint>(release.size()));
    failed = raw->ExceptionCheck();
  }
  if (failed) {
    if (raw->ExceptionCheck()) {
      raw->ExceptionDescribe();
      raw->ExceptionClear();
    }
    // Java may have weakened some of them before failing: a canary rebuilt
    // meanwhile must make its object strong again.
    for (jint id : weaken) {
      if (CanaryRecord* record = FindCanaryRecord(id)) {
        record->javaWeak = true;
      }
    }
    m_pendingCanaryDeaths.insert(m_pendingCanaryDeaths.end(), deaths.begin(), deaths.end());
    return false;
  }

  m_lifetimeStats.collected += release.size();
  if (!weaken.empty()) {
    std::vector<jint> results(weaken.size());
    raw->GetIntArrayRegion((jintArray) weakenArray, 0, static_cast<jsize>(weaken.size()),
                           results.data());
    for (size_t i = 0; i < weaken.size(); i++) {
      if (results[i] < 0) {
        // Never registered (a failed construction), or already collected.
        EraseCanaryRecord(weaken[i]);
        continue;
      }
      if (CanaryRecord* record = FindCanaryRecord(weaken[i])) {
        record->javaWeak = true;
        m_lifetimeStats.weakened++;
      }
    }
  }
  return true;
}

std::map<std::string, ObjectManager::ClassCounts> ObjectManager::GetWrapperClassCounts() {
  std::map<std::string, ClassCounts> counts;
  std::unordered_map<jclass, std::string> names;
  auto nameOf = [&names](jclass clazz) -> const std::string& {
    auto it = names.find(clazz);
    if (it != names.end()) {
      return it->second;
    }
    std::string name = clazz != nullptr ? GetClassName(clazz) : std::string("<unknown>");
    std::replace(name.begin(), name.end(), '/', '.');
    return names.emplace(clazz, std::move(name)).first->second;
  };
  m_idToObject.ForEach([&](ObjectWeakCallbackState* state) {
    if (!state->linked) {
      return;
    }
    ClassCounts& entry = counts[nameOf(state->objectClazz)];
    entry.wrappers++;
    if (state->canary) {
      entry.canaries++;
    }
  });
  for (const auto& [id, record] : m_canaryRecords) {
    if (record->state.IsEmpty()) {
      continue;
    }
    jclass clazz = record->clazz;
    if (auto live = m_idToObject.Find(id)) {
      clazz = live->objectClazz;
    }
    counts[nameOf(clazz)].states++;
  }
  return counts;
}

bool ObjectManager::AddWeakCollectionFunctions(Local<Context> context, Local<Object> binding) {
  auto isolate = m_isolate;
  Local<Function> unwrap;
  Local<Function> wrap;
  return Function::New(context, UnwrapForWeakCollection).ToLocal(&unwrap) &&
         Function::New(context, WrapForWeakCollection).ToLocal(&wrap) &&
         binding->Set(context, ArgConverter::ConvertToV8String(isolate, "unwrap"), unwrap)
             .FromMaybe(false) &&
         binding->Set(context, ArgConverter::ConvertToV8String(isolate, "wrap"), wrap)
             .FromMaybe(false);
}

// unwrap(value, create): the weak collections' key for a canary's Java object,
// a frozen, null-prototype object that lives as long as the object's state,
// that is, until Java collects the object. Lookups pass `create` false and never
// make one: with no key yet, no entry can be keyed by it. The value itself for
// anything that is not a canary, a Proxy included (it is a key of its own).
namespace {
// Null rather than a throw while the isolate has no runtime.
ObjectManager* WeakCollectionManagerOf(Isolate* isolate) {
  Runtime* runtime = Runtime::TryGetRuntime(isolate);
  return runtime != nullptr ? runtime->GetObjectManager() : nullptr;
}

void RethrowToV8(const char* what) {
  NativeScriptException nsEx(std::string("Error: c++ exception: ") + what);
  nsEx.ReThrowToV8();
}
}  // namespace

void ObjectManager::UnwrapForWeakCollection(const FunctionCallbackInfo<Value>& info) {
  try {
    Isolate* isolate = info.GetIsolate();
    Local<Value> value = info[0];
    bool create = info[1]->IsTrue();
    info.GetReturnValue().Set(value);
    if (!value->IsObject() || value->IsProxy()) {
      return;
    }
    ObjectManager* thisPtr = WeakCollectionManagerOf(isolate);
    if (thisPtr == nullptr) {
      return;
    }
    Local<Object> holder = value.As<Object>();
    Local<Context> context = isolate->GetCurrentContext();
    Local<Private> keyOfState = thisPtr->m_keyOfState.Get(isolate);
    Local<Value> key;
    Local<Object> state;
    if (create ? !thisPtr->CanaryStateForWrite(holder, &state)
               : !thisPtr->ExistingCanaryState(holder, &state)) {
      // A canary whose link was released keeps its key itself.
      if (HasMetadataField(holder) && holder->GetPrivate(context, keyOfState).ToLocal(&key) &&
          key->IsObject()) {
        info.GetReturnValue().Set(key);
      }
      return;
    }
    if (state->GetPrivate(context, keyOfState).ToLocal(&key) && key->IsObject()) {
      info.GetReturnValue().Set(key);
      return;
    }
    if (!create) {
      return;
    }
    Local<Object> newKey = Object::New(isolate, Null(isolate), nullptr, nullptr, 0);
    if (!newKey
             ->SetPrivate(context, thisPtr->m_objectOfKey.Get(isolate),
                          Integer::NewFromUnsigned(isolate, static_cast<uint32_t>(LinkedIdOf(holder))))
             .FromMaybe(false) ||
        !newKey->SetIntegrityLevel(context, IntegrityLevel::kFrozen).FromMaybe(false) ||
        !state->SetPrivate(context, keyOfState, newKey).FromMaybe(false)) {
      return;
    }
    info.GetReturnValue().Set(newKey);
  } catch (NativeScriptException& e) {
    e.ReThrowToV8();
  } catch (std::exception& e) {
    RethrowToV8(e.what());
  } catch (...) {
    RethrowToV8("unknown");
  }
}

// wrap(key): the Java object a key stands for, as its live canary or a new one
// with its state, or undefined once Java collected it. Anything that is not a
// key passes through.
void ObjectManager::WrapForWeakCollection(const FunctionCallbackInfo<Value>& info) {
  try {
    Isolate* isolate = info.GetIsolate();
    Local<Value> value = info[0];
    info.GetReturnValue().Set(value);
    if (!value->IsObject()) {
      return;
    }
    ObjectManager* thisPtr = WeakCollectionManagerOf(isolate);
    if (thisPtr == nullptr) {
      return;
    }
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> key = value.As<Object>();
    Local<Value> idValue;
    if (!key->GetPrivate(context, thisPtr->m_objectOfKey.Get(isolate)).ToLocal(&idValue) ||
        !idValue->IsUint32()) {
      return;
    }
    // The key of a released canary lives exactly as long as the canary.
    Local<Value> released;
    if (key->GetPrivate(context, thisPtr->m_releasedOfKey.Get(isolate)).ToLocal(&released) &&
        released->IsObject()) {
      info.GetReturnValue().Set(released);
      return;
    }
    info.GetReturnValue().SetUndefined();
    int id = static_cast<int>(idValue.As<Uint32>()->Value());
    CanaryRecord* record = thisPtr->FindCanaryRecord(id);
    if (record == nullptr || record->state.IsEmpty()) {
      return;
    }
    Local<Value> recordKey;
    if (!record->state.Get(isolate)->GetPrivate(context, thisPtr->m_keyOfState.Get(isolate))
             .ToLocal(&recordKey) ||
        !recordKey->StrictEquals(key)) {
      return;
    }
    if (auto live = thisPtr->m_idToObject.Find(id)) {
      info.GetReturnValue().Set(thisPtr->JsObjectFor(live));
      return;
    }
    Local<Object> result = thisPtr->RebuildCanary(id);
    if (result.IsEmpty() && thisPtr->FindCanaryRecord(id) != nullptr) {
      // A plain object: a wrapper made from it the usual way is hydrated from its state.
      JniLocalRef javaObject(thisPtr->JavaObjectOrNull(id));
      if (!javaObject.IsNull()) {
        result = thisPtr->GetOrCreateJSWrapper(javaObject);
      }
    }
    if (!result.IsEmpty()) {
      info.GetReturnValue().Set(result);
    }
  } catch (NativeScriptException& e) {
    e.ReThrowToV8();
  } catch (std::exception& e) {
    RethrowToV8(e.what());
  } catch (...) {
    RethrowToV8("unknown");
  }
}

void ObjectManager::KeepWeakCollectionKey(int javaObjectID) {
  CanaryRecord* record = FindCanaryRecord(javaObjectID);
  auto live = m_idToObject.Find(javaObjectID);
  if (record == nullptr || record->state.IsEmpty() || live == nullptr || !live->canary ||
      m_keyOfState.IsEmpty()) {
    return;
  }
  Local<Context> context = m_isolate->GetCurrentContext();
  Local<Private> keyOfState = m_keyOfState.Get(m_isolate);
  Local<Value> key;
  if (!record->state.Get(m_isolate)->GetPrivate(context, keyOfState).ToLocal(&key) ||
      !key->IsObject()) {
    return;
  }
  // Private symbols bypass the canary's write handler, and V8 lets them be
  // added to the frozen key.
  Local<Object> canary = Local<Object>::New(m_isolate, *live->target);
  (void)canary->SetPrivate(context, keyOfState, key);
  (void)key.As<Object>()->SetPrivate(context, m_releasedOfKey.Get(m_isolate), canary);
}

jobject ObjectManager::JavaObjectOrNull(int javaObjectID) {
  JEnv env;
  JNIEnv* raw = env;
  AssertIsolateLocked();
  // getJavaObjectByID throws for an id whose object was collected.
  jobject object = raw->CallObjectMethod(m_javaRuntimeObject, GET_JAVAOBJECT_BY_ID_METHOD_ID,
                                         javaObjectID);
  if (raw->ExceptionCheck()) {
    raw->ExceptionClear();
    return nullptr;
  }
  return object;
}

#ifdef APPLICATION_IN_DEBUG
std::vector<ObjectManager::LeakProbeEntry> ObjectManager::GetLeakProbeEntries() {
  auto isolate = m_isolate;
  std::unordered_map<jclass, std::string> names;
  auto nameOf = [&names](jclass clazz) -> std::string {
    auto it = names.find(clazz);
    if (it != names.end()) {
      return it->second;
    }
    std::string name = clazz != nullptr ? GetClassName(clazz) : std::string("<unknown>");
    std::replace(name.begin(), name.end(), '/', '.');
    names.emplace(clazz, name);
    return name;
  };
  std::unordered_map<int, size_t> byId;
  std::vector<LeakProbeEntry> entries;
  m_idToObject.ForEach([&](ObjectWeakCallbackState* state) {
    if (!state->linked || state->target->IsEmpty()) {
      return;
    }
    LeakProbeEntry entry;
    entry.id = static_cast<int>(state->javaObjectID);
    entry.className = nameOf(state->objectClazz);
    entry.wrapper = state->target->Get(isolate);
    // A resurrecting wrapper's object turns weak once its wrapper was first collected;
    // one still queued from its first collection is garbage the next flush settles.
    entry.wrapperHoldsJava = state->canary || (!state->javaWeakened && !state->queued);
    entry.javaKeepsWrapper = !state->canary && state->javaWeakened;
    byId.emplace(entry.id, entries.size());
    entries.push_back(std::move(entry));
  });
  for (const auto& [id, record] : m_canaryRecords) {
    auto it = byId.find(id);
    if (it == byId.end()) {
      LeakProbeEntry entry;
      entry.id = id;
      entry.className = nameOf(record->clazz);
      it = byId.emplace(id, entries.size()).first;
      entries.push_back(std::move(entry));
    }
    LeakProbeEntry& entry = entries[it->second];
    const Global<Value>* kept[] = {&record->prototype, &record->callSuper, &record->metadata,
                                   &record->implementationObject};
    for (const Global<Value>* value : kept) {
      if (!value->IsEmpty()) {
        entry.recordValues.push_back(value->Get(isolate));
      }
    }
    if (!record->shell.IsEmpty()) {
      entry.recordValues.push_back(record->shell.Get(isolate));
    }
    if (!record->state.IsEmpty()) {
      entry.recordValues.push_back(record->state.Get(isolate));
    }
  }
  return entries;
}

jobjectArray ObjectManager::LeakProbeObjects(const std::vector<LeakProbeEntry>& entries) {
  JEnv env;
  JNIEnv* raw = env;
  jclass objectClass = env.FindClass("java/lang/Object");
  jobjectArray array =
      objectClass != nullptr
          ? raw->NewObjectArray(static_cast<jsize>(entries.size()), objectClass, nullptr)
          : nullptr;
  if (array == nullptr) {
    raw->ExceptionClear();
    return nullptr;
  }
  for (size_t i = 0; i < entries.size(); i++) {
    // Released at once: a local reference to an element would root it in the dump.
    JniLocalRef object(JavaObjectOrNull(entries[i].id));
    if (!object.IsNull()) {
      raw->SetObjectArrayElement(array, static_cast<jsize>(i), object);
    }
  }
  return array;
}
#endif

void ObjectManager::ReleaseCollectedCanaryStates() {
  if (!m_deathInboxOpen) {
    return;
  }
  for (int id : WrapperLifetimeCanary::TakeCollected(m_deathInboxRuntimeId)) {
    // A live canary keeps its object strongly held, so Java cannot have
    // collected an object that has one.
    if (m_idToObject.Find(id) != nullptr) {
      continue;
    }
    if (FindCanaryRecord(id) != nullptr) {
      EraseCanaryRecord(id);
      m_lifetimeStats.canaryNativeDeaths++;
    }
  }
}

bool ObjectManager::CloneLink(const Local<Object>& src,
                              const Local<Object>& dest) {
  auto source = UnwrapProxy(src);
  if (!source->IsObject() || !IsJsRuntimeObject(source.As<Object>())) {
    return false;
  }

  auto owner = GetJSInstanceInfoOwner(source.As<Object>());
  if (owner.IsEmpty()) {
    return false;
  }

  auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
  dest->SetInternalField(jsInfoIdx, owner->GetInternalField(jsInfoIdx));
  return true;
}

string ObjectManager::GetClassName(jobject javaObject) {
  InitializeJNI();
  JEnv env;
  JniLocalRef objectClass(env.GetObjectClass(javaObject));

  return GetClassName((jclass)objectClass);
}

string ObjectManager::GetClassName(jclass clazz) {
  InitializeJNI();
  JEnv env;
  JniLocalRef javaCanonicalName(
      env.CallObjectMethod(clazz, GET_NAME_METHOD_ID));

  string className = ArgConverter::jstringToString(javaCanonicalName);

  std::replace(className.begin(), className.end(), '.', '/');

  return className;
}

void ObjectManager::JSObjectFinalizerStatic(
    const WeakCallbackInfo<ObjectWeakCallbackState>& data) {
  ObjectWeakCallbackState* callbackState = data.GetParameter();

  ObjectManager* thisPtr = callbackState->thisPtr;

  auto isolate = data.GetIsolate();

  thisPtr->JSObjectFinalizer(isolate, callbackState);
}

void ObjectManager::JSObjectFinalizer(Isolate* isolate,
                                      ObjectWeakCallbackState* callbackState) {
  HandleScope handleScope(m_isolate);
  Persistent<Object>* po = callbackState->target;

  if (!callbackState->linked) {
    // The link was already torn down (ReleaseNativeCounterpart); nothing but
    // the registration is left to drop.
    m_idToObject.Erase(callbackState->javaObjectID);
    po->Reset();
    delete callbackState;
    return;
  }

  // Kept alive until the batch has asked the Java side whether it may die.
  po->SetWeak(callbackState, JSObjectFinalizerStatic,
              WeakCallbackType::kFinalizer);
  m_lifetimeStats.resurrections++;

  if (callbackState->queued) {
    // Collected again before the flush, so whatever handed it back has let go.
    callbackState->reacquired = false;
    return;
  }
  callbackState->queued = true;
  auto javaObjectID = static_cast<jint>(callbackState->javaObjectID);
  (callbackState->javaWeakened ? m_pendingCheck : m_pendingWeaken)
      .push_back(javaObjectID);

  // Never flushed from here: that would call into Java and free other
  // wrappers' handles in the middle of V8's finalizer pass. The next JS<->Java
  // crossing flushes; this task covers a thread that goes idle first, and is
  // delayed so that a busy runtime does not wake its event loop per GC.
  constexpr double kIdleFlushDelayMs = 4;
  ScheduleFinalizedBatchFlush(kIdleFlushDelayMs);
}

bool ObjectManager::ScheduleFinalizedBatchFlush(double delayMs) {
  if (m_flushScheduled) {
    return true;
  }
  auto runtime = Runtime::GetRuntime(m_isolate);
  auto eventLoop = runtime->GetEventLoop();
  if (eventLoop == nullptr) {
    return false;
  }
  m_flushScheduled = true;
  int runtimeId = runtime->GetId();
  auto flush = [runtimeId]() {
    // Runs on the runtime's own thread; skip if the runtime is gone.
    auto current = Runtime::GetCurrentRuntime();
    if (current == nullptr || current->GetId() != runtimeId) {
      return;
    }
    auto isolate = current->GetIsolate();
    v8::Locker locker(isolate);
    Isolate::Scope isolateScope(isolate);
    HandleScope handleScope(isolate);
    current->GetObjectManager()->FlushFinalizedBatch();
  };
  if (delayMs > 0) {
    eventLoop->PostInternalDelayed(std::move(flush), delayMs);
  } else {
    eventLoop->PostInternal(std::move(flush));
  }
  return true;
}

void ObjectManager::FlushFinalizedBatch() {
  m_flushScheduled = false;
  m_flushBackingOff = false;
  if (m_pendingWeaken.empty() && m_pendingCheck.empty() &&
      m_pendingCanaryDeaths.empty() && !m_deathInboxOpen) {
    return;
  }
  // A failed Java->JS call returns with its exception pending: JNI may not be used until it
  // is thrown, and the flush must not mistake it for its own failure and clear it.
  if (JEnv().ExceptionCheck()) {
    ScheduleFinalizedBatchFlush();
    return;
  }
  HandleScope handleScope(m_isolate);

  if (!FlushCanaryDeaths()) {
    constexpr double kRetryDelayMs = 100;
    m_flushBackingOff = ScheduleFinalizedBatchFlush(kRetryDelayMs);
    return;
  }
  if (m_pendingWeaken.empty() && m_pendingCheck.empty()) {
    return;
  }

  auto weaken = std::move(m_pendingWeaken);
  auto check = std::move(m_pendingCheck);
  m_pendingWeaken.clear();
  m_pendingCheck.clear();

  // A wrapper handed back to JS since it was queued is reachable again: its
  // Java side keeps its current strength until a later collection.
  auto keepCollected = [this](std::vector<jint>& ids) {
    size_t kept = 0;
    for (jint id : ids) {
      ObjectWeakCallbackState* state = m_idToObject.Find(id);
      if (state == nullptr) {
        continue;
      }
      if (state->reacquired) {
        state->reacquired = false;
        state->queued = false;
        continue;
      }
      ids[kept++] = id;
    }
    ids.resize(kept);
  };
  keepCollected(weaken);
  keepCollected(check);
  if (weaken.empty() && check.empty()) {
    return;
  }

  JEnv env;
  JNIEnv* raw = env;
  auto toJava = [raw](const std::vector<jint>& ids) -> jintArray {
    if (ids.empty()) {
      return nullptr;
    }
    auto count = static_cast<jsize>(ids.size());
    jintArray array = raw->NewIntArray(count);
    if (array != nullptr) {
      raw->SetIntArrayRegion(array, 0, count, ids.data());
    }
    return array;
  };
  JniLocalRef weakenArray(toJava(weaken));
  JniLocalRef checkArray(toJava(check));
  bool failed = (!weaken.empty() && weakenArray.IsNull()) ||
                (!check.empty() && checkArray.IsNull());
  if (!failed) {
    // Java overwrites the id of every object that is already dead with -1.
    // Called without JEnv's check, which would throw into whichever JS call
    // happens to be crossing.
    AssertIsolateLocked();
    raw->CallVoidMethod(m_javaRuntimeObject, PROCESS_FINALIZED_BATCH_METHOD_ID,
                        (jintArray) weakenArray,
                        static_cast<jint>(weaken.size()),
                        (jintArray) checkArray,
                        static_cast<jint>(check.size()));
    failed = raw->ExceptionCheck();
  }
  if (failed) {
    // Most likely an OutOfMemoryError. Every Java-side step is idempotent, so
    // the whole batch is retried, after a pause so a persistent failure
    // cannot spin the event loop.
    if (raw->ExceptionCheck()) {
      raw->ExceptionDescribe();
      raw->ExceptionClear();
    }
    m_pendingWeaken.insert(m_pendingWeaken.end(), weaken.begin(), weaken.end());
    m_pendingCheck.insert(m_pendingCheck.end(), check.begin(), check.end());
    constexpr double kRetryDelayMs = 100;
    m_flushBackingOff = ScheduleFinalizedBatchFlush(kRetryDelayMs);
    return;
  }

  auto settle = [this, raw](const std::vector<jint>& ids, jintArray array,
                            bool weakened) {
    if (ids.empty()) {
      return;
    }
    std::vector<jint> results(ids.size());
    raw->GetIntArrayRegion(array, 0, static_cast<jsize>(ids.size()),
                           results.data());
    for (size_t i = 0; i < ids.size(); i++) {
      ObjectWeakCallbackState* state = m_idToObject.Find(ids[i]);
      if (state == nullptr) {
        continue;
      }
      if (results[i] < 0) {
        ReleaseCollectedWrapper(ids[i]);
        continue;
      }
      state->queued = false;
      if (weakened) {
        state->javaWeakened = true;
        m_lifetimeStats.weakened++;
      }
    }
  };
  settle(weaken, (jintArray) weakenArray, true);
  settle(check, (jintArray) checkArray, false);
}

void ObjectManager::ReleaseCollectedWrapper(int javaObjectID) {
  ObjectWeakCallbackState* state = m_idToObject.Find(javaObjectID);
  if (state == nullptr) {
    return;
  }
  m_lifetimeStats.collected++;
  Persistent<Object>* po = state->target;
  if (state->linked) {
    MarkLinkReleased(po->Get(m_isolate));
  }
  m_idToObject.Erase(javaObjectID);
  po->Reset();
  delete state;
}

int ObjectManager::GenerateNewObjectID() {
  const int one = 1;
  int oldValue = __sync_fetch_and_add(&m_currentObjectId, one);

  return oldValue;
}

void ObjectManager::ReleaseAllRegistered() {
  HandleScope handleScope(m_isolate);

  m_pendingWeaken.clear();
  m_pendingCheck.clear();
  m_pendingCanaryDeaths.clear();
  if (m_canaryEpilogueAdded) {
    m_isolate->RemoveGCEpilogueCallback(CanaryGCEpilogue, this);
    m_canaryEpilogueAdded = false;
  }
  if (WrapperLifetimeCanary::Enabled()) {
    if (m_deathInboxOpen) {
      WrapperLifetimeCanary::CloseDeathInbox(m_deathInboxRuntimeId);
      m_deathInboxOpen = false;
    }
  }
  m_idToObject.ForEach([](ObjectWeakCallbackState* state) { state->record = nullptr; });
  for (auto& set : m_stateNames) {
    for (StateName& entry : set) {
      entry.record = nullptr;
      entry.name.Reset();
    }
  }
  m_canaryRecords.clear();
  m_instanceFieldTableKey.Reset();
  m_keyOfState.Reset();
  m_objectOfKey.Reset();
  m_releasedOfKey.Reset();

  auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);

  // Detached first: nothing below should observe a half-emptied map, and the
  // finalizers these handles were armed with will never run now anyway.
  m_idToObject.DrainAll([this, jsInfoIdx](ObjectWeakCallbackState* state) {
    Persistent<Object>* po = state->target;

    if (!po->IsEmpty()) {
      auto local = po->Get(m_isolate);
      if (!local.IsEmpty()) {
        local->SetInternalField(jsInfoIdx, Undefined(m_isolate));
      }
      po->Reset();
    }

    delete state;
  });

  if (m_poJsWrapperFunc != nullptr) {
    m_poJsWrapperFunc->Reset();
    delete m_poJsWrapperFunc;
    m_poJsWrapperFunc = nullptr;
  }
  m_jsWrapperTemplate.Reset();
}

ObjectManager::~ObjectManager() {
  // JNI only -- the isolate is already disposed by the time this runs. The LRU
  // cache holds a JNI weak global ref per entry (up to its capacity) and only
  // ever evicted them under capacity pressure, so a worker that touched Java
  // objects abandoned the whole cache when it died. ART's weak-global table is
  // bounded, so enough worker cycles turned that leak into an abort.
  m_cache.clear();

  JEnv env;
  for (const auto& entry : m_wrapperClasses) {
    if (entry != nullptr) {
      env.DeleteGlobalRef(entry->clazz);
    }
  }
  m_wrapperClasses.clear();
}

jweak ObjectManager::NewWeakGlobalRefCallback(const int& javaObjectID,
                                              void* state) {
  auto objManager = reinterpret_cast<ObjectManager*>(state);

  JniLocalRef obj(objManager->GetJavaObjectByIDImpl(javaObjectID));

  JEnv env;
  jweak weakRef = env.NewWeakGlobalRef(obj);

  return weakRef;
}

void ObjectManager::DeleteWeakGlobalRefCallback(const jweak& object,
                                                void* state) {
  JEnv env;
  env.DeleteWeakGlobalRef(object);
}

bool ObjectManager::ValidateWeakGlobalRefCallback(const int& javaObjectID,
                                                  const jweak& object,
                                                  void* state) {
  JEnv env;
  return !env.isSameObject(object, NULL);
}

Local<Object> ObjectManager::GetEmptyObject(Isolate* isolate) {
  // The same object `new` on m_poJsWrapperFunc makes, without entering its callback.
  auto context = Runtime::GetRuntime(isolate)->GetContext();
  auto val = m_jsWrapperTemplate.Get(isolate)->NewInstance(context);
  if (val.IsEmpty()) {
    return Local<Object>();
  }
  auto localVal = val.ToLocalChecked();
  NS_DCHECK(localVal->IsObject());
  auto obj = localVal.As<Object>();
  return obj;
}

void ObjectManager::JSWrapperConstructorCallback(
    const v8::FunctionCallbackInfo<v8::Value>& info) {
  NS_DCHECK(info.IsConstructCall());
}

void ObjectManager::ReleaseNativeCounterpart(v8::Local<v8::Object>& object) {
  if (!object->IsObject()) {
    throw NativeScriptException("Argument is not an object!");
  }

  auto source = UnwrapProxy(object);
  if (!source->IsObject()) {
    throw NativeScriptException("Trying to release a non native object!");
  }
  auto instance = source.As<Object>();

  // A `super` object shares its owner's JsInfo link; the owner is the one
  // registered, so the release is resolved and recorded there.
  Local<Value> superOwner;
  V8GetPrivateValue(m_isolate, instance, ArgConverter::ConvertToV8String(m_isolate, "superowner"), superOwner);
  if (!superOwner.IsEmpty() && superOwner->IsObject()) {
    instance = superOwner.As<Object>();
  }

  Local<Object> owner;
  if (IsJsRuntimeObject(instance)) {
    owner = GetJSInstanceInfoOwner(instance);
  }
  if (owner.IsEmpty()) {
    throw NativeScriptException("Trying to release a non native object!");
  }

  auto javaObjectID = LinkedIdOf(owner);

  JEnv env;
  AssertIsolateLocked();
  env.CallVoidMethod(m_javaRuntimeObject, RELEASE_NATIVE_INSTANCE_METHOD_ID,
                     javaObjectID);
  m_lifetimeStats.releasedExplicitly++;
  // What weak collections keyed on the object stays keyed on the released
  // canary, now a plain JS object, for as long as it lives.
  KeepWeakCollectionKey(javaObjectID);
  // Java no longer knows the id, so nothing could rebuild a canary from it.
  EraseCanaryRecord(javaObjectID);

  // The registration outlives the link: it is dropped by the finalizer once
  // the wrapper is collected.
  auto state = m_idToObject.Find(javaObjectID);
  if (state != nullptr) {
    state->linked = false;
  }

  MarkLinkReleased(owner);

  // Every alias of the link is cleared with it.
  Local<Value> aliasesValue;
  V8GetPrivateValue(m_isolate, owner, ArgConverter::ConvertToV8String(m_isolate, "superaliases"), aliasesValue);
  if (!aliasesValue.IsEmpty() && aliasesValue->IsArray()) {
    auto aliases = aliasesValue.As<Array>();
    auto context = m_isolate->GetCurrentContext();
    for (uint32_t i = 0; i < aliases->Length(); i++) {
      Local<Value> alias;
      if (aliases->Get(context, i).ToLocal(&alias) && alias->IsObject() &&
          IsJsRuntimeObject(alias.As<Object>())) {
        MarkLinkReleased(alias.As<Object>());
      }
    }
  }
}

jclass ObjectManager::JAVA_LANG_CLASS = nullptr;
jmethodID ObjectManager::GET_NAME_METHOD_ID = nullptr;
jmethodID ObjectManager::GET_JAVAOBJECT_BY_ID_METHOD_ID = nullptr;
jmethodID ObjectManager::GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID = nullptr;
jmethodID ObjectManager::GET_OR_CREATE_JAVA_OBJECT_ID_AND_CLASS_METHOD_ID =
    nullptr;
jmethodID ObjectManager::FIND_JAVA_OBJECT_ID_METHOD_ID = nullptr;
jmethodID ObjectManager::MAKE_INSTANCE_STRONG_METHOD_ID = nullptr;
jmethodID ObjectManager::PROCESS_FINALIZED_BATCH_METHOD_ID = nullptr;
jmethodID ObjectManager::RELEASE_NATIVE_INSTANCE_METHOD_ID = nullptr;
jmethodID ObjectManager::RELEASE_IF_COLLECTED_METHOD_ID = nullptr;
jmethodID ObjectManager::PROCESS_CANARY_BATCH_METHOD_ID = nullptr;
jmethodID ObjectManager::STRENGTHEN_INSTANCE_METHOD_ID = nullptr;
