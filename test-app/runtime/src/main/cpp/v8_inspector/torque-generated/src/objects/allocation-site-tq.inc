class AllocationSite;
class AllocationMemento;
// Alias for HeapObject::IsAllocationMemento() that avoids inlining.
V8_EXPORT_PRIVATE bool IsAllocationMemento_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedAllocationMemento : public P {
  static_assert(std::is_same<AllocationMemento, D>::value,
    "Use this class as direct base for AllocationMemento.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedAllocationMemento.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedAllocationMemento<D,P>;

inline AllocationSite allocation_site() const;
inline AllocationSite allocation_site(PtrComprCageBase cage_base) const;
inline void set_allocation_site(AllocationSite value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void AllocationMementoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kAllocationSiteOffset = P::kHeaderSize;
  static constexpr int kAllocationSiteOffsetEnd = kAllocationSiteOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kAllocationSiteOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kAllocationSiteOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kAllocationSiteOffsetEnd + 1;
  static constexpr int kHeaderSize = kAllocationSiteOffsetEnd + 1;
  static constexpr int kSize = kAllocationSiteOffsetEnd + 1;

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
  constexpr TorqueGeneratedAllocationMemento() : P() {
    static_assert(std::is_base_of<TorqueGeneratedAllocationMemento, 
      DAlias>::value,
      "class TorqueGeneratedAllocationMemento should be used as direct base for AllocationMemento.");
  }
 protected:
  inline explicit TorqueGeneratedAllocationMemento(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedAllocationMemento(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

