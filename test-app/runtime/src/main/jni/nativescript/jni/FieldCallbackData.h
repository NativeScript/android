#ifndef FIELDCALLBACKDATA_H_
#define FIELDCALLBACKDATA_H_

#include "jni.h"
#include "MetadataEntry.h"

namespace tns {
struct FieldCallbackData {
    FieldCallbackData(const MetadataEntry& metadata)
        :
        fid(nullptr), clazz(nullptr) {
        name = metadata.name;
        signature = metadata.sig;
        declaringType = metadata.declaringType;
        isStatic = metadata.isStatic;
        isFinal = metadata.isFinal;
    }

    std::string name;
    std::string signature;
    std::string declaringType;
    bool isStatic;
    bool isFinal;
    jfieldID fid;
    jclass clazz;
};

}

#endif /* FIELDCALLBACKDATA_H_ */
