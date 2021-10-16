class JSArgumentsObject;
class JSArgumentsObject;
class JSArgumentsObject;
class SloppyArgumentsElements;
class AliasedArgumentsEntry;
// Alias for HeapObject::IsJSArgumentsObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSArgumentsObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSArgumentsObject : public P {
  static_assert(std::is_same<JSArgumentsObject, D>::value,
    "Use this class as direct base for JSArgumentsObject.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSArgumentsObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSArgumentsObject<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSArgumentsObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSArgumentsObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSArgumentsObject, 
      DAlias>::value,
      "class TorqueGeneratedJSArgumentsObject should be used as direct base for JSArgumentsObject.");
  }
 protected:
  inline explicit TorqueGeneratedJSArgumentsObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSArgumentsObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsSloppyArgumentsElements() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSloppyArgumentsElements_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSloppyArgumentsElements : public P {
  static_assert(std::is_same<SloppyArgumentsElements, D>::value,
    "Use this class as direct base for SloppyArgumentsElements.");
  static_assert(std::is_same<FixedArrayBase, P>::value,
    "Pass in FixedArrayBase as second template parameter for TorqueGeneratedSloppyArgumentsElements.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSloppyArgumentsElements<D,P>;

inline Context context() const;
inline Context context(PtrComprCageBase cage_base) const;
inline void set_context(Context value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray arguments() const;
inline FixedArray arguments(PtrComprCageBase cage_base) const;
inline void set_arguments(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | TheHole)
inline Object mapped_entries(int i, RelaxedLoadTag) const;
inline Object mapped_entries(PtrComprCageBase cage_base, int i, RelaxedLoadTag) const;
inline void set_mapped_entries(int i, Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(SloppyArgumentsElements)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SloppyArgumentsElementsVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kContextOffset = P::kHeaderSize;
  static constexpr int kContextOffsetEnd = kContextOffset + kTaggedSize - 1;
  static constexpr int kArgumentsOffset = kContextOffsetEnd + 1;
  static constexpr int kArgumentsOffsetEnd = kArgumentsOffset + kTaggedSize - 1;
  static constexpr int kHeaderSize = kArgumentsOffsetEnd + 1;
  static constexpr int kMappedEntriesOffset = kArgumentsOffsetEnd + 1;
  static constexpr int kMappedEntriesOffsetEnd = kMappedEntriesOffset + 0 - 1;
  static constexpr int kEndOfStrongFieldsOffset = kMappedEntriesOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kMappedEntriesOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kMappedEntriesOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor(int length) {
    int32_t size = kHeaderSize;
    size += length * 4;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor(this->length());
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedSloppyArgumentsElements() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSloppyArgumentsElements, 
      DAlias>::value,
      "class TorqueGeneratedSloppyArgumentsElements should be used as direct base for SloppyArgumentsElements.");
  }
 protected:
  inline explicit TorqueGeneratedSloppyArgumentsElements(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSloppyArgumentsElements(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class SloppyArgumentsElements : public TorqueGeneratedSloppyArgumentsElements<SloppyArgumentsElements, FixedArrayBase> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(SloppyArgumentsElements)
};

// Alias for HeapObject::IsAliasedArgumentsEntry() that avoids inlining.
V8_EXPORT_PRIVATE bool IsAliasedArgumentsEntry_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedAliasedArgumentsEntry : public P {
  static_assert(std::is_same<AliasedArgumentsEntry, D>::value,
    "Use this class as direct base for AliasedArgumentsEntry.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedAliasedArgumentsEntry.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedAliasedArgumentsEntry<D,P>;

inline int aliased_context_slot() const;
inline void set_aliased_context_slot(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(AliasedArgumentsEntry)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void AliasedArgumentsEntryVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kAliasedContextSlotOffset = P::kHeaderSize;
  static constexpr int kAliasedContextSlotOffsetEnd = kAliasedContextSlotOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kAliasedContextSlotOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kAliasedContextSlotOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kAliasedContextSlotOffsetEnd + 1;
  static constexpr int kHeaderSize = kAliasedContextSlotOffsetEnd + 1;
  static constexpr int kSize = kAliasedContextSlotOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 8);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedAliasedArgumentsEntry() : P() {
    static_assert(std::is_base_of<TorqueGeneratedAliasedArgumentsEntry, 
      DAlias>::value,
      "class TorqueGeneratedAliasedArgumentsEntry should be used as direct base for AliasedArgumentsEntry.");
  }
 protected:
  inline explicit TorqueGeneratedAliasedArgumentsEntry(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedAliasedArgumentsEntry(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

