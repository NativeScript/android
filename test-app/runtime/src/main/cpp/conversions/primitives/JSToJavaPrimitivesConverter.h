//
// Created by Vladimir Mutafov on 16.11.22.
//

#ifndef TEST_APP_JSTOJAVAPRIMITIVESCONVERTER_H
#define TEST_APP_JSTOJAVAPRIMITIVESCONVERTER_H

#include "v8.h"
#include "../JNIArgRefsState.h"

namespace tns {
    bool ConvertJavaScriptBoolean(
            v8::Isolate *isolate,
            const v8::Local<v8::Value> &jsValue,
            int index,
            std::vector<std::string> &tokens,
            JNIArgRefsState &jniArgRefsState
    );

    bool ConvertJavaScriptNumber(
            v8::Isolate *isolate,
            const v8::Local<v8::Value> &jsValue,
            int index,
            std::vector<std::string> &tokens,
            JNIArgRefsState &jniArgRefsState
    );

    bool ConvertJavaScriptString(
            const v8::Local<v8::Value> &jsValue,
            int index,
            JNIArgRefsState &jniArgRefsState
    );
}

#endif //TEST_APP_JSTOJAVAPRIMITIVESCONVERTER_H
