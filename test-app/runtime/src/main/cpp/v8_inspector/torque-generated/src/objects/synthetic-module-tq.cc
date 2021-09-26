#include "src/objects/synthetic-module-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsSyntheticModule_NonInline(HeapObject o) {
  return o.IsSyntheticModule();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedSyntheticModule<SyntheticModule, Module>::SyntheticModuleVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::SyntheticModuleVerify(SyntheticModule::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
