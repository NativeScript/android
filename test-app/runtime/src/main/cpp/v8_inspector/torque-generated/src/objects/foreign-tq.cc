#include "src/objects/foreign-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsForeign_NonInline(HeapObject o) {
  return o.IsForeign();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedForeign<Foreign, HeapObject>::ForeignVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ForeignVerify(Foreign::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
