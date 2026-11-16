#ifndef METHODINFOSMARTPOINTER_H_
#define METHODINFOSMARTPOINTER_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace tns {
    /*
     * A view of one method record in the metadata's value stream. The record is
     * the offset of the method's name and a reference to its signature; a static
     * method or an extension function is followed by the id of its declaring
     * type. MetadataReader knows how wide each of those is. Nothing is copied
     * out of the record, and the signature itself is only read when the method
     * is first resolved.
     */
    class MethodInfo {
    public:
        MethodInfo(uint8_t *record)
                : isStatic(false), m_record(record) {
        }

        std::string GetName();

        uint8_t CheckIsResolved();

        // The return type and the parameter types.
        uint16_t GetSignatureLength();

        std::string GetSignature();

        std::string GetDeclaringType(); //used only for static methods

        int GetSizeOfReadMethodInfo();

        bool isStatic;

    private:
        // The signature's record in the signature region: a byte holding the
        // number of type ids less one, then the ids.
        const uint8_t *Signature();

        uint8_t *m_record;
    };
}

#endif /* METHODINFOSMARTPOINTER_H_ */
