//
//  ModuleBinding.hpp
//  NativeScript
//
//  Created by Eduardo Speroni on 5/11/23.
//  Copyright © 2023 Progress. All rights reserved.
//

#ifndef ModuleBinding_hpp
#define ModuleBinding_hpp

#include "v8.h"


namespace tns {

#define NODE_BINDING_CONTEXT_AWARE_CPP(modname, regfunc, priv, flags)          \
  static tns::ns_module _module = {                                         \
      NODE_MODULE_VERSION,                                                     \
      flags,                                                                   \
      nullptr,                                                                 \
      __FILE__,                                                                \
      nullptr,                                                                 \
      (tns::addon_context_register_func)(regfunc),                            \
      NODE_STRINGIFY(modname),                                                 \
      priv,                                                                    \
      nullptr};                                                                \
  void _register_##modname() { node_module_register(&_module); }

#define NODE_BINDING_CONTEXT_AWARE_INTERNAL(modname, regfunc)                  \
  NODE_BINDING_CONTEXT_AWARE_CPP(modname, regfunc, nullptr, NM_F_INTERNAL)








#define NODE_BINDING_PER_ISOLATE_INIT(modname, per_isolate_func)               \
  void _register_isolate_##modname(v8::Isolate* isolate,            \
                                   v8::Local<v8::FunctionTemplate> target) {   \
    per_isolate_func(isolate, target);                                    \
  }


// This is a helepr that gives the target as an ObjectTemplate (using target.PrototypeTemplate())

#define NODE_BINDING_PER_ISOLATE_INIT_OBJ(modname, per_isolate_func)               \
  void _register_isolate_##modname(v8::Isolate* isolate,            \
                                   v8::Local<v8::FunctionTemplate> target) {   \
    per_isolate_func(isolate, target->PrototypeTemplate());                                    \
  }


#define NODE_BINDINGS_WITH_PER_ISOLATE_INIT(V) \
V(timers)

enum {
    NM_F_BUILTIN = 1 << 0,  // Unused.
    NM_F_LINKED = 1 << 1,
    NM_F_INTERNAL = 1 << 2,
    NM_F_DELETEME = 1 << 3,
};

typedef void (*addon_register_func)(
                                    v8::Local<v8::Object> exports,
                                    v8::Local<v8::Value> module,
                                    void* priv);

typedef void (*addon_context_register_func)(
                                            v8::Local<v8::Object> exports,
                                            v8::Local<v8::Value> module,
                                            v8::Local<v8::Context> context,
                                            void* priv);

struct ns_module {
    int nm_version;
    unsigned int nm_flags;
    void* nm_dso_handle;
    const char* nm_filename;
    tns::addon_register_func nm_register_func;
    tns::addon_context_register_func nm_context_register_func;
    const char* nm_modname;
    void* nm_priv;
    struct ns_module* nm_link;
};

namespace binding {
void RegisterBuiltinBindings();
void CreateInternalBindingTemplates(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> templ);
};


};


#endif /* ModuleBinding_hpp */
