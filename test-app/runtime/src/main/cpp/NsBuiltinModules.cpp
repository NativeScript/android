#include "NsBuiltinModules.h"

#include <android/log.h>

#include <vector>

#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "HttpLoader.h"
#include "NativeScriptAssert.h"
#include "Runtime.h"
#include "RuntimeState.h"
#include "TraceLog.h"
#include "console/Console.h"
#include "robin_hood.h"

using namespace v8;

namespace tns {

namespace {

constexpr const char* kNsPrefix = "ns:";
constexpr const char* kNodePrefix = "node:";

struct Registration {
    const char* specifier;
    BuiltinId builtin;
};

/*
 * The v1 registry (docs/ns-builtin-modules.md). One specifier, one source
 * file: a `node:` shim is its own builtin that requires the `ns:` module it
 * adapts, so the two module objects stay distinct and the standard module
 * never carries compatibility code.
 */
constexpr Registration kRegistry[] = {
        {"ns:module", BuiltinId::kNsModule},
        {"ns:runtime", BuiltinId::kNsRuntime},
        {"ns:util", BuiltinId::kNsUtil},
        {"node:module", BuiltinId::kNodeModule},
        {"node:url", BuiltinId::kNodeUrl},
        {"node:util", BuiltinId::kNodeUtil},
};

constexpr const char* kDebugKey = "debug";

void ThrowTypeError(Isolate* isolate, const std::string& message) {
    isolate->ThrowException(Exception::TypeError(ArgConverter::ConvertToV8String(isolate, message)));
}

bool EnsureMainIsolateWrite(Isolate* isolate, const std::string& key) {
    Runtime* runtime = Runtime::GetRuntime(isolate);
    if (runtime == nullptr || !runtime->IsMainThread()) {
        ThrowTypeError(isolate, "'" + key +
                                        "' is process-wide and can only be set from the main "
                                        "isolate");
        return false;
    }
    return true;
}

void SetConfigCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    if (info.Length() < 2 || !info[0]->IsString()) {
        ThrowTypeError(isolate, "setConfig expects (key: string, value)");
        return;
    }
    std::string key = ArgConverter::ConvertToString(info[0].As<String>());
    if (key == kDebugKey) {
        if (!EnsureMainIsolateWrite(isolate, key)) {
            return;
        }
        if (!info[1]->IsString()) {
            ThrowTypeError(isolate, "'" + key + "' must be a comma-separated category string (" +
                                            tns::AllLogCategoryNames() +
                                            "), or '' to disable tracing");
            return;
        }
        // The list replaces the whole mask, so a caller never has to know what
        // was already on to turn something off.
        std::string value = ArgConverter::ConvertToString(info[1].As<String>());
        bool hadUnknown = false;
        uint32_t mask = tns::ParseLogCategories(value, &hadUnknown);
        tns::SetEnabledLogCategories(mask);
        if (hadUnknown) {
            DEBUG_WRITE_FORCE(
                    "ns:runtime setConfig('debug', '%s'): ignoring unknown categories; valid "
                    "categories are %s",
                    value.c_str(), tns::AllLogCategoryNames().c_str());
        }
        return;
    }
    ThrowTypeError(isolate, "Unknown runtime config key: '" + key + "'");
}

void GetConfigCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    if (info.Length() < 1 || !info[0]->IsString()) {
        ThrowTypeError(isolate, "getConfig expects (key: string)");
        return;
    }
    std::string key = ArgConverter::ConvertToString(info[0].As<String>());
    if (key == kDebugKey) {
        info.GetReturnValue().Set(
                ArgConverter::ConvertToV8String(isolate, tns::EnabledLogCategoryNames()));
        return;
    }
    ThrowTypeError(isolate, "Unknown runtime config key: '" + key + "'");
}

const Registration* Find(const std::string& specifier) {
    for (const Registration& registration : kRegistry) {
        if (specifier == registration.specifier) {
            return &registration;
        }
    }
    return nullptr;
}

bool HasPrefix(const std::string& specifier, const char* prefix) {
    return specifier.rfind(prefix, 0) == 0;
}

/*
 * A builtin module is a singleton per realm, so every cache here is per
 * runtime: workers get their own exports objects and their own synthetic
 * modules. The process-global g_moduleRegistry deliberately holds none of
 * this. Touched only from its own runtime's thread.
 */
struct RealmState {
    robin_hood::unordered_map<std::string, Persistent<Object>*> exports;
    robin_hood::unordered_map<std::string, Persistent<Module>*> modules;
    // Specifiers currently being built, so a shim requiring back into the
    // module that is loading it fails instead of recursing.
    robin_hood::unordered_set<std::string> inProgress;
    Persistent<Function>* format = nullptr;
    bool formatUnavailable = false;

    ~RealmState() {
        for (auto& entry : exports) {
            delete entry.second;
        }
        for (auto& entry : modules) {
            delete entry.second;
        }
        delete format;
    }
};

/*
 * This runtime's realm. Per-runtime state rather than an isolate-keyed shared
 * map, so reaching it needs no lock and it is released with the runtime. Null
 * once the runtime has begun tearing down.
 */
RealmState* GetRealm(Isolate* isolate) {
    return RuntimeState::For<RealmState>(isolate);
}

MaybeLocal<Object> BuildBinding(Local<Context> context, BuiltinId builtin) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    Local<Object> binding = Object::New(isolate);

    switch (builtin) {
        case BuiltinId::kNsModule: {
            if (!BuildNsModuleBinding(context, binding)) {
                return MaybeLocal<Object>();
            }
            break;
        }
        case BuiltinId::kNsRuntime: {
            Local<v8::Function> setConfig, getConfig;
            if (!v8::Function::New(context, SetConfigCallback).ToLocal(&setConfig) ||
                !v8::Function::New(context, GetConfigCallback).ToLocal(&getConfig) ||
                !binding->Set(context, ArgConverter::ConvertToV8String(isolate, "setConfig"),
                              setConfig)
                         .FromMaybe(false) ||
                !binding->Set(context, ArgConverter::ConvertToV8String(isolate, "getConfig"),
                              getConfig)
                         .FromMaybe(false)) {
                return MaybeLocal<Object>();
            }
            break;
        }
        case BuiltinId::kNsUtil: {
            // The console formatter is built once per realm; ns:util
            // re-exports that instance instead of creating a second one.
            Local<v8::Function> inspect = Console::getInspect(context);
            if (inspect.IsEmpty()) {
                return MaybeLocal<Object>();
            }
            if (!binding->Set(context, ArgConverter::ConvertToV8String(isolate, "inspect"), inspect)
                         .FromMaybe(false)) {
                return MaybeLocal<Object>();
            }
            break;
        }
        default:
            break;
    }

    return binding;
}

/*
 * Runs a module's builtin and caches its exports. Always leaves an exception
 * pending when it returns false.
 */
bool Instantiate(Local<Context> context, const Registration& requested) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    RealmState* realmState = GetRealm(isolate);
    if (realmState == nullptr) {
        isolate->ThrowException(Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "Cannot load a builtin module: the runtime is shutting down")));
        return false;
    }
    RealmState& realm = *realmState;

    /*
     * A shim reaches its ns: module through the builtin require, so the graph
     * is walked while a module is still being built; a cycle would otherwise
     * recurse until the stack runs out.
     */
    if (realm.inProgress.count(requested.specifier) > 0) {
        isolate->ThrowException(Exception::Error(ArgConverter::ConvertToV8String(
                isolate, "Circular require of built-in module: " +
                                 std::string(requested.specifier))));
        return false;
    }
    realm.inProgress.emplace(requested.specifier);

    TryCatch tc(isolate);
    Local<Object> binding;
    Local<Value> result;
    bool built = BuildBinding(context, requested.builtin).ToLocal(&binding) &&
                 BuiltinLoader::RunBuiltin(context, requested.builtin, binding).ToLocal(&result) &&
                 result->IsObject();
    realm.inProgress.erase(requested.specifier);

    if (built) {
        realm.exports[requested.specifier] = new Persistent<Object>(isolate, result.As<Object>());
        return true;
    }

    if (tc.HasCaught()) {
        tc.ReThrow();
        return false;
    }
    isolate->ThrowException(Exception::Error(ArgConverter::ConvertToV8String(
            isolate, "Failed to initialize built-in module '" + std::string(requested.specifier) +
                             "'")));
    return false;
}

/*
 * The exports a synthetic module re-exports by name, in the order used both
 * when declaring the export names and when populating them.
 */
MaybeLocal<v8::Array> ExportKeys(Local<Context> context, Local<Object> exports) {
    return exports->GetOwnPropertyNames(context, PropertyFilter::ONLY_ENUMERABLE,
                                        KeyConversionMode::kConvertToString);
}

MaybeLocal<Module> NoDependencies(Local<Context> context, Local<v8::String> specifier,
                                  Local<FixedArray> import_attributes, Local<Module> referrer) {
    // Synthetic modules never request anything.
    return MaybeLocal<Module>();
}

MaybeLocal<Value> PopulateSyntheticModule(Local<Context> context, Local<Module> module) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    // The specifier was handed to CreateSyntheticModule as the module name,
    // which is how these steps find the exports object again.
    std::string specifier = ArgConverter::ConvertToString(module->GetResourceName().As<v8::String>());

    Local<Object> exports;
    if (!NsBuiltinModules::GetExports(context, specifier).ToLocal(&exports)) {
        return MaybeLocal<Value>();
    }

    Local<v8::String> defaultName = ArgConverter::ConvertToV8String(isolate, "default");
    if (module->SetSyntheticModuleExport(isolate, defaultName, exports).IsNothing()) {
        return MaybeLocal<Value>();
    }

    Local<v8::Array> keys;
    if (!ExportKeys(context, exports).ToLocal(&keys)) {
        return MaybeLocal<Value>();
    }
    for (uint32_t i = 0; i < keys->Length(); i++) {
        Local<Value> key;
        Local<Value> value;
        if (!keys->Get(context, i).ToLocal(&key) || !key->IsString()) {
            return MaybeLocal<Value>();
        }
        if (key.As<v8::String>()->StringEquals(defaultName)) {
            continue;
        }
        if (!exports->Get(context, key).ToLocal(&value) ||
            module->SetSyntheticModuleExport(isolate, key.As<v8::String>(), value).IsNothing()) {
            return MaybeLocal<Value>();
        }
    }

    Local<Promise::Resolver> resolver;
    if (!Promise::Resolver::New(context).ToLocal(&resolver) ||
        !resolver->Resolve(context, Undefined(isolate)).FromMaybe(false)) {
        return MaybeLocal<Value>();
    }
    return resolver->GetPromise();
}

}  // namespace

bool NsBuiltinModules::IsBuiltinScheme(const std::string& specifier) {
    return HasPrefix(specifier, kNsPrefix) || HasPrefix(specifier, kNodePrefix);
}

bool NsBuiltinModules::IsNsScheme(const std::string& specifier) {
    return HasPrefix(specifier, kNsPrefix);
}

bool NsBuiltinModules::IsRegistered(const std::string& specifier) {
    return Find(specifier) != nullptr;
}

std::string NsBuiltinModules::NotFoundMessage(const std::string& specifier) {
    return "No such built-in module: " + specifier;
}

MaybeLocal<Object> NsBuiltinModules::GetExports(Local<Context> context,
                                                const std::string& specifier) {
    const Registration* registration = Find(specifier);
    if (registration == nullptr) {
        return MaybeLocal<Object>();
    }

    Isolate* isolate = v8::Isolate::GetCurrent();
    RealmState* realmState = GetRealm(isolate);
    if (realmState == nullptr) {
        return MaybeLocal<Object>();
    }
    RealmState& realm = *realmState;
    auto it = realm.exports.find(specifier);
    if (it == realm.exports.end()) {
        if (!Instantiate(context, *registration)) {
            return MaybeLocal<Object>();
        }
        it = realm.exports.find(specifier);
        if (it == realm.exports.end()) {
            return MaybeLocal<Object>();
        }
    }
    return it->second->Get(isolate);
}

MaybeLocal<Module> NsBuiltinModules::GetModule(Local<Context> context,
                                               const std::string& specifier) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    RealmState* realmState = GetRealm(isolate);
    if (realmState == nullptr) {
        return MaybeLocal<Module>();
    }
    RealmState& realm = *realmState;

    auto it = realm.modules.find(specifier);
    if (it != realm.modules.end()) {
        Local<Module> cached = it->second->Get(isolate);
        if (!cached.IsEmpty() && cached->GetStatus() != Module::kErrored) {
            return cached;
        }
        delete it->second;
        realm.modules.erase(it);
    }

    Local<Object> exports;
    if (!GetExports(context, specifier).ToLocal(&exports)) {
        return MaybeLocal<Module>();
    }

    Local<v8::String> defaultName = ArgConverter::ConvertToV8String(isolate, "default");
    std::vector<Local<v8::String>> exportNames{defaultName};
    Local<v8::Array> keys;
    if (!ExportKeys(context, exports).ToLocal(&keys)) {
        return MaybeLocal<Module>();
    }
    for (uint32_t i = 0; i < keys->Length(); i++) {
        Local<Value> key;
        if (!keys->Get(context, i).ToLocal(&key) || !key->IsString()) {
            return MaybeLocal<Module>();
        }
        if (!key.As<v8::String>()->StringEquals(defaultName)) {
            exportNames.push_back(key.As<v8::String>());
        }
    }

    Local<Module> module = Module::CreateSyntheticModule(
            isolate, ArgConverter::ConvertToV8String(isolate, specifier),
            MemorySpan<const Local<v8::String>>(exportNames.data(), exportNames.size()),
            PopulateSyntheticModule);
    // No dependencies and no user code, so the module can be driven to its
    // final state here; importers then only ever see an evaluated module.
    if (!module->InstantiateModule(context, &NoDependencies).FromMaybe(false)) {
        return MaybeLocal<Module>();
    }
    if (module->Evaluate(context).IsEmpty()) {
        return MaybeLocal<Module>();
    }

    realm.modules[specifier] = new Persistent<Module>(isolate, module);
    return module;
}

Local<v8::Function> NsBuiltinModules::GetFormatFunc(Local<Context> context) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    RealmState* realmState = GetRealm(isolate);
    if (realmState == nullptr) {
        return Local<v8::Function>();
    }
    RealmState& realm = *realmState;
    if (realm.format != nullptr) {
        return realm.format->Get(isolate);
    }
    if (realm.formatUnavailable) {
        return Local<v8::Function>();
    }

    TryCatch tc(isolate);
    Local<Object> exports;
    Local<Value> format;
    if (!GetExports(context, "ns:util").ToLocal(&exports) ||
        !exports->Get(context, ArgConverter::ConvertToV8String(isolate, "format")).ToLocal(&format) ||
        !format->IsFunction()) {
        // The caller degrades silently, so this is the only place the failure
        // is ever visible.
        __android_log_write(ANDROID_LOG_WARN, "JS",
                            "Warning: console failed to initialize the ns:util formatter");
        // One attempt per realm: a broken builtin must not make every log call
        // recompile it.
        realm.formatUnavailable = true;
        return Local<v8::Function>();
    }

    realm.format = new Persistent<v8::Function>(isolate, format.As<v8::Function>());
    return format.As<v8::Function>();
}

}  // namespace tns
