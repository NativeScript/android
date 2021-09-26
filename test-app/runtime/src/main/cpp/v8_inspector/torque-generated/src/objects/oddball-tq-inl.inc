template<class D, class P>
double TorqueGeneratedOddball<D, P>::to_number_raw() const {
  double value;
  value = this->template ReadField<double>(kToNumberRawOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedOddball<D, P>::set_to_number_raw(double value) {
  this->template WriteField<double>(kToNumberRawOffset, value);
}

template<class D, class P>
String TorqueGeneratedOddball<D, P>::to_string() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedOddball::to_string(cage_base);
}

template<class D, class P>
String TorqueGeneratedOddball<D, P>::to_string(PtrComprCageBase cage_base) const {
  String value;
  value = TaggedField<String>::load(cage_base, *this, kToStringOffset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedOddball<D, P>::set_to_string(String value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, kToStringOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kToStringOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedOddball<D, P>::to_number() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedOddball::to_number(cage_base);
}

template<class D, class P>
Object TorqueGeneratedOddball<D, P>::to_number(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kToNumberOffset);
  DCHECK(value.IsSmi() || value.IsHeapNumber());
  return value;
}

template<class D, class P>
void TorqueGeneratedOddball<D, P>::set_to_number(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsSmi() || value.IsHeapNumber());
  WRITE_FIELD(*this, kToNumberOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kToNumberOffset, value, mode);
}

template<class D, class P>
String TorqueGeneratedOddball<D, P>::type_of() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedOddball::type_of(cage_base);
}

template<class D, class P>
String TorqueGeneratedOddball<D, P>::type_of(PtrComprCageBase cage_base) const {
  String value;
  value = TaggedField<String>::load(cage_base, *this, kTypeOfOffset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedOddball<D, P>::set_type_of(String value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, kTypeOfOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kTypeOfOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedOddball<D, P>::kind() const {
  int value;
  value = TaggedField<Smi>::load(*this, kKindOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedOddball<D, P>::set_kind(int value) {
  WRITE_FIELD(*this, kKindOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedOddball<D, P>::TorqueGeneratedOddball(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsOddball_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedOddball<D, P>::TorqueGeneratedOddball(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsOddball_NonInline(*this));
}
