#include "MetadataNode.h"
#include "NativeScriptAssert.h"
#include "Constants.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "SimpleProfiler.h"
#include "JniLocalRef.h"
#include "CallbackHandlers.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include <sstream>
#include <cctype>
#include <assert.h>

using namespace v8;
using namespace std;
using namespace tns;

void MetadataNode::Init(Isolate *isolate) {
    auto key = ConvertToV8String("tns::MetadataKey");
    auto cache = GetCache(isolate);
    cache->MetadataKey = new Persistent<String>(isolate, key);
}

MetadataNode::MetadataNode(MetadataTreeNode *treeNode) :
        m_treeNode(treeNode), m_poCtorFunc(nullptr) {
    uint8_t nodeType = s_metadataReader.GetNodeType(treeNode);

    m_name = s_metadataReader.ReadTypeName(m_treeNode);

    uint8_t parentNodeType = s_metadataReader.GetNodeType(treeNode->parent);

    m_isArray = s_metadataReader.IsNodeTypeArray(parentNodeType);

    bool isInterface = s_metadataReader.IsNodeTypeInterface(nodeType);

    if (!m_isArray && isInterface) {
        bool isPrefix;
        auto impTypeName = s_metadataReader.ReadInterfaceImplementationTypeName(m_treeNode,
                                                                                isPrefix);
        m_implType = isPrefix
                     ? (impTypeName + m_name)
                     :
                     impTypeName;
    }
}

Local<Object> MetadataNode::CreateExtendedJSWrapper(Isolate *isolate, ObjectManager *objectManager,
                                                    const string &proxyClassName) {
    Local<Object> extInstance;

    auto cacheData = GetCachedExtendedClassData(isolate, proxyClassName);
    if (cacheData.node != nullptr) {
        extInstance = objectManager->GetEmptyObject(isolate);
        extInstance->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper),
                                      True(isolate));
        auto extdCtorFunc = Local<Function>::New(isolate, *cacheData.extendedCtorFunction);
        extInstance->SetPrototype(extdCtorFunc->Get(ConvertToV8String("prototype")));

        SetInstanceMetadata(isolate, extInstance, cacheData.node);
    }

    return extInstance;
}

MetadataNode *MetadataNode::GetOrCreate(const string &className) {
    MetadataNode *node = nullptr;

    auto it = s_name2NodeCache.find(className);

    if (it == s_name2NodeCache.end()) {
        MetadataTreeNode *treeNode = GetOrCreateTreeNodeByName(className);

        node = GetOrCreateInternal(treeNode);

        s_name2NodeCache.insert(make_pair(className, node));
    }
    else {
        node = it->second;
    }

    return node;
}

MetadataNode *MetadataNode::GetOrCreateInternal(MetadataTreeNode *treeNode) {
    MetadataNode *result = nullptr;

    auto it = s_treeNode2NodeCache.find(treeNode);

    if (it != s_treeNode2NodeCache.end()) {
        result = it->second;
    }
    else {
        result = new MetadataNode(treeNode);

        s_treeNode2NodeCache.insert(make_pair(treeNode, result));
    }

    return result;
}

MetadataTreeNode *MetadataNode::GetOrCreateTreeNodeByName(const string &className) {
    MetadataTreeNode *result = nullptr;

    auto itFound = s_name2TreeNodeCache.find(className);

    if (itFound != s_name2TreeNodeCache.end()) {
        result = itFound->second;
    }
    else {
        result = s_metadataReader.GetOrCreateTreeNodeByName(className);

        s_name2TreeNodeCache.insert(make_pair(className, result));
    }

    return result;
}

string MetadataNode::GetName() {
    return m_name;
}

string MetadataNode::GetTypeMetadataName(Isolate *isolate, Local<Value> &value) {
    auto data = GetTypeMetadata(isolate, value.As<Function>());

    return data->name;
}

Local<Object> MetadataNode::CreateWrapper(Isolate *isolate) {
    EscapableHandleScope handle_scope(isolate);

    Local<Object> obj;

    uint8_t nodeType = s_metadataReader.GetNodeType(m_treeNode);

    bool isClass = s_metadataReader.IsNodeTypeClass(nodeType),
            isInterface = s_metadataReader.IsNodeTypeInterface(nodeType);

    if (isClass || isInterface) {
        obj = GetConstructorFunction(isolate);
    }
    else if (s_metadataReader.IsNodeTypePackage(nodeType)) {
        obj = CreatePackageObject(isolate);
    }
    else {
        stringstream ss;
        ss << "(InternalError): Can't create proxy for this type=" << static_cast<int>(nodeType);
        throw NativeScriptException(ss.str());
    }

    return handle_scope.Escape(obj);
}

Local<Object> MetadataNode::CreateJSWrapper(Isolate *isolate, ObjectManager *objectManager) {
    Local<Object> obj;

    if (m_isArray) {
        obj = CreateArrayWrapper(isolate);
    }
    else {
        obj = objectManager->GetEmptyObject(isolate);
        if (!obj.IsEmpty()) {
            auto ctorFunc = GetConstructorFunction(isolate);

            // TODO: Pete: VERIFY THAT OBJECT -> SET(FUNCTION) works
            obj->Set(ConvertToV8String("constructor"), ctorFunc);
            obj->SetPrototype(ctorFunc->Get(ConvertToV8String("prototype")));
            SetInstanceMetadata(isolate, obj, this);
        }
    }

    return obj;
}

void MetadataNode::ArrayLengthGetterCallack(Local<String> property,
                                            const PropertyCallbackInfo<Value> &info) {
    try {
        auto thiz = info.This();
        auto isolate = info.GetIsolate();
        auto length = CallbackHandlers::GetArrayLength(isolate, thiz);
        info.GetReturnValue().Set(Integer::New(isolate, length));
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

Local<Object> MetadataNode::CreateArrayWrapper(Isolate *isolate) {
    auto node = GetOrCreate("java/lang/Object");
    auto objPrototype = node->GetConstructorFunction(isolate);

    auto arrayObjectTemplate = ObjectTemplate::New(isolate);
    arrayObjectTemplate->SetInternalFieldCount(
            static_cast<int>(ObjectManager::MetadataNodeKeys::END));
    arrayObjectTemplate->SetIndexedPropertyHandler(ArrayIndexedPropertyGetterCallback,
                                                   ArrayIndexedPropertySetterCallback);

    auto arr = arrayObjectTemplate->NewInstance();
    arr->SetPrototype(objPrototype->Get(ConvertToV8String("prototype")));
    arr->SetAccessor(ConvertToV8String("length"), ArrayLengthGetterCallack, nullptr, Local<Value>(),
                     AccessControl::ALL_CAN_READ, PropertyAttribute::DontDelete);

    SetInstanceMetadata(isolate, arr, this);

    return arr;
}

Local<Object> MetadataNode::CreatePackageObject(Isolate *isolate) {
    auto packageObj = Object::New(isolate);
    auto ptrChildren = this->m_treeNode->children;
    if (ptrChildren != nullptr) {
        auto ctx = isolate->GetCurrentContext();
        auto extData = External::New(isolate, this);
        const auto &children = *ptrChildren;
        for (auto childNode: children) {
            packageObj->SetAccessor(ctx, ConvertToV8String(childNode->name),
                                    PackageGetterCallback,
                                    nullptr,
                                    extData);
        }
    }

    return packageObj;
}

void MetadataNode::SetClassAccessor(Local<Function> &ctorFunction) {
    auto classFieldName = ConvertToV8String("class");
    ctorFunction->SetAccessor(classFieldName, ClassAccessorGetterCallback, nullptr, Local<Value>(),
                              AccessControl::ALL_CAN_READ, PropertyAttribute::DontDelete);
}

void MetadataNode::ClassAccessorGetterCallback(Local<String> property,
                                               const PropertyCallbackInfo<Value> &info) {
    try {
        auto thiz = info.This();
        auto isolate = info.GetIsolate();
        auto func = thiz.As<Function>();
        auto data = GetTypeMetadata(isolate, func);

        auto value = CallbackHandlers::FindClass(isolate, data->name);
        info.GetReturnValue().Set(value);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::NullObjectAccessorGetterCallback(Local<String> property,
                                                    const PropertyCallbackInfo<Value> &info) {
    try {
        DEBUG_WRITE("NullObjectAccessorGetterCallback");
        auto isolate = info.GetIsolate();

        auto thiz = info.This();
        Local<Private> propName = Private::New(isolate, V8StringConstants::GetNullNodeName());
        auto maybeValue = thiz->GetPrivate(isolate->GetCurrentContext(), propName);
        Local<Value> hiddenValue;

        maybeValue.FromMaybe(hiddenValue);
        if (hiddenValue.IsEmpty()) {
            auto node = reinterpret_cast<MetadataNode *>(info.Data().As<External>()->Value());
            thiz->SetPrivate(isolate->GetCurrentContext(), propName, External::New(isolate, node));
            auto funcTemplate = FunctionTemplate::New(isolate, MetadataNode::NullValueOfCallback);
            thiz->Delete(V8StringConstants::GetValueOf());
            thiz->Set(V8StringConstants::GetValueOf(), funcTemplate->GetFunction());
        }

        info.GetReturnValue().Set(thiz);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::NullValueOfCallback(const FunctionCallbackInfo<Value> &args) {
    try {
        auto isolate = args.GetIsolate();
        args.GetReturnValue().Set(Null(isolate));
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::FieldAccessorGetterCallback(Local<String> property,
                                               const PropertyCallbackInfo<Value> &info) {
    try {
        auto thiz = info.This();
        auto fieldCallbackData = reinterpret_cast<FieldCallbackData *>(info.Data().As<External>()->Value());

        if (!fieldCallbackData->isStatic && thiz->StrictEquals(info.Holder())) {
            info.GetReturnValue().SetUndefined();
            return;
        }

        auto isolate = info.GetIsolate();
        auto value = CallbackHandlers::GetJavaField(isolate, thiz, fieldCallbackData);
        info.GetReturnValue().Set(value);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::FieldAccessorSetterCallback(Local<String> property, Local<Value> value,
                                               const PropertyCallbackInfo<void> &info) {
    try {
        auto thiz = info.This();
        auto fieldCallbackData = reinterpret_cast<FieldCallbackData *>(info.Data().As<External>()->Value());

        if (!fieldCallbackData->isStatic && thiz->StrictEquals(info.Holder())) {
            info.GetReturnValue().SetUndefined();
            return;
        }

        if (fieldCallbackData->isFinal) {
            stringstream ss;
            ss << "You are trying to set \"" << fieldCallbackData->name <<
            "\" which is a final field! Final fields can only be read.";
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }
        else {
            auto isolate = info.GetIsolate();
            CallbackHandlers::SetJavaField(isolate, thiz, value, fieldCallbackData);
            info.GetReturnValue().Set(value);
        }
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::SuperAccessorGetterCallback(Local<String> property,
                                               const PropertyCallbackInfo<Value> &info) {
    try {
        auto thiz = info.This();
        auto isolate = info.GetIsolate();
        auto key = Private::New(isolate, ConvertToV8String("supervalue"));
        auto maybeSuperValue = thiz->GetPrivate(isolate->GetCurrentContext(), key);
        Local<Object> superObject;
        Local<Value> superValue;

        if (!maybeSuperValue.IsEmpty()) {
            maybeSuperValue.FromMaybe(superValue);
            superObject = superValue.As<Object>();

            if (superValue.IsEmpty()) {
                auto runtime = Runtime::GetRuntime(isolate);
                auto objectManager = runtime->GetObjectManager();

                superValue = objectManager->GetEmptyObject(isolate);
                bool d = superObject->Delete(V8StringConstants::GetToString());
                d = superObject->Delete(V8StringConstants::GetValueOf());
                superObject->SetInternalField(
                        static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper),
                        True(isolate));

                superObject->SetPrototype(
                        thiz->GetPrototype().As<Object>()->GetPrototype().As<Object>()->GetPrototype());
                thiz->SetPrivate(isolate->GetCurrentContext(), key, superObject);
                objectManager->CloneLink(thiz, superObject);

                DEBUG_WRITE("superValue.GetPrototype=%d",
                            superObject->GetPrototype().As<Object>()->GetIdentityHash());

                auto node = GetInstanceMetadata(isolate, thiz);
                SetInstanceMetadata(isolate, superObject, node);
            }
        }

        info.GetReturnValue().Set(superObject);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::SetInstanceMembers(Isolate *isolate, Local<FunctionTemplate> &ctorFuncTemplate,
                                      Local<ObjectTemplate> &prototypeTemplate,
                                      vector<MethodCallbackData *> &instanceMethodsCallbackData,
                                      const vector<MethodCallbackData *> &baseInstanceMethodsCallbackData,
                                      MetadataTreeNode *treeNode) {
    auto hasCustomMetadata = treeNode->metadata != nullptr;

    if (hasCustomMetadata) {
        SetInstanceMembersFromRuntimeMetadata(isolate, ctorFuncTemplate, prototypeTemplate,
                                              instanceMethodsCallbackData,
                                              baseInstanceMethodsCallbackData, treeNode);
    }
    else {
        SetInstanceMembersFromStaticMetadata(isolate, ctorFuncTemplate, prototypeTemplate,
                                             instanceMethodsCallbackData,
                                             baseInstanceMethodsCallbackData, treeNode);
    }
}

void MetadataNode::TestCrash(const v8::FunctionCallbackInfo<v8::Value> &info) {

}

void MetadataNode::SetInstanceMembersFromStaticMetadata(Isolate *isolate,
                                                        Local<FunctionTemplate> &ctorFuncTemplate,
                                                        Local<ObjectTemplate> &prototypeTemplate,
                                                        vector<MethodCallbackData *> &instanceMethodsCallbackData,
                                                        const vector<MethodCallbackData *> &baseInstanceMethodsCallbackData,
                                                        MetadataTreeNode *treeNode) {
    SET_PROFILER_FRAME();

    Local<Function> ctorFunction;

    prototypeTemplate->Set(ConvertToV8String(std::string("ctor")),
                           FunctionTemplate::New(isolate, MethodCallback));

    uint8_t *curPtr = s_metadataReader.GetValueData() + treeNode->offsetValue + 1;

    auto nodeType = s_metadataReader.GetNodeType(treeNode);

    auto curType = s_metadataReader.ReadTypeName(treeNode);

    curPtr += sizeof(uint16_t /* baseClassId */);

    if (s_metadataReader.IsNodeTypeInterface(nodeType)) {
        curPtr += sizeof(uint8_t) + sizeof(uint32_t);
    }

    //get candidates from instance methods metadata
    auto instanceMethodCout = *reinterpret_cast<uint16_t *>(curPtr);
    curPtr += sizeof(uint16_t);
    string lastMethodName;
    MethodCallbackData *callbackData = nullptr;

    auto origin = Constants::APP_ROOT_FOLDER_PATH + GetOrCreateInternal(treeNode)->m_name;

    for (auto i = 0; i < instanceMethodCout; i++) {
        auto entry = s_metadataReader.ReadInstanceMethodEntry(&curPtr);

        // attach a function to the prototype of a javascript Object
        if (entry.name != lastMethodName) {
            callbackData = new MethodCallbackData(this);

            instanceMethodsCallbackData.push_back(callbackData);
            auto itBegin = baseInstanceMethodsCallbackData.begin();
            auto itEnd = baseInstanceMethodsCallbackData.end();
            auto itFound = find_if(itBegin, itEnd, [&entry](MethodCallbackData *x) {
                return x->candidates.front().name == entry.name;
            });
            if (itFound != itEnd) {
                callbackData->parent = *itFound;
            }

            auto funcData = External::New(isolate, callbackData);
            auto funcTemplate = FunctionTemplate::New(isolate, MethodCallback, funcData);
            auto func = funcTemplate->GetFunction();
            auto funcName = ConvertToV8String(entry.name);
            // TODO: Pete: fix the WRAP functionality and reintroduce it
            // TODO: Pete: NEW V8's Set can only take a primitive for a value or a template
            prototypeTemplate->Set(funcName,
                                   funcTemplate); // Wrap(isolate, func, entry.name, origin, false /* isCtorFunc */));
            lastMethodName = entry.name;
        }

        callbackData->candidates.push_back(entry);
    }

    //get candidates from instance fields metadata
    auto instanceFieldCout = *reinterpret_cast<uint16_t *>(curPtr);
    curPtr += sizeof(uint16_t);
    for (auto i = 0; i < instanceFieldCout; i++) {
        auto entry = s_metadataReader.ReadInstanceFieldEntry(&curPtr);

        auto fieldName = ConvertToV8String(entry.name);
        auto fieldInfo = new FieldCallbackData(entry);
        fieldInfo->declaringType = curType;
        auto fieldData = External::New(isolate, fieldInfo);
        prototypeTemplate->SetAccessor(fieldName, FieldAccessorGetterCallback,
                                       FieldAccessorSetterCallback, fieldData,
                                       AccessControl::DEFAULT, PropertyAttribute::DontDelete);
    }
}

void MetadataNode::SetInstanceMembersFromRuntimeMetadata(Isolate *isolate,
                                                         Local<FunctionTemplate> &ctorFuncTemplate,
                                                         Local<ObjectTemplate> &prototypeTemplate,
                                                         vector<MethodCallbackData *> &instanceMethodsCallbackData,
                                                         const vector<MethodCallbackData *> &baseInstanceMethodsCallbackData,
                                                         MetadataTreeNode *treeNode) {
    SET_PROFILER_FRAME();

    assert(treeNode->metadata != nullptr);

    string line;
    const string &metadata = *treeNode->metadata;
    stringstream s(metadata);

    string kind;
    string name;
    string signature;
    int paramCount;

    getline(s, line); // type line
    getline(s, line); // base class line

    string lastMethodName;
    MethodCallbackData *callbackData = nullptr;

    while (getline(s, line)) {
        stringstream tmp(line);
        tmp >> kind >> name >> signature >> paramCount;

        char chKind = kind[0];

        // method or field
        assert((chKind == 'M') || (chKind == 'F'));

        MetadataEntry entry;
        entry.name = name;
        entry.sig = signature;
        MetadataReader::FillReturnType(entry);
        entry.paramCount = paramCount;
        entry.isStatic = false;

        if (chKind == 'M') {
            if (entry.name != lastMethodName) {
                callbackData = new MethodCallbackData(this);

                instanceMethodsCallbackData.push_back(callbackData);
                auto itBegin = baseInstanceMethodsCallbackData.begin();
                auto itEnd = baseInstanceMethodsCallbackData.end();
                auto itFound = find_if(itBegin, itEnd, [&entry](MethodCallbackData *x) {
                    return x->candidates.front().name == entry.name;
                });
                if (itFound != itEnd) {
                    callbackData->parent = *itFound;
                }

                auto funcData = External::New(isolate, callbackData);
                auto funcTemplate = FunctionTemplate::New(isolate, MethodCallback, funcData);
                auto func = funcTemplate->GetFunction();
                auto funcName = ConvertToV8String(entry.name);
                // TODO: Pete: NEW V8's Set can only take a primitive for a value or a template
                prototypeTemplate->Set(funcName, funcTemplate);
                lastMethodName = entry.name;
            }
            callbackData->candidates.push_back(entry);
        }
        else if (chKind == 'F') {
            auto fieldName = ConvertToV8String(entry.name);
            auto fieldData = External::New(isolate, new FieldCallbackData(entry));
            auto access = entry.isFinal ? AccessControl::ALL_CAN_READ : AccessControl::DEFAULT;
            prototypeTemplate->SetAccessor(fieldName, FieldAccessorGetterCallback,
                                           FieldAccessorSetterCallback, fieldData, access,
                                           PropertyAttribute::DontDelete);
        }
    }
}

void MetadataNode::SetStaticMembers(Isolate *isolate, Local<Function> &ctorFunction,
                                    MetadataTreeNode *treeNode) {
    auto hasCustomMetadata = treeNode->metadata != nullptr;

    if (!hasCustomMetadata) {
        uint8_t *curPtr = s_metadataReader.GetValueData() + treeNode->offsetValue + 1;
        auto nodeType = s_metadataReader.GetNodeType(treeNode);
        auto curType = s_metadataReader.ReadTypeName(treeNode);
        curPtr += sizeof(uint16_t /* baseClassId */);
        if (s_metadataReader.IsNodeTypeInterface(nodeType)) {
            curPtr += sizeof(uint8_t) + sizeof(uint32_t);
        }
        auto instanceMethodCout = *reinterpret_cast<uint16_t *>(curPtr);
        curPtr += sizeof(uint16_t);
        for (auto i = 0; i < instanceMethodCout; i++) {
            auto entry = s_metadataReader.ReadInstanceMethodEntry(&curPtr);
        }
        auto instanceFieldCout = *reinterpret_cast<uint16_t *>(curPtr);
        curPtr += sizeof(uint16_t);
        for (auto i = 0; i < instanceFieldCout; i++) {
            auto entry = s_metadataReader.ReadInstanceFieldEntry(&curPtr);
        }

        string lastMethodName;
        MethodCallbackData *callbackData = nullptr;

        auto origin = Constants::APP_ROOT_FOLDER_PATH + GetOrCreateInternal(treeNode)->m_name;

        //get candidates from static methods metadata
        auto staticMethodCout = *reinterpret_cast<uint16_t *>(curPtr);
        curPtr += sizeof(uint16_t);
        for (auto i = 0; i < staticMethodCout; i++) {
            auto entry = s_metadataReader.ReadStaticMethodEntry(&curPtr);
            if (entry.name != lastMethodName) {
                callbackData = new MethodCallbackData(this);
                auto funcData = External::New(isolate, callbackData);
                auto funcTemplate = FunctionTemplate::New(isolate, MethodCallback, funcData);
                auto func = funcTemplate->GetFunction();
                auto funcName = ConvertToV8String(entry.name);
                ctorFunction->Set(funcName,
                                  Wrap(isolate, func, entry.name, origin, false /* isCtorFunc */));
                lastMethodName = entry.name;
            }
            callbackData->candidates.push_back(entry);
        }

        //attach .extend function
        auto extendFuncName = V8StringConstants::GetExtend();
        auto extendFuncTemplate = FunctionTemplate::New(isolate, ExtendCallMethodCallback,
                                                        External::New(isolate, this));
        ctorFunction->Set(extendFuncName, extendFuncTemplate->GetFunction());

        //get candidates from static fields metadata
        auto staticFieldCout = *reinterpret_cast<uint16_t *>(curPtr);
        curPtr += sizeof(uint16_t);
        for (auto i = 0; i < staticFieldCout; i++) {
            auto entry = s_metadataReader.ReadStaticFieldEntry(&curPtr);

            auto fieldName = ConvertToV8String(entry.name);
            auto fieldData = External::New(isolate, new FieldCallbackData(entry));
            ctorFunction->SetAccessor(fieldName, FieldAccessorGetterCallback,
                                      FieldAccessorSetterCallback, fieldData,
                                      AccessControl::DEFAULT, PropertyAttribute::DontDelete);
        }

        auto nullObjectName = V8StringConstants::GetNullObject();

        Local<Value> nullObjectData = External::New(isolate, this);
        ctorFunction->SetAccessor(nullObjectName, NullObjectAccessorGetterCallback, nullptr,
                                  nullObjectData);

        SetClassAccessor(ctorFunction);
    }
}


void MetadataNode::InnerClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
    try {
        auto thiz = info.This();
        auto isolate = info.GetIsolate();
        auto data = reinterpret_cast<InnerClassData *>(info.Data().As<External>()->Value());

        auto outerThis = Local<Object>::New(isolate, *data->outerThis);
        string extendName;
        auto className = data->node->m_name;

        SetInstanceMetadata(info.GetIsolate(), thiz, data->node);

        ArgsWrapper argWrapper(info, ArgType::Class, outerThis);

        string fullClassName = CreateFullClassName(className, extendName);
        bool success = CallbackHandlers::RegisterInstance(isolate, thiz, fullClassName, argWrapper,
                                                          outerThis, false);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::InnerClassAccessorGetterCallback(Local<String> property,
                                                    const PropertyCallbackInfo<Value> &info) {
    try {
        auto isolate = info.GetIsolate();
        auto thiz = info.This();
        auto node = reinterpret_cast<MetadataNode *>(info.Data().As<External>()->Value());

        auto innerKey = Private::New(isolate, ConvertToV8String("inner:" + node->m_treeNode->name));

        auto maybeInnerTypeCtorFunc = thiz->GetPrivate(isolate->GetCurrentContext(), innerKey);
        Local<Value> valFromMaybe;

        if (!maybeInnerTypeCtorFunc.IsEmpty()) {
            maybeInnerTypeCtorFunc.FromMaybe(valFromMaybe);

            if (valFromMaybe.IsEmpty()) {
                auto innerTypeCtorFunc = valFromMaybe.As<Function>();
                auto funcTemplate = node->GetConstructorFunctionTemplate(isolate, node->m_treeNode);
                auto ctorFunc = funcTemplate->GetFunction();

                auto innerClassData = External::New(isolate, new InnerClassData(
                        new Persistent<Object>(isolate, thiz), node));
                auto innerTypeCtorFuncTemplate = FunctionTemplate::New(isolate,
                                                                       InnerClassConstructorCallback,
                                                                       innerClassData);
                innerTypeCtorFuncTemplate->InstanceTemplate()->SetInternalFieldCount(
                        static_cast<int>(ObjectManager::MetadataNodeKeys::END));
                innerTypeCtorFunc = innerTypeCtorFuncTemplate->GetFunction();
                auto prototypeName = ConvertToV8String("prototype");
                auto innerTypeCtorFuncPrototype = innerTypeCtorFunc->Get(
                        prototypeName).As<Object>();
                innerTypeCtorFuncPrototype->SetPrototype(ctorFunc->Get(prototypeName));

                thiz->SetPrivate(isolate->GetCurrentContext(), innerKey, innerTypeCtorFunc);
            }
        }

        info.GetReturnValue().Set(valFromMaybe);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::SetInnnerTypes(Isolate *isolate, Local<Function> &ctorFunction,
                                  MetadataTreeNode *treeNode) {
    if (treeNode->children != nullptr) {
        const auto &children = *treeNode->children;

        auto prototypeTemplate2 = ctorFunction->Get(ConvertToV8String("prototype")).As<Object>();

        for (auto curChild : children) {
            auto childNode = GetOrCreateInternal(curChild);

            auto type = s_metadataReader.GetNodeType(curChild);
            auto isStatic = s_metadataReader.IsNodeTypeStatic(type);

            if (isStatic) {
                auto innerTypeCtorFuncTemplate = childNode->GetConstructorFunctionTemplate(isolate,
                                                                                           curChild);
                auto innerTypeCtorFunc = Local<Function>::New(isolate, *GetOrCreateInternal(
                        curChild)->m_poCtorFunc);
                auto innerTypeName = ConvertToV8String(curChild->name);
                // TODO: Pete: NEW V8's Set can only take a primitive for a value or a template
                // TODO: change func to funcTemplate
                ctorFunction->Set(innerTypeName, innerTypeCtorFunc);
            }
            else {
                auto innerTypeName = ConvertToV8String(curChild->name);
                prototypeTemplate2->SetAccessor(innerTypeName, InnerClassAccessorGetterCallback,
                                                nullptr, External::New(isolate, childNode));
            }
        }
    }
}

Local<FunctionTemplate> MetadataNode::GetConstructorFunctionTemplate(Isolate *isolate,
                                                                     MetadataTreeNode *treeNode) {
    vector < MethodCallbackData * > instanceMethodsCallbackData;

    auto ft = GetConstructorFunctionTemplate(isolate, treeNode, instanceMethodsCallbackData);

    return ft;
}

Local<FunctionTemplate> MetadataNode::GetConstructorFunctionTemplate(Isolate *isolate,
                                                                     MetadataTreeNode *treeNode,
                                                                     vector<MethodCallbackData *> &instanceMethodsCallbackData) {
    SET_PROFILER_FRAME();

    Local<FunctionTemplate> ctorFuncTemplate;
    auto cache = GetCache(isolate);
    auto itFound = cache->CtorFuncCache.find(treeNode);
    if (itFound != cache->CtorFuncCache.end()) {
        auto &ctorCacheItem = itFound->second;
        instanceMethodsCallbackData = ctorCacheItem.instanceMethodCallbacks;
        ctorFuncTemplate = Local<FunctionTemplate>::New(isolate, *ctorCacheItem.ft);
        return ctorFuncTemplate;
    }

    auto node = GetOrCreateInternal(treeNode);
    auto ctorCallbackData = External::New(isolate, node);
    auto isInterface = s_metadataReader.IsNodeTypeInterface(treeNode->type);
    auto funcCallback = isInterface ? InterfaceConstructorCallback : ClassConstructorCallback;
    ctorFuncTemplate = FunctionTemplate::New(isolate, funcCallback, ctorCallbackData);
    ctorFuncTemplate->InstanceTemplate()->SetInternalFieldCount(
            static_cast<int>(ObjectManager::MetadataNodeKeys::END));

    auto baseTreeNode = s_metadataReader.GetBaseClassNode(treeNode);
    Local<Function> baseCtorFunc;
    vector < MethodCallbackData * > baseInstanceMethodsCallbackData;
    if ((baseTreeNode != treeNode) && (baseTreeNode != nullptr) &&
        (baseTreeNode->offsetValue > 0)) {
        auto baseFuncTemplate = GetConstructorFunctionTemplate(isolate, baseTreeNode,
                                                               baseInstanceMethodsCallbackData);
        if (!baseFuncTemplate.IsEmpty()) {
            ctorFuncTemplate->Inherit(baseFuncTemplate);
            baseCtorFunc = Local<Function>::New(isolate,
                                                *GetOrCreateInternal(baseTreeNode)->m_poCtorFunc);
        }
    }

    auto prototypeTemplate = ctorFuncTemplate->PrototypeTemplate();

    node->SetInstanceMembers(isolate, ctorFuncTemplate, prototypeTemplate,
                             instanceMethodsCallbackData, baseInstanceMethodsCallbackData,
                             treeNode);

    auto ctorFunc = ctorFuncTemplate->GetFunction();

    auto origin = Constants::APP_ROOT_FOLDER_PATH + node->m_name;

    auto wrappedCtorFunc = Wrap(isolate, ctorFunc, node->m_treeNode->name, origin,
                                true /* isCtorFunc */);

    node->SetStaticMembers(isolate, wrappedCtorFunc, treeNode);

    node->m_poCtorFunc = new Persistent<Function>(isolate, wrappedCtorFunc);
    if (!baseCtorFunc.IsEmpty()) {
        wrappedCtorFunc->SetPrototype(baseCtorFunc);
    }

    auto pft = new Persistent<FunctionTemplate>(isolate, ctorFuncTemplate);
    CtorCacheItem ctorCacheItem(pft, instanceMethodsCallbackData);
    cache->CtorFuncCache.insert(make_pair(treeNode, ctorCacheItem));

    SetInnnerTypes(isolate, wrappedCtorFunc, treeNode);

    SetTypeMetadata(isolate, wrappedCtorFunc,
                    new TypeMetadata(s_metadataReader.ReadTypeName(treeNode)));

    return ctorFuncTemplate;
}

Local<Function> MetadataNode::GetConstructorFunction(Isolate *isolate) {
    auto ctorFuncTemplate = GetConstructorFunctionTemplate(isolate, m_treeNode);
    auto ctorFunc = Local<Function>::New(isolate, *m_poCtorFunc);
    return ctorFunc;
}

MetadataNode::TypeMetadata *MetadataNode::GetTypeMetadata(Isolate *isolate,
                                                          const Local<Function> &value) {
    auto maybeValue = value->GetPrivate(isolate->GetCurrentContext(), Private::New(isolate,
                                                                                   tns::ConvertToV8String(
                                                                                           "typemetadata")));
    Local<Value> valFromMaybe;

    if (!maybeValue.IsEmpty()) {
        maybeValue.FromMaybe(valFromMaybe);

        if (valFromMaybe.IsEmpty()) {
            stringstream ss;
            ss << "Failed to GetTypeMetadata: GetPrivate of the function returned nil";
            NativeScriptException nsEx(ss.str());
            nsEx.ReThrowToV8();
        } else {
            auto data = reinterpret_cast<TypeMetadata *>(valFromMaybe.As<External>()->Value());

            return data;
        }
    } else {
        stringstream ss;
        ss << "Failed to GetTypeMetadata";
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::SetTypeMetadata(Isolate *isolate, Local<Function> value, TypeMetadata *data) {
    auto privateKey = Private::New(isolate,tns::ConvertToV8String("typemetadata"));
    auto res = value->SetPrivate(isolate->GetCurrentContext(),
                                 privateKey,
                                 External::New(isolate, data));

    auto hasPrivate = value->HasPrivate(isolate->GetCurrentContext(), privateKey);
    TypeMetadata* dataa;

    Local<Value> valFromMaybe;
    value->GetPrivate(isolate->GetCurrentContext(), privateKey).FromMaybe(valFromMaybe);//.ToLocal(valFromMaybe);



    if (!valFromMaybe->IsEmpty()) {
        dataa = reinterpret_cast<TypeMetadata *>(valFromMaybe->As<External>()->Value());
    }

    if (res.IsNothing()) {
        stringstream ss;
        ss << "Failed to SetTypeMetadata for: " << data->name.c_str() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
}

MetadataNode *MetadataNode::GetInstanceMetadata(Isolate *isolate, const Local<Object> &value) {
    MetadataNode *node = nullptr;
    auto cache = GetCache(isolate);
    auto key = Private::New(isolate, Local<String>::New(isolate, *cache->MetadataKey));
    auto ext = value->GetPrivate(isolate->GetCurrentContext(), key);
    Local<Value> extFromMaybe;

    if (!ext.IsEmpty()) {
        ext.FromMaybe(extFromMaybe);
        node = reinterpret_cast<MetadataNode *>(extFromMaybe.As<External>()->Value());
    }

    return node;
}

void MetadataNode::SetInstanceMetadata(Isolate *isolate, Local<Object> value, MetadataNode *node) {
    auto cache = GetCache(isolate);
    auto key = Private::New(isolate, Local<String>::New(isolate, *cache->MetadataKey));
    value->SetPrivate(isolate->GetCurrentContext(), key, External::New(isolate, node));
}

void MetadataNode::ExtendedClassConstructorCallback(
        const v8::FunctionCallbackInfo<v8::Value> &info) {
    try {
        SET_PROFILER_FRAME();

        assert(info.IsConstructCall());
        auto isolate = info.GetIsolate();
        auto thiz = info.This();
        auto extData = reinterpret_cast<ExtendedClassData *>(info.Data().As<External>()->Value());

        auto implementationObject = Local<Object>::New(isolate, *extData->implementationObject);

        const auto &extendName = extData->extendedName;

        SetInstanceMetadata(isolate, thiz, extData->node);
        thiz->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper),
                               True(isolate));
        thiz->SetPrivate(isolate->GetCurrentContext(),
                         Private::New(isolate, ConvertToV8String("t::implObj")),
                         implementationObject);

        ArgsWrapper argWrapper(info, ArgType::Class, Local<Object>());

        string fullClassName = extData->fullClassName;

        bool success = CallbackHandlers::RegisterInstance(isolate, thiz, fullClassName, argWrapper,
                                                          implementationObject, false);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::InterfaceConstructorCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
    try {
        SET_PROFILER_FRAME();

        auto isolate = info.GetIsolate();
        auto thiz = info.This();
        auto node = reinterpret_cast<MetadataNode *>(info.Data().As<External>()->Value());

        Local<Object> implementationObject;
        Local<String> v8ExtendName;

        if (info.Length() == 1) {
            if (!info[0]->IsObject()) {
                throw NativeScriptException(string("First argument must be implementation object"));
            }

            implementationObject = info[0]->ToObject();
        }
        else if (info.Length() == 2) {
            if (!info[0]->IsString()) {
                throw NativeScriptException(string("First argument must be string"));
            }
            if (!info[1]->IsObject()) {
                throw NativeScriptException(
                        string("Second argument must be implementation object"));
            }

            v8ExtendName = info[0]->ToString();
            implementationObject = info[1]->ToObject();
        }
        else {
            throw NativeScriptException(string("Invalid number of arguments"));
        }

        auto className = node->m_implType;
        SetInstanceMetadata(isolate, implementationObject, node);

        //@@@ Refactor
        thiz->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper),
                               True(isolate));

        implementationObject->SetPrototype(thiz->GetPrototype());
        thiz->SetPrototype(implementationObject);
        thiz->SetPrivate(isolate->GetCurrentContext(),
                         Private::New(isolate, ConvertToV8String("t::implObj")),
                         implementationObject);

        ArgsWrapper argWrapper(info, ArgType::Interface, Local<Object>());

        auto success = CallbackHandlers::RegisterInstance(isolate, thiz, className, argWrapper,
                                                          implementationObject, true);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::ClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
    try {
        SET_PROFILER_FRAME();

        auto thiz = info.This();
        auto isolate = info.GetIsolate();
        auto node = reinterpret_cast<MetadataNode *>(info.Data().As<External>()->Value());

        Local<Object> outerThis;
        string extendName;
        auto className = node->m_name;

        SetInstanceMetadata(isolate, thiz, node);

        ArgsWrapper argWrapper(info, ArgType::Class, outerThis);

        string fullClassName = CreateFullClassName(className, extendName);
        bool success = CallbackHandlers::RegisterInstance(isolate, thiz, fullClassName, argWrapper,
                                                          outerThis, false);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::MethodCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
    try {
        SET_PROFILER_FRAME();

        auto e = info.Data().As<External>();

        auto callbackData = reinterpret_cast<MethodCallbackData *>(e->Value());

        // Number of arguments the method is invoked with
        int argLength = info.Length();

        MetadataEntry *entry = nullptr;

        string *className;
        const auto &first = callbackData->candidates.front();
        const auto &methodName = first.name;

        while ((callbackData != nullptr) && (entry == nullptr)) {
            auto &candidates = callbackData->candidates;

            className = &callbackData->node->m_name;

            // Iterates through all methods and finds the best match based on the number of arguments
            auto found = false;
            for (auto &c : candidates) {
                found = c.paramCount == argLength;
                if (found) {
                    entry = &c;
                    DEBUG_WRITE("MetaDataEntry Method %s's signature is: %s", entry->name.c_str(),
                                entry->sig.c_str());
                    break;
                }
            }

            // Iterates through the parent class's methods to find a good match
            if (!found) {
                callbackData = callbackData->parent;
            }
        }

        auto thiz = info.This();

        auto isSuper = false;
        if (!first.isStatic) {
            auto superValue = thiz->GetInternalField(
                    static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper));
            isSuper = !superValue.IsEmpty() && superValue->IsTrue();
        }

        if ((argLength == 0) && (methodName == V8StringConstants::VALUE_OF)) {
            info.GetReturnValue().Set(thiz);
        }
        else {
            CallbackHandlers::CallJavaMethod(thiz, *className, methodName, entry, first.isStatic,
                                             isSuper, info);
        }
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::ArrayIndexedPropertyGetterCallback(uint32_t index,
                                                      const PropertyCallbackInfo<Value> &info) {
    try {
        auto thiz = info.This();
        auto isolate = info.GetIsolate();

        auto node = GetNodeFromHandle(thiz);

        auto element = CallbackHandlers::GetArrayElement(isolate, thiz, index, node->m_name);

        info.GetReturnValue().Set(element);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void MetadataNode::ArrayIndexedPropertySetterCallback(uint32_t index, Local<Value> value,
                                                      const PropertyCallbackInfo<Value> &info) {
    try {
        auto thiz = info.This();
        auto isolate = info.GetIsolate();

        auto node = GetNodeFromHandle(thiz);

        CallbackHandlers::SetArrayElement(isolate, thiz, index, node->m_name, value);

        info.GetReturnValue().Set(value);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

Local<Object> MetadataNode::GetImplementationObject(Isolate *isolate, const Local<Object> &object) {
    DEBUG_WRITE("GetImplementationObject called  on object:%d", object->GetIdentityHash());

    auto target = object;
    Local<Value> currentPrototype = target;
    Local<Value> implementationObject;

    MaybeLocal<Value> maybeImplementationObject;

    maybeImplementationObject = object->GetPrivate(isolate->GetCurrentContext(),
                                                   Private::New(isolate,
                                                                ConvertToV8String("t::implObj")));
    if (!maybeImplementationObject.IsEmpty()) {
        maybeImplementationObject.FromMaybe(implementationObject);

        if (!implementationObject.IsEmpty()) {
            return implementationObject.As<Object>();
        }

        if (object->HasOwnProperty(V8StringConstants::GetIsPrototypeImplementationObject())) {
            auto v8Prototype = V8StringConstants::GetPrototype();
            if (!object->HasOwnProperty(v8Prototype)) {
                return Local<Object>();
            }

            DEBUG_WRITE("GetImplementationObject returning the prototype of the object :%d",
                        object->GetIdentityHash());
            return object->Get(v8Prototype).As<Object>();
        }

        auto maybeObj = V8GetHiddenValue(isolate, object, "t::ActivityImplementationObject");
        Local<Value> obj;

        if (!obj.IsEmpty()) {
            maybeObj.FromMaybe(obj);

            if (!obj.IsEmpty()) {
                DEBUG_WRITE(
                        "GetImplementationObject returning ActivityImplementationObject property on object: %d",
                        object->GetIdentityHash());
                return obj.As<Object>();
            }
        }

        Local<Value> lastPrototype;
        bool prototypeCycleDetected = false;
        while (implementationObject.IsEmpty()) {
            //
            currentPrototype = currentPrototype.As<Object>()->GetPrototype();

            if (currentPrototype->IsNull())
                break;

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
            }
            else {
                auto maybeValue = currentPrototype.As<Object>()->GetPrivate(
                        isolate->GetCurrentContext(),
                        Private::New(isolate,
                                     V8StringConstants::GetClassImplementationObject()));
                Local<Value> value;

                if (!maybeValue.IsEmpty()) {
                    maybeValue.FromMaybe(value);

                    if (!value.IsEmpty()) {
                        implementationObject = value.As<Object>();
                    }
                }
            }

            lastPrototype = currentPrototype;
        }

    }

    return implementationObject.As<Object>();
}

void MetadataNode::PackageGetterCallback(Local<Name> property,
                                         const PropertyCallbackInfo<Value> &info) {
    try {
        if (property.IsEmpty() || !property->IsString()) {
            return;
        }

        auto strProperty = property.As<String>();

        string propName = ConvertToString(strProperty);

        if (propName.empty()) {
            return;
        }

        auto isolate = info.GetIsolate();

        auto thiz = info.This();

        auto maybeCachedItem = thiz->GetPrivate(isolate->GetCurrentContext(),
                                                Private::New(isolate, strProperty));
        Local<Value> cachedItem;

        if (!maybeCachedItem.IsEmpty()) {
//			cachedItem = maybeCachedItem.ToLocalChecked();

            maybeCachedItem.FromMaybe(cachedItem);

            if (cachedItem.IsEmpty()) {
                auto node = reinterpret_cast<MetadataNode *>(info.Data().As<External>()->Value());

                uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);

                DEBUG_WRITE(
                        "MetadataNode::GetterCallback: prop '%s' for node '%s' called, nodeType=%d, hash=%d",
                        propName.c_str(), node->m_treeNode->name.c_str(), nodeType,
                        thiz.IsEmpty() ? -42 : thiz->GetIdentityHash());

                auto child = GetChildMetadataForPackage(node, propName);
                auto foundChild = child.treeNode != nullptr;

                auto childName = child.name.c_str();

                if (foundChild) {
                    auto childNode = MetadataNode::GetOrCreateInternal(child.treeNode);
                    cachedItem = childNode->CreateWrapper(isolate);
                    thiz->SetPrivate(isolate->GetCurrentContext(),
                                     Private::New(isolate, strProperty), cachedItem);
                }
            }
        }
        else {
            DEBUG_WRITE(">>PackageGetterCallback maybeItem is empty");

            string msg = "Empty handle when getting hidden value";
            NativeScriptException nsEx(msg.c_str());
        }

        info.GetReturnValue().Set(cachedItem);
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

bool MetadataNode::ValidateExtendArguments(const FunctionCallbackInfo<Value> &info,
                                           string &extendLocation,
                                           v8::Local<v8::String> &extendName,
                                           Local<Object> &implementationObject) {
    bool extendLocationFound = GetExtendLocation(extendLocation);

    if (info.Length() == 1) {
        if (!extendLocationFound) {
            stringstream ss;
            ss << "Invalid extend() call. No name specified for extend at location: " <<
            extendLocation.c_str();
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }

        if (!info[0]->IsObject()) {
            stringstream ss;
            ss << "Invalid extend() call. No implementation object specified at location: " <<
            extendLocation.c_str();
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }

        implementationObject = info[0]->ToObject();
    }
    else if (info.Length() == 2) {
        if (!info[0]->IsString()) {
            stringstream ss;
            ss << "Invalid extend() call. No name for extend specified at location: " <<
            extendLocation.c_str();
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }

        if (!info[1]->IsObject()) {
            stringstream ss;
            ss <<
            "Invalid extend() call. Named extend should be called with second object parameter containing overridden methods at location: " <<
            extendLocation.c_str();
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }

        DEBUG_WRITE("ExtendsCallMethodHandler: getting extend name");
        extendName = info[0]->ToString();
        bool isValidExtendName = IsValidExtendName(extendName);
        if (!isValidExtendName) {
            stringstream ss;
            ss << "The extend name \"" << ConvertToString(extendName) <<
            "\" you provided contains invalid symbols. Try using the symbols [a-z, A-Z, 0-9, _]." <<
            endl;
            string exceptionMessage = ss.str();

            throw NativeScriptException(exceptionMessage);
        }
        implementationObject = info[1]->ToObject();
    }
    else {
        stringstream ss;
        ss << "Invalid extend() call at location: " << extendLocation.c_str();
        string exceptionMessage = ss.str();

        throw NativeScriptException(exceptionMessage);
    }

    return true;
}

MetadataNode::ExtendedClassCacheData MetadataNode::GetCachedExtendedClassData(Isolate *isolate,
                                                                              const string &proxyClassName) {
    auto cache = GetCache(isolate);
    ExtendedClassCacheData cacheData;
    auto itFound = cache->ExtendedCtorFuncCache.find(proxyClassName);
    if (itFound != cache->ExtendedCtorFuncCache.end()) {
        cacheData = itFound->second;
    }

    return cacheData;
}

string MetadataNode::CreateFullClassName(const std::string &className,
                                         const std::string &extendNameAndLocation = "") {
    string fullClassName = className;

    if (!extendNameAndLocation.empty()) {
        fullClassName += Constants::CLASS_NAME_LOCATION_SEPARATOR + extendNameAndLocation;
    }

    return fullClassName;
}

void MetadataNode::ExtendCallMethodCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
    try {
        if (info.IsConstructCall()) {
            string exMsg("Cannot call 'extend' as constructor");
            throw NativeScriptException(exMsg);
        }

        SET_PROFILER_FRAME();

        Local<Object> implementationObject;
        Local<String> extendName;
        string extendLocation;

        auto hasDot = false;
        if (info.Length() == 2) {
            if (info[0].IsEmpty() || !info[0]->IsString()) {
                stringstream ss;
                ss << "Invalid extend() call. No name for extend specified at location: " <<
                extendLocation.c_str();
                string exceptionMessage = ss.str();

                throw NativeScriptException(exceptionMessage);
            }
            if (info[1].IsEmpty() || !info[1]->IsObject()) {
                stringstream ss;
                ss <<
                "Invalid extend() call. Named extend should be called with second object parameter containing overridden methods at location: " <<
                extendLocation.c_str();
                string exceptionMessage = ss.str();

                throw NativeScriptException(exceptionMessage);
            }
            string strName = ConvertToString(info[0].As<String>());
            hasDot = strName.find('.') != string::npos;
        }

        if (hasDot) {
            extendName = info[0].As<String>();
            implementationObject = info[1].As<Object>();
        }
        else {
            auto validArgs = ValidateExtendArguments(info, extendLocation, extendName,
                                                     implementationObject);

            if (!validArgs)
                return;
        }

        auto node = reinterpret_cast<MetadataNode *>(info.Data().As<External>()->Value());

        DEBUG_WRITE("ExtendsCallMethodHandler: called with %s",
                    ConvertToString(extendName).c_str());

        string extendNameAndLocation = extendLocation + ConvertToString(extendName);
        string fullClassName;
        if (!hasDot) {
            fullClassName = TNS_PREFIX + CreateFullClassName(node->m_name, extendNameAndLocation);
        }
        else {
            fullClassName = ConvertToString(info[0].As<String>());
        }

        auto isolate = info.GetIsolate();

        //resolve class (pre-generated or generated runtime from dex generator)
        uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);
        bool isInterface = s_metadataReader.IsNodeTypeInterface(nodeType);
        auto fullExtendedName = CallbackHandlers::ResolveClassName(isolate, fullClassName,
                                                                   implementationObject,
                                                                   isInterface);
        DEBUG_WRITE("ExtendsCallMethodHandler: extend full name %s", fullClassName.c_str());

        auto cachedData = GetCachedExtendedClassData(isolate, fullExtendedName);
        if (cachedData.extendedCtorFunction != nullptr) {
            auto cachedExtendedCtorFunc = Local<Function>::New(isolate,
                                                               *cachedData.extendedCtorFunction);
            info.GetReturnValue().Set(cachedExtendedCtorFunc);
            return;
        }

        auto implementationObjectPropertyName = Private::New(isolate,
                                                             V8StringConstants::GetClassImplementationObject());
        //reuse validation - checks that implementationObject is not reused for different classes
        auto maybeimplementationObjectProperty = implementationObject->GetPrivate(
                isolate->GetCurrentContext(), implementationObjectPropertyName);
        Local<Value> implementationObjectProperty;

        if (!maybeimplementationObjectProperty.IsEmpty()) {
            maybeimplementationObjectProperty.FromMaybe(implementationObjectProperty);

            Local<String> implementationObjectPropertyStr = implementationObjectProperty.As<String>();

            if (implementationObjectPropertyStr.IsEmpty()) {
                //mark the implementationObject as such and set a pointer to it's class node inside it for reuse validation later
                implementationObject->SetPrivate(isolate->GetCurrentContext(),
                                                 implementationObjectPropertyName,
                                                 String::NewFromUtf8(isolate,
                                                                     fullExtendedName.c_str()));
            } else {
                string usedClassName = ConvertToString(implementationObjectPropertyStr);
                stringstream s;
                s << "This object is used to extend another class '" << usedClassName << "'";
                throw NativeScriptException(s.str());
            }
        }


        auto baseClassCtorFunc = node->GetConstructorFunction(isolate);
        auto extendData = External::New(isolate, new ExtendedClassData(node, extendNameAndLocation,
                                                                       implementationObject,
                                                                       fullExtendedName));
        auto extendFuncTemplate = FunctionTemplate::New(isolate, ExtendedClassConstructorCallback,
                                                        extendData);
        extendFuncTemplate->InstanceTemplate()->SetInternalFieldCount(
                static_cast<int>(ObjectManager::MetadataNodeKeys::END));

        auto extendFunc = extendFuncTemplate->GetFunction();
        auto prototypeName = ConvertToV8String("prototype");
        implementationObject->SetPrototype(baseClassCtorFunc->Get(prototypeName));
        implementationObject->SetAccessor(ConvertToV8String("super"), SuperAccessorGetterCallback,
                                          nullptr, implementationObject);

        auto extendFuncPrototype = extendFunc->Get(prototypeName).As<Object>();
        auto p = extendFuncPrototype->GetPrototype();
        extendFuncPrototype->SetPrototype(implementationObject);
        extendFunc->SetPrototype(baseClassCtorFunc);

        SetClassAccessor(extendFunc);
        SetTypeMetadata(isolate, extendFunc, new TypeMetadata(fullExtendedName));
        info.GetReturnValue().Set(extendFunc);

        s_name2NodeCache.insert(make_pair(fullExtendedName, node));

        ExtendedClassCacheData cacheData(extendFunc, fullExtendedName, node);
        auto cache = GetCache(isolate);
        cache->ExtendedCtorFuncCache.insert(make_pair(fullExtendedName, cacheData));
    }
    catch (NativeScriptException &e) {
        e.ReThrowToV8();
    }
    catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    }
    catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

bool MetadataNode::IsValidExtendName(const Local<String> &name) {
    string extendNam = ConvertToString(name);

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

bool MetadataNode::GetExtendLocation(string &extendLocation) {
    stringstream extendLocationStream;
    auto stackTrace = StackTrace::CurrentStackTrace(Isolate::GetCurrent(), 1,
                                                    StackTrace::kOverview);
    if (!stackTrace.IsEmpty()) {
        auto frame = stackTrace->GetFrame(0);
        if (!frame.IsEmpty()) {
            auto scriptName = frame->GetScriptName();
            if (scriptName.IsEmpty()) {
                extendLocationStream << "unkown location";
                extendLocation = extendLocationStream.str();
                return false;
            }

            string srcFileName = ConvertToString(scriptName);
            string fullPathToFile;
            if (srcFileName == "<embedded script>") {
                // Corner case, extend call is coming from the heap snapshot script
                // This is possible for lazily compiled code - e.g. from the body of a function
                fullPathToFile = "_embedded_script_";
            }
            else {
                string hardcodedPathToSkip = Constants::APP_ROOT_FOLDER_PATH;

                int startIndex = hardcodedPathToSkip.length();
                int strToTakeLen = (srcFileName.length() - startIndex -
                                    3); // 3 refers to .js at the end of file name
                fullPathToFile = srcFileName.substr(startIndex, strToTakeLen);

                std::replace(fullPathToFile.begin(), fullPathToFile.end(), '/', '_');
                std::replace(fullPathToFile.begin(), fullPathToFile.end(), '.', '_');
            }

            int lineNumber = frame->GetLineNumber();
            if (lineNumber < 0) {
                extendLocationStream << fullPathToFile.c_str() << " unkown line number";
                extendLocation = extendLocationStream.str();
                return false;
            }

            int column = frame->GetColumn();
            if (column < 0) {
                extendLocationStream << fullPathToFile.c_str() << " line:" << lineNumber <<
                " unkown column number";
                extendLocation = extendLocationStream.str();
                return false;
            }

            extendLocationStream << "f" << fullPathToFile.c_str() << "_l" << lineNumber << "_c" <<
            column << "__";
            //DEBUG_WRITE("EXTEND_LOCATION %s", extendLocationStream.str().c_str());
        }
    }

    extendLocation = extendLocationStream.str();
    return true;
}

MetadataNode *MetadataNode::GetNodeFromHandle(const Local<Object> &value) {
    auto node = GetInstanceMetadata(Isolate::GetCurrent(), value);
    return node;
}

MetadataEntry MetadataNode::GetChildMetadataForPackage(MetadataNode *node, const string &propName) {
    MetadataEntry child;

    assert(node->m_treeNode->children != nullptr);

    const auto &children = *node->m_treeNode->children;

    for (auto treeNodeChild : children) {
        if (propName == treeNodeChild->name) {
            child.name = propName;
            child.treeNode = treeNodeChild;

            uint8_t childNodeType = s_metadataReader.GetNodeType(treeNodeChild);
            if (s_metadataReader.IsNodeTypeInterface(childNodeType)) {
                bool isPrefix;
                string declaringType = s_metadataReader.ReadInterfaceImplementationTypeName(
                        treeNodeChild, isPrefix);
                child.declaringType = isPrefix
                                      ? (declaringType +
                                         s_metadataReader.ReadTypeName(child.treeNode))
                                      : declaringType;
            }
        }
    }

    return child;
}

void MetadataNode::BuildMetadata(const string &filesPath) {
    timeval time1;
    gettimeofday(&time1, nullptr);

    string baseDir = filesPath;
    baseDir.append("/metadata");
    string nodesFile = baseDir + "/treeNodeStream.dat";
    string namesFile = baseDir + "/treeStringsStream.dat";
    string valuesFile = baseDir + "/treeValueStream.dat";

    FILE *f = fopen(nodesFile.c_str(), "rb");
    if (f == nullptr) {
        throw NativeScriptException(
                string("metadata file (treeNodeStream.dat) couldn't be opened!"));
    }
    fseek(f, 0, SEEK_END);
    int lenNodes = ftell(f);
    assert((lenNodes % sizeof(MetadataTreeNodeRawData)) == 0);
    char *nodes = new char[lenNodes];
    rewind(f);
    fread(nodes, 1, lenNodes, f);
    fclose(f);

    const int _512KB = 524288;

    f = fopen(namesFile.c_str(), "rb");
    if (f == nullptr) {
        throw NativeScriptException(
                string("metadata file (treeStringsStream.dat) couldn't be opened!"));
    }
    fseek(f, 0, SEEK_END);
    int lenNames = ftell(f);
    char *names = new char[lenNames + _512KB];
    rewind(f);
    fread(names, 1, lenNames, f);
    fclose(f);

    f = fopen(valuesFile.c_str(), "rb");
    if (f == nullptr) {
        throw NativeScriptException(
                string("metadata file (treeValueStream.dat) couldn't be opened!"));
    }
    fseek(f, 0, SEEK_END);
    int lenValues = ftell(f);
    char *values = new char[lenValues + _512KB];
    rewind(f);
    fread(values, 1, lenValues, f);
    fclose(f);

    timeval time2;
    gettimeofday(&time2, nullptr);

    __android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", "lenNodes=%d, lenNames=%d, lenValues=%d",
                        lenNodes, lenNames, lenValues);

    long millis1 = (time1.tv_sec * 1000) + (time1.tv_usec / 1000);
    long millis2 = (time2.tv_sec * 1000) + (time2.tv_usec / 1000);

    __android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", "time=%ld", (millis2 - millis1));

    BuildMetadata(lenNodes, reinterpret_cast<uint8_t *>(nodes), lenNames,
                  reinterpret_cast<uint8_t *>(names), lenValues,
                  reinterpret_cast<uint8_t *>(values));

    delete[] nodes;
    //delete[] names;
    //delete[] values;
}

void MetadataNode::BuildMetadata(uint32_t nodesLength, uint8_t *nodeData, uint32_t nameLength,
                                 uint8_t *nameData, uint32_t valueLength, uint8_t *valueData) {
    s_metadataReader = MetadataReader(nodesLength, nodeData, nameLength, nameData, valueLength,
                                      valueData, CallbackHandlers::GetTypeMetadata);
}

void MetadataNode::CreateTopLevelNamespaces(Isolate *isolate, const Local<Object> &global) {
    auto root = s_metadataReader.GetRoot();

    const auto &children = *root->children;

    for (auto treeNode : children) {
        uint8_t nodeType = s_metadataReader.GetNodeType(treeNode);

        if (nodeType == MetadataTreeNode::PACKAGE) {
            auto node = GetOrCreateInternal(treeNode);

            auto packageObj = node->CreateWrapper(isolate);
            global->Set(ConvertToV8String(node->m_treeNode->name), packageObj);
        }
    }
}

MetadataNode::MetadataNodeCache *MetadataNode::GetCache(Isolate *isolate) {
    MetadataNodeCache *cache;
    auto itFound = s_cache.find(isolate);
    if (itFound == s_cache.end()) {
        cache = new MetadataNodeCache;
        s_cache.insert(make_pair(isolate, cache));
    }
    else {
        cache = itFound->second;
    }
    return cache;
}

void MetadataNode::EnableProfiler(bool enableProfiler) {
    s_profilerEnabled = enableProfiler;
}

Local<Function> MetadataNode::Wrap(Isolate *isolate, const Local<Function> &f, const string &name,
                                   const string &origin, bool isCtorFunc) {
    if (!s_profilerEnabled) {
        return f;
    }

    static set <string> keywords;

    if (keywords.empty()) {
        string kw[]{"abstract", "arguments", "boolean", "break", "byte", "case", "catch", "char",
                    "class", "const", "continue", "debugger", "default", "delete", "do",
                    "double", "else", "enum", "eval", "export", "extends", "false", "final",
                    "finally", "float", "for", "function", "goto", "if", "implements",
                    "import", "in", "instanceof", "int", "interface", "let", "long", "native",
                    "new", "null", "package", "private", "protected", "public", "return",
                    "short", "static", "super", "switch", "synchronized", "this", "throw", "throws",
                    "transient", "true", "try", "typeof", "var", "void", "volatile", "while",
                    "with", "yield"};

        keywords = set<string>(kw, kw + sizeof(kw) / sizeof(kw[0]));
    }

    if (name == "<init>") {
        return f;
    }

    string actualName = name;
    while (keywords.find(actualName) != keywords.end()) {
        actualName.append("_");
    }

    Local<Function> ret;

    stringstream ss;
    ss << "(function() { ";
    ss << "function " << actualName << "() { ";
    if (isCtorFunc) {
        ss <<
        "var args = [null]; for (var i=0; i<arguments.length; i++) { args.push(arguments[i]); }; ";
        ss << "return new (Function.prototype.bind.apply(" << actualName << ".__func, args)); ";
    }
    else {
        ss << "return " << actualName << ".__func.apply(this, arguments); ";
    }
    ss << "} ";
    ss << "return " << actualName << "; ";
    ss << "})()";

    auto str = ss.str();
    auto source = ConvertToV8String(str);
    auto context = isolate->GetCurrentContext();

    TryCatch tc;

    Local<Script> script;
    ScriptOrigin jsOrigin(ConvertToV8String(origin));
    auto maybeScript = Script::Compile(context, source, &jsOrigin).ToLocal(&script);

    if (tc.HasCaught()) {
        throw NativeScriptException(tc, "Cannot compile wrapper");
    }

    if (!script.IsEmpty()) {
        Local<Value> result;
        auto maybeResult = script->Run(context).ToLocal(&result);

        if (!result.IsEmpty()) {
            ret = result.As<Function>();
            ret->Set(ConvertToV8String("__func"), f);

            auto prototypePropName = ConvertToV8String("prototype");
            ret->Set(prototypePropName, f->Get(prototypePropName));
        }
        else {
            throw NativeScriptException("Cannot create wrapper function");
        }
    }
    else {
        throw NativeScriptException(str);
    }

    return ret;
}

string MetadataNode::TNS_PREFIX = "com/tns/gen/";
MetadataReader MetadataNode::s_metadataReader;
std::map<std::string, MetadataNode *> MetadataNode::s_name2NodeCache;
std::map<std::string, MetadataTreeNode *> MetadataNode::s_name2TreeNodeCache;
std::map<MetadataTreeNode *, MetadataNode *> MetadataNode::s_treeNode2NodeCache;
map<Isolate *, MetadataNode::MetadataNodeCache *> MetadataNode::s_cache;
bool MetadataNode::s_profilerEnabled = false;
