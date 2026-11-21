#include "WrapperLifetimeCanary.h"

#include <mutex>
#include <unordered_map>
#include <vector>

#include "EventLoop.h"
#include "MetadataNode.h"
#include "NativeScriptAssert.h"
#include "ObjectManager.h"
#include "Runtime.h"

using namespace v8;

namespace tns {

namespace {

Local<Value> DescriptorField(Isolate* isolate, Local<Context> context, Local<Object> descriptor,
                             const char* name) {
  Local<Value> value;
  return descriptor
                 ->Get(context, String::NewFromUtf8(isolate, name).ToLocalChecked())
                 .ToLocal(&value)
             ? value
             : Local<Value>(Undefined(isolate));
}

bool HasOwn(Local<Context> context, Local<Object> state, Local<Name> property) {
  return state->HasOwnProperty(context, property).FromMaybe(false);
}

// The object of `holder`'s prototype chain, `holder` included, with a real own
// `property`, interceptors skipped; empty if there is none.
Local<Object> OwnerOf(Local<Context> context, Local<Object> holder, Local<Name> property) {
  Local<Object> object = holder;
  while (!object->HasRealNamedProperty(context, property).FromMaybe(false)) {
    Local<Value> prototype = object->GetPrototype();
    if (!prototype->IsObject()) {
      return Local<Object>();
    }
    object = prototype.As<Object>();
  }
  return object;
}

ObjectManager* ObjectManagerOf(Local<Value> data) {
  return static_cast<ObjectManager*>(
      data.As<External>()->Value(v8::kExternalPointerTypeTagDefault));
}

// Mirrors a write of a name the state already has; false if the state lacks it.
bool RewriteState(ObjectManager* objectManager, ObjectManager::CanaryHandle* canary,
                  Local<Context> context, Local<Object> state, Local<Name> property,
                  Local<Value> value) {
  if (!objectManager->StateIsPlain(canary)) {
    if (!HasOwn(context, state, property)) {
      return false;
    }
    if (!state->HasRealNamedCallbackProperty(context, property).FromMaybe(true)) {
      (void)state->Set(context, property, value);
    }
    return true;
  }
  // A null-prototype object of data properties: one lookup answers both
  // whether it is own and whether it is writable.
  PropertyAttribute attributes;
  if (!state->GetRealNamedPropertyAttributes(context, property).To(&attributes)) {
    return false;
  }
  if ((attributes & ReadOnly) == 0) {
    (void)state->Set(context, property, value);
  }
  return true;
}

// The handlers mirror: each copies its write into the state and declines, so
// V8 also applies it to the canary, where reads find a real own property and
// never call into C++. There is no getter, query, enumerator or descriptor
// callback for the same reason. The handlers are masking, so they also see
// names the prototype chain has. After the setter declines, V8 adds a new name
// to the canary without calling the definer, so the setter itself decides what
// becomes state. V8 calls the setter only when the canary is the receiver.
Intercepted Setter(Local<Name> property, Local<Value> value,
                   const PropertyCallbackInfo<Boolean>& info) {
  ObjectManager* objectManager = ObjectManagerOf(info.Data());
  // v8::Object::DefineOwnProperty reaches the setter, not the definer.
  if (objectManager->CopyingCanaryProperties()) {
    return Intercepted::kNo;
  }
  Local<Object> holder = info.Holder();
  ObjectManager::CanaryHandle* canary = objectManager->CanaryOfInstance(holder);
  if (canary == nullptr) {
    return Intercepted::kNo;
  }
  objectManager->CountCanaryWrite();
  Isolate* isolate = info.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> state = objectManager->StateOf(canary);
  // Own on the state means own on the canary: a rewrite of a JS property.
  if (!state.IsEmpty() &&
      RewriteState(objectManager, canary, context, state, property, value)) {
    return Intercepted::kNo;
  }
  // Searches the prototype chain too: absent there means a new name.
  PropertyAttribute attributes;
  if (holder->GetRealNamedPropertyAttributes(context, property).To(&attributes)) {
    // V8 rejects a write to a read-only property and runs an accessor's setter:
    // a Java field's (an accessor on its class's prototype), a JS one's, or one
    // defined on the canary, which runs with the canary as `this` and so
    // mirrors whatever it writes. None is state.
    if ((attributes & ReadOnly) != 0) {
      return Intercepted::kNo;
    }
    Local<Object> owner = OwnerOf(context, holder, property);
    if (owner.IsEmpty()) {
      return Intercepted::kNo;
    }
    // A Java field's setter runs from here, sparing V8 a second lookup.
    if (owner != holder &&
        MetadataNode::TrySetInstanceField(isolate, context, objectManager->InstanceFieldTableKey(),
                                          owner, holder, property, value)) {
      return Intercepted::kYes;
    }
    Maybe<bool> accessor = owner->HasRealNamedCallbackProperty(context, property);
    if (accessor.IsNothing() || accessor.FromJust()) {
      return Intercepted::kNo;
    }
  }
  if (state.IsEmpty()) {
    state = objectManager->StateForWrite(canary);
    if (state.IsEmpty()) {
      return Intercepted::kNo;
    }
    (void)state->CreateDataProperty(context, property, value);
  } else if (objectManager->StateIsPlain(canary)) {
    // RewriteState found the name absent from the state.
    (void)state->CreateDataProperty(context, property, value);
  } else if (!HasOwn(context, state, property)) {
    (void)state->CreateDataProperty(context, property, value);
  } else if (!state->HasRealNamedCallbackProperty(context, property).FromMaybe(true)) {
    (void)state->Set(context, property, value);
  }
  return Intercepted::kNo;
}

Intercepted Deleter(Local<Name> property, const PropertyCallbackInfo<Boolean>& info) {
  ObjectManager* objectManager = ObjectManagerOf(info.Data());
  ObjectManager::CanaryHandle* canary = objectManager->CanaryOfInstance(info.Holder());
  if (canary == nullptr) {
    return Intercepted::kNo;
  }
  Local<Object> state = objectManager->StateOf(canary);
  if (!state.IsEmpty()) {
    (void)state->Delete(info.GetIsolate()->GetCurrentContext(), property);
  }
  return Intercepted::kNo;
}

// Object.defineProperty, define-semantics class fields and `super.x = v` on a
// receiver that lacks `x` reach here.
Intercepted Definer(Local<Name> property, const PropertyDescriptor& descriptor,
                    const PropertyCallbackInfo<Boolean>& info) {
  ObjectManager* objectManager = ObjectManagerOf(info.Data());
  if (objectManager->CopyingCanaryProperties()) {
    return Intercepted::kNo;
  }
  ObjectManager::CanaryHandle* canary = objectManager->CanaryOfInstance(info.Holder());
  if (canary == nullptr) {
    return Intercepted::kNo;
  }
  objectManager->CountCanaryWrite();
  Local<Object> state = objectManager->StateForWrite(canary);
  if (!state.IsEmpty()) {
    bool plain = descriptor.has_value() && descriptor.has_writable() && descriptor.writable() &&
                 descriptor.has_enumerable() && descriptor.enumerable() &&
                 descriptor.has_configurable() && descriptor.configurable();
    if (!plain) {
      objectManager->SetStateNotPlain(canary);
    }
    (void)state->DefineProperty(info.GetIsolate()->GetCurrentContext(), property,
                                const_cast<PropertyDescriptor&>(descriptor));
  }
  return Intercepted::kNo;
}

// Reports from WrapperLifetimeWatcher's thread for one runtime.
struct DeathInbox {
  std::vector<int> ids;
  std::weak_ptr<EventLoop> loop;
  bool drainPosted = false;
};

std::mutex inboxesMutex_;
std::unordered_map<int, DeathInbox> inboxes_;

void DrainOnRuntimeThread(int runtimeId) {
  Runtime* runtime = Runtime::GetCurrentRuntime();
  if (runtime == nullptr || runtime->GetId() != runtimeId) {
    return;
  }
  Isolate* isolate = runtime->GetIsolate();
  Locker locker(isolate);
  Isolate::Scope isolateScope(isolate);
  HandleScope handleScope(isolate);
  runtime->GetObjectManager()->ReleaseCollectedCanaryStates();
}

}  // namespace

void WrapperLifetimeCanary::ConfigureInstanceTemplate(Local<ObjectTemplate> instanceTemplate,
                                                      TemplateKind kind,
                                                      ObjectManager* objectManager) {
  if (!InScope(kind == TemplateKind::kExtended)) {
    return;
  }
  NS_CHECK(objectManager != nullptr);
  Isolate* isolate = Isolate::GetCurrent();
  instanceTemplate->SetHandler(NamedPropertyHandlerConfiguration(
      nullptr, Setter, nullptr, Deleter, nullptr, Definer, nullptr,
      External::New(isolate, objectManager, v8::kExternalPointerTypeTagDefault),
      PropertyHandlerFlags::kNone));
}

void WrapperLifetimeCanary::CopyOwnProperties(ObjectManager* objectManager,
                                              Local<Context> context, Local<Object> from,
                                              Local<Object> to, bool plain) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  TryCatch tc(isolate);
  bool wasCopying = objectManager->CopyingCanaryProperties();
  objectManager->SetCopyingCanaryProperties(true);
  Local<Array> names;
  if (from->GetPropertyNames(context, KeyCollectionMode::kOwnOnly, PropertyFilter::ALL_PROPERTIES,
                             IndexFilter::kSkipIndices, KeyConversionMode::kConvertToString)
          .ToLocal(&names)) {
    for (uint32_t i = 0; i < names->Length(); i++) {
      Local<Value> name;
      if (!names->Get(context, i).ToLocal(&name) || !name->IsName()) {
        continue;
      }
      Local<Name> key = name.As<Name>();
      if (plain) {
        Local<Value> value;
        if (from->Get(context, key).ToLocal(&value)) {
          (void)to->CreateDataProperty(context, key, value);
        }
        continue;
      }
      if (!from->HasRealNamedCallbackProperty(context, key).FromMaybe(true)) {
        PropertyAttribute attributes;
        Local<Value> value;
        if (from->GetRealNamedPropertyAttributes(context, key).To(&attributes) &&
            from->GetRealNamedProperty(context, key).ToLocal(&value)) {
          (void)to->DefineOwnProperty(context, key, value, attributes);
        }
        continue;
      }
      Local<Value> descriptorValue;
      if (!from->GetOwnPropertyDescriptor(context, key).ToLocal(&descriptorValue) ||
          !descriptorValue->IsObject()) {
        continue;
      }
      Local<Object> descriptor = descriptorValue.As<Object>();
      PropertyDescriptor accessor(DescriptorField(isolate, context, descriptor, "get"),
                                  DescriptorField(isolate, context, descriptor, "set"));
      accessor.set_enumerable(
          DescriptorField(isolate, context, descriptor, "enumerable")->BooleanValue(isolate));
      accessor.set_configurable(
          DescriptorField(isolate, context, descriptor, "configurable")->BooleanValue(isolate));
      (void)to->DefineProperty(context, key, accessor);
    }
  }
  objectManager->SetCopyingCanaryProperties(wasCopying);
}

void WrapperLifetimeCanary::OpenDeathInbox(int runtimeId, const std::shared_ptr<EventLoop>& loop) {
  std::lock_guard<std::mutex> lock(inboxesMutex_);
  inboxes_[runtimeId].loop = loop;
}

void WrapperLifetimeCanary::CloseDeathInbox(int runtimeId) {
  std::lock_guard<std::mutex> lock(inboxesMutex_);
  inboxes_.erase(runtimeId);
}

void WrapperLifetimeCanary::NotifyCollected(int runtimeId, const int* ids, int count) {
  std::shared_ptr<EventLoop> loop;
  {
    std::lock_guard<std::mutex> lock(inboxesMutex_);
    auto it = inboxes_.find(runtimeId);
    if (it == inboxes_.end()) {
      return;
    }
    DeathInbox& inbox = it->second;
    inbox.ids.insert(inbox.ids.end(), ids, ids + count);
    if (inbox.drainPosted) {
      return;
    }
    loop = inbox.loop.lock();
    inbox.drainPosted = loop != nullptr;
  }
  // Without a loop, or with the post dropped at shutdown, the next settle of
  // collected canaries drains the inbox.
  if (loop != nullptr) {
    loop->PostInternal([runtimeId]() { DrainOnRuntimeThread(runtimeId); });
  }
}

std::vector<int> WrapperLifetimeCanary::TakeCollected(int runtimeId) {
  std::vector<int> ids;
  std::lock_guard<std::mutex> lock(inboxesMutex_);
  auto it = inboxes_.find(runtimeId);
  if (it != inboxes_.end()) {
    ids.swap(it->second.ids);
    it->second.drainPosted = false;
  }
  return ids;
}

}  // namespace tns
