inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceContextElements(Context p_o);
inline intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int31_0(int31_t p_i);
inline intptr_t TqRuntimeConvert_intptr_Smi_0(Smi p_s);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_Object_0(Object p_object, intptr_t p_offset, intptr_t p_length);
#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceContextElements
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceContextElements
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceContextElements(Context p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  Smi tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  Object tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp1 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp0));
  tmp2 = TqRuntimeConvert_intptr_Smi_0(tmp1);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  std::tie(tmp4, tmp5, tmp6) = TqRuntimeNewMutableSlice_Object_0(p_o, tmp3, tmp2);
  goto block2;

  block2:
  return std::make_tuple(tmp4, tmp5, tmp6);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceContextElements

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_intptr_constexpr_int31_0
intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int31_0(int31_t p_i) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_Smi_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_Smi_0
intptr_t TqRuntimeConvert_intptr_Smi_0(Smi p_s) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = TorqueRuntimeMacroShims::CodeStubAssembler::SmiUntag(p_s);
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_Object_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_Object_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_Object_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return std::make_tuple(tmp0, tmp1, tmp2);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_Object_0

template<class D, class P>
int TorqueGeneratedContext<D, P>::length() const {
  int value;
  value = TaggedField<Smi>::load(*this, kLengthOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedContext<D, P>::set_length(int value) {
  WRITE_FIELD(*this, kLengthOffset, Smi::FromInt(value));
}

template<class D, class P>
Object TorqueGeneratedContext<D, P>::elements(int i) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedContext::elements(cage_base, i);
}

template<class D, class P>
Object TorqueGeneratedContext<D, P>::elements(PtrComprCageBase cage_base, int i) const {
  Object value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->length());
  int offset = kElementsOffset + i * kTaggedSize;
  value = TaggedField<Object>::load(cage_base, *this, offset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedContext<D, P>::set_elements(int i, Object value, WriteBarrierMode mode) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->length());
  int offset = kElementsOffset + i * kTaggedSize;
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedContext<D, P>::TorqueGeneratedContext(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsContext_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedContext<D, P>::TorqueGeneratedContext(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsContext_NonInline(*this));
}
