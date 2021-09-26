#include "src/objects/heap-number-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsHeapNumber_NonInline(HeapObject o) {
  return o.IsHeapNumber();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedHeapNumber<HeapNumber, PrimitiveHeapObject>::HeapNumberVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::HeapNumberVerify(HeapNumber::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
