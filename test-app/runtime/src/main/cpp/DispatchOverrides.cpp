#include "DispatchOverrides.h"

#ifdef APPLICATION_IN_DEBUG

#include <android/log.h>

#include <algorithm>
#include <set>
#include <sstream>
#include <utility>

#include "ArgConverter.h"
#include "ObjectManager.h"
#include "RuntimeState.h"

using namespace v8;

namespace tns {

DispatchOverrides::DispatchOverrides(Isolate* isolate, Local<Object> implementationObject,
                                     std::string className)
    : className_(std::move(className)), implementationObject_(isolate, implementationObject) {
  auto context = isolate->GetCurrentContext();
  TryCatch tc(isolate);

  Local<Array> names;
  if (!implementationObject
           ->GetOwnPropertyNames(context, PropertyFilter::SKIP_SYMBOLS)
           .ToLocal(&names)) {
    return;
  }

  auto valueKey = ArgConverter::ConvertToV8String(isolate, std::string("value"));
  uint32_t length = names->Length();
  captured_.reserve(length);
  for (uint32_t i = 0; i < length; i++) {
    Local<Value> name;
    if (!names->Get(context, i).ToLocal(&name) || !name->IsString()) {
      continue;
    }

    Captured entry;
    entry.name.Reset(isolate, name.As<String>());

    Local<Value> descriptor;
    Local<Value> value;
    if (implementationObject->GetOwnPropertyDescriptor(context, name.As<String>())
            .ToLocal(&descriptor) &&
        descriptor->IsObject() &&
        descriptor.As<Object>()->Get(context, valueKey).ToLocal(&value) &&
        value->IsFunction()) {
      entry.function.Reset(isolate, value.As<Function>());
    }

    captured_.push_back(std::move(entry));
  }
}

void DispatchOverrides::CheckSlow(Isolate* isolate, Local<Context> context,
                                  Local<Object> instance, Local<String> methodName,
                                  size_t index, Local<Function> method) {
  if (index >= byNameId_.size()) {
    byNameId_.resize(index + 1);
  }
  auto& entry = byNameId_[index];

  if (!entry.bound) {
    entry.bound = true;

    const Captured* captured = nullptr;
    for (const auto& candidate : captured_) {
      if (candidate.name.Get(isolate)->StringEquals(methodName)) {
        captured = &candidate;
        break;
      }
    }

    if (captured != nullptr) {
      if (captured->function.IsEmpty()) {
        entry.quiet = true;
        return;
      }
      entry.expected.Reset(isolate, captured->function);
      if (entry.expected == method) {
        return;
      }
    } else if (!DefinedAfterExtend(isolate, context, instance, methodName)) {
      // The class does not implement the method, so the call resolved to
      // whatever it inherits.
      entry.expected.Reset(isolate, method);
      return;
    }
  }

  entry.quiet = true;
  Warn(className_, methodName, method);
}

bool DispatchOverrides::DefinedAfterExtend(Isolate* isolate, Local<Context> context,
                                           Local<Object> instance, Local<String> name) {
  // HasRealNamedProperty runs neither interceptors nor getters.
  if (instance->HasRealNamedProperty(context, name).FromMaybe(false)) {
    return true;
  }

  auto prototype = instance->GetPrototype();
  if (prototype->IsObject() &&
      prototype.As<Object>()->HasRealNamedProperty(context, name).FromMaybe(false)) {
    return true;
  }

  return implementationObject_.Get(isolate)->HasRealNamedProperty(context, name).FromMaybe(false);
}

namespace {
// Keyed by jclass: an instance holds its class's global ref from the class cache,
// which has one per class.
struct InstancePropertyWarnings {
  std::set<std::pair<jclass, int>> warned;
};
}  // namespace

void DispatchOverrides::CheckInstanceProperty(Isolate* isolate, Local<Context> context,
                                              Local<Object> instance, Local<String> methodName,
                                              int methodNameId, Local<Function> method) {
  if (methodNameId < 0 ||
      !instance->HasRealNamedProperty(context, methodName).FromMaybe(false)) {
    return;
  }

  auto* runtime = Runtime::TryGetRuntime(isolate);
  if (runtime == nullptr) {
    return;
  }
  auto* warnings = RuntimeState::For<InstancePropertyWarnings>(runtime);
  if (warnings == nullptr) {
    return;
  }

  jclass clazz = runtime->GetObjectManager()->GetJavaClass(instance);
  if (!warnings->warned.emplace(clazz, methodNameId).second) {
    return;
  }

  std::string className = clazz != nullptr ? ObjectManager::GetClassName(clazz) : "a Java object";
  std::replace(className.begin(), className.end(), '/', '.');
  Warn(className, methodName, method);
}

void DispatchOverrides::Warn(const std::string& className, Local<String> methodName,
                             Local<Function> method) {
  std::stringstream ss;
  ss << "Java called \"" << ArgConverter::ConvertToString(methodName) << "\" on " << className
     << " through a function other than the class's own implementation (an instance property "
        "or a method replaced after extend)";

  int line = method->GetScriptLineNumber();
  if (line != Function::kLineOffsetNotFound) {
    auto resourceName = method->GetScriptOrigin().ResourceName();
    ss << " defined at "
       << (resourceName->IsString() ? ArgConverter::ConvertToString(resourceName.As<String>())
                                    : std::string("<anonymous>"))
       << ":" << line + 1 << ":" << method->GetScriptColumnNumber() + 1;
  }

  ss << ". This may stop working in a future version, as on iOS. Define it on the class. "
        "Please report your use case.";

  __android_log_write(ANDROID_LOG_WARN, "JS", ss.str().c_str());
}

}  // namespace tns

#endif  // APPLICATION_IN_DEBUG
