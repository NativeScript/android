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

		static v8::Handle<v8::Array> ConvertJavaArgsToJsArgs(jobjectArray args);

		static v8::Local<v8::Value> ConvertFromJavaLong(jlong value);

		static int64_t ConvertToJavaLong(const v8::Handle<v8::Value>& value);

		static bool TryConvertToJavaLong(const v8::Handle<v8::Value>& value, jlong& javaLong);

		static v8::Local<v8::String> jstringToV8String(jstring value);

		static std::string jstringToString(jstring value);

	private:

		static JavaVM *jvm;

		static jlong ObjectToLong(jobject object);

		static jboolean ObjectToBoolean(jobject object);

		static jchar ObjectToChar(jobject object);

		static jbyte ObjectToByte(jobject object);

		static jshort ObjectToShort(jobject object);

		static jint ObjectToInt(jobject object);

		static jfloat ObjectToFloat(jobject object);

		static jdouble ObjectToDouble(jobject object);

		static jstring ObjectToString(jobject object);

		static v8::Handle<v8::String> jcharToV8String(jchar value);

		static void NativeScriptLongFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void NativeScriptLongValueOfFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static void NativeScriptLongToStringFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

		static const int TypeID_Boolean = 0;
		static const int TypeID_Char = 1;
		static const int TypeID_Byte = 2;
		static const int TypeID_Short = 3;
		static const int TypeID_Int = 4;
		static const int TypeID_Long = 5;
		static const int TypeID_Float = 6;
		static const int TypeID_Double = 7;
		static const int TypeID_String = 8;
		static const int TypeID_JsObject = 9;
		static const int TypeID_Null = 10;

		static const long long JS_LONG_LIMIT = ((long long)1) << 53;

		static v8::Persistent<v8::Function> *NATIVESCRIPT_NUMERA_CTOR_FUNC;

		static v8::Persistent<v8::NumberObject> *NAN_NUMBER_OBJECT;

		static jclass STRING_CLASS;

		static jmethodID GET_BYTES_METHOD_ID;

		static jstring UTF_8_ENCODING;
	};
}

#endif /* ARGCONVERTER_H_ */
