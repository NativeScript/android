template<class D, class P>
int32_t TorqueGeneratedPreparseData<D, P>::data_length() const {
  int32_t value;
  value = this->template ReadField<int32_t>(kDataLengthOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedPreparseData<D, P>::set_data_length(int32_t value) {
  this->template WriteField<int32_t>(kDataLengthOffset, value);
}

template<class D, class P>
int32_t TorqueGeneratedPreparseData<D, P>::children_length() const {
  int32_t value;
  value = this->template ReadField<int32_t>(kChildrenLengthOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedPreparseData<D, P>::set_children_length(int32_t value) {
  this->template WriteField<int32_t>(kChildrenLengthOffset, value);
}

template<class D, class P>
inline TorqueGeneratedPreparseData<D, P>::TorqueGeneratedPreparseData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsPreparseData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedPreparseData<D, P>::TorqueGeneratedPreparseData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsPreparseData_NonInline(*this));
}
template<class D, class P>
Code TorqueGeneratedBaselineData<D, P>::baseline_code() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedBaselineData::baseline_code(cage_base);
}

template<class D, class P>
Code TorqueGeneratedBaselineData<D, P>::baseline_code(PtrComprCageBase cage_base) const {
  Code value;
  value = TaggedField<Code>::load(cage_base, *this, kBaselineCodeOffset);
  DCHECK(value.IsCode());
  return value;
}

template<class D, class P>
void TorqueGeneratedBaselineData<D, P>::set_baseline_code(Code value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsCode());
  WRITE_FIELD(*this, kBaselineCodeOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kBaselineCodeOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedBaselineData<D, P>::data() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedBaselineData::data(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedBaselineData<D, P>::data(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kDataOffset);
  DCHECK(value.IsBytecodeArray() || value.IsInterpreterData());
  return value;
}

template<class D, class P>
void TorqueGeneratedBaselineData<D, P>::set_data(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsBytecodeArray() || value.IsInterpreterData());
  WRITE_FIELD(*this, kDataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kDataOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedBaselineData<D, P>::TorqueGeneratedBaselineData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsBaselineData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedBaselineData<D, P>::TorqueGeneratedBaselineData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsBaselineData_NonInline(*this));
}
template<class D, class P>
Object TorqueGeneratedSharedFunctionInfo<D, P>::function_data() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSharedFunctionInfo::function_data(cage_base);
}

template<class D, class P>
Object TorqueGeneratedSharedFunctionInfo<D, P>::function_data(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kFunctionDataOffset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_function_data(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, kFunctionDataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kFunctionDataOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedSharedFunctionInfo<D, P>::name_or_scope_info() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSharedFunctionInfo::name_or_scope_info(cage_base);
}

template<class D, class P>
Object TorqueGeneratedSharedFunctionInfo<D, P>::name_or_scope_info(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kNameOrScopeInfoOffset);
  DCHECK(value.IsString() || value.IsNoSharedNameSentinel() || value.IsScopeInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_name_or_scope_info(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString() || value.IsNoSharedNameSentinel() || value.IsScopeInfo());
  WRITE_FIELD(*this, kNameOrScopeInfoOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kNameOrScopeInfoOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedSharedFunctionInfo<D, P>::outer_scope_info_or_feedback_metadata() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSharedFunctionInfo::outer_scope_info_or_feedback_metadata(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedSharedFunctionInfo<D, P>::outer_scope_info_or_feedback_metadata(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kOuterScopeInfoOrFeedbackMetadataOffset);
  DCHECK(value.IsHeapObject());
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_outer_scope_info_or_feedback_metadata(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject());
  WRITE_FIELD(*this, kOuterScopeInfoOrFeedbackMetadataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kOuterScopeInfoOrFeedbackMetadataOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedSharedFunctionInfo<D, P>::script_or_debug_info() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSharedFunctionInfo::script_or_debug_info(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedSharedFunctionInfo<D, P>::script_or_debug_info(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kScriptOrDebugInfoOffset);
  DCHECK(value.IsUndefined() || value.IsDebugInfo() || value.IsScript());
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_script_or_debug_info(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsDebugInfo() || value.IsScript());
  WRITE_FIELD(*this, kScriptOrDebugInfoOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kScriptOrDebugInfoOffset, value, mode);
}

template<class D, class P>
int16_t TorqueGeneratedSharedFunctionInfo<D, P>::length() const {
  int16_t value;
  value = this->template ReadField<int16_t>(kLengthOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_length(int16_t value) {
  this->template WriteField<int16_t>(kLengthOffset, value);
}

template<class D, class P>
uint16_t TorqueGeneratedSharedFunctionInfo<D, P>::formal_parameter_count() const {
  uint16_t value;
  value = this->template ReadField<uint16_t>(kFormalParameterCountOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_formal_parameter_count(uint16_t value) {
  this->template WriteField<uint16_t>(kFormalParameterCountOffset, value);
}

template<class D, class P>
uint16_t TorqueGeneratedSharedFunctionInfo<D, P>::function_token_offset() const {
  uint16_t value;
  value = this->template ReadField<uint16_t>(kFunctionTokenOffsetOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_function_token_offset(uint16_t value) {
  this->template WriteField<uint16_t>(kFunctionTokenOffsetOffset, value);
}

template<class D, class P>
uint8_t TorqueGeneratedSharedFunctionInfo<D, P>::expected_nof_properties() const {
  uint8_t value;
  value = this->template ReadField<uint8_t>(kExpectedNofPropertiesOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_expected_nof_properties(uint8_t value) {
  this->template WriteField<uint8_t>(kExpectedNofPropertiesOffset, value);
}

template<class D, class P>
uint8_t TorqueGeneratedSharedFunctionInfo<D, P>::flags2() const {
  uint8_t value;
  value = this->template ReadField<uint8_t>(kFlags2Offset);
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_flags2(uint8_t value) {
  this->template WriteField<uint8_t>(kFlags2Offset, value);
}

template<class D, class P>
uint32_t TorqueGeneratedSharedFunctionInfo<D, P>::flags() const {
  uint32_t value;
  value = this->template ReadField<uint32_t>(kFlagsOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_flags(uint32_t value) {
  this->template WriteField<uint32_t>(kFlagsOffset, value);
}

template<class D, class P>
int32_t TorqueGeneratedSharedFunctionInfo<D, P>::function_literal_id() const {
  int32_t value;
  value = this->template ReadField<int32_t>(kFunctionLiteralIdOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedSharedFunctionInfo<D, P>::set_function_literal_id(int32_t value) {
  this->template WriteField<int32_t>(kFunctionLiteralIdOffset, value);
}

template<class D, class P>
inline TorqueGeneratedSharedFunctionInfo<D, P>::TorqueGeneratedSharedFunctionInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsSharedFunctionInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedSharedFunctionInfo<D, P>::TorqueGeneratedSharedFunctionInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsSharedFunctionInfo_NonInline(*this));
}
template<class D, class P>
String TorqueGeneratedUncompiledData<D, P>::inferred_name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedUncompiledData::inferred_name(cage_base);
}

template<class D, class P>
String TorqueGeneratedUncompiledData<D, P>::inferred_name(PtrComprCageBase cage_base) const {
  String value;
  value = TaggedField<String>::load(cage_base, *this, kInferredNameOffset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedUncompiledData<D, P>::set_inferred_name(String value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, kInferredNameOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kInferredNameOffset, value, mode);
}

template<class D, class P>
int32_t TorqueGeneratedUncompiledData<D, P>::start_position() const {
  int32_t value;
  value = this->template ReadField<int32_t>(kStartPositionOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedUncompiledData<D, P>::set_start_position(int32_t value) {
  this->template WriteField<int32_t>(kStartPositionOffset, value);
}

template<class D, class P>
int32_t TorqueGeneratedUncompiledData<D, P>::end_position() const {
  int32_t value;
  value = this->template ReadField<int32_t>(kEndPositionOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedUncompiledData<D, P>::set_end_position(int32_t value) {
  this->template WriteField<int32_t>(kEndPositionOffset, value);
}

template<class D, class P>
inline TorqueGeneratedUncompiledData<D, P>::TorqueGeneratedUncompiledData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsUncompiledData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedUncompiledData<D, P>::TorqueGeneratedUncompiledData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsUncompiledData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedUncompiledDataWithoutPreparseData<D, P>::TorqueGeneratedUncompiledDataWithoutPreparseData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsUncompiledDataWithoutPreparseData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedUncompiledDataWithoutPreparseData<D, P>::TorqueGeneratedUncompiledDataWithoutPreparseData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsUncompiledDataWithoutPreparseData_NonInline(*this));
}
template<class D, class P>
PreparseData TorqueGeneratedUncompiledDataWithPreparseData<D, P>::preparse_data() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedUncompiledDataWithPreparseData::preparse_data(cage_base);
}

template<class D, class P>
PreparseData TorqueGeneratedUncompiledDataWithPreparseData<D, P>::preparse_data(PtrComprCageBase cage_base) const {
  PreparseData value;
  value = TaggedField<PreparseData>::load(cage_base, *this, kPreparseDataOffset);
  DCHECK(value.IsPreparseData());
  return value;
}

template<class D, class P>
void TorqueGeneratedUncompiledDataWithPreparseData<D, P>::set_preparse_data(PreparseData value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsPreparseData());
  WRITE_FIELD(*this, kPreparseDataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPreparseDataOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedUncompiledDataWithPreparseData<D, P>::TorqueGeneratedUncompiledDataWithPreparseData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsUncompiledDataWithPreparseData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedUncompiledDataWithPreparseData<D, P>::TorqueGeneratedUncompiledDataWithPreparseData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsUncompiledDataWithPreparseData_NonInline(*this));
}
template<class D, class P>
ByteArray TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::block_ids() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedOnHeapBasicBlockProfilerData::block_ids(cage_base);
}

template<class D, class P>
ByteArray TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::block_ids(PtrComprCageBase cage_base) const {
  ByteArray value;
  value = TaggedField<ByteArray>::load(cage_base, *this, kBlockIdsOffset);
  DCHECK(value.IsByteArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::set_block_ids(ByteArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsByteArray());
  WRITE_FIELD(*this, kBlockIdsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kBlockIdsOffset, value, mode);
}

template<class D, class P>
ByteArray TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::counts() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedOnHeapBasicBlockProfilerData::counts(cage_base);
}

template<class D, class P>
ByteArray TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::counts(PtrComprCageBase cage_base) const {
  ByteArray value;
  value = TaggedField<ByteArray>::load(cage_base, *this, kCountsOffset);
  DCHECK(value.IsByteArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::set_counts(ByteArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsByteArray());
  WRITE_FIELD(*this, kCountsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCountsOffset, value, mode);
}

template<class D, class P>
String TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedOnHeapBasicBlockProfilerData::name(cage_base);
}

template<class D, class P>
String TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::name(PtrComprCageBase cage_base) const {
  String value;
  value = TaggedField<String>::load(cage_base, *this, kNameOffset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::set_name(String value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, kNameOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kNameOffset, value, mode);
}

template<class D, class P>
String TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::schedule() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedOnHeapBasicBlockProfilerData::schedule(cage_base);
}

template<class D, class P>
String TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::schedule(PtrComprCageBase cage_base) const {
  String value;
  value = TaggedField<String>::load(cage_base, *this, kScheduleOffset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::set_schedule(String value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, kScheduleOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kScheduleOffset, value, mode);
}

template<class D, class P>
String TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::code() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedOnHeapBasicBlockProfilerData::code(cage_base);
}

template<class D, class P>
String TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::code(PtrComprCageBase cage_base) const {
  String value;
  value = TaggedField<String>::load(cage_base, *this, kCodeOffset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::set_code(String value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, kCodeOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCodeOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::hash() const {
  int value;
  value = TaggedField<Smi>::load(*this, kHashOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::set_hash(int value) {
  WRITE_FIELD(*this, kHashOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::TorqueGeneratedOnHeapBasicBlockProfilerData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsOnHeapBasicBlockProfilerData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedOnHeapBasicBlockProfilerData<D, P>::TorqueGeneratedOnHeapBasicBlockProfilerData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsOnHeapBasicBlockProfilerData_NonInline(*this));
}
TQ_OBJECT_CONSTRUCTORS_IMPL(OnHeapBasicBlockProfilerData)
