class CachedTemplateObject;
class TemplateObjectDescription;
// Alias for HeapObject::IsCachedTemplateObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsCachedTemplateObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedCachedTemplateObject : public P {
  static_assert(std::is_same<CachedTemplateObject, D>::value,
    "Use this class as direct base for CachedTemplateObject.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedCachedTemplateObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedCachedTemplateObject<D,P>;

inline int slot_id() const;
inline void set_slot_id(int value);

inline JSArray template_object() const;
inline JSArray template_object(PtrComprCageBase cage_base) const;
inline void set_template_object(JSArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (TheHole | class CachedTemplateObject)
inline HeapObject next() const;
inline HeapObject next(PtrComprCageBase cage_base) const;
inline void set_next(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(CachedTemplateObject)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void CachedTemplateObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kSlotIdOffset = P::kHeaderSize;
  static constexpr int kSlotIdOffsetEnd = kSlotIdOffset + kTaggedSize - 1;
  static constexpr int kTemplateObjectOffset = kSlotIdOffsetEnd + 1;
  static constexpr int kTemplateObjectOffsetEnd = kTemplateObjectOffset + kTaggedSize - 1;
  static constexpr int kNextOffset = kTemplateObjectOffsetEnd + 1;
  static constexpr int kNextOffsetEnd = kNextOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kNextOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kNextOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kNextOffsetEnd + 1;
  static constexpr int kHeaderSize = kNextOffsetEnd + 1;
  static constexpr int kSize = kNextOffsetEnd + 1;

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
  constexpr TorqueGeneratedCachedTemplateObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedCachedTemplateObject, 
      DAlias>::value,
      "class TorqueGeneratedCachedTemplateObject should be used as direct base for CachedTemplateObject.");
  }
 protected:
  inline explicit TorqueGeneratedCachedTemplateObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedCachedTemplateObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsTemplateObjectDescription() that avoids inlining.
V8_EXPORT_PRIVATE bool IsTemplateObjectDescription_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedTemplateObjectDescription : public P {
  static_assert(std::is_same<TemplateObjectDescription, D>::value,
    "Use this class as direct base for TemplateObjectDescription.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedTemplateObjectDescription.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedTemplateObjectDescription<D,P>;

inline FixedArray raw_strings() const;
inline FixedArray raw_strings(PtrComprCageBase cage_base) const;
inline void set_raw_strings(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray cooked_strings() const;
inline FixedArray cooked_strings(PtrComprCageBase cage_base) const;
inline void set_cooked_strings(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(TemplateObjectDescription)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void TemplateObjectDescriptionVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kRawStringsOffset = P::kHeaderSize;
  static constexpr int kRawStringsOffsetEnd = kRawStringsOffset + kTaggedSize - 1;
  static constexpr int kCookedStringsOffset = kRawStringsOffsetEnd + 1;
  static constexpr int kCookedStringsOffsetEnd = kCookedStringsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kCookedStringsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kCookedStringsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kCookedStringsOffsetEnd + 1;
  static constexpr int kHeaderSize = kCookedStringsOffsetEnd + 1;
  static constexpr int kSize = kCookedStringsOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 12);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedTemplateObjectDescription() : P() {
    static_assert(std::is_base_of<TorqueGeneratedTemplateObjectDescription, 
      DAlias>::value,
      "class TorqueGeneratedTemplateObjectDescription should be used as direct base for TemplateObjectDescription.");
  }
 protected:
  inline explicit TorqueGeneratedTemplateObjectDescription(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedTemplateObjectDescription(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

