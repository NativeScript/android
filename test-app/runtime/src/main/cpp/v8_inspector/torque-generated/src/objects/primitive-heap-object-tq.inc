class PrimitiveHeapObject;
// Alias for HeapObject::IsPrimitiveHeapObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPrimitiveHeapObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPrimitiveHeapObject : public P {
  static_assert(std::is_same<PrimitiveHeapObject, D>::value,
    "Use this class as direct base for PrimitiveHeapObject.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedPrimitiveHeapObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPrimitiveHeapObject<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PrimitiveHeapObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedPrimitiveHeapObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPrimitiveHeapObject, 
      DAlias>::value,
      "class TorqueGeneratedPrimitiveHeapObject should be used as direct base for PrimitiveHeapObject.");
  }
 protected:
  inline explicit TorqueGeneratedPrimitiveHeapObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPrimitiveHeapObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

