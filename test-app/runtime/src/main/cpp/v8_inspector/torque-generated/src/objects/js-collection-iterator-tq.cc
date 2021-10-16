#include "src/objects/js-collection-iterator-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsJSCollectionIterator_NonInline(HeapObject o) {
  return o.IsJSCollectionIterator();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSCollectionIterator<JSCollectionIterator, JSObject>::JSCollectionIteratorVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSCollectionIteratorVerify(JSCollectionIterator::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
