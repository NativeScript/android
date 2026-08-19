// ModuleInternalCallbacks.cpp
#include "ModuleInternalCallbacks.h"

#include <android/looper.h>
#include <sys/stat.h>
#include <v8.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <time.h>
#include <vector>

#include "ArgConverter.h"
#include "Constants.h"
#include "EventLoop.h"
#include "HttpLoader.h"
#include "JEnv.h"
#include "ModuleInternal.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "NativeScriptPlatform.h"
#include "NsBuiltinModules.h"
#include "Runtime.h"
#include "RuntimeState.h"
#include "TraceLog.h"
#include "Util.h"
#include "robin_hood.h"

using namespace v8;
using namespace std;
using namespace tns;

namespace tns {

// ─────────────────────────────────────────────────────────────
// Small string helpers (kept file-local — used everywhere below).

static inline bool StartsWith(const std::string& s, const char* prefix) {
  size_t n = strlen(prefix);
  return s.size() >= n && s.compare(0, n, prefix) == 0;
}

static inline bool EndsWith(const std::string& value, const std::string& suffix) {
  if (suffix.size() > value.size()) return false;
  return std::equal(suffix.rbegin(), suffix.rend(), value.rbegin());
}

// Filesystem: `path` names an existing regular file.
static bool IsFile(const std::string& path) {
  struct stat st;
  if (stat(path.c_str(), &st) != 0) {
    return false;
  }
  return (st.st_mode & S_IFMT) == S_IFREG;
}

// Append `ext` if `path` doesn't already carry it.
static std::string WithExtension(const std::string& path, const std::string& ext) {
  if (path.size() >= ext.size() &&
      path.compare(path.size() - ext.size(), ext.size(), ext) == 0) {
    return path;
  }
  return path + ext;
}

// Application filesystem root for on-disk .mjs/.js resolution.
// Mirrors Module.java's getApplicationFilesPath + "/app". Cached after first
// JNI call — the value is process-stable, and re-entering JNI on every
// resolver hit would add avoidable overhead to hot module-graph walks.
static std::string GetApplicationPath() {
  static std::string cached;
  static std::once_flag flag;
  std::call_once(flag, []() {
    JEnv env;
    jstring applicationFilesPath = (jstring)env.CallStaticObjectMethod(
        ModuleInternal::MODULE_CLASS,
        ModuleInternal::GET_APPLICATION_FILES_PATH_METHOD_ID);
    if (applicationFilesPath != nullptr) {
      cached = ArgConverter::jstringToString(applicationFilesPath) + "/app";
    }
  });
  return cached;
}

// Collapse "." and ".." segments, preserving a leading "/".
static std::string NormalizeDotSegments(const std::string& path) {
  std::vector<std::string> stack;
  bool absolute = !path.empty() && path[0] == '/';
  size_t i = 0;
  while (i <= path.size()) {
    size_t j = path.find('/', i);
    std::string seg = (j == std::string::npos) ? path.substr(i) : path.substr(i, j - i);
    if (seg.empty() || seg == ".") {
      // skip
    } else if (seg == "..") {
      if (!stack.empty()) stack.pop_back();
    } else {
      stack.push_back(std::move(seg));
    }
    if (j == std::string::npos) break;
    i = j + 1;
  }
  std::string norm = absolute ? "/" : std::string();
  for (size_t k = 0; k < stack.size(); k++) {
    if (k > 0) norm += "/";
    norm += stack[k];
  }
  return norm;
}

// Normalize a filesystem path: collapse duplicate slashes, "./" and "../"
// segments. Same intent as iOS's `stringByStandardizingPath`, minus the
// Foundation dependency (no HOME expansion, which we never used anyway).
static std::string NormalizePath(const std::string& path) {
  if (path.empty()) return path;
  return NormalizeDotSegments(path);
}

// Convert a file:// URL to a filesystem path. Handles both file:///a/b and
// file:/a/b variants. Percent-decoding is deliberately omitted — the runtime
// only emits ASCII file:// URLs internally.
static std::string FileURLToPath(const std::string& url) {
  if (url.empty()) return url;
  if (!StartsWith(url, "file://")) return url;
  std::string tail = url.substr(7);
  // Strip host component when present (file://host/path → /path). NS never
  // emits a host, but be tolerant.
  if (!tail.empty() && tail[0] != '/') {
    size_t slash = tail.find('/');
    tail = (slash == std::string::npos) ? std::string() : tail.substr(slash);
  }
  // Drop query and fragment — these have no meaning for filesystem paths.
  size_t cut = tail.find_first_of("?#");
  if (cut != std::string::npos) tail = tail.substr(0, cut);
  return NormalizePath(tail);
}

// Resolve a relative or root-absolute spec against an HTTP(S) referrer URL.
// Returns empty string if resolution is not applicable.
static std::string ResolveHttpRelative(const std::string& referrerUrl,
                                       const std::string& spec) {
  if (referrerUrl.empty()) return std::string();
  if (!(StartsWith(referrerUrl, "http://") || StartsWith(referrerUrl, "https://"))) {
    return std::string();
  }
  // Normalize referrer: drop fragment and query.
  std::string base = referrerUrl;
  size_t hashPos = base.find('#');
  if (hashPos != std::string::npos) base = base.substr(0, hashPos);
  size_t qPos = base.find('?');
  if (qPos != std::string::npos) base = base.substr(0, qPos);

  size_t schemePos = base.find("://");
  if (schemePos == std::string::npos) return std::string();
  size_t pathStart = base.find('/', schemePos + 3);
  std::string origin = (pathStart == std::string::npos) ? base : base.substr(0, pathStart);
  std::string path = (pathStart == std::string::npos) ? std::string("/")
                                                      : base.substr(pathStart);

  std::string specPath = spec;
  std::string specSuffix;
  size_t specQ = specPath.find('?');
  size_t specH = specPath.find('#');
  size_t cut = std::string::npos;
  if (specQ != std::string::npos && specH != std::string::npos) {
    cut = std::min(specQ, specH);
  } else if (specQ != std::string::npos) {
    cut = specQ;
  } else if (specH != std::string::npos) {
    cut = specH;
  }
  if (cut != std::string::npos) {
    specSuffix = specPath.substr(cut);
    specPath = specPath.substr(0, cut);
  }

  std::string newPath;
  if (!specPath.empty() && specPath[0] == '/') {
    newPath = specPath;
  } else {
    size_t lastSlash = path.find_last_of('/');
    std::string baseDir = (lastSlash == std::string::npos)
                              ? std::string("/")
                              : path.substr(0, lastSlash + 1);
    newPath = baseDir + specPath;
  }
  return origin + NormalizeDotSegments(newPath) + specSuffix;
}

// Resolve a relative "./" or "../" specifier against a file:// referrer URL.
// Returns an absolute file:// URL, or empty when not applicable. Preserved
// for parity with the earlier Android loader; the current resolver builds
// filesystem candidates directly against GetApplicationPath() so this helper
// is unused for now.
[[maybe_unused]] static std::string ResolveFileRelative(
    const std::string& referrerUrl, const std::string& spec) {
  const std::string filePrefix = "file://";
  if (!StartsWith(referrerUrl, filePrefix.c_str())) return std::string();
  if (spec.empty() || spec[0] != '.') return std::string();
  std::string refPath = referrerUrl.substr(filePrefix.size());
  size_t hashPos = refPath.find('#');
  if (hashPos != std::string::npos) refPath = refPath.substr(0, hashPos);
  size_t qPos = refPath.find('?');
  if (qPos != std::string::npos) refPath = refPath.substr(0, qPos);
  size_t lastSlash = refPath.find_last_of('/');
  std::string baseDir = (lastSlash == std::string::npos)
                            ? std::string("/")
                            : refPath.substr(0, lastSlash + 1);
  return filePrefix + NormalizeDotSegments(baseDir + spec);
}

// Forward declarations for helpers referenced before their definitions.
static bool ShouldTraceRegistryKey(const std::string& rawKey,
                                   const std::string& registryKey);
static const char* ModuleStatusToString(v8::Module::Status status);
static void KillAsyncGraphLoadsForIsolate(v8::Isolate* isolate);
static bool IsCurrentIsolateWorker(v8::Isolate* isolate);
static v8::MaybeLocal<v8::Module> CompileJsonTextAsEsModule(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& jsonText, const std::string& registryAbsPath,
    const std::string& displayUrl);
static void RejectAndClearInvalidatedModuleState(v8::Isolate* isolate,
                                                 v8::Local<v8::Context> context,
                                                 const std::string& registryKey);

namespace {
struct AsyncGraphLoad;

// One require(esm) exports facade and the module it wraps. Held as a pair
// because identity hashes collide: lookups compare the target handle.
struct RequireFacadeEntry {
  v8::Global<v8::Module> target;
  v8::Global<v8::Module> facade;
};

// One import-map section: specifier key → target. Lookup within a section is
// exact-then-trailing-slash-prefix with longest match, per the import-maps
// spec.
using ImportMapEntries = robin_hood::unordered_map<std::string, std::string>;

// A parsed import map. `scopes` is kept ordered most-specific-first so the
// resolution cascade walks it without re-sorting on every lookup.
struct ParsedImportMap {
  ImportMapEntries imports;
  std::vector<std::pair<std::string, ImportMapEntries>> scopes;

  bool empty() const { return imports.empty() && scopes.empty(); }
};

// Everything the dev client teaches one isolate's loader (see the header's
// long-form note): the import map, the canonicalization vocabulary and the
// volatile-URL patterns.
struct LoaderVocabulary {
  // Bare specifier → resolved URL, plus the per-referrer `scopes` overrides.
  // Instead of rewriting import statements on the bundler side, the runtime
  // resolves bare specifiers through this map to HTTP module URLs; source code
  // is served as-is.
  ParsedImportMap importMap;

  // URLs matching any of these substrings are always re-fetched (the cache is
  // evicted before loading). The vocabulary is server/framework policy, so the
  // runtime carries no framework-specific URL strings of its own.
  std::vector<std::string> volatilePatterns;

  CanonicalizationConfig canonicalization;
  // Distinguishes "no vocabulary supplied" (mechanical canonicalization only)
  // from "supplied, and empty" — an empty vocabulary is explicit policy.
  bool canonicalizationConfigured = false;
};

// ─────────────────────────────────────────────────────────────
// Per-isolate module-loader state
//
// Why per-isolate (not process-global, not thread_local): v8::Global<T>
// handles are bound to the isolate that created them; reading their internal
// state from a different isolate is undefined behaviour. NS Workers each run
// a separate v8::Isolate on their own thread and, under HMR, may fetch the
// same URLs the main thread already loaded — a shared map would hand the
// worker isolate a Module the main isolate compiled, and V8's linker would
// read the cross-isolate export table and emit bogus errors like:
//   SyntaxError: The requested module 'X' does not provide an export named 'Y'
//
// Lifetime: the state lives in a RuntimeState slot, so it is destroyed with
// the runtime (Runtime::DestroyRuntime → RuntimeState::Clear), on the
// runtime's own thread while the isolate is still alive — which lets the
// v8::Global members Reset safely in their own destructors and leaves nothing
// to static/thread destructors, where a post-disposal Reset would crash.
// Access from the isolate's own thread only, per the slot contract.
struct ModuleLoaderState {
  ModuleHandleMap registry;  // canonical key -> compiled module

  // What the dev client taught THIS isolate's loader: import map,
  // canonicalization vocabulary, volatile patterns.
  LoaderVocabulary vocabulary;

  // In-flight async graph walks; entries are weak so a finished load frees
  // itself. A pending background fetch completion can hold a load's
  // shared_ptr past teardown, so QuiesceModuleLoadsForIsolate must flag these
  // dead and Reset their context Globals while the isolate is still alive —
  // the slot destructor alone is not enough for them.
  std::vector<std::weak_ptr<AsyncGraphLoad>> asyncGraphLoads;

  // HTTP dynamic imports currently fetching/evaluating, for coalescing.
  robin_hood::unordered_set<std::string> modulesInFlight;

  // Dynamic HTTP import waiters: resolve to the module namespace.
  robin_hood::unordered_map<std::string,
                            std::vector<v8::Global<v8::Promise::Resolver>>>
      httpDynamicWaiters;

  // Reverse index: v8::Module::GetIdentityHash() -> registry keys, so
  // module→key lookups (resolver referrer discovery, import.meta) are O(1)
  // instead of a scan of the whole registry. Hashes collide, so a bucket holds
  // candidates; FindKeyForModule confirms each against the registry and prunes
  // the ones it no longer backs, so a stale candidate can never answer a
  // lookup.
  robin_hood::unordered_map<int, std::vector<std::string>> keysByModuleHash;

  // require(esm) facades, keyed by the TARGET module's identity hash — same
  // bucket-plus-handle-compare shape as keysByModuleHash. Repeated require() of
  // one ES module must hand back the identical exports object, and a facade
  // must never outlive the module it re-exports (UnindexRegistryKey drops the
  // entry as the target stops being the registry's answer for its key).
  robin_hood::unordered_map<int, std::vector<RequireFacadeEntry>>
      requireFacadesByTargetHash;

  // Holds the facade target across that facade's InstantiateModule and nothing
  // else — the facade's resolve callback is the only reader.
  v8::Global<v8::Module> pendingFacadeTarget;
};

// This isolate's loader state, or null once teardown has begun — callers must
// bail, not recreate state.
ModuleLoaderState* ModuleLoaderStateFor(v8::Isolate* isolate) {
  if (isolate == nullptr) return nullptr;
  return RuntimeState::For<ModuleLoaderState>(isolate);
}

// Record `key` as a candidate for `mod`'s identity hash. Call alongside every
// registry insert.
void IndexRegisteredModule(ModuleLoaderState& state, const std::string& key,
                           v8::Local<v8::Module> mod) {
  if (mod.IsEmpty()) return;
  auto& keys = state.keysByModuleHash[mod->GetIdentityHash()];
  if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
    keys.push_back(key);
  }
}

// Drop any facade wrapping `target`. Called as the target stops being the
// registry's answer for its key: a facade whose re-export source is gone would
// serve a dead namespace.
void DropRequireFacadesForTarget(ModuleLoaderState& state, v8::Isolate* isolate,
                                 v8::Local<v8::Module> target) {
  if (target.IsEmpty()) return;
  auto bucketIt = state.requireFacadesByTargetHash.find(target->GetIdentityHash());
  if (bucketIt == state.requireFacadesByTargetHash.end()) return;
  auto& entries = bucketIt->second;
  for (auto it = entries.begin(); it != entries.end();) {
    if (it->target.Get(isolate) == target) {
      it = entries.erase(it);
    } else {
      ++it;
    }
  }
  if (entries.empty()) {
    state.requireFacadesByTargetHash.erase(bucketIt);
  }
}

// Drop `key` from the bucket of whatever module the registry holds under it
// right now. Call before replacing or erasing that entry, while the outgoing
// handle is still reachable — afterwards its hash is unrecoverable.
void UnindexRegistryKey(ModuleLoaderState& state, v8::Isolate* isolate,
                        const std::string& key) {
  auto regIt = state.registry.find(key);
  if (regIt == state.registry.end() || regIt->second.IsEmpty()) return;
  v8::Local<v8::Module> outgoing = regIt->second.Get(isolate);
  if (outgoing.IsEmpty()) return;
  DropRequireFacadesForTarget(state, isolate, outgoing);
  auto bucketIt = state.keysByModuleHash.find(outgoing->GetIdentityHash());
  if (bucketIt == state.keysByModuleHash.end()) return;
  auto& keys = bucketIt->second;
  keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
  if (keys.empty()) {
    state.keysByModuleHash.erase(bucketIt);
  }
}

// The registry key whose live entry is `mod`, or empty. Prunes candidates the
// registry no longer confirms.
std::string FindKeyForModule(ModuleLoaderState& state, v8::Isolate* isolate,
                             v8::Local<v8::Module> mod) {
  if (mod.IsEmpty()) return std::string();
  auto bucketIt = state.keysByModuleHash.find(mod->GetIdentityHash());
  if (bucketIt == state.keysByModuleHash.end()) return std::string();
  auto& keys = bucketIt->second;
  for (auto it = keys.begin(); it != keys.end();) {
    auto regIt = state.registry.find(*it);
    if (regIt == state.registry.end() || regIt->second.IsEmpty()) {
      it = keys.erase(it);
      continue;
    }
    if (regIt->second.Get(isolate) == mod) {
      return *it;
    }
    ++it;
  }
  if (keys.empty()) {
    state.keysByModuleHash.erase(bucketIt);
  }
  return std::string();
}
}  // namespace

std::string LookupModuleKeyForModule(v8::Isolate* isolate,
                                     v8::Local<v8::Module> mod) {
  auto* state = ModuleLoaderStateFor(isolate);
  if (state == nullptr) return std::string();
  return FindKeyForModule(*state, isolate, mod);
}

namespace {
// The single module request in the facade source, and the source itself. Both
// match Node's required_module_facade_source_string so the semantics (live
// bindings, enumerable re-exports, overridable __esModule) stay identical.
constexpr const char* kRequireFacadeSpecifier = "original";
constexpr const char* kRequireFacadeSource =
    "export * from 'original'; export { default } from 'original'; "
    "export const __esModule = true;";

// Resolves the facade's one request. Passed only to a facade's
// InstantiateModule, so the general resolver never sees 'original' and user
// code can never reach this slot.
v8::MaybeLocal<v8::Module> ResolveRequireFacadeTarget(
    v8::Local<v8::Context> context, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> /*import_assertions*/,
    v8::Local<v8::Module> /*referrer*/) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto* state = ModuleLoaderStateFor(isolate);
  v8::String::Utf8Value specUtf8(isolate, specifier);
  const std::string spec = *specUtf8 ? *specUtf8 : "";
  if (state == nullptr || state->pendingFacadeTarget.IsEmpty() ||
      spec != kRequireFacadeSpecifier) {
    DEBUG_WRITE_FORCE("FATAL: require(esm) facade resolve for '%s' with no pending target",
                      spec.c_str());
    isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(
        isolate, "require(esm) facade could not be linked to its target module")));
    return v8::MaybeLocal<v8::Module>();
  }
  return v8::MaybeLocal<v8::Module>(state->pendingFacadeTarget.Get(isolate));
}
}  // namespace

v8::MaybeLocal<v8::Module> GetOrCreateRequireFacade(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    v8::Local<v8::Module> target, const std::string& targetCanonicalPath) {
  if (target.IsEmpty()) return v8::MaybeLocal<v8::Module>();
  auto* state = ModuleLoaderStateFor(isolate);
  if (state == nullptr) return v8::MaybeLocal<v8::Module>();

  auto bucketIt = state->requireFacadesByTargetHash.find(target->GetIdentityHash());
  if (bucketIt != state->requireFacadesByTargetHash.end()) {
    for (auto& entry : bucketIt->second) {
      if (entry.target.Get(isolate) == target) {
        return v8::MaybeLocal<v8::Module>(entry.facade.Get(isolate));
      }
    }
  }

  v8::EscapableHandleScope hs(isolate);
  const std::string facadeUrl = "ns:require-facade:" + targetCanonicalPath;

  v8::Local<v8::String> urlV8;
  if (!v8::String::NewFromUtf8(isolate, facadeUrl.c_str(), v8::NewStringType::kNormal)
           .ToLocal(&urlV8)) {
    return v8::MaybeLocal<v8::Module>();
  }
  v8::ScriptOrigin origin(urlV8, 0, 0, false, -1, v8::Local<v8::Value>(), false,
                          false, true /* is_module */);
  v8::ScriptCompiler::Source source(
      ArgConverter::ConvertToV8String(isolate, kRequireFacadeSource), origin);

  v8::TryCatch tc(isolate);
  v8::Local<v8::Module> facade;
  if (!v8::ScriptCompiler::CompileModule(isolate, &source).ToLocal(&facade)) {
    throw NativeScriptException(
        tc, "Cannot compile the require() facade for " + targetCanonicalPath);
  }

  bool linked = false;
  {
    // The slot must be clear again whichever way instantiation ends.
    struct PendingTargetScope {
      ModuleLoaderState* state;
      ~PendingTargetScope() { state->pendingFacadeTarget.Reset(); }
    } pendingScope{state};
    state->pendingFacadeTarget.Reset(isolate, target);
    linked = facade->InstantiateModule(context, &ResolveRequireFacadeTarget)
                 .FromMaybe(false);
  }
  if (!linked) {
    throw NativeScriptException(
        tc, "Cannot link the require() facade for " + targetCanonicalPath);
  }

  // Three re-export statements over an already-evaluated module: trivially
  // synchronous, so the strict policy's settled-promise requirement holds.
  ModuleEvaluationOptions evalOptions;
  evalOptions.policy = ModuleEvaluationPolicy::kSyncStrict;
  EvaluateModuleGraph(isolate, context, facade, facadeUrl, evalOptions);

  // The facade is deliberately absent from the registry and the identity-hash
  // index: nothing resolves to it by name, and its source has no import.meta or
  // dynamic import, so no host callback ever needs to find it.
  RequireFacadeEntry entry;
  entry.target.Reset(isolate, target);
  entry.facade.Reset(isolate, facade);
  state->requireFacadesByTargetHash[target->GetIdentityHash()].push_back(
      std::move(entry));

  return hs.Escape(facade);
}

void IndexModuleForIsolate(v8::Isolate* isolate, const std::string& canonicalKey,
                           v8::Local<v8::Module> mod) {
  auto* state = ModuleLoaderStateFor(isolate);
  if (state == nullptr) return;
  IndexRegisteredModule(*state, canonicalKey, mod);
}

void UnindexModuleForIsolate(v8::Isolate* isolate,
                             const std::string& canonicalKey) {
  auto* state = ModuleLoaderStateFor(isolate);
  if (state == nullptr) return;
  UnindexRegistryKey(*state, isolate, canonicalKey);
}

static bool IsVolatileUrl(const LoaderVocabulary& vocabulary,
                          const std::string& url);

// ─────────────────────────────────────────────────────────────
// AdoptThenable
//
// Turn any thenable value into a real v8::Promise. Promises returned by
// V8 itself (Module::Evaluate) are genuine and take the fast path;
// user-space thenables (e.g. Proxy'd Promises) fail v8::Value::IsPromise
// but adopting them via Promise::Resolver::New + Resolve preserves their
// state.
static v8::MaybeLocal<v8::Promise> AdoptThenable(v8::Isolate* isolate,
                                                 v8::Local<v8::Context> context,
                                                 v8::Local<v8::Value> value) {
  if (value.IsEmpty()) return v8::MaybeLocal<v8::Promise>();
  if (value->IsPromise()) return value.As<v8::Promise>();
  if (!value->IsObject()) return v8::MaybeLocal<v8::Promise>();

  v8::Local<v8::Value> thenVal;
  if (!value.As<v8::Object>()
           ->Get(context, ArgConverter::ConvertToV8String(isolate, "then"))
           .ToLocal(&thenVal) ||
      !thenVal->IsFunction()) {
    return v8::MaybeLocal<v8::Promise>();
  }

  v8::Local<v8::Promise::Resolver> adopter;
  if (!v8::Promise::Resolver::New(context).ToLocal(&adopter) ||
      adopter->Resolve(context, value).IsNothing()) {
    return v8::MaybeLocal<v8::Promise>();
  }
  return adopter->GetPromise();
}

// ─────────────────────────────────────────────────────────────
// Compile helpers

static v8::MaybeLocal<v8::Module> CompileModuleFromSource(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& code, const std::string& urlStr) {
  v8::EscapableHandleScope hs(isolate);
  // NUL-preserving conversion: module source may contain embedded NUL bytes;
  // the char* path would truncate.
  v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, code);
  v8::Local<v8::String> urlV8;
  if (!v8::String::NewFromUtf8(isolate, urlStr.c_str(),
                               v8::NewStringType::kNormal)
           .ToLocal(&urlV8)) {
    return v8::MaybeLocal<v8::Module>();
  }
  v8::ScriptOrigin origin(urlV8, 0, 0, false, -1, v8::Local<v8::Value>(),
                          false, false, true /* is_module */);
  v8::ScriptCompiler::Source src(sourceText, origin);
  v8::Local<v8::Module> mod;
  if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&mod)) {
    return v8::MaybeLocal<v8::Module>();
  }
  if (mod->GetStatus() == v8::Module::kUninstantiated) {
    if (!mod->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
      return v8::MaybeLocal<v8::Module>();
    }
  }
  if (mod->GetStatus() != v8::Module::kEvaluated) {
    if (mod->Evaluate(context).IsEmpty()) {
      return v8::MaybeLocal<v8::Module>();
    }
  }
  return hs.Escape(mod);
}

// "message (line L:C)" for a caught exception, or empty. The line/column are
// the part no caller can reconstruct from a failure code.
static std::string DescribeCaughtError(v8::Isolate* isolate,
                                       v8::Local<v8::Context> context,
                                       const v8::TryCatch& tc) {
  if (!tc.HasCaught()) return std::string();
  v8::Local<v8::Message> message = tc.Message();
  if (message.IsEmpty()) return std::string();
  v8::String::Utf8Value text(isolate, message->Get());
  std::string described = *text ? *text : "";
  int line = message->GetLineNumber(context).FromMaybe(0);
  if (line > 0) {
    described += " (line " + std::to_string(line) + ":" +
                 std::to_string(message->GetStartColumn()) + ")";
  }
  return described;
}

// Compile-only variant used inside ResolveModuleCallback. Compiles a
// v8::Module and registers it under urlStr but does NOT instantiate or
// evaluate. V8 is currently instantiating the importer and will handle
// instantiation of this dependency.
//
// On compile failure the exception is left PENDING, the same contract as
// ModuleInternal::CompileFileEsModule: it names the file, line and column,
// which nothing downstream can reconstruct. A caller that cannot let it
// propagate must consume it through its own TryCatch and route the text into
// its own failure channel — never drop it.
static v8::MaybeLocal<v8::Module> CompileModuleForResolveRegisterOnly(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& code, const std::string& urlStr) {
  v8::EscapableHandleScope hs(isolate);
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) {
    return v8::MaybeLocal<v8::Module>();
  }
  auto& g_moduleRegistry = moduleState->registry;
  const std::string registryKey = CanonicalizeRegistryKey(urlStr);
  if (LogCategoryEnabled(LogCategory::Esm) &&
      ShouldTraceRegistryKey(urlStr, registryKey)) {
    TNS_DEBUG(Esm, "[resolver][register-resolve-only] raw=%s key=%s",
                   urlStr.c_str(), registryKey.c_str());
  }

  // Checked before compiling: recompiling a key that is already registered
  // would mint a second module identity while importers hold the first.
  auto itExisting = g_moduleRegistry.find(registryKey);
  if (itExisting != g_moduleRegistry.end()) {
    v8::Local<v8::Module> existing = itExisting->second.Get(isolate);
    if (!existing.IsEmpty()) {
      return hs.Escape(existing);
    }
  }

  v8::Local<v8::String> sourceText =
      ArgConverter::ConvertToV8String(isolate, code);
  v8::Local<v8::String> urlV8;
  if (!v8::String::NewFromUtf8(isolate, urlStr.c_str(),
                               v8::NewStringType::kNormal)
           .ToLocal(&urlV8)) {
    return v8::MaybeLocal<v8::Module>();
  }
  v8::ScriptOrigin origin(urlV8, 0, 0, false, -1, v8::Local<v8::Value>(),
                          false, false, true /* is_module */);
  v8::ScriptCompiler::Source src(sourceText, origin);
  v8::Local<v8::Module> mod;
  {
    v8::TryCatch tcCompile(isolate);
    if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&mod)) {
      TNS_DEBUG(Esm, "[http-esm][compile][fail] %s %s", urlStr.c_str(),
                     DescribeCaughtError(isolate, context, tcCompile).c_str());
      tcCompile.ReThrow();
      return v8::MaybeLocal<v8::Module>();
    }
  }
  UnindexRegistryKey(*moduleState, isolate, registryKey);
  g_moduleRegistry[registryKey].Reset(isolate, mod);
  IndexRegisteredModule(*moduleState, registryKey, mod);
  return hs.Escape(mod);
}

// Each access site binds a local reference (e.g.
// `auto& g_moduleRegistry = moduleState->registry;`) so the bodies below read
// as though the maps were plain globals. Accessors return null once teardown
// has begun.
ModuleHandleMap* ModuleRegistryFor(v8::Isolate* isolate) {
  auto* state = ModuleLoaderStateFor(isolate);
  return state == nullptr ? nullptr : &state->registry;
}

// Neutralize any in-flight async graph loads for `isolate`: their fetch
// completions check the dead flag before touching V8, and their context
// Globals are Reset here, while the isolate is still alive. The rest of the
// loader state is destroyed with the isolate's RuntimeState.
void QuiesceModuleLoadsForIsolate(v8::Isolate* isolate) {
  KillAsyncGraphLoadsForIsolate(isolate);
}

// The calling isolate's vocabulary, or null once teardown has begun.
static LoaderVocabulary* VocabularyForCurrentIsolate() {
  auto* state = ModuleLoaderStateFor(v8::Isolate::TryGetCurrent());
  return state != nullptr ? &state->vocabulary : nullptr;
}

static bool ShouldTraceRegistryKey(const std::string& rawKey,
                                   const std::string& registryKey) {
  if (rawKey != registryKey) return true;
  return StartsWith(registryKey, "optional:") ||
         StartsWith(registryKey, "node:") ||
         StartsWith(registryKey, "blob:");
}

std::string CanonicalizeRegistryKey(const std::string& key) {
  if (key.empty()) return key;

  std::string registryKey;
  const char* classification = "path";
  bool traceEvenWithoutChange = false;

  if (StartsWith(key, "http://") || StartsWith(key, "https://") ||
      StartsWith(key, "file://http://") || StartsWith(key, "file://https://")) {
    registryKey = CanonicalizeHttpUrlKey(key);
    classification = "http";
  } else if (StartsWith(key, "file://")) {
    registryKey = NormalizePath(FileURLToPath(key));
    classification = "file-url";
  } else if (StartsWith(key, "blob:")) {
    registryKey = key;
    classification = "blob";
    traceEvenWithoutChange = true;
  } else {
    // Preserve non-filesystem module namespaces such as optional: and node:
    // so synthetic/in-memory modules keep their exact registry identity.
    size_t schemePos = key.find(':');
    size_t slashPos = key.find('/');
    if (schemePos != std::string::npos &&
        (slashPos == std::string::npos || schemePos < slashPos)) {
      registryKey = key;
      classification = "custom-scheme";
      traceEvenWithoutChange = true;
    } else {
      registryKey = NormalizePath(key);
    }
  }

  if (traceEvenWithoutChange || registryKey != key) {
    TNS_DEBUG(Esm, "[resolver][registry-key][%s] raw=%s key=%s", classification,
                   key.c_str(), registryKey.c_str());
  }
  return registryKey;
}

v8::MaybeLocal<v8::Module> LoadHttpModuleForUrl(v8::Isolate* isolate,
                                                v8::Local<v8::Context> context,
                                                const std::string& requestedUrl) {
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) {
    return v8::MaybeLocal<v8::Module>();
  }
  auto& g_moduleRegistry = moduleState->registry;
  const std::string registryKey = CanonicalizeHttpUrlKey(requestedUrl);

  TNS_DEBUG(Esm, "[http-esm][load][begin] request=%s key=%s",
                 requestedUrl.c_str(), registryKey.c_str());

  auto itExisting = g_moduleRegistry.find(registryKey);
  if (itExisting != g_moduleRegistry.end()) {
    v8::Local<v8::Module> existing = itExisting->second.Get(isolate);
    if (!existing.IsEmpty() && existing->GetStatus() != v8::Module::kErrored) {
      TNS_DEBUG(Esm, "[http-esm][load][cache-hit] key=%s", registryKey.c_str());
      return v8::MaybeLocal<v8::Module>(existing);
    }
    TNS_DEBUG(Esm, "[http-esm][load][drop-errored] key=%s", registryKey.c_str());
    RemoveModuleFromRegistry(registryKey);
  }

  // Reaching this point means the graph walk did not discover this URL, so the
  // module is about to be fetched synchronously, blocking the JS thread for a
  // whole round trip. That is an invariant violation, not a mode — always
  // visible, in every build, so it cannot hide behind a disabled trace
  // category. The fallback itself stays: correctness first, diagnosis loud.
  DEBUG_WRITE_FORCE(
      "NativeScript: module graph walk missed %s — falling back to a blocking "
      "synchronous fetch. This should not happen; please report it.",
      requestedUrl.c_str());

  ModuleFetchResult fetched;
  if (!HttpFetchModule(requestedUrl, fetched)) {
    TNS_DEBUG(Esm, "[http-esm][load][fetch-fail] request=%s key=%s status=%d",
                   requestedUrl.c_str(), registryKey.c_str(), fetched.status);
    // The classifier's reason names the URL and the cause (status, MIME or
    // transport); a generic message here would lose all of it. V8 requires an
    // exception whenever a resolve callback returns empty, so this is thrown
    // in every build.
    isolate->ThrowException(v8::Exception::Error(
        ArgConverter::ConvertToV8String(isolate, fetched.failureReason)));
    return v8::MaybeLocal<v8::Module>();
  }

  if (fetched.kind == ModuleResponseKind::kJson) {
    return CompileJsonTextAsEsModule(isolate, context, fetched.body, registryKey,
                                     requestedUrl);
  }

  v8::Local<v8::Module> loaded;
  {
    v8::TryCatch tcCompile(isolate);
    if (!CompileModuleForResolveRegisterOnly(isolate, context, fetched.body,
                                             registryKey)
             .ToLocal(&loaded)) {
      TNS_DEBUG(Esm, "[http-esm][load][compile-fail] request=%s key=%s bytes=%zu",
                     requestedUrl.c_str(), registryKey.c_str(),
                     fetched.body.size());
      if (tcCompile.HasCaught()) {
        // The compile error names the module, line and column; replacing it
        // with a generic "compile failed" would strictly lose information.
        tcCompile.ReThrow();
      } else {
        std::string msg = "HTTP import compile failed: " + requestedUrl;
        isolate->ThrowException(v8::Exception::Error(
            ArgConverter::ConvertToV8String(isolate, msg)));
      }
      return v8::MaybeLocal<v8::Module>();
    }
  }

  TNS_DEBUG(Esm, "[http-esm][load][ok] request=%s key=%s type=%s bytes=%zu",
                 requestedUrl.c_str(), registryKey.c_str(),
                 fetched.contentType.c_str(), fetched.body.size());
  return loaded;
}

// ─────────────────────────────────────────────────────────────
// Import map helpers

// Read one imports-shaped section. Every rejection names the offending key so
// a bad map is fixable from the message alone.
static bool ParseImportMapEntries(v8::Isolate* isolate, v8::Local<v8::Context> context,
                                  v8::Local<v8::Object> source,
                                  const std::string& sectionLabel,
                                  ImportMapEntries* out, std::string* error) {
  v8::Local<v8::Array> keys;
  if (!source->GetOwnPropertyNames(context).ToLocal(&keys)) {
    *error = sectionLabel + ": could not be read";
    return false;
  }
  for (uint32_t i = 0; i < keys->Length(); i++) {
    v8::Local<v8::Value> keyVal;
    if (!keys->Get(context, i).ToLocal(&keyVal) || !keyVal->IsString()) {
      *error = sectionLabel + ": every key must be a string";
      return false;
    }
    v8::String::Utf8Value keyUtf8(isolate, keyVal);
    if (!*keyUtf8) {
      *error = sectionLabel + ": every key must be a string";
      return false;
    }
    const std::string specifier(*keyUtf8);
    if (specifier.empty()) {
      *error = sectionLabel + ": a specifier key must not be empty";
      return false;
    }

    v8::Local<v8::Value> value;
    if (!source->Get(context, keyVal).ToLocal(&value) || !value->IsString()) {
      *error = sectionLabel + ": the target for '" + specifier + "' must be a string";
      return false;
    }
    v8::String::Utf8Value valueUtf8(isolate, value);
    if (!*valueUtf8) {
      *error = sectionLabel + ": the target for '" + specifier + "' must be a string";
      return false;
    }
    const std::string target(*valueUtf8);
    if (target.empty()) {
      *error = sectionLabel + ": the target for '" + specifier + "' must not be empty";
      return false;
    }

    // A trailing-slash key maps a whole subtree, so its target must name one
    // too — otherwise the remainder would be pasted onto a file path.
    if (specifier.back() == '/' && target.back() != '/') {
      *error = sectionLabel + ": the target for '" + specifier +
               "' must end with '/' because the specifier key does";
      return false;
    }

    (*out)[specifier] = target;
  }
  return true;
}

// Parse without touching the live map. On any failure `error` explains what is
// wrong and `out` is meaningless — the caller keeps whatever it already had.
// V8's JSON parser stands in for iOS's NSJSONSerialization: escapes, nesting
// and malformed input are handled by the engine rather than a hand-rolled
// scanner, and this always runs on the isolate's own thread.
static bool ParseImportMap(v8::Isolate* isolate, const std::string& json,
                           ParsedImportMap* out, std::string* error) {
  if (json.empty()) {
    *error = "an import map must be a non-empty JSON object";
    return false;
  }

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::TryCatch tc(isolate);
  v8::Local<v8::Value> parsed;
  if (!v8::JSON::Parse(context, ArgConverter::ConvertToV8String(isolate, json))
           .ToLocal(&parsed)) {
    std::string detail = DescribeCaughtError(isolate, context, tc);
    *error = "an import map must be valid JSON" + (detail.empty() ? "" : ": " + detail);
    return false;
  }
  if (!parsed->IsObject() || parsed->IsArray()) {
    *error = "an import map must be a JSON object";
    return false;
  }
  v8::Local<v8::Object> top = parsed.As<v8::Object>();

  // Only the map's OWN keys are sections; reading through the prototype would
  // let a polluted Object.prototype smuggle one in.
  v8::Local<v8::Array> sections;
  if (!top->GetOwnPropertyNames(context).ToLocal(&sections)) {
    *error = "an import map must be a JSON object";
    return false;
  }
  bool hasImports = false;
  bool hasScopes = false;
  for (uint32_t i = 0; i < sections->Length(); i++) {
    v8::Local<v8::Value> sectionVal;
    std::string name;
    if (sections->Get(context, i).ToLocal(&sectionVal) && sectionVal->IsString()) {
      v8::String::Utf8Value utf8(isolate, sectionVal);
      if (*utf8) name = *utf8;
    }
    if (name == "imports") {
      hasImports = true;
    } else if (name == "scopes") {
      hasScopes = true;
    } else {
      *error = "unsupported import-map section '" + name +
               "'; only \"imports\" and \"scopes\" are supported";
      return false;
    }
  }

  v8::Local<v8::Value> imports;
  if (hasImports &&
      top->Get(context, ArgConverter::ConvertToV8String(isolate, "imports")).ToLocal(&imports) &&
      !imports->IsUndefined()) {
    if (!imports->IsObject() || imports->IsArray()) {
      *error = "the \"imports\" section must be an object";
      return false;
    }
    if (!ParseImportMapEntries(isolate, context, imports.As<v8::Object>(), "imports",
                               &out->imports, error)) {
      return false;
    }
  }

  v8::Local<v8::Value> scopes;
  if (hasScopes &&
      top->Get(context, ArgConverter::ConvertToV8String(isolate, "scopes")).ToLocal(&scopes) &&
      !scopes->IsUndefined()) {
    if (!scopes->IsObject() || scopes->IsArray()) {
      *error = "the \"scopes\" section must be an object";
      return false;
    }
    v8::Local<v8::Object> scopesObj = scopes.As<v8::Object>();
    v8::Local<v8::Array> scopeKeys;
    if (!scopesObj->GetOwnPropertyNames(context).ToLocal(&scopeKeys)) {
      *error = "the \"scopes\" section must be an object";
      return false;
    }
    for (uint32_t i = 0; i < scopeKeys->Length(); i++) {
      v8::Local<v8::Value> scopeKeyVal;
      if (!scopeKeys->Get(context, i).ToLocal(&scopeKeyVal) || !scopeKeyVal->IsString()) {
        *error = "scopes: every scope key must be a string";
        return false;
      }
      v8::String::Utf8Value scopeUtf8(isolate, scopeKeyVal);
      const std::string scopePrefix(*scopeUtf8 ? *scopeUtf8 : "");
      if (scopePrefix.empty()) {
        *error = "scopes: a scope key must not be empty";
        return false;
      }
      v8::Local<v8::Value> scopeMap;
      if (!scopesObj->Get(context, scopeKeyVal).ToLocal(&scopeMap) || !scopeMap->IsObject() ||
          scopeMap->IsArray()) {
        *error = "scopes: the map for scope '" + scopePrefix + "' must be an object";
        return false;
      }
      ImportMapEntries entries;
      if (!ParseImportMapEntries(isolate, context, scopeMap.As<v8::Object>(),
                                 "scope '" + scopePrefix + "'", &entries, error)) {
        return false;
      }
      out->scopes.emplace_back(scopePrefix, std::move(entries));
    }
  }

  // Most specific first: a longer prefix is the more specific scope, and the
  // key comparison keeps the order deterministic for equal-length prefixes.
  std::sort(out->scopes.begin(), out->scopes.end(),
            [](const std::pair<std::string, ImportMapEntries>& a,
               const std::pair<std::string, ImportMapEntries>& b) {
              if (a.first.size() != b.first.size()) {
                return a.first.size() > b.first.size();
              }
              return a.first > b.first;
            });
  return true;
}

bool SetImportMap(const std::string& json, std::string* error) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  LoaderVocabulary* vocabulary = VocabularyForCurrentIsolate();
  std::string localError;
  std::string& err = error != nullptr ? *error : localError;
  if (vocabulary == nullptr) {
    err = "the calling isolate has no loader vocabulary";
    return false;
  }

  // Parse-validate-swap: the live vocabulary is replaced only once a complete
  // map has been built, so a rejected update leaves resolution exactly as it
  // was rather than silently emptying it.
  ParsedImportMap parsedMap;
  if (!ParseImportMap(isolate, json, &parsedMap, &err)) {
    return false;
  }
  vocabulary->importMap = std::move(parsedMap);
  TNS_DEBUG(Esm, "[import-map] loaded %lu entries, %lu scopes",
                 (unsigned long)vocabulary->importMap.imports.size(),
                 (unsigned long)vocabulary->importMap.scopes.size());
  return true;
}

void SetVolatilePatterns(const std::vector<std::string>& patterns) {
  LoaderVocabulary* vocabulary = VocabularyForCurrentIsolate();
  if (vocabulary == nullptr) return;
  vocabulary->volatilePatterns = patterns;
  TNS_DEBUG(Esm, "[import-map] volatile patterns: %lu",
                 (unsigned long)vocabulary->volatilePatterns.size());
}

const CanonicalizationConfig* CanonicalizationConfigForCurrentIsolate() {
  const LoaderVocabulary* vocabulary = VocabularyForCurrentIsolate();
  if (vocabulary == nullptr || !vocabulary->canonicalizationConfigured) {
    return nullptr;
  }
  return &vocabulary->canonicalization;
}

void SetCanonicalizationConfig(CanonicalizationConfig config) {
  LoaderVocabulary* vocabulary = VocabularyForCurrentIsolate();
  if (vocabulary == nullptr) return;
  vocabulary->canonicalization = std::move(config);
  vocabulary->canonicalizationConfigured = true;
  TNS_DEBUG(Esm, "[ns:module configureLoader] canonicalization set (strip=%lu "
                 "devPrefixes=%lu preserve=%lu)",
                 (unsigned long)vocabulary->canonicalization.stripParams.size(),
                 (unsigned long)vocabulary->canonicalization.devPathPrefixes.size(),
                 (unsigned long)vocabulary->canonicalization.preserveQueryPrefixes.size());
}

static bool IsVolatileUrl(const LoaderVocabulary& vocabulary,
                          const std::string& url) {
  for (const auto& pat : vocabulary.volatilePatterns) {
    if (url.find(pat) != std::string::npos) return true;
  }
  return false;
}

// Normalize a Vite-rewritten specifier into the canonical import-map key.
// Handles two common patterns:
//   1. Prebundled deps:  "/node_modules/.vite/deps/solid-js.js?v=abc"   → "solid-js"
//                        "/node_modules/.vite/deps/@tanstack_solid-router.js" →
//                        "@tanstack/solid-router"
//   2. Explicit node_modules paths:
//        "/node_modules/@angular/core/fesm2022/core.mjs" → "@angular/core/fesm2022/core.mjs"
//        "/node_modules/tslib/tslib.es6.mjs"             → "tslib"
static std::string NormalizeViteSpecifier(const std::string& specifier) {
  // Pattern 1: Vite prebundled deps.
  {
    const std::string viteDepsPrefix = "/node_modules/.vite/deps/";
    const std::string viteDepsPrefix2 = "node_modules/.vite/deps/";
    std::string prefix;
    if (specifier.compare(0, viteDepsPrefix.size(), viteDepsPrefix) == 0)
      prefix = viteDepsPrefix;
    else if (specifier.compare(0, viteDepsPrefix2.size(), viteDepsPrefix2) == 0)
      prefix = viteDepsPrefix2;

    if (!prefix.empty()) {
      std::string id = specifier.substr(prefix.size());
      auto qpos = id.find('?');
      if (qpos != std::string::npos) id = id.substr(0, qpos);
      auto dotpos = id.rfind('.');
      if (dotpos != std::string::npos) id = id.substr(0, dotpos);
      if (!id.empty() && id[0] == '@') {
        auto upos = id.find('_');
        if (upos != std::string::npos) {
          id = id.substr(0, upos) + "/" + id.substr(upos + 1);
          auto upos2 = id.find('_', upos + 1);
          if (upos2 != std::string::npos) {
            id = id.substr(0, upos2);
          }
        }
      }
      TNS_DEBUG(Esm, "[import-map][normalize] vite-deps: %s -> %s",
                     specifier.c_str(), id.c_str());
      return id;
    }
  }

  // Pattern 2: Resolved node_modules path — /node_modules/<pkg>/...
  {
    const std::string nmPrefix = "/node_modules/";
    const std::string nmPrefix2 = "node_modules/";
    std::string sub;
    if (specifier.compare(0, nmPrefix.size(), nmPrefix) == 0)
      sub = specifier.substr(nmPrefix.size());
    else if (specifier.compare(0, nmPrefix2.size(), nmPrefix2) == 0)
      sub = specifier.substr(nmPrefix2.size());

    if (!sub.empty() && sub[0] != '.') {
      if (sub.compare(0, 6, ".vite/") == 0) return "";

      std::string subNoQuery = sub;
      std::string querySuffix;
      auto subQueryPos = sub.find('?');
      if (subQueryPos != std::string::npos) {
        subNoQuery = sub.substr(0, subQueryPos);
        querySuffix = sub.substr(subQueryPos);
      }

      std::string pkgName;
      if (subNoQuery[0] == '@') {
        auto slash1 = subNoQuery.find('/');
        if (slash1 != std::string::npos) {
          auto slash2 = subNoQuery.find('/', slash1 + 1);
          pkgName = (slash2 != std::string::npos) ? subNoQuery.substr(0, slash2)
                                                  : subNoQuery;
        }
      } else {
        auto slash = subNoQuery.find('/');
        pkgName = (slash != std::string::npos) ? subNoQuery.substr(0, slash)
                                               : subNoQuery;
      }
      if (!pkgName.empty()) {
        std::string normalized = pkgName;
        std::string remainder;
        if (subNoQuery.size() > pkgName.size()) {
          remainder = subNoQuery.substr(pkgName.size());
          if (!remainder.empty() && remainder[0] == '/') {
            remainder.erase(0, 1);
          }
        }

        if (!remainder.empty()) {
          bool preserveSubpath = remainder.find('/') != std::string::npos;

          if (!preserveSubpath) {
            const std::string pkgBaseName =
                pkgName.substr(pkgName.find_last_of('/') + 1);
            std::string withoutExt = remainder;
            auto dot = withoutExt.rfind('.');
            if (dot != std::string::npos) {
              withoutExt = withoutExt.substr(0, dot);
            }
            std::string withoutPlatform = withoutExt;
            for (const auto& suffix : {std::string(".ios"), std::string(".android"),
                                       std::string(".visionos")}) {
              if (EndsWith(withoutPlatform, suffix)) {
                withoutPlatform =
                    withoutPlatform.substr(0, withoutPlatform.size() - suffix.size());
                break;
              }
            }
            const bool isRootLevelMainEntry =
                withoutPlatform == "index" ||
                withoutPlatform == pkgBaseName ||
                withoutPlatform.rfind(pkgBaseName + ".", 0) == 0;
            preserveSubpath = !isRootLevelMainEntry;
          }

          if (preserveSubpath) {
            normalized = pkgName + "/" + remainder + querySuffix;
          }
        }

        TNS_DEBUG(Esm, "[import-map][normalize] node_modules: %s -> %s",
                       specifier.c_str(), normalized.c_str());
        return normalized;
      }
    }
  }
  return "";
}

// Look up a specifier in ONE import-map section: exact match first, then the
// longest trailing-slash prefix entry, whose remainder is appended to the
// target. Returns empty when the section has no answer.
static std::string LookupInEntries(const ImportMapEntries& entries,
                                   const std::string& specifier) {
  auto it = entries.find(specifier);
  if (it != entries.end()) {
    TNS_DEBUG(Esm, "[import-map] exact: %s -> %s", specifier.c_str(),
                   it->second.c_str());
    return it->second;
  }

  std::string bestKey;
  std::string bestValue;
  for (const auto& kv : entries) {
    const std::string& key = kv.first;
    if (key.back() != '/') continue;  // only trailing-slash entries map subtrees
    if (specifier.size() > key.size() &&
        specifier.compare(0, key.size(), key) == 0) {
      if (key.size() > bestKey.size()) {
        bestKey = key;
        bestValue = kv.second;
      }
    }
  }
  if (bestKey.empty()) return "";
  std::string resolved = bestValue + specifier.substr(bestKey.size());
  TNS_DEBUG(Esm, "[import-map] prefix: %s -> %s (via %s)", specifier.c_str(),
                 resolved.c_str(), bestKey.c_str());
  return resolved;
}

// The import-map resolution cascade: the most specific applicable scope first,
// then progressively less specific ones, then the top-level imports — each
// consulted with the same per-section lookup.
//
// A scope key matches as a plain prefix of `referrerKey`, the importing
// module's canonical registry key: an absolute http(s) URL for a served
// module, or a canonical absolute path for a file. That key is this runtime's
// analogue of the web's resolved referrer URL, which is what scope prefixes
// match there. Ending a scope key with '/' keeps it on a directory boundary,
// exactly as on the web.
static std::string LookupImportMap(const LoaderVocabulary& vocabulary,
                                   const std::string& specifier,
                                   const std::string& referrerKey) {
  for (const auto& scope : vocabulary.importMap.scopes) {
    const std::string& prefix = scope.first;
    if (referrerKey.size() < prefix.size() ||
        referrerKey.compare(0, prefix.size(), prefix) != 0) {
      continue;
    }
    std::string mapped = LookupInEntries(scope.second, specifier);
    if (!mapped.empty()) {
      TNS_DEBUG(Esm, "[import-map] scope '%s' matched referrer %s", prefix.c_str(),
                     referrerKey.c_str());
      return mapped;
    }
  }
  return LookupInEntries(vocabulary.importMap.imports, specifier);
}

// ─────────────────────────────────────────────────────────────
// The shared resolution seam
//
// One module specifier resolved to something the loader can act on. Both
// ResolveModuleCallback and the graph walk go through this, so a module gets
// the same registry key whichever of them reaches it first — a divergence here
// mints two identities for one file.
//
// It consults the import map and the filesystem but never compiles, registers,
// fetches or throws. The one V8 touch is the `__NS_HTTP_ORIGIN__` global read
// for root-absolute specifiers, which is why `context` is a parameter: both
// callers must see the same anchor or they would classify the same specifier
// differently.
struct ModuleResolution {
  enum class Kind {
    kUnresolved,    // nothing locatable; the caller decides how to report it
    kBuiltin,       // ns:/node: — served from the builtin registry
    kHttp,          // absolute http(s) URL
    kFile,          // absolute filesystem path, confirmed to be a regular file
  };

  Kind kind = Kind::kUnresolved;
  std::string url;        // kHttp
  std::string path;       // kFile
  std::string specifier;  // the specifier after import-map rewriting
  std::string attempted;  // kUnresolved: the last candidate tried
};

// Rebuild an HTTP URL a path join swallowed ('/app/http:/host/x' →
// 'http://host/x'), or empty when the path embeds none.
static std::string HttpUrlEmbeddedInPath(const std::string& p) {
  size_t pos1 = p.find("/http:/");
  size_t pos2 = p.find("/https:/");
  size_t pos = std::min(pos1 == std::string::npos ? SIZE_MAX : pos1,
                        pos2 == std::string::npos ? SIZE_MAX : pos2);
  if (pos == SIZE_MAX) return "";
  std::string tail = p.substr(pos + 1);
  if (StartsWith(tail, "http:/") && !StartsWith(tail, "http://")) {
    tail.insert(5, "/");
  } else if (StartsWith(tail, "https:/") && !StartsWith(tail, "https://")) {
    tail.insert(6, "/");
  }
  if (!(StartsWith(tail, "http://") || StartsWith(tail, "https://"))) return "";
  return tail;
}

// The origin the dev client is serving from, or empty. Anchors root-absolute
// specifiers imported by a module that itself came off disk.
static std::string HttpOriginAnchor(v8::Isolate* isolate,
                                    v8::Local<v8::Context> context) {
  if (context.IsEmpty()) return std::string();
  // Reading a JS global can run a getter; resolution must stay side-effect
  // free from the caller's point of view, so an exception here is swallowed
  // rather than left pending on a resolver or walk frame.
  v8::TryCatch tc(isolate);
  v8::Local<v8::Value> originVal;
  if (!context->Global()
           ->Get(context,
                 ArgConverter::ConvertToV8String(isolate, "__NS_HTTP_ORIGIN__"))
           .ToLocal(&originVal) ||
      !originVal->IsString()) {
    return std::string();
  }
  v8::String::Utf8Value o8(isolate, originVal);
  std::string origin = *o8 ? *o8 : "";
  if (origin.empty() ||
      !(StartsWith(origin, "http://") || StartsWith(origin, "https://"))) {
    return std::string();
  }
  if (origin.back() != '/') origin += '/';
  return origin;
}

// `referrerKey` is the registry key of the importing module — empty when the
// importer is unknown (a dynamic import with no compiled referrer).
static ModuleResolution ResolveSpecifierToPath(v8::Isolate* isolate,
                                               v8::Local<v8::Context> context,
                                               const std::string& rawSpec,
                                               const std::string& referrerKey) {
  ModuleResolution result;
  if (rawSpec.empty()) return result;

  // Builtins resolve before any path handling, so a file can never shadow one.
  // The whole scheme is claimed, registered or not, so an unknown `node:` name
  // fails as a missing builtin instead of falling through to the filesystem.
  if (NsBuiltinModules::IsBuiltinScheme(rawSpec)) {
    result.kind = ModuleResolution::Kind::kBuiltin;
    result.specifier = rawSpec;
    return result;
  }

  std::string spec = rawSpec;
  // Repair 'http:/host' (single slash) left by upstream path joins, so the URL
  // takes the HTTP path instead of becoming '/app/http:/host'.
  if (spec.rfind("http:/", 0) == 0 && spec.rfind("http://", 0) != 0) {
    spec.insert(5, "/");
  } else if (spec.rfind("https:/", 0) == 0 && spec.rfind("https://", 0) != 0) {
    spec.insert(6, "/");
  }

  TNS_DEBUG(Esm, "[resolver][spec] %s", spec.c_str());

  // A bare '@' is never a module; some dev toolchains emit it during bootstrap.
  if (spec == "@") return result;

  // The import map is consulted before any other resolution: bare specifiers
  // resolve through it to vendor or HTTP URLs. A client that rewrites
  // specifiers must map every form it emits — keys are matched literally.
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState != nullptr && !moduleState->vocabulary.importMap.empty()) {
    const LoaderVocabulary& vocabulary = moduleState->vocabulary;
    std::string mapped = LookupImportMap(vocabulary, spec, referrerKey);
    if (mapped.empty()) {
      std::string normalized = NormalizeViteSpecifier(spec);
      if (!normalized.empty()) {
        mapped = LookupImportMap(vocabulary, normalized, referrerKey);
        if (!mapped.empty()) {
          TNS_DEBUG(Esm, "[resolver][import-map] normalized: %s -> %s -> %s",
                         spec.c_str(), normalized.c_str(), mapped.c_str());
        }
      }
    }
    if (!mapped.empty()) {
      TNS_DEBUG(Esm, "[resolver][import-map] rewrite: %s -> %s", spec.c_str(),
                     mapped.c_str());
      spec = mapped;
    } else {
      // A bare-looking specifier the map didn't match is about to fall back to
      // filesystem resolution and almost certainly fail; surface the missing
      // entry before the more cryptic `Cannot find module` follow-on.
      bool looksBare = spec[0] != '/' && spec[0] != '.' &&
                       spec.find("://") == std::string::npos &&
                       spec.find('\\') == std::string::npos;
      if (looksBare) {
        TNS_DEBUG(Esm, "[resolver][import-map][miss] bare='%s' importMap.size=%lu",
                       spec.c_str(),
                       (unsigned long)vocabulary.importMap.imports.size());
      }
    }
  }

  result.specifier = spec;

  if (StartsWith(spec, "http://") || StartsWith(spec, "https://")) {
    result.kind = ModuleResolution::Kind::kHttp;
    result.url = spec;
    return result;
  }

  TNS_DEBUG(Esm, "[resolver] resolving '%s'", spec.c_str());

  const bool specIsRelative = spec[0] == '.';
  const bool specIsRootAbs = spec[0] == '/';
  std::string referrer = referrerKey;
  if (referrer.empty() && specIsRelative) {
    TNS_DEBUG(Esm, "[resolver] No referrer for relative '%s' - assuming app root",
                   spec.c_str());
    referrer = GetApplicationPath() + "/index.mjs";
  }
  size_t slash = referrer.find_last_of("/\\");
  const std::string baseDir =
      slash == std::string::npos ? "" : referrer.substr(0, slash + 1);

  // A referrer fetched over HTTP makes its relative and root-absolute imports
  // HTTP too, the way a browser resolves them.
  const bool referrerIsHttp = StartsWith(referrer, "http://") ||
                              StartsWith(referrer, "https://");
  if (referrerIsHttp && (specIsRelative || specIsRootAbs)) {
    std::string resolvedHttp = ResolveHttpRelative(referrer, spec);
    if (StartsWith(resolvedHttp, "http://") ||
        StartsWith(resolvedHttp, "https://")) {
      TNS_DEBUG(Esm, "[resolver][http-rel] base=%s spec=%s -> %s",
                     referrer.c_str(), spec.c_str(), resolvedHttp.c_str());
      result.kind = ModuleResolution::Kind::kHttp;
      result.url = resolvedHttp;
      return result;
    }
  } else if (!referrerIsHttp && specIsRootAbs) {
    std::string origin = HttpOriginAnchor(isolate, context);
    if (!origin.empty()) {
      std::string resolved = ResolveHttpRelative(origin, spec);
      if (StartsWith(resolved, "http://") || StartsWith(resolved, "https://")) {
        TNS_DEBUG(Esm, "[resolver][http-origin][fallback] origin=%s spec=%s -> %s",
                       origin.c_str(), spec.c_str(), resolved.c_str());
        result.kind = ModuleResolution::Kind::kHttp;
        result.url = resolved;
        return result;
      }
    }
  }

  // Build the filesystem candidates for this specifier shape. The specifier may
  // omit its extension or name a directory, so each candidate is probed with
  // Node-style extension and index fallbacks below.
  const std::string appPath = GetApplicationPath();
  std::vector<std::string> candidateBases;

  if (specIsRelative) {
    std::string cleanSpec = spec.rfind("./", 0) == 0 ? spec.substr(2) : spec;
    std::string candidate = NormalizePath(baseDir + cleanSpec);
    candidateBases.push_back(candidate);
    TNS_DEBUG(Esm, "[resolver][normalize-rel] %s + %s -> %s", baseDir.c_str(),
                   cleanSpec.c_str(), candidate.c_str());
  } else if (StartsWith(spec, "file://")) {
    // Absolute file URL. Handle the two virtual roots the runtime emits.
    std::string tail = spec.substr(7);
    if (tail.empty() || tail[0] != '/') tail = "/" + tail;

    const std::string appVirtualRoot = "/app/";
    const std::string androidAssetAppRoot = "/android_asset/app/";
    std::string candidate;
    if (tail.rfind(appVirtualRoot, 0) == 0) {
      candidate = appPath + "/" + tail.substr(appVirtualRoot.size());
    } else if (tail.rfind(androidAssetAppRoot, 0) == 0) {
      candidate = appPath + "/" + tail.substr(androidAssetAppRoot.size());
    } else {
      candidate = tail;
    }
    candidateBases.push_back(NormalizePath(candidate));
    TNS_DEBUG(Esm, "[resolver][file-url] tail=%s -> %s", tail.c_str(),
                   candidateBases.back().c_str());
  } else if (spec[0] == '~') {
    std::string tail = spec.size() >= 2 && spec[1] == '/' ? spec.substr(2)
                                                          : spec.substr(1);
    std::string base = NormalizePath(appPath + "/" + tail);
    candidateBases.push_back(base);
    // Also try appPath/app for projects that bundle JS under an app folder.
    std::string baseApp = NormalizePath(appPath + "/app/" + tail);
    if (baseApp != base) candidateBases.push_back(baseApp);
    TNS_DEBUG(Esm, "[resolver][tilde] spec=%s base=%s appBase=%s", spec.c_str(),
                   base.c_str(), baseApp.c_str());
  } else if (specIsRootAbs) {
    // Dynamic import may already have resolved a relative specifier to a real
    // filesystem path under the application root; use that as-is so we don't
    // prefix ApplicationPath twice. Bundle-relative paths like /app/... or
    // /src/... still resolve against appPath.
    if (!appPath.empty() && spec.rfind(appPath, 0) == 0) {
      candidateBases.push_back(NormalizePath(spec));
      TNS_DEBUG(Esm, "[resolver][abs-fs] spec=%s", spec.c_str());
    } else {
      std::string base = NormalizePath(appPath + spec);
      candidateBases.push_back(base);
      const std::string appPrefix = "/app/";
      if (spec.rfind(appPrefix, 0) == 0) {
        std::string tailNoApp = spec.substr(appPrefix.size() - 1);
        std::string baseNoApp = NormalizePath(appPath + tailNoApp);
        if (baseNoApp != base) candidateBases.push_back(baseNoApp);
      }
      TNS_DEBUG(Esm, "[resolver][abs] spec=%s base=%s", spec.c_str(),
                     base.c_str());
    }
  } else {
    // Bare specifier — resolve relative to the application root.
    std::string base = NormalizePath(appPath + "/" + spec);
    candidateBases.push_back(base);
    // Underscore-separated bundler chunk heuristic.
    std::string withSlashes = spec;
    std::replace(withSlashes.begin(), withSlashes.end(), '_', '/');
    std::string baseSlashes = NormalizePath(appPath + "/" + withSlashes);
    if (baseSlashes != base) candidateBases.push_back(baseSlashes);
  }

  std::string absPath;
  bool found = false;
  for (const std::string& baseCandidate : candidateBases) {
    absPath = baseCandidate;

    std::string embedded = HttpUrlEmbeddedInPath(absPath);
    if (!embedded.empty()) {
      TNS_DEBUG(Esm, "[resolver][http-embedded] %s -> %s", absPath.c_str(),
                     embedded.c_str());
      result.kind = ModuleResolution::Kind::kHttp;
      result.url = embedded;
      return result;
    }

    if (IsFile(absPath)) {
      found = true;
      break;
    }
    for (const char* e : {".mjs", ".js"}) {
      std::string cand = NormalizePath(WithExtension(absPath, e));
      if (IsFile(cand)) {
        absPath = cand;
        found = true;
        break;
      }
    }
    if (found) break;
    for (const char* idx : {"/index.mjs", "/index.js"}) {
      std::string cand = NormalizePath(absPath + idx);
      if (IsFile(cand)) {
        absPath = cand;
        found = true;
        break;
      }
    }
    if (found) break;
  }

  if (found) {
    result.kind = ModuleResolution::Kind::kFile;
    result.path = NormalizePath(absPath);
    return result;
  }

  result.attempted = absPath;
  return result;
}

// ─────────────────────────────────────────────────────────────
// Worker isolate detection: iOS keys off Caches::Get(isolate)->isWorker.
// Android encodes the same signal by installing a WORKER_WRAPPER pointer in
// the isolate's data slot on worker isolates only (see Runtime.h).
static bool IsCurrentIsolateWorker(v8::Isolate* isolate) {
  if (isolate == nullptr) return false;
  return isolate->GetData((uint32_t)Runtime::IsolateData::WORKER_WRAPPER) !=
         nullptr;
}

// Monotonic microseconds since some fixed epoch — matches iOS's
// CFAbsoluteTimeGetCurrent() semantic (used for internal timing only, never
// exposed to JS).
static uint64_t MonotonicUs() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)(ts.tv_nsec / 1000);
}

// ─────────────────────────────────────────────────────────────
// The module-graph walk
//
// See the contract comment in ModuleInternalCallbacks.h. Mechanically, per
// edge — local edges never leave the JS thread:
//
//   Enqueue(root)
//     → local: CompileFileEsModule + register under the canonical key
//     → http:  FetchModuleBodyAsync (background thread — see HttpLoader.cpp)
//              → back to the isolate's own event loop as a nestable v8 task
//              → CompileModuleForResolveRegisterOnly (registers under the
//                canonical URL key — the exact entry the resolver looks up)
//     → GetModuleRequests() → ResolveSpecifierToPath → Enqueue(…)
//     → when pendingFetches drains, onComplete fires on the JS thread.
//
// Thread discipline: `visited`, `pendingFetches`, `failed`, `completed` are
// touched ONLY on the isolate's JS thread (every fetch completion hops there
// first). Only raw I/O runs off-thread. The one crossing signal is `dead`,
// an atomic set by isolate teardown so in-flight completions become no-ops
// instead of touching a disposed isolate.

namespace {
struct AsyncGraphLoad {
  v8::Isolate* isolate = nullptr;
  v8::Global<v8::Context> context;
  std::string rootKey;                              // canonical registry key of the root
  robin_hood::unordered_set<std::string> visited;   // canonical keys (JS thread only)
  int pendingFetches = 0;                           // JS thread only
  bool failed = false;                              // JS thread only (root failure)
  bool completed = false;                           // JS thread only
  std::string failureMessage;
  size_t fetchedCount = 0;
  size_t compiledCount = 0;
  uint64_t startUs = 0;
  std::atomic<bool> dead{false};                    // set by isolate teardown (any thread)
  std::function<void(bool ok, const std::string& errorMessage,
                     v8::Local<v8::Context> context)>
      onComplete;

  ~AsyncGraphLoad() {
    g_asyncGraphLoadsInFlightCounter().fetch_sub(1, std::memory_order_acq_rel);
  }

  static std::atomic<int>& g_asyncGraphLoadsInFlightCounter() {
    static std::atomic<int> counter{0};
    return counter;
  }
};

// Adapter so fetch completions ride the isolate's foreground task queue
// (EventLoop::PostV8Task) like any other v8 platform task.
class FetchCompletionTask : public v8::Task {
 public:
  explicit FetchCompletionTask(std::function<void()> fn) : fn_(std::move(fn)) {}
  void Run() override { fn_(); }

 private:
  std::function<void()> fn_;
};

// Registration and quiesce both run on the isolate's thread (the slot
// contract); background fetch completions only ever touch the AsyncGraphLoad
// they retain, never this list, so no lock is needed.
void RegisterAsyncGraphLoad(v8::Isolate* isolate,
                            const std::shared_ptr<AsyncGraphLoad>& load) {
  auto* state = ModuleLoaderStateFor(isolate);
  if (state == nullptr) return;
  auto& loads = state->asyncGraphLoads;
  // Prune expired entries opportunistically so the vector stays small.
  loads.erase(std::remove_if(loads.begin(), loads.end(),
                             [](const std::weak_ptr<AsyncGraphLoad>& w) {
                               return w.expired();
                             }),
              loads.end());
  loads.push_back(load);
}
}  // namespace

bool HasPendingAsyncModuleGraphWork() {
  return AsyncGraphLoad::g_asyncGraphLoadsInFlightCounter().load(
             std::memory_order_acquire) > 0;
}

// Isolate-teardown hook: mark every in-flight load owned by `isolate` dead
// (pending fetch completions become no-ops) and Reset their context Globals
// NOW, while the isolate is still alive — nothing may destroy a v8::Global
// after isolate disposal, and a pending background fetch completion can hold a
// load's shared_ptr past teardown, so the slot destructor alone cannot cover
// these. Called from QuiesceModuleLoadsForIsolate.
static void KillAsyncGraphLoadsForIsolate(v8::Isolate* isolate) {
  auto* state = ModuleLoaderStateFor(isolate);
  if (state == nullptr) return;
  for (auto& weak : state->asyncGraphLoads) {
    if (auto load = weak.lock()) {
      load->dead.store(true, std::memory_order_release);
      load->context.Reset();
    }
  }
  state->asyncGraphLoads.clear();
}

static void AsyncGraphEnqueue(const std::shared_ptr<AsyncGraphLoad>& load,
                              const ModuleResolution& resolution);

// Walk `mod`'s static module requests and enqueue every edge the walk can
// resolve. JS thread only; `moduleKey` is the registry key the module was
// registered under, which is also the referrer for relative resolution.
static void AsyncGraphWalkModuleRequests(
    const std::shared_ptr<AsyncGraphLoad>& load, v8::Local<v8::Context> context,
    v8::Local<v8::Module> mod, const std::string& moduleKey) {
  v8::Isolate* isolate = load->isolate;
  v8::Local<v8::FixedArray> requests = mod->GetModuleRequests();
  const int length = requests->Length();
  for (int i = 0; i < length; i++) {
    v8::Local<v8::ModuleRequest> request =
        requests->Get(i).As<v8::ModuleRequest>();
    if (request.IsEmpty()) continue;
    v8::Local<v8::String> specV8 = request->GetSpecifier();
    v8::String::Utf8Value specUtf8(isolate, specV8);
    if (!*specUtf8) continue;
    // Builtins are served by the resolver from the builtin registry, and an
    // unresolved specifier (typically a bare name with no import-map entry)
    // stays on the resolver's lazy path — where it either resolves later or
    // fails with the resolver's own message. An unmapped bare specifier's
    // subtree is therefore not discovered here; any HTTP edge inside it is
    // pathological and lands on the synchronous anomaly guard.
    const ModuleResolution resolution =
        ResolveSpecifierToPath(isolate, context, *specUtf8, moduleKey);
    if (resolution.kind != ModuleResolution::Kind::kHttp &&
        resolution.kind != ModuleResolution::Kind::kFile) {
      continue;
    }
    AsyncGraphEnqueue(load, resolution);
  }
}

// Fire onComplete exactly once, when the frontier has drained. JS thread only.
static void AsyncGraphMaybeComplete(const std::shared_ptr<AsyncGraphLoad>& load,
                                    v8::Local<v8::Context> context) {
  if (load->completed || load->pendingFetches > 0) return;
  load->completed = true;
  if (LogCategoryEnabled(LogCategory::Esm)) {
    const uint64_t endUs = MonotonicUs();
    const uint64_t ms = endUs > load->startUs ? (endUs - load->startUs) / 1000ull : 0ull;
    TNS_DEBUG(
        Esm,
        "[graph][done] root=%s urls=%lu fetched=%lu compiled=%lu ms=%llu ok=%d",
        load->rootKey.c_str(), (unsigned long)load->visited.size(),
        (unsigned long)load->fetchedCount, (unsigned long)load->compiledCount,
        (unsigned long long)ms, load->failed ? 0 : 1);
  }
  auto onComplete = std::move(load->onComplete);
  load->onComplete = nullptr;
  if (onComplete) {
    v8::TryCatch tc(load->isolate);
    onComplete(!load->failed, load->failureMessage, context);
    (void)tc;  // swallow any pending exception; failures already surface as rejections
  }
}

// A fetch verdict arrived on the isolate's JS thread: compile + register the
// module, then walk its requests. Runs outside any V8 scope, so it enters the
// isolate the same way other cross-thread callbacks do.
static void AsyncGraphOnFetchCompleted(
    const std::shared_ptr<AsyncGraphLoad>& load, const std::string& url,
    const std::shared_ptr<ModuleFetchResult>& fetched) {
  if (load->dead.load(std::memory_order_acquire)) return;
  v8::Isolate* isolate = load->isolate;
  if (Runtime::GetRuntime(isolate) == nullptr) return;

  v8::Locker locker(isolate);
  v8::Isolate::Scope isolate_scope(isolate);
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = load->context.Get(isolate);
  if (context.IsEmpty()) return;
  v8::Context::Scope context_scope(context);

  load->pendingFetches--;

  const std::string key = CanonicalizeRegistryKey(url);
  const bool isRoot = (key == load->rootKey);

  if (!load->failed) {
    if (!fetched->ok) {
      if (isRoot) {
        load->failed = true;
        load->failureMessage = fetched->failureReason;
      } else {
        TNS_DEBUG(Esm, "[graph][dep-fetch-fail] %s (left to sync resolver)",
                       fetched->failureReason.c_str());
      }
    } else if (fetched->kind == ModuleResponseKind::kJson) {
      // JSON compiles, instantiates and evaluates in one step and carries no
      // module requests, so there is nothing further to walk from here.
      load->fetchedCount++;
      v8::TryCatch tcJson(isolate);
      if (CompileJsonTextAsEsModule(isolate, context, fetched->body, key, url)
              .IsEmpty()) {
        if (isRoot) {
          load->failed = true;
          load->failureMessage = "JSON module failed to compile: " + url;
        } else {
          TNS_DEBUG(Esm, "[graph][dep-json-fail] %s (left to sync resolver)",
                         url.c_str());
        }
      } else {
        load->compiledCount++;
      }
    } else {
      load->fetchedCount++;
      v8::Local<v8::Module> mod;
      bool compiled = false;
      std::string compileError;
      {
        // This callback runs on to completion and a microtask checkpoint, so a
        // compile exception must be consumed here rather than left pending; its
        // text goes into the load's own failure channel instead.
        v8::TryCatch tcCompile(isolate);
        compiled = CompileModuleForResolveRegisterOnly(isolate, context,
                                                       fetched->body, key)
                       .ToLocal(&mod);
        if (!compiled) {
          compileError = DescribeCaughtError(isolate, context, tcCompile);
        }
      }
      if (!compiled) {
        if (isRoot) {
          load->failed = true;
          load->failureMessage = "HTTP import compile failed: " + url;
          if (!compileError.empty()) {
            load->failureMessage += " — " + compileError;
          }
        } else {
          TNS_DEBUG(Esm, "[graph][dep-compile-fail] %s %s (left to sync resolver)",
                         url.c_str(), compileError.c_str());
        }
      } else {
        load->compiledCount++;
        AsyncGraphWalkModuleRequests(load, context, mod, key);
      }
    }
  }

  AsyncGraphMaybeComplete(load, context);
  isolate->PerformMicrotaskCheckpoint();
}

// A local edge: read + compile + register it inline, then keep walking. No
// thread hop — the bytes are already on disk, and a hop would only reorder
// discovery. A compile failure is deliberately swallowed here: the walk is a
// discovery optimization, and the resolver (or LoadESModule, for the root)
// owns the error message for a module that will not compile. Leaving it
// unregistered is exactly what makes those paths run and report.
static void AsyncGraphCompileLocalModule(
    const std::shared_ptr<AsyncGraphLoad>& load, v8::Local<v8::Context> context,
    const std::string& path, const std::string& key) {
  v8::Isolate* isolate = load->isolate;
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return;

  v8::Local<v8::Module> mod;
  {
    v8::TryCatch tcCompile(isolate);
    bool compiled = false;
    try {
      compiled =
          tns::ModuleInternal::CompileFileEsModule(isolate, path).ToLocal(&mod);
    } catch (NativeScriptException& ex) {
      TNS_DEBUG(Esm, "[graph][local-compile-fail] %s %s (left to the resolver)",
                     path.c_str(), ex.GetErrorMessage().c_str());
      return;
    }
    if (!compiled) {
      TNS_DEBUG(Esm, "[graph][local-compile-fail] %s (left to the resolver)",
                     path.c_str());
      return;
    }
  }

  UnindexRegistryKey(*moduleState, isolate, key);
  moduleState->registry[key].Reset(isolate, mod);
  IndexRegisteredModule(*moduleState, key, mod);
  load->compiledCount++;
  AsyncGraphWalkModuleRequests(load, context, mod, key);
}

// Enqueue one resolved edge into the walk frontier. JS thread only.
static void AsyncGraphEnqueue(const std::shared_ptr<AsyncGraphLoad>& load,
                              const ModuleResolution& resolution) {
  const bool isHttp = resolution.kind == ModuleResolution::Kind::kHttp;
  const std::string& target = isHttp ? resolution.url : resolution.path;
  // One keying function for both schemes: it dispatches to the HTTP canonical
  // key for URLs and to the normalized path otherwise, so the walk registers
  // every module under the exact key the resolver will look up.
  const std::string key = CanonicalizeRegistryKey(target);
  if (!load->visited.insert(key).second) return;

  v8::Isolate* isolate = load->isolate;
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return;
  auto& g_moduleRegistry = moduleState->registry;
  auto it = g_moduleRegistry.find(key);
  if (it != g_moduleRegistry.end()) {
    v8::Local<v8::Module> existing = it->second.Get(isolate);
    if (!existing.IsEmpty() && existing->GetStatus() != v8::Module::kErrored) {
      if (existing->GetStatus() == v8::Module::kUninstantiated) {
        v8::Local<v8::Context> context = load->context.Get(isolate);
        if (!context.IsEmpty()) {
          AsyncGraphWalkModuleRequests(load, context, existing, key);
        }
      }
      return;  // instantiated/evaluated → its closure is already resolved
    }
    // Errored entry: drop and reload, mirroring LoadHttpModuleForUrl.
    RemoveModuleFromRegistry(key);
  }

  if (!isHttp) {
    // JSON carries no module requests, and it compiles through a different
    // path; there is nothing for the walk to discover in it.
    if (EndsWith(target, ".json")) return;
    v8::Local<v8::Context> context = load->context.Get(isolate);
    if (!context.IsEmpty()) {
      AsyncGraphCompileLocalModule(load, context, target, key);
    }
    return;
  }

  load->pendingFetches++;
  std::shared_ptr<AsyncGraphLoad> loadRef = load;
  const std::string url = target;
  FetchModuleBodyAsync(url, [loadRef, url](ModuleFetchResult result) {
    // Arbitrary thread. Hop to the isolate's home thread as a nestable v8
    // foreground task — delivery is a property of the isolate, not of the
    // thread that started the fetch, and the pumped walk's
    // RunNestableV8Tasks can drain it with JS frames on the stack. A null
    // lookup means the isolate is gone; drop everything, and since teardown
    // quiesces the loads before shutting the loop down, a dropped post holds
    // only already-Reset state.
    if (loadRef->dead.load(std::memory_order_acquire)) return;
    auto* platform = NativeScriptPlatform::Instance();
    std::shared_ptr<EventLoop> loop =
        platform != nullptr ? platform->LookupEventLoop(loadRef->isolate)
                            : nullptr;
    if (loop == nullptr) return;
    auto resultPtr = std::make_shared<ModuleFetchResult>(std::move(result));
    loop->PostV8Task(
        std::make_unique<FetchCompletionTask>([loadRef, url, resultPtr]() {
          AsyncGraphOnFetchCompleted(loadRef, url, resultPtr);
        }),
        /*nestable=*/true, /*delaySeconds=*/0);
  });
}

// Classify a walk root. The root arrives already resolved — an absolute URL
// from the HTTP loader, or a canonical path from LoadESModule — so it needs
// only scheme dispatch, not the full specifier resolution.
static ModuleResolution ResolutionForRoot(const std::string& root) {
  ModuleResolution resolution;
  resolution.specifier = root;
  if (StartsWith(root, "http://") || StartsWith(root, "https://")) {
    resolution.kind = ModuleResolution::Kind::kHttp;
    resolution.url = root;
  } else if (IsFile(root)) {
    resolution.kind = ModuleResolution::Kind::kFile;
    resolution.path = root;
  }
  // Anything else stays kUnresolved: there is nothing to walk, and the
  // caller's own load path reports why.
  return resolution;
}

void StartModuleGraphLoad(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& root,
    std::function<void(bool ok, const std::string& errorMessage,
                       v8::Local<v8::Context> context)>
        onComplete) {
  auto load = std::make_shared<AsyncGraphLoad>();
  load->isolate = isolate;
  load->context.Reset(isolate, context);
  load->rootKey = CanonicalizeRegistryKey(root);
  load->startUs = MonotonicUs();
  load->onComplete = std::move(onComplete);

  AsyncGraphLoad::g_asyncGraphLoadsInFlightCounter().fetch_add(
      1, std::memory_order_acq_rel);
  RegisterAsyncGraphLoad(isolate, load);

  TNS_DEBUG(Esm, "[graph][start] root=%s key=%s", root.c_str(),
                 load->rootKey.c_str());

  const ModuleResolution rootResolution = ResolutionForRoot(root);
  if (rootResolution.kind != ModuleResolution::Kind::kUnresolved) {
    AsyncGraphEnqueue(load, rootResolution);
  }
  // Nothing left pending (a disk-only graph finishes entirely here): complete
  // inline, so the pumped runner below never enters its wait loop.
  AsyncGraphMaybeComplete(load, context);
}

bool RunModuleGraphLoadPumped(v8::Isolate* isolate,
                              v8::Local<v8::Context> context,
                              const std::string& root, double timeoutSeconds) {
  if (timeoutSeconds <= 0.0) timeoutSeconds = kModuleEvaluateDeadlineSeconds;
  auto done = std::make_shared<bool>(false);
  StartModuleGraphLoad(isolate, context, root,
                       [done](bool /*ok*/, const std::string& /*errorMessage*/,
                              v8::Local<v8::Context>) { *done = true; });

  // Manual pump ("until either all is settled or the app takes over"). Fetch
  // completions are nestable v8 foreground tasks on the isolate's event loop,
  // drained directly; the short ALooper slice stays as the idle-wait and still
  // services the other looper-delivered work the walk indirectly depends on. A
  // graph with no HTTP edges is already done here, so the loop body never runs.
  Runtime* runtime = Runtime::GetRuntime(isolate);
  std::shared_ptr<EventLoop> eventLoop =
      runtime != nullptr ? runtime->GetEventLoop() : nullptr;
  const auto deadline =
      std::chrono::steady_clock::now() +
      std::chrono::milliseconds(static_cast<int64_t>(timeoutSeconds * 1000.0));
  while (!*done && std::chrono::steady_clock::now() < deadline) {
    if (eventLoop != nullptr) {
      eventLoop->RunNestableV8Tasks();
    }
    if (*done) break;
    ALooper_pollOnce(10 /* ms */, nullptr, nullptr, nullptr);
  }
  if (!*done) {
    TNS_DEBUG(
        Esm,
        "[graph][pumped][timeout] root=%s after %.1fs (sync loader takes over)",
        root.c_str(), timeoutSeconds);
  }
  return *done;
}

// ─────────────────────────────────────────────────────────────
// Registry mutation + diagnostics

static const char* ModuleStatusToString(v8::Module::Status status) {
  switch (status) {
    case v8::Module::kUninstantiated:
      return "Uninstantiated";
    case v8::Module::kInstantiating:
      return "Instantiating";
    case v8::Module::kInstantiated:
      return "Instantiated";
    case v8::Module::kEvaluating:
      return "Evaluating";
    case v8::Module::kEvaluated:
      return "Evaluated";
    case v8::Module::kErrored:
      return "Errored";
  }
  return "Unknown";
}

void RemoveModuleFromRegistry(const std::string& canonicalPath) {
  // Only ever called on an isolate's own JS thread during module
  // resolution/loading, so the entered isolate owns the maps to mutate.
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return;
  auto& g_moduleRegistry = moduleState->registry;
  const std::string registryKey = CanonicalizeRegistryKey(canonicalPath);

  // Defensive: never operate on an anomalous/sentinel key.
  auto isSentinel = [](const std::string& s) -> bool {
    if (s == "@") return true;
    return s.find("__invalid_at__.mjs") != std::string::npos;
  };
  if (isSentinel(registryKey)) {
    TNS_DEBUG(Esm, "[resolver][guard-v3] ignore remove for sentinel %s",
                   registryKey.c_str());
    return;
  }

  const LoaderVocabulary& vocabulary = moduleState->vocabulary;
  auto classify = [&vocabulary](const std::string& s) -> const char* {
    if (s == "@") return "sentinel:@";
    if (s.find("__invalid_at__.mjs") != std::string::npos)
      return "sentinel:invalid_at";
    bool http = StartsWith(s, "http://") || StartsWith(s, "https://");
    if (http) {
      if (IsVolatileUrl(vocabulary, s)) return "http:volatile";
      if (s.find("/@ns/sfc/") != std::string::npos) return "http:sfc";
      if (s.find("/@ns/m/") != std::string::npos) return "http:m";
      return "http:other";
    }
    if (StartsWith(s, "file://")) return "file-url";
    return "path";
  };

  if (registryKey != canonicalPath) {
    TNS_DEBUG(Esm, "[resolver][remove:pre] raw=%s key=%s class=%s",
                   canonicalPath.c_str(), registryKey.c_str(),
                   classify(registryKey));
  } else {
    TNS_DEBUG(Esm, "[resolver][remove:pre] key=%s class=%s", registryKey.c_str(),
                   classify(registryKey));
  }

  size_t regPre = g_moduleRegistry.size();

  auto it = g_moduleRegistry.find(registryKey);
  if (it != g_moduleRegistry.end()) {
    bool isHttpKey =
        StartsWith(registryKey, "http://") || StartsWith(registryKey, "https://");
    if (!isHttpKey) {
      TNS_DEBUG(Esm, "[resolver] removing stale module %s", registryKey.c_str());
    }
    UnindexRegistryKey(*moduleState, isolate, registryKey);
    it->second.Reset();
    g_moduleRegistry.erase(it);
  } else {
    TNS_DEBUG(Esm, "[resolver][remove:miss] key not found (%s)",
                   registryKey.c_str());
  }

  TNS_DEBUG(Esm, "[resolver][remove:post] reg %lu->%lu", (unsigned long)regPre,
                 (unsigned long)g_moduleRegistry.size());
}

std::vector<std::string> GetLoadedModuleUrls() {
  std::vector<std::string> urls;
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return urls;
  auto& g_moduleRegistry = moduleState->registry;
  urls.reserve(g_moduleRegistry.size());

  for (const auto& entry : g_moduleRegistry) {
    const std::string& key = entry.first;
    if (key.empty()) continue;
    if (StartsWith(key, "blob:") || key.find("://") != std::string::npos) {
      urls.push_back(key);
    }
  }
  std::sort(urls.begin(), urls.end());
  urls.erase(std::unique(urls.begin(), urls.end()), urls.end());
  return urls;
}

void InvalidateModules(v8::Isolate* isolate, v8::Local<v8::Context> context,
                       const std::vector<std::string>& urls) {
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return;
  auto& g_moduleRegistry = moduleState->registry;
  if (urls.empty()) return;

  robin_hood::unordered_set<std::string> seen;
  std::vector<std::string> uniqueUrls;
  uniqueUrls.reserve(urls.size());

  for (const auto& url : urls) {
    if (url.empty()) continue;
    std::string registryKey = CanonicalizeRegistryKey(url);
    if (registryKey.empty()) continue;
    if (!seen.insert(registryKey).second) continue;
    uniqueUrls.push_back(registryKey);
  }

  size_t hits = 0, misses = 0;
  for (const auto& url : uniqueUrls) {
    bool present = g_moduleRegistry.find(url) != g_moduleRegistry.end();
    if (present) hits++;
    else misses++;
    TNS_DEBUG(Registry, "invalidate %s key=%s", present ? "HIT " : "MISS",
                        url.c_str());
    RejectAndClearInvalidatedModuleState(isolate, context, url);
    RemoveModuleFromRegistry(url);
  }

  // Second layer: the OS HTTP cache is outside our control and may serve
  // a previous save's body even with no-store headers. Mark every
  // invalidated key so the NEXT network fetch carries a unique
  // `__ns_dev_nonce` query param — the network sees a URL it has never
  // cached and must go to origin. The nonce is transport-only; module
  // identity stays the canonical URL.
  MarkUrlsForCacheBust(uniqueUrls);

  TNS_DEBUG(Registry, "invalidate summary unique=%lu hits=%lu misses=%lu "
                      "(registry now=%lu)",
                      (unsigned long)uniqueUrls.size(), (unsigned long)hits,
                      (unsigned long)misses, (unsigned long)g_moduleRegistry.size());
}

// ─────────────────────────────────────────────────────────────
// Resolver state
//
// The dynamic-import in-flight set and waiter lists live in ModuleLoaderState
// (per isolate, in a RuntimeState slot).

static bool IsModuleEvaluationInProgress(v8::Module::Status status) {
  return status == v8::Module::kInstantiating ||
         status == v8::Module::kEvaluating;
}

static void ResolveResolversWithModuleNamespace(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    std::vector<v8::Global<v8::Promise::Resolver>>& resolvers,
    v8::Local<v8::Module> module, const std::string& registryKey) {
  if (resolvers.empty()) return;
  if (module.IsEmpty() || module->GetStatus() != v8::Module::kEvaluated) {
    std::string msg = "Module did not finish evaluation: " + registryKey;
    v8::Local<v8::Value> errObj =
        v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg));
    for (auto& resGlobal : resolvers) {
      v8::Local<v8::Promise::Resolver> resolver = resGlobal.Get(isolate);
      if (!resolver.IsEmpty()) {
        resolver->Reject(context, errObj).FromMaybe(false);
      }
      resGlobal.Reset();
    }
    return;
  }
  v8::Local<v8::Value> moduleNamespace = module->GetModuleNamespace();
  for (auto& resGlobal : resolvers) {
    v8::Local<v8::Promise::Resolver> resolver = resGlobal.Get(isolate);
    if (!resolver.IsEmpty()) {
      resolver->Resolve(context, moduleNamespace).FromMaybe(false);
    }
    resGlobal.Reset();
  }
}

static void RejectResolversWithReason(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    std::vector<v8::Global<v8::Promise::Resolver>>& resolvers,
    v8::Local<v8::Value> reason) {
  if (resolvers.empty()) return;
  for (auto& resGlobal : resolvers) {
    v8::Local<v8::Promise::Resolver> resolver = resGlobal.Get(isolate);
    if (!resolver.IsEmpty()) {
      resolver->Reject(context, reason).FromMaybe(false);
    }
    resGlobal.Reset();
  }
}

static bool QueueHttpDynamicWaiterIfInFlight(
    v8::Isolate* isolate, const std::string& registryKey,
    v8::Local<v8::Module> module, v8::Local<v8::Promise::Resolver> resolver) {
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return false;
  auto& g_modulesInFlight = moduleState->modulesInFlight;
  if (registryKey.empty() || module.IsEmpty() ||
      !IsModuleEvaluationInProgress(module->GetStatus()) ||
      g_modulesInFlight.find(registryKey) == g_modulesInFlight.end()) {
    return false;
  }
  moduleState->httpDynamicWaiters[registryKey].emplace_back(isolate, resolver);
  TNS_DEBUG(Esm, "[dyn-import][http-await] queued waiter for %s status=%s",
                 registryKey.c_str(),
                 ModuleStatusToString(module->GetStatus()));
  return true;
}

// Build a rejection reason that PRESERVES the underlying V8 exception text.
static v8::Local<v8::Value> BuildModuleFailureReason(v8::Isolate* isolate,
                                                     v8::TryCatch& tc,
                                                     const char* stage,
                                                     const std::string& urlOrKey) {
  std::string message = std::string(stage) + ": " + urlOrKey;
  if (tc.HasCaught()) {
    v8::Local<v8::Message> excMessage = tc.Message();
    if (!excMessage.IsEmpty()) {
      v8::String::Utf8Value text(isolate, excMessage->Get());
      if (*text != nullptr && strlen(*text) > 0) {
        message += std::string(" — ") + *text;
      }
    } else {
      v8::Local<v8::Value> exception = tc.Exception();
      if (!exception.IsEmpty()) {
        v8::String::Utf8Value text(isolate, exception);
        if (*text != nullptr && strlen(*text) > 0) {
          message += std::string(" — ") + *text;
        }
      }
    }
  }
  TNS_DEBUG(Esm, "[dyn-import][failure] %s", message.c_str());
  return v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, message));
}

static void ResolveHttpDynamicWaiters(v8::Isolate* isolate,
                                      v8::Local<v8::Context> context,
                                      const std::string& registryKey,
                                      v8::Local<v8::Module> module) {
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return;
  // Settling a promise can run its reactions immediately: with the default
  // microtask policy, a Resolve/Reject issued from a plain platform task (no
  // JS on the stack) drains the queue as the API call unwinds. A reaction that
  // re-imports this URL would then see stale routing state and park on a
  // waiter list that was just flushed — a promise nothing would ever settle.
  // So every piece of state that can route a new import onto the waiter list
  // is cleared FIRST; a re-entrant import then takes the registry-hit path.
  std::vector<v8::Global<v8::Promise::Resolver>> resolvers;
  auto& g_httpDynamicWaiters = moduleState->httpDynamicWaiters;
  auto waitIt = g_httpDynamicWaiters.find(registryKey);
  if (waitIt != g_httpDynamicWaiters.end()) {
    resolvers.swap(waitIt->second);
    g_httpDynamicWaiters.erase(waitIt);
  }
  moduleState->modulesInFlight.erase(registryKey);

  ResolveResolversWithModuleNamespace(isolate, context, resolvers, module,
                                      registryKey);
}

static void RejectHttpDynamicWaiters(v8::Isolate* isolate,
                                     v8::Local<v8::Context> context,
                                     const std::string& registryKey,
                                     v8::Local<v8::Value> reason) {
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return;
  // Cleared before rejecting, for the same reason as the resolve path: a
  // rejection handler that retries this URL must not join a flushed waiter
  // list.
  std::vector<v8::Global<v8::Promise::Resolver>> resolvers;
  auto& g_httpDynamicWaiters = moduleState->httpDynamicWaiters;
  auto waitIt = g_httpDynamicWaiters.find(registryKey);
  if (waitIt != g_httpDynamicWaiters.end()) {
    resolvers.swap(waitIt->second);
    g_httpDynamicWaiters.erase(waitIt);
  }
  moduleState->modulesInFlight.erase(registryKey);

  RejectResolversWithReason(isolate, context, resolvers, reason);
}

static void RejectResolversForInvalidation(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    std::vector<v8::Global<v8::Promise::Resolver>>& resolvers,
    const std::string& registryKey) {
  if (resolvers.empty()) return;
  std::string message = "Module invalidated during dev reload: " + registryKey;
  v8::Local<v8::Value> error =
      v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, message));
  for (auto& resolverGlobal : resolvers) {
    v8::Local<v8::Promise::Resolver> resolver = resolverGlobal.Get(isolate);
    if (!resolver.IsEmpty()) {
      resolver->Reject(context, error).FromMaybe(false);
    }
    resolverGlobal.Reset();
  }
}

static void RejectAndClearInvalidatedModuleState(v8::Isolate* isolate,
                                                 v8::Local<v8::Context> context,
                                                 const std::string& registryKey) {
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return;
  auto& g_httpDynamicWaiters = moduleState->httpDynamicWaiters;
  moduleState->modulesInFlight.erase(registryKey);

  auto dynamicWaitIt = g_httpDynamicWaiters.find(registryKey);
  if (dynamicWaitIt != g_httpDynamicWaiters.end()) {
    std::vector<v8::Global<v8::Promise::Resolver>> resolvers;
    resolvers.swap(dynamicWaitIt->second);
    g_httpDynamicWaiters.erase(dynamicWaitIt);
    RejectResolversForInvalidation(isolate, context, resolvers, registryKey);
  }
  TNS_DEBUG(Esm, "[resolver][invalidate-state] cleared in-flight state for %s",
                 registryKey.c_str());
}

// ─────────────────────────────────────────────────────────────
// JSON module → synthetic ES module

// Wrap JSON source as an ES module with the parsed value as its default
// export. Shared by the filesystem path and the HTTP path so a served JSON
// module and an imported .json file behave identically; `displayUrl` only
// names the module in stack traces. Handles registry insertion and eager
// evaluation.
static v8::MaybeLocal<v8::Module> CompileJsonTextAsEsModule(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& jsonText, const std::string& registryAbsPath,
    const std::string& displayUrl) {
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) {
    return v8::MaybeLocal<v8::Module>();
  }
  auto& g_moduleRegistry = moduleState->registry;

  // JSON modules are compiled eagerly to kEvaluated, so a registered entry is
  // complete and must be reused — recompiling would mint a second module
  // identity (and namespace) for the same source on every resolve.
  auto existingIt = g_moduleRegistry.find(registryAbsPath);
  if (existingIt != g_moduleRegistry.end()) {
    v8::Local<v8::Module> existing = existingIt->second.Get(isolate);
    if (!existing.IsEmpty() && existing->GetStatus() == v8::Module::kEvaluated) {
      return v8::MaybeLocal<v8::Module>(existing);
    }
    UnindexRegistryKey(*moduleState, isolate, registryAbsPath);
    existingIt->second.Reset();
    g_moduleRegistry.erase(existingIt);
  }

  TNS_DEBUG(Esm, "[json] wrapping %s", displayUrl.c_str());

  std::string moduleSource = "export default " + jsonText + ";";
  v8::Local<v8::String> sourceText =
      ArgConverter::ConvertToV8String(isolate, moduleSource);
  const std::string& url = displayUrl;

  v8::Local<v8::String> urlString;
  if (!v8::String::NewFromUtf8(isolate, url.c_str(),
                               v8::NewStringType::kNormal)
           .ToLocal(&urlString)) {
    isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(
        isolate, "Failed to create URL string for JSON module")));
    return v8::MaybeLocal<v8::Module>();
  }

  v8::ScriptOrigin origin(urlString, 0, 0, false, -1, v8::Local<v8::Value>(),
                          false, false, true /* is_module */);
  v8::ScriptCompiler::Source src(sourceText, origin);

  v8::Local<v8::Module> jsonModule;
  if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&jsonModule)) {
    isolate->ThrowException(v8::Exception::SyntaxError(
        ArgConverter::ConvertToV8String(isolate, "Failed to compile JSON module")));
    return v8::MaybeLocal<v8::Module>();
  }

  if (!jsonModule->InstantiateModule(context, &ResolveModuleCallback)
           .FromMaybe(false)) {
    return v8::MaybeLocal<v8::Module>();
  }
  v8::MaybeLocal<v8::Value> evalResult = jsonModule->Evaluate(context);
  if (evalResult.IsEmpty()) return v8::MaybeLocal<v8::Module>();

  UnindexRegistryKey(*moduleState, isolate, registryAbsPath);
  auto it = g_moduleRegistry.find(registryAbsPath);
  if (it != g_moduleRegistry.end()) it->second.Reset();
  g_moduleRegistry[registryAbsPath].Reset(isolate, jsonModule);
  IndexRegisteredModule(*moduleState, registryAbsPath, jsonModule);
  return v8::MaybeLocal<v8::Module>(jsonModule);
}

// The filesystem entry point: read the file, then share the wrap.
static v8::MaybeLocal<v8::Module> CompileJsonAsEsModule(
    v8::Isolate* isolate, v8::Local<v8::Context> context,
    const std::string& absPath, const std::string& registryAbsPath) {
  const std::string jsonText = Runtime::GetRuntime(isolate)->ReadFileText(absPath);
  return CompileJsonTextAsEsModule(isolate, context, jsonText, registryAbsPath,
                                   "file://" + absPath);
}

// ─────────────────────────────────────────────────────────────
// ResolveModuleCallback — invoked by V8 to resolve `import X from '<spec>'`.
//
// Every resolution decision belongs to ResolveSpecifierToPath, shared with the
// graph walk; what stays here is the V8-facing half — serving builtins,
// delegating HTTP, and compiling + registering a file.

v8::MaybeLocal<v8::Module> ResolveModuleCallback(
    v8::Local<v8::Context> context, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> /*import_assertions*/,
    v8::Local<v8::Module> referrer) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) {
    return v8::MaybeLocal<v8::Module>();
  }
  auto& g_moduleRegistry = moduleState->registry;

  v8::String::Utf8Value specUtf8(isolate, specifier);
  const std::string rawSpec = *specUtf8 ? *specUtf8 : "";
  if (rawSpec.empty()) return v8::MaybeLocal<v8::Module>();

  // A bare '@' is invalid; refuse to poison the registry, and stay silent
  // rather than throwing — some dev toolchains emit one during bootstrap.
  if (rawSpec == "@") {
    TNS_DEBUG(Esm, "[resolver][normalize] ignoring invalid '@' static spec");
    return v8::MaybeLocal<v8::Module>();
  }

  const bool isWorker = IsCurrentIsolateWorker(isolate);
  const std::string referrerPath =
      FindKeyForModule(*moduleState, isolate, referrer);
  const ModuleResolution resolution =
      ResolveSpecifierToPath(isolate, context, rawSpec, referrerPath);

  switch (resolution.kind) {
    case ModuleResolution::Kind::kBuiltin: {
      v8::Local<v8::Module> builtin;
      if (NsBuiltinModules::GetModule(context, rawSpec).ToLocal(&builtin)) {
        return v8::MaybeLocal<v8::Module>(builtin);
      }
      if (!NsBuiltinModules::IsRegistered(rawSpec)) {
        isolate->ThrowException(
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, NsBuiltinModules::NotFoundMessage(rawSpec))));
      }
      return v8::MaybeLocal<v8::Module>();
    }
    case ModuleResolution::Kind::kHttp:
      // Security: HttpFetchModule gates remote module access centrally.
      return LoadHttpModuleForUrl(isolate, context, resolution.url);
    case ModuleResolution::Kind::kUnresolved: {
      // Surfaced as an exception rather than left to ReadFileText, which would
      // abort trying to open a directory.
      std::string msg = "Cannot find module '" + resolution.specifier +
                        "' (tried " + resolution.attempted + ")";
      isolate->ThrowException(
          v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
      return v8::MaybeLocal<v8::Module>();
    }
    case ModuleResolution::Kind::kFile:
      break;
  }

  const std::string& absPath = resolution.path;
  const std::string registryAbsPath = CanonicalizeRegistryKey(absPath);

  // JSON module: compile a synthetic ESM.
  if (EndsWith(absPath, ".json")) {
    return CompileJsonAsEsModule(isolate, context, absPath, registryAbsPath);
  }

  // Reuse any live, non-errored registry entry. The resolver never evaluates,
  // so an unfinished entry (kUninstantiated / kInstantiating / kEvaluating)
  // simply rejoins the graph V8 is currently linking — that is how import
  // cycles terminate, the same way Node/Blink break them with the module-map
  // self-insert.
  auto it = g_moduleRegistry.find(registryAbsPath);
  if (it != g_moduleRegistry.end()) {
    v8::Local<v8::Module> existing = it->second.Get(isolate);
    if (!existing.IsEmpty() && existing->GetStatus() != v8::Module::kErrored) {
      TNS_DEBUG(Esm, "[resolver] cache hit %s (status=%s)", absPath.c_str(),
                     ModuleStatusToString(existing->GetStatus()));
      return v8::MaybeLocal<v8::Module>(existing);
    }
    RemoveModuleFromRegistry(absPath);
  }

  // Compile + register only — never instantiate or evaluate here. V8 is
  // instantiating the importer and continues the graph walk by resolving this
  // module's own requests next; evaluating inside the resolver would run
  // dependencies in resolver order instead of the spec's evaluation order.
  TNS_DEBUG(Esm, "[resolver] -> compile-register %s", absPath.c_str());
  try {
    v8::Local<v8::Module> mod;
    if (!tns::ModuleInternal::CompileFileEsModule(isolate, absPath)
             .ToLocal(&mod)) {
      // The compile exception is pending on the isolate; V8 fails the
      // importer's instantiation with it.
      return v8::MaybeLocal<v8::Module>();
    }
    UnindexRegistryKey(*moduleState, isolate, registryAbsPath);
    g_moduleRegistry[registryAbsPath].Reset(isolate, mod);
    IndexRegisteredModule(*moduleState, registryAbsPath, mod);
    return v8::MaybeLocal<v8::Module>(mod);
  } catch (NativeScriptException& ex) {
    if (isWorker) {
      DEBUG_WRITE("[resolver] Worker failed to compile '%s' -> '%s'",
                  resolution.specifier.c_str(), absPath.c_str());
    }
    ex.ReThrowToV8();
    return v8::MaybeLocal<v8::Module>();
  }
}

// ─────────────────────────────────────────────────────────────
// FinishHttpDynamicImport
//
// Called on the JS thread once the async graph walk has fetched (and
// registered as uninstantiated) the transitive closure for an HTTP dynamic
// import. Instantiates + evaluates the root and settles all queued
// dynamic-import waiters. Top-level await is fanned out to a Then handler so
// waiters only settle after the returned promise settles.
static void FinishHttpDynamicImport(v8::Isolate* isolate,
                                    v8::Local<v8::Context> context,
                                    const std::string& key,
                                    const std::string& requestUrl) {
  if (LogCategoryEnabled(LogCategory::Esm)) {
    auto* moduleState = ModuleLoaderStateFor(isolate);
    if (moduleState != nullptr &&
        moduleState->registry.find(key) == moduleState->registry.end()) {
      TNS_DEBUG(Esm, "[graph][fallback-sync-load] root missed walk: %s",
                     key.c_str());
    }
  }
  // The loader throws the classifier's reason (status, MIME or transport) on
  // failure; catch it here so it becomes the waiters' rejection instead of a
  // generic message left beside a pending exception.
  v8::TryCatch tcLoad(isolate);
  v8::MaybeLocal<v8::Module> modMaybe =
      LoadHttpModuleForUrl(isolate, context, requestUrl);
  if (!modMaybe.IsEmpty()) {
    v8::Local<v8::Module> mod;
    if (modMaybe.ToLocal(&mod)) {
      if (mod->GetStatus() == v8::Module::kUninstantiated) {
        v8::TryCatch tcInstantiate(isolate);
        if (!mod->InstantiateModule(context, &ResolveModuleCallback)
                 .FromMaybe(false)) {
          RemoveModuleFromRegistry(key);
          RejectHttpDynamicWaiters(
              isolate, context, key,
              BuildModuleFailureReason(isolate, tcInstantiate,
                                       "Instantiation failed (http-loader)",
                                       requestUrl));
          return;
        }
      }

      if (IsModuleEvaluationInProgress(mod->GetStatus())) {
        TNS_DEBUG(Esm,
               "[dyn-import][http-loader] waiting on existing evaluation for %s status=%s",
               key.c_str(), ModuleStatusToString(mod->GetStatus()));
        return;
      }

      if (mod->GetStatus() != v8::Module::kEvaluated) {
        v8::Local<v8::Value> evalResult;
        {
          v8::TryCatch tcEvaluate(isolate);
          if (!mod->Evaluate(context).ToLocal(&evalResult)) {
            RemoveModuleFromRegistry(key);
            RejectHttpDynamicWaiters(
                isolate, context, key,
                BuildModuleFailureReason(isolate, tcEvaluate,
                                         "Evaluation failed (http-loader)",
                                         requestUrl));
            return;
          }
        }
        if (!evalResult.IsEmpty() && evalResult->IsPromise()) {
          v8::Local<v8::Promise> p = evalResult.As<v8::Promise>();
          struct EvalWaitData2 {
            std::string key;
            v8::Global<v8::Context> ctx;
            v8::Global<v8::Module> mod;
          };
          auto* data2 = new EvalWaitData2{
              key, v8::Global<v8::Context>(isolate, context),
              v8::Global<v8::Module>(isolate, mod)};
          auto onFulfilled2 = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            v8::Isolate* iso = info.GetIsolate();
            v8::HandleScope hs(iso);
            if (!info.Data()->IsExternal()) return;
            auto* d = static_cast<EvalWaitData2*>(
                info.Data().As<v8::External>()->Value(
                    v8::kExternalPointerTypeTagDefault));
            v8::Local<v8::Context> ctx = d->ctx.Get(iso);
            std::string keyLocal = d->key;
            v8::Local<v8::Module> modLocal = d->mod.Get(iso);
            ResolveHttpDynamicWaiters(iso, ctx, keyLocal, modLocal);
            delete d;
          };
          auto onRejected2 = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            v8::Isolate* iso = info.GetIsolate();
            v8::HandleScope hs(iso);
            if (!info.Data()->IsExternal()) return;
            auto* d = static_cast<EvalWaitData2*>(
                info.Data().As<v8::External>()->Value(
                    v8::kExternalPointerTypeTagDefault));
            v8::Local<v8::Context> ctx = d->ctx.Get(iso);
            std::string keyLocal = d->key;
            v8::Local<v8::Value> reason =
                (info.Length() > 0)
                    ? info[0]
                    : v8::Exception::Error(ArgConverter::ConvertToV8String(
                          iso, "Evaluation failed (http-loader TLA)"));
            if (LogCategoryEnabled(LogCategory::Esm)) {
              v8::String::Utf8Value r(iso, reason);
              if (*r) {
                TNS_DEBUG(Esm, "[dyn-import][http-loader][tla] rejected: %s", *r);
              }
            }
            RejectHttpDynamicWaiters(iso, ctx, keyLocal, reason);
            delete d;
          };
          v8::Local<v8::FunctionTemplate> thenFulfillTpl2 =
              v8::FunctionTemplate::New(
                  isolate, onFulfilled2,
                  v8::External::New(isolate, data2,
                                    v8::kExternalPointerTypeTagDefault));
          v8::Local<v8::Function> thenFulfill2 =
              thenFulfillTpl2->GetFunction(context).ToLocalChecked();
          v8::Local<v8::FunctionTemplate> thenRejectTpl2 =
              v8::FunctionTemplate::New(
                  isolate, onRejected2,
                  v8::External::New(isolate, data2,
                                    v8::kExternalPointerTypeTagDefault));
          v8::Local<v8::Function> thenReject2 =
              thenRejectTpl2->GetFunction(context).ToLocalChecked();
          p->Then(context, thenFulfill2, thenReject2).ToLocalChecked();
          return;
        }
      }
      ResolveHttpDynamicWaiters(isolate, context, key, mod);
      return;
    }
  }
  v8::Local<v8::Value> reason =
      tcLoad.HasCaught()
          ? tcLoad.Exception()
          : v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "HTTP fetch/compile failed: " + requestUrl));
  tcLoad.Reset();
  RejectHttpDynamicWaiters(isolate, context, key, reason);
}

// ─────────────────────────────────────────────────────────────
// ImportModuleDynamicallyCallback — host callback for `import()` expressions.
//
// Structure mirrors iOS: builtins → import-map → invalid-'@' guard → blob URL
// path → HTTP fast path (with coalescing + cache) → filesystem resolution via
// ResolveModuleCallback → instantiate/evaluate/TLA settle.
v8::MaybeLocal<v8::Promise> ImportModuleDynamicallyCallback(
    v8::Local<v8::Context> context, v8::Local<v8::Data> /*host_defined_options*/,
    v8::Local<v8::Value> resource_name, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> import_assertions) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) {
    return v8::MaybeLocal<v8::Promise>();
  }
  auto& g_moduleRegistry = moduleState->registry;
  auto& g_modulesInFlight = moduleState->modulesInFlight;
  auto& g_httpDynamicWaiters = moduleState->httpDynamicWaiters;

  v8::String::Utf8Value specUtf8(isolate, specifier);
  const char* cSpec = (*specUtf8) ? *specUtf8 : "<invalid>";
  TNS_DEBUG(Esm, "[dyn-import] -> %s", cSpec);
  if (LogCategoryEnabled(LogCategory::Esm)) {
    v8::Local<v8::Value> resName = resource_name;
    if (!resName.IsEmpty() && resName->IsString()) {
      v8::String::Utf8Value rn(isolate, resName);
      if (*rn) {
        TNS_DEBUG(Esm, "[dyn-import][referrer] %s", *rn);
      }
    }
  }

  std::string rawSpec = cSpec ? std::string(cSpec) : std::string();

  // Builtin modules never touch the loader below; the namespace comes straight
  // from the realm's synthetic module.
  if (NsBuiltinModules::IsRegistered(rawSpec) ||
      NsBuiltinModules::IsNsScheme(rawSpec)) {
    v8::EscapableHandleScope builtinScope(isolate);
    v8::Local<v8::Promise::Resolver> builtinResolver;
    if (!v8::Promise::Resolver::New(context).ToLocal(&builtinResolver)) {
      return v8::MaybeLocal<v8::Promise>();
    }
    v8::TryCatch tc(isolate);
    v8::Local<v8::Module> builtin;
    if (NsBuiltinModules::GetModule(context, rawSpec).ToLocal(&builtin)) {
      builtinResolver->Resolve(context, builtin->GetModuleNamespace())
          .FromMaybe(false);
    } else {
      v8::Local<v8::Value> error =
          tc.HasCaught()
              ? tc.Exception()
              : v8::Exception::Error(ArgConverter::ConvertToV8String(
                    isolate, NsBuiltinModules::NotFoundMessage(rawSpec)));
      // Reject must not run with a pending exception on the isolate.
      tc.Reset();
      builtinResolver->Reject(context, error).FromMaybe(false);
    }
    return builtinScope.Escape(builtinResolver->GetPromise());
  }

  std::string normalizedSpec = rawSpec;
  // remove query/hash ONLY for non-HTTP specs
  bool isHttpLike =
      (!normalizedSpec.empty() && (StartsWith(normalizedSpec, "http://") ||
                                    StartsWith(normalizedSpec, "https://")));
  if (!isHttpLike) {
    size_t qpos = normalizedSpec.find_first_of("?#");
    if (qpos != std::string::npos) {
      normalizedSpec = normalizedSpec.substr(0, qpos);
    }
  }
  if (normalizedSpec != rawSpec) {
    specifier = ArgConverter::ConvertToV8String(isolate, normalizedSpec);
    TNS_DEBUG(Esm, "[dyn-import][normalize] %s -> %s", rawSpec.c_str(),
                   normalizedSpec.c_str());
  }

  v8::EscapableHandleScope scope(isolate);

  v8::Local<v8::Promise::Resolver> resolver;
  if (!v8::Promise::Resolver::New(context).ToLocal(&resolver)) {
    return v8::MaybeLocal<v8::Promise>();
  }

  // ── Import map resolution for dynamic import() ──
  // The same scoped lookup the resolver and the walk use. The referrer key
  // comes from the host-supplied resource name, canonicalized the way the
  // registry keys it, so a scope matches an import() exactly as it matches a
  // static import from the same module.
  const LoaderVocabulary& vocabulary = moduleState->vocabulary;
  if (!vocabulary.importMap.empty() && !normalizedSpec.empty() &&
      normalizedSpec != "@") {
    std::string dynamicReferrerKey;
    if (!resource_name.IsEmpty() && resource_name->IsString()) {
      v8::String::Utf8Value resourceUtf8(isolate, resource_name);
      if (*resourceUtf8) {
        dynamicReferrerKey = CanonicalizeRegistryKey(*resourceUtf8);
      }
    }
    std::string mapped = LookupImportMap(vocabulary, normalizedSpec, dynamicReferrerKey);
    if (mapped.empty()) {
      std::string normalized = NormalizeViteSpecifier(normalizedSpec);
      if (!normalized.empty()) {
        mapped = LookupImportMap(vocabulary, normalized, dynamicReferrerKey);
        if (!mapped.empty()) {
          TNS_DEBUG(Esm, "[dyn-import][import-map] normalized: %s -> %s -> %s",
                         normalizedSpec.c_str(), normalized.c_str(), mapped.c_str());
        }
      }
    }
    if (!mapped.empty()) {
      normalizedSpec = mapped;
      specifier = ArgConverter::ConvertToV8String(isolate, normalizedSpec);
      TNS_DEBUG(Esm, "[dyn-import][import-map] rewrite: %s -> %s",
                     rawSpec.c_str(), normalizedSpec.c_str());
    }
  }

  try {
    // Defensive guard: some dev-time toolchains emit a stray import('@') during
    // bootstrap. Treat it as a no-op module to avoid a hard failure.
    if (!normalizedSpec.empty() && normalizedSpec == "@") {
      TNS_DEBUG(Esm,
             "[dyn-import] ignoring invalid '@' spec (returning empty module)");
      const char* kEmptySrc = "export {}\n";
      std::string url = "file:///app/__invalid_at__.mjs";
      v8::MaybeLocal<v8::Module> modMaybe =
          CompileModuleFromSource(isolate, context, kEmptySrc, url);
      v8::Local<v8::Module> mod;
      if (modMaybe.ToLocal(&mod)) {
        const std::string atStubKey = CanonicalizeRegistryKey(url);
        UnindexRegistryKey(*moduleState, isolate, atStubKey);
        g_moduleRegistry[atStubKey].Reset(isolate, mod);
        IndexRegisteredModule(*moduleState, atStubKey, mod);
        if (mod->GetStatus() != v8::Module::kEvaluated) {
          if (mod->Evaluate(context).IsEmpty()) {
            resolver
                ->Reject(context,
                         v8::Exception::Error(ArgConverter::ConvertToV8String(
                             isolate, "Evaluation failed for empty module")))
                .FromMaybe(false);
            return scope.Escape(resolver->GetPromise());
          }
        }
        resolver->Resolve(context, mod->GetModuleNamespace()).FromMaybe(false);
        return scope.Escape(resolver->GetPromise());
      }
    }

    // ── Blob URL support (e.g. blob:nativescript/<uuid>) ──
    // Retrieve the blob content from the global BLOB_STORE via
    // URL.InternalAccessor.getData() (installed by Android's blob-url.js) and
    // compile it as an ES module.
    if (!normalizedSpec.empty() &&
        StartsWith(normalizedSpec, "blob:nativescript/")) {
      const std::string blobRegistryKey = CanonicalizeRegistryKey(normalizedSpec);
      TNS_DEBUG(Esm, "[dyn-import][blob] trying blob URL %s key=%s",
                     normalizedSpec.c_str(), blobRegistryKey.c_str());

      auto existingIt = g_moduleRegistry.find(blobRegistryKey);
      if (existingIt != g_moduleRegistry.end()) {
        v8::Local<v8::Module> existing = existingIt->second.Get(isolate);
        if (!existing.IsEmpty()) {
          v8::Module::Status existingStatus = existing->GetStatus();
          TNS_DEBUG(Esm, "[dyn-import][blob-cache] hit %s status=%s",
                         blobRegistryKey.c_str(),
                         ModuleStatusToString(existingStatus));
          if (existingStatus == v8::Module::kErrored) {
            RemoveModuleFromRegistry(blobRegistryKey);
          } else if (IsModuleEvaluationInProgress(existingStatus)) {
            g_modulesInFlight.insert(blobRegistryKey);
            g_httpDynamicWaiters[blobRegistryKey].emplace_back(isolate, resolver);
            TNS_DEBUG(Esm,
                   "[dyn-import][blob-await] queued waiter for %s status=%s",
                   blobRegistryKey.c_str(), ModuleStatusToString(existingStatus));
            return scope.Escape(resolver->GetPromise());
          } else {
            resolver->Resolve(context, existing->GetModuleNamespace())
                .FromMaybe(false);
            return scope.Escape(resolver->GetPromise());
          }
        } else {
          RemoveModuleFromRegistry(blobRegistryKey);
        }
      }

      if (g_modulesInFlight.find(blobRegistryKey) != g_modulesInFlight.end()) {
        TNS_DEBUG(Esm, "[dyn-import][blob] coalesce in-flight %s",
                       blobRegistryKey.c_str());
        g_httpDynamicWaiters[blobRegistryKey].emplace_back(isolate, resolver);
        return scope.Escape(resolver->GetPromise());
      }

      g_modulesInFlight.insert(blobRegistryKey);
      g_httpDynamicWaiters[blobRegistryKey].emplace_back(isolate, resolver);

      v8::TryCatch tc(isolate);
      v8::Local<v8::Object> globalObj = context->Global();

      v8::Local<v8::Value> urlCtorVal;
      if (!globalObj
               ->Get(context, ArgConverter::ConvertToV8String(isolate, "URL"))
               .ToLocal(&urlCtorVal) ||
          !urlCtorVal->IsFunction()) {
        TNS_DEBUG(Esm, "[dyn-import][blob] URL constructor not found");
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "URL constructor not available")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Object> urlCtor = urlCtorVal.As<v8::Object>();

      v8::Local<v8::Value> internalAccessorVal;
      if (!urlCtor
               ->Get(context, ArgConverter::ConvertToV8String(isolate,
                                                              "InternalAccessor"))
               .ToLocal(&internalAccessorVal) ||
          !internalAccessorVal->IsObject()) {
        TNS_DEBUG(Esm, "[dyn-import][blob] URL.InternalAccessor not found");
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "URL.InternalAccessor not available")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Object> internalAccessor =
          internalAccessorVal.As<v8::Object>();

      v8::Local<v8::Value> getDataVal;
      if (!internalAccessor
               ->Get(context,
                     ArgConverter::ConvertToV8String(isolate, "getData"))
               .ToLocal(&getDataVal) ||
          !getDataVal->IsFunction()) {
        TNS_DEBUG(Esm, "[dyn-import][blob] URL.InternalAccessor.getData not found");
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "URL.InternalAccessor.getData not available")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Function> getDataFn = getDataVal.As<v8::Function>();

      v8::Local<v8::Value> urlArg =
          ArgConverter::ConvertToV8String(isolate, normalizedSpec);
      v8::Local<v8::Value> blobDataVal;
      if (!getDataFn->Call(context, internalAccessor, 1, &urlArg)
               .ToLocal(&blobDataVal) ||
          blobDataVal->IsNullOrUndefined()) {
        TNS_DEBUG(Esm, "[dyn-import][blob] blob not found in BLOB_STORE: %s",
                       normalizedSpec.c_str());
        std::string msg = "Blob not found: " + normalizedSpec;
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
        return scope.Escape(resolver->GetPromise());
      }

      if (!blobDataVal->IsObject()) {
        TNS_DEBUG(Esm, "[dyn-import][blob] blob data is not an object");
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(
                ArgConverter::ConvertToV8String(isolate, "Invalid blob data")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Object> blobData = blobDataVal.As<v8::Object>();

      v8::Local<v8::Value> blobVal;
      if (!blobData
               ->Get(context, ArgConverter::ConvertToV8String(isolate, "blob"))
               .ToLocal(&blobVal) ||
          !blobVal->IsObject()) {
        TNS_DEBUG(Esm, "[dyn-import][blob] blob property not found");
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "Blob object not found")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Object> blobObj = blobVal.As<v8::Object>();

      v8::Local<v8::Value> textFnVal;
      if (!blobObj
               ->Get(context, ArgConverter::ConvertToV8String(isolate, "text"))
               .ToLocal(&textFnVal) ||
          !textFnVal->IsFunction()) {
        TNS_DEBUG(Esm, "[dyn-import][blob] Blob.text() not available");
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "Blob.text() not available")));
        return scope.Escape(resolver->GetPromise());
      }
      v8::Local<v8::Function> textFn = textFnVal.As<v8::Function>();

      // Keep the two failure modes distinct — a throw out of text() and a
      // non-thenable return — and carry the thrown value's text into the
      // rejection to preserve diagnostics.
      v8::Local<v8::Value> textResultVal;
      std::string textFailure;
      {
        v8::TryCatch textTc(isolate);
        if (!textFn->Call(context, blobObj, 0, nullptr)
                 .ToLocal(&textResultVal)) {
          textFailure = "Blob.text() threw";
          if (textTc.HasCaught()) {
            v8::String::Utf8Value thrown(isolate, textTc.Exception());
            if (*thrown) {
              textFailure += std::string(": ") + *thrown;
            }
          }
        }
      }

      v8::Local<v8::Promise> textPromise;
      if (textFailure.empty() &&
          !AdoptThenable(isolate, context, textResultVal).ToLocal(&textPromise)) {
        textFailure = "Blob.text() did not return a thenable";
      }
      if (!textFailure.empty()) {
        TNS_DEBUG(Esm, "[dyn-import][blob] %s", textFailure.c_str());
        RejectHttpDynamicWaiters(
            isolate, context, blobRegistryKey,
            v8::Exception::Error(
                ArgConverter::ConvertToV8String(isolate, textFailure)));
        return scope.Escape(resolver->GetPromise());
      }

      struct BlobImportData {
        v8::Global<v8::Context> ctx;
        std::string blobUrl;
        std::string registryKey;
      };
      auto* data = new BlobImportData{v8::Global<v8::Context>(isolate, context),
                                       normalizedSpec, blobRegistryKey};

      auto onFulfilled = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        v8::Isolate* iso = info.GetIsolate();
        v8::HandleScope hs(iso);
        if (!info.Data()->IsExternal()) return;
        auto* d = static_cast<BlobImportData*>(
            info.Data().As<v8::External>()->Value(
                v8::kExternalPointerTypeTagDefault));
        v8::Local<v8::Context> ctx = d->ctx.Get(iso);

        if (info.Length() < 1 || !info[0]->IsString()) {
          RejectHttpDynamicWaiters(iso, ctx, d->registryKey,
                                   v8::Exception::Error(
                                       ArgConverter::ConvertToV8String(
                                           iso, "Blob text is not a string")));
          delete d;
          return;
        }

        v8::String::Utf8Value codeUtf8(iso, info[0]);
        std::string code = *codeUtf8 ? *codeUtf8 : "";

        TNS_DEBUG(Esm, "[dyn-import][blob] compiling blob module, code length=%zu",
                       code.size());

        v8::Local<v8::Module> mod;
        bool compiled = false;
        std::string compileError;
        {
          // A pending exception would escape this callback into V8's promise
          // machinery; the waiters are this path's failure channel.
          v8::TryCatch tcCompile(iso);
          compiled = CompileModuleForResolveRegisterOnly(iso, ctx, code, d->blobUrl)
                         .ToLocal(&mod);
          if (!compiled) {
            compileError = DescribeCaughtError(iso, ctx, tcCompile);
          }
        }
        if (!compiled) {
          std::string msg = "Failed to compile blob module";
          if (!compileError.empty()) {
            msg += ": " + compileError;
          }
          RejectHttpDynamicWaiters(
              iso, ctx, d->registryKey,
              v8::Exception::Error(ArgConverter::ConvertToV8String(iso, msg)));
          delete d;
          return;
        }

        if (mod->GetStatus() == v8::Module::kUninstantiated &&
            !mod->InstantiateModule(ctx, &ResolveModuleCallback)
                 .FromMaybe(false)) {
          RemoveModuleFromRegistry(d->registryKey);
          RejectHttpDynamicWaiters(
              iso, ctx, d->registryKey,
              v8::Exception::Error(ArgConverter::ConvertToV8String(
                  iso, "Failed to instantiate blob module")));
          delete d;
          return;
        }

        if (IsModuleEvaluationInProgress(mod->GetStatus())) {
          TNS_DEBUG(Esm,
                 "[dyn-import][blob] waiting on existing evaluation for %s status=%s",
                 d->registryKey.c_str(), ModuleStatusToString(mod->GetStatus()));
          delete d;
          return;
        }

        if (mod->GetStatus() != v8::Module::kEvaluated) {
          v8::Local<v8::Value> evalResult;
          if (!mod->Evaluate(ctx).ToLocal(&evalResult)) {
            RemoveModuleFromRegistry(d->registryKey);
            RejectHttpDynamicWaiters(
                iso, ctx, d->registryKey,
                v8::Exception::Error(ArgConverter::ConvertToV8String(
                    iso, "Failed to evaluate blob module")));
            delete d;
            return;
          }

          if (!evalResult.IsEmpty() && evalResult->IsPromise()) {
            struct BlobEvalData {
              std::string registryKey;
              v8::Global<v8::Context> ctx;
              v8::Global<v8::Module> mod;
            };
            auto* evalData = new BlobEvalData{
                d->registryKey, v8::Global<v8::Context>(iso, ctx),
                v8::Global<v8::Module>(iso, mod)};

            auto onEvalFulfilled =
                [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                  v8::Isolate* iso = info.GetIsolate();
                  v8::HandleScope hs(iso);
                  if (!info.Data()->IsExternal()) return;
                  auto* d = static_cast<BlobEvalData*>(
                      info.Data().As<v8::External>()->Value(
                          v8::kExternalPointerTypeTagDefault));
                  v8::Local<v8::Context> ctx = d->ctx.Get(iso);
                  v8::Local<v8::Module> mod = d->mod.Get(iso);
                  ResolveHttpDynamicWaiters(iso, ctx, d->registryKey, mod);
                  delete d;
                };

            auto onEvalRejected =
                [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                  v8::Isolate* iso = info.GetIsolate();
                  v8::HandleScope hs(iso);
                  if (!info.Data()->IsExternal()) return;
                  auto* d = static_cast<BlobEvalData*>(
                      info.Data().As<v8::External>()->Value(
                          v8::kExternalPointerTypeTagDefault));
                  v8::Local<v8::Context> ctx = d->ctx.Get(iso);
                  v8::Local<v8::Value> reason =
                      info.Length() > 0
                          ? info[0]
                          : v8::Exception::Error(
                                ArgConverter::ConvertToV8String(
                                    iso, "Blob module evaluation failed"));
                  RemoveModuleFromRegistry(d->registryKey);
                  RejectHttpDynamicWaiters(iso, ctx, d->registryKey, reason);
                  delete d;
                };

            v8::Local<v8::Promise> evalPromise = evalResult.As<v8::Promise>();
            v8::Local<v8::Function> onEvalFulfilledFn =
                v8::Function::New(
                    ctx, onEvalFulfilled,
                    v8::External::New(iso, evalData,
                                      v8::kExternalPointerTypeTagDefault))
                    .ToLocalChecked();
            v8::Local<v8::Function> onEvalRejectedFn =
                v8::Function::New(
                    ctx, onEvalRejected,
                    v8::External::New(iso, evalData,
                                      v8::kExternalPointerTypeTagDefault))
                    .ToLocalChecked();
            evalPromise->Then(ctx, onEvalFulfilledFn, onEvalRejectedFn)
                .FromMaybe(v8::Local<v8::Promise>());
            delete d;
            return;
          }
        }

        ResolveHttpDynamicWaiters(iso, ctx, d->registryKey, mod);
        delete d;
      };

      auto onRejected = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        v8::Isolate* iso = info.GetIsolate();
        v8::HandleScope hs(iso);
        if (!info.Data()->IsExternal()) return;
        auto* d = static_cast<BlobImportData*>(
            info.Data().As<v8::External>()->Value(
                v8::kExternalPointerTypeTagDefault));
        v8::Local<v8::Context> ctx = d->ctx.Get(iso);
        v8::Local<v8::Value> reason =
            info.Length() > 0
                ? info[0]
                : v8::Exception::Error(ArgConverter::ConvertToV8String(
                      iso, "Blob text() failed"));
        RejectHttpDynamicWaiters(iso, ctx, d->registryKey, reason);
        delete d;
      };

      v8::Local<v8::Function> onFulfilledFn =
          v8::Function::New(
              context, onFulfilled,
              v8::External::New(isolate, data,
                                v8::kExternalPointerTypeTagDefault))
              .ToLocalChecked();
      v8::Local<v8::Function> onRejectedFn =
          v8::Function::New(
              context, onRejected,
              v8::External::New(isolate, data,
                                v8::kExternalPointerTypeTagDefault))
              .ToLocalChecked();

      textPromise->Then(context, onFulfilledFn, onRejectedFn)
          .FromMaybe(v8::Local<v8::Promise>());

      return scope.Escape(resolver->GetPromise());
    }

    // ── HTTP(S) fast path ──
    // Security: HttpFetchModule gates remote module access centrally.
    if (!normalizedSpec.empty() &&
        (StartsWith(normalizedSpec, "http://") ||
         StartsWith(normalizedSpec, "https://"))) {
      TNS_DEBUG(Esm, "[dyn-import][http-loader] trying URL %s",
                     normalizedSpec.c_str());
      std::string key = CanonicalizeHttpUrlKey(normalizedSpec);

      // Volatile-pattern eviction: if the URL matches any configured volatile
      // pattern, evict the cached module so we always re-fetch. Policy is
      // supplied exclusively by JS via ns:module `configureLoader({
      // volatilePatterns })` — the runtime carries no framework or server URL
      // vocabulary of its own.
      bool isVolatile = IsVolatileUrl(vocabulary, normalizedSpec);
      if (isVolatile) {
        auto ex = g_moduleRegistry.find(key);
        if (ex != g_moduleRegistry.end()) {
          TNS_DEBUG(Esm, "[dyn-import][http-cache] drop volatile %s", key.c_str());
          RemoveModuleFromRegistry(key);
        }
      }
      // Coalesce concurrent dynamic imports for the same HTTP key.
      auto inflight = g_modulesInFlight.find(key) != g_modulesInFlight.end();
      if (inflight) {
        TNS_DEBUG(Esm, "[dyn-import][http] coalesce in-flight %s", key.c_str());
        g_httpDynamicWaiters[key].emplace_back(isolate, resolver);
        return scope.Escape(resolver->GetPromise());
      }
      // If module was already compiled, resolve immediately.
      auto itExisting = g_moduleRegistry.find(key);
      if (itExisting != g_moduleRegistry.end()) {
        v8::Local<v8::Module> existing = itExisting->second.Get(isolate);
        if (!existing.IsEmpty()) {
          TNS_DEBUG(Esm, "[dyn-import][http-cache] hit %s status=%s", key.c_str(),
                         ModuleStatusToString(existing->GetStatus()));
          v8::Module::Status st = existing->GetStatus();
          if (st == v8::Module::kErrored) {
            TNS_DEBUG(Esm, "[dyn-import][http-cache] dropping errored module for %s",
                           key.c_str());
            RemoveModuleFromRegistry(key);
          } else if (IsModuleEvaluationInProgress(st)) {
            if (QueueHttpDynamicWaiterIfInFlight(isolate, key, existing,
                                                 resolver)) {
              return scope.Escape(resolver->GetPromise());
            }
            TNS_DEBUG(Esm,
                   "[dyn-import][http-cache] avoiding re-entrant Evaluate for %s status=%s",
                   key.c_str(), ModuleStatusToString(st));
            resolver->Resolve(context, existing->GetModuleNamespace())
                .FromMaybe(false);
            return scope.Escape(resolver->GetPromise());
          } else {
            if (st != v8::Module::kEvaluated) {
              g_modulesInFlight.insert(key);
              TNS_DEBUG(Esm, "[dyn-import][http-cache] awaiting evaluation %s",
                             key.c_str());
              g_httpDynamicWaiters[key].emplace_back(isolate, resolver);
              if (st == v8::Module::kUninstantiated) {
                v8::TryCatch tcInstantiate(isolate);
                if (!existing->InstantiateModule(context, &ResolveModuleCallback)
                         .FromMaybe(false)) {
                  RemoveModuleFromRegistry(key);
                  RejectHttpDynamicWaiters(
                      isolate, context, key,
                      BuildModuleFailureReason(
                          isolate, tcInstantiate,
                          "Instantiation failed (http-cache hit)", key));
                  return scope.Escape(resolver->GetPromise());
                }
              }

              if (IsModuleEvaluationInProgress(existing->GetStatus())) {
                return scope.Escape(resolver->GetPromise());
              }

              v8::Local<v8::Value> evalResult;
              {
                v8::TryCatch tcEvaluate(isolate);
                if (!existing->Evaluate(context).ToLocal(&evalResult)) {
                  RemoveModuleFromRegistry(key);
                  RejectHttpDynamicWaiters(
                      isolate, context, key,
                      BuildModuleFailureReason(
                          isolate, tcEvaluate,
                          "Evaluation failed (http-cache hit)", key));
                  return scope.Escape(resolver->GetPromise());
                }
              }
              if (!evalResult.IsEmpty() && evalResult->IsPromise()) {
                v8::Local<v8::Promise> p = evalResult.As<v8::Promise>();
                struct EvalWaitData {
                  std::string key;
                  v8::Global<v8::Context> ctx;
                  v8::Global<v8::Module> mod;
                };
                auto* data = new EvalWaitData{
                    key, v8::Global<v8::Context>(isolate, context),
                    v8::Global<v8::Module>(isolate, existing)};
                auto onFulfilled =
                    [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                      v8::Isolate* iso = info.GetIsolate();
                      v8::HandleScope hs(iso);
                      if (!info.Data()->IsExternal()) return;
                      auto* d = static_cast<EvalWaitData*>(
                          info.Data().As<v8::External>()->Value(
                              v8::kExternalPointerTypeTagDefault));
                      v8::Local<v8::Context> ctx = d->ctx.Get(iso);
                      std::string keyLocal = d->key;
                      v8::Local<v8::Module> modLocal = d->mod.Get(iso);
                      ResolveHttpDynamicWaiters(iso, ctx, keyLocal, modLocal);
                      delete d;
                    };
                auto onRejected =
                    [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                      v8::Isolate* iso = info.GetIsolate();
                      v8::HandleScope hs(iso);
                      if (!info.Data()->IsExternal()) return;
                      auto* d = static_cast<EvalWaitData*>(
                          info.Data().As<v8::External>()->Value(
                              v8::kExternalPointerTypeTagDefault));
                      v8::Local<v8::Context> ctx = d->ctx.Get(iso);
                      std::string keyLocal = d->key;
                      v8::Local<v8::Value> reason =
                          (info.Length() > 0)
                              ? info[0]
                              : v8::Exception::Error(
                                    ArgConverter::ConvertToV8String(
                                        iso, "Evaluation failed (http-cache TLA)"));
                      if (LogCategoryEnabled(LogCategory::Esm)) {
                        v8::String::Utf8Value r(iso, reason);
                        if (*r) {
                          TNS_DEBUG(Esm,
                                    "[dyn-import][http-cache][tla] rejected: %s",
                                    *r);
                        }
                      }
                      RejectHttpDynamicWaiters(iso, ctx, keyLocal, reason);
                      delete d;
                    };
                v8::Local<v8::FunctionTemplate> thenFulfillTpl =
                    v8::FunctionTemplate::New(
                        isolate, onFulfilled,
                        v8::External::New(isolate, data,
                                          v8::kExternalPointerTypeTagDefault));
                v8::Local<v8::Function> thenFulfill =
                    thenFulfillTpl->GetFunction(context).ToLocalChecked();
                v8::Local<v8::FunctionTemplate> thenRejectTpl =
                    v8::FunctionTemplate::New(
                        isolate, onRejected,
                        v8::External::New(isolate, data,
                                          v8::kExternalPointerTypeTagDefault));
                v8::Local<v8::Function> thenReject =
                    thenRejectTpl->GetFunction(context).ToLocalChecked();
                p->Then(context, thenFulfill, thenReject).ToLocalChecked();
                return scope.Escape(resolver->GetPromise());
              }
              ResolveHttpDynamicWaiters(isolate, context, key, existing);
              return scope.Escape(resolver->GetPromise());
            }
            resolver->Resolve(context, existing->GetModuleNamespace())
                .FromMaybe(false);
            return scope.Escape(resolver->GetPromise());
          }
        }
      }
      // Mark in-flight and start the async graph load.
      g_modulesInFlight.insert(key);
      g_httpDynamicWaiters[key].emplace_back(isolate, resolver);
      const std::string requestUrl = normalizedSpec;
      StartModuleGraphLoad(
          isolate, context, requestUrl,
          [key, requestUrl, isolate](bool ok, const std::string& errorMessage,
                                      v8::Local<v8::Context> completionContext) {
            v8::Isolate* iso = isolate;
            if (!ok) {
              RejectHttpDynamicWaiters(
                  iso, completionContext, key,
                  v8::Exception::Error(
                      ArgConverter::ConvertToV8String(iso, errorMessage)));
              return;
            }
            FinishHttpDynamicImport(iso, completionContext, key, requestUrl);
          });
      return scope.Escape(resolver->GetPromise());
    }

    // ── Filesystem path ──
    // For relative specs, adjust against the referrer's resource URL so
    // ../-segments collapse and the resolver can find the target on disk.
    v8::Local<v8::Module> refMod;
    v8::Local<v8::String> adjustedSpecifier = specifier;
    if (!normalizedSpec.empty() &&
        (normalizedSpec.rfind("./", 0) == 0 ||
         normalizedSpec.rfind("../", 0) == 0)) {
      v8::Local<v8::Value> resName = resource_name;
      if (!resName.IsEmpty() && resName->IsString()) {
        v8::String::Utf8Value rn(isolate, resName);
        std::string refUrl = *rn ? *rn : std::string();
        if (!refUrl.empty()) {
          std::string refPath = FileURLToPath(refUrl);
          size_t slash = refPath.find_last_of("/\\");
          std::string baseDir = slash == std::string::npos
                                    ? std::string()
                                    : refPath.substr(0, slash + 1);
          TNS_DEBUG(Esm, "[dyn-import][ref] url=%s base=%s spec=%s", refUrl.c_str(),
                         baseDir.c_str(), normalizedSpec.c_str());
          std::string fsPath = NormalizePath(baseDir + normalizedSpec);
          if (!fsPath.empty()) {
            adjustedSpecifier =
                ArgConverter::ConvertToV8String(isolate, fsPath);
            TNS_DEBUG(Esm, "[dyn-import][normalize-rel] %s + %s -> %s",
                           baseDir.c_str(), normalizedSpec.c_str(),
                           fsPath.c_str());
          }
        }
      } else {
        TNS_DEBUG(
            Esm,
            "[dyn-import][ref] missing resource name; cannot normalize relative "
            "spec against referrer");
      }
    }

    // Discovery pre-pass, the same one the static path runs: a local graph can
    // reach HTTP edges, and without the walk those meet the resolver cold and
    // fetch serially, one blocking round trip each. A graph with no HTTP edges
    // settles inside the call, so a local-only dynamic import is unchanged —
    // it neither waits nor touches the looper.
    {
      v8::String::Utf8Value adjustedUtf8(isolate, adjustedSpecifier);
      const ModuleResolution rootResolution = ResolveSpecifierToPath(
          isolate, context, *adjustedUtf8 ? *adjustedUtf8 : "", std::string());
      if (rootResolution.kind == ModuleResolution::Kind::kFile) {
        RunModuleGraphLoadPumped(isolate, context, rootResolution.path,
                                 kModuleEvaluateDeadlineSeconds);
      }
    }

    v8::TryCatch resolveTc(isolate);
    v8::MaybeLocal<v8::Module> maybeModule = ResolveModuleCallback(
        context, adjustedSpecifier, import_assertions, refMod);
    if (LogCategoryEnabled(LogCategory::Esm)) {
      v8::String::Utf8Value adj(isolate, adjustedSpecifier);
      const char* cAdj = (*adj) ? *adj : "<invalid>";
      TNS_DEBUG(Esm, "[dyn-import][resolver-call] raw=%s normalized=%s adjusted=%s",
                     rawSpec.c_str(), normalizedSpec.c_str(), cAdj);
    }
    if (maybeModule.IsEmpty()) {
      if (resolveTc.HasCaught()) {
        resolver->Reject(context, resolveTc.Exception()).FromMaybe(false);
        return scope.Escape(resolver->GetPromise());
      } else {
        std::string msg = "Module resolution failed for dynamic import: ";
        msg += normalizedSpec.empty() ? "<empty>" : normalizedSpec;
        resolver
            ->Reject(context, v8::Exception::Error(
                                  ArgConverter::ConvertToV8String(isolate, msg)))
            .FromMaybe(false);
        return scope.Escape(resolver->GetPromise());
      }
    }

    v8::Local<v8::Module> module = maybeModule.ToLocalChecked();

    if (module->GetStatus() == v8::Module::kUninstantiated) {
      v8::TryCatch ictc(isolate);
      if (!module->InstantiateModule(context, &ResolveModuleCallback)
               .FromMaybe(false)) {
        TNS_DEBUG(Esm, "[dyn-import] instantiate failed %s",
                       normalizedSpec.c_str());
        std::string msg =
            std::string("Failed to instantiate module: ") + normalizedSpec;
        if (ictc.HasCaught()) {
          std::string exStr = ArgConverter::ToString(isolate, ictc.Exception());
          if (!exStr.empty()) {
            msg.append(" - ");
            msg.append(exStr);
          }
        }
        resolver
            ->Reject(context, v8::Exception::Error(
                                  ArgConverter::ConvertToV8String(isolate, msg)))
            .Check();
        return scope.Escape(resolver->GetPromise());
      }
    }

    // A kEvaluating module (TLA in flight, or a cycle re-entry) falls through
    // deliberately: Evaluate() on an already-evaluating module returns its
    // existing top-level capability promise, so the TLA chain below coalesces
    // this import with the in-flight evaluation.
    if (module->GetStatus() != v8::Module::kEvaluated) {
      v8::Local<v8::Value> evalResult;
      if (!module->Evaluate(context).ToLocal(&evalResult)) {
        TNS_DEBUG(Esm, "[dyn-import] evaluation failed %s",
                       normalizedSpec.c_str());
        std::string msg =
            std::string("Evaluation failed for module: ") + normalizedSpec;
        v8::Local<v8::Value> ex = v8::Exception::Error(
            ArgConverter::ConvertToV8String(isolate, msg));
        resolver->Reject(context, ex).Check();
        return scope.Escape(resolver->GetPromise());
      }
      if (!evalResult.IsEmpty() && evalResult->IsPromise()) {
        v8::Local<v8::Promise> p = evalResult.As<v8::Promise>();
        struct DynEvalData {
          v8::Global<v8::Context> ctx;
          v8::Global<v8::Module> mod;
          v8::Global<v8::Promise::Resolver> res;
        };
        auto* d = new DynEvalData{
            v8::Global<v8::Context>(isolate, context),
            v8::Global<v8::Module>(isolate, module),
            v8::Global<v8::Promise::Resolver>(isolate, resolver)};
        auto onFulfilled = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
          v8::Isolate* iso = info.GetIsolate();
          v8::HandleScope hs(iso);
          if (!info.Data()->IsExternal()) return;
          auto* d = static_cast<DynEvalData*>(
              info.Data().As<v8::External>()->Value(
                  v8::kExternalPointerTypeTagDefault));
          v8::Local<v8::Context> ctx = d->ctx.Get(iso);
          v8::Local<v8::Module> modLocal = d->mod.Get(iso);
          v8::Local<v8::Promise::Resolver> res = d->res.Get(iso);
          TNS_DEBUG(Esm, "[dyn-import][tla] fulfilled, resolving namespace");
          if (!res.IsEmpty())
            res->Resolve(ctx, modLocal->GetModuleNamespace()).FromMaybe(false);
          delete d;
        };
        auto onRejected = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
          v8::Isolate* iso = info.GetIsolate();
          v8::HandleScope hs(iso);
          if (!info.Data()->IsExternal()) return;
          auto* d = static_cast<DynEvalData*>(
              info.Data().As<v8::External>()->Value(
                  v8::kExternalPointerTypeTagDefault));
          v8::Local<v8::Context> ctx = d->ctx.Get(iso);
          v8::Local<v8::Promise::Resolver> res = d->res.Get(iso);
          v8::Local<v8::Value> reason =
              (info.Length() > 0)
                  ? info[0]
                  : v8::Exception::Error(ArgConverter::ConvertToV8String(
                        iso, "Evaluation failed (TLA)"));
          if (LogCategoryEnabled(LogCategory::Esm)) {
            v8::String::Utf8Value r(iso, reason);
            if (*r) {
              TNS_DEBUG(Esm, "[dyn-import][tla] rejected: %s", *r);
            }
          }
          if (!res.IsEmpty()) res->Reject(ctx, reason).FromMaybe(false);
          delete d;
        };
        v8::Local<v8::FunctionTemplate> fulfillTpl = v8::FunctionTemplate::New(
            isolate, onFulfilled,
            v8::External::New(isolate, d, v8::kExternalPointerTypeTagDefault));
        v8::Local<v8::Function> fulfill =
            fulfillTpl->GetFunction(context).ToLocalChecked();
        v8::Local<v8::FunctionTemplate> rejectTpl = v8::FunctionTemplate::New(
            isolate, onRejected,
            v8::External::New(isolate, d, v8::kExternalPointerTypeTagDefault));
        v8::Local<v8::Function> reject =
            rejectTpl->GetFunction(context).ToLocalChecked();
        p->Then(context, fulfill, reject).ToLocalChecked();
        return scope.Escape(resolver->GetPromise());
      }
    }

    // Final verify before resolving for non-HTTP paths.
    v8::Local<v8::Value> nsFinal = module->GetModuleNamespace();
    if (nsFinal->IsObject()) {
      v8::Local<v8::Object> o = nsFinal.As<v8::Object>();
      v8::TryCatch tc3(isolate);
      v8::Local<v8::Value> defVal;
      if (!o->Get(context, ArgConverter::ConvertToV8String(isolate, "default"))
               .ToLocal(&defVal)) {
        TNS_DEBUG(Esm,
               "[dyn-import][verify] ns.default threw after eval (generic) %s",
               normalizedSpec.c_str());
        resolver
            ->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(
                                  isolate, "TDZ on default after eval (generic)")))
            .Check();
        return scope.Escape(resolver->GetPromise());
      }
    }
    resolver->Resolve(context, module->GetModuleNamespace()).Check();
    TNS_DEBUG(Esm, "[dyn-import] resolved %s", normalizedSpec.c_str());
  } catch (NativeScriptException& ex) {
    ex.ReThrowToV8();
    TNS_DEBUG(Esm, "[dyn-import] native failed %s", normalizedSpec.c_str());
    resolver
        ->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(
                              isolate, "Native error during dynamic import")))
        .Check();
  }

  return scope.Escape(resolver->GetPromise());
}

// ─────────────────────────────────────────────────────────────
// InitializeImportMetaObject — populates `import.meta.url` and
// `import.meta.dirname`. `import.meta.hot` is JS policy and is deliberately
// NOT set here (matches the port spec).
void InitializeImportMetaObject(v8::Local<v8::Context> context,
                                v8::Local<v8::Module> module,
                                v8::Local<v8::Object> meta) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto* moduleState = ModuleLoaderStateFor(isolate);
  if (moduleState == nullptr) return;

  std::string modulePath = FindKeyForModule(*moduleState, isolate, module);
  if (modulePath.empty()) return;

  std::string moduleUrl;
  std::string moduleDirname;
  if (StartsWith(modulePath, "http://") || StartsWith(modulePath, "https://")) {
    moduleUrl = modulePath;
    size_t slash = modulePath.find_last_of('/');
    moduleDirname = slash == std::string::npos ? modulePath
                                               : modulePath.substr(0, slash);
  } else if (StartsWith(modulePath, "blob:")) {
    moduleUrl = modulePath;
    moduleDirname = modulePath;
  } else {
    moduleUrl = StartsWith(modulePath, "file://") ? modulePath
                                                  : ("file://" + modulePath);
    std::string filesystemPath = FileURLToPath(moduleUrl);
    size_t slash = filesystemPath.find_last_of("/\\");
    moduleDirname = slash == std::string::npos ? filesystemPath
                                                : filesystemPath.substr(0, slash);
  }

  meta->CreateDataProperty(
          context, ArgConverter::ConvertToV8String(isolate, "url"),
          ArgConverter::ConvertToV8String(isolate, moduleUrl))
      .FromMaybe(false);
  meta->CreateDataProperty(
          context, ArgConverter::ConvertToV8String(isolate, "dirname"),
          ArgConverter::ConvertToV8String(isolate, moduleDirname))
      .FromMaybe(false);
}

}  // namespace tns
