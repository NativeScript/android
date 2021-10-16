inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceDescriptorArrayDescriptors(DescriptorArray p_o);
inline intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int31_0(int31_t p_i);
inline intptr_t TqRuntimeConvert_intptr_uint16_0(uint16_t p_ui);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_DescriptorEntry_0(Object p_object, intptr_t p_offset, intptr_t p_length);
#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceDescriptorArrayDescriptors
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceDescriptorArrayDescriptors
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceDescriptorArrayDescriptors(DescriptorArray p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  uint16_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  Object tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp1 = (p_o).ReadField<uint16_t>(tmp0);
  tmp2 = TqRuntimeConvert_intptr_uint16_0(tmp1);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(16);
  std::tie(tmp4, tmp5, tmp6) = TqRuntimeNewMutableSlice_DescriptorEntry_0(p_o, tmp3, tmp2);
  goto block2;

  block2:
  return std::make_tuple(tmp4, tmp5, tmp6);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceDescriptorArrayDescriptors

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

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_uint16_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_uint16_0
intptr_t TqRuntimeConvert_intptr_uint16_0(uint16_t p_ui) {
  DisallowGarbageCollection no_gc;
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = TorqueRuntimeMacroShims::CodeStubAssembler::ChangeUint32ToWord(p_ui);
  tmp1 = TorqueRuntimeMacroShims::CodeStubAssembler::Signed(tmp0);
  goto block2;

  block2:
  return tmp1;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_uint16_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_DescriptorEntry_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_DescriptorEntry_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_DescriptorEntry_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_DescriptorEntry_0

template<class D, class P>
FixedArray TorqueGeneratedEnumCache<D, P>::keys() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedEnumCache::keys(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedEnumCache<D, P>::keys(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kKeysOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedEnumCache<D, P>::set_keys(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kKeysOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kKeysOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedEnumCache<D, P>::indices() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedEnumCache::indices(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedEnumCache<D, P>::indices(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kIndicesOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedEnumCache<D, P>::set_indices(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kIndicesOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kIndicesOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedEnumCache<D, P>::TorqueGeneratedEnumCache(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsEnumCache_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedEnumCache<D, P>::TorqueGeneratedEnumCache(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsEnumCache_NonInline(*this));
}
template<class D, class P>
uint16_t TorqueGeneratedDescriptorArray<D, P>::number_of_all_descriptors() const {
  uint16_t value;
  value = this->template ReadField<uint16_t>(kNumberOfAllDescriptorsOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedDescriptorArray<D, P>::set_number_of_all_descriptors(uint16_t value) {
  this->template WriteField<uint16_t>(kNumberOfAllDescriptorsOffset, value);
}

template<class D, class P>
uint16_t TorqueGeneratedDescriptorArray<D, P>::number_of_descriptors() const {
  uint16_t value;
  value = this->template ReadField<uint16_t>(kNumberOfDescriptorsOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedDescriptorArray<D, P>::set_number_of_descriptors(uint16_t value) {
  this->template WriteField<uint16_t>(kNumberOfDescriptorsOffset, value);
}

template<class D, class P>
uint16_t TorqueGeneratedDescriptorArray<D, P>::raw_number_of_marked_descriptors() const {
  uint16_t value;
  value = this->template ReadField<uint16_t>(kRawNumberOfMarkedDescriptorsOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedDescriptorArray<D, P>::set_raw_number_of_marked_descriptors(uint16_t value) {
  this->template WriteField<uint16_t>(kRawNumberOfMarkedDescriptorsOffset, value);
}

template<class D, class P>
uint16_t TorqueGeneratedDescriptorArray<D, P>::filler16_bits() const {
  uint16_t value;
  value = this->template ReadField<uint16_t>(kFiller16BitsOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedDescriptorArray<D, P>::set_filler16_bits(uint16_t value) {
  this->template WriteField<uint16_t>(kFiller16BitsOffset, value);
}

template<class D, class P>
EnumCache TorqueGeneratedDescriptorArray<D, P>::enum_cache() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDescriptorArray::enum_cache(cage_base);
}

template<class D, class P>
EnumCache TorqueGeneratedDescriptorArray<D, P>::enum_cache(PtrComprCageBase cage_base) const {
  EnumCache value;
  value = TaggedField<EnumCache>::load(cage_base, *this, kEnumCacheOffset);
  DCHECK(value.IsEnumCache());
  return value;
}

template<class D, class P>
void TorqueGeneratedDescriptorArray<D, P>::set_enum_cache(EnumCache value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsEnumCache());
  WRITE_FIELD(*this, kEnumCacheOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kEnumCacheOffset, value, mode);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedDescriptorArray<D, P>::descriptors_key(int i) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDescriptorArray::descriptors_key(cage_base, i);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedDescriptorArray<D, P>::descriptors_key(PtrComprCageBase cage_base, int i) const {
  PrimitiveHeapObject value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->number_of_all_descriptors());
  int offset = kDescriptorsOffset + 0 + i * 12;
  value = TaggedField<PrimitiveHeapObject>::load(cage_base, *this, offset);
  DCHECK(value.IsName() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedDescriptorArray<D, P>::set_descriptors_key(int i, PrimitiveHeapObject value, WriteBarrierMode mode) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->number_of_all_descriptors());
  int offset = kDescriptorsOffset + 0 + i * 12;
  SLOW_DCHECK(value.IsName() || value.IsUndefined());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedDescriptorArray<D, P>::descriptors_details(int i) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDescriptorArray::descriptors_details(cage_base, i);
}

template<class D, class P>
Object TorqueGeneratedDescriptorArray<D, P>::descriptors_details(PtrComprCageBase cage_base, int i) const {
  Object value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->number_of_all_descriptors());
  int offset = kDescriptorsOffset + 4 + i * 12;
  value = TaggedField<Object>::load(cage_base, *this, offset);
  DCHECK(value.IsSmi() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedDescriptorArray<D, P>::set_descriptors_details(int i, Object value, WriteBarrierMode mode) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->number_of_all_descriptors());
  int offset = kDescriptorsOffset + 4 + i * 12;
  SLOW_DCHECK(value.IsSmi() || value.IsUndefined());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
MaybeObject TorqueGeneratedDescriptorArray<D, P>::descriptors_value(int i) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDescriptorArray::descriptors_value(cage_base, i);
}

template<class D, class P>
MaybeObject TorqueGeneratedDescriptorArray<D, P>::descriptors_value(PtrComprCageBase cage_base, int i) const {
  MaybeObject value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->number_of_all_descriptors());
  int offset = kDescriptorsOffset + 8 + i * 12;
  value = TaggedField<MaybeObject>::load(cage_base, *this, offset);
  DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsJSReceiver()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapNumber()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsBigInt()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsString()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSymbol()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsTrue()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsFalse()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsNull()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsUndefined()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsAccessorInfo()) || (value.IsWeak() && value.GetHeapObjectOrSmi().IsMap()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsAccessorPair()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsClassPositions()));
  return value;
}

template<class D, class P>
void TorqueGeneratedDescriptorArray<D, P>::set_descriptors_value(int i, MaybeObject value, WriteBarrierMode mode) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->number_of_all_descriptors());
  int offset = kDescriptorsOffset + 8 + i * 12;
  SLOW_DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsJSReceiver()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapNumber()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsBigInt()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsString()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSymbol()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsTrue()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsFalse()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsNull()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsUndefined()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsAccessorInfo()) || (value.IsWeak() && value.GetHeapObjectOrSmi().IsMap()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsAccessorPair()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsClassPositions()));
  RELAXED_WRITE_WEAK_FIELD(*this, offset, value);
  CONDITIONAL_WEAK_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedDescriptorArray<D, P>::TorqueGeneratedDescriptorArray(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsDescriptorArray_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedDescriptorArray<D, P>::TorqueGeneratedDescriptorArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsDescriptorArray_NonInline(*this));
}
