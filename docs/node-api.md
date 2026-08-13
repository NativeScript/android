# Node-API

The runtime implements [Node-API](https://nodejs.org/api/n-api.html), the ABI-stable C interface Node.js addons are written against. The implementation is Node v26.7.0's own `js_native_api` sources, vendored unmodified under `test-app/runtime/src/main/cpp/napi/vendor/`; only the pieces Node implements on top of libuv and its module loader are reimplemented here, against the runtime's per-runtime event loop and its own `require()`. The surface, the divergences and the vendored sources are shared byte-for-byte with the iOS runtime, so one addon source compiles against Node, the iOS runtime and this one.

An addon for this runtime is ordinary C or C++ compiled with the NDK — either linked into a library the app already loads, or built as its own `.so`. The addon registers itself from a static constructor and JS reaches it through `require("<name>")`.

```c
#include <node_api.h>

static napi_value Add(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  if (napi_get_cb_info(env, info, &argc, args, NULL, NULL) != napi_ok) {
    return NULL;
  }
  if (argc < 2) {
    napi_throw_type_error(env, NULL, "add expects two numbers");
    return NULL;
  }

  double a = 0, b = 0;
  if (napi_get_value_double(env, args[0], &a) != napi_ok ||
      napi_get_value_double(env, args[1], &b) != napi_ok) {
    napi_throw_type_error(env, NULL, "add expects two numbers");
    return NULL;
  }

  napi_value result = NULL;
  if (napi_create_double(env, a + b, &result) != napi_ok) {
    return NULL;
  }
  return result;
}

static napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor properties[] = {
      {"add", NULL, Add, NULL, NULL, NULL, napi_default, NULL},
  };
  napi_define_properties(env, exports, 1, properties);
  return exports;
}

static napi_module sModule = {
    NAPI_MODULE_VERSION, 0, __FILE__, Init, "myaddon", NULL, {0},
};

__attribute__((constructor)) static void RegisterMyModule(void) {
  napi_module_register(&sModule);
}
```

```js
const addon = require("myaddon");
addon.add(1, 2); // 3
```

`test-app/runtime/src/main/cpp/napi/tests/NapiTestModule.cpp` and `NapiCoverageModule.cpp` are complete working addons in this shape, and `test-app/app/src/main/assets/app/tests/NapiTests.js` / `NapiCoverageTests.js` are their specs.

## Building an addon (plugin authors)

Two pieces: headers come from the runtime's prefab package; linking follows the same convention native V8 plugins (e.g. `@nativescript/canvas`) already use.

**Headers.** The runtime `.aar` embeds a header-only [Prefab](https://google.github.io/prefab/) package. In the plugin's (or app's) Android library module:

```groovy
android {
    buildFeatures {
        prefab true
    }
}
```

and in its `CMakeLists.txt`:

```cmake
find_package(NativeScript REQUIRED CONFIG)
target_link_libraries(myaddon NativeScript::NativeScript)
```

That puts the headers on the include path — so the ecosystem-standard bare include, exactly as an addon for Node.js writes it, compiles unchanged:

```c
#include <node_api.h>
```

This is the portable form: the same source compiles against Node itself, the iOS runtime and this runtime, and it is required if you use the `node-addon-api` C++ wrapper (its `napi.h` hard-codes `#include <node_api.h>`). The exported headers are `node_api.h`, `node_api_types.h`, `js_native_api.h`, `js_native_api_types.h` and `NapiRuntime.h` (the NativeScript-specific `NativeScriptNapiEnv()` declaration, see below).

**Linking.** The prefab package is deliberately header-only, so the addon `.so` links against the runtime the way native V8 plugins do today: against a local copy of `libNativeScript.so` that exists *only* to satisfy the linker at build time and is never shipped.

1. Extract `libNativeScript.so` per ABI from the runtime `.aar` in the `@nativescript/android` npm package (`framework/app/libs/runtime-libs/nativescript-optimized.aar` → `jni/<abi>/`), into e.g. `src/main/libs/<abi>/`.
2. Link it: `target_link_libraries(myaddon ${CMAKE_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libNativeScript.so)`. This gives full link-time symbol checking and a `DT_NEEDED libNativeScript.so` entry.
3. Exclude the copy from the plugin's own packaging: `packagingOptions { jniLibs { excludes += "**/libNativeScript.so" } }`.

At runtime the addon's `DT_NEEDED` binds against the runtime library the app already loaded. Unlike direct-V8 plugins, a Node-API addon does **not** need the app to set `useV8Symbols`: every runtime flavor exports the full `napi_*`/`node_api_*` surface and `NativeScriptNapiEnv`.

## Registering a module

**Prefer constructor registration over the `NAPI_MODULE` / `NAPI_MODULE_INIT` macros.** The macros emit the exported symbols (`napi_register_module_v1`, `node_api_module_get_api_version_v1`) that a dlopen loader scans for — and the `.so` require path here does scan for them — but the symbols carry no module name (so a macro-registered addon is only reachable by path, never by bare `require("name")`) and only one of each can exist per binary.

The pattern that supports both loading forms is the one above: fill in a `napi_module` and call `napi_module_register` from a constructor.

- `nm_version` must be `NAPI_MODULE_VERSION`.
- `nm_modname` is the name JS passes to `require()`. It is the key in a process-wide registry, so it must be unique across every addon loaded into the app.
- `nm_register_func` is called once per environment, lazily, the first time that environment requires the module.
- `napi_module_register` is the only registration entry point. There is deliberately no `node_module_register` alias: Node's symbol of that name takes a `node_module*` with a different field layout, so accepting it would misread the struct rather than help old `NODE_MODULE`-style addons.

The constructor runs when the addon's library is loaded — at app start if the addon is linked into a library the app loads eagerly, or at the first `require()` of the `.so` (below) otherwise.

## Loading from JS

`require("myaddon")` returns the addon's exports. Resolution order inside `require()` is:

1. `ns:` / `node:` builtin modules,
2. registered Node-API addons — bare specifiers only, so `require("./myaddon")` or `require("~/myaddon")` will never reach an addon,
3. ordinary file and `node_modules` resolution.

An unregistered name is not a Node-API error; it falls through to step 3 and fails (or succeeds) as any other package name would.

An addon shipped as its own `.so` can also be loaded by path: `require("path/to/libmyaddon.so")` (or `require("system_lib://libmyaddon.so")` for a library packaged in the APK's `jniLibs`) `dlopen`s the library and initializes it Node's way — if its constructors registered a Node-API module, the call returns that module's exports (Node's `modpending` dance); otherwise, if the library carries the `napi_register_module_v1` symbol that the `NAPI_MODULE` / `node-addon-api` registration macros emit, that entry point is called instead, so stock ecosystem addons load unmodified by path. After a constructor-registered addon's first load, the bare `require("myaddon")` also resolves (macro-registered addons carry no name, so they are only reachable by path). Libraries that do neither keep the pre-existing `NSMain` protocol.

Exports are cached **per environment**, not per process. `require("myaddon") === require("myaddon")` within one isolate, but a `Worker` gets its own environment, runs `nm_register_func` again, and receives a different exports object with different native state. Anything an addon keeps in file-scope statics is shared across every environment in the process; anything it wants to keep per-environment belongs in `napi_set_instance_data`.

## The environment

Every runtime — the main one and each `Worker` — owns one `napi_env`. Native code that is not already inside a Node-API callback gets it from `NapiRuntime.h`:

```c
#include <NapiRuntime.h>

napi_env env = NativeScriptNapiEnv();
if (env != NULL) {
  // ...
}
```

The lookup is thread-local: it returns the env of the runtime running **on the calling thread**, and `NULL` on any other thread, before the runtime finishes initializing, or after it has torn down. There is no way to obtain another thread's env, by design — see below.

A `Worker`'s env is destroyed when the worker terminates: cleanup hooks run, threadsafe functions are aborted, and every finalizer fires. The **main** runtime's env lives for the process — nothing invokes the runtime's teardown path (`DestroyRuntime`, which is where env destruction is wired) for the main runtime today, so its cleanup hooks and instance-data finalizers only run if an embedder drives that path; on a normal Android app, process death does the reaping.

## Threading

**Every Node-API call must happen on the thread that owns the env.** That thread holds the isolate's lock and drives the looper; entering the isolate from anywhere else deadlocks against the runtime's cross-isolate locking. The API does not check this for you (`napi_make_callback` and blocking threadsafe-function calls are the only two that do), so a wrong-thread call is undefined behaviour rather than an error status.

Two supported ways to get work off that thread:

- **`napi_create_async_work` / `napi_queue_async_work`** for background compute. The `execute` callback runs on a shared worker pool bounded at 4 concurrent tasks — matching Node's default libuv pool, so an addon that fans out more than 4 *interdependent* blocking executes deadlocks here exactly as it would on Node — and must not touch the isolate or the `napi_env` at all; the `complete` callback is posted to the env's event loop and may. The pool threads are plain native threads, not attached to the JVM. Every completion entry ends with a microtask checkpoint, so a promise resolved from `complete` settles promptly even if nothing else enters JS. Note the divergence on `napi_delete_async_work` below.
- **Threadsafe functions** for calling into JS from a thread you own. `napi_create_threadsafe_function` on the JS thread, then `napi_call_threadsafe_function` from anywhere. Calls are queued and drained on the env's event loop, at most 64 per entry so a fast producer cannot starve timers or the UI.

Finalizer drains, threadsafe-function callbacks and async-work completions ride the runtime's event loop (the internal lane added in the per-runtime EventLoop work), which drops everything still queued when the runtime shuts down — so work in flight when a `Worker` terminates is dropped rather than delivered to a dead isolate. For async work specifically: an `execute` that has not started when the worker terminates is skipped, an in-flight completion is dropped, and the work item is parked in the completed state so a cleanup hook's `napi_delete_async_work` still succeeds; the addon's `data` for dropped items is never handed back (the same trade Node makes at environment shutdown).

An exception thrown by JS during one of these entries is always **contained**: it is routed through the runtime's error pipeline (`error` event, then the uncaught-error hooks). It never propagates out of the entry — including under `uncaughtErrorPolicy: "throw"`, where the error is still fully reported but there is no native caller beneath a loop entry to hand it to.

## Finalizers

Finalizers registered through `napi_wrap`, `napi_add_finalizer`, `napi_create_external` and friends **never run during garbage collection**. V8's weak callback only enqueues them; they are drained on a later entry of the owning thread's event loop, with the isolate locked and entered, so a finalizer may call back into JS. The drain shares the loop's internal lane with V8's own tasks, so its order relative to timers is not specified — code waiting for a finalizer should poll, not count looper turns.

Practically this means code that drops the last reference to a wrapped object and immediately checks whether the finalizer ran will always see "no" — it has to yield to the looper first. In the test runner, where `__collect()` forces a collection:

```js
__collect();
(function poll() {
  if (!finalizerRan()) {
    setTimeout(poll, 0);
    return;
  }
  // the finalizer has run
})();
```

At environment teardown the queue is flushed synchronously instead, so nothing is left unfinalized when a `Worker` exits.

External `ArrayBuffer`s (`napi_create_external_buffer`, `napi_create_external_arraybuffer`) hand the pointer to V8 without copying, and V8's backing-store deleter can fire on any thread — so the deleter never runs the callback itself. The finalizer is registered with the env: the deleter posts it to the event loop, and anything still unclaimed when the environment tears down runs in the teardown sweep, while the env is alive. Either way it runs exactly once, on the env's thread.

## Versions

`napi_get_version` reports **10**, the highest Node-API version the vendored sources implement.

Separately, every env is created at **module API version 8**, which is what determines *behaviour*. An addon that includes the headers without asking for anything else also compiles at `NAPI_VERSION 8`, so declarations and behaviour line up by default. Two things follow:

- An addon can opt into the version 9 and 10 *declarations* by defining `NAPI_VERSION` before including the headers (`node_api_symbol_for`, `node_api_create_syntax_error`, the `node_api_create_property_key_*` family, external strings). Those functions are implemented and work.
- It cannot opt into version 10 *semantics*, because the env is fixed at 8. The one that bites in practice is `napi_create_reference`: at version 8 it only accepts objects, functions and symbols, and returns `napi_invalid_arg` for strings, numbers, booleans, `null` and `undefined`. Version 10 would allow all of them.

`napi_get_node_version` reports `26.7.0`, the Node release the sources were taken from. It says nothing about a Node process being present — there isn't one.

## Divergences

Everything in `js_native_api.h` behaves exactly as upstream: it is upstream, compiled unmodified. The differences are confined to the `node_api.h` surface, where Node's implementation depends on libuv, `node::Buffer` or the module loader — and they are identical to the iOS runtime's, so an addon written against one runtime's divergence table holds on the other.

| API | Behaviour here | Why |
| --- | --- | --- |
| `napi_get_uv_event_loop` | Always `napi_generic_failure` | There is no `uv_loop_t`. The runtime drives an Android Looper; handing out a fabricated loop would be worse than reporting failure. |
| `node_api_get_module_file_name` | Always `napi_generic_failure` | Nothing identifies the calling module at that point — addons are either linked into a larger library or dlopened by the runtime without per-module bookkeeping. |
| `napi_create_buffer`, `napi_create_buffer_copy`, `napi_create_external_buffer`, `node_api_create_buffer_from_arraybuffer` | Produce a plain `Uint8Array` | There is no `node::Buffer` class. `napi_get_buffer_info` returns the same thing either way. |
| `napi_is_buffer` | Exactly "is this a `Uint8Array`" | Follows from the above. An `Int8Array` is not a buffer; a `Uint8Array` from any source is. |
| `napi_ref_threadsafe_function`, `napi_unref_threadsafe_function` | Track the flag, affect nothing; always `napi_ok` | The looper belongs to the app or the worker and does not exit because an addon released its last reference. |
| `napi_call_threadsafe_function`, blocking mode, called on the env's own thread with a full queue | `napi_would_deadlock` instead of blocking | The thread that would have to drain the queue is the caller. Node blocks regardless and never returns this status; wedging the looper is worse than a status an addon may not expect. Non-blocking calls still return `napi_queue_full`, and a closed function still returns `napi_closing`. |
| `napi_delete_async_work` on work that is queued or executing | `napi_generic_failure`, work not deleted | Node deletes unconditionally and leaves the queued work holding a dangling pointer. Deleting after a *successful* cancel, or after completion, works as usual. |
| `napi_add_async_cleanup_hook` | Hook runs at teardown but is **not awaited** | The thread running teardown is the one that would have to run the completion, so a hook that defers is never given the chance to report back. Its handle stays valid, so a late `napi_remove_async_cleanup_hook` is still safe, but it can no longer reach JS. |
| `napi_fatal_exception` | Reports through the runtime's error pipeline (`error` event, uncaught-error hooks, `uncaughtErrorPolicy`) and returns; does not abort by itself | `napi_fatal_error` still aborts the process, as upstream. |
| `napi_async_init`, `napi_open_callback_scope` | Accepted; the resource and name arguments are inert | There is no `async_hooks`, so there is nothing to report the async context to. `napi_make_callback` itself works, and rejects a call from the wrong thread with `napi_generic_failure`. |

## Not supported

`node_api.h` is the entire native surface. The Node **runtime** is not here: no `process`, no `fs`, no `require()` of Node core modules from native, no libuv handles, no worker_threads C API. An addon that only uses Node-API works; an addon that reaches into `node.h`, `v8.h` or `uv.h` does not.
