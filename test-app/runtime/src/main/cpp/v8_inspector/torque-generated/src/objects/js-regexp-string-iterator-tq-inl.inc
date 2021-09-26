template<class D, class P>
JSReceiver TorqueGeneratedJSRegExpStringIterator<D, P>::iterating_reg_exp() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSRegExpStringIterator::iterating_reg_exp(cage_base);
}

template<class D, class P>
JSReceiver TorqueGeneratedJSRegExpStringIterator<D, P>::iterating_reg_exp(PtrComprCageBase cage_base) const {
  JSReceiver value;
  value = TaggedField<JSReceiver>::load(cage_base, *this, kIteratingRegExpOffset);
  DCHECK(value.IsJSReceiver());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSRegExpStringIterator<D, P>::set_iterating_reg_exp(JSReceiver value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver());
  WRITE_FIELD(*this, kIteratingRegExpOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kIteratingRegExpOffset, value, mode);
}

template<class D, class P>
String TorqueGeneratedJSRegExpStringIterator<D, P>::iterated_string() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSRegExpStringIterator::iterated_string(cage_base);
}

template<class D, class P>
String TorqueGeneratedJSRegExpStringIterator<D, P>::iterated_string(PtrComprCageBase cage_base) const {
  String value;
  value = TaggedField<String>::load(cage_base, *this, kIteratedStringOffset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSRegExpStringIterator<D, P>::set_iterated_string(String value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, kIteratedStringOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kIteratedStringOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedJSRegExpStringIterator<D, P>::flags() const {
  int value;
  value = TaggedField<Smi>::load(*this, kFlagsOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedJSRegExpStringIterator<D, P>::set_flags(int value) {
  WRITE_FIELD(*this, kFlagsOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedJSRegExpStringIterator<D, P>::TorqueGeneratedJSRegExpStringIterator(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsJSRegExpStringIterator_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedJSRegExpStringIterator<D, P>::TorqueGeneratedJSRegExpStringIterator(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsJSRegExpStringIterator_NonInline(*this));
}
