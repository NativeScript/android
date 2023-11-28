//
//  ModuleBinding.cpp
//  NativeScript
//
//  Created by Eduardo Speroni on 5/11/23.
//  Copyright © 2023 Progress. All rights reserved.
//

#include "ModuleBinding.h"

// TODO: add here
//#define NSC_BUILTIN_STANDARD_BINDINGS(V)                                      \
//V(fs)

#define NSC_BUILTIN_STANDARD_BINDINGS(V)


#define NSC_BUILTIN_BINDINGS(V)                                               \
NSC_BUILTIN_STANDARD_BINDINGS(V)

// This is used to load built-in bindings. Instead of using
// __attribute__((constructor)), we call the _register_<modname>
// function for each built-in bindings explicitly in
// binding::RegisterBuiltinBindings(). This is only forward declaration.
// The definitions are in each binding's implementation when calling
// the NODE_BINDING_CONTEXT_AWARE_INTERNAL.
#define V(modname) void _register_##modname();
NSC_BUILTIN_BINDINGS(V)
#undef V




#define V(modname)                                                             \
  void _register_isolate_##modname(v8::Isolate* isolate,            \
                                   v8::Local<v8::FunctionTemplate> target);
NODE_BINDINGS_WITH_PER_ISOLATE_INIT(V)
#undef V



using v8::Context;
using v8::EscapableHandleScope;
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

namespace tns {
// Globals per process
static ns_module* modlist_internal;
static ns_module* modlist_linked;
static thread_local ns_module* thread_local_modpending;
bool node_is_initialized = false;

extern "C" void nativescript_module_register(void* m) {
    struct ns_module* mp = reinterpret_cast<struct ns_module*>(m);
    
    if (mp->nm_flags & NM_F_INTERNAL) {
        mp->nm_link = modlist_internal;
        modlist_internal = mp;
    } else if (!node_is_initialized) {
        // "Linked" modules are included as part of the node project.
        // Like builtins they are registered *before* node::Init runs.
        mp->nm_flags = NM_F_LINKED;
        mp->nm_link = modlist_linked;
        modlist_linked = mp;
    } else {
        thread_local_modpending = mp;
    }
}

namespace binding {

void RegisterBuiltinBindings() {
#define V(modname) _register_##modname();
    NSC_BUILTIN_BINDINGS(V)
#undef V
}

void CreateInternalBindingTemplates(v8::Isolate* isolate, Local<FunctionTemplate> templ) {
#define V(modname)                                                             \
  do {                                                                         \
    /*templ->InstanceTemplate()->SetInternalFieldCount(                          \
        BaseObject::kInternalFieldCount);*/                                      \
    _register_isolate_##modname(isolate, templ);                          \
    /*isolate_data->set_##modname##_binding(templ);*/                              \
  } while (0);
  NODE_BINDINGS_WITH_PER_ISOLATE_INIT(V)
#undef V
}

};

};

