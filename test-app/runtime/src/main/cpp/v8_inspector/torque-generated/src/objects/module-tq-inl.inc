template<class D, class P>
Module TorqueGeneratedJSModuleNamespace<D, P>::module() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedJSModuleNamespace::module(cage_base);
}

template<class D, class P>
Module TorqueGeneratedJSModuleNamespace<D, P>::module(PtrComprCageBase cage_base) const {
  Module value;
  value = TaggedField<Module>::load(cage_base, *this, kModuleOffset);
  DCHECK(value.IsModule());
  return value;
}

template<class D, class P>
void TorqueGeneratedJSModuleNamespace<D, P>::set_module(Module value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsModule());
  WRITE_FIELD(*this, kModuleOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kModuleOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedJSModuleNamespace<D, P>::TorqueGeneratedJSModuleNamespace(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsJSModuleNamespace_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedJSModuleNamespace<D, P>::TorqueGeneratedJSModuleNamespace(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsJSModuleNamespace_NonInline(*this));
}
