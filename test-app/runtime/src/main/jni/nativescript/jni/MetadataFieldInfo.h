#ifndef METADATAFIELDINFO_H_
#define METADATAFIELDINFO_H_

namespace tns {
struct __attribute__ ((__packed__)) FieldInfo {
    FieldInfo()
    :
nameOffset(0), nodeId(0), finalModifier(0) {
}

uint32_t nameOffset;
uint16_t nodeId;
uint8_t finalModifier;
};

struct __attribute__ ((__packed__)) StaticFieldInfo: FieldInfo {
    StaticFieldInfo()
    :
FieldInfo(), declaringType(0) {
}

uint16_t declaringType;
};
}

#endif /* METADATAFIELDINFO_H_ */
