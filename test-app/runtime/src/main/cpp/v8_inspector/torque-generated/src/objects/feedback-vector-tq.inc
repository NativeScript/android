class FeedbackVector;
class FeedbackMetadata;
// Alias for HeapObject::IsFeedbackVector() that avoids inlining.
V8_EXPORT_PRIVATE bool IsFeedbackVector_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedFeedbackVector : public P {
  static_assert(std::is_same<FeedbackVector, D>::value,
    "Use this class as direct base for FeedbackVector.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedFeedbackVector.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedFeedbackVector<D,P>;

inline int32_t length() const;
inline void set_length(int32_t value);

inline int32_t invocation_count() const;
inline void set_invocation_count(int32_t value);

inline int32_t profiler_ticks() const;
inline void set_profiler_ticks(int32_t value);

inline uint32_t flags() const;
inline void set_flags(uint32_t value);

inline SharedFunctionInfo shared_function_info() const;
inline SharedFunctionInfo shared_function_info(PtrComprCageBase cage_base) const;
inline void set_shared_function_info(SharedFunctionInfo value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: Weak<class Code>
inline MaybeObject maybe_optimized_code() const;
inline MaybeObject maybe_optimized_code(PtrComprCageBase cage_base) const;
inline void set_maybe_optimized_code(MaybeObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline ClosureFeedbackCellArray closure_feedback_cell_array() const;
inline ClosureFeedbackCellArray closure_feedback_cell_array(PtrComprCageBase cage_base) const;
inline void set_closure_feedback_cell_array(ClosureFeedbackCellArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: MaybeObject
inline MaybeObject raw_feedback_slots(int i, RelaxedLoadTag) const;
inline MaybeObject raw_feedback_slots(PtrComprCageBase cage_base, int i, RelaxedLoadTag) const;
inline void set_raw_feedback_slots(int i, MaybeObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void FeedbackVectorVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kLengthOffset = P::kHeaderSize;
  static constexpr int kLengthOffsetEnd = kLengthOffset + kInt32Size - 1;
  static constexpr int kInvocationCountOffset = kLengthOffsetEnd + 1;
  static constexpr int kInvocationCountOffsetEnd = kInvocationCountOffset + kInt32Size - 1;
  static constexpr int kProfilerTicksOffset = kInvocationCountOffsetEnd + 1;
  static constexpr int kProfilerTicksOffsetEnd = kProfilerTicksOffset + kInt32Size - 1;
  static constexpr int kFlagsOffset = kProfilerTicksOffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kInt32Size - 1;
  static constexpr int kStartOfStrongFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kSharedFunctionInfoOffset = kFlagsOffsetEnd + 1;
  static constexpr int kSharedFunctionInfoOffsetEnd = kSharedFunctionInfoOffset + kTaggedSize - 1;
  static constexpr int kMaybeOptimizedCodeOffset = kSharedFunctionInfoOffsetEnd + 1;
  static constexpr int kMaybeOptimizedCodeOffsetEnd = kMaybeOptimizedCodeOffset + kTaggedSize - 1;
  static constexpr int kClosureFeedbackCellArrayOffset = kMaybeOptimizedCodeOffsetEnd + 1;
  static constexpr int kClosureFeedbackCellArrayOffsetEnd = kClosureFeedbackCellArrayOffset + kTaggedSize - 1;
  static constexpr int kHeaderSize = kClosureFeedbackCellArrayOffsetEnd + 1;
  static constexpr int kRawFeedbackSlotsOffset = kClosureFeedbackCellArrayOffsetEnd + 1;
  static constexpr int kRawFeedbackSlotsOffsetEnd = kRawFeedbackSlotsOffset + 0 - 1;
  static constexpr int kEndOfStrongFieldsOffset = kRawFeedbackSlotsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kRawFeedbackSlotsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kRawFeedbackSlotsOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor(int length) {
    int32_t size = kHeaderSize;
    size += length * 4;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor(this->length());
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedFeedbackVector() : P() {
    static_assert(std::is_base_of<TorqueGeneratedFeedbackVector, 
      DAlias>::value,
      "class TorqueGeneratedFeedbackVector should be used as direct base for FeedbackVector.");
  }
 protected:
  inline explicit TorqueGeneratedFeedbackVector(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedFeedbackVector(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

