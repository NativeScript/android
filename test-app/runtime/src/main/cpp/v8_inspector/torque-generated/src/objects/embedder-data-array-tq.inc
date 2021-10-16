class EmbedderDataArray;
// Alias for HeapObject::IsEmbedderDataArray() that avoids inlining.
V8_EXPORT_PRIVATE bool IsEmbedderDataArray_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedEmbedderDataArray : public P {
  static_assert(std::is_same<EmbedderDataArray, D>::value,
    "Use this class as direct base for EmbedderDataArray.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedEmbedderDataArray.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedEmbedderDataArray<D,P>;

inline int length() const;
inline void set_length(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void EmbedderDataArrayVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kLengthOffset = P::kHeaderSize;
  static constexpr int kLengthOffsetEnd = kLengthOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kLengthOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kLengthOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kLengthOffsetEnd + 1;
  static constexpr int kHeaderSize = kLengthOffsetEnd + 1;
  static constexpr int kSize = kLengthOffsetEnd + 1;

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
  constexpr TorqueGeneratedEmbedderDataArray() : P() {
    static_assert(std::is_base_of<TorqueGeneratedEmbedderDataArray, 
      DAlias>::value,
      "class TorqueGeneratedEmbedderDataArray should be used as direct base for EmbedderDataArray.");
  }
 protected:
  inline explicit TorqueGeneratedEmbedderDataArray(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedEmbedderDataArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

