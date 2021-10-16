#include "src/objects/feedback-cell-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsFeedbackCell_NonInline(HeapObject o) {
  return o.IsFeedbackCell();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedFeedbackCell<FeedbackCell, Struct>::FeedbackCellVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::FeedbackCellVerify(FeedbackCell::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
