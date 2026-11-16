#include "MetadataTreeNode.h"

using namespace std;
using namespace tns;

MetadataTreeNode::MetadataTreeNode()
    :
    parent(nullptr), children(nullptr), metadata(nullptr), runtimeBaseClass(nullptr), offsetValue(0), type(INVALID_TYPE) {
}

static_assert(sizeof(void*) != 8 || sizeof(MetadataTreeNode) == 64,
              "MetadataTreeNode grew; see the note on its member order");

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

