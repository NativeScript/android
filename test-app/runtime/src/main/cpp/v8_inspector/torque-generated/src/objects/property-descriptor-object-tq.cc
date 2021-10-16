#include "src/objects/property-descriptor-object-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsPropertyDescriptorObject_NonInline(HeapObject o) {
  return o.IsPropertyDescriptorObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedPropertyDescriptorObject<PropertyDescriptorObject, Struct>::PropertyDescriptorObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::PropertyDescriptorObjectVerify(PropertyDescriptorObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
