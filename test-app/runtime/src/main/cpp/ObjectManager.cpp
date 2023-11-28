#include "ObjectManager.h"
#include "NativeScriptAssert.h"
#include "MetadataNode.h"
#include "ArgConverter.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "include/v8.h"
#include <algorithm>
#include <sstream>
#include "ManualInstrumentation.h"

using namespace v8;
using namespace std;
using namespace tns;

ObjectManager::ObjectManager(jobject javaRuntimeObject) :
        m_javaRuntimeObject(javaRuntimeObject),
        m_numberOfGC(0),
        m_currentObjectId(0),
        m_cache(NewWeakGlobalRefCallback, DeleteWeakGlobalRefCallback, 1000, this) {

    JEnv env;
    auto runtimeClass = env.FindClass("com/tns/Runtime");
    assert(runtimeClass != nullptr);

    GET_JAVAOBJECT_BY_ID_METHOD_ID = env.GetMethodID(runtimeClass, "getJavaObjectByID",
                                                       "(I)Ljava/lang/Object;");
    assert(GET_JAVAOBJECT_BY_ID_METHOD_ID != nullptr);

    GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID = env.GetMethodID(runtimeClass,
                                                               "getOrCreateJavaObjectID",
                                                               "(Ljava/lang/Object;)I");
    assert(GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID != nullptr);

    MAKE_INSTANCE_WEAK_BATCH_METHOD_ID = env.GetMethodID(runtimeClass, "makeInstanceWeak",
                                                           "(Ljava/nio/ByteBuffer;IZ)V");
    assert(MAKE_INSTANCE_WEAK_BATCH_METHOD_ID != nullptr);

    MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID = env.GetMethodID(runtimeClass,
                                                                        "makeInstanceWeakAndCheckIfAlive",
                                                                        "(I)Z");
    assert(MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID != nullptr);

    RELEASE_NATIVE_INSTANCE_METHOD_ID = env.GetMethodID(runtimeClass, "releaseNativeCounterpart",
                                                          "(I)V");
    assert(RELEASE_NATIVE_INSTANCE_METHOD_ID != nullptr);

    CHECK_WEAK_OBJECTS_ARE_ALIVE_METHOD_ID = env.GetMethodID(runtimeClass,
                                                               "checkWeakObjectAreAlive",
                                                               "(Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;I)V");
    assert(CHECK_WEAK_OBJECTS_ARE_ALIVE_METHOD_ID != nullptr);

    JAVA_LANG_CLASS = env.FindClass("java/lang/Class");
    assert(JAVA_LANG_CLASS != nullptr);

    GET_NAME_METHOD_ID = env.GetMethodID(JAVA_LANG_CLASS, "getName", "()Ljava/lang/String;");
    assert(GET_NAME_METHOD_ID != nullptr);

    auto useGlobalRefsMethodID = env.GetStaticMethodID(runtimeClass, "useGlobalRefs", "()Z");
    assert(useGlobalRefsMethodID != nullptr);

    auto useGlobalRefs = env.CallStaticBooleanMethod(runtimeClass, useGlobalRefsMethodID);
    m_useGlobalRefs = useGlobalRefs == JNI_TRUE;

    auto getMarkingModeOrdinalMethodID = env.GetMethodID(runtimeClass, "getMarkingModeOrdinal",
                                                           "()I");
    jint markingMode = env.CallIntMethod(m_javaRuntimeObject, getMarkingModeOrdinalMethodID);
    m_markingMode = static_cast<JavaScriptMarkingMode>(markingMode);
}

void ObjectManager::SetInstanceIsolate(Isolate *isolate) {
    m_isolate = isolate;
}

void ObjectManager::Init(Isolate *isolate) {
    auto jsWrapperFuncTemplate = FunctionTemplate::New(isolate, JSWrapperConstructorCallback);
    jsWrapperFuncTemplate->InstanceTemplate()->SetInternalFieldCount(
            static_cast<int>(MetadataNodeKeys::END));
    auto context = isolate->GetCurrentContext();
    auto jsWrapperFunc = jsWrapperFuncTemplate->GetFunction(context).ToLocalChecked();
    m_poJsWrapperFunc = new Persistent<Function>(isolate, jsWrapperFunc);
}


JniLocalRef ObjectManager::GetJavaObjectByJsObject(const Local<Object> &object) {
    JSInstanceInfo *jsInstanceInfo = GetJSInstanceInfo(object);

    if (jsInstanceInfo != nullptr) {
        if (m_useGlobalRefs) {
            JniLocalRef javaObject(GetJavaObjectByID(jsInstanceInfo->JavaObjectID), true);
            return javaObject;
        } else {
            JEnv env;
            JniLocalRef javaObject(
                    env.NewLocalRef(GetJavaObjectByID(jsInstanceInfo->JavaObjectID)));
            return javaObject;
        }
    }

    return JniLocalRef();
}

ObjectManager::JSInstanceInfo *ObjectManager::GetJSInstanceInfo(const Local<Object> &object) {
    JSInstanceInfo *jsInstanceInfo = nullptr;
    if (IsJsRuntimeObject(object)) {
        return GetJSInstanceInfoFromRuntimeObject(object);
    }
    return nullptr;
}

ObjectManager::JSInstanceInfo *
ObjectManager::GetJSInstanceInfoFromRuntimeObject(const Local<Object> &object) {
    HandleScope handleScope(m_isolate);

    const int jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
    auto jsInfo = object->GetInternalField(jsInfoIdx);
    if (jsInfo->IsUndefined()) {
        //Typescript object layout has an object instance as child of the actual registered instance. checking for that
        auto prototypeObject = object->GetPrototype().As<Object>();

        if (!prototypeObject.IsEmpty() && prototypeObject->IsObject()) {
            DEBUG_WRITE("GetJSInstanceInfo: need to check prototype :%d",
                        prototypeObject->GetIdentityHash());
            if (IsJsRuntimeObject(prototypeObject)) {
                jsInfo = prototypeObject->GetInternalField(jsInfoIdx);
            }
        }
    }

    if (!jsInfo.IsEmpty() && jsInfo->IsExternal()) {
        auto external = jsInfo.As<External>();
        return static_cast<JSInstanceInfo *>(external->Value());
    }

    return nullptr;
}

bool ObjectManager::IsJsRuntimeObject(const v8::Local<v8::Object> &object) {
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
    jobject object = env.CallObjectMethod(m_javaRuntimeObject, GET_JAVAOBJECT_BY_ID_METHOD_ID,
                                            javaObjectID);
    return object;
}

void ObjectManager::UpdateCache(int objectID, jobject obj) {
    m_cache.update(objectID, obj);
}

jclass ObjectManager::GetJavaClass(const Local<Object> &instance) {
    DEBUG_WRITE("GetClass called");

    JSInstanceInfo *jsInfo = GetJSInstanceInfo(instance);
    jclass clazz = jsInfo->ObjectClazz;

    return clazz;
}

void ObjectManager::SetJavaClass(const Local<Object> &instance, jclass clazz) {
    JSInstanceInfo *jsInfo = GetJSInstanceInfo(instance);
    jsInfo->ObjectClazz = clazz;
}

int ObjectManager::GetOrCreateObjectId(jobject object) {
    JEnv env;
    jint javaObjectID = env.CallIntMethod(m_javaRuntimeObject,
                                            GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID, object);

    return javaObjectID;
}

Local<Object> ObjectManager::GetJsObjectByJavaObject(int javaObjectID) {
    auto isolate = m_isolate;
    EscapableHandleScope handleScope(isolate);

    auto it = m_idToObject.find(javaObjectID);
    if (it == m_idToObject.end()) {
        return handleScope.Escape(Local<Object>());
    }

    Persistent<Object> *jsObject = it->second;

    auto localObject = Local<Object>::New(isolate, *jsObject);
    return handleScope.Escape(localObject);
}


Local<Object> ObjectManager::CreateJSWrapper(jint javaObjectID, const string &typeName) {
    return CreateJSWrapperHelper(javaObjectID, typeName, nullptr);
}

Local<Object>
ObjectManager::CreateJSWrapper(jint javaObjectID, const string &typeName, jobject instance) {
    JEnv env;
    JniLocalRef clazz(env.GetObjectClass(instance));

    return CreateJSWrapperHelper(javaObjectID, typeName, clazz);
}

Local<Object>
ObjectManager::CreateJSWrapperHelper(jint javaObjectID, const string &typeName, jclass clazz) {
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
void ObjectManager::Link(const Local<Object> &object, uint32_t javaObjectID, jclass clazz) {
    if (!IsJsRuntimeObject(object)) {
        string errMsg("Trying to link invalid 'this' to a Java object");
        throw NativeScriptException(errMsg);
    }

    auto isolate = m_isolate;

    DEBUG_WRITE("Linking js object: %d and java instance id: %d", object->GetIdentityHash(),
                javaObjectID);

    auto jsInstanceInfo = new JSInstanceInfo(false/*isJavaObjWeak*/, javaObjectID, clazz);

    auto objectHandle = new Persistent<Object>(isolate, object);
    auto state = new ObjectWeakCallbackState(this, jsInstanceInfo, objectHandle);

    // subscribe for JS GC event
    if (m_markingMode == JavaScriptMarkingMode::None) {
        objectHandle->SetWeak(state, JSObjectFinalizerStatic, WeakCallbackType::kFinalizer);
    } else {
        objectHandle->SetWeak(state, JSObjectWeakCallbackStatic, WeakCallbackType::kFinalizer);
    }

    auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);

    auto jsInfo = External::New(isolate, jsInstanceInfo);

    //link
    object->SetInternalField(jsInfoIdx, jsInfo);

    m_idToObject.emplace(javaObjectID, objectHandle);
}

bool ObjectManager::CloneLink(const Local<Object> &src, const Local<Object> &dest) {
    auto jsInfo = GetJSInstanceInfo(src);

    auto success = jsInfo != nullptr;

    if (success) {
        auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
        auto jsInfo = src->GetInternalField(jsInfoIdx);
        dest->SetInternalField(jsInfoIdx, jsInfo);
    }

    return success;
}

string ObjectManager::GetClassName(jobject javaObject) {
    JEnv env;
    JniLocalRef objectClass(env.GetObjectClass(javaObject));

    return GetClassName((jclass) objectClass);
}

string ObjectManager::GetClassName(jclass clazz) {
    JEnv env;
    JniLocalRef javaCanonicalName(env.CallObjectMethod(clazz, GET_NAME_METHOD_ID));

    string className = ArgConverter::jstringToString(javaCanonicalName);

    std::replace(className.begin(), className.end(), '.', '/');

    return className;
}

void
ObjectManager::JSObjectWeakCallbackStatic(const WeakCallbackInfo<ObjectWeakCallbackState> &data) {
    ObjectWeakCallbackState *callbackState = data.GetParameter();

    ObjectManager *thisPtr = callbackState->thisPtr;

    auto isolate = data.GetIsolate();

    thisPtr->JSObjectWeakCallback(isolate, callbackState);
}

void ObjectManager::JSObjectFinalizerStatic(const WeakCallbackInfo<ObjectWeakCallbackState> &data) {
    ObjectWeakCallbackState *callbackState = data.GetParameter();

    ObjectManager *thisPtr = callbackState->thisPtr;

    auto isolate = data.GetIsolate();

    thisPtr->JSObjectFinalizer(isolate, callbackState);
}

void ObjectManager::JSObjectFinalizer(Isolate *isolate, ObjectWeakCallbackState *callbackState) {
    HandleScope handleScope(m_isolate);
    Persistent<Object> *po = callbackState->target;
    auto jsInstanceInfo = GetJSInstanceInfoFromRuntimeObject(po->Get(m_isolate));

    if (jsInstanceInfo == nullptr) {
        po->Reset();
        delete po;
        delete callbackState;
        return;
    }

    auto javaObjectID = jsInstanceInfo->JavaObjectID;
    JEnv env;
    jboolean isJavaInstanceAlive = env.CallBooleanMethod(m_javaRuntimeObject,
                                                           MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID,
                                                           javaObjectID);
    if (isJavaInstanceAlive) {
        // If the Java instance is alive, keep the JavaScript instance alive.
        po->SetWeak(callbackState, JSObjectFinalizerStatic, WeakCallbackType::kFinalizer);
    } else {
        // If the Java instance is dead, this JavaScript instance can be let die.
        delete jsInstanceInfo;
        auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
        po->Get(m_isolate)->SetInternalField(jsInfoIdx, Undefined(m_isolate));
        po->Reset();
        m_idToObject.erase(javaObjectID);
        delete po;
        delete callbackState;
    }
}


/*
 * When JS GC happens change state of the java counterpart to mirror state of JS object and REVIVE the JS object unconditionally
 * "Regular" js objects are pushed into the "regular objects" array
 * "Callback" js objects (ones that have implementation object) are pushed into two "special objects" array:
 * 		-ones called for the first time which are originally strong in java
 * 		-ones called for the second or next time which are already weak in java too
 *	These objects are categorized by "regular" and "callback" and saved in different arrays for performance optimizations during GC
 * */
void ObjectManager::JSObjectWeakCallback(Isolate *isolate, ObjectWeakCallbackState *callbackState) {
    HandleScope handleScope(isolate);

    Persistent<Object> *po = callbackState->target;

    auto itFound = m_visitedPOs.find(po);

    if (itFound == m_visitedPOs.end()) {
        m_visitedPOs.insert(po);

        auto obj = Local<Object>::New(isolate, *po);
        JSInstanceInfo *jsInstanceInfo = GetJSInstanceInfo(obj);

        if(jsInstanceInfo != nullptr){
            int javaObjectID = jsInstanceInfo->JavaObjectID;

            bool hasImplObj = HasImplObject(isolate, obj);

            DEBUG_WRITE("JSObjectWeakCallback objectId: %d, hasImplObj=%d", javaObjectID, hasImplObj);

            if (hasImplObj) {
                if (jsInstanceInfo->IsJavaObjectWeak) {
                    m_implObjWeak.emplace_back(po, javaObjectID);
                } else {
                    m_implObjStrong.emplace(javaObjectID, po);
                    jsInstanceInfo->IsJavaObjectWeak = true;
                }
            } else {
                if(m_markedForGC.empty()){
                    // Emulates the behavior in the OnGcStarted callback. Тhis is necessary as the hooking
                    // to the V8 GC is done only on the markSweepCompact phase. The JSObjectWeakCallback
                    // however is still triggered in other V8 GC phases (scavenger for example). This
                    // creates a problem that there is no 'top' on the m_markedForGC stack.
                    GarbageCollectionInfo gcInfo(++m_numberOfGC);
                    gcInfo.markedForGC.push_back(po);
                    m_markedForGC.push(gcInfo);
                } else {
                    auto &topGCInfo = m_markedForGC.top();
                    topGCInfo.markedForGC.push_back(po);
                }
            }
        }
    }

    po->SetWeak(callbackState, JSObjectWeakCallbackStatic, WeakCallbackType::kFinalizer);
}

int ObjectManager::GenerateNewObjectID() {
    const int one = 1;
    int oldValue = __sync_fetch_and_add(&m_currentObjectId, one);

    return oldValue;
}

void ObjectManager::ReleaseJSInstance(Persistent<Object> *po, JSInstanceInfo *jsInstanceInfo) {
    int javaObjectID = jsInstanceInfo->JavaObjectID;

    auto it = m_idToObject.find(javaObjectID);

    if (it == m_idToObject.end()) {
        stringstream ss;
        ss << "(InternalError): Js object with id: " << javaObjectID << " not found";
        throw NativeScriptException(ss.str());
    }

    assert(po == it->second);

    m_idToObject.erase(it);
    m_released.insert(po, javaObjectID);
    po->Reset();

    delete po;
    delete jsInstanceInfo;

    DEBUG_WRITE("ReleaseJSObject instance disposed. id:%d", javaObjectID);
}

/*
 * The "regular" JS objects added on ObjectManager::JSObjectWeakCallback are dealt with(released) here.
 * */
void ObjectManager::ReleaseRegularObjects() {
    TNSPERF();

    HandleScope handleScope(m_isolate);

    auto propName = String::NewFromUtf8(m_isolate, "t::gcNum",
                                        NewStringType::kNormal).ToLocalChecked();

    auto &topGCInfo = m_markedForGC.top();
    auto &marked = topGCInfo.markedForGC;
    int numberOfGC = topGCInfo.numberOfGC;

    for (auto po : marked) {
        if (m_released.contains(po)) {
            continue;
        }

        auto obj = Local<Object>::New(m_isolate, *po);

        assert(!obj.IsEmpty());

        Local<Value> gcNum;
        V8GetPrivateValue(m_isolate, obj, propName, gcNum);

        bool isReachableFromImplementationObject = false;

        if (!gcNum.IsEmpty() && gcNum->IsNumber()) {
            double objGcNum = gcNum.As<Number>()->Value();

            // done so we can release only java objects from this GC stack and pass all objects that will be released in parent GC stacks
            isReachableFromImplementationObject = objGcNum >= numberOfGC;
        }

        JSInstanceInfo *jsInstanceInfo = GetJSInstanceInfo(obj);

        if (!isReachableFromImplementationObject) {
            if (!jsInstanceInfo->IsJavaObjectWeak) {
                jsInstanceInfo->IsJavaObjectWeak = true;

                ReleaseJSInstance(po, jsInstanceInfo);
            }
        }
    }

    marked.clear();
}

bool ObjectManager::HasImplObject(Isolate *isolate, const Local<Object> &obj) {
    auto implObject = MetadataNode::GetImplementationObject(isolate, obj);

    bool hasImplObj = !implObject.IsEmpty();

    return hasImplObj;
}

jweak ObjectManager::NewWeakGlobalRefCallback(const int &javaObjectID, void *state) {
    auto objManager = reinterpret_cast<ObjectManager *>(state);

    JniLocalRef obj(objManager->GetJavaObjectByIDImpl(javaObjectID));

    JEnv env;
    jweak weakRef = env.NewWeakGlobalRef(obj);

    return weakRef;
}

void ObjectManager::DeleteWeakGlobalRefCallback(const jweak &object, void *state) {
    auto objManager = reinterpret_cast<ObjectManager *>(state);

    JEnv env;
    env.DeleteWeakGlobalRef(object);
}

Local<Object> ObjectManager::GetEmptyObject(Isolate *isolate) {
    auto emptyObjCtorFunc = Local<Function>::New(isolate, *m_poJsWrapperFunc);
    auto context = Runtime::GetRuntime(isolate)->GetContext();
    auto val = emptyObjCtorFunc->CallAsConstructor(context, 0, nullptr);
    if (val.IsEmpty()) {
        return Local<Object>();
    }
    auto localVal = val.ToLocalChecked();
    assert(localVal->IsObject());
    auto obj = localVal.As<Object>();
    return obj;
}

void ObjectManager::JSWrapperConstructorCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
    assert(info.IsConstructCall());
}

void ObjectManager::ReleaseNativeCounterpart(v8::Local<v8::Object> &object) {

    if(!object->IsObject()){
        throw NativeScriptException("Argument is not an object!");
    }

    JSInstanceInfo *jsInstanceInfo = GetJSInstanceInfo(object);

    if(jsInstanceInfo == nullptr){
        throw NativeScriptException("Trying to release a non native object!");
    }

    JEnv env;
    env.CallVoidMethod(m_javaRuntimeObject, RELEASE_NATIVE_INSTANCE_METHOD_ID,
                         jsInstanceInfo->JavaObjectID);

    delete jsInstanceInfo;
    auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
    object->SetInternalField(jsInfoIdx, Undefined(m_isolate));
}

ObjectManager::JavaScriptMarkingMode ObjectManager::GetMarkingMode() {
    return this->m_markingMode;
}