#ifndef JSARGTOARRAYCONVERTER_H_
#define JSARGTOARRAYCONVERTER_H_

#include "JEnv.h"
#include "v8.h"
#include "JniLocalRef.h"
#include <vector>
#include <string>

namespace tns {
class JsArgToArrayConverter {
    public:
        JsArgToArrayConverter(const v8::FunctionCallbackInfo<v8::Value>& args, bool hasImplementationObject);

        JsArgToArrayConverter(v8::Isolate* isolate, v8::Local<v8::Context> context, const v8::Local<v8::Value>& arg, bool isImplementationObject, int classReturnType);

        JsArgToArrayConverter(const JsArgToArrayConverter&) = delete;

        JsArgToArrayConverter& operator=(const JsArgToArrayConverter&) = delete;

        ~JsArgToArrayConverter();

        jobjectArray ToJavaArray();

        jobject GetConvertedArg();

        // The converted value as a local reference the caller deletes, or null. The
        // converter no longer refers to it afterwards.
        jobject ReleaseConvertedArg();

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

    private:
        bool ConvertArg(v8::Local<v8::Context> context, const v8::Local<v8::Value>& jsArg, int index);

        void SetConvertedObject(JEnv& env, int index, jobject obj, bool isGlobal = false);

        v8::Isolate* m_isolate;

        int m_argsLen;

        int m_return_type;

        bool m_isValid;

        Error m_error;

        struct ConvertedArg {
            jobject obj = nullptr;
            bool ownsLocalRef = false;
        };

        // The single-value constructor converts into m_inlineArg.
        ConvertedArg m_inlineArg;

        ConvertedArg* m_args;

        jobjectArray m_arr;

        short MAX_JAVA_PARAMS_COUNT = 256;

        static jclass JAVA_LANG_OBJECT_CLASS;
};
}

#endif /* JSARGTOARRAYCONVERTER_H_ */
