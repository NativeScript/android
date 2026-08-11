#ifndef JSONOBJECTHELPER_H_
#define JSONOBJECTHELPER_H_

#include "v8.h"

namespace tns {

class JSONObjectHelper {
public:
    static void RegisterFromFunction(v8::Isolate *isolate, v8::Local<v8::Value>& jsonObject);
    static void onDisposeIsolate(v8::Isolate* isolate);
private:
    static v8::Persistent<v8::Function>* GetSerializeFunc(v8::Local<v8::Context> context);
    static void ConvertCallbackStatic(const v8::FunctionCallbackInfo<v8::Value>& info);
};

}

#endif //JSONOBJECTHELPER_H_
