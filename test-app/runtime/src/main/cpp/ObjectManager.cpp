#include "ObjectManager.h"

#include <algorithm>
#include <sstream>

#include "ArgConverter.h"
#include "ManualInstrumentation.h"
#include "MetadataNode.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "include/v8.h"

using namespace v8;
using namespace std;
using namespace tns;

ObjectManager::ObjectManager(jobject javaRuntimeObject)
    : m_javaRuntimeObject(javaRuntimeObject),
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
}

JniLocalRef ObjectManager::GetJavaObjectByJsObject(
    const Local<Object>& object) {
  JSInstanceInfo* jsInstanceInfo = GetJSInstanceInfo(object);

  if (jsInstanceInfo != nullptr) {
    jweak existingObject;
    try {
      existingObject = GetJavaObjectByID(jsInstanceInfo->JavaObjectID);
    } catch (const NativeScriptException& e) {
      // this captures usually the error
      auto className = GetClassName(jsInstanceInfo->ObjectClazz);
      throw NativeScriptException("Failed to get Java object by ID. id=" +
                                  std::to_string(jsInstanceInfo->JavaObjectID) +
                                  ", class=" + className + ". " +
                                  e.GetErrorMessage());
    }
    if (m_useGlobalRefs) {
      JniLocalRef javaObject(existingObject, true);
      return javaObject;
    } else {
      JEnv env;
      JniLocalRef javaObject(env.NewLocalRef(existingObject));
      return javaObject;
    }
  }

  return JniLocalRef();
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

Local<Object> ObjectManager::GetJSInstanceInfoOwner(const Local<Object>& object) {
  const int jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
  if (object->GetInternalField(jsInfoIdx).As<Value>()->IsExternal()) {
    return object;
  }

  // Typescript object layout has an object instance as child of the actual
  // registered instance. checking for that
  auto prototypeObject = object->GetPrototype().As<Object>();
  if (!prototypeObject.IsEmpty() && prototypeObject->IsObject()) {
    DEBUG_WRITE("GetJSInstanceInfo: need to check prototype :%d",
                prototypeObject->GetIdentityHash());
    if (IsJsRuntimeObject(prototypeObject) &&
        prototypeObject->GetInternalField(jsInfoIdx).As<Value>()->IsExternal()) {
      return prototypeObject;
    }
  }

  return Local<Object>();
}

ObjectManager::JSInstanceInfo*
ObjectManager::GetJSInstanceInfoFromRuntimeObject(const Local<Object>& object) {
  HandleScope handleScope(m_isolate);

  auto owner = GetJSInstanceInfoOwner(object);
  if (owner.IsEmpty()) {
    return nullptr;
  }

  auto external = owner->GetInternalField(static_cast<int>(MetadataNodeKeys::JsInfo)).As<External>();
  return static_cast<JSInstanceInfo*>(external->Value(v8::kExternalPointerTypeTagDefault));
}

bool ObjectManager::IsJsRuntimeObject(const v8::Local<v8::Object>& object) {
  int internalFieldCount = object->InternalFieldCount();
  const int count = static_cast<int>(MetadataNodeKeys::END);
  return internalFieldCount == count;
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
  jclass clazz = jsInfo->ObjectClazz;

  return clazz;
}

void ObjectManager::SetJavaClass(const Local<Object>& instance, jclass clazz) {
  JSInstanceInfo* jsInfo = GetJSInstanceInfo(instance);
  jsInfo->ObjectClazz = clazz;
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

  auto it = m_idToObject.find(javaObjectID);
  if (it == m_idToObject.end()) {
    return handleScope.Escape(Local<Object>());
  }

  ObjectWeakCallbackState* state = it->second;
  if (state->queued) {
    state->reacquired = true;
  }

  auto localObject = Local<Object>::New(isolate, *state->target);
  return handleScope.Escape(localObject);
}

Local<Object> ObjectManager::GetLinkedJsObjectByJavaObject(int javaObjectID) {
  auto it = m_idToObject.find(javaObjectID);
  if (it == m_idToObject.end() || it->second->jsInfo == nullptr) {
    return Local<Object>();
  }
  return GetJsObjectByJavaObject(javaObjectID);
}

Local<Object> ObjectManager::CreateJSWrapper(jint javaObjectID,
                                             const string& typeName) {
  return CreateJSWrapperHelper(javaObjectID, typeName, nullptr);
}

Local<Object> ObjectManager::CreateJSWrapper(jint javaObjectID,
                                             const string& typeName,
                                             jobject instance) {
  JEnv env;
  JniLocalRef clazz(env.GetObjectClass(instance));

  return CreateJSWrapperHelper(javaObjectID, typeName, clazz);
}

Local<Object> ObjectManager::CreateJSWrapperHelper(jint javaObjectID,
                                                   const string& typeName,
                                                   jclass clazz) {
  auto isolate = m_isolate;

  auto className = (clazz != nullptr) ? GetClassName(clazz) : typeName;

  auto node = MetadataNode::GetOrCreate(className);

  auto jsWrapper = node->CreateJSWrapper(isolate, this);

  if (!jsWrapper.IsEmpty()) {
    JEnv env;
    auto claz = env.FindClass(className);
    Link(jsWrapper, javaObjectID, claz);
  }
  return jsWrapper;
}

/* *
 * Link the JavaScript object and it's java counterpart with an ID
 */
void ObjectManager::Link(const Local<Object>& object, uint32_t javaObjectID,
                         jclass clazz) {
  if (!IsJsRuntimeObject(object)) {
    string errMsg("Trying to link invalid 'this' to a Java object");
    throw NativeScriptException(errMsg);
  }

  auto isolate = m_isolate;

  DEBUG_WRITE("Linking js object: %d and java instance id: %d",
              object->GetIdentityHash(), javaObjectID);

  auto jsInstanceInfo =
      new JSInstanceInfo(javaObjectID, clazz);

  auto objectHandle = new Persistent<Object>(isolate, object);
  auto state = new ObjectWeakCallbackState(this, jsInstanceInfo, objectHandle);

  // subscribe for JS GC event
  objectHandle->SetWeak(state, JSObjectFinalizerStatic,
                        WeakCallbackType::kFinalizer);

  auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);

  auto jsInfo = External::New(isolate, jsInstanceInfo, v8::kExternalPointerTypeTagDefault);

  // link
  object->SetInternalField(jsInfoIdx, jsInfo);

  m_idToObject.emplace(javaObjectID, state);
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
  auto jsInstanceInfo = GetJSInstanceInfoFromRuntimeObject(po->Get(m_isolate));

  if (jsInstanceInfo == nullptr) {
    // The link was already torn down (ReleaseNativeCounterpart); nothing but
    // the registration is left to drop.
    m_idToObject.erase(callbackState->javaObjectID);
    po->Reset();
    delete po;
    delete callbackState;
    return;
  }

  // Kept alive until the batch has asked the Java side whether it may die.
  po->SetWeak(callbackState, JSObjectFinalizerStatic,
              WeakCallbackType::kFinalizer);

  if (callbackState->queued) {
    // Collected again before the flush, so whatever handed it back has let go.
    callbackState->reacquired = false;
    return;
  }
  callbackState->queued = true;
  auto javaObjectID = static_cast<jint>(jsInstanceInfo->JavaObjectID);
  (callbackState->javaWeakened ? m_pendingCheck : m_pendingWeaken)
      .push_back(javaObjectID);

  // Never flushed from here: that would call into Java and free other
  // wrappers' handles in the middle of V8's finalizer pass. The next JS<->Java
  // crossing flushes; this task covers a thread that goes idle first.
  ScheduleFinalizedBatchFlush();
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
  if (m_pendingWeaken.empty() && m_pendingCheck.empty()) {
    return;
  }
  HandleScope handleScope(m_isolate);

  auto weaken = std::move(m_pendingWeaken);
  auto check = std::move(m_pendingCheck);
  m_pendingWeaken.clear();
  m_pendingCheck.clear();

  // A wrapper handed back to JS since it was queued is reachable again: its
  // Java side keeps its current strength until a later collection.
  auto keepCollected = [this](std::vector<jint>& ids) {
    size_t kept = 0;
    for (jint id : ids) {
      auto it = m_idToObject.find(id);
      if (it == m_idToObject.end()) {
        continue;
      }
      ObjectWeakCallbackState* state = it->second;
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
      auto it = m_idToObject.find(ids[i]);
      if (it == m_idToObject.end()) {
        continue;
      }
      if (results[i] < 0) {
        ReleaseCollectedWrapper(ids[i]);
        continue;
      }
      it->second->queued = false;
      if (weakened) {
        it->second->javaWeakened = true;
      }
    }
  };
  settle(weaken, (jintArray) weakenArray, true);
  settle(check, (jintArray) checkArray, false);
}

void ObjectManager::ReleaseCollectedWrapper(int javaObjectID) {
  auto it = m_idToObject.find(javaObjectID);
  if (it == m_idToObject.end()) {
    return;
  }
  ObjectWeakCallbackState* state = it->second;
  Persistent<Object>* po = state->target;
  auto object = po->Get(m_isolate);
  auto jsInstanceInfo = GetJSInstanceInfoFromRuntimeObject(object);
  if (jsInstanceInfo != nullptr) {
    delete jsInstanceInfo;
    auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
    object->SetInternalField(jsInfoIdx, Undefined(m_isolate));
  }
  m_idToObject.erase(it);
  po->Reset();
  delete po;
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

  auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);

  // Detached first: nothing below should observe a half-emptied map, and the
  // finalizers these handles were armed with will never run now anyway.
  auto survivors = std::move(m_idToObject);
  m_idToObject.clear();

  for (auto& entry : survivors) {
    ObjectWeakCallbackState* state = entry.second;
    Persistent<Object>* po = state->target;

    if (!po->IsEmpty()) {
      auto local = po->Get(m_isolate);
      if (!local.IsEmpty()) {
        // Drop the back-pointer before the JSInstanceInfo goes away.
        local->SetInternalField(jsInfoIdx, Undefined(m_isolate));
      }
      po->Reset();
    }

    delete po;
    delete state->jsInfo;
    delete state;
  }

  if (m_poJsWrapperFunc != nullptr) {
    m_poJsWrapperFunc->Reset();
    delete m_poJsWrapperFunc;
    m_poJsWrapperFunc = nullptr;
  }
}

ObjectManager::~ObjectManager() {
  // JNI only -- the isolate is already disposed by the time this runs. The LRU
  // cache holds a JNI weak global ref per entry (up to its capacity) and only
  // ever evicted them under capacity pressure, so a worker that touched Java
  // objects abandoned the whole cache when it died. ART's weak-global table is
  // bounded, so enough worker cycles turned that leak into an abort.
  m_cache.clear();
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
  auto emptyObjCtorFunc = Local<Function>::New(isolate, *m_poJsWrapperFunc);
  auto context = Runtime::GetRuntime(isolate)->GetContext();
  auto val = emptyObjCtorFunc->CallAsConstructor(context, 0, nullptr);
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

  // A `super` object shares its owner's JsInfo External; the owner is the one
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

  auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
  auto jsInstanceInfo = static_cast<JSInstanceInfo*>(
      owner->GetInternalField(jsInfoIdx).As<External>()->Value(v8::kExternalPointerTypeTagDefault));

  JEnv env;
  AssertIsolateLocked();
  env.CallVoidMethod(m_javaRuntimeObject, RELEASE_NATIVE_INSTANCE_METHOD_ID,
                     jsInstanceInfo->JavaObjectID);

  // The registration outlives the link: it is dropped by the finalizer once
  // the wrapper is collected. Until then the entry must not point at the
  // JSInstanceInfo being freed here.
  auto it = m_idToObject.find(jsInstanceInfo->JavaObjectID);
  if (it != m_idToObject.end()) {
    it->second->jsInfo = nullptr;
  }

  delete jsInstanceInfo;
  owner->SetInternalField(jsInfoIdx, Undefined(m_isolate));

  // Every alias of the freed External must be cleared with it.
  Local<Value> aliasesValue;
  V8GetPrivateValue(m_isolate, owner, ArgConverter::ConvertToV8String(m_isolate, "superaliases"), aliasesValue);
  if (!aliasesValue.IsEmpty() && aliasesValue->IsArray()) {
    auto aliases = aliasesValue.As<Array>();
    auto context = m_isolate->GetCurrentContext();
    for (uint32_t i = 0; i < aliases->Length(); i++) {
      Local<Value> alias;
      if (aliases->Get(context, i).ToLocal(&alias) && alias->IsObject() &&
          IsJsRuntimeObject(alias.As<Object>())) {
        alias.As<Object>()->SetInternalField(jsInfoIdx, Undefined(m_isolate));
      }
    }
  }
}

jclass ObjectManager::JAVA_LANG_CLASS = nullptr;
jmethodID ObjectManager::GET_NAME_METHOD_ID = nullptr;
jmethodID ObjectManager::GET_JAVAOBJECT_BY_ID_METHOD_ID = nullptr;
jmethodID ObjectManager::GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID = nullptr;
jmethodID ObjectManager::FIND_JAVA_OBJECT_ID_METHOD_ID = nullptr;
jmethodID ObjectManager::MAKE_INSTANCE_STRONG_METHOD_ID = nullptr;
jmethodID ObjectManager::PROCESS_FINALIZED_BATCH_METHOD_ID = nullptr;
jmethodID ObjectManager::RELEASE_NATIVE_INSTANCE_METHOD_ID = nullptr;
