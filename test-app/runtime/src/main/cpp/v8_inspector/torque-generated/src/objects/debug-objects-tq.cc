#include "src/objects/debug-objects-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsBreakPoint_NonInline(HeapObject o) {
  return o.IsBreakPoint();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedBreakPoint<BreakPoint, Struct>::BreakPointVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::BreakPointVerify(BreakPoint::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsBreakPointInfo_NonInline(HeapObject o) {
  return o.IsBreakPointInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedBreakPointInfo<BreakPointInfo, Struct>::BreakPointInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::BreakPointInfoVerify(BreakPointInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsDebugInfo_NonInline(HeapObject o) {
  return o.IsDebugInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedDebugInfo<DebugInfo, Struct>::DebugInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::DebugInfoVerify(DebugInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsCoverageInfo_NonInline(HeapObject o) {
  return o.IsCoverageInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedCoverageInfo<CoverageInfo, HeapObject>::CoverageInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::CoverageInfoVerify(CoverageInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
