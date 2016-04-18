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
#include <map>

namespace tns
{
	class ArgConverter
	{
		public:
			static void Init(v8::Isolate *isolate);

			static v8::Local<v8::Array> ConvertJavaArgsToJsArgs(v8::Isolate *isolate, jobjectArray args);

			static v8::Local<v8::Value> ConvertFromJavaLong(v8::Isolate *isolate, jlong value);

			static int64_t ConvertToJavaLong(const v8::Local<v8::Value>& value);

			static v8::Local<v8::Value> jstringToV8String(jstring value);

			static std::string jstringToString(jstring value);

		private:
			struct Cache;

			static Cache* GetCache(v8::Isolate *isolate);

			static bool ReadJStringInBuffer(jstring value, jsize& utfLength);

			static jstring ObjectToString(jobject object);

			static v8::Local<v8::String> jcharToV8String(jchar value);

			static void NativeScriptLongFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void NativeScriptLongValueOfFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			static void NativeScriptLongToStringFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

			static const long long JS_LONG_LIMIT = ((long long) 1) << 53;

			struct Cache
			{
				v8::Persistent<v8::Function> *LongNumberCtorFunc;

				v8::Persistent<v8::NumberObject> *NanNumberObject;
			};

			static char *charBuffer;
			static const int BUFFER_SIZE = 1024 * 64; // 64KB size. TODO: Do we need a larger/smaller buffer?
			static std::map<v8::Isolate*, Cache*> s_cache;
	};
}

#endif /* ARGCONVERTER_H_ */
