#ifndef METADATAREADER_H_
#define METADATAREADER_H_

#include "MetadataEntry.h"
#include "MetadataFieldInfo.h"
#include <condition_variable>
#include <cstring>
#include <map>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include "NativeScriptAssert.h"
#include "robin_hood.h"

namespace tns {
    typedef std::vector<std::string> (*GetTypeMetadataCallback)(const std::string &classname,
                                                                int index);

    class MetadataReader {
    public:
        /*
         * The layout of the value stream, which the metadata generator's
         * Writer.java produces and has to be changed together with:
         *
         *   format             4 bytes, FORMAT
         *   widths             the bytes a node id, an offset into the names and a
         *                      signature reference each take (1 to 4), and a
         *                      reserved byte. The generator picks the fewest that
         *                      hold this metadata's largest value of each.
         *   signatures length  4 bytes
         *   signatures         each distinct method signature once: a byte holding
         *                      the number of type ids less one, then the ids, the
         *                      return type first
         *   values             one per primitive type and per class or interface,
         *                      at the offset its tree node gives
         *   padding            4 bytes, so the last field can be read at full width
         *
         * A class value is its type (1 byte), its base class id and a byte with a
         * bit for each member section it has. Those sections follow in the order
         * of the bits, each a 2-byte count and its entries.
         */
        static constexpr uint32_t FORMAT = 'N' | ('S' << 8) | ('M' << 16) | (3 << 24);
        static constexpr uint32_t VALUE_HEADER_SIZE = 3 * sizeof(uint32_t);

        static constexpr uint8_t SECTION_EXTENSION_FUNCTIONS = 1;
        static constexpr uint8_t SECTION_INSTANCE_METHODS = 1 << 1;
        static constexpr uint8_t SECTION_INSTANCE_FIELDS = 1 << 2;
        static constexpr uint8_t SECTION_PROPERTIES = 1 << 3;
        static constexpr uint8_t SECTION_STATIC_METHODS = 1 << 4;
        static constexpr uint8_t SECTION_STATIC_FIELDS = 1 << 5;

        static constexpr uint8_t PROPERTY_HAS_GETTER = 1;
        static constexpr uint8_t PROPERTY_HAS_SETTER = 1 << 1;

        MetadataReader();

        /*
         * Replaces assignment from a temporary: the reader owns a mutex, so it
         * is neither copyable nor assignable.
         */
        void Initialize(uint32_t nodesLength, uint8_t *nodeData, uint32_t nameLength,
                        uint8_t *nameData, uint32_t valueLength, uint8_t *valueData,
                        GetTypeMetadataCallback getTypeMetadataCallack);

        /*
         * A field of the node or value stream that is 1 to 4 bytes wide. It always
         * loads four bytes and masks, so there is no branch on the width: the
         * value stream is padded for its last field, and a node record ends in a
         * full four-byte one.
         */
        inline static uint32_t ReadField(const uint8_t *data, uint32_t mask) {
            uint32_t value;
            memcpy(&value, data, sizeof(value));
            return value & mask;
        }

        inline uint32_t ReadId(const uint8_t *data) const {
            return ReadField(data, m_idMask);
        }

        inline uint32_t ReadNameOffset(const uint8_t *data) const {
            return ReadField(data, m_nameMask);
        }

        inline uint32_t ReadSignatureReference(const uint8_t *data) const {
            return ReadField(data, m_signatureMask);
        }

        inline uint32_t IdBytes() const {
            return m_idBytes;
        }

        inline uint32_t NameBytes() const {
            return m_nameBytes;
        }

        inline uint32_t SignatureBytes() const {
            return m_signatureBytes;
        }

        // A field record: its name, the id of its type, its flags (see FieldInfo)
        // and, on a static field flagged for it, the id of its declaring type.
        inline uint32_t FieldNameOffset(const uint8_t *field) const {
            return ReadNameOffset(field);
        }

        inline uint32_t FieldTypeId(const uint8_t *field) const {
            return ReadId(field + m_nameBytes);
        }

        inline uint8_t FieldFlags(const uint8_t *field) const {
            return field[m_nameBytes + m_idBytes];
        }

        inline uint32_t FieldDeclaringTypeId(const uint8_t *field) const {
            return ReadId(field + m_nameBytes + m_idBytes + sizeof(uint8_t));
        }

        inline MetadataEntry ReadInstanceFieldEntry(uint8_t **data) {
            MetadataEntry entry(nullptr, NodeType::Field);
            entry.field = *data;
            entry.isStatic = false;
            entry.isTypeMember = false;

            *data += m_nameBytes + m_idBytes + sizeof(uint8_t);

            return entry;
        }

        inline MetadataEntry ReadStaticFieldEntry(uint8_t **data) {
            MetadataEntry entry(nullptr, NodeType::StaticField);
            entry.field = *data;
            entry.isStatic = true;
            entry.isTypeMember = false;

            *data += m_nameBytes + m_idBytes + sizeof(uint8_t);
            if (FieldFlags(entry.field) & FieldInfo::HAS_DECLARING_TYPE) {
                *data += m_idBytes;
            }

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

        inline std::string ReadTypeName(uint32_t nodeId) {
            MetadataTreeNode *treeNode = GetNodeById(nodeId);

            return ReadTypeName(treeNode);
        }

        std::string ReadTypeName(MetadataTreeNode *treeNode);

        inline std::string_view ReadNameView(uint32_t offset) {
            uint16_t length = *reinterpret_cast<uint16_t *>(m_nameData + offset);

            return {reinterpret_cast<char *>(m_nameData + offset + sizeof(uint16_t)), length};
        }

        inline std::string ReadName(uint32_t offset) {
            uint16_t length = *reinterpret_cast<short *>(m_nameData + offset);

            std::string name(reinterpret_cast<char *>(m_nameData + offset + sizeof(uint16_t)),
                             length);

            return name;
        }

        uint8_t *GetValueData() const;

        inline const uint8_t *GetSignatureData() const {
            return m_signatureData;
        }

        inline uint8_t GetClassSections(MetadataTreeNode *treeNode) const {
            return m_valueData[treeNode->offsetValue + sizeof(uint8_t) + m_idBytes];
        }

        // Where a class value's member sections start: after its type, its base
        // class id and the byte saying which sections it has.
        inline uint8_t *GetClassMembers(MetadataTreeNode *treeNode) const {
            return m_valueData + treeNode->offsetValue + sizeof(uint8_t) + m_idBytes + sizeof(uint8_t);
        }

        // The number of entries in one member section of a class value, leaving
        // data at the first of them. A section the class lacks takes no bytes.
        inline static uint16_t ReadSectionCount(uint8_t sections, uint8_t section, uint8_t **data) {
            if ((sections & section) == 0) {
                return 0;
            }

            uint16_t count;
            memcpy(&count, *data, sizeof(count));
            *data += sizeof(count);

            return count;
        }

        uint8_t GetNodeType(MetadataTreeNode *treeNode);

        uint32_t GetNodeId(MetadataTreeNode *treeNode);

        MetadataTreeNode *GetRoot() const;

        MetadataTreeNode *GetOrCreateTreeNodeByName(const std::string &className);

        MetadataTreeNode *GetBaseClassNode(MetadataTreeNode *treeNode);

        MetadataTreeNode *GetNodeById(uint32_t nodeId);

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
                    NS_DCHECK(false);
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

        // A type resolved at run time has no value in the stream, which is mapped
        // read-only. Its node only needs an offset that reads as "a class": not 0,
        // which means a package, and below ARRAY_OFFSET. This one is inside the
        // stream's header, where no value starts.
        static const uint32_t RUNTIME_TYPE_VALUE_OFFSET = 1;

        // The static metadata numbers its nodes from 0, and the generator stops
        // before this many. Nodes created at run time are numbered from here, which
        // still leaves an array's marker, ARRAY_OFFSET plus an id, within 32 bits.
        static const uint32_t RUNTIME_NODE_ID_BASE = 1 << 30;

        MetadataTreeNode *BuildTree();

        // Looks through the node's static children, then the ones created at run
        // time. The caller holds the state lock.
        template<typename Predicate>
        MetadataTreeNode *FindChildIf(MetadataTreeNode *parent, Predicate matches);

        MetadataTreeNode *FindChild(MetadataTreeNode *parent, const std::string &name);

        void AddRuntimeChild(MetadataTreeNode *parent, MetadataTreeNode *child);

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
        const uint8_t *m_signatureData;
        uint32_t m_idBytes;
        uint32_t m_nameBytes;
        uint32_t m_signatureBytes;
        uint32_t m_idMask;
        uint32_t m_nameMask;
        uint32_t m_signatureMask;
        std::vector<MetadataTreeNode *> m_v;
        // Nodes created at run time, in creation order and by parent. They stay out
        // of MetadataTreeNode::children so the static tree never changes once
        // built, which is what lets MetadataNode walk it without this reader's lock.
        std::vector<MetadataTreeNode *> m_runtimeNodes;
        robin_hood::unordered_node_map<MetadataTreeNode *, std::vector<MetadataTreeNode *>> m_runtimeChildren;
        GetTypeMetadataCallback m_getTypeMetadataCallback;

        robin_hood::unordered_map<MetadataTreeNode *, std::string> m_typeNameCache;

        mutable StateMutex m_stateMutex;
    };
}

#endif /* METADATAREADER_H_ */
