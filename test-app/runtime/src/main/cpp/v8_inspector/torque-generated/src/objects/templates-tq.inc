class TemplateInfo;
class FunctionTemplateRareData;
class FunctionTemplateInfo;
class ObjectTemplateInfo;
// Alias for HeapObject::IsTemplateInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsTemplateInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedTemplateInfo : public P {
  static_assert(std::is_same<TemplateInfo, D>::value,
    "Use this class as direct base for TemplateInfo.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedTemplateInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedTemplateInfo<D,P>;

inline int tag() const;
inline void set_tag(int value);

inline int serial_number() const;
inline void set_serial_number(int value);

inline int number_of_properties() const;
inline void set_number_of_properties(int value);

  // Torque type: (Undefined | class TemplateList)
inline HeapObject property_list() const;
inline HeapObject property_list(PtrComprCageBase cage_base) const;
inline void set_property_list(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class TemplateList)
inline HeapObject property_accessors() const;
inline HeapObject property_accessors(PtrComprCageBase cage_base) const;
inline void set_property_accessors(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void TemplateInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kTagOffset = P::kHeaderSize;
  static constexpr int kTagOffsetEnd = kTagOffset + kTaggedSize - 1;
  static constexpr int kSerialNumberOffset = kTagOffsetEnd + 1;
  static constexpr int kSerialNumberOffsetEnd = kSerialNumberOffset + kTaggedSize - 1;
  static constexpr int kNumberOfPropertiesOffset = kSerialNumberOffsetEnd + 1;
  static constexpr int kNumberOfPropertiesOffsetEnd = kNumberOfPropertiesOffset + kTaggedSize - 1;
  static constexpr int kPropertyListOffset = kNumberOfPropertiesOffsetEnd + 1;
  static constexpr int kPropertyListOffsetEnd = kPropertyListOffset + kTaggedSize - 1;
  static constexpr int kPropertyAccessorsOffset = kPropertyListOffsetEnd + 1;
  static constexpr int kPropertyAccessorsOffsetEnd = kPropertyAccessorsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kPropertyAccessorsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kPropertyAccessorsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kPropertyAccessorsOffsetEnd + 1;
  static constexpr int kHeaderSize = kPropertyAccessorsOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedTemplateInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedTemplateInfo, 
      DAlias>::value,
      "class TorqueGeneratedTemplateInfo should be used as direct base for TemplateInfo.");
  }
 protected:
  inline explicit TorqueGeneratedTemplateInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedTemplateInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsFunctionTemplateRareData() that avoids inlining.
V8_EXPORT_PRIVATE bool IsFunctionTemplateRareData_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedFunctionTemplateRareData : public P {
  static_assert(std::is_same<FunctionTemplateRareData, D>::value,
    "Use this class as direct base for FunctionTemplateRareData.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedFunctionTemplateRareData.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedFunctionTemplateRareData<D,P>;

  // Torque type: (Undefined | class ObjectTemplateInfo)
inline HeapObject prototype_template() const;
inline HeapObject prototype_template(PtrComprCageBase cage_base) const;
inline void set_prototype_template(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class FunctionTemplateInfo)
inline HeapObject prototype_provider_template() const;
inline HeapObject prototype_provider_template(PtrComprCageBase cage_base) const;
inline void set_prototype_provider_template(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class FunctionTemplateInfo)
inline HeapObject parent_template() const;
inline HeapObject parent_template(PtrComprCageBase cage_base) const;
inline void set_parent_template(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class InterceptorInfo)
inline HeapObject named_property_handler() const;
inline HeapObject named_property_handler(PtrComprCageBase cage_base) const;
inline void set_named_property_handler(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class InterceptorInfo)
inline HeapObject indexed_property_handler() const;
inline HeapObject indexed_property_handler(PtrComprCageBase cage_base) const;
inline void set_indexed_property_handler(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class ObjectTemplateInfo)
inline HeapObject instance_template() const;
inline HeapObject instance_template(PtrComprCageBase cage_base) const;
inline void set_instance_template(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class CallHandlerInfo)
inline HeapObject instance_call_handler() const;
inline HeapObject instance_call_handler(PtrComprCageBase cage_base) const;
inline void set_instance_call_handler(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class AccessCheckInfo)
inline HeapObject access_check_info() const;
inline HeapObject access_check_info(PtrComprCageBase cage_base) const;
inline void set_access_check_info(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray c_function_overloads() const;
inline FixedArray c_function_overloads(PtrComprCageBase cage_base) const;
inline void set_c_function_overloads(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(FunctionTemplateRareData)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void FunctionTemplateRareDataVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kPrototypeTemplateOffset = P::kHeaderSize;
  static constexpr int kPrototypeTemplateOffsetEnd = kPrototypeTemplateOffset + kTaggedSize - 1;
  static constexpr int kPrototypeProviderTemplateOffset = kPrototypeTemplateOffsetEnd + 1;
  static constexpr int kPrototypeProviderTemplateOffsetEnd = kPrototypeProviderTemplateOffset + kTaggedSize - 1;
  static constexpr int kParentTemplateOffset = kPrototypeProviderTemplateOffsetEnd + 1;
  static constexpr int kParentTemplateOffsetEnd = kParentTemplateOffset + kTaggedSize - 1;
  static constexpr int kNamedPropertyHandlerOffset = kParentTemplateOffsetEnd + 1;
  static constexpr int kNamedPropertyHandlerOffsetEnd = kNamedPropertyHandlerOffset + kTaggedSize - 1;
  static constexpr int kIndexedPropertyHandlerOffset = kNamedPropertyHandlerOffsetEnd + 1;
  static constexpr int kIndexedPropertyHandlerOffsetEnd = kIndexedPropertyHandlerOffset + kTaggedSize - 1;
  static constexpr int kInstanceTemplateOffset = kIndexedPropertyHandlerOffsetEnd + 1;
  static constexpr int kInstanceTemplateOffsetEnd = kInstanceTemplateOffset + kTaggedSize - 1;
  static constexpr int kInstanceCallHandlerOffset = kInstanceTemplateOffsetEnd + 1;
  static constexpr int kInstanceCallHandlerOffsetEnd = kInstanceCallHandlerOffset + kTaggedSize - 1;
  static constexpr int kAccessCheckInfoOffset = kInstanceCallHandlerOffsetEnd + 1;
  static constexpr int kAccessCheckInfoOffsetEnd = kAccessCheckInfoOffset + kTaggedSize - 1;
  static constexpr int kCFunctionOverloadsOffset = kAccessCheckInfoOffsetEnd + 1;
  static constexpr int kCFunctionOverloadsOffsetEnd = kCFunctionOverloadsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kCFunctionOverloadsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kCFunctionOverloadsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kCFunctionOverloadsOffsetEnd + 1;
  static constexpr int kHeaderSize = kCFunctionOverloadsOffsetEnd + 1;
  static constexpr int kSize = kCFunctionOverloadsOffsetEnd + 1;

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
  constexpr TorqueGeneratedFunctionTemplateRareData() : P() {
    static_assert(std::is_base_of<TorqueGeneratedFunctionTemplateRareData, 
      DAlias>::value,
      "class TorqueGeneratedFunctionTemplateRareData should be used as direct base for FunctionTemplateRareData.");
  }
 protected:
  inline explicit TorqueGeneratedFunctionTemplateRareData(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedFunctionTemplateRareData(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsFunctionTemplateInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsFunctionTemplateInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedFunctionTemplateInfo : public P {
  static_assert(std::is_same<FunctionTemplateInfo, D>::value,
    "Use this class as direct base for FunctionTemplateInfo.");
  static_assert(std::is_same<TemplateInfo, P>::value,
    "Pass in TemplateInfo as second template parameter for TorqueGeneratedFunctionTemplateInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedFunctionTemplateInfo<D,P>;

  // Torque type: (Undefined | class CallHandlerInfo)
inline HeapObject call_code(AcquireLoadTag) const;
inline HeapObject call_code(PtrComprCageBase cage_base, AcquireLoadTag) const;
inline void set_call_code(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class String | Undefined)
inline PrimitiveHeapObject class_name() const;
inline PrimitiveHeapObject class_name(PtrComprCageBase cage_base) const;
inline void set_class_name(PrimitiveHeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class FunctionTemplateInfo)
inline HeapObject signature() const;
inline HeapObject signature(PtrComprCageBase cage_base) const;
inline void set_signature(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class FunctionTemplateRareData)
inline HeapObject rare_data(AcquireLoadTag) const;
inline HeapObject rare_data(PtrComprCageBase cage_base, AcquireLoadTag) const;
inline void set_rare_data(HeapObject value, ReleaseStoreTag, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | class SharedFunctionInfo)
inline HeapObject shared_function_info() const;
inline HeapObject shared_function_info(PtrComprCageBase cage_base) const;
inline void set_shared_function_info(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int flag() const;
inline void set_flag(int value);

inline int length() const;
inline void set_length(int value);

inline Object cached_property_name() const;
inline Object cached_property_name(PtrComprCageBase cage_base) const;
inline void set_cached_property_name(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int instance_type() const;
inline void set_instance_type(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void FunctionTemplateInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kCallCodeOffset = P::kHeaderSize;
  static constexpr int kCallCodeOffsetEnd = kCallCodeOffset + kTaggedSize - 1;
  static constexpr int kClassNameOffset = kCallCodeOffsetEnd + 1;
  static constexpr int kClassNameOffsetEnd = kClassNameOffset + kTaggedSize - 1;
  static constexpr int kSignatureOffset = kClassNameOffsetEnd + 1;
  static constexpr int kSignatureOffsetEnd = kSignatureOffset + kTaggedSize - 1;
  static constexpr int kRareDataOffset = kSignatureOffsetEnd + 1;
  static constexpr int kRareDataOffsetEnd = kRareDataOffset + kTaggedSize - 1;
  static constexpr int kSharedFunctionInfoOffset = kRareDataOffsetEnd + 1;
  static constexpr int kSharedFunctionInfoOffsetEnd = kSharedFunctionInfoOffset + kTaggedSize - 1;
  static constexpr int kFlagOffset = kSharedFunctionInfoOffsetEnd + 1;
  static constexpr int kFlagOffsetEnd = kFlagOffset + kTaggedSize - 1;
  static constexpr int kLengthOffset = kFlagOffsetEnd + 1;
  static constexpr int kLengthOffsetEnd = kLengthOffset + kTaggedSize - 1;
  static constexpr int kCachedPropertyNameOffset = kLengthOffsetEnd + 1;
  static constexpr int kCachedPropertyNameOffsetEnd = kCachedPropertyNameOffset + kTaggedSize - 1;
  static constexpr int kInstanceTypeOffset = kCachedPropertyNameOffsetEnd + 1;
  static constexpr int kInstanceTypeOffsetEnd = kInstanceTypeOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kInstanceTypeOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kInstanceTypeOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kInstanceTypeOffsetEnd + 1;
  static constexpr int kHeaderSize = kInstanceTypeOffsetEnd + 1;
  static constexpr int kSize = kInstanceTypeOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 60);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedFunctionTemplateInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedFunctionTemplateInfo, 
      DAlias>::value,
      "class TorqueGeneratedFunctionTemplateInfo should be used as direct base for FunctionTemplateInfo.");
  }
 protected:
  inline explicit TorqueGeneratedFunctionTemplateInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedFunctionTemplateInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsObjectTemplateInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsObjectTemplateInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedObjectTemplateInfo : public P {
  static_assert(std::is_same<ObjectTemplateInfo, D>::value,
    "Use this class as direct base for ObjectTemplateInfo.");
  static_assert(std::is_same<TemplateInfo, P>::value,
    "Pass in TemplateInfo as second template parameter for TorqueGeneratedObjectTemplateInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedObjectTemplateInfo<D,P>;

  // Torque type: (Undefined | class FunctionTemplateInfo)
inline HeapObject constructor() const;
inline HeapObject constructor(PtrComprCageBase cage_base) const;
inline void set_constructor(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int data() const;
inline void set_data(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void ObjectTemplateInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kConstructorOffset = P::kHeaderSize;
  static constexpr int kConstructorOffsetEnd = kConstructorOffset + kTaggedSize - 1;
  static constexpr int kDataOffset = kConstructorOffsetEnd + 1;
  static constexpr int kDataOffsetEnd = kDataOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kHeaderSize = kDataOffsetEnd + 1;
  static constexpr int kSize = kDataOffsetEnd + 1;

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
  constexpr TorqueGeneratedObjectTemplateInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedObjectTemplateInfo, 
      DAlias>::value,
      "class TorqueGeneratedObjectTemplateInfo should be used as direct base for ObjectTemplateInfo.");
  }
 protected:
  inline explicit TorqueGeneratedObjectTemplateInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedObjectTemplateInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

