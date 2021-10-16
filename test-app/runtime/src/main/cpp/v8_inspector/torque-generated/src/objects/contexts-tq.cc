#include "src/objects/contexts-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsContext_NonInline(HeapObject o) {
  return o.IsContext();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedContext<Context, HeapObject>::ContextVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ContextVerify(Context::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
