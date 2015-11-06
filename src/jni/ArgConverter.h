/*
 * ArgConverter.h
 *
 *  Created on: Jan 29, 2014
 *      Author: slavchev
 */

#ifndef ARGCONVERTER_H_
#define ARGCONVERTER_H_

#include "v8.h"
#include "JEnv.h"
#include <string>

namespace tns
{
	class ArgConverter
	{
	public:
		static void Init(JavaVM *jvm);

		static v8::Local<v8::Array> ConvertJavaArgsToJsArgs(jobjectArray args);

		static v8::Local<v8::Value> ConvertFromJavaLong(jlong value);

		static int64_t ConvertToJavaLong(const v8::Local<v8::Value>& value);

		static bool TryConvertToJavaLong(const v8::Local<v8::Value>& value, jlong& javaLong);

		static v8::Local<v8::Value> jstringToV8String(jstring value);

		static std::string jstringToString(jstring value);

	private:

		static JavaVM *jvm;

		static bool ReadJStringInBuffer(jstring value, jsize& utfLength);

		static jstring ObjectToString(jobject object);

		static v8::Local<v8::String> jcharToV8String(jchar value);

		static void NativeScriptLongFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void NativeScriptLongValueOfFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void NativeScriptLongToStringFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static const long long JS_LONG_LIMIT = ((long long)1) << 53;

		static v8::Persistent<v8::Function> *NATIVESCRIPT_NUMERA_CTOR_FUNC;
		static v8::Persistent<v8::NumberObject> *NAN_NUMBER_OBJECT;

		static char *charBuffer;
		static const int BUFFER_SIZE = 1024 * 64; // 64KB size. TODO: Do we need a larger/smaller buffer?
	};
}

#endif /* ARGCONVERTER_H_ */
