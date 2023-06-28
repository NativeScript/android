#ifndef V8GLOBALHELPERS_H_
#define V8GLOBALHELPERS_H_

#include "jni.h"
#include "v8.h"
#include "include/v8.h"
#include <string>
#include <map>

namespace tns {
std::string JsonStringifyObject(v8::Isolate* isolate, v8::Handle<v8::Object> value, bool handleCircularReferences = true);

bool V8GetPrivateValue(v8::Isolate* isolate, const v8::Local<v8::Object>& obj, const v8::Local<v8::String>& propName, v8::Local<v8::Value>& out);

bool V8SetPrivateValue(v8::Isolate* isolate, const v8::Local<v8::Object>& obj, const v8::Local<v8::String>& propName, const v8::Local<v8::Value>& value);

namespace V8GlobalHelpers {
    void onDisposeIsolate(v8::Isolate* isolate);
}
}

#endif /* V8GLOBALHELPERS_H_ */
