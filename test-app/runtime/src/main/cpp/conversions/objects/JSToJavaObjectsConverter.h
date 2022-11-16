//
// Created by Vladimir Mutafov on 16.11.22.
//

#ifndef TEST_APP_JSTOJAVAOBJECTSCONVERTER_H
#define TEST_APP_JSTOJAVAOBJECTSCONVERTER_H

#include <jni.h>
#include "v8.h"
#include "string"
#include "vector"
#include "array"
#include "../ConversionConsts.h"
#include "../JNIArgRefsState.h"

namespace tns {
    bool ConvertJavaScriptObject(
            v8::Isolate *isolate,
            const v8::Local<v8::Value> &jsValue,
            int index,
            std::string &methodSignature,
            std::vector<std::string> &tokens,
            JNIArgRefsState &jniArgRefsState
    );

    template<typename T>
    bool ConvertFromCastFunctionObject(
            T value,
            int index,
            std::string &methodSignature,
            std::vector<std::string> &tokens,
            JNIArgRefsState &jniArgRefsState
    );
}

#endif //TEST_APP_JSTOJAVAOBJECTSCONVERTER_H
