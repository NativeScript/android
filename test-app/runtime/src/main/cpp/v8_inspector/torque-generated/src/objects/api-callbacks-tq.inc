class CallHandlerInfo;
class InterceptorInfo;
class AccessCheckInfo;
class AccessorInfo;
// Alias for HeapObject::IsCallHandlerInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsCallHandlerInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedCallHandlerInfo : public P {
  static_assert(std::is_same<CallHandlerInfo, D>::value,
    "Use this class as direct base for CallHandlerInfo.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedCallHandlerInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedCallHandlerInfo<D,P>;

  // Torque type: (Undefined | Zero | NonNullForeign)
inline Object callback() const;
inline Object callback(PtrComprCageBase cage_base) const;
inline void set_callback(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | Zero | NonNullForeign)
inline Object js_callback() const;
inline Object js_callback(PtrComprCageBase cage_base) const;
inline void set_js_callback(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object data() const;
inline Object data(PtrComprCageBase cage_base) const;
inline void set_data(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void CallHandlerInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kCallbackOffset = P::kHeaderSize;
  static constexpr int kCallbackOffsetEnd = kCallbackOffset + kTaggedSize - 1;
  static constexpr int kJsCallbackOffset = kCallbackOffsetEnd + 1;
  static constexpr int kJsCallbackOffsetEnd = kJsCallbackOffset + kTaggedSize - 1;
  static constexpr int kDataOffset = kJsCallbackOffsetEnd + 1;
  static constexpr int kDataOffsetEnd = kDataOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kHeaderSize = kDataOffsetEnd + 1;
  static constexpr int kSize = kDataOffsetEnd + 1;

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
  constexpr TorqueGeneratedCallHandlerInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedCallHandlerInfo, 
      DAlias>::value,
      "class TorqueGeneratedCallHandlerInfo should be used as direct base for CallHandlerInfo.");
  }
 protected:
  inline explicit TorqueGeneratedCallHandlerInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedCallHandlerInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsInterceptorInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsInterceptorInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedInterceptorInfo : public P {
  static_assert(std::is_same<InterceptorInfo, D>::value,
    "Use this class as direct base for InterceptorInfo.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedInterceptorInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedInterceptorInfo<D,P>;

  // Torque type: (Undefined | Zero | NonNullForeign)
inline Object getter() const;
inline Object getter(PtrComprCageBase cage_base) const;
inline void set_getter(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | Zero | NonNullForeign)
inline Object setter() const;
inline Object setter(PtrComprCageBase cage_base) const;
inline void set_setter(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | Zero | NonNullForeign)
inline Object query() const;
inline Object query(PtrComprCageBase cage_base) const;
inline void set_query(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | Zero | NonNullForeign)
inline Object descriptor() const;
inline Object descriptor(PtrComprCageBase cage_base) const;
inline void set_descriptor(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | Zero | NonNullForeign)
inline Object deleter() const;
inline Object deleter(PtrComprCageBase cage_base) const;
inline void set_deleter(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | Zero | NonNullForeign)
inline Object enumerator() const;
inline Object enumerator(PtrComprCageBase cage_base) const;
inline void set_enumerator(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | Zero | NonNullForeign)
inline Object definer() const;
inline Object definer(PtrComprCageBase cage_base) const;
inline void set_definer(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object data() const;
inline Object data(PtrComprCageBase cage_base) const;
inline void set_data(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int flags() const;
inline void set_flags(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(InterceptorInfo)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void InterceptorInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kGetterOffset = P::kHeaderSize;
  static constexpr int kGetterOffsetEnd = kGetterOffset + kTaggedSize - 1;
  static constexpr int kSetterOffset = kGetterOffsetEnd + 1;
  static constexpr int kSetterOffsetEnd = kSetterOffset + kTaggedSize - 1;
  static constexpr int kQueryOffset = kSetterOffsetEnd + 1;
  static constexpr int kQueryOffsetEnd = kQueryOffset + kTaggedSize - 1;
  static constexpr int kDescriptorOffset = kQueryOffsetEnd + 1;
  static constexpr int kDescriptorOffsetEnd = kDescriptorOffset + kTaggedSize - 1;
  static constexpr int kDeleterOffset = kDescriptorOffsetEnd + 1;
  static constexpr int kDeleterOffsetEnd = kDeleterOffset + kTaggedSize - 1;
  static constexpr int kEnumeratorOffset = kDeleterOffsetEnd + 1;
  static constexpr int kEnumeratorOffsetEnd = kEnumeratorOffset + kTaggedSize - 1;
  static constexpr int kDefinerOffset = kEnumeratorOffsetEnd + 1;
  static constexpr int kDefinerOffsetEnd = kDefinerOffset + kTaggedSize - 1;
  static constexpr int kDataOffset = kDefinerOffsetEnd + 1;
  static constexpr int kDataOffsetEnd = kDataOffset + kTaggedSize - 1;
  static constexpr int kFlagsOffset = kDataOffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kFlagsOffsetEnd + 1;
  static constexpr int kHeaderSize = kFlagsOffsetEnd + 1;
  static constexpr int kSize = kFlagsOffsetEnd + 1;

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
  constexpr TorqueGeneratedInterceptorInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedInterceptorInfo, 
      DAlias>::value,
      "class TorqueGeneratedInterceptorInfo should be used as direct base for InterceptorInfo.");
  }
 protected:
  inline explicit TorqueGeneratedInterceptorInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedInterceptorInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsAccessCheckInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsAccessCheckInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedAccessCheckInfo : public P {
  static_assert(std::is_same<AccessCheckInfo, D>::value,
    "Use this class as direct base for AccessCheckInfo.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedAccessCheckInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedAccessCheckInfo<D,P>;

  // Torque type: (Undefined | Zero | class Foreign)
inline Object callback() const;
inline Object callback(PtrComprCageBase cage_base) const;
inline void set_callback(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | Zero | class InterceptorInfo)
inline Object named_interceptor() const;
inline Object named_interceptor(PtrComprCageBase cage_base) const;
inline void set_named_interceptor(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | Zero | class InterceptorInfo)
inline Object indexed_interceptor() const;
inline Object indexed_interceptor(PtrComprCageBase cage_base) const;
inline void set_indexed_interceptor(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object data() const;
inline Object data(PtrComprCageBase cage_base) const;
inline void set_data(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(AccessCheckInfo)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void AccessCheckInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kCallbackOffset = P::kHeaderSize;
  static constexpr int kCallbackOffsetEnd = kCallbackOffset + kTaggedSize - 1;
  static constexpr int kNamedInterceptorOffset = kCallbackOffsetEnd + 1;
  static constexpr int kNamedInterceptorOffsetEnd = kNamedInterceptorOffset + kTaggedSize - 1;
  static constexpr int kIndexedInterceptorOffset = kNamedInterceptorOffsetEnd + 1;
  static constexpr int kIndexedInterceptorOffsetEnd = kIndexedInterceptorOffset + kTaggedSize - 1;
  static constexpr int kDataOffset = kIndexedInterceptorOffsetEnd + 1;
  static constexpr int kDataOffsetEnd = kDataOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kDataOffsetEnd + 1;
  static constexpr int kHeaderSize = kDataOffsetEnd + 1;
  static constexpr int kSize = kDataOffsetEnd + 1;

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
  constexpr TorqueGeneratedAccessCheckInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedAccessCheckInfo, 
      DAlias>::value,
      "class TorqueGeneratedAccessCheckInfo should be used as direct base for AccessCheckInfo.");
  }
 protected:
  inline explicit TorqueGeneratedAccessCheckInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedAccessCheckInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsAccessorInfo() that avoids inlining.
V8_EXPORT_PRIVATE bool IsAccessorInfo_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedAccessorInfo : public P {
  static_assert(std::is_same<AccessorInfo, D>::value,
    "Use this class as direct base for AccessorInfo.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedAccessorInfo.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedAccessorInfo<D,P>;

inline Name name() const;
inline Name name(PtrComprCageBase cage_base) const;
inline void set_name(Name value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int flags() const;
inline void set_flags(int value);

inline Object expected_receiver_type() const;
inline Object expected_receiver_type(PtrComprCageBase cage_base) const;
inline void set_expected_receiver_type(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Zero | NonNullForeign)
inline Object setter() const;
inline Object setter(PtrComprCageBase cage_base) const;
inline void set_setter(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Zero | NonNullForeign)
inline Object getter() const;
inline Object getter(PtrComprCageBase cage_base) const;
inline void set_getter(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Zero | NonNullForeign)
inline Object js_getter() const;
inline Object js_getter(PtrComprCageBase cage_base) const;
inline void set_js_getter(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object data() const;
inline Object data(PtrComprCageBase cage_base) const;
inline void set_data(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(AccessorInfo)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void AccessorInfoVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kNameOffset = P::kHeaderSize;
  static constexpr int kNameOffsetEnd = kNameOffset + kTaggedSize - 1;
  static constexpr int kFlagsOffset = kNameOffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kTaggedSize - 1;
  static constexpr int kExpectedReceiverTypeOffset = kFlagsOffsetEnd + 1;
  static constexpr int kExpectedReceiverTypeOffsetEnd = kExpectedReceiverTypeOffset + kTaggedSize - 1;
  static constexpr int kSetterOffset = kExpectedReceiverTypeOffsetEnd + 1;
  static constexpr int kSetterOffsetEnd = kSetterOffset + kTaggedSize - 1;
  static constexpr int kGetterOffset = kSetterOffsetEnd + 1;
  static constexpr int kGetterOffsetEnd = kGetterOffset + kTaggedSize - 1;
  static constexpr int kJsGetterOffset = kGetterOffsetEnd + 1;
  static constexpr int kJsGetterOffsetEnd = kJsGetterOffset + kTaggedSize - 1;
  static constexpr int kDataOffset = kJsGetterOffsetEnd + 1;
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
  constexpr TorqueGeneratedAccessorInfo() : P() {
    static_assert(std::is_base_of<TorqueGeneratedAccessorInfo, 
      DAlias>::value,
      "class TorqueGeneratedAccessorInfo should be used as direct base for AccessorInfo.");
  }
 protected:
  inline explicit TorqueGeneratedAccessorInfo(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedAccessorInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

