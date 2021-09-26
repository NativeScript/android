template<class D, class P>
int TorqueGeneratedTemplateInfo<D, P>::tag() const {
  int value;
  value = TaggedField<Smi>::load(*this, kTagOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedTemplateInfo<D, P>::set_tag(int value) {
  WRITE_FIELD(*this, kTagOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedTemplateInfo<D, P>::serial_number() const {
  int value;
  value = TaggedField<Smi>::load(*this, kSerialNumberOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedTemplateInfo<D, P>::set_serial_number(int value) {
  WRITE_FIELD(*this, kSerialNumberOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedTemplateInfo<D, P>::number_of_properties() const {
  int value;
  value = TaggedField<Smi>::load(*this, kNumberOfPropertiesOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedTemplateInfo<D, P>::set_number_of_properties(int value) {
  WRITE_FIELD(*this, kNumberOfPropertiesOffset, Smi::FromInt(value));
}

template<class D, class P>
HeapObject TorqueGeneratedTemplateInfo<D, P>::property_list() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedTemplateInfo::property_list(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedTemplateInfo<D, P>::property_list(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kPropertyListOffset);
  DCHECK(value.IsUndefined() || value.IsTemplateList());
  return value;
}

template<class D, class P>
void TorqueGeneratedTemplateInfo<D, P>::set_property_list(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsTemplateList());
  WRITE_FIELD(*this, kPropertyListOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPropertyListOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedTemplateInfo<D, P>::property_accessors() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedTemplateInfo::property_accessors(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedTemplateInfo<D, P>::property_accessors(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kPropertyAccessorsOffset);
  DCHECK(value.IsUndefined() || value.IsTemplateList());
  return value;
}

template<class D, class P>
void TorqueGeneratedTemplateInfo<D, P>::set_property_accessors(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsTemplateList());
  WRITE_FIELD(*this, kPropertyAccessorsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPropertyAccessorsOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedTemplateInfo<D, P>::TorqueGeneratedTemplateInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsTemplateInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedTemplateInfo<D, P>::TorqueGeneratedTemplateInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsTemplateInfo_NonInline(*this));
}
template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::prototype_template() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateRareData::prototype_template(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::prototype_template(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kPrototypeTemplateOffset);
  DCHECK(value.IsUndefined() || value.IsObjectTemplateInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateRareData<D, P>::set_prototype_template(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsObjectTemplateInfo());
  WRITE_FIELD(*this, kPrototypeTemplateOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPrototypeTemplateOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::prototype_provider_template() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateRareData::prototype_provider_template(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::prototype_provider_template(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kPrototypeProviderTemplateOffset);
  DCHECK(value.IsUndefined() || value.IsFunctionTemplateInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateRareData<D, P>::set_prototype_provider_template(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFunctionTemplateInfo());
  WRITE_FIELD(*this, kPrototypeProviderTemplateOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPrototypeProviderTemplateOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::parent_template() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateRareData::parent_template(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::parent_template(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kParentTemplateOffset);
  DCHECK(value.IsUndefined() || value.IsFunctionTemplateInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateRareData<D, P>::set_parent_template(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFunctionTemplateInfo());
  WRITE_FIELD(*this, kParentTemplateOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kParentTemplateOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::named_property_handler() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateRareData::named_property_handler(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::named_property_handler(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kNamedPropertyHandlerOffset);
  DCHECK(value.IsUndefined() || value.IsInterceptorInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateRareData<D, P>::set_named_property_handler(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsInterceptorInfo());
  WRITE_FIELD(*this, kNamedPropertyHandlerOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kNamedPropertyHandlerOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::indexed_property_handler() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateRareData::indexed_property_handler(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::indexed_property_handler(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kIndexedPropertyHandlerOffset);
  DCHECK(value.IsUndefined() || value.IsInterceptorInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateRareData<D, P>::set_indexed_property_handler(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsInterceptorInfo());
  WRITE_FIELD(*this, kIndexedPropertyHandlerOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kIndexedPropertyHandlerOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::instance_template() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateRareData::instance_template(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::instance_template(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kInstanceTemplateOffset);
  DCHECK(value.IsUndefined() || value.IsObjectTemplateInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateRareData<D, P>::set_instance_template(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsObjectTemplateInfo());
  WRITE_FIELD(*this, kInstanceTemplateOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kInstanceTemplateOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::instance_call_handler() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateRareData::instance_call_handler(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::instance_call_handler(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kInstanceCallHandlerOffset);
  DCHECK(value.IsUndefined() || value.IsCallHandlerInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateRareData<D, P>::set_instance_call_handler(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsCallHandlerInfo());
  WRITE_FIELD(*this, kInstanceCallHandlerOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kInstanceCallHandlerOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::access_check_info() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateRareData::access_check_info(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateRareData<D, P>::access_check_info(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kAccessCheckInfoOffset);
  DCHECK(value.IsUndefined() || value.IsAccessCheckInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateRareData<D, P>::set_access_check_info(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsAccessCheckInfo());
  WRITE_FIELD(*this, kAccessCheckInfoOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kAccessCheckInfoOffset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedFunctionTemplateRareData<D, P>::c_function_overloads() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateRareData::c_function_overloads(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedFunctionTemplateRareData<D, P>::c_function_overloads(PtrComprCageBase cage_base) const {
  FixedArray value;
  value = TaggedField<FixedArray>::load(cage_base, *this, kCFunctionOverloadsOffset);
  DCHECK(value.IsFixedArray());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateRareData<D, P>::set_c_function_overloads(FixedArray value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsFixedArray());
  WRITE_FIELD(*this, kCFunctionOverloadsOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCFunctionOverloadsOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedFunctionTemplateRareData<D, P>::TorqueGeneratedFunctionTemplateRareData(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsFunctionTemplateRareData_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedFunctionTemplateRareData<D, P>::TorqueGeneratedFunctionTemplateRareData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsFunctionTemplateRareData_NonInline(*this));
}
template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::call_code(AcquireLoadTag) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateInfo::call_code(cage_base, kAcquireLoad);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::call_code(PtrComprCageBase cage_base, AcquireLoadTag) const {
  HeapObject value;
  value = TaggedField<HeapObject>::Acquire_Load(cage_base, *this, kCallCodeOffset);
  DCHECK(value.IsUndefined() || value.IsCallHandlerInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateInfo<D, P>::set_call_code(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsCallHandlerInfo());
  RELEASE_WRITE_FIELD(*this, kCallCodeOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCallCodeOffset, value, mode);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::class_name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateInfo::class_name(cage_base);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::class_name(PtrComprCageBase cage_base) const {
  PrimitiveHeapObject value;
  value = TaggedField<PrimitiveHeapObject>::load(cage_base, *this, kClassNameOffset);
  DCHECK(value.IsString() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateInfo<D, P>::set_class_name(PrimitiveHeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsString() || value.IsUndefined());
  WRITE_FIELD(*this, kClassNameOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kClassNameOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::signature() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateInfo::signature(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::signature(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kSignatureOffset);
  DCHECK(value.IsUndefined() || value.IsFunctionTemplateInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateInfo<D, P>::set_signature(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFunctionTemplateInfo());
  WRITE_FIELD(*this, kSignatureOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSignatureOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::rare_data(AcquireLoadTag) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateInfo::rare_data(cage_base, kAcquireLoad);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::rare_data(PtrComprCageBase cage_base, AcquireLoadTag) const {
  HeapObject value;
  value = TaggedField<HeapObject>::Acquire_Load(cage_base, *this, kRareDataOffset);
  DCHECK(value.IsUndefined() || value.IsFunctionTemplateRareData());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateInfo<D, P>::set_rare_data(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFunctionTemplateRareData());
  RELEASE_WRITE_FIELD(*this, kRareDataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kRareDataOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::shared_function_info() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateInfo::shared_function_info(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedFunctionTemplateInfo<D, P>::shared_function_info(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kSharedFunctionInfoOffset);
  DCHECK(value.IsUndefined() || value.IsSharedFunctionInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateInfo<D, P>::set_shared_function_info(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsSharedFunctionInfo());
  WRITE_FIELD(*this, kSharedFunctionInfoOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kSharedFunctionInfoOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedFunctionTemplateInfo<D, P>::flag() const {
  int value;
  value = TaggedField<Smi>::load(*this, kFlagOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateInfo<D, P>::set_flag(int value) {
  WRITE_FIELD(*this, kFlagOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedFunctionTemplateInfo<D, P>::length() const {
  int value;
  value = TaggedField<Smi>::load(*this, kLengthOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateInfo<D, P>::set_length(int value) {
  WRITE_FIELD(*this, kLengthOffset, Smi::FromInt(value));
}

template<class D, class P>
Object TorqueGeneratedFunctionTemplateInfo<D, P>::cached_property_name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedFunctionTemplateInfo::cached_property_name(cage_base);
}

template<class D, class P>
Object TorqueGeneratedFunctionTemplateInfo<D, P>::cached_property_name(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kCachedPropertyNameOffset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateInfo<D, P>::set_cached_property_name(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, kCachedPropertyNameOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kCachedPropertyNameOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedFunctionTemplateInfo<D, P>::instance_type() const {
  int value;
  value = TaggedField<Smi>::load(*this, kInstanceTypeOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedFunctionTemplateInfo<D, P>::set_instance_type(int value) {
  WRITE_FIELD(*this, kInstanceTypeOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedFunctionTemplateInfo<D, P>::TorqueGeneratedFunctionTemplateInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsFunctionTemplateInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedFunctionTemplateInfo<D, P>::TorqueGeneratedFunctionTemplateInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsFunctionTemplateInfo_NonInline(*this));
}
template<class D, class P>
HeapObject TorqueGeneratedObjectTemplateInfo<D, P>::constructor() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedObjectTemplateInfo::constructor(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedObjectTemplateInfo<D, P>::constructor(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kConstructorOffset);
  DCHECK(value.IsUndefined() || value.IsFunctionTemplateInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedObjectTemplateInfo<D, P>::set_constructor(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsFunctionTemplateInfo());
  WRITE_FIELD(*this, kConstructorOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kConstructorOffset, value, mode);
}

template<class D, class P>
int TorqueGeneratedObjectTemplateInfo<D, P>::data() const {
  int value;
  value = TaggedField<Smi>::load(*this, kDataOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedObjectTemplateInfo<D, P>::set_data(int value) {
  WRITE_FIELD(*this, kDataOffset, Smi::FromInt(value));
}

template<class D, class P>
inline TorqueGeneratedObjectTemplateInfo<D, P>::TorqueGeneratedObjectTemplateInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsObjectTemplateInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedObjectTemplateInfo<D, P>::TorqueGeneratedObjectTemplateInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsObjectTemplateInfo_NonInline(*this));
}
