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
#include <vector>

namespace tns {

// The argument arrays com.tns.Runtime.packageArgs builds for a Java -> JS call.
struct JavaCallArgs {
    jintArray types;
    jlongArray values;
    jobjectArray refs;
};

class ArgConverter {
    public:
        static void Init(v8::Local<v8::Context> context);

        static int GetJavaArgsLength(JNIEnv* env, const JavaCallArgs& args);

        // out must have room for GetJavaArgsLength(args) values.
        static void ConvertJavaArgsToJsArgs(v8::Isolate* isolate, JNIEnv* env, const JavaCallArgs& args, int argc, v8::Local<v8::Value>* out);

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

        inline static v8::Local<v8::String> jcharToV8String(v8::Isolate* isolate, jchar value) {
                auto v8String = ConvertToV8String(isolate, &value, 1);
                return v8String;
        }

        // For property keys: an internalized key skips the string-table insert a
        // fresh key costs on every property lookup.
        static v8::Local<v8::String> jstringToInternalizedV8String(v8::Isolate* isolate, JNIEnv* env, jstring value) {
                if (value == nullptr) {
                        return v8::String::Empty(isolate);
                }

                constexpr jsize kInlineUnits = 64;
                jsize length = env->GetStringLength(value);
                jchar inlineUnits[kInlineUnits];
                std::vector<jchar> heapUnits;
                jchar* units = inlineUnits;
                if (length > kInlineUnits) {
                        heapUnits.resize(length);
                        units = heapUnits.data();
                }
                env->GetStringRegion(value, 0, length, units);
                return v8::String::NewFromTwoByte(isolate, reinterpret_cast<const uint16_t*>(units), v8::NewStringType::kInternalized, length).ToLocalChecked();
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
                        return {*str, static_cast<size_t>(str.length())};
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

        /*
         * Per-runtime state (RuntimeState owns one of these per runtime, so it
         * has to be constructible from outside ArgConverter). Destroyed with
         * the runtime, while its isolate is still alive.
         */
        struct TypeLongOperationsCache {
            v8::Persistent<v8::Function>* LongNumberCtorFunc = nullptr;

            v8::Persistent<v8::NumberObject>* NanNumberObject = nullptr;

            ~TypeLongOperationsCache() {
                delete LongNumberCtorFunc;
                delete NanNumberObject;
            }
        };

    private:

        // TODO: plamen5kov: rewrite logic for java long number operations in javascript (java long -> javascript number operations check)
        static const long long JS_LONG_LIMIT = ((long long) 1) << 53;

        static TypeLongOperationsCache* GetTypeLongCache(v8::Isolate* isolate);

        inline static jstring ObjectToString(jobject object) {
                return (jstring) object;
        }

        static void NativeScriptLongFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void NativeScriptLongValueOfFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void NativeScriptLongToStringFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

};
}

#endif /* ARGCONVERTER_H_ */
