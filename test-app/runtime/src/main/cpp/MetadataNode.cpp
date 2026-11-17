#include "MetadataNode.h"
#include "RuntimeState.h"
#include "NativeScriptAssert.h"
#include "Constants.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "ArgConverter.h"
#include "V8StringConstants.h"
#include "SimpleProfiler.h"
#include "CallbackHandlers.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <android/log.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ManualInstrumentation.h"
#include "ExtendFileIdentifier.h"
#include "JSONObjectHelper.h"



#include "v8.h"

using namespace v8;
using namespace std;
using namespace tns;

// Objects that get more members than this are moved to dictionary mode before the members are
// added: adding properties one by one to a fast-mode object copies its descriptors each time.
static constexpr size_t kManyMembers = 64;

// V8 has no API for this; deleting a property other than the last one added does it.
static void MoveToDictionaryMode(Isolate* isolate, Local<Context> context, Local<Object> object) {
    auto first = String::NewFromUtf8Literal(isolate, "__ns_dictionary_a");
    auto second = String::NewFromUtf8Literal(isolate, "__ns_dictionary_b");
    object->Set(context, first, v8::True(isolate)).Check();
    object->Set(context, second, v8::True(isolate)).Check();
    object->Delete(context, first).Check();
    object->Delete(context, second).Check();
}

// For everything but class constructors and static methods: V8 builds a prototype object and
// an instance map for each constructible API function it instantiates, which a method never uses.
static Local<FunctionTemplate> NewNonConstructor(Isolate* isolate, FunctionCallback callback,
                                                 Local<Value> data = Local<Value>()) {
    return FunctionTemplate::New(isolate, callback, data, Local<Signature>(), 0,
                                 ConstructorBehavior::kThrow);
}

// Property names are internalized by V8 anyway; creating them internalized skips a copy.
static Local<String> NewMemberName(Isolate* isolate, std::string_view name) {
    return String::NewFromUtf8(isolate, name.data(), NewStringType::kInternalized,
                               static_cast<int>(name.size())).ToLocalChecked();
}

void MetadataNode::Init(Isolate* isolate) {
    auto key = ArgConverter::ConvertToV8String(isolate, "tns::MetadataKey");
    auto cache = GetMetadataNodeCache(isolate);
    cache->MetadataKey = new Persistent<String>(isolate, key);
    cache->PackageKey = new Persistent<String>(
        isolate, ArgConverter::ConvertToV8String(isolate, "tns::PackageKey"));
}

// Deliberately not V8GetPrivateValue: that one throws when the lookup comes
// back Nothing. Callers here take arbitrary values, so a failed read must also
// leave no exception pending for whoever calls next.
static void* TryReadPrivateExternal(Isolate* isolate, const Local<Object>& value, Persistent<String>* key) {
    if (key == nullptr) {
        return nullptr;
    }

    auto context = isolate->GetCurrentContext();
    if (context.IsEmpty()) {
        return nullptr;
    }

    TryCatch tc(isolate);
    Local<Value> hidden;
    auto privateKey = Private::ForApi(isolate, Local<String>::New(isolate, *key));
    if (!value->GetPrivate(context, privateKey).ToLocal(&hidden) || !hidden->IsExternal()) {
        tc.Reset();
        return nullptr;
    }

    return hidden.As<External>()->Value(v8::kExternalPointerTypeTagDefault);
}

static NativeScriptException ProxyTypeError(Isolate* isolate, const string& message) {
    return NativeScriptException(isolate, Exception::TypeError(ArgConverter::ConvertToV8String(isolate, message)), message);
}

// Reactive frameworks put Java wrappers behind a Proxy, which carries none of
// the wrapper's internal fields or private symbols.
static Local<Object> ResolveProxyReceiver(Isolate* isolate, const Local<Object>& receiver, const char* verb, const string& member, bool requireJavaObject) {
    if (!receiver->IsProxy()) {
        return receiver;
    }

    auto target = ObjectManager::UnwrapProxy(receiver);
    if (!target->IsObject()) {
        throw ProxyTypeError(isolate, string("Cannot ") + verb + " '" + member + "' on a revoked Proxy");
    }

    auto object = target.As<Object>();
    if (requireJavaObject && !ObjectManager::IsJsRuntimeObject(object)) {
        throw ProxyTypeError(isolate, string("Cannot ") + verb + " '" + member + "' on a Proxy whose target is not a Java object");
    }

    return object;
}

static string DottedClassName(string className) {
    std::replace(className.begin(), className.end(), '/', '.');
    return className;
}

static Local<Object> ResolveImplementationObject(Isolate* isolate, const Local<Object>& implementationObject) {
    if (!implementationObject->IsProxy()) {
        return implementationObject;
    }

    auto target = ObjectManager::UnwrapProxy(implementationObject);
    if (!target->IsObject()) {
        throw ProxyTypeError(isolate, "Cannot use a revoked Proxy as an implementation object");
    }

    return target.As<Object>();
}

bool MetadataNode::TryGetInstanceTypeName(Isolate* isolate, const Local<Object>& value, std::string& out) {
    MetadataNode* node;
    if (ObjectManager::HasMetadataField(value)) {
        node = GetInstanceMetadata(isolate, value);
    } else {
        auto cache = GetMetadataNodeCache(isolate);
        node = static_cast<MetadataNode*>(TryReadPrivateExternal(isolate, value, cache->MetadataKey));
    }
    if (node == nullptr) {
        return false;
    }

    out = node->m_name;
    return true;
}

bool MetadataNode::TryGetPackageName(Isolate* isolate, const Local<Object>& value, std::string& out) {
    auto cache = GetMetadataNodeCache(isolate);
    auto node = static_cast<MetadataNode*>(TryReadPrivateExternal(isolate, value, cache->PackageKey));
    if (node == nullptr) {
        return false;
    }

    out = node->m_name;
    return true;
}

Local<ObjectTemplate> MetadataNode::GetOrCreateArrayObjectTemplate(Isolate* isolate) {
    auto cache = GetMetadataNodeCache(isolate);
    if (cache->ArrayObjectTemplate != nullptr) {
        return cache->ArrayObjectTemplate->Get(isolate);
    }

    auto arrayObjectTemplate = ObjectTemplate::New(isolate);
    arrayObjectTemplate->SetInternalFieldCount(static_cast<int>(ObjectManager::MetadataNodeKeys::END));
    arrayObjectTemplate->SetHandler(IndexedPropertyHandlerConfiguration(
            ArrayIndexedPropertyGetterCallback, ArrayIndexedPropertySetterCallback));

    cache->ArrayObjectTemplate = new Persistent<ObjectTemplate>(isolate, arrayObjectTemplate);

    return arrayObjectTemplate;
}

MetadataNode::MetadataNode(MetadataTreeNode* treeNode) :
    m_treeNode(treeNode) {
    uint8_t nodeType = s_metadataReader.GetNodeType(treeNode);

    m_name = s_metadataReader.ReadTypeName(m_treeNode);

    uint8_t parentNodeType = s_metadataReader.GetNodeType(treeNode->parent);

    m_isArray = s_metadataReader.IsNodeTypeArray(parentNodeType);

    m_isInterface = s_metadataReader.IsNodeTypeInterface(nodeType);

    if (!m_isArray && m_isInterface) {
        m_implType = TNS_PREFIX + m_name;
    }
}

Local<Object> MetadataNode::CreateExtendedJSWrapper(Isolate* isolate, ObjectManager* objectManager, const string& proxyClassName) {
    Local<Object> extInstance;

    auto cacheData = GetCachedExtendedClassData(isolate, proxyClassName);
    if (cacheData.node != nullptr) {
        auto cache = GetMetadataNodeCache(isolate);
        if (!cache->SharedExtendedClasses.empty()) {
            auto shared = cache->SharedExtendedClasses.find(proxyClassName);
            if (shared != cache->SharedExtendedClasses.end() && !shared->second) {
                shared->second = true;
                __android_log_print(ANDROID_LOG_WARN, "JS",
                                    "Java created an instance of %s by its class name, but more than one extend() call shares that class; using the implementation of the first one. Give the class a unique name, e.g. with @JavaProxy.",
                                    DottedClassName(proxyClassName).c_str());
            }
        }

        // Starts on the extended class's initial map, with its prototype, and without
        // running its constructor callback.
        auto context = Runtime::GetRuntime(isolate)->GetContext();
        if (cacheData.instanceTemplate->Get(isolate)->NewInstance(context).ToLocal(&extInstance)) {
            // V8 may copy instances from a cached instantiation of the template, which keeps the
            // prototype the class had then. This only matters if code reassigns the extended
            // class's `prototype` property, which nothing in the runtime or core does; consider
            // dropping the check.
            auto extdCtorFunc = Local<Function>::New(isolate, *cacheData.extendedCtorFunction);
            Local<Value> prototype;
            if (extdCtorFunc->Get(context, V8StringConstants::GetPrototype(isolate)).ToLocal(&prototype) &&
                !prototype->StrictEquals(extInstance->GetPrototype())) {
                extInstance->SetPrototype(context, prototype);
            }
            extInstance->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper), True(isolate));
            SetInstanceMetadata(isolate, extInstance, cacheData.node);
        }
    }

    return extInstance;
}

/*
 * The three caches below are process-wide and reached from every runtime's
 * thread -- GetOrCreate runs on every JS-wrapper creation -- so each access is
 * guarded. The lock covers map access only: it is dropped around the metadata
 * reader, which takes its own lock and, for a type absent from the static
 * metadata, calls into Java to resolve it.
 *
 * Losing the race is therefore possible and handled at the insert: the entry
 * already in the map is the one other threads have been handed, so it wins.
 */
MetadataNode* MetadataNode::GetOrCreate(const string& className) {
    {
        std::lock_guard<std::mutex> lock(s_nodeCacheMutex);
        auto it = s_name2NodeCache.find(className);
        if (it != s_name2NodeCache.end()) {
            return it->second;
        }
    }

    MetadataTreeNode* treeNode = GetOrCreateTreeNodeByName(className);
    MetadataNode* node = GetOrCreateInternal(treeNode);

    std::lock_guard<std::mutex> lock(s_nodeCacheMutex);
    return s_name2NodeCache.emplace(className, node).first->second;
}

void MetadataNode::RegisterInterfaceProxyClass(const string& proxyClassName, const string& interfaceName) {
    MetadataNode* node = GetOrCreate(interfaceName);

    std::lock_guard<std::mutex> lock(s_nodeCacheMutex);
    s_name2NodeCache.emplace(proxyClassName, node);
}

MetadataNode* MetadataNode::GetOrCreateInternal(MetadataTreeNode* treeNode) {
    {
        std::lock_guard<std::mutex> lock(s_nodeCacheMutex);
        auto it = s_treeNode2NodeCache.find(treeNode);
        if (it != s_treeNode2NodeCache.end()) {
            return it->second;
        }
    }

    // Constructed outside the lock: the constructor reads type names out of the
    // metadata reader.
    auto* candidate = new MetadataNode(treeNode);

    std::lock_guard<std::mutex> lock(s_nodeCacheMutex);
    auto inserted = s_treeNode2NodeCache.emplace(treeNode, candidate);
    if (!inserted.second) {
        delete candidate;
    }

    return inserted.first->second;
}

MetadataTreeNode* MetadataNode::GetOrCreateTreeNodeByName(const string& className) {
    {
        std::lock_guard<std::mutex> lock(s_nodeCacheMutex);
        auto itFound = s_name2TreeNodeCache.find(className);
        if (itFound != s_name2TreeNodeCache.end()) {
            return itFound->second;
        }
    }

    MetadataTreeNode* result = s_metadataReader.GetOrCreateTreeNodeByName(className);

    std::lock_guard<std::mutex> lock(s_nodeCacheMutex);
    return s_name2TreeNodeCache.emplace(className, result).first->second;
}

const string& MetadataNode::GetName() {
    return m_name;
}

bool MetadataNode::IsNodeTypeInterface() {
    return m_isInterface;
}

string MetadataNode::GetTypeMetadataName(Isolate* isolate, Local<Value>& value) {
    auto target = ObjectManager::UnwrapProxy(value);
    auto data = target->IsFunction() ? GetTypeMetadata(isolate, target.As<Function>()) : nullptr;
    if (data == nullptr) {
        throw NativeScriptException(string("Expected a Java class or interface constructor"));
    }

    return data->name;
}

Local<Object> MetadataNode::CreateWrapper(Isolate* isolate) {
    EscapableHandleScope handle_scope(isolate);

    Local<Object> obj;

    uint8_t nodeType = s_metadataReader.GetNodeType(m_treeNode);

    bool isClass = s_metadataReader.IsNodeTypeClass(nodeType),
         isInterface = s_metadataReader.IsNodeTypeInterface(nodeType);

    if (isClass || isInterface) {
        obj = GetConstructorFunction(isolate);
    } else if (s_metadataReader.IsNodeTypePackage(nodeType)) {
        obj = CreatePackageObject(isolate);
    } else {
        stringstream ss;
        ss << "(InternalError): Can't create proxy for this type=" << static_cast<int>(nodeType);
        throw NativeScriptException(ss.str());
    }

    return handle_scope.Escape(obj);
}

Local<Object> MetadataNode::CreateJSWrapper(Isolate* isolate, ObjectManager* objectManager) {
    Local<Object> obj;

    if (m_isArray) {
        obj = CreateArrayWrapper(isolate);
    } else {
        auto& data = GetInstanceData(isolate);
        if (data.instanceTemplate == nullptr) {
            // Throws for a class missing at runtime, which has no template.
            GetConstructorFunction(isolate);
            auto cache = GetMetadataNodeCache(isolate);
            auto ft = Local<FunctionTemplate>::New(isolate, *cache->CtorFuncCache.at(m_treeNode).ft);
            data.instanceTemplate = new Persistent<ObjectTemplate>(isolate, ft->InstanceTemplate());
        }
        // Starts on the class's initial map, with the constructor's prototype, and
        // without running the constructor callback.
        auto context = isolate->GetCurrentContext();
        if (data.instanceTemplate->Get(isolate)->NewInstance(context).ToLocal(&obj)) {
            obj->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::Metadata),
                                  data.metadata->Get(isolate));
        }
    }

    return obj;
}

void MetadataNode::ArrayLengthGetterCallack(Local<Name> property, const PropertyCallbackInfo<Value>& info) {
    try {
        auto thiz = info.Holder();
        auto isolate = info.GetIsolate();
        auto length = CallbackHandlers::GetArrayLength(isolate, thiz);
        info.GetReturnValue().Set(length);
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

Local<Object> MetadataNode::CreateArrayWrapper(Isolate* isolate) {
    auto node = GetOrCreate("java/lang/Object");
    auto ctorFunc = node->GetConstructorFunction(isolate);

    auto arrayObjectTemplate = GetOrCreateArrayObjectTemplate(isolate);

    auto context = isolate->GetCurrentContext();
    auto arr = arrayObjectTemplate->NewInstance(context).ToLocalChecked();
    arr->SetPrototype(context, ctorFunc->Get(context, V8StringConstants::GetPrototype(isolate)).ToLocalChecked());
    arr->SetNativeDataProperty(context, ArgConverter::ConvertToV8String(isolate, "length"),
                               ArrayLengthGetterCallack, nullptr, Local<Value>(),
                               PropertyAttribute::DontDelete).ToChecked();

    SetInstanceMetadata(isolate, arr, this);

    return arr;
}

Local<Object> MetadataNode::CreatePackageObject(Isolate* isolate) {
    auto packageObj = Object::New(isolate);
    auto ctx = isolate->GetCurrentContext();
    auto extData = External::New(isolate, this, v8::kExternalPointerTypeTagDefault);

    // Every child is a lazy data property (replaced by its value on first read),
    // so anything that reads a property descriptor off a package object -
    // console formatting, most notably - would materialize the whole subtree.
    // The marker lets such callers recognize a package and stop.
    auto cache = GetMetadataNodeCache(isolate);
    packageObj->SetPrivate(ctx,
                           Private::ForApi(isolate, Local<String>::New(isolate, *cache->PackageKey)),
                           extData).ToChecked();

    auto ptrChildren = this->m_treeNode->children;
    if (ptrChildren != nullptr) {
        const auto& children = *ptrChildren;
        if (children.size() > kManyMembers) {
            MoveToDictionaryMode(isolate, ctx, packageObj);
        }
        for (auto childNode: children) {
            packageObj->SetLazyDataProperty(ctx,
                                            NewMemberName(isolate, childNode->name),
                                            PackageGetterCallback,
                                            extData,
                                            PropertyAttribute::ReadOnly).ToChecked();
        }
    }

    return packageObj;
}

void MetadataNode::SetClassAccessor(Local<Function>& ctorFunction) {
    auto isolate = v8::Isolate::GetCurrent();
    auto classFieldName = ArgConverter::ConvertToV8String(isolate, "class");
    auto context = isolate->GetCurrentContext();
    auto classGetter = NewNonConstructor(isolate, ClassAccessorGetterCallback)
                           ->GetFunction(context).ToLocalChecked();
    ctorFunction->SetAccessorProperty(classFieldName, classGetter, Local<Function>(),
                                      PropertyAttribute::DontDelete);
}

void MetadataNode::ClassAccessorGetterCallback(const FunctionCallbackInfo<Value>& info) {
    try {
        auto isolate = info.GetIsolate();
        auto thiz = ResolveProxyReceiver(isolate, info.This(), "read", "class", false);
        auto data = thiz->IsFunction() ? GetTypeMetadata(isolate, thiz.As<Function>()) : nullptr;
        if (data == nullptr) {
            throw NativeScriptException(string("'class' can only be read from a Java class constructor"));
        }

        auto value = CallbackHandlers::FindClass(isolate, data->name);
        info.GetReturnValue().Set(value);
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

void MetadataNode::NullObjectAccessorGetterCallback(const FunctionCallbackInfo<Value>& info) {
    try {
        DEBUG_WRITE("NullObjectAccessorGetterCallback called");
        auto isolate = info.GetIsolate();

        auto thiz = ResolveProxyReceiver(isolate, info.This(), "read", "null", false);
        auto context = isolate->GetCurrentContext();
        auto nullObjectKey = V8StringConstants::GetNullObjectPrivate(isolate);

        // A separate object, cached on the receiver so X.null is stable. Never tag
        // or modify the constructor: anything carrying the null-node private
        // converts to a Java null, and a constructor's valueOf may be a static
        // Java method. Through the accessor a JS-extended class inherits, This is
        // the subclass and Data the Java base class node, so that typed null is
        // of the base class.
        Local<Value> nullObject;
        if (!thiz->GetPrivate(context, nullObjectKey).ToLocal(&nullObject) || !nullObject->IsObject()) {
            auto node = reinterpret_cast<MetadataNode*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));
            auto sentinel = Object::New(isolate);
            V8SetPrivateValue(isolate, sentinel, V8StringConstants::GetNullNodePrivate(isolate), External::New(isolate, node, v8::kExternalPointerTypeTagDefault));
            // Function::New does not cache its template in the context, so the
            // function lives only as long as the sentinel.
            auto valueOf = Function::New(context, MetadataNode::NullValueOfCallback, Local<Value>(), 0,
                                         ConstructorBehavior::kThrow).ToLocalChecked();
            sentinel->DefineOwnProperty(context, V8StringConstants::GetValueOf(isolate), valueOf,
                                        PropertyAttribute::DontEnum).Check();
            thiz->SetPrivate(context, nullObjectKey, sentinel).Check();
            nullObject = sentinel;
        }

        info.GetReturnValue().Set(nullObject);
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

void MetadataNode::NullValueOfCallback(const FunctionCallbackInfo<Value>& args) {
    try {
        auto isolate = args.GetIsolate();
        args.GetReturnValue().SetNull();
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

void MetadataNode::FieldAccessorGetterCallback(const FunctionCallbackInfo<Value>& info) {
    try {
        auto fieldCallbackData = reinterpret_cast<FieldCallbackData*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));
        auto &fieldCallbackMetadata = fieldCallbackData->metadata;
        auto thiz = ResolveProxyReceiver(info.GetIsolate(), info.This(),
                                         "read Java field", fieldCallbackMetadata.getName(),
                                         !fieldCallbackMetadata.isStatic);

        auto objectManager =
            Runtime::GetRuntime(info.GetIsolate())->GetObjectManager();
        if ((!fieldCallbackMetadata.isStatic &&
             !objectManager->IsJsRuntimeObject(thiz))
                // check whether there's a declaring type to get the class from it
                || (fieldCallbackMetadata.getDeclaringType() == "")) {
            info.GetReturnValue().SetUndefined();
            return;
        }

        auto isolate = info.GetIsolate();
        auto value = CallbackHandlers::GetJavaField(isolate, thiz, fieldCallbackData);
        info.GetReturnValue().Set(value);
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
void MetadataNode::StaticFieldGetterCallback(Local<Name> property, const PropertyCallbackInfo<Value>& info) {
    try {
        auto fieldCallbackData = reinterpret_cast<FieldCallbackData*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));
        if (fieldCallbackData->metadata.getDeclaringType() == "") {
            info.GetReturnValue().SetUndefined();
            return;
        }
        auto isolate = info.GetIsolate();
        info.GetReturnValue().Set(CallbackHandlers::GetJavaField(isolate, info.Holder(), fieldCallbackData));
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

void MetadataNode::StaticFieldSetterCallback(Local<Name> property, Local<Value> value, const PropertyCallbackInfo<Boolean>& info) {
    try {
        auto fieldCallbackData = reinterpret_cast<FieldCallbackData*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));
        auto &fieldCallbackMetadata = fieldCallbackData->metadata;
        if (fieldCallbackMetadata.getIsFinal()) {
            stringstream ss;
            ss << "You are trying to set \"" << fieldCallbackMetadata.getName() << "\" which is a final field! Final fields can only be read.";
            throw NativeScriptException(ss.str());
        }
        CallbackHandlers::SetJavaField(info.GetIsolate(), info.Holder(), value, fieldCallbackData);
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

void MetadataNode::FieldAccessorSetterCallback(const FunctionCallbackInfo<Value>& info) {
    auto value = info[0];
    try {
        auto fieldCallbackData = reinterpret_cast<FieldCallbackData*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));
        auto &fieldCallbackMetadata = fieldCallbackData->metadata;
        auto thiz = ResolveProxyReceiver(info.GetIsolate(), info.This(),
                                         "write Java field", fieldCallbackMetadata.getName(),
                                         !fieldCallbackMetadata.isStatic);

        auto objectManager =
            Runtime::GetRuntime(info.GetIsolate())->GetObjectManager();
        if (!fieldCallbackMetadata.isStatic &&
            !objectManager->IsJsRuntimeObject(thiz)) {
            auto isolate = info.GetIsolate();
            info.GetReturnValue().Set(v8::Undefined(isolate));
            return;
        }

        if (fieldCallbackMetadata.getIsFinal()) {
            stringstream ss;
            ss << "You are trying to set \"" << fieldCallbackMetadata.getName() << "\" which is a final field! Final fields can only be read.";
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        } else {
            auto isolate = info.GetIsolate();
            CallbackHandlers::SetJavaField(isolate, thiz, value, fieldCallbackData);
            info.GetReturnValue().Set(value);
        }
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

void MetadataNode::PropertyAccessorGetterCallback(const FunctionCallbackInfo<Value>& info) {
    try {
        auto isolate = info.GetIsolate();
        auto context = isolate->GetCurrentContext();
        auto thiz = info.This();
        auto propertyCallbackData = reinterpret_cast<PropertyCallbackData*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));

        std::string getterMethodName = propertyCallbackData->getterMethodName;
        if(getterMethodName == ""){
            throw NativeScriptException("Missing getter method for property: " + propertyCallbackData->propertyName);
        }

        auto getter = thiz->Get(context, v8::String::NewFromUtf8(isolate, getterMethodName.c_str()).ToLocalChecked()).ToLocalChecked();
        auto value = getter.As<Function>()->Call(context, thiz, 0, nullptr).ToLocalChecked();
        info.GetReturnValue().Set(value);
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
void MetadataNode::PropertyAccessorSetterCallback(const FunctionCallbackInfo<Value>& info) {
    Local<Value> value = info[0];
    try {
        auto isolate = info.GetIsolate();
        auto context = isolate->GetCurrentContext();
        auto thiz = info.This();
        auto propertyCallbackData = reinterpret_cast<PropertyCallbackData*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));

        std::string setterMethodName = propertyCallbackData->setterMethodName;
        if(setterMethodName == ""){
            throw NativeScriptException("Missing setter method for property: " + propertyCallbackData->propertyName);
        }

        auto setter = thiz->Get(context, v8::String::NewFromUtf8(isolate, setterMethodName.c_str()).ToLocalChecked()).ToLocalChecked();
        setter.As<Function>()->Call(context, thiz, 1, &value).ToLocalChecked();
        info.GetReturnValue().Set(value);
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

void MetadataNode::SuperAccessorGetterCallback(const FunctionCallbackInfo<Value>& info) {
    try {
        auto isolate = info.GetIsolate();
        auto thiz = ResolveProxyReceiver(isolate, info.This(), "read", "super", true);
        auto key = ArgConverter::ConvertToV8String(isolate, "supervalue");
        Local<Value> hidenVal;
        V8GetPrivateValue(isolate, thiz, key, hidenVal);
        auto superValue = hidenVal.As<Object>();

        if (superValue.IsEmpty()) {
            auto runtime = Runtime::GetRuntime(isolate);
            auto objectManager = runtime->GetObjectManager();
            auto context = isolate->GetCurrentContext();

            superValue = objectManager->GetEmptyObject(isolate);
            superValue->Delete(context, V8StringConstants::GetToString(isolate));
            superValue->Delete(context, V8StringConstants::GetValueOf(isolate));
            superValue->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper), True(isolate));

            superValue->SetPrototype(context, thiz->GetPrototype().As<Object>()->GetPrototype().As<Object>()->GetPrototype());
            V8SetPrivateValue(isolate, thiz, key, superValue);
            objectManager->CloneLink(thiz, superValue);

            // The alias is tracked on the object that owns the link, which for
            // the TypeScript layout is the prototype rather than `thiz`.
            auto owner = ObjectManager::IsJsRuntimeObject(thiz) ? ObjectManager::GetJSInstanceInfoOwner(thiz) : Local<Object>();
            if (owner.IsEmpty()) {
                owner = thiz;
            }
            V8SetPrivateValue(isolate, superValue, ArgConverter::ConvertToV8String(isolate, "superowner"), owner);
            auto aliasesKey = ArgConverter::ConvertToV8String(isolate, "superaliases");
            Local<Value> aliasesValue;
            V8GetPrivateValue(isolate, owner, aliasesKey, aliasesValue);
            Local<Array> aliases;
            if (!aliasesValue.IsEmpty() && aliasesValue->IsArray()) {
                aliases = aliasesValue.As<Array>();
            } else {
                aliases = Array::New(isolate);
                V8SetPrivateValue(isolate, owner, aliasesKey, aliases);
            }
            aliases->CreateDataProperty(context, aliases->Length(), superValue).Check();

            DEBUG_WRITE("superValue.GetPrototype=%d", superValue->GetPrototype().As<Object>()->GetIdentityHash());

            auto node = GetInstanceMetadata(isolate, thiz);
            SetInstanceMetadata(isolate, superValue, node);
        }

        info.GetReturnValue().Set(superValue);
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

class MetadataNode::PrototypeTemplateFiller {
public:
    explicit PrototypeTemplateFiller(Local<ObjectTemplate> protoTemplate)
        : m_protoTemplate(protoTemplate) {}

    void FillPrototypeMethod(Isolate *isolate, const std::string& name,
                             MethodCallbackData* methodInfo) {
        if (m_addedNames.count(name) != 0) {
            DEBUG_WRITE("Not defining method prototype.%s which has already been added", name.c_str());
            return;
        }

        Local<External> funcData = External::New(isolate, methodInfo, v8::kExternalPointerTypeTagDefault);
        Local<FunctionTemplate> funcTemplate = NewNonConstructor(isolate, MetadataNode::MethodCallback, funcData);
        m_protoTemplate->Set(NewMemberName(isolate, name), funcTemplate);
        m_addedNames.insert(name);
    }

    void FillPrototypeField(Isolate* isolate, const std::string& name,
                            FieldCallbackData* fieldInfo) {
        if (m_addedNames.count(name) != 0) {
            DEBUG_WRITE("Not defining field prototype.%s which already exists", name.c_str());
            return;
        }

        Local<External> fieldData = External::New(isolate, fieldInfo, v8::kExternalPointerTypeTagDefault);
        Local<Name> nameString = NewMemberName(isolate, name);
        m_protoTemplate->SetAccessorProperty(
                nameString,
                NewNonConstructor(isolate, MetadataNode::FieldAccessorGetterCallback, fieldData),
                NewNonConstructor(isolate, MetadataNode::FieldAccessorSetterCallback, fieldData),
                PropertyAttribute::DontDelete);
        m_addedNames.insert(name);
    }

    void FillPrototypeProperty(Isolate* isolate, const std::string& name,
                               PropertyCallbackData* propertyInfo) {
        if (m_addedNames.count(name) != 0) {
            DEBUG_WRITE("Not defining property prototype.%s which already exists", name.c_str());
            return;
        }

        Local<External> propertyData = External::New(isolate, propertyInfo, v8::kExternalPointerTypeTagDefault);
        Local<Name> nameString = NewMemberName(isolate, name);
        m_protoTemplate->SetAccessorProperty(
                nameString,
                NewNonConstructor(isolate, MetadataNode::PropertyAccessorGetterCallback, propertyData),
                NewNonConstructor(isolate, MetadataNode::PropertyAccessorSetterCallback, propertyData),
                PropertyAttribute::DontDelete);
        m_addedNames.insert(name);
    }

private:
    Local<ObjectTemplate> m_protoTemplate;
    std::unordered_set<std::string> m_addedNames;
};

void MetadataNode::SetInstanceMembers(
        Isolate* isolate, Local<FunctionTemplate>& ctorFuncTemplate,
        PrototypeTemplateFiller& protoFiller,
        vector<MethodCallbackData*>& instanceMethodsCallbackData,
        const vector<MethodCallbackData*>& baseInstanceMethodsCallbackData,
        MetadataTreeNode* treeNode, uint8_t* &curPtr) {
    auto hasCustomMetadata = treeNode->metadata != nullptr;

    if (hasCustomMetadata) {
        SetInstanceMembersFromRuntimeMetadata(
            isolate, protoFiller, instanceMethodsCallbackData,
            baseInstanceMethodsCallbackData, treeNode);
        return;
    }

    SetInstanceMethodsFromStaticMetadata(
        isolate, ctorFuncTemplate, protoFiller, instanceMethodsCallbackData,
        baseInstanceMethodsCallbackData, treeNode, curPtr);
}

void MetadataNode::SetInstanceMethodsFromStaticMetadata(
        Isolate *isolate, Local<FunctionTemplate> &ctorFuncTemplate,
        PrototypeTemplateFiller& protoFiller,
        vector<MethodCallbackData *> &instanceMethodsCallbackData,
        const vector<MethodCallbackData *> &baseInstanceMethodsCallbackData,
        MetadataTreeNode *treeNode, uint8_t* &curPtr) {
    SET_PROFILER_FRAME();

    uint8_t sections = s_metadataReader.GetClassSections(treeNode);
    curPtr = s_metadataReader.GetClassMembers(treeNode);

    auto curType = s_metadataReader.ReadTypeName(treeNode);

    string lastMethodName;
    MethodCallbackData *callbackData = nullptr;

    auto context = isolate->GetCurrentContext();
    auto cache = GetMetadataNodeCache(isolate);

    robin_hood::unordered_map<std::string, MethodCallbackData *> collectedExtensionMethodDatas;

    // The base class is fully built, so its method names stay put while this class is.
    robin_hood::unordered_flat_map<std::string_view, MethodCallbackData *> baseMethodsByName;
    baseMethodsByName.reserve(baseInstanceMethodsCallbackData.size());
    for (auto baseMethod: baseInstanceMethodsCallbackData) {
        baseMethodsByName.emplace(baseMethod->candidates.front().name, baseMethod);
    }
    auto findBaseMethod = [&baseMethodsByName](const std::string &name) -> MethodCallbackData * {
        auto itFound = baseMethodsByName.find(std::string_view(name));
        return itFound != baseMethodsByName.end() ? itFound->second : nullptr;
    };

    auto extensionFunctionsCount = MetadataReader::ReadSectionCount(
            sections, MetadataReader::SECTION_EXTENSION_FUNCTIONS, &curPtr);
    for (auto i = 0; i < extensionFunctionsCount; i++) {
        auto entry = MetadataReader::ReadExtensionFunctionEntry(&curPtr);

        auto &methodName = entry.getName();
        if (methodName!= lastMethodName) {
            //
            callbackData = tryGetExtensionMethodCallbackData(collectedExtensionMethodDatas,
                                                             methodName);
            if (callbackData == nullptr) {
                callbackData = cache->Own<MethodCallbackData>(this);
                protoFiller.FillPrototypeMethod(isolate, methodName, callbackData);

                lastMethodName = methodName;
                std::pair<std::string, MethodCallbackData *> p(methodName, callbackData);
                collectedExtensionMethodDatas.emplace(p);
            }
        }
        callbackData->candidates.push_back(std::move(entry));
    }


    //get candidates from instance methods metadata
    auto instanceMethodCount = MetadataReader::ReadSectionCount(
            sections, MetadataReader::SECTION_INSTANCE_METHODS, &curPtr);

    for (auto i = 0; i < instanceMethodCount; i++) {
        auto entry = MetadataReader::ReadInstanceMethodEntry(&curPtr);

        // attach a function to the prototype of a javascript Object
        auto &methodName = entry.getName();
        if (methodName != lastMethodName) {
            // See if we have tracked the callback data before (meaning another version of entry.name exists with different parameters)
            callbackData = tryGetExtensionMethodCallbackData(collectedExtensionMethodDatas,
                                                             methodName);
            if (callbackData == nullptr) {
                callbackData = cache->Own<MethodCallbackData>(this);

                // If we have no tracking of this callback data, create tracking so that we can find it if need be for future itterations where the entry.name is the same...
                std::pair<std::string, MethodCallbackData *> p(methodName, callbackData);
                collectedExtensionMethodDatas.emplace(p);
            }

            instanceMethodsCallbackData.push_back(callbackData);
            callbackData->parent = findBaseMethod(methodName);

            if (s_profilerEnabled.load(std::memory_order_relaxed)) {
                Local<External> funcData = External::New(isolate, callbackData, v8::kExternalPointerTypeTagDefault);
                Local<FunctionTemplate> funcTemplate = NewNonConstructor(isolate, MethodCallback, funcData);
                auto func = funcTemplate->GetFunction(context).ToLocalChecked();
                std::string origin = Constants::APP_ROOT_FOLDER_PATH + GetOrCreateInternal(treeNode)->m_name;
                Local<Function> wrapperFunc = Wrap(isolate, func, methodName, origin,
                                                   false /* isCtorFunc */);
                Local<Function> ctorFunc = ctorFuncTemplate->GetFunction(context).ToLocalChecked();
                Local<Value> protoVal;
                ctorFunc->Get(context,
                              ArgConverter::ConvertToV8String(isolate, "prototype")).ToLocal(
                        &protoVal);
                Local<String> funcName = ArgConverter::ConvertToV8String(isolate, methodName);
                if (!protoVal.IsEmpty() && !protoVal->IsUndefined() && !protoVal->IsNull()) {
                    protoVal.As<Object>()->Set(context, funcName, wrapperFunc);
                }
            } else {
                protoFiller.FillPrototypeMethod(isolate, methodName, callbackData);
            }

            lastMethodName = methodName;
        }

        callbackData->candidates.push_back(std::move(entry));
    }

    //get candidates from instance fields metadata
    auto instanceFieldCout = MetadataReader::ReadSectionCount(
            sections, MetadataReader::SECTION_INSTANCE_FIELDS, &curPtr);
    for (auto i = 0; i < instanceFieldCout; i++) {
        auto entry = s_metadataReader.ReadInstanceFieldEntry(&curPtr);
        auto fieldName = entry.getName();
        auto fieldInfo = cache->Own<FieldCallbackData>(entry);
        fieldInfo->metadata.declaringType = curType;
        protoFiller.FillPrototypeField(isolate, fieldName, fieldInfo);
    }

    auto kotlinPropertiesCount = MetadataReader::ReadSectionCount(
            sections, MetadataReader::SECTION_PROPERTIES, &curPtr);
    for (int i = 0; i < kotlinPropertiesCount; ++i) {
        string propertyName = s_metadataReader.ReadName(s_metadataReader.ReadNameOffset(curPtr));
        curPtr += s_metadataReader.NameBytes();

        uint8_t accessors = *curPtr++;

        std::string getterMethodName = "";
        if (accessors & MetadataReader::PROPERTY_HAS_GETTER) {
            auto entry = MetadataReader::ReadInstanceMethodEntry(&curPtr);
            getterMethodName = entry.getName();
        }

        std::string setterMethodName = "";
        if (accessors & MetadataReader::PROPERTY_HAS_SETTER) {
            auto entry = MetadataReader::ReadInstanceMethodEntry(&curPtr);
            setterMethodName = entry.getName();
        }

        auto propertyInfo = cache->Own<PropertyCallbackData>(propertyName, getterMethodName, setterMethodName);
        protoFiller.FillPrototypeProperty(isolate, propertyName, propertyInfo);
    }
}

MetadataNode::MethodCallbackData *MetadataNode::tryGetExtensionMethodCallbackData(
        const robin_hood::unordered_map<std::string, MethodCallbackData *> &collectedMethodCallbackDatas,
        const std::string &lookupName) {

    if (collectedMethodCallbackDatas.empty()) {
        return nullptr;
    }

    auto iter = collectedMethodCallbackDatas.find(lookupName);

    if (iter != collectedMethodCallbackDatas.end()) {
        return iter->second;
    }

    return nullptr;
}


void MetadataNode::SetInstanceMembersFromRuntimeMetadata(
        Isolate* isolate, PrototypeTemplateFiller& protoFiller,
        vector<MethodCallbackData*>& instanceMethodsCallbackData,
        const vector<MethodCallbackData*>& baseInstanceMethodsCallbackData,
        MetadataTreeNode* treeNode) {
    SET_PROFILER_FRAME();

    NS_DCHECK(treeNode->metadata != nullptr);

    string line;
    const string& metadata = *treeNode->metadata;
    stringstream s(metadata);

    string kind;
    string name;
    string signature;
    int paramCount;

    getline(s, line); // type line
    getline(s, line); // base class line

    string lastMethodName;
    MethodCallbackData* callbackData = nullptr;
    auto cache = GetMetadataNodeCache(isolate);

    while (getline(s, line)) {
        // Implemented interfaces; SetInterfaceMethodsFromRuntimeMetadata reads them.
        if (!line.empty() && line[0] == 'N') {
            continue;
        }

        stringstream tmp(line);
        tmp >> kind >> name >> signature >> paramCount;

        char chKind = kind[0];

        // method or field
        NS_DCHECK((chKind == 'M') || (chKind == 'F'));

        MetadataEntry entry(nullptr, NodeType::Field);

        entry.name = name;
        entry.sig = signature;
        entry.paramCount = paramCount;
        entry.isStatic = false;

        if (chKind == 'M') {
            if (entry.name != lastMethodName) {
                callbackData = cache->Own<MethodCallbackData>(this);

                instanceMethodsCallbackData.push_back(callbackData);
                auto itBegin = baseInstanceMethodsCallbackData.begin();
                auto itEnd = baseInstanceMethodsCallbackData.end();
                auto itFound = find_if(itBegin, itEnd, [&entry] (MethodCallbackData *x) {
                    return x->candidates.front().name == entry.name;
                });
                if (itFound != itEnd) {
                    callbackData->parent = *itFound;
                }

                protoFiller.FillPrototypeMethod(isolate, entry.name, callbackData);
                lastMethodName = entry.name;
            }
            callbackData->candidates.push_back(std::move(entry));
        } else if (chKind == 'F') {
            auto* fieldInfo = cache->Own<FieldCallbackData>(entry);
            protoFiller.FillPrototypeField(isolate, entry.name, fieldInfo);
        }
    }
}

/*
 * Adds to a runtime-resolved class the methods of its public interfaces (the
 * `N` lines, most specific first) under every name the class chain does not
 * already have. The interface's own callback data is installed, so a call
 * resolves the method on the interface, never on the class: Android 9 hides
 * some boot classes' members from reflection and from JNI lookups made with app
 * code on the stack, while the interface method is public API.
 *
 * Only interfaces in the build-time metadata count: the point is the public API
 * the platform hides, not the members of non-SDK interfaces, and those are never
 * resolved at run time for this. An interface that fails to materialize is
 * skipped; the class's wrapper does not depend on it.
 */
void MetadataNode::SetInterfaceMethodsFromRuntimeMetadata(
        Isolate* isolate, PrototypeTemplateFiller& protoFiller,
        const Local<FunctionTemplate>& baseFuncTemplate, MetadataTreeNode* treeNode) {
    NS_DCHECK(treeNode->metadata != nullptr);

    vector<string> interfaceNames;
    {
        stringstream s(*treeNode->metadata);
        string line;
        while (getline(s, line)) {
            if (line.size() > 2 && line[0] == 'N' && line[1] == ' ') {
                interfaceNames.push_back(line.substr(2));
            }
        }
    }
    if (interfaceNames.empty()) {
        return;
    }

    auto context = isolate->GetCurrentContext();

    Local<Object> basePrototype;
    if (!baseFuncTemplate.IsEmpty()) {
        Local<Function> baseFunc;
        Local<Value> prototype;
        if (baseFuncTemplate->GetFunction(context).ToLocal(&baseFunc) &&
            baseFunc->Get(context, V8StringConstants::GetPrototype(isolate)).ToLocal(&prototype) &&
            prototype->IsObject()) {
            basePrototype = prototype.As<Object>();
        }
    }

    for (const auto& interfaceName : interfaceNames) {
        auto interfaceTreeNode = s_metadataReader.FindStaticTreeNodeByName(interfaceName);
        if (interfaceTreeNode == nullptr) {
            continue;
        }

        vector<MethodCallbackData*> interfaceMethods;
        try {
            auto interfaceNode = GetOrCreateInternal(interfaceTreeNode);
            if (!interfaceNode->IsNodeTypeInterface()) {
                continue;
            }
            auto interfaceTemplate = interfaceNode->GetConstructorFunctionTemplate(
                    isolate, interfaceNode->m_treeNode, interfaceMethods);
            if (interfaceTemplate.IsEmpty()) {
                continue;
            }
        } catch (NativeScriptException&) {
            DEBUG_WRITE("Skipping interface %s of %s: it cannot be materialized",
                        interfaceName.c_str(), m_name.c_str());
            continue;
        }

        for (auto* methodData : interfaceMethods) {
            if (methodData->candidates.empty()) {
                continue;
            }
            const auto& methodName = methodData->candidates.front().name;
            if (!basePrototype.IsEmpty() &&
                basePrototype->Has(context, NewMemberName(isolate, methodName)).FromMaybe(true)) {
                continue;
            }
            // A name the class itself declares, or an earlier interface added, is kept.
            protoFiller.FillPrototypeMethod(isolate, methodName, methodData);
        }
    }
}


void MetadataNode::SetStaticMembers(Isolate* isolate, Local<Function>& ctorFunction, MetadataTreeNode* treeNode, uint8_t* &curPtr) {
    auto hasCustomMetadata = treeNode->metadata != nullptr;
    auto context = isolate->GetCurrentContext();

    if (!hasCustomMetadata) {
        string lastMethodName;
        MethodCallbackData* callbackData = nullptr;
        auto cache = GetMetadataNodeCache(isolate);

        auto origin = Constants::APP_ROOT_FOLDER_PATH + GetOrCreateInternal(treeNode)->m_name;

        uint8_t sections = s_metadataReader.GetClassSections(treeNode);

        //get candidates from static methods metadata
        auto staticMethodCout = MetadataReader::ReadSectionCount(
                sections, MetadataReader::SECTION_STATIC_METHODS, &curPtr);
        for (auto i = 0; i < staticMethodCout; i++) {
            auto entry = MetadataReader::ReadStaticMethodEntry(&curPtr);
            auto &methodName = entry.getName();
            if (methodName != lastMethodName) {
                callbackData = cache->Own<MethodCallbackData>(this);
                auto funcData = External::New(isolate, callbackData, v8::kExternalPointerTypeTagDefault);
                // Constructible on purpose: `new X.staticFactory()` returns the factory's object.
                auto funcTemplate = FunctionTemplate::New(isolate, MethodCallback, funcData);
                auto func = funcTemplate->GetFunction(context).ToLocalChecked();
                auto funcName = NewMemberName(isolate, methodName);
                ctorFunction->Set(context, funcName, Wrap(isolate, func, methodName, origin, false /* isCtorFunc */));
                lastMethodName = methodName;
            }
            callbackData->candidates.push_back(std::move(entry));
        }

        //attach .extend function
        auto extendFuncName = V8StringConstants::GetExtend(isolate);
        auto extendFuncTemplate = NewNonConstructor(isolate, ExtendMethodCallback, External::New(isolate, this, v8::kExternalPointerTypeTagDefault));
        ctorFunction->Set(context, extendFuncName, extendFuncTemplate->GetFunction(context).ToLocalChecked());

        //get candidates from static fields metadata
        auto staticFieldCout = MetadataReader::ReadSectionCount(
                sections, MetadataReader::SECTION_STATIC_FIELDS, &curPtr);
        // A class with many static fields (every android.R inner class) installs all of them
        // on first touch. Native accessors skip creating a getter and a setter function per
        // field, but adding them one by one to a fast-mode object copies its descriptors each
        // time, so such a class is moved to dictionary mode first. Its final fields become
        // their value on first read (read-only), since a native accessor on a dictionary-mode
        // object is slower to call than reading a plain property.
        bool manyFields = staticFieldCout > kManyMembers;
        if (manyFields) {
            MoveToDictionaryMode(isolate, context, ctorFunction);
        }
        for (auto i = 0; i < staticFieldCout; i++) {
            auto entry = s_metadataReader.ReadStaticFieldEntry(&curPtr);
            // A static field that does not name its declaring type is this class's own.
            entry.treeNode = treeNode;

            auto fieldName = NewMemberName(isolate, s_metadataReader.ReadNameView(s_metadataReader.FieldNameOffset(entry.field)));
            auto fieldData = External::New(isolate, cache->Own<FieldCallbackData>(entry), v8::kExternalPointerTypeTagDefault);
            if (manyFields) {
                if (entry.getIsFinal()) {
                    ctorFunction->SetLazyDataProperty(context, fieldName, StaticFieldGetterCallback, fieldData,
                                                      static_cast<PropertyAttribute>(PropertyAttribute::ReadOnly | PropertyAttribute::DontDelete)).ToChecked();
                } else {
                    ctorFunction->SetNativeDataProperty(context, fieldName, StaticFieldGetterCallback,
                                                        StaticFieldSetterCallback, fieldData,
                                                        PropertyAttribute::DontDelete).ToChecked();
                }
                continue;
            }
            auto fieldGetter = NewNonConstructor(isolate, FieldAccessorGetterCallback, fieldData)
                                   ->GetFunction(context).ToLocalChecked();
            auto fieldSetter = NewNonConstructor(isolate, FieldAccessorSetterCallback, fieldData)
                                   ->GetFunction(context).ToLocalChecked();
            ctorFunction->SetAccessorProperty(fieldName, fieldGetter, fieldSetter,
                                              PropertyAttribute::DontDelete);
        }

        auto nullObjectName = V8StringConstants::GetNullObject(isolate);

        Local<Value> nullObjectData = External::New(isolate, this, v8::kExternalPointerTypeTagDefault);
        auto nullObjectGetter =
            NewNonConstructor(isolate, NullObjectAccessorGetterCallback, nullObjectData)
                ->GetFunction(context).ToLocalChecked();
        ctorFunction->SetAccessorProperty(nullObjectName, nullObjectGetter);

        SetClassAccessor(ctorFunction);
    }
}

void MetadataNode::SetInnerTypes(v8::Isolate* isolate, Local<Function>& ctorFunction, MetadataTreeNode *treeNode) {
    if (treeNode->children != nullptr) {
        auto context = isolate->GetCurrentContext();
        const auto &children = *treeNode->children;
        for (auto curChild: children) {
            auto childName = NewMemberName(isolate, curChild->name);
            bool hasOwnProperty = ctorFunction->HasOwnProperty(context, childName).ToChecked();
                // Child is defined as a function already when the inner type is a companion object
                if (!hasOwnProperty) {
                    ctorFunction->SetNativeDataProperty(
                        context,
                        childName,
                        InnerTypeAccessorGetterCallback, nullptr,
                        v8::External::New(isolate, curChild, v8::kExternalPointerTypeTagDefault)
                ).ToChecked();
            }
        }
    }
}

void MetadataNode::InnerTypeAccessorGetterCallback(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();
    v8::HandleScope handleScope(isolate);

    MetadataTreeNode* curChild = static_cast<MetadataTreeNode*>(
        v8::External::Cast(*info.Data())->Value(v8::kExternalPointerTypeTagDefault));
    auto childNode = GetOrCreateInternal(curChild);
    auto innerCache = GetMetadataNodeCache(isolate);
    auto itFound = innerCache->CtorFunctions.find(childNode);
    if (itFound != innerCache->CtorFunctions.end()) {
        info.GetReturnValue().Set(itFound->second->Get(isolate));
        return;
    }
    auto innerTypeCtorFuncTemplate= childNode->GetConstructorFunctionTemplate(isolate, curChild);

    if(innerTypeCtorFuncTemplate.IsEmpty()) {
        info.GetReturnValue().Set(v8::Undefined(isolate));
        return;
    }

    auto innerTypeCtorFunc = Local<Function>::New(isolate, *GetOrCreateInternal(curChild)->GetPersistentConstructorFunction(isolate));
    auto innerTypeName = ArgConverter::ConvertToV8String(isolate, curChild->name);
    // TODO: remove this once we solve https://github.com/NativeScript/android/pull/1771
    // this avoids a crash, but the companion object is still unaccessible
    TryCatch tc(isolate);
    info.GetReturnValue().Set(innerTypeCtorFunc);
}


Local<FunctionTemplate> MetadataNode::GetConstructorFunctionTemplate(Isolate* isolate, MetadataTreeNode* treeNode) {
    std::vector<MethodCallbackData*> instanceMethodsCallbackData;

    v8::HandleScope handleScope(isolate);
    auto ft = GetConstructorFunctionTemplate(isolate, treeNode, instanceMethodsCallbackData);

    return ft;
}

Local<FunctionTemplate> MetadataNode::GetConstructorFunctionTemplate(Isolate* isolate, MetadataTreeNode* treeNode, vector<MethodCallbackData*>& instanceMethodsCallbackData) {
    SET_PROFILER_FRAME();
    tns::instrumentation::Frame frame;

    //try get cached "ctorFuncTemplate"
    Local<FunctionTemplate> ctorFuncTemplate;
    auto cache = GetMetadataNodeCache(isolate);
    auto itFound = cache->CtorFuncCache.find(treeNode);
    if (itFound != cache->CtorFuncCache.end()) {
        auto& ctorCacheItem = itFound->second;
        instanceMethodsCallbackData = ctorCacheItem.instanceMethodCallbacks;
        ctorFuncTemplate = Local<FunctionTemplate>::New(isolate, *ctorCacheItem.ft);
        return ctorFuncTemplate;
    }
    //

    auto node = GetOrCreateInternal(treeNode);

    JEnv env;
    // if we already have an exception (which will be rethrown later)
    // then we don't want to ignore the next exception
    bool ignoreFindClassException = env.ExceptionCheck() == JNI_FALSE;
    auto currentClass = env.FindClass(node->m_name);
    if (ignoreFindClassException && env.ExceptionCheck()) {
        env.ExceptionClear();
        // JNI found an exception looking up this class
        // but we don't care, because this means this class doesn't exist
        // like when you try to get a class that only exists in a higher API level
        ctorFuncTemplate.Clear();
        auto pft = new Persistent<FunctionTemplate>(isolate, ctorFuncTemplate);
        CtorCacheData ctorCacheItem(pft, instanceMethodsCallbackData);
        cache->CtorFuncCache.emplace(treeNode, ctorCacheItem);
        return ctorFuncTemplate;
    }

    auto ctorCallbackData = External::New(isolate, node, v8::kExternalPointerTypeTagDefault);
    auto isInterface = s_metadataReader.IsNodeTypeInterface(treeNode->type);
    auto funcCallback = isInterface ? InterfaceConstructorCallback : ClassConstructorCallback;
    ctorFuncTemplate = FunctionTemplate::New(isolate, funcCallback, ctorCallbackData);
    auto currentNode = treeNode;
    std::string finalName(currentNode->name);
    while (currentNode->parent) {
        if (!currentNode->parent->name.empty()) {
            finalName = currentNode->parent->name + "." + finalName;
        }
        currentNode = currentNode->parent;
    }
    ctorFuncTemplate->SetClassName(v8::String::NewFromUtf8(isolate, finalName.c_str()).ToLocalChecked());
    ctorFuncTemplate->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(ObjectManager::MetadataNodeKeys::END));

    Local<Function> baseCtorFunc;
    Local<FunctionTemplate> baseFuncTemplate;
    std::vector<MethodCallbackData*> baseInstanceMethodsCallbackData;
    auto tmpTreeNode = treeNode;
    while (true) {
        auto baseTreeNode = s_metadataReader.GetBaseClassNode(tmpTreeNode);
        if (CheckClassHierarchy(env, currentClass, treeNode, baseTreeNode)) {
            tmpTreeNode = baseTreeNode;
            continue;
        }
        if ((baseTreeNode != treeNode) && (baseTreeNode != nullptr) && (baseTreeNode->offsetValue > 0)) {
            baseFuncTemplate = GetConstructorFunctionTemplate(isolate, baseTreeNode, baseInstanceMethodsCallbackData);
            if (!baseFuncTemplate.IsEmpty()) {
                ctorFuncTemplate->Inherit(baseFuncTemplate);
                baseCtorFunc = Local<Function>::New(isolate, *GetOrCreateInternal(baseTreeNode)->GetPersistentConstructorFunction(isolate));
            }
        }
        break;
    }

    PrototypeTemplateFiller protoFiller{ctorFuncTemplate->PrototypeTemplate()};

    uint8_t* curPtr = nullptr;

    node->SetInstanceMembers(
            isolate, ctorFuncTemplate, protoFiller, instanceMethodsCallbackData,
            baseInstanceMethodsCallbackData, treeNode, curPtr);

    if (treeNode->metadata != nullptr && !isInterface) {
        node->SetInterfaceMethodsFromRuntimeMetadata(isolate, protoFiller, baseFuncTemplate, treeNode);
    }

    auto context = isolate->GetCurrentContext();
    auto ctorFunc = ctorFuncTemplate->GetFunction(context).ToLocalChecked();

    auto origin = Constants::APP_ROOT_FOLDER_PATH + node->m_name;

    auto wrappedCtorFunc = Wrap(isolate, ctorFunc, node->m_treeNode->name, origin, true /* isCtorFunc */);

    node->SetStaticMembers(isolate, wrappedCtorFunc, treeNode, curPtr);

    // insert isolate-specific persistent function handle
    cache->CtorFunctions.emplace(node, new Persistent<Function>(isolate, wrappedCtorFunc));
    if (!baseCtorFunc.IsEmpty()) {
        auto currentContext = isolate->GetCurrentContext();
        wrappedCtorFunc->SetPrototype(currentContext, baseCtorFunc);
    }

   //cache "ctorFuncTemplate"
   auto pft = new Persistent<FunctionTemplate>(isolate, ctorFuncTemplate);
   CtorCacheData ctorCacheItem(pft, instanceMethodsCallbackData);
   cache->CtorFuncCache.emplace(treeNode, ctorCacheItem);

    SetInnerTypes(isolate, wrappedCtorFunc, treeNode);

    SetTypeMetadata(isolate, wrappedCtorFunc, cache->Own<TypeMetadata>(s_metadataReader.ReadTypeName(treeNode)));

    if (frame.check()) {
        frame.log("Materializing class: " + node->m_name);
    }

    return ctorFuncTemplate;
}

Local<Function> MetadataNode::GetConstructorFunction(Isolate* isolate) {
    auto cache = GetMetadataNodeCache(isolate);
    auto itFound = cache->CtorFunctions.find(this);
    if (itFound != cache->CtorFunctions.end()) {
        return Local<Function>::New(isolate, *itFound->second);
    }
    GetConstructorFunctionTemplate(isolate, m_treeNode);
    auto ctorFunc = Local<Function>::New(isolate, *GetPersistentConstructorFunction(isolate));

    return ctorFunc;
}

Persistent<Function>* MetadataNode::GetPersistentConstructorFunction(Isolate* isolate) {
    auto cache = GetMetadataNodeCache(isolate);
    auto itFound = cache->CtorFunctions.find(this);
    if (itFound != cache->CtorFunctions.end()) {
        auto& constrFunction = itFound->second;

        return constrFunction;
    } else {
        throw NativeScriptException("Constructor function not found for node: " + this->m_name);
    }
}

MetadataNode::TypeMetadata* MetadataNode::GetTypeMetadata(Isolate* isolate, const Local<Function>& value) {
    Local<Value> hiddenVal;
    V8GetPrivateValue(isolate, value, String::NewFromUtf8(isolate, "typemetadata").ToLocalChecked(), hiddenVal);
    if (hiddenVal.IsEmpty() || !hiddenVal->IsExternal()) {
        return nullptr;
    }

    auto data = reinterpret_cast<TypeMetadata*>(hiddenVal.As<External>()->Value(v8::kExternalPointerTypeTagDefault));
    return data;
}

void MetadataNode::SetTypeMetadata(Isolate* isolate, Local<Function> value, TypeMetadata* data) {
    V8SetPrivateValue(isolate, value, String::NewFromUtf8(isolate, "typemetadata").ToLocalChecked(),  External::New(isolate, data, v8::kExternalPointerTypeTagDefault));
}

MetadataNode::InstanceData& MetadataNode::GetInstanceData(Isolate* isolate) {
    auto cache = GetMetadataNodeCache(isolate);
    auto& data = cache->Instances[this];
    if (data.metadata == nullptr) {
        data.metadata = new Persistent<External>(isolate, External::New(isolate, this, v8::kExternalPointerTypeTagDefault));
    }
    return data;
}

MetadataNode* MetadataNode::GetInstanceMetadata(Isolate* isolate, const Local<Object>& value) {
    if (ObjectManager::HasMetadataField(value)) {
        auto field = value->GetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::Metadata)).As<Value>();
        if (!field->IsExternal()) {
            return nullptr;
        }
        return static_cast<MetadataNode*>(field.As<External>()->Value(v8::kExternalPointerTypeTagDefault));
    }

    MetadataNode* node = nullptr;
    auto cache = GetMetadataNodeCache(isolate);
    auto key = Local<String>::New(isolate, *cache->MetadataKey);
    Local<Value> hiddenVal;
    V8GetPrivateValue(isolate, value, key, hiddenVal);
    auto ext = hiddenVal;
    if (!ext.IsEmpty()) {
        node = reinterpret_cast<MetadataNode*>(ext.As<External>()->Value(v8::kExternalPointerTypeTagDefault));
    }

    return node;
}

void MetadataNode::SetInstanceMetadata(Isolate* isolate, Local<Object> object, MetadataNode* node) {
    if (ObjectManager::HasMetadataField(object)) {
        // node is null for `super` read through an object that is not a Java instance.
        Local<Data> metadata = node == nullptr ? Local<Data>(Undefined(isolate))
                                               : node->GetInstanceData(isolate).metadata->Get(isolate);
        object->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::Metadata), metadata);
        return;
    }

    auto cache = GetMetadataNodeCache(isolate);
    auto key = Local<String>::New(isolate, *cache->MetadataKey);
    V8SetPrivateValue(isolate, object, key, External::New(isolate, node, v8::kExternalPointerTypeTagDefault));
}

void MetadataNode::ExtendedClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    TNSPERF();
    try {
        SET_PROFILER_FRAME();

        if (!info.IsConstructCall()) {
            throw NativeScriptException(string("Incorrectly calling a Java class as a method. Class must be created by invoking its constructor with the `new` keyword."));
        }

        auto isolate = info.GetIsolate();
        auto thiz = info.This();
        auto extData = reinterpret_cast<ExtendedClassCallbackData*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));

        v8::HandleScope handleScope(isolate);

        auto implementationObject = Local<Object>::New(isolate, *extData->implementationObject);

        SetInstanceMetadata(isolate, thiz, extData->node);
        thiz->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper), True(isolate));
        V8SetPrivateValue(isolate, thiz, V8StringConstants::GetImplementationObject(isolate), implementationObject);

        ArgsWrapper argWrapper(info, ArgType::Class);

        string fullClassName = extData->fullClassName;

        bool success = CallbackHandlers::RegisterInstance(isolate, thiz, fullClassName, argWrapper, implementationObject, false, extData->node->m_name, extData->defaultOverrides,
                                                          extData->contentMemberNames ? &*extData->contentMemberNames : nullptr);
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

void MetadataNode::InterfaceConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    tns::instrumentation::Frame frame;
    try {
        SET_PROFILER_FRAME();

        if (!info.IsConstructCall()) {
            throw NativeScriptException(string("Interface implementation must be invoked as a constructor with the `new` keyword."));
        }

        auto isolate = info.GetIsolate();
        auto thiz = info.This();
        auto node = reinterpret_cast<MetadataNode*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));

        v8::HandleScope handleScope(isolate);

        Local<Object> implementationObject;
        Local<String> v8ExtendName;
        auto context = isolate->GetCurrentContext();

        if (info.Length() == 1) {
            if (!info[0]->IsObject()) {
                throw NativeScriptException(string("First argument must be implementation object"));
            }
            implementationObject = ResolveImplementationObject(isolate, info[0].As<Object>());
        } else if (info.Length() == 2) {
            if (!info[0]->IsString()) {
                throw NativeScriptException(string("First argument must be string"));
            }
            if (!info[1]->IsObject()) {
                throw NativeScriptException(string("Second argument must be implementation object"));
            }

            v8ExtendName = info[0]->ToString(context).ToLocalChecked();
            implementationObject = ResolveImplementationObject(isolate, info[1].As<Object>());
        } else {
            throw NativeScriptException(string("Invalid number of arguments"));
        }

        auto className = node->m_implType;
        uint64_t defaultOverrides = node->GetDefaultOverrides(isolate, implementationObject);
        if (defaultOverrides != 0) {
            className += DEFAULT_OVERRIDING_SUFFIX;
        }
        SetInstanceMetadata(isolate, thiz, node);

        //@@@ Refactor
        thiz->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper), True(isolate));

        implementationObject->SetPrototype(context, thiz->GetPrototype());
        thiz->SetPrototype(context, implementationObject);
        V8SetPrivateValue(isolate, thiz, V8StringConstants::GetImplementationObject(isolate), implementationObject);

        ArgsWrapper argWrapper(info, ArgType::Interface);

        auto success = CallbackHandlers::RegisterInstance(isolate, thiz, className, argWrapper, implementationObject, true, std::string(), defaultOverrides);

        if (frame.check()) {
            frame.log("Interface constructor: " + node->m_name);
        }
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

void MetadataNode::ClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    TNSPERF();
    try {
        SET_PROFILER_FRAME();

        auto thiz = info.This();

        auto isolate = info.GetIsolate();
        auto node = reinterpret_cast<MetadataNode*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));

        string extendName;
        auto className = node->m_name;

        SetInstanceMetadata(isolate, thiz, node);

        ArgsWrapper argWrapper(info, ArgType::Class);

        string fullClassName = CreateFullClassName(className, extendName);
        bool success = CallbackHandlers::RegisterInstance(isolate, thiz, fullClassName, argWrapper, Local<Object>(), false, className);
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

void MetadataNode::MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        SET_PROFILER_FRAME();

        auto e = info.Data().As<External>();

        auto callbackData = reinterpret_cast<MethodCallbackData*>(e->Value(v8::kExternalPointerTypeTagDefault));
        auto initialCallbackData = reinterpret_cast<MethodCallbackData*>(e->Value(v8::kExternalPointerTypeTagDefault));

        // Number of arguments the method is invoked with
        int argLength = info.Length();

        MetadataEntry* entry = nullptr;

        string* className;
        const auto& first = callbackData->candidates.front();
        const auto& methodName = first.name;

        while ((callbackData != nullptr) && (entry == nullptr)) {
            auto& candidates = callbackData->candidates;

            className = &callbackData->node->m_name;

            // Iterates through all methods and finds the best match based on the number of arguments
            auto found = false;
            for (auto& c : candidates) {
                found = (!c.isExtensionFunction && c.getParamCount() == argLength) || (
                        c.isExtensionFunction && c.getParamCount() == argLength + 1);
                if (found) {
                    if(c.isExtensionFunction){
                        className = &c.getDeclaringType();
                    }
                    entry = &c;
                    DEBUG_WRITE("MetaDataEntry Method %s's signature is: %s", entry->name.c_str(), entry->getSig().c_str());
                    break;
                }
            }

            // Iterates through the parent class's methods to find a good match
            if (!found) {
                callbackData = callbackData->parent;
            }
        }

        auto thiz = ResolveProxyReceiver(info.GetIsolate(), info.This(),
                                         "call Java method", methodName, !first.isStatic);

        auto isSuper = false;
        if (!first.isStatic && ObjectManager::IsJsRuntimeObject(thiz)) {
            auto superValue = thiz->GetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper)).As<Value>();
            isSuper = !superValue.IsEmpty() && superValue->IsTrue();
        }

        if ((argLength == 0) && (methodName == V8StringConstants::VALUE_OF)) {
            info.GetReturnValue().Set(info.This());
        } else {
            bool isFromInterface = initialCallbackData->node->IsNodeTypeInterface();
            CallbackHandlers::CallJavaMethod(thiz, *className, methodName, entry, isFromInterface, first.isStatic, isSuper, info);
        }
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

Intercepted MetadataNode::ArrayIndexedPropertyGetterCallback(uint32_t index, const PropertyCallbackInfo<Value>& info) {
    try {
        auto thiz = info.Holder();
        auto isolate = info.GetIsolate();
        auto context = isolate->GetCurrentContext();

        auto node = GetNodeFromHandle(thiz);

        auto element = CallbackHandlers::GetArrayElement(context, thiz, index, node->m_name);

        info.GetReturnValue().Set(element);
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
    return Intercepted::kYes;
}

Intercepted MetadataNode::ArrayIndexedPropertySetterCallback(uint32_t index, Local<Value> value, const PropertyCallbackInfo<Boolean>& info) {
    try {
        auto thiz = info.Holder();
        auto isolate = info.GetIsolate();
        auto context = isolate->GetCurrentContext();

        auto node = GetNodeFromHandle(thiz);

        CallbackHandlers::SetArrayElement(context, thiz, index, node->m_name, value);
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
    return Intercepted::kYes;
}

Local<Object> MetadataNode::GetImplementationObject(Isolate* isolate, const Local<Object>& object) {
    TNSPERF();
    DEBUG_WRITE("GetImplementationObject called  on object:%d", object->GetIdentityHash());

    auto target = object;
    Local<Value> currentPrototype = target;

    Local<Object> implementationObject;

    Local<Value> hiddenVal;
    V8GetPrivateValue(isolate, object, V8StringConstants::GetImplementationObject(isolate), hiddenVal);
    implementationObject = hiddenVal.As<Object>();

    if (!implementationObject.IsEmpty()) {
        return implementationObject;
    }

    auto context = GetCreationContextOrCurrent(isolate, object);
    if (object->HasOwnProperty(context, V8StringConstants::GetIsPrototypeImplementationObject(isolate)).ToChecked()) {
        auto v8Prototype = V8StringConstants::GetPrototype(isolate);
        auto maybeHasOwnProperty = object->HasOwnProperty(context, v8Prototype);
        if (!object->HasOwnProperty(context, v8Prototype).ToChecked()) {
            return Local<Object>();
        }

        DEBUG_WRITE("GetImplementationObject returning the prototype of the object :%d", object->GetIdentityHash());
        Local<Value> result;
        if (object->Get(context, v8Prototype).ToLocal(&result)) {
            return result.As<Object>();
        } else {
            return Local<Object>();
        }
    }

    Local<Value> hiddenValue;
    V8GetPrivateValue(isolate, object, String::NewFromUtf8(isolate, "t::ActivityImplementationObject").ToLocalChecked(), hiddenValue);
    auto obj = hiddenValue.As<Object>();
    if (!obj.IsEmpty()) {
        DEBUG_WRITE("GetImplementationObject returning ActivityImplementationObject property on object: %d", object->GetIdentityHash());
        return obj;
    }

    Local<Value> lastPrototype;
    bool prototypeCycleDetected = false;
    while (implementationObject.IsEmpty()) {
        //
        currentPrototype = currentPrototype.As<Object>()->GetPrototype();

        if (currentPrototype->IsNull()) {
            break;
        }

        //DEBUG_WRITE("GetImplementationObject currentPrototypeObject:%d", (currentPrototype.IsEmpty() || currentPrototype.As<Object>().IsEmpty()) ? -1 :  currentPrototype.As<Object>()->GetIdentityHash());
        //DEBUG_WRITE("GetImplementationObject lastPrototypeObject:%d", (lastPrototype.IsEmpty() || lastPrototype.As<Object>().IsEmpty()) ? -1 :  lastPrototype.As<Object>()->GetIdentityHash());

        if (currentPrototype == lastPrototype) {
            auto abovePrototype = currentPrototype.As<Object>()->GetPrototype();
            prototypeCycleDetected = abovePrototype == currentPrototype;
        }

        if (currentPrototype.IsEmpty() || prototypeCycleDetected) {
            //Local<Value> abovePrototype = currentPrototype.As<Object>()->GetPrototype();
            //DEBUG_WRITE("GetImplementationObject not found since cycle parents reached abovePrototype:%d", (abovePrototype.IsEmpty() || abovePrototype.As<Object>().IsEmpty()) ? -1 :  abovePrototype.As<Object>()->GetIdentityHash());
            return Local<Object>();
        } else {
            Local<Value> _hiddenVal;
            V8GetPrivateValue(isolate, currentPrototype.As<Object>(), V8StringConstants::GetClassImplementationObject(isolate), _hiddenVal);
            auto value = _hiddenVal;

            if (!value.IsEmpty()) {
                implementationObject = currentPrototype.As<Object>();
            }
        }

        lastPrototype = currentPrototype;
    }

    return implementationObject;
}

void MetadataNode::PackageGetterCallback(Local<Name> property, const PropertyCallbackInfo<Value>& info) {
    try {
        if (property.IsEmpty() || !property->IsString()) {
            return;
        }

        auto strProperty = property.As<String>();

        string propName = ArgConverter::ConvertToString(strProperty);

        if (propName.empty()) {
            return;
        }

        auto isolate = info.GetIsolate();

        auto thiz = info.Holder();

        Local<Value> hiddenVal;
        V8GetPrivateValue(isolate, thiz, strProperty, hiddenVal);
        auto cachedItem = hiddenVal;

        if (cachedItem.IsEmpty()) {
            auto node = reinterpret_cast<MetadataNode*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));

            uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);

            DEBUG_WRITE("MetadataNode::GetterCallback: prop '%s' for node '%s' called, nodeType=%d, hash=%d", propName.c_str(), node->m_treeNode->name.c_str(), nodeType, thiz.IsEmpty() ? -42 : thiz->GetIdentityHash());

            auto child = GetChildMetadataForPackage(node, propName);
            auto foundChild = child.treeNode != nullptr;

            if (foundChild) {
                auto childNode = MetadataNode::GetOrCreateInternal(child.treeNode);
                cachedItem = childNode->CreateWrapper(isolate);

                uint8_t childNodeType = s_metadataReader.GetNodeType(child.treeNode);
                bool isInterface = s_metadataReader.IsNodeTypeInterface(childNodeType);
                if (isInterface) {
                    // For all java interfaces we register the special Symbol.hasInstance property
                    // which is invoked by the instanceof operator (https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Symbol/hasInstance).
                    // For example:
                    //
                    // Object.defineProperty(android.view.animation.Interpolator, Symbol.hasInstance, {
                    //    value: function(obj) {
                    //        return true;
                    //    }
                    // });
                    RegisterSymbolHasInstanceCallback(isolate, child, cachedItem);
                }

                V8SetPrivateValue(isolate, thiz, strProperty, cachedItem);

                if (node->m_name == "org/json" && child.name == "JSONObject") {
                    JSONObjectHelper::RegisterFromFunction(isolate, cachedItem);
                }
            }
        }

        info.GetReturnValue().Set(cachedItem);
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

// SimpleNameChar of the dex 035 format the proxy generator writes. Later formats add only
// space characters, which no Java identifier can contain either.
static bool IsDexSimpleNameChar(uint32_t c) {
    if (c < 0x80) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
               c == '$' || c == '-' || c == '_';
    }
    return (c >= 0xA1 && c <= 0x1FFF) || (c >= 0x2010 && c <= 0x2027) ||
           (c >= 0x2030 && c <= 0xD7FF) || (c >= 0xE000 && c <= 0xFFEF) ||
           (c >= 0x10000 && c <= 0x10FFFF);
}

// Rejects only names no class can have. A part that is a dex name but not a Java
// identifier still generates at runtime; the static binding generator cannot compile it.
static void ValidateDottedExtendName(const string& name) {
    bool isJavaName = true;
    size_t partStart = 0;
    while (true) {
        // ClassResolver reads '/' as '.'.
        size_t partEnd = name.find_first_of("./", partStart);
        if (partEnd == string::npos) {
            partEnd = name.size();
        }
        if (partEnd == partStart) {
            throw NativeScriptException("Invalid extend() name \"" + name + "\": a class name cannot have an empty part between dots.");
        }

        string part = name.substr(partStart, partEnd - partStart);
        for (size_t i = 0; i < part.size();) {
            unsigned char lead = part[i];
            uint32_t c;
            size_t length;
            if (lead < 0x80) {
                c = lead;
                length = 1;
            } else if ((lead & 0xE0) == 0xC0) {
                c = lead & 0x1F;
                length = 2;
            } else if ((lead & 0xF0) == 0xE0) {
                c = lead & 0x0F;
                length = 3;
            } else {
                c = lead & 0x07;
                length = 4;
            }
            for (size_t k = 1; k < length && i + k < part.size(); k++) {
                c = (c << 6) | (static_cast<unsigned char>(part[i + k]) & 0x3F);
            }
            if (!IsDexSimpleNameChar(c)) {
                char codePoint[16];
                snprintf(codePoint, sizeof(codePoint), "U+%04X", c);
                throw NativeScriptException("Invalid extend() name \"" + name + "\": \"" + part + "\" contains a character a class name cannot contain (" + codePoint + ").");
            }
            i += length;
        }

        if ((part[0] >= '0' && part[0] <= '9') || part.find('-') != string::npos) {
            isJavaName = false;
        }

        if (partEnd == name.size()) {
            break;
        }
        partStart = partEnd + 1;
    }

    if (!isJavaName) {
        __android_log_print(ANDROID_LOG_WARN, "JS",
                            "extend() name \"%s\" is not a Java class name (a part starts with a digit or contains '-'). The static binding generator cannot compile it, so the class is only ever generated at runtime.",
                            name.c_str());
    }
}

bool MetadataNode::ValidateExtendArguments(const FunctionCallbackInfo<Value>& info, bool extendLocationFound, string& extendLocation, v8::Local<v8::String>& extendName, Local<Object>& implementationObject, bool isTypeScriptExtend) {
    auto isolate = info.GetIsolate();

    if (info.Length() == 1) {
        if (!extendLocationFound) {
            stringstream ss;
            ss << "Invalid extend() call. No name specified for extend at location: " << extendLocation.c_str();
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }

        if (!info[0]->IsObject()) {
            stringstream ss;
            ss << "Invalid extend() call. No implementation object specified at location: " << extendLocation.c_str();
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }

        auto context = isolate->GetCurrentContext();
        implementationObject = info[0]->ToObject(context).ToLocalChecked();
    } else if (info.Length() == 2 || isTypeScriptExtend) {
        if (!info[0]->IsString()) {
            stringstream ss;
            ss << "Invalid extend() call. No name for extend specified at location: " << extendLocation.c_str();
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }

        if (!info[1]->IsObject()) {
            stringstream ss;
            ss << "Invalid extend() call. Named extend should be called with second object parameter containing overridden methods at location: " << extendLocation.c_str();
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }

        DEBUG_WRITE("ExtendsCallMethodHandler: getting extend name");
        auto context = isolate->GetCurrentContext();
        extendName = info[0]->ToString(context).ToLocalChecked();
        bool isValidExtendName = IsValidExtendName(extendName);
        if (!isValidExtendName) {
            stringstream ss;
            ss << "The extend name \"" << ArgConverter::ConvertToString(extendName) << "\" you provided contains invalid symbols. Try using the symbols [a-z, A-Z, 0-9, _]." << endl;
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }
        implementationObject = info[1]->ToObject(context).ToLocalChecked();
    } else {
        stringstream ss;
        ss << "Invalid extend() call at location: " << extendLocation.c_str();
        string exceptionMessage = ss.str();

        throw NativeScriptException(exceptionMessage);
    }

    return true;
}

MetadataNode::ExtendedClassCacheData MetadataNode::GetCachedExtendedClassData(Isolate* isolate, const string& proxyClassName) {
    auto cache = GetMetadataNodeCache(isolate);
    ExtendedClassCacheData cacheData;
    auto itFound = cache->ExtendedCtorFuncCache.find(proxyClassName);
    if (itFound != cache->ExtendedCtorFuncCache.end()) {
        cacheData = itFound->second;
    }

    return cacheData;
}

string MetadataNode::CreateFullClassName(const std::string& className, const std::string& extendNameAndLocation = "") {
    string fullClassName = className;

    // create a class name consisting only of the base class name + last file name part + line + column + variable identifier
    if (!extendNameAndLocation.empty()) {
        string tempClassName = className;
        fullClassName = Util::ReplaceAll(tempClassName, "$", "_");
        fullClassName += "_" + extendNameAndLocation;
    }

    return fullClassName;
}

void MetadataNode::InitContentKeyedBindings(const vector<string>& collisions) {
    static std::once_flag once;
    std::call_once(once, [&collisions]() {
        auto bindings = new ContentKeyedBindings();
        bindings->collisions.insert(collisions.begin(), collisions.end());
        s_contentKeyedBindings.store(bindings, std::memory_order_release);
    });
}

// JS source spells a nested type like a package, `a.B.C`, and the static binding
// generator reads names from JS source; both separators become '.'.
static string CanonicalTypeName(string name) {
    std::replace(name.begin(), name.end(), '/', '.');
    std::replace(name.begin(), name.end(), '$', '.');
    return name;
}

static string JoinSortedUnique(vector<string>& values) {
    values.erase(std::remove(values.begin(), values.end(), string()), values.end());
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());

    string joined;
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) {
            joined += ",";
        }
        joined += values[i];
    }
    return joined;
}

string MetadataNode::CreateContentKey(const string& baseClassName, vector<string> interfaceNames, vector<string> memberNames) {
    for (auto& interfaceName : interfaceNames) {
        interfaceName = CanonicalTypeName(interfaceName);
    }

    string payload = CanonicalTypeName(baseClassName) + "|" + JoinSortedUnique(interfaceNames) + "|" + JoinSortedUnique(memberNames);

    uint64_t hash = 0xcbf29ce484222325ULL;
    for (unsigned char c : payload) {
        hash ^= c;
        hash *= 0x100000001b3ULL;
    }

    char key[18];
    snprintf(key, sizeof(key), "h%016llx", static_cast<unsigned long long>(hash));
    DEBUG_WRITE("CreateContentKey: %s -> %s", payload.c_str(), key);
    return key;
}

void MetadataNode::ExtendMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    tns::instrumentation::Frame frame;
    try {
        if (info.IsConstructCall()) {
            string exMsg("Can't call 'extend' as constructor");
            throw NativeScriptException(exMsg);
        }

        SET_PROFILER_FRAME();

        Local<Object> implementationObject;
        Local<String> extendName;
        string extendLocation;

        auto hasDot = false;
        auto isTypeScriptExtend = false;
        auto isolate = info.GetIsolate();
        if (info.Length() == 2) {
            if (info[0].IsEmpty() || !info[0]->IsString()) {
                stringstream ss;
                ss << "Invalid extend() call. No name for extend specified at location: " << extendLocation.c_str();
                string exceptionMessage = ss.str();

                throw NativeScriptException(exceptionMessage);
            }
            if (info[1].IsEmpty() || !info[1]->IsObject()) {
                stringstream ss;
                ss << "Invalid extend() call. Named extend should be called with second object parameter containing overridden methods at location: " << extendLocation.c_str();
                string exceptionMessage = ss.str();

                throw NativeScriptException(exceptionMessage);
            }
            string strName = ArgConverter::ConvertToString(info[0].As<String>());
            hasDot = strName.find('.') != string::npos;
            if (hasDot) {
                ValidateDottedExtendName(strName);
            }
        } else if (info.Length() == 3) {
            auto context = isolate->GetCurrentContext();
            if (info[2]->IsBoolean() && info[2]->BooleanValue(isolate)) {
                isTypeScriptExtend = true;
            }
        }

        if (hasDot) {
            extendName = info[0].As<String>();
            implementationObject = info[1].As<Object>();
        } else {
            auto isValidExtendLocation = GetExtendLocation(isolate, extendLocation, isTypeScriptExtend);
            auto validArgs = ValidateExtendArguments(info, isValidExtendLocation, extendLocation, extendName, implementationObject, isTypeScriptExtend);

            if (!validArgs) {
                return;
            }
        }
        implementationObject = ResolveImplementationObject(isolate, implementationObject);

        auto node = reinterpret_cast<MetadataNode*>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));

        string extendNameString = ArgConverter::ConvertToString(extendName);
        DEBUG_WRITE("ExtendsCallMethodHandler: called with %s", extendNameString.c_str());

        string extendNameAndLocation = extendLocation + extendNameString;
        string fullClassName;
        string baseClassName = node->m_name;
        if (!hasDot) {
            fullClassName = TNS_PREFIX + CreateFullClassName(baseClassName, extendNameAndLocation);
        } else {
            fullClassName = ArgConverter::ConvertToString(info[0].As<String>());
        }

        uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);
        bool isInterface = s_metadataReader.IsNodeTypeInterface(nodeType);

        // The call-site name identifies the extend() call within a runtime even when
        // the Java class is named after its content and shared with other calls.
        const string siteClassName = fullClassName;
        auto contentKeyedBindings = s_contentKeyedBindings.load(std::memory_order_acquire);
        bool contentKeyed = contentKeyedBindings != nullptr && !hasDot && !isInterface;
        string contentClassName;
        std::optional<vector<string>> contentMemberNames;
        auto cache = GetMetadataNodeCache(isolate);
        if (contentKeyed) {
            auto cachedData = GetCachedExtendedClassData(isolate, siteClassName);
            if (cachedData.extendedCtorFunction != nullptr) {
                info.GetReturnValue().Set(Local<Function>::New(isolate, *cachedData.extendedCtorFunction));
                return;
            }

            contentMemberNames = CallbackHandlers::CollectContentMemberNames(isolate, implementationObject);
            auto contentKey = CreateContentKey(baseClassName, CallbackHandlers::CollectImplementedInterfaceNames(isolate, implementationObject), *contentMemberNames);
            contentClassName = TNS_PREFIX + CreateFullClassName(baseClassName, extendNameString.empty() ? contentKey : extendNameString + "_" + contentKey);

            // Java finds a named extend's implementation by its class name alone, so a
            // named content-keyed class belongs to one call site: the generator's
            // collision list names the ones it saw at more than one, and otherwise
            // the first call site in this runtime keeps it.
            bool useSiteClassName = false;
            if (!extendNameString.empty()) {
                auto owner = cache->ContentNameOwners.find(contentClassName);
                useSiteClassName = (owner != cache->ContentNameOwners.end() && owner->second != siteClassName) ||
                                   contentKeyedBindings->collisions.count(DottedClassName(contentClassName)) > 0;
            }
            if (!useSiteClassName) {
                fullClassName = contentClassName;
            }
        }

        // Every instance of the extend shares its implementation object, and so its mask.
        uint64_t defaultOverrides = 0;
        // Such an extend gets a class of its own call site rather than the interface's.
        bool hasImplementedInterfaces = isInterface && !hasDot && CallbackHandlers::HasImplementedInterfaces(isolate, implementationObject);
        if (isInterface && !hasDot && !hasImplementedInterfaces) {
            defaultOverrides = node->GetDefaultOverrides(isolate, implementationObject);
        }

        //resolve class (pre-generated or generated runtime from dex generator)
        auto clazz = defaultOverrides != 0
                     ? CallbackHandlers::ResolveClass(isolate, string(), node->m_implType + DEFAULT_OVERRIDING_SUFFIX, Local<Object>(), isInterface)
                     : CallbackHandlers::ResolveClass(isolate, baseClassName, fullClassName, implementationObject, isInterface,
                                                      contentMemberNames ? &*contentMemberNames : nullptr);
        auto fullExtendedName = CallbackHandlers::ResolveClassName(isolate, clazz);
        DEBUG_WRITE("ExtendsCallMethodHandler: extend full name %s", fullClassName.c_str());
        if (defaultOverrides != 0 || hasImplementedInterfaces) {
            // The constructor resolves the class by its own name, which can differ from the
            // requested one ('$' became '_'), and must get this class, not another one the
            // runtime would find or generate under that name.
            JEnv env;
            auto localClass = static_cast<jclass>(env.NewLocalRef(clazz));
            env.InsertClassIntoCache(fullExtendedName, localClass);
        }

        // Every anonymous extend of an interface resolves to the interface's one
        // class, as do content-keyed extends with the same content, so keying
        // their constructor by that class would hand each call site the
        // implementation object of the first.
        const string& extendedCtorKey = ((isInterface && !hasDot) || contentKeyed) ? siteClassName : fullExtendedName;
        auto cachedData = GetCachedExtendedClassData(isolate, extendedCtorKey);
        if (cachedData.extendedCtorFunction != nullptr) {
            auto cachedExtendedCtorFunc = Local<Function>::New(isolate, *cachedData.extendedCtorFunction);
            info.GetReturnValue().Set(cachedExtendedCtorFunc);
            return;
        }

        auto implementationObjectPropertyName = V8StringConstants::GetClassImplementationObject(isolate);
        //reuse validation - checks that implementationObject is not reused for different classes
        Local<Value> hiddenVal;
        V8GetPrivateValue(isolate, implementationObject, implementationObjectPropertyName, hiddenVal);
        auto implementationObjectProperty = hiddenVal.As<String>();
        if (implementationObjectProperty.IsEmpty()) {
            //mark the implementationObject as such and set a pointer to it's class node inside it for reuse validation later
            V8SetPrivateValue(isolate, implementationObject, implementationObjectPropertyName, String::NewFromUtf8(isolate, fullExtendedName.c_str()).ToLocalChecked());
        } else {
            string usedClassName = ArgConverter::ConvertToString(implementationObjectProperty);
            stringstream s;
            s << "This object is used to extend another class '" << usedClassName << "'";
            throw NativeScriptException(s.str());
        }

        auto baseClassCtorFunc = node->GetConstructorFunction(isolate);
        auto extendedClassCallbackData = cache->Own<ExtendedClassCallbackData>(node, extendNameAndLocation, implementationObject, fullExtendedName);
        extendedClassCallbackData->defaultOverrides = defaultOverrides;
        extendedClassCallbackData->contentMemberNames = std::move(contentMemberNames);
        auto extendData = External::New(isolate, extendedClassCallbackData, v8::kExternalPointerTypeTagDefault);
        auto extendFuncTemplate = FunctionTemplate::New(isolate, ExtendedClassConstructorCallback, extendData);
        extendFuncTemplate->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(ObjectManager::MetadataNodeKeys::END));

        auto context = isolate->GetCurrentContext();
        auto extendFunc = extendFuncTemplate->GetFunction(context).ToLocalChecked();
        auto prototypeName = V8StringConstants::GetPrototype(isolate);
        implementationObject->SetPrototype(context, baseClassCtorFunc->Get(context, prototypeName).ToLocalChecked());
        auto superGetter =
            NewNonConstructor(isolate, SuperAccessorGetterCallback, implementationObject)
                ->GetFunction(context).ToLocalChecked();
        implementationObject->SetAccessorProperty(V8StringConstants::GetSuper(isolate), superGetter);

        auto extendFuncPrototype = extendFunc->Get(context, prototypeName).ToLocalChecked().As<Object>();
        auto p = extendFuncPrototype->GetPrototype();
        extendFuncPrototype->SetPrototype(context, implementationObject);
        extendFunc->SetPrototype(context, baseClassCtorFunc);

        SetClassAccessor(extendFunc);
        SetTypeMetadata(isolate, extendFunc, cache->Own<TypeMetadata>(fullExtendedName));
        info.GetReturnValue().Set(extendFunc);

        {
            std::lock_guard<std::mutex> lock(s_nodeCacheMutex);
            s_name2NodeCache.emplace(fullExtendedName, node);
        }

        ExtendedClassCacheData cacheData(extendFunc, extendFuncTemplate->InstanceTemplate(), fullExtendedName, node);
        cache->ExtendedCtorFuncCache.emplace(extendedCtorKey, cacheData);
        // CreateExtendedJSWrapper looks instances created from Java up by their
        // class name; the first extend of the class keeps serving those. Each
        // entry needs its own handles: the cache destructor frees them per entry.
        bool usesContentClassName = contentKeyed && fullClassName == contentClassName;
        if (extendedCtorKey != fullExtendedName) {
            if (cache->ExtendedCtorFuncCache.find(fullExtendedName) == cache->ExtendedCtorFuncCache.end()) {
                cache->ExtendedCtorFuncCache.emplace(fullExtendedName, ExtendedClassCacheData(extendFunc, extendFuncTemplate->InstanceTemplate(), fullExtendedName, node));
            } else if (usesContentClassName) {
                cache->SharedExtendedClasses.emplace(fullExtendedName, false);
            }
        }
        if (usesContentClassName && !extendNameString.empty()) {
            cache->ContentNameOwners.emplace(contentClassName, siteClassName);
        }

        if (frame.check()) {
            frame.log("Extending: " + node->m_name);
        }
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

bool MetadataNode::IsValidExtendName(const Local<String>& name) {
    string extendNam = ArgConverter::ConvertToString(name);

    for (int i = 0; i < extendNam.size(); i++) {
        char currentSymbol = extendNam[i];
        bool isValidExtendNameSymbol = isalpha(currentSymbol) ||
                                       isdigit(currentSymbol) ||
                                       currentSymbol == '_';
        if (!isValidExtendNameSymbol) {
            return false;
        }
    }

    return true;
}

bool MetadataNode::GetExtendLocation(v8::Isolate* isolate, string& extendLocation, bool isTypeScriptExtend) {
    stringstream extendLocationStream;
    auto stackTrace = StackTrace::CurrentStackTrace(Isolate::GetCurrent(), 3, StackTrace::kOverview);
    if (!stackTrace.IsEmpty()) {
        Local<StackFrame> frame;
        if (isTypeScriptExtend) {
            frame = stackTrace->GetFrame(isolate, 2); // the _super.apply call to ts_helpers will always be the third call frame
        }  else {
            frame = stackTrace->GetFrame(isolate, 0);
        }

        if (!frame.IsEmpty()) {
            auto scriptName = frame->GetScriptName();
            if (scriptName.IsEmpty()) {
                extendLocationStream << "unknown_location";
                extendLocation = extendLocationStream.str();
                return true;
            }

            string srcFileName = ArgConverter::ConvertToString(scriptName);

            string fullPathToFile;
            // ES modules (`.mjs`, and HTTP-loaded modules) have no CommonJS wrapper.
            bool hasCommonJsPrologue = true;
            if (srcFileName == "<embedded>") {
                // Corner case, extend call is coming from the heap snapshot script
                // This is possible for lazily compiled code - e.g. from the body of a function

                // Replace embedded_script with 'script' as the SBG will emit classes from the
                // embedded_script file as 'Object_script_line_col', getting rid of fragments
                // preceding the underscore (_)
                fullPathToFile = "script";
            } else {
                // srcFileName is not always `file://<APP_ROOT>/<path>.js`:
                // HTTP ESM loading (HMR dev workflow) passes a full URL like
                // `http://<dev-host>/<path>` with no `.js` suffix and no
                // app-root prefix, so naive scheme/app-root/`.js` stripping
                // can yield an empty `fullPathToFile` and crash downstream on
                // an empty token list.
                string normalized = srcFileName;

                auto stripPrefix = [](string& s, const string& prefix) {
                    if (s.size() >= prefix.size() &&
                        s.compare(0, prefix.size(), prefix) == 0) {
                        s.erase(0, prefix.size());
                    }
                };

                stripPrefix(normalized, "file://");
                if (normalized.rfind("http://", 0) == 0 ||
                    normalized.rfind("https://", 0) == 0) {
                    hasCommonJsPrologue = false;
                    size_t schemeEnd = normalized.find("://");
                    size_t pathStart = normalized.find('/', schemeEnd + 3);
                    if (pathStart == string::npos) {
                        normalized.clear();
                    } else {
                        normalized.erase(0, pathStart + 1);
                    }
                }

                size_t queryOrFragment = normalized.find_first_of("?#");
                if (queryOrFragment != string::npos) {
                    normalized.resize(queryOrFragment);
                }

                const string& appRoot = Constants::APP_ROOT_FOLDER_PATH;
                if (!appRoot.empty()) {
                    stripPrefix(normalized, appRoot);
                }

                auto endsWith = [](const string& s, const string& suffix) {
                    return s.size() >= suffix.size() &&
                           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
                };
                if (endsWith(normalized, ".mjs")) {
                    hasCommonJsPrologue = false;
                    normalized.resize(normalized.size() - 4);
                } else if (endsWith(normalized, ".js")) {
                    normalized.resize(normalized.size() - 3);
                }

                fullPathToFile = ExtendFileIdentifier(normalized);
            }

            int lineNumber = frame->GetLineNumber();
            if (lineNumber < 0) {
                extendLocationStream << fullPathToFile.c_str() << " unkown line number";
                extendLocation = extendLocationStream.str();
                return false;
            }

            int column = frame->GetColumn();
            if (column < 0) {
                extendLocationStream << fullPathToFile.c_str() << " line:" << lineNumber << " unkown column number";
                extendLocation = extendLocationStream.str();
                return false;
            }

            // Account for the column length offset added by the addition of the Common JS function wrapper
            // See issue https://github.com/NativeScript/android-runtime/issues/975
            if (lineNumber == 1 && hasCommonJsPrologue) {
                column = column - ModuleInternal::MODULE_PROLOGUE_LENGTH;
            }

            extendLocationStream << fullPathToFile.c_str() << "_" << lineNumber << "_" << column << "_";
        }
    }

    extendLocation = extendLocationStream.str();
    return true;
}

MetadataNode* MetadataNode::GetNodeFromHandle(const Local<Object>& value) {
    auto target = ObjectManager::UnwrapProxy(value);
    if (!target->IsObject()) {
        return nullptr;
    }
    auto node = GetInstanceMetadata(Isolate::GetCurrent(), target.As<Object>());
    return node;
}

MetadataEntry MetadataNode::GetChildMetadataForPackage(MetadataNode *node, const std::string &propName) {
    NS_DCHECK(node->m_treeNode->children != nullptr);

    MetadataEntry child(nullptr, NodeType::Class);

    const auto &children = *node->m_treeNode->children;

    for (auto treeNodeChild: children) {
        if (propName == treeNodeChild->name) {
            child.name = propName;
            child.treeNode = treeNodeChild;
            child.type = static_cast<NodeType>(s_metadataReader.GetNodeType(treeNodeChild));

            if (s_metadataReader.IsNodeTypeInterface((uint8_t) child.type)) {
                child.declaringType = TNS_PREFIX + s_metadataReader.ReadTypeName(child.treeNode);
            }
        }
    }

    return child;
}

namespace {
    struct MappedMetadataFile {
        uint8_t* data;
        size_t length;
    };

    /*
     * Maps one of the metadata files read-only rather than reading it into the
     * heap. Its pages are the file's own, clean ones, which the system can drop
     * under memory pressure and load again, where a heap copy is private memory.
     *
     * The whole file is read ahead and mapped up front. Left to fault in on
     * first touch, that cost and its variance land on the first access of each
     * class. Populated without the read-ahead, the pages arrive one read-ahead
     * window at a time and a cold start takes longer than a plain read.
     *
     * The extractor replaces these files by renaming a new one over them, never
     * by writing in place, so a mapping keeps the contents it was made from.
     */
    MappedMetadataFile MapMetadataFile(const string& dir, const char* name) {
        string path = dir + "/" + name;

        void* data = MAP_FAILED;
        struct stat info{};
        int fd = open(path.c_str(), O_RDONLY | O_CLOEXEC);
        if (fd >= 0) {
            if (fstat(fd, &info) == 0 && info.st_size > 0) {
                posix_fadvise(fd, 0, info.st_size, POSIX_FADV_WILLNEED);
                data = mmap(nullptr, static_cast<size_t>(info.st_size), PROT_READ,
                            MAP_PRIVATE | MAP_POPULATE, fd, 0);
            }
            int error = errno;
            close(fd);
            errno = error;
        }

        if (data == MAP_FAILED) {
            stringstream ss;
            ss << "metadata file (" << name << ") couldn't be opened! (Error: ";
            ss << errno;
            ss << ") ";
            throw NativeScriptException(ss.str());
        }

        return {static_cast<uint8_t*>(data), static_cast<size_t>(info.st_size)};
    }
}

void MetadataNode::BuildMetadata(const string& filesPath) {
    string baseDir = filesPath;
    baseDir.append("/metadata");

    DIR* dir = opendir(baseDir.c_str());

    if(dir == nullptr){
        stringstream ss;
        ss << "metadata folder couldn't be opened! (Error: ";
        ss << errno;
        ss << ") ";

        // TODO: Is there a way to detect if the screen is locked as verification
        // We assume based on the error that this is the only way to get this specific error here at this point
        if (errno == ENOENT || errno == EACCES) {
            // Log the error with error code
            __android_log_print(ANDROID_LOG_ERROR, "TNS.error", "%s", ss.str().c_str());

            // While the screen is locked after boot; we cannot access our own apps directory on Android 9+
            // So the only thing to do at this point is just exit normally w/o crashing!

            // The only reason we should be in this specific path; is if:
            // 1) android:directBootAware="true" flag is set on receiver
            // 2) android.intent.action.LOCKED_BOOT_COMPLETED intent is set in manifest on above receiver
            // See:  https://developer.android.com/guide/topics/manifest/receiver-element
            //  and: https://developer.android.com/training/articles/direct-boot
            // This specific path occurs if you using the NativeScript-Local-Notification plugin, the
            // receiver code runs fine, but the app actually doesn't need to startup.  The Native code tries to
            // startup because the receiver is triggered.  So even though we are exiting, the receiver will have
            // done its job

            _Exit(0);
        }
        else {
          throw NativeScriptException(ss.str());
        }
    }

    auto nodes = MapMetadataFile(baseDir, "treeNodeStream.dat");
    auto names = MapMetadataFile(baseDir, "treeStringsStream.dat");
    auto values = MapMetadataFile(baseDir, "treeValueStream.dat");

    DEBUG_WRITE("lenNodes=%zu, lenNames=%zu, lenValues=%zu", nodes.length, names.length, values.length);

    BuildMetadata(nodes.length, nodes.data, names.length, names.data, values.length, values.data);

    // The tree is built from the nodes; the names and the values stay mapped for
    // the life of the process.
    munmap(nodes.data, nodes.length);
}

void MetadataNode::BuildMetadata(uint32_t nodesLength, uint8_t* nodeData, uint32_t nameLength, uint8_t* nameData, uint32_t valueLength, uint8_t* valueData) {
    s_metadataReader.Initialize(nodesLength, nodeData, nameLength, nameData, valueLength, valueData, CallbackHandlers::GetTypeMetadata);
}

void MetadataNode::CreateTopLevelNamespaces(Isolate* isolate, const Local<Object>& global) {
    auto context = isolate->GetCurrentContext();
    auto root = s_metadataReader.GetRoot();

    const auto& children = *root->children;

    for (auto treeNode : children) {
        uint8_t nodeType = s_metadataReader.GetNodeType(treeNode);

        if (nodeType == MetadataTreeNode::PACKAGE) {
            auto node = GetOrCreateInternal(treeNode);

            auto packageObj = node->CreateWrapper(isolate);
            string nameSpace = node->m_treeNode->name;
            // if the namespaces matches a javascript keyword, prefix it with $ to avoid TypeScript and JavaScript errors
            if (IsJavascriptKeyword(nameSpace)) {
                nameSpace = "$" + nameSpace;
            }
            global->Set(context, ArgConverter::ConvertToV8String(isolate, nameSpace), packageObj);
        }
    }
}

MetadataNode::MetadataNodeCache* MetadataNode::GetMetadataNodeCache(Isolate* isolate) {
    // Per runtime; see RuntimeState.h. Null only once the runtime has begun
    // tearing down, which no caller here can legitimately reach.
    auto* cache = RuntimeState::For<MetadataNodeCache>(isolate);
    if (cache == nullptr) {
        throw NativeScriptException("Metadata cache requested after the runtime was torn down");
    }
    return cache;
}

void MetadataNode::EnableProfiler(bool enableProfiler) {
    s_profilerEnabled.store(enableProfiler, std::memory_order_relaxed);
}

bool MetadataNode::IsJavascriptKeyword(const std::string &word) {
    // Initialized in place rather than filled on first call: every runtime's
    // thread reaches this, and a lazy fill races the reads.
    static const set<string> keywords {
        "abstract", "arguments", "boolean", "break", "byte", "case", "catch", "char", "class", "const", "continue", "debugger", "default", "delete", "do",
        "double", "else", "enum", "eval", "export", "extends", "false", "final", "finally", "float", "for", "function", "goto", "if", "implements",
        "import", "in", "instanceof", "int", "interface", "let", "long", "native", "new", "null", "package", "private", "protected", "public", "return",
        "short", "static", "super", "switch", "synchronized", "this", "throw", "throws", "transient", "true", "try", "typeof", "var", "void", "volatile", "while", "with", "yield"
    };

    return keywords.find(word) != keywords.end();
}

Local<Function> MetadataNode::Wrap(Isolate* isolate, const Local<Function>& function, const string& name, const string& origin, bool isCtorFunc) {
    if (!s_profilerEnabled.load(std::memory_order_relaxed) || name == "<init>") {
        return function;
    }

    string actualName = name;
    while (IsJavascriptKeyword(actualName)) {
        actualName.append("_");
    }

    Local<Function> ret;

    stringstream ss;
    ss << "(function() { ";
    ss << "function " << actualName << "() { ";
    if (isCtorFunc) {
        ss << "var args = [null]; for (var i=0; i<arguments.length; i++) { args.push(arguments[i]); }; ";
        ss << "return new (Function.prototype.bind.apply(" << actualName << ".__func, args)); ";
    } else {
        ss << "return " << actualName << ".__func.apply(this, arguments); ";
    }
    ss << "} ";
    ss << "return " << actualName << "; ";
    ss << "})()";

    auto str = ss.str();
    auto source = ArgConverter::ConvertToV8String(isolate, str);
    auto context = isolate->GetCurrentContext();

    TryCatch tc(isolate);

    Local<Script> script;
    ScriptOrigin jsOrigin(ArgConverter::ConvertToV8String(isolate, origin));
    auto maybeScript = Script::Compile(context, source, &jsOrigin).ToLocal(&script);

    if (tc.HasCaught()) {
        throw NativeScriptException(tc, "Cannot compile wrapper");
    }

    if (!script.IsEmpty()) {
        Local<Value> result;
        auto maybeResult = script->Run(context).ToLocal(&result);

        if (!result.IsEmpty()) {
            ret = result.As<Function>();
            ret->Set(context, ArgConverter::ConvertToV8String(isolate, "__func"), function);
            ret->SetName(ArgConverter::ConvertToV8String(isolate, actualName));

            auto prototypePropName = V8StringConstants::GetPrototype(isolate);
            ret->Set(context, prototypePropName, function->Get(context, prototypePropName).ToLocalChecked());
        } else {
            throw NativeScriptException("Cannot create wrapper function");
        }
    } else {
        throw NativeScriptException(str);
    }

    return ret;
}

bool MetadataNode::CheckClassHierarchy(JEnv& env, jclass currentClass, MetadataTreeNode* curentTreeNode, MetadataTreeNode* baseTreeNode) {
    auto shouldSkipBaseClass = false;
    if ((currentClass != nullptr) && (baseTreeNode != curentTreeNode) && (baseTreeNode != nullptr) &&
            (baseTreeNode->offsetValue > 0)) {
        auto baseNode = GetOrCreateInternal(baseTreeNode);
        auto baseClass = env.FindClass(baseNode->m_name);
        if (baseClass != nullptr) {
            auto isBaseClass = env.IsAssignableFrom(currentClass, baseClass) == JNI_TRUE;
            shouldSkipBaseClass = !isBaseClass;
        }
    }
    return shouldSkipBaseClass;
}

void MetadataNode::RegisterSymbolHasInstanceCallback(Isolate* isolate, MetadataEntry& entry, Local<Value> interface) {
    if (interface->IsNullOrUndefined()) {
        return;
    }

    JEnv env;
    auto className = GetJniClassName(entry);
    auto clazz = env.FindClass(className);
    if (clazz == nullptr) {
        return;
    }

    auto extData = External::New(isolate, clazz, v8::kExternalPointerTypeTagDefault);
    auto hasInstanceTemplate = NewNonConstructor(isolate, MetadataNode::SymbolHasInstanceCallback, extData);
    auto context = isolate->GetCurrentContext();
    auto hasInstanceFunc = hasInstanceTemplate->GetFunction(context).ToLocalChecked();
    PropertyDescriptor descriptor(hasInstanceFunc, false);
    auto hasInstanceSymbol = Symbol::GetHasInstance(isolate);
    interface->ToObject(context).ToLocalChecked()->DefineProperty(context, hasInstanceSymbol, descriptor);
}

void MetadataNode::SymbolHasInstanceCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    try {
        auto length = info.Length();
        if (length != 1) {
            throw NativeScriptException(string("Symbol.hasInstance must take exactly 1 argument"));
        }

        auto arg = info[0];
        if (!arg->IsObject()) {
            info.GetReturnValue().Set(false);
            return;
        }

        auto clazz = reinterpret_cast<jclass>(info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));

        auto isolate = info.GetIsolate();
        auto runtime = Runtime::GetRuntime(isolate);

        auto objectManager = runtime->GetObjectManager();
        auto obj = objectManager->GetJavaObjectByJsObject(arg.As<Object>());

        if (obj.IsNull()) {
            // Couldn't find a corresponding java instance counterpart. This could happen
            // if the "instanceof" operator is invoked on a pure javascript instance
            info.GetReturnValue().Set(false);
            return;
        }

        JEnv env;
        auto isInstanceOf = env.IsInstanceOf(obj, clazz);

        info.GetReturnValue().Set(isInstanceOf);
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

std::string MetadataNode::GetJniClassName(MetadataEntry& entry) {
    std::stack<string> s;
    MetadataTreeNode* n = entry.treeNode;
    while (n != nullptr && n->name != "") {
        s.push(n->name);
        n = n->parent;
    }

    string fullClassName;
    while (!s.empty()) {
        auto top = s.top();
        fullClassName = (fullClassName == "") ? top : fullClassName + "/" + top;
        s.pop();
    }

    return fullClassName;
}

const MetadataNode::DefaultMethodNames* MetadataNode::LoadDefaultMethodNames() {
    static const DefaultMethodNames none;

    const DefaultMethodNames* loaded = &none;
    auto names = CallbackHandlers::GetDefaultMethodNames(m_name);
    if (!names.empty()) {
        if (names.size() > 64) {
            names.resize(64);
        }
        loaded = new DefaultMethodNames{std::move(names)};
    }

    const DefaultMethodNames* published = nullptr;
    if (!m_defaultMethodNames.compare_exchange_strong(published, loaded, std::memory_order_acq_rel, std::memory_order_acquire)) {
        if (loaded != &none) {
            delete loaded;
        }
        return published;
    }
    return loaded;
}

uint64_t MetadataNode::GetDefaultOverrides(Isolate* isolate, const Local<Object>& implementationObject) {
    auto defaults = m_defaultMethodNames.load(std::memory_order_acquire);
    if (defaults == nullptr) {
        defaults = LoadDefaultMethodNames();
    }
    if (defaults->names.empty()) {
        return 0;
    }

    auto& keys = GetMetadataNodeCache(isolate)->DefaultMethodNameKeys[this];
    if (keys.empty()) {
        keys.reserve(defaults->names.size());
        for (const auto& name : defaults->names) {
            keys.emplace_back(isolate, String::NewFromUtf8(isolate, name.c_str(), NewStringType::kInternalized).ToLocalChecked());
        }
    }

    auto context = isolate->GetCurrentContext();
    TryCatch tc(isolate);
    uint64_t mask = 0;
    Local<String> getKey;
    Local<String> setKey;
    Local<String> valueKey;
    for (size_t bit = 0; bit < keys.size(); bit++) {
        auto key = keys[bit].Get(isolate);
        if (!implementationObject->HasOwnProperty(context, key).FromMaybe(false)) {
            continue;
        }

        Local<Value> descriptor;
        if (!implementationObject->GetOwnPropertyDescriptor(context, key).ToLocal(&descriptor) || !descriptor->IsObject()) {
            continue;
        }
        if (getKey.IsEmpty()) {
            getKey = String::NewFromUtf8Literal(isolate, "get");
            setKey = String::NewFromUtf8Literal(isolate, "set");
            valueKey = String::NewFromUtf8Literal(isolate, "value");
        }
        auto fields = descriptor.As<Object>();
        Local<Value> value;
        bool isAccessor = fields->HasOwnProperty(context, getKey).FromMaybe(false) || fields->HasOwnProperty(context, setKey).FromMaybe(false);
        if (isAccessor || (fields->Get(context, valueKey).ToLocal(&value) && value->IsFunction())) {
            mask |= uint64_t{1} << bit;
        }
    }

    // Only a Proxy implementation object can throw here, from one of its traps.
    if (tc.HasCaught()) {
        throw NativeScriptException(tc, "Could not read the properties of the implementation object");
    }
    return mask;
}

const char* const MetadataNode::DEFAULT_OVERRIDING_SUFFIX = "-defaults";
string MetadataNode::TNS_PREFIX = "com/tns/gen/";
MetadataReader MetadataNode::s_metadataReader;
robin_hood::unordered_map<std::string, MetadataNode*> MetadataNode::s_name2NodeCache;
robin_hood::unordered_map<std::string, MetadataTreeNode*> MetadataNode::s_name2TreeNodeCache;
robin_hood::unordered_map<MetadataTreeNode*, MetadataNode*> MetadataNode::s_treeNode2NodeCache;
std::mutex MetadataNode::s_nodeCacheMutex;
std::atomic<bool> MetadataNode::s_profilerEnabled{false};
std::atomic<const MetadataNode::ContentKeyedBindings*> MetadataNode::s_contentKeyedBindings{nullptr};

