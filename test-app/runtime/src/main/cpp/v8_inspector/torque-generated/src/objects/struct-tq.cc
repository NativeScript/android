#include "src/objects/struct-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsStruct_NonInline(HeapObject o) {
  return o.IsStruct();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedStruct<Struct, HeapObject>::StructVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::StructVerify(Struct::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsAccessorPair_NonInline(HeapObject o) {
  return o.IsAccessorPair();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedAccessorPair<AccessorPair, Struct>::AccessorPairVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::AccessorPairVerify(AccessorPair::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsClassPositions_NonInline(HeapObject o) {
  return o.IsClassPositions();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedClassPositions<ClassPositions, Struct>::ClassPositionsVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ClassPositionsVerify(ClassPositions::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsTuple2_NonInline(HeapObject o) {
  return o.IsTuple2();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedTuple2<Tuple2, Struct>::Tuple2Verify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::Tuple2Verify(Tuple2::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
