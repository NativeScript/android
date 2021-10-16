#include "src/objects/source-text-module-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsSourceTextModule_NonInline(HeapObject o) {
  return o.IsSourceTextModule();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedSourceTextModule<SourceTextModule, Module>::SourceTextModuleVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::SourceTextModuleVerify(SourceTextModule::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsModuleRequest_NonInline(HeapObject o) {
  return o.IsModuleRequest();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedModuleRequest<ModuleRequest, Struct>::ModuleRequestVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ModuleRequestVerify(ModuleRequest::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsSourceTextModuleInfoEntry_NonInline(HeapObject o) {
  return o.IsSourceTextModuleInfoEntry();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedSourceTextModuleInfoEntry<SourceTextModuleInfoEntry, Struct>::SourceTextModuleInfoEntryVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::SourceTextModuleInfoEntryVerify(SourceTextModuleInfoEntry::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
