//
// Created by Vladimir Mutafov on 16.11.22.
//

#ifndef TEST_APP_JSTOJAVAARRAYSCONVERTER_H
#define TEST_APP_JSTOJAVAARRAYSCONVERTER_H

#include "v8.h"
#include "../JNIArgRefsState.h"

namespace tns {
    bool ConvertJavaScriptArray(
            v8::Isolate *isolate,
            const v8::Local<v8::Value> &jsValue,
            int index,
            std::vector<std::string> &tokens,
            JNIArgRefsState &jniArgRefsState
    );
}

#endif //TEST_APP_JSTOJAVAARRAYSCONVERTER_H
