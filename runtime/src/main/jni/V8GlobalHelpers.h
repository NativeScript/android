#ifndef V8GLOBALHELPERS_H_
#define V8GLOBALHELPERS_H_

#include "jni.h"
#include "v8.h"
#include "include/v8.h"
#include <string>

namespace tns
{
	std::string ConvertToString(const v8::Local<v8::String>& s);

	jstring ConvertToJavaString(const v8::Local<v8::Value>& jsValue);

	v8::Local<v8::String> ConvertToV8String(const jchar* data, int length);

	v8::Local<v8::String> ConvertToV8String(const std::string& s);

	v8::Local<v8::String> ConvertToV8String(const char *data, int length);

	v8::MaybeLocal<v8::Value> V8GetHiddenValue(v8::Isolate *isolate, const v8::Local<v8::Object>& obj, const std::string& propName);

	v8::Maybe<bool> V8SetHiddenValue(v8::Isolate *isolate, const v8::Local<v8::Object>& obj, const std::string& propName, const v8::Local<v8::Value>& value);
}

#endif /* V8GLOBALHELPERS_H_ */
