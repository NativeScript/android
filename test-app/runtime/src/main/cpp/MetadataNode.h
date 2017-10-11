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

namespace tns {
class MetadataNode {
    public:
        static void Init(v8::Isolate* isolate);

        static void BuildMetadata(const std::string& filesPath);

        static void EnableProfiler(bool enableProfiler);

        std::string GetName();

        v8::Local<v8::Object> CreateWrapper(v8::Isolate* isolate);

        v8::Local<v8::Object> CreateJSWrapper(v8::Isolate* isolate, ObjectManager* objectManager);

        v8::Local<v8::Object> CreateArrayWrapper(v8::Isolate* isolate);

        static MetadataNode* GetNodeFromHandle(const v8::Local<v8::Object>& value);

        static v8::Local<v8::Object> CreateExtendedJSWrapper(v8::Isolate* isolate, ObjectManager* objectManager, const std::string& proxyClassName);

        static v8::Local<v8::Object> GetImplementationObject(v8::Isolate* isolate, const v8::Local<v8::Object>& object);

        static void CreateTopLevelNamespaces(v8::Isolate* isolate, const v8::Local<v8::Object>& global);

        static MetadataNode* GetOrCreate(const std::string& className);

        static std::string GetTypeMetadataName(v8::Isolate* isolate, v8::Local<v8::Value>& value);

    private:
        struct MethodCallbackData;

        struct ExtendedClassCallbackData;

        struct ExtendedClassCacheData;

        struct TypeMetadata;

        struct MetadataNodeCache;

        MetadataNode(MetadataTreeNode* treeNode);

        v8::Local<v8::Object> CreatePackageObject(v8::Isolate* isolate);

        v8::Local<v8::Function> GetConstructorFunction(v8::Isolate* isolate);
        v8::Local<v8::FunctionTemplate> GetConstructorFunctionTemplate(v8::Isolate* isolate, MetadataTreeNode* treeNode);
        v8::Local<v8::FunctionTemplate> GetConstructorFunctionTemplate(v8::Isolate* isolate, MetadataTreeNode* treeNode, std::vector<MethodCallbackData*>& instanceMethodsCallbackData);
        v8::Persistent<v8::Function>* GetPersistentConstructorFunction(v8::Isolate* isolate);

        std::vector<MethodCallbackData*> SetInstanceMembers(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate>& ctorFuncTemplate, v8::Local<v8::ObjectTemplate>& prototypeTemplate, std::vector<MethodCallbackData*>& instanceMethodsCallbackData, const std::vector<MethodCallbackData*>& baseInstanceMethodsCallbackData, MetadataTreeNode* treeNode);
        std::vector<MethodCallbackData*> SetInstanceMethodsFromStaticMetadata(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate>& ctorFuncTemplate, v8::Local<v8::ObjectTemplate>& prototypeTemplate, std::vector<MethodCallbackData*>& instanceMethodsCallbackData, const std::vector<MethodCallbackData*>& baseInstanceMethodsCallbackData, MetadataTreeNode* treeNode);
        void SetInstanceFieldsFromStaticMetadata(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate>& ctorFuncTemplate, v8::Local<v8::ObjectTemplate>& prototypeTemplate, std::vector<MethodCallbackData*>& instanceMethodsCallbackData, const std::vector<MethodCallbackData*>& baseInstanceMethodsCallbackData, MetadataTreeNode* treeNode);
        std::vector<MethodCallbackData*> SetInstanceMembersFromRuntimeMetadata(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate>& ctorFuncTemplate, v8::Local<v8::ObjectTemplate>& prototypeTemplate, std::vector<MethodCallbackData*>& instanceMethodsCallbackData, const std::vector<MethodCallbackData*>& baseInstanceMethodsCallbackData, MetadataTreeNode* treeNode);
        void SetStaticMembers(v8::Isolate* isolate, v8::Local<v8::Function>& ctorFunction, MetadataTreeNode* treeNode);
        void SetInnerTypes(v8::Isolate* isolate, v8::Local<v8::Function>& ctorFunction, MetadataTreeNode* treeNode);

        static void BuildMetadata(uint32_t nodesLength, uint8_t* nodeData, uint32_t nameLength, uint8_t* nameData, uint32_t valueLength, uint8_t* valueData);

        static MetadataNodeCache* GetMetadataNodeCache(v8::Isolate* isolate);

        static MetadataNode* GetOrCreateInternal(MetadataTreeNode* treeNode);

        static MetadataTreeNode* GetOrCreateTreeNodeByName(const std::string& className);

        static MetadataEntry GetChildMetadataForPackage(MetadataNode* node, const std::string& propName);

        static MetadataNode* GetInstanceMetadata(v8::Isolate* isolate, const v8::Local<v8::Object>& value);

        static void SetInstanceMetadata(v8::Isolate* isolate, v8::Local<v8::Object> value, MetadataNode* node);

        static TypeMetadata* GetTypeMetadata(v8::Isolate* isolate, const v8::Local<v8::Function>& value);

        static void SetTypeMetadata(v8::Isolate* isolate, v8::Local<v8::Function> value, TypeMetadata* data);

        static std::string CreateFullClassName(const std::string& className, const std::string& extendNameAndLocation);
        static void MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void InterfaceConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void ClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void ExtendMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static bool ValidateExtendArguments(const v8::FunctionCallbackInfo<v8::Value>& info, std::string& extendLocation, v8::Local<v8::String>& extendName, v8::Local<v8::Object>& implementationObject);
        static void ExtendedClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

        static void NullObjectAccessorGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void NullValueOfCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void FieldAccessorGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void FieldAccessorSetterCallback(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void ClassAccessorGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetClassAccessor(v8::Local<v8::Function>& ctorFunction);
        static void SuperAccessorGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void ArrayLengthGetterCallack(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static void PackageGetterCallback(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static void ArrayIndexedPropertyGetterCallback(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void ArrayIndexedPropertySetterCallback(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

        static bool IsValidExtendName(const v8::Local<v8::String>& name);
        static bool GetExtendLocation(std::string& extendLocation);
        static ExtendedClassCacheData GetCachedExtendedClassData(v8::Isolate* isolate, const std::string& proxyClassName);

        v8::Local<v8::Function> Wrap(v8::Isolate* isolate, const v8::Local<v8::Function>& function, const std::string& name, const std::string& origin, bool isCtorFunc);

        bool CheckClassHierarchy(JEnv& env, jclass currentClass, MetadataTreeNode* curentTreeNode, MetadataTreeNode* baseTreeNode, std::vector<MetadataTreeNode*>& skippedBaseTypes);
        void SetMissingBaseMethods(v8::Isolate* isolate, const std::vector<MetadataTreeNode*>& skippedBaseTypes, const std::vector<MethodCallbackData*>& instanceMethodData, v8::Local<v8::ObjectTemplate>& prototypeTemplate);

        MetadataTreeNode* m_treeNode;
        std::map<v8::Isolate*, v8::Persistent<v8::Function>*> m_poCtorCachePerIsolate;
        std::string m_name;
        std::string m_implType;
        bool m_isArray;

        static std::string TNS_PREFIX;
        static MetadataReader s_metadataReader;
        static std::map<std::string, MetadataNode*> s_name2NodeCache;
        static std::map<std::string, MetadataTreeNode*> s_name2TreeNodeCache;
        static std::map<MetadataTreeNode*, MetadataNode*> s_treeNode2NodeCache;
        static std::map<v8::Isolate*, MetadataNodeCache*> s_metadata_node_cache;
        static bool s_profilerEnabled;

        struct MethodCallbackData {
            MethodCallbackData()
                :
                node(nullptr), parent(nullptr), isSuper(false) {
            }

            MethodCallbackData(MetadataNode* _node)
                :
                node(_node), parent(nullptr), isSuper(false) {
            }

            std::vector<MetadataEntry> candidates;
            MetadataNode* node;
            MethodCallbackData* parent;
            bool isSuper;
        };

        struct ExtendedClassCallbackData {
            ExtendedClassCallbackData(MetadataNode* _node, const std::string& _extendedName, const v8::Local<v8::Object>& _implementationObject, std::string _fullClassName)
                :
                node(_node), extendedName(_extendedName), fullClassName(_fullClassName) {
                implementationObject = new v8::Persistent<v8::Object>(_implementationObject->GetIsolate(), _implementationObject);
            }

            MetadataNode* node;
            std::string extendedName;
            v8::Persistent<v8::Object>* implementationObject;

            std::string fullClassName;
        };

        struct TypeMetadata {
            TypeMetadata(const std::string& _name)
                :
                name(_name) {
            }

            std::string name;
        };

        struct CtorCacheData {
            CtorCacheData(v8::Persistent<v8::FunctionTemplate>* _ft, std::vector<MethodCallbackData*> _instanceMethodCallbacks)
                :
                ft(_ft), instanceMethodCallbacks(_instanceMethodCallbacks) {
            }

            v8::Persistent<v8::FunctionTemplate>* ft;
            std::vector<MethodCallbackData*> instanceMethodCallbacks;
        };

        struct ExtendedClassCacheData {
            ExtendedClassCacheData()
                :
                extendedCtorFunction(nullptr), node(nullptr) {
            }
            ExtendedClassCacheData(const v8::Local<v8::Function>& extCtorFunc, const std::string& _extendedName, MetadataNode* _node)
                :
                extendedName(_extendedName), node(_node) {
                extendedCtorFunction = new v8::Persistent<v8::Function>(extCtorFunc->GetIsolate(), extCtorFunc);
            }
            v8::Persistent<v8::Function>* extendedCtorFunction;
            std::string extendedName;
            MetadataNode* node;
        };

        struct MetadataNodeCache {
            v8::Persistent<v8::String>* MetadataKey;

            std::map<MetadataTreeNode*, CtorCacheData> CtorFuncCache;

            std::map<std::string, MetadataNode::ExtendedClassCacheData> ExtendedCtorFuncCache;
        };
};
}
