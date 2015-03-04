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
#include "ArgsWrapper.h"
#include <string>
#include <vector>
#include <map>

namespace tns
{
	typedef void (*SetJavaFieldCallback)(const v8::Handle<v8::Object>& target, const v8::Handle<v8::Value>& value, const std::string& className, const std::string& fieldName, const std::string& fieldTypeName, const std::string& declaringTypeName, bool isStatic);

	typedef v8::Handle<v8::Value> (*GetJavaFieldCallback)(const v8::Handle<v8::Object>& caller, const std::string& classJniSignature, const std::string& fieldName, const std::string& fieldTypeName, const std::string& staticSignature, const bool isStatic);

	typedef v8::Handle<v8::Value> (*GetArrayElementCallback)(const v8::Handle<v8::Object>& array, uint32_t index, const std::string& arraySignature);

	typedef void (*SetArrayElementCallback)(const v8::Handle<v8::Object>& array, uint32_t index, const std::string& arraySignature, v8::Handle<v8::Value>& value);

	typedef void (*CallJavaMethodCallback)(const v8::Handle<v8::Object>& caller, const std::string& classJniSignature, const std::string& methodName, const std::string& methodJniSignature, const std::string& declaringClassJniSignature, bool isStatic, bool isSuper, const v8::FunctionCallbackInfo<v8::Value>& args);

	typedef bool (*RegisterInstanceCallback)(const v8::Handle<v8::Object>& jsObject, const std::string& name, const std::string& className, const ArgsWrapper& argWrapper, const v8::Handle<v8::Object>& implementationObject, bool isInterface);

	typedef void (*MakeClassInstanceOfTypeStrongCallback)(const std::string& classPath, const v8::Handle<v8::Object>& classObj);

	class MetadataNode
	{
	public:
		static void SubscribeCallbacks(GetJavaFieldCallback getJavaFieldCallback,
										SetJavaFieldCallback setJavaFieldCallback,
										GetArrayElementCallback getArrayElementCallback,
										SetArrayElementCallback setArrayElementCallback,
										MakeClassInstanceOfTypeStrongCallback makeClassInstanceOfTypeStrongCallback,
										CallJavaMethodCallback callJavaMethodCallback,
										RegisterInstanceCallback registerInstanceCallback,
										GetTypeMetadataCallback getTypeMetadataCallback);


		static void BuildMetadata(uint8_t *nodes, int nodesLength, uint8_t *names, uint8_t *values);

		std::string GetName();

		v8::Handle<v8::Object> CreateProxy(v8::Isolate *isolate);

		v8::Handle<v8::Object> CreateInstanceProxy(v8::Isolate *isolate);

		static v8::Handle<v8::Object> CreateJSInstance(const v8::Handle<v8::Object>& classProxy, MetadataNode *node, const v8::Handle<v8::Object>& implementationObject, std::string& name);

		static v8::Handle<v8::Object> GetExistingClassProxy(const std::string& name);

		static bool ExistsExtendName(const std::string& name);

		static MetadataNode* GetNodeFromHandle(const v8::Handle<v8::Object>& value);

		static v8::Handle<v8::Value> GetPropertyFromImplementationObject(const v8::Handle<v8::Object> object, const v8::Handle<v8::String>& propertyName);

		static v8::Handle<v8::Object> GetImplementationObject(const v8::Handle<v8::Object>& object);

		static void CreateTopLevelNamespaces(const v8::Handle<v8::Object>& global);

		static std::string METADATA_NODE_KEY_NAME;

		static std::string TNS_PREFIX;

		static std::string TNS_TESTS_PREFIX;

		static MetadataNode* GetOrCreate(const std::string& className);

		static void BuildMetadata(uint32_t nodesLength, uint8_t *nodeData, uint8_t *nameData, uint8_t *valueData);


	private:
		struct MethodCallbackState
		{
			MethodCallbackState()
				: node(nullptr), isSuper(false)
			{
			}

			std::vector<MetadataEntry> candidates;
			MetadataNode *node;
			bool isSuper;
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

		static std::vector<MetadataEntry> GetMetadataCandidates(MetadataNode *node, const std::string& propName);

		static std::vector<MetadataEntry> GetMetadataCandidatesForType(MetadataNode *node, const std::string& propName);

		static std::vector<MetadataEntry> GetMetadataCandidatesForTypeWithCustomMetadata(MetadataNode *node, const std::string& propName);

		static std::vector<MetadataEntry> GetMetadataCandidatesForTypeWithoutCustomMetadata(MetadataNode *node, const std::string& propName);

		static std::vector<MetadataEntry> GetMetadataCandidatesForArray(MetadataNode *node, const std::string& propName);

		static std::vector<MetadataEntry> GetMetadataCandidatesForPackage(MetadataNode *node, const std::string& propName);


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
		NodeType m_type;
		bool m_isArray;
		std::vector<MetadataCacheItem> m_childCache;
		std::vector<MetadataCacheItem> m_superMethodCache;


		v8::Handle<v8::Object> CreatePackageProxy(v8::Isolate *isolate);
		v8::Handle<v8::Object> CreateClassProxy(v8::Isolate *isolate, bool isClass);
		v8::Handle<v8::Function> CreateExtendedClassProxy(v8::Isolate *isolate, const v8::Handle<v8::Object>& classProxy, const v8::Handle<v8::Object>& implementationObject, const v8::Handle<v8::String>& name);
		v8::Handle<v8::Object> CreateStaticClassProxy(v8::Isolate *isolate);
		v8::Handle<v8::Function> CreateFunction(const v8::Handle<v8::Object>& thiz, const std::vector<MetadataEntry>& candidates);

		bool CheckForCachedMetadataItem(const std::string& key, const v8::PropertyCallbackInfo<v8::Value>& info, const v8::Local<v8::Object>& thiz, bool isSuper);
		void CacheMetadataItem(const std::string& key, v8::Persistent<v8::Value> *po, MetadataCacheItemType itemType, bool isSuper);
		void CacheMetadataItem(const std::string& key, v8::Persistent<v8::Value> *po, MetadataCacheItemType itemType, bool isSuper, const MetadataEntry& entry);


		bool IsArray();


		static bool IsClassMetadataName(const std::string& name);

		static bool IsMarkedAsSuper(const v8::Handle<v8::Object>& obj);

		static void MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void ConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void ExtendMethodHandler(const v8::FunctionCallbackInfo<v8::Value>& args, MetadataNode *node);
		static void ExtendCallMethodHandler(const v8::FunctionCallbackInfo<v8::Value>& args, MetadataNode *node);

		static void InvokeClassCtor(const v8::FunctionCallbackInfo<v8::Value>& args, MetadataNode *node);
		static void InvokeInterfaceCtor(const v8::FunctionCallbackInfo<v8::Value>& args, MetadataNode *node);

		static std::string ResolveMethodOverload(const v8::FunctionCallbackInfo<v8::Value>& args, const std::string& className, const std::string& methodName);

		static void GetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
		static void SetterCallback(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

		static void ArrayIndexedPropertyGetterCallback(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);
		static void ArrayIndexedPropertySetterCallback(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

		static std::map<std::string, v8::Persistent<v8::Object>* > s_classProxies;
		static std::map<std::string, int> s_usedExtendNames;
		static std::map<std::string, MetadataNode*> s_name2NodeCache;
		static std::map<std::string, MetadataTreeNode*> s_name2TreeNodeCache;
		static std::map<MetadataTreeNode*, MetadataNode*> s_treeNode2NodeCache;

		static void SetDebugName(const std::string& name, const v8::Handle<v8::Object>& value);

		static bool IsValidExtendName(const v8::Handle<v8::String>& name);
		static bool GetExtendLocation(std::string& extendLocation);
		static GetJavaFieldCallback s_getJavaField;
		static SetJavaFieldCallback s_setJavaField;
		static GetArrayElementCallback s_getArrayElement;
		static SetArrayElementCallback s_setArrayElement;
		static MakeClassInstanceOfTypeStrongCallback s_makeClassInstanceOfTypeStrong;
		static CallJavaMethodCallback s_callJavaMethod;
		static RegisterInstanceCallback s_registerInstance;
		static GetTypeMetadataCallback s_getTypeMetadata;

		static MetadataReader s_metadataReader;

		static std::string CLASS_PROXY;
	};
}
