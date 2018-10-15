#include "ObjectManager.h"
#include "NativeScriptAssert.h"
#include "MetadataNode.h"
#include "ArgConverter.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8NativeScriptExtension.h"
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
    m_env(JEnv()),
    m_numberOfGC(0),
    m_currentObjectId(0),
    m_cache(NewWeakGlobalRefCallback, DeleteWeakGlobalRefCallback, 1000, this) {

    auto runtimeClass = m_env.FindClass("com/tns/Runtime");
    assert(runtimeClass != nullptr);

    GET_JAVAOBJECT_BY_ID_METHOD_ID = m_env.GetMethodID(runtimeClass, "getJavaObjectByID", "(I)Ljava/lang/Object;");
    assert(GET_JAVAOBJECT_BY_ID_METHOD_ID != nullptr);

    GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID = m_env.GetMethodID(runtimeClass, "getOrCreateJavaObjectID", "(Ljava/lang/Object;)I");
    assert(GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID != nullptr);

    MAKE_INSTANCE_WEAK_BATCH_METHOD_ID = m_env.GetMethodID(runtimeClass, "makeInstanceWeak", "(Ljava/nio/ByteBuffer;IZ)V");
    assert(MAKE_INSTANCE_WEAK_BATCH_METHOD_ID != nullptr);

    MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID = m_env.GetMethodID(runtimeClass, "makeInstanceWeakAndCheckIfAlive", "(I)Z");
    assert(MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID != nullptr);

    CHECK_WEAK_OBJECTS_ARE_ALIVE_METHOD_ID = m_env.GetMethodID(runtimeClass, "checkWeakObjectAreAlive", "(Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;I)V");
    assert(CHECK_WEAK_OBJECTS_ARE_ALIVE_METHOD_ID != nullptr);

    JAVA_LANG_CLASS = m_env.FindClass("java/lang/Class");
    assert(JAVA_LANG_CLASS != nullptr);

    GET_NAME_METHOD_ID = m_env.GetMethodID(JAVA_LANG_CLASS, "getName", "()Ljava/lang/String;");
    assert(GET_NAME_METHOD_ID != nullptr);

    auto useGlobalRefsMethodID = m_env.GetStaticMethodID(runtimeClass, "useGlobalRefs", "()Z");
    assert(useGlobalRefsMethodID != nullptr);

    auto useGlobalRefs = m_env.CallStaticBooleanMethod(runtimeClass, useGlobalRefsMethodID);
    m_useGlobalRefs = useGlobalRefs == JNI_TRUE;

    auto getMarkingModeOrdinalMethodID = m_env.GetMethodID(runtimeClass, "getMarkingModeOrdinal", "()I");
    jint markingMode = m_env.CallIntMethod(m_javaRuntimeObject, getMarkingModeOrdinalMethodID);
    m_markingMode = static_cast<JavaScriptMarkingMode>(markingMode);
}

void ObjectManager::SetInstanceIsolate(Isolate* isolate) {
    m_isolate = isolate;
}

void ObjectManager::Init(Isolate* isolate) {
    auto jsWrapperFuncTemplate = FunctionTemplate::New(isolate, JSWrapperConstructorCallback);
    jsWrapperFuncTemplate->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(MetadataNodeKeys::END));
    auto jsWrapperFunc = jsWrapperFuncTemplate->GetFunction();
    m_poJsWrapperFunc = new Persistent<Function>(isolate, jsWrapperFunc);

    if (m_markingMode != JavaScriptMarkingMode::None) {
        isolate->AddGCPrologueCallback(ObjectManager::OnGcStartedStatic, kGCTypeAll);
        isolate->AddGCEpilogueCallback(ObjectManager::OnGcFinishedStatic, kGCTypeAll);
    }
}


JniLocalRef ObjectManager::GetJavaObjectByJsObject(const Local<Object>& object) {
    JSInstanceInfo* jsInstanceInfo = GetJSInstanceInfo(object);

    if (jsInstanceInfo != nullptr) {
        if (m_useGlobalRefs) {
            JniLocalRef javaObject(GetJavaObjectByID(jsInstanceInfo->JavaObjectID), true);
            return javaObject;
        } else {
            JniLocalRef javaObject(m_env.NewLocalRef(GetJavaObjectByID(jsInstanceInfo->JavaObjectID)));
            return javaObject;
        }
    }

    return JniLocalRef();
}

ObjectManager::JSInstanceInfo* ObjectManager::GetJSInstanceInfo(const Local<Object>& object) {
    JSInstanceInfo* jsInstanceInfo = nullptr;
    if (IsJsRuntimeObject(object)) {
        return GetJSInstanceInfoFromRuntimeObject(object);
    }
    return nullptr;
}

ObjectManager::JSInstanceInfo* ObjectManager::GetJSInstanceInfoFromRuntimeObject(const Local<Object>& object) {
    HandleScope handleScope(m_isolate);

    const int jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
    auto jsInfo = object->GetInternalField(jsInfoIdx);
    if (jsInfo->IsUndefined()) {
        //Typescript object layout has an object instance as child of the actual registered instance. checking for that
        auto prototypeObject = object->GetPrototype().As<Object>();

        if (!prototypeObject.IsEmpty() && prototypeObject->IsObject()) {
            DEBUG_WRITE("GetJSInstanceInfo: need to check prototype :%d", prototypeObject->GetIdentityHash());
            if (IsJsRuntimeObject(prototypeObject)) {
                jsInfo = prototypeObject->GetInternalField(jsInfoIdx);
            }
        }
    }

    if (!jsInfo.IsEmpty() && jsInfo->IsExternal()) {
        auto external = jsInfo.As<External>();
        return static_cast<JSInstanceInfo*>(external->Value());
    }

    return nullptr;
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
    jobject object = m_env.CallObjectMethod(m_javaRuntimeObject, GET_JAVAOBJECT_BY_ID_METHOD_ID, javaObjectID);
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
    jint javaObjectID = m_env.CallIntMethod(m_javaRuntimeObject, GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID, object);

    return javaObjectID;
}

Local<Object> ObjectManager::GetJsObjectByJavaObject(int javaObjectID) {
    auto isolate = m_isolate;
    EscapableHandleScope handleScope(isolate);

    auto it = m_idToObject.find(javaObjectID);
    if (it == m_idToObject.end()) {
        return handleScope.Escape(Local<Object>());
    }

    Persistent<Object>* jsObject = it->second;

    auto localObject = Local<Object>::New(isolate, *jsObject);
    return handleScope.Escape(localObject);
}


Local<Object> ObjectManager::CreateJSWrapper(jint javaObjectID, const string& typeName) {
    return CreateJSWrapperHelper(javaObjectID, typeName, nullptr);
}

Local<Object> ObjectManager::CreateJSWrapper(jint javaObjectID, const string& typeName, jobject instance) {
    JniLocalRef clazz(m_env.GetObjectClass(instance));

    return CreateJSWrapperHelper(javaObjectID, typeName, clazz);
}

Local<Object> ObjectManager::CreateJSWrapperHelper(jint javaObjectID, const string& typeName, jclass clazz) {
    auto isolate = m_isolate;

    auto className = (clazz != nullptr) ? GetClassName(clazz) : typeName;

    auto node = MetadataNode::GetOrCreate(className);

    auto jsWrapper = node->CreateJSWrapper(isolate, this);

    if (!jsWrapper.IsEmpty()) {
        auto claz = m_env.FindClass(className);
        Link(jsWrapper, javaObjectID, claz);
    }
    return jsWrapper;
}


/* *
 * Link the JavaScript object and it's java counterpart with an ID
 */
void ObjectManager::Link(const Local<Object>& object, uint32_t javaObjectID, jclass clazz) {
    if (!IsJsRuntimeObject(object)) {
        string errMsg("Trying to link invalid 'this' to a Java object");
        throw NativeScriptException(errMsg);
    }

    auto isolate = m_isolate;

    DEBUG_WRITE("Linking js object: %d and java instance id: %d", object->GetIdentityHash(), javaObjectID);

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

    m_idToObject.insert(make_pair(javaObjectID, objectHandle));
}

bool ObjectManager::CloneLink(const Local<Object>& src, const Local<Object>& dest) {
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
    JniLocalRef objectClass(m_env.GetObjectClass(javaObject));

    return GetClassName((jclass) objectClass);
}

string ObjectManager::GetClassName(jclass clazz) {
    JniLocalRef javaCanonicalName(m_env.CallObjectMethod(clazz, GET_NAME_METHOD_ID));

    string className = ArgConverter::jstringToString(javaCanonicalName);

    std::replace(className.begin(), className.end(), '.', '/');

    return className;
}

void ObjectManager::JSObjectWeakCallbackStatic(const WeakCallbackInfo<ObjectWeakCallbackState>& data) {
    ObjectWeakCallbackState* callbackState = data.GetParameter();

    ObjectManager* thisPtr = callbackState->thisPtr;

    auto isolate = data.GetIsolate();

    thisPtr->JSObjectWeakCallback(isolate, callbackState);
}

void ObjectManager::JSObjectFinalizerStatic(const WeakCallbackInfo<ObjectWeakCallbackState>& data) {
    ObjectWeakCallbackState* callbackState = data.GetParameter();

    ObjectManager* thisPtr = callbackState->thisPtr;

    auto isolate = data.GetIsolate();

    thisPtr->JSObjectFinalizer(isolate, callbackState);
}

void ObjectManager::JSObjectFinalizer(Isolate* isolate, ObjectWeakCallbackState* callbackState) {
    HandleScope handleScope(m_isolate);
    Persistent<Object>* po = callbackState->target;
    auto jsInstanceInfo = GetJSInstanceInfoFromRuntimeObject(po->Get(m_isolate));

    if (jsInstanceInfo == nullptr) {
        po->Reset();
        return;
    }

    auto javaObjectID = jsInstanceInfo->JavaObjectID;
    jboolean isJavaInstanceAlive = m_env.CallBooleanMethod(m_javaRuntimeObject, MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID, javaObjectID);
    if (isJavaInstanceAlive) {
        // If the Java instance is alive, keep the JavaScript instance alive.
        po->SetWeak(callbackState, JSObjectFinalizerStatic, WeakCallbackType::kFinalizer);
    } else {
        // If the Java instance is dead, this JavaScript instance can be let die.
        delete jsInstanceInfo;
        auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
        po->Get(m_isolate)->SetInternalField(jsInfoIdx, Undefined(m_isolate));
        po->Reset();
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
void ObjectManager::JSObjectWeakCallback(Isolate* isolate, ObjectWeakCallbackState* callbackState) {
    HandleScope handleScope(isolate);

    Persistent<Object>* po = callbackState->target;

    auto itFound = m_visitedPOs.find(po);

    if (itFound == m_visitedPOs.end()) {
        m_visitedPOs.insert(po);

        auto obj = Local<Object>::New(isolate, *po);
        JSInstanceInfo* jsInstanceInfo = GetJSInstanceInfo(obj);
        int javaObjectID = jsInstanceInfo->JavaObjectID;

        bool hasImplObj = HasImplObject(isolate, obj);

        DEBUG_WRITE("JSObjectWeakCallback objectId: %d, hasImplObj=%d", javaObjectID, hasImplObj);

        if (hasImplObj) {
            if (jsInstanceInfo->IsJavaObjectWeak) {
                m_implObjWeak.push_back(PersistentObjectIdPair(po, javaObjectID));
            } else {
                m_implObjStrong.insert(make_pair(javaObjectID, po));
                jsInstanceInfo->IsJavaObjectWeak = true;
            }
        } else {
            assert(!m_markedForGC.empty());
            auto& topGCInfo = m_markedForGC.top();
            topGCInfo.markedForGC.push_back(po);
        }
    }

    po->SetWeak(callbackState, JSObjectWeakCallbackStatic, WeakCallbackType::kFinalizer);
}

int ObjectManager::GenerateNewObjectID() {
    const int one = 1;
    int oldValue = __sync_fetch_and_add(&m_currentObjectId, one);

    return oldValue;
}

void ObjectManager::ReleaseJSInstance(Persistent<Object>* po, JSInstanceInfo* jsInstanceInfo) {
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

    auto propName = String::NewFromUtf8(m_isolate, "t::gcNum", NewStringType::kNormal).ToLocalChecked();

    auto& topGCInfo = m_markedForGC.top();
    auto& marked = topGCInfo.markedForGC;
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

        if (!gcNum.IsEmpty()) {
            int objGcNum = gcNum->Int32Value();

            // done so we can release only java objects from this GC stack and pass all objects that will be released in parent GC stacks
            isReachableFromImplementationObject = objGcNum >= numberOfGC;
        }

        JSInstanceInfo* jsInstanceInfo = GetJSInstanceInfo(obj);

        if (!isReachableFromImplementationObject) {
            if (!jsInstanceInfo->IsJavaObjectWeak) {
                jsInstanceInfo->IsJavaObjectWeak = true;

                ReleaseJSInstance(po, jsInstanceInfo);
            }
        }
    }

    marked.clear();
}

bool ObjectManager::HasImplObject(Isolate* isolate, const Local<Object>& obj) {
    auto implObject = MetadataNode::GetImplementationObject(isolate, obj);

    bool hasImplObj = !implObject.IsEmpty();

    return hasImplObj;
}

/*
 * When "MarkReachableObjects" is called V8 has marked all JS objects that can be released.
 * This method builds on top of V8s marking phase, because we need to consider the JAVA counterpart objects (is it "regular" or "callback"), when marking JS ones.
 * */
void ObjectManager::MarkReachableObjects(Isolate* isolate, const Local<Object>& obj) {
    tns::instrumentation::Frame frame;

    stack<Local<Value>> s;

    s.push(obj);

    auto propName = String::NewFromUtf8(isolate, "t::gcNum");

    assert(!m_markedForGC.empty());
    auto& topGCInfo = m_markedForGC.top();
    int numberOfGC = topGCInfo.numberOfGC;
    auto fromJsInfo = GetJSInstanceInfo(obj);
    auto fromId = fromJsInfo->JavaObjectID;

    auto curGCNumValue = Integer::New(isolate, numberOfGC);
    bool firstRun = true;

    while (!s.empty()) {
        auto isInFirstRun = firstRun;
        firstRun = false;

        auto top = s.top();
        s.pop();

        if (top.IsEmpty() || !top->IsObject()) {
            continue;
        }

        auto o = top.As<Object>();
        if(!isInFirstRun) {
            uint8_t* addr = NativeScriptExtension::GetAddress(o);
            auto itFound = m_visited.find(addr);
            if (itFound != m_visited.end()) {
                continue;
            }
            m_visited.insert(addr); // set as processed only if the current object is not the object we are starting from
        }

        auto jsInfo = GetJSInstanceInfo(o);
        if ((jsInfo != nullptr) && (jsInfo->JavaObjectID != fromId)) {
            auto hasImplObject = HasImplObject(isolate, o);
            if (hasImplObject) {
                // this is a special case when one "callback1" object (one we are currently traversing)
                // can reach another "callback2" object (jsInfo->JavaObjectID)
                // here we are leaving "callback2" object to remain strong in java
                m_implObjStrong[jsInfo->JavaObjectID] = nullptr;
            }

            V8SetPrivateValue(isolate, o, propName, curGCNumValue);
        }

        if (o->IsFunction()) {
            auto func = o.As<Function>();

            int closureObjectLength;
            auto closureObjects = NativeScriptExtension::GetClosureObjects(isolate, func, &closureObjectLength);
            for (int i = 0; i < closureObjectLength; i++) {
                auto& curV = *(closureObjects + i);
                if (!curV.IsEmpty() && curV->IsObject()) {
                    s.push(curV);
                }
            }
            NativeScriptExtension::ReleaseClosureObjects(closureObjects);
        }

        if (o->IsArray()) {
            MarkReachableArrayElements(o, s);
        }

        auto proto = o->GetPrototype();
        if (!proto.IsEmpty() && !proto->IsNull() && !proto->IsUndefined() && proto->IsObject()) {
            s.push(proto);
        }

        auto context = isolate->GetCurrentContext();
        bool success = false;
        auto propNames = NativeScriptExtension::GetPropertyKeys(isolate, context, o, success);
        assert(success);
        int len = propNames->Length();
        for (int i = 0; i < len; i++) {
            auto propName = propNames->Get(i);
            if (propName->IsString()) {
                auto name = propName.As<String>();

                bool isPropDescriptor = o->HasRealNamedCallbackProperty(name);
                if (isPropDescriptor) {
                    Local<Value> getter;
                    Local<Value> setter;
                    NativeScriptExtension::GetAssessorPair(isolate, o, name, getter, setter);

                    if (!getter.IsEmpty() && getter->IsFunction()) {
                        int getterClosureObjectLength = 0;
                        auto getterClosureObjects = NativeScriptExtension::GetClosureObjects(isolate, getter.As<Function>(), &getterClosureObjectLength);
                        for (int i = 0; i < getterClosureObjectLength; i++) {
                            auto& curV = *(getterClosureObjects + i);
                            if (!curV.IsEmpty() && curV->IsObject()) {
                                s.push(curV);
                            }
                        }
                        NativeScriptExtension::ReleaseClosureObjects(getterClosureObjects);
                    }

                    if (!setter.IsEmpty() && setter->IsFunction()) {
                        int setterClosureObjectLength = 0;
                        auto setterClosureObjects = NativeScriptExtension::GetClosureObjects(isolate, setter.As<Function>(), &setterClosureObjectLength);
                        for (int i = 0; i < setterClosureObjectLength; i++) {
                            auto& curV = *(setterClosureObjects + i);
                            if (!curV.IsEmpty() && curV->IsObject()) {
                                s.push(curV);
                            }
                        }

                        NativeScriptExtension::ReleaseClosureObjects(setterClosureObjects);
                    }
                } else {
                    auto prop = o->Get(propName);

                    if (!prop.IsEmpty() && prop->IsObject()) {
                        s.push(prop);
                    }
                }
            }
        } // for

    } // while

    if (frame.check()) {
        auto cls = fromJsInfo->ObjectClazz;
        JniLocalRef className(m_env.CallObjectMethod(cls, GET_NAME_METHOD_ID));
        frame.log("MarkReachableObjects: " + ArgConverter::jstringToString(className));
    }
}

void ObjectManager::MarkReachableArrayElements(Local<Object>& o, stack<Local<Value>>& s) {
    auto arr = o.As<Array>();

    int arrEnclosedObjectsLength = arr->Length();
    for (int i = 0; i < arrEnclosedObjectsLength; i++) {
        auto enclosedElement = arr->Get(i);

        if (!enclosedElement.IsEmpty() && enclosedElement->IsObject()) {
            s.push(enclosedElement);
        }
    }
}

void ObjectManager::OnGcStartedStatic(Isolate* isolate, GCType type, GCCallbackFlags flags) {
    try {
        auto runtime = Runtime::GetRuntime(isolate);
        auto objectManager = runtime->GetObjectManager();
        objectManager->OnGcStarted(type, flags);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void ObjectManager::OnGcFinishedStatic(Isolate* isolate, GCType type, GCCallbackFlags flags) {
    try {
        auto runtime = Runtime::GetRuntime(isolate);
        auto objectManager = runtime->GetObjectManager();
        objectManager->OnGcFinished(type, flags);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void ObjectManager::OnGcStarted(GCType type, GCCallbackFlags flags) {
    TNSPERF();

    GarbageCollectionInfo gcInfo(++m_numberOfGC);
    m_markedForGC.push(gcInfo);
}

/*
 * When GC is called we need to evaluate the situation and decide what js objects to release
 * */
void ObjectManager::OnGcFinished(GCType type, GCCallbackFlags flags) {
    assert(!m_markedForGC.empty());

    //deal with all "callback" objects
    auto isolate = m_isolate;
    for (auto weakObj : m_implObjWeak) {
        auto obj = Local<Object>::New(isolate, *weakObj.po);
        MarkReachableObjects(isolate, obj);
    }
    for (const auto& kv : m_implObjStrong) {
        Persistent<Object>* po = kv.second;
        if (po != nullptr) {
            auto obj = Local<Object>::New(isolate, *po);
            MarkReachableObjects(isolate, obj);
        }
    }

    //deal with regular objects
    ReleaseRegularObjects();

    m_markedForGC.pop();

    if (m_markedForGC.empty()) {
        MakeRegularObjectsWeak(m_released.m_IDs, m_buff);

        MakeImplObjectsWeak(m_implObjStrong, m_buff);

        CheckWeakObjectsAreAlive(m_implObjWeak, m_buff, m_outBuff);

        m_buff.Reset();
        m_released.clear();
        m_visited.clear();
        m_visitedPOs.clear();
        m_implObjWeak.clear();
        m_implObjStrong.clear();
    }
}

/*
 * We have all the JS "regular" objects that JS has made weak and ready to by GC'd,
 * so we tell java to take the JAVA objects out of strong reference so they can be collected by JAVA GC
 * */
void ObjectManager::MakeRegularObjectsWeak(const set<int>& instances, DirectBuffer& inputBuff) {
    jboolean keepAsWeak = JNI_FALSE;

    for (auto javaObjectId : instances) {
        bool success = inputBuff.Write(javaObjectId);

        if (!success) {
            int length = inputBuff.Length();
            m_env.CallVoidMethod(m_javaRuntimeObject, MAKE_INSTANCE_WEAK_BATCH_METHOD_ID, (jobject) inputBuff, length, keepAsWeak);
            inputBuff.Reset();
            success = inputBuff.Write(javaObjectId);
            assert(success);
        }
    }
    int size = inputBuff.Size();
    if (size > 0) {
        m_env.CallVoidMethod(m_javaRuntimeObject, MAKE_INSTANCE_WEAK_BATCH_METHOD_ID, (jobject) inputBuff, size, keepAsWeak);
    }

    inputBuff.Reset();
}

/*
 * We have all the JS "callback" objects that JS has made weak and ready to by GC'd,
 * so we tell java to take the JAVA objects out of strong, BUT KEEP THEM AS WEEK REFERENCES,
 * so that if java needs to release them, it can, on a later stage.
 * */
void ObjectManager::MakeImplObjectsWeak(const map<int, Persistent<Object>*>& instances, DirectBuffer& inputBuff) {
    jboolean keepAsWeak = JNI_TRUE;

    for (const auto& kv : instances) {
        if (kv.second != nullptr) {
            int javaObjectId = kv.first;

            bool success = inputBuff.Write(javaObjectId);

            if (!success) {
                int length = inputBuff.Length();
                jboolean keepAsWeak = JNI_TRUE;
                m_env.CallVoidMethod(m_javaRuntimeObject, MAKE_INSTANCE_WEAK_BATCH_METHOD_ID, (jobject) inputBuff, length, keepAsWeak);
                inputBuff.Reset();
                success = inputBuff.Write(javaObjectId);
                assert(success);
            }
        }
    }
    int size = inputBuff.Size();
    if (size > 0) {
        jboolean keepAsWeak = JNI_TRUE;
        m_env.CallVoidMethod(m_javaRuntimeObject, MAKE_INSTANCE_WEAK_BATCH_METHOD_ID, (jobject) inputBuff, size, keepAsWeak);
    }

    inputBuff.Reset();
}

/*
 * Consult with JAVA world to check if a java object is still in kept as a strong or weak reference
 * If the JAVA objects are released, we can release the their counterpart JS objects
 * */
void ObjectManager::CheckWeakObjectsAreAlive(const vector<PersistentObjectIdPair>& instances, DirectBuffer& inputBuff, DirectBuffer& outputBuff) {
    TNSPERF();

    for (const auto& poIdPair : instances) {
        int javaObjectId = poIdPair.javaObjectId;

        bool success = inputBuff.Write(javaObjectId);

        if (!success) {
            int length = inputBuff.Length();
            m_env.CallVoidMethod(m_javaRuntimeObject, CHECK_WEAK_OBJECTS_ARE_ALIVE_METHOD_ID, (jobject) inputBuff, (jobject) outputBuff, length);
            //
            int* released = outputBuff.GetData();
            for (int i = 0; i < length; i++) {
                bool isReleased = *released++ != 0;

                if (isReleased) {
                    Persistent<Object>* po = instances[i].po;
                    po->Reset();
                }
            }
            //
            inputBuff.Reset();
            success = inputBuff.Write(javaObjectId);
            assert(success);
        }
    }
    int size = inputBuff.Size();
    if (size > 0) {
        m_env.CallVoidMethod(m_javaRuntimeObject, CHECK_WEAK_OBJECTS_ARE_ALIVE_METHOD_ID, (jobject) inputBuff, (jobject) outputBuff, size);
        int* released = outputBuff.GetData();
        for (int i = 0; i < size; i++) {
            bool isReleased = *released++ != 0;

            if (isReleased) {
                Persistent<Object>* po = instances[i].po;
                po->Reset();
            }
        }
    }
}

jweak ObjectManager::NewWeakGlobalRefCallback(const int& javaObjectID, void* state) {
    auto objManager = reinterpret_cast<ObjectManager*>(state);

    JniLocalRef obj(objManager->GetJavaObjectByIDImpl(javaObjectID));

    jweak weakRef = objManager->m_env.NewWeakGlobalRef(obj);

    return weakRef;
}

void ObjectManager::DeleteWeakGlobalRefCallback(const jweak& object, void* state) {
    auto objManager = reinterpret_cast<ObjectManager*>(state);

    objManager->m_env.DeleteWeakGlobalRef(object);
}

Local<Object> ObjectManager::GetEmptyObject(Isolate* isolate) {
    auto emptyObjCtorFunc = Local<Function>::New(isolate, *m_poJsWrapperFunc);
    auto val = emptyObjCtorFunc->CallAsConstructor(isolate->GetCurrentContext(), 0, nullptr);
    if (val.IsEmpty()) {
        return Local<Object>();
    }
    auto localVal = val.ToLocalChecked();
    assert(localVal->IsObject());
    auto obj = localVal.As<Object>();
    return obj;
}

void ObjectManager::JSWrapperConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    assert(info.IsConstructCall());
}
