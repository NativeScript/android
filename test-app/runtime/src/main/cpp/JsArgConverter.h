#ifndef JSARGCONVERTER_H_
#define JSARGCONVERTER_H_

#include <string>
#include <vector>
#include "JEnv.h"
#include "JniLocalRef.h"
#include "v8.h"
#include "MetadataEntry.h"

namespace tns {

    class ObjectManager;

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

        JsArgConverter(const JsArgConverter&) = delete;

        JsArgConverter& operator=(const JsArgConverter&) = delete;

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

        static bool IsArrayBufferLike(const v8::Local<v8::Object>& object);

        // A direct Java buffer over the object's memory, linked to the object so that the
        // object, and with it the memory, lives as long as the buffer.
        static JniLocalRef LinkNewJavaBuffer(ObjectManager* objectManager, const v8::Local<v8::Object>& object);

    private:

        bool ConvertArg(const v8::Local<v8::Value>& jsArg, int index);

        bool ConvertJavaScriptArray(const v8::Local<v8::Object>& source, jsize length, int index);

        bool ConvertArrayBufferToJavaArray(const v8::Local<v8::Object>& source, int index);

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

        std::vector<std::string> m_ownedTokens;

        // May point at m_ownedTokens, which is why the converter is not copyable.
        const std::vector<std::string>* m_tokens = &m_ownedTokens;

        Error m_error;
    };
}

#endif /* JSARGCONVERTER_H_ */
