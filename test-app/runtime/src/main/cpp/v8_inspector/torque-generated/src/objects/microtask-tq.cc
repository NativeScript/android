#include "src/objects/microtask-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsMicrotask_NonInline(HeapObject o) {
  return o.IsMicrotask();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedMicrotask<Microtask, Struct>::MicrotaskVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::MicrotaskVerify(Microtask::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsCallbackTask_NonInline(HeapObject o) {
  return o.IsCallbackTask();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedCallbackTask<CallbackTask, Microtask>::CallbackTaskVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::CallbackTaskVerify(CallbackTask::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsCallableTask_NonInline(HeapObject o) {
  return o.IsCallableTask();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedCallableTask<CallableTask, Microtask>::CallableTaskVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::CallableTaskVerify(CallableTask::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
