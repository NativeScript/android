#ifndef MODULE_INTERNAL_CALLBACKS_H
#define MODULE_INTERNAL_CALLBACKS_H

#include "v8.h"

// Module resolution callback for ES modules
v8::MaybeLocal<v8::Module> ResolveModuleCallback(v8::Local<v8::Context> context,
                                                 v8::Local<v8::String> specifier,
                                                 v8::Local<v8::FixedArray> import_assertions,
                                                 v8::Local<v8::Module> referrer);

// Helper functions
bool IsFile(const std::string& path);
std::string WithExtension(const std::string& path, const std::string& ext);
bool IsNodeBuiltinModule(const std::string& spec);
std::string GetApplicationPath();

#endif // MODULE_INTERNAL_CALLBACKS_H
