/*
 * Module.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */
#include "ModuleInternal.h"
#include "ModuleInternalCallbacks.h"
#include "BuiltinLoader.h"
#include "File.h"
#include "HttpLoader.h"
#include "JniLocalRef.h"
#include "ArgConverter.h"
#include "V8GlobalHelpers.h"
#include "NativeScriptAssert.h"
#include "Constants.h"
#include "CrashBreadcrumbs.h"
#include "NativeScriptException.h"
#include "NsBuiltinModules.h"
#include "napi/NapiModules.h"
#include "Util.h"
#include "SimpleProfiler.h"
#include "include/v8.h"
#include "CallbackHandlers.h"
#include "ManualInstrumentation.h"
#include "Runtime.h"
#include <sstream>
#include <mutex>
#include <libgen.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>
#include <android/looper.h>
#include <chrono>
#include <cstring>

using namespace v8;
using namespace std;
using namespace tns;

static bool IsHttpModulePath(const std::string& path) {
    return path.rfind("http://", 0) == 0 || path.rfind("https://", 0) == 0 ||
           path.rfind("file://http://", 0) == 0 || path.rfind("file://https://", 0) == 0;
}

static std::string NormalizeHttpModuleUrl(const std::string& path) {
    if (path.rfind("file://http://", 0) == 0 || path.rfind("file://https://", 0) == 0) {
        return path.substr(strlen("file://"));
    }
    return path;
}

static std::string PromiseRejectionMessage(Isolate* isolate, Local<Promise> promise,
                                           const std::string& path) {
    std::string errorMessage = "Module evaluation promise rejected: " + path;
    TryCatch tc(isolate);
    Local<Value> reason = promise->Result();
    if (reason.IsEmpty()) {
        return errorMessage;
    }
    if (reason->IsObject()) {
        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> errorObj = reason.As<Object>();
        Local<Value> messageVal;
        if (errorObj->Get(context, ArgConverter::ConvertToV8String(isolate, "message"))
                    .ToLocal(&messageVal) &&
            messageVal->IsString()) {
            v8::String::Utf8Value messageUtf8(isolate, messageVal);
            if (*messageUtf8) {
                errorMessage.append(" — ");
                errorMessage.append(*messageUtf8);
            }
        }
    } else {
        Local<Context> context = isolate->GetCurrentContext();
        auto maybeReasonStr = reason->ToString(context);
        if (!maybeReasonStr.IsEmpty()) {
            v8::String::Utf8Value reasonUtf8(isolate, maybeReasonStr.ToLocalChecked());
            if (*reasonUtf8) {
                errorMessage.append(" — ");
                errorMessage.append(*reasonUtf8);
            }
        }
    }
    if (tc.HasCaught()) {
        tc.Reset();
    }
    return errorMessage;
}

// A package-style specifier: neither a path nor a scheme, so it may be claimed
// by a registry rather than resolved on disk.
static bool IsBareSpecifier(const std::string& specifier) {
    if (specifier.empty() || specifier[0] == '.' || specifier[0] == '/' ||
        specifier[0] == '~') {
        return false;
    }

    return specifier.find(':') == std::string::npos;
}

// Helper function to check if a file path is an ES module (.mjs) but not a source map (.mjs.map)
bool ModuleInternal::IsESModule(const std::string& path) {
    return path.size() >= 4 && path.compare(path.size() - 4, 4, ".mjs") == 0 &&
           !(path.size() >= 8 && path.compare(path.size() - 8, 8, ".mjs.map") == 0);
}

ModuleInternal::ModuleInternal()
    : m_isolate(nullptr), m_requireFunction(nullptr), m_requireFactoryFunction(nullptr) {
}

ModuleInternal::~ModuleInternal() {
    delete this->m_requireFunction;
    delete this->m_requireFactoryFunction;
    for (const auto& pair: this->m_requireCache) {
        delete pair.second;
    }
    this->m_requireCache.clear();
}

void ModuleInternal::Init(Isolate* isolate, const string& baseDir) {
    JEnv env;

    if (MODULE_CLASS == nullptr) {
        MODULE_CLASS = env.FindClass("com/tns/Module");
        assert(MODULE_CLASS != nullptr);

        RESOLVE_PATH_METHOD_ID = env.GetStaticMethodID(MODULE_CLASS, "resolvePath", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        assert(RESOLVE_PATH_METHOD_ID != nullptr);
        
        GET_APPLICATION_FILES_PATH_METHOD_ID = env.GetStaticMethodID(MODULE_CLASS, "getApplicationFilesPath", "()Ljava/lang/String;");
        assert(GET_APPLICATION_FILES_PATH_METHOD_ID != nullptr);
    }

    m_isolate = isolate;

    auto context = isolate->GetCurrentContext();

    auto global = context->Global();

    Local<Value> result;
    auto success = BuiltinLoader::RunBuiltin(context, BuiltinId::kRequireFactory).ToLocal(&result);

    assert(success && result->IsFunction());

    auto requireFactoryFunction = result.As<Function>();

    m_requireFactoryFunction = new Persistent<Function>(isolate, requireFactoryFunction);

    auto requireFuncTemplate = FunctionTemplate::New(isolate, RequireCallback, External::New(isolate, this, v8::kExternalPointerTypeTagDefault));
    auto requireFunc = requireFuncTemplate->GetFunction(context).ToLocalChecked();
    global->Set(context, ArgConverter::ConvertToV8String(isolate, "__nativeRequire"), requireFunc);
    m_requireFunction = new Persistent<Function>(isolate, requireFunc);

    Local<Function> globalRequire;

    if (!baseDir.empty()) {
        globalRequire = GetRequireFunction(isolate, baseDir);
    } else {
        globalRequire = GetRequireFunction(isolate, Constants::APP_ROOT_FOLDER_PATH);
    }
    global->Set(context, ArgConverter::ConvertToV8String(isolate, "require"), globalRequire);
}

Local<Function> ModuleInternal::GetRequireFunction(Isolate* isolate, const string& dirName) {
    TNSPERF();
    Local<Function> requireFunc;

    auto itFound = m_requireCache.find(dirName);

    if (itFound != m_requireCache.end()) {
        requireFunc = Local<Function>::New(isolate, *itFound->second);
    } else {
        auto requireFuncFactory = Local<Function>::New(isolate, *m_requireFactoryFunction);

        auto context = isolate->GetCurrentContext();

        auto requireInternalFunc = Local<Function>::New(isolate, *m_requireFunction);

        Local<Value> args[2] {
            requireInternalFunc, ArgConverter::ConvertToV8String(isolate, dirName)
        };
        Local<Value> result;
        auto thiz = Object::New(isolate);
        auto success = requireFuncFactory->Call(context, thiz, 2, args).ToLocal(&result);

        assert(success && !result.IsEmpty() && result->IsFunction());

        requireFunc = result.As<Function>();

        auto poFunc = new Persistent<Function>(isolate, requireFunc);

        m_requireCache.emplace(dirName, poFunc);
    }

    return requireFunc;
}

void ModuleInternal::RequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    try {
        auto thiz = static_cast<ModuleInternal*>(args.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));
        thiz->RequireCallbackImpl(args);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToV8();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToV8();
    }
}

void ModuleInternal::RequireCallbackImpl(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = args.GetIsolate();

    if (args.Length() != 2) {
        throw NativeScriptException(string("require should be called with two parameters"));
    }
    if (!args[0]->IsString()) {
        throw NativeScriptException(string("require's first parameter should be string"));
    }
    if (!args[1]->IsString()) {
        throw NativeScriptException(string("require's second parameter should be string"));
    }

    string moduleName = ArgConverter::ConvertToString(args[0].As<String>());

    // Builtin modules resolve before any path handling, so they can never be
    // shadowed by a file or a package, and an unknown one fails as a missing
    // builtin rather than as a missing file. Only prefixed specifiers get
    // here: a bare `util` still resolves through npm.
    if (NsBuiltinModules::IsBuiltinScheme(moduleName)) {
        auto context = isolate->GetCurrentContext();
        Local<Object> exports;
        if (NsBuiltinModules::GetExports(context, moduleName).ToLocal(&exports)) {
            args.GetReturnValue().Set(exports);
        } else if (!NsBuiltinModules::IsRegistered(moduleName)) {
            isolate->ThrowException(Exception::Error(ArgConverter::ConvertToV8String(
                    isolate, NsBuiltinModules::NotFoundMessage(moduleName))));
        }
        return;
    }

    // Node-API addons claim a bare name only, so a path can never be diverted
    // into the addon registry, and builtins keep priority over both.
    if (IsBareSpecifier(moduleName) && NapiModules::IsRegistered(moduleName)) {
        auto context = isolate->GetCurrentContext();
        Local<Object> exports;
        if (NapiModules::GetExports(context, moduleName).ToLocal(&exports)) {
            args.GetReturnValue().Set(exports);
        } else if (!isolate->HasPendingException()) {
            isolate->ThrowException(Exception::Error(ArgConverter::ConvertToV8String(
                    isolate, "Node-API module '" + moduleName + "' failed to initialize")));
        }
        return;
    }

    tns::instrumentation::Frame frame("RequireCallback " + moduleName);
    string callingModuleDirName = ArgConverter::ConvertToString(args[1].As<String>());
    auto isData = false;

    auto moduleObj = LoadImpl(isolate, moduleName, callingModuleDirName, isData);

    if (isData) {
        assert(!moduleObj.IsEmpty());

        args.GetReturnValue().Set(moduleObj);
    } else {
        auto context = isolate->GetCurrentContext();
        Local<Value> exportsVal;
        moduleObj->Get(context, ArgConverter::ConvertToV8String(isolate, "exports")).ToLocal(&exportsVal);

        assert(!exportsVal.IsEmpty());

        auto exportsObj = exportsVal.As<Object>();
        args.GetReturnValue().Set(exportsObj);
    }
}

void ModuleInternal::RequireNativeCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    TNSPERF();
    auto ext = args.Data().As<External>();
    auto funcPtr = reinterpret_cast<FunctionCallback>(ext->Value(v8::kExternalPointerTypeTagDefault));
    funcPtr(args);
}

void ModuleInternal::Load(Local<Context> context, const string& path) {
    TNSPERF();
    auto isolate = m_isolate;
    if (IsHttpModulePath(path) || IsESModule(path)) {
        LoadESModule(isolate, path);
        return;
    }
    auto globalObject = context->Global();
    auto require = globalObject->Get(context, ArgConverter::ConvertToV8String(isolate, "require")).ToLocalChecked().As<Function>();
    Local<Value> args[] = { ArgConverter::ConvertToV8String(isolate, path) };
    require->Call(context, globalObject, 1, args);
}

void ModuleInternal::LoadWorker(Local<Context> context, const string& path) {
    TNSPERF();
    auto isolate = m_isolate;
    TryCatch tc(isolate);

    try {
        Load(context, path);
    } catch (NativeScriptException& e) {
        e.ReThrowToV8();
    }

    if (tc.HasCaught()) {
        // This will handle any errors that occur when first loading a script (new worker)
        // exceptions include: syntax errors, throw statements, access of properties of undefined objects
        // Check if `onerror` handle is implemented
        // Web behaviour - if onerror handle comes before exception throw - execute it, else - bubble up to main's worker object
        CallbackHandlers::CallWorkerScopeOnErrorHandle(isolate, tc);
    }
}

void ModuleInternal::CheckFileExists(Isolate* isolate, const std::string& path, const std::string& baseDir) {
    JEnv env;
    JniLocalRef jsModulename(env.NewStringUTF(path.c_str()));
    JniLocalRef jsBaseDir(env.NewStringUTF(baseDir.c_str()));

    env.CallStaticObjectMethod(MODULE_CLASS, RESOLVE_PATH_METHOD_ID, (jstring) jsModulename, (jstring) jsBaseDir);
}

Local<Object> ModuleInternal::LoadImpl(Isolate* isolate, const string& moduleName, const string& baseDir, bool& isData) {
    auto pathKind = GetModulePathKind(moduleName);
    auto cachePathKey = (pathKind == ModulePathKind::Global) ? moduleName : (baseDir + "*" + moduleName);

    Local<Object> result;

    DEBUG_WRITE(">>LoadImpl cachePathKey=%s", cachePathKey.c_str());

    auto it = m_loadedModules.find(cachePathKey);

    if (it == m_loadedModules.end()) {
        std::string path;

        // Search App System libs
        std::string sys_lib("system_lib://");
        if (moduleName.rfind(sys_lib, 0) == 0) {
            auto pos = moduleName.find(sys_lib);
            path = std::string(moduleName);
            path.replace(pos, sys_lib.length(), "");
        } else {
            // Handle tilde path resolution before calling Java path resolution
            std::string resolvedModuleName = moduleName;
            if (!moduleName.empty() && moduleName[0] == '~') {
                // Convert ~/path to ApplicationPath/path
                std::string tail = moduleName.size() >= 2 && moduleName[1] == '/' ? moduleName.substr(2) : moduleName.substr(1);
                resolvedModuleName = Constants::APP_ROOT_FOLDER_PATH + "/" + tail;
                
                // For .mjs files with tilde paths, use resolved path directly
                if (Util::EndsWith(resolvedModuleName, ".mjs")) {
                    path = resolvedModuleName;
                } else {
                    // For non-.mjs files, still use Java resolution with the resolved name
                    JEnv env;
                    JniLocalRef jsModulename(env.NewStringUTF(resolvedModuleName.c_str()));
                    JniLocalRef jsBaseDir(env.NewStringUTF(baseDir.c_str()));
                    JniLocalRef jsModulePath(
                            env.CallStaticObjectMethod(MODULE_CLASS, RESOLVE_PATH_METHOD_ID,
                                                       (jstring) jsModulename, (jstring) jsBaseDir));

                    path = ArgConverter::jstringToString((jstring) jsModulePath);
                }
            } else {
                JEnv env;
                JniLocalRef jsModulename(env.NewStringUTF(moduleName.c_str()));
                JniLocalRef jsBaseDir(env.NewStringUTF(baseDir.c_str()));
                JniLocalRef jsModulePath(
                        env.CallStaticObjectMethod(MODULE_CLASS, RESOLVE_PATH_METHOD_ID,
                                                   (jstring) jsModulename, (jstring) jsBaseDir));

                path = ArgConverter::jstringToString((jstring) jsModulePath);
            }
        }

        auto it2 = m_loadedModules.find(path);

        if (it2 == m_loadedModules.end()) {
            if (Util::EndsWith(path, ".js") || Util::EndsWith(path, ".mjs") || Util::EndsWith(path, ".so")) {
                isData = false;
                result = LoadModule(isolate, path, cachePathKey);
            } else if (Util::EndsWith(path, ".json")) {
                isData = true;
                result = LoadData(isolate, path);
            } else {
                string errMsg = "Unsupported file extension: " + path;
                throw NativeScriptException(errMsg);
            }
        } else {
            auto& cacheEntry = it2->second;
            isData = cacheEntry.isData;
            result = Local<Object>::New(isolate, *cacheEntry.obj);
        }
    } else {
        auto& cacheEntry = it->second;
        isData = cacheEntry.isData;
        result = Local<Object>::New(isolate, *cacheEntry.obj);
    }

    return result;
}

Local<Object> ModuleInternal::LoadModule(Isolate* isolate, const string& modulePath, const string& moduleCacheKey) {
    string frameName("LoadModule " + modulePath);
    tns::instrumentation::Frame frame(frameName);
    CrashBreadcrumbs::ModuleScope moduleBreadcrumb(modulePath.c_str());
    Local<Object> result;

    auto context = isolate->GetCurrentContext();
    auto moduleObj = Object::New(isolate);
    auto exportsObj = Object::New(isolate);
    auto exportsPropName = ArgConverter::ConvertToV8String(isolate, "exports");
    moduleObj->Set(context, exportsPropName, exportsObj);
    auto fullRequiredModulePath = ArgConverter::ConvertToV8String(isolate, modulePath);
    moduleObj->Set(context, ArgConverter::ConvertToV8String(isolate, "filename"), fullRequiredModulePath);

    auto poModuleObj = new Persistent<Object>(isolate, moduleObj);
    TempModule tempModule(this, modulePath, moduleCacheKey, poModuleObj);

    TryCatch tc(isolate);

    // Check if this is an ES module (.mjs)
    if (Util::EndsWith(modulePath, ".mjs")) {
        // For ES modules, load using the ES module system
        Local<Value> moduleNamespace = LoadESModule(isolate, modulePath);
        
        // Create a wrapper object that behaves like a CommonJS module
        // but exports the ES module namespace
        moduleObj->Set(context, ArgConverter::ConvertToV8String(isolate, "exports"), moduleNamespace);
        
        tempModule.SaveToCache();
        result = moduleObj;
        return result;
    }

    Local<Function> moduleFunc;

    if (Util::EndsWith(modulePath, ".js")) {
        auto script = LoadScript(isolate, modulePath, fullRequiredModulePath);

        Local<Value> moduleFuncValue;
        if (!script->Run(context).ToLocal(&moduleFuncValue) || tc.HasCaught()) {
            throw NativeScriptException(tc, "Error running script " + modulePath);
        }
        moduleFunc = moduleFuncValue.As<Function>();
    } else if (Util::EndsWith(modulePath, ".so")) {
        // Registrations from libraries this loader did not dlopen (statically
        // linked addons whose constructors ran at app start) must not be
        // attributed to whatever `.so` happens to load next.
        NapiModules::ClaimPendingModule();

        auto handle = dlopen(modulePath.c_str(), RTLD_LAZY);
        if (handle == nullptr) {
            auto error = dlerror();
            string errMsg(error != nullptr ? error : "dlopen failed for " + modulePath);
            throw NativeScriptException(errMsg);
        }

        // A library whose constructors registered a Node-API module is a
        // Node-API addon: its exports come from the addon registry (Node's
        // dlopen consumes `modpending` the same way). A library carrying the
        // NAPI_MODULE / node-addon-api registration symbol instead is
        // initialized through it — this is also the path a *re*-dlopen of an
        // already-loaded addon takes, since constructors only run on first
        // load. NSMain remains the protocol for plain native modules.
        string napiModuleName = NapiModules::ClaimPendingModule();
        void* napiInitSymbol = napiModuleName.empty()
                ? dlsym(handle, "napi_register_module_v1")
                : nullptr;
        if (!napiModuleName.empty() || napiInitSymbol != nullptr) {
            Local<Object> napiExports;
            bool initialized = napiModuleName.empty()
                    ? NapiModules::InitAddonFromSymbol(context, napiInitSymbol, modulePath).ToLocal(&napiExports)
                    : NapiModules::GetExports(context, napiModuleName).ToLocal(&napiExports);
            if (!initialized || tc.HasCaught()) {
                throw NativeScriptException(tc, "Error initializing Node-API module " +
                        (napiModuleName.empty() ? modulePath : napiModuleName));
            }
            moduleObj->Set(context, exportsPropName, napiExports);
            tempModule.SaveToCache();
            result = moduleObj;
            return result;
        }

        auto func = dlsym(handle, "NSMain");
        if (func == nullptr) {
            string errMsg("Cannot find 'NSMain' in " + modulePath);
            throw NativeScriptException(errMsg);
        }
        auto extFunc = External::New(isolate, func, v8::kExternalPointerTypeTagDefault);
        auto ft = FunctionTemplate::New(isolate, RequireNativeCallback, extFunc);
        auto maybeFunc = ft->GetFunction(context);
        if (maybeFunc.IsEmpty() || tc.HasCaught()) {
            throw NativeScriptException(tc, "Cannot create native module function callback");
        }
        moduleFunc = maybeFunc.ToLocalChecked();
    } else {
        string errMsg = "Unsupported file extension: " + modulePath;
        throw NativeScriptException(errMsg);
    }

    SET_PROFILER_FRAME();

    auto fileName = ArgConverter::ConvertToV8String(isolate, modulePath);
    char pathcopy[1024];
    strcpy(pathcopy, modulePath.c_str());
    string strDirName(dirname(pathcopy));
    auto dirName = ArgConverter::ConvertToV8String(isolate, strDirName);
    auto require = GetRequireFunction(isolate, strDirName);
    Local<Value> requireArgs[5] {
        moduleObj, exportsObj, require, fileName, dirName
    };

    moduleObj->Set(context, ArgConverter::ConvertToV8String(isolate, "require"), require);

    auto moduleIdProp = ArgConverter::ConvertToV8String(isolate, "id");
    const auto readOnlyFlags = static_cast<PropertyAttribute>(PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);
    Maybe<bool> success = moduleObj->DefineOwnProperty(isolate->GetCurrentContext(), moduleIdProp, fileName, readOnlyFlags);
    if (success.IsNothing()) {
        throw NativeScriptException(string("Couldn't execute method 'DefineOwnProperty' on 'moduleObj' in 'Module::LoadModule'."));
    }

    auto thiz = Object::New(isolate);
    auto extendsName = ArgConverter::ConvertToV8String(isolate, "__extends");
    Local<Value> extendsFunc;
    if (!context->Global()->Get(context, extendsName).ToLocal(&extendsFunc) || tc.HasCaught()) {
        throw NativeScriptException(tc, "Cannot read '__extends' while loading " + modulePath);
    }
    thiz->Set(context, extendsName, extendsFunc);
    moduleFunc->Call(context, thiz, sizeof(requireArgs) / sizeof(Local<Value> ), requireArgs);

    if (tc.HasCaught()) {
        throw NativeScriptException(tc, "Error calling module function ");
    }

    tempModule.SaveToCache();
    result = moduleObj;

    return result;
}

Local<Script> ModuleInternal::LoadScript(Isolate* isolate, const string& path, const Local<String>& fullRequiredModulePath) {
    string frameName("LoadScript " + path);
    tns::instrumentation::Frame frame(frameName);
    Local<Script> script;

    TryCatch tc(isolate);

    auto scriptText = ModuleInternal::WrapModuleContent(path);

    DEBUG_WRITE("Compiling script (module %s)", path.c_str());
    //
    auto cacheData = TryLoadScriptCache(path);

    auto fullRequiredModulePathWithSchema = ArgConverter::ConvertToV8String(isolate, "file://" + path);
    ScriptOrigin origin(fullRequiredModulePathWithSchema);
    ScriptCompiler::Source source(scriptText, origin, cacheData);
    ScriptCompiler::CompileOptions option = ScriptCompiler::kNoCompileOptions;

    if (cacheData != nullptr) {
        tns::instrumentation::Frame frame("Compile, cached");
        option = ScriptCompiler::kConsumeCodeCache;
        auto maybeScript = ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, option);
        if (maybeScript.IsEmpty() || tc.HasCaught()) {
            throw NativeScriptException(tc, "Cannot compile " + path);
        }
        script = maybeScript.ToLocalChecked();
    } else {
        tns::instrumentation::Frame frame("Compile, no cache");
        auto maybeScript = ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, option);
        if (maybeScript.IsEmpty() || tc.HasCaught()) {
            throw NativeScriptException(tc, "Cannot compile " + path);
        }
        script = maybeScript.ToLocalChecked();
        SaveScriptCache(script, path);
    }

    DEBUG_WRITE("Compiled script (module %s)", path.c_str());

    return script;
}

Local<Object> ModuleInternal::LoadData(Isolate* isolate, const string& path) {
    string frameName("LoadData " + path);
    tns::instrumentation::Frame frame(frameName);
    Local<Object> json;

    auto jsonData = Runtime::GetRuntime(m_isolate)->ReadFileText(path);

    TryCatch tc(isolate);

    auto jsonStr = ArgConverter::ConvertToV8String(isolate, jsonData);

    auto context = isolate->GetCurrentContext();
    auto maybeValue = JSON::Parse(context, jsonStr);

    if (maybeValue.IsEmpty() || tc.HasCaught()) {
        string errMsg = "Cannot parse JSON file " + path;
        throw NativeScriptException(tc, errMsg);
    }

    auto value = maybeValue.ToLocalChecked();

    if (!value->IsObject()) {
        string errMsg = "JSON is not valid, file=" + path;
        throw NativeScriptException(errMsg);
    }

    json = value.As<Object>();

    auto poObj = new Persistent<Object>(isolate, json);

    m_loadedModules.emplace(path, ModuleCacheEntry(poObj, true /* isData */));

    return json;
}

Local<Value> ModuleInternal::LoadESModule(Isolate* isolate, const std::string& path) {
    auto context = isolate->GetCurrentContext();
    const bool isHttpModule = IsHttpModulePath(path);
    const std::string requestPath = isHttpModule ? NormalizeHttpModuleUrl(path) : path;

    Local<Module> module;
    ScriptCompiler::CachedData* cacheData = nullptr;

    if (isHttpModule) {
        RunAsyncHttpModuleGraphLoadPumped(isolate, context, requestPath, 60.0);
        MaybeLocal<Module> maybeMod = LoadHttpModuleForUrl(isolate, context, requestPath);
        if (!maybeMod.ToLocal(&module)) {
            std::string reason = TakeLastHttpFetchErrorReason();
            std::string message = "Cannot load ES module " + requestPath;
            if (!reason.empty()) {
                message.append(" — ");
                message.append(reason);
            }
            throw NativeScriptException(message);
        }
        if (module->GetStatus() == Module::kEvaluated) {
            UpdateModuleFallback(isolate, CanonicalizeHttpUrlKey(requestPath), module);
            return module->GetModuleNamespace();
        }
    } else {
        // 1) Prepare URL & source
        string url = "file://" + path;
        string content = Runtime::GetRuntime(isolate)->ReadFileText(path);

        Local<String> sourceText = ArgConverter::ConvertToV8String(isolate, content);

        Local<String> urlString;
        if (!String::NewFromUtf8(isolate, url.c_str(), NewStringType::kNormal).ToLocal(&urlString)) {
            throw NativeScriptException(string("Failed to create URL string for ES module ") + path);
        }

        ScriptOrigin origin(urlString, 0, 0, false, -1, Local<Value>(), false, false,
                            true  // ← is_module
        );
        ScriptCompiler::Source source(sourceText, origin, cacheData);

        // 2) Compile with its own TryCatch
        {
            TryCatch tcCompile(isolate);
            MaybeLocal<Module> maybeMod = ScriptCompiler::CompileModule(
                isolate, &source,
                cacheData ? ScriptCompiler::kConsumeCodeCache : ScriptCompiler::kNoCompileOptions);

            if (!maybeMod.ToLocal(&module)) {
                if (tcCompile.HasCaught()) {
                    throw NativeScriptException(tcCompile, "Cannot compile ES module " + path);
                } else {
                    throw NativeScriptException(string("Cannot compile ES module ") + path);
                }
            }
        }

        // 3) Register for resolution callback
        auto& g_moduleRegistry = ModuleRegistryFor(isolate);
        auto it = g_moduleRegistry.find(path);
        if (it != g_moduleRegistry.end()) {
            it->second.Reset();
        }
        g_moduleRegistry[path].Reset(isolate, module);
    }

    // 4) Instantiate (link) with ResolveModuleCallback
    if (module->GetStatus() < Module::kInstantiated) {
        TryCatch tcLink(isolate);
        bool linked = module->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false);

        if (!linked) {
            if (tcLink.HasCaught()) {
                throw NativeScriptException(tcLink, "Cannot instantiate module " + path);
            } else {
                throw NativeScriptException(string("Cannot instantiate module ") + path);
            }
        }
    }

    // 5) Evaluate with its own TryCatch
    Local<Value> result;
    {
        TryCatch tcEval(isolate);
        if (!module->Evaluate(context).ToLocal(&result)) {
            if (tcEval.HasCaught()) {
                throw NativeScriptException(tcEval, "Cannot evaluate module " + path);
            } else {
                throw NativeScriptException(string("Cannot evaluate module ") + path);
            }
        }

        // Handle the case where evaluation returns a Promise (for top-level await)
        if (result->IsPromise()) {
            Local<Promise> promise = result.As<Promise>();
            const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(30);

            while (true) {
                isolate->PerformMicrotaskCheckpoint();
                Promise::PromiseState state = promise->State();

                if (state != Promise::kPending) {
                    if (state == Promise::kRejected) {
                        Local<Value> reason = promise->Result();
                        isolate->ThrowException(reason);
                        throw NativeScriptException(PromiseRejectionMessage(isolate, promise, path));
                    }
                    break;
                }

                if (std::chrono::steady_clock::now() >= deadline) {
                    throw NativeScriptException(string("Module evaluation promise timed out: ") + path);
                }

                ALooper_pollOnce(10, nullptr, nullptr, nullptr);
                usleep(100);
            }
        }
    }

    // 6) Return the namespace
    return module->GetModuleNamespace();
}

Local<String> ModuleInternal::WrapModuleContent(const string& path) {
    TNSPERF();

    string content = Runtime::GetRuntime(m_isolate)->ReadFileText(path);

    // TODO: Use statically allocated buffer for better performance
    string result(MODULE_PROLOGUE);
    result.reserve(content.length() + 1024);
    result += content;
    result += MODULE_EPILOGUE;

    return ArgConverter::ConvertToV8String(m_isolate, result);
}

ScriptCompiler::CachedData* ModuleInternal::TryLoadScriptCache(const std::string& path) {
    TNSPERF();
    if (!Constants::V8_CACHE_COMPILED_CODE) {
        return nullptr;
    }

    auto cachePath = path + ".cache";

    struct stat result;
    if (stat(cachePath.c_str(), &result) == 0) {
        auto cacheLastModifiedTime = result.st_mtime;
        if (stat(path.c_str(), &result) == 0) {
            auto jsLastModifiedTime = result.st_mtime;
            if (jsLastModifiedTime != cacheLastModifiedTime) {
                // files have different dates, ignore the cache file (this is enforced by the
                // SaveScriptCache function)
                return nullptr;
            }
        }
    }

    int length = 0;
    auto data = File::ReadBinary(cachePath, length);
    if (!data) {
        return nullptr;
    }

    return new ScriptCompiler::CachedData(reinterpret_cast<uint8_t*>(data), length, ScriptCompiler::CachedData::BufferOwned);
}

void ModuleInternal::SaveScriptCache(const Local<Script> script, const std::string& path) {
    if (!Constants::V8_CACHE_COMPILED_CODE) {
        return;
    }

    tns::instrumentation::Frame frame("SaveScriptCache");

    Local<UnboundScript> unboundScript = script->GetUnboundScript();
    ScriptCompiler::CachedData* cachedData = ScriptCompiler::CreateCodeCache(unboundScript);

    int length = cachedData->length;
    auto cachePath = path + ".cache";
    File::WriteBinary(cachePath, cachedData->data, length);
    delete cachedData;
    // make sure cache and js file have the same modification date
    struct stat result;
    struct utimbuf new_times;
    new_times.actime = time(nullptr);
    new_times.modtime = time(nullptr);
    if (stat(path.c_str(), &result) == 0) {
        auto jsLastModifiedTime = result.st_mtime;
        new_times.modtime = jsLastModifiedTime;
    }
    utime(cachePath.c_str(), &new_times);
}

ModuleInternal::ModulePathKind ModuleInternal::GetModulePathKind(const std::string& path) {
    ModulePathKind kind;
    switch (path[0]) {
    case '.':
        kind = ModulePathKind::Relative;
        break;
    case '/':
        kind = ModulePathKind::Absolute;
        break;
    default:
        kind = ModulePathKind::Global;
        break;
    }
    return kind;
}

jclass ModuleInternal::MODULE_CLASS = nullptr;
jmethodID ModuleInternal::RESOLVE_PATH_METHOD_ID = nullptr;
jmethodID ModuleInternal::GET_APPLICATION_FILES_PATH_METHOD_ID = nullptr;

const char* ModuleInternal::MODULE_PROLOGUE = "(function(module, exports, require, __filename, __dirname){ ";
const char* ModuleInternal::MODULE_EPILOGUE = "\n})";
int ModuleInternal::MODULE_PROLOGUE_LENGTH = std::string(ModuleInternal::MODULE_PROLOGUE).length();
