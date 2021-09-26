class JSArrayBufferView;
class JSTypedArray;
class JSArrayBuffer;
class JSDataView;
class JSFunction;
class JSFunction;
class JSFunction;
class JSFunction;
class JSFunction;
class JSFunction;
class JSFunction;
class JSFunction;
class JSFunction;
class JSFunction;
class JSFunction;
class JSFunction;
// Alias for HeapObject::IsJSArrayBufferView() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSArrayBufferView_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSArrayBufferView : public P {
  static_assert(std::is_same<JSArrayBufferView, D>::value,
    "Use this class as direct base for JSArrayBufferView.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSArrayBufferView.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSArrayBufferView<D,P>;

inline JSArrayBuffer buffer() const;
inline JSArrayBuffer buffer(PtrComprCageBase cage_base) const;
inline void set_buffer(JSArrayBuffer value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline uintptr_t byte_offset() const;
inline void set_byte_offset(uintptr_t value);

inline uintptr_t byte_length() const;
inline void set_byte_length(uintptr_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSArrayBufferViewVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kBufferOffset = P::kHeaderSize;
  static constexpr int kBufferOffsetEnd = kBufferOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kBufferOffsetEnd + 1;
  static constexpr int kByteOffsetOffset = kBufferOffsetEnd + 1;
  static constexpr int kByteOffsetOffsetEnd = kByteOffsetOffset + kIntptrSize - 1;
  static constexpr int kByteLengthOffset = kByteOffsetOffsetEnd + 1;
  static constexpr int kByteLengthOffsetEnd = kByteLengthOffset + kIntptrSize - 1;
  static constexpr int kStartOfWeakFieldsOffset = kByteLengthOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kByteLengthOffsetEnd + 1;
  static constexpr int kHeaderSize = kByteLengthOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSArrayBufferView() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSArrayBufferView, 
      DAlias>::value,
      "class TorqueGeneratedJSArrayBufferView should be used as direct base for JSArrayBufferView.");
  }
 protected:
  inline explicit TorqueGeneratedJSArrayBufferView(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSArrayBufferView(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSTypedArray() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSTypedArray_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSTypedArray : public P {
  static_assert(std::is_same<JSTypedArray, D>::value,
    "Use this class as direct base for JSTypedArray.");
  static_assert(std::is_same<JSArrayBufferView, P>::value,
    "Pass in JSArrayBufferView as second template parameter for TorqueGeneratedJSTypedArray.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSTypedArray<D,P>;

inline uintptr_t length() const;
inline void set_length(uintptr_t value);

  // Torque type: (Smi | class ByteArray)
inline Object base_pointer() const;
inline Object base_pointer(PtrComprCageBase cage_base) const;
inline void set_base_pointer(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline uint32_t bit_field() const;
inline void set_bit_field(uint32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSTypedArrayVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kLengthOffset = P::kHeaderSize;
  static constexpr int kLengthOffsetEnd = kLengthOffset + kIntptrSize - 1;
  static constexpr int kExternalPointerOffset = kLengthOffsetEnd + 1;
  static constexpr int kExternalPointerOffsetEnd = kExternalPointerOffset + kExternalPointerSize - 1;
  static constexpr int kStartOfStrongFieldsOffset = kExternalPointerOffsetEnd + 1;
  static constexpr int kBasePointerOffset = kExternalPointerOffsetEnd + 1;
  static constexpr int kBasePointerOffsetEnd = kBasePointerOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kBasePointerOffsetEnd + 1;
  static constexpr int kBitFieldOffset = kBasePointerOffsetEnd + 1;
  static constexpr int kBitFieldOffsetEnd = kBitFieldOffset + kInt32Size - 1;
  static constexpr int kOptionalPaddingOffset = kBitFieldOffsetEnd + 1;
  static constexpr int kOptionalPaddingOffsetEnd = kOptionalPaddingOffset + 0 - 1;
  static constexpr int kStartOfWeakFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kHeaderSize = kOptionalPaddingOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSTypedArray() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSTypedArray, 
      DAlias>::value,
      "class TorqueGeneratedJSTypedArray should be used as direct base for JSTypedArray.");
  }
 protected:
  inline explicit TorqueGeneratedJSTypedArray(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSTypedArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSArrayBuffer() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSArrayBuffer_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSArrayBuffer : public P {
  static_assert(std::is_same<JSArrayBuffer, D>::value,
    "Use this class as direct base for JSArrayBuffer.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSArrayBuffer.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSArrayBuffer<D,P>;

inline uintptr_t byte_length() const;
inline void set_byte_length(uintptr_t value);

inline uintptr_t max_byte_length() const;
inline void set_max_byte_length(uintptr_t value);

inline Address extension() const;
inline void set_extension(Address value);

inline uint32_t bit_field() const;
inline void set_bit_field(uint32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSArrayBufferVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kByteLengthOffset = P::kHeaderSize;
  static constexpr int kByteLengthOffsetEnd = kByteLengthOffset + kIntptrSize - 1;
  static constexpr int kMaxByteLengthOffset = kByteLengthOffsetEnd + 1;
  static constexpr int kMaxByteLengthOffsetEnd = kMaxByteLengthOffset + kIntptrSize - 1;
  static constexpr int kBackingStoreOffset = kMaxByteLengthOffsetEnd + 1;
  static constexpr int kBackingStoreOffsetEnd = kBackingStoreOffset + kExternalPointerSize - 1;
  static constexpr int kExtensionOffset = kBackingStoreOffsetEnd + 1;
  static constexpr int kExtensionOffsetEnd = kExtensionOffset + kSystemPointerSize - 1;
  static constexpr int kBitFieldOffset = kExtensionOffsetEnd + 1;
  static constexpr int kBitFieldOffsetEnd = kBitFieldOffset + kInt32Size - 1;
  static constexpr int kOptionalPaddingOffset = kBitFieldOffsetEnd + 1;
  static constexpr int kOptionalPaddingOffsetEnd = kOptionalPaddingOffset + 0 - 1;
  static constexpr int kStartOfWeakFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kOptionalPaddingOffsetEnd + 1;
  static constexpr int kHeaderSize = kOptionalPaddingOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSArrayBuffer() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSArrayBuffer, 
      DAlias>::value,
      "class TorqueGeneratedJSArrayBuffer should be used as direct base for JSArrayBuffer.");
  }
 protected:
  inline explicit TorqueGeneratedJSArrayBuffer(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSArrayBuffer(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSDataView() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSDataView_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSDataView : public P {
  static_assert(std::is_same<JSDataView, D>::value,
    "Use this class as direct base for JSDataView.");
  static_assert(std::is_same<JSArrayBufferView, P>::value,
    "Pass in JSArrayBufferView as second template parameter for TorqueGeneratedJSDataView.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSDataView<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSDataViewVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kDataPointerOffset = P::kHeaderSize;
  static constexpr int kDataPointerOffsetEnd = kDataPointerOffset + kExternalPointerSize - 1;
  static constexpr int kStartOfWeakFieldsOffset = kDataPointerOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kDataPointerOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kDataPointerOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kDataPointerOffsetEnd + 1;
  static constexpr int kHeaderSize = kDataPointerOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSDataView() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSDataView, 
      DAlias>::value,
      "class TorqueGeneratedJSDataView should be used as direct base for JSDataView.");
  }
 protected:
  inline explicit TorqueGeneratedJSDataView(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSDataView(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

