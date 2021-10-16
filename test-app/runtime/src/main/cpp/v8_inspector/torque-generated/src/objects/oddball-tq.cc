#include "src/objects/oddball-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsOddball_NonInline(HeapObject o) {
  return o.IsOddball();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedOddball<Oddball, PrimitiveHeapObject>::OddballVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::OddballVerify(Oddball::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
