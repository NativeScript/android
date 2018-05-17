#ifndef ESMODULE_H
#define ESMODULE_H

#include <string>
#include <map>
#include "v8.h"
#include "jni.h"

namespace tns {
class ESModule {
    public:
        ESModule();
        void Init(v8::Isolate* isolate, const std::string& baseDir = "");
        void Load(const std::string& path);
    private:
        v8::Isolate* m_isolate;
        static std::string m_baseDir;
        static v8::MaybeLocal<v8::Module> ResolveCallback(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::Module> referrer);
        static v8::Local<v8::Module> CompileModule(v8::Isolate* isolate, const std::string& modulePath, const v8::Local<v8::Module> referrer = v8::Local<v8::Module>());
        static std::string ResolveModulePath(const std::string& path, const std::string& baseDir);
        static std::string GetModuleCacheKey(const std::string& modulePath);
        static std::map<std::string, v8::Persistent<v8::Module>*> m_modules;
        static std::map<int, std::string> m_modulePaths;

        static jclass MODULE_CLASS;
        static jmethodID RESOLVE_PATH_METHOD_ID;

};
}

#endif //ESMODULE_H
