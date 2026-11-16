#ifndef TREENODE_H_
#define TREENODE_H_

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

namespace tns {
struct MetadataTreeNode {
    MetadataTreeNode();

    MetadataTreeNode* GetChild(const std::string& name);

    // There is one of these per metadata node, tens of thousands in a large app, so
    // the two narrow members sit together at the end rather than each leaving a hole.
    std::string name;
    MetadataTreeNode* parent;
    // Grows when a type under this node is resolved at run time, on any runtime's
    // thread. Outside MetadataReader, read it through MetadataReader::GetChildren
    // or MetadataReader::GetChild.
    std::vector<MetadataTreeNode*>* children;
    // Set for a type resolved at run time, null for one from the static metadata.
    std::string* metadata;
    // Base class of a type resolved at run time. That base can itself be resolved at
    // run time, and the 16-bit base class id of a value record cannot address those.
    MetadataTreeNode* runtimeBaseClass;
    uint32_t offsetValue;
    // Write-once memo: INVALID_TYPE until GetNodeType derives it from the
    // immutable value data, so a reader that sees any other value can use it
    // without the reader's state lock.
    std::atomic<uint8_t> type;

    static const uint8_t PACKAGE = 0;
    static const uint8_t CLASS = 1 << 0;
    static const uint8_t INTERFACE = 1 << 1;
    static const uint8_t STATIC = 1 << 2;
    static const uint8_t ARRAY = 1 << 3;
    static const uint8_t PRIMITIVE = 1 << 4;

    static const uint8_t FINAL = 1;

    static const uint8_t PRIMITIVE_BYTE = 1 + PRIMITIVE;
    static const uint8_t PRIMITIVE_SHORT = 2 + PRIMITIVE;
    static const uint8_t PRIMITIVE_INT = 3 + PRIMITIVE;
    static const uint8_t PRIMITIVE_LONG = 4 + PRIMITIVE;
    static const uint8_t PRIMITIVE_FLOAT = 5 + PRIMITIVE;
    static const uint8_t PRIMITIVE_DOUBLE = 6 + PRIMITIVE;
    static const uint8_t PRIMITIVE_BOOL = 7 + PRIMITIVE;
    static const uint8_t PRIMITIVE_CHAR = 8 + PRIMITIVE;
    static const uint8_t INVALID_TYPE = 0xFF;
};

struct MetadataTreeNodeRawData {
    uint16_t firstChildId;
    uint16_t nextSiblingId;
    uint32_t offsetName;
    uint32_t offsetValue;
};
}

#endif /* TREENODE_H_ */
