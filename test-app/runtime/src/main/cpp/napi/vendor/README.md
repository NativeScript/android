# Vendored Node-API sources

Node.js's engine-independent Node-API implementation and its public headers,
copied here verbatim so plugins get the standard ABI without this runtime
reimplementing it.

- Source: https://github.com/nodejs/node
- Tag: `v26.7.0`
- Commit: `b4f23d3619c98bed09af93a21192f6080197a8c6`

Files, all from `src/` at that commit:

| File | Role |
| --- | --- |
| `js_native_api.h` | Engine-independent public API |
| `js_native_api_types.h` | Public types shared by both headers |
| `js_native_api_v8.h` | `napi_env__` and the V8 implementation's internals |
| `js_native_api_v8.cc` | The implementation |
| `node_api.h` | Node-specific public API (modules, buffers, async) |
| `node_api_types.h` | Node-specific public types |

`src/node_api.cc` is deliberately *not* vendored: it is bound to Node's event
loop and environment. Its role is filled by `../NodeApiEmbed.cpp`,
`../NapiThreadSafeFunction.cpp` and `../NapiEnv.cpp`, and the idioms
`js_native_api_v8.cc` expects from Node's
internal headers are supplied by `../shim/`.

## Local deviations

None. Every file is byte-identical to upstream (and to the copy vendored in
the iOS runtime).

One build-level accommodation keeps it that way:

- `../shim/env-inl.h` includes `node_api.h`, because
  `napi_create_external_arraybuffer` calls `napi_create_external_buffer`
  without including a header for it; upstream that declaration arrives
  transitively through Node's `env.h`.

## Re-syncing

```sh
V=v26.7.0
for f in js_native_api.h js_native_api_types.h js_native_api_v8.h js_native_api_v8.cc node_api.h node_api_types.h; do
  curl -sSfo "test-app/runtime/src/main/cpp/napi/vendor/$f" "https://raw.githubusercontent.com/nodejs/node/$V/src/$f"
done
```

Then update the tag and commit above, re-check `../shim/` against upstream's
`src/js_native_api_v8_internals.h` and `src/node_version.h` (the shim mirrors
`NODE_API_SUPPORTED_VERSION_MAX/MIN` and
`NODE_API_DEFAULT_MODULE_API_VERSION`), and cover any new `node_api.h` entry
point in `../NodeApiEmbed.cpp` (or `../NapiThreadSafeFunction.cpp`, which owns
the threadsafe function surface) — nothing declared there may be left
undefined.
