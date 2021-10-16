template<class D, class P>
HeapObject TorqueGeneratedSourceTextModule<D, P>::code() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModule::code(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedSourceTextModule<D, P>::code(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kCodeOffset);
  DCHECK(value.IsJSFunction() || value.IsJSGeneratorObject() || value.IsSharedFunctionInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_code(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSFunction() || value.IsJSGeneratorObject() || value.IsSharedFunctionInfo());
  WRITE_FIELD(*this, kCodeOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCodeOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedSourceTextModule<D, P>::regular_exports() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModule::regular_exports(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedSourceTextModule<D, P>::regular_exports(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kRegularExportsOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_regular_exports(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kRegularExportsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kRegularExportsOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedSourceTextModule<D, P>::regular_imports() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModule::regular_imports(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedSourceTextModule<D, P>::regular_imports(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kRegularImportsOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_regular_imports(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kRegularImportsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kRegularImportsOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedSourceTextModule<D, P>::requested_modules() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModule::requested_modules(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedSourceTextModule<D, P>::requested_modules(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kRequestedModulesOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_requested_modules(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kRequestedModulesOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kRequestedModulesOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedSourceTextModule<D, P>::import_meta(AcquireLoadTag) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModule::import_meta(cage_base, kAcquireLoad);
}

template<class D, class P>
HeapObject TorqueGeneratedSourceTextModule<D, P>::import_meta(PtrComprCageBase cage_base, AcquireLoadTag) const {
  HeapObject value;
  value = TaggedField<HeapObject>::Acquire_Load(cage_base, *this, kImportMetaOffset);
  DCHECK(value.IsJSObject() || value.IsTheHole());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_import_meta(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSObject() || value.IsTheHole());
  RELEASE_WRITE_FIELD(*this, kImportMetaOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kImportMetaOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedSourceTextModule<D, P>::cycle_root() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModule::cycle_root(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedSourceTextModule<D, P>::cycle_root(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kCycleRootOffset);
  DCHECK(value.IsTheHole() || value.IsSourceTextModule());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_cycle_root(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsTheHole() || value.IsSourceTextModule());
  WRITE_FIELD(*this, kCycleRootOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCycleRootOffset, value, mode);
}

template<class D, class P>
ArrayList TorqueGeneratedSourceTextModule<D, P>::async_parent_modules() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModule::async_parent_modules(cage_base);
}

template<class D, class P>
ArrayList TorqueGeneratedSourceTextModule<D, P>::async_parent_modules(PtrComprCageBase cage_base) const {
  ArrayList value;
  value = TaggedField<ArrayList>::load(cage_base, *this, kAsyncParentModulesOffset);
  DCHECK(value.IsArrayList());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_async_parent_modules(ArrayList value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsArrayList());
  WRITE_FIELD(*this, kAsyncParentModulesOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kAsyncParentModulesOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedSourceTextModule<D, P>::dfs_index() const {
  int value;
  value = TaggedField<Smi>::load(*this, kDfsIndexOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_dfs_index(int value) {
  WRITE_FIELD(*this, kDfsIndexOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedSourceTextModule<D, P>::dfs_ancestor_index() const {
  int value;
  value = TaggedField<Smi>::load(*this, kDfsAncestorIndexOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_dfs_ancestor_index(int value) {
  WRITE_FIELD(*this, kDfsAncestorIndexOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedSourceTextModule<D, P>::pending_async_dependencies() const {
  int value;
  value = TaggedField<Smi>::load(*this, kPendingAsyncDependenciesOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_pending_async_dependencies(int value) {
  WRITE_FIELD(*this, kPendingAsyncDependenciesOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedSourceTextModule<D, P>::flags() const {
  int value;
  value = TaggedField<Smi>::load(*this, kFlagsOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModule<D, P>::set_flags(int value) {
  WRITE_FIELD(*this, kFlagsOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedSourceTextModule<D, P>::TorqueGeneratedSourceTextModule(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsSourceTextModule_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedSourceTextModule<D, P>::TorqueGeneratedSourceTextModule(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsSourceTextModule_NonInline(*this));
}
template<class D, class P>
String TorqueGeneratedModuleRequest<D, P>::specifier() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedModuleRequest::specifier(cage_base);
}

template<class D, class P>
String TorqueGeneratedModuleRequest<D, P>::specifier(PtrComprCageBase cage_base) const {
  String value;
  value = TaggedField<String>::load(cage_base, *this, kSpecifierOffset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedModuleRequest<D, P>::set_specifier(String value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, kSpecifierOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSpecifierOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedModuleRequest<D, P>::import_assertions() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedModuleRequest::import_assertions(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedModuleRequest<D, P>::import_assertions(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kImportAssertionsOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedModuleRequest<D, P>::set_import_assertions(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kImportAssertionsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kImportAssertionsOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedModuleRequest<D, P>::position() const {
  int value;
  value = TaggedField<Smi>::load(*this, kPositionOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedModuleRequest<D, P>::set_position(int value) {
  WRITE_FIELD(*this, kPositionOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedModuleRequest<D, P>::TorqueGeneratedModuleRequest(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsModuleRequest_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedModuleRequest<D, P>::TorqueGeneratedModuleRequest(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsModuleRequest_NonInline(*this));
}
template<class D, class P>
PrimitiveHeapObject TorqueGeneratedSourceTextModuleInfoEntry<D, P>::export_name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModuleInfoEntry::export_name(cage_base);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedSourceTextModuleInfoEntry<D, P>::export_name(PtrComprCageBase cage_base) const {
  PrimitiveHeapObject value;
  value = TaggedField<PrimitiveHeapObject>::load(cage_base, *this, kExportNameOffset);
  DCHECK(value.IsString() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModuleInfoEntry<D, P>::set_export_name(PrimitiveHeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString() || value.IsUndefined());
  WRITE_FIELD(*this, kExportNameOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kExportNameOffset, value, mode);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedSourceTextModuleInfoEntry<D, P>::local_name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModuleInfoEntry::local_name(cage_base);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedSourceTextModuleInfoEntry<D, P>::local_name(PtrComprCageBase cage_base) const {
  PrimitiveHeapObject value;
  value = TaggedField<PrimitiveHeapObject>::load(cage_base, *this, kLocalNameOffset);
  DCHECK(value.IsString() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModuleInfoEntry<D, P>::set_local_name(PrimitiveHeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString() || value.IsUndefined());
  WRITE_FIELD(*this, kLocalNameOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kLocalNameOffset, value, mode);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedSourceTextModuleInfoEntry<D, P>::import_name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedSourceTextModuleInfoEntry::import_name(cage_base);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedSourceTextModuleInfoEntry<D, P>::import_name(PtrComprCageBase cage_base) const {
  PrimitiveHeapObject value;
  value = TaggedField<PrimitiveHeapObject>::load(cage_base, *this, kImportNameOffset);
  DCHECK(value.IsString() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModuleInfoEntry<D, P>::set_import_name(PrimitiveHeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString() || value.IsUndefined());
  WRITE_FIELD(*this, kImportNameOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kImportNameOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedSourceTextModuleInfoEntry<D, P>::module_request() const {
  int value;
  value = TaggedField<Smi>::load(*this, kModuleRequestOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModuleInfoEntry<D, P>::set_module_request(int value) {
  WRITE_FIELD(*this, kModuleRequestOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedSourceTextModuleInfoEntry<D, P>::cell_index() const {
  int value;
  value = TaggedField<Smi>::load(*this, kCellIndexOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModuleInfoEntry<D, P>::set_cell_index(int value) {
  WRITE_FIELD(*this, kCellIndexOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedSourceTextModuleInfoEntry<D, P>::beg_pos() const {
  int value;
  value = TaggedField<Smi>::load(*this, kBegPosOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModuleInfoEntry<D, P>::set_beg_pos(int value) {
  WRITE_FIELD(*this, kBegPosOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedSourceTextModuleInfoEntry<D, P>::end_pos() const {
  int value;
  value = TaggedField<Smi>::load(*this, kEndPosOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedSourceTextModuleInfoEntry<D, P>::set_end_pos(int value) {
  WRITE_FIELD(*this, kEndPosOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedSourceTextModuleInfoEntry<D, P>::TorqueGeneratedSourceTextModuleInfoEntry(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsSourceTextModuleInfoEntry_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedSourceTextModuleInfoEntry<D, P>::TorqueGeneratedSourceTextModuleInfoEntry(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsSourceTextModuleInfoEntry_NonInline(*this));
}
