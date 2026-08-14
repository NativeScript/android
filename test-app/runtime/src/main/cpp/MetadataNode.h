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
#include <atomic>
#include "MetadataEntry.h"
#include "MetadataTreeNode.h"
#include "MetadataReader.h"
#include "FieldCallbackData.h"
#include "ArgsWrapper.h"
#include "ObjectManager.h"
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace tns {
class MetadataNode {
    public:
        static void Init(v8::Isolate* isolate);

        static void BuildMetadata(const std::string& filesPath);

        static void EnableProfiler(bool enableProfiler);

        std::string GetName();

        bool IsNodeTypeInterface();

        v8::Local<v8::Object> CreateWrapper(v8::Isolate* isolate);

        v8::Local<v8::Object> CreateJSWrapper(v8::Isolate* isolate, ObjectManager* objectManager);

        v8::Local<v8::Object> CreateArrayWrapper(v8::Isolate* isolate);

        static MetadataNode* GetNodeFromHandle(const v8::Local<v8::Object>& value);

        static v8::Local<v8::Object> CreateExtendedJSWrapper(v8::Isolate* isolate, ObjectManager* objectManager, const std::string& proxyClassName);

        /*
         * Java-born instances of an ES-derived proxy (clazz.newInstance(),
         * framework inflation, etc.) are adopted into a real construct of the
         * ES class so fields and the constructor body run. super() binds the
         * existing Java object and does not allocate again.
         */
        static bool TryConstructESDerivedInstance(v8::Isolate* isolate, const std::string& proxyClassName, int javaObjectID, v8::Local<v8::Object>& out);

        static bool TryConsumePendingESAdopt(v8::Isolate* isolate, int& javaObjectID);

        static v8::Local<v8::Object> GetImplementationObject(v8::Isolate* isolate, const v8::Local<v8::Object>& object);

        static void CreateTopLevelNamespaces(v8::Isolate* isolate, const v8::Local<v8::Object>& global);

        static MetadataNode* GetOrCreate(const std::string& className);

        static std::string GetTypeMetadataName(v8::Isolate* isolate, v8::Local<v8::Value>& value);

        /*
         * Non-throwing metadata lookups for the console formatter. They read
         * private symbols only, so they never re-enter JS or trip an
         * interceptor, and they tolerate values carrying no metadata at all
         * (unlike GetNodeFromHandle/GetTypeMetadataName, which assume it is
         * there). Names come back in the metadata's slashed JNI form.
         */
        static bool TryGetInstanceTypeName(v8::Isolate* isolate, const v8::Local<v8::Object>& value, std::string& out);

        static bool TryGetPackageName(v8::Isolate* isolate, const v8::Local<v8::Object>& value, std::string& out);

        /*
         * Resolves the Java class name a constructor function stands for, lazily registering a
         * Java proxy class when the function is a plain ES `class X extends NativeType {}` that
         * has not been registered yet. Returns an empty string when the function is not part of
         * a native inheritance chain. Used when marshalling a constructor function to a Java
         * `java.lang.Class` (or `java.lang.Object`) argument.
         */
        static std::string TryResolveClassCtorTypeName(v8::Isolate* isolate, const v8::Local<v8::Function>& func);

        static MetadataReader* getMetadataReader();
    private:
        struct MethodCallbackData;

        struct ExtendedClassCallbackData;

        struct ExtendedClassCacheData;

        struct TypeMetadata;

        struct MetadataNodeCache;
        class PrototypeTemplateFiller;

        MetadataNode(MetadataTreeNode* treeNode);

        static bool IsJavascriptKeyword(const std::string &word);
        v8::Local<v8::Object> CreatePackageObject(v8::Isolate* isolate);

        v8::Local<v8::Function> GetConstructorFunction(v8::Isolate* isolate);
        v8::Local<v8::FunctionTemplate> GetConstructorFunctionTemplate(v8::Isolate* isolate, MetadataTreeNode* treeNode);
        v8::Local<v8::FunctionTemplate> GetConstructorFunctionTemplate(v8::Isolate* isolate, MetadataTreeNode* treeNode, std::vector<MethodCallbackData*>& instanceMethodsCallbackData);
        v8::Persistent<v8::Function>* GetPersistentConstructorFunction(v8::Isolate* isolate);
        static v8::Local<v8::ObjectTemplate> GetOrCreateArrayObjectTemplate(v8::Isolate* isolate);

        std::vector<MethodCallbackData*> SetInstanceMembers(
                v8::Isolate* isolate, v8::Local<v8::FunctionTemplate>& ctorFuncTemplate,
                PrototypeTemplateFiller& protoFiller,
                std::vector<MethodCallbackData*>& instanceMethodsCallbackData,
                const std::vector<MethodCallbackData*>& baseInstanceMethodsCallbackData,
                MetadataTreeNode* treeNode, uint8_t* &curPtr);
        std::vector<MethodCallbackData*> SetInstanceMethodsFromStaticMetadata(
                v8::Isolate* isolate, v8::Local<v8::FunctionTemplate>& ctorFuncTemplate,
                PrototypeTemplateFiller& protoFiller,
                std::vector<MethodCallbackData*>& instanceMethodsCallbackData,
                const std::vector<MethodCallbackData*>& baseInstanceMethodsCallbackData,
                MetadataTreeNode* treeNode, uint8_t* &curPtr);
        static MethodCallbackData* tryGetExtensionMethodCallbackData(
                const robin_hood::unordered_map<std::string, MethodCallbackData *> &collectedMethodCallbackDatas,
                const std::string &lookupName);
        void SetInstanceFieldsFromStaticMetadata(
                v8::Isolate* isolate, PrototypeTemplateFiller& protoFiller,
                MetadataTreeNode* treeNode);
        std::vector<MethodCallbackData*> SetInstanceMembersFromRuntimeMetadata(
                v8::Isolate* isolate, PrototypeTemplateFiller& protoFiller,
                std::vector<MethodCallbackData*>& instanceMethodsCallbackData,
                const std::vector<MethodCallbackData*>& baseInstanceMethodsCallbackData,
                MetadataTreeNode* treeNode);

        void SetStaticMembers(v8::Isolate* isolate, v8::Local<v8::Function>& ctorFunction, MetadataTreeNode* treeNode, uint8_t* &curPtr);
        static void InnerTypeAccessorGetterCallback(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void SetInnerTypes(v8::Isolate* isolate, v8::Local<v8::Function>& ctorFunction, MetadataTreeNode* treeNode);

        static void BuildMetadata(uint32_t nodesLength, uint8_t* nodeData, uint32_t nameLength, uint8_t* nameData, uint32_t valueLength, uint8_t* valueData);

        static MetadataNodeCache* GetMetadataNodeCache(v8::Isolate* isolate);

        static MetadataNode* GetOrCreateInternal(MetadataTreeNode* treeNode);

        static MetadataTreeNode* GetOrCreateTreeNodeByName(const std::string& className);

        static MetadataEntry GetChildMetadataForPackage(MetadataNode *node, const std::string &propName);

        static MetadataNode* GetInstanceMetadata(v8::Isolate* isolate, const v8::Local<v8::Object>& value);

        static void SetInstanceMetadata(v8::Isolate* isolate, v8::Local<v8::Object> value, MetadataNode* node);

        static TypeMetadata* GetTypeMetadata(v8::Isolate* isolate, const v8::Local<v8::Function>& value);

        // Safe variant of GetTypeMetadata - returns nullptr when the function carries no type
        // metadata (e.g. a plain ES class constructor) instead of crashing
        static TypeMetadata* TryGetTypeMetadata(v8::Isolate* isolate, const v8::Local<v8::Function>& value);

        static void SetTypeMetadata(v8::Isolate* isolate, v8::Local<v8::Function> value, TypeMetadata* data);

        /*
         * Lazily registers a Java proxy class for a plain ES `class X extends NativeType {}`
         * constructor function (no `.extend()` call, no downleveling). Walks the constructor
         * prototype chain to the native base, collects overridden method names from every ES
         * level's prototype and implemented interfaces from `static interfaces = [...]`, resolves
         * the proxy class through the regular DexFactory pipeline and tags the constructor the
         * same way `.extend()` tags its result (typemetadata + ExtendedCtorFuncCache entry).
         * Returns nullptr when ctorFunc is not part of a native inheritance chain or the chain
         * goes through a legacy `.extend()`-created class. Idempotent.
         */
        static TypeMetadata* EnsureExtendedESClass(v8::Isolate* isolate, v8::Local<v8::Function> ctorFunc);

        static std::string CreateFullClassName(const std::string& className, const std::string& extendNameAndLocation);
        static void MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void InterfaceConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void ClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void ExtendMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static bool ValidateExtendArguments(const v8::FunctionCallbackInfo<v8::Value>& info, bool extendLocationFound, std::string& extendLocation, v8::Local<v8::String>& extendName, v8::Local<v8::Object>& implementationObject, bool isTypeScriptExtend);
        static void ExtendedClassConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

        static void NullObjectAccessorGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void NullValueOfCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void FieldAccessorGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void FieldAccessorSetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void PropertyAccessorGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void PropertyAccessorSetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

        static void ClassAccessorGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void SetClassAccessor(v8::Local<v8::Function>& ctorFunction);
        static void SuperAccessorGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void ArrayLengthGetterCallack(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static void PackageGetterCallback(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static v8::Intercepted ArrayIndexedPropertyGetterCallback(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);
        static v8::Intercepted ArrayIndexedPropertySetterCallback(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Boolean>& info);

        static bool IsValidExtendName(const v8::Local<v8::String>& name);
        static bool GetExtendLocation(v8::Isolate* isolate, std::string& extendLocation, bool isTypeScriptExtend);
        static ExtendedClassCacheData GetCachedExtendedClassData(v8::Isolate* isolate, const std::string& proxyClassName);

        static void RegisterSymbolHasInstanceCallback(v8::Isolate* isolate, MetadataEntry& entry, v8::Local<v8::Value> interface);
        static void SymbolHasInstanceCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
        static std::string GetJniClassName(MetadataEntry& entry);

        static v8::Local<v8::Function> Wrap(v8::Isolate* isolate, const v8::Local<v8::Function>& function, const std::string& name, const std::string& origin, bool isCtorFunc);

        static bool CheckClassHierarchy(JEnv& env, jclass currentClass, MetadataTreeNode* curentTreeNode, MetadataTreeNode* baseTreeNode, std::vector<MetadataTreeNode*>& skippedBaseTypes);
        void SetMissingBaseMethods(v8::Isolate* isolate,
                                   const std::vector<MetadataTreeNode*>& skippedBaseTypes,
                                   const std::vector<MethodCallbackData*>& instanceMethodData,
                                   PrototypeTemplateFiller& protoFiller);

        MetadataTreeNode* m_treeNode;
        std::string m_name;
        std::string m_implType;
        bool m_isArray;

        static std::string TNS_PREFIX;
        static MetadataReader s_metadataReader;
        static robin_hood::unordered_map<std::string, MetadataNode*> s_name2NodeCache;
        static robin_hood::unordered_map<std::string, MetadataTreeNode*> s_name2TreeNodeCache;
        static robin_hood::unordered_map<MetadataTreeNode*, MetadataNode*> s_treeNode2NodeCache;
        // Guards the three caches above; see GetOrCreate for the ordering rule.
        static std::mutex s_nodeCacheMutex;
        static std::atomic<bool> s_profilerEnabled;

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

        struct PropertyCallbackData {
            PropertyCallbackData(std::string _propertyName, std::string _getterMethodName, std::string _setterMethodName)
                :
                    propertyName(_propertyName), getterMethodName(_getterMethodName), setterMethodName(_setterMethodName) {

            }
            std::string propertyName;
            std::string getterMethodName;
            std::string setterMethodName;
        };

        struct ExtendedClassCallbackData {
            ExtendedClassCallbackData(MetadataNode* _node, const std::string& _extendedName, const v8::Local<v8::Object>& _implementationObject, std::string _fullClassName)
                :
                node(_node), extendedName(_extendedName), fullClassName(_fullClassName) {
                implementationObject = new v8::Persistent<v8::Object>(v8::Isolate::GetCurrent(), _implementationObject);
            }

            // Sole owner of implementationObject, which is a strong handle
            // pinning the whole JS implementation object; copying would make
            // that a double free.
            ExtendedClassCallbackData(const ExtendedClassCallbackData&) = delete;
            ExtendedClassCallbackData& operator=(const ExtendedClassCallbackData&) = delete;

            ~ExtendedClassCallbackData() {
                implementationObject->Reset();
                delete implementationObject;
            }

            MetadataNode* node;
            std::string extendedName;
            v8::Persistent<v8::Object>* implementationObject;

            std::string fullClassName;
        };

        struct TypeMetadata {
            TypeMetadata(const std::string& _name, bool _isESDerived = false)
                :
                name(_name), isESDerived(_isESDerived) {
            }

            std::string name;

            // true when the class was registered lazily from a plain ES
            // `class X extends NativeType {}` constructor (see EnsureExtendedESClass)
            bool isESDerived;
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
                extendedCtorFunction = new v8::Persistent<v8::Function>(v8::Isolate::GetCurrent(), extCtorFunc);
            }
            v8::Persistent<v8::Function>* extendedCtorFunction;
            std::string extendedName;
            MetadataNode* node;
        };

        /*
         * Metadata state for one runtime. Owned by RuntimeState, so it is
         * reached without a shared container and destroyed with the runtime,
         * while its isolate is still alive.
         */
        struct MetadataNodeCache {
            // Initialized rather than left indeterminate: the cache is created
            // on first use, which may precede MetadataNode::Init populating
            // these, and the destructor below releases them.
            v8::Persistent<v8::String>* MetadataKey = nullptr;

            v8::Persistent<v8::String>* PackageKey = nullptr;

            robin_hood::unordered_map<MetadataTreeNode*, CtorCacheData> CtorFuncCache;

            robin_hood::unordered_map<std::string, MetadataNode::ExtendedClassCacheData> ExtendedCtorFuncCache;

            // The array wrapper template for this runtime.
            v8::Persistent<v8::ObjectTemplate>* ArrayObjectTemplate = nullptr;

            /*
             * This runtime's constructor function per node. The nodes
             * themselves are shared between runtimes, so this cannot live on
             * them -- it used to, as a map keyed by isolate, which meant every
             * runtime's teardown walked every node to erase its entry.
             */
            robin_hood::unordered_map<MetadataNode*, v8::Persistent<v8::Function>*> CtorFunctions;

            /*
             * Owns every callback payload this runtime hands to V8 as External
             * or FunctionTemplate data -- MethodCallbackData, FieldCallbackData,
             * PropertyCallbackData, TypeMetadata, ExtendedClassCallbackData.
             * V8 attaches no finalizer to any of them, so without an owner they
             * leaked on every GC and, for the ones reachable from a template,
             * for the life of the runtime.
             *
             * An arena rather than per-holder ownership because the same
             * MethodCallbackData is shared: it is reachable from a prototype
             * method, from CtorCacheData::instanceMethodCallbacks, and from the
             * `instanceMethodsCallbackData` a derived class copies out of the
             * cache. One owner sidesteps that entirely.
             *
             * Type-erased so the deleter, not the container, restores the type.
             */
            using OwnedPtr = std::unique_ptr<void, void (*)(void*)>;
            std::vector<OwnedPtr> OwnedCallbackData;

            template <typename T, typename... Args>
            T* Own(Args&&... args) {
                T* ptr = new T(std::forward<Args>(args)...);
                OwnedCallbackData.emplace_back(
                    ptr, [](void* value) { delete static_cast<T*>(value); });
                return ptr;
            }

            // Java object id being adopted by an in-flight ES construct
            // (CreateJSInstanceNative → CallAsConstructor → super()).
            // RegisterInstance consumes it so super() binds that id and does
            // not NewObject again.
            int PendingESAdoptObjectId = -1;

            ~MetadataNodeCache() {
                delete MetadataKey;
                delete PackageKey;
                delete ArrayObjectTemplate;
                for (auto& entry : CtorFunctions) {
                    delete entry.second;
                }
                /*
                 * Freed from the maps rather than from CtorCacheData and
                 * ExtendedClassCacheData themselves: both are held by value and
                 * handed out by value (GetCachedExtendedClassData returns a
                 * copy), and the copies share these raw pointers. A destructor
                 * on either struct would turn every copy into a double free.
                 */
                for (auto& entry : CtorFuncCache) {
                    delete entry.second.ft;
                }
                for (auto& entry : ExtendedCtorFuncCache) {
                    delete entry.second.extendedCtorFunction;
                }
            }
        };
};
}
