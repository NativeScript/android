#include "MetadataMethodInfo.h"
#include "MetadataNode.h"
#include <cstring>


using namespace tns;

namespace {
    // The value stream is packed, so nothing in it is aligned.
    inline uint16_t ReadUInt16(const uint8_t *data) {
        uint16_t value;
        memcpy(&value, data, sizeof(value));
        return value;
    }

    inline uint32_t ReadUInt32(const uint8_t *data) {
        uint32_t value;
        memcpy(&value, data, sizeof(value));
        return value;
    }
}

std::string MethodInfo::GetName() {
    string methodName = MetadataNode::getMetadataReader()->ReadName(ReadUInt32(m_record));
    return methodName;
}

uint8_t MethodInfo::CheckIsResolved() {
    return m_record[sizeof(uint32_t)] & 1;
}

const uint8_t *MethodInfo::Signature() {
    // Read a byte at a time: the record can be the last thing in the stream,
    // and a four-byte load would run past it.
    const uint8_t *reference = m_record + sizeof(uint32_t);
    uint32_t offset = (reference[0] | (reference[1] << 8) | (reference[2] << 16)) >> 1;

    return MetadataNode::getMetadataReader()->GetSignatureData() + offset;
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
        uint16_t nodeId = ReadUInt16(nodeIds + i * sizeof(uint16_t));
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

    return m_reader->ReadTypeName(ReadUInt16(m_record + RECORD_SIZE));
}

int MethodInfo::GetSizeOfReadMethodInfo() {
    return RECORD_SIZE + (isStatic ? sizeof(uint16_t) : 0);
}
