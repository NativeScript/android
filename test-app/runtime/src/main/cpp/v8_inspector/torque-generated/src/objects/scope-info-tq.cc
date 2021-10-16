#include "src/objects/scope-info-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsScopeInfo_NonInline(HeapObject o) {
  return o.IsScopeInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedScopeInfo<ScopeInfo, HeapObject>::ScopeInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ScopeInfoVerify(ScopeInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
