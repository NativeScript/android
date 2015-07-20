#ifndef V8GLOBALHELPERS_H_
#define V8GLOBALHELPERS_H_

#include "jni.h"
#include "v8.h"
#include <string>

namespace tns
{
	std::string ConvertToString(const v8::Handle<v8::String>& s);

	jstring ConvertToJavaString(const v8::Handle<v8::Value>& jsValue);

	v8::Local<v8::String> ConvertToV8String(const jchar* data, int length);

	v8::Local<v8::String> ConvertToV8String(const std::string& s);

	v8::Local<v8::String> ConvertToV8String(const char *data, int length);

	v8::Local<v8::Value> V8GetHiddenValue(const v8::Handle<v8::Object>& obj, const std::string& propName);

	bool V8SetHiddenValue(const v8::Handle<v8::Object>& obj, const std::string& propName, const v8::Handle<v8::Value>& value);
}


#endif /* V8GLOBALHELPERS_H_ */
