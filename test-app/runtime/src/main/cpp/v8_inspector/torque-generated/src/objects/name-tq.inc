class Name;
class Symbol;
// Alias for HeapObject::IsName() that avoids inlining.
V8_EXPORT_PRIVATE bool IsName_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedName : public P {
  static_assert(std::is_same<Name, D>::value,
    "Use this class as direct base for Name.");
  static_assert(std::is_same<PrimitiveHeapObject, P>::value,
    "Pass in PrimitiveHeapObject as second template parameter for TorqueGeneratedName.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedName<D,P>;

inline uint32_t raw_hash_field() const;
inline void set_raw_hash_field(uint32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void NameVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kRawHashFieldOffset = P::kHeaderSize;
  static constexpr int kRawHashFieldOffsetEnd = kRawHashFieldOffset + kInt32Size - 1;
  static constexpr int kStartOfWeakFieldsOffset = kRawHashFieldOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kRawHashFieldOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kRawHashFieldOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kRawHashFieldOffsetEnd + 1;
  static constexpr int kHeaderSize = kRawHashFieldOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedName() : P() {
    static_assert(std::is_base_of<TorqueGeneratedName, 
      DAlias>::value,
      "class TorqueGeneratedName should be used as direct base for Name.");
  }
 protected:
  inline explicit TorqueGeneratedName(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedName(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsSymbol() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSymbol_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSymbol : public P {
  static_assert(std::is_same<Symbol, D>::value,
    "Use this class as direct base for Symbol.");
  static_assert(std::is_same<Name, P>::value,
    "Pass in Name as second template parameter for TorqueGeneratedSymbol.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSymbol<D,P>;

inline uint32_t flags() const;
inline void set_flags(uint32_t value);

  // Torque type: (class String | Undefined)
inline PrimitiveHeapObject description() const;
inline PrimitiveHeapObject description(PtrComprCageBase cage_base) const;
inline void set_description(PrimitiveHeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SymbolVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kFlagsOffset = P::kHeaderSize;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kInt32Size - 1;
  static constexpr int kStartOfStrongFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kDescriptionOffset = kFlagsOffsetEnd + 1;
  static constexpr int kDescriptionOffsetEnd = kDescriptionOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kDescriptionOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kDescriptionOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kDescriptionOffsetEnd + 1;
  static constexpr int kHeaderSize = kDescriptionOffsetEnd + 1;
  static constexpr int kSize = kDescriptionOffsetEnd + 1;

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
  constexpr TorqueGeneratedSymbol() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSymbol, 
      DAlias>::value,
      "class TorqueGeneratedSymbol should be used as direct base for Symbol.");
  }
 protected:
  inline explicit TorqueGeneratedSymbol(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSymbol(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

