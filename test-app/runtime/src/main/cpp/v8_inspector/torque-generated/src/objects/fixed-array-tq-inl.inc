inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceFixedArrayObjects(FixedArray p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceByteArrayBytes(ByteArray p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceWeakFixedArrayObjects(WeakFixedArray p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceFixedDoubleArrayFloats(FixedDoubleArray p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceWeakArrayListObjects(WeakArrayList p_o);
inline intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int31_0(int31_t p_i);
inline intptr_t TqRuntimeConvert_intptr_Smi_0(Smi p_s);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_Object_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_uint8_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_MaybeObject_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_float64_or_hole_0(Object p_object, intptr_t p_offset, intptr_t p_length);
#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceFixedArrayObjects
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceFixedArrayObjects
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceFixedArrayObjects(FixedArray p_o) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceFixedArrayObjects

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceByteArrayBytes
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceByteArrayBytes
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceByteArrayBytes(ByteArray p_o) {
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
  std::tie(tmp4, tmp5, tmp6) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp3, tmp2);
  goto block2;

  block2:
  return std::make_tuple(tmp4, tmp5, tmp6);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceByteArrayBytes

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceWeakFixedArrayObjects
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceWeakFixedArrayObjects
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceWeakFixedArrayObjects(WeakFixedArray p_o) {
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
  std::tie(tmp4, tmp5, tmp6) = TqRuntimeNewMutableSlice_MaybeObject_0(p_o, tmp3, tmp2);
  goto block2;

  block2:
  return std::make_tuple(tmp4, tmp5, tmp6);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceWeakFixedArrayObjects

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceFixedDoubleArrayFloats
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceFixedDoubleArrayFloats
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceFixedDoubleArrayFloats(FixedDoubleArray p_o) {
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
  std::tie(tmp4, tmp5, tmp6) = TqRuntimeNewMutableSlice_float64_or_hole_0(p_o, tmp3, tmp2);
  goto block2;

  block2:
  return std::make_tuple(tmp4, tmp5, tmp6);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceFixedDoubleArrayFloats

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceWeakArrayListObjects
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceWeakArrayListObjects
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceWeakArrayListObjects(WeakArrayList p_o) {
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
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(12);
  std::tie(tmp4, tmp5, tmp6) = TqRuntimeNewMutableSlice_MaybeObject_0(p_o, tmp3, tmp2);
  goto block2;

  block2:
  return std::make_tuple(tmp4, tmp5, tmp6);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceWeakArrayListObjects

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

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_uint8_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_uint8_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_uint8_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_uint8_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_MaybeObject_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_MaybeObject_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_MaybeObject_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_MaybeObject_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_float64_or_hole_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_float64_or_hole_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_float64_or_hole_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_float64_or_hole_0

template<class D, class P>
int TorqueGeneratedFixedArrayBase<D, P>::length() const {
  int value;
  value = TaggedField<Smi>::load(*this, kLengthOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedFixedArrayBase<D, P>::set_length(int value) {
  WRITE_FIELD(*this, kLengthOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedFixedArrayBase<D, P>::TorqueGeneratedFixedArrayBase(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsFixedArrayBase_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedFixedArrayBase<D, P>::TorqueGeneratedFixedArrayBase(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsFixedArrayBase_NonInline(*this));
}
template<class D, class P>
Object TorqueGeneratedFixedArray<D, P>::objects(int i) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFixedArray::objects(cage_base, i);
}

template<class D, class P>
Object TorqueGeneratedFixedArray<D, P>::objects(PtrComprCageBase cage_base, int i) const {
  Object value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->length());
  int offset = kObjectsOffset + i * kTaggedSize;
  value = TaggedField<Object>::load(cage_base, *this, offset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedFixedArray<D, P>::set_objects(int i, Object value, WriteBarrierMode mode) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->length());
  int offset = kObjectsOffset + i * kTaggedSize;
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedFixedArray<D, P>::TorqueGeneratedFixedArray(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsFixedArray_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedFixedArray<D, P>::TorqueGeneratedFixedArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsFixedArray_NonInline(*this));
}
template<class D, class P>
uint8_t TorqueGeneratedByteArray<D, P>::bytes(int i) const {
  uint8_t value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->length());
  int offset = kBytesOffset + i * kUInt8Size;
  value = this->template ReadField<uint8_t>(offset);
  return value;
}

template<class D, class P>
void TorqueGeneratedByteArray<D, P>::set_bytes(int i, uint8_t value) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->length());
  int offset = kBytesOffset + i * kUInt8Size;
  this->template WriteField<uint8_t>(offset, value);
}

template<class D, class P>
inline TorqueGeneratedByteArray<D, P>::TorqueGeneratedByteArray(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsByteArray_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedByteArray<D, P>::TorqueGeneratedByteArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsByteArray_NonInline(*this));
}
template<class D, class P>
int TorqueGeneratedWeakFixedArray<D, P>::length() const {
  int value;
  value = TaggedField<Smi>::load(*this, kLengthOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakFixedArray<D, P>::set_length(int value) {
  WRITE_FIELD(*this, kLengthOffset, Smi::FromInt(value));
}

template<class D, class P>
MaybeObject TorqueGeneratedWeakFixedArray<D, P>::objects(int i, RelaxedLoadTag) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakFixedArray::objects(cage_base, i, kRelaxedLoad);
}

template<class D, class P>
MaybeObject TorqueGeneratedWeakFixedArray<D, P>::objects(PtrComprCageBase cage_base, int i, RelaxedLoadTag) const {
  MaybeObject value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->length());
  int offset = kObjectsOffset + i * kTaggedSize;
  value = TaggedField<MaybeObject>::Relaxed_Load(cage_base, *this, offset);
  DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapObject()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || value.IsWeak());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakFixedArray<D, P>::set_objects(int i, MaybeObject value, WriteBarrierMode mode) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->length());
  int offset = kObjectsOffset + i * kTaggedSize;
  SLOW_DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapObject()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || value.IsWeak());
  RELAXED_WRITE_WEAK_FIELD(*this, offset, value);
  CONDITIONAL_WEAK_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWeakFixedArray<D, P>::TorqueGeneratedWeakFixedArray(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWeakFixedArray_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWeakFixedArray<D, P>::TorqueGeneratedWeakFixedArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWeakFixedArray_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedFixedDoubleArray<D, P>::TorqueGeneratedFixedDoubleArray(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsFixedDoubleArray_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedFixedDoubleArray<D, P>::TorqueGeneratedFixedDoubleArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsFixedDoubleArray_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedArrayList<D, P>::TorqueGeneratedArrayList(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsArrayList_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedArrayList<D, P>::TorqueGeneratedArrayList(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsArrayList_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedTemplateList<D, P>::TorqueGeneratedTemplateList(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsTemplateList_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedTemplateList<D, P>::TorqueGeneratedTemplateList(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsTemplateList_NonInline(*this));
}
template<class D, class P>
int TorqueGeneratedWeakArrayList<D, P>::capacity() const {
  int value;
  value = TaggedField<Smi>::load(*this, kCapacityOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakArrayList<D, P>::set_capacity(int value) {
  WRITE_FIELD(*this, kCapacityOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedWeakArrayList<D, P>::length() const {
  int value;
  value = TaggedField<Smi>::load(*this, kLengthOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakArrayList<D, P>::set_length(int value) {
  WRITE_FIELD(*this, kLengthOffset, Smi::FromInt(value));
}

template<class D, class P>
MaybeObject TorqueGeneratedWeakArrayList<D, P>::objects(int i, RelaxedLoadTag) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWeakArrayList::objects(cage_base, i, kRelaxedLoad);
}

template<class D, class P>
MaybeObject TorqueGeneratedWeakArrayList<D, P>::objects(PtrComprCageBase cage_base, int i, RelaxedLoadTag) const {
  MaybeObject value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->capacity());
  int offset = kObjectsOffset + i * kTaggedSize;
  value = TaggedField<MaybeObject>::Relaxed_Load(cage_base, *this, offset);
  DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapObject()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || value.IsWeak());
  return value;
}

template<class D, class P>
void TorqueGeneratedWeakArrayList<D, P>::set_objects(int i, MaybeObject value, WriteBarrierMode mode) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->capacity());
  int offset = kObjectsOffset + i * kTaggedSize;
  SLOW_DCHECK(value.IsCleared() || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsHeapObject()) || (!value.IsWeak() && value.GetHeapObjectOrSmi().IsSmi()) || value.IsWeak());
  RELAXED_WRITE_WEAK_FIELD(*this, offset, value);
  CONDITIONAL_WEAK_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWeakArrayList<D, P>::TorqueGeneratedWeakArrayList(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWeakArrayList_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWeakArrayList<D, P>::TorqueGeneratedWeakArrayList(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWeakArrayList_NonInline(*this));
}
