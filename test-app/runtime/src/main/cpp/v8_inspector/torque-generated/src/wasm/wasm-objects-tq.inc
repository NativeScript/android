class WasmInstanceObject;
class WasmFunctionData;
class WasmExportedFunctionData;
class WasmJSFunctionData;
class WasmCapiFunctionData;
class WasmIndirectFunctionTable;
class WasmExceptionTag;
class WasmModuleObject;
class WasmTableObject;
class WasmMemoryObject;
class WasmGlobalObject;
class WasmExceptionObject;
class AsmWasmData;
class WasmTypeInfo;
class WasmObject;
class WasmStruct;
class WasmArray;
// Alias for HeapObject::IsWasmFunctionData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmFunctionData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmFunctionData : public P {
  static_assert(std::is_same<WasmFunctionData, D>::value,
    "Use this class as direct base for WasmFunctionData.");
  static_assert(std::is_same<Foreign, P>::value,
    "Pass in Foreign as second template parameter for TorqueGeneratedWasmFunctionData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmFunctionData<D,P>;

  // Torque type: (class Tuple2 | class WasmInstanceObject)
inline HeapObject ref() const;
inline HeapObject ref(PtrComprCageBase cage_base) const;
inline void set_ref(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Code wrapper_code() const;
inline Code wrapper_code(PtrComprCageBase cage_base) const;
inline void set_wrapper_code(Code value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmFunctionDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kRefOffset = P::kHeaderSize;
  static constexpr int kRefOffsetEnd = kRefOffset + kTaggedSize - 1;
  static constexpr int kWrapperCodeOffset = kRefOffsetEnd + 1;
  static constexpr int kWrapperCodeOffsetEnd = kWrapperCodeOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kWrapperCodeOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kWrapperCodeOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kWrapperCodeOffsetEnd + 1;
  static constexpr int kHeaderSize = kWrapperCodeOffsetEnd + 1;
  static constexpr int kSize = kWrapperCodeOffsetEnd + 1;

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
  constexpr TorqueGeneratedWasmFunctionData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmFunctionData, 
      DAlias>::value,
      "class TorqueGeneratedWasmFunctionData should be used as direct base for WasmFunctionData.");
  }
 protected:
  inline explicit TorqueGeneratedWasmFunctionData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmFunctionData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmExportedFunctionData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmExportedFunctionData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmExportedFunctionData : public P {
  static_assert(std::is_same<WasmExportedFunctionData, D>::value,
    "Use this class as direct base for WasmExportedFunctionData.");
  static_assert(std::is_same<WasmFunctionData, P>::value,
    "Pass in WasmFunctionData as second template parameter for TorqueGeneratedWasmExportedFunctionData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmExportedFunctionData<D,P>;

inline WasmInstanceObject instance() const;
inline WasmInstanceObject instance(PtrComprCageBase cage_base) const;
inline void set_instance(WasmInstanceObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int function_index() const;
inline void set_function_index(int value);

inline Foreign signature() const;
inline Foreign signature(PtrComprCageBase cage_base) const;
inline void set_signature(Foreign value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int wrapper_budget() const;
inline void set_wrapper_budget(int value);

inline Code c_wrapper_code() const;
inline Code c_wrapper_code(PtrComprCageBase cage_base) const;
inline void set_c_wrapper_code(Code value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int packed_args_size() const;
inline void set_packed_args_size(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmExportedFunctionDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kInstanceOffset = P::kHeaderSize;
  static constexpr int kInstanceOffsetEnd = kInstanceOffset + kTaggedSize - 1;
  static constexpr int kFunctionIndexOffset = kInstanceOffsetEnd + 1;
  static constexpr int kFunctionIndexOffsetEnd = kFunctionIndexOffset + kTaggedSize - 1;
  static constexpr int kSignatureOffset = kFunctionIndexOffsetEnd + 1;
  static constexpr int kSignatureOffsetEnd = kSignatureOffset + kTaggedSize - 1;
  static constexpr int kWrapperBudgetOffset = kSignatureOffsetEnd + 1;
  static constexpr int kWrapperBudgetOffsetEnd = kWrapperBudgetOffset + kTaggedSize - 1;
  static constexpr int kCWrapperCodeOffset = kWrapperBudgetOffsetEnd + 1;
  static constexpr int kCWrapperCodeOffsetEnd = kCWrapperCodeOffset + kTaggedSize - 1;
  static constexpr int kPackedArgsSizeOffset = kCWrapperCodeOffsetEnd + 1;
  static constexpr int kPackedArgsSizeOffsetEnd = kPackedArgsSizeOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kPackedArgsSizeOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kPackedArgsSizeOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kPackedArgsSizeOffsetEnd + 1;
  static constexpr int kHeaderSize = kPackedArgsSizeOffsetEnd + 1;
  static constexpr int kSize = kPackedArgsSizeOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 44);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWasmExportedFunctionData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmExportedFunctionData, 
      DAlias>::value,
      "class TorqueGeneratedWasmExportedFunctionData should be used as direct base for WasmExportedFunctionData.");
  }
 protected:
  inline explicit TorqueGeneratedWasmExportedFunctionData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmExportedFunctionData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmJSFunctionData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmJSFunctionData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmJSFunctionData : public P {
  static_assert(std::is_same<WasmJSFunctionData, D>::value,
    "Use this class as direct base for WasmJSFunctionData.");
  static_assert(std::is_same<WasmFunctionData, P>::value,
    "Pass in WasmFunctionData as second template parameter for TorqueGeneratedWasmJSFunctionData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmJSFunctionData<D,P>;

inline Code wasm_to_js_wrapper_code() const;
inline Code wasm_to_js_wrapper_code(PtrComprCageBase cage_base) const;
inline void set_wasm_to_js_wrapper_code(Code value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int serialized_return_count() const;
inline void set_serialized_return_count(int value);

inline int serialized_parameter_count() const;
inline void set_serialized_parameter_count(int value);

  // Torque type: PodArrayOfWasmValueType
inline PodArray<wasm::ValueType> serialized_signature() const;
inline PodArray<wasm::ValueType> serialized_signature(PtrComprCageBase cage_base) const;
inline void set_serialized_signature(PodArray<wasm::ValueType> value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmJSFunctionDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kWasmToJsWrapperCodeOffset = P::kHeaderSize;
  static constexpr int kWasmToJsWrapperCodeOffsetEnd = kWasmToJsWrapperCodeOffset + kTaggedSize - 1;
  static constexpr int kSerializedReturnCountOffset = kWasmToJsWrapperCodeOffsetEnd + 1;
  static constexpr int kSerializedReturnCountOffsetEnd = kSerializedReturnCountOffset + kTaggedSize - 1;
  static constexpr int kSerializedParameterCountOffset = kSerializedReturnCountOffsetEnd + 1;
  static constexpr int kSerializedParameterCountOffsetEnd = kSerializedParameterCountOffset + kTaggedSize - 1;
  static constexpr int kSerializedSignatureOffset = kSerializedParameterCountOffsetEnd + 1;
  static constexpr int kSerializedSignatureOffsetEnd = kSerializedSignatureOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kSerializedSignatureOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kSerializedSignatureOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kSerializedSignatureOffsetEnd + 1;
  static constexpr int kHeaderSize = kSerializedSignatureOffsetEnd + 1;
  static constexpr int kSize = kSerializedSignatureOffsetEnd + 1;

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
  constexpr TorqueGeneratedWasmJSFunctionData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmJSFunctionData, 
      DAlias>::value,
      "class TorqueGeneratedWasmJSFunctionData should be used as direct base for WasmJSFunctionData.");
  }
 protected:
  inline explicit TorqueGeneratedWasmJSFunctionData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmJSFunctionData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmCapiFunctionData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmCapiFunctionData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmCapiFunctionData : public P {
  static_assert(std::is_same<WasmCapiFunctionData, D>::value,
    "Use this class as direct base for WasmCapiFunctionData.");
  static_assert(std::is_same<WasmFunctionData, P>::value,
    "Pass in WasmFunctionData as second template parameter for TorqueGeneratedWasmCapiFunctionData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmCapiFunctionData<D,P>;

inline Foreign embedder_data() const;
inline Foreign embedder_data(PtrComprCageBase cage_base) const;
inline void set_embedder_data(Foreign value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: PodArrayOfWasmValueType
inline PodArray<wasm::ValueType> serialized_signature() const;
inline PodArray<wasm::ValueType> serialized_signature(PtrComprCageBase cage_base) const;
inline void set_serialized_signature(PodArray<wasm::ValueType> value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmCapiFunctionDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEmbedderDataOffset = P::kHeaderSize;
  static constexpr int kEmbedderDataOffsetEnd = kEmbedderDataOffset + kTaggedSize - 1;
  static constexpr int kSerializedSignatureOffset = kEmbedderDataOffsetEnd + 1;
  static constexpr int kSerializedSignatureOffsetEnd = kSerializedSignatureOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kSerializedSignatureOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kSerializedSignatureOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kSerializedSignatureOffsetEnd + 1;
  static constexpr int kHeaderSize = kSerializedSignatureOffsetEnd + 1;
  static constexpr int kSize = kSerializedSignatureOffsetEnd + 1;

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
  constexpr TorqueGeneratedWasmCapiFunctionData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmCapiFunctionData, 
      DAlias>::value,
      "class TorqueGeneratedWasmCapiFunctionData should be used as direct base for WasmCapiFunctionData.");
  }
 protected:
  inline explicit TorqueGeneratedWasmCapiFunctionData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmCapiFunctionData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmIndirectFunctionTable() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmIndirectFunctionTable_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmIndirectFunctionTable : public P {
  static_assert(std::is_same<WasmIndirectFunctionTable, D>::value,
    "Use this class as direct base for WasmIndirectFunctionTable.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedWasmIndirectFunctionTable.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmIndirectFunctionTable<D,P>;

inline uint32_t size() const;
inline void set_size(uint32_t value);

inline Address sig_ids() const;
inline void set_sig_ids(Address value);

inline Address targets() const;
inline void set_targets(Address value);

  // Torque type: (Undefined | class Foreign)
inline HeapObject managed_native_allocations() const;
inline HeapObject managed_native_allocations(PtrComprCageBase cage_base) const;
inline void set_managed_native_allocations(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray refs() const;
inline FixedArray refs(PtrComprCageBase cage_base) const;
inline void set_refs(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmIndirectFunctionTableVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kSizeOffset = P::kHeaderSize;
  static constexpr int kSizeOffsetEnd = kSizeOffset + kInt32Size - 1;
  static constexpr int kOptionalPaddingOffset = kSizeOffsetEnd + 1;
  static constexpr int kOptionalPaddingOffsetEnd = kOptionalPaddingOffset + 0 - 1;
  static constexpr int kSigIdsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kSigIdsOffsetEnd = kSigIdsOffset + kSystemPointerSize - 1;
  static constexpr int kTargetsOffset = kSigIdsOffsetEnd + 1;
  static constexpr int kTargetsOffsetEnd = kTargetsOffset + kSystemPointerSize - 1;
  static constexpr int kStartOfStrongFieldsOffset = kTargetsOffsetEnd + 1;
  static constexpr int kManagedNativeAllocationsOffset = kTargetsOffsetEnd + 1;
  static constexpr int kManagedNativeAllocationsOffsetEnd = kManagedNativeAllocationsOffset + kTaggedSize - 1;
  static constexpr int kRefsOffset = kManagedNativeAllocationsOffsetEnd + 1;
  static constexpr int kRefsOffsetEnd = kRefsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kRefsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kRefsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kRefsOffsetEnd + 1;
  static constexpr int kHeaderSize = kRefsOffsetEnd + 1;
  static constexpr int kSize = kRefsOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 32);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWasmIndirectFunctionTable() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmIndirectFunctionTable, 
      DAlias>::value,
      "class TorqueGeneratedWasmIndirectFunctionTable should be used as direct base for WasmIndirectFunctionTable.");
  }
 protected:
  inline explicit TorqueGeneratedWasmIndirectFunctionTable(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmIndirectFunctionTable(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmExceptionTag() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmExceptionTag_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmExceptionTag : public P {
  static_assert(std::is_same<WasmExceptionTag, D>::value,
    "Use this class as direct base for WasmExceptionTag.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedWasmExceptionTag.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmExceptionTag<D,P>;

inline int index() const;
inline void set_index(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmExceptionTagVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kIndexOffset = P::kHeaderSize;
  static constexpr int kIndexOffsetEnd = kIndexOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kIndexOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kIndexOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kIndexOffsetEnd + 1;
  static constexpr int kHeaderSize = kIndexOffsetEnd + 1;
  static constexpr int kSize = kIndexOffsetEnd + 1;

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
  constexpr TorqueGeneratedWasmExceptionTag() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmExceptionTag, 
      DAlias>::value,
      "class TorqueGeneratedWasmExceptionTag should be used as direct base for WasmExceptionTag.");
  }
 protected:
  inline explicit TorqueGeneratedWasmExceptionTag(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmExceptionTag(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmModuleObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmModuleObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmModuleObject : public P {
  static_assert(std::is_same<WasmModuleObject, D>::value,
    "Use this class as direct base for WasmModuleObject.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedWasmModuleObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmModuleObject<D,P>;

  // Torque type: ManagedWasmNativeModule
inline Managed<wasm::NativeModule> managed_native_module() const;
inline Managed<wasm::NativeModule> managed_native_module(PtrComprCageBase cage_base) const;
inline void set_managed_native_module(Managed<wasm::NativeModule> value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray export_wrappers() const;
inline FixedArray export_wrappers(PtrComprCageBase cage_base) const;
inline void set_export_wrappers(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Script script() const;
inline Script script(PtrComprCageBase cage_base) const;
inline void set_script(Script value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmModuleObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kManagedNativeModuleOffset = P::kHeaderSize;
  static constexpr int kManagedNativeModuleOffsetEnd = kManagedNativeModuleOffset + kTaggedSize - 1;
  static constexpr int kExportWrappersOffset = kManagedNativeModuleOffsetEnd + 1;
  static constexpr int kExportWrappersOffsetEnd = kExportWrappersOffset + kTaggedSize - 1;
  static constexpr int kScriptOffset = kExportWrappersOffsetEnd + 1;
  static constexpr int kScriptOffsetEnd = kScriptOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kScriptOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kScriptOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kScriptOffsetEnd + 1;
  static constexpr int kHeaderSize = kScriptOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWasmModuleObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmModuleObject, 
      DAlias>::value,
      "class TorqueGeneratedWasmModuleObject should be used as direct base for WasmModuleObject.");
  }
 protected:
  inline explicit TorqueGeneratedWasmModuleObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmModuleObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmTableObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmTableObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmTableObject : public P {
  static_assert(std::is_same<WasmTableObject, D>::value,
    "Use this class as direct base for WasmTableObject.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedWasmTableObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmTableObject<D,P>;

  // Torque type: (Undefined | class WasmInstanceObject)
inline HeapObject instance() const;
inline HeapObject instance(PtrComprCageBase cage_base) const;
inline void set_instance(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray entries() const;
inline FixedArray entries(PtrComprCageBase cage_base) const;
inline void set_entries(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int current_length() const;
inline void set_current_length(int value);

  // Torque type: NumberOrUndefined
inline Object maximum_length() const;
inline Object maximum_length(PtrComprCageBase cage_base) const;
inline void set_maximum_length(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray dispatch_tables() const;
inline FixedArray dispatch_tables(PtrComprCageBase cage_base) const;
inline void set_dispatch_tables(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int raw_type() const;
inline void set_raw_type(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmTableObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kInstanceOffset = P::kHeaderSize;
  static constexpr int kInstanceOffsetEnd = kInstanceOffset + kTaggedSize - 1;
  static constexpr int kEntriesOffset = kInstanceOffsetEnd + 1;
  static constexpr int kEntriesOffsetEnd = kEntriesOffset + kTaggedSize - 1;
  static constexpr int kCurrentLengthOffset = kEntriesOffsetEnd + 1;
  static constexpr int kCurrentLengthOffsetEnd = kCurrentLengthOffset + kTaggedSize - 1;
  static constexpr int kMaximumLengthOffset = kCurrentLengthOffsetEnd + 1;
  static constexpr int kMaximumLengthOffsetEnd = kMaximumLengthOffset + kTaggedSize - 1;
  static constexpr int kDispatchTablesOffset = kMaximumLengthOffsetEnd + 1;
  static constexpr int kDispatchTablesOffsetEnd = kDispatchTablesOffset + kTaggedSize - 1;
  static constexpr int kRawTypeOffset = kDispatchTablesOffsetEnd + 1;
  static constexpr int kRawTypeOffsetEnd = kRawTypeOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kRawTypeOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kRawTypeOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kRawTypeOffsetEnd + 1;
  static constexpr int kHeaderSize = kRawTypeOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWasmTableObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmTableObject, 
      DAlias>::value,
      "class TorqueGeneratedWasmTableObject should be used as direct base for WasmTableObject.");
  }
 protected:
  inline explicit TorqueGeneratedWasmTableObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmTableObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmMemoryObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmMemoryObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmMemoryObject : public P {
  static_assert(std::is_same<WasmMemoryObject, D>::value,
    "Use this class as direct base for WasmMemoryObject.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedWasmMemoryObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmMemoryObject<D,P>;

inline JSArrayBuffer array_buffer() const;
inline JSArrayBuffer array_buffer(PtrComprCageBase cage_base) const;
inline void set_array_buffer(JSArrayBuffer value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int maximum_pages() const;
inline void set_maximum_pages(int value);

  // Torque type: (Undefined | class WeakArrayList)
inline HeapObject instances() const;
inline HeapObject instances(PtrComprCageBase cage_base) const;
inline void set_instances(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmMemoryObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kArrayBufferOffset = P::kHeaderSize;
  static constexpr int kArrayBufferOffsetEnd = kArrayBufferOffset + kTaggedSize - 1;
  static constexpr int kMaximumPagesOffset = kArrayBufferOffsetEnd + 1;
  static constexpr int kMaximumPagesOffsetEnd = kMaximumPagesOffset + kTaggedSize - 1;
  static constexpr int kInstancesOffset = kMaximumPagesOffsetEnd + 1;
  static constexpr int kInstancesOffsetEnd = kInstancesOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kInstancesOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kInstancesOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kInstancesOffsetEnd + 1;
  static constexpr int kHeaderSize = kInstancesOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWasmMemoryObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmMemoryObject, 
      DAlias>::value,
      "class TorqueGeneratedWasmMemoryObject should be used as direct base for WasmMemoryObject.");
  }
 protected:
  inline explicit TorqueGeneratedWasmMemoryObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmMemoryObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmGlobalObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmGlobalObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmGlobalObject : public P {
  static_assert(std::is_same<WasmGlobalObject, D>::value,
    "Use this class as direct base for WasmGlobalObject.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedWasmGlobalObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmGlobalObject<D,P>;

  // Torque type: (Undefined | class WasmInstanceObject)
inline HeapObject instance() const;
inline HeapObject instance(PtrComprCageBase cage_base) const;
inline void set_instance(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class JSArrayBuffer)
inline HeapObject untagged_buffer() const;
inline HeapObject untagged_buffer(PtrComprCageBase cage_base) const;
inline void set_untagged_buffer(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class FixedArray)
inline HeapObject tagged_buffer() const;
inline HeapObject tagged_buffer(PtrComprCageBase cage_base) const;
inline void set_tagged_buffer(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int offset() const;
inline void set_offset(int value);

inline int raw_type() const;
inline void set_raw_type(int value);

inline int is_mutable() const;
inline void set_is_mutable(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmGlobalObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kInstanceOffset = P::kHeaderSize;
  static constexpr int kInstanceOffsetEnd = kInstanceOffset + kTaggedSize - 1;
  static constexpr int kUntaggedBufferOffset = kInstanceOffsetEnd + 1;
  static constexpr int kUntaggedBufferOffsetEnd = kUntaggedBufferOffset + kTaggedSize - 1;
  static constexpr int kTaggedBufferOffset = kUntaggedBufferOffsetEnd + 1;
  static constexpr int kTaggedBufferOffsetEnd = kTaggedBufferOffset + kTaggedSize - 1;
  static constexpr int kOffsetOffset = kTaggedBufferOffsetEnd + 1;
  static constexpr int kOffsetOffsetEnd = kOffsetOffset + kTaggedSize - 1;
  static constexpr int kRawTypeOffset = kOffsetOffsetEnd + 1;
  static constexpr int kRawTypeOffsetEnd = kRawTypeOffset + kTaggedSize - 1;
  static constexpr int kIsMutableOffset = kRawTypeOffsetEnd + 1;
  static constexpr int kIsMutableOffsetEnd = kIsMutableOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kIsMutableOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kIsMutableOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kIsMutableOffsetEnd + 1;
  static constexpr int kHeaderSize = kIsMutableOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWasmGlobalObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmGlobalObject, 
      DAlias>::value,
      "class TorqueGeneratedWasmGlobalObject should be used as direct base for WasmGlobalObject.");
  }
 protected:
  inline explicit TorqueGeneratedWasmGlobalObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmGlobalObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmExceptionObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmExceptionObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmExceptionObject : public P {
  static_assert(std::is_same<WasmExceptionObject, D>::value,
    "Use this class as direct base for WasmExceptionObject.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedWasmExceptionObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmExceptionObject<D,P>;

  // Torque type: PodArrayOfWasmValueType
inline PodArray<wasm::ValueType> serialized_signature() const;
inline PodArray<wasm::ValueType> serialized_signature(PtrComprCageBase cage_base) const;
inline void set_serialized_signature(PodArray<wasm::ValueType> value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline HeapObject exception_tag() const;
inline HeapObject exception_tag(PtrComprCageBase cage_base) const;
inline void set_exception_tag(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmExceptionObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kSerializedSignatureOffset = P::kHeaderSize;
  static constexpr int kSerializedSignatureOffsetEnd = kSerializedSignatureOffset + kTaggedSize - 1;
  static constexpr int kExceptionTagOffset = kSerializedSignatureOffsetEnd + 1;
  static constexpr int kExceptionTagOffsetEnd = kExceptionTagOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kExceptionTagOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kExceptionTagOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kExceptionTagOffsetEnd + 1;
  static constexpr int kHeaderSize = kExceptionTagOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWasmExceptionObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmExceptionObject, 
      DAlias>::value,
      "class TorqueGeneratedWasmExceptionObject should be used as direct base for WasmExceptionObject.");
  }
 protected:
  inline explicit TorqueGeneratedWasmExceptionObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmExceptionObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsAsmWasmData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsAsmWasmData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedAsmWasmData : public P {
  static_assert(std::is_same<AsmWasmData, D>::value,
    "Use this class as direct base for AsmWasmData.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedAsmWasmData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedAsmWasmData<D,P>;

  // Torque type: ManagedWasmNativeModule
inline Managed<wasm::NativeModule> managed_native_module() const;
inline Managed<wasm::NativeModule> managed_native_module(PtrComprCageBase cage_base) const;
inline void set_managed_native_module(Managed<wasm::NativeModule> value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray export_wrappers() const;
inline FixedArray export_wrappers(PtrComprCageBase cage_base) const;
inline void set_export_wrappers(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline HeapNumber uses_bitset() const;
inline HeapNumber uses_bitset(PtrComprCageBase cage_base) const;
inline void set_uses_bitset(HeapNumber value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void AsmWasmDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kManagedNativeModuleOffset = P::kHeaderSize;
  static constexpr int kManagedNativeModuleOffsetEnd = kManagedNativeModuleOffset + kTaggedSize - 1;
  static constexpr int kExportWrappersOffset = kManagedNativeModuleOffsetEnd + 1;
  static constexpr int kExportWrappersOffsetEnd = kExportWrappersOffset + kTaggedSize - 1;
  static constexpr int kUsesBitsetOffset = kExportWrappersOffsetEnd + 1;
  static constexpr int kUsesBitsetOffsetEnd = kUsesBitsetOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kUsesBitsetOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kUsesBitsetOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kUsesBitsetOffsetEnd + 1;
  static constexpr int kHeaderSize = kUsesBitsetOffsetEnd + 1;
  static constexpr int kSize = kUsesBitsetOffsetEnd + 1;

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
  constexpr TorqueGeneratedAsmWasmData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedAsmWasmData, 
      DAlias>::value,
      "class TorqueGeneratedAsmWasmData should be used as direct base for AsmWasmData.");
  }
 protected:
  inline explicit TorqueGeneratedAsmWasmData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedAsmWasmData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmTypeInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmTypeInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmTypeInfo : public P {
  static_assert(std::is_same<WasmTypeInfo, D>::value,
    "Use this class as direct base for WasmTypeInfo.");
  static_assert(std::is_same<Foreign, P>::value,
    "Pass in Foreign as second template parameter for TorqueGeneratedWasmTypeInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmTypeInfo<D,P>;

inline FixedArray supertypes() const;
inline FixedArray supertypes(PtrComprCageBase cage_base) const;
inline void set_supertypes(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline ArrayList subtypes() const;
inline ArrayList subtypes(PtrComprCageBase cage_base) const;
inline void set_subtypes(ArrayList value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int instance_size() const;
inline void set_instance_size(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmTypeInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kSupertypesOffset = P::kHeaderSize;
  static constexpr int kSupertypesOffsetEnd = kSupertypesOffset + kTaggedSize - 1;
  static constexpr int kSubtypesOffset = kSupertypesOffsetEnd + 1;
  static constexpr int kSubtypesOffsetEnd = kSubtypesOffset + kTaggedSize - 1;
  static constexpr int kInstanceSizeOffset = kSubtypesOffsetEnd + 1;
  static constexpr int kInstanceSizeOffsetEnd = kInstanceSizeOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kInstanceSizeOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kInstanceSizeOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kInstanceSizeOffsetEnd + 1;
  static constexpr int kHeaderSize = kInstanceSizeOffsetEnd + 1;
  static constexpr int kSize = kInstanceSizeOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 24);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWasmTypeInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmTypeInfo, 
      DAlias>::value,
      "class TorqueGeneratedWasmTypeInfo should be used as direct base for WasmTypeInfo.");
  }
 protected:
  inline explicit TorqueGeneratedWasmTypeInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmTypeInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmObject : public P {
  static_assert(std::is_same<WasmObject, D>::value,
    "Use this class as direct base for WasmObject.");
  static_assert(std::is_same<JSReceiver, P>::value,
    "Pass in JSReceiver as second template parameter for TorqueGeneratedWasmObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmObject<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedWasmObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmObject, 
      DAlias>::value,
      "class TorqueGeneratedWasmObject should be used as direct base for WasmObject.");
  }
 protected:
  inline explicit TorqueGeneratedWasmObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmStruct() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmStruct_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmStruct : public P {
  static_assert(std::is_same<WasmStruct, D>::value,
    "Use this class as direct base for WasmStruct.");
  static_assert(std::is_same<WasmObject, P>::value,
    "Pass in WasmObject as second template parameter for TorqueGeneratedWasmStruct.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmStruct<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmStructVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;
  static constexpr int kSize = P::kHeaderSize;

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
  constexpr TorqueGeneratedWasmStruct() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmStruct, 
      DAlias>::value,
      "class TorqueGeneratedWasmStruct should be used as direct base for WasmStruct.");
  }
 protected:
  inline explicit TorqueGeneratedWasmStruct(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmStruct(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsWasmArray() that avoids inlining.
V8_EXPORT_PRIVATE bool IsWasmArray_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedWasmArray : public P {
  static_assert(std::is_same<WasmArray, D>::value,
    "Use this class as direct base for WasmArray.");
  static_assert(std::is_same<WasmObject, P>::value,
    "Pass in WasmObject as second template parameter for TorqueGeneratedWasmArray.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedWasmArray<D,P>;

inline uint32_t length() const;
inline void set_length(uint32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void WasmArrayVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kLengthOffset = P::kHeaderSize;
  static constexpr int kLengthOffsetEnd = kLengthOffset + kInt32Size - 1;
  static constexpr int kOptionalPaddingOffset = kLengthOffsetEnd + 1;
  static constexpr int kOptionalPaddingOffsetEnd = kOptionalPaddingOffset + 0 - 1;
  static constexpr int kStartOfWeakFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kHeaderSize = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kSize = kOptionalPaddingOffsetEnd + 1;

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
  constexpr TorqueGeneratedWasmArray() : P() {
    static_assert(std::is_base_of<TorqueGeneratedWasmArray, 
      DAlias>::value,
      "class TorqueGeneratedWasmArray should be used as direct base for WasmArray.");
  }
 protected:
  inline explicit TorqueGeneratedWasmArray(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedWasmArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

