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

namespace tns {
class ArgConverter {
    public:
        static void Init(v8::Isolate* isolate);

        static v8::Local<v8::Array> ConvertJavaArgsToJsArgs(v8::Isolate* isolate, jobjectArray args);

        static v8::Local<v8::Value> ConvertFromJavaLong(v8::Isolate* isolate, jlong value);

        static int64_t ConvertToJavaLong(v8::Isolate* isolate, const v8::Local<v8::Value>& value);

        static v8::Local<v8::Value> jstringToV8String(v8::Isolate* isolate, jstring value);

        static std::string jstringToString(jstring value);

        static std::string ConvertToString(const v8::Local<v8::String>& s);

        static std::u16string ConvertToUtf16String(const v8::Local<v8::String>& s);

        static jstring ConvertToJavaString(const v8::Local<v8::Value>& jsValue);

        static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, const jchar* data, int length);

        static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, const std::string& s);

        static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, const char* data, int length);

        static v8::Local<v8::String> ConvertToV8UTF16String(v8::Isolate* isolate, const std::string& string);

        static v8::Local<v8::String> ConvertToV8UTF16String(v8::Isolate* isolate, const uint16_t* utf16string, int size);

        static v8::Local<v8::String> ConvertToV8UTF16String(v8::Isolate* isolate, const std::u16string& utf16string);

    private:

        // TODO: plamen5kov: rewrite logic for java long number operations in javascript (java long -> javascript number operations check)
        static const long long JS_LONG_LIMIT = ((long long) 1) << 53;

        struct TypeLongOperationsCache {
            v8::Persistent<v8::Function>* LongNumberCtorFunc;

            v8::Persistent<v8::NumberObject>* NanNumberObject;
        };
        //

        static TypeLongOperationsCache* GetTypeLongCache(v8::Isolate* isolate);

        static jstring ObjectToString(jobject object);

        static v8::Local<v8::String> jcharToV8String(v8::Isolate* isolate, jchar value);

        static void NativeScriptLongFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void NativeScriptLongValueOfFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void NativeScriptLongToStringFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * "s_type_long_operations_cache" used to keep function
         * dealing with operations concerning java long -> javascript number.
         */
        static std::map<v8::Isolate*, TypeLongOperationsCache*> s_type_long_operations_cache;
};
}

#endif /* ARGCONVERTER_H_ */
