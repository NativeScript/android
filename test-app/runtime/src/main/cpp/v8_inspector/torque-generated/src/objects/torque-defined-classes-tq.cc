#include "src/objects/torque-defined-classes-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsStrongDescriptorArray_NonInline(HeapObject o) {
  return o.IsStrongDescriptorArray();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedStrongDescriptorArray<StrongDescriptorArray, DescriptorArray>::StrongDescriptorArrayVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::StrongDescriptorArrayVerify(StrongDescriptorArray::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsInternalClass_NonInline(HeapObject o) {
  return o.IsInternalClass();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedInternalClass<InternalClass, HeapObject>::InternalClassVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::InternalClassVerify(InternalClass::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsSmiPair_NonInline(HeapObject o) {
  return o.IsSmiPair();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedSmiPair<SmiPair, HeapObject>::SmiPairVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::SmiPairVerify(SmiPair::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsSmiBox_NonInline(HeapObject o) {
  return o.IsSmiBox();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedSmiBox<SmiBox, HeapObject>::SmiBoxVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::SmiBoxVerify(SmiBox::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsExportedSubClassBase_NonInline(HeapObject o) {
  return o.IsExportedSubClassBase();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedExportedSubClassBase<ExportedSubClassBase, HeapObject>::ExportedSubClassBaseVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ExportedSubClassBaseVerify(ExportedSubClassBase::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsExportedSubClass_NonInline(HeapObject o) {
  return o.IsExportedSubClass();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedExportedSubClass<ExportedSubClass, ExportedSubClassBase>::ExportedSubClassVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ExportedSubClassVerify(ExportedSubClass::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsAbstractInternalClass_NonInline(HeapObject o) {
  return o.IsAbstractInternalClass();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedAbstractInternalClass<AbstractInternalClass, HeapObject>::AbstractInternalClassVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::AbstractInternalClassVerify(AbstractInternalClass::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsAbstractInternalClassSubclass1_NonInline(HeapObject o) {
  return o.IsAbstractInternalClassSubclass1();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedAbstractInternalClassSubclass1<AbstractInternalClassSubclass1, AbstractInternalClass>::AbstractInternalClassSubclass1Verify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::AbstractInternalClassSubclass1Verify(AbstractInternalClassSubclass1::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsAbstractInternalClassSubclass2_NonInline(HeapObject o) {
  return o.IsAbstractInternalClassSubclass2();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedAbstractInternalClassSubclass2<AbstractInternalClassSubclass2, AbstractInternalClass>::AbstractInternalClassSubclass2Verify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::AbstractInternalClassSubclass2Verify(AbstractInternalClassSubclass2::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsInternalClassWithSmiElements_NonInline(HeapObject o) {
  return o.IsInternalClassWithSmiElements();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedInternalClassWithSmiElements<InternalClassWithSmiElements, FixedArrayBase>::InternalClassWithSmiElementsVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::InternalClassWithSmiElementsVerify(InternalClassWithSmiElements::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsInternalClassWithStructElements_NonInline(HeapObject o) {
  return o.IsInternalClassWithStructElements();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedInternalClassWithStructElements<InternalClassWithStructElements, HeapObject>::InternalClassWithStructElementsVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::InternalClassWithStructElementsVerify(InternalClassWithStructElements::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsExportedSubClass2_NonInline(HeapObject o) {
  return o.IsExportedSubClass2();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedExportedSubClass2<ExportedSubClass2, ExportedSubClassBase>::ExportedSubClass2Verify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::ExportedSubClass2Verify(ExportedSubClass2::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsSortState_NonInline(HeapObject o) {
  return o.IsSortState();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedSortState<SortState, HeapObject>::SortStateVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::SortStateVerify(SortState::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
