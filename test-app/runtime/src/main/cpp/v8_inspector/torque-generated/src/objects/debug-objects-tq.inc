class BreakPoint;
class BreakPointInfo;
class DebugInfo;
class CoverageInfo;
// Alias for HeapObject::IsBreakPoint() that avoids inlining.
V8_EXPORT_PRIVATE bool IsBreakPoint_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedBreakPoint : public P {
  static_assert(std::is_same<BreakPoint, D>::value,
    "Use this class as direct base for BreakPoint.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedBreakPoint.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedBreakPoint<D,P>;

inline int id() const;
inline void set_id(int value);

inline String condition() const;
inline String condition(PtrComprCageBase cage_base) const;
inline void set_condition(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(BreakPoint)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void BreakPointVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kIdOffset = P::kHeaderSize;
  static constexpr int kIdOffsetEnd = kIdOffset + kTaggedSize - 1;
  static constexpr int kConditionOffset = kIdOffsetEnd + 1;
  static constexpr int kConditionOffsetEnd = kConditionOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kConditionOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kConditionOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kConditionOffsetEnd + 1;
  static constexpr int kHeaderSize = kConditionOffsetEnd + 1;
  static constexpr int kSize = kConditionOffsetEnd + 1;

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
  constexpr TorqueGeneratedBreakPoint() : P() {
    static_assert(std::is_base_of<TorqueGeneratedBreakPoint, 
      DAlias>::value,
      "class TorqueGeneratedBreakPoint should be used as direct base for BreakPoint.");
  }
 protected:
  inline explicit TorqueGeneratedBreakPoint(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedBreakPoint(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsBreakPointInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsBreakPointInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedBreakPointInfo : public P {
  static_assert(std::is_same<BreakPointInfo, D>::value,
    "Use this class as direct base for BreakPointInfo.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedBreakPointInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedBreakPointInfo<D,P>;

inline int source_position() const;
inline void set_source_position(int value);

  // Torque type: (Undefined | class FixedArray | class BreakPoint)
inline HeapObject break_points() const;
inline HeapObject break_points(PtrComprCageBase cage_base) const;
inline void set_break_points(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(BreakPointInfo)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void BreakPointInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kSourcePositionOffset = P::kHeaderSize;
  static constexpr int kSourcePositionOffsetEnd = kSourcePositionOffset + kTaggedSize - 1;
  static constexpr int kBreakPointsOffset = kSourcePositionOffsetEnd + 1;
  static constexpr int kBreakPointsOffsetEnd = kBreakPointsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kBreakPointsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kBreakPointsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kBreakPointsOffsetEnd + 1;
  static constexpr int kHeaderSize = kBreakPointsOffsetEnd + 1;
  static constexpr int kSize = kBreakPointsOffsetEnd + 1;

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
  constexpr TorqueGeneratedBreakPointInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedBreakPointInfo, 
      DAlias>::value,
      "class TorqueGeneratedBreakPointInfo should be used as direct base for BreakPointInfo.");
  }
 protected:
  inline explicit TorqueGeneratedBreakPointInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedBreakPointInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsDebugInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsDebugInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedDebugInfo : public P {
  static_assert(std::is_same<DebugInfo, D>::value,
    "Use this class as direct base for DebugInfo.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedDebugInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedDebugInfo<D,P>;

inline SharedFunctionInfo shared() const;
inline SharedFunctionInfo shared(PtrComprCageBase cage_base) const;
inline void set_shared(SharedFunctionInfo value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int debugger_hints() const;
inline void set_debugger_hints(int value);

  // Torque type: (Undefined | class Script)
inline HeapObject script() const;
inline HeapObject script(PtrComprCageBase cage_base) const;
inline void set_script(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class BytecodeArray)
inline HeapObject original_bytecode_array(AcquireLoadTag) const;
inline HeapObject original_bytecode_array(PtrComprCageBase cage_base, AcquireLoadTag) const;
inline void set_original_bytecode_array(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class BytecodeArray)
inline HeapObject debug_bytecode_array(AcquireLoadTag) const;
inline HeapObject debug_bytecode_array(PtrComprCageBase cage_base, AcquireLoadTag) const;
inline void set_debug_bytecode_array(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray break_points() const;
inline FixedArray break_points(PtrComprCageBase cage_base) const;
inline void set_break_points(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int flags(RelaxedLoadTag) const;
inline void set_flags(int value, RelaxedStoreTag);

  // Torque type: (Undefined | class CoverageInfo)
inline HeapObject coverage_info() const;
inline HeapObject coverage_info(PtrComprCageBase cage_base) const;
inline void set_coverage_info(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(DebugInfo)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void DebugInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kSharedOffset = P::kHeaderSize;
  static constexpr int kSharedOffsetEnd = kSharedOffset + kTaggedSize - 1;
  static constexpr int kDebuggerHintsOffset = kSharedOffsetEnd + 1;
  static constexpr int kDebuggerHintsOffsetEnd = kDebuggerHintsOffset + kTaggedSize - 1;
  static constexpr int kScriptOffset = kDebuggerHintsOffsetEnd + 1;
  static constexpr int kScriptOffsetEnd = kScriptOffset + kTaggedSize - 1;
  static constexpr int kOriginalBytecodeArrayOffset = kScriptOffsetEnd + 1;
  static constexpr int kOriginalBytecodeArrayOffsetEnd = kOriginalBytecodeArrayOffset + kTaggedSize - 1;
  static constexpr int kDebugBytecodeArrayOffset = kOriginalBytecodeArrayOffsetEnd + 1;
  static constexpr int kDebugBytecodeArrayOffsetEnd = kDebugBytecodeArrayOffset + kTaggedSize - 1;
  static constexpr int kBreakPointsOffset = kDebugBytecodeArrayOffsetEnd + 1;
  static constexpr int kBreakPointsOffsetEnd = kBreakPointsOffset + kTaggedSize - 1;
  static constexpr int kFlagsOffset = kBreakPointsOffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kTaggedSize - 1;
  static constexpr int kCoverageInfoOffset = kFlagsOffsetEnd + 1;
  static constexpr int kCoverageInfoOffsetEnd = kCoverageInfoOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kCoverageInfoOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kCoverageInfoOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kCoverageInfoOffsetEnd + 1;
  static constexpr int kHeaderSize = kCoverageInfoOffsetEnd + 1;
  static constexpr int kSize = kCoverageInfoOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 36);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedDebugInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedDebugInfo, 
      DAlias>::value,
      "class TorqueGeneratedDebugInfo should be used as direct base for DebugInfo.");
  }
 protected:
  inline explicit TorqueGeneratedDebugInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedDebugInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsCoverageInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsCoverageInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedCoverageInfo : public P {
  static_assert(std::is_same<CoverageInfo, D>::value,
    "Use this class as direct base for CoverageInfo.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedCoverageInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedCoverageInfo<D,P>;

inline int32_t slot_count() const;
inline void set_slot_count(int32_t value);

inline int32_t slots_start_source_position(int i) const;
inline void set_slots_start_source_position(int i, int32_t value);

inline int32_t slots_end_source_position(int i) const;
inline void set_slots_end_source_position(int i, int32_t value);

inline int32_t slots_block_count(int i) const;
inline void set_slots_block_count(int i, int32_t value);

inline int32_t slots_padding(int i) const;
inline void set_slots_padding(int i, int32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void CoverageInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kSlotCountOffset = P::kHeaderSize;
  static constexpr int kSlotCountOffsetEnd = kSlotCountOffset + kInt32Size - 1;
  static constexpr int kHeaderSize = kSlotCountOffsetEnd + 1;
  static constexpr int kSlotsOffset = kSlotCountOffsetEnd + 1;
  static constexpr int kSlotsOffsetEnd = kSlotsOffset + 0 - 1;
  static constexpr int kStartOfWeakFieldsOffset = kSlotsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kSlotsOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kSlotsOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kSlotsOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor(int slot_count) {
    int32_t size = kHeaderSize;
    size += slot_count * 16;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor(this->slot_count());
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedCoverageInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedCoverageInfo, 
      DAlias>::value,
      "class TorqueGeneratedCoverageInfo should be used as direct base for CoverageInfo.");
  }
 protected:
  inline explicit TorqueGeneratedCoverageInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedCoverageInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

struct TorqueGeneratedCoverageInfoSlotOffsets {
  static constexpr int kStartSourcePositionOffset = 0;
  static constexpr int kEndSourcePositionOffset = 4;
  static constexpr int kBlockCountOffset = 8;
  static constexpr int kPaddingOffset = 12;
  static constexpr int kSize = 16;
};

