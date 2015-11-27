#include "MetadataNode.h"
#include "NativeScriptAssert.h"
#include "Constants.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "SimpleProfiler.h"
#include "JniLocalRef.h"
#include "NativeScriptRuntime.h"
#include "NativeScriptException.h"
#include <sstream>
#include <cctype>
#include <assert.h>

using namespace v8;
using namespace std;
using namespace tns;

void MetadataNode::Init(ObjectManager *objectManager)
{
	s_objectManager = objectManager;

	auto isolate = Isolate::GetCurrent();
	auto key = ConvertToV8String("tns::MetadataKey");
	s_metadataKey = new Persistent<String>(isolate, key);
}


MetadataNode::MetadataNode(MetadataTreeNode *treeNode) :
		m_treeNode(treeNode)
{
	uint8_t nodeType = s_metadataReader.GetNodeType(treeNode);

	m_name = s_metadataReader.ReadTypeName(m_treeNode);

	uint8_t parentNodeType = s_metadataReader.GetNodeType(treeNode->parent);

	m_isArray = s_metadataReader.IsNodeTypeArray(parentNodeType);

	bool isInterface = s_metadataReader.IsNodeTypeInterface(nodeType);

	if (!m_isArray && isInterface)
	{
		bool isPrefix;
		auto impTypeName = s_metadataReader.ReadInterfaceImplementationTypeName(m_treeNode, isPrefix);
		m_implType = isPrefix
					? (impTypeName + m_name)
					: impTypeName;
	}
}

Local<Object> MetadataNode::CreateExtendedJSWrapper(Isolate *isolate, const string& proxyClassName)
{
	Local<Object> extInstance;

	auto cacheData = GetCachedExtendedClassData(isolate, proxyClassName);
	if (cacheData.node != nullptr)
	{
		extInstance = s_objectManager->GetEmptyObject(isolate);
		extInstance->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper), True(isolate));
		auto extdCtorFunc = Local<Function>::New(isolate, *cacheData.extendedCtorFunction);
		extInstance->SetPrototype(extdCtorFunc->Get(ConvertToV8String("prototype")));

		SetInstanceMetadata(isolate, extInstance, cacheData.node);
	}

	return extInstance;
}

MetadataNode* MetadataNode::GetOrCreate(const string& className)
{
	return GetOrCreateInternal(className);
}


MetadataNode* MetadataNode::GetOrCreateInternal(const string& className)
{
	MetadataNode *node = nullptr;

	auto it = s_name2NodeCache.find(className);

	if (it == s_name2NodeCache.end())
	{
		MetadataTreeNode *treeNode = GetOrCreateTreeNodeByName(className);

		node = GetOrCreateInternal(treeNode);

		s_name2NodeCache.insert(make_pair(className, node));
	}
	else
	{
		node = it->second;
	}

	return node;
}

MetadataNode* MetadataNode::GetOrCreateInternal(MetadataTreeNode *treeNode)
{
	MetadataNode *result = nullptr;

	auto it = s_treeNode2NodeCache.find(treeNode);

	if (it != s_treeNode2NodeCache.end())
	{
		result = it->second;
	}
	else
	{
		result = new MetadataNode(treeNode);

		s_treeNode2NodeCache.insert(make_pair(treeNode, result));
	}

	return result;
}

MetadataTreeNode* MetadataNode::GetOrCreateTreeNodeByName(const string& className)
{
	MetadataTreeNode *result = nullptr;

	auto itFound = s_name2TreeNodeCache.find(className);

	if (itFound != s_name2TreeNodeCache.end())
	{
		result = itFound->second;
	}
	else
	{
		result = s_metadataReader.GetOrCreateTreeNodeByName(className);

		s_name2TreeNodeCache.insert(make_pair(className, result));
	}

	return result;
}


string MetadataNode::GetName()
{
	return m_name;
}

Local<Object> MetadataNode::CreateWrapper(Isolate *isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<Object> obj;

	uint8_t nodeType = s_metadataReader.GetNodeType(m_treeNode);

	bool isClass = s_metadataReader.IsNodeTypeClass(nodeType),
		isInterface = s_metadataReader.IsNodeTypeInterface(nodeType);

	if (isClass || isInterface)
	{
		obj = GetConstructorFunction(isolate);
	}
	else if (s_metadataReader.IsNodeTypePackage(nodeType))
	{
		obj = CreatePackageProxy(isolate);
	}
	else
	{
		stringstream ss;
		ss << "(InternalError): Can't create proxy for this type=" <<  static_cast<int>(nodeType);
		throw NativeScriptException(ss.str());
	}

	return handle_scope.Escape(obj);
}

Local<Object> MetadataNode::CreateJSWrapper(Isolate *isolate)
{
	Local<Object> obj;

	if (m_isArray)
	{
		obj = CreateArrayWrapper(isolate);
	}
	else
	{
		auto ctorFunc = GetConstructorFunction(isolate);

		//obj = Object::New(isolate);
		obj = s_objectManager->GetEmptyObject(isolate);
		obj->Set(ConvertToV8String("constructor"), ctorFunc);
		obj->SetPrototype(ctorFunc->Get(ConvertToV8String("prototype")));
		SetInstanceMetadata(isolate, obj, this);
	}

	return obj;
}

void MetadataNode::ArrayLengthGetterCallack(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	try {
	auto thiz = info.This();
	auto length = NativeScriptRuntime::GetArrayLength(thiz);
	info.GetReturnValue().Set(Integer::New(info.GetIsolate(), length));
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

Local<Object> MetadataNode::CreateArrayWrapper(Isolate *isolate)
{
	auto node = GetOrCreateInternal("java/lang/Object");
	auto objPrototype = node->GetConstructorFunction(isolate);

	auto arrayObjectTemplate = ObjectTemplate::New(isolate);
	arrayObjectTemplate->SetInternalFieldCount(static_cast<int>(ObjectManager::MetadataNodeKeys::END));
	arrayObjectTemplate->SetIndexedPropertyHandler(ArrayIndexedPropertyGetterCallback, ArrayIndexedPropertySetterCallback);

	auto arr = arrayObjectTemplate->NewInstance();
	arr->SetPrototype(objPrototype->Get(ConvertToV8String("prototype")));
	arr->SetAccessor(ConvertToV8String("length"), ArrayLengthGetterCallack, nullptr, Local<Value>(), AccessControl::ALL_CAN_READ, PropertyAttribute::DontDelete);

	SetInstanceMetadata(isolate, arr, this);

	return arr;
}


Local<Object> MetadataNode::CreatePackageProxy(Isolate *isolate)
{
	EscapableHandleScope handleScope(isolate);

	auto packageTemplate = ObjectTemplate::New();
	packageTemplate->SetNamedPropertyHandler(MetadataNode::PackageGetterCallback);

	auto package = packageTemplate->NewInstance();
	SetPackageMetadata(isolate, package, this);

	return handleScope.Escape(package);
}



void MetadataNode::SetClassAccessor(Local<Function>& ctorFunction)
{
	auto classFieldName = ConvertToV8String("class");
	ctorFunction->SetAccessor(classFieldName, ClassAccessorGetterCallback, nullptr, Local<Value>(), AccessControl::ALL_CAN_READ, PropertyAttribute::DontDelete);
}

void MetadataNode::ClassAccessorGetterCallback(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	try {
	auto thiz = info.This();
	auto isolate = info.GetIsolate();
	auto data = GetTypeMetadata(isolate, thiz.As<Function>());

	auto value = NativeScriptRuntime::FindClass(data->name);
	info.GetReturnValue().Set(value);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void MetadataNode::FieldAccessorGetterCallback(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	try {
	auto thiz = info.This();
	auto fieldCallbackData = reinterpret_cast<FieldCallbackData*>(info.Data().As<External>()->Value());
	auto value = NativeScriptRuntime::GetJavaField(thiz, fieldCallbackData);
	info.GetReturnValue().Set(value);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}
void MetadataNode::FieldAccessorSetterCallback(Local<String> property,Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	try {
	DEBUG_WRITE("FieldAccessorSetterCallback");

	auto thiz = info.This();
	auto fieldCallbackData = reinterpret_cast<FieldCallbackData*>(info.Data().As<External>()->Value());

	if (fieldCallbackData->isFinal)
	{
		stringstream ss;
		ss << "You are trying to set \"" << fieldCallbackData->name << "\" which is a final field! Final fields can only be read.";
		string exceptionMessage = ss.str();

		throw NativeScriptException(exceptionMessage);
	}
	else
	{
		NativeScriptRuntime::SetJavaField(thiz, value, fieldCallbackData);
		info.GetReturnValue().Set(value);
	}
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void MetadataNode::SuperAccessorGetterCallback(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	try {
	auto thiz = info.This();
	auto isolate = info.GetIsolate();
	auto k = ConvertToV8String("supervalue");
	auto superValue = thiz->GetHiddenValue(k).As<Object>();
	if (superValue.IsEmpty())
	{
		superValue = s_objectManager->GetEmptyObject(isolate);
		bool d = superValue->Delete(ConvertToV8String("toString"));
		d = superValue->Delete(ConvertToV8String("valueOf"));
		superValue->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper), True(isolate));

		superValue->SetPrototype(thiz->GetPrototype().As<Object>()->GetPrototype().As<Object>()->GetPrototype());
		thiz->SetHiddenValue(k, superValue);
		s_objectManager->CloneLink(thiz, superValue);

		DEBUG_WRITE("superValue.GetPrototype=%d", superValue->GetPrototype().As<Object>()->GetIdentityHash());

		auto node = GetInstanceMetadata(isolate, thiz);
		SetInstanceMetadata(isolate, superValue, node);

		thiz->SetHiddenValue(k, superValue);
	}

	info.GetReturnValue().Set(superValue);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

Local<Function> MetadataNode::SetMembers(Isolate *isolate, Local<FunctionTemplate>& ctorFuncTemplate, Local<ObjectTemplate>& prototypeTemplate, vector<MethodCallbackData*>& instanceMethodsCallbackData, const vector<MethodCallbackData*>& baseInstanceMethodsCallbackData, MetadataTreeNode *treeNode)
{
	auto hasCustomMetadata = treeNode->metadata != nullptr;

	return hasCustomMetadata
			? SetMembersFromRuntimeMetadata(isolate, ctorFuncTemplate, prototypeTemplate, instanceMethodsCallbackData, baseInstanceMethodsCallbackData, treeNode)
			: SetMembersFromStaticMetadata(isolate, ctorFuncTemplate, prototypeTemplate, instanceMethodsCallbackData, baseInstanceMethodsCallbackData, treeNode);
}

Local<Function> MetadataNode::SetMembersFromStaticMetadata(Isolate *isolate, Local<FunctionTemplate>& ctorFuncTemplate, Local<ObjectTemplate>& prototypeTemplate, vector<MethodCallbackData*>& instanceMethodsCallbackData, const vector<MethodCallbackData*>& baseInstanceMethodsCallbackData, MetadataTreeNode *treeNode)
{
	SET_PROFILER_FRAME();

	Local<Function> ctorFunction;

	uint8_t *curPtr = s_metadataReader.GetValueData() + treeNode->offsetValue + 1;

	auto nodeType = s_metadataReader.GetNodeType(treeNode);

	auto curType = s_metadataReader.ReadTypeName(treeNode);

	curPtr += sizeof(uint16_t /* baseClassId */);

	if (s_metadataReader.IsNodeTypeInterface(nodeType))
	{
		curPtr += sizeof(uint8_t) + sizeof(uint32_t);
	}

	//get candidates from instance methods metadata
	auto instanceMethodCout = *reinterpret_cast<uint16_t*>(curPtr);
	curPtr += sizeof(uint16_t);
	string lastMethodName;
	MethodCallbackData *callbackData = nullptr;
	for (auto i = 0; i < instanceMethodCout; i++)
	{
		auto entry = s_metadataReader.ReadInstanceMethodEntry(&curPtr);
		if (entry.name != lastMethodName)
		{
			callbackData = new MethodCallbackData(this);

			instanceMethodsCallbackData.push_back(callbackData);
			auto itBegin = baseInstanceMethodsCallbackData.begin();
			auto itEnd = baseInstanceMethodsCallbackData.end();
			auto itFound = find_if(itBegin, itEnd, [&entry] (MethodCallbackData *x) { return x->candidates.front().name == entry.name; });
			if (itFound != itEnd)
			{
				callbackData->parent = *itFound;
			}

			auto funcData = External::New(isolate, callbackData);
			auto funcTemplate = FunctionTemplate::New(isolate, MethodCallback, funcData);
			auto funcName = ConvertToV8String(entry.name);
			prototypeTemplate->Set(funcName, funcTemplate->GetFunction());
			lastMethodName = entry.name;
		}
		callbackData->candidates.push_back(entry);
	}

	//get candidates from instance fields metadata
	auto instanceFieldCout = *reinterpret_cast<uint16_t*>(curPtr);
	curPtr += sizeof(uint16_t);
	for (auto i = 0; i < instanceFieldCout; i++)
	{
		auto entry = s_metadataReader.ReadInstanceFieldEntry(&curPtr);

		auto fieldName = ConvertToV8String(entry.name);
		auto fieldInfo = new FieldCallbackData(entry);
		fieldInfo->declaringType = curType;
		auto fieldData = External::New(isolate, fieldInfo);
		prototypeTemplate->SetAccessor(fieldName, FieldAccessorGetterCallback, FieldAccessorSetterCallback, fieldData, AccessControl::DEFAULT, PropertyAttribute::DontDelete);
	}

	ctorFunction = ctorFuncTemplate->GetFunction();

	//get candidates from static methods metadata
	callbackData = nullptr;
	lastMethodName.clear();
	auto staticMethodCout = *reinterpret_cast<uint16_t*>(curPtr);
	curPtr += sizeof(uint16_t);
	for (auto i = 0; i < staticMethodCout; i++)
	{
		auto entry = s_metadataReader.ReadStaticMethodEntry(&curPtr);
		if (entry.name != lastMethodName)
		{
			callbackData = new MethodCallbackData(this);
			auto funcData = External::New(isolate, callbackData);
			auto funcTemplate = FunctionTemplate::New(isolate, MethodCallback, funcData);
			auto funcName = ConvertToV8String(entry.name);
			ctorFunction->Set(funcName, funcTemplate->GetFunction());
			lastMethodName = entry.name;
		}
		callbackData->candidates.push_back(entry);
	}

	auto extendFuncName = V8StringConstants::GetExtend();
	auto extendFuncTemplate = FunctionTemplate::New(isolate, ExtendCallMethodCallback, External::New(isolate, this));
	ctorFunction->Set(extendFuncName, extendFuncTemplate->GetFunction());

	//get candidates from static fields metadata
	auto staticFieldCout = *reinterpret_cast<uint16_t*>(curPtr);
	curPtr += sizeof(uint16_t);
	for (auto i = 0; i < staticFieldCout; i++)
	{
		auto entry = s_metadataReader.ReadStaticFieldEntry(&curPtr);

		auto fieldName = ConvertToV8String(entry.name);
		auto fieldData = External::New(isolate, new FieldCallbackData(entry));
		ctorFunction->SetAccessor(fieldName, FieldAccessorGetterCallback, FieldAccessorSetterCallback, fieldData, AccessControl::DEFAULT, PropertyAttribute::DontDelete);
	}

	SetClassAccessor(ctorFunction);

	return ctorFunction;
}

Local<Function> MetadataNode::SetMembersFromRuntimeMetadata(Isolate *isolate, Local<FunctionTemplate>& ctorFuncTemplate, Local<ObjectTemplate>& prototypeTemplate, vector<MethodCallbackData*>& instanceMethodsCallbackData, const vector<MethodCallbackData*>& baseInstanceMethodsCallbackData, MetadataTreeNode *treeNode)
{
	SET_PROFILER_FRAME();

	assert(treeNode->metadata != nullptr);

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
	MethodCallbackData *callbackData = nullptr;

	while (getline(s, line))
	{
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

		if (chKind == 'M')
		{
			if (entry.name != lastMethodName)
			{
				callbackData = new MethodCallbackData(this);

				instanceMethodsCallbackData.push_back(callbackData);
				auto itBegin = baseInstanceMethodsCallbackData.begin();
				auto itEnd = baseInstanceMethodsCallbackData.end();
				auto itFound = find_if(itBegin, itEnd, [&entry] (MethodCallbackData *x) { return x->candidates.front().name == entry.name; });
				if (itFound != itEnd)
				{
					callbackData->parent = *itFound;
				}

				auto funcData = External::New(isolate, callbackData);
				auto funcTemplate = FunctionTemplate::New(isolate, MethodCallback, funcData);
				auto funcName = ConvertToV8String(entry.name);
				prototypeTemplate->Set(funcName, funcTemplate->GetFunction());
				lastMethodName = entry.name;
			}
			callbackData->candidates.push_back(entry);
		}
		else if (chKind == 'F')
		{
			auto fieldName = ConvertToV8String(entry.name);
			auto fieldData = External::New(isolate, new FieldCallbackData(entry));
			auto access = entry.isFinal ? AccessControl::ALL_CAN_READ : AccessControl::DEFAULT;
			prototypeTemplate->SetAccessor(fieldName, FieldAccessorGetterCallback, FieldAccessorSetterCallback, fieldData, access, PropertyAttribute::DontDelete);
		}
	}

	auto ctorFunction = ctorFuncTemplate->GetFunction();

	return ctorFunction;
}

void MetadataNode::InnerClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	try {
	auto thiz = info.This();
	auto isolate = info.GetIsolate();
	auto data = reinterpret_cast<InnerClassData*>(info.Data().As<External>()->Value());

	auto outerThis = Local<Object>::New(isolate, *data->outerThis);
	string extendName;
	auto className = data->node->m_name;

	SetInstanceMetadata(info.GetIsolate(), thiz, data->node);

	ArgsWrapper argWrapper(info, ArgType::Class, outerThis);

	string fullClassName = CreateFullClassName(className, extendName);
	bool success = NativeScriptRuntime::RegisterInstance(thiz, fullClassName, argWrapper, outerThis, false);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void MetadataNode::InnerClassAccessorGetterCallback(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	try {
	auto isolate = info.GetIsolate();
	auto thiz = info.This();
	auto node = reinterpret_cast<MetadataNode*>(info.Data().As<External>()->Value());

	auto innerKey = ConvertToV8String("inner:" + node->m_treeNode->name);

	auto innerTypeCtorFunc = thiz->GetHiddenValue(innerKey).As<Function>();
	if (innerTypeCtorFunc.IsEmpty())
	{
		auto funcTemplate = node->GetConstructorFunctionTemplate(isolate, node->m_treeNode);
		auto ctorFunc = funcTemplate->GetFunction();

		auto innerClassData = External::New(isolate, new InnerClassData(new Persistent<Object>(isolate, thiz), node));
		auto innerTypeCtorFuncTemplate = FunctionTemplate::New(isolate, InnerClassConstructorCallback, innerClassData);
		innerTypeCtorFuncTemplate->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(ObjectManager::MetadataNodeKeys::END));
		innerTypeCtorFunc = innerTypeCtorFuncTemplate->GetFunction();
		auto prototypeName = ConvertToV8String("prototype");
		auto innerTypeCtorFuncPrototype = innerTypeCtorFunc->Get(prototypeName).As<Object>();
		innerTypeCtorFuncPrototype->SetPrototype(ctorFunc->Get(prototypeName));

		thiz->SetHiddenValue(innerKey, innerTypeCtorFunc);
	}

	info.GetReturnValue().Set(innerTypeCtorFunc);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void MetadataNode::SetInnnerTypes(Isolate *isolate, Local<Function>& ctorFunction, MetadataTreeNode *treeNode)
{
	if (treeNode->children != nullptr)
	{
		const auto& children = *treeNode->children;

		auto prototypeTemplate2 = ctorFunction->Get(ConvertToV8String("prototype")).As<Object>();

		for (auto curChild: children)
		{
			auto childNode = GetOrCreateInternal(curChild);

			auto type = s_metadataReader.GetNodeType(curChild);
			auto isStatic = s_metadataReader.IsNodeTypeStatic(type);

			if (isStatic)
			{
				auto innerTypeCtorFuncTemplate = childNode->GetConstructorFunctionTemplate(isolate, curChild);
				auto innerTypeCtorFunc = innerTypeCtorFuncTemplate->GetFunction();
				auto innerTypeName = ConvertToV8String(curChild->name);
				ctorFunction->Set(innerTypeName, innerTypeCtorFunc);
			}
			else
			{
				auto innerTypeName = ConvertToV8String(curChild->name);
				prototypeTemplate2->SetAccessor(innerTypeName, InnerClassAccessorGetterCallback, nullptr, External::New(isolate, childNode));
			}
		}
	}
}


Local<FunctionTemplate> MetadataNode::GetConstructorFunctionTemplate(Isolate *isolate, MetadataTreeNode *treeNode)
{
	vector<MethodCallbackData*> instanceMethodsCallbackData;

	auto ft = GetConstructorFunctionTemplate(isolate, treeNode, instanceMethodsCallbackData);

	return ft;
}

Local<FunctionTemplate> MetadataNode::GetConstructorFunctionTemplate(Isolate *isolate, MetadataTreeNode *treeNode, vector<MethodCallbackData*>& instanceMethodsCallbackData)
{
	SET_PROFILER_FRAME();

	Local<FunctionTemplate> ctorFuncTemplate;
	auto itFound = s_ctorFuncCache.find(treeNode);
	if (itFound != s_ctorFuncCache.end())
	{
		auto& ctorCacheItem = itFound->second;
		instanceMethodsCallbackData = ctorCacheItem.instanceMethodCallbacks;
		ctorFuncTemplate = Local<FunctionTemplate>::New(isolate, *ctorCacheItem.ft);
		return ctorFuncTemplate;
	}

	auto node = GetOrCreateInternal(treeNode);
	auto ctorCallbackData = External::New(isolate, node);
	auto isInterface = s_metadataReader.IsNodeTypeInterface(treeNode->type);
	auto funcCallback = isInterface ? InterfaceConstructorCallback : ClassConstructorCallback;
	ctorFuncTemplate = FunctionTemplate::New(isolate, funcCallback, ctorCallbackData);
	ctorFuncTemplate->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(ObjectManager::MetadataNodeKeys::END));

	auto baseClass = s_metadataReader.GetBaseClassNode(treeNode);
	Local<Function> baseCtorFunc;
	vector<MethodCallbackData*> baseInstanceMethodsCallbackData;
	if ((baseClass != treeNode) && (baseClass != nullptr) && (baseClass->offsetValue > 0))
	{
		auto baseFuncTemplate = GetConstructorFunctionTemplate(isolate, baseClass, baseInstanceMethodsCallbackData);
		if (!baseFuncTemplate.IsEmpty())
		{
			ctorFuncTemplate->Inherit(baseFuncTemplate);
			baseCtorFunc = baseFuncTemplate->GetFunction();
		}
	}

	auto prototypeTemplate = ctorFuncTemplate->PrototypeTemplate();

	auto ctorFunc = node->SetMembers(isolate, ctorFuncTemplate, prototypeTemplate, instanceMethodsCallbackData, baseInstanceMethodsCallbackData, treeNode);
	if (!baseCtorFunc.IsEmpty())
	{
		ctorFunc->SetPrototype(baseCtorFunc);
	}

	auto pft = new Persistent<FunctionTemplate>(isolate, ctorFuncTemplate);
	CtorCacheItem ctorCacheItem(pft, instanceMethodsCallbackData);
	s_ctorFuncCache.insert(make_pair(treeNode, ctorCacheItem));

	SetInnnerTypes(isolate, ctorFunc, treeNode);

	SetTypeMetadata(isolate, ctorFunc, new TypeMetadata(s_metadataReader.ReadTypeName(treeNode)));

	return ctorFuncTemplate;
}

Local<Function> MetadataNode::GetConstructorFunction(Isolate *isolate)
{
	auto ctorFuncTemplate = GetConstructorFunctionTemplate(isolate, m_treeNode);
	auto ctorFunc = ctorFuncTemplate->GetFunction();
	return ctorFunc;
}




MetadataNode::TypeMetadata* MetadataNode::GetTypeMetadata(Isolate *isolate, const Local<Function>& value)
{
	auto data = reinterpret_cast<TypeMetadata*>(V8GetHiddenValue(value, "typemetadata").As<External>()->Value());
	return data;
}

void MetadataNode::SetTypeMetadata(Isolate *isolate, Local<Function> value, TypeMetadata *data)
{
	V8SetHiddenValue(value, "typemetadata", External::New(isolate, data));
}

MetadataNode* MetadataNode::GetInstanceMetadata(Isolate *isolate, const Local<Object>& value)
{
	MetadataNode *node = nullptr;
	auto key = Local<String>::New(isolate, *s_metadataKey);
	auto ext = value->GetHiddenValue(key);
	if (!ext.IsEmpty())
	{
		node = reinterpret_cast<MetadataNode*>(ext.As<External>()->Value());
	}
	return node;
}

void MetadataNode::SetInstanceMetadata(Isolate *isolate, Local<Object> value, MetadataNode *node)
{
	auto key = Local<String>::New(isolate, *s_metadataKey);
	value->SetHiddenValue(key, External::New(isolate, node));
}

MetadataNode* MetadataNode::GetPackageMetadata(Isolate *isolate, const Local<Object>& value)
{
	MetadataNode *node = nullptr;
	auto ext =  value->GetHiddenValue(ConvertToV8String("tns::PackageMetadata"));
	if (!ext.IsEmpty())
	{
		node = reinterpret_cast<MetadataNode*>(ext.As<External>()->Value());
	}
	return node;
}

void MetadataNode::SetPackageMetadata(Isolate *isolate, Local<Object> value, MetadataNode *node)
{
	value->SetHiddenValue(ConvertToV8String("tns::PackageMetadata"), External::New(isolate, node));
}


void MetadataNode::ExtendedClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	try {
	SET_PROFILER_FRAME();

	assert(info.IsConstructCall());

	auto isolate = info.GetIsolate();
	auto thiz = info.This();
	auto extData = reinterpret_cast<ExtendedClassData*>(info.Data().As<External>()->Value());

	auto implementationObject = Local<Object>::New(isolate, *extData->implementationObject);

	const auto& extendName = extData->extendedName;

	SetInstanceMetadata(isolate, thiz, extData->node);
	thiz->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper), True(isolate));
	thiz->SetHiddenValue(ConvertToV8String("t::implObj"), implementationObject);

	ArgsWrapper argWrapper(info, ArgType::Class, Local<Object>());

	string fullClassName = extData->fullClassName;

	bool success = NativeScriptRuntime::RegisterInstance(thiz, fullClassName, argWrapper, implementationObject, false);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}


void MetadataNode::InterfaceConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	try {
	SET_PROFILER_FRAME();

	auto isolate = info.GetIsolate();
	auto thiz = info.This();
	auto node = reinterpret_cast<MetadataNode*>(info.Data().As<External>()->Value());

	Local<Object> implementationObject;
	Local<String> v8ExtendName;
	string extendLocation;
	bool extendLocationFound = GetExtendLocation(extendLocation);
	if (info.Length() == 1)
	{
		if (!extendLocationFound)
		{
			stringstream ss;
			ss << "(InternalError): Invalid extend() call. No name specified for extend. Location: " << extendLocation.c_str();
			throw NativeScriptException(ss.str());
		}

		if (!info[0]->IsObject())
		{
			throw NativeScriptException(string("First argument must be implementation object"));
		}
		implementationObject = info[0]->ToObject();
	}
	else if (info.Length() == 2)
	{
		if (!info[0]->IsString())
		{
			throw NativeScriptException(string("First argument must be string"));
		}
		if (!info[1]->IsObject())
		{
			throw NativeScriptException(string("Second argument must be implementation object"));
		}

		DEBUG_WRITE("InterfaceConstructorCallback: getting extend name");
		v8ExtendName = info[0]->ToString();
		implementationObject = info[1]->ToObject();
	}
	else
	{
		throw NativeScriptException(string("Invalid number of arguments"));
	}

	auto className = node->m_implType;
	auto extendName = ConvertToString(v8ExtendName);
	auto extendNameAndLocation = extendLocation + extendName;
	SetInstanceMetadata(isolate, implementationObject, node);

	//@@@ Refactor
	thiz->SetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper), True(isolate));

	string fullClassName = CreateFullClassName(className, extendNameAndLocation);

	implementationObject->SetPrototype(thiz->GetPrototype());
	thiz->SetPrototype(implementationObject);
	thiz->SetHiddenValue(ConvertToV8String("t::implObj"), implementationObject);

	ArgsWrapper argWrapper(info, ArgType::Interface, Local<Object>());

	auto success = NativeScriptRuntime::RegisterInstance(thiz, fullClassName, argWrapper, implementationObject, true);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void MetadataNode::ClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	try {
	SET_PROFILER_FRAME();

	auto thiz = info.This();
	auto node = reinterpret_cast<MetadataNode*>(info.Data().As<External>()->Value());

	Local<Object> outerThis;
	string extendName;
	auto className = node->m_name;

	SetInstanceMetadata(info.GetIsolate(), thiz, node);

	ArgsWrapper argWrapper(info, ArgType::Class, outerThis);

	string fullClassName = CreateFullClassName(className, extendName);
	bool success = NativeScriptRuntime::RegisterInstance(thiz, fullClassName, argWrapper, outerThis, false);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void MetadataNode::MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	try {
		SET_PROFILER_FRAME();

		auto e = info.Data().As<External>();

		auto callbackData = reinterpret_cast<MethodCallbackData*>(e->Value());

		int argLength = info.Length();

		MetadataEntry *entry = nullptr;

		string *className;
		const auto& first = callbackData->candidates.front();
		const auto& methodName = first.name;

		while ((callbackData != nullptr) && (entry == nullptr))
		{
			auto& candidates = callbackData->candidates;

			className = &callbackData->node->m_name;

			auto found = false;
			for (auto& c: candidates)
			{
				found = c.paramCount == argLength;
				if (found)
				{
					entry = &c;
					break;
				}
			}

			if (!found)
			{
				callbackData = callbackData->parent;
			}
		}

		auto thiz = info.This();

		auto isSuper = false;
		if (!first.isStatic)
		{
			auto superValue = thiz->GetInternalField(static_cast<int>(ObjectManager::MetadataNodeKeys::CallSuper));
			isSuper = !superValue.IsEmpty() && superValue->IsTrue();
		}

	//	// TODO: refactor this
		if (isSuper && (*className == "com/tns/NativeScriptActivity"))
		{
			string activityBaseClassName("android/app/Activity");
			className = &activityBaseClassName;
		}

		if ((argLength == 0) && (methodName == V8StringConstants::VALUE_OF))
		{
			info.GetReturnValue().Set(thiz);
		}
		else
		{
			NativeScriptRuntime::CallJavaMethod(thiz, *className, methodName, entry, first.isStatic, isSuper, info);
		}
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void MetadataNode::ArrayIndexedPropertyGetterCallback(uint32_t index, const PropertyCallbackInfo<Value>& info)
{
	try {
	auto node = GetNodeFromHandle(info.This());

	auto element = NativeScriptRuntime::GetArrayElement(info.This(), index, node->m_name);

	info.GetReturnValue().Set(element);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

void MetadataNode::ArrayIndexedPropertySetterCallback(uint32_t index, Local<Value> value, const PropertyCallbackInfo<Value>& info)
{
	try {
		auto node = GetNodeFromHandle(info.This());

		NativeScriptRuntime::SetArrayElement(info.This(), index, node->m_name, value);

		info.GetReturnValue().Set(value);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

Local<Object> MetadataNode::GetImplementationObject(const Local<Object>& object)
{
	DEBUG_WRITE("GetImplementationObject called  on object:%d", object->GetIdentityHash());

	auto target = object;
	Local<Value> currentPrototype = target;

	Local<Object> implementationObject;

	implementationObject = object->GetHiddenValue(ConvertToV8String("t::implObj")).As<Object>();
	if (!implementationObject.IsEmpty())
	{
		return implementationObject;
	}

	if (object->HasOwnProperty(V8StringConstants::GetIsPrototypeImplementationObject()))
	{
		auto v8Prototype = V8StringConstants::GetPrototype();
		if (!object->HasOwnProperty(v8Prototype))
		{
			return Local<Object>();
		}

		DEBUG_WRITE("GetImplementationObject returning the prototype of the object :%d", object->GetIdentityHash());
		return object->Get(v8Prototype).As<Object>();
	}

	auto obj = V8GetHiddenValue(object, "t::ActivityImplementationObject").As<Object>();
	if (!obj.IsEmpty())
	{
		DEBUG_WRITE("GetImplementationObject returning ActivityImplementationObject property on object: %d", object->GetIdentityHash());
		return obj;
	}

	Local<Value> lastPrototype;
	bool prototypeCycleDetected = false;
	while (implementationObject.IsEmpty())
	{
		//
		currentPrototype = currentPrototype.As<Object>()->GetPrototype();

		if (currentPrototype->IsNull())
			break;

		//DEBUG_WRITE("GetImplementationObject currentPrototypeObject:%d", (currentPrototype.IsEmpty() || currentPrototype.As<Object>().IsEmpty()) ? -1 :  currentPrototype.As<Object>()->GetIdentityHash());
		//DEBUG_WRITE("GetImplementationObject lastPrototypeObject:%d", (lastPrototype.IsEmpty() || lastPrototype.As<Object>().IsEmpty()) ? -1 :  lastPrototype.As<Object>()->GetIdentityHash());

		if (currentPrototype == lastPrototype)
		{
			auto abovePrototype = currentPrototype.As<Object>()->GetPrototype();
			prototypeCycleDetected = abovePrototype == currentPrototype;
		}

		if (currentPrototype.IsEmpty() || prototypeCycleDetected)
		{
			//Local<Value> abovePrototype = currentPrototype.As<Object>()->GetPrototype();
			//DEBUG_WRITE("GetImplementationObject not found since cycle parents reached abovePrototype:%d", (abovePrototype.IsEmpty() || abovePrototype.As<Object>().IsEmpty()) ? -1 :  abovePrototype.As<Object>()->GetIdentityHash());
			return Local<Object>();
		}
		else
		{
			auto value = currentPrototype.As<Object>()->GetHiddenValue(V8StringConstants::GetClassImplementationObject());

			if (!value.IsEmpty())
			{
				implementationObject = currentPrototype.As<Object>();
			}
		}

		lastPrototype = currentPrototype;
	}

	return implementationObject;
}

void MetadataNode::PackageGetterCallback(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	try {
	string propName = ConvertToString(property);

	if (propName.empty())
		return;

	auto isolate = Isolate::GetCurrent();
	HandleScope handleScope(isolate);

	auto thiz = info.This();

	auto cachedItem = thiz->GetHiddenValue(property);

	if (cachedItem.IsEmpty())
	{
		auto node = GetPackageMetadata(isolate, thiz);

		uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);

		DEBUG_WRITE("MetadataNode::GetterCallback: prop '%s' for node '%s' called, nodeType=%d, hash=%d", propName.c_str(), node->m_treeNode->name.c_str(), nodeType, thiz.IsEmpty() ? -42 : thiz->GetIdentityHash());

		auto child = GetChildMetadataForPackage(node, propName);
		auto foundChild = child.treeNode != nullptr;

		if (foundChild)
		{
			auto childNode = MetadataNode::GetOrCreateInternal(child.treeNode);
			cachedItem = childNode->CreateWrapper(isolate);
			thiz->SetHiddenValue(property, cachedItem);
		}
	}

	info.GetReturnValue().Set(cachedItem);
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}




bool MetadataNode::ValidateExtendArguments(const FunctionCallbackInfo<Value>& info, string& extendLocation, v8::Local<v8::String>& extendName, Local<Object>& implementationObject)
{
	bool extendLocationFound = GetExtendLocation(extendLocation);

	if (info.Length() == 1)
	{
		if (!extendLocationFound)
		{
			stringstream ss;
			ss << "Invalid extend() call. No name specified for extend at location: " << extendLocation.c_str();
			string exceptionMessage = ss.str();

			throw NativeScriptException(exceptionMessage);
		}


		if (!info[0]->IsObject())
		{
			stringstream ss;
			ss << "Invalid extend() call. No implementation object specified at location: " << extendLocation.c_str();
			string exceptionMessage = ss.str();

			throw NativeScriptException(exceptionMessage);
		}

		implementationObject = info[0]->ToObject();
	}
	else if (info.Length() == 2)
	{
		if (!info[0]->IsString())
		{
			stringstream ss;
			ss << "Invalid extend() call. No name for extend specified at location: " << extendLocation.c_str();
			string exceptionMessage = ss.str();

			throw NativeScriptException(exceptionMessage);
		}

		if (!info[1]->IsObject())
		{
			stringstream ss;
			ss << "Invalid extend() call. Named extend should be called with second object parameter containing overridden methods at location: " << extendLocation.c_str();
			string exceptionMessage = ss.str();

			throw NativeScriptException(exceptionMessage);
		}

		DEBUG_WRITE("ExtendsCallMethodHandler: getting extend name");
		extendName = info[0]->ToString();
		bool isValidExtendName = IsValidExtendName(extendName);
		if(!isValidExtendName)
		{
			stringstream ss;
			ss << "The extend name \"" << ConvertToString(extendName) << "\" you provided contains invalid symbols. Try using the symbols [a-z, A-Z, 0-9, _]." << endl;
			string exceptionMessage = ss.str();

			throw NativeScriptException(exceptionMessage);
		}
		implementationObject = info[1]->ToObject();
	}
	else
	{
		stringstream ss;
		ss << "Invalid extend() call at location: " << extendLocation.c_str();
		string exceptionMessage = ss.str();

		throw NativeScriptException(exceptionMessage);
	}

	return true;
}


MetadataNode::ExtendedClassCacheData MetadataNode::GetCachedExtendedClassData(Isolate *isolate, const string& proxyClassName)
{

	ExtendedClassCacheData cacheData;
	auto itFound = s_extendedCtorFuncCache.find(proxyClassName);
	if (itFound != s_extendedCtorFuncCache.end())
	{
		cacheData = itFound->second;
	}

	return cacheData;
}

string MetadataNode::CreateFullClassName(const std::string& className, const std::string& extendNameAndLocation = "")
{
	string fullClassName = className;

	if(!extendNameAndLocation.empty())
	{
		fullClassName += Constants::CLASS_NAME_LOCATION_SEPARATOR + extendNameAndLocation;
	}

	return fullClassName;
}

void MetadataNode::ExtendCallMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	try {
	if (info.IsConstructCall())
	{
		string exMsg("Cannot call 'extend' as constructor");
		throw NativeScriptException(exMsg);
	}

	SET_PROFILER_FRAME();

	Local<Object> implementationObject;
	Local<String> extendName;
	string extendLocation;
	auto validArgs = ValidateExtendArguments(info, extendLocation, extendName, implementationObject);

	if (!validArgs)
		return;

	auto node = reinterpret_cast<MetadataNode*>(info.Data().As<External>()->Value());

	DEBUG_WRITE("ExtendsCallMethodHandler: called with %s", ConvertToString(extendName).c_str());

	string extendNameAndLocation = extendLocation + ConvertToString(extendName);
	auto fullClassName = TNS_PREFIX + CreateFullClassName(node->m_name, extendNameAndLocation);


	//
	//resolve class (pre-generated or generated runtime from dex generator)
	jclass resolvedClass = NativeScriptRuntime::ResolveClass(fullClassName, implementationObject); //resolve class returns GlobalRef
	std::string generatedFullClassName = s_objectManager->GetClassName(resolvedClass);
	//

	auto fullExtendedName = generatedFullClassName;
	DEBUG_WRITE("ExtendsCallMethodHandler: extend full name %s", fullClassName.c_str());

	auto isolate = info.GetIsolate();
	auto cachedData = GetCachedExtendedClassData(isolate, fullExtendedName);
	if (cachedData.extendedCtorFunction != nullptr)
	{
		auto cachedExtendedCtorFunc = Local<Function>::New(isolate, *cachedData.extendedCtorFunction);
		info.GetReturnValue().Set(cachedExtendedCtorFunc);
		return;
	}

	auto implementationObjectPropertyName = V8StringConstants::GetClassImplementationObject();
	//reuse validation - checks that implementationObject is not reused for different classes
	auto implementationObjectProperty = implementationObject->GetHiddenValue(implementationObjectPropertyName).As<String>();
	if (implementationObjectProperty.IsEmpty())
	{
		//mark the implementationObject as such and set a pointer to it's class node inside it for reuse validation later
		implementationObject->SetHiddenValue(implementationObjectPropertyName, String::NewFromUtf8(isolate, fullExtendedName.c_str()));
	}
	else
	{
		string usedClassName = ConvertToString(implementationObjectProperty);
		stringstream s;
		s << "This object is used to extend another class '" << usedClassName << "'";
		throw NativeScriptException(s.str());
	}

	auto baseClassCtorFunc = node->GetConstructorFunction(isolate);
	auto extendData = External::New(isolate, new ExtendedClassData(node, extendNameAndLocation, implementationObject, fullExtendedName));
	auto extendFuncTemplate = FunctionTemplate::New(isolate, ExtendedClassConstructorCallback, extendData);
	extendFuncTemplate->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(ObjectManager::MetadataNodeKeys::END));

	auto extendFunc = extendFuncTemplate->GetFunction();
	auto prototypeName = ConvertToV8String("prototype");
	implementationObject->SetPrototype(baseClassCtorFunc->Get(prototypeName));
	implementationObject->SetAccessor(ConvertToV8String("super"), SuperAccessorGetterCallback, nullptr, implementationObject);

	auto extendFuncPrototype = extendFunc->Get(prototypeName).As<Object>();
	auto p = extendFuncPrototype->GetPrototype();
	extendFuncPrototype->SetPrototype(implementationObject);
	extendFunc->SetPrototype(baseClassCtorFunc);


	SetClassAccessor(extendFunc);
	SetTypeMetadata(isolate, extendFunc, new TypeMetadata(fullExtendedName));
	info.GetReturnValue().Set(extendFunc);

	s_name2NodeCache.insert(make_pair(fullExtendedName, node));

	ExtendedClassCacheData cacheData(extendFunc, fullExtendedName, node);
	s_extendedCtorFuncCache.insert(make_pair(fullExtendedName, cacheData));
	} catch (NativeScriptException& e) {
		e.ReThrowToV8();
	}
	catch (exception e) {
		DEBUG_WRITE("Error: c++ exception: %s", e.what());
	}
	catch (...) {
		DEBUG_WRITE("Error: c++ exception!");
	}
}

bool MetadataNode::IsValidExtendName(const Local<String>& name)
{
	string extendNam = ConvertToString(name);

	for(int i = 0; i < extendNam.size(); i++)
	{
		char currentSymbol = extendNam[i];
		bool isValidExtendNameSymbol = isalpha(currentSymbol) ||
										isdigit(currentSymbol) ||
										currentSymbol == '_';
		if(!isValidExtendNameSymbol)
		{
			return false;
		}
	}

	return true;
}


bool MetadataNode::GetExtendLocation(string& extendLocation)
{
	stringstream extendLocationStream;
	auto stackTrace = StackTrace::CurrentStackTrace(Isolate::GetCurrent(), 1, StackTrace::kOverview);
	if (!stackTrace.IsEmpty())
	{
		auto frame = stackTrace->GetFrame(0);
		if (!frame.IsEmpty())
		{
			auto scriptName = frame->GetScriptName();
			if (scriptName.IsEmpty())
			{
				extendLocationStream << "unkown location";
				extendLocation = extendLocationStream.str();
				return false;
			}

			string srcFileName = ConvertToString(scriptName);

			string hardcodedPathToSkip = Constants::APP_ROOT_FOLDER_PATH;

			int startIndex = hardcodedPathToSkip.length();
			int strToTakeLen = (srcFileName.length() - startIndex - 3); // 3 refers to .js at the end of file name
			string fullPathToFile = srcFileName.substr(startIndex, strToTakeLen);

			std::replace(fullPathToFile.begin(), fullPathToFile.end(), '/', '_');
			std::replace(fullPathToFile.begin(), fullPathToFile.end(), '.', '_');
			int lineNumber = frame->GetLineNumber();
			if (lineNumber < 0)
			{
				extendLocationStream << fullPathToFile.c_str() << " unkown line number";
				extendLocation = extendLocationStream.str();
				return false;
			}

			int column = frame->GetColumn();
			if (column < 0)
			{
				extendLocationStream << fullPathToFile.c_str() << " line:" << lineNumber << " unkown column number";
				extendLocation = extendLocationStream.str();
				return false;
			}


			extendLocationStream << "f" << fullPathToFile.c_str() << "_l" << lineNumber << "_c" << column << "__";
			//DEBUG_WRITE("EXTEND_LOCATION %s", extendLocationStream.str().c_str());
		}
	}

	extendLocation = extendLocationStream.str();
	return true;
}


MetadataNode* MetadataNode::GetNodeFromHandle(const Local<Object>& value)
{
	auto node = GetInstanceMetadata(Isolate::GetCurrent(), value);
	return node;
}



MetadataEntry MetadataNode::GetChildMetadataForPackage(MetadataNode *node, const string& propName)
{
	MetadataEntry child;

	assert(node->m_treeNode->children != nullptr);

	const auto& children = *node->m_treeNode->children;

	for (auto treeNodeChild: children)
	{
		if (propName == treeNodeChild->name)
		{
			child.name = propName;
			child.treeNode = treeNodeChild;

			uint8_t childNodeType = s_metadataReader.GetNodeType(treeNodeChild);
			if (s_metadataReader.IsNodeTypeInterface(childNodeType))
			{
				bool isPrefix;
				string declaringType = s_metadataReader.ReadInterfaceImplementationTypeName(treeNodeChild, isPrefix);
				child.declaringType = isPrefix
										? (declaringType + s_metadataReader.ReadTypeName(child.treeNode))
										: declaringType;
			}
		}
	}

	return child;
}








void MetadataNode::BuildMetadata(uint32_t nodesLength, uint8_t *nodeData, uint32_t nameLength, uint8_t *nameData, uint32_t valueLength, uint8_t *valueData)
{
	s_metadataReader = MetadataReader(nodesLength, nodeData, nameLength, nameData, valueLength, valueData, NativeScriptRuntime::GetTypeMetadata);
}

void MetadataNode::CreateTopLevelNamespaces(const Local<Object>& global)
{
	auto root = s_metadataReader.GetRoot();

	auto isolate = Isolate::GetCurrent();

	const auto& children = *root->children;

	for (auto treeNode: children)
	{
		uint8_t nodeType = s_metadataReader.GetNodeType(treeNode);

		if (nodeType == MetadataTreeNode::PACKAGE)
		{
			auto node = GetOrCreateInternal(treeNode);

			auto packageObj = node->CreateWrapper(isolate);
			global->Set(ConvertToV8String(node->m_treeNode->name), packageObj);
		}
	}
}

void MetadataNode::InjectPrototype(Local<Object>& target, Local<Object>& implementationObject)
{
	auto isolate = Isolate::GetCurrent();

	implementationObject->SetAccessor(ConvertToV8String("super"), SuperAccessorGetterCallback, nullptr, implementationObject);
	implementationObject->SetPrototype(target->GetPrototype());
	target->SetPrototype(implementationObject);
}


string MetadataNode::TNS_PREFIX = "com/tns/gen/";
std::map<std::string, MetadataNode*> MetadataNode::s_name2NodeCache;
std::map<std::string, MetadataTreeNode*> MetadataNode::s_name2TreeNodeCache;
std::map<MetadataTreeNode*, MetadataNode*> MetadataNode::s_treeNode2NodeCache;

MetadataReader MetadataNode::s_metadataReader;
ObjectManager* MetadataNode::s_objectManager = nullptr;

Persistent<String>* MetadataNode::s_metadataKey = nullptr;
map<MetadataTreeNode*, MetadataNode::CtorCacheItem> MetadataNode::s_ctorFuncCache;
map<string, MetadataNode::ExtendedClassCacheData> MetadataNode::s_extendedCtorFuncCache;
