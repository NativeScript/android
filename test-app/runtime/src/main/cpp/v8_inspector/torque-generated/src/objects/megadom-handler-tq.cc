#include "src/objects/megadom-handler-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsMegaDomHandler_NonInline(HeapObject o) {
  return o.IsMegaDomHandler();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedMegaDomHandler<MegaDomHandler, HeapObject>::MegaDomHandlerVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::MegaDomHandlerVerify(MegaDomHandler::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
