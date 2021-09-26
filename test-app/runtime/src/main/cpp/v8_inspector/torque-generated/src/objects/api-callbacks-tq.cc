#include "src/objects/api-callbacks-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsCallHandlerInfo_NonInline(HeapObject o) {
  return o.IsCallHandlerInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedCallHandlerInfo<CallHandlerInfo, Struct>::CallHandlerInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::CallHandlerInfoVerify(CallHandlerInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsInterceptorInfo_NonInline(HeapObject o) {
  return o.IsInterceptorInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedInterceptorInfo<InterceptorInfo, Struct>::InterceptorInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::InterceptorInfoVerify(InterceptorInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsAccessCheckInfo_NonInline(HeapObject o) {
  return o.IsAccessCheckInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedAccessCheckInfo<AccessCheckInfo, Struct>::AccessCheckInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::AccessCheckInfoVerify(AccessCheckInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsAccessorInfo_NonInline(HeapObject o) {
  return o.IsAccessorInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedAccessorInfo<AccessorInfo, Struct>::AccessorInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::AccessorInfoVerify(AccessorInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
