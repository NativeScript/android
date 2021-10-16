#include "src/objects/js-promise-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsJSPromise_NonInline(HeapObject o) {
  return o.IsJSPromise();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSPromise<JSPromise, JSObject>::JSPromiseVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSPromiseVerify(JSPromise::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
