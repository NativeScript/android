#ifndef WRAPPERLIFETIMECANARY_H_
#define WRAPPERLIFETIMECANARY_H_

#include <memory>
#include <vector>

#include "WrapperLifetime.h"
#include "v8.h"

namespace tns {

class EventLoop;
class ObjectManager;

// The canary wrapper lifetime mode (WrapperLifetimeMode::kCanary).
//
// A Java object's wrapper in scope is a canary: a phantom-weak JS object that
// dies with its JS references. A masking named interceptor with only a setter,
// deleter and definer mirrors every JS property written to a canary into a
// state object and then declines, so the property also lands on the canary and
// reads never leave V8. Writes that resolve to an inherited accessor (a Java
// field, a JS getter/setter) or a read-only property are not state. A canary
// built later for the same object gets the state's properties copied back.
// The state object is the source of truth.
//
// ObjectManager owns the records (state, and for JS-extended instances what a
// new canary needs: prototype, internal fields, implementation object). A
// record lives until Java collects its object, which
// com.tns.WrapperLifetimeWatcher reports from its own thread. It cannot be
// rooted only once the canary dies instead: a state reachable only through the
// canary is garbage in the same collection. The cost is that state referencing
// its own canary (a closure over `this`) keeps the canary, and through the
// Java object table its Java object, alive: the contract iOS documents too.
class WrapperLifetimeCanary {
 public:
  enum class TemplateKind {
    // Instances may be JS-extended (extend(), interface implementations).
    kExtended,
    // Instances are plain wrappers: canaries only under scope "all".
    kPlain,
  };

  static bool Enabled() {
    return GetWrapperLifetimeMode() == WrapperLifetimeMode::kCanary;
  }

  // Whether a wrapper linked as `extended` is a canary.
  static bool InScope(bool extended) {
    return Enabled() &&
           (extended || GetWrapperLifetimeScope() == WrapperLifetimeScope::kAll);
  }

  // Installs the write handlers on an instance template whose instances may be
  // canaries. No-op unless the mode is canary and kind is in scope.
  static void ConfigureInstanceTemplate(v8::Local<v8::ObjectTemplate> instanceTemplate,
                                        TemplateKind kind, ObjectManager* objectManager);

  // Copies `from`'s own string- and symbol-keyed properties onto `to`, with
  // their attributes, the handlers of both set aside. `plain` promises that
  // every property of `from` is a writable, enumerable, configurable data
  // property.
  static void CopyOwnProperties(ObjectManager* objectManager, v8::Local<v8::Context> context,
                                v8::Local<v8::Object> from, v8::Local<v8::Object> to,
                                bool plain);

  // Routes Java's reports of collected watched objects to runtimeId's thread.
  // Registered by the runtime's thread, before its first watched object.
  static void OpenDeathInbox(int runtimeId, const std::shared_ptr<EventLoop>& loop);
  static void CloseDeathInbox(int runtimeId);
  // Any thread.
  static void NotifyCollected(int runtimeId, const int* ids, int count);
  // The runtime's thread: the ids reported since the last call.
  static std::vector<int> TakeCollected(int runtimeId);
};

}  // namespace tns

#endif /* WRAPPERLIFETIMECANARY_H_ */
