class JSRegExpStringIterator;
// Alias for HeapObject::IsJSRegExpStringIterator() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSRegExpStringIterator_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSRegExpStringIterator : public P {
  static_assert(std::is_same<JSRegExpStringIterator, D>::value,
    "Use this class as direct base for JSRegExpStringIterator.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSRegExpStringIterator.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSRegExpStringIterator<D,P>;

inline JSReceiver iterating_reg_exp() const;
inline JSReceiver iterating_reg_exp(PtrComprCageBase cage_base) const;
inline void set_iterating_reg_exp(JSReceiver value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline String iterated_string() const;
inline String iterated_string(PtrComprCageBase cage_base) const;
inline void set_iterated_string(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int flags() const;
inline void set_flags(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSRegExpStringIteratorVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kIteratingRegExpOffset = P::kHeaderSize;
  static constexpr int kIteratingRegExpOffsetEnd = kIteratingRegExpOffset + kTaggedSize - 1;
  static constexpr int kIteratedStringOffset = kIteratingRegExpOffsetEnd + 1;
  static constexpr int kIteratedStringOffsetEnd = kIteratedStringOffset + kTaggedSize - 1;
  static constexpr int kFlagsOffset = kIteratedStringOffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kHeaderSize = kFlagsOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSRegExpStringIterator() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSRegExpStringIterator, 
      DAlias>::value,
      "class TorqueGeneratedJSRegExpStringIterator should be used as direct base for JSRegExpStringIterator.");
  }
 protected:
  inline explicit TorqueGeneratedJSRegExpStringIterator(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSRegExpStringIterator(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

