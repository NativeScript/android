template<class D, class P>
Object TorqueGeneratedStackFrameInfo<D, P>::receiver_or_instance() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedStackFrameInfo::receiver_or_instance(cage_base);
}

template<class D, class P>
Object TorqueGeneratedStackFrameInfo<D, P>::receiver_or_instance(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kReceiverOrInstanceOffset);
  DCHECK(value.IsJSReceiver() || value.IsSmi() || value.IsHeapNumber() || value.IsBigInt() || value.IsString() || value.IsSymbol() || value.IsTrue() || value.IsFalse() || value.IsNull() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedStackFrameInfo<D, P>::set_receiver_or_instance(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver() || value.IsSmi() || value.IsHeapNumber() || value.IsBigInt() || value.IsString() || value.IsSymbol() || value.IsTrue() || value.IsFalse() || value.IsNull() || value.IsUndefined());
  WRITE_FIELD(*this, kReceiverOrInstanceOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kReceiverOrInstanceOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedStackFrameInfo<D, P>::function() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedStackFrameInfo::function(cage_base);
}

template<class D, class P>
Object TorqueGeneratedStackFrameInfo<D, P>::function(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kFunctionOffset);
  DCHECK(value.IsSmi() || value.IsJSFunction());
  return value;
}

template<class D, class P>
void TorqueGeneratedStackFrameInfo<D, P>::set_function(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsSmi() || value.IsJSFunction());
  WRITE_FIELD(*this, kFunctionOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kFunctionOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedStackFrameInfo<D, P>::code_object() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedStackFrameInfo::code_object(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedStackFrameInfo<D, P>::code_object(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kCodeObjectOffset);
  DCHECK(value.IsHeapObject());
  return value;
}

template<class D, class P>
void TorqueGeneratedStackFrameInfo<D, P>::set_code_object(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject());
  WRITE_FIELD(*this, kCodeObjectOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCodeObjectOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedStackFrameInfo<D, P>::code_offset_or_source_position() const {
  int value;
  value = TaggedField<Smi>::load(*this, kCodeOffsetOrSourcePositionOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedStackFrameInfo<D, P>::set_code_offset_or_source_position(int value) {
  WRITE_FIELD(*this, kCodeOffsetOrSourcePositionOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedStackFrameInfo<D, P>::flags() const {
  int value;
  value = TaggedField<Smi>::load(*this, kFlagsOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedStackFrameInfo<D, P>::set_flags(int value) {
  WRITE_FIELD(*this, kFlagsOffset, Smi::FromInt(value));
}

template<class D, class P>
FixedArray TorqueGeneratedStackFrameInfo<D, P>::parameters() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedStackFrameInfo::parameters(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedStackFrameInfo<D, P>::parameters(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kParametersOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedStackFrameInfo<D, P>::set_parameters(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kParametersOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kParametersOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedStackFrameInfo<D, P>::TorqueGeneratedStackFrameInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsStackFrameInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedStackFrameInfo<D, P>::TorqueGeneratedStackFrameInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsStackFrameInfo_NonInline(*this));
}
