#ifndef V8GLOBALHELPERS_H_
#define V8GLOBALHELPERS_H_

#include "jni.h"
#include "v8.h"
#include <string>

namespace tns
{
	v8::Local<v8::Value> V8GetHiddenValue(const v8::Local<v8::Object>& obj, const std::string& propName);

	bool V8SetHiddenValue(const v8::Local<v8::Object>& obj, const std::string& propName, const v8::Local<v8::Value>& value);
}

#endif /* V8GLOBALHELPERS_H_ */
