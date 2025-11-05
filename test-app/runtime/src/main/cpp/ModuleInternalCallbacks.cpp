#include "ModuleInternal.h"
#include "ArgConverter.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"
#include "Runtime.h"
#include "Util.h"
#include <sys/stat.h>
#include <string>
#include <vector>
#include <algorithm>
#include <atomic>

using namespace v8;
using namespace std;
using namespace tns;

// External global module registry declared in ModuleInternal.cpp
extern std::unordered_map<std::string, v8::Global<v8::Module>> g_moduleRegistry;

// Forward declaration used by logging helper
std::string GetApplicationPath();

// Cached toggle for verbose script loading logs sourced from Java AppConfig via JNI
static bool ShouldLogScriptLoading() {
    static std::atomic<int> cached{-1}; // -1 unknown, 0 false, 1 true
    int v = cached.load(std::memory_order_acquire);
    if (v != -1) {
        return v == 1;
    }

    static std::once_flag initFlag;
    std::call_once(initFlag, []() {
        bool enabled = false;
        try {
            JEnv env;
            jclass runtimeClass = env.FindClass("com/tns/Runtime");
            if (runtimeClass != nullptr) {
                jmethodID mid = env.GetStaticMethodID(runtimeClass, "getLogScriptLoadingEnabled", "()Z");
                if (mid != nullptr) {
                    jboolean res = env.CallStaticBooleanMethod(runtimeClass, mid);
                    enabled = (res == JNI_TRUE);
                }
            }
        } catch (...) {
            // ignore and keep default false
        }
        cached.store(enabled ? 1 : 0, std::memory_order_release);
    });

    return cached.load(std::memory_order_acquire) == 1;
}

// Import meta callback to support import.meta.url and import.meta.dirname
void InitializeImportMetaObject(Local<Context> context, Local<Module> module, Local<Object> meta) {
    Isolate* isolate = context->GetIsolate();
    
    // Look up the module path in the global module registry (with safety checks)
    std::string modulePath;
    
    try {
        for (auto& kv : g_moduleRegistry) {
            // Check if Global handle is empty before accessing
            if (kv.second.IsEmpty()) {
                continue;
            }
            
            Local<Module> registered = kv.second.Get(isolate);
            if (!registered.IsEmpty() && registered == module) {
                modulePath = kv.first;
                break;
            }
        }
    } catch (...) {
        DEBUG_WRITE("InitializeImportMetaObject: Exception during module registry lookup, using fallback");
        modulePath = "";  // Will use fallback path
    }
    
    if (ShouldLogScriptLoading()) {
        DEBUG_WRITE("InitializeImportMetaObject: Module lookup: found path = %s",
                    modulePath.empty() ? "(empty)" : modulePath.c_str());
        DEBUG_WRITE("InitializeImportMetaObject: Registry size: %zu", g_moduleRegistry.size());
    }
    
    // Convert file path to file:// URL
    std::string moduleUrl;
    if (!modulePath.empty()) {
        // Create file:// URL from the full path
        moduleUrl = "file://" + modulePath;
    } else {
        // Fallback URL if module not found in registry
        moduleUrl = "file:///android_asset/app/";
    }
    
    if (ShouldLogScriptLoading()) {
        DEBUG_WRITE("InitializeImportMetaObject: Final URL: %s", moduleUrl.c_str());
    }
    
    Local<String> url = ArgConverter::ConvertToV8String(isolate, moduleUrl);
    
    // Set import.meta.url property
    meta->CreateDataProperty(context, ArgConverter::ConvertToV8String(isolate, "url"), url).Check();
    
    // Add import.meta.dirname support (extract directory from path)
    std::string dirname;
    if (!modulePath.empty()) {
        size_t lastSlash = modulePath.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            dirname = modulePath.substr(0, lastSlash);
        } else {
            dirname = "/android_asset/app";  // fallback
        }
    } else {
        dirname = "/android_asset/app";  // fallback
    }
    
    Local<String> dirnameStr = ArgConverter::ConvertToV8String(isolate, dirname);
    
    // Set import.meta.dirname property
    meta->CreateDataProperty(context, ArgConverter::ConvertToV8String(isolate, "dirname"), dirnameStr).Check();
}

// Helper function to check if a file exists and is a regular file
bool IsFile(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        return false;
    }
    return (st.st_mode & S_IFMT) == S_IFREG;
}

// Helper function to add extension if missing
std::string WithExtension(const std::string& path, const std::string& ext) {
    if (path.size() >= ext.size() && path.compare(path.size() - ext.size(), ext.size(), ext) == 0) {
        return path;
    }
    return path + ext;
}

// Helper function to check if a module is a Node.js built-in (e.g., node:url)
bool IsNodeBuiltinModule(const std::string& spec) {
    return spec.size() > 5 && spec.substr(0, 5) == "node:";
}

// Helper function to get application path (for Android, we'll use a simple approach)
std::string GetApplicationPath() {
    // For Android, use the actual file system path instead of asset path
    // This should match the ApplicationFilesPath + "/app" from Module.java
    JEnv env;
    jstring applicationFilesPath = (jstring) env.CallStaticObjectMethod(ModuleInternal::MODULE_CLASS, ModuleInternal::GET_APPLICATION_FILES_PATH_METHOD_ID);
    std::string path = ArgConverter::jstringToString(applicationFilesPath);
    return path + "/app";
}

// ResolveModuleCallback - Main callback invoked by V8 to resolve import statements
v8::MaybeLocal<v8::Module> ResolveModuleCallback(v8::Local<v8::Context> context,
                                                 v8::Local<v8::String> specifier,
                                                 v8::Local<v8::FixedArray> import_assertions,
                                                 v8::Local<v8::Module> referrer) {
    v8::Isolate* isolate = context->GetIsolate();

    // 1) Convert specifier to std::string
    v8::String::Utf8Value specUtf8(isolate, specifier);
    std::string spec = *specUtf8 ? *specUtf8 : "";
    if (spec.empty()) {
        return v8::MaybeLocal<v8::Module>();
    }

    // Debug logging
    if (ShouldLogScriptLoading()) {
        DEBUG_WRITE("ResolveModuleCallback: Resolving '%s'", spec.c_str());
    }

    // 2) Find which filepath the referrer was compiled under
    std::string referrerPath;
    for (auto& kv : g_moduleRegistry) {
        v8::Local<v8::Module> registered = kv.second.Get(isolate);
        if (registered == referrer) {
            referrerPath = kv.first;
            break;
        }
    }

    // If we couldn't identify the referrer and the specifier is relative, 
    // assume the base directory is the application root
    bool specIsRelative = !spec.empty() && spec[0] == '.';
    if (referrerPath.empty() && specIsRelative) {
        referrerPath = GetApplicationPath() + "/index.mjs";  // Default referrer
    }

    // 3) Compute base directory from referrer path
    size_t slash = referrerPath.find_last_of("/\\");
    std::string baseDir = slash == std::string::npos ? "" : referrerPath.substr(0, slash + 1);

    // 4) Build candidate paths for resolution
    std::vector<std::string> candidateBases;
    std::string appPath = GetApplicationPath();

    if (!spec.empty() && spec[0] == '.') {
        // Relative import (./ or ../)
        std::string cleanSpec = spec.substr(0, 2) == "./" ? spec.substr(2) : spec;
        std::string candidate = baseDir + cleanSpec;
        candidateBases.push_back(candidate);
    if (ShouldLogScriptLoading()) {
            DEBUG_WRITE("ResolveModuleCallback: Relative import: '%s' + '%s' -> '%s'",
                       baseDir.c_str(), cleanSpec.c_str(), candidate.c_str());
        }
    } else if (spec.size() > 7 && spec.substr(0, 7) == "file://") {
        // Absolute file URL
        std::string tail = spec.substr(7);  // strip file://
        if (tail.empty() || tail[0] != '/') {
            tail = "/" + tail;
        }

        // Map common virtual roots to the real appPath
        const std::string appVirtualRoot = "/app/";                      // e.g. file:///app/foo.mjs
        const std::string androidAssetAppRoot = "/android_asset/app/";   // e.g. file:///android_asset/app/foo.mjs

        std::string candidate;
        if (tail.rfind(appVirtualRoot, 0) == 0) {
            // Drop the leading "/app/" and prepend real appPath
            candidate = appPath + "/" + tail.substr(appVirtualRoot.size());
            if (ShouldLogScriptLoading()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// to appPath mapping: '%s' -> '%s'", tail.c_str(), candidate.c_str());
            }
        } else if (tail.rfind(androidAssetAppRoot, 0) == 0) {
            // Replace "/android_asset/app/" with the real appPath
            candidate = appPath + "/" + tail.substr(androidAssetAppRoot.size());
            if (ShouldLogScriptLoading()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// android_asset mapping: '%s' -> '%s'", tail.c_str(), candidate.c_str());
            }
        } else if (tail.rfind(appPath, 0) == 0) {
            // Already an absolute on-disk path to the app folder
            candidate = tail;
            if (ShouldLogScriptLoading()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// absolute path preserved: '%s'", candidate.c_str());
            }
        } else {
            // Fallback: treat as absolute on-disk path
            candidate = tail;
            if (ShouldLogScriptLoading()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// generic absolute: '%s'", candidate.c_str());
            }
        }

        candidateBases.push_back(candidate);
    } else if (!spec.empty() && spec[0] == '~') {
        // Alias to application root using ~/path
        std::string tail = spec.size() >= 2 && spec[1] == '/' ? spec.substr(2) : spec.substr(1);
        std::string candidate = appPath + "/" + tail;
        candidateBases.push_back(candidate);
    } else if (!spec.empty() && spec[0] == '/') {
        // Absolute path within the bundle
        candidateBases.push_back(appPath + spec);
    } else {
        // Bare specifier – resolve relative to the application root
        std::string candidate = appPath + "/" + spec;
        candidateBases.push_back(candidate);

        // Try converting underscores to slashes (bundler heuristic)
        std::string withSlashes = spec;
        std::replace(withSlashes.begin(), withSlashes.end(), '_', '/');
        std::string candidateSlashes = appPath + "/" + withSlashes;
        if (candidateSlashes != candidate) {
            candidateBases.push_back(candidateSlashes);
        }
    }

    // 5) Attempt to resolve to an actual file
    std::string absPath;
    bool found = false;

    for (const std::string& baseCandidate : candidateBases) {
        absPath = baseCandidate;

        // Check if file exists as-is
        if (IsFile(absPath)) {
            found = true;
            break;
        }

        // Try adding extensions
        const char* exts[] = {".mjs", ".js"};
        for (const char* ext : exts) {
            std::string candidate = WithExtension(absPath, ext);
            if (IsFile(candidate)) {
                absPath = candidate;
                found = true;
                break;
            }
        }
        if (found) break;

        // Try index files if path is a directory
        const char* indexExts[] = {"/index.mjs", "/index.js"};
        for (const char* idx : indexExts) {
            std::string candidate = absPath + idx;
            if (IsFile(candidate)) {
                absPath = candidate;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    // 6) Handle special cases if file not found
    if (!found) {
        // Check for Node.js built-in modules
        if (IsNodeBuiltinModule(spec)) {
            std::string builtinName = spec.substr(5);  // Remove "node:" prefix
            
            // Create polyfill content for Node.js built-in modules
            std::string polyfillContent;
            
            if (builtinName == "url") {
                // Create a polyfill for node:url with fileURLToPath
                polyfillContent = "// Polyfill for node:url\n"
                                "export function fileURLToPath(url) {\n"
                                "  if (typeof url === 'string') {\n"
                                "    if (url.startsWith('file://')) {\n"
                                "      return decodeURIComponent(url.slice(7));\n"
                                "    }\n"
                                "    return url;\n"
                                "  }\n"
                                "  if (url && typeof url.href === 'string') {\n"
                                "    return fileURLToPath(url.href);\n"
                                "  }\n"
                                "  throw new Error('Invalid URL');\n"
                                "}\n"
                                "\n"
                                "export function pathToFileURL(path) {\n"
                                "  return new URL('file://' + encodeURIComponent(path));\n"
                                "}\n";
            } else if (builtinName == "module") {
                // Create a polyfill for node:module with createRequire
                polyfillContent = "// Polyfill for node:module\n"
                                "export function createRequire(filename) {\n"
                                "  // Return the global require function\n"
                                "  // In NativeScript, require is globally available\n"
                                "  if (typeof require === 'function') {\n"
                                "    return require;\n"
                                "  }\n"
                                "  \n"
                                "  // Fallback: create a basic require function\n"
                                "  return function(id) {\n"
                                "    throw new Error('Module ' + id + ' not found. NativeScript require() not available.');\n"
                                "  };\n"
                                "}\n"
                                "\n"
                                "// Export as default as well for compatibility\n"
                                "export default { createRequire };\n";
            } else if (builtinName == "path") {
                // Create a polyfill for node:path
                polyfillContent = "// Polyfill for node:path\n"
                                "export const sep = '/';\n"
                                "export const delimiter = ':';\n"
                                "\n"
                                "export function basename(path, ext) {\n"
                                "  const name = path.split('/').pop() || '';\n"
                                "  return ext && name.endsWith(ext) ? name.slice(0, -ext.length) : name;\n"
                                "}\n"
                                "\n"
                                "export function dirname(path) {\n"
                                "  const parts = path.split('/');\n"
                                "  return parts.slice(0, -1).join('/') || '/';\n"
                                "}\n"
                                "\n"
                                "export function extname(path) {\n"
                                "  const name = basename(path);\n"
                                "  const dot = name.lastIndexOf('.');\n"
                                "  return dot > 0 ? name.slice(dot) : '';\n"
                                "}\n"
                                "\n"
                                "export function join(...paths) {\n"
                                "  return paths.filter(Boolean).join('/').replace(/\\/+/g, '/');\n"
                                "}\n"
                                "\n"
                                "export function resolve(...paths) {\n"
                                "  let resolved = '';\n"
                                "  for (let path of paths) {\n"
                                "    if (path.startsWith('/')) {\n"
                                "      resolved = path;\n"
                                "    } else {\n"
                                "      resolved = join(resolved, path);\n"
                                "    }\n"
                                "  }\n"
                                "  return resolved || '/';\n"
                                "}\n"
                                "\n"
                                "export function isAbsolute(path) {\n"
                                "  return path.startsWith('/');\n"
                                "}\n"
                                "\n"
                                "export default { basename, dirname, extname, join, resolve, isAbsolute, sep, delimiter };\n";
            } else if (builtinName == "fs") {
                // Create a basic polyfill for node:fs
                polyfillContent = "// Polyfill for node:fs\n"
                                "console.warn('Node.js fs module is not supported in NativeScript. Use @nativescript/core File APIs instead.');\n"
                                "\n"
                                "export function readFileSync() {\n"
                                "  throw new Error('fs.readFileSync is not supported in NativeScript. Use @nativescript/core File APIs.');\n"
                                "}\n"
                                "\n"
                                "export function writeFileSync() {\n"
                                "  throw new Error('fs.writeFileSync is not supported in NativeScript. Use @nativescript/core File APIs.');\n"
                                "}\n"
                                "\n"
                                "export function existsSync() {\n"
                                "  throw new Error('fs.existsSync is not supported in NativeScript. Use @nativescript/core File APIs.');\n"
                                "}\n"
                                "\n"
                                "export default { readFileSync, writeFileSync, existsSync };\n";
            } else {
                // Generic polyfill for other Node.js built-in modules
                polyfillContent = "// Polyfill for node:" + builtinName + "\n"
                                "console.warn('Node.js built-in module \\'node:" + builtinName + "\\' is not fully supported in NativeScript');\n"
                                "export default {};\n";
            }
            
            // Create module source and compile it in-memory
            v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, polyfillContent);
            
            // Build URL for stack traces
            std::string moduleUrl = "node:" + builtinName;
            v8::Local<v8::String> urlString = ArgConverter::ConvertToV8String(isolate, moduleUrl);
            
            v8::ScriptOrigin origin(isolate, urlString, 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true /* is_module */);
            v8::ScriptCompiler::Source src(sourceText, origin);
            
            v8::Local<v8::Module> polyfillModule;
            if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&polyfillModule)) {
                std::string msg = "Failed to compile polyfill for: " + spec;
                isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
                return v8::MaybeLocal<v8::Module>();
            }
            
            // Store in registry before instantiation
            g_moduleRegistry[spec].Reset(isolate, polyfillModule);
            
            // Instantiate the module
            if (!polyfillModule->InstantiateModule(context, ResolveModuleCallback).FromMaybe(false)) {
                g_moduleRegistry.erase(spec);
                std::string msg = "Failed to instantiate polyfill for: " + spec;
                isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
                return v8::MaybeLocal<v8::Module>();
            }
            
            // Evaluate the module
            v8::MaybeLocal<v8::Value> evalResult = polyfillModule->Evaluate(context);
            if (evalResult.IsEmpty()) {
                g_moduleRegistry.erase(spec);
                std::string msg = "Failed to evaluate polyfill for: " + spec;
                isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
                return v8::MaybeLocal<v8::Module>();
            }
            
            return v8::MaybeLocal<v8::Module>(polyfillModule);
            
        } else if (tns::ModuleInternal::IsLikelyOptionalModule(spec)) {
            // For optional modules, create a placeholder
            std::string msg = "Optional module not found: " + spec;
            DEBUG_WRITE("ResolveModuleCallback: %s", msg.c_str());
            // Return empty to indicate module not found gracefully
            return v8::MaybeLocal<v8::Module>();
        } else {
            // Regular module not found
            std::string msg = "Cannot find module " + spec + " (tried " + absPath + ")";
            isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
            return v8::MaybeLocal<v8::Module>();
        }
    }

    // 7) Handle JSON modules
    if (absPath.size() >= 5 && absPath.compare(absPath.size() - 5, 5, ".json") == 0) {
    if (ShouldLogScriptLoading()) {
            DEBUG_WRITE("ResolveModuleCallback: Handling JSON module '%s'", absPath.c_str());
        }

        // Read JSON file content
        std::string jsonText = Runtime::GetRuntime(isolate)->ReadFileText(absPath);

        // Create ES module that exports the JSON as default
        std::string moduleSource = "export default " + jsonText + ";";

        v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, moduleSource);
        std::string url = "file://" + absPath;

        v8::Local<v8::String> urlString;
        if (!v8::String::NewFromUtf8(isolate, url.c_str(), v8::NewStringType::kNormal).ToLocal(&urlString)) {
            isolate->ThrowException(v8::Exception::Error(
                ArgConverter::ConvertToV8String(isolate, "Failed to create URL string for JSON module")));
            return v8::MaybeLocal<v8::Module>();
        }

        v8::ScriptOrigin origin(isolate, urlString, 0, 0, false, -1, v8::Local<v8::Value>(), false,
                                false, true /* is_module */);

        v8::ScriptCompiler::Source src(sourceText, origin);

        v8::Local<v8::Module> jsonModule;
        if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&jsonModule)) {
            isolate->ThrowException(v8::Exception::SyntaxError(
                ArgConverter::ConvertToV8String(isolate, "Failed to compile JSON module")));
            return v8::MaybeLocal<v8::Module>();
        }

        // Instantiate and evaluate the JSON module
        if (!jsonModule->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
            return v8::MaybeLocal<v8::Module>();
        }

        v8::MaybeLocal<v8::Value> evalResult = jsonModule->Evaluate(context);
        if (evalResult.IsEmpty()) {
            return v8::MaybeLocal<v8::Module>();
        }

        // Store in registry with safe handle management
        auto it = g_moduleRegistry.find(absPath);
        if (it != g_moduleRegistry.end()) {
            it->second.Reset();
        }
        g_moduleRegistry[absPath].Reset(isolate, jsonModule);
        return v8::MaybeLocal<v8::Module>(jsonModule);
    }

    // 8) Check if we've already compiled this module
    auto it = g_moduleRegistry.find(absPath);
    if (it != g_moduleRegistry.end()) {
    if (ShouldLogScriptLoading()) {
            DEBUG_WRITE("ResolveModuleCallback: Found cached module '%s'", absPath.c_str());
        }
        return v8::MaybeLocal<v8::Module>(it->second.Get(isolate));
    }

    // 9) Compile and register the new module
    if (ShouldLogScriptLoading()) {
        DEBUG_WRITE("ResolveModuleCallback: Compiling new module '%s'", absPath.c_str());
    }
    try {
        // Use our existing LoadESModule function to compile the module
        tns::ModuleInternal::LoadESModule(isolate, absPath);
    } catch (NativeScriptException& ex) {
        DEBUG_WRITE("ResolveModuleCallback: Failed to compile module '%s'", absPath.c_str());
        ex.ReThrowToV8();
        return v8::MaybeLocal<v8::Module>();
    }

    // LoadESModule should have added it to g_moduleRegistry
    auto it2 = g_moduleRegistry.find(absPath);
    if (it2 == g_moduleRegistry.end()) {
        // Something went wrong
        std::string msg = "Failed to register compiled module: " + absPath;
        isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
        return v8::MaybeLocal<v8::Module>();
    }

    return v8::MaybeLocal<v8::Module>(it2->second.Get(isolate));
}

// Dynamic import() host callback
v8::MaybeLocal<v8::Promise> ImportModuleDynamicallyCallback(
    v8::Local<v8::Context> context, v8::Local<v8::Data> host_defined_options,
    v8::Local<v8::Value> resource_name, v8::Local<v8::String> specifier,
    v8::Local<v8::FixedArray> import_assertions) {
    v8::Isolate* isolate = context->GetIsolate();
    
    // Convert specifier to std::string for logging
    v8::String::Utf8Value specUtf8(isolate, specifier);
    std::string spec = *specUtf8 ? *specUtf8 : "";
    
    if (ShouldLogScriptLoading()) {
        DEBUG_WRITE("ImportModuleDynamicallyCallback: Dynamic import for '%s'", spec.c_str());
    }
    
    v8::EscapableHandleScope scope(isolate);

    // Create a Promise resolver we'll resolve/reject synchronously for now.
    v8::Local<v8::Promise::Resolver> resolver;
    if (!v8::Promise::Resolver::New(context).ToLocal(&resolver)) {
        // Failed to create resolver, return empty promise
        return v8::MaybeLocal<v8::Promise>();
    }

    // Re-use the static resolver to locate / compile the module.
    try {
        // Pass empty referrer since this V8 version doesn't expose GetModule() on
        // ScriptOrModule. The resolver will fall back to absolute-path heuristics.
        v8::Local<v8::Module> refMod;

        v8::MaybeLocal<v8::Module> maybeModule =
            ResolveModuleCallback(context, specifier, import_assertions, refMod);

        v8::Local<v8::Module> module;
        if (!maybeModule.ToLocal(&module)) {
            // Resolution failed; reject to avoid leaving a pending Promise (white screen)
            if (ShouldLogScriptLoading()) {
                DEBUG_WRITE("ImportModuleDynamicallyCallback: Resolution failed for '%s'", spec.c_str());
            }
            v8::Local<v8::Value> ex = v8::Exception::Error(
                ArgConverter::ConvertToV8String(isolate, std::string("Failed to resolve module: ") + spec));
            resolver->Reject(context, ex).Check();
            return scope.Escape(resolver->GetPromise());
        }

        // If not yet instantiated/evaluated, do it now
        if (module->GetStatus() == v8::Module::kUninstantiated) {
            if (!module->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
                if (ShouldLogScriptLoading()) {
                    DEBUG_WRITE("ImportModuleDynamicallyCallback: Instantiate failed for '%s'", spec.c_str());
                }
                resolver
                    ->Reject(context,
                             v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "Failed to instantiate module")))
                    .Check();
                return scope.Escape(resolver->GetPromise());
            }
        }

        if (module->GetStatus() != v8::Module::kEvaluated) {
            if (module->Evaluate(context).IsEmpty()) {
                if (ShouldLogScriptLoading()) {
                    DEBUG_WRITE("ImportModuleDynamicallyCallback: Evaluation failed for '%s'", spec.c_str());
                }
                v8::Local<v8::Value> ex =
                    v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "Evaluation failed"));
                resolver->Reject(context, ex).Check();
                return scope.Escape(resolver->GetPromise());
            }
        }

        resolver->Resolve(context, module->GetModuleNamespace()).Check();
    if (ShouldLogScriptLoading()) {
            DEBUG_WRITE("ImportModuleDynamicallyCallback: Successfully resolved '%s'", spec.c_str());
        }
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
    if (ShouldLogScriptLoading()) {
            DEBUG_WRITE("ImportModuleDynamicallyCallback: Native exception for '%s'", spec.c_str());
        }
        resolver
            ->Reject(context, v8::Exception::Error(
                                  ArgConverter::ConvertToV8String(isolate, "Native error during dynamic import")))
            .Check();
    }

    return scope.Escape(resolver->GetPromise());
}
