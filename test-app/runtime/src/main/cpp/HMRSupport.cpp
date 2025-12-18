// HMRSupport.cpp
#include "HMRSupport.h"
#include "ArgConverter.h"
#include "JEnv.h"
#include <algorithm>
#include <cctype>
#include <jni.h>
#include <unordered_map>
#include <cstring>
#include <string>

namespace tns {

static inline bool StartsWith(const std::string& s, const char* prefix) {
  size_t n = strlen(prefix);
  return s.size() >= n && s.compare(0, n, prefix) == 0;
}

static inline bool EndsWith(const std::string& s, const char* suffix) {
  size_t n = strlen(suffix);
  return s.size() >= n && s.compare(s.size() - n, n, suffix) == 0;
}

// Per-module hot data and callbacks. Keyed by canonical module path (file path or URL).
static std::unordered_map<std::string, v8::Global<v8::Object>> g_hotData;
static std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>> g_hotAccept;
static std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>> g_hotDispose;

v8::Local<v8::Object> GetOrCreateHotData(v8::Isolate* isolate, const std::string& key) {
  auto it = g_hotData.find(key);
  if (it != g_hotData.end() && !it->second.IsEmpty()) {
    return it->second.Get(isolate);
  }
  v8::Local<v8::Object> obj = v8::Object::New(isolate);
  g_hotData[key].Reset(isolate, obj);
  return obj;
}

void RegisterHotAccept(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb) {
  if (cb.IsEmpty()) return;
  g_hotAccept[key].emplace_back(v8::Global<v8::Function>(isolate, cb));
}

void RegisterHotDispose(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb) {
  if (cb.IsEmpty()) return;
  g_hotDispose[key].emplace_back(v8::Global<v8::Function>(isolate, cb));
}

std::vector<v8::Local<v8::Function>> GetHotAcceptCallbacks(v8::Isolate* isolate, const std::string& key) {
  std::vector<v8::Local<v8::Function>> out;
  auto it = g_hotAccept.find(key);
  if (it != g_hotAccept.end()) {
    for (auto& gfn : it->second) {
      if (!gfn.IsEmpty()) out.push_back(gfn.Get(isolate));
    }
  }
  return out;
}

std::vector<v8::Local<v8::Function>> GetHotDisposeCallbacks(v8::Isolate* isolate, const std::string& key) {
  std::vector<v8::Local<v8::Function>> out;
  auto it = g_hotDispose.find(key);
  if (it != g_hotDispose.end()) {
    for (auto& gfn : it->second) {
      if (!gfn.IsEmpty()) out.push_back(gfn.Get(isolate));
    }
  }
  return out;
}

void InitializeImportMetaHot(v8::Isolate* isolate,
                             v8::Local<v8::Context> context,
                             v8::Local<v8::Object> importMeta,
                             const std::string& modulePath) {
  using v8::Function;
  using v8::FunctionCallbackInfo;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  v8::HandleScope scope(isolate);

  // Canonicalize key to ensure per-module hot.data persists across HMR URLs.
  // Important: this must NOT affect the HTTP loader cache key; otherwise HMR fetches
  // can collapse onto an already-evaluated module and no update occurs.
  auto canonicalHotKey = [&](const std::string& in) -> std::string {
    // Some loaders wrap HTTP module URLs as file://http(s)://...
    std::string s = in;
    if (StartsWith(s, "file://http://") || StartsWith(s, "file://https://")) {
      s = s.substr(strlen("file://"));
    }

    // Drop fragment
    size_t hashPos = s.find('#');
    if (hashPos != std::string::npos) {
      s = s.substr(0, hashPos);
    }

    // Drop query (we want hot key stability)
    size_t qPos = s.find('?');
    std::string noQuery = (qPos == std::string::npos) ? s : s.substr(0, qPos);

    // If it's an http(s) URL, normalize only the path portion below.
    size_t schemePos = noQuery.find("://");
    size_t pathStart = (schemePos == std::string::npos) ? 0 : noQuery.find('/', schemePos + 3);
    if (pathStart == std::string::npos) {
      // No path; return without query
      return noQuery;
    }

    std::string origin = noQuery.substr(0, pathStart);
    std::string path = noQuery.substr(pathStart);

    // Normalize NS HMR virtual module paths:
    // /ns/m/__ns_hmr__/<token>/<rest> -> /ns/m/<rest>
    const char* hmrPrefix = "/ns/m/__ns_hmr__/";
    size_t hmrLen = strlen(hmrPrefix);
    if (path.compare(0, hmrLen, hmrPrefix) == 0) {
      size_t nextSlash = path.find('/', hmrLen);
      if (nextSlash != std::string::npos) {
        path = std::string("/ns/m/") + path.substr(nextSlash + 1);
      }
    }

    // Normalize common script extensions so `/foo` and `/foo.ts` share hot.data.
    const char* exts[] = {".ts", ".tsx", ".js", ".jsx", ".mjs", ".cjs"};
    for (auto ext : exts) {
      if (EndsWith(path, ext)) {
        path = path.substr(0, path.size() - strlen(ext));
        break;
      }
    }

    // Also drop `.vue`? No — SFC endpoints should stay distinct.
    return origin + path;
  };

  const std::string key = canonicalHotKey(modulePath);

  auto makeKeyData = [&](const std::string& key) -> Local<Value> {
    return ArgConverter::ConvertToV8String(isolate, key);
  };

  auto acceptCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    Local<Value> data = info.Data();
    std::string key;
    if (!data.IsEmpty()) {
      v8::String::Utf8Value s(iso, data);
      key = *s ? *s : "";
    }
    v8::Local<v8::Function> cb;
    if (info.Length() >= 1 && info[0]->IsFunction()) {
      cb = info[0].As<v8::Function>();
    } else if (info.Length() >= 2 && info[1]->IsFunction()) {
      cb = info[1].As<v8::Function>();
    }
    if (!cb.IsEmpty()) {
      RegisterHotAccept(iso, key, cb);
    }
    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  auto disposeCb = [](const FunctionCallbackInfo<Value>& info) {
    v8::Isolate* iso = info.GetIsolate();
    Local<Value> data = info.Data();
    std::string key;
    if (!data.IsEmpty()) { v8::String::Utf8Value s(iso, data); key = *s ? *s : ""; }
    if (info.Length() >= 1 && info[0]->IsFunction()) {
      RegisterHotDispose(iso, key, info[0].As<v8::Function>());
    }
    info.GetReturnValue().Set(v8::Undefined(iso));
  };

  auto declineCb = [](const FunctionCallbackInfo<Value>& info) {
    info.GetReturnValue().Set(v8::Undefined(info.GetIsolate()));
  };

  auto invalidateCb = [](const FunctionCallbackInfo<Value>& info) {
    info.GetReturnValue().Set(v8::Undefined(info.GetIsolate()));
  };

  Local<Object> hot = Object::New(isolate);
  hot->CreateDataProperty(context, ArgConverter::ConvertToV8String(isolate, "data"),
                GetOrCreateHotData(isolate, key)).Check();
  hot->CreateDataProperty(context, ArgConverter::ConvertToV8String(isolate, "prune"),
                          v8::Boolean::New(isolate, false)).Check();
  hot->CreateDataProperty(
      context, ArgConverter::ConvertToV8String(isolate, "accept"),
      v8::Function::New(context, acceptCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
      context, ArgConverter::ConvertToV8String(isolate, "dispose"),
      v8::Function::New(context, disposeCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
      context, ArgConverter::ConvertToV8String(isolate, "decline"),
      v8::Function::New(context, declineCb, makeKeyData(key)).ToLocalChecked()).Check();
  hot->CreateDataProperty(
      context, ArgConverter::ConvertToV8String(isolate, "invalidate"),
      v8::Function::New(context, invalidateCb, makeKeyData(key)).ToLocalChecked()).Check();

  importMeta->CreateDataProperty(context, ArgConverter::ConvertToV8String(isolate, "hot"), hot).Check();
}

// Drop fragments and normalize parameters for consistent registry keys.
std::string CanonicalizeHttpUrlKey(const std::string& url) {
  // Some loaders wrap HTTP module URLs as file://http(s)://...
  std::string normalizedUrl = url;
  if (StartsWith(normalizedUrl, "file://http://") || StartsWith(normalizedUrl, "file://https://")) {
    normalizedUrl = normalizedUrl.substr(strlen("file://"));
  }
  if (!(StartsWith(normalizedUrl, "http://") || StartsWith(normalizedUrl, "https://"))) {
    return normalizedUrl;
  }
  // Remove fragment
  size_t hashPos = normalizedUrl.find('#');
  std::string noHash = (hashPos == std::string::npos) ? normalizedUrl : normalizedUrl.substr(0, hashPos);

  // Split into origin+path and query
  size_t qPos = noHash.find('?');
  std::string originAndPath = (qPos == std::string::npos) ? noHash : noHash.substr(0, qPos);
  std::string query = (qPos == std::string::npos) ? std::string() : noHash.substr(qPos + 1);

  // Normalize bridge endpoints to keep a single realm across HMR updates:
  // - /ns/rt/<ver>    -> /ns/rt
  // - /ns/core/<ver>  -> /ns/core
  size_t schemePos = originAndPath.find("://");
  size_t pathStart = (schemePos == std::string::npos) ? std::string::npos : originAndPath.find('/', schemePos + 3);
  if (pathStart == std::string::npos) {
    // No path; preserve query as-is (fragment already removed).
    return noHash;
  }
  {
    std::string pathOnly = originAndPath.substr(pathStart);
      auto normalizeBridge = [&](const char* needle) {
        size_t nlen = strlen(needle);
        if (pathOnly.size() <= nlen) return false;
        if (pathOnly.compare(0, nlen, needle) != 0) return false;
        if (pathOnly.size() == nlen) return true;
        if (pathOnly[nlen] != '/') return false;
        size_t i = nlen + 1;
        size_t j = i;
        while (j < pathOnly.size() && isdigit((unsigned char)pathOnly[j])) j++;
        // Only normalize exact version segment: /ns/*/<digits> (no further segments)
        if (j == i) return false;
        if (j != pathOnly.size()) return false;
        originAndPath = originAndPath.substr(0, pathStart) + std::string(needle);
        return true;
      };
      if (!normalizeBridge("/ns/rt")) {
        normalizeBridge("/ns/core");
      }
  }

  // IMPORTANT: This function is used as an HTTP module registry/cache key.
  // For general-purpose HTTP module loading (public internet), the query string
  // can be part of the module's identity (auth, content versioning, routing, etc).
  // Therefore we only apply query normalization (sorting/dropping) for known
  // NativeScript dev endpoints where `t`/`v`/`import` are purely cache busters.
  {
    std::string pathOnly = originAndPath.substr(pathStart);
    const bool isDevEndpoint =
        StartsWith(pathOnly, "/ns/") ||
        StartsWith(pathOnly, "/node_modules/.vite/") ||
        StartsWith(pathOnly, "/@id/") ||
        StartsWith(pathOnly, "/@fs/");
    if (!isDevEndpoint) {
      // Preserve query as-is (fragment already removed).
      return noHash;
    }
  }

  if (query.empty()) return originAndPath;

  // Strip ?import markers / cache busters and sort remaining query params for stability
  std::vector<std::string> kept;
  size_t start = 0;
  while (start <= query.size()) {
    size_t amp = query.find('&', start);
    std::string pair = (amp == std::string::npos) ? query.substr(start) : query.substr(start, amp - start);
    if (!pair.empty()) {
      size_t eq = pair.find('=');
      std::string name = (eq == std::string::npos) ? pair : pair.substr(0, eq);
      if (!(name == "import" || name == "t" || name == "v")) kept.push_back(pair);
    }
    if (amp == std::string::npos) break;
    start = amp + 1;
  }
  if (kept.empty()) return originAndPath;
  std::sort(kept.begin(), kept.end());
  std::string rebuilt = originAndPath + "?";
  for (size_t i = 0; i < kept.size(); i++) {
    if (i > 0) rebuilt += "&";
    rebuilt += kept[i];
  }
  return rebuilt;
}

// Minimal HTTP fetch using java.net.* via JNI. Returns true on success (2xx) and non-empty body.
bool HttpFetchText(const std::string& url, std::string& out, std::string& contentType, int& status) {
  out.clear();
  contentType.clear();
  status = 0;
  try {
    JEnv env;

    // Allow network operations on the current thread (dev-only HMR path)
    // Some Android environments enforce StrictMode which throws NetworkOnMainThreadException
    // when performing network I/O on the main thread. Since this fetch runs on the JS/V8 thread
    // during development, explicitly relax the policy here.
    {
      jclass clsStrict = env.FindClass("android/os/StrictMode");
      jclass clsPolicyBuilder = env.FindClass("android/os/StrictMode$ThreadPolicy$Builder");
      if (clsStrict && clsPolicyBuilder) {
        jmethodID builderCtor = env.GetMethodID(clsPolicyBuilder, "<init>", "()V");
        jobject builder = env.NewObject(clsPolicyBuilder, builderCtor);
        if (builder) {
          jmethodID permitAll = env.GetMethodID(clsPolicyBuilder, "permitAll", "()Landroid/os/StrictMode$ThreadPolicy$Builder;");
          jobject builder2 = permitAll ? env.CallObjectMethod(builder, permitAll) : builder;
          jmethodID build = env.GetMethodID(clsPolicyBuilder, "build", "()Landroid/os/StrictMode$ThreadPolicy;");
          jobject policy = build ? env.CallObjectMethod(builder2 ? builder2 : builder, build) : nullptr;
          if (policy) {
            jmethodID setThreadPolicy = env.GetStaticMethodID(clsStrict, "setThreadPolicy", "(Landroid/os/StrictMode$ThreadPolicy;)V");
            if (setThreadPolicy) {
              env.CallStaticVoidMethod(clsStrict, setThreadPolicy, policy);
            }
          }
        }
      }
    }

    jclass clsURL = env.FindClass("java/net/URL");
    if (!clsURL) return false;
    jmethodID urlCtor = env.GetMethodID(clsURL, "<init>", "(Ljava/lang/String;)V");
    jmethodID openConnection = env.GetMethodID(clsURL, "openConnection", "()Ljava/net/URLConnection;");
    jstring jUrlStr = env.NewStringUTF(url.c_str());
    jobject urlObj = env.NewObject(clsURL, urlCtor, jUrlStr);

  jobject conn = env.CallObjectMethod(urlObj, openConnection);
    if (!conn) return false;

    jclass clsConn = env.GetObjectClass(conn);
    jmethodID setConnectTimeout = env.GetMethodID(clsConn, "setConnectTimeout", "(I)V");
    jmethodID setReadTimeout = env.GetMethodID(clsConn, "setReadTimeout", "(I)V");
    jmethodID setDoInput = env.GetMethodID(clsConn, "setDoInput", "(Z)V");
    jmethodID setUseCaches = env.GetMethodID(clsConn, "setUseCaches", "(Z)V");
    jmethodID setReqProp = env.GetMethodID(clsConn, "setRequestProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
  env.CallVoidMethod(conn, setConnectTimeout, 15000);
  env.CallVoidMethod(conn, setReadTimeout, 15000);
  if (setDoInput) { env.CallVoidMethod(conn, setDoInput, JNI_TRUE); }
  if (setUseCaches) { env.CallVoidMethod(conn, setUseCaches, JNI_FALSE); }
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Accept"), env.NewStringUTF("application/javascript, text/javascript, */*;q=0.1"));
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Accept-Encoding"), env.NewStringUTF("identity"));
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Cache-Control"), env.NewStringUTF("no-cache"));
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("Connection"), env.NewStringUTF("close"));
  env.CallVoidMethod(conn, setReqProp, env.NewStringUTF("User-Agent"), env.NewStringUTF("NativeScript-HTTP-ESM"));

    // Try to get status via HttpURLConnection if possible
    jclass clsHttp = env.FindClass("java/net/HttpURLConnection");
    bool isHttp = clsHttp && env.IsInstanceOf(conn, clsHttp);
    jmethodID getResponseCode = isHttp ? env.GetMethodID(clsHttp, "getResponseCode", "()I") : nullptr;
    jmethodID getErrorStream = isHttp ? env.GetMethodID(clsHttp, "getErrorStream", "()Ljava/io/InputStream;") : nullptr;
    if (isHttp && getResponseCode) {
      status = env.CallIntMethod(conn, getResponseCode);
    }

    // Read InputStream (prefer error stream on HTTP error codes)
    jmethodID getInputStream = env.GetMethodID(clsConn, "getInputStream", "()Ljava/io/InputStream;");
    jobject inStream = nullptr;
    if (isHttp && status >= 400 && getErrorStream) {
      inStream = env.CallObjectMethod(conn, getErrorStream);
    }
    if (!inStream) {
      inStream = env.CallObjectMethod(conn, getInputStream);
    }
    if (!inStream) return false;

    jclass clsIS = env.GetObjectClass(inStream);
    jmethodID readMethod = env.GetMethodID(clsIS, "read", "([B)I");
    jmethodID closeIS = env.GetMethodID(clsIS, "close", "()V");

    jclass clsBAOS = env.FindClass("java/io/ByteArrayOutputStream");
    jmethodID baosCtor = env.GetMethodID(clsBAOS, "<init>", "()V");
    jmethodID baosWrite = env.GetMethodID(clsBAOS, "write", "([BII)V");
    jmethodID baosToByteArray = env.GetMethodID(clsBAOS, "toByteArray", "()[B");
    jmethodID baosClose = env.GetMethodID(clsBAOS, "close", "()V");
    jobject baos = env.NewObject(clsBAOS, baosCtor);

    jbyteArray buffer = env.NewByteArray(8192);
    while (true) {
      jint n = env.CallIntMethod(inStream, readMethod, buffer);
      if (n < 0) break; // -1 indicates EOF
      if (n == 0) {
        // Defensive: continue reading if zero bytes returned
        continue;
      }
      env.CallVoidMethod(baos, baosWrite, buffer, 0, n);
    }

    env.CallVoidMethod(inStream, closeIS);
    jbyteArray bytes = (jbyteArray) env.CallObjectMethod(baos, baosToByteArray);
    env.CallVoidMethod(baos, baosClose);

    if (!bytes) return false;
    jsize len = env.GetArrayLength(bytes);
    out.resize(static_cast<size_t>(len));
    if (len > 0) {
      env.GetByteArrayRegion(bytes, 0, len, reinterpret_cast<jbyte*>(&out[0]));
    }

    // Content-Type if available
    jmethodID getContentType = env.GetMethodID(clsConn, "getContentType", "()Ljava/lang/String;");
    jstring jct = (jstring) env.CallObjectMethod(conn, getContentType);
    if (jct) {
      contentType = ArgConverter::jstringToString(jct);
    }

    if (status == 0) status = 200; // assume OK if not HTTP
    return status >= 200 && status < 300 && !out.empty();
  } catch (...) {
    return false;
  }
}

} // namespace tns
