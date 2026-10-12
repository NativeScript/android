# V8 fast calls through Node-API (unstable extension)

A normal Node-API call crosses into native code through `FunctionCallbackInfo`,
a handle scope and boxed arguments — on the order of 100 ns. V8's *fast API
calls* let optimized JS call a plain C function with unboxed arguments in
single-digit nanoseconds. This runtime exposes that through a Node-API-shaped
extension, so an addon gets the speedup without writing V8 code.

**This API is explicitly unstable.** Upstream Node-API
[declined to expose fast calls](https://github.com/nodejs/node/issues/54731)
(closed *not planned*): Node-API's contract says a callback may call `napi_*`,
and a fast callback may not — there is no safe general subset, so any support
is an embedder extension by definition. The descriptor vocabulary here is
NativeScript's, it tracks a V8 API that has already broken its consumers more
than once, and it may change or disappear in any release. It is gated
accordingly (see below).

## Gating

Two separate mechanisms, because they answer different questions.

**Compile time — "does this runtime offer the API?"** The header defines
`NODE_API_NS_FAST_CALLS_VERSION`, and an addon that wants to stay portable
compiles the fast path conditionally:

```c
#if defined(__has_include)
#  if __has_include(<node_api_ns_fast.h>)
#    define MY_ADDON_TRY_FAST_CALLS 1
#  endif
#endif

#ifdef MY_ADDON_TRY_FAST_CALLS
#  define NODE_API_NS_EXPERIMENTAL_FAST_CALLS
#  include <node_api_ns_fast.h>
#endif
```

**Compile time — "do you accept an unstable API?"** `node_api_ns_fast.h` is a
hard `#error` unless the addon defines `NODE_API_NS_EXPERIMENTAL_FAST_CALLS`
first. Nobody gets here by accident.

**Run time — "can a fast call actually fire?"**
`node_api_ns_fast_calls_available()` reports false on a jitless embed (the iOS
runtime, or an app that passes `--jitless`), where the slow path serves every
call. You rarely need it: registration succeeds either way and the behaviour is
identical, so it is for diagnostics and benchmarks rather than control flow.

## A complete addon

```c
#define NODE_API_NS_EXPERIMENTAL_FAST_CALLS
#include <node_api_ns_fast.h>

// The fast path: a plain C function. Receiver first, then unboxed arguments.
static int32_t FastAdd(ns_fast_receiver receiver, int32_t a, int32_t b) {
  (void)receiver;
  return a + b;
}

// The slow path: an ordinary napi_callback that MUST do the same thing.
static napi_value SlowAdd(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  napi_get_cb_info(env, info, &argc, args, NULL, NULL);

  int32_t a = 0, b = 0;
  napi_get_value_int32(env, args[0], &a);
  napi_get_value_int32(env, args[1], &b);

  napi_value result = NULL;
  napi_create_int32(env, a + b, &result);
  return result;
}

static napi_value Init(napi_env env, napi_value exports) {
  static const ns_fast_param params[] = {
      {ns_fast_int32, ns_fast_flag_none},
      {ns_fast_int32, ns_fast_flag_none},
  };

  ns_fast_descriptor descriptor = {0};
  descriptor.slow_cb = SlowAdd;
  descriptor.fast_fn = (const void*)FastAdd;
  descriptor.return_type.type = ns_fast_int32;
  descriptor.argc = 2;
  descriptor.arg_types = params;

  napi_value add = NULL;
  if (node_api_ns_create_fast_function(env, "add", NAPI_AUTO_LENGTH,
                                       &descriptor, &add) != napi_ok) {
    return NULL;
  }
  napi_set_named_property(env, exports, "add", add);
  return exports;
}
```

```js
const addon = require("myaddon");
addon.add(1, 2); // 3 — via either path, indistinguishably
```

`test-app/runtime/src/main/cpp/napi/tests/NapiFastCallsModule.cpp` is a working
addon covering every supported shape, and
`test-app/app/src/main/assets/app/tests/NapiFastCallsTests.js` is its spec.

## The contract

**Both paths must be semantically identical.** V8 chooses between them freely
and without notice — unoptimized tiers, deoptimization, an argument-count
mismatch, a two-byte string where a one-byte one was expected, or a jitless
embed all run the slow path. Any observable difference is a bug that surfaces
as nondeterminism.

**Inside the fast function:** no `napi_*` calls, no JS-heap allocation, no JS
execution, no blocking, no exceptions except through the throw shims below.
This is a V8-level requirement, not a Node-API one — violating it is undefined
behaviour, not an error status.

**The receiver is always the first parameter**, whether or not you use it. It
is deliberately opaque (`ns_fast_receiver`): it is not a `napi_value` and no
`napi_*` call may be made on it. It is in the ABI from day one because V8
introduced it once already and broke every embedder that had not planned for
it ([denoland/deno#15139](https://github.com/denoland/deno/issues/15139)).

**`descriptor.data` reaches only the slow path.** The fast function has no env
to read callback data through; keep fast-path state in statics, or pass it
explicitly as an `ns_fast_pointer` argument.

## Types

| `ns_fast_type` | C type in the fast function | Notes |
| --- | --- | --- |
| `ns_fast_void` | `void` | Return only. |
| `ns_fast_bool` | `bool` | |
| `ns_fast_uint8` | `uint8_t` | |
| `ns_fast_int32` / `ns_fast_uint32` | `int32_t` / `uint32_t` | |
| `ns_fast_int64` / `ns_fast_uint64` | `int64_t` / `uint64_t` | Number by default, BigInt with `descriptor.int64_as_bigint`. |
| `ns_fast_float32` / `ns_fast_float64` | `float` / `double` | |
| `ns_fast_pointer` | `void*` | Unvalidated by V8; the addon owns every guarantee. |
| `ns_fast_value` | opaque handle | Passed through untouched. |
| `ns_fast_one_byte_string` | `const ns_fast_one_byte_string_view*` | Zero-copy Latin-1 view, valid for the call only. Two-byte strings simply take the slow path. |

Returns are limited to `void`, `bool`, the integral types and the float types —
a handle or string return would require allocation, which the fast path
forbids.

Per-parameter flags apply WebIDL conversions *before* the call:
`ns_fast_flag_clamp` and `ns_fast_flag_enforce_range` (integral parameters,
mutually exclusive), `ns_fast_flag_is_restricted` (float parameters, rejects
NaN/Infinity), `ns_fast_flag_allow_shared` (handle parameters that may be a
shared ArrayBuffer). A flag on a type that cannot carry it is rejected with
`napi_invalid_arg` at registration — V8 would otherwise assert at some later,
much less obvious point.

## Throwing

Since V8 12.6 a fast callback may throw directly; the old `fallback` escape
hatch is gone (the header's own prose still describes it — doc rot, upstream
included). Set `descriptor.fallible`, take a trailing `ns_fast_options`
parameter, and throw through the shims:

```c
static double FastDivide(ns_fast_receiver receiver, double a, double b,
                         ns_fast_options options) {
  (void)receiver;
  if (b == 0) {
    node_api_ns_fast_throw_range_error(options, "ERR_DIV_ZERO",
                                       "division by zero");
    return 0;  // ignored once an exception is pending
  }
  return a / b;
}
```

The shims (`node_api_ns_fast_throw_error`, `_type_error`, `_range_error`) open
the handle scope and touch V8 for you, so the addon never sees an isolate.
V8 checks the pending-exception slot when the C call returns and unwinds
normally; **throwing does not deoptimize the caller**, and nothing re-executes,
so the old "be idempotent before bailing out" rule no longer applies.

## Where it pays

Leaf, allocation-free, side-effect-free functions with scalar or raw-buffer
arguments in a hot loop: vector and geometry math, hashing, codecs, byte
crunching.

Measured on this runtime with a two-`int32` addition — the cheapest possible
body, so essentially pure call overhead — 2M calls from an optimized caller,
on an **arm64 emulator** (absolute numbers are inflated by virtualization; the
ratio is the meaningful part):

| | ns/call |
| --- | --- |
| Fast path (99.4% of calls; the rest ran before the caller tiered up) | **24** |
| Plain Node-API function | **860** |

So roughly **35× on call overhead** for a function that does nothing else,
which is the ceiling rather than a typical result: the moment the body does
real work, or the arguments need anything the fast path forbids, the ratio
collapses toward 1. The slow path's extra forwarding call (see below) is not
separately measured.

Where it does not pay: anything that needs handles, the env, string
materialization beyond a one-byte view, or JNI marshalling. Node removed the
fast path from `InternalModuleStat` for precisely this reason — it needed a
handle scope anyway, which made the "fast" path slower than the plain binding.
And keep perspective: for most real plugins async and marshalling costs
dominate, and sync call overhead only matters in tight loops. Measure first.

## Verifying which path ran

There is no API for it, by design — the paths are supposed to be
indistinguishable. For development, do what Node's own tests do: keep a counter
in each implementation and call the function from a small monomorphic function
a few hundred thousand times, which is what makes TurboFan optimize the caller
and emit the fast call. `NapiFastCallsTests.js` does this.

## Implementation notes and deviations

For whoever maintains this, and for the iOS runtime mirroring the surface (see
`NAPI_FAST_CALLS.md` there — this implementation deviates from those notes in a
few places):

- **The registration API is a descriptor struct**, not the flat parameter list
  the design notes sketched, so an explicitly unstable API can gain fields
  (`fallible`, `int64_as_bigint` already) without breaking every call site.
- **The slow path costs one extra V8 call.** The vendored
  `js_native_api_v8.cc` builds `napi_callback_info` out of classes in an
  anonymous namespace, so no other translation unit can invoke a
  `napi_callback` directly. The forwarding callback therefore calls a real
  function made by `napi_create_function`, which keeps napi's argument,
  receiver and exception semantics exactly — at the cost of one extra call
  whenever the fast path is not taken. It is the warmup path for any function
  hot enough to deserve a fast call; if that ever matters, the fix is upstream
  (exporting a way to invoke a `napi_callback`), not a local hack.
- **Registering with `fast_fn == NULL` returns a plain Node-API function** with
  no wrapper and no overhead. That is the portable-addon path, and it is why a
  single addon source can target every runtime.
- **Fast functions cannot be used as constructors**
  (`ConstructorBehavior::kThrow`): `new` would have to run the slow path with a
  `new.target` the forwarding cannot reproduce.
- **`CFunctionInfo` keeps its argument array by pointer**, and V8 keeps the
  `CFunctionInfo` by pointer for as long as the template lives, so both are
  interned per distinct signature and intentionally never freed — the same
  shape Node gets for free by declaring its `CFunctionInfo`s as file-scope
  statics.
- Not yet supported, in rough priority order: overloads (V8 resolves them by
  argument count), typed-array parameters as anything richer than an opaque
  handle, and exposing the `FastApiCallbackOptions` data pointer.
