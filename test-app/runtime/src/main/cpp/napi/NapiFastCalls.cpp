// V8 fast API calls behind a Node-API-shaped surface. See
// docs/node-api-fast-calls.md for the contract and node_api_ns_fast.h for the
// plugin-facing API.
//
// The whole point of the indirection is that no V8 type reaches the addon: the
// descriptor vocabulary is ours, and V8's CTypeInfo/CFunction churn — which has
// already broken embedders twice (the receiver parameter, the `fallback`
// field) — stays behind this file.

#define NAPI_EXPERIMENTAL
#define NODE_API_EXPERIMENTAL_NO_WARNING
#define NODE_API_NS_EXPERIMENTAL_FAST_CALLS

#include "node_api_ns_fast.h"

#include <map>
#include <mutex>
#include <vector>

#include "js_native_api_v8.h"
#include "v8-fast-api-calls.h"
#include "v8-template.h"

#include "Constants.h"

namespace {

using v8::CFunction;
using v8::CFunctionInfo;
using v8::CTypeInfo;

//=== Descriptor -> V8 type mapping ========================================

bool MapType(ns_fast_type type, CTypeInfo::Type* result) {
  switch (type) {
    case ns_fast_void: *result = CTypeInfo::Type::kVoid; return true;
    case ns_fast_bool: *result = CTypeInfo::Type::kBool; return true;
    case ns_fast_uint8: *result = CTypeInfo::Type::kUint8; return true;
    case ns_fast_int32: *result = CTypeInfo::Type::kInt32; return true;
    case ns_fast_uint32: *result = CTypeInfo::Type::kUint32; return true;
    case ns_fast_int64: *result = CTypeInfo::Type::kInt64; return true;
    case ns_fast_uint64: *result = CTypeInfo::Type::kUint64; return true;
    case ns_fast_float32: *result = CTypeInfo::Type::kFloat32; return true;
    case ns_fast_float64: *result = CTypeInfo::Type::kFloat64; return true;
    case ns_fast_pointer: *result = CTypeInfo::Type::kPointer; return true;
    case ns_fast_value: *result = CTypeInfo::Type::kV8Value; return true;
    case ns_fast_one_byte_string:
      *result = CTypeInfo::Type::kSeqOneByteString;
      return true;
    default: return false;
  }
}

bool IsIntegral(ns_fast_type type) {
  return type == ns_fast_uint8 || type == ns_fast_int32 ||
         type == ns_fast_uint32 || type == ns_fast_int64 ||
         type == ns_fast_uint64;
}

// V8 asserts on a flag its type cannot carry, so a bad descriptor has to be
// rejected as napi_invalid_arg here rather than crashing inside V8 later.
bool MapFlags(const ns_fast_param& param, CTypeInfo::Flags* result) {
  uint32_t known = ns_fast_flag_clamp | ns_fast_flag_enforce_range |
                   ns_fast_flag_is_restricted | ns_fast_flag_allow_shared;
  if ((param.flags & ~known) != 0) {
    return false;
  }

  uint8_t flags = static_cast<uint8_t>(CTypeInfo::Flags::kNone);

  if ((param.flags & (ns_fast_flag_clamp | ns_fast_flag_enforce_range)) != 0) {
    if (!IsIntegral(param.type)) {
      return false;
    }
    // Clamping and range-enforcing are mutually exclusive conversions.
    if ((param.flags & ns_fast_flag_clamp) != 0 &&
        (param.flags & ns_fast_flag_enforce_range) != 0) {
      return false;
    }
    if ((param.flags & ns_fast_flag_clamp) != 0) {
      flags |= static_cast<uint8_t>(CTypeInfo::Flags::kClampBit);
    } else {
      flags |= static_cast<uint8_t>(CTypeInfo::Flags::kEnforceRangeBit);
    }
  }

  if ((param.flags & ns_fast_flag_is_restricted) != 0) {
    if (param.type != ns_fast_float32 && param.type != ns_fast_float64) {
      return false;
    }
    flags |= static_cast<uint8_t>(CTypeInfo::Flags::kIsRestrictedBit);
  }

  if ((param.flags & ns_fast_flag_allow_shared) != 0) {
    // Only a handle parameter can be an ArrayBuffer/TypedArray.
    if (param.type != ns_fast_value) {
      return false;
    }
    flags |= static_cast<uint8_t>(CTypeInfo::Flags::kAllowSharedBit);
  }

  *result = static_cast<CTypeInfo::Flags>(flags);
  return true;
}

// Returns are limited to what V8 can hand back unboxed. A handle or a string
// return would need allocation, which the fast path forbids.
bool IsSupportedReturnType(ns_fast_type type) {
  return type == ns_fast_void || type == ns_fast_bool || IsIntegral(type) ||
         type == ns_fast_float32 || type == ns_fast_float64;
}

//=== Per-signature immortal type info =====================================
//
// CFunctionInfo keeps the argument array BY POINTER, and V8 keeps the
// CFunctionInfo by pointer for as long as the function template lives. Both
// therefore have to outlive every registration, so they are interned once per
// distinct signature and never freed — the same shape Node gets for free by
// declaring its CFunctionInfos as file-scope statics.

struct SignatureCache {
  std::mutex mutex;
  std::map<std::vector<uint32_t>, const CFunctionInfo*> entries;
};

SignatureCache& Signatures() {
  static SignatureCache* cache = new SignatureCache();
  return *cache;
}

const CFunctionInfo* InternSignature(
    const CTypeInfo& return_info,
    const std::vector<CTypeInfo>& args,
    CFunctionInfo::Int64Representation repr) {
  std::vector<uint32_t> key;
  key.reserve(args.size() + 2);
  key.push_back(static_cast<uint32_t>(repr));
  key.push_back(return_info.GetId());
  for (const CTypeInfo& arg : args) {
    key.push_back(arg.GetId());
  }

  SignatureCache& cache = Signatures();
  std::lock_guard<std::mutex> lock(cache.mutex);

  auto it = cache.entries.find(key);
  if (it != cache.entries.end()) {
    return it->second;
  }

  // Intentionally leaked; see the note above.
  auto* stored_args = new std::vector<CTypeInfo>(args);
  auto* info = new CFunctionInfo(return_info,
                                 static_cast<unsigned int>(stored_args->size()),
                                 stored_args->data(), repr);
  cache.entries.emplace(std::move(key), info);
  return info;
}

//=== The slow path ========================================================
//
// V8 requires a slow callback that is semantically identical to the fast one,
// and it is what actually runs until the caller tiers up. The vendored
// js_native_api_v8.cc builds napi_callback_info out of classes in an anonymous
// namespace, so there is no way to invoke a napi_callback from another
// translation unit: the slow path forwards to a real function made by
// napi_create_function, which costs one extra V8 call and keeps napi's own
// argument/receiver/exception semantics exactly.

struct FastFunctionState {
  napi_env env;
  v8::Global<v8::Function> slow;
};

void DeleteFastFunctionState(void* arg) {
  auto* state = static_cast<FastFunctionState*>(arg);
  state->slow.Reset();
  delete state;
}

void ForwardToSlow(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  auto* state = static_cast<FastFunctionState*>(
      info.Data().As<v8::External>()->Value(v8::kExternalPointerTypeTagDefault));

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Function> slow = state->slow.Get(isolate);

  int argc = info.Length();
  v8::LocalVector<v8::Value> args(isolate);
  args.reserve(argc);
  for (int i = 0; i < argc; i++) {
    args.push_back(info[i]);
  }

  v8::Local<v8::Value> result;
  if (!slow->Call(context, info.This(), argc, args.data()).ToLocal(&result)) {
    // The exception is already pending; V8 unwinds from here.
    return;
  }

  info.GetReturnValue().Set(result);
}

//=== Throwing from a fast callback ========================================
//
// Legal since V8 12.6: the callback may throw directly as long as a
// HandleScope is opened first, and V8 checks the pending-exception slot when
// the C call returns. The addon never sees the isolate, so the scope and the
// throw happen here.

enum class FastErrorKind { kError, kTypeError, kRangeError };

void ThrowFromFastCallback(ns_fast_options options,
                           const char* code,
                           const char* message,
                           FastErrorKind kind) {
  auto* v8_options = reinterpret_cast<v8::FastApiCallbackOptions*>(options);
  if (v8_options == nullptr || v8_options->isolate == nullptr) {
    return;
  }

  v8::Isolate* isolate = v8_options->isolate;
  v8::HandleScope scope(isolate);

  v8::Local<v8::String> message_string;
  if (!v8::String::NewFromUtf8(isolate, message != nullptr ? message : "")
           .ToLocal(&message_string)) {
    return;
  }

  v8::Local<v8::Value> error;
  switch (kind) {
    case FastErrorKind::kTypeError:
      error = v8::Exception::TypeError(message_string);
      break;
    case FastErrorKind::kRangeError:
      error = v8::Exception::RangeError(message_string);
      break;
    default:
      error = v8::Exception::Error(message_string);
      break;
  }

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  if (code != nullptr && !context.IsEmpty() && error->IsObject()) {
    v8::Local<v8::String> code_key;
    v8::Local<v8::String> code_value;
    if (v8::String::NewFromUtf8(isolate, "code").ToLocal(&code_key) &&
        v8::String::NewFromUtf8(isolate, code).ToLocal(&code_value)) {
      error.As<v8::Object>()->Set(context, code_key, code_value).FromMaybe(false);
    }
  }

  isolate->ThrowException(error);
}

}  // namespace

//=== Entry points =========================================================

napi_status NAPI_CDECL
node_api_ns_create_fast_function(napi_env env,
                                 const char* utf8name,
                                 size_t length,
                                 const ns_fast_descriptor* descriptor,
                                 napi_value* result) {
  NAPI_PREAMBLE(env);
  CHECK_ARG(env, descriptor);
  CHECK_ARG(env, result);
  CHECK_ARG(env, descriptor->slow_cb);
  if (descriptor->argc > 0) {
    CHECK_ARG(env, descriptor->arg_types);
  }

  // The slow function is the real Node-API function, and on its own it is a
  // complete, correct registration.
  napi_value slow_value = nullptr;
  STATUS_CALL(napi_create_function(env, utf8name, length, descriptor->slow_cb,
                                   descriptor->data, &slow_value));

  // No fast function (a portable addon on a runtime where fast calls cannot
  // fire), so there is nothing to attach and nothing to pay for.
  if (descriptor->fast_fn == nullptr) {
    *result = slow_value;
    return GET_RETURN_STATUS(env);
  }

  if (!IsSupportedReturnType(descriptor->return_type.type) ||
      descriptor->return_type.flags != ns_fast_flag_none) {
    return napi_set_last_error(env, napi_invalid_arg);
  }

  CTypeInfo::Type return_v8_type;
  if (!MapType(descriptor->return_type.type, &return_v8_type)) {
    return napi_set_last_error(env, napi_invalid_arg);
  }
  CTypeInfo return_info(return_v8_type);

  std::vector<CTypeInfo> args;
  args.reserve(descriptor->argc + 2);
  // The receiver is always the first parameter of the C function.
  args.emplace_back(CTypeInfo::Type::kV8Value);

  for (size_t i = 0; i < descriptor->argc; i++) {
    const ns_fast_param& param = descriptor->arg_types[i];
    CTypeInfo::Type arg_type;
    CTypeInfo::Flags arg_flags;
    if (!MapType(param.type, &arg_type) || param.type == ns_fast_void ||
        !MapFlags(param, &arg_flags)) {
      return napi_set_last_error(env, napi_invalid_arg);
    }
    args.emplace_back(arg_type, arg_flags);
  }

  if (descriptor->fallible) {
    args.emplace_back(CTypeInfo::kCallbackOptionsType);
  }

  const CFunctionInfo* signature = InternSignature(
      return_info, args,
      descriptor->int64_as_bigint ? CFunctionInfo::Int64Representation::kBigInt
                                  : CFunctionInfo::Int64Representation::kNumber);
  CFunction fast_function(descriptor->fast_fn, signature);

  auto* state = new FastFunctionState();
  state->env = env;
  state->slow.Reset(
      env->isolate,
      v8impl::V8LocalValueFromJsValue(slow_value).As<v8::Function>());

  v8::Local<v8::External> data =
      v8::External::New(env->isolate, state, v8::kExternalPointerTypeTagDefault);

  CFunction overloads[] = {fast_function};
  v8::Local<v8::FunctionTemplate> function_template =
      v8::FunctionTemplate::NewWithCFunctionOverloads(
          env->isolate, ForwardToSlow, data, v8::Local<v8::Signature>(),
          static_cast<int>(descriptor->argc),
          // A fast function is a plain function: `new` on it would have to run
          // the slow path with a new.target the forwarding cannot reproduce.
          v8::ConstructorBehavior::kThrow, v8::SideEffectType::kHasSideEffect,
          {overloads, 1});

  v8::Local<v8::Function> function;
  if (!function_template->GetFunction(env->context()).ToLocal(&function)) {
    DeleteFastFunctionState(state);
    return napi_set_last_error(env, napi_generic_failure);
  }

  if (utf8name != nullptr) {
    v8::Local<v8::String> name;
    if (v8::String::NewFromUtf8(
            env->isolate, utf8name, v8::NewStringType::kInternalized,
            length == NAPI_AUTO_LENGTH ? -1 : static_cast<int>(length))
            .ToLocal(&name)) {
      function->SetName(name);
    }
  }

  // Registered only now that nothing else can fail: the state has to outlive
  // the function, and the env outlives both.
  napi_add_env_cleanup_hook(env, DeleteFastFunctionState, state);

  *result = v8impl::JsValueFromV8LocalValue(function);
  return GET_RETURN_STATUS(env);
}

napi_status NAPI_CDECL node_api_ns_fast_calls_available(napi_env env,
                                                        bool* result) {
  CHECK_ENV(env);
  CHECK_ARG(env, result);

  // V8 only emits fast calls from optimized code, and --jitless disables every
  // optimizing tier. The app's startup flags are the only way this runtime can
  // end up jitless, so they are what gets consulted.
  *result = Constants::V8_STARTUP_FLAGS.find("jitless") == std::string::npos;

  return napi_clear_last_error(env);
}

void NAPI_CDECL node_api_ns_fast_throw_error(ns_fast_options options,
                                             const char* code,
                                             const char* message) {
  ThrowFromFastCallback(options, code, message, FastErrorKind::kError);
}

void NAPI_CDECL node_api_ns_fast_throw_type_error(ns_fast_options options,
                                                  const char* code,
                                                  const char* message) {
  ThrowFromFastCallback(options, code, message, FastErrorKind::kTypeError);
}

void NAPI_CDECL node_api_ns_fast_throw_range_error(ns_fast_options options,
                                                   const char* code,
                                                   const char* message) {
  ThrowFromFastCallback(options, code, message, FastErrorKind::kRangeError);
}
