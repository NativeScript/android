#include "MetadataReader.h"
#include "NativeScriptAssert.h"
#include "MetadataMethodInfo.h"
#include <android/log.h>
#include "NativeScriptException.h"
#include "Util.h"
#include <sstream>

using namespace std;
using namespace tns;

MetadataReader::MetadataReader() : m_root(nullptr), m_nodesLength(0), m_nameLength(0),
                                   m_valueLength(0),
                                   m_nodeData(nullptr), m_nameData(nullptr), m_valueData(nullptr),
                                   m_signatureData(nullptr),
                                   m_idBytes(0), m_nameBytes(0), m_signatureBytes(0),
                                   m_idMask(0), m_nameMask(0), m_signatureMask(0),
                                   m_getTypeMetadataCallback(nullptr) {}

void MetadataReader::Initialize(uint32_t nodesLength, uint8_t *nodeData, uint32_t nameLength,
                                uint8_t *nameData, uint32_t valueLength, uint8_t *valueData,
                                GetTypeMetadataCallback getTypeMetadataCallback) {
    StateLock lock(m_stateMutex);

    uint32_t format = 0;
    uint32_t idBytes = 0;
    uint32_t nameBytes = 0;
    uint32_t signatureBytes = 0;
    uint32_t signaturesLength = 0;
    if (valueLength >= VALUE_HEADER_SIZE) {
        memcpy(&format, valueData, sizeof(format));
        idBytes = valueData[4];
        nameBytes = valueData[5];
        signatureBytes = valueData[6];
        memcpy(&signaturesLength, valueData + 2 * sizeof(uint32_t), sizeof(signaturesLength));
    }
    auto isFieldWidth = [](uint32_t bytes) { return bytes >= 1 && bytes <= sizeof(uint32_t); };
    if (format != FORMAT
        || !isFieldWidth(idBytes) || !isFieldWidth(nameBytes) || !isFieldWidth(signatureBytes)
        || signaturesLength > valueLength - VALUE_HEADER_SIZE
        || nodesLength % (2 * idBytes + nameBytes + sizeof(uint32_t)) != 0) {
        stringstream ss;
        ss << "The app's metadata is not in the format this runtime reads (found 0x" << hex
           << format << ", expected 0x" << FORMAT
           << "). It was generated for a different version of the runtime, or is damaged. Rebuild the app.";
        // Logged here as well: nothing is set up yet to report an exception this early.
        __android_log_print(ANDROID_LOG_ERROR, "TNS.error", "%s", ss.str().c_str());
        throw NativeScriptException(ss.str());
    }

    auto maskFor = [](uint32_t bytes) {
        return bytes == sizeof(uint32_t) ? UINT32_MAX : (1u << (8 * bytes)) - 1;
    };
    m_idBytes = idBytes;
    m_nameBytes = nameBytes;
    m_signatureBytes = signatureBytes;
    m_idMask = maskFor(idBytes);
    m_nameMask = maskFor(nameBytes);
    m_signatureMask = maskFor(signatureBytes);
    m_signatureData = valueData + VALUE_HEADER_SIZE;

    m_nodesLength = nodesLength;
    m_nodeData = nodeData;
    m_nameLength = nameLength;
    m_nameData = nameData;
    m_valueLength = valueLength;
    m_valueData = valueData;
    m_getTypeMetadataCallback = getTypeMetadataCallback;
    m_v.clear();
    m_runtimeNodes.clear();
    m_runtimeChildren.clear();
    m_typeNameCache.clear();

    m_root = BuildTree();
}

void MetadataReader::StateMutex::Lock() {
    std::unique_lock<std::mutex> guard(mutex_);
    auto self = std::this_thread::get_id();
    available_.wait(guard, [this, self] { return depth_ == 0 || owner_ == self; });
    owner_ = self;
    ++depth_;
}

void MetadataReader::StateMutex::Unlock() {
    std::lock_guard<std::mutex> guard(mutex_);
    // An unmatched unlock would wrap depth_ and hold the mutex forever.
    NS_DCHECK(depth_ > 0 && owner_ == std::this_thread::get_id());
    if (--depth_ == 0) {
        owner_ = std::thread::id();
        // Every waiter is blocked on the same `depth_ == 0`, so waking one is
        // enough -- it takes the lock and the rest stay parked.
        available_.notify_one();
    }
}

unsigned MetadataReader::StateMutex::ReleaseAll() {
    std::lock_guard<std::mutex> guard(mutex_);
    // Only the owner may release: doing this from a non-owner would drop
    // another thread's lock while it is still inside its guarded section.
    NS_DCHECK(depth_ > 0 && owner_ == std::this_thread::get_id());
    unsigned held = depth_;
    depth_ = 0;
    owner_ = std::thread::id();
    available_.notify_one();

    return held;
}

void MetadataReader::StateMutex::Reacquire(unsigned depth) {
    if (depth == 0) {
        return;
    }

    std::unique_lock<std::mutex> guard(mutex_);
    available_.wait(guard, [this] { return depth_ == 0; });
    owner_ = std::this_thread::get_id();
    depth_ = depth;
}



// helper debug function when need to convert a metadata node to its full name
//std::string toFullName(MetadataTreeNode* p) {
//    std::string final = p->name;
//    while((p = p->parent) && !p->name.empty()) {
//        final.insert(0,p->name + ".");
//    };
//    return final;
//}

MetadataTreeNode* MetadataReader::BuildTree() {
    // A node record: the ids of the node's first child and of its next sibling,
    // either of which is the node's own id when there is none, then the offset
    // of its name and the four-byte offset of its value.
    const uint32_t recordSize = 2 * m_idBytes + m_nameBytes + sizeof(uint32_t);
    const uint32_t len = m_nodesLength / recordSize;

    auto createNode = [&](uint32_t id) {
        const uint8_t* record = m_nodeData + id * recordSize + 2 * m_idBytes;
        auto node = new MetadataTreeNode;
        node->name = ReadName(ReadNameOffset(record));
        memcpy(&node->offsetValue, record + m_nameBytes, sizeof(node->offsetValue));
        return node;
    };

    MetadataTreeNode* emptyNode = nullptr;
    m_v.assign(len, emptyNode);

    for (uint32_t i = 0; i < len; i++) {
        MetadataTreeNode* node = m_v[i];
        if (nullptr == node) {
            node = createNode(i);
            m_v[i] = node;
        }

        uint32_t childId = ReadId(m_nodeData + i * recordSize);

        if (childId != i) {
            node->children = new vector<MetadataTreeNode*>;
            while (true) {
                // node (and its next siblings) already visited, so we don't need to visit it again
                if (childId >= len || m_v[childId] != emptyNode) {
                    __android_log_print(ANDROID_LOG_ERROR, "TNS.error", "Consistency error in metadata. A child should never have been visited before its parent. Parent: %s. Child metadata id: %u", node->name.c_str(), childId);
                    break;
                }

                MetadataTreeNode* childNode = createNode(childId);
                childNode->parent = node;

                node->children->push_back(childNode);

                m_v[childId] = childNode;

                uint32_t siblingId = ReadId(m_nodeData + childId * recordSize + m_idBytes);
                if (siblingId == childId) {
                    break;
                }

                childId = siblingId;
            }
        }
    }

    return GetNodeById(0);
}

MetadataTreeNode *MetadataReader::GetNodeById(uint32_t nodeId) {
    StateLock lock(m_stateMutex);

    bool isRuntimeNode = nodeId >= RUNTIME_NODE_ID_BASE;
    const auto &nodes = isRuntimeNode ? m_runtimeNodes : m_v;
    size_t index = isRuntimeNode ? nodeId - RUNTIME_NODE_ID_BASE : nodeId;

    if (index >= nodes.size()) {
        __android_log_print(ANDROID_LOG_ERROR, "TNS.error",
                            "Metadata node id %u is out of range (%zu static nodes, %zu runtime nodes). The metadata is inconsistent.",
                            nodeId, m_v.size(), m_runtimeNodes.size());
        return nullptr;
    }

    return nodes[index];
}


string MetadataReader::ReadTypeName(MetadataTreeNode *treeNode) {
    // Guards the whole family: the node id overload and ReadTypeNameInternal's
    // array-element forward both reach here with a GetNodeById result.
    if (treeNode == nullptr) {
        throw NativeScriptException("Cannot read a type name: the metadata refers to a node that does not exist.");
    }

    StateLock lock(m_stateMutex);

    string name;

    auto itFound = m_typeNameCache.find(treeNode);

    if (itFound != m_typeNameCache.end()) {
        name = itFound->second;
    } else {
        name = ReadTypeNameInternal(treeNode);

        m_typeNameCache.emplace(treeNode, name);
    }

    return name;
}

string MetadataReader::ReadTypeNameInternal(MetadataTreeNode *treeNode) {
    string name;

    uint8_t prevNodeType;

    while (treeNode->parent != nullptr) {
        int curNodeType = GetNodeType(treeNode);

        bool isArrayElement = treeNode->offsetValue > ARRAY_OFFSET;

        if (isArrayElement) {
            uint32_t forwardNodeId = treeNode->offsetValue - ARRAY_OFFSET;
            MetadataTreeNode * forwardNode = GetNodeById(forwardNodeId);
            name = ReadTypeName(forwardNode);
            uint8_t forwardNodeType = GetNodeType(forwardNode);
            if (IsNodeTypeInterface(forwardNodeType) || IsNodeTypeClass(forwardNodeType)) {
                name = "L" + name + ";";
            }
        } else {
            if (!name.empty()) {
                if (!IsNodeTypeArray(curNodeType)) {
                    if ((IsNodeTypeClass(prevNodeType) || IsNodeTypeInterface(prevNodeType))
                        && (IsNodeTypeClass(curNodeType) || IsNodeTypeInterface(curNodeType))) {
                        name = "$" + name;
                    } else {
                        name = "/" + name;
                    }
                }
            }

            name = treeNode->name + name;

            prevNodeType = curNodeType;
        }

        treeNode = treeNode->parent;
    }

    return name;
}

uint8_t *MetadataReader::GetValueData() const {
    return m_valueData;
}

uint32_t MetadataReader::GetNodeId(MetadataTreeNode *treeNode) {
    StateLock lock(m_stateMutex);

    auto itStatic = find(m_v.begin(), m_v.end(), treeNode);
    if (itStatic != m_v.end()) {
        return static_cast<uint32_t>(itStatic - m_v.begin());
    }

    auto itRuntime = find(m_runtimeNodes.begin(), m_runtimeNodes.end(), treeNode);
    NS_DCHECK(itRuntime != m_runtimeNodes.end());

    return RUNTIME_NODE_ID_BASE + static_cast<uint32_t>(itRuntime - m_runtimeNodes.begin());
}

MetadataTreeNode *MetadataReader::GetRoot() const {
    return m_root;
}

MetadataTreeNode *MetadataReader::FindStaticTreeNodeByName(const string &className) const {
    vector<string> names;
    Util::SplitString(className, "/$", names);

    MetadataTreeNode *treeNode = m_root;
    for (const auto &name: names) {
        if (treeNode == nullptr || treeNode->children == nullptr) {
            return nullptr;
        }
        MetadataTreeNode *found = nullptr;
        for (auto child: *treeNode->children) {
            if (child->name == name) {
                found = child;
                break;
            }
        }
        treeNode = found;
    }

    return names.empty() ? nullptr : treeNode;
}

uint8_t MetadataReader::GetNodeType(MetadataTreeNode *treeNode) {
    uint8_t memoized = treeNode->type.load(std::memory_order_relaxed);
    if (memoized != MetadataTreeNode::INVALID_TYPE) {
        return memoized;
    }

    StateLock lock(m_stateMutex);

    if (treeNode->type.load(std::memory_order_relaxed) == MetadataTreeNode::INVALID_TYPE) {
        uint8_t nodeType;

        uint32_t offsetValue = treeNode->offsetValue;

        if (offsetValue == 0) {
            nodeType = MetadataTreeNode::PACKAGE;
        } else if ((0 < offsetValue) && (offsetValue < ARRAY_OFFSET)) {
            nodeType = *(m_valueData + offsetValue);
        } else if (offsetValue == ARRAY_OFFSET) {
            nodeType = MetadataTreeNode::ARRAY;
        } else {
            uint32_t nodeId = offsetValue - ARRAY_OFFSET;
            MetadataTreeNode * arrElemNode = GetNodeById(nodeId);
            if (arrElemNode == nullptr) {
                throw NativeScriptException("Cannot resolve an array element type: the metadata refers to a node that does not exist.");
            }
            nodeType = GetNodeType(arrElemNode);
        }

        treeNode->type.store(nodeType, std::memory_order_relaxed);
    }

    return treeNode->type.load(std::memory_order_relaxed);
}

template<typename Predicate>
MetadataTreeNode *MetadataReader::FindChildIf(MetadataTreeNode *parent, Predicate matches) {
    if (parent->children != nullptr) {
        for (auto child: *parent->children) {
            if (matches(child)) {
                return child;
            }
        }
    }

    auto itFound = m_runtimeChildren.find(parent);
    if (itFound != m_runtimeChildren.end()) {
        for (auto child: itFound->second) {
            if (matches(child)) {
                return child;
            }
        }
    }

    return nullptr;
}

MetadataTreeNode *MetadataReader::FindChild(MetadataTreeNode *parent, const string &name) {
    return FindChildIf(parent, [&name](MetadataTreeNode *child) {
        return child->name == name;
    });
}

void MetadataReader::AddRuntimeChild(MetadataTreeNode *parent, MetadataTreeNode *child) {
    child->parent = parent;

    m_runtimeNodes.push_back(child);
    m_runtimeChildren[parent].push_back(child);
}

MetadataTreeNode *MetadataReader::GetOrCreateTreeNodeByName(const string &className) {
    StateLock lock(m_stateMutex);

    MetadataTreeNode * treeNode = GetRoot();

    int arrayIdx = -1;
    string arrayName = "[";

    while (className[++arrayIdx] == '[') {
        MetadataTreeNode * child = FindChild(treeNode, arrayName);

        if (child == nullptr) {
            child = new MetadataTreeNode;
            child->name = "[";
            child->offsetValue = ARRAY_OFFSET;

            AddRuntimeChild(treeNode, child);
        }

        treeNode = child;
    }

    string cn = className.substr(arrayIdx);

    if (arrayIdx > 0) {
        char last = *cn.rbegin();
        if (last == ';') {
            cn = cn.substr(1, cn.length() - 2);
        }
    }

    vector<string> names;
    Util::SplitString(cn, "/$", names);

    if (arrayIdx > 0) {
        MetadataTreeNode * forwardedNode = GetOrCreateTreeNodeByName(cn);

        uint32_t forwardedNodeId = GetNodeId(forwardedNode);
        MetadataTreeNode * forwardNode = FindChildIf(treeNode, [&](MetadataTreeNode *childNode) {
            uint32_t childNodeId = (childNode->offsetValue >= ARRAY_OFFSET)
                                   ? (childNode->offsetValue - ARRAY_OFFSET)
                                   :
                                   GetNodeId(childNode);

            return childNodeId == forwardedNodeId;
        });

        if (forwardNode == nullptr) {
            forwardNode = new MetadataTreeNode;
            forwardNode->offsetValue = forwardedNodeId + ARRAY_OFFSET;

            AddRuntimeChild(treeNode, forwardNode);
        }

        return forwardNode;
    }

    int curIdx = 0;
    for (auto it = names.begin(); it != names.end(); ++it) {
        MetadataTreeNode * child = FindChild(treeNode, *it);

        if (child == nullptr) {
            vector<string> api;
            {
                // Resolving an unknown type runs Class.forName and, for an
                // .extend()ed class, dex generation; see StateMutex.
                StateUnlock unlocked(m_stateMutex);
                api = m_getTypeMetadataCallback(cn, curIdx);
            }

            // Another thread may have resolved the same type meanwhile.
            child = FindChild(treeNode, *it);
            if (child != nullptr) {
                treeNode = child;
                ++curIdx;
                continue;
            }

            for (const auto &part: api) {
                const string &childName = *it++;

                string line;
                string kind;
                string name;
                stringstream s(part);

                getline(s, line);
                stringstream typeLine(line);
                typeLine >> kind >> name;
                auto cKind = kind[0];

                // package, class, interface
                NS_DCHECK((cKind == 'P') || (cKind == 'C') || (cKind == 'I'));

                bool isType = (cKind == 'C') || (cKind == 'I');
                uint8_t childType = MetadataTreeNode::PACKAGE;
                MetadataTreeNode *baseClass = nullptr;

                if (isType) {
                    childType = (cKind == 'C') ? MetadataTreeNode::CLASS
                                               : MetadataTreeNode::INTERFACE;
                    if (name == "S") {
                        childType |= MetadataTreeNode::STATIC;
                    }

                    getline(s, line);
                    stringstream baseClassLine(line);
                    // Empty for an interface, which has no superclass.
                    string baseClassName;
                    baseClassLine >> kind >> baseClassName;
                    cKind = kind[0];

                    NS_DCHECK(cKind == 'B');
                    if (!baseClassName.empty()) {
                        baseClass = GetOrCreateTreeNodeByName(baseClassName);
                    }
                }

                // Resolving the base class can release the state lock, so another
                // thread may have published this node since the lookup above. Nothing
                // from this lookup to AddRuntimeChild releases the lock.
                child = FindChild(treeNode, childName);

                if (child == nullptr) {
                    child = new MetadataTreeNode;
                    child->name = childName;
                    child->type.store(childType, std::memory_order_relaxed);

                    if (isType) {
                        child->metadata = new string(part);
                        child->runtimeBaseClass = baseClass;

                        child->offsetValue = RUNTIME_TYPE_VALUE_OFFSET;
                    }

                    AddRuntimeChild(treeNode, child);
                }

                treeNode = child;
            }

            return treeNode;
        } else {
            treeNode = child;
        }
        ++curIdx;
    }

    return treeNode;
}

MetadataTreeNode *MetadataReader::GetBaseClassNode(MetadataTreeNode *treeNode) {
    StateLock lock(m_stateMutex);

    MetadataTreeNode * baseClassNode = nullptr;

    if (treeNode != nullptr && treeNode->metadata != nullptr) {
        baseClassNode = treeNode->runtimeBaseClass;
    } else if (treeNode != nullptr) {
        uint32_t baseClassNodeId = ReadId(m_valueData + treeNode->offsetValue + sizeof(uint8_t));

        // Null for an id the metadata got wrong, which GetNodeById logs; every
        // caller already treats a missing base class as "no base class".
        baseClassNode = GetNodeById(baseClassNodeId);
    }

    return baseClassNode;
}

