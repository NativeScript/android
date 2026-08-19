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
#include "EventLoop.h"
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

static ModuleEvaluationOptions BootEntryEvaluationOptions(bool isHttpModule);
static ModuleEvaluationOptions RequireEvaluationOptions(ModuleEvaluationPolicy policy);

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

    /*
     * Runs after Isolate::Dispose, so the handles are freed with the isolate and
     * only the wrappers are deleted -- v8::Persistent does not reset in its
     * destructor, which is what makes that safe here.
     *
     * Deduplicated by pointer: a module is cached under both its resolved path
     * and its cache key, and both entries hold the same Persistent.
     */
    robin_hood::unordered_set<v8::Persistent<v8::Object>*> freed;
    for (const auto& pair: this->m_loadedModules) {
        if (freed.insert(pair.second.obj).second) {
            delete pair.second.obj;
        }
    }
    this->m_loadedModules.clear();
}

void ModuleInternal::Init(Isolate* isolate, const string& baseDir) {
    JEnv env;

    if (MODULE_CLASS == nullptr) {
        MODULE_CLASS = env.FindClass("com/tns/Module");
        NS_CHECK(MODULE_CLASS != nullptr);

        RESOLVE_PATH_METHOD_ID = env.GetStaticMethodID(MODULE_CLASS, "resolvePath", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        NS_CHECK(RESOLVE_PATH_METHOD_ID != nullptr);
        
        GET_APPLICATION_FILES_PATH_METHOD_ID = env.GetStaticMethodID(MODULE_CLASS, "getApplicationFilesPath", "()Ljava/lang/String;");
        NS_CHECK(GET_APPLICATION_FILES_PATH_METHOD_ID != nullptr);
    }

    m_isolate = isolate;

    auto context = isolate->GetCurrentContext();

    auto global = context->Global();

    Local<Value> result;
    auto success = BuiltinLoader::RunBuiltin(context, BuiltinId::kRequireFactory).ToLocal(&result);

    NS_CHECK(success && result->IsFunction());

    auto requireFactoryFunction = result.As<Function>();

    m_requireFactoryFunction = new Persistent<Function>(isolate, requireFactoryFunction);

    auto requireFuncTemplate = FunctionTemplate::New(isolate, RequireCallback, External::New(isolate, this, v8::kExternalPointerTypeTagDefault));
    auto requireFunc = requireFuncTemplate->GetFunction(context).ToLocalChecked();
    global->Set(context, ArgConverter::ConvertToV8String(isolate, "__nativeRequire"), requireFunc);
    m_requireFunction = new Persistent<Function>(isolate, requireFunc);

    Local<Function> globalRequire;

    if (!baseDir.empty()) {
        globalRequire = GetRequireFunction(isolate, baseDir, RequireEvaluationOptions(
                ModuleEvaluationPolicy::kSyncStrict));
    } else {
        globalRequire = GetRequireFunction(isolate, Constants::APP_ROOT_FOLDER_PATH,
                                           RequireEvaluationOptions(
                                                   ModuleEvaluationPolicy::kSyncStrict));
    }
    global->Set(context, ArgConverter::ConvertToV8String(isolate, "require"), globalRequire);
}

// How an entry module's graph settles. For local modules the bound is a yield,
// not a timeout: only nestable V8 tasks can run while these JS frames are on
// the stack, so a TLA parked on a non-nestable foreground task can never settle
// in-pump — give it one short window, then return and let the real event loop
// finish it after the turn. HTTP entries must settle in-pump — the dev client
// needs the rejection reason synchronously — so they get the full deadline and
// the looper slices their transport needs.
static ModuleEvaluationOptions BootEntryEvaluationOptions(bool isHttpModule) {
    ModuleEvaluationOptions options;
    options.policy = ModuleEvaluationPolicy::kSyncPumping;
    options.deadlineSeconds = isHttpModule ? kModuleEvaluateDeadlineSeconds : 1.0;
    options.timeoutBehavior = isHttpModule
                                      ? ModuleEvaluationOptions::TimeoutBehavior::kThrow
                                      : ModuleEvaluationOptions::TimeoutBehavior::kReturnPending;
    options.pumpRunLoop = isHttpModule;
    return options;
}

// How a graph reached through require() settles. A pumping require must settle
// or throw — handing back a half-initialized namespace is what the strict
// policy exists to prevent — so it gets the full deadline. It never slices the
// looper by default: outside boot the loop belongs to the app, and re-entering
// arbitrary looper sources from the middle of a require would run UI callbacks
// underneath JS frames.
static ModuleEvaluationOptions RequireEvaluationOptions(ModuleEvaluationPolicy policy) {
    ModuleEvaluationOptions options;
    options.policy = policy;
    if (policy == ModuleEvaluationPolicy::kSyncPumping) {
        options.deadlineSeconds = kModuleEvaluateDeadlineSeconds;
        options.timeoutBehavior = ModuleEvaluationOptions::TimeoutBehavior::kThrow;
        options.pumpRunLoop = false;
    }
    return options;
}

// The require cache is keyed by directory AND by the options the require was
// minted with: a pumping require for a directory must never be served from a
// strict require cached for the same directory, in either direction.
static std::string RequireCacheKey(const std::string& dirName,
                                   const ModuleEvaluationOptions& options) {
    std::string key = dirName;
    key += '\x1f';
    key += std::to_string(static_cast<int>(options.policy));
    key += '\x1f';
    key += std::to_string(options.deadlineSeconds);
    key += '\x1f';
    key += (options.timeoutBehavior == ModuleEvaluationOptions::TimeoutBehavior::kThrow) ? '1' : '0';
    key += options.pumpRunLoop ? '1' : '0';
    return key;
}

Local<Function> ModuleInternal::GetRequireFunction(Isolate* isolate, const string& dirName,
                                                   const ModuleEvaluationOptions& options) {
    TNSPERF();
    Local<Function> requireFunc;

    const std::string cacheKey = RequireCacheKey(dirName, options);
    auto itFound = m_requireCache.find(cacheKey);

    if (itFound != m_requireCache.end()) {
        requireFunc = Local<Function>::New(isolate, *itFound->second);
    } else {
        auto requireFuncFactory = Local<Function>::New(isolate, *m_requireFactoryFunction);

        auto context = isolate->GetCurrentContext();

        auto requireInternalFunc = Local<Function>::New(isolate, *m_requireFunction);

        Local<Value> args[6] {
            requireInternalFunc,
            ArgConverter::ConvertToV8String(isolate, dirName),
            Integer::New(isolate, static_cast<int>(options.policy)),
            Number::New(isolate, options.deadlineSeconds),
            v8::Boolean::New(isolate, options.timeoutBehavior ==
                                              ModuleEvaluationOptions::TimeoutBehavior::kThrow),
            v8::Boolean::New(isolate, options.pumpRunLoop)
        };
        Local<Value> result;
        auto thiz = Object::New(isolate);
        auto success = requireFuncFactory->Call(context, thiz, 6, args).ToLocal(&result);

        NS_CHECK(success && !result.IsEmpty() && result->IsFunction());

        requireFunc = result.As<Function>();

        auto poFunc = new Persistent<Function>(isolate, requireFunc);

        m_requireCache.emplace(cacheKey, poFunc);
    }

    return requireFunc;
}

void ModuleInternal::CreateRequireCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1 || !args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(ArgConverter::ConvertToV8String(
                isolate, "createRequire expects a base directory string")));
        return;
    }

    Runtime* runtime = Runtime::GetRuntime(isolate);
    ModuleInternal* moduleInternal = runtime != nullptr ? runtime->GetModuleInternal() : nullptr;
    if (moduleInternal == nullptr) {
        isolate->ThrowException(Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "createRequire is unavailable: this isolate has no module loader")));
        return;
    }

    string dirName = ArgConverter::ConvertToString(args[0].As<String>());
    const bool pumping = args.Length() > 1 && args[1]->BooleanValue(isolate);
    ModuleEvaluationOptions options = RequireEvaluationOptions(
            pumping ? ModuleEvaluationPolicy::kSyncPumping : ModuleEvaluationPolicy::kSyncStrict);

    // ns-module.js has already validated these and passes undefined for anything
    // the caller left out, so each present value simply overrides its default.
    if (args.Length() > 2 && args[2]->IsNumber()) {
        options.deadlineSeconds = args[2].As<Number>()->Value();
    }
    if (args.Length() > 3 && args[3]->IsBoolean()) {
        options.timeoutBehavior = args[3]->BooleanValue(isolate)
                                          ? ModuleEvaluationOptions::TimeoutBehavior::kThrow
                                          : ModuleEvaluationOptions::TimeoutBehavior::kReturnPending;
    }
    if (args.Length() > 4 && args[4]->IsBoolean()) {
        options.pumpRunLoop = args[4]->BooleanValue(isolate);
    }

    args.GetReturnValue().Set(moduleInternal->GetRequireFunction(isolate, dirName, options));
}

bool ModuleInternal::InstallCreateRequireBinding(Local<Context> context, Local<Object> binding) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    Local<Function> fn;
    if (!Function::New(context, ModuleInternal::CreateRequireCallback).ToLocal(&fn)) {
        return false;
    }
    fn->SetName(ArgConverter::ConvertToV8String(isolate, "createRequire"));
    return binding->CreateDataProperty(context,
                                       ArgConverter::ConvertToV8String(isolate, "createRequire"),
                                       fn)
            .FromMaybe(false);
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

    if (args.Length() < 2) {
        throw NativeScriptException(string("require should be called with at least two parameters"));
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

    // The require factory forwards the options its require was minted with; an
    // absent policy is the strict default every ordinary require uses.
    ModuleEvaluationPolicy policy = ModuleEvaluationPolicy::kSyncStrict;
    if (args.Length() > 2 && args[2]->IsInt32() &&
        args[2].As<Int32>()->Value() == static_cast<int>(ModuleEvaluationPolicy::kSyncPumping)) {
        policy = ModuleEvaluationPolicy::kSyncPumping;
    }
    ModuleEvaluationOptions evaluationOptions = RequireEvaluationOptions(policy);
    if (args.Length() > 3 && args[3]->IsNumber()) {
        evaluationOptions.deadlineSeconds = args[3].As<Number>()->Value();
    }
    if (args.Length() > 4 && args[4]->IsBoolean()) {
        evaluationOptions.timeoutBehavior =
                args[4]->BooleanValue(isolate)
                        ? ModuleEvaluationOptions::TimeoutBehavior::kThrow
                        : ModuleEvaluationOptions::TimeoutBehavior::kReturnPending;
    }
    if (args.Length() > 5 && args[5]->IsBoolean()) {
        evaluationOptions.pumpRunLoop = args[5]->BooleanValue(isolate);
    }

    auto moduleObj = LoadImpl(isolate, moduleName, callingModuleDirName, isData, evaluationOptions);

    if (isData) {
        NS_DCHECK(!moduleObj.IsEmpty());

        args.GetReturnValue().Set(moduleObj);
    } else {
        auto context = isolate->GetCurrentContext();
        Local<Value> exportsVal;
        moduleObj->Get(context, ArgConverter::ConvertToV8String(isolate, "exports")).ToLocal(&exportsVal);

        NS_DCHECK(!exportsVal.IsEmpty());

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
    // Entry evaluation is this thread's boot window: while it is active, the
    // yield inside synchronous HTTP fetches may pump the looper (nothing else
    // owns it yet). Balanced on every exit path, throws included.
    struct BootEvalScope {
        BootEvalScope() { SetBootEvaluationActive(true); }
        ~BootEvalScope() { SetBootEvaluationActive(false); }
    } bootEvalScope;

    // The ES module branch compiles and links against
    // isolate->GetCurrentContext(); a caller that enters the isolate through a
    // fresh Isolate::Scope has no current context, and CompileModule would
    // dereference a null native context. The require branch never needed this
    // because Function::Call enters the context it is handed.
    Context::Scope context_scope(context);
    if (IsHttpModulePath(path) || IsESModule(path)) {
        // The entry runs before this thread's event loop does, so its graph can
        // only make progress from the pump inside LoadESModule.
        LoadESModule(isolate, path, BootEntryEvaluationOptions(IsHttpModulePath(path)));
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

Local<Object> ModuleInternal::LoadImpl(Isolate* isolate, const string& moduleName,
                                       const string& baseDir, bool& isData,
                                       const ModuleEvaluationOptions& options) {
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
                result = LoadModule(isolate, path, cachePathKey, options);
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

static bool NamespaceHasOwn(Isolate* isolate, Local<Context> context, Local<Object> ns,
                            const char* name) {
    return ns->HasOwnProperty(context, ArgConverter::ConvertToV8String(isolate, name))
            .FromMaybe(false);
}

// The live compiled module behind a registry key, or empty.
static Local<Module> RegisteredModuleForPath(Isolate* isolate, const std::string& canonicalPath) {
    auto* registryPtr = ModuleRegistryFor(isolate);
    if (registryPtr == nullptr) {
        return Local<Module>();
    }
    auto it = registryPtr->find(canonicalPath);
    if (it == registryPtr->end()) {
        return Local<Module>();
    }
    return it->second.Get(isolate);
}

// What `require()` of an ES module hands back, per Node's
// populateCJSExportsFromESM: an explicit `module.exports` export wins outright;
// a namespace with no default export, or one that already declares
// __esModule, passes through untouched; everything else gets the facade so
// transpiled consumers reading `_mod.__esModule ? _mod.default : _mod` find the
// default. Export names are arbitrary strings, hence the own-property probes.
static Local<Value> RequireExportsForNamespace(Isolate* isolate, Local<Context> context,
                                               Local<Object> ns,
                                               const std::string& canonicalPath) {
    TryCatch tc(isolate);

    if (NamespaceHasOwn(isolate, context, ns, "module.exports")) {
        Local<Value> moduleExports;
        if (!ns->Get(context, ArgConverter::ConvertToV8String(isolate, "module.exports"))
                    .ToLocal(&moduleExports)) {
            throw NativeScriptException(
                    tc, "Cannot read the 'module.exports' export of " + canonicalPath);
        }
        return moduleExports;
    }

    bool hasDefault = NamespaceHasOwn(isolate, context, ns, "default");
    bool hasEsModuleMarker = NamespaceHasOwn(isolate, context, ns, "__esModule");
    if (!hasDefault || hasEsModuleMarker) {
        return ns;
    }

    Local<Module> target = RegisteredModuleForPath(isolate, canonicalPath);
    if (target.IsEmpty()) {
        // The load that produced this namespace registered the module under this
        // very key, so a miss means the registry and the namespace disagree —
        // returning the bare namespace would drop __esModule and misroute every
        // transpiled consumer downstream.
        throw NativeScriptException(
                "require() cannot build the exports facade for " + canonicalPath +
                ": the module evaluated but is absent from the registry under its canonical key");
    }

    Local<Module> facade;
    if (!GetOrCreateRequireFacade(isolate, context, target, canonicalPath).ToLocal(&facade)) {
        throw NativeScriptException("Cannot build the require() exports facade for " +
                                    canonicalPath);
    }
    return facade->GetModuleNamespace();
}

Local<Object> ModuleInternal::LoadModule(Isolate* isolate, const string& modulePath,
                                         const string& moduleCacheKey,
                                         const ModuleEvaluationOptions& options) {
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
        // require()'s route into the ES module system, which cannot wait: an
        // async graph is refused rather than pumped.
        Local<Value> moduleNamespace = LoadESModule(isolate, modulePath, options);

        // `module.exports` is what Node's populateCJSExportsFromESM produces for
        // this namespace, not the namespace itself. A namespace can still be
        // empty when the load bailed on a torn-down isolate; nothing to interop.
        Local<Value> esmExports = moduleNamespace;
        if (!moduleNamespace.IsEmpty() && moduleNamespace->IsObject()) {
            esmExports = RequireExportsForNamespace(isolate, context,
                                                    moduleNamespace.As<Object>(),
                                                    CanonicalizeRegistryKey(modulePath));
        }
        if (!esmExports.IsEmpty()) {
            moduleObj->Set(context, ArgConverter::ConvertToV8String(isolate, "exports"),
                           esmExports);
        }

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
    // A module's own require inherits the options it was loaded under, so a
    // pumping require's whole dependency tree keeps pumping.
    auto require = GetRequireFunction(isolate, strDirName, options);
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

MaybeLocal<Module> ModuleInternal::CompileFileEsModule(Isolate* isolate, const std::string& path) {
    // The resolver only ever hands over a path it already probed, but the ENTRY
    // routes (app main, worker main) reach here straight from the caller's
    // specifier — so the existence check has to live here, or a missing entry
    // reads a null FILE* instead of failing with a name.
    struct stat st;
    if (stat(path.c_str(), &st) != 0 || !S_ISREG(st.st_mode)) {
        throw NativeScriptException("Cannot find module " + path);
    }

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
    ScriptCompiler::Source source(sourceText, origin);

    return ScriptCompiler::CompileModule(isolate, &source);
}

namespace {

// `require()` cannot wait, so an async graph is refused rather than evaluated.
// Never evicts: the module is perfectly loadable through import().
[[noreturn]] void ThrowAsyncGraphRefusal(const std::string& canonicalPath) {
    throw NativeScriptException("require() cannot load ES module '" + canonicalPath +
                                "': the module graph contains top-level await. Use import() or "
                                "createPumpingRequire from ns:module instead.");
}

// The pump advances the loop with nestable tasks and microtask checkpoints, and
// V8 ignores a checkpoint while the isolate is already draining the microtask
// queue — so a graph whose top-level await resumes through a promise reaction
// could never settle from here. Refused up front, before evaluation, so the
// graph stays instantiated and import() can still load it.
[[noreturn]] void ThrowMicrotaskPumpRefusal(const std::string& canonicalPath) {
    throw NativeScriptException(
            "createPumpingRequire cannot settle module graph '" + canonicalPath +
            "' from inside a microtask (after an await or inside a promise callback): the event "
            "loop cannot be pumped re-entrantly. Call it from a task context, or use import().");
}

// Evicts the module and surfaces the rejection reason. Always throws, in every
// build — the reason has to reach the boundary handler that reports it.
[[noreturn]] void ThrowModuleEvaluationRejection(Isolate* isolate, Local<Promise> promise,
                                                 TryCatch& tc,
                                                 const std::string& canonicalPath) {
    RemoveModuleFromRegistry(canonicalPath);
    std::string detail = PromiseRejectionMessage(isolate, promise, canonicalPath);
    if (!tc.HasCaught()) {
        Local<Value> reason = promise->Result();
        if (!reason.IsEmpty()) {
            isolate->ThrowException(reason);
        }
    }
    if (tc.HasCaught()) {
        throw NativeScriptException(tc, detail);
    }
    throw NativeScriptException(detail);
}

}  // namespace

MaybeLocal<Promise> tns::EvaluateModuleGraph(Isolate* isolate, Local<Context> context,
                                             Local<Module> module,
                                             const std::string& canonicalPath,
                                             const ModuleEvaluationOptions& options) {
    if (options.policy == ModuleEvaluationPolicy::kSyncStrict) {
        if (module->IsGraphAsync()) {
            // Refusing before evaluation leaves the graph at kInstantiated, so a
            // later import() can still evaluate it, and keeps this diagnosis ahead
            // of whatever runtime error the graph would have produced first.
            ThrowAsyncGraphRefusal(canonicalPath);
        }
        if (module->GetStatus() == Module::kEvaluating) {
            // Re-entered through a cycle while the graph is still on the stack; its
            // namespace holds whatever has been initialized so far.
            return MaybeLocal<Promise>();
        }
    }

    if (options.policy == ModuleEvaluationPolicy::kSyncPumping && module->IsGraphAsync() &&
        v8::MicrotasksScope::IsRunningMicrotasks(isolate)) {
        // Only an async graph needs the pump; a synchronous one settles on its own
        // and stays legal from anywhere. Entry modules also arrive here, but from
        // native at task level, so they never trip this.
        ThrowMicrotaskPumpRefusal(canonicalPath);
    }

    TryCatch tcEval(isolate);
    Local<Value> result;
    if (!module->Evaluate(context).ToLocal(&result)) {
        RemoveModuleFromRegistry(canonicalPath);
        if (tcEval.HasCaught()) {
            throw NativeScriptException(tcEval, "Cannot evaluate module " + canonicalPath);
        }
        throw NativeScriptException(string("Cannot evaluate module ") + canonicalPath);
    }

    if (!result->IsPromise()) {
        return MaybeLocal<Promise>();
    }
    Local<Promise> promise = result.As<Promise>();

    if (options.policy == ModuleEvaluationPolicy::kAsync) {
        return promise;
    }

    TryCatch promiseTc(isolate);

    if (options.policy == ModuleEvaluationPolicy::kSyncStrict) {
        Promise::PromiseState state = promise->State();
        if (state == Promise::kRejected) {
            ThrowModuleEvaluationRejection(isolate, promise, promiseTc, canonicalPath);
        }
        if (state == Promise::kPending) {
            // V8 guarantees a settled capability for a graph that reported
            // !IsGraphAsync, so reaching here means the graph classification and the
            // evaluation disagree — never paper over it with a half-initialized
            // namespace.
            throw NativeScriptException("ES module " + canonicalPath +
                                        " left its evaluation promise pending on a graph reported "
                                        "as synchronous");
        }
        return MaybeLocal<Promise>();
    }

    // Top-level await can depend on native async work such as fetch(), which
    // needs both V8 microtasks and the looper to advance. An await whose
    // resolution arrives as a v8 foreground task never settles from checkpoints
    // alone; JS frames are on the stack, so like the inspector pause loops only
    // nestable tasks may run here.
    Runtime* runtime = Runtime::GetRuntime(isolate);
    std::shared_ptr<EventLoop> eventLoop = runtime != nullptr ? runtime->GetEventLoop() : nullptr;

    auto pumpAsyncProgress = [&]() {
        if (eventLoop != nullptr) {
            eventLoop->RunNestableV8Tasks();
        }
        isolate->PerformMicrotaskCheckpoint();
        if (options.pumpRunLoop) {
            ALooper_pollOnce(10 /* ms */, nullptr, nullptr, nullptr);
            isolate->PerformMicrotaskCheckpoint();
        }
    };

    const auto deadline =
            std::chrono::steady_clock::now() +
            std::chrono::milliseconds(static_cast<int64_t>(options.deadlineSeconds * 1000.0));
    bool settled = false;

    // State is checked before the first pump: a synchronous graph's evaluation
    // promise is already settled when Evaluate() returns, so it exits here
    // without paying for a looper slice.
    while (!promiseTc.HasCaught()) {
        Promise::PromiseState state = promise->State();
        if (state != Promise::kPending) {
            settled = true;
            if (state == Promise::kRejected) {
                ThrowModuleEvaluationRejection(isolate, promise, promiseTc, canonicalPath);
            }
            break;
        }

        if (std::chrono::steady_clock::now() >= deadline) {
            break;
        }

        pumpAsyncProgress();
        if (!options.pumpRunLoop) {
            usleep(1000);  // 1ms delay for non-HTTP top-level await polling
        }
    }

    if (!settled && promise->State() == Promise::kPending &&
        options.timeoutBehavior == ModuleEvaluationOptions::TimeoutBehavior::kThrow) {
        RemoveModuleFromRegistry(canonicalPath);
        throw NativeScriptException("Top-level await timed out for ES module " + canonicalPath);
    }

    return MaybeLocal<Promise>();
}

// The shared probe behind both entry-evaluation queries: a registry hit plus
// Evaluate(), which hands back the SAME capability promise rather than
// re-running anything, so it is cheap enough to call from a pump loop.
static MaybeLocal<Promise> EntryEvaluationPromise(Isolate* isolate, const std::string& path) {
    if (!ModuleInternal::IsESModule(path) && !IsHttpModulePath(path)) {
        return MaybeLocal<Promise>();
    }
    auto* registryPtr = ModuleRegistryFor(isolate);
    if (registryPtr == nullptr) {
        return MaybeLocal<Promise>();
    }
    auto it = registryPtr->find(CanonicalizeRegistryKey(path));
    if (it == registryPtr->end()) {
        return MaybeLocal<Promise>();
    }
    Local<Module> mod = it->second.Get(isolate);
    // A TLA-parked module reports kEvaluated while its promise is still
    // pending, so the status is the gate to *having* a promise, never to its
    // state.
    if (mod.IsEmpty() || mod->GetStatus() != Module::kEvaluated) {
        return MaybeLocal<Promise>();
    }
    TryCatch tc(isolate);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Value> result;
    if (!mod->Evaluate(context).ToLocal(&result) || !result->IsPromise()) {
        return MaybeLocal<Promise>();
    }
    return MaybeLocal<Promise>(result.As<Promise>());
}

MaybeLocal<Promise> ModuleInternal::PendingEntryEvaluation(Isolate* isolate,
                                                           const std::string& path) {
    Local<Promise> promise;
    if (!EntryEvaluationPromise(isolate, path).ToLocal(&promise)) {
        return MaybeLocal<Promise>();
    }
    if (promise->State() != Promise::kPending) {
        return MaybeLocal<Promise>();
    }
    return MaybeLocal<Promise>(promise);
}

EntryEvaluationState ModuleInternal::PollEntryEvaluation(Isolate* isolate, const std::string& path,
                                                         std::string* rejectionReason) {
    Local<Promise> promise;
    if (!EntryEvaluationPromise(isolate, path).ToLocal(&promise)) {
        return EntryEvaluationState::kNone;
    }
    switch (promise->State()) {
        case Promise::kPending:
            return EntryEvaluationState::kPending;
        case Promise::kFulfilled:
            return EntryEvaluationState::kFulfilled;
        case Promise::kRejected:
            break;
    }
    if (rejectionReason != nullptr) {
        Local<Value> reason = promise->Result();
        *rejectionReason =
                reason.IsEmpty() ? "<no reason>" : ArgConverter::ToString(isolate, reason);
    }
    return EntryEvaluationState::kRejected;
}

// The root entry point for an ES module graph: compile + register the root,
// then instantiate and evaluate it once. Dependencies are compiled and
// registered by ResolveModuleCallback while V8 walks the graph from here;
// nothing below the root evaluates on its own.
Local<Value> ModuleInternal::LoadESModule(Isolate* isolate, const std::string& path,
                                          const ModuleEvaluationOptions& options) {
    auto context = isolate->GetCurrentContext();
    const bool isHttpModule = IsHttpModulePath(path);
    // The key the resolver would derive for this same module as someone's
    // dependency. Keying the root by anything else mints a second identity for
    // one file, so a cycle back to the root would not terminate on its entry.
    const std::string canonicalPath = CanonicalizeRegistryKey(path);
    const std::string requestPath = isHttpModule ? NormalizeHttpModuleUrl(path) : canonicalPath;

    Local<Module> module;

    if (isHttpModule) {
        RunModuleGraphLoadPumped(isolate, context, requestPath, kModuleEvaluateDeadlineSeconds);
        // The loader throws the classifier's reason (status, MIME or
        // transport); catch it so it lands in the message instead of staying
        // pending on the isolate behind a C++ throw.
        TryCatch tcLoad(isolate);
        MaybeLocal<Module> maybeMod = LoadHttpModuleForUrl(isolate, context, requestPath);
        if (!maybeMod.ToLocal(&module)) {
            std::string message = "Cannot load ES module " + requestPath;
            if (tcLoad.HasCaught()) {
                throw NativeScriptException(tcLoad, message);
            }
            std::string reason = TakeLastHttpFetchErrorReason();
            if (!reason.empty()) {
                message.append(" — ");
                message.append(reason);
            }
            throw NativeScriptException(message);
        }
        if (module->GetStatus() == Module::kEvaluated) {
            // A top-level-await graph reports kEvaluated while its capability
            // promise is still pending, so the namespace here may be in its TDZ;
            // require() refuses the graph whatever the load order, matching Node.
            if (options.policy == ModuleEvaluationPolicy::kSyncStrict && module->IsGraphAsync()) {
                ThrowAsyncGraphRefusal(canonicalPath);
            }
            return module->GetModuleNamespace();
        }
    } else {
        auto* registryPtr = ModuleRegistryFor(isolate);
        if (registryPtr == nullptr) {
            return Local<Value>();
        }
        auto& registry = *registryPtr;

        auto existingIt = registry.find(canonicalPath);
        if (existingIt != registry.end()) {
            Local<Module> existing = existingIt->second.Get(isolate);
            Module::Status status = existing.IsEmpty() ? Module::kErrored : existing->GetStatus();
            if (status == Module::kErrored) {
                RemoveModuleFromRegistry(canonicalPath);
            } else if (status == Module::kEvaluated) {
                // A top-level-await graph reports kEvaluated while its capability
                // promise is still pending, so the namespace here may be in its TDZ;
                // require() refuses the graph whatever the load order, matching Node.
                if (options.policy == ModuleEvaluationPolicy::kSyncStrict &&
                    existing->IsGraphAsync()) {
                    ThrowAsyncGraphRefusal(canonicalPath);
                }
                return existing->GetModuleNamespace();
            } else if (status == Module::kUninstantiated || status == Module::kInstantiated) {
                // Recompiling would mint a second module identity while importers still
                // hold this one; reuse it and let InstantiateModule below no-op
                // (kInstantiated) or link it (kUninstantiated).
                module = existing;
            }
        }

        if (module.IsEmpty()) {
            // Discovery pre-pass for local roots too: a local graph can reach HTTP
            // edges, and without this they hit the resolver cold and fetch serially,
            // one blocking request at a time. The walk compiles and registers the
            // whole closure up front — including this root — so instantiation
            // resolves as pure lookup. A graph with no HTTP edges settles inside the
            // call and pays no wait.
            RunModuleGraphLoadPumped(isolate, context, canonicalPath,
                                     kModuleEvaluateDeadlineSeconds);
            auto walkedIt = registry.find(canonicalPath);
            if (walkedIt != registry.end()) {
                Local<Module> walked = walkedIt->second.Get(isolate);
                if (!walked.IsEmpty() && walked->GetStatus() != Module::kErrored) {
                    module = walked;
                }
            }
        }

        if (module.IsEmpty()) {
            TryCatch tcCompile(isolate);
            if (!CompileFileEsModule(isolate, canonicalPath).ToLocal(&module)) {
                if (tcCompile.HasCaught()) {
                    throw NativeScriptException(tcCompile, "Cannot compile ES module " + canonicalPath);
                } else {
                    throw NativeScriptException(string("Cannot compile ES module ") + canonicalPath);
                }
            }

            UnindexModuleForIsolate(isolate, canonicalPath);
            auto it = registry.find(canonicalPath);
            if (it != registry.end()) {
                it->second.Reset();
            }
            registry[canonicalPath].Reset(isolate, module);
            IndexModuleForIsolate(isolate, canonicalPath, module);
        }
    }

    // Instantiate (link) with ResolveModuleCallback
    if (module->GetStatus() < Module::kInstantiated) {
        TryCatch tcLink(isolate);
        bool linked = module->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false);

        if (!linked) {
            if (tcLink.HasCaught()) {
                throw NativeScriptException(tcLink, "Cannot instantiate module " + canonicalPath);
            } else {
                throw NativeScriptException(string("Cannot instantiate module ") + canonicalPath);
            }
        }
    }

    // Evaluate the graph under the caller's options.
    EvaluateModuleGraph(isolate, context, module, canonicalPath, options);

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
