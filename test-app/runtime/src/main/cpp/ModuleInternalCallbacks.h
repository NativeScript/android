#ifndef MODULE_INTERNAL_CALLBACKS_H
#define MODULE_INTERNAL_CALLBACKS_H

#include "v8.h"

// Module resolution callback for ES modules
v8::MaybeLocal<v8::Module> ResolveModuleCallback(v8::Local<v8::Context> context,
                                                 v8::Local<v8::String> specifier,
                                                 v8::Local<v8::FixedArray> import_assertions,
                                                 v8::Local<v8::Module> referrer);

// InitializeImportMetaObject - Callback invoked by V8 to initialize import.meta object
void InitializeImportMetaObject(v8::Local<v8::Context> context, 
                                v8::Local<v8::Module> module, 
                                v8::Local<v8::Object> meta);

// Helper functions
bool IsFile(const std::string& path);
std::string WithExtension(const std::string& path, const std::string& ext);
bool IsNodeBuiltinModule(const std::string& spec);
std::string GetApplicationPath();

#endif // MODULE_INTERNAL_CALLBACKS_H
