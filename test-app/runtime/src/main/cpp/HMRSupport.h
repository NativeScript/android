// HMRSupport.h
#pragma once

#include <string>
#include <vector>
#include <v8.h>

namespace tns {

// import.meta.hot support
v8::Local<v8::Object> GetOrCreateHotData(v8::Isolate* isolate, const std::string& key);
void RegisterHotAccept(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb);
void RegisterHotDispose(v8::Isolate* isolate, const std::string& key, v8::Local<v8::Function> cb);
std::vector<v8::Local<v8::Function>> GetHotAcceptCallbacks(v8::Isolate* isolate, const std::string& key);
std::vector<v8::Local<v8::Function>> GetHotDisposeCallbacks(v8::Isolate* isolate, const std::string& key);
void InitializeImportMetaHot(v8::Isolate* isolate,
                             v8::Local<v8::Context> context,
                             v8::Local<v8::Object> importMeta,
                             const std::string& modulePath);

// Dev HTTP loader helpers
std::string CanonicalizeHttpUrlKey(const std::string& url);
bool HttpFetchText(const std::string& url, std::string& out, std::string& contentType, int& status);

} // namespace tns
