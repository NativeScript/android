#ifndef FIELDACCESSOR_H_
#define FIELDACCESSOR_H_

#include "v8.h"
#include "JEnv.h"
#include <string>
#include "ObjectManager.h"
#include "FieldCallbackData.h"

namespace tns {
class FieldAccessor {
    public:
        v8::Local<v8::Value> GetJavaField(v8::Isolate* isolate, const v8::Local<v8::Object>& target, FieldCallbackData* fieldData);

        void SetJavaField(v8::Isolate* isolate, const v8::Local<v8::Object>& target, const v8::Local<v8::Value>& value, FieldCallbackData* fieldData);
};
}

#endif /* FIELDACCESSOR_H_ */
