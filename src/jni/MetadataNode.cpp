#include "MetadataNode.h"
#include "NativeScriptAssert.h"
#include "MethodCache.h"
#include "Constants.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "ExceptionUtil.h"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <assert.h>

using namespace v8;
using namespace std;
using namespace tns;

void MetadataNode::SubscribeCallbacks(GetJavaFieldCallback getJavaFieldCallback,
									SetJavaFieldCallback setJavaFieldCallback,
									GetArrayElementCallback getArrayElementCallback,
									SetArrayElementCallback setArrayElementCallback,
									MakeClassInstanceOfTypeStrongCallback makeClassInstanceOfTypeStrongCallback,
									CallJavaMethodCallback callJavaMethodCallback,
									RegisterInstanceCallback registerInstanceCallback,
									GetTypeMetadataCallback getTypeMetadataCallback)
{
	s_getJavaField = getJavaFieldCallback;
	s_setJavaField = setJavaFieldCallback;
	s_getArrayElement = getArrayElementCallback;
	s_setArrayElement = setArrayElementCallback;
	s_makeClassInstanceOfTypeStrong = makeClassInstanceOfTypeStrongCallback;
	s_callJavaMethod = callJavaMethodCallback;
	s_registerInstance = registerInstanceCallback;
	s_getTypeMetadata = getTypeMetadataCallback;
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
		string impTypeName = s_metadataReader.ReadInterfaceImplementationTypeName(m_treeNode, isPrefix);
		m_implType = isPrefix
					? (impTypeName + m_name)
					: impTypeName;
	}
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

Handle<Object> MetadataNode::CreateProxy(Isolate *isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<Object> obj;

	uint8_t nodeType = s_metadataReader.GetNodeType(m_treeNode);

	bool isClass = s_metadataReader.IsNodeTypeClass(nodeType),
		isInterface = s_metadataReader.IsNodeTypeInterface(nodeType);

	if (isClass || isInterface)
	{
		obj = CreateClassProxy(isolate, isClass);
	}
	else if (s_metadataReader.IsNodeTypePackage(nodeType))
	{
		obj = CreatePackageProxy(isolate);
	}
	else if (m_isArray)
	{
		obj = CreateClassProxy(isolate, isClass);
	}
	else
	{
		ASSERT_FAIL("Can't create proxy for this type=%d", nodeType);
	}

	return handle_scope.Escape(obj);
}

Handle<Object> MetadataNode::CreatePackageProxy(Isolate *isolate)
{
	EscapableHandleScope handleScope(isolate);

	auto packageTemplate = ObjectTemplate::New();
	packageTemplate->SetNamedPropertyHandler(MetadataNode::GetterCallback, MetadataNode::SetterCallback);

	auto packagePtr = External::New(isolate, this);
	auto instance = packageTemplate->NewInstance();
	V8SetHiddenValue(instance, METADATA_NODE_KEY_NAME, packagePtr);

	SetDebugName(m_name, instance);

	return handleScope.Escape(instance);
}

Handle<Object> MetadataNode::GetExistingClassProxy(const string& name)
{
	auto isolate = Isolate::GetCurrent();

	Handle<Object> empty;
	auto it = MetadataNode::s_classProxies.find(name);
	if (it == MetadataNode::s_classProxies.end())
	{
		return empty;
	}

	auto classProxy = it->second;
	return Handle<Object>::New(isolate, *classProxy);
}

bool MetadataNode::ExistsExtendName(const string& name)
{
	auto it = MetadataNode::s_usedExtendNames.find(name);
	if (it == MetadataNode::s_usedExtendNames.end())
	{
		return false;
	}

	return true;
}

bool MetadataNode::IsArray()
{
	char prefix = *(m_name.begin());

	return prefix == '[';
}

bool MetadataNode::IsClassMetadataName(const string& name)
{
	bool isClassMetadataName = false;

	size_t found = name.find_last_of(".");
	if (found != string::npos)
	{
		size_t next = found + 1;

		if (next < name.size())
		{
			string className = name.substr(next);
			isClassMetadataName = className == "class";
		}
	}

	DEBUG_WRITE("IsClassMetadataName name=%s, ret=%d", name.c_str(), isClassMetadataName);

	return isClassMetadataName;
}

bool MetadataNode::IsMarkedAsSuper(const Handle<Object>& obj)
{
	return !obj->GetHiddenValue(V8StringConstants::GetTSuper()).IsEmpty();
}

Handle<Object> MetadataNode::CreateClassProxy(Isolate *isolate, bool isClass)
{
	auto existingClassProxy = GetExistingClassProxy(m_name);

	if (!existingClassProxy.IsEmpty())
	{
		return existingClassProxy;
	}

	Handle<Object> instance;
	Handle<Function> classProxyFunction;
	Handle<Object> classProxy;

	if (IsClassMetadataName(m_name))
	{
		auto clazz = MetadataNode::GetOrCreateInternal("java.lang.Class");
		instance = clazz->CreateInstanceProxy(Isolate::GetCurrent());
		s_makeClassInstanceOfTypeStrong(this->m_treeNode->name, instance);
		classProxy = instance;
	}
	else
	{
		auto objectTemplate = ObjectTemplate::New();
		objectTemplate->SetNamedPropertyHandler(MetadataNode::GetterCallback, MetadataNode::SetterCallback);
		auto external = External::New(isolate, this);
		instance = objectTemplate->NewInstance();
		V8SetHiddenValue(instance, METADATA_NODE_KEY_NAME, external);

		DEBUG_WRITE("CreateClassProxy: ClassProxy instance is  id:%d", instance->GetIdentityHash());

		auto funcTemplate = FunctionTemplate::New(isolate, MetadataNode::ConstructorCallback);
		funcTemplate->ReadOnlyPrototype();
		classProxyFunction = funcTemplate->GetFunction();
		bool success = classProxyFunction->SetPrototype(instance); //this is needed for static functions like so: var MyButton = Button.extends(); MyButton.StaticMethod();
		ASSERT_MESSAGE(success, "CreateClassProxy: SetPrototype failed");
		V8SetHiddenValue(classProxyFunction, METADATA_NODE_KEY_NAME, external);

		if (isClass)
		{
			V8SetHiddenValue(classProxyFunction, CLASS_PROXY, Boolean::New(isolate, true));

			success = classProxyFunction->SetHiddenValue(V8StringConstants::GetTSuper(), Boolean::New(isolate, true));
			ASSERT_MESSAGE(success, "CreateClassProxy: mark classProxyFunction as super object failed");
		}

		classProxy = classProxyFunction;
	}

	SetDebugName(m_name, classProxy);

	auto classProxyPersistent = new Persistent<Object>(isolate, classProxy);

	DEBUG_WRITE("CreateClassProxy: ClassProxy for %s created id:%d", m_name.c_str(), classProxy->GetIdentityHash());
	s_classProxies[m_name] = classProxyPersistent;
	return classProxy;
}

void MetadataNode::SetDebugName(const string& name, const Handle<Object>& value)
{
	value->Set(V8StringConstants::GetDebugName(), ConvertToV8String(name));
}

void MetadataNode::ConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	auto thiz = args.Callee();

	auto e = V8GetHiddenValue(thiz, METADATA_NODE_KEY_NAME).As<External>();

//	if (e.IsEmpty())
//	{
//		thiz = args.Callee();
//		e = V8GetHiddenValue(thiz, METADATA_NODE_KEY_NAME).As<External>();
//	}

	ASSERT_MESSAGE(!e.IsEmpty(), "ConstructorCallback: MetadataNode not found");

	auto node = (MetadataNode*) e->Value();
	uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);

	if (s_metadataReader.IsNodeTypeClass(nodeType))
	{
		InvokeClassCtor(args, node);
	}
	else if (s_metadataReader.IsNodeTypeInterface(nodeType))
	{
		InvokeInterfaceCtor(args, node);
	}
	else
	{
		ASSERT_FAIL("ConstructorCallback: Metadata type not supported when invoking method");
	}
}

void MetadataNode::MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	auto e = args.Data().As<External>();

	auto callbackState = (MethodCallbackState*) e->Value();

	const auto& candidates = callbackState->candidates;

	string className = callbackState->node->m_name;
	string methodName = candidates.front().name;

	auto callee = args.Callee();

	auto This = args.This();

	//TODO: Do not allow passing This object that is not created by the callee function except when This is the implementation object
//	if (This->GetConstructor() != callee)
//	{
//	  throw exception here
//	}

	if (methodName == V8StringConstants::EXTEND)
	{
		ExtendCallMethodHandler(args, callbackState->node);
		return;
	}

	auto superValue = This->GetHiddenValue(V8StringConstants::GetTSuper());
	bool isMarkedAsSuper = !superValue.IsEmpty();

	if (superValue.IsEmpty()) //this handles Button1.prototype.someMethod.call(this, args) which is equivalent of this.super.someMethod(args)
	{
		isMarkedAsSuper = callbackState->isSuper;
	}

	int argLength = args.Length();

	int candicateCount = 0;
	MetadataEntry entry;

	for (auto curEntry: candidates)
	{
		if (curEntry.paramCount == argLength)
		{
			entry = curEntry;
			++candicateCount;
		}
	}

	bool isStatic = false;
	if (candicateCount == 1)
	{
		isStatic = entry.isStatic;
	}

	Handle<Object> instance;

	if (!isStatic)
	{
		if (isMarkedAsSuper)
		{
			auto superInstance = V8GetHiddenValue(This, "t::superJsInstance");

			if (!superInstance.IsEmpty())
			{
				instance = superInstance.As<Object>();
			}
			else
			{
				//this handles Button1.prototype.Method.call(this, args) which is equivalent of this.super.Method(args)
				instance = This;
			}
		}
		else
		{
			instance = This;
		}
	}

	bool isSuper = !entry.isStatic && callbackState->isSuper;

	// TODO: refactor this
	if (isSuper && (className == "com/tns/NativeScriptActivity"))
	{
		className = "android/app/Activity";
	}

	if (candicateCount == 0)
	{
		if ((methodName == V8StringConstants::VALUE_OF) && (argLength == 0))
		{
			args.GetReturnValue().Set(args.This());
		}
		else
		{
			//TODO: throw js exception here
			ASSERT_FAIL("Method %s.%s not found. Argc:%d", className.c_str(), methodName.c_str(), argLength);
		}
	}
	else if (candicateCount == 1)
	{
		s_callJavaMethod(instance, className, methodName, entry.sig, entry.declaringType, entry.isStatic, isSuper, args);
	}
	else
	{
		if ((methodName == V8StringConstants::VALUE_OF) && (argLength == 0))
		{
			args.GetReturnValue().Set(args.This());
		}
		else
		{
			string methodSignature = ResolveMethodOverload(args, className, entry.name);
			s_callJavaMethod(instance, className, methodName, methodSignature, entry.declaringType, entry.isStatic, isSuper, args);
		}
	}
}


Handle<Object> MetadataNode::CreateInstanceProxy(Isolate *isolate)
{
	EscapableHandleScope handleScope(isolate);

	auto objectTemplate = ObjectTemplate::New();
	objectTemplate->SetNamedPropertyHandler(MetadataNode::GetterCallback, MetadataNode::SetterCallback);

	bool isArray = IsArray();
	if (isArray)
	{
		objectTemplate->SetIndexedPropertyHandler(ArrayIndexedPropertyGetterCallback, ArrayIndexedPropertySetterCallback);
	}

	auto instance = objectTemplate->NewInstance();
	V8SetHiddenValue(instance, METADATA_NODE_KEY_NAME, External::New(isolate, this));
	DEBUG_WRITE("InstanceProxy created %s@%d", m_name.c_str(), instance->GetIdentityHash());

	SetDebugName(m_name, instance);

	return handleScope.Escape(instance);
}

void MetadataNode::ArrayIndexedPropertyGetterCallback(uint32_t index, const PropertyCallbackInfo<Value>& info)
{
	auto node = GetNodeFromHandle(info.This());

	auto element = s_getArrayElement(info.This(), index, node->m_name);

	info.GetReturnValue().Set(element);
}

void MetadataNode::ArrayIndexedPropertySetterCallback(uint32_t index, Local<Value> value, const PropertyCallbackInfo<Value>& info)
{
	auto node = GetNodeFromHandle(info.This());

	s_setArrayElement(info.This(), index, node->m_name, value);
}

Handle<Object> MetadataNode::GetImplementationObject(const Handle<Object>& object)
{
	DEBUG_WRITE("GetImplementationObject called  on object:%d", object->GetIdentityHash());

	auto target = object;
	Handle<Value> currentPrototype = target;

	Handle<Object> implementationObject;

	if (object->HasOwnProperty(V8StringConstants::GetIsPrototypeImplementationObject()))
	{
		auto v8Prototype = V8StringConstants::GetPrototype();
		if (!object->HasOwnProperty(v8Prototype))
		{
			return Handle<Object>();
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

	Handle<Value> lastPrototype;
	bool prototypeCycleDetected = false;
	while (implementationObject.IsEmpty())
	{
		if (!currentPrototype.IsEmpty() && currentPrototype->IsObject())
		{
			auto x = V8GetHiddenValue(currentPrototype.As<Object>(), "t::ext");
			if (!x.IsEmpty())
			{
				return x.As<Object>();
			}
		}
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
			//Handle<Value> abovePrototype = currentPrototype.As<Object>()->GetPrototype();
			//DEBUG_WRITE("GetImplementationObject not found since cycle parents reached abovePrototype:%d", (abovePrototype.IsEmpty() || abovePrototype.As<Object>().IsEmpty()) ? -1 :  abovePrototype.As<Object>()->GetIdentityHash());
			return Handle<Object>();
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

void MetadataNode::GetterCallback(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
	string propName = ConvertToString(property);

	if (propName.empty())
		return;

	auto isolate = Isolate::GetCurrent();
	HandleScope handleScope(isolate);

	if (propName == "__proto__" /*let he v8 engine handle protoype chain calls*/
			|| propName == V8StringConstants::IS_PROTOTYPE_IMPLEMENTATION_OBJECT /*skip typescript implementationProperty mark*/)
	{
		return;
	}

	auto thiz = info.This();
	MetadataNode *node = GetNodeFromHandle(thiz);
	bool isMarkedAsSuper = IsMarkedAsSuper(thiz);

	if (node->CheckForCachedMetadataItem(propName, info, thiz, isMarkedAsSuper))
		return;

	uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);

	auto superValue = thiz->GetHiddenValue(V8StringConstants::GetTSuper());
	if (propName == V8StringConstants::TO_STRING)
	{
		auto classProxy = V8GetHiddenValue(thiz, CLASS_PROXY);
		if (!classProxy.IsEmpty())
			return;
	}
	bool shouldCallJSOverrideWithoutGoingToJava = !thiz.IsEmpty() && propName != V8StringConstants::TO_STRING && !isMarkedAsSuper;
	if (shouldCallJSOverrideWithoutGoingToJava)
	{
		auto prop = GetPropertyFromImplementationObject(thiz, property);
		if (!prop.IsEmpty())
		{
			DEBUG_WRITE("MedataNode::GetterCallback: found property on implementation object");
			return;
		}
	}

	DEBUG_WRITE("MetadataNode::GetterCallback: prop '%s' for node '%s' called, nodeType=%d, hash=%d", propName.c_str(), node->m_treeNode->name.c_str(), nodeType, thiz.IsEmpty() ? -42 : thiz->GetIdentityHash());

	auto candidates = GetMetadataCandidates(node, propName);
	bool foundCandidates = !candidates.empty();

	if (foundCandidates)
	{
		auto first = candidates.front();

		if (first.isTypeMember)
		{
			if (first.type == NodeType::Method)
			{
				auto method = node->CreateFunction(thiz, candidates);
				info.GetReturnValue().Set(method);

				node->CacheMetadataItem(propName, new Persistent<Value>(isolate, method), MetadataCacheItemType::NonField, isMarkedAsSuper);
			}
			else if ((first.type == NodeType::Field) || (first.type == NodeType::StaticField))
			{
				bool isStatic = first.type == NodeType::StaticField;
				auto value = s_getJavaField(thiz, Util::ToJniName(node->m_name), propName, first.sig, first.declaringType, isStatic);
				info.GetReturnValue().Set(value);

				node->CacheMetadataItem(propName, nullptr, MetadataCacheItemType::Field, isMarkedAsSuper, first);
			}
		}
		else
		{
			assert(first.treeNode != nullptr);

			auto child = MetadataNode::GetOrCreateInternal(first.treeNode);
			auto childObj = child->CreateProxy(isolate);
			info.GetReturnValue().Set(childObj);

			node->CacheMetadataItem(propName, new Persistent<Value>(isolate, childObj), MetadataCacheItemType::NonField, isMarkedAsSuper);
		}

		return;
	}

	string capitalizedPropName = propName;
	capitalizedPropName[0] = toupper(capitalizedPropName[0]);
	candidates = GetMetadataCandidates(node, "get" + capitalizedPropName);
	foundCandidates = !candidates.empty();
	if (!foundCandidates)
	{
		candidates = GetMetadataCandidates(node, "has" + capitalizedPropName);
		foundCandidates = !candidates.empty();
		if (!foundCandidates)
		{
			candidates = GetMetadataCandidates(node, "is" + capitalizedPropName);
			foundCandidates = !candidates.empty();
		}
	}

	if (!foundCandidates)
	{
		candidates = GetMetadataCandidates(node, "get" + propName);
		foundCandidates = !candidates.empty();
		if (!foundCandidates)
		{
			candidates = GetMetadataCandidates(node, "has" + propName);
			foundCandidates = !candidates.empty();
			if (!foundCandidates)
			{
				candidates = GetMetadataCandidates(node, "is" + propName);
				foundCandidates = !candidates.empty();
			}
		}
	}

	if (foundCandidates)
	{
		auto func = node->CreateFunction(thiz, candidates);

		auto arr(Array::New(isolate, 0));
		Handle<Value> arguments[1];
		arguments[0] = arr;

		auto result = func->Call(thiz, 0, arguments);
		info.GetReturnValue().Set(result);
	}
	else
	{
		node->CacheMetadataItem(propName, nullptr, MetadataCacheItemType::None, isMarkedAsSuper);
	}
}

bool MetadataNode::CheckForCachedMetadataItem(const string& key, const PropertyCallbackInfo<Value>& info, const Local<Object>& thiz, bool isSuper)
{
	auto& cache = isSuper ? m_superMethodCache : m_childCache;
	auto itStart = cache.begin();
	auto itEnd = cache.end();
	auto itFound = find_if(itStart, itEnd, [&key] (const MetadataCacheItem& item) { return item.name == key; } );

	bool foundItem = itFound != itEnd;

	if (foundItem)
	{
		const auto& cacheItem = *itFound;
		Handle<Value> value;
		if (cacheItem.type == MetadataCacheItemType::NonField)
		{
			const auto& pv = *cacheItem.pv;
			value = Handle<Value>::New(Isolate::GetCurrent(), pv);
		}
		else if (cacheItem.type == MetadataCacheItemType::Field)
		{
			bool isStatic = cacheItem.entry.type == NodeType::StaticField;
			value = s_getJavaField(thiz, m_name, key, cacheItem.entry.sig, cacheItem.entry.declaringType, isStatic);
		}
		else if (cacheItem.type != MetadataCacheItemType::None)
		{
			assert(false);
		}
		info.GetReturnValue().Set(value);
	}

	return foundItem;
}

void MetadataNode::CacheMetadataItem(const string& key, Persistent<Value> *po, MetadataCacheItemType itemType, bool isSuper)
{
	auto& cache = isSuper ? m_superMethodCache : m_childCache;
	MetadataCacheItem cacheItem(key, po, itemType);
	cache.push_back(cacheItem);
}

void MetadataNode::CacheMetadataItem(const string& key, Persistent<Value> *po, MetadataCacheItemType itemType, bool isSuper, const MetadataEntry& entry)
{
	auto& cache = isSuper ? m_superMethodCache : m_childCache;
	MetadataCacheItem cacheItem(key, po, itemType);
	cacheItem.entry = entry;
	cache.push_back(cacheItem);
}

void MetadataNode::InvokeClassCtor(const v8::FunctionCallbackInfo<v8::Value>& args, MetadataNode *node)
{
	bool instanceCreated = false;

	auto classProxy = args.Callee();

	auto implementationObject = GetImplementationObject(classProxy);

	string extendName;

	string extendsPropertyName = "t::ExtendsName";
	Handle<String> v8ExtendName = V8GetHiddenValue(classProxy, extendsPropertyName).As<String>();
	if (!v8ExtendName.IsEmpty())
	{
		extendName = ConvertToString(v8ExtendName);
	}


	string className;
	auto instance = CreateJSInstance(classProxy, node, implementationObject, className);
	bool shouldRegisterInstance = V8GetHiddenValue(classProxy, "t::TypescriptActivity::DonNotRegisterInstance").IsEmpty();

	if (shouldRegisterInstance)
	{
		uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);
		bool isStatic = s_metadataReader.IsNodeTypeStatic(nodeType);
		bool isInnerClass = ((node->m_name.find('$') != string::npos)) && !isStatic;

		ArgsWrapper argWrapper(args, ArgType::Class, isInnerClass);
		instanceCreated = s_registerInstance(instance, extendName, className, argWrapper, implementationObject, false);
	}

	if (instanceCreated)
	{
		args.GetReturnValue().Set(instance);
	}
}

Handle<Object> MetadataNode::CreateJSInstance(const Handle<Object>& classProxy, MetadataNode *node, const Handle<Object>& implementationObject, string& className)
{
	auto isolate = Isolate::GetCurrent();

	auto instance = node->CreateInstanceProxy(isolate);

	bool hasImplementationObject = !implementationObject.IsEmpty();


	if (hasImplementationObject)
	{
		auto superObj = Object::New(isolate);
		superObj->SetHiddenValue(V8StringConstants::GetTSuper(), Boolean::New(isolate, true));
		V8SetHiddenValue(superObj, METADATA_NODE_KEY_NAME, V8GetHiddenValue(classProxy, METADATA_NODE_KEY_NAME));
		V8SetHiddenValue(superObj, "t::superJsInstance", instance);
		bool success = superObj->SetPrototype(instance);
		ASSERT_MESSAGE(success, "CreateJSInstance : SetPrototype failed on super");
		instance->Set(V8StringConstants::GetSuper(), superObj);

		bool startsWithTesstTelerikName = Util::StartsWith(node->m_name, TNS_TESTS_PREFIX);
		if (startsWithTesstTelerikName)
		{
			className.append(TNS_PREFIX);
		}
		else
		{
			bool startsWithTelerikName = Util::StartsWith(node->m_name, TNS_PREFIX);
			if (!startsWithTelerikName)
			{
				className.append(TNS_PREFIX);
			}
		}

		className.append(node->m_name);
	}
	else
	{
		className = node->m_name;
	}

	bool success = instance->SetPrototype(classProxy);
	ASSERT_MESSAGE(success, "CreateJSInstance: SetPrototype failed");
	instance->Set(V8StringConstants::GetPrototype(), classProxy);

	SetDebugName(className, instance);

	return instance;
}


void MetadataNode::InvokeInterfaceCtor(const v8::FunctionCallbackInfo<v8::Value>& args, MetadataNode *node)
{
	//ASSERT_MESSAGE(args.Length() == 1, "Interfaces should be created with single parameter");
	//ASSERT_MESSAGE(args[0]->IsObject(), "Interfaces  should be created with single object parameter containing overridden methods");


	Handle<Object> implementationObject;
	Handle<String> v8ExtendName;
	string extendLocation;
	bool extendLocationFound = GetExtendLocation(extendLocation);
	if (args.Length() == 1)
	{
		//ASSERT_MESSAGE(args[0]->IsObject(), "extends() should be called with object parameter containing overridden methods when no name specified");

		//implementationObject = args[0]->ToObject();
		//ASSERT_MESSAGE(args[0]->IsString(), "Invalid extends() call. No name for extend specified");
		//ASSERT_FAIL("Invalid extend() call. No name for extend specified");

		if (!extendLocationFound)
		{
			ASSERT_FAIL("Invalid extend() call. No name specified for extend. Location: %s", extendLocation.c_str());
		}

		ASSERT_MESSAGE(args[0]->IsObject(), "Invalid extend() call. No implementation object specified. Location: %s", extendLocation.c_str());
		implementationObject = args[0]->ToObject();
	}
	else if (args.Length() == 2)
	{
		ASSERT_MESSAGE(args[0]->IsString(), "Invalid extend() call. No name for extend specified. Location: %s", extendLocation.c_str());
		ASSERT_MESSAGE(args[1]->IsObject(), "Invalid extend() call. Named extend should be called with second object parameter containing overridden methods. Location: %s", extendLocation.c_str());

		DEBUG_WRITE("InvokeInterfaceCtor: getting extend name");
		v8ExtendName = args[0]->ToString();
		implementationObject = args[1]->ToObject();
	}
	else
	{
		ASSERT_FAIL("Invalid extend() call. Location: %s", extendLocation.c_str());
	}

	string className = node->m_implType;
	string extendName = ConvertToString(v8ExtendName);


	string extendNameAndLocation = extendLocation + extendName;

	//string fullInterfaceName = className + '-' + extendNameAndLocation;
//	if (ExistsExtendName(fullInterfaceName))
//	{
//		ASSERT_FAIL("Extend name %s already used", fullInterfaceName.c_str());
//		APP_FAIL("Failed: Extend name already used");
//		return;
//	}

	auto isolate = Isolate::GetCurrent();
	auto interfaceProxy = args.Callee();
	implementationObject->SetHiddenValue(V8StringConstants::GetClassImplementationObject(), External::New(isolate, node));
	bool success = implementationObject->SetPrototype(interfaceProxy);
	ASSERT_MESSAGE(success, "InvokeInterfaceCtor: SetPrototype failed on implementation object");
	implementationObject->Set(V8StringConstants::GetPrototype(), interfaceProxy);

	DEBUG_WRITE("InvokeInterfaceCtor: interfaceProxy:%d", interfaceProxy->GetIdentityHash());
	DEBUG_WRITE("InvokeInterfaceCtor: interface implementationObject:%d", implementationObject->GetIdentityHash());

	auto instance = node->CreateInstanceProxy(isolate);
	success = instance->SetPrototype(implementationObject);
	ASSERT_MESSAGE(success, "InvokeInterfaceCtor: SetPrototype failed");
	instance->Set(V8StringConstants::GetPrototype(), implementationObject);

	DEBUG_WRITE("InvokeInterfaceCtor: interface instanceproxy:%d", instance->GetIdentityHash());

	ArgsWrapper argWrapper(args, ArgType::Interface, false);

	bool instanceCreated = s_registerInstance(instance, extendNameAndLocation, className, argWrapper, implementationObject, true);

	if (instanceCreated)
	{
		//s_usedExtendNames[fullInterfaceName] = 1;
		args.GetReturnValue().Set(instance);
	}
}

string MetadataNode::ResolveMethodOverload(const v8::FunctionCallbackInfo<Value>& args, const string& className, const string& methodName)
{
	string cannonicalClassName = className;
	std::replace(cannonicalClassName.begin(), cannonicalClassName.end(), '/', '.');

	string methodSignature = MethodCache::ResolveMethodSignature(cannonicalClassName, methodName, args);

	DEBUG_WRITE("ResolveMethodOverload: result: %s", methodSignature.c_str());

	return methodSignature;
}

void MetadataNode::ExtendCallMethodHandler(const v8::FunctionCallbackInfo<v8::Value>& args, MetadataNode *node)
{
	Handle<Object> implementationObject;
	Handle<String> extendName;
	string extendLocation;
	bool extendLocationFound = GetExtendLocation(extendLocation);

	if (args.Length() == 1)
	{
		if (!extendLocationFound)
		{
			stringstream ss;
			ss << "Invalid extend() call. No name specified for extend at location: " << extendLocation.c_str();
			string exceptionMessage = ss.str();

			Isolate *isolate(Isolate::GetCurrent());
			isolate->ThrowException(v8::Exception::Error((ConvertToV8String(exceptionMessage))));
			return;
		}


		if (!args[0]->IsObject())
		{
			stringstream ss;
			ss << "Invalid extend() call. No implementation object specified at location: " << extendLocation.c_str();
			string exceptionMessage = ss.str();

			Isolate *isolate(Isolate::GetCurrent());
			isolate->ThrowException(v8::Exception::Error((ConvertToV8String(exceptionMessage))));
			return;
		}

		implementationObject = args[0]->ToObject();
	}
	else if (args.Length() == 2)
	{
		if (!args[0]->IsString())
		{
			stringstream ss;
			ss << "Invalid extend() call. No name for extend specified at location: " << extendLocation.c_str();
			string exceptionMessage = ss.str();

			Isolate *isolate(Isolate::GetCurrent());
			isolate->ThrowException(v8::Exception::Error((ConvertToV8String(exceptionMessage))));
			return;
		}

		if (!args[1]->IsObject())
		{
			stringstream ss;
			ss << "Invalid extend() call. Named extend should be called with second object parameter containing overridden methods at location: " << extendLocation.c_str();
			string exceptionMessage = ss.str();

			Isolate *isolate(Isolate::GetCurrent());
			isolate->ThrowException(v8::Exception::Error((ConvertToV8String(exceptionMessage))));
			return;
		}

		DEBUG_WRITE("ExtendsCallMethodHandler: getting extend name");
		extendName = args[0]->ToString();
		implementationObject = args[1]->ToObject();
	}
	else
	{
		stringstream ss;
		ss << "Invalid extend() call at location: " << extendLocation.c_str();
		string exceptionMessage = ss.str();

		Isolate *isolate(Isolate::GetCurrent());
		isolate->ThrowException(v8::Exception::Error((ConvertToV8String(exceptionMessage))));
		return;
	}

	DEBUG_WRITE("ExtendsCallMethodHandler: called with %s", ConvertToString(extendName).c_str());
	string currExtClass = node->m_name;
	auto classProxy = GetExistingClassProxy(currExtClass);

	if (classProxy.IsEmpty())
	{
		stringstream ss;
		ss << "ClassProxy " << currExtClass.c_str() << " not found when extending.";
		string exceptionMessage = ss.str();

		Isolate *isolate(Isolate::GetCurrent());
		isolate->ThrowException(v8::Exception::Error((ConvertToV8String(exceptionMessage))));
		return;
	}

	auto isolate = Isolate::GetCurrent();

	string extendNameAndLocation = extendLocation + ConvertToString(extendName);
	string fullClassName = node->m_name + '-' + extendNameAndLocation; //ConvertToString(extendName);
	DEBUG_WRITE("ExtendsCallMethodHandler: extend full name %s", fullClassName.c_str());


	if (ExistsExtendName(fullClassName))
	{
		stringstream ss;
		ss << "Extend name " << fullClassName.c_str() << "already used.";
		string exceptionMessage = ss.str();

		Isolate *isolate(Isolate::GetCurrent());
		isolate->ThrowException(v8::Exception::Error((ConvertToV8String(exceptionMessage))));
		return;
	}

	auto implementationObjectPropertyName = V8StringConstants::GetClassImplementationObject();
	//reuse validation - checks that implementationObject is not reused for different classes
	auto implementationObjectProperty = implementationObject->GetHiddenValue(implementationObjectPropertyName).As<String>();
	if (implementationObjectProperty.IsEmpty())
	{
		//mark the implementationObject as such and set a pointer to it's class node inside it for reuse validation later
		implementationObject->SetHiddenValue(implementationObjectPropertyName, String::NewFromUtf8(isolate, fullClassName.c_str()));
	}
	else
	{
		string usedClassName = ConvertToString(implementationObjectProperty);
		stringstream s;
		s << "This object is used to extend another class '" << usedClassName << "'";
		ExceptionUtil::GetInstance()->HandleInvalidState(s.str(), false);
		return;
	}

	//TODO: We may need to expose super object for Typescript world
	auto superObj = Object::New(isolate);
	superObj->SetHiddenValue(V8StringConstants::GetTSuper(), Boolean::New(isolate, true));
	V8SetHiddenValue(superObj, METADATA_NODE_KEY_NAME, V8GetHiddenValue(classProxy, METADATA_NODE_KEY_NAME));
	bool success = superObj->SetPrototype(classProxy);
	ASSERT_MESSAGE(success, "ExtendsCallMethodHandler: SetPrototype failed on super");
	implementationObject->Set(V8StringConstants::GetSuper(), superObj);
	success = implementationObject->SetPrototype(classProxy);
	ASSERT_MESSAGE(success, "ExtendsCallMethodHandler: SetPrototype failed on implementation object");
	implementationObject->Set(V8StringConstants::GetPrototype(), classProxy);

	Local<Function> extendedClass = node->CreateExtendedClassProxy(isolate, implementationObject, ConvertToV8String(extendNameAndLocation));

	auto classProxyPersistent = new Persistent<Object>(isolate, extendedClass);
	DEBUG_WRITE("CreateExtendedClassProxy: ExtendedClassProxy for %s created id:%d",  node->m_name.c_str(), extendedClass->GetIdentityHash());
	s_classProxies[fullClassName] = classProxyPersistent;

	V8SetHiddenValue(extendedClass, "t::ext", implementationObject);
	s_usedExtendNames[fullClassName] = 1;
	DEBUG_WRITE("ExtendsCallMethodHandler: created ExtendedClassProxy on %s, Id: %d", currExtClass.c_str(), extendedClass->GetIdentityHash());
	args.GetReturnValue().Set(extendedClass);
}

Handle<Function> MetadataNode::CreateExtendedClassProxy(Isolate *isolate, const Handle<Object>& implementationObject, const Handle<String>& name)
{
	EscapableHandleScope handleScope(isolate);

	auto external = External::New(isolate, this);

	auto funcTemplate = FunctionTemplate::New(isolate, MetadataNode::ConstructorCallback);
	auto extended = funcTemplate->GetFunction();
	bool success = extended->SetPrototype(implementationObject);
	ASSERT_MESSAGE(success, "CreateExtendedClassProxy: setPrototype failed");
	V8SetHiddenValue(extended, METADATA_NODE_KEY_NAME, external);

	success = extended->SetHiddenValue(V8StringConstants::GetTSuper(), Boolean::New(isolate, true));
	ASSERT_MESSAGE(success, "CreateExtendedClassProxy: mark classProxyFunction as super object failed");

	success = extended->SetHiddenValue(String::NewFromUtf8(isolate, "t::ExtendsName"), name);
	ASSERT_MESSAGE(success, "CreateExtendedClassProxy: setting extends name failed");

	SetDebugName(m_name, extended);

	return handleScope.Escape(extended);
}

bool MetadataNode::GetExtendLocation(string& extendLocation)
{
	stringstream extendLocationStream;
	Local<StackTrace> stackTrace = StackTrace::CurrentStackTrace(Isolate::GetCurrent(), 1, StackTrace::kOverview);
	if (!stackTrace.IsEmpty())
	{
		Handle<StackFrame> frame = stackTrace->GetFrame(0);
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
			std::replace(srcFileName.begin(), srcFileName.end(), '/', '-');
			std::replace(srcFileName.begin(), srcFileName.end(), '.', '-');
			int lineNumber = frame->GetLineNumber();
			if (lineNumber < 0)
			{
				extendLocationStream << srcFileName.c_str() << " unkown line number";
				extendLocation = extendLocationStream.str();
				return false;
			}

			if (lineNumber > Constants::MODULE_LINES_OFFSET)
			{
				lineNumber -= Constants::MODULE_LINES_OFFSET;
			}

			int column = frame->GetColumn();
			if (column < 0)
			{
				extendLocationStream << srcFileName.c_str() << " line:" << lineNumber << " unkown column number";
				extendLocation = extendLocationStream.str();
				return false;
			}


			extendLocationStream << "f" << srcFileName.c_str() << "-l" << lineNumber << "-c" << column << "--";
			//DEBUG_WRITE("EXTEND_LOCATION %s", extendLocationStream.str().c_str());
		}
	}

	extendLocation = extendLocationStream.str();
	return true;
}


MetadataNode* MetadataNode::GetNodeFromHandle(const Handle<Object>& value)
{
	DEBUG_WRITE("GetNodeFromHandle called on :%d", value->GetIdentityHash());
	auto hiddenValue = V8GetHiddenValue(value, METADATA_NODE_KEY_NAME);
	if (hiddenValue.IsEmpty())
	{
		//Typescript function is special. We set the function __proto__ object to the class proxy function inside the tns extends function. ie. MyActivity.__proto__ = android.app.Activity
		auto functionPrototype = value->GetPrototype().As<Function>();
		if (value->IsFunction() && !functionPrototype.IsEmpty() && functionPrototype->IsFunction())
		{
			DEBUG_WRITE("GetNodeFromHandle need to check function prototype :%d", functionPrototype->GetIdentityHash());
			hiddenValue = V8GetHiddenValue(functionPrototype, METADATA_NODE_KEY_NAME);
		}
		else //value is an object check for instance MetadataNode
		{
			//Typescript object layout has an object instance as child of the actual registered instance. checking for that
			auto prototypeObject = value->GetPrototype().As<Object>();
			if (!prototypeObject.IsEmpty())
			{
				DEBUG_WRITE("GetNodeFromHandle need to check prototype :%d", prototypeObject->GetIdentityHash());

				hiddenValue = V8GetHiddenValue(prototypeObject, METADATA_NODE_KEY_NAME);
			}
		}
	}

	ASSERT_MESSAGE(!hiddenValue.IsEmpty(), "MetadataNode must be set");

	MetadataNode *node = static_cast<MetadataNode*>(hiddenValue.As<External>()->Value());
	return node;
}

//return the property if overriden by the implementationObject which should be a prototype of the object
Handle<Value> MetadataNode::GetPropertyFromImplementationObject(const Handle<Object> object, const Handle<String>& propertyName)
{
	if (!object.IsEmpty())
	{
		DEBUG_WRITE("MetadataNode::GetPropertyFromImplementationObject: called for object: %d", object->GetIdentityHash());

		auto implementationObject = GetImplementationObject(object);

		if (!implementationObject.IsEmpty())
		{
			if (implementationObject->HasOwnProperty(propertyName))
			{
				DEBUG_WRITE("MetadataNode::GetPropertyFromImplementationObject: found own property on object: %d", object->GetIdentityHash());
				return implementationObject->Get(propertyName);
			}
		}
	}

	return Handle<Value>();
}

vector<MetadataEntry> MetadataNode::GetMetadataCandidates(MetadataNode *node, const string& propName)
{
	vector<MetadataEntry> candidates;

	uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);

	if (node->m_isArray)
	{
		candidates = GetMetadataCandidatesForArray(node, propName);
	}
	else if (s_metadataReader.IsNodeTypePackage(nodeType))
	{
		candidates = GetMetadataCandidatesForPackage(node, propName);
	}
	else if (s_metadataReader.IsNodeTypeInterface(nodeType) || s_metadataReader.IsNodeTypeClass(nodeType))
	{
		candidates = GetMetadataCandidatesForType(node, propName);
	}
	else
	{
		assert(false);
	}

	return candidates;
}



vector<MetadataEntry> MetadataNode::GetMetadataCandidatesForPackage(MetadataNode *node, const string& propName)
{
	vector<MetadataEntry> candidates;

	assert(node->m_treeNode->children != nullptr);

	const auto& children = *node->m_treeNode->children;

	for (auto treeNodeChild: children)
	{
		if (propName == treeNodeChild->name)
		{
			MetadataEntry entry;
			entry.treeNode = treeNodeChild;

			uint8_t childNodeType = s_metadataReader.GetNodeType(treeNodeChild);
			if (s_metadataReader.IsNodeTypeInterface(childNodeType))
			{
				bool isPrefix;
				string declaringType = s_metadataReader.ReadInterfaceImplementationTypeName(treeNodeChild, isPrefix);
				entry.declaringType = isPrefix
										? (declaringType + s_metadataReader.ReadTypeName(entry.treeNode))
										: declaringType;
			}

			candidates.push_back(entry);
		}
	}

	return candidates;
}



vector<MetadataEntry> MetadataNode::GetMetadataCandidatesForType(MetadataNode *node, const string& propName)
{
	bool hasCustomMetadata = node->m_treeNode->metadata !=  nullptr;

	return hasCustomMetadata
			? GetMetadataCandidatesForTypeWithCustomMetadata(node, propName)
			: GetMetadataCandidatesForTypeWithoutCustomMetadata(node, propName);
}

vector<MetadataEntry> MetadataNode::GetMetadataCandidatesForTypeWithCustomMetadata(MetadataNode *node, const string& propName)
{
	assert(node->m_treeNode->metadata != nullptr);

	vector<MetadataEntry> candidates;

	string line;
	const string& metadata = *node->m_treeNode->metadata;
	stringstream s(metadata);
	bool skipFirstLine = true;

	string kind;
	string name;
	string signature;
	int paramCount;

	while (getline(s, line))
	{
		if (skipFirstLine)
		{
			skipFirstLine = false;
			continue;
		}

		stringstream tmp(line);
		tmp >> kind >> name >> signature >> paramCount;

		if (name == propName)
		{
			char chKind = kind[0];

			MetadataEntry entry;

			switch (chKind)
			{
				case 'F':
				case 'M':
					entry.name = name;
					entry.isTypeMember = true;
					entry.type = (chKind == 'F') ? NodeType::Field : NodeType::Method;
					entry.sig = signature;
					entry.paramCount = paramCount;
					entry.treeNode = node->m_treeNode;
					break;

				default:
					assert(false);
					break;
			}

			candidates.push_back(entry);
		}
	}


	return candidates;
}

vector<MetadataEntry> MetadataNode::GetMetadataCandidatesForTypeWithoutCustomMetadata(MetadataNode *node, const string& propName)
{
	vector<MetadataEntry> candidates;

	uint8_t *curPtr = s_metadataReader.GetValueData() + node->m_treeNode->offsetValue + 1;

	uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);

	curPtr += sizeof(uint16_t /* baseClassId */);

	if (s_metadataReader.IsNodeTypeInterface(nodeType))
	{
		curPtr += sizeof(uint8_t) + sizeof(uint32_t);
	}

	//get candidates from instance methods metadata
	uint16_t instanceMethodCout = *reinterpret_cast<uint16_t*>(curPtr);
	curPtr += sizeof(uint16_t);
	for (int i = 0; i < instanceMethodCout; i++)
	{
		MetadataEntry entry = s_metadataReader.ReadInstanceMethodEntry(&curPtr);

		if (entry.name == propName)
		{
			entry.treeNode = node->m_treeNode;
			candidates.push_back(entry);
		}
	}

	//get candidates from static methods metadata
	uint16_t staticMethodCout = *reinterpret_cast<uint16_t*>(curPtr);
	curPtr += sizeof(uint16_t);
	for (int i = 0; i < staticMethodCout; i++)
	{
		MetadataEntry entry = s_metadataReader.ReadStaticMethodEntry(&curPtr);

		if (entry.name == propName)
		{
			entry.treeNode = node->m_treeNode;
			candidates.push_back(entry);
		}
	}

	if (propName == V8StringConstants::EXTEND)
	{
		MetadataEntry extendEntry;
		extendEntry.name = "extend";
		extendEntry.type = NodeType::Method;
		extendEntry.treeNode = node->m_treeNode;
		extendEntry.isTypeMember = true;
		candidates.push_back(extendEntry);
	}

	//get candidates from instance fields metadata
	uint16_t instanceFieldCout = *reinterpret_cast<uint16_t*>(curPtr);
	curPtr += sizeof(uint16_t);
	for (int i = 0; i < instanceFieldCout; i++)
	{
		MetadataEntry entry = s_metadataReader.ReadInstanceFieldEntry(&curPtr);

		if (entry.name == propName)
		{
			entry.treeNode = node->m_treeNode;
			candidates.push_back(entry);
		}
	}

	//get candidates from static fields metadata
	uint16_t staticFieldCout = *reinterpret_cast<uint16_t*>(curPtr);
	curPtr += sizeof(uint16_t);
	for (int i = 0; i < staticFieldCout; i++)
	{
		MetadataEntry entry = s_metadataReader.ReadStaticFieldEntry(&curPtr);

		if (entry.name == propName)
		{
			entry.treeNode = node->m_treeNode;
			candidates.push_back(entry);
		}
	}

	if (propName == "class")
	{
		MetadataEntry classEntry;
		classEntry.name = "class";
		classEntry.sig = "java/lang/Class";
		classEntry.declaringType = node->m_name;
		classEntry.type = NodeType::StaticField;
		classEntry.treeNode = node->m_treeNode;
		classEntry.isTypeMember = true;
		classEntry.isStatic = true;
		candidates.push_back(classEntry);
	}

	if (node->m_treeNode->children != nullptr)
	{
		const auto& children = *node->m_treeNode->children;

		for (auto curChild: children)
		{
			if (curChild->name == propName)
			{
				MetadataEntry entry;
				entry.name = propName;
				entry.treeNode = curChild;

				uint8_t childNodeType = s_metadataReader.GetNodeType(curChild);
				if (s_metadataReader.IsNodeTypeInterface(childNodeType))
				{
					bool isPrefix;
					string declaringType = s_metadataReader.ReadInterfaceImplementationTypeName(curChild, isPrefix);
					entry.declaringType = isPrefix
											? (declaringType + s_metadataReader.ReadTypeName(entry.treeNode))
											: declaringType;
				}

				candidates.push_back(entry);
				break;
			}
		}
	}

	return candidates;
}

vector<MetadataEntry> MetadataNode::GetMetadataCandidatesForArray(MetadataNode *node, const string& propName)
{
	vector<MetadataEntry> candidates;

	if (propName == "length")
	{
		MetadataEntry classEntry;
		classEntry.name = "class";
		classEntry.sig = "I";
		classEntry.declaringType = node->m_name;
		classEntry.type = NodeType::Field;
		classEntry.treeNode = node->m_treeNode;
		classEntry.isTypeMember = true;
		candidates.push_back(classEntry);
	}

	return candidates;
}

Handle<Function> MetadataNode::CreateFunction(const Handle<Object>& thiz, const vector<MetadataEntry>& candidates)
{
	auto isolate = Isolate::GetCurrent();

	EscapableHandleScope handleScope(isolate);

	auto callbackState = new MethodCallbackState;
	callbackState->candidates = candidates;
	callbackState->node = this;
	callbackState->isSuper = IsMarkedAsSuper(thiz);
	auto ext = External::New(isolate, callbackState);

	auto methodFuncTemplate = FunctionTemplate::New(isolate, MetadataNode::MethodCallback, ext);
	//methodFuncTemplate->SetClassName(name);

	auto methodFunc = methodFuncTemplate->GetFunction();
	//methodFunc->SetName(name);

	SetDebugName(m_name, methodFunc);

	return handleScope.Escape(methodFunc);
}


void MetadataNode::SetterCallback(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info)
{
	string propName = ConvertToString(property);

	if (propName.empty())
		return;

	auto thiz = info.This();
	MetadataNode *node = GetNodeFromHandle(thiz);

	auto& cache = node->m_childCache;
	auto itStart = cache.begin();
	auto itEnd = cache.end();
	auto itFound = find_if(itStart, itEnd, [&propName] (const MetadataCacheItem& item) { return item.name == propName; } );
	if (itFound != itEnd)
	{
		const auto& cacheItem = *itFound;

		if (cacheItem.type == MetadataCacheItemType::Field)
		{
			bool isStatic = cacheItem.entry.type == NodeType::StaticField;
			s_setJavaField(thiz, value, node->m_name, cacheItem.entry.name, cacheItem.entry.sig, cacheItem.entry.declaringType, cacheItem.entry.isStatic);
			info.GetReturnValue().Set(value);
		}
		else if (cacheItem.type != MetadataCacheItemType::None)
		{
			assert(false);
		}
		if (!cacheItem.entry.isFinal)
		{
			return;
		}
	}

	DEBUG_WRITE("MetadataNode::SetterCallback propName='%s', node='%s', hash=%d", propName.c_str(), node->m_name.c_str(), thiz->GetIdentityHash());

	uint8_t nodeType = s_metadataReader.GetNodeType(node->m_treeNode);

	if (!s_metadataReader.IsNodeTypeClass(nodeType) && !s_metadataReader.IsNodeTypeInterface(nodeType))
	{
		return;
	}

	vector<MetadataEntry> candidates = GetMetadataCandidates(node, propName);
	bool foundCandidates = !candidates.empty();

	if (foundCandidates)
	{
		auto first = candidates.front();

		if ((first.type == NodeType::Field) || (first.type == NodeType::StaticField))
		{
			if(first.isFinal)
			{
				Isolate *isolate(Isolate::GetCurrent());
				Handle<String> exceptionMessage = ConvertToV8String("You are trying to SET a final field! Final fields can only be read.");
				Local<Value> IllegalAccessException(exceptionMessage);

				isolate->ThrowException(IllegalAccessException);
			}
			s_setJavaField(thiz, value, node->m_name, first.name, first.sig, first.declaringType, first.isStatic);

			MetadataCacheItem cacheItem(propName, nullptr, MetadataCacheItemType::Field);
			cacheItem.entry = first;
			cache.push_back(cacheItem);
		}
		else
		{
			assert(false);
		}

		return;
	}

	string capilalizeFieldName = propName;
	capilalizeFieldName[0] = toupper(capilalizeFieldName[0]);
	candidates = GetMetadataCandidates(node, "set" + capilalizeFieldName);
	foundCandidates = !candidates.empty();

	if (!foundCandidates)
	{
		candidates = GetMetadataCandidates(node, "set" + propName);
		foundCandidates = !candidates.empty();
	}

	if (foundCandidates)
	{
		auto first = candidates.front();

		if ((first.type == NodeType::Method) && (first.paramCount == 1))
		{
			auto func = node->CreateFunction(thiz, candidates);

			Handle<Value> arguments[1];
			arguments[0] = value;

			auto result = func->Call(thiz, 1, arguments);
			info.GetReturnValue().Set(result);
		}
	}
	else
	{
		MetadataCacheItem cacheItem(propName, nullptr, MetadataCacheItemType::None);
		cache.push_back(cacheItem);
	}
}


void MetadataNode::BuildMetadata(uint32_t nodesLength, uint8_t *nodeData, uint8_t *nameData, uint8_t *valueData)
{
	s_metadataReader = MetadataReader(nodesLength, nodeData, nameData, valueData, s_getTypeMetadata);
}

void MetadataNode::CreateTopLevelNamespaces(const Handle<Object>& global)
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

			auto packageObj = node->CreateProxy(isolate);
			global->Set(ConvertToV8String(node->m_treeNode->name), packageObj);
		}
	}
}



std::map<std::string, Persistent<Object>*> MetadataNode::s_classProxies;
std::map<std::string, int> MetadataNode::s_usedExtendNames;
std::map<std::string, MetadataNode*> MetadataNode::s_name2NodeCache;
std::map<std::string, MetadataTreeNode*> MetadataNode::s_name2TreeNodeCache;
std::map<MetadataTreeNode*, MetadataNode*> MetadataNode::s_treeNode2NodeCache;

GetJavaFieldCallback MetadataNode::s_getJavaField = nullptr;
SetJavaFieldCallback MetadataNode::s_setJavaField = nullptr;
GetArrayElementCallback MetadataNode::s_getArrayElement = nullptr;
SetArrayElementCallback MetadataNode::s_setArrayElement = nullptr;
MakeClassInstanceOfTypeStrongCallback MetadataNode::s_makeClassInstanceOfTypeStrong = nullptr;
CallJavaMethodCallback MetadataNode::s_callJavaMethod = nullptr;
RegisterInstanceCallback MetadataNode::s_registerInstance = nullptr;
GetTypeMetadataCallback MetadataNode::s_getTypeMetadata = nullptr;
string MetadataNode::METADATA_NODE_KEY_NAME = "t::MetadataNode";
string MetadataNode::CLASS_PROXY = "t::ClassProxy";
string MetadataNode::TNS_PREFIX = "com/tns/gen/";
string MetadataNode::TNS_TESTS_PREFIX = "com/tns/tests/";
MetadataReader MetadataNode::s_metadataReader;
