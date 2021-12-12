#ifndef METHODINFOSMARTPOINTER_H_
#define METHODINFOSMARTPOINTER_H_

#include "MetadataReader.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace tns {
class MethodInfo {
    public:
        MethodInfo(uint8_t* pValue, MetadataReader* reader)
            : m_pData(pValue), m_pStartData(pValue), m_reader(reader), m_signatureLength(0) {
        }

        std::string GetName();
        uint8_t CheckIsResolved();
        uint16_t GetSignatureLength();
        std::string GetSignature();
        std::string GetDeclaringType(); //used only for static methods

        int GetSizeOfReadMethodInfo();

    private:
        uint8_t* m_pData; //where we currently read
        uint8_t* m_pStartData;  // pointer to the beginning
        uint16_t m_signatureLength;
        MetadataReader* m_reader;
};
}

#endif /* METHODINFOSMARTPOINTER_H_ */
