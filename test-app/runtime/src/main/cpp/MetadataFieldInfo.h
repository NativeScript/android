#ifndef METADATAFIELDINFO_H_
#define METADATAFIELDINFO_H_

#include <cstdint>

namespace tns {
struct __attribute__ ((__packed__)) FieldInfo {
    static const uint8_t FINAL = 1;
    // On a static field: the id of its declaring type follows. A static field
    // without it is declared by the class whose value lists it.
    static const uint8_t HAS_DECLARING_TYPE = 1 << 1;

    FieldInfo()
    :
nameOffset(0), nodeId(0), flags(0) {
}

uint32_t nameOffset;
uint16_t nodeId;
uint8_t flags;
};

struct __attribute__ ((__packed__)) StaticFieldInfo: FieldInfo {
    StaticFieldInfo()
    :
FieldInfo(), declaringType(0) {
}

// Part of the record only when HAS_DECLARING_TYPE is set.
uint16_t declaringType;
};
}

#endif /* METADATAFIELDINFO_H_ */
