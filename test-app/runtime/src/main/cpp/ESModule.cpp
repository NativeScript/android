#include "ManualInstrumentation.h"
#include "NativeScriptException.h"
#include "ArgConverter.h"
#include "Constants.h"
#include "ESModule.h"
#include "File.h"

using namespace v8;
using namespace std;
using namespace tns;

ESModule::ESModule()
    : m_isolate(nullptr) {
}

void ESModule::Init(Isolate* isolate, const string& baseDir) {
    m_isolate = isolate;
    m_baseDir = baseDir.empty() ? Constants::APP_ROOT_FOLDER_PATH : baseDir;

    JEnv env;
    if (MODULE_CLASS == nullptr) {
        MODULE_CLASS = env.FindClass("com/tns/Module");
        assert(MODULE_CLASS != nullptr);

        RESOLVE_PATH_METHOD_ID = env.GetStaticMethodID(MODULE_CLASS, "resolvePath", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        assert(RESOLVE_PATH_METHOD_ID != nullptr);
    }
}

void ESModule::Load(const string& path) {
    string frameName("LoadES6Module " + path);
    tns::instrumentation::Frame frame(frameName.c_str());

    auto isolate = m_isolate;
    auto context = isolate->GetCurrentContext();

    string cacheKey = GetModuleCacheKey(path);
    auto it = m_modules.find(cacheKey);
    if (it != m_modules.end()) {
        Persistent<Module>* poModule = it->second;
        Local<Module> module = poModule->Get(isolate);
        module->Evaluate(context);
        return;
    }

    Local<Module> module = CompileModule(isolate, path);

    TryCatch instantiateTryCatch(isolate);
    Maybe<bool> ok = module->InstantiateModule(context, ESModule::ResolveCallback);
    if (!ok.FromMaybe(false)) {
        throw NativeScriptException(instantiateTryCatch);
    }

    TryCatch evaluateTryCatch(isolate);
    module->Evaluate(context);
    if (evaluateTryCatch.HasCaught()) {
        throw NativeScriptException(evaluateTryCatch);
    }

    Persistent<Module>* poModule = new Persistent<Module>(isolate, module);
    m_modules.emplace(cacheKey, poModule);
}

MaybeLocal<Module> ESModule::ResolveCallback(Local<Context> context, Local<String> specifier, Local<Module> referrer) {
    auto isolate = context->GetIsolate();

    string path = ArgConverter::ConvertToString(specifier);
    string cacheKey = GetModuleCacheKey(path);

    string frameName("ResolveES6Module " + path);
    tns::instrumentation::Frame frame(frameName.c_str());

    auto it = m_modules.find(cacheKey);
    if (it == m_modules.end()) {
        Local<Module> module = CompileModule(isolate, path, referrer);
        Persistent<Module>* poModule = new Persistent<Module>(isolate, module);
        m_modules.emplace(cacheKey, poModule);
        return module;
    }

    Persistent<Module>* poModule = it->second;
    Local<Module> module = poModule->Get(isolate);
    return module;
}

Local<Module> ESModule::CompileModule(Isolate* isolate, const string& modulePath, const Local<Module> referrer) {
    string frameName("CompileES6Module " + modulePath);
    tns::instrumentation::Frame frame(frameName.c_str());

    string baseDir = m_baseDir;
    if (!referrer.IsEmpty()) {
        auto it = m_modulePaths.find(referrer->GetIdentityHash());
        if (it != m_modulePaths.end()) {
            auto referrerPath = it->second;
            baseDir = it->second.substr(0, it->second.find_last_of("\\/"));
        }
    }

    string filename = ResolveModulePath(modulePath, baseDir);
    string content = File::ReadText(filename);

    TryCatch compileTryCatch(isolate);
    Local<Module> module;
    {
        ScriptOrigin origin(
            ArgConverter::ConvertToV8String(isolate, "file://" + filename),
            Local<v8::Integer>(),
            Local<v8::Integer>(),
            Local<v8::Boolean>(),
            Local<v8::Integer>(),
            Local<v8::Value>(),
            Local<v8::Boolean>(),
            Local<v8::Boolean>(),
            True(isolate)
        );

        auto sourceText = ArgConverter::ConvertToV8String(isolate, content);
        ScriptCompiler::Source source(sourceText, origin);
        if (!ScriptCompiler::CompileModule(isolate, &source).ToLocal(&module)) {
            throw NativeScriptException(compileTryCatch, "Script " + filename + " contains compilation errors!");
        }
    }

    m_modulePaths.emplace(module->GetIdentityHash(), filename);

    return module;
}

string ESModule::ResolveModulePath(const string& path, const std::string& baseDir) {
    JEnv env;
    JniLocalRef jsModulename(env.NewStringUTF(path.c_str()));
    JniLocalRef jsBaseDir(env.NewStringUTF(baseDir.c_str()));
    JniLocalRef jsModulePath(env.CallStaticObjectMethod(MODULE_CLASS, RESOLVE_PATH_METHOD_ID, (jstring) jsModulename, (jstring) jsBaseDir));

    auto modulePath = ArgConverter::jstringToString((jstring) jsModulePath);
    return modulePath;
}

string ESModule::GetModuleCacheKey(const string& modulePath) {
    bool isGlobal = modulePath[0] != '.' && modulePath[0] != '/';
    string cacheKey = isGlobal ? modulePath : (m_baseDir + "*" + modulePath);
    return cacheKey;
}

std::map<std::string, v8::Persistent<v8::Module>*> ESModule::m_modules;
std::map<int, std::string> ESModule::m_modulePaths;
string ESModule::m_baseDir;
jclass ESModule::MODULE_CLASS = nullptr;
jmethodID ESModule::RESOLVE_PATH_METHOD_ID = nullptr;
