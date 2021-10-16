class HeapNumber;
// Alias for HeapObject::IsHeapNumber() that avoids inlining.
V8_EXPORT_PRIVATE bool IsHeapNumber_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedHeapNumber : public P {
  static_assert(std::is_same<HeapNumber, D>::value,
    "Use this class as direct base for HeapNumber.");
  static_assert(std::is_same<PrimitiveHeapObject, P>::value,
    "Pass in PrimitiveHeapObject as second template parameter for TorqueGeneratedHeapNumber.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedHeapNumber<D,P>;

inline double value() const;
inline void set_value(double value, RelaxedStoreTag);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void HeapNumberVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kValueOffset = P::kHeaderSize;
  static constexpr int kValueOffsetEnd = kValueOffset + kDoubleSize - 1;
  static constexpr int kStartOfWeakFieldsOffset = kValueOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kValueOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kValueOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kValueOffsetEnd + 1;
  static constexpr int kHeaderSize = kValueOffsetEnd + 1;
  static constexpr int kSize = kValueOffsetEnd + 1;

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
  constexpr TorqueGeneratedHeapNumber() : P() {
    static_assert(std::is_base_of<TorqueGeneratedHeapNumber, 
      DAlias>::value,
      "class TorqueGeneratedHeapNumber should be used as direct base for HeapNumber.");
  }
 protected:
  inline explicit TorqueGeneratedHeapNumber(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedHeapNumber(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

