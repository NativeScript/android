#ifndef JSARGCONVERTER_H_
#define JSARGCONVERTER_H_

#include <string>
#include <vector>
#include "JEnv.h"
#include "v8.h"
#include "MetadataEntry.h"

namespace tns {

    enum class BufferCastType {
        Byte,
        Short,
        Int,
        Long,
        Float,
        Double
    };

    class JsArgConverter {
    public:

        JsArgConverter(const v8::Local<v8::Object>& caller, const v8::FunctionCallbackInfo<v8::Value>& args, const std::string& methodSignature, MetadataEntry* entry);

        JsArgConverter(const v8::FunctionCallbackInfo<v8::Value>& args, bool hasImplementationObject, const std::string& methodSignature, MetadataEntry* entry);

        JsArgConverter(const v8::FunctionCallbackInfo<v8::Value>& args, const std::string& methodSignature);

        ~JsArgConverter();

        jvalue* ToArgs();

        int Length() const;

        bool IsValid() const;

        struct Error;

        Error GetError() const;

        struct Error {
            Error() :
                    index(-1), msg(std::string()) {
            }

            int index;
            std::string msg;
        };

        static BufferCastType GetCastType(const v8::Local<v8::ArrayBufferView>& view);

    private:

        bool ConvertArg(const v8::Local<v8::Value>& arg, int index);

        bool ConvertJavaScriptArray(const v8::Local<v8::Array>& jsArr, int index);

        bool ConvertJavaScriptNumber(const v8::Local<v8::Value>& jsValue, int index);

        bool ConvertJavaScriptBoolean(const v8::Local<v8::Value>& jsValue, int index);

        bool ConvertJavaScriptString(const v8::Local<v8::Value>& jsValue, int index);

        void SetConvertedObject(int index, jobject obj, bool isGlobal = false);

        template<typename T>
        bool ConvertFromCastFunctionObject(T value, int index);

        v8::Isolate* m_isolate;

        int m_argsLen;

        bool m_isValid;

        jvalue m_args[255];
        int m_args_refs[255];
        int m_args_refs_size = 0;

        std::string m_methodSignature;

        std::vector<std::string> m_tokens;

        Error m_error;
    };
}

#endif /* JSARGCONVERTER_H_ */
