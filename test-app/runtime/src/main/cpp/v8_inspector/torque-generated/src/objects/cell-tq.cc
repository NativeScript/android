#include "src/objects/cell-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsCell_NonInline(HeapObject o) {
  return o.IsCell();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedCell<Cell, HeapObject>::CellVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::CellVerify(Cell::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
