#include  "MetadataNode.h"
#include "MetadataEntry.h"
#include "MetadataMethodInfo.h"
#include "MetadataReader.h"

using namespace tns;

MetadataEntry::MetadataEntry(MetadataTreeNode *m_treeNode, NodeType nodeType) :
        treeNode(m_treeNode), type(nodeType), isExtensionFunction(false), isStatic(false),
        isTypeMember(false), memberId(nullptr), clazz(nullptr), mi(nullptr), field(nullptr),
        retType(MethodReturnType::Unknown),
        paramCount(-1), isFinal(false), isResolved(false), retTypeParsed(false),
        isFinalSet(false), isResolvedSet(false) {}

std::string &MetadataEntry::readName() {
    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::Field || type == NodeType::StaticField) {
        name = reader->ReadName(reader->FieldNameOffset(field));
    } else if (type == NodeType::Method) {
        name = mi.GetName();
    }

    return name;
}

std::string &MetadataEntry::readSig() {
    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::Field || type == NodeType::StaticField) {
        sig = reader->ReadTypeName(reader->FieldTypeId(field));
    } else if (type == NodeType::Method) {
        auto sigLength = mi.GetSignatureLength();
        if (sigLength > 0)
            sig = mi.GetSignature();

    }

    return sig;
}

std::string &MetadataEntry::readReturnType() {
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

MethodReturnType MetadataEntry::readRetType() {
    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::Method && !this->getReturnType().empty()) {
        retType = MetadataReader::GetReturnType(this->returnType);
    }

    retTypeParsed = true;

    return retType;
}

std::string &MetadataEntry::readDeclaringType() {
    auto reader = MetadataNode::getMetadataReader();

    if (type == NodeType::StaticField) {
        declaringType = (reader->FieldFlags(field) & FieldInfo::HAS_DECLARING_TYPE)
                        ? reader->ReadTypeName(reader->FieldDeclaringTypeId(field))
                        : reader->ReadTypeName(treeNode);
    } else if (type == NodeType::Method && isStatic) {
        declaringType = mi.GetDeclaringType();
    }

    return declaringType;
}

int MetadataEntry::readParamCount() {
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

bool MetadataEntry::readIsFinal() {
    if (type == NodeType::Field || type == NodeType::StaticField) {
        auto reader = MetadataNode::getMetadataReader();
        isFinal = (reader->FieldFlags(field) & FieldInfo::FINAL) != 0;
    }

    isFinalSet = true;

    return isFinal;
}

bool MetadataEntry::readIsResolved() {
    auto reader = MetadataNode::getMetadataReader();
    if (type == NodeType::Method) {
        isResolved = mi.CheckIsResolved() == 1;
    }

    isResolvedSet = true;

    return isResolved;
}
