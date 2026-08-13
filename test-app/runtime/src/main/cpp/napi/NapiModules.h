#ifndef NAPIMODULES_H_
#define NAPIMODULES_H_

#include <string>

#include "v8.h"

namespace tns {

// The process-global table of Node-API addons registered through
// napi_module_register, and their per-env instantiation. Shaped after
// NsBuiltinModules so require() can consult both the same way.
class NapiModules {
 public:
  static bool IsRegistered(const std::string& name);

  // Returns the addon's exports for the context's env, initializing it on
  // first use and reusing that object afterwards. Empty on failure, with the
  // exception left pending on the isolate.
  static v8::MaybeLocal<v8::Object> GetExports(v8::Local<v8::Context> context,
                                               const std::string& name);

  // The name of the module registered on this thread since the last claim, or
  // empty. Node's dlopen does the same dance (modpending): a `.so` addon that
  // self-registers from a static constructor has no other way to tell the
  // loader which module the library it just opened contributed.
  static std::string ClaimPendingModule();
};

}  // namespace tns

#endif /* NAPIMODULES_H_ */
