#include "src/objects/script-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsScript_NonInline(HeapObject o) {
  return o.IsScript();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedScript<Script, Struct>::ScriptVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ScriptVerify(Script::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
