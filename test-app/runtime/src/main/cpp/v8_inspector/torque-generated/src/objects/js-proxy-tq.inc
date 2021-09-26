class JSProxy;
class JSObject;
// Alias for HeapObject::IsJSProxy() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSProxy_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSProxy : public P {
  static_assert(std::is_same<JSProxy, D>::value,
    "Use this class as direct base for JSProxy.");
  static_assert(std::is_same<JSReceiver, P>::value,
    "Pass in JSReceiver as second template parameter for TorqueGeneratedJSProxy.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSProxy<D,P>;

  // Torque type: (class JSReceiver | Null)
inline HeapObject target() const;
inline HeapObject target(PtrComprCageBase cage_base) const;
inline void set_target(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class JSReceiver | Null)
inline HeapObject handler() const;
inline HeapObject handler(PtrComprCageBase cage_base) const;
inline void set_handler(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSProxyVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kTargetOffset = P::kHeaderSize;
  static constexpr int kTargetOffsetEnd = kTargetOffset + kTaggedSize - 1;
  static constexpr int kHandlerOffset = kTargetOffsetEnd + 1;
  static constexpr int kHandlerOffsetEnd = kHandlerOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kHandlerOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kHandlerOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kHandlerOffsetEnd + 1;
  static constexpr int kHeaderSize = kHandlerOffsetEnd + 1;
  static constexpr int kSize = kHandlerOffsetEnd + 1;

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
  constexpr TorqueGeneratedJSProxy() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSProxy, 
      DAlias>::value,
      "class TorqueGeneratedJSProxy should be used as direct base for JSProxy.");
  }
 protected:
  inline explicit TorqueGeneratedJSProxy(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSProxy(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

