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

        // Helper functions for ES module support
        static bool IsLikelyOptionalModule(const std::string& moduleName);
        static bool IsESModule(const std::string& path);

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

        static jclass MODULE_CLASS;
        static jmethodID RESOLVE_PATH_METHOD_ID;
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
