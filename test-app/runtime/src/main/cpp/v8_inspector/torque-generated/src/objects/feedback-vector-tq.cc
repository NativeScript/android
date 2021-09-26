#include "src/objects/feedback-vector-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsFeedbackVector_NonInline(HeapObject o) {
  return o.IsFeedbackVector();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedFeedbackVector<FeedbackVector, HeapObject>::FeedbackVectorVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::FeedbackVectorVerify(FeedbackVector::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
