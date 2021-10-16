#include "src/objects/name-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsName_NonInline(HeapObject o) {
  return o.IsName();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedName<Name, PrimitiveHeapObject>::NameVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::NameVerify(Name::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsSymbol_NonInline(HeapObject o) {
  return o.IsSymbol();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedSymbol<Symbol, Name>::SymbolVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::SymbolVerify(Symbol::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
