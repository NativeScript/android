#include "MetadataTreeNode.h"

using namespace std;
using namespace tns;

MetadataTreeNode::MetadataTreeNode()
    :
    children(nullptr), parent(nullptr), metadata(nullptr), offsetValue(0), type(INVALID_TYPE) {
}

MetadataTreeNode* MetadataTreeNode::GetChild(const string& name) {
    MetadataTreeNode* child = nullptr;

    if (children != nullptr) {
        auto itEnd = children->end();
        auto itFound = find_if(children->begin(), itEnd, [&name] (MetadataTreeNode *x) {
            return x->name == name;
        });
        if (itFound != itEnd) {
            child = *itFound;
        }
    }

    return child;
}

