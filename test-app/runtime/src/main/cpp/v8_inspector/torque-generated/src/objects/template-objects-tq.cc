#include "src/objects/template-objects-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsCachedTemplateObject_NonInline(HeapObject o) {
  return o.IsCachedTemplateObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedCachedTemplateObject<CachedTemplateObject, Struct>::CachedTemplateObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::CachedTemplateObjectVerify(CachedTemplateObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsTemplateObjectDescription_NonInline(HeapObject o) {
  return o.IsTemplateObjectDescription();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedTemplateObjectDescription<TemplateObjectDescription, Struct>::TemplateObjectDescriptionVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::TemplateObjectDescriptionVerify(TemplateObjectDescription::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
