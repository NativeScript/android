class PromiseCapability;
class PromiseReaction;
class PromiseReactionJobTask;
class PromiseFulfillReactionJobTask;
class PromiseRejectReactionJobTask;
class PromiseResolveThenableJobTask;
// Alias for HeapObject::IsPromiseCapability() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPromiseCapability_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPromiseCapability : public P {
  static_assert(std::is_same<PromiseCapability, D>::value,
    "Use this class as direct base for PromiseCapability.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedPromiseCapability.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPromiseCapability<D,P>;

  // Torque type: (class JSReceiver | Undefined)
inline HeapObject promise() const;
inline HeapObject promise(PtrComprCageBase cage_base) const;
inline void set_promise(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object resolve() const;
inline Object resolve(PtrComprCageBase cage_base) const;
inline void set_resolve(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object reject() const;
inline Object reject(PtrComprCageBase cage_base) const;
inline void set_reject(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PromiseCapabilityVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kPromiseOffset = P::kHeaderSize;
  static constexpr int kPromiseOffsetEnd = kPromiseOffset + kTaggedSize - 1;
  static constexpr int kResolveOffset = kPromiseOffsetEnd + 1;
  static constexpr int kResolveOffsetEnd = kResolveOffset + kTaggedSize - 1;
  static constexpr int kRejectOffset = kResolveOffsetEnd + 1;
  static constexpr int kRejectOffsetEnd = kRejectOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kRejectOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kRejectOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kRejectOffsetEnd + 1;
  static constexpr int kHeaderSize = kRejectOffsetEnd + 1;
  static constexpr int kSize = kRejectOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 16);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedPromiseCapability() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPromiseCapability, 
      DAlias>::value,
      "class TorqueGeneratedPromiseCapability should be used as direct base for PromiseCapability.");
  }
 protected:
  inline explicit TorqueGeneratedPromiseCapability(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPromiseCapability(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsPromiseReaction() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPromiseReaction_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPromiseReaction : public P {
  static_assert(std::is_same<PromiseReaction, D>::value,
    "Use this class as direct base for PromiseReaction.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedPromiseReaction.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPromiseReaction<D,P>;

  // Torque type: (Zero | class PromiseReaction)
inline Object next() const;
inline Object next(PtrComprCageBase cage_base) const;
inline void set_next(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | CallableApiObject | CallableJSProxy | class JSFunction | class JSBoundFunction)
inline HeapObject reject_handler() const;
inline HeapObject reject_handler(PtrComprCageBase cage_base) const;
inline void set_reject_handler(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | CallableApiObject | CallableJSProxy | class JSFunction | class JSBoundFunction)
inline HeapObject fulfill_handler() const;
inline HeapObject fulfill_handler(PtrComprCageBase cage_base) const;
inline void set_fulfill_handler(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class JSPromise | class PromiseCapability)
inline HeapObject promise_or_capability() const;
inline HeapObject promise_or_capability(PtrComprCageBase cage_base) const;
inline void set_promise_or_capability(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object continuation_preserved_embedder_data() const;
inline Object continuation_preserved_embedder_data(PtrComprCageBase cage_base) const;
inline void set_continuation_preserved_embedder_data(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PromiseReactionVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kNextOffset = P::kHeaderSize;
  static constexpr int kNextOffsetEnd = kNextOffset + kTaggedSize - 1;
  static constexpr int kRejectHandlerOffset = kNextOffsetEnd + 1;
  static constexpr int kRejectHandlerOffsetEnd = kRejectHandlerOffset + kTaggedSize - 1;
  static constexpr int kFulfillHandlerOffset = kRejectHandlerOffsetEnd + 1;
  static constexpr int kFulfillHandlerOffsetEnd = kFulfillHandlerOffset + kTaggedSize - 1;
  static constexpr int kPromiseOrCapabilityOffset = kFulfillHandlerOffsetEnd + 1;
  static constexpr int kPromiseOrCapabilityOffsetEnd = kPromiseOrCapabilityOffset + kTaggedSize - 1;
  static constexpr int kContinuationPreservedEmbedderDataOffset = kPromiseOrCapabilityOffsetEnd + 1;
  static constexpr int kContinuationPreservedEmbedderDataOffsetEnd = kContinuationPreservedEmbedderDataOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kContinuationPreservedEmbedderDataOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kContinuationPreservedEmbedderDataOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kContinuationPreservedEmbedderDataOffsetEnd + 1;
  static constexpr int kHeaderSize = kContinuationPreservedEmbedderDataOffsetEnd + 1;
  static constexpr int kSize = kContinuationPreservedEmbedderDataOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 24);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedPromiseReaction() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPromiseReaction, 
      DAlias>::value,
      "class TorqueGeneratedPromiseReaction should be used as direct base for PromiseReaction.");
  }
 protected:
  inline explicit TorqueGeneratedPromiseReaction(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPromiseReaction(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsPromiseReactionJobTask() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPromiseReactionJobTask_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPromiseReactionJobTask : public P {
  static_assert(std::is_same<PromiseReactionJobTask, D>::value,
    "Use this class as direct base for PromiseReactionJobTask.");
  static_assert(std::is_same<Microtask, P>::value,
    "Pass in Microtask as second template parameter for TorqueGeneratedPromiseReactionJobTask.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPromiseReactionJobTask<D,P>;

inline Object argument() const;
inline Object argument(PtrComprCageBase cage_base) const;
inline void set_argument(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Context context() const;
inline Context context(PtrComprCageBase cage_base) const;
inline void set_context(Context value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | CallableApiObject | CallableJSProxy | class JSFunction | class JSBoundFunction)
inline HeapObject handler() const;
inline HeapObject handler(PtrComprCageBase cage_base) const;
inline void set_handler(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class JSPromise | class PromiseCapability)
inline HeapObject promise_or_capability() const;
inline HeapObject promise_or_capability(PtrComprCageBase cage_base) const;
inline void set_promise_or_capability(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object continuation_preserved_embedder_data() const;
inline Object continuation_preserved_embedder_data(PtrComprCageBase cage_base) const;
inline void set_continuation_preserved_embedder_data(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PromiseReactionJobTaskVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kArgumentOffset = P::kHeaderSize;
  static constexpr int kArgumentOffsetEnd = kArgumentOffset + kTaggedSize - 1;
  static constexpr int kContextOffset = kArgumentOffsetEnd + 1;
  static constexpr int kContextOffsetEnd = kContextOffset + kTaggedSize - 1;
  static constexpr int kHandlerOffset = kContextOffsetEnd + 1;
  static constexpr int kHandlerOffsetEnd = kHandlerOffset + kTaggedSize - 1;
  static constexpr int kPromiseOrCapabilityOffset = kHandlerOffsetEnd + 1;
  static constexpr int kPromiseOrCapabilityOffsetEnd = kPromiseOrCapabilityOffset + kTaggedSize - 1;
  static constexpr int kContinuationPreservedEmbedderDataOffset = kPromiseOrCapabilityOffsetEnd + 1;
  static constexpr int kContinuationPreservedEmbedderDataOffsetEnd = kContinuationPreservedEmbedderDataOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kContinuationPreservedEmbedderDataOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kContinuationPreservedEmbedderDataOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kContinuationPreservedEmbedderDataOffsetEnd + 1;
  static constexpr int kHeaderSize = kContinuationPreservedEmbedderDataOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedPromiseReactionJobTask() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPromiseReactionJobTask, 
      DAlias>::value,
      "class TorqueGeneratedPromiseReactionJobTask should be used as direct base for PromiseReactionJobTask.");
  }
 protected:
  inline explicit TorqueGeneratedPromiseReactionJobTask(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPromiseReactionJobTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsPromiseFulfillReactionJobTask() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPromiseFulfillReactionJobTask_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPromiseFulfillReactionJobTask : public P {
  static_assert(std::is_same<PromiseFulfillReactionJobTask, D>::value,
    "Use this class as direct base for PromiseFulfillReactionJobTask.");
  static_assert(std::is_same<PromiseReactionJobTask, P>::value,
    "Pass in PromiseReactionJobTask as second template parameter for TorqueGeneratedPromiseFulfillReactionJobTask.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPromiseFulfillReactionJobTask<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PromiseFulfillReactionJobTaskVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;
  static constexpr int kSize = P::kHeaderSize;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 24);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedPromiseFulfillReactionJobTask() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPromiseFulfillReactionJobTask, 
      DAlias>::value,
      "class TorqueGeneratedPromiseFulfillReactionJobTask should be used as direct base for PromiseFulfillReactionJobTask.");
  }
 protected:
  inline explicit TorqueGeneratedPromiseFulfillReactionJobTask(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPromiseFulfillReactionJobTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsPromiseRejectReactionJobTask() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPromiseRejectReactionJobTask_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPromiseRejectReactionJobTask : public P {
  static_assert(std::is_same<PromiseRejectReactionJobTask, D>::value,
    "Use this class as direct base for PromiseRejectReactionJobTask.");
  static_assert(std::is_same<PromiseReactionJobTask, P>::value,
    "Pass in PromiseReactionJobTask as second template parameter for TorqueGeneratedPromiseRejectReactionJobTask.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPromiseRejectReactionJobTask<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PromiseRejectReactionJobTaskVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;
  static constexpr int kSize = P::kHeaderSize;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 24);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedPromiseRejectReactionJobTask() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPromiseRejectReactionJobTask, 
      DAlias>::value,
      "class TorqueGeneratedPromiseRejectReactionJobTask should be used as direct base for PromiseRejectReactionJobTask.");
  }
 protected:
  inline explicit TorqueGeneratedPromiseRejectReactionJobTask(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPromiseRejectReactionJobTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsPromiseResolveThenableJobTask() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPromiseResolveThenableJobTask_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPromiseResolveThenableJobTask : public P {
  static_assert(std::is_same<PromiseResolveThenableJobTask, D>::value,
    "Use this class as direct base for PromiseResolveThenableJobTask.");
  static_assert(std::is_same<Microtask, P>::value,
    "Pass in Microtask as second template parameter for TorqueGeneratedPromiseResolveThenableJobTask.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPromiseResolveThenableJobTask<D,P>;

inline Context context() const;
inline Context context(PtrComprCageBase cage_base) const;
inline void set_context(Context value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline JSPromise promise_to_resolve() const;
inline JSPromise promise_to_resolve(PtrComprCageBase cage_base) const;
inline void set_promise_to_resolve(JSPromise value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline JSReceiver thenable() const;
inline JSReceiver thenable(PtrComprCageBase cage_base) const;
inline void set_thenable(JSReceiver value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline JSReceiver then() const;
inline JSReceiver then(PtrComprCageBase cage_base) const;
inline void set_then(JSReceiver value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PromiseResolveThenableJobTaskVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kContextOffset = P::kHeaderSize;
  static constexpr int kContextOffsetEnd = kContextOffset + kTaggedSize - 1;
  static constexpr int kPromiseToResolveOffset = kContextOffsetEnd + 1;
  static constexpr int kPromiseToResolveOffsetEnd = kPromiseToResolveOffset + kTaggedSize - 1;
  static constexpr int kThenableOffset = kPromiseToResolveOffsetEnd + 1;
  static constexpr int kThenableOffsetEnd = kThenableOffset + kTaggedSize - 1;
  static constexpr int kThenOffset = kThenableOffsetEnd + 1;
  static constexpr int kThenOffsetEnd = kThenOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kThenOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kThenOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kThenOffsetEnd + 1;
  static constexpr int kHeaderSize = kThenOffsetEnd + 1;
  static constexpr int kSize = kThenOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 20);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedPromiseResolveThenableJobTask() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPromiseResolveThenableJobTask, 
      DAlias>::value,
      "class TorqueGeneratedPromiseResolveThenableJobTask should be used as direct base for PromiseResolveThenableJobTask.");
  }
 protected:
  inline explicit TorqueGeneratedPromiseResolveThenableJobTask(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPromiseResolveThenableJobTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

