class StackFrameInfo;
// Alias for HeapObject::IsStackFrameInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsStackFrameInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedStackFrameInfo : public P {
  static_assert(std::is_same<StackFrameInfo, D>::value,
    "Use this class as direct base for StackFrameInfo.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedStackFrameInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedStackFrameInfo<D,P>;

  // Torque type: JSAny
inline Object receiver_or_instance() const;
inline Object receiver_or_instance(PtrComprCageBase cage_base) const;
inline void set_receiver_or_instance(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | class JSFunction)
inline Object function() const;
inline Object function(PtrComprCageBase cage_base) const;
inline void set_function(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline HeapObject code_object() const;
inline HeapObject code_object(PtrComprCageBase cage_base) const;
inline void set_code_object(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int code_offset_or_source_position() const;
inline void set_code_offset_or_source_position(int value);

inline int flags() const;
inline void set_flags(int value);

inline FixedArray parameters() const;
inline FixedArray parameters(PtrComprCageBase cage_base) const;
inline void set_parameters(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void StackFrameInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kReceiverOrInstanceOffset = P::kHeaderSize;
  static constexpr int kReceiverOrInstanceOffsetEnd = kReceiverOrInstanceOffset + kTaggedSize - 1;
  static constexpr int kFunctionOffset = kReceiverOrInstanceOffsetEnd + 1;
  static constexpr int kFunctionOffsetEnd = kFunctionOffset + kTaggedSize - 1;
  static constexpr int kCodeObjectOffset = kFunctionOffsetEnd + 1;
  static constexpr int kCodeObjectOffsetEnd = kCodeObjectOffset + kTaggedSize - 1;
  static constexpr int kCodeOffsetOrSourcePositionOffset = kCodeObjectOffsetEnd + 1;
  static constexpr int kCodeOffsetOrSourcePositionOffsetEnd = kCodeOffsetOrSourcePositionOffset + kTaggedSize - 1;
  static constexpr int kFlagsOffset = kCodeOffsetOrSourcePositionOffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kTaggedSize - 1;
  static constexpr int kParametersOffset = kFlagsOffsetEnd + 1;
  static constexpr int kParametersOffsetEnd = kParametersOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kParametersOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kParametersOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kParametersOffsetEnd + 1;
  static constexpr int kHeaderSize = kParametersOffsetEnd + 1;
  static constexpr int kSize = kParametersOffsetEnd + 1;

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
  constexpr TorqueGeneratedStackFrameInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedStackFrameInfo, 
      DAlias>::value,
      "class TorqueGeneratedStackFrameInfo should be used as direct base for StackFrameInfo.");
  }
 protected:
  inline explicit TorqueGeneratedStackFrameInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedStackFrameInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

