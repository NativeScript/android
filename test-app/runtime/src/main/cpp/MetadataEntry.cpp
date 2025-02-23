#include  "MetadataNode.h"
#include "MetadataEntry.h"
#include "MetadataMethodInfo.h"
#include "MetadataReader.h"

using namespace tns;

MetadataEntry::MetadataEntry(MetadataTreeNode *m_treeNode, NodeType nodeType) :
        treeNode(m_treeNode), type(nodeType), isExtensionFunction(false), isStatic(false),
        isTypeMember(false), memberId(nullptr), clazz(nullptr), mi(nullptr),fi(nullptr), sfi(nullptr),
        retType(MethodReturnType::Unknown),
        paramCount(-1), isFinal(false), isResolved(false), retTypeParsed(false),
        isFinalSet(false), isResolvedSet(false) {}

std::string &MetadataEntry::getName() {
    if (!name.empty()) return name;

    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::Field) {
        name = reader->ReadName(fi->nameOffset);
    } else if (type == NodeType::StaticField) {
        name = reader->ReadName(sfi->nameOffset);
    } else if (type == NodeType::Method) {
        name = mi.GetName();
    }

    return name;
}

std::string &MetadataEntry::getSig() {
    if (!sig.empty()) return sig;

    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::Field) {
        sig = reader->ReadTypeName(fi->nodeId);
    } else if (type == NodeType::StaticField) {
        sig = reader->ReadTypeName(sfi->nodeId);
    } else if (type == NodeType::Method) {
        uint8_t sigLength = mi.GetSignatureLength();
        if (sigLength > 0)
            sig = mi.GetSignature();

    }

    return sig;
}

std::string &MetadataEntry::getReturnType() {
    if (!returnType.empty()) return returnType;

    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::Method) {
        if (mi.GetSignatureLength() > 0) {
            returnType = MetadataReader::ParseReturnType(this->getSig());
        }
    } else {
        return returnType;
    }

    return returnType;
}

MethodReturnType MetadataEntry::getRetType() {
    if (retTypeParsed) return retType;
    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::Method && !this->getReturnType().empty()) {
        retType = MetadataReader::GetReturnType(this->returnType);
    }

    retTypeParsed = true;

    return retType;
}

std::string &MetadataEntry::getDeclaringType() {
    if (!declaringType.empty()) return declaringType;

    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::StaticField) {
        declaringType = reader->ReadTypeName(sfi->declaringType);
    } else if (type == NodeType::Method && isStatic) {
        declaringType = mi.GetDeclaringType();
    }

    return declaringType;
}

int MetadataEntry::getParamCount() {
    if (paramCount != -1) return paramCount;

    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::Method) {
        auto sigLength = mi.GetSignatureLength();
        if (sigLength > 0) {
            paramCount = sigLength - 1;
        } else {
            paramCount = 0;
        }
    }

    return paramCount;
}

bool MetadataEntry::getIsFinal() {
    if (isFinalSet) return isFinal;

    if (type == NodeType::Field) {
        isFinal = fi->finalModifier == MetadataTreeNode::FINAL;
    } else if (type == NodeType::StaticField) {
        isFinal = sfi->finalModifier == MetadataTreeNode::FINAL;
    }

    isFinalSet = true;

    return isFinal;
}

bool MetadataEntry::getIsResolved() {
    if (isResolvedSet) return isResolved;

    auto reader = MetadataNode::getMetadataReader();
    if (type == NodeType::Method) {
        isResolved = mi.CheckIsResolved() == 1;
    }

    isResolvedSet = true;

    return isResolved;
}
