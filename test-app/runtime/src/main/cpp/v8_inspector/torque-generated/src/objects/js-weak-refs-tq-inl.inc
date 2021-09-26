template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::finalization_registry() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakCell::finalization_registry(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::finalization_registry(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kFinalizationRegistryOffset);
  DCHECK(value.IsUndefined() || value.IsJSFinalizationRegistry());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakCell<D, P>::set_finalization_registry(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsJSFinalizationRegistry());
  WRITE_FIELD(*this, kFinalizationRegistryOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kFinalizationRegistryOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::target() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakCell::target(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::target(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kTargetOffset);
  DCHECK(value.IsJSReceiver() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakCell<D, P>::set_target(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver() || value.IsUndefined());
  WRITE_FIELD(*this, kTargetOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kTargetOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::unregister_token() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakCell::unregister_token(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::unregister_token(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kUnregisterTokenOffset);
  DCHECK(value.IsJSReceiver() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakCell<D, P>::set_unregister_token(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver() || value.IsUndefined());
  WRITE_FIELD(*this, kUnregisterTokenOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kUnregisterTokenOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedWeakCell<D, P>::holdings() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakCell::holdings(cage_base);
}

template<class D, class P>
Object TorqueGeneratedWeakCell<D, P>::holdings(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kHoldingsOffset);
  DCHECK(value.IsJSReceiver() || value.IsSmi() || value.IsHeapNumber() || value.IsBigInt() || value.IsString() || value.IsSymbol() || value.IsTrue() || value.IsFalse() || value.IsNull() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakCell<D, P>::set_holdings(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver() || value.IsSmi() || value.IsHeapNumber() || value.IsBigInt() || value.IsString() || value.IsSymbol() || value.IsTrue() || value.IsFalse() || value.IsNull() || value.IsUndefined());
  WRITE_FIELD(*this, kHoldingsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kHoldingsOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::prev() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakCell::prev(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::prev(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kPrevOffset);
  DCHECK(value.IsUndefined() || value.IsWeakCell());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakCell<D, P>::set_prev(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsWeakCell());
  WRITE_FIELD(*this, kPrevOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPrevOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::next() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakCell::next(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::next(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kNextOffset);
  DCHECK(value.IsUndefined() || value.IsWeakCell());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakCell<D, P>::set_next(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsWeakCell());
  WRITE_FIELD(*this, kNextOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kNextOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::key_list_prev() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakCell::key_list_prev(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::key_list_prev(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kKeyListPrevOffset);
  DCHECK(value.IsUndefined() || value.IsWeakCell());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakCell<D, P>::set_key_list_prev(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsWeakCell());
  WRITE_FIELD(*this, kKeyListPrevOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kKeyListPrevOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::key_list_next() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakCell::key_list_next(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWeakCell<D, P>::key_list_next(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kKeyListNextOffset);
  DCHECK(value.IsUndefined() || value.IsWeakCell());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakCell<D, P>::set_key_list_next(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsWeakCell());
  WRITE_FIELD(*this, kKeyListNextOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kKeyListNextOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWeakCell<D, P>::TorqueGeneratedWeakCell(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWeakCell_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWeakCell<D, P>::TorqueGeneratedWeakCell(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWeakCell_NonInline(*this));
}
template<class D, class P>
HeapObject TorqueGeneratedJSWeakRef<D, P>::target() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSWeakRef::target(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedJSWeakRef<D, P>::target(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kTargetOffset);
  DCHECK(value.IsJSReceiver() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSWeakRef<D, P>::set_target(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver() || value.IsUndefined());
  WRITE_FIELD(*this, kTargetOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kTargetOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedJSWeakRef<D, P>::TorqueGeneratedJSWeakRef(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsJSWeakRef_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedJSWeakRef<D, P>::TorqueGeneratedJSWeakRef(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsJSWeakRef_NonInline(*this));
}
