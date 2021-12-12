#ifndef ARRAYHELPER_H_
#define ARRAYHELPER_H_

#include "v8.h"
#include "ObjectManager.h"
#include <string.h>

namespace tns {
class ArrayHelper {
    public:
        static void Init(const v8::Local<v8::Context>& context);

    private:
        ArrayHelper();

        static void CreateJavaArrayCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

        static void CreateJavaArray(const v8::FunctionCallbackInfo<v8::Value>& info);

        static void Throw(v8::Isolate* isolate, const std::string& errorMessage);

        static jobject CreateArrayByClassName(const std::string& typeName, int length);

        static jclass RUNTIME_CLASS;

        static jmethodID CREATE_ARRAY_HELPER;
};
}

#endif /* ARRAYHELPER_H_ */
