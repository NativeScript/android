#ifndef METHODINFOSMARTPOINTER_H_
#define METHODINFOSMARTPOINTER_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace tns {
    class MethodInfo {
    public:

        MethodInfo(uint8_t *pValue)
                : isStatic(false),  m_pData(pValue), m_pStartData(pValue), m_signatureLength(0),
                  sizeMeasured(false), nameOffset(0), resolvedData(0),
                  declaringNodeId(0){
        }
        
        MethodInfo(const MethodInfo& other) = default;

        MethodInfo& operator=(const MethodInfo& other) {
            if (this != &other) {
                isStatic = other.isStatic;
                m_pData = other.m_pData;
                m_pStartData = other.m_pStartData;
                m_signatureLength = other.m_signatureLength;
                sizeMeasured = other.sizeMeasured;
                nameOffset = other.nameOffset;
                resolvedData = other.resolvedData;
                declaringNodeId = other.declaringNodeId;
                nodeIds = other.nodeIds;
            }
            return *this;
        }

        std::string GetName();

        uint8_t CheckIsResolved();

        uint16_t GetSignatureLength();

        std::string GetSignature();

        std::string GetDeclaringType(); //used only for static methods

        int GetSizeOfReadMethodInfo();

        bool isStatic;

    private:
        uint8_t *m_pData; //where we currently read
        uint8_t *m_pStartData;  // pointer to the beginning
        uint16_t m_signatureLength;
        bool sizeMeasured;

        uint32_t nameOffset;
        uint8_t resolvedData;
        uint16_t declaringNodeId;
        std::vector<uint16_t> nodeIds;


    };
}

#endif /* METHODINFOSMARTPOINTER_H_ */
