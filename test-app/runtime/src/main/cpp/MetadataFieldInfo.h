#ifndef METADATAFIELDINFO_H_
#define METADATAFIELDINFO_H_

#include <cstdint>

namespace tns {
/*
 * The flags of a field record in the metadata's value stream. The record is the
 * offset of the field's name, the id of its type and these flags. MetadataReader
 * reads it, because how wide the first two are depends on the metadata.
 */
struct FieldInfo {
    static const uint8_t FINAL = 1;
    // On a static field: the id of its declaring type follows. A static field
    // without it is declared by the class whose value lists it.
    static const uint8_t HAS_DECLARING_TYPE = 1 << 1;
};
}

#endif /* METADATAFIELDINFO_H_ */
