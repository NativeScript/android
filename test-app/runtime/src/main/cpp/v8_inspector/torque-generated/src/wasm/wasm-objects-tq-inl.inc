template<class D, class P>
HeapObject TorqueGeneratedWasmFunctionData<D, P>::ref() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmFunctionData::ref(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmFunctionData<D, P>::ref(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kRefOffset);
  DCHECK(value.IsTuple2() || value.IsWasmInstanceObject());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmFunctionData<D, P>::set_ref(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsTuple2() || value.IsWasmInstanceObject());
  WRITE_FIELD(*this, kRefOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kRefOffset, value, mode);
}

template<class D, class P>
Code TorqueGeneratedWasmFunctionData<D, P>::wrapper_code() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmFunctionData::wrapper_code(cage_base);
}

template<class D, class P>
Code TorqueGeneratedWasmFunctionData<D, P>::wrapper_code(PtrComprCageBase cage_base) const {
  Code value;
  value = TaggedField<Code>::load(cage_base, *this, kWrapperCodeOffset);
  DCHECK(value.IsCode());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmFunctionData<D, P>::set_wrapper_code(Code value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsCode());
  WRITE_FIELD(*this, kWrapperCodeOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kWrapperCodeOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWasmFunctionData<D, P>::TorqueGeneratedWasmFunctionData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmFunctionData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmFunctionData<D, P>::TorqueGeneratedWasmFunctionData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmFunctionData_NonInline(*this));
}
template<class D, class P>
WasmInstanceObject TorqueGeneratedWasmExportedFunctionData<D, P>::instance() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmExportedFunctionData::instance(cage_base);
}

template<class D, class P>
WasmInstanceObject TorqueGeneratedWasmExportedFunctionData<D, P>::instance(PtrComprCageBase cage_base) const {
  WasmInstanceObject value;
  value = TaggedField<WasmInstanceObject>::load(cage_base, *this, kInstanceOffset);
  DCHECK(value.IsWasmInstanceObject());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmExportedFunctionData<D, P>::set_instance(WasmInstanceObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsWasmInstanceObject());
  WRITE_FIELD(*this, kInstanceOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kInstanceOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedWasmExportedFunctionData<D, P>::function_index() const {
  int value;
  value = TaggedField<Smi>::load(*this, kFunctionIndexOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmExportedFunctionData<D, P>::set_function_index(int value) {
  WRITE_FIELD(*this, kFunctionIndexOffset, Smi::FromInt(value));
}

template<class D, class P>
Foreign TorqueGeneratedWasmExportedFunctionData<D, P>::signature() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmExportedFunctionData::signature(cage_base);
}

template<class D, class P>
Foreign TorqueGeneratedWasmExportedFunctionData<D, P>::signature(PtrComprCageBase cage_base) const {
  Foreign value;
  value = TaggedField<Foreign>::load(cage_base, *this, kSignatureOffset);
  DCHECK(value.IsForeign());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmExportedFunctionData<D, P>::set_signature(Foreign value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsForeign());
  WRITE_FIELD(*this, kSignatureOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSignatureOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedWasmExportedFunctionData<D, P>::wrapper_budget() const {
  int value;
  value = TaggedField<Smi>::load(*this, kWrapperBudgetOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmExportedFunctionData<D, P>::set_wrapper_budget(int value) {
  WRITE_FIELD(*this, kWrapperBudgetOffset, Smi::FromInt(value));
}

template<class D, class P>
Code TorqueGeneratedWasmExportedFunctionData<D, P>::c_wrapper_code() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmExportedFunctionData::c_wrapper_code(cage_base);
}

template<class D, class P>
Code TorqueGeneratedWasmExportedFunctionData<D, P>::c_wrapper_code(PtrComprCageBase cage_base) const {
  Code value;
  value = TaggedField<Code>::load(cage_base, *this, kCWrapperCodeOffset);
  DCHECK(value.IsCode());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmExportedFunctionData<D, P>::set_c_wrapper_code(Code value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsCode());
  WRITE_FIELD(*this, kCWrapperCodeOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCWrapperCodeOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedWasmExportedFunctionData<D, P>::packed_args_size() const {
  int value;
  value = TaggedField<Smi>::load(*this, kPackedArgsSizeOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmExportedFunctionData<D, P>::set_packed_args_size(int value) {
  WRITE_FIELD(*this, kPackedArgsSizeOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedWasmExportedFunctionData<D, P>::TorqueGeneratedWasmExportedFunctionData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmExportedFunctionData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmExportedFunctionData<D, P>::TorqueGeneratedWasmExportedFunctionData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmExportedFunctionData_NonInline(*this));
}
template<class D, class P>
Code TorqueGeneratedWasmJSFunctionData<D, P>::wasm_to_js_wrapper_code() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmJSFunctionData::wasm_to_js_wrapper_code(cage_base);
}

template<class D, class P>
Code TorqueGeneratedWasmJSFunctionData<D, P>::wasm_to_js_wrapper_code(PtrComprCageBase cage_base) const {
  Code value;
  value = TaggedField<Code>::load(cage_base, *this, kWasmToJsWrapperCodeOffset);
  DCHECK(value.IsCode());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmJSFunctionData<D, P>::set_wasm_to_js_wrapper_code(Code value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsCode());
  WRITE_FIELD(*this, kWasmToJsWrapperCodeOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kWasmToJsWrapperCodeOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedWasmJSFunctionData<D, P>::serialized_return_count() const {
  int value;
  value = TaggedField<Smi>::load(*this, kSerializedReturnCountOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmJSFunctionData<D, P>::set_serialized_return_count(int value) {
  WRITE_FIELD(*this, kSerializedReturnCountOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedWasmJSFunctionData<D, P>::serialized_parameter_count() const {
  int value;
  value = TaggedField<Smi>::load(*this, kSerializedParameterCountOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmJSFunctionData<D, P>::set_serialized_parameter_count(int value) {
  WRITE_FIELD(*this, kSerializedParameterCountOffset, Smi::FromInt(value));
}

template<class D, class P>
PodArray<wasm::ValueType> TorqueGeneratedWasmJSFunctionData<D, P>::serialized_signature() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmJSFunctionData::serialized_signature(cage_base);
}

template<class D, class P>
PodArray<wasm::ValueType> TorqueGeneratedWasmJSFunctionData<D, P>::serialized_signature(PtrComprCageBase cage_base) const {
  PodArray<wasm::ValueType> value;
  value = TaggedField<PodArray<wasm::ValueType>>::load(cage_base, *this, kSerializedSignatureOffset);
  DCHECK(value.IsByteArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmJSFunctionData<D, P>::set_serialized_signature(PodArray<wasm::ValueType> value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsByteArray());
  WRITE_FIELD(*this, kSerializedSignatureOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSerializedSignatureOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWasmJSFunctionData<D, P>::TorqueGeneratedWasmJSFunctionData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmJSFunctionData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmJSFunctionData<D, P>::TorqueGeneratedWasmJSFunctionData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmJSFunctionData_NonInline(*this));
}
template<class D, class P>
Foreign TorqueGeneratedWasmCapiFunctionData<D, P>::embedder_data() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmCapiFunctionData::embedder_data(cage_base);
}

template<class D, class P>
Foreign TorqueGeneratedWasmCapiFunctionData<D, P>::embedder_data(PtrComprCageBase cage_base) const {
  Foreign value;
  value = TaggedField<Foreign>::load(cage_base, *this, kEmbedderDataOffset);
  DCHECK(value.IsForeign());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmCapiFunctionData<D, P>::set_embedder_data(Foreign value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsForeign());
  WRITE_FIELD(*this, kEmbedderDataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kEmbedderDataOffset, value, mode);
}

template<class D, class P>
PodArray<wasm::ValueType> TorqueGeneratedWasmCapiFunctionData<D, P>::serialized_signature() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmCapiFunctionData::serialized_signature(cage_base);
}

template<class D, class P>
PodArray<wasm::ValueType> TorqueGeneratedWasmCapiFunctionData<D, P>::serialized_signature(PtrComprCageBase cage_base) const {
  PodArray<wasm::ValueType> value;
  value = TaggedField<PodArray<wasm::ValueType>>::load(cage_base, *this, kSerializedSignatureOffset);
  DCHECK(value.IsByteArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmCapiFunctionData<D, P>::set_serialized_signature(PodArray<wasm::ValueType> value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsByteArray());
  WRITE_FIELD(*this, kSerializedSignatureOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSerializedSignatureOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWasmCapiFunctionData<D, P>::TorqueGeneratedWasmCapiFunctionData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmCapiFunctionData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmCapiFunctionData<D, P>::TorqueGeneratedWasmCapiFunctionData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmCapiFunctionData_NonInline(*this));
}
template<class D, class P>
uint32_t TorqueGeneratedWasmIndirectFunctionTable<D, P>::size() const {
  uint32_t value;
  value = this->template ReadField<uint32_t>(kSizeOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmIndirectFunctionTable<D, P>::set_size(uint32_t value) {
  this->template WriteField<uint32_t>(kSizeOffset, value);
}

template<class D, class P>
Address TorqueGeneratedWasmIndirectFunctionTable<D, P>::sig_ids() const {
  Address value;
  value = this->template ReadField<Address>(kSigIdsOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmIndirectFunctionTable<D, P>::set_sig_ids(Address value) {
  this->template WriteField<Address>(kSigIdsOffset, value);
}

template<class D, class P>
Address TorqueGeneratedWasmIndirectFunctionTable<D, P>::targets() const {
  Address value;
  value = this->template ReadField<Address>(kTargetsOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmIndirectFunctionTable<D, P>::set_targets(Address value) {
  this->template WriteField<Address>(kTargetsOffset, value);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmIndirectFunctionTable<D, P>::managed_native_allocations() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmIndirectFunctionTable::managed_native_allocations(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmIndirectFunctionTable<D, P>::managed_native_allocations(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kManagedNativeAllocationsOffset);
  DCHECK(value.IsUndefined() || value.IsForeign());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmIndirectFunctionTable<D, P>::set_managed_native_allocations(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsForeign());
  WRITE_FIELD(*this, kManagedNativeAllocationsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kManagedNativeAllocationsOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedWasmIndirectFunctionTable<D, P>::refs() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmIndirectFunctionTable::refs(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedWasmIndirectFunctionTable<D, P>::refs(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kRefsOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmIndirectFunctionTable<D, P>::set_refs(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kRefsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kRefsOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWasmIndirectFunctionTable<D, P>::TorqueGeneratedWasmIndirectFunctionTable(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmIndirectFunctionTable_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmIndirectFunctionTable<D, P>::TorqueGeneratedWasmIndirectFunctionTable(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmIndirectFunctionTable_NonInline(*this));
}
template<class D, class P>
int TorqueGeneratedWasmExceptionTag<D, P>::index() const {
  int value;
  value = TaggedField<Smi>::load(*this, kIndexOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmExceptionTag<D, P>::set_index(int value) {
  WRITE_FIELD(*this, kIndexOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedWasmExceptionTag<D, P>::TorqueGeneratedWasmExceptionTag(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmExceptionTag_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmExceptionTag<D, P>::TorqueGeneratedWasmExceptionTag(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmExceptionTag_NonInline(*this));
}
template<class D, class P>
Managed<wasm::NativeModule> TorqueGeneratedWasmModuleObject<D, P>::managed_native_module() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmModuleObject::managed_native_module(cage_base);
}

template<class D, class P>
Managed<wasm::NativeModule> TorqueGeneratedWasmModuleObject<D, P>::managed_native_module(PtrComprCageBase cage_base) const {
  Managed<wasm::NativeModule> value;
  value = TaggedField<Managed<wasm::NativeModule>>::load(cage_base, *this, kManagedNativeModuleOffset);
  DCHECK(value.IsForeign());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmModuleObject<D, P>::set_managed_native_module(Managed<wasm::NativeModule> value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsForeign());
  WRITE_FIELD(*this, kManagedNativeModuleOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kManagedNativeModuleOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedWasmModuleObject<D, P>::export_wrappers() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmModuleObject::export_wrappers(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedWasmModuleObject<D, P>::export_wrappers(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kExportWrappersOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmModuleObject<D, P>::set_export_wrappers(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kExportWrappersOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kExportWrappersOffset, value, mode);
}

template<class D, class P>
Script TorqueGeneratedWasmModuleObject<D, P>::script() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmModuleObject::script(cage_base);
}

template<class D, class P>
Script TorqueGeneratedWasmModuleObject<D, P>::script(PtrComprCageBase cage_base) const {
  Script value;
  value = TaggedField<Script>::load(cage_base, *this, kScriptOffset);
  DCHECK(value.IsScript());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmModuleObject<D, P>::set_script(Script value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsScript());
  WRITE_FIELD(*this, kScriptOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kScriptOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWasmModuleObject<D, P>::TorqueGeneratedWasmModuleObject(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmModuleObject_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmModuleObject<D, P>::TorqueGeneratedWasmModuleObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmModuleObject_NonInline(*this));
}
template<class D, class P>
HeapObject TorqueGeneratedWasmTableObject<D, P>::instance() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmTableObject::instance(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmTableObject<D, P>::instance(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kInstanceOffset);
  DCHECK(value.IsUndefined() || value.IsWasmInstanceObject());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmTableObject<D, P>::set_instance(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsWasmInstanceObject());
  WRITE_FIELD(*this, kInstanceOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kInstanceOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedWasmTableObject<D, P>::entries() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmTableObject::entries(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedWasmTableObject<D, P>::entries(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kEntriesOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmTableObject<D, P>::set_entries(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kEntriesOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kEntriesOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedWasmTableObject<D, P>::current_length() const {
  int value;
  value = TaggedField<Smi>::load(*this, kCurrentLengthOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmTableObject<D, P>::set_current_length(int value) {
  WRITE_FIELD(*this, kCurrentLengthOffset, Smi::FromInt(value));
}

template<class D, class P>
Object TorqueGeneratedWasmTableObject<D, P>::maximum_length() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmTableObject::maximum_length(cage_base);
}

template<class D, class P>
Object TorqueGeneratedWasmTableObject<D, P>::maximum_length(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kMaximumLengthOffset);
  DCHECK(value.IsSmi() || value.IsHeapNumber() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmTableObject<D, P>::set_maximum_length(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsSmi() || value.IsHeapNumber() || value.IsUndefined());
  WRITE_FIELD(*this, kMaximumLengthOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kMaximumLengthOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedWasmTableObject<D, P>::dispatch_tables() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmTableObject::dispatch_tables(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedWasmTableObject<D, P>::dispatch_tables(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kDispatchTablesOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmTableObject<D, P>::set_dispatch_tables(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kDispatchTablesOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kDispatchTablesOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedWasmTableObject<D, P>::raw_type() const {
  int value;
  value = TaggedField<Smi>::load(*this, kRawTypeOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmTableObject<D, P>::set_raw_type(int value) {
  WRITE_FIELD(*this, kRawTypeOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedWasmTableObject<D, P>::TorqueGeneratedWasmTableObject(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmTableObject_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmTableObject<D, P>::TorqueGeneratedWasmTableObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmTableObject_NonInline(*this));
}
template<class D, class P>
JSArrayBuffer TorqueGeneratedWasmMemoryObject<D, P>::array_buffer() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmMemoryObject::array_buffer(cage_base);
}

template<class D, class P>
JSArrayBuffer TorqueGeneratedWasmMemoryObject<D, P>::array_buffer(PtrComprCageBase cage_base) const {
  JSArrayBuffer value;
  value = TaggedField<JSArrayBuffer>::load(cage_base, *this, kArrayBufferOffset);
  DCHECK(value.IsJSArrayBuffer());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmMemoryObject<D, P>::set_array_buffer(JSArrayBuffer value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSArrayBuffer());
  WRITE_FIELD(*this, kArrayBufferOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kArrayBufferOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedWasmMemoryObject<D, P>::maximum_pages() const {
  int value;
  value = TaggedField<Smi>::load(*this, kMaximumPagesOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmMemoryObject<D, P>::set_maximum_pages(int value) {
  WRITE_FIELD(*this, kMaximumPagesOffset, Smi::FromInt(value));
}

template<class D, class P>
HeapObject TorqueGeneratedWasmMemoryObject<D, P>::instances() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmMemoryObject::instances(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmMemoryObject<D, P>::instances(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kInstancesOffset);
  DCHECK(value.IsUndefined() || value.IsWeakArrayList());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmMemoryObject<D, P>::set_instances(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsWeakArrayList());
  WRITE_FIELD(*this, kInstancesOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kInstancesOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWasmMemoryObject<D, P>::TorqueGeneratedWasmMemoryObject(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmMemoryObject_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmMemoryObject<D, P>::TorqueGeneratedWasmMemoryObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmMemoryObject_NonInline(*this));
}
template<class D, class P>
HeapObject TorqueGeneratedWasmGlobalObject<D, P>::instance() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmGlobalObject::instance(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmGlobalObject<D, P>::instance(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kInstanceOffset);
  DCHECK(value.IsUndefined() || value.IsWasmInstanceObject());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmGlobalObject<D, P>::set_instance(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsWasmInstanceObject());
  WRITE_FIELD(*this, kInstanceOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kInstanceOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmGlobalObject<D, P>::untagged_buffer() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmGlobalObject::untagged_buffer(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmGlobalObject<D, P>::untagged_buffer(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kUntaggedBufferOffset);
  DCHECK(value.IsUndefined() || value.IsJSArrayBuffer());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmGlobalObject<D, P>::set_untagged_buffer(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsJSArrayBuffer());
  WRITE_FIELD(*this, kUntaggedBufferOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kUntaggedBufferOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmGlobalObject<D, P>::tagged_buffer() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmGlobalObject::tagged_buffer(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmGlobalObject<D, P>::tagged_buffer(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kTaggedBufferOffset);
  DCHECK(value.IsUndefined() || value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmGlobalObject<D, P>::set_tagged_buffer(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFixedArray());
  WRITE_FIELD(*this, kTaggedBufferOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kTaggedBufferOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedWasmGlobalObject<D, P>::offset() const {
  int value;
  value = TaggedField<Smi>::load(*this, kOffsetOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmGlobalObject<D, P>::set_offset(int value) {
  WRITE_FIELD(*this, kOffsetOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedWasmGlobalObject<D, P>::raw_type() const {
  int value;
  value = TaggedField<Smi>::load(*this, kRawTypeOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmGlobalObject<D, P>::set_raw_type(int value) {
  WRITE_FIELD(*this, kRawTypeOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedWasmGlobalObject<D, P>::is_mutable() const {
  int value;
  value = TaggedField<Smi>::load(*this, kIsMutableOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmGlobalObject<D, P>::set_is_mutable(int value) {
  WRITE_FIELD(*this, kIsMutableOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedWasmGlobalObject<D, P>::TorqueGeneratedWasmGlobalObject(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmGlobalObject_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmGlobalObject<D, P>::TorqueGeneratedWasmGlobalObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmGlobalObject_NonInline(*this));
}
template<class D, class P>
PodArray<wasm::ValueType> TorqueGeneratedWasmExceptionObject<D, P>::serialized_signature() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmExceptionObject::serialized_signature(cage_base);
}

template<class D, class P>
PodArray<wasm::ValueType> TorqueGeneratedWasmExceptionObject<D, P>::serialized_signature(PtrComprCageBase cage_base) const {
  PodArray<wasm::ValueType> value;
  value = TaggedField<PodArray<wasm::ValueType>>::load(cage_base, *this, kSerializedSignatureOffset);
  DCHECK(value.IsByteArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmExceptionObject<D, P>::set_serialized_signature(PodArray<wasm::ValueType> value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsByteArray());
  WRITE_FIELD(*this, kSerializedSignatureOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSerializedSignatureOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmExceptionObject<D, P>::exception_tag() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmExceptionObject::exception_tag(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedWasmExceptionObject<D, P>::exception_tag(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kExceptionTagOffset);
  DCHECK(value.IsHeapObject());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmExceptionObject<D, P>::set_exception_tag(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject());
  WRITE_FIELD(*this, kExceptionTagOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kExceptionTagOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedWasmExceptionObject<D, P>::TorqueGeneratedWasmExceptionObject(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmExceptionObject_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmExceptionObject<D, P>::TorqueGeneratedWasmExceptionObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmExceptionObject_NonInline(*this));
}
template<class D, class P>
Managed<wasm::NativeModule> TorqueGeneratedAsmWasmData<D, P>::managed_native_module() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedAsmWasmData::managed_native_module(cage_base);
}

template<class D, class P>
Managed<wasm::NativeModule> TorqueGeneratedAsmWasmData<D, P>::managed_native_module(PtrComprCageBase cage_base) const {
  Managed<wasm::NativeModule> value;
  value = TaggedField<Managed<wasm::NativeModule>>::load(cage_base, *this, kManagedNativeModuleOffset);
  DCHECK(value.IsForeign());
  return value;
}

template<class D, class P>
void TorqueGeneratedAsmWasmData<D, P>::set_managed_native_module(Managed<wasm::NativeModule> value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsForeign());
  WRITE_FIELD(*this, kManagedNativeModuleOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kManagedNativeModuleOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedAsmWasmData<D, P>::export_wrappers() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedAsmWasmData::export_wrappers(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedAsmWasmData<D, P>::export_wrappers(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kExportWrappersOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedAsmWasmData<D, P>::set_export_wrappers(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kExportWrappersOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kExportWrappersOffset, value, mode);
}

template<class D, class P>
HeapNumber TorqueGeneratedAsmWasmData<D, P>::uses_bitset() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedAsmWasmData::uses_bitset(cage_base);
}

template<class D, class P>
HeapNumber TorqueGeneratedAsmWasmData<D, P>::uses_bitset(PtrComprCageBase cage_base) const {
  HeapNumber value;
  value = TaggedField<HeapNumber>::load(cage_base, *this, kUsesBitsetOffset);
  DCHECK(value.IsHeapNumber());
  return value;
}

template<class D, class P>
void TorqueGeneratedAsmWasmData<D, P>::set_uses_bitset(HeapNumber value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapNumber());
  WRITE_FIELD(*this, kUsesBitsetOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kUsesBitsetOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedAsmWasmData<D, P>::TorqueGeneratedAsmWasmData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsAsmWasmData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedAsmWasmData<D, P>::TorqueGeneratedAsmWasmData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsAsmWasmData_NonInline(*this));
}
template<class D, class P>
FixedArray TorqueGeneratedWasmTypeInfo<D, P>::supertypes() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmTypeInfo::supertypes(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedWasmTypeInfo<D, P>::supertypes(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kSupertypesOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmTypeInfo<D, P>::set_supertypes(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kSupertypesOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSupertypesOffset, value, mode);
}

template<class D, class P>
ArrayList TorqueGeneratedWasmTypeInfo<D, P>::subtypes() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedWasmTypeInfo::subtypes(cage_base);
}

template<class D, class P>
ArrayList TorqueGeneratedWasmTypeInfo<D, P>::subtypes(PtrComprCageBase cage_base) const {
  ArrayList value;
  value = TaggedField<ArrayList>::load(cage_base, *this, kSubtypesOffset);
  DCHECK(value.IsArrayList());
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmTypeInfo<D, P>::set_subtypes(ArrayList value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsArrayList());
  WRITE_FIELD(*this, kSubtypesOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSubtypesOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedWasmTypeInfo<D, P>::instance_size() const {
  int value;
  value = TaggedField<Smi>::load(*this, kInstanceSizeOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmTypeInfo<D, P>::set_instance_size(int value) {
  WRITE_FIELD(*this, kInstanceSizeOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedWasmTypeInfo<D, P>::TorqueGeneratedWasmTypeInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmTypeInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmTypeInfo<D, P>::TorqueGeneratedWasmTypeInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmTypeInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmObject<D, P>::TorqueGeneratedWasmObject(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmObject_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmObject<D, P>::TorqueGeneratedWasmObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmObject_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmStruct<D, P>::TorqueGeneratedWasmStruct(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmStruct_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmStruct<D, P>::TorqueGeneratedWasmStruct(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmStruct_NonInline(*this));
}
template<class D, class P>
uint32_t TorqueGeneratedWasmArray<D, P>::length() const {
  uint32_t value;
  value = this->template ReadField<uint32_t>(kLengthOffset);
  return value;
}

template<class D, class P>
void TorqueGeneratedWasmArray<D, P>::set_length(uint32_t value) {
  this->template WriteField<uint32_t>(kLengthOffset, value);
}

template<class D, class P>
inline TorqueGeneratedWasmArray<D, P>::TorqueGeneratedWasmArray(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsWasmArray_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedWasmArray<D, P>::TorqueGeneratedWasmArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsWasmArray_NonInline(*this));
}
