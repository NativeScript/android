class PropertyDescriptorObject;
// Alias for HeapObject::IsPropertyDescriptorObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPropertyDescriptorObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPropertyDescriptorObject : public P {
  static_assert(std::is_same<PropertyDescriptorObject, D>::value,
    "Use this class as direct base for PropertyDescriptorObject.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedPropertyDescriptorObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPropertyDescriptorObject<D,P>;

inline int flags() const;
inline void set_flags(int value);

  // Torque type: (class JSReceiver | Smi | class HeapNumber | BigInt | class String | class Symbol | True | False | Null | Undefined | TheHole)
inline Object value() const;
inline Object value(PtrComprCageBase cage_base) const;
inline void set_value(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class JSReceiver | Smi | class HeapNumber | BigInt | class String | class Symbol | True | False | Null | Undefined | TheHole)
inline Object get() const;
inline Object get(PtrComprCageBase cage_base) const;
inline void set_get(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class JSReceiver | Smi | class HeapNumber | BigInt | class String | class Symbol | True | False | Null | Undefined | TheHole)
inline Object set() const;
inline Object set(PtrComprCageBase cage_base) const;
inline void set_set(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(PropertyDescriptorObject)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PropertyDescriptorObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kFlagsOffset = P::kHeaderSize;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kTaggedSize - 1;
  static constexpr int kValueOffset = kFlagsOffsetEnd + 1;
  static constexpr int kValueOffsetEnd = kValueOffset + kTaggedSize - 1;
  static constexpr int kGetOffset = kValueOffsetEnd + 1;
  static constexpr int kGetOffsetEnd = kGetOffset + kTaggedSize - 1;
  static constexpr int kSetOffset = kGetOffsetEnd + 1;
  static constexpr int kSetOffsetEnd = kSetOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kSetOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kSetOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kSetOffsetEnd + 1;
  static constexpr int kHeaderSize = kSetOffsetEnd + 1;
  static constexpr int kSize = kSetOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 20);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedPropertyDescriptorObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPropertyDescriptorObject, 
      DAlias>::value,
      "class TorqueGeneratedPropertyDescriptorObject should be used as direct base for PropertyDescriptorObject.");
  }
 protected:
  inline explicit TorqueGeneratedPropertyDescriptorObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPropertyDescriptorObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

