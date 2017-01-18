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

        JsArgToArrayConverter(v8::Isolate* isolate, const v8::Local<v8::Value>& arg, bool isImplementationObject, int classReturnType);

        ~JsArgToArrayConverter();

        jobjectArray ToJavaArray();

        jobject GetConvertedArg();

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
        bool ConvertArg(const v8::Local<v8::Value>& arg, int index);

        void SetConvertedObject(JEnv& env, int index, jobject obj, bool isGlobal = false);

        v8::Isolate* m_isolate;

        int m_argsLen;

        int m_return_type;

        bool m_isValid;

        Error m_error;

        std::vector<int> m_storedIndexes;

        jobject* m_argsAsObject;

        jobjectArray m_arr;

        short MAX_JAVA_PARAMS_COUNT = 256;

        static jclass JAVA_LANG_OBJECT_CLASS;
};
}

#endif /* JSARGTOARRAYCONVERTER_H_ */
