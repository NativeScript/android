#ifndef METADATAREADER_H_
#define METADATAREADER_H_

#include "MetadataEntry.h"
#include "MetadataFieldInfo.h"
#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <assert.h>
#include "robin_hood.h"

namespace tns {
    typedef std::vector<std::string> (*GetTypeMetadataCallback)(const std::string &classname,
                                                                int index);

    class MetadataReader {
    public:
        MetadataReader();

        /*
         * Replaces assignment from a temporary: the reader owns a mutex, so it
         * is neither copyable nor assignable.
         */
        void Initialize(uint32_t nodesLength, uint8_t *nodeData, uint32_t nameLength,
                        uint8_t *nameData, uint32_t valueLength, uint8_t *valueData,
                        GetTypeMetadataCallback getTypeMetadataCallack);

        inline static MetadataEntry ReadInstanceFieldEntry(uint8_t **data) {
            MetadataEntry entry(nullptr, NodeType::Field);
            entry.fi = *reinterpret_cast<FieldInfo **>(data);
            entry.isStatic = false;
            entry.isTypeMember = false;

            *data += sizeof(FieldInfo);

            return entry;
        }

        inline static MetadataEntry ReadStaticFieldEntry(uint8_t **data) {
            MetadataEntry entry(nullptr, NodeType::StaticField);
            entry.sfi = *reinterpret_cast<StaticFieldInfo **>(data);
            entry.isStatic = true;
            entry.isTypeMember = false;

            *data += sizeof(StaticFieldInfo);

            return entry;
        }

        inline static MetadataEntry ReadInstanceMethodEntry(uint8_t **data) {
            MetadataEntry entry(nullptr, NodeType::Method);
            entry.isTypeMember = true;

            entry.mi = MethodInfo(*data); // Assign MethodInfo object directly
            *data += entry.mi.GetSizeOfReadMethodInfo();

            return entry;
        }

        inline static MetadataEntry ReadStaticMethodEntry(uint8_t **data) {
            MetadataEntry entry(nullptr, NodeType::Method);
            entry.isTypeMember = true;

            entry.mi = MethodInfo(*data); // Assign MethodInfo object directly
            entry.mi.isStatic = true;
            entry.isStatic = true;

            *data += entry.mi.GetSizeOfReadMethodInfo();

            return entry;
        }

        inline static MetadataEntry ReadExtensionFunctionEntry(uint8_t **data) {
            MetadataEntry entry(nullptr, NodeType::Method);

            entry.mi = MethodInfo(*data); // Assign MethodInfo object directly
            entry.mi.isStatic = true;
            entry.isExtensionFunction = true;
            entry.isStatic = true;

            *data += entry.mi.GetSizeOfReadMethodInfo();

            return entry;
        }

        inline std::string ReadTypeName(uint16_t nodeId) {
            MetadataTreeNode *treeNode = GetNodeById(nodeId);

            return ReadTypeName(treeNode);
        }

        std::string ReadTypeName(MetadataTreeNode *treeNode);

        inline std::string ReadName(uint32_t offset) {
            uint16_t length = *reinterpret_cast<short *>(m_nameData + offset);

            std::string name(reinterpret_cast<char *>(m_nameData + offset + sizeof(uint16_t)),
                             length);

            return name;
        }

        inline std::string
        ReadInterfaceImplementationTypeName(MetadataTreeNode *treeNode, bool &isPrefix) {
            uint8_t *data =
                    m_valueData + treeNode->offsetValue + sizeof(uint8_t) + sizeof(uint16_t);

            isPrefix = *data == 1;

            uint32_t pos = *reinterpret_cast<uint32_t *>(data + sizeof(uint8_t));

            uint16_t len = *reinterpret_cast<uint16_t *>(m_nameData + pos);

            char *ptr = reinterpret_cast<char *>(m_nameData + pos + sizeof(uint16_t));

            std::string name(ptr, len);

            assert(name.length() == len);

            return name;
        }

        uint8_t *GetValueData() const;

        uint8_t GetNodeType(MetadataTreeNode *treeNode);

        uint16_t GetNodeId(MetadataTreeNode *treeNode);

        MetadataTreeNode *GetRoot() const;

        MetadataTreeNode *GetOrCreateTreeNodeByName(const std::string &className);

        MetadataTreeNode *GetBaseClassNode(MetadataTreeNode *treeNode);

        MetadataTreeNode *GetNodeById(uint16_t nodeId);

        inline bool IsNodeTypeArray(uint8_t type) {
            bool isArray = (((type & MetadataTreeNode::PRIMITIVE) == 0) &&
                            ((type & MetadataTreeNode::ARRAY) == MetadataTreeNode::ARRAY));

            return isArray;
        }

        inline bool IsNodeTypeStatic(uint8_t type) {
            bool isStatic = (type & MetadataTreeNode::STATIC) == MetadataTreeNode::STATIC;

            return isStatic;
        }

        inline bool IsNodeTypeClass(uint8_t type) {
            bool isClass = (((type & MetadataTreeNode::PRIMITIVE) == 0) &&
                            ((type & MetadataTreeNode::CLASS) == MetadataTreeNode::CLASS));

            return isClass;
        }

        inline bool IsNodeTypeInterface(uint8_t type) {
            bool isInterface = (((type & MetadataTreeNode::PRIMITIVE) == 0) &&
                                ((type & MetadataTreeNode::INTERFACE) ==
                                 MetadataTreeNode::INTERFACE));

            return isInterface;
        }

        inline bool IsNodeTypePackage(uint8_t type) {
            bool isPackage = type == MetadataTreeNode::PACKAGE;

            return isPackage;
        }

        inline static std::string ParseReturnType(const std::string &signature) {
            int idx = signature.find(')');
            auto returnType = signature.substr(idx + 1);
            return returnType;
        }

        inline static MethodReturnType GetReturnType(const std::string &returnType) {
            MethodReturnType retType;
            char retTypePrefix = returnType[0];
            switch (retTypePrefix) {
                case 'V':
                    retType = MethodReturnType::Void;
                    break;
                case 'B':
                    retType = MethodReturnType::Byte;
                    break;
                case 'S':
                    retType = MethodReturnType::Short;
                    break;
                case 'I':
                    retType = MethodReturnType::Int;
                    break;
                case 'J':
                    retType = MethodReturnType::Long;
                    break;
                case 'F':
                    retType = MethodReturnType::Float;
                    break;
                case 'D':
                    retType = MethodReturnType::Double;
                    break;
                case 'C':
                    retType = MethodReturnType::Char;
                    break;
                case 'Z':
                    retType = MethodReturnType::Boolean;
                    break;
                case '[':
                case 'L':
                    retType = (returnType == "Ljava/lang/String;")
                              ? MethodReturnType::String
                              : MethodReturnType::Object;
                    break;
                default:
                    assert(false);
                    break;
            }
            return retType;
        }

    private:

        /*
         * Guards the reader's mutable state: the node vector (which reallocates
         * on push_back), the value-buffer bump allocator, the type-name cache
         * and the node types memoized onto the tree. Every runtime's thread
         * reaches all of it through MetadataNode, on the path that runs for
         * every JS-wrapper creation.
         *
         * Reentrant because GetOrCreateTreeNodeByName recurses into itself, and
         * releasable to zero mid-section because that function resolves an
         * unknown type by calling into Java -- ART class loading, and dex
         * generation on the .extend() path. Held across that, it could invert
         * against the monitor com.tns.Runtime's constructor takes and against
         * the cross-thread callJSMethod wait. std::recursive_mutex cannot
         * express the release: unlocking it once drops a single level, so a
         * nested caller would still exclude every other thread.
         *
         * Ordering rule: the only lock that may be taken while this one is held
         * is Runtime::s_runtimeCacheMutex.
         */
        class StateMutex {
        public:
            void Lock();

            void Unlock();

            // Drops every level this thread holds, returning the count so
            // Reacquire can restore it. Only for use around a JNI call that can
            // re-enter JS.
            unsigned ReleaseAll();

            void Reacquire(unsigned depth);

        private:
            std::mutex mutex_;
            std::condition_variable available_;
            std::thread::id owner_;
            unsigned depth_ = 0;
        };

        class StateLock {
        public:
            explicit StateLock(StateMutex &mutex) : mutex_(mutex) {
                mutex_.Lock();
            }

            ~StateLock() {
                mutex_.Unlock();
            }

            StateLock(const StateLock &) = delete;
            StateLock &operator=(const StateLock &) = delete;

        private:
            StateMutex &mutex_;
        };

        // The inverse guard: releases the lock for the duration of a scope.
        class StateUnlock {
        public:
            explicit StateUnlock(StateMutex &mutex)
                    : mutex_(mutex), depth_(mutex.ReleaseAll()) {
            }

            ~StateUnlock() {
                mutex_.Reacquire(depth_);
            }

            StateUnlock(const StateUnlock &) = delete;
            StateUnlock &operator=(const StateUnlock &) = delete;

        private:
            StateMutex &mutex_;
            unsigned depth_;
        };

        static const uint32_t ARRAY_OFFSET = INT32_MAX; // 2147483647

        MetadataTreeNode *BuildTree();

        std::string ReadTypeNameInternal(MetadataTreeNode *treeNode);

        void FillEntryWithFiedldInfo(FieldInfo *fi, MetadataEntry &entry);

        void FillEntryWithMethodInfo(MethodInfo &mi, MetadataEntry &entry);

        MetadataTreeNode *m_root;
        uint32_t m_nodesLength;
        uint32_t m_nameLength;
        uint32_t m_valueLength;
        uint8_t *m_nodeData;
        uint8_t *m_nameData;
        uint8_t *m_valueData;
        std::vector<MetadataTreeNode *> m_v;
        GetTypeMetadataCallback m_getTypeMetadataCallback;

        robin_hood::unordered_map<MetadataTreeNode *, std::string> m_typeNameCache;

        mutable StateMutex m_stateMutex;
    };
}

#endif /* METADATAREADER_H_ */
