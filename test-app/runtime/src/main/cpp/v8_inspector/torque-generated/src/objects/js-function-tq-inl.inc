template<class D, class P>
inline TorqueGeneratedJSFunctionOrBoundFunction<D, P>::TorqueGeneratedJSFunctionOrBoundFunction(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsJSFunctionOrBoundFunction_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedJSFunctionOrBoundFunction<D, P>::TorqueGeneratedJSFunctionOrBoundFunction(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsJSFunctionOrBoundFunction_NonInline(*this));
}
template<class D, class P>
JSReceiver TorqueGeneratedJSBoundFunction<D, P>::bound_target_function() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSBoundFunction::bound_target_function(cage_base);
}

template<class D, class P>
JSReceiver TorqueGeneratedJSBoundFunction<D, P>::bound_target_function(PtrComprCageBase cage_base) const {
  JSReceiver value;
  value = TaggedField<JSReceiver>::load(cage_base, *this, kBoundTargetFunctionOffset);
  DCHECK(value.IsCallableApiObject() || value.IsCallableJSProxy() || value.IsJSFunction() || value.IsJSBoundFunction());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSBoundFunction<D, P>::set_bound_target_function(JSReceiver value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsCallableApiObject() || value.IsCallableJSProxy() || value.IsJSFunction() || value.IsJSBoundFunction());
  WRITE_FIELD(*this, kBoundTargetFunctionOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kBoundTargetFunctionOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedJSBoundFunction<D, P>::bound_this() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSBoundFunction::bound_this(cage_base);
}

template<class D, class P>
Object TorqueGeneratedJSBoundFunction<D, P>::bound_this(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kBoundThisOffset);
  DCHECK(value.IsJSReceiver() || value.IsSmi() || value.IsHeapNumber() || value.IsBigInt() || value.IsString() || value.IsSymbol() || value.IsTrue() || value.IsFalse() || value.IsNull() || value.IsUndefined() || value.IsSourceTextModule());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSBoundFunction<D, P>::set_bound_this(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver() || value.IsSmi() || value.IsHeapNumber() || value.IsBigInt() || value.IsString() || value.IsSymbol() || value.IsTrue() || value.IsFalse() || value.IsNull() || value.IsUndefined() || value.IsSourceTextModule());
  WRITE_FIELD(*this, kBoundThisOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kBoundThisOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedJSBoundFunction<D, P>::bound_arguments() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSBoundFunction::bound_arguments(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedJSBoundFunction<D, P>::bound_arguments(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kBoundArgumentsOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSBoundFunction<D, P>::set_bound_arguments(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kBoundArgumentsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kBoundArgumentsOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedJSBoundFunction<D, P>::TorqueGeneratedJSBoundFunction(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsJSBoundFunction_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedJSBoundFunction<D, P>::TorqueGeneratedJSBoundFunction(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsJSBoundFunction_NonInline(*this));
}
