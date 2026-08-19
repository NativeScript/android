// ModuleInternalCallbacks.h
#pragma once
#include <v8.h>

#include <functional>
#include <string>
#include <vector>

#include "HttpLoader.h"
#include "robin_hood.h"

namespace tns {

// Canonical module key → compiled-module handle map used by the per-isolate
// registries below.
using ModuleHandleMap =
    robin_hood::unordered_map<std::string, v8::Global<v8::Module>>;

// The registry key for `key`, whatever form it arrives in (filesystem path,
// file:// URL, http(s) URL, blob:, or a custom scheme such as node:). Every
// registry read and write goes through this, so a module reached as a root and
// the same module reached as someone's dependency land on one entry — and one
// identity for import.meta.
std::string CanonicalizeRegistryKey(const std::string& key);

// Per-isolate module registry accessor: map canonical keys → compiled
// v8::Module handles for `isolate`. Keyed by v8::Isolate* (not thread) because
// v8::Global<Module> handles are isolate-bound; see the long-form comment
// above the definition in ModuleInternalCallbacks.cpp for the
// cross-isolate-handle bug this prevents. The map lives in a RuntimeState
// slot, so this returns null once the isolate's teardown has begun — callers
// must bail.
ModuleHandleMap* ModuleRegistryFor(v8::Isolate* isolate);

// Mark every in-flight async graph load owned by `isolate` dead and Reset
// their context Globals. Must be called while the isolate is still alive (the
// Runtime destructor calls this before disposal); the rest of the loader state
// is destroyed with the isolate's RuntimeState.
void QuiesceModuleLoadsForIsolate(v8::Isolate* isolate);

// Utility to drop modules from the registry when compilation/instantiation
// fails. Operates on the *current* isolate's maps (resolved internally); only
// ever called on the isolate's own JS thread during module resolution/loading.
void RemoveModuleFromRegistry(const std::string& canonicalPath);

// The canonical registry key whose live entry is `mod`, or empty when the
// module is not registered for `isolate`. O(1) via the loader state's
// identity-hash index.
std::string LookupModuleKeyForModule(v8::Isolate* isolate,
                                     v8::Local<v8::Module> mod);

// Keep the identity-hash index in step with a registry write performed outside
// ModuleInternalCallbacks.cpp. Unindex first, while the key's outgoing module
// is still reachable — once its handle is Reset its hash is unrecoverable and
// the bucket entry would leak — then index the incoming one after the write.
void UnindexModuleForIsolate(v8::Isolate* isolate,
                             const std::string& canonicalKey);
void IndexModuleForIsolate(v8::Isolate* isolate, const std::string& canonicalKey,
                           v8::Local<v8::Module> mod);

// Authoritative HTTP URL loader for dev-served ESM. This compiles and
// registers the module under its canonical URL key without evaluating it.
v8::MaybeLocal<v8::Module> LoadHttpModuleForUrl(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& requestedUrl);

// ── Async HTTP module-graph pipeline ─────────────
//
// Standard three-phase module-map pipeline (the Node/Blink shape) under V8's
// synchronous ResolveModuleCallback: the sync constraint applies to
// *resolution*, not *fetching*. Starting from `rootUrl`, the walk fetches
// bodies concurrently off-thread (FetchModuleBodyAsync), compiles each on the
// isolate's JS thread (ScriptCompiler::CompileModule parses without
// resolving), resolves every static module request with the same import-map +
// relative-URL logic ResolveModuleCallback uses, and recurses until the
// transitive closure is compiled + registered. By InstantiateModule time the
// resolver is a pure registry lookup for the walked graph; anything the walk
// missed falls back to the legacy synchronous fetch inside the resolver.
//
// `onComplete(ok, errorMessage, context)` runs exactly once on the isolate's
// JS thread with the isolate entered and `context` (the context captured at
// start) already scoped. `ok` is false only when the ROOT fetch/compile
// failed — dependency failures are logged and left to surface through the
// resolver during instantiation, so the walk itself introduces no new
// failure modes.
void StartAsyncHttpModuleGraphLoad(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& rootUrl,
    std::function<void(bool ok, const std::string& errorMessage,
                       v8::Local<v8::Context> context)>
        onComplete);

// Synchronous wrapper for callers that need the graph ready before
// continuing (static HTTP entry loads): starts the walk, then pumps the
// current thread's Android Looper until it settles or `timeoutSeconds`
// elapses. Returns true when the walk completed (regardless of root success
// — the caller's own load path reports root failures). This is the "manual
// run loop until settled" boot handoff.
bool RunAsyncHttpModuleGraphLoadPumped(v8::Isolate* isolate,
                                       v8::Local<v8::Context> context,
                                       const std::string& rootUrl,
                                       double timeoutSeconds);

// True while any async graph load (any isolate) has fetches or compiles
// outstanding.
bool HasPendingAsyncModuleGraphWork();

// Drop exact URL-keyed modules from the registry and clear any in-flight
// invalidation bookkeeping tied to those canonical keys.
void InvalidateModules(v8::Isolate* isolate, v8::Local<v8::Context> context,
                       const std::vector<std::string>& urls);

// Diagnostics helper: returns URL-like keys currently loaded in the module
// registry.
std::vector<std::string> GetLoadedModuleUrls();

// Resolve callback signature (with import‑assertions slot)
v8::MaybeLocal<v8::Module> ResolveModuleCallback(
    v8::Local<v8::Context> context, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> import_assertions,
    v8::Local<v8::Module> referrer);

// Host callback for dynamic import() expressions
v8::MaybeLocal<v8::Promise> ImportModuleDynamicallyCallback(
    v8::Local<v8::Context> context, v8::Local<v8::Data> host_defined_options,
    v8::Local<v8::Value> resource_name, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> import_assertions);

// Host callback for import.meta initialization — Android-specific. Populates
// `import.meta.url` and `import.meta.dirname`. Kept here (not on iOS) because
// Runtime.cpp installs it via SetHostInitializeImportMetaObjectCallback. No
// `import.meta.hot` — that surface is JS policy, not native.
void InitializeImportMetaObject(v8::Local<v8::Context> context,
                                v8::Local<v8::Module> module,
                                v8::Local<v8::Object> meta);

// ── The loader vocabulary ─────────────────────────────────────
//
// Everything the dev client teaches one isolate's module loader: which bare
// specifiers resolve where, how URLs are keyed, and which URLs are never
// cached. Per-isolate, not process-wide — it lives in the isolate's loader
// state and dies with the isolate, so each isolate only ever reads and writes
// its own and nothing here needs synchronization. All of it must be set from
// the isolate's own thread.

// Parse and store an import map from JSON on the calling isolate. Expected
// shape: {"imports": {"key": "value", ...}}
void SetImportMap(const std::string& json);

// Set URL patterns that should bypass module cache (e.g. "/@ns/sfc/", "?v=")
// on the calling isolate.
void SetVolatilePatterns(const std::vector<std::string>& patterns);

// The calling isolate's canonicalization vocabulary, or null when it has none
// (the mechanical canonicalization applies). Isolate thread only — the
// transport never calls this, it carries canonical keys instead.
const CanonicalizationConfig* CanonicalizationConfigForCurrentIsolate();

// Install the client-supplied canonicalization vocabulary on the calling
// isolate. Its presence replaces the mechanical default entirely — empty
// vectors are honored as explicit policy.
void SetCanonicalizationConfig(CanonicalizationConfig config);

}  // namespace tns
