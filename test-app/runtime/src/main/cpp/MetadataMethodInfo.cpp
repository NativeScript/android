#include "MetadataMethodInfo.h"
#include "MetadataNode.h"


using namespace tns;

std::string MethodInfo::GetName() {
    string methodName = MetadataNode::getMetadataReader()->ReadName(nameOffset);
    return methodName;
}

uint8_t MethodInfo::CheckIsResolved() {
    return resolvedData;
}

uint16_t MethodInfo::GetSignatureLength() {
    return m_signatureLength;
}

std::string MethodInfo::GetSignature() { //use nodeId's to read the whole signature
    auto m_reader = MetadataNode::getMetadataReader();
    string signature = "(";
    string ret;
    for (int i = 0; i < m_signatureLength; i++) {
        uint16_t nodeId = nodeIds[i];
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

    return m_reader->ReadTypeName(declaringNodeId);
}

int MethodInfo::GetSizeOfReadMethodInfo() {

    if (!sizeMeasured) {
        sizeMeasured = true;
        // name
        nameOffset = *reinterpret_cast<uint32_t*>(m_pData);
        m_pData += sizeof(uint32_t);
        // resolved data
        resolvedData = *reinterpret_cast<uint8_t*>(m_pData);
        m_pData += sizeof(uint8_t);
        // sig length
        m_signatureLength = *reinterpret_cast<uint16_t*>(m_pData);
        m_pData += sizeof(uint16_t);

        // signature
        if (m_signatureLength > 0) {
            uint16_t* nodeIdPtr = reinterpret_cast<uint16_t*>(m_pData);
            nodeIds.resize(m_signatureLength);
            for (int i = 0; i < m_signatureLength; i++) {
                nodeIds[i] = *nodeIdPtr++;
            }
            m_pData +=  m_signatureLength * sizeof(uint16_t);
        }

        // declaring type
        if (isStatic) {
            auto declaringTypePtr = reinterpret_cast<uint16_t*>(m_pData);
            declaringNodeId = *declaringTypePtr;
            m_pData += sizeof(uint16_t);
        }



    }

    return m_pData - m_pStartData;
}