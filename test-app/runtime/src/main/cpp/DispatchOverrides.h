#ifndef DISPATCHOVERRIDES_H_
#define DISPATCHOVERRIDES_H_

#include <string>
#include <vector>

#include "v8.h"

namespace tns {

/*
 * The methods one extend() call's implementation object defined when the class
 * was created, checked in debug builds against what each Java->JS call actually
 * dispatches to. Java->JS calls look the method up on the instance per call, so
 * an own property of the instance or a method replaced after extend() wins; the
 * iOS runtime binds methods at extend time and never sees either. A mismatch is
 * logged once per class and method.
 *
 * Release builds compile none of this: the type stays incomplete and every
 * pointer to it is null.
 */
class DispatchOverrides;

#ifdef APPLICATION_IN_DEBUG

class DispatchOverrides {
 public:
  // Reads only own data properties, so no getter of the object runs.
  DispatchOverrides(v8::Isolate* isolate, v8::Local<v8::Object> implementationObject,
                    std::string className);

  DispatchOverrides(const DispatchOverrides&) = delete;
  DispatchOverrides& operator=(const DispatchOverrides&) = delete;

  /*
   * For an instance whose class has no record: `new SomeInterface({...})` gives
   * each instance its own implementation object, its prototype, so only an own
   * property of the instance is reported. Same contract as Check.
   */
  static void CheckInstanceProperty(v8::Isolate* isolate, v8::Local<v8::Context> context,
                                    v8::Local<v8::Object> instance,
                                    v8::Local<v8::String> methodName, int methodNameId,
                                    v8::Local<v8::Function> method);

  // method is the function the call resolved to on instance. Never throws and
  // never runs JS: microtasks would drain before the dispatched method.
  void Check(v8::Isolate* isolate, v8::Local<v8::Context> context,
             v8::Local<v8::Object> instance, v8::Local<v8::String> methodName,
             int methodNameId, v8::Local<v8::Function> method) {
    if (methodNameId < 0) {
      return;
    }
    auto index = static_cast<size_t>(methodNameId);
    if (index < byNameId_.size()) {
      const auto& entry = byNameId_[index];
      if (entry.quiet || entry.expected == method) {
        return;
      }
    }
    CheckSlow(isolate, context, instance, methodName, index, method);
  }

 private:
  struct Captured {
    v8::Global<v8::String> name;
    // Empty when the property was an accessor or held no function.
    v8::Global<v8::Function> function;
  };

  // Indexed by the id Java's MethodNameIds gives the method name; ids are
  // handed out on Java's first call with a name, so entries are bound lazily.
  struct Bound {
    bool bound = false;
    // Warned already, or a name that is never checked.
    bool quiet = false;
    v8::Global<v8::Function> expected;
  };

  void CheckSlow(v8::Isolate* isolate, v8::Local<v8::Context> context,
                 v8::Local<v8::Object> instance, v8::Local<v8::String> methodName,
                 size_t index, v8::Local<v8::Function> method);

  // Whether JS code put name on the instance or its class after extend():
  // an own property of the instance, of its prototype, or of the
  // implementation object.
  bool DefinedAfterExtend(v8::Isolate* isolate, v8::Local<v8::Context> context,
                          v8::Local<v8::Object> instance, v8::Local<v8::String> name);

  static void Warn(const std::string& className, v8::Local<v8::String> methodName,
                   v8::Local<v8::Function> method);

  std::string className_;
  v8::Global<v8::Object> implementationObject_;
  std::vector<Captured> captured_;
  std::vector<Bound> byNameId_;
};

#endif  // APPLICATION_IN_DEBUG

}  // namespace tns

#endif  // DISPATCHOVERRIDES_H_
