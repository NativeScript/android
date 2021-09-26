class FeedbackCell;
// Alias for HeapObject::IsFeedbackCell() that avoids inlining.
V8_EXPORT_PRIVATE bool IsFeedbackCell_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedFeedbackCell : public P {
  static_assert(std::is_same<FeedbackCell, D>::value,
    "Use this class as direct base for FeedbackCell.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedFeedbackCell.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedFeedbackCell<D,P>;

  // Torque type: (Undefined | class FixedArray | class FeedbackVector)
inline HeapObject value() const;
inline HeapObject value(PtrComprCageBase cage_base) const;
inline void set_value(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int32_t interrupt_budget() const;
inline void set_interrupt_budget(int32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void FeedbackCellVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kValueOffset = P::kHeaderSize;
  static constexpr int kValueOffsetEnd = kValueOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kValueOffsetEnd + 1;
  static constexpr int kInterruptBudgetOffset = kValueOffsetEnd + 1;
  static constexpr int kInterruptBudgetOffsetEnd = kInterruptBudgetOffset + kInt32Size - 1;
  static constexpr int kStartOfWeakFieldsOffset = kInterruptBudgetOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kInterruptBudgetOffsetEnd + 1;
  static constexpr int kHeaderSize = kInterruptBudgetOffsetEnd + 1;
  static constexpr int kSize = kInterruptBudgetOffsetEnd + 1;

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
  constexpr TorqueGeneratedFeedbackCell() : P() {
    static_assert(std::is_base_of<TorqueGeneratedFeedbackCell, 
      DAlias>::value,
      "class TorqueGeneratedFeedbackCell should be used as direct base for FeedbackCell.");
  }
 protected:
  inline explicit TorqueGeneratedFeedbackCell(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedFeedbackCell(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

