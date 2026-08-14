#ifndef SRC_ENV_INL_H_
#define SRC_ENV_INL_H_

// js_native_api_v8.cc includes this name expecting Node's environment header.
// It is the first include after the NAPI_EXPERIMENTAL opt-in, which is why the
// shim's own definitions are pulled in from here rather than only through
// js_native_api_v8.h.

#include "js_native_api_v8_internals.h"

// napi_create_external_arraybuffer delegates to napi_create_external_buffer,
// which js_native_api_v8.cc never includes a header for: upstream it arrives
// through node.h, pulled in by the real env.h.
#include "node_api.h"

#endif  // SRC_ENV_INL_H_
