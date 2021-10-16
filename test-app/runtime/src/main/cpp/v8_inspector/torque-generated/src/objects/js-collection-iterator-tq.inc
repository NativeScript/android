class JSCollectionIterator;
// Alias for HeapObject::IsJSCollectionIterator() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSCollectionIterator_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSCollectionIterator : public P {
  static_assert(std::is_same<JSCollectionIterator, D>::value,
    "Use this class as direct base for JSCollectionIterator.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSCollectionIterator.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSCollectionIterator<D,P>;

inline Object table() const;
inline Object table(PtrComprCageBase cage_base) const;
inline void set_table(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object index() const;
inline Object index(PtrComprCageBase cage_base) const;
inline void set_index(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSCollectionIteratorVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kTableOffset = P::kHeaderSize;
  static constexpr int kTableOffsetEnd = kTableOffset + kTaggedSize - 1;
  static constexpr int kIndexOffset = kTableOffsetEnd + 1;
  static constexpr int kIndexOffsetEnd = kIndexOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kIndexOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kIndexOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kIndexOffsetEnd + 1;
  static constexpr int kHeaderSize = kIndexOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSCollectionIterator() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSCollectionIterator, 
      DAlias>::value,
      "class TorqueGeneratedJSCollectionIterator should be used as direct base for JSCollectionIterator.");
  }
 protected:
  inline explicit TorqueGeneratedJSCollectionIterator(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSCollectionIterator(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

