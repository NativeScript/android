//
//Object layout description
//
//Object layout w/ Extends
//	ProxyObject->Function->ImplementationObject->ExtendedFunction->InstanceProxy
//	Registered(InstanceProxy)
//
//Object layout w/o Extends
//	ProxyObject->Function->InstanceProxy
//
//Object layout w/ Interfaces
//	ProxyObject->Function->ImplementationObject->InstanceProxy
//
//Object layout for objects from method result, fields, array indexers
//	ProxyObject->Function->InstanceProxy
//
//Object layout w/ TypeScript
//	....->InstanceProxy->EmptyInstance


#include "v8.h"
#include "MetadataEntry.h"
#include "MetadataTreeNode.h"
#include "MetadataReader.h"
#include "FieldCallbackData.h"
#include "ArgsWrapper.h"
#include "ObjectManager.h"
#include <string>
#include <vector>
#include <map>

namespace tns
{
	typedef void (*SetJavaFieldCallback)(const v8::Handle<v8::Object>& target, const v8::Handle<v8::Value>& value, FieldCallbackData *fieldData);

	typedef v8::Handle<v8::Value> (*GetJavaFieldCallback)(const v8::Handle<v8::Object>& caller, FieldCallbackData *fieldData);

	typedef v8::Handle<v8::Value> (*GetArrayElementCallback)(const v8::Handle<v8::Object>& array, uint32_t index, const std::string& arraySignature);

	typedef void (*SetArrayElementCallback)(const v8::Handle<v8::Object>& array, uint32_t index, const std::string& arraySignature, v8::Handle<v8::Value>& value);

	typedef void (*CallJavaMethodCallback)(const v8::Handle<v8::Object>& caller, const std::string& className, const std::string& methodName, MetadataEntry *entry, bool isStatic, bool isSuper, const v8::FunctionCallbackInfo<v8::Value>& args);

	typedef bool (*RegisterInstanceCallback)(const v8::Handle<v8::Object>& jsObject, const std::string& fullClassName, const ArgsWrapper& argWrapper, const v8::Handle<v8::Object>& implementationObject, bool isInterface);

	typedef int (*GetArrayLengthCallback)(const v8::Handle<v8::Object>& classObj);

	typedef jclass (*ResolveClassCallback)(const std::string& fullClassname, const v8::Handle<v8::Object>& implementationObject);

	typedef v8::Handle<v8::Object> (*FindClassCallback)(const std::string& className);

	class MetadataNode
	{
	public:
		static void SubscribeCallbacks(ObjectManager *objectManager,
										GetJavaFieldCallback getJavaFieldCallback,
										SetJavaFieldCallback setJavaFieldCallback,
										GetArrayElementCallback getArrayElementCallback,
										SetArrayElementCallback setArrayElementCallback,
										CallJavaMethodCallback callJavaMethodCallback,
										RegisterInstanceCallback registerInstanceCallback,
										GetTypeMetadataCallback getTypeMetadataCallback,
										FindClassCallback findClassCallback,
										GetArrayLengthCallback getArrayLengthCallback,
										ResolveClassCallback resolveClassCallback);


		static void BuildMetadata(uint8_t *nodes, int nodesLength, uint8_t *names, uint8_t *values);

		std::string GetName();

		v8::Handle<v8::Object> CreateWrapper(v8::Isolate *isolate);

		v8::Handle<v8::Object> CreateJSWrapper(v8::Isolate *isolate);

		v8::Handle<v8::Object> CreateArrayWrapper(v8::Isolate *isolate);

		static MetadataNode* GetNodeFromHandle(const v8::Handle<v8::Object>& value);

		static v8::Handle<v8::Object> CreateExtendedJSWrapper(v8::Isolate *isolate, const std::string& proxyClassName);

		static v8::Handle<v8::Object> GetImplementationObject(const v8::Handle<v8::Object>& object);

		static void CreateTopLevelNamespaces(const v8::Handle<v8::Object>& global);

		static std::string TNS_PREFIX;

		static MetadataNode* GetOrCreate(const std::string& className);

		static void BuildMetadata(uint32_t nodesLength, uint8_t *nodeData, uint32_t nameLength, uint8_t *nameData, uint32_t valueLength, uint8_t *valueData);

		static void InjectPrototype(v8::Handle<v8::Object>& target, v8::Handle<v8::Object>& implementationObject);


	private:
		struct MethodCallbackData
		{
			MethodCallbackData()
				: node(nullptr), isSuper(false)
			{
			}

			MethodCallbackData(MetadataNode *_node)
				: node(_node), isSuper(false)
			{
			}

			std::vector<MetadataEntry> candidates;
			MetadataNode *node;
			bool isSuper;
		};

		struct ExtendedClassData
		{
			ExtendedClassData(MetadataNode *_node, const std::string& _extendedName, const v8::Handle<v8::Object>& _implementationObject, std::string _fullClassName)
				: node(_node), extendedName(_extendedName), fullClassName(_fullClassName)
			{
				implementationObject = new v8::Persistent<v8::Object>(v8::Isolate::GetCurrent(), _implementationObject);
			}

			MetadataNode *node;
			std::string extendedName;
			v8::Persistent<v8::Object> *implementationObject;

			std::string fullClassName;
		};

		struct ExtendedClassCacheData
		{
			ExtendedClassCacheData()
				: extendedCtorFunction(nullptr), node(nullptr)
			{
			}
			ExtendedClassCacheData(const v8::Handle<v8::Function>& extCtorFunc, const std::string& _extendedName, MetadataNode *_node)
				: extendedName(_extendedName), node(_node)
			{
				extendedCtorFunction = new v8::Persistent<v8::Function>(v8::Isolate::GetCurrent(), extCtorFunc);
			}
			v8::Persistent<v8::Function> *extendedCtorFunction;
			std::string extendedName;
			MetadataNode *node;
		};

		struct TypeMetadata
		{
			TypeMetadata(const std::string& _name)
				: name(_name)
			{
			}

			std::string name;
		};

		struct InnerClassData
		{
			InnerClassData(v8::Persistent<v8::Object> *_outerThis, MetadataNode *_node)
				: outerThis(_outerThis), node(_node)
			{
			}

			v8::Persistent<v8::Object> *outerThis;
			MetadataNode *node;
		};

		enum class MetadataCacheItemType
		{
			None,
			NonField,
			Field
		};

		static MetadataNode* GetOrCreateInternal(MetadataTreeNode *treeNode);

		static MetadataNode* GetOrCreateInternal(const std::string& className);

		static MetadataTreeNode* GetOrCreateTreeNodeByName(const std::string& className);

		static MetadataTreeNode* GetOrCreateTreeNodeByNameHelper(const std::string& className);

		static MetadataEntry GetChildMetadataForPackage(MetadataNode *node, const std::string& propName);

		static MetadataNode* GetInstanceMetadata(v8::Isolate *isolate, const v8::Handle<v8::Object>& value);

		static void SetInstanceMetadata(v8::Isolate *isolate, v8::Handle<v8::Object> value, MetadataNode *node);

		static TypeMetadata* GetTypeMetadata(v8::Isolate *isolate, const v8::Handle<v8::Function>& value);

		static void SetTypeMetadata(v8::Isolate *isolate, v8::Handle<v8::Function> value, TypeMetadata *data);

		static MetadataNode* GetPackageMetadata(v8::Isolate *isolate, const v8::Handle<v8::Object>& value);

		static void SetPackageMetadata(v8::Isolate *isolate, v8::Handle<v8::Object> value, MetadataNode *node);

		struct MetadataCacheItem
		{
				MetadataCacheItem(const std::string& methodName, v8::Persistent<v8::Value> *po, MetadataCacheItemType itemType)
				: name(methodName), pv(po), type(itemType), entry(MetadataEntry())
			{
			}
			std::string name;
			MetadataCacheItemType type;
			MetadataEntry entry;
			v8::Persistent<v8::Value> *pv;
		};

		MetadataNode(MetadataTreeNode *treeNode);

		MetadataTreeNode *m_treeNode;
		std::string m_name;
		std::string m_implType;
		bool m_isArray;
		std::vector<MetadataCacheItem> m_childCache;
		std::vector<MetadataCacheItem> m_superMethodCache;


		v8::Handle<v8::Object> CreatePackageProxy(v8::Isolate *isolate);

		v8::Handle<v8::Function> GetConstructorFunction(v8::Isolate *isolate);
		v8::Handle<v8::FunctionTemplate> GetConstructorFunctionTemplate(v8::Isolate *isolate, MetadataTreeNode *treeNode);

		v8::Handle<v8::Function> SetMembers(v8::Isolate *isolate, v8::Handle<v8::FunctionTemplate>& ctorFuncTemplate, v8::Handle<v8::ObjectTemplate>& prototypeTemplate, MetadataTreeNode *treeNode);
		v8::Handle<v8::Function> SetMembersFromStaticMetadata(v8::Isolate *isolate, v8::Handle<v8::FunctionTemplate>& ctorFuncTemplate, v8::Handle<v8::ObjectTemplate>& prototypeTemplate, MetadataTreeNode *treeNode);
		v8::Handle<v8::Function> SetMembersFromRuntimeMetadata(v8::Isolate *isolate, v8::Handle<v8::FunctionTemplate>& ctorFuncTemplate, v8::Handle<v8::ObjectTemplate>& prototypeTemplate, MetadataTreeNode *treeNode);
		void SetInnnerTypes(v8::Isolate *isolate, v8::Handle<v8::Function>& ctorFunction, MetadataTreeNode *treeNode);


		static std::string CreateFullClassName(const std::string& className, const std::string& extendNameAndLocation);
		static void MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
		static void InterfaceConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
		static void ClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
		static void ExtendCallMethodHandler(const v8::FunctionCallbackInfo<v8::Value>& info);
		static bool ValidateExtendArguments(const v8::FunctionCallbackInfo<v8::Value>& info, std::string& extendLocation, v8::Handle<v8::String>& extendName, v8::Handle<v8::Object>& implementationObject);
		static void ExtendedClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

		static void InnerClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
		static void InnerClassAccessorGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

		static void FieldAccessorGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
		static void FieldAccessorSetterCallback(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
		static void ClassAccessorGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
		static void SetClassAccessor(v8::Handle<v8::Function>& ctorFunction);
		static void SuperAccessorGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
		static void ArrayLengthGetterCallack(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);


		static void PackageGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

		static void ArrayIndexedPropertyGetterCallback(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);
		static void ArrayIndexedPropertySetterCallback(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

		static std::map<std::string, MetadataNode*> s_name2NodeCache;
		static std::map<std::string, MetadataTreeNode*> s_name2TreeNodeCache;
		static std::map<MetadataTreeNode*, MetadataNode*> s_treeNode2NodeCache;

		static bool IsValidExtendName(const v8::Handle<v8::String>& name);
		static bool GetExtendLocation(std::string& extendLocation);
		static ExtendedClassCacheData GetCachedExtendedClassData(v8::Isolate *isolate, const std::string& proxyClassName);
		static GetJavaFieldCallback s_getJavaField;
		static SetJavaFieldCallback s_setJavaField;
		static GetArrayElementCallback s_getArrayElement;
		static SetArrayElementCallback s_setArrayElement;
		static CallJavaMethodCallback s_callJavaMethod;
		static RegisterInstanceCallback s_registerInstance;
		static GetTypeMetadataCallback s_getTypeMetadata;
		static FindClassCallback s_findClass;
		static GetArrayLengthCallback s_getArrayLength;
		static ResolveClassCallback s_resolveClass;

		static MetadataReader s_metadataReader;

		static v8::Persistent<v8::String> *s_metadataKey;

		static ObjectManager *s_objectManager;

		static std::map<MetadataTreeNode*, v8::Persistent<v8::FunctionTemplate>*> s_ctorFuncCache;


		static std::map<std::string, MetadataNode::ExtendedClassCacheData> s_extendedCtorFuncCache;
	};
}
