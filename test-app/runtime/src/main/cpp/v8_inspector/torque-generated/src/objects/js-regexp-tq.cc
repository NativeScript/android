#include "src/objects/js-regexp-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsJSRegExp_NonInline(HeapObject o) {
  return o.IsJSRegExp();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSRegExp<JSRegExp, JSObject>::JSRegExpVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSRegExpVerify(JSRegExp::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
