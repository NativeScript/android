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
#include <cstring>
#include "HMRSupport.h"
#include "DevFlags.h"
#include "JEnv.h"

using namespace v8;
using namespace std;
using namespace tns;

// External global module registry declared in ModuleInternal.cpp
extern std::unordered_map<std::string, v8::Global<v8::Module>> g_moduleRegistry;

// Forward declaration used by logging helper
std::string GetApplicationPath();

// Logging flag now provided via DevFlags for fast cached access

// Diagnostic helper: emit detailed V8 compile error info for HTTP ESM sources.
static void LogHttpCompileDiagnostics(v8::Isolate* isolate,
                                      v8::Local<v8::Context> context,
                                      const std::string& url,
                                      const std::string& code,
                                      v8::TryCatch& tc) {
    if (!IsScriptLoadingLogEnabled()) {
        return;
    }
    using namespace v8;

    const char* classification = "unknown";
    std::string msgStr;
    std::string srcLineStr;
    int lineNum = 0;
    int startCol = 0;
    int endCol = 0;

    Local<Message> message = tc.Message();
    if (!message.IsEmpty()) {
        String::Utf8Value m8(isolate, message->Get());
        if (*m8) msgStr = *m8;
        lineNum = message->GetLineNumber(context).FromMaybe(0);
        startCol = message->GetStartColumn();
        endCol = message->GetEndColumn();
        MaybeLocal<String> maybeLine = message->GetSourceLine(context);
        if (!maybeLine.IsEmpty()) {
            String::Utf8Value l8(isolate, maybeLine.ToLocalChecked());
            if (*l8) srcLineStr = *l8;
        }
        // Heuristics similar to iOS for quick triage
        if (msgStr.find("Unexpected identifier") != std::string::npos ||
            msgStr.find("Unexpected token") != std::string::npos) {
            if (msgStr.find("export") != std::string::npos &&
                code.find("export default") == std::string::npos &&
                code.find("__sfc__") != std::string::npos) {
                classification = "missing-export-default";
            } else {
                classification = "syntax";
            }
        } else if (msgStr.find("Cannot use import statement") != std::string::npos) {
            classification = "wrap-error";
        }
    }
    if (strcmp(classification, "unknown") == 0) {
        if (code.find("export default") == std::string::npos && code.find("__sfc__") != std::string::npos) classification = "missing-export-default";
        else if (code.find("__sfc__") != std::string::npos && code.find("export {") == std::string::npos && code.find("export ") == std::string::npos) classification = "no-exports";
        else if (code.find("import ") == std::string::npos && code.find("export ") == std::string::npos) classification = "not-module";
        else if (code.find("_openBlock") != std::string::npos && code.find("openBlock") == std::string::npos) classification = "underscore-helper-unmapped";
    }

    // FNV-1a 64-bit hash of source for correlation
    unsigned long long h = 1469598103934665603ull; // offset basis
    for (unsigned char c : code) { h ^= c; h *= 1099511628211ull; }

    // Trim the snippet for readability
    std::string snippet = code.substr(0, 600);
    for (char& ch : snippet) { if (ch == '\n' || ch == '\r') ch = ' '; }
    if (srcLineStr.size() > 240) srcLineStr = srcLineStr.substr(0, 240);

    DEBUG_WRITE("[http-esm][compile][v8-error][%s] %s line=%d col=%d..%d hash=%llx bytes=%lu msg=%s srcLine=%s snippet=%s",
                classification,
                url.c_str(),
                lineNum,
                startCol,
                endCol,
                (unsigned long long)h,
                (unsigned long)code.size(),
                msgStr.c_str(),
                srcLineStr.c_str(),
                snippet.c_str());
}

// Helper: resolve relative or root-absolute spec against an HTTP(S) referrer URL.
// Returns empty string if resolution is not possible.
static std::string ResolveHttpRelative(const std::string& referrerUrl, const std::string& spec) {
    if (referrerUrl.empty()) {
        return std::string();
    }
    auto startsWith = [](const std::string& s, const char* pre) -> bool {
        size_t n = strlen(pre);
        return s.size() >= n && s.compare(0, n, pre) == 0;
    };
    if (!(startsWith(referrerUrl, "http://") || startsWith(referrerUrl, "https://"))) {
        return std::string();
    }
    // Normalize referrer: drop fragment and query
    std::string base = referrerUrl;
    size_t hashPos = base.find('#');
    if (hashPos != std::string::npos) base = base.substr(0, hashPos);
    size_t qPos = base.find('?');
    if (qPos != std::string::npos) base = base.substr(0, qPos);

    // Extract origin and path
    size_t schemePos = base.find("://");
    if (schemePos == std::string::npos) {
        return std::string();
    }
    size_t pathStart = base.find('/', schemePos + 3);
    std::string origin = (pathStart == std::string::npos) ? base : base.substr(0, pathStart);
    std::string path = (pathStart == std::string::npos) ? std::string("/") : base.substr(pathStart);

    // Separate query/fragment from spec
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

    // Build new path
    std::string newPath;
    if (!specPath.empty() && specPath[0] == '/') {
        // Root-absolute relative to origin
        newPath = specPath;
    } else {
        // Relative to directory of referrer path
        size_t lastSlash = path.find_last_of('/');
        std::string baseDir = (lastSlash == std::string::npos) ? std::string("/") : path.substr(0, lastSlash + 1);
        newPath = baseDir + specPath;
    }

    // Normalize "." and ".." segments
    std::vector<std::string> stack;
    bool absolute = !newPath.empty() && newPath[0] == '/';
    size_t i = 0;
    while (i <= newPath.size()) {
        size_t j = newPath.find('/', i);
        std::string seg = (j == std::string::npos) ? newPath.substr(i) : newPath.substr(i, j - i);
        if (seg.empty() || seg == ".") {
            // skip
        } else if (seg == "..") {
            if (!stack.empty()) stack.pop_back();
        } else {
            stack.push_back(seg);
        }
        if (j == std::string::npos) break;
        i = j + 1;
    }
    std::string normPath = absolute ? "/" : std::string();
    for (size_t k = 0; k < stack.size(); k++) {
        if (k > 0) normPath += "/";
        normPath += stack[k];
    }
    return origin + normPath + specSuffix;
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
    
    if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("InitializeImportMetaObject: Module lookup: found path = %s",
                    modulePath.empty() ? "(empty)" : modulePath.c_str());
        DEBUG_WRITE("InitializeImportMetaObject: Registry size: %zu", g_moduleRegistry.size());
    }
    
    // Convert to URL for import.meta.url; keep http(s) untouched, file paths with file://
    std::string moduleUrl;
    if (!modulePath.empty()) {
        if (modulePath.rfind("http://", 0) == 0 || modulePath.rfind("https://", 0) == 0) {
            moduleUrl = modulePath;
        } else {
            moduleUrl = "file://" + modulePath;
        }
    } else {
        // Fallback URL if module not found in registry
        moduleUrl = "file:///android_asset/app/";
    }
    
    if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("InitializeImportMetaObject: Final URL: %s", moduleUrl.c_str());
    }
    
    Local<String> url = ArgConverter::ConvertToV8String(isolate, moduleUrl);
    
    // Set import.meta.url property
    meta->CreateDataProperty(context, ArgConverter::ConvertToV8String(isolate, "url"), url).Check();
    
    // Add import.meta.dirname support (extract directory)
    std::string dirname;
    if (!modulePath.empty()) {
        if (modulePath.rfind("http://", 0) == 0 || modulePath.rfind("https://", 0) == 0) {
            // For URLs, compute dirname by trimming after last '/'
            size_t q = modulePath.find('?');
            std::string noQuery = (q == std::string::npos) ? modulePath : modulePath.substr(0, q);
            size_t lastSlash = noQuery.find_last_of('/');
            dirname = (lastSlash == std::string::npos) ? modulePath : noQuery.substr(0, lastSlash);
        } else {
            size_t lastSlash = modulePath.find_last_of("/\\");
            if (lastSlash != std::string::npos) {
                dirname = modulePath.substr(0, lastSlash);
            } else {
                dirname = "/android_asset/app";  // fallback
            }
        }
    } else {
        dirname = "/android_asset/app";  // fallback
    }
    
    Local<String> dirnameStr = ArgConverter::ConvertToV8String(isolate, dirname);
    
    // Set import.meta.dirname property
    meta->CreateDataProperty(context, ArgConverter::ConvertToV8String(isolate, "dirname"), dirnameStr).Check();

    // Attach import.meta.hot for HMR
    tns::InitializeImportMetaHot(isolate, context, meta, modulePath);
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
    if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("ResolveModuleCallback: Resolving '%s'", spec.c_str());
    }

    // Normalize malformed http:/ and https:/ prefixes
    if (spec.rfind("http:/", 0) == 0 && spec.rfind("http://", 0) != 0) {
        spec.insert(5, "/");
    } else if (spec.rfind("https:/", 0) == 0 && spec.rfind("https://", 0) != 0) {
        spec.insert(6, "/");
    }

    // Attempt to resolve relative or root-absolute specifiers against an HTTP referrer URL
    std::string referrerPath;
    for (auto& kv : g_moduleRegistry) {
        v8::Local<v8::Module> registered = kv.second.Get(isolate);
        if (!registered.IsEmpty() && registered == referrer) {
            referrerPath = kv.first;
            break;
        }
    }
    bool specIsRelative = !spec.empty() && spec[0] == '.';
    bool specIsRootAbs = !spec.empty() && spec[0] == '/';
    auto startsWithHttp = [](const std::string& s) -> bool {
        return s.rfind("http://", 0) == 0 || s.rfind("https://", 0) == 0;
    };
    if (!startsWithHttp(spec) && (specIsRelative || specIsRootAbs)) {
        if (!referrerPath.empty() && startsWithHttp(referrerPath)) {
            std::string resolved = ResolveHttpRelative(referrerPath, spec);
            if (!resolved.empty()) {
                if (IsScriptLoadingLogEnabled()) {
                    DEBUG_WRITE("ResolveModuleCallback: HTTP-relative resolved '%s' + '%s' -> '%s'",
                                referrerPath.c_str(), spec.c_str(), resolved.c_str());
                }
                spec = resolved;
            }
        } else if (specIsRootAbs) {
            // Fallback: use global __NS_HTTP_ORIGIN__ if present to anchor root-absolute specs
            v8::Local<v8::String> key = ArgConverter::ConvertToV8String(isolate, "__NS_HTTP_ORIGIN__");
            v8::Local<v8::Object> global = context->Global();
            v8::MaybeLocal<v8::Value> maybeOriginVal = global->Get(context, key);
            v8::Local<v8::Value> originVal;
            if (!maybeOriginVal.IsEmpty() && maybeOriginVal.ToLocal(&originVal) && originVal->IsString()) {
                v8::String::Utf8Value o8(isolate, originVal);
                std::string origin = *o8 ? *o8 : "";
                if (!origin.empty() && (origin.rfind("http://", 0) == 0 || origin.rfind("https://", 0) == 0)) {
                    std::string refBase = origin;
                    if (refBase.back() != '/') refBase += '/';
                    std::string resolved = ResolveHttpRelative(refBase, spec);
                    if (!resolved.empty()) {
                        if (IsScriptLoadingLogEnabled()) {
                            DEBUG_WRITE("[http-esm][http-origin][fallback] origin=%s spec=%s -> %s", refBase.c_str(), spec.c_str(), resolved.c_str());
                        }
                        spec = resolved;
                    }
                }
            }
        }
    }

    // HTTP(S) ESM support: resolve, fetch and compile from dev server
    if (spec.rfind("http://", 0) == 0 || spec.rfind("https://", 0) == 0) {
        std::string canonical = tns::CanonicalizeHttpUrlKey(spec);
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][resolve] spec=%s canonical=%s", spec.c_str(), canonical.c_str());
        }
        auto it = g_moduleRegistry.find(canonical);
        if (it != g_moduleRegistry.end()) {
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][cache] hit %s", canonical.c_str());
            }
            return v8::MaybeLocal<v8::Module>(it->second.Get(isolate));
        }

        std::string body, ct;
        int status = 0;
        if (!tns::HttpFetchText(spec, body, ct, status)) {
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][fetch][fail] url=%s status=%d", spec.c_str(), status);
            }
            std::string msg = std::string("Failed to fetch ") + spec + ", status=" + std::to_string(status);
            isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, msg)));
            return v8::MaybeLocal<v8::Module>();
        }
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][fetch][ok] url=%s status=%d bytes=%lu ct=%s", spec.c_str(), status, (unsigned long)body.size(), ct.c_str());
        }

        v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, body);
        v8::Local<v8::String> urlString = ArgConverter::ConvertToV8String(isolate, canonical);
        v8::ScriptOrigin origin(isolate, urlString, 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true);
        v8::ScriptCompiler::Source src(sourceText, origin);
        v8::Local<v8::Module> mod;
        {
            v8::TryCatch tc(isolate);
            if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&mod)) {
                LogHttpCompileDiagnostics(isolate, context, canonical, body, tc);
                isolate->ThrowException(v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "HTTP module compile failed")));
                return v8::MaybeLocal<v8::Module>();
            }
        }
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][compile][ok] %s bytes=%lu", canonical.c_str(), (unsigned long)body.size());
        }
        // Register before instantiation to allow cyclic imports to resolve to same instance
        g_moduleRegistry[canonical].Reset(isolate, mod);
        // Do not evaluate here; allow V8 to handle instantiation/evaluation in importer context.
        // Instantiate proactively if desired (safe), but not required.
        // if (mod->GetStatus() == v8::Module::kUninstantiated) {
        //     if (!mod->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
        //         g_moduleRegistry.erase(canonical);
        //         return v8::MaybeLocal<v8::Module>();
        //     }
        // }
        // Let V8 evaluate during importer evaluation. Returning compiled module is fine.
        return v8::MaybeLocal<v8::Module>(mod);
    }

    // 2) Find which filepath the referrer was compiled under (local filesystem case)
    // referrerPath may already be set above; leave as-is if found.
    if (referrerPath.empty()) {
        for (auto& kv : g_moduleRegistry) {
            v8::Local<v8::Module> registered = kv.second.Get(isolate);
            if (registered == referrer) {
                referrerPath = kv.first;
                break;
            }
        }
    }

    // If we couldn't identify the referrer and the specifier is relative, 
    // assume the base directory is the application root
    bool specIsRelativeFs = !spec.empty() && spec[0] == '.';
    if (referrerPath.empty() && specIsRelativeFs) {
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
    if (IsScriptLoadingLogEnabled()) {
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
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// to appPath mapping: '%s' -> '%s'", tail.c_str(), candidate.c_str());
            }
        } else if (tail.rfind(androidAssetAppRoot, 0) == 0) {
            // Replace "/android_asset/app/" with the real appPath
            candidate = appPath + "/" + tail.substr(androidAssetAppRoot.size());
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// android_asset mapping: '%s' -> '%s'", tail.c_str(), candidate.c_str());
            }
        } else if (tail.rfind(appPath, 0) == 0) {
            // Already an absolute on-disk path to the app folder
            candidate = tail;
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("ResolveModuleCallback: file:// absolute path preserved: '%s'", candidate.c_str());
            }
        } else {
            // Fallback: treat as absolute on-disk path
            candidate = tail;
            if (IsScriptLoadingLogEnabled()) {
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
                                "  const encoded = encodeURIComponent(path).replace(/%2F/g, '/');\n"
                                "  return new URL('file://' + encoded);\n"
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
    if (IsScriptLoadingLogEnabled()) {
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
    if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("ResolveModuleCallback: Found cached module '%s'", absPath.c_str());
        }
        return v8::MaybeLocal<v8::Module>(it->second.Get(isolate));
    }

    // 9) Compile and register the new module
    if (IsScriptLoadingLogEnabled()) {
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
    
    if (IsScriptLoadingLogEnabled()) {
        DEBUG_WRITE("ImportModuleDynamicallyCallback: Dynamic import for '%s'", spec.c_str());
    }
    
    v8::EscapableHandleScope scope(isolate);

    // Create a Promise resolver we'll resolve/reject synchronously for now.
    v8::Local<v8::Promise::Resolver> resolver;
    if (!v8::Promise::Resolver::New(context).ToLocal(&resolver)) {
        // Failed to create resolver, return empty promise
        return v8::MaybeLocal<v8::Promise>();
    }

    // Resolve relative or root-absolute dynamic imports against the referrer's URL when provided
    auto isHttpLike = [](const std::string& s) -> bool {
        return s.rfind("http://", 0) == 0 || s.rfind("https://", 0) == 0;
    };
    bool specIsRelative = !spec.empty() && spec[0] == '.';
    bool specIsRootAbs = !spec.empty() && spec[0] == '/';
    std::string referrerUrl;
    if (!resource_name.IsEmpty() && resource_name->IsString()) {
        v8::String::Utf8Value r8(isolate, resource_name);
        referrerUrl = *r8 ? *r8 : "";
    }
    if ((specIsRelative || specIsRootAbs) && isHttpLike(referrerUrl)) {
        std::string resolved = ResolveHttpRelative(referrerUrl, spec);
        if (!resolved.empty()) {
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][dyn][http-rel] base=%s spec=%s -> %s", referrerUrl.c_str(), spec.c_str(), resolved.c_str());
            }
            spec = resolved;
        } else if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][dyn][http-rel][skip] base=%s spec=%s", referrerUrl.c_str(), spec.c_str());
        }
    }

    // Handle HTTP(S) dynamic import directly
    if (!spec.empty() && isHttpLike(spec)) {
        std::string canonical = tns::CanonicalizeHttpUrlKey(spec);
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("[http-esm][dyn][resolve] spec=%s canonical=%s", spec.c_str(), canonical.c_str());
        }
        v8::Local<v8::Module> mod;
        auto it = g_moduleRegistry.find(canonical);
        if (it != g_moduleRegistry.end()) {
            mod = it->second.Get(isolate);
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][dyn][cache] hit %s", canonical.c_str());
            }
        } else {
            std::string body, ct; int status = 0;
            if (!tns::HttpFetchText(spec, body, ct, status)) {
                if (IsScriptLoadingLogEnabled()) {
                    DEBUG_WRITE("[http-esm][dyn][fetch][fail] url=%s status=%d", spec.c_str(), status);
                }
                resolver->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, std::string("Failed to fetch ")+spec))).Check();
                return scope.Escape(resolver->GetPromise());
            }
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][dyn][fetch][ok] url=%s status=%d bytes=%lu ct=%s", spec.c_str(), status, (unsigned long)body.size(), ct.c_str());
            }
            v8::Local<v8::String> sourceText = ArgConverter::ConvertToV8String(isolate, body);
            v8::Local<v8::String> urlString = ArgConverter::ConvertToV8String(isolate, canonical);
            v8::ScriptOrigin origin(isolate, urlString, 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true);
            v8::ScriptCompiler::Source src(sourceText, origin);
            {
                v8::TryCatch tc(isolate);
                if (!v8::ScriptCompiler::CompileModule(isolate, &src).ToLocal(&mod)) {
                    LogHttpCompileDiagnostics(isolate, context, canonical, body, tc);
                    resolver->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "HTTP module compile failed"))).Check();
                    return scope.Escape(resolver->GetPromise());
                }
            }
            if (IsScriptLoadingLogEnabled()) {
                DEBUG_WRITE("[http-esm][dyn][compile][ok] %s bytes=%lu", canonical.c_str(), (unsigned long)body.size());
            }
            g_moduleRegistry[canonical].Reset(isolate, mod);
        }
        if (mod->GetStatus() == v8::Module::kUninstantiated) {
            if (!mod->InstantiateModule(context, &ResolveModuleCallback).FromMaybe(false)) {
                resolver->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "Instantiate failed"))).Check();
                return scope.Escape(resolver->GetPromise());
            }
        }
        if (mod->GetStatus() != v8::Module::kEvaluated) {
            if (mod->Evaluate(context).IsEmpty()) {
                resolver->Reject(context, v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "Evaluation failed"))).Check();
                return scope.Escape(resolver->GetPromise());
            }
        }
        resolver->Resolve(context, mod->GetModuleNamespace()).Check();
        return scope.Escape(resolver->GetPromise());
    }

    // Re-use the static resolver to locate / compile the module for non-HTTP cases.
    try {
        // Pass empty referrer since this V8 version doesn't expose GetModule() on
        // ScriptOrModule. The resolver will fall back to absolute-path heuristics.
        v8::Local<v8::Module> refMod;

        v8::MaybeLocal<v8::Module> maybeModule =
            ResolveModuleCallback(context, specifier, import_assertions, refMod);

        v8::Local<v8::Module> module;
        if (!maybeModule.ToLocal(&module)) {
            // Resolution failed; reject to avoid leaving a pending Promise (white screen)
            if (IsScriptLoadingLogEnabled()) {
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
                if (IsScriptLoadingLogEnabled()) {
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
                if (IsScriptLoadingLogEnabled()) {
                    DEBUG_WRITE("ImportModuleDynamicallyCallback: Evaluation failed for '%s'", spec.c_str());
                }
                v8::Local<v8::Value> ex =
                    v8::Exception::Error(ArgConverter::ConvertToV8String(isolate, "Evaluation failed"));
                resolver->Reject(context, ex).Check();
                return scope.Escape(resolver->GetPromise());
            }
        }

        resolver->Resolve(context, module->GetModuleNamespace()).Check();
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("ImportModuleDynamicallyCallback: Successfully resolved '%s'", spec.c_str());
        }
    } catch (NativeScriptException& ex) {
        ex.ReThrowToV8();
        if (IsScriptLoadingLogEnabled()) {
            DEBUG_WRITE("ImportModuleDynamicallyCallback: Native exception for '%s'", spec.c_str());
        }
        resolver
            ->Reject(context, v8::Exception::Error(
                                  ArgConverter::ConvertToV8String(isolate, "Native error during dynamic import")))
            .Check();
    }

    return scope.Escape(resolver->GetPromise());
}
