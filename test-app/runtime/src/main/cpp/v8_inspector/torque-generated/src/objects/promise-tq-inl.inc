template<class D, class P>
HeapObject TorqueGeneratedPromiseCapability<D, P>::promise() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseCapability::promise(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseCapability<D, P>::promise(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kPromiseOffset);
  DCHECK(value.IsJSReceiver() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseCapability<D, P>::set_promise(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver() || value.IsUndefined());
  WRITE_FIELD(*this, kPromiseOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPromiseOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedPromiseCapability<D, P>::resolve() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseCapability::resolve(cage_base);
}

template<class D, class P>
Object TorqueGeneratedPromiseCapability<D, P>::resolve(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kResolveOffset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseCapability<D, P>::set_resolve(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, kResolveOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kResolveOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedPromiseCapability<D, P>::reject() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseCapability::reject(cage_base);
}

template<class D, class P>
Object TorqueGeneratedPromiseCapability<D, P>::reject(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kRejectOffset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseCapability<D, P>::set_reject(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, kRejectOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kRejectOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedPromiseCapability<D, P>::TorqueGeneratedPromiseCapability(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsPromiseCapability_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedPromiseCapability<D, P>::TorqueGeneratedPromiseCapability(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsPromiseCapability_NonInline(*this));
}
template<class D, class P>
Object TorqueGeneratedPromiseReaction<D, P>::next() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReaction::next(cage_base);
}

template<class D, class P>
Object TorqueGeneratedPromiseReaction<D, P>::next(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kNextOffset);
  DCHECK(value.IsZero() || value.IsPromiseReaction());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReaction<D, P>::set_next(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsZero() || value.IsPromiseReaction());
  WRITE_FIELD(*this, kNextOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kNextOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReaction<D, P>::reject_handler() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReaction::reject_handler(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReaction<D, P>::reject_handler(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kRejectHandlerOffset);
  DCHECK(value.IsUndefined() || value.IsCallableApiObject() || value.IsCallableJSProxy() || value.IsJSFunction() || value.IsJSBoundFunction());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReaction<D, P>::set_reject_handler(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsCallableApiObject() || value.IsCallableJSProxy() || value.IsJSFunction() || value.IsJSBoundFunction());
  WRITE_FIELD(*this, kRejectHandlerOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kRejectHandlerOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReaction<D, P>::fulfill_handler() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReaction::fulfill_handler(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReaction<D, P>::fulfill_handler(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kFulfillHandlerOffset);
  DCHECK(value.IsUndefined() || value.IsCallableApiObject() || value.IsCallableJSProxy() || value.IsJSFunction() || value.IsJSBoundFunction());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReaction<D, P>::set_fulfill_handler(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsCallableApiObject() || value.IsCallableJSProxy() || value.IsJSFunction() || value.IsJSBoundFunction());
  WRITE_FIELD(*this, kFulfillHandlerOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kFulfillHandlerOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReaction<D, P>::promise_or_capability() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReaction::promise_or_capability(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReaction<D, P>::promise_or_capability(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kPromiseOrCapabilityOffset);
  DCHECK(value.IsUndefined() || value.IsJSPromise() || value.IsPromiseCapability());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReaction<D, P>::set_promise_or_capability(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsJSPromise() || value.IsPromiseCapability());
  WRITE_FIELD(*this, kPromiseOrCapabilityOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPromiseOrCapabilityOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedPromiseReaction<D, P>::continuation_preserved_embedder_data() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReaction::continuation_preserved_embedder_data(cage_base);
}

template<class D, class P>
Object TorqueGeneratedPromiseReaction<D, P>::continuation_preserved_embedder_data(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kContinuationPreservedEmbedderDataOffset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReaction<D, P>::set_continuation_preserved_embedder_data(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, kContinuationPreservedEmbedderDataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kContinuationPreservedEmbedderDataOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedPromiseReaction<D, P>::TorqueGeneratedPromiseReaction(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsPromiseReaction_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedPromiseReaction<D, P>::TorqueGeneratedPromiseReaction(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsPromiseReaction_NonInline(*this));
}
template<class D, class P>
Object TorqueGeneratedPromiseReactionJobTask<D, P>::argument() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReactionJobTask::argument(cage_base);
}

template<class D, class P>
Object TorqueGeneratedPromiseReactionJobTask<D, P>::argument(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kArgumentOffset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReactionJobTask<D, P>::set_argument(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, kArgumentOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kArgumentOffset, value, mode);
}

template<class D, class P>
Context TorqueGeneratedPromiseReactionJobTask<D, P>::context() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReactionJobTask::context(cage_base);
}

template<class D, class P>
Context TorqueGeneratedPromiseReactionJobTask<D, P>::context(PtrComprCageBase cage_base) const {
  Context value;
  value = TaggedField<Context>::load(cage_base, *this, kContextOffset);
  DCHECK(value.IsContext());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReactionJobTask<D, P>::set_context(Context value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsContext());
  WRITE_FIELD(*this, kContextOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kContextOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReactionJobTask<D, P>::handler() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReactionJobTask::handler(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReactionJobTask<D, P>::handler(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kHandlerOffset);
  DCHECK(value.IsUndefined() || value.IsCallableApiObject() || value.IsCallableJSProxy() || value.IsJSFunction() || value.IsJSBoundFunction());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReactionJobTask<D, P>::set_handler(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsCallableApiObject() || value.IsCallableJSProxy() || value.IsJSFunction() || value.IsJSBoundFunction());
  WRITE_FIELD(*this, kHandlerOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kHandlerOffset, value, mode);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReactionJobTask<D, P>::promise_or_capability() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReactionJobTask::promise_or_capability(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedPromiseReactionJobTask<D, P>::promise_or_capability(PtrComprCageBase cage_base) const {
  HeapObject value;
  value = TaggedField<HeapObject>::load(cage_base, *this, kPromiseOrCapabilityOffset);
  DCHECK(value.IsUndefined() || value.IsJSPromise() || value.IsPromiseCapability());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReactionJobTask<D, P>::set_promise_or_capability(HeapObject value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsUndefined() || value.IsJSPromise() || value.IsPromiseCapability());
  WRITE_FIELD(*this, kPromiseOrCapabilityOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPromiseOrCapabilityOffset, value, mode);
}

template<class D, class P>
Object TorqueGeneratedPromiseReactionJobTask<D, P>::continuation_preserved_embedder_data() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseReactionJobTask::continuation_preserved_embedder_data(cage_base);
}

template<class D, class P>
Object TorqueGeneratedPromiseReactionJobTask<D, P>::continuation_preserved_embedder_data(PtrComprCageBase cage_base) const {
  Object value;
  value = TaggedField<Object>::load(cage_base, *this, kContinuationPreservedEmbedderDataOffset);
  DCHECK(value.IsHeapObject() || value.IsSmi());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseReactionJobTask<D, P>::set_continuation_preserved_embedder_data(Object value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsHeapObject() || value.IsSmi());
  WRITE_FIELD(*this, kContinuationPreservedEmbedderDataOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kContinuationPreservedEmbedderDataOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedPromiseReactionJobTask<D, P>::TorqueGeneratedPromiseReactionJobTask(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsPromiseReactionJobTask_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedPromiseReactionJobTask<D, P>::TorqueGeneratedPromiseReactionJobTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsPromiseReactionJobTask_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedPromiseFulfillReactionJobTask<D, P>::TorqueGeneratedPromiseFulfillReactionJobTask(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsPromiseFulfillReactionJobTask_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedPromiseFulfillReactionJobTask<D, P>::TorqueGeneratedPromiseFulfillReactionJobTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsPromiseFulfillReactionJobTask_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedPromiseRejectReactionJobTask<D, P>::TorqueGeneratedPromiseRejectReactionJobTask(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsPromiseRejectReactionJobTask_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedPromiseRejectReactionJobTask<D, P>::TorqueGeneratedPromiseRejectReactionJobTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsPromiseRejectReactionJobTask_NonInline(*this));
}
template<class D, class P>
Context TorqueGeneratedPromiseResolveThenableJobTask<D, P>::context() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseResolveThenableJobTask::context(cage_base);
}

template<class D, class P>
Context TorqueGeneratedPromiseResolveThenableJobTask<D, P>::context(PtrComprCageBase cage_base) const {
  Context value;
  value = TaggedField<Context>::load(cage_base, *this, kContextOffset);
  DCHECK(value.IsContext());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseResolveThenableJobTask<D, P>::set_context(Context value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsContext());
  WRITE_FIELD(*this, kContextOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kContextOffset, value, mode);
}

template<class D, class P>
JSPromise TorqueGeneratedPromiseResolveThenableJobTask<D, P>::promise_to_resolve() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseResolveThenableJobTask::promise_to_resolve(cage_base);
}

template<class D, class P>
JSPromise TorqueGeneratedPromiseResolveThenableJobTask<D, P>::promise_to_resolve(PtrComprCageBase cage_base) const {
  JSPromise value;
  value = TaggedField<JSPromise>::load(cage_base, *this, kPromiseToResolveOffset);
  DCHECK(value.IsJSPromise());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseResolveThenableJobTask<D, P>::set_promise_to_resolve(JSPromise value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSPromise());
  WRITE_FIELD(*this, kPromiseToResolveOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kPromiseToResolveOffset, value, mode);
}

template<class D, class P>
JSReceiver TorqueGeneratedPromiseResolveThenableJobTask<D, P>::thenable() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseResolveThenableJobTask::thenable(cage_base);
}

template<class D, class P>
JSReceiver TorqueGeneratedPromiseResolveThenableJobTask<D, P>::thenable(PtrComprCageBase cage_base) const {
  JSReceiver value;
  value = TaggedField<JSReceiver>::load(cage_base, *this, kThenableOffset);
  DCHECK(value.IsJSReceiver());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseResolveThenableJobTask<D, P>::set_thenable(JSReceiver value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver());
  WRITE_FIELD(*this, kThenableOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kThenableOffset, value, mode);
}

template<class D, class P>
JSReceiver TorqueGeneratedPromiseResolveThenableJobTask<D, P>::then() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedPromiseResolveThenableJobTask::then(cage_base);
}

template<class D, class P>
JSReceiver TorqueGeneratedPromiseResolveThenableJobTask<D, P>::then(PtrComprCageBase cage_base) const {
  JSReceiver value;
  value = TaggedField<JSReceiver>::load(cage_base, *this, kThenOffset);
  DCHECK(value.IsJSReceiver());
  return value;
}

template<class D, class P>
void TorqueGeneratedPromiseResolveThenableJobTask<D, P>::set_then(JSReceiver value, WriteBarrierMode mode) {
  SLOW_DCHECK(value.IsJSReceiver());
  WRITE_FIELD(*this, kThenOffset, value);
  CONDITIONAL_WRITE_BARRIER(*this, kThenOffset, value, mode);
}

template<class D, class P>
inline TorqueGeneratedPromiseResolveThenableJobTask<D, P>::TorqueGeneratedPromiseResolveThenableJobTask(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsPromiseResolveThenableJobTask_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedPromiseResolveThenableJobTask<D, P>::TorqueGeneratedPromiseResolveThenableJobTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsPromiseResolveThenableJobTask_NonInline(*this));
}
