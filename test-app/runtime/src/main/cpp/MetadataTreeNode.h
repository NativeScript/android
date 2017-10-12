#ifndef TREENODE_H_
#define TREENODE_H_

#include <string>
#include <vector>

namespace tns {
struct MetadataTreeNode {
    MetadataTreeNode();

    MetadataTreeNode* GetChild(const std::string& name);

    std::string name;
    MetadataTreeNode* parent;
    uint32_t offsetValue;
    std::vector<MetadataTreeNode*>* children;
    //
    std::string* metadata;
    uint8_t type;

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
