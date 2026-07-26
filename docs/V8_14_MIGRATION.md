# V8 10.3 → 14.9 migration notes (Android)

Pinned version: **14.9.207.39** (`branch-heads/14.9`), see `tools/v8/fetch_v8.sh`.
Built with `tools/v8/build_v8_source.sh`. The iOS runtime moved to the same
version; where the two runtimes hit the same API change the notes are kept in
sync.

## What ships

`libv8_monolith.a` per ABI under `test-app/runtime/src/main/libs/<abi>/`, plus
the public headers under `test-app/runtime/src/main/cpp/include/` and the
vendored V8 internals under `test-app/runtime/src/main/cpp/v8_inspector/`.
All three come out of `build_v8_source.sh`; they are a matched set and must be
updated together.

## Build configuration

The gn args reproduce the 10.3 build: JIT and WebAssembly on, i18n off. What is
new, and why:

- **`use_allocator_shim=false`** — the shim interposes on `malloc` through
  linker `--wrap` flags that the embedder would also have to pass. Without it
  the link fails on `__real_realpath` / `__real_getcwd`. PartitionAlloc itself
  stays enabled (V8 depends on the target); it just no longer replaces malloc.
- **`use_thin_lto=false`** — `is_official_build` now turns ThinLTO on, and a
  ThinLTO build emits LLVM bitcode rather than object code, which the app's NDK
  cannot link.
- **`chrome_pgo_phase=0`** — `is_official_build` also turns PGO on, and
  standalone V8 has no `tools/update_pgo_profiles.py`.
- **`v8_enable_temporal_support=false`** — Temporal is implemented in Rust and
  pulls in a Rust sysroot this build does not link. 10.3 had no Temporal.
- **`v8_array_buffer_internal_field_count=2`** and
  **`v8_array_buffer_view_internal_field_count=2`** — both default to `0` in
  14.9 and defaulted to `2` in 10.3. When an `ArrayBuffer`/`SharedArrayBuffer`/
  typed array is marshalled to a Java NIO buffer,
  `JSToJavaObjectsConverter` calls `ObjectManager::Link` on the buffer object
  itself, and `Link` stores its `JSInstanceInfo` in internal field 0. With zero
  internal fields every such conversion throws *"Trying to link invalid 'this'
  to a Java object"*. `v8-array-buffer.h` still falls back to `2` when the macro
  is undefined, which is what the runtime compiles against, so leaving the gn
  default in place also puts the two sides out of agreement.
- **`v8_enable_sandbox=false`** — pinned rather than left to default. It would
  otherwise follow pointer compression (on for 64-bit) and change the object
  layout the runtime compiles against, which is a much larger change than a
  version bump.
- **`android_ndk_root`** pinned to the NDK the runtime is built with — see
  below.

`build_v8_source.sh` deletes the output directory before each build. ninja never
removes outputs orphaned by a config change, so reusing one across V8 versions
silently keeps stale objects, and the packaging step would vendor them.

### The NDK has to match on both sides

V8's bundled NDK (CIPD `30.0.14608247`) is newer than any released one. Its
libc++ exports symbols the runtime's `libc++_static.a` does not — the link fails
on `std::__ndk1::__hash_memory`, referenced from `liveedit.cc`. `libc++` is only
ABI-compatible with itself across a static link, so V8 is built against the same
NDK the runtime uses, via the `android_ndk_root` gn arg (made overridable by
`android_build.patch`).

**The runtime moved from NDK r27d to r29.** This is forced, not optional: V8
14.9's `src/base/atomicops.h` uses `std::atomic_ref` unconditionally, and
r27d ships libc++ 18, which does not implement it (`__cpp_lib_atomic_ref` is
commented out in its `<version>`). The runtime compiles those headers because
`v8_inspector` vendors V8 internals. r29 is the first released NDK with a
libc++ new enough. `minSdk` is unchanged at 21.

### Building on macOS

Chromium asserts a Linux host for Android targets. Everything below that assert
still handles macOS — the host-arch block maps arm64 hosts to the
`darwin-x86_64` NDK tag deliberately, and `android_toolchain_root` is only read
for the (host-independent) sysroot — so `android_build.patch` relaxes
the assert. It also lowers `min_supported_sdk_version` from 23 to 21; that floor
exists for Java/dex tooling and this build produces only the native
`v8_monolith` target.

Two things the macOS clang package does not carry, both handled by `fetch_v8.sh`:

- the Android **compiler-rt builtins** (`libclang_rt.builtins-*-android.a`),
  which only the Linux clang package bundles — they are extracted from it;
- a `darwin-x86_64` directory in the CIPD NDK, which only ships `linux-x86_64` —
  symlinked (only relevant when `android_ndk_root` is left at its default).

Because two of the four ABIs cannot be rebuilt locally, the gradle builds accept
`-Pabis=arm64-v8a,x86_64` to restrict `abiFilters` to the ones that have a
current `libv8_monolith.a`.

**The 32-bit ABIs cannot be built on an Apple Silicon host.** `armeabi-v7a` and
`x86` need mksnapshot to run V8's simulator for a 32-bit target, and
`v8config.h` hard-errors with *"Target architecture arm is only supported on arm
and ia32 host"*. The Linux x64 path is unaffected — it builds mksnapshot as a
32-bit x86 host binary — so those two ABIs have to come from a Linux x64
builder, which is where CI builds them anyway.

## API changes applied to the runtime

| Change | Sites | Migration |
|---|---|---|
| `Context/Object/Function/Promise/Message::GetIsolate()` removed | 46 | `v8::Isolate::GetCurrent()` |
| `External::New` / `External::Value()` take a type tag | 47 | `v8::kExternalPointerTypeTagDefault` |
| `PropertyCallbackInfo::This()` removed | 25 | `Holder()`, or a function-backed accessor — see below |
| `Object/ObjectTemplate/Function::SetAccessor` | 21 | `SetNativeDataProperty` / `SetAccessorProperty` |
| Accessor callbacks take `Local<Name>` | 23 | was `Local<String>` |
| `Object::CreationContext()` | 7 | `GetCreationContext(isolate).ToLocalChecked()` |
| `ScriptOrigin` no longer takes an `Isolate*` | 8 | drop the first argument |
| `AccessControl` removed | 5 | drop the argument |
| `GetInternalField` returns `Local<Data>` | 4 | `.As<v8::Value>()` |
| Interceptor callbacks return `v8::Intercepted` | 2 | see below |
| `SetIndexedPropertyHandler` | 1 | `SetHandler(IndexedPropertyHandlerConfiguration(...))` |
| `V8Inspector::connect` needs a trust level | 3 | `kFullyTrusted` |
| `V8ConsoleMessage::createForConsoleAPI` takes a span | 2 | `{args.data(), args.size()}` |
| `FunctionCallbackInfo` is no longer copyable | 1 | `ArgsWrapper` holds a reference |

`unistd.h` also has to be included explicitly in `ModuleInternal.cpp` and
`WorkerWrapper.cpp`; `usleep`/`read` used to arrive transitively through headers
that no longer pull it in.

### Accessors that are inherited need a real accessor pair

`PropertyCallbackInfo` no longer exposes the receiver at all, and
`SetNativeDataProperty` is not a drop-in replacement for `SetAccessor` on
anything that is inherited from. Six accessors in `MetadataNode` are installed
on an object other than the one they are read through:

- on the **constructor function**, which derived constructors inherit:
  `class`, `nullObject`, and static fields;
- on the **implementation object**, which instances inherit: `super`;
- on the **prototype template**, which instances inherit: instance fields and
  properties.

All six are now `SetAccessorProperty` with `FunctionTemplate`-backed
getter/setter, whose `FunctionCallbackInfo::This()` still returns the receiver.
Static fields matter most: they have a setter, and `SetNativeDataProperty`
installs something data-like, so `Derived.baseField = x` would shadow the base's
property with an own data property and never reach the native setter.

The accessors that stay `SetNativeDataProperty` are the ones installed as own
properties on the object they are read through — the array wrapper's `length`,
the package object's children, inner types on a constructor, and the `URL*`
instance templates. For those `Holder() == This()`.

The rule of thumb: if an accessor lives on anything that is inherited from, use
`SetAccessorProperty` with function-backed callbacks. `SetNativeDataProperty` is
only safe where nothing inherits it.

This costs an allocation the old API did not: each converted accessor now needs
a real `Function` object rather than an `AccessorInfo`. On the prototype
template that is deferred to instantiation, but static fields are materialised
eagerly, so a class with many static fields pays for them when its constructor
function is first built.

#### One guard could not be translated directly

`FieldAccessorGetter/SetterCallback` used `thiz->StrictEquals(info.Holder())` to
detect an instance field being read straight off the prototype. Function-backed
accessors have no holder, so the check is now
`!objectManager->IsJsRuntimeObject(thiz)` — being a runtime-managed object is
the property that actually distinguishes an instance from the prototype. The
two agree for every receiver the old check could see; the new one additionally
returns `undefined` (rather than reaching `GetJavaField` with a non-instance)
for something like `Object.create(SomeClass.prototype).field`.

### Interceptors

Only one interceptor pair exists here — the array wrapper's indexed
getter/setter. Both handle the access completely, so both return
`Intercepted::kYes`; neither ever fell through to the ordinary lookup. The
setter's `PropertyCallbackInfo` also changed from `<Value>` to `<Boolean>`, so
its old `GetReturnValue().Set(value)` is dropped — the return value is now the
strict-mode success flag, not the stored value.

The conversion rule in general: a path that set a return value or threw becomes
`kYes`; a path that returned without setting one, **including falling off the
end**, becomes `kNo`. Getting it backwards is silent in both directions.

### V8 flags must be set before `V8::Initialize()`

`V8::Initialize()` calls `FlagList::FreezeFlags()`, and changing a flag
afterwards aborts the process. `PrepareV8Runtime` used to apply
`Constants::V8_STARTUP_FLAGS` per isolate, after initialization; it now happens
once in `InitializeV8()`, before `V8::Initialize()`. `Runtime::Init` has already
read the flags out of the Java config by then.

## Things that did *not* need changing

- **Resurrecting finalizers.** `ObjectManager` uses
  `WeakCallbackType::kFinalizer` in four places. Upstream removed it right after
  10.3.22; `tools/v8/v8_resurrecting_finalizers.patch` restores it. See the iOS
  runtime's `V8_RESURRECTING_FINALIZERS.md` for the patch design.
- **Teardown disposal.** The runtime never used
  `Isolate::VisitHandlesWithClassIds` or `SetWrapperClassId`, so the registry
  the iOS runtime had to grow is not needed here.

## Test status

The runtime test suite on an API 35 arm64 emulator: **594 tests, 0 failures,
0 errors, 5 skipped.** All five skips are pre-existing `xit()` in the checked-in
suite (`testNativeModules`, `exceptionHandlingTests` SIGABRT, `testArrays`
memory leak, `TNS require` index.json, `TNS Workers` circular postMessage).

### Inspector

Verified end to end against the debug build over the runtime's WebSocket
(`<package>-inspectorServer`, an abstract local socket): `Runtime.enable`
(execution context creation and replay of stored console messages, with stack
traces), `Runtime.evaluate` for arithmetic, object serialisation and Java
interop, `Debugger.enable`, and exception reporting through `exceptionDetails`.
This is the part most exposed to a stale `v8_inspector` tree, since it compiles
against V8 internals rather than the public API.

## Known follow-ups

- `armeabi-v7a` and `x86` still hold 10.3 libraries; they need a Linux x64
  builder (see above).
- **`V8_STATIC_ROOTS` is deliberately not defined by the runtime.** V8 is built
  with it, and it would let `Value::IsUndefined()`/`IsNull()` and friends use
  the inline static-root comparison instead of reading the map. It is left off
  because the root addresses are hardcoded constants in `v8-internal.h` that
  must match the library exactly (`kBuildDependentTheHoleValue` even varies with
  `V8_ENABLE_WEBASSEMBLY`), and 10.3 had no such fast path either — so this is
  parity, and enabling it is a measurable but separate change.
- On-device compiled-code caches written by 10.3 are stale. `TryLoadScriptCache`
  validates them only by comparing mtimes with the `.js` file, so an app update
  (which rewrites the scripts) discards them and they are regenerated. If the
  mtimes did happen to match, V8 rejects the data by its own version hash and
  recompiles from source — correct, but `SaveScriptCache` is only called on the
  no-cache path, so that file would never be refreshed. Pre-existing behaviour,
  newly reachable on a version bump.
- Maglev is enabled by default in 14.9 and did not exist in 10.3, so the
  monolith carries a whole extra compiler tier. `v8_enable_maglev=false` is the
  lever if the size matters more than the warm-up performance.
