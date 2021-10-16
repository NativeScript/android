class SyntheticModule;
// Alias for HeapObject::IsSyntheticModule() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSyntheticModule_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSyntheticModule : public P {
  static_assert(std::is_same<SyntheticModule, D>::value,
    "Use this class as direct base for SyntheticModule.");
  static_assert(std::is_same<Module, P>::value,
    "Pass in Module as second template parameter for TorqueGeneratedSyntheticModule.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSyntheticModule<D,P>;

inline String name() const;
inline String name(PtrComprCageBase cage_base) const;
inline void set_name(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray export_names() const;
inline FixedArray export_names(PtrComprCageBase cage_base) const;
inline void set_export_names(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Foreign evaluation_steps() const;
inline Foreign evaluation_steps(PtrComprCageBase cage_base) const;
inline void set_evaluation_steps(Foreign value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SyntheticModuleVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kNameOffset = P::kHeaderSize;
  static constexpr int kNameOffsetEnd = kNameOffset + kTaggedSize - 1;
  static constexpr int kExportNamesOffset = kNameOffsetEnd + 1;
  static constexpr int kExportNamesOffsetEnd = kExportNamesOffset + kTaggedSize - 1;
  static constexpr int kEvaluationStepsOffset = kExportNamesOffsetEnd + 1;
  static constexpr int kEvaluationStepsOffsetEnd = kEvaluationStepsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kEvaluationStepsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kEvaluationStepsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kEvaluationStepsOffsetEnd + 1;
  static constexpr int kHeaderSize = kEvaluationStepsOffsetEnd + 1;
  static constexpr int kSize = kEvaluationStepsOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 40);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedSyntheticModule() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSyntheticModule, 
      DAlias>::value,
      "class TorqueGeneratedSyntheticModule should be used as direct base for SyntheticModule.");
  }
 protected:
  inline explicit TorqueGeneratedSyntheticModule(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSyntheticModule(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

