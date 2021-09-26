#include "src/objects/primitive-heap-object-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsPrimitiveHeapObject_NonInline(HeapObject o) {
  return o.IsPrimitiveHeapObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedPrimitiveHeapObject<PrimitiveHeapObject, HeapObject>::PrimitiveHeapObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::PrimitiveHeapObjectVerify(PrimitiveHeapObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
