template<class D, class P>
HeapObject TorqueGeneratedJSRegExp<D, P>::data() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSRegExp::data(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedJSRegExp<D, P>::data(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kDataOffset);
  DCHECK(value.IsUndefined() || value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSRegExp<D, P>::set_data(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFixedArray());
  WRITE_FIELD(*this, kDataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kDataOffset, value, mode);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedJSRegExp<D, P>::source() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSRegExp::source(cage_base);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedJSRegExp<D, P>::source(PtrComprCageBase cage_base) const {
  PrimitiveHeapObject value;
  value = TaggedField<PrimitiveHeapObject>::load(cage_base, *this, kSourceOffset);
  DCHECK(value.IsString() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSRegExp<D, P>::set_source(PrimitiveHeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString() || value.IsUndefined());
  WRITE_FIELD(*this, kSourceOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSourceOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedJSRegExp<D, P>::flags() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSRegExp::flags(cage_base);
}

template<class D, class P>
Object TorqueGeneratedJSRegExp<D, P>::flags(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kFlagsOffset);
  DCHECK(value.IsUndefined() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSRegExp<D, P>::set_flags(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsSmi());
  WRITE_FIELD(*this, kFlagsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kFlagsOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedJSRegExp<D, P>::TorqueGeneratedJSRegExp(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsJSRegExp_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedJSRegExp<D, P>::TorqueGeneratedJSRegExp(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsJSRegExp_NonInline(*this));
}
