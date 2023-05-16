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

    MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID = env.GetMethodID(runtimeClass,
                                                                        "makeInstanceWeakAndCheckIfAlive",
                                                                        "(I)Z");
    assert(MAKE_INSTANCE_WEAK_AND_CHECK_IF_ALIVE_METHOD_ID != nullptr);

    RELEASE_NATIVE_INSTANCE_METHOD_ID = env.GetMethodID(runtimeClass, "releaseNativeCounterpart",
                                                          "(I)V");
    assert(RELEASE_NATIVE_INSTANCE_METHOD_ID != nullptr);

    jclass javaLangClass = env.FindClass("java/lang/Class");
    assert(javaLangClass != nullptr);

    GET_NAME_METHOD_ID = env.GetMethodID(javaLangClass, "getName", "()Ljava/lang/String;");
    assert(GET_NAME_METHOD_ID != nullptr);
}

void ObjectManager::Init(Isolate *isolate) {
    m_isolate = isolate;
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
        JniLocalRef javaObject(GetJavaObjectByID(jsInstanceInfo->JavaObjectID), true);
        return javaObject;
    }

    return JniLocalRef();
}

ObjectManager::JSInstanceInfo *ObjectManager::GetJSInstanceInfo(const Local<Object> &object) {
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

jweak ObjectManager::GetJavaObjectByID(jint javaObjectID) {
    jweak obj = m_cache(javaObjectID);

    return obj;
}

jobject ObjectManager::GetJavaObjectByIDImpl(jint javaObjectID) {
    JEnv env;
    jobject object = env.CallObjectMethod(m_javaRuntimeObject, GET_JAVAOBJECT_BY_ID_METHOD_ID,
                                            javaObjectID);
    return object;
}

jint ObjectManager::GetOrCreateObjectId(jobject object) {
    JEnv env;
    jint javaObjectID = env.CallIntMethod(m_javaRuntimeObject,
                                            GET_OR_CREATE_JAVA_OBJECT_ID_METHOD_ID, object);

    return javaObjectID;
}

Local<Object> ObjectManager::GetJsObjectByJavaObject(jint javaObjectID) {
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
        Link(jsWrapper, javaObjectID);
    }
    return jsWrapper;
}


/* *
 * Link the JavaScript object and it's java counterpart with an ID
 */
void ObjectManager::Link(const Local<Object> &object, jint javaObjectID) {
    if (!IsJsRuntimeObject(object)) {
        string errMsg("Trying to link invalid 'this' to a Java object");
        throw NativeScriptException(errMsg);
    }

    auto isolate = m_isolate;

    DEBUG_WRITE("Linking js object: %d and java instance id: %d", object->GetIdentityHash(),
                javaObjectID);

    auto jsInstanceInfo = new JSInstanceInfo(javaObjectID);

    auto objectHandle = new Persistent<Object>(isolate, object);
    auto state = new ObjectWeakCallbackState(this, objectHandle);

    // subscribe for JS GC event
    objectHandle->SetWeak(state, JSObjectFinalizerStatic, WeakCallbackType::kFinalizer);

    auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);

    auto jsInfo = External::New(isolate, jsInstanceInfo);

    //link
    object->SetInternalField(jsInfoIdx, jsInfo);

    m_idToObject.insert(make_pair(javaObjectID, objectHandle));
}

bool ObjectManager::CloneLink(const Local<Object> &src, const Local<Object> &dest) {
    auto jsInfo = GetJSInstanceInfo(src);

    auto success = jsInfo != nullptr;

    if (success) {
        auto jsInfoIdx = static_cast<int>(MetadataNodeKeys::JsInfo);
        // fetches the JSInstanceInfo again, but allows reusing the same v8::External
        Local<Value> jsInfoExternal = src->GetInternalField(jsInfoIdx);
        dest->SetInternalField(jsInfoIdx, jsInfoExternal);
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

jint ObjectManager::GenerateNewObjectID() {
    const jint one = 1;
    jint oldValue = __sync_fetch_and_add(&m_currentObjectId, one);

    return oldValue;
}

jweak ObjectManager::NewWeakGlobalRefCallback(const jint &javaObjectID, void *state) {
    auto objManager = reinterpret_cast<ObjectManager *>(state);

    JniLocalRef obj(objManager->GetJavaObjectByIDImpl(javaObjectID));

    JEnv env;
    jweak weakRef = env.NewWeakGlobalRef(obj);

    return weakRef;
}

void ObjectManager::DeleteWeakGlobalRefCallback(const jweak &object, [[maybe_unused]] void *state) {
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
