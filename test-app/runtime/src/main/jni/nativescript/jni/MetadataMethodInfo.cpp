#include "MetadataMethodInfo.h"

using namespace tns;

std::string MethodInfo::GetName() {
    uint32_t nameOfffset = *reinterpret_cast<uint32_t*>(m_pData);
    string methodName = m_reader->ReadName(nameOfffset);
    m_pData += sizeof(uint32_t);

    return methodName;
}

uint8_t MethodInfo::CheckIsResolved() {
    auto resolvedData = *reinterpret_cast<uint8_t*>(m_pData);
    m_pData += sizeof(uint8_t);

    return resolvedData;
}

uint16_t MethodInfo::GetSignatureLength() {
    m_signatureLength = *reinterpret_cast<uint16_t*>(m_pData);
    m_pData += sizeof(uint16_t);

    return m_signatureLength;
}

std::string MethodInfo::GetSignature() { //use nodeId's to read the whole signature
    uint16_t* nodeIdPtr = reinterpret_cast<uint16_t*>(m_pData);
    string signature = "(";
    string ret;
    for (int i = 0; i < m_signatureLength; i++) {
        uint16_t nodeId = *nodeIdPtr++;
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

    int sizeofReadNodeIds = m_signatureLength * sizeof(uint16_t);
    m_pData += sizeofReadNodeIds;

    return signature;
}

std::string MethodInfo::GetDeclaringType() {
    uint16_t* declaringTypePtr = reinterpret_cast<uint16_t*>(m_pData);
    uint16_t nodeId = *declaringTypePtr;

    string declTypeName = m_reader->ReadTypeName(nodeId);

    m_pData += sizeof(uint16_t);

    return declTypeName;
}

int MethodInfo::GetSizeOfReadMethodInfo() {
    return m_pData - m_pStartData;
}
