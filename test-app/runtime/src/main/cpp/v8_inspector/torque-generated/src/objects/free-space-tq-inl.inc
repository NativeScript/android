template<class D, class P>
int TorqueGeneratedFreeSpace<D, P>::size() const {
  int value;
  value = TaggedField<Smi>::load(*this, kSizeOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedFreeSpace<D, P>::set_size(int value) {
  WRITE_FIELD(*this, kSizeOffset, Smi::FromInt(value));
}

template<class D, class P>
MaybeObject TorqueGeneratedFreeSpace<D, P>::next() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFreeSpace::next(cage_base);
}

template<class D, class P>
MaybeObject TorqueGeneratedFreeSpace<D, P>::next(PtrComprCageBase cage_base) const {
  MaybeObject value;
  value = TaggedField<MaybeObject>::load(cage_base, *this, kNextOffset);
  DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsUninitialized()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsFreeSpace()));
  return value;
}

template<class D, class P>
void TorqueGeneratedFreeSpace<D, P>::set_next(MaybeObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsUninitialized()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsFreeSpace()));
  RELAXED_WRITE_WEAK_FIELD(*this, kNextOffset, value);
  CONDITIONAL_WEAK_WRITE_BARRIER(*this, kNextOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedFreeSpace<D, P>::TorqueGeneratedFreeSpace(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsFreeSpace_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedFreeSpace<D, P>::TorqueGeneratedFreeSpace(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsFreeSpace_NonInline(*this));
}
