#ifndef SRC_JS_NATIVE_API_V8_INTERNALS_H_
#define SRC_JS_NATIVE_API_V8_INTERNALS_H_

// Supplies the idioms `js_native_api_v8.{h,cc}` expect from their embedder.
// Upstream bridges these to Node's internal headers; here they are defined
// against this runtime instead, so the vendored files stay byte-identical.
//
// Anything needing the runtime (private keys, fatal errors) is declared here
// and defined in NapiEnv.cpp.

// js_native_api_v8.cc opts into NAPI_EXPERIMENTAL so it implements every
// versioned entry point; the accompanying #warning is not actionable here and
// -Werror would turn it into a build failure.
#ifndef NODE_API_EXPERIMENTAL_NO_WARNING
#define NODE_API_EXPERIMENTAL_NO_WARNING
#endif

#include <memory>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include "v8.h"
#pragma clang diagnostic pop

// Mirrors src/node_version.h of the vendored Node.js release.
#define NODE_API_SUPPORTED_VERSION_MAX 10
#define NODE_API_SUPPORTED_VERSION_MIN 1
#define NODE_API_DEFAULT_MODULE_API_VERSION 8

#define NAPI_ARRAYSIZE(array) (sizeof(::v8impl::ArraySizeHelper(array)))

#define NAPI_FIXED_ONE_BYTE_STRING(isolate, string)                            \
  (::v8impl::OneByteString((isolate), (string), sizeof(string) - 1))

namespace tns {

// The suffixes js_native_api_v8.cc passes to NAPI_PRIVATE_KEY. Named to match
// the macro argument so the macro can paste them unchanged.
enum class NapiPrivateKeySlot { wrapper, type_tag };

// Node keys these off node::Environment; we key off the napi_env owning the
// context, which is reachable from the isolate.
v8::Local<v8::Private> NapiPrivateKey(v8::Local<v8::Context> context,
                                      NapiPrivateKeySlot slot);

}  // namespace tns

#define NAPI_PRIVATE_KEY(context, suffix)                                      \
  (::tns::NapiPrivateKey((context), ::tns::NapiPrivateKeySlot::suffix))

namespace v8impl {

template <typename T>
using Persistent = v8::Global<T>;

// Reads a v8::Global as a v8::Local without allocating a handle. Sound only
// for strong references: a weak one may have been cleared, so the slot has to
// be re-read through the isolate.
class PersistentToLocal {
 public:
  template <class TypeName>
  static inline v8::Local<TypeName> Strong(
      const v8::PersistentBase<TypeName>& persistent) {
    return *reinterpret_cast<v8::Local<TypeName>*>(
        const_cast<v8::PersistentBase<TypeName>*>(&persistent));
  }

  template <class TypeName>
  static inline v8::Local<TypeName> Weak(
      v8::Isolate* isolate, const v8::PersistentBase<TypeName>& persistent) {
    return v8::Local<TypeName>::New(isolate, persistent);
  }

  template <class TypeName>
  static inline v8::Local<TypeName> Default(
      v8::Isolate* isolate, const v8::PersistentBase<TypeName>& persistent) {
    return persistent.IsWeak() ? Weak(isolate, persistent) : Strong(persistent);
  }
};

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                           const char* data, int length) {
  return v8::String::NewFromOneByte(isolate,
                                    reinterpret_cast<const uint8_t*>(data),
                                    v8::NewStringType::kInternalized, length)
      .ToLocalChecked();
}

[[noreturn]] void OnFatalError(const char* location, const char* message);

}  // end of namespace v8impl

#define NAPI_STRINGIFY_HELPER(x) #x
#define NAPI_STRINGIFY(x) NAPI_STRINGIFY_HELPER(x)

// Unprefixed because the vendored sources spell them that way. The inspector
// headers define the same names as equivalent abort-on-false assertions, so a
// translation unit that already has them keeps its own rather than clashing.
#ifndef CHECK
#define CHECK(expr)                                                            \
  do {                                                                         \
    if (!(expr)) {                                                             \
      ::v8impl::OnFatalError(__FILE__ ":" NAPI_STRINGIFY(__LINE__),            \
                             "CHECK(" #expr ") failed");                       \
    }                                                                          \
  } while (0)
#endif

#ifndef CHECK_EQ
#define CHECK_EQ(a, b) CHECK((a) == (b))
#endif
#ifndef CHECK_NE
#define CHECK_NE(a, b) CHECK((a) != (b))
#endif
#ifndef CHECK_LE
#define CHECK_LE(a, b) CHECK((a) <= (b))
#endif
#ifndef CHECK_LT
#define CHECK_LT(a, b) CHECK((a) < (b))
#endif
#ifndef CHECK_GE
#define CHECK_GE(a, b) CHECK((a) >= (b))
#endif
#ifndef CHECK_GT
#define CHECK_GT(a, b) CHECK((a) > (b))
#endif
#ifndef CHECK_NULL
#define CHECK_NULL(val) CHECK((val) == nullptr)
#endif
#ifndef CHECK_NOT_NULL
#define CHECK_NOT_NULL(val) CHECK((val) != nullptr)
#endif

#endif  // SRC_JS_NATIVE_API_V8_INTERNALS_H_
