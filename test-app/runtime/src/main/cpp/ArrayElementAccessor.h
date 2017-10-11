#ifndef ARRAYELEMENTACCESSOR_H_
#define ARRAYELEMENTACCESSOR_H_

#include "JEnv.h"
#include "v8.h"
#include <string>
#include "ObjectManager.h"

namespace tns {
class ArrayElementAccessor {
    public:
        v8::Local<v8::Value> GetArrayElement(v8::Isolate* isolate, const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature);

        void SetArrayElement(v8::Isolate* isolate, const v8::Local<v8::Object>& array, uint32_t index, const std::string& arraySignature, v8::Local<v8::Value>& value);

    private:
        v8::Local<v8::Value> ConvertToJsValue(v8::Isolate* isolate, ObjectManager* objectManager, JEnv& env, const std::string& elementSignature, const void* value);
};
}

#endif /* ARRAYELEMENTACCESSOR_H_ */
