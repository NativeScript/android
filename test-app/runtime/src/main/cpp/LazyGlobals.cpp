#include "LazyGlobals.h"

#include "ArgConverter.h"
#include "Base64.h"
#include "BuiltinLoader.h"
#include "RuntimeState.h"
#include "TextEncoding.h"

using namespace v8;

namespace tns {

namespace {

using BindingFactory = Local<Object> (*)(Local<Context>);

struct LazyGlobalEntry {
    const char* name;
    BuiltinId builtin;
    const char* exportName;  // key of `name` in the builtin's module.exports
    BindingFactory binding;  // natives the builtin needs, null if it needs none
};

constexpr LazyGlobalEntry kLazyGlobals[] = {
        {"TextEncoder", BuiltinId::kTextEncoding, "TextEncoder",
         TextEncoding::CreateBinding},
        {"TextDecoder", BuiltinId::kTextEncoding, "TextDecoder",
         TextEncoding::CreateBinding},
        {"atob", BuiltinId::kBase64, "atob", Base64::CreateBinding},
        {"btoa", BuiltinId::kBase64, "btoa", Base64::CreateBinding},
};

// One entry per builtin this tier can run, so two globals from the same file
// cost one run.
struct LazyGlobalsState {
    v8::Global<Object> exports[static_cast<unsigned>(BuiltinId::kCount)];
};

MaybeLocal<Object> GetExports(Local<Context> context,
                              const LazyGlobalEntry& entry) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    auto* state = RuntimeState::For<LazyGlobalsState>(isolate);
    if (state == nullptr) {
        return MaybeLocal<Object>();
    }

    const unsigned index = static_cast<unsigned>(entry.builtin);
    if (!state->exports[index].IsEmpty()) {
        return state->exports[index].Get(isolate);
    }

    Local<Value> binding;
    if (entry.binding != nullptr) {
        binding = entry.binding(context);
    }

    Local<Value> result;
    if (!BuiltinLoader::RunBuiltin(context, entry.builtin, binding)
                 .ToLocal(&result) ||
        !result->IsObject()) {
        return MaybeLocal<Object>();
    }

    Local<Object> exports = result.As<Object>();
    state->exports[index].Reset(isolate, exports);
    return exports;
}

void LazyGlobalGetter(Local<v8::Name> property,
                      const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    const auto* entry = static_cast<const LazyGlobalEntry*>(
            info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));
    Local<Context> context = isolate->GetCurrentContext();

    Local<Object> exports;
    if (!GetExports(context, *entry).ToLocal(&exports)) {
        return;
    }
    Local<Value> value;
    if (!exports->Get(context,
                      ArgConverter::ConvertToV8String(isolate, entry->exportName))
                 .ToLocal(&value)) {
        return;
    }
    info.GetReturnValue().Set(value);
}

}  // namespace

void LazyGlobals::Init(Isolate* isolate, Local<ObjectTemplate> globalTemplate) {
    for (const LazyGlobalEntry& entry : kLazyGlobals) {
        Local<External> data =
                External::New(isolate, const_cast<LazyGlobalEntry*>(&entry),
                              v8::kExternalPointerTypeTagDefault);
        // SetLazyDataProperty, not a getter that rewrites the property itself:
        // defining over an API accessor reads its current descriptor, which
        // calls the getter again and recurses. V8 does the rewrite from the
        // outside, and gives a setter-less accessor its
        // ReconfigureToDataProperty setter, so an assignment landing before
        // the first read replaces the global as well.
        globalTemplate->SetLazyDataProperty(
                ArgConverter::ConvertToV8String(isolate, entry.name),
                LazyGlobalGetter, data, PropertyAttribute::DontEnum);
    }
}

}  // namespace tns
