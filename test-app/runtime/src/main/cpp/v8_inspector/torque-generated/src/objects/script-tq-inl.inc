template<class D, class P>
PrimitiveHeapObject TorqueGeneratedScript<D, P>::source() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::source(cage_base);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedScript<D, P>::source(PtrComprCageBase cage_base) const {
  PrimitiveHeapObject value;
  value = TaggedField<PrimitiveHeapObject>::load(cage_base, *this, kSourceOffset);
  DCHECK(value.IsString() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_source(PrimitiveHeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString() || value.IsUndefined());
  WRITE_FIELD(*this, kSourceOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSourceOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedScript<D, P>::name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::name(cage_base);
}

template<class D, class P>
Object TorqueGeneratedScript<D, P>::name(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kNameOffset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_name(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, kNameOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kNameOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedScript<D, P>::line_offset() const {
  int value;
  value = TaggedField<Smi>::load(*this, kLineOffsetOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_line_offset(int value) {
  WRITE_FIELD(*this, kLineOffsetOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedScript<D, P>::column_offset() const {
  int value;
  value = TaggedField<Smi>::load(*this, kColumnOffsetOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_column_offset(int value) {
  WRITE_FIELD(*this, kColumnOffsetOffset, Smi::FromInt(value));
}

template<class D, class P>
Object TorqueGeneratedScript<D, P>::context_data() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::context_data(cage_base);
}

template<class D, class P>
Object TorqueGeneratedScript<D, P>::context_data(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kContextDataOffset);
  DCHECK(value.IsSmi() || value.IsSymbol() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_context_data(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsSmi() || value.IsSymbol() || value.IsUndefined());
  WRITE_FIELD(*this, kContextDataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kContextDataOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedScript<D, P>::script_type() const {
  int value;
  value = TaggedField<Smi>::load(*this, kScriptTypeOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_script_type(int value) {
  WRITE_FIELD(*this, kScriptTypeOffset, Smi::FromInt(value));
}

template<class D, class P>
HeapObject TorqueGeneratedScript<D, P>::line_ends() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::line_ends(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedScript<D, P>::line_ends(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kLineEndsOffset);
  DCHECK(value.IsUndefined() || value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_line_ends(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFixedArray());
  WRITE_FIELD(*this, kLineEndsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kLineEndsOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedScript<D, P>::id() const {
  int value;
  value = TaggedField<Smi>::load(*this, kIdOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_id(int value) {
  WRITE_FIELD(*this, kIdOffset, Smi::FromInt(value));
}

template<class D, class P>
HeapObject TorqueGeneratedScript<D, P>::eval_from_shared_or_wrapped_arguments_or_sfi_table() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::eval_from_shared_or_wrapped_arguments_or_sfi_table(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedScript<D, P>::eval_from_shared_or_wrapped_arguments_or_sfi_table(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kEvalFromSharedOrWrappedArgumentsOrSfiTableOffset);
  DCHECK(value.IsUndefined() || value.IsFixedArray() || value.IsSharedFunctionInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_eval_from_shared_or_wrapped_arguments_or_sfi_table(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFixedArray() || value.IsSharedFunctionInfo());
  WRITE_FIELD(*this, kEvalFromSharedOrWrappedArgumentsOrSfiTableOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kEvalFromSharedOrWrappedArgumentsOrSfiTableOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedScript<D, P>::eval_from_position() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::eval_from_position(cage_base);
}

template<class D, class P>
Object TorqueGeneratedScript<D, P>::eval_from_position(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kEvalFromPositionOffset);
  DCHECK(value.IsSmi() || value.IsForeign());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_eval_from_position(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsSmi() || value.IsForeign());
  WRITE_FIELD(*this, kEvalFromPositionOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kEvalFromPositionOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedScript<D, P>::shared_function_infos() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::shared_function_infos(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedScript<D, P>::shared_function_infos(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kSharedFunctionInfosOffset);
  DCHECK(value.IsWeakFixedArray() || value.IsWeakArrayList());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_shared_function_infos(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsWeakFixedArray() || value.IsWeakArrayList());
  WRITE_FIELD(*this, kSharedFunctionInfosOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSharedFunctionInfosOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedScript<D, P>::flags() const {
  int value;
  value = TaggedField<Smi>::load(*this, kFlagsOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_flags(int value) {
  WRITE_FIELD(*this, kFlagsOffset, Smi::FromInt(value));
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedScript<D, P>::source_url() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::source_url(cage_base);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedScript<D, P>::source_url(PtrComprCageBase cage_base) const {
  PrimitiveHeapObject value;
  value = TaggedField<PrimitiveHeapObject>::load(cage_base, *this, kSourceUrlOffset);
  DCHECK(value.IsString() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_source_url(PrimitiveHeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString() || value.IsUndefined());
  WRITE_FIELD(*this, kSourceUrlOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSourceUrlOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedScript<D, P>::source_mapping_url() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::source_mapping_url(cage_base);
}

template<class D, class P>
Object TorqueGeneratedScript<D, P>::source_mapping_url(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kSourceMappingUrlOffset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_source_mapping_url(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, kSourceMappingUrlOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSourceMappingUrlOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedScript<D, P>::host_defined_options() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScript::host_defined_options(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedScript<D, P>::host_defined_options(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kHostDefinedOptionsOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedScript<D, P>::set_host_defined_options(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kHostDefinedOptionsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kHostDefinedOptionsOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedScript<D, P>::TorqueGeneratedScript(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsScript_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedScript<D, P>::TorqueGeneratedScript(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsScript_NonInline(*this));
}
