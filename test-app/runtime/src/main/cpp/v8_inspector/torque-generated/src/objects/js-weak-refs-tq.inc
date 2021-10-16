class WeakCell;
class JSFinalizationRegistry;
class JSWeakRef;
// Alias for HeapObject::IsWeakCell() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWeakCell_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWeakCell : public P {
  static_assert(std::is_same<WeakCell, D>::value,
    "Use this class as direct base for WeakCell.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedWeakCell.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWeakCell<D,P>;

  // Torque type: (Undefined | class JSFinalizationRegistry)
inline HeapObject finalization_registry() const;
inline HeapObject finalization_registry(PtrComprCageBase cage_base) const;
inline void set_finalization_registry(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class JSReceiver | Undefined)
inline HeapObject target() const;
inline HeapObject target(PtrComprCageBase cage_base) const;
inline void set_target(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class JSReceiver | Undefined)
inline HeapObject unregister_token() const;
inline HeapObject unregister_token(PtrComprCageBase cage_base) const;
inline void set_unregister_token(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: JSAny
inline Object holdings() const;
inline Object holdings(PtrComprCageBase cage_base) const;
inline void set_holdings(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class WeakCell)
inline HeapObject prev() const;
inline HeapObject prev(PtrComprCageBase cage_base) const;
inline void set_prev(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class WeakCell)
inline HeapObject next() const;
inline HeapObject next(PtrComprCageBase cage_base) const;
inline void set_next(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class WeakCell)
inline HeapObject key_list_prev() const;
inline HeapObject key_list_prev(PtrComprCageBase cage_base) const;
inline void set_key_list_prev(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class WeakCell)
inline HeapObject key_list_next() const;
inline HeapObject key_list_next(PtrComprCageBase cage_base) const;
inline void set_key_list_next(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WeakCellVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kFinalizationRegistryOffset = P::kHeaderSize;
  static constexpr int kFinalizationRegistryOffsetEnd = kFinalizationRegistryOffset + kTaggedSize - 1;
  static constexpr int kTargetOffset = kFinalizationRegistryOffsetEnd + 1;
  static constexpr int kTargetOffsetEnd = kTargetOffset + kTaggedSize - 1;
  static constexpr int kUnregisterTokenOffset = kTargetOffsetEnd + 1;
  static constexpr int kUnregisterTokenOffsetEnd = kUnregisterTokenOffset + kTaggedSize - 1;
  static constexpr int kHoldingsOffset = kUnregisterTokenOffsetEnd + 1;
  static constexpr int kHoldingsOffsetEnd = kHoldingsOffset + kTaggedSize - 1;
  static constexpr int kPrevOffset = kHoldingsOffsetEnd + 1;
  static constexpr int kPrevOffsetEnd = kPrevOffset + kTaggedSize - 1;
  static constexpr int kNextOffset = kPrevOffsetEnd + 1;
  static constexpr int kNextOffsetEnd = kNextOffset + kTaggedSize - 1;
  static constexpr int kKeyListPrevOffset = kNextOffsetEnd + 1;
  static constexpr int kKeyListPrevOffsetEnd = kKeyListPrevOffset + kTaggedSize - 1;
  static constexpr int kKeyListNextOffset = kKeyListPrevOffsetEnd + 1;
  static constexpr int kKeyListNextOffsetEnd = kKeyListNextOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kKeyListNextOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kKeyListNextOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kKeyListNextOffsetEnd + 1;
  static constexpr int kHeaderSize = kKeyListNextOffsetEnd + 1;
  static constexpr int kSize = kKeyListNextOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 36);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWeakCell() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWeakCell, 
      DAlias>::value,
      "class TorqueGeneratedWeakCell should be used as direct base for WeakCell.");
  }
 protected:
  inline explicit TorqueGeneratedWeakCell(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWeakCell(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSWeakRef() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSWeakRef_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSWeakRef : public P {
  static_assert(std::is_same<JSWeakRef, D>::value,
    "Use this class as direct base for JSWeakRef.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSWeakRef.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSWeakRef<D,P>;

  // Torque type: (class JSReceiver | Undefined)
inline HeapObject target() const;
inline HeapObject target(PtrComprCageBase cage_base) const;
inline void set_target(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSWeakRefVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kTargetOffset = P::kHeaderSize;
  static constexpr int kTargetOffsetEnd = kTargetOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kTargetOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kTargetOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kTargetOffsetEnd + 1;
  static constexpr int kHeaderSize = kTargetOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSWeakRef() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSWeakRef, 
      DAlias>::value,
      "class TorqueGeneratedJSWeakRef should be used as direct base for JSWeakRef.");
  }
 protected:
  inline explicit TorqueGeneratedJSWeakRef(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSWeakRef(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

