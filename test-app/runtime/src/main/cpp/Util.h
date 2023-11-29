#include "v8.h"
#include <string>
#include <vector>

#ifndef UTIL_H_
#define UTIL_H_

namespace tns {
class Util {
    public:
        static std::string JniClassPathToCanonicalName(const std::string& jniClassPath);

        static void SplitString(const std::string& str, const std::string& delimiters, std::vector<std::string>& tokens);

        static void JoinString(const std::vector<std::string>& list, const std::string& delimiter, std::string& out);

        static bool EndsWith(const std::string& str, const std::string& suffix);

        static std::string ConvertFromJniToCanonicalName(const std::string& name);

        static std::string ConvertFromCanonicalToJniName(const std::string& name);

        static std::string ReplaceAll(std::string& str, const std::string& from, const std::string& to);

        static std::u16string ConvertFromUtf8ToUtf16(const std::string& str);

       // static std::uint16_t* ConvertFromUtf8ToProtocolUtf16(const std::string& str);
};


// Helpers from Node
inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                           const char* data,
                                           int length) {
    return v8::String::NewFromOneByte(isolate,
                                      reinterpret_cast<const uint8_t*>(data),
                                      v8::NewStringType::kNormal,
                                      length).ToLocalChecked();
}
inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                           const signed char* data,
                                           int length) {
    return v8::String::NewFromOneByte(isolate,
                                      reinterpret_cast<const uint8_t*>(data),
                                      v8::NewStringType::kNormal,
                                      length).ToLocalChecked();
}
inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                           const unsigned char* data,
                                           int length) {
    return v8::String::NewFromOneByte(
                                      isolate, data, v8::NewStringType::kNormal, length)
    .ToLocalChecked();
}

// Convenience wrapper around v8::String::NewFromOneByte().
inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                           const char* data,
                                           int length = -1);
// For the people that compile with -funsigned-char.
inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                           const signed char* data,
                                           int length = -1);
inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                           const unsigned char* data,
                                           int length = -1);



v8::Local<v8::FunctionTemplate> NewFunctionTemplate(
                                                    v8::Isolate* isolate,
                                                    v8::FunctionCallback callback,
                                                    v8::Local<v8::Value> data = v8::Local<v8::Value>(),
                                                    v8::Local<v8::Signature> signature = v8::Local<v8::Signature>(),
                                                    v8::ConstructorBehavior behavior = v8::ConstructorBehavior::kAllow,
                                                    v8::SideEffectType side_effect = v8::SideEffectType::kHasSideEffect,
                                                    const v8::CFunction* c_function = nullptr);
// Convenience methods for NewFunctionTemplate().
void SetMethod(v8::Local<v8::Context> context,
               v8::Local<v8::Object> that,
               const char* name,
               v8::FunctionCallback callback,
               v8::Local<v8::Value> data = v8::Local<v8::Value>());
// Similar to SetProtoMethod but without receiver signature checks.
void SetMethod(v8::Isolate* isolate,
               v8::Local<v8::Template> that,
               const char* name,
               v8::FunctionCallback callback,
               v8::Local<v8::Value> data = v8::Local<v8::Value>());
void SetFastMethod(v8::Isolate* isolate,
                   v8::Local<v8::Template> that,
                   const char* name,
                   v8::FunctionCallback slow_callback,
                   const v8::CFunction* c_function,
                   v8::Local<v8::Value> data = v8::Local<v8::Value>());
void SetFastMethod(v8::Local<v8::Context> context,
                   v8::Local<v8::Object> that,
                   const char* name,
                   v8::FunctionCallback slow_callback,
                   const v8::CFunction* c_function,
                   v8::Local<v8::Value> data = v8::Local<v8::Value>());
void SetFastMethodNoSideEffect(v8::Isolate* isolate,
                               v8::Local<v8::Template> that,
                               const char* name,
                               v8::FunctionCallback slow_callback,
                               const v8::CFunction* c_function,
                               v8::Local<v8::Value> data = v8::Local<v8::Value>());
void SetFastMethodNoSideEffect(v8::Local<v8::Context> context,
                               v8::Local<v8::Object> that,
                               const char* name,
                               v8::FunctionCallback slow_callback,
                               const v8::CFunction* c_function,
                               v8::Local<v8::Value> data = v8::Local<v8::Value>());
void SetProtoMethod(v8::Isolate* isolate,
                    v8::Local<v8::FunctionTemplate> that,
                    const char* name,
                    v8::FunctionCallback callback,
                    v8::Local<v8::Value> data = v8::Local<v8::Value>());
void SetInstanceMethod(v8::Isolate* isolate,
                       v8::Local<v8::FunctionTemplate> that,
                       const char* name,
                       v8::FunctionCallback callback,
                       v8::Local<v8::Value> data = v8::Local<v8::Value>());
// Safe variants denote the function has no side effects.
void SetMethodNoSideEffect(v8::Local<v8::Context> context,
                           v8::Local<v8::Object> that,
                           const char* name,
                           v8::FunctionCallback callback,
                           v8::Local<v8::Value> data = v8::Local<v8::Value>());
void SetProtoMethodNoSideEffect(v8::Isolate* isolate,
                                v8::Local<v8::FunctionTemplate> that,
                                const char* name,
                                v8::FunctionCallback callback,
                                v8::Local<v8::Value> data = v8::Local<v8::Value>());
void SetMethodNoSideEffect(v8::Isolate* isolate,
                           v8::Local<v8::Template> that,
                           const char* name,
                           v8::FunctionCallback callback,
                           v8::Local<v8::Value> data = v8::Local<v8::Value>());
enum class SetConstructorFunctionFlag {
    NONE,
    SET_CLASS_NAME,
};
void SetConstructorFunction(v8::Local<v8::Context> context,
                            v8::Local<v8::Object> that,
                            const char* name,
                            v8::Local<v8::FunctionTemplate> tmpl,
                            SetConstructorFunctionFlag flag =
                            SetConstructorFunctionFlag::SET_CLASS_NAME);
void SetConstructorFunction(v8::Local<v8::Context> context,
                            v8::Local<v8::Object> that,
                            v8::Local<v8::String> name,
                            v8::Local<v8::FunctionTemplate> tmpl,
                            SetConstructorFunctionFlag flag =
                            SetConstructorFunctionFlag::SET_CLASS_NAME);
void SetConstructorFunction(v8::Isolate* isolate,
                            v8::Local<v8::Template> that,
                            const char* name,
                            v8::Local<v8::FunctionTemplate> tmpl,
                            SetConstructorFunctionFlag flag =
                            SetConstructorFunctionFlag::SET_CLASS_NAME);
void SetConstructorFunction(v8::Isolate* isolate,
                            v8::Local<v8::Template> that,
                            v8::Local<v8::String> name,
                            v8::Local<v8::FunctionTemplate> tmpl,
                            SetConstructorFunctionFlag flag =
                            SetConstructorFunctionFlag::SET_CLASS_NAME);


template <int N>
inline v8::Local<v8::String> FIXED_ONE_BYTE_STRING(
    v8::Isolate* isolate,
    const char(&data)[N]) {
  return OneByteString(isolate, data, N - 1);
}

template <std::size_t N>
inline v8::Local<v8::String> FIXED_ONE_BYTE_STRING(
    v8::Isolate* isolate,
    const std::array<char, N>& arr) {
  return OneByteString(isolate, arr.data(), N - 1);
}

class PersistentToLocal {
 public:
  // If persistent.IsWeak() == false, then do not call persistent.Reset()
  // while the returned Local<T> is still in scope, it will destroy the
  // reference to the object.
  template <class TypeName>
  static inline v8::Local<TypeName> Default(
      v8::Isolate* isolate,
      const v8::PersistentBase<TypeName>& persistent) {
    if (persistent.IsWeak()) {
      return PersistentToLocal::Weak(isolate, persistent);
    } else {
      return PersistentToLocal::Strong(persistent);
    }
  }

  // Unchecked conversion from a non-weak Persistent<T> to Local<T>,
  // use with care!
  //
  // Do not call persistent.Reset() while the returned Local<T> is still in
  // scope, it will destroy the reference to the object.
  template <class TypeName>
  static inline v8::Local<TypeName> Strong(
      const v8::PersistentBase<TypeName>& persistent) {
//    DCHECK(!persistent.IsWeak());
    return *reinterpret_cast<v8::Local<TypeName>*>(
        const_cast<v8::PersistentBase<TypeName>*>(&persistent));
  }

  template <class TypeName>
  static inline v8::Local<TypeName> Weak(
      v8::Isolate* isolate,
      const v8::PersistentBase<TypeName>& persistent) {
    return v8::Local<TypeName>::New(isolate, persistent);
  }
};

}

#endif /* UTIL_H_ */