#ifndef MODULE_INTERNAL_CALLBACKS_H
#define MODULE_INTERNAL_CALLBACKS_H

#include "v8.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace tns {

// Per-isolate ES module registry. `thread_local`: each NS isolate (main thread +
// each Worker thread) gets its own per-thread map, because v8::Global<Module>
// handles are isolate-bound — sharing one map across isolates would let one
// thread try to read another thread's handle and trip V8's identity-check on
// instantiation. See the long-form comment above the definition in
// ModuleInternalCallbacks.cpp for the cross-isolate-handle bug this prevents.
extern thread_local std::unordered_map<std::string, v8::Global<v8::Module>>& g_moduleRegistry;

// Import-map and volatile-pattern configuration.
//
// `SetImportMap` accepts the dev server's JSON import-map blob (parsed and
// merged into the process-wide bare-specifier → URL map used by
// `ResolveModuleCallback`). `SetVolatilePatterns` accepts a list of URL
// substrings that should always re-fetch (never serve from the
// speculative-prefetch cache). Both are applied via `__nsConfigureRuntime`
// / `__nsConfigureDevRuntime` at session start and again at every HMR
// graph version bump.
void SetImportMap(const std::string& json);
void SetVolatilePatterns(const std::vector<std::string>& patterns);

// Drop all per-process import-map / vendor / in-flight state.
// Called from `Runtime::~Runtime()` on the MAIN isolate only — workers
// share the process-wide registries but the main isolate owns their
// lifetime, so wiping them on a worker would race with the next
// re-launched main isolate.
void CleanupImportMapGlobals();

// Snapshot the keys currently registered in `g_moduleRegistry` (file paths +
// canonical HTTP URLs). Used by `CollectSessionModuleUrls` to enumerate
// modules the dev session needs to invalidate.
std::vector<std::string> GetLoadedModuleUrls();

// Evict the given keys (canonical registry keys) from `g_moduleRegistry`.
// No-op if the key is missing. Used by `__nsInvalidateModules` and by
// the HMR cycle to drop stale modules before re-importing.
void RemoveModuleFromRegistry(const std::string& canonicalKey);

// Drop a list of keys + their HTTP cache entries in one pass. Returns the
// number of registry entries removed.
size_t InvalidateModules(const std::vector<std::string>& keys);

// Compile + register-only path used by the speculative HTTP loader so that
// a module can be cached without being instantiated/evaluated. The caller
// is responsible for instantiation + evaluation on the JS thread.
v8::MaybeLocal<v8::Module> LoadHttpModuleForUrl(v8::Isolate* isolate,
                                                v8::Local<v8::Context> context,
                                                const std::string& url,
                                                std::string* errorMessage);

}  // namespace tns

// Module resolution callback for ES modules
v8::MaybeLocal<v8::Module> ResolveModuleCallback(v8::Local<v8::Context> context,
                                                 v8::Local<v8::String> specifier,
                                                 v8::Local<v8::FixedArray> import_assertions,
                                                 v8::Local<v8::Module> referrer);

// InitializeImportMetaObject - Callback invoked by V8 to initialize import.meta object
void InitializeImportMetaObject(v8::Local<v8::Context> context, 
                                v8::Local<v8::Module> module, 
                                v8::Local<v8::Object> meta);

// Dynamic import() host callback
v8::MaybeLocal<v8::Promise> ImportModuleDynamicallyCallback(
    v8::Local<v8::Context> context, v8::Local<v8::Data> host_defined_options,
    v8::Local<v8::Value> resource_name, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> import_assertions);

// Helper functions
bool IsFile(const std::string& path);
std::string WithExtension(const std::string& path, const std::string& ext);
bool IsNodeBuiltinModule(const std::string& spec);
std::string GetApplicationPath();

#endif // MODULE_INTERNAL_CALLBACKS_H
