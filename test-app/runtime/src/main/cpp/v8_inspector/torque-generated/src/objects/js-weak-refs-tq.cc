#include "src/objects/js-weak-refs-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsWeakCell_NonInline(HeapObject o) {
  return o.IsWeakCell();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWeakCell<WeakCell, HeapObject>::WeakCellVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WeakCellVerify(WeakCell::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSWeakRef_NonInline(HeapObject o) {
  return o.IsJSWeakRef();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSWeakRef<JSWeakRef, JSObject>::JSWeakRefVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSWeakRefVerify(JSWeakRef::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
