#ifndef TEST_APP_CASTTYPES_H
#define TEST_APP_CASTTYPES_H

#include "v8.h"

namespace tns {

    enum class JavaObjectCastType {
        Byte,
        Short,
        Int,
        Long,
        Float,
        Double
    };

    tns::JavaObjectCastType GetCastType(const v8::Local <v8::ArrayBufferView> &view) {
        if (view->IsUint16Array() || view->IsInt16Array()) {
            return tns::JavaObjectCastType::Short;
        } else if (view->IsUint32Array() || view->IsInt32Array()) {
            return tns::JavaObjectCastType::Int;
        } else if (view->IsFloat32Array()) {
            return tns::JavaObjectCastType::Float;
        } else if (view->IsFloat64Array()) {
            return tns::JavaObjectCastType::Double;
        } else if (view->IsBigUint64Array() || view->IsBigInt64Array()) {
            return tns::JavaObjectCastType::Long;
        }
        return tns::JavaObjectCastType::Byte;
    }

}

#endif //TEST_APP_CASTTYPES_H
