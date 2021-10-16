#include "src/objects/js-function-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsJSFunctionOrBoundFunction_NonInline(HeapObject o) {
  return o.IsJSFunctionOrBoundFunction();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSFunctionOrBoundFunction<JSFunctionOrBoundFunction, JSObject>::JSFunctionOrBoundFunctionVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSFunctionOrBoundFunctionVerify(JSFunctionOrBoundFunction::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSBoundFunction_NonInline(HeapObject o) {
  return o.IsJSBoundFunction();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSBoundFunction<JSBoundFunction, JSFunctionOrBoundFunction>::JSBoundFunctionVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSBoundFunctionVerify(JSBoundFunction::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
