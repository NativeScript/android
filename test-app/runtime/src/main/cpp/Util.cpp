#include "Util.h"
#include "JsV8InspectorClient.h"
#include <sstream>
#include <iostream>
#include <codecvt>

using namespace v8;
using namespace std;
namespace tns {


    string Util::JniClassPathToCanonicalName(const string &jniClassPath) {
        std::string canonicalName;

        const char prefix = jniClassPath[0];

        std::string rest;
        int lastIndex;

        switch (prefix) {
            case 'L':
                canonicalName = jniClassPath.substr(1, jniClassPath.size() - 2);
                std::replace(canonicalName.begin(), canonicalName.end(), '/', '.');
                std::replace(canonicalName.begin(), canonicalName.end(), '$', '.');
                break;

            case '[':
                canonicalName = jniClassPath;
                lastIndex = canonicalName.find_last_of('[');
                rest = canonicalName.substr(lastIndex + 1);
                canonicalName = canonicalName.substr(0, lastIndex + 1);
                canonicalName.append(JniClassPathToCanonicalName(rest));
                break;

            default:
                // TODO:
                canonicalName = jniClassPath;
                break;
        }
        return canonicalName;
    }

    void Util::SplitString(const string &str, const string &delimiters, vector <string> &tokens) {
        string::size_type delimPos = 0, tokenPos = 0, pos = 0;

        if (str.length() < 1) {
            return;
        }

        while (true) {
            delimPos = str.find_first_of(delimiters, pos);
            tokenPos = str.find_first_not_of(delimiters, pos);

            if (string::npos != delimPos) {
                if (string::npos != tokenPos) {
                    if (tokenPos < delimPos) {
                        tokens.push_back(str.substr(pos, delimPos - pos));
                    } else {
                        tokens.emplace_back("");
                    }
                } else {
                    tokens.emplace_back("");
                }
                pos = delimPos + 1;
            } else {
                if (string::npos != tokenPos) {
                    tokens.push_back(str.substr(pos));
                } else {
                    tokens.emplace_back("");
                }
                break;
            }
        }
    }

    bool Util::EndsWith(const string &str, const string &suffix) {
        bool res = false;
        if (str.size() > suffix.size()) {
            res = equal(suffix.rbegin(), suffix.rend(), str.rbegin());
        }
        return res;
    }

    string Util::ConvertFromJniToCanonicalName(const string &name) {
        string converted = name;
        replace(converted.begin(), converted.end(), '/', '.');
        return converted;
    }

    string Util::ConvertFromCanonicalToJniName(const string &name) {
        string converted = name;
        replace(converted.begin(), converted.end(), '.', '/');
        return converted;
    }

    string Util::ReplaceAll(string &str, const string &from, const string &to) {
        if (from.empty()) {
            return str;
        }

        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }

        return str;
    }

    u16string Util::ConvertFromUtf8ToUtf16(const string &str) {
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        auto utf16String =
                std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>().from_bytes(str);

        return utf16String;
    }

//uint16_t* Util::ConvertFromUtf8ToProtocolUtf16(const string& str) {
//    auto utf16String =
//        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>().from_bytes(str);
//    return (uint16_t *) utf16String.c_str();
//}

    void Util::JoinString(const std::vector<std::string> &list, const std::string &delimiter,
                          std::string &out) {
        out.clear();

        stringstream ss;

        for (auto it = list.begin(); it != list.end(); ++it) {
            ss << *it;

            if (it != list.end() - 1) {
                ss << delimiter;
            }
        }

        out = ss.str();
    }

    Local <v8::FunctionTemplate> NewFunctionTemplate(
            v8::Isolate *isolate,
            v8::FunctionCallback callback,
            Local <v8::Value> data,
            Local <v8::Signature> signature,
            v8::ConstructorBehavior behavior,
            v8::SideEffectType side_effect_type,
            const v8::CFunction *c_function) {
        return v8::FunctionTemplate::New(isolate,
                                         callback,
                                         data,
                                         signature,
                                         0,
                                         behavior,
                                         side_effect_type,
                                         c_function);
    }

    void SetMethod(Local <v8::Context> context,
                   Local <v8::Object> that,
                   const char *name,
                   v8::FunctionCallback callback,
                   Local <v8::Value> data) {
        Isolate *isolate = context->GetIsolate();
        Local<v8::Function> function =
                NewFunctionTemplate(isolate,
                                    callback,
                                    data,
                                    Local<v8::Signature>(),
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasSideEffect)
                        ->GetFunction(context)
                        .ToLocalChecked();
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->Set(context, name_string, function).Check();
        function->SetName(name_string);  // NODE_SET_METHOD() compatibility.
    }

    void SetMethod(v8::Isolate *isolate,
                   v8::Local<v8::Template> that,
                   const char *name,
                   v8::FunctionCallback callback,
                   Local <v8::Value> data) {
        Local<v8::FunctionTemplate> t =
                NewFunctionTemplate(isolate,
                                    callback,
                                    data,
                                    Local<v8::Signature>(),
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasSideEffect);
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->Set(name_string, t);
    }

    void SetFastMethod(Isolate *isolate,
                       Local <Template> that,
                       const char *name,
                       v8::FunctionCallback slow_callback,
                       const v8::CFunction *c_function,
                       Local <v8::Value> data) {
        Local<v8::FunctionTemplate> t =
                NewFunctionTemplate(isolate,
                                    slow_callback,
                                    data,
                                    Local<v8::Signature>(),
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasSideEffect,
                                    c_function);
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->Set(name_string, t);
    }

    void SetFastMethod(Local <v8::Context> context,
                       Local <v8::Object> that,
                       const char *name,
                       v8::FunctionCallback slow_callback,
                       const v8::CFunction *c_function,
                       Local <v8::Value> data) {
        Isolate *isolate = context->GetIsolate();
        Local<v8::Function> function =
                NewFunctionTemplate(isolate,
                                    slow_callback,
                                    data,
                                    Local<v8::Signature>(),
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasSideEffect,
                                    c_function)
                        ->GetFunction(context)
                        .ToLocalChecked();
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->Set(context, name_string, function).Check();
    }

    void SetFastMethodNoSideEffect(Local <v8::Context> context,
                                   Local <v8::Object> that,
                                   const char *name,
                                   v8::FunctionCallback slow_callback,
                                   const v8::CFunction *c_function,
                                   Local <v8::Value> data) {
        Isolate *isolate = context->GetIsolate();
        Local<v8::Function> function =
                NewFunctionTemplate(isolate,
                                    slow_callback,
                                    data,
                                    Local<v8::Signature>(),
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasNoSideEffect,
                                    c_function)
                        ->GetFunction(context)
                        .ToLocalChecked();
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->Set(context, name_string, function).Check();
    }

    void SetFastMethodNoSideEffect(Isolate *isolate,
                                   Local <Template> that,
                                   const char *name,
                                   v8::FunctionCallback slow_callback,
                                   const v8::CFunction *c_function,
                                   Local <v8::Value> data) {
        Local<v8::FunctionTemplate> t =
                NewFunctionTemplate(isolate,
                                    slow_callback,
                                    data,
                                    Local<v8::Signature>(),
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasNoSideEffect,
                                    c_function);
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->Set(name_string, t);
    }

    void SetMethodNoSideEffect(Local <v8::Context> context,
                               Local <v8::Object> that,
                               const char *name,
                               v8::FunctionCallback callback,
                               Local <v8::Value> data) {
        Isolate *isolate = context->GetIsolate();
        Local<v8::Function> function =
                NewFunctionTemplate(isolate,
                                    callback,
                                    data,
                                    Local<v8::Signature>(),
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasNoSideEffect)
                        ->GetFunction(context)
                        .ToLocalChecked();
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->Set(context, name_string, function).Check();
        function->SetName(name_string);  // NODE_SET_METHOD() compatibility.
    }

    void SetMethodNoSideEffect(Isolate *isolate,
                               Local <v8::Template> that,
                               const char *name,
                               v8::FunctionCallback callback,
                               Local <v8::Value> data) {
        Local<v8::FunctionTemplate> t =
                NewFunctionTemplate(isolate,
                                    callback,
                                    data,
                                    Local<v8::Signature>(),
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasNoSideEffect);
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->Set(name_string, t);
    }

    void SetProtoMethod(v8::Isolate *isolate,
                        Local <v8::FunctionTemplate> that,
                        const char *name,
                        v8::FunctionCallback callback,
                        Local <v8::Value> data) {
        Local<v8::Signature> signature = v8::Signature::New(isolate, that);
        Local<v8::FunctionTemplate> t =
                NewFunctionTemplate(isolate,
                                    callback,
                                    data,
                                    signature,
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasSideEffect);
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->PrototypeTemplate()->Set(name_string, t);
        t->SetClassName(name_string);  // NODE_SET_PROTOTYPE_METHOD() compatibility.
    }

    void SetProtoMethodNoSideEffect(v8::Isolate *isolate,
                                    Local <v8::FunctionTemplate> that,
                                    const char *name,
                                    v8::FunctionCallback callback,
                                    Local <v8::Value> data) {
        Local<v8::Signature> signature = v8::Signature::New(isolate, that);
        Local<v8::FunctionTemplate> t =
                NewFunctionTemplate(isolate,
                                    callback,
                                    data,
                                    signature,
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasNoSideEffect);
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->PrototypeTemplate()->Set(name_string, t);
        t->SetClassName(name_string);  // NODE_SET_PROTOTYPE_METHOD() compatibility.
    }

    void SetInstanceMethod(v8::Isolate *isolate,
                           Local <v8::FunctionTemplate> that,
                           const char *name,
                           v8::FunctionCallback callback,
                           Local <v8::Value> data) {
        Local<v8::Signature> signature = v8::Signature::New(isolate, that);
        Local<v8::FunctionTemplate> t =
                NewFunctionTemplate(isolate,
                                    callback,
                                    data,
                                    signature,
                                    v8::ConstructorBehavior::kThrow,
                                    v8::SideEffectType::kHasSideEffect);
        // kInternalized strings are created in the old space.
        const v8::NewStringType type = v8::NewStringType::kInternalized;
        Local<v8::String> name_string =
                v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
        that->InstanceTemplate()->Set(name_string, t);
        t->SetClassName(name_string);
    }

    void SetConstructorFunction(Local <v8::Context> context,
                                Local <v8::Object> that,
                                const char *name,
                                Local <v8::FunctionTemplate> tmpl,
                                SetConstructorFunctionFlag flag) {
        Isolate *isolate = context->GetIsolate();
        SetConstructorFunction(
                context, that, tns::OneByteString(isolate, name), tmpl, flag);
    }

    void SetConstructorFunction(Local <Context> context,
                                Local <Object> that,
                                Local <v8::String> name,
                                Local <FunctionTemplate> tmpl,
                                SetConstructorFunctionFlag flag) {
        if (flag == SetConstructorFunctionFlag::SET_CLASS_NAME)
            tmpl->SetClassName(name);
        that->Set(context, name, tmpl->GetFunction(context).ToLocalChecked()).Check();
    }

    void SetConstructorFunction(Isolate *isolate,
                                Local <Template> that,
                                const char *name,
                                Local <FunctionTemplate> tmpl,
                                SetConstructorFunctionFlag flag) {
        SetConstructorFunction(
                isolate, that, OneByteString(isolate, name), tmpl, flag);
    }

    void SetConstructorFunction(Isolate *isolate,
                                Local <Template> that,
                                Local <v8::String> name,
                                Local <FunctionTemplate> tmpl,
                                SetConstructorFunctionFlag flag) {
        if (flag == SetConstructorFunctionFlag::SET_CLASS_NAME)
            tmpl->SetClassName(name);
        that->Set(name, tmpl);
    }

    std::vector<uint16_t> Util::ToVector(const std::string &value) {
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        // FIXME: std::codecvt_utf8_utf16 is deprecated
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        std::u16string value16 = convert.from_bytes(value);

        const uint16_t *begin = reinterpret_cast<uint16_t const *>(value16.data());
        const uint16_t *end = reinterpret_cast<uint16_t const *>(value16.data() + value16.size());
        std::vector<uint16_t> vector(begin, end);
        return vector;
    }
};
