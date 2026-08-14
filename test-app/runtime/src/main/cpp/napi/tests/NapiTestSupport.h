#ifndef NapiTestSupport_h
#define NapiTestSupport_h

#include <string.h>

#include "napi/vendor/node_api.h"

// A failed call leaves either a pending JS exception or only an extended error
// info record; JS must see a throw either way.
static inline void NapiThrowLastError(napi_env env) {
  // The message has to be copied out before anything else runs: every napi
  // call clears the error record on its way out, the pending-exception check
  // below included, after which napi_get_last_error_info reports napi_ok —
  // whose message is null.
  char message[256];
  const napi_extended_error_info* info = NULL;
  napi_get_last_error_info(env, &info);
  if (info != NULL && info->error_message != NULL) {
    strncpy(message, info->error_message, sizeof(message) - 1);
    message[sizeof(message) - 1] = '\0';
  } else {
    strcpy(message, "napi call failed");
  }

  bool pending = false;
  if (napi_is_exception_pending(env, &pending) == napi_ok && pending) {
    return;
  }

  napi_throw_error(env, NULL, message);
}

#define NAPI_CALL(env, call)   \
  do {                         \
    if ((call) != napi_ok) {   \
      NapiThrowLastError(env); \
      return NULL;             \
    }                          \
  } while (0)

#define NAPI_METHOD(name, fn) \
  {(name), NULL, (fn), NULL, NULL, NULL, napi_default, NULL}
#define NAPI_GETTER(name, fn) \
  {(name), NULL, NULL, (fn), NULL, NULL, napi_enumerable, NULL}
#define NAPI_VALUE(name, val) \
  {(name), NULL, NULL, NULL, NULL, (val), napi_enumerable, NULL}

// Statuses cross into JS as names so a spec can assert on them.
static inline const char* NapiStatusName(napi_status status) {
  switch (status) {
    case napi_ok:
      return "ok";
    case napi_invalid_arg:
      return "invalid_arg";
    case napi_object_expected:
      return "object_expected";
    case napi_string_expected:
      return "string_expected";
    case napi_name_expected:
      return "name_expected";
    case napi_function_expected:
      return "function_expected";
    case napi_number_expected:
      return "number_expected";
    case napi_boolean_expected:
      return "boolean_expected";
    case napi_array_expected:
      return "array_expected";
    case napi_generic_failure:
      return "generic_failure";
    case napi_pending_exception:
      return "pending_exception";
    case napi_cancelled:
      return "cancelled";
    case napi_escape_called_twice:
      return "escape_called_twice";
    case napi_handle_scope_mismatch:
      return "handle_scope_mismatch";
    case napi_callback_scope_mismatch:
      return "callback_scope_mismatch";
    case napi_queue_full:
      return "queue_full";
    case napi_closing:
      return "closing";
    case napi_bigint_expected:
      return "bigint_expected";
    case napi_date_expected:
      return "date_expected";
    case napi_arraybuffer_expected:
      return "arraybuffer_expected";
    case napi_detachable_arraybuffer_expected:
      return "detachable_arraybuffer_expected";
    case napi_would_deadlock:
      return "would_deadlock";
    case napi_no_external_buffers_allowed:
      return "no_external_buffers_allowed";
    case napi_cannot_run_js:
      return "cannot_run_js";
    default:
      return "other";
  }
}

static inline napi_value NapiStatusValue(napi_env env, napi_status status) {
  napi_value result = NULL;
  napi_create_string_utf8(env, NapiStatusName(status), NAPI_AUTO_LENGTH,
                          &result);
  return result;
}

// Result builders for the object-shaped returns the specs assert against. A
// failed create yields NULL, which napi_set_named_property then rejects.
static inline napi_value NapiDouble(napi_env env, double value) {
  napi_value result = NULL;
  napi_create_double(env, value, &result);
  return result;
}

static inline napi_value NapiBool(napi_env env, bool value) {
  napi_value result = NULL;
  napi_get_boolean(env, value, &result);
  return result;
}

static inline napi_value NapiString(napi_env env, const char* value) {
  napi_value result = NULL;
  napi_create_string_utf8(env, value, NAPI_AUTO_LENGTH, &result);
  return result;
}

#endif /* NapiTestSupport_h */
