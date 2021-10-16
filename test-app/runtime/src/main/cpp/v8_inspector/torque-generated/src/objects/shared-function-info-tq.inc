class PreparseData;
class InterpreterData;
class BaselineData;
class SharedFunctionInfo;
class UncompiledData;
class UncompiledDataWithoutPreparseData;
class UncompiledDataWithPreparseData;
class OnHeapBasicBlockProfilerData;
// Alias for HeapObject::IsPreparseData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPreparseData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPreparseData : public P {
  static_assert(std::is_same<PreparseData, D>::value,
    "Use this class as direct base for PreparseData.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedPreparseData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPreparseData<D,P>;

inline int32_t data_length() const;
inline void set_data_length(int32_t value);

inline int32_t children_length() const;
inline void set_children_length(int32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PreparseDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kDataLengthOffset = P::kHeaderSize;
  static constexpr int kDataLengthOffsetEnd = kDataLengthOffset + kInt32Size - 1;
  static constexpr int kChildrenLengthOffset = kDataLengthOffsetEnd + 1;
  static constexpr int kChildrenLengthOffsetEnd = kChildrenLengthOffset + kInt32Size - 1;
  static constexpr int kStartOfWeakFieldsOffset = kChildrenLengthOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kChildrenLengthOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kChildrenLengthOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kChildrenLengthOffsetEnd + 1;
  static constexpr int kHeaderSize = kChildrenLengthOffsetEnd + 1;
  static constexpr int kSize = kChildrenLengthOffsetEnd + 1;

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
  constexpr TorqueGeneratedPreparseData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPreparseData, 
      DAlias>::value,
      "class TorqueGeneratedPreparseData should be used as direct base for PreparseData.");
  }
 protected:
  inline explicit TorqueGeneratedPreparseData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPreparseData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsBaselineData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsBaselineData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedBaselineData : public P {
  static_assert(std::is_same<BaselineData, D>::value,
    "Use this class as direct base for BaselineData.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedBaselineData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedBaselineData<D,P>;

inline Code baseline_code() const;
inline Code baseline_code(PtrComprCageBase cage_base) const;
inline void set_baseline_code(Code value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class BytecodeArray | class InterpreterData)
inline HeapObject data() const;
inline HeapObject data(PtrComprCageBase cage_base) const;
inline void set_data(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(BaselineData)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void BaselineDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kBaselineCodeOffset = P::kHeaderSize;
  static constexpr int kBaselineCodeOffsetEnd = kBaselineCodeOffset + kTaggedSize - 1;
  static constexpr int kDataOffset = kBaselineCodeOffsetEnd + 1;
  static constexpr int kDataOffsetEnd = kDataOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kHeaderSize = kDataOffsetEnd + 1;
  static constexpr int kSize = kDataOffsetEnd + 1;

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
  constexpr TorqueGeneratedBaselineData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedBaselineData, 
      DAlias>::value,
      "class TorqueGeneratedBaselineData should be used as direct base for BaselineData.");
  }
 protected:
  inline explicit TorqueGeneratedBaselineData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedBaselineData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsSharedFunctionInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSharedFunctionInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSharedFunctionInfo : public P {
  static_assert(std::is_same<SharedFunctionInfo, D>::value,
    "Use this class as direct base for SharedFunctionInfo.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedSharedFunctionInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSharedFunctionInfo<D,P>;

inline Object function_data() const;
inline Object function_data(PtrComprCageBase cage_base) const;
inline void set_function_data(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class String | NoSharedNameSentinel | class ScopeInfo)
inline Object name_or_scope_info() const;
inline Object name_or_scope_info(PtrComprCageBase cage_base) const;
inline void set_name_or_scope_info(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline HeapObject outer_scope_info_or_feedback_metadata() const;
inline HeapObject outer_scope_info_or_feedback_metadata(PtrComprCageBase cage_base) const;
inline void set_outer_scope_info_or_feedback_metadata(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class DebugInfo | class Script)
inline HeapObject script_or_debug_info() const;
inline HeapObject script_or_debug_info(PtrComprCageBase cage_base) const;
inline void set_script_or_debug_info(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int16_t length() const;
inline void set_length(int16_t value);

inline uint16_t formal_parameter_count() const;
inline void set_formal_parameter_count(uint16_t value);

inline uint16_t function_token_offset() const;
inline void set_function_token_offset(uint16_t value);

inline uint8_t expected_nof_properties() const;
inline void set_expected_nof_properties(uint8_t value);

inline uint8_t flags2() const;
inline void set_flags2(uint8_t value);

inline uint32_t flags() const;
inline void set_flags(uint32_t value);

inline int32_t function_literal_id() const;
inline void set_function_literal_id(int32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(SharedFunctionInfo)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SharedFunctionInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kFunctionDataOffset = P::kHeaderSize;
  static constexpr int kFunctionDataOffsetEnd = kFunctionDataOffset + kTaggedSize - 1;
  static constexpr int kEndOfWeakFieldsOffset = kFunctionDataOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kFunctionDataOffsetEnd + 1;
  static constexpr int kNameOrScopeInfoOffset = kFunctionDataOffsetEnd + 1;
  static constexpr int kNameOrScopeInfoOffsetEnd = kNameOrScopeInfoOffset + kTaggedSize - 1;
  static constexpr int kOuterScopeInfoOrFeedbackMetadataOffset = kNameOrScopeInfoOffsetEnd + 1;
  static constexpr int kOuterScopeInfoOrFeedbackMetadataOffsetEnd = kOuterScopeInfoOrFeedbackMetadataOffset + kTaggedSize - 1;
  static constexpr int kScriptOrDebugInfoOffset = kOuterScopeInfoOrFeedbackMetadataOffsetEnd + 1;
  static constexpr int kScriptOrDebugInfoOffsetEnd = kScriptOrDebugInfoOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kScriptOrDebugInfoOffsetEnd + 1;
  static constexpr int kLengthOffset = kScriptOrDebugInfoOffsetEnd + 1;
  static constexpr int kLengthOffsetEnd = kLengthOffset + kUInt16Size - 1;
  static constexpr int kFormalParameterCountOffset = kLengthOffsetEnd + 1;
  static constexpr int kFormalParameterCountOffsetEnd = kFormalParameterCountOffset + kUInt16Size - 1;
  static constexpr int kFunctionTokenOffsetOffset = kFormalParameterCountOffsetEnd + 1;
  static constexpr int kFunctionTokenOffsetOffsetEnd = kFunctionTokenOffsetOffset + kUInt16Size - 1;
  static constexpr int kExpectedNofPropertiesOffset = kFunctionTokenOffsetOffsetEnd + 1;
  static constexpr int kExpectedNofPropertiesOffsetEnd = kExpectedNofPropertiesOffset + kUInt8Size - 1;
  static constexpr int kFlags2Offset = kExpectedNofPropertiesOffsetEnd + 1;
  static constexpr int kFlags2OffsetEnd = kFlags2Offset + kUInt8Size - 1;
  static constexpr int kFlagsOffset = kFlags2OffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kInt32Size - 1;
  static constexpr int kFunctionLiteralIdOffset = kFlagsOffsetEnd + 1;
  static constexpr int kFunctionLiteralIdOffsetEnd = kFunctionLiteralIdOffset + kInt32Size - 1;
  static constexpr int kHeaderSize = kFunctionLiteralIdOffsetEnd + 1;
  static constexpr int kSize = kFunctionLiteralIdOffsetEnd + 1;

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
  constexpr TorqueGeneratedSharedFunctionInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSharedFunctionInfo, 
      DAlias>::value,
      "class TorqueGeneratedSharedFunctionInfo should be used as direct base for SharedFunctionInfo.");
  }
 protected:
  inline explicit TorqueGeneratedSharedFunctionInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSharedFunctionInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsUncompiledData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsUncompiledData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedUncompiledData : public P {
  static_assert(std::is_same<UncompiledData, D>::value,
    "Use this class as direct base for UncompiledData.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedUncompiledData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedUncompiledData<D,P>;

inline String inferred_name() const;
inline String inferred_name(PtrComprCageBase cage_base) const;
inline void set_inferred_name(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int32_t start_position() const;
inline void set_start_position(int32_t value);

inline int32_t end_position() const;
inline void set_end_position(int32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(UncompiledData)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void UncompiledDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kInferredNameOffset = P::kHeaderSize;
  static constexpr int kInferredNameOffsetEnd = kInferredNameOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kInferredNameOffsetEnd + 1;
  static constexpr int kStartPositionOffset = kInferredNameOffsetEnd + 1;
  static constexpr int kStartPositionOffsetEnd = kStartPositionOffset + kInt32Size - 1;
  static constexpr int kEndPositionOffset = kStartPositionOffsetEnd + 1;
  static constexpr int kEndPositionOffsetEnd = kEndPositionOffset + kInt32Size - 1;
  static constexpr int kStartOfWeakFieldsOffset = kEndPositionOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kEndPositionOffsetEnd + 1;
  static constexpr int kHeaderSize = kEndPositionOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedUncompiledData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedUncompiledData, 
      DAlias>::value,
      "class TorqueGeneratedUncompiledData should be used as direct base for UncompiledData.");
  }
 protected:
  inline explicit TorqueGeneratedUncompiledData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedUncompiledData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsUncompiledDataWithoutPreparseData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsUncompiledDataWithoutPreparseData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedUncompiledDataWithoutPreparseData : public P {
  static_assert(std::is_same<UncompiledDataWithoutPreparseData, D>::value,
    "Use this class as direct base for UncompiledDataWithoutPreparseData.");
  static_assert(std::is_same<UncompiledData, P>::value,
    "Pass in UncompiledData as second template parameter for TorqueGeneratedUncompiledDataWithoutPreparseData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedUncompiledDataWithoutPreparseData<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(UncompiledDataWithoutPreparseData)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void UncompiledDataWithoutPreparseDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;
  static constexpr int kSize = P::kHeaderSize;

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
  constexpr TorqueGeneratedUncompiledDataWithoutPreparseData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedUncompiledDataWithoutPreparseData, 
      DAlias>::value,
      "class TorqueGeneratedUncompiledDataWithoutPreparseData should be used as direct base for UncompiledDataWithoutPreparseData.");
  }
 protected:
  inline explicit TorqueGeneratedUncompiledDataWithoutPreparseData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedUncompiledDataWithoutPreparseData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsUncompiledDataWithPreparseData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsUncompiledDataWithPreparseData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedUncompiledDataWithPreparseData : public P {
  static_assert(std::is_same<UncompiledDataWithPreparseData, D>::value,
    "Use this class as direct base for UncompiledDataWithPreparseData.");
  static_assert(std::is_same<UncompiledData, P>::value,
    "Pass in UncompiledData as second template parameter for TorqueGeneratedUncompiledDataWithPreparseData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedUncompiledDataWithPreparseData<D,P>;

inline PreparseData preparse_data() const;
inline PreparseData preparse_data(PtrComprCageBase cage_base) const;
inline void set_preparse_data(PreparseData value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(UncompiledDataWithPreparseData)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void UncompiledDataWithPreparseDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kPreparseDataOffset = P::kHeaderSize;
  static constexpr int kPreparseDataOffsetEnd = kPreparseDataOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kPreparseDataOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kPreparseDataOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kPreparseDataOffsetEnd + 1;
  static constexpr int kHeaderSize = kPreparseDataOffsetEnd + 1;
  static constexpr int kSize = kPreparseDataOffsetEnd + 1;

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
  constexpr TorqueGeneratedUncompiledDataWithPreparseData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedUncompiledDataWithPreparseData, 
      DAlias>::value,
      "class TorqueGeneratedUncompiledDataWithPreparseData should be used as direct base for UncompiledDataWithPreparseData.");
  }
 protected:
  inline explicit TorqueGeneratedUncompiledDataWithPreparseData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedUncompiledDataWithPreparseData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsOnHeapBasicBlockProfilerData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsOnHeapBasicBlockProfilerData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedOnHeapBasicBlockProfilerData : public P {
  static_assert(std::is_same<OnHeapBasicBlockProfilerData, D>::value,
    "Use this class as direct base for OnHeapBasicBlockProfilerData.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedOnHeapBasicBlockProfilerData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedOnHeapBasicBlockProfilerData<D,P>;

inline ByteArray block_ids() const;
inline ByteArray block_ids(PtrComprCageBase cage_base) const;
inline void set_block_ids(ByteArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline ByteArray counts() const;
inline ByteArray counts(PtrComprCageBase cage_base) const;
inline void set_counts(ByteArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline String name() const;
inline String name(PtrComprCageBase cage_base) const;
inline void set_name(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline String schedule() const;
inline String schedule(PtrComprCageBase cage_base) const;
inline void set_schedule(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline String code() const;
inline String code(PtrComprCageBase cage_base) const;
inline void set_code(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int hash() const;
inline void set_hash(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(OnHeapBasicBlockProfilerData)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void OnHeapBasicBlockProfilerDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kBlockIdsOffset = P::kHeaderSize;
  static constexpr int kBlockIdsOffsetEnd = kBlockIdsOffset + kTaggedSize - 1;
  static constexpr int kCountsOffset = kBlockIdsOffsetEnd + 1;
  static constexpr int kCountsOffsetEnd = kCountsOffset + kTaggedSize - 1;
  static constexpr int kNameOffset = kCountsOffsetEnd + 1;
  static constexpr int kNameOffsetEnd = kNameOffset + kTaggedSize - 1;
  static constexpr int kScheduleOffset = kNameOffsetEnd + 1;
  static constexpr int kScheduleOffsetEnd = kScheduleOffset + kTaggedSize - 1;
  static constexpr int kCodeOffset = kScheduleOffsetEnd + 1;
  static constexpr int kCodeOffsetEnd = kCodeOffset + kTaggedSize - 1;
  static constexpr int kHashOffset = kCodeOffsetEnd + 1;
  static constexpr int kHashOffsetEnd = kHashOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kHashOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kHashOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kHashOffsetEnd + 1;
  static constexpr int kHeaderSize = kHashOffsetEnd + 1;
  static constexpr int kSize = kHashOffsetEnd + 1;

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
  constexpr TorqueGeneratedOnHeapBasicBlockProfilerData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedOnHeapBasicBlockProfilerData, 
      DAlias>::value,
      "class TorqueGeneratedOnHeapBasicBlockProfilerData should be used as direct base for OnHeapBasicBlockProfilerData.");
  }
 protected:
  inline explicit TorqueGeneratedOnHeapBasicBlockProfilerData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedOnHeapBasicBlockProfilerData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class OnHeapBasicBlockProfilerData : public TorqueGeneratedOnHeapBasicBlockProfilerData<OnHeapBasicBlockProfilerData, HeapObject> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(OnHeapBasicBlockProfilerData)
};

