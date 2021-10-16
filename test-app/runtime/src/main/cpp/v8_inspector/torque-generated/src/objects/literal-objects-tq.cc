#include "src/objects/literal-objects-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsArrayBoilerplateDescription_NonInline(HeapObject o) {
  return o.IsArrayBoilerplateDescription();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedArrayBoilerplateDescription<ArrayBoilerplateDescription, Struct>::ArrayBoilerplateDescriptionVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ArrayBoilerplateDescriptionVerify(ArrayBoilerplateDescription::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsRegExpBoilerplateDescription_NonInline(HeapObject o) {
  return o.IsRegExpBoilerplateDescription();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedRegExpBoilerplateDescription<RegExpBoilerplateDescription, Struct>::RegExpBoilerplateDescriptionVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::RegExpBoilerplateDescriptionVerify(RegExpBoilerplateDescription::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
