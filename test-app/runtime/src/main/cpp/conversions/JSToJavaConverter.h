#ifndef TEST_APP_JSTOJAVACONVERTER_H
#define TEST_APP_JSTOJAVACONVERTER_H

#include <string>
#include <vector>
#include "JEnv.h"
#include "v8.h"
#include "MetadataEntry.h"
#include "array"
#include "ConversionConsts.h"
#include "JNIArgRefsState.h"

namespace tns {

    class JSToJavaConverter {
    public:

        JSToJavaConverter(
                const v8::FunctionCallbackInfo<v8::Value> &args,
                const std::string &methodSignature,
                MetadataEntry *entry);

        JSToJavaConverter(
                const v8::FunctionCallbackInfo<v8::Value> &args,
                const std::string &methodSignature,
                MetadataEntry *entry,
                const v8::Local<v8::Object> kotlinExtensionFunctionThis);

        JSToJavaConverter(
                const v8::FunctionCallbackInfo<v8::Value> &args,
                const std::string &methodSignature);

        ~JSToJavaConverter();

        jvalue *ToArgs();

        int Length() const;

    private:

        bool ConvertArg(const v8::Local<v8::Value> &arg, int index);

        v8::Isolate *m_isolate;
        std::string m_methodSignature;
        std::vector<std::string> *m_tokens;
        int m_argsLen;
        JNIArgRefsState m_jniArgRefsState;
    };
}


#endif //TEST_APP_JSTOJAVACONVERTER_H
