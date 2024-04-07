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
        static void Init(v8::Local<v8::Context> context);

        static v8::Local<v8::Array> ConvertJavaArgsToJsArgs(v8::Local<v8::Context> context, jobjectArray args);

        static v8::Local<v8::Value> ConvertFromJavaLong(v8::Isolate* isolate, jlong value);

        static int64_t ConvertToJavaLong(v8::Isolate* isolate, const v8::Local<v8::Value>& value);

        static v8::Local<v8::Value> jstringToV8String(v8::Isolate* isolate, jstring value) {
                if (value == nullptr) {
                        return Null(isolate);
                }

                JEnv env;
                auto chars = env.GetStringChars(value, NULL);
                auto length = env.GetStringLength(value);
                auto v8String = ConvertToV8String(isolate, chars, length);
                env.ReleaseStringChars(value, chars);

                return v8String;
        }

        static std::string jstringToString(jstring value) {
                if (value == nullptr) {
                        return {};
                }

                JEnv env;

                jboolean f = JNI_FALSE;
                auto chars = env.GetStringUTFChars(value, &f);
                std::string s(chars);
                env.ReleaseStringUTFChars(value, chars);

                return s;
        }

        inline static std::string ConvertToString(const v8::Local<v8::String>& s) {
                if (s.IsEmpty()) {
                        return {};
                } else {
                        auto isolate = v8::Isolate::GetCurrent();
                        v8::String::Utf8Value str(isolate, s);
                        return {*str};
                }
        }

    inline static v8::Local<v8::String> ToV8String(v8::Isolate *isolate, const std::string &value) {
        return v8::String::NewFromUtf8(isolate, value.c_str(), v8::NewStringType::kNormal,
                                       (int) value.length()).ToLocalChecked();
    }

    inline static std::string ToString(v8::Isolate *isolate, const v8::Local<v8::Value> &value) {
        if (value.IsEmpty()) {
            return std::string();
        }

        if (value->IsStringObject()) {
            v8::Local<v8::String> obj = value.As<v8::StringObject>()->ValueOf();
            return ToString(isolate, obj);
        }

        v8::String::Utf8Value result(isolate, value);

        const char *val = *result;
        if (val == nullptr) {
            return std::string();
        }

        return std::string(*result, result.length());
    }


    static std::u16string ConvertToUtf16String(const v8::Local<v8::String>& s);

        inline static jstring ConvertToJavaString(const v8::Local<v8::Value>& jsValue) {
                JEnv env;
                auto isolate = v8::Isolate::GetCurrent();
                v8::String::Value stringValue(isolate, jsValue);
                return env.NewString((const jchar*) *stringValue, stringValue.length());
        }

        inline static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, const jchar* data, int length) {
                return v8::String::NewFromTwoByte(isolate, (const uint16_t*) data, v8::NewStringType::kNormal, length).ToLocalChecked();
        }

        inline static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, const std::string& s) {
                return v8::String::NewFromUtf8(isolate, s.c_str(), v8::NewStringType::kNormal, s.length()).ToLocalChecked();
        }

        inline static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, const char* data, int length)  {
                return v8::String::NewFromUtf8(isolate, (const char*) data, v8::NewStringType::kNormal, length).ToLocalChecked();
        }

        inline static v8::Local<v8::String> ConvertToV8UTF16String(v8::Isolate* isolate, const std::u16string& utf16string) {
                return v8::String::NewFromTwoByte(isolate, ((const uint16_t*) utf16string.data())).ToLocalChecked();
        }

        static void onDisposeIsolate(v8::Isolate* isolate);

    private:

        // TODO: plamen5kov: rewrite logic for java long number operations in javascript (java long -> javascript number operations check)
        static const long long JS_LONG_LIMIT = ((long long) 1) << 53;

        struct TypeLongOperationsCache {
            v8::Persistent<v8::Function>* LongNumberCtorFunc;

            v8::Persistent<v8::NumberObject>* NanNumberObject;
        };
        //

        static TypeLongOperationsCache* GetTypeLongCache(v8::Isolate* isolate);

        inline static jstring ObjectToString(jobject object) {
                return (jstring) object;
        }

        inline static v8::Local<v8::String> jcharToV8String(v8::Isolate* isolate, jchar value) {
                auto v8String = ConvertToV8String(isolate, &value, 1);
                return v8String;
        }

        static void NativeScriptLongFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void NativeScriptLongValueOfFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void NativeScriptLongToStringFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        /*
         * "s_type_long_operations_cache" used to keep function
         * dealing with operations concerning java long -> javascript number.
         */
        static robin_hood::unordered_map<v8::Isolate*, TypeLongOperationsCache*> s_type_long_operations_cache;
};
}

#endif /* ARGCONVERTER_H_ */
