#include "LazyGlobals.h"

#include "ArgConverter.h"
#include "Base64.h"
#include "BuiltinLoader.h"
#include "Messaging.h"
#include "StructuredSerialization.h"
#include "TextEncoding.h"

using namespace v8;

namespace tns {

namespace {

/*
 * The builtin's `module.exports`, from the single run it gets per isolate
 * (BuiltinLoader::GetExports). Two globals out of the same file therefore
 * cost one run, and so does a module that exports the same interfaces.
 */
using ExportsAccessor = MaybeLocal<Object> (*)(Local<Context>);

struct LazyGlobalEntry {
    const char* name;
    const char* exportName;  // key of `name` in the builtin's module.exports
    ExportsAccessor exports;
};

/*
 * Exports accessor for a builtin with no natives of its own; modules with a
 * binding (TextEncoding, Base64) own a hand-written accessor instead.
 */
template <BuiltinId id>
MaybeLocal<Object> BuiltinExports(Local<Context> context) {
    return BuiltinLoader::GetExports(context, id, nullptr);
}

constexpr LazyGlobalEntry kLazyGlobals[] = {
        {"TextEncoder", "TextEncoder", TextEncoding::GetExports},
        {"TextDecoder", "TextDecoder", TextEncoding::GetExports},
        {"atob", "atob", Base64::GetExports},
        {"btoa", "btoa", Base64::GetExports},
        {"DOMException", "DOMException", serialization::GetDomExceptionExports},
        // events.js is an eager builtin (Events::Init), so this row never runs
        // a file: the read hits the exports cache and only the placement is
        // lazy.
        {"CustomEvent", "CustomEvent", BuiltinExports<BuiltinId::kEvents>},
        {"MessageEvent", "MessageEvent", BuiltinExports<BuiltinId::kMessageEvent>},
        {"MessagePort", "MessagePort", messaging::GetMessageChannelExports},
        {"MessageChannel", "MessageChannel", messaging::GetMessageChannelExports},
        {"BroadcastChannel", "BroadcastChannel", messaging::GetBroadcastChannelExports},
};

void LazyGlobalGetter(Local<v8::Name> property,
                      const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    const auto* entry = static_cast<const LazyGlobalEntry*>(
            info.Data().As<External>()->Value(v8::kExternalPointerTypeTagDefault));
    Local<Context> context = isolate->GetCurrentContext();

    Local<Object> exports;
    if (!entry->exports(context).ToLocal(&exports)) {
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
