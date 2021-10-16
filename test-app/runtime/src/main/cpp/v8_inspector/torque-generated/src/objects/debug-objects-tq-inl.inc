inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceCoverageInfoSlots(CoverageInfo p_o);
inline intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int31_0(int31_t p_i);
inline intptr_t TqRuntimeConvert_intptr_int32_0(int32_t p_i);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_CoverageInfoSlot_0(Object p_object, intptr_t p_offset, intptr_t p_length);
#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceCoverageInfoSlots
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceCoverageInfoSlots
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceCoverageInfoSlots(CoverageInfo p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  int32_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  Object tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp1 = (p_o).ReadField<int32_t>(tmp0);
  tmp2 = TqRuntimeConvert_intptr_int32_0(tmp1);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  std::tie(tmp4, tmp5, tmp6) = TqRuntimeNewMutableSlice_CoverageInfoSlot_0(p_o, tmp3, tmp2);
  goto block2;

  block2:
  return std::make_tuple(tmp4, tmp5, tmp6);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceCoverageInfoSlots

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

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_int32_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_int32_0
intptr_t TqRuntimeConvert_intptr_int32_0(int32_t p_i) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = TorqueRuntimeMacroShims::CodeStubAssembler::ChangeInt32ToIntPtr(p_i);
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_int32_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_CoverageInfoSlot_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_CoverageInfoSlot_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_CoverageInfoSlot_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_CoverageInfoSlot_0

template<class D, class P>
int TorqueGeneratedBreakPoint<D, P>::id() const {
  int value;
  value = TaggedField<Smi>::load(*this, kIdOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedBreakPoint<D, P>::set_id(int value) {
  WRITE_FIELD(*this, kIdOffset, Smi::FromInt(value));
}

template<class D, class P>
String TorqueGeneratedBreakPoint<D, P>::condition() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedBreakPoint::condition(cage_base);
}

template<class D, class P>
String TorqueGeneratedBreakPoint<D, P>::condition(PtrComprCageBase cage_base) const {
  String value;
  value = TaggedField<String>::load(cage_base, *this, kConditionOffset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedBreakPoint<D, P>::set_condition(String value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, kConditionOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kConditionOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedBreakPoint<D, P>::TorqueGeneratedBreakPoint(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsBreakPoint_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedBreakPoint<D, P>::TorqueGeneratedBreakPoint(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsBreakPoint_NonInline(*this));
}
template<class D, class P>
int TorqueGeneratedBreakPointInfo<D, P>::source_position() const {
  int value;
  value = TaggedField<Smi>::load(*this, kSourcePositionOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedBreakPointInfo<D, P>::set_source_position(int value) {
  WRITE_FIELD(*this, kSourcePositionOffset, Smi::FromInt(value));
}

template<class D, class P>
HeapObject TorqueGeneratedBreakPointInfo<D, P>::break_points() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedBreakPointInfo::break_points(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedBreakPointInfo<D, P>::break_points(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kBreakPointsOffset);
  DCHECK(value.IsUndefined() || value.IsFixedArray() || value.IsBreakPoint());
  return value;
}

template<class D, class P>
void TorqueGeneratedBreakPointInfo<D, P>::set_break_points(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFixedArray() || value.IsBreakPoint());
  WRITE_FIELD(*this, kBreakPointsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kBreakPointsOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedBreakPointInfo<D, P>::TorqueGeneratedBreakPointInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsBreakPointInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedBreakPointInfo<D, P>::TorqueGeneratedBreakPointInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsBreakPointInfo_NonInline(*this));
}
template<class D, class P>
SharedFunctionInfo TorqueGeneratedDebugInfo<D, P>::shared() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDebugInfo::shared(cage_base);
}

template<class D, class P>
SharedFunctionInfo TorqueGeneratedDebugInfo<D, P>::shared(PtrComprCageBase cage_base) const {
  SharedFunctionInfo value;
  value = TaggedField<SharedFunctionInfo>::load(cage_base, *this, kSharedOffset);
  DCHECK(value.IsSharedFunctionInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedDebugInfo<D, P>::set_shared(SharedFunctionInfo value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsSharedFunctionInfo());
  WRITE_FIELD(*this, kSharedOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSharedOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedDebugInfo<D, P>::debugger_hints() const {
  int value;
  value = TaggedField<Smi>::load(*this, kDebuggerHintsOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedDebugInfo<D, P>::set_debugger_hints(int value) {
  WRITE_FIELD(*this, kDebuggerHintsOffset, Smi::FromInt(value));
}

template<class D, class P>
HeapObject TorqueGeneratedDebugInfo<D, P>::script() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDebugInfo::script(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedDebugInfo<D, P>::script(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kScriptOffset);
  DCHECK(value.IsUndefined() || value.IsScript());
  return value;
}

template<class D, class P>
void TorqueGeneratedDebugInfo<D, P>::set_script(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsScript());
  WRITE_FIELD(*this, kScriptOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kScriptOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedDebugInfo<D, P>::original_bytecode_array(AcquireLoadTag) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDebugInfo::original_bytecode_array(cage_base, kAcquireLoad);
}

template<class D, class P>
HeapObject TorqueGeneratedDebugInfo<D, P>::original_bytecode_array(PtrComprCageBase cage_base, AcquireLoadTag) const {
  HeapObject value;
  value = TaggedField<HeapObject>::Acquire_Load(cage_base, *this, kOriginalBytecodeArrayOffset);
  DCHECK(value.IsUndefined() || value.IsBytecodeArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedDebugInfo<D, P>::set_original_bytecode_array(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsBytecodeArray());
  RELEASE_WRITE_FIELD(*this, kOriginalBytecodeArrayOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kOriginalBytecodeArrayOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedDebugInfo<D, P>::debug_bytecode_array(AcquireLoadTag) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDebugInfo::debug_bytecode_array(cage_base, kAcquireLoad);
}

template<class D, class P>
HeapObject TorqueGeneratedDebugInfo<D, P>::debug_bytecode_array(PtrComprCageBase cage_base, AcquireLoadTag) const {
  HeapObject value;
  value = TaggedField<HeapObject>::Acquire_Load(cage_base, *this, kDebugBytecodeArrayOffset);
  DCHECK(value.IsUndefined() || value.IsBytecodeArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedDebugInfo<D, P>::set_debug_bytecode_array(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsBytecodeArray());
  RELEASE_WRITE_FIELD(*this, kDebugBytecodeArrayOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kDebugBytecodeArrayOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedDebugInfo<D, P>::break_points() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDebugInfo::break_points(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedDebugInfo<D, P>::break_points(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kBreakPointsOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedDebugInfo<D, P>::set_break_points(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kBreakPointsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kBreakPointsOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedDebugInfo<D, P>::flags(RelaxedLoadTag) const {
  int value;
  value = TaggedField<Smi>::Relaxed_Load(*this, kFlagsOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedDebugInfo<D, P>::set_flags(int value, RelaxedStoreTag) {
  RELAXED_WRITE_FIELD(*this, kFlagsOffset, Smi::FromInt(value));
}

template<class D, class P>
HeapObject TorqueGeneratedDebugInfo<D, P>::coverage_info() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedDebugInfo::coverage_info(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedDebugInfo<D, P>::coverage_info(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kCoverageInfoOffset);
  DCHECK(value.IsUndefined() || value.IsCoverageInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedDebugInfo<D, P>::set_coverage_info(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsCoverageInfo());
  WRITE_FIELD(*this, kCoverageInfoOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCoverageInfoOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedDebugInfo<D, P>::TorqueGeneratedDebugInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsDebugInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedDebugInfo<D, P>::TorqueGeneratedDebugInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsDebugInfo_NonInline(*this));
}
template<class D, class P>
int32_t TorqueGeneratedCoverageInfo<D, P>::slot_count() const {
  int32_t value;
  value = this->template ReadField<int32_t>(kSlotCountOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedCoverageInfo<D, P>::set_slot_count(int32_t value) {
  this->template WriteField<int32_t>(kSlotCountOffset, value);
}

template<class D, class P>
int32_t TorqueGeneratedCoverageInfo<D, P>::slots_start_source_position(int i) const {
  int32_t value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->slot_count());
  int offset = kSlotsOffset + 0 + i * 16;
  value = this->template ReadField<int32_t>(offset);
  return value;
}

template<class D, class P>
void TorqueGeneratedCoverageInfo<D, P>::set_slots_start_source_position(int i, int32_t value) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->slot_count());
  int offset = kSlotsOffset + 0 + i * 16;
  this->template WriteField<int32_t>(offset, value);
}

template<class D, class P>
int32_t TorqueGeneratedCoverageInfo<D, P>::slots_end_source_position(int i) const {
  int32_t value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->slot_count());
  int offset = kSlotsOffset + 4 + i * 16;
  value = this->template ReadField<int32_t>(offset);
  return value;
}

template<class D, class P>
void TorqueGeneratedCoverageInfo<D, P>::set_slots_end_source_position(int i, int32_t value) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->slot_count());
  int offset = kSlotsOffset + 4 + i * 16;
  this->template WriteField<int32_t>(offset, value);
}

template<class D, class P>
int32_t TorqueGeneratedCoverageInfo<D, P>::slots_block_count(int i) const {
  int32_t value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->slot_count());
  int offset = kSlotsOffset + 8 + i * 16;
  value = this->template ReadField<int32_t>(offset);
  return value;
}

template<class D, class P>
void TorqueGeneratedCoverageInfo<D, P>::set_slots_block_count(int i, int32_t value) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->slot_count());
  int offset = kSlotsOffset + 8 + i * 16;
  this->template WriteField<int32_t>(offset, value);
}

template<class D, class P>
int32_t TorqueGeneratedCoverageInfo<D, P>::slots_padding(int i) const {
  int32_t value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->slot_count());
  int offset = kSlotsOffset + 12 + i * 16;
  value = this->template ReadField<int32_t>(offset);
  return value;
}

template<class D, class P>
void TorqueGeneratedCoverageInfo<D, P>::set_slots_padding(int i, int32_t value) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->slot_count());
  int offset = kSlotsOffset + 12 + i * 16;
  this->template WriteField<int32_t>(offset, value);
}

template<class D, class P>
inline TorqueGeneratedCoverageInfo<D, P>::TorqueGeneratedCoverageInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsCoverageInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedCoverageInfo<D, P>::TorqueGeneratedCoverageInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsCoverageInfo_NonInline(*this));
}
