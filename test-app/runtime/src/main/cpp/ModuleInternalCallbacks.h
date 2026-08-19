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

// The require(esm) exports facade: a synthetic source-text module that
// re-exports everything from `target` and adds `__esModule = true`, so
// transpiled CJS consumers (`_mod.__esModule ? _mod.default : _mod`) pick up a
// real ESM default export through require(). Re-exports keep the target's live
// bindings and enumerability, which a copied object would not. Returns the
// facade instantiated and evaluated; the caller takes GetModuleNamespace().
// One facade per target module, cached until the target leaves the registry.
v8::MaybeLocal<v8::Module> GetOrCreateRequireFacade(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    v8::Local<v8::Module> target, const std::string& targetCanonicalPath);

// Authoritative HTTP URL loader for dev-served ESM. This compiles and
// registers the module under its canonical URL key without evaluating it.
v8::MaybeLocal<v8::Module> LoadHttpModuleForUrl(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& requestedUrl);

// ── The module-graph walk ────────────────────────
//
// Standard three-phase module-map pipeline (the Node/Blink shape) under V8's
// synchronous ResolveModuleCallback: the sync constraint applies to
// *resolution*, not *fetching*. Starting from `root` (an absolute http(s) URL
// or a canonical filesystem path), the walk discovers the transitive closure
// and compiles + registers every module in it, so that by InstantiateModule
// time the resolver is a pure registry lookup.
//
// Discovery is scheme-agnostic; only the fetch is per-scheme. Every edge goes
// through the same resolution the resolver uses (ResolveSpecifierToPath), so
// both agree on a module's registry key:
//   - http(s) edges are fetched concurrently off-thread
//     (FetchModuleBodyAsync) and compiled on the isolate's JS thread;
//   - local edges are read and compiled inline during the walk — the bytes
//     are already on disk, and a thread hop would only reorder discovery;
//   - builtins are left to the resolver, which serves them from the builtin
//     registry;
//   - specifiers the walk cannot resolve (typically a bare name with no
//     import-map entry) stay on the resolver's lazy path.
//
// Compilation runs no user code, so pre-compiling the closure cannot change
// evaluation order: V8 still evaluates in spec order from the root.
//
// `onComplete(ok, errorMessage, context)` runs exactly once on the isolate's
// JS thread with the isolate entered and `context` (the context captured at
// start) already scoped. `ok` is false only when an HTTP ROOT fetch/compile
// failed. Every other failure — a dependency, or anything local including the
// root — is left unregistered for the resolver (or the caller's own load
// path) to report with its own message, so the walk introduces no new failure
// modes and steals no error text.
void StartModuleGraphLoad(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& root,
    std::function<void(bool ok, const std::string& errorMessage,
                       v8::Local<v8::Context> context)>
        onComplete);

// Synchronous wrapper for callers that need the graph ready before
// continuing: starts the walk, then pumps the current thread's Android Looper
// until it settles or `timeoutSeconds` elapses. A graph with no http(s) edges
// completes entirely inside StartModuleGraphLoad, so this returns without
// entering the wait loop at all — a disk-only load pays no looper slice.
// Returns true when the walk completed (regardless of root success — the
// caller's own load path reports root failures).
bool RunModuleGraphLoadPumped(v8::Isolate* isolate,
                              v8::Local<v8::Context> context,
                              const std::string& root, double timeoutSeconds);

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

// Import map support.
//
// Shape: {"imports": {"specifier": "target", ...},
//         "scopes": {"<referrer-key-prefix>": {imports-shaped map}, ...}}
//
// Parsed and validated in full before anything is installed: on any invalid
// input this returns false with `error` explaining which key or section is
// wrong, and the calling isolate's currently installed map is left untouched.
// Per-isolate like the rest of the loader vocabulary — a worker resolves
// through the copy taken at spawn.
bool SetImportMap(const std::string& json, std::string* error);

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
