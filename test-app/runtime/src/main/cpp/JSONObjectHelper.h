#ifndef JSONOBJECTHELPER_H_
#define JSONOBJECTHELPER_H_

#include "v8.h"
#include "ObjectManager.h"

namespace tns {

class JSONObjectHelper {
public:
    JSONObjectHelper();
    void CreateConvertFunctions(v8::Isolate* isolate, const v8::Local<v8::Object>& global, ObjectManager* objectManager);
private:
    ObjectManager* m_objectManager;
    v8::Persistent<v8::Function>* m_serializeFunc;

    v8::Local<v8::Function> CreateSerializeFunc(v8::Isolate* isolate);
    void ConvertCallback(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void ConvertCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& info);
};

}

#endif //JSONOBJECTHELPER_H_
