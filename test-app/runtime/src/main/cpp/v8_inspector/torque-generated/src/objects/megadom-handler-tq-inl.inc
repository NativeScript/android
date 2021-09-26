template<class D, class P>
MaybeObject TorqueGeneratedMegaDomHandler<D, P>::accessor() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedMegaDomHandler::accessor(cage_base);
}

template<class D, class P>
MaybeObject TorqueGeneratedMegaDomHandler<D, P>::accessor(PtrComprCageBase cage_base) const {
  MaybeObject value;
  value = TaggedField<MaybeObject>::load(cage_base, *this, kAccessorOffset);
  DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapObject()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || value.IsWeak());
  return value;
}

template<class D, class P>
void TorqueGeneratedMegaDomHandler<D, P>::set_accessor(MaybeObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapObject()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || value.IsWeak());
  RELAXED_WRITE_WEAK_FIELD(*this, kAccessorOffset, value);
  CONDITIONAL_WEAK_WRITE_BARRIER(*this, kAccessorOffset, value, mode);
}

template<class D, class P>
MaybeObject TorqueGeneratedMegaDomHandler<D, P>::context() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedMegaDomHandler::context(cage_base);
}

template<class D, class P>
MaybeObject TorqueGeneratedMegaDomHandler<D, P>::context(PtrComprCageBase cage_base) const {
  MaybeObject value;
  value = TaggedField<MaybeObject>::load(cage_base, *this, kContextOffset);
  DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapObject()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || value.IsWeak());
  return value;
}

template<class D, class P>
void TorqueGeneratedMegaDomHandler<D, P>::set_context(MaybeObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapObject()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || value.IsWeak());
  RELAXED_WRITE_WEAK_FIELD(*this, kContextOffset, value);
  CONDITIONAL_WEAK_WRITE_BARRIER(*this, kContextOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedMegaDomHandler<D, P>::TorqueGeneratedMegaDomHandler(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsMegaDomHandler_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedMegaDomHandler<D, P>::TorqueGeneratedMegaDomHandler(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsMegaDomHandler_NonInline(*this));
}
