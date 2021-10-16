#include "src/objects/shared-function-info-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsPreparseData_NonInline(HeapObject o) {
  return o.IsPreparseData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedPreparseData<PreparseData, HeapObject>::PreparseDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::PreparseDataVerify(PreparseData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsBaselineData_NonInline(HeapObject o) {
  return o.IsBaselineData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedBaselineData<BaselineData, Struct>::BaselineDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::BaselineDataVerify(BaselineData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsSharedFunctionInfo_NonInline(HeapObject o) {
  return o.IsSharedFunctionInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedSharedFunctionInfo<SharedFunctionInfo, HeapObject>::SharedFunctionInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::SharedFunctionInfoVerify(SharedFunctionInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsUncompiledData_NonInline(HeapObject o) {
  return o.IsUncompiledData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedUncompiledData<UncompiledData, HeapObject>::UncompiledDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::UncompiledDataVerify(UncompiledData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsUncompiledDataWithoutPreparseData_NonInline(HeapObject o) {
  return o.IsUncompiledDataWithoutPreparseData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedUncompiledDataWithoutPreparseData<UncompiledDataWithoutPreparseData, UncompiledData>::UncompiledDataWithoutPreparseDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::UncompiledDataWithoutPreparseDataVerify(UncompiledDataWithoutPreparseData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsUncompiledDataWithPreparseData_NonInline(HeapObject o) {
  return o.IsUncompiledDataWithPreparseData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedUncompiledDataWithPreparseData<UncompiledDataWithPreparseData, UncompiledData>::UncompiledDataWithPreparseDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::UncompiledDataWithPreparseDataVerify(UncompiledDataWithPreparseData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsOnHeapBasicBlockProfilerData_NonInline(HeapObject o) {
  return o.IsOnHeapBasicBlockProfilerData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedOnHeapBasicBlockProfilerData<OnHeapBasicBlockProfilerData, HeapObject>::OnHeapBasicBlockProfilerDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::OnHeapBasicBlockProfilerDataVerify(OnHeapBasicBlockProfilerData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
