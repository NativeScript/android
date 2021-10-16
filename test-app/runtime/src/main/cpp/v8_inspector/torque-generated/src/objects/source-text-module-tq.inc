class SourceTextModule;
class ModuleRequest;
class SourceTextModuleInfoEntry;
// Alias for HeapObject::IsSourceTextModule() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSourceTextModule_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSourceTextModule : public P {
  static_assert(std::is_same<SourceTextModule, D>::value,
    "Use this class as direct base for SourceTextModule.");
  static_assert(std::is_same<Module, P>::value,
    "Pass in Module as second template parameter for TorqueGeneratedSourceTextModule.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSourceTextModule<D,P>;

  // Torque type: (class JSFunction | class JSGeneratorObject | class SharedFunctionInfo)
inline HeapObject code() const;
inline HeapObject code(PtrComprCageBase cage_base) const;
inline void set_code(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray regular_exports() const;
inline FixedArray regular_exports(PtrComprCageBase cage_base) const;
inline void set_regular_exports(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray regular_imports() const;
inline FixedArray regular_imports(PtrComprCageBase cage_base) const;
inline void set_regular_imports(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray requested_modules() const;
inline FixedArray requested_modules(PtrComprCageBase cage_base) const;
inline void set_requested_modules(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class JSObject | TheHole)
inline HeapObject import_meta(AcquireLoadTag) const;
inline HeapObject import_meta(PtrComprCageBase cage_base, AcquireLoadTag) const;
inline void set_import_meta(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (TheHole | class SourceTextModule)
inline HeapObject cycle_root() const;
inline HeapObject cycle_root(PtrComprCageBase cage_base) const;
inline void set_cycle_root(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline ArrayList async_parent_modules() const;
inline ArrayList async_parent_modules(PtrComprCageBase cage_base) const;
inline void set_async_parent_modules(ArrayList value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int dfs_index() const;
inline void set_dfs_index(int value);

inline int dfs_ancestor_index() const;
inline void set_dfs_ancestor_index(int value);

inline int pending_async_dependencies() const;
inline void set_pending_async_dependencies(int value);

inline int flags() const;
inline void set_flags(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SourceTextModuleVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kCodeOffset = P::kHeaderSize;
  static constexpr int kCodeOffsetEnd = kCodeOffset + kTaggedSize - 1;
  static constexpr int kRegularExportsOffset = kCodeOffsetEnd + 1;
  static constexpr int kRegularExportsOffsetEnd = kRegularExportsOffset + kTaggedSize - 1;
  static constexpr int kRegularImportsOffset = kRegularExportsOffsetEnd + 1;
  static constexpr int kRegularImportsOffsetEnd = kRegularImportsOffset + kTaggedSize - 1;
  static constexpr int kRequestedModulesOffset = kRegularImportsOffsetEnd + 1;
  static constexpr int kRequestedModulesOffsetEnd = kRequestedModulesOffset + kTaggedSize - 1;
  static constexpr int kImportMetaOffset = kRequestedModulesOffsetEnd + 1;
  static constexpr int kImportMetaOffsetEnd = kImportMetaOffset + kTaggedSize - 1;
  static constexpr int kCycleRootOffset = kImportMetaOffsetEnd + 1;
  static constexpr int kCycleRootOffsetEnd = kCycleRootOffset + kTaggedSize - 1;
  static constexpr int kAsyncParentModulesOffset = kCycleRootOffsetEnd + 1;
  static constexpr int kAsyncParentModulesOffsetEnd = kAsyncParentModulesOffset + kTaggedSize - 1;
  static constexpr int kDfsIndexOffset = kAsyncParentModulesOffsetEnd + 1;
  static constexpr int kDfsIndexOffsetEnd = kDfsIndexOffset + kTaggedSize - 1;
  static constexpr int kDfsAncestorIndexOffset = kDfsIndexOffsetEnd + 1;
  static constexpr int kDfsAncestorIndexOffsetEnd = kDfsAncestorIndexOffset + kTaggedSize - 1;
  static constexpr int kPendingAsyncDependenciesOffset = kDfsAncestorIndexOffsetEnd + 1;
  static constexpr int kPendingAsyncDependenciesOffsetEnd = kPendingAsyncDependenciesOffset + kTaggedSize - 1;
  static constexpr int kFlagsOffset = kPendingAsyncDependenciesOffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kHeaderSize = kFlagsOffsetEnd + 1;
  static constexpr int kSize = kFlagsOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 72);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedSourceTextModule() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSourceTextModule, 
      DAlias>::value,
      "class TorqueGeneratedSourceTextModule should be used as direct base for SourceTextModule.");
  }
 protected:
  inline explicit TorqueGeneratedSourceTextModule(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSourceTextModule(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsModuleRequest() that avoids inlining.
V8_EXPORT_PRIVATE bool IsModuleRequest_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedModuleRequest : public P {
  static_assert(std::is_same<ModuleRequest, D>::value,
    "Use this class as direct base for ModuleRequest.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedModuleRequest.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedModuleRequest<D,P>;

inline String specifier() const;
inline String specifier(PtrComprCageBase cage_base) const;
inline void set_specifier(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray import_assertions() const;
inline FixedArray import_assertions(PtrComprCageBase cage_base) const;
inline void set_import_assertions(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int position() const;
inline void set_position(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(ModuleRequest)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void ModuleRequestVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kSpecifierOffset = P::kHeaderSize;
  static constexpr int kSpecifierOffsetEnd = kSpecifierOffset + kTaggedSize - 1;
  static constexpr int kImportAssertionsOffset = kSpecifierOffsetEnd + 1;
  static constexpr int kImportAssertionsOffsetEnd = kImportAssertionsOffset + kTaggedSize - 1;
  static constexpr int kPositionOffset = kImportAssertionsOffsetEnd + 1;
  static constexpr int kPositionOffsetEnd = kPositionOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kPositionOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kPositionOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kPositionOffsetEnd + 1;
  static constexpr int kHeaderSize = kPositionOffsetEnd + 1;
  static constexpr int kSize = kPositionOffsetEnd + 1;

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
  constexpr TorqueGeneratedModuleRequest() : P() {
    static_assert(std::is_base_of<TorqueGeneratedModuleRequest, 
      DAlias>::value,
      "class TorqueGeneratedModuleRequest should be used as direct base for ModuleRequest.");
  }
 protected:
  inline explicit TorqueGeneratedModuleRequest(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedModuleRequest(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsSourceTextModuleInfoEntry() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSourceTextModuleInfoEntry_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSourceTextModuleInfoEntry : public P {
  static_assert(std::is_same<SourceTextModuleInfoEntry, D>::value,
    "Use this class as direct base for SourceTextModuleInfoEntry.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedSourceTextModuleInfoEntry.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSourceTextModuleInfoEntry<D,P>;

  // Torque type: (class String | Undefined)
inline PrimitiveHeapObject export_name() const;
inline PrimitiveHeapObject export_name(PtrComprCageBase cage_base) const;
inline void set_export_name(PrimitiveHeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class String | Undefined)
inline PrimitiveHeapObject local_name() const;
inline PrimitiveHeapObject local_name(PtrComprCageBase cage_base) const;
inline void set_local_name(PrimitiveHeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class String | Undefined)
inline PrimitiveHeapObject import_name() const;
inline PrimitiveHeapObject import_name(PtrComprCageBase cage_base) const;
inline void set_import_name(PrimitiveHeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int module_request() const;
inline void set_module_request(int value);

inline int cell_index() const;
inline void set_cell_index(int value);

inline int beg_pos() const;
inline void set_beg_pos(int value);

inline int end_pos() const;
inline void set_end_pos(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SourceTextModuleInfoEntryVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kExportNameOffset = P::kHeaderSize;
  static constexpr int kExportNameOffsetEnd = kExportNameOffset + kTaggedSize - 1;
  static constexpr int kLocalNameOffset = kExportNameOffsetEnd + 1;
  static constexpr int kLocalNameOffsetEnd = kLocalNameOffset + kTaggedSize - 1;
  static constexpr int kImportNameOffset = kLocalNameOffsetEnd + 1;
  static constexpr int kImportNameOffsetEnd = kImportNameOffset + kTaggedSize - 1;
  static constexpr int kModuleRequestOffset = kImportNameOffsetEnd + 1;
  static constexpr int kModuleRequestOffsetEnd = kModuleRequestOffset + kTaggedSize - 1;
  static constexpr int kCellIndexOffset = kModuleRequestOffsetEnd + 1;
  static constexpr int kCellIndexOffsetEnd = kCellIndexOffset + kTaggedSize - 1;
  static constexpr int kBegPosOffset = kCellIndexOffsetEnd + 1;
  static constexpr int kBegPosOffsetEnd = kBegPosOffset + kTaggedSize - 1;
  static constexpr int kEndPosOffset = kBegPosOffsetEnd + 1;
  static constexpr int kEndPosOffsetEnd = kEndPosOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kEndPosOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kEndPosOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kEndPosOffsetEnd + 1;
  static constexpr int kHeaderSize = kEndPosOffsetEnd + 1;
  static constexpr int kSize = kEndPosOffsetEnd + 1;

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
  constexpr TorqueGeneratedSourceTextModuleInfoEntry() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSourceTextModuleInfoEntry, 
      DAlias>::value,
      "class TorqueGeneratedSourceTextModuleInfoEntry should be used as direct base for SourceTextModuleInfoEntry.");
  }
 protected:
  inline explicit TorqueGeneratedSourceTextModuleInfoEntry(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSourceTextModuleInfoEntry(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

