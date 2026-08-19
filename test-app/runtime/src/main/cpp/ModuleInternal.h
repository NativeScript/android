/*
 * Module.h
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */

#ifndef JNI_MODULE_H_
#define JNI_MODULE_H_

#include "JEnv.h"
#include "v8.h"

#include <string>
#include <map>

namespace tns {

// The single deadline for every module-graph settle wait: the entry
// top-level-await pump in LoadESModule and the pumped module-graph walk. One
// knob, so the waits stay ordered — transport timeouts < this.
inline constexpr double kModuleEvaluateDeadlineSeconds = 60.0;

// How a module graph's evaluation promise is settled.
//   kSyncStrict  - Node's `require(esm)`: an async graph is refused before it
//                  ever evaluates, and the capability promise must already be
//                  settled when Evaluate() returns.
//   kSyncPumping - drive this thread in place until the promise settles or the
//                  window closes. Only legal while nothing else owns the loop
//                  (entry evaluation), and only nestable V8 tasks can run.
//   kAsync       - evaluate and hand the caller the capability promise.
enum class ModuleEvaluationPolicy { kSyncStrict, kSyncPumping, kAsync };

// The state of an entry module's evaluation promise. kNone means the path
// names no registered ES module — a classic script settles synchronously and
// never has one, so it needs no boot backstop.
enum class EntryEvaluationState { kNone, kPending, kFulfilled, kRejected };

struct ModuleEvaluationOptions {
    enum class TimeoutBehavior { kReturnPending, kThrow };

    ModuleEvaluationPolicy policy = ModuleEvaluationPolicy::kSyncStrict;
    // kSyncPumping only: how long the graph gets to settle in-pump.
    double deadlineSeconds = 0.0;
    // kSyncPumping only: what an expired window means.
    TimeoutBehavior timeoutBehavior = TimeoutBehavior::kReturnPending;
    // kSyncPumping only: also give the Android looper a slice per iteration, for
    // graphs whose progress depends on native transports rather than V8 tasks.
    bool pumpRunLoop = false;
};

// Evaluates an instantiated graph under `options`. Returns the capability
// promise for kAsync and an empty handle otherwise; the namespace always comes
// from the module itself. Throws NativeScriptException on failure, in every
// build. `canonicalPath` names the registry entry to evict on failure.
v8::MaybeLocal<v8::Promise> EvaluateModuleGraph(v8::Isolate* isolate,
                                                v8::Local<v8::Context> context,
                                                v8::Local<v8::Module> module,
                                                const std::string& canonicalPath,
                                                const ModuleEvaluationOptions& options);

class ModuleInternal {
    public:
        ModuleInternal();

        ~ModuleInternal();

        void Init(v8::Isolate* isolate, const std::string& baseDir = "");

        void Load(v8::Local<v8::Context> context, const std::string& path);

        /*
         * Reuses `Load` logic and adds TryCatch exception handling to push any unhandled exceptions
         * during script's initial load through the worker scope's `onerror` handler (if implemented before the exception was thrown)
         */
        void LoadWorker(v8::Local<v8::Context> context, const std::string& path);

        /*
         * Checks if target script exists, will throw if negative
         * Used before initializing workers, to ensure a thread will not be created, when the file doesn't exist
         */
        static void CheckFileExists(v8::Isolate* isolate, const std::string& path, const std::string& baseDir);

        // Helper functions for ES module support
        static bool IsESModule(const std::string& path);

        /*
         * Compile/link/evaluate an ES module; returns its namespace object. `policy`
         * decides how the graph's evaluation promise is settled — see
         * ModuleEvaluationPolicy.
         */
        static v8::Local<v8::Value> LoadESModule(v8::Isolate* isolate, const std::string& path,
                                                 const ModuleEvaluationOptions& options);

        /*
         * Installs `createRequire` on the `ns:module` binding object. Kept here rather
         * than with the dev-loader members because it hands out the very require the
         * CommonJS loader builds for every module.
         */
        static bool InstallCreateRequireBinding(v8::Local<v8::Context> context,
                                                v8::Local<v8::Object> binding);

        /*
         * Read + compile `path` as an ES module WITHOUT registering, instantiating or
         * evaluating it. On compile failure the exception is left pending on the isolate
         * (or a NativeScriptException is thrown for setup failures) and the result is empty.
         * This is the resolver's file loader: the resolver must only ever hand V8 a
         * compiled module — evaluation order belongs to V8.
         */
        static v8::MaybeLocal<v8::Module> CompileFileEsModule(v8::Isolate* isolate, const std::string& path);

        /*
         * The entry module's still-pending evaluation promise, or empty when
         * evaluation has settled (classic scripts settle synchronously and always
         * return empty). Callers use this after the entry load to observe a
         * top-level await that outlived the settle window. Note a TLA-parked module
         * reports kEvaluated while its capability promise is still pending, so this
         * probes the promise (Evaluate() hands back the same capability), not the
         * status enum.
         */
        static v8::MaybeLocal<v8::Promise> PendingEntryEvaluation(v8::Isolate* isolate,
                                                                  const std::string& path);

        /*
         * The same probe, but reporting the promise's state rather than only
         * "pending or not" — the boot backstop must tell a rejection from a
         * successful settle. Cheap enough to call once per pump slice: a registry
         * hit plus Evaluate(), which returns the existing capability promise.
         * `rejectionReason` (when non-null) receives the reason's text on kRejected.
         */
        static EntryEvaluationState PollEntryEvaluation(v8::Isolate* isolate,
                                                        const std::string& path,
                                                        std::string* rejectionReason);

        static int MODULE_PROLOGUE_LENGTH;
    private:
        enum class ModulePathKind {
            Global,
            Relative,
            Absolute
        };

        struct ModuleCacheEntry {
            ModuleCacheEntry(v8::Persistent<v8::Object>* _obj)
                    : obj(_obj), isData(false) {
            }

            ModuleCacheEntry(v8::Persistent<v8::Object>* _obj, bool _isData)
                    : obj(_obj), isData(_isData) {
            }

            bool isData;
            v8::Persistent<v8::Object>* obj;
        };

        static void RequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void RequireNativeCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void CreateRequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        void RequireCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args);

        v8::Local<v8::String> WrapModuleContent(const std::string& path);

        v8::Local<v8::Object> LoadImpl(v8::Isolate* isolate, const std::string& moduleName,
                                       const std::string& baseDir, bool& isData,
                                       const ModuleEvaluationOptions& options);

        v8::Local<v8::Object> LoadModule(v8::Isolate* isolate, const std::string& path,
                                         const std::string& moduleCacheKey,
                                         const ModuleEvaluationOptions& options);

        v8::Local<v8::Object> LoadData(v8::Isolate* isolate, const std::string& path);

        v8::Local<v8::Script> LoadScript(v8::Isolate* isolate, const std::string& modulePath, const v8::Local<v8::String>& fullRequiredModulePath);

        /*
         * A require bound to `dirName`, whose ES module loads evaluate under `options`.
         * The options ride along as trailing arguments to the require factory, so
         * nothing about them is ambient — and they are resolved once at mint time,
         * never per require() call.
         */
        v8::Local<v8::Function> GetRequireFunction(v8::Isolate* isolate,
                                                   const std::string& dirName,
                                                   const ModuleEvaluationOptions& options);

        v8::ScriptCompiler::CachedData* TryLoadScriptCache(const std::string& path);

        void SaveScriptCache(const v8::Local<v8::Script> script, const std::string& path);

        ModulePathKind GetModulePathKind(const std::string& path);

    public:
        static jclass MODULE_CLASS;
        static jmethodID RESOLVE_PATH_METHOD_ID;
        static jmethodID GET_APPLICATION_FILES_PATH_METHOD_ID;
        static const char* MODULE_PROLOGUE;
        static const char* MODULE_EPILOGUE;

        v8::Isolate* m_isolate;
        v8::Persistent<v8::Function>* m_requireFunction;
        v8::Persistent<v8::Function>* m_requireFactoryFunction;
        robin_hood::unordered_map<std::string, v8::Persistent<v8::Function>*> m_requireCache;
        robin_hood::unordered_map<std::string, ModuleCacheEntry> m_loadedModules;

        class TempModule {
            public:
                TempModule(ModuleInternal* module, const std::string& modulePath, const std::string& cacheKey, v8::Persistent<v8::Object>* poModuleObj)
                    :m_module(module), m_dispose(true), m_modulePath(modulePath), m_cacheKey(cacheKey), m_poModuleObj(poModuleObj) {
                    m_module->m_loadedModules.emplace(m_modulePath, ModuleCacheEntry(m_poModuleObj));
                    m_module->m_loadedModules.emplace(m_cacheKey, ModuleCacheEntry(m_poModuleObj));
                }

                ~TempModule() {
                    if (m_dispose) {
                        m_module->m_loadedModules.erase(m_modulePath);
                        m_module->m_loadedModules.erase(m_cacheKey);
                        // The isolate is alive on this path, so the handle is
                        // released rather than left for isolate disposal.
                        m_poModuleObj->Reset();
                        delete m_poModuleObj;
                    }
                }

                void SaveToCache() {
                    m_dispose = false;
                }

            private:
                bool m_dispose;
                ModuleInternal* m_module;
                std::string m_modulePath;
                std::string m_cacheKey;
                v8::Persistent<v8::Object>* m_poModuleObj;
        };

};
}

#endif /* JNI_MODULE_H_ */
