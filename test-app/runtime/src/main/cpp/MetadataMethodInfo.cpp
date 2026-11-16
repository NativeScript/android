#include "MetadataMethodInfo.h"
#include "MetadataNode.h"


using namespace tns;

std::string MethodInfo::GetName() {
    auto m_reader = MetadataNode::getMetadataReader();
    string methodName = m_reader->ReadName(m_reader->ReadNameOffset(m_record));
    return methodName;
}

uint8_t MethodInfo::CheckIsResolved() {
    return m_record[MetadataNode::getMetadataReader()->NameBytes()] & 1;
}

const uint8_t *MethodInfo::Signature() {
    // The reference is the signature's offset in the signature region, shifted
    // left by one to leave the low bit for the resolved flag.
    auto m_reader = MetadataNode::getMetadataReader();
    uint32_t reference = m_reader->ReadSignatureReference(m_record + m_reader->NameBytes());

    return m_reader->GetSignatureData() + (reference >> 1);
}

uint16_t MethodInfo::GetSignatureLength() {
    return Signature()[0] + 1;
}

std::string MethodInfo::GetSignature() { //use nodeId's to read the whole signature
    auto m_reader = MetadataNode::getMetadataReader();
    const uint8_t *signatureRecord = Signature();
    int signatureLength = signatureRecord[0] + 1;
    const uint8_t *nodeIds = signatureRecord + 1;

    string signature = "(";
    string ret;
    for (int i = 0; i < signatureLength; i++) {
        uint32_t nodeId = m_reader->ReadId(nodeIds + i * m_reader->IdBytes());
        string curArgTypeName = m_reader->ReadTypeName(nodeId);
        MetadataTreeNode* node = m_reader->GetNodeById(nodeId);

        uint8_t nodeType = m_reader->GetNodeType(node);
        bool isRefType = m_reader->IsNodeTypeClass(nodeType) || m_reader->IsNodeTypeInterface(nodeType);
        if (i == 0) {
            if ((curArgTypeName[0] != '[') && isRefType) {
                ret.append("L");
            }
            ret.append(curArgTypeName);
            if ((curArgTypeName[0] != '[') && isRefType) {
                ret.append(";");
            }
        } else {
            if ((curArgTypeName[0] != '[') && isRefType) {
                signature.append("L");
            }
            signature.append(curArgTypeName);
            if ((curArgTypeName[0] != '[') && isRefType) {
                signature.append(";");
            }
        }
    }
    if (ret.empty()) {
        ret = "V";
    }
    signature += ")" + ret;

    return signature;
}

std::string MethodInfo::GetDeclaringType() {
    auto m_reader = MetadataNode::getMetadataReader();

    return m_reader->ReadTypeName(
            m_reader->ReadId(m_record + m_reader->NameBytes() + m_reader->SignatureBytes()));
}

int MethodInfo::GetSizeOfReadMethodInfo() {
    auto m_reader = MetadataNode::getMetadataReader();

    return m_reader->NameBytes() + m_reader->SignatureBytes() + (isStatic ? m_reader->IdBytes() : 0);
}
