#ifndef NODE_API_NS_FAST_H_
#define NODE_API_NS_FAST_H_

// V8 fast API calls through Node-API — a NativeScript vendor extension.
//
// Upstream Node-API declined to expose V8 fast calls (nodejs/node#54731,
// closed not-planned): the ABI-stable convention assumes a callback may call
// napi_*, and a fast callback may not. So this is an embedder extension by
// definition, and it is EXPLICITLY UNSTABLE — the descriptor vocabulary here
// is NativeScript's, it tracks a V8 API that has already broken its own
// consumers (the `fallback` field, the receiver parameter, the `v8::Value*`
// parameter type), and it may change or disappear in any release.
//
// Gating, two levels:
//
//   * NODE_API_NS_FAST_CALLS_VERSION is defined by this header, so portable
//     addon code can feature-detect at compile time and keep one source for
//     every runtime:
//
//         #if defined(__has_include)
//         #  if __has_include(<node_api_ns_fast.h>)
//         #    define MY_ADDON_TRY_FAST_CALLS 1
//         #  endif
//         #endif
//
//   * NODE_API_NS_EXPERIMENTAL_FAST_CALLS must be defined by the addon before
//     including this header. It is a deliberate acknowledgement that the API
//     is unstable; without it this header is a hard error.
//
// A runtime that ships this header always accepts registrations, even where
// the fast path can never fire (a jitless embed such as the iOS runtime, or
// any non-V8 engine): the slow path then serves every call. Register once,
// run everywhere, and go fast where the tier exists — see
// node_api_ns_fast_calls_available().

#define NODE_API_NS_FAST_CALLS_VERSION 1

#ifndef NODE_API_NS_EXPERIMENTAL_FAST_CALLS
#error "<node_api_ns_fast.h> exposes an unstable NativeScript extension to Node-API. Define NODE_API_NS_EXPERIMENTAL_FAST_CALLS before including it to acknowledge that this API may change without notice."
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "node_api.h"

EXTERN_C_START

// The unboxed types a fast call can carry, mirroring V8's own vocabulary. The
// enum is NativeScript's stable-ish contract: V8's CTypeInfo::Type is never
// exposed, so its churn stays inside the runtime.
typedef enum {
  ns_fast_void = 0,
  ns_fast_bool,
  ns_fast_uint8,
  ns_fast_int32,
  ns_fast_uint32,
  ns_fast_int64,
  ns_fast_uint64,
  ns_fast_float32,
  ns_fast_float64,
  // A raw pointer, e.g. previously handed out as external data. V8 performs no
  // validation whatsoever; the addon owns every guarantee about it.
  ns_fast_pointer,
  // An opaque JS value handle, passed through untouched (V8's kV8Value).
  ns_fast_value,
  // A zero-copy view of a one-byte (Latin-1) string; the parameter type in the
  // C function is `const ns_fast_one_byte_string_view*`. The view is valid only for
  // the duration of the call, and V8 simply does not take the fast path for a
  // two-byte string, so the slow path must handle those.
  ns_fast_one_byte_string,
} ns_fast_type;

// WebIDL-style conversion behaviour, applied by V8 *before* the call.
typedef enum {
  ns_fast_flag_none = 0,
  // Integral parameters only.
  ns_fast_flag_clamp = 1 << 0,
  ns_fast_flag_enforce_range = 1 << 1,
  // Float parameters only: reject NaN/Infinity.
  ns_fast_flag_is_restricted = 1 << 2,
  // Typed-array/ArrayBuffer parameters only: accept shared backing stores.
  ns_fast_flag_allow_shared = 1 << 3,
} ns_fast_flags;

typedef struct {
  ns_fast_type type;
  uint32_t flags;  // a bitwise OR of ns_fast_flags
} ns_fast_param;

// The zero-copy string view handed to an ns_fast_one_byte_string parameter.
// Layout-compatible with v8::FastOneByteString.
typedef struct {
  const char* data;
  uint32_t length;
} ns_fast_one_byte_string_view;

// The receiver (`this`) — always the FIRST parameter of the C function,
// whether or not the addon uses it. It is deliberately opaque: it is NOT a
// napi_value and no napi_* call may be made on it. It exists in the ABI from
// day one because V8 introduced it once already and broke every embedder that
// had not planned for it (denoland/deno#15139).
typedef struct ns_fast_receiver__* ns_fast_receiver;

// The trailing parameter of a *fallible* fast function (descriptor.fallible).
// Opaque; its only use is the throw shims below.
typedef struct ns_fast_options__* ns_fast_options;

// How a fast function is registered. Passed by pointer so this explicitly
// unstable API can grow fields without breaking every call site.
typedef struct {
  // REQUIRED, and required to be semantically identical to fast_fn. V8 chooses
  // between the two paths freely — unoptimized tiers, deopts, argument-count
  // mismatch and non-JIT embeds all run the slow one — so any observable
  // difference between them is a bug that will surface as nondeterminism.
  napi_callback slow_cb;

  // Passed to slow_cb as its callback data, exactly as napi_create_function
  // would. The fast function does not receive it (it has no env to read it
  // through); keep fast-path state in statics or behind ns_fast_pointer.
  void* data;

  // The C function, or NULL to register slow-path-only (which is what a
  // portable addon does on runtimes where fast calls cannot fire).
  //
  // Its C signature must be, in order:
  //   1. ns_fast_receiver
  //   2. one parameter per arg_types entry, in the unboxed C type
  //   3. ns_fast_options, if and only if `fallible` is true
  // returning the C type named by return_type.
  //
  // Inside it: NO napi_* calls, no JS-heap allocation, no JS execution, no
  // blocking, and no exceptions other than through the shims below. Violating
  // that is undefined behaviour at the V8 level, not a napi error.
  const void* fast_fn;

  // Void, bool, the integral types and the float types. Handle and string
  // returns are not supported in this version.
  ns_fast_param return_type;

  // The addon's own parameters: the receiver and the options handle are NOT
  // counted or described here, the runtime adds them.
  size_t argc;
  const ns_fast_param* arg_types;

  // True if fast_fn takes the trailing ns_fast_options and may throw through
  // node_api_ns_fast_throw_*. Fallible fast functions are supported (V8 12.6+
  // allows a fast callback to throw); returning after a throw shim is called
  // is fine, the return value is ignored once an exception is pending.
  bool fallible;

  // Whether ns_fast_int64/ns_fast_uint64 surface to JS as BigInt (true) or as
  // Number (false, the default and what Node uses).
  bool int64_as_bigint;
} ns_fast_descriptor;

// Creates a function backed by both paths and returns it as an ordinary
// napi_value. Fails with napi_invalid_arg on a malformed descriptor (bad type
// for a return, a flag on a type that cannot carry it, argc without
// arg_types). The function cannot be used as a constructor.
NAPI_EXTERN napi_status NAPI_CDECL
node_api_ns_create_fast_function(napi_env env,
                                 const char* utf8name,
                                 size_t length,
                                 const ns_fast_descriptor* descriptor,
                                 napi_value* result);

// Whether this runtime can register V8 fast-call trampolines at all: false on
// a jitless or non-V8 embed, where every call runs the slow path. True does
// NOT promise any individual call takes the fast path — that is V8's choice,
// always, and depends on the caller tiering up.
NAPI_EXTERN napi_status NAPI_CDECL
node_api_ns_fast_calls_available(napi_env env, bool* result);

// Throws from inside a fallible fast function. `code` may be NULL; `message`
// may not. The runtime opens the handle scope and touches V8 on the addon's
// behalf, so the addon never sees an isolate. Control returns to the caller —
// return any value, it is ignored.
NAPI_EXTERN void NAPI_CDECL node_api_ns_fast_throw_error(
    ns_fast_options options, const char* code, const char* message);

NAPI_EXTERN void NAPI_CDECL node_api_ns_fast_throw_type_error(
    ns_fast_options options, const char* code, const char* message);

NAPI_EXTERN void NAPI_CDECL node_api_ns_fast_throw_range_error(
    ns_fast_options options, const char* code, const char* message);

EXTERN_C_END

#endif  // NODE_API_NS_FAST_H_
