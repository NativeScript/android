#include "src/objects/promise-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsPromiseCapability_NonInline(HeapObject o) {
  return o.IsPromiseCapability();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedPromiseCapability<PromiseCapability, Struct>::PromiseCapabilityVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::PromiseCapabilityVerify(PromiseCapability::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsPromiseReaction_NonInline(HeapObject o) {
  return o.IsPromiseReaction();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedPromiseReaction<PromiseReaction, Struct>::PromiseReactionVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::PromiseReactionVerify(PromiseReaction::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsPromiseReactionJobTask_NonInline(HeapObject o) {
  return o.IsPromiseReactionJobTask();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedPromiseReactionJobTask<PromiseReactionJobTask, Microtask>::PromiseReactionJobTaskVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::PromiseReactionJobTaskVerify(PromiseReactionJobTask::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsPromiseFulfillReactionJobTask_NonInline(HeapObject o) {
  return o.IsPromiseFulfillReactionJobTask();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedPromiseFulfillReactionJobTask<PromiseFulfillReactionJobTask, PromiseReactionJobTask>::PromiseFulfillReactionJobTaskVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::PromiseFulfillReactionJobTaskVerify(PromiseFulfillReactionJobTask::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsPromiseRejectReactionJobTask_NonInline(HeapObject o) {
  return o.IsPromiseRejectReactionJobTask();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedPromiseRejectReactionJobTask<PromiseRejectReactionJobTask, PromiseReactionJobTask>::PromiseRejectReactionJobTaskVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::PromiseRejectReactionJobTaskVerify(PromiseRejectReactionJobTask::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsPromiseResolveThenableJobTask_NonInline(HeapObject o) {
  return o.IsPromiseResolveThenableJobTask();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedPromiseResolveThenableJobTask<PromiseResolveThenableJobTask, Microtask>::PromiseResolveThenableJobTaskVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::PromiseResolveThenableJobTaskVerify(PromiseResolveThenableJobTask::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
