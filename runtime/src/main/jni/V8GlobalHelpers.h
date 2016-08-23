#ifndef V8GLOBALHELPERS_H_
#define V8GLOBALHELPERS_H_

#include "jni.h"
#include "v8.h"
#include <string>

namespace tns
{
	std::string ConvertToString(const v8::Local<v8::String>& s);

	jstring ConvertToJavaString(const v8::Local<v8::Value>& jsValue);

	v8::Local<v8::String> ConvertToV8String(const jchar* data, int length);

	v8::Local<v8::String> ConvertToV8String(const std::string& s);

	v8::Local<v8::String> ConvertToV8String(const char *data, int length);

	bool V8HasPrivateValue(v8::Isolate *isolate, const v8::Local<v8::Object> &obj, const v8::Local<v8::String> &propName);

	bool V8GetPrivateValue(v8::Isolate *isolate, const v8::Local<v8::Object>& obj, const v8::Local<v8::String>& propName, v8::Local<v8::Value>& out);

	bool V8SetPrivateValue(v8::Isolate *isolate, const v8::Local<v8::Object>& obj, const v8::Local<v8::String>& propName, const v8::Local<v8::Value>& value);
}

#endif /* V8GLOBALHELPERS_H_ */
