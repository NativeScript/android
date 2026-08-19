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
                                                 ModuleEvaluationPolicy policy);

        /*
         * Read + compile `path` as an ES module WITHOUT registering, instantiating or
         * evaluating it. On compile failure the exception is left pending on the isolate
         * (or a NativeScriptException is thrown for setup failures) and the result is empty.
         * This is the resolver's file loader: the resolver must only ever hand V8 a
         * compiled module — evaluation order belongs to V8.
         */
        static v8::MaybeLocal<v8::Module> CompileFileEsModule(v8::Isolate* isolate, const std::string& path);

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

        void RequireCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args);

        v8::Local<v8::String> WrapModuleContent(const std::string& path);

        v8::Local<v8::Object> LoadImpl(v8::Isolate* isolate, const std::string& moduleName, const std::string& baseDir, bool& isData);

        v8::Local<v8::Object> LoadModule(v8::Isolate* isolate, const std::string& path, const std::string& moduleCacheKey);

        v8::Local<v8::Object> LoadData(v8::Isolate* isolate, const std::string& path);

        v8::Local<v8::Script> LoadScript(v8::Isolate* isolate, const std::string& modulePath, const v8::Local<v8::String>& fullRequiredModulePath);

        v8::Local<v8::Function> GetRequireFunction(v8::Isolate* isolate, const std::string& dirName);

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
