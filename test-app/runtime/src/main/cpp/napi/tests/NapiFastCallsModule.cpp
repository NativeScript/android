// Test addon for the V8 fast-call extension. Debug builds only.
//
// Each function is registered with both paths, and each path bumps its own
// counter so the specs can tell which one V8 actually chose. That counter is
// the single deliberate difference between the two implementations — the
// fixture exists to observe the choice, which the contract otherwise forbids
// user code from noticing (see docs/node-api-fast-calls.md).

#define NAPI_EXPERIMENTAL
#define NODE_API_EXPERIMENTAL_NO_WARNING
#define NODE_API_NS_EXPERIMENTAL_FAST_CALLS

#include <stdint.h>
#include <string.h>

#include <atomic>

#include "NapiTestSupport.h"
#include "napi/node_api_ns_fast.h"

namespace {

std::atomic<uint32_t> g_fast_calls{0};
std::atomic<uint32_t> g_slow_calls{0};
// Throws raised from inside the fast path specifically — the one thing the
// error shape observed from JS cannot tell you, since both paths produce an
// identical RangeError.
std::atomic<uint32_t> g_fast_throws{0};

void CountFast() { g_fast_calls.fetch_add(1, std::memory_order_relaxed); }
void CountSlow() { g_slow_calls.fetch_add(1, std::memory_order_relaxed); }

//=== addInt32(a, b) -> int32 ==============================================

int32_t FastAddInt32(ns_fast_receiver receiver, int32_t a, int32_t b) {
  (void)receiver;
  CountFast();
  return a + b;
}

napi_value SlowAddInt32(napi_env env, napi_callback_info info) {
  CountSlow();

  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  int32_t a = 0;
  int32_t b = 0;
  if (argc > 0) {
    NAPI_CALL(env, napi_get_value_int32(env, args[0], &a));
  }
  if (argc > 1) {
    NAPI_CALL(env, napi_get_value_int32(env, args[1], &b));
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_int32(env, a + b, &result));
  return result;
}

//=== scale(x) -> double ===================================================

double FastScale(ns_fast_receiver receiver, double x) {
  (void)receiver;
  CountFast();
  return x * 2.5;
}

napi_value SlowScale(napi_env env, napi_callback_info info) {
  CountSlow();

  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double x = 0;
  if (argc > 0) {
    NAPI_CALL(env, napi_get_value_double(env, args[0], &x));
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_double(env, x * 2.5, &result));
  return result;
}

//=== divide(a, b) -> double, fallible =====================================

double FastDivide(ns_fast_receiver receiver,
                  double a,
                  double b,
                  ns_fast_options options) {
  (void)receiver;
  CountFast();
  if (b == 0) {
    g_fast_throws.fetch_add(1, std::memory_order_relaxed);
    // Returns normally; the pending exception is what the caller observes.
    node_api_ns_fast_throw_range_error(options, "ERR_DIV_ZERO",
                                       "division by zero");
    return 0;
  }
  return a / b;
}

napi_value SlowDivide(napi_env env, napi_callback_info info) {
  CountSlow();

  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double a = 0;
  double b = 0;
  if (argc > 0) {
    NAPI_CALL(env, napi_get_value_double(env, args[0], &a));
  }
  if (argc > 1) {
    NAPI_CALL(env, napi_get_value_double(env, args[1], &b));
  }

  if (b == 0) {
    napi_throw_range_error(env, "ERR_DIV_ZERO", "division by zero");
    return NULL;
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_double(env, a / b, &result));
  return result;
}

//=== byteLength(str) -> uint32 ============================================
//
// V8 only takes the fast path for one-byte (Latin-1) strings; a two-byte
// string falls through to the slow path, which is why both have to agree.

uint32_t FastByteLength(ns_fast_receiver receiver,
                        const ns_fast_one_byte_string_view* text) {
  (void)receiver;
  CountFast();
  return text == NULL ? 0 : text->length;
}

napi_value SlowByteLength(napi_env env, napi_callback_info info) {
  CountSlow();

  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  size_t length = 0;
  if (argc > 0) {
    NAPI_CALL(env,
              napi_get_value_string_utf8(env, args[0], NULL, 0, &length));
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_uint32(env, (uint32_t)length, &result));
  return result;
}

//=== clamped(x) -> uint8, exercising the conversion flags =================

uint32_t FastClamped(ns_fast_receiver receiver, uint8_t value) {
  (void)receiver;
  CountFast();
  return value;
}

napi_value SlowClamped(napi_env env, napi_callback_info info) {
  CountSlow();

  size_t argc = 1;
  napi_value args[1];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  double raw = 0;
  if (argc > 0) {
    NAPI_CALL(env, napi_get_value_double(env, args[0], &raw));
  }

  // The same WebIDL [Clamp] conversion V8 applies before the fast call.
  double clamped = raw;
  if (clamped != clamped) {  // NaN
    clamped = 0;
  } else if (clamped < 0) {
    clamped = 0;
  } else if (clamped > 255) {
    clamped = 255;
  } else {
    // Round half to even, as WebIDL specifies.
    double floor_value = clamped - (clamped - (int64_t)clamped);
    double fraction = clamped - floor_value;
    if (fraction > 0.5 || (fraction == 0.5 && ((int64_t)floor_value & 1))) {
      floor_value += 1;
    }
    clamped = floor_value;
  }

  napi_value result = NULL;
  NAPI_CALL(env, napi_create_uint32(env, (uint32_t)clamped, &result));
  return result;
}

//=== Counters and capability ==============================================

napi_value FastCallCount(napi_env env, napi_callback_info info) {
  napi_value result = NULL;
  NAPI_CALL(env, napi_create_uint32(
                     env, g_fast_calls.load(std::memory_order_relaxed), &result));
  return result;
}

napi_value SlowCallCount(napi_env env, napi_callback_info info) {
  napi_value result = NULL;
  NAPI_CALL(env, napi_create_uint32(
                     env, g_slow_calls.load(std::memory_order_relaxed), &result));
  return result;
}

napi_value FastThrowCount(napi_env env, napi_callback_info info) {
  napi_value result = NULL;
  NAPI_CALL(env,
            napi_create_uint32(
                env, g_fast_throws.load(std::memory_order_relaxed), &result));
  return result;
}

napi_value ResetCounts(napi_env env, napi_callback_info info) {
  g_fast_calls.store(0, std::memory_order_relaxed);
  g_slow_calls.store(0, std::memory_order_relaxed);
  g_fast_throws.store(0, std::memory_order_relaxed);
  return NULL;
}

napi_value FastCallsAvailable(napi_env env, napi_callback_info info) {
  bool available = false;
  NAPI_CALL(env, node_api_ns_fast_calls_available(env, &available));

  napi_value result = NULL;
  NAPI_CALL(env, napi_get_boolean(env, available, &result));
  return result;
}

//=== Registration =========================================================

bool DefineFast(napi_env env,
                napi_value exports,
                const char* name,
                napi_callback slow_cb,
                const void* fast_fn,
                ns_fast_type return_type,
                size_t argc,
                const ns_fast_param* arg_types,
                bool fallible) {
  ns_fast_descriptor descriptor;
  memset(&descriptor, 0, sizeof(descriptor));
  descriptor.slow_cb = slow_cb;
  descriptor.fast_fn = fast_fn;
  descriptor.return_type.type = return_type;
  descriptor.argc = argc;
  descriptor.arg_types = arg_types;
  descriptor.fallible = fallible;

  napi_value function = NULL;
  if (node_api_ns_create_fast_function(env, name, NAPI_AUTO_LENGTH, &descriptor,
                                       &function) != napi_ok) {
    NapiThrowLastError(env);
    return false;
  }

  return napi_set_named_property(env, exports, name, function) == napi_ok;
}

napi_value Init(napi_env env, napi_value exports) {
  static const ns_fast_param kTwoInt32[] = {
      {ns_fast_int32, ns_fast_flag_none},
      {ns_fast_int32, ns_fast_flag_none},
  };
  static const ns_fast_param kOneDouble[] = {
      {ns_fast_float64, ns_fast_flag_none},
  };
  static const ns_fast_param kTwoDoubles[] = {
      {ns_fast_float64, ns_fast_flag_none},
      {ns_fast_float64, ns_fast_flag_none},
  };
  static const ns_fast_param kOneString[] = {
      {ns_fast_one_byte_string, ns_fast_flag_none},
  };
  static const ns_fast_param kOneClampedByte[] = {
      {ns_fast_uint8, ns_fast_flag_clamp},
  };

  if (!DefineFast(env, exports, "addInt32", SlowAddInt32,
                  (const void*)FastAddInt32, ns_fast_int32, 2, kTwoInt32,
                  false) ||
      !DefineFast(env, exports, "scale", SlowScale, (const void*)FastScale,
                  ns_fast_float64, 1, kOneDouble, false) ||
      !DefineFast(env, exports, "divide", SlowDivide, (const void*)FastDivide,
                  ns_fast_float64, 2, kTwoDoubles, true) ||
      !DefineFast(env, exports, "byteLength", SlowByteLength,
                  (const void*)FastByteLength, ns_fast_uint32, 1, kOneString,
                  false) ||
      !DefineFast(env, exports, "clamped", SlowClamped,
                  (const void*)FastClamped, ns_fast_uint32, 1, kOneClampedByte,
                  false) ||
      // Registered with no fast function at all: the portable-addon shape,
      // which must behave exactly like a plain Node-API function.
      !DefineFast(env, exports, "slowOnlyAddInt32", SlowAddInt32, NULL,
                  ns_fast_int32, 2, kTwoInt32, false)) {
    return NULL;
  }

  napi_property_descriptor properties[] = {
      {"fastCallCount", NULL, FastCallCount, NULL, NULL, NULL, napi_default,
       NULL},
      {"slowCallCount", NULL, SlowCallCount, NULL, NULL, NULL, napi_default,
       NULL},
      {"fastThrowCount", NULL, FastThrowCount, NULL, NULL, NULL, napi_default,
       NULL},
      {"resetCounts", NULL, ResetCounts, NULL, NULL, NULL, napi_default, NULL},
      {"fastCallsAvailable", NULL, FastCallsAvailable, NULL, NULL, NULL,
       napi_default, NULL},
  };
  if (napi_define_properties(env, exports,
                             sizeof(properties) / sizeof(properties[0]),
                             properties) != napi_ok) {
    return NULL;
  }

  return exports;
}

napi_module sModule = {
    NAPI_MODULE_VERSION, 0, __FILE__, Init, "napifastcallsmodule", NULL, {0},
};

}  // namespace

__attribute__((constructor)) static void RegisterNapiFastCallsModule(void) {
  napi_module_register(&sModule);
}
