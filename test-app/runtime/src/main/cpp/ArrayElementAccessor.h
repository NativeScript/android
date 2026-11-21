#ifndef ARRAYELEMENTACCESSOR_H_
#define ARRAYELEMENTACCESSOR_H_

#include "JEnv.h"
#include "v8.h"
#include <string>
#include "ObjectManager.h"

namespace tns {
class ArrayElementAccessor {
    public:
        v8::Local<v8::Value> GetArrayElement(v8::Isolate* isolate, v8::Local<v8::Context> context, const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature);

        void SetArrayElement(v8::Isolate* isolate, v8::Local<v8::Context> context, const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature, v8::Local<v8::Value>& value);

    private:
        void assertNonNullNativeArray(tns::JniLocalRef& arrayReference);
};
}

#endif /* ARRAYELEMENTACCESSOR_H_ */
