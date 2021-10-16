class JSPromise;
class JSFunction;
// Alias for HeapObject::IsJSPromise() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSPromise_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSPromise : public P {
  static_assert(std::is_same<JSPromise, D>::value,
    "Use this class as direct base for JSPromise.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSPromise.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSPromise<D,P>;

  // Torque type: (class JSReceiver | Smi | class HeapNumber | BigInt | class String | class Symbol | True | False | Null | Undefined | class PromiseReaction)
inline Object reactions_or_result() const;
inline Object reactions_or_result(PtrComprCageBase cage_base) const;
inline void set_reactions_or_result(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int flags() const;
inline void set_flags(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSPromiseVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kReactionsOrResultOffset = P::kHeaderSize;
  static constexpr int kReactionsOrResultOffsetEnd = kReactionsOrResultOffset + kTaggedSize - 1;
  static constexpr int kFlagsOffset = kReactionsOrResultOffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kHeaderSize = kFlagsOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSPromise() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSPromise, 
      DAlias>::value,
      "class TorqueGeneratedJSPromise should be used as direct base for JSPromise.");
  }
 protected:
  inline explicit TorqueGeneratedJSPromise(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSPromise(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

