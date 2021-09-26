class Oddball;
// Alias for HeapObject::IsOddball() that avoids inlining.
V8_EXPORT_PRIVATE bool IsOddball_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedOddball : public P {
  static_assert(std::is_same<Oddball, D>::value,
    "Use this class as direct base for Oddball.");
  static_assert(std::is_same<PrimitiveHeapObject, P>::value,
    "Pass in PrimitiveHeapObject as second template parameter for TorqueGeneratedOddball.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedOddball<D,P>;

inline double to_number_raw() const;
inline void set_to_number_raw(double value);

inline String to_string() const;
inline String to_string(PtrComprCageBase cage_base) const;
inline void set_to_string(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: Number
inline Object to_number() const;
inline Object to_number(PtrComprCageBase cage_base) const;
inline void set_to_number(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline String type_of() const;
inline String type_of(PtrComprCageBase cage_base) const;
inline void set_type_of(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int kind() const;
inline void set_kind(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(Oddball)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void OddballVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kToNumberRawOffset = P::kHeaderSize;
  static constexpr int kToNumberRawOffsetEnd = kToNumberRawOffset + kDoubleSize - 1;
  static constexpr int kStartOfStrongFieldsOffset = kToNumberRawOffsetEnd + 1;
  static constexpr int kToStringOffset = kToNumberRawOffsetEnd + 1;
  static constexpr int kToStringOffsetEnd = kToStringOffset + kTaggedSize - 1;
  static constexpr int kToNumberOffset = kToStringOffsetEnd + 1;
  static constexpr int kToNumberOffsetEnd = kToNumberOffset + kTaggedSize - 1;
  static constexpr int kTypeOfOffset = kToNumberOffsetEnd + 1;
  static constexpr int kTypeOfOffsetEnd = kTypeOfOffset + kTaggedSize - 1;
  static constexpr int kKindOffset = kTypeOfOffsetEnd + 1;
  static constexpr int kKindOffsetEnd = kKindOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kKindOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kKindOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kKindOffsetEnd + 1;
  static constexpr int kHeaderSize = kKindOffsetEnd + 1;
  static constexpr int kSize = kKindOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 28);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedOddball() : P() {
    static_assert(std::is_base_of<TorqueGeneratedOddball, 
      DAlias>::value,
      "class TorqueGeneratedOddball should be used as direct base for Oddball.");
  }
 protected:
  inline explicit TorqueGeneratedOddball(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedOddball(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

