class PrototypeInfo;
// Alias for HeapObject::IsPrototypeInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsPrototypeInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedPrototypeInfo : public P {
  static_assert(std::is_same<PrototypeInfo, D>::value,
    "Use this class as direct base for PrototypeInfo.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedPrototypeInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedPrototypeInfo<D,P>;

  // Torque type: (Undefined | class JSModuleNamespace)
inline HeapObject module_namespace() const;
inline HeapObject module_namespace(PtrComprCageBase cage_base) const;
inline void set_module_namespace(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Zero | class WeakArrayList)
inline Object prototype_users() const;
inline Object prototype_users(PtrComprCageBase cage_base) const;
inline void set_prototype_users(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class FixedArray | Zero)
inline Object prototype_chain_enum_cache() const;
inline Object prototype_chain_enum_cache(PtrComprCageBase cage_base) const;
inline void set_prototype_chain_enum_cache(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int registry_slot() const;
inline void set_registry_slot(int value);

  // Torque type: (Undefined | Weak<class Map>)
inline MaybeObject object_create_map() const;
inline MaybeObject object_create_map(PtrComprCageBase cage_base) const;
inline void set_object_create_map(MaybeObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int bit_field() const;
inline void set_bit_field(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void PrototypeInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kModuleNamespaceOffset = P::kHeaderSize;
  static constexpr int kModuleNamespaceOffsetEnd = kModuleNamespaceOffset + kTaggedSize - 1;
  static constexpr int kPrototypeUsersOffset = kModuleNamespaceOffsetEnd + 1;
  static constexpr int kPrototypeUsersOffsetEnd = kPrototypeUsersOffset + kTaggedSize - 1;
  static constexpr int kPrototypeChainEnumCacheOffset = kPrototypeUsersOffsetEnd + 1;
  static constexpr int kPrototypeChainEnumCacheOffsetEnd = kPrototypeChainEnumCacheOffset + kTaggedSize - 1;
  static constexpr int kRegistrySlotOffset = kPrototypeChainEnumCacheOffsetEnd + 1;
  static constexpr int kRegistrySlotOffsetEnd = kRegistrySlotOffset + kTaggedSize - 1;
  static constexpr int kObjectCreateMapOffset = kRegistrySlotOffsetEnd + 1;
  static constexpr int kObjectCreateMapOffsetEnd = kObjectCreateMapOffset + kTaggedSize - 1;
  static constexpr int kBitFieldOffset = kObjectCreateMapOffsetEnd + 1;
  static constexpr int kBitFieldOffsetEnd = kBitFieldOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kBitFieldOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kBitFieldOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kBitFieldOffsetEnd + 1;
  static constexpr int kHeaderSize = kBitFieldOffsetEnd + 1;
  static constexpr int kSize = kBitFieldOffsetEnd + 1;

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
  constexpr TorqueGeneratedPrototypeInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedPrototypeInfo, 
      DAlias>::value,
      "class TorqueGeneratedPrototypeInfo should be used as direct base for PrototypeInfo.");
  }
 protected:
  inline explicit TorqueGeneratedPrototypeInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedPrototypeInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

