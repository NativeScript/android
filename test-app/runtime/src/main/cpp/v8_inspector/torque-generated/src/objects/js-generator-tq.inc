class JSGeneratorObject;
class JSAsyncFunctionObject;
class JSAsyncGeneratorObject;
class AsyncGeneratorRequest;
// Alias for HeapObject::IsJSGeneratorObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSGeneratorObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSGeneratorObject : public P {
  static_assert(std::is_same<JSGeneratorObject, D>::value,
    "Use this class as direct base for JSGeneratorObject.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSGeneratorObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSGeneratorObject<D,P>;

inline JSFunction function() const;
inline JSFunction function(PtrComprCageBase cage_base) const;
inline void set_function(JSFunction value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Context context() const;
inline Context context(PtrComprCageBase cage_base) const;
inline void set_context(Context value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: JSAny
inline Object receiver() const;
inline Object receiver(PtrComprCageBase cage_base) const;
inline void set_receiver(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object input_or_debug_pos() const;
inline Object input_or_debug_pos(PtrComprCageBase cage_base) const;
inline void set_input_or_debug_pos(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int resume_mode() const;
inline void set_resume_mode(int value);

inline int continuation() const;
inline void set_continuation(int value);

inline FixedArray parameters_and_registers() const;
inline FixedArray parameters_and_registers(PtrComprCageBase cage_base) const;
inline void set_parameters_and_registers(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSGeneratorObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kFunctionOffset = P::kHeaderSize;
  static constexpr int kFunctionOffsetEnd = kFunctionOffset + kTaggedSize - 1;
  static constexpr int kContextOffset = kFunctionOffsetEnd + 1;
  static constexpr int kContextOffsetEnd = kContextOffset + kTaggedSize - 1;
  static constexpr int kReceiverOffset = kContextOffsetEnd + 1;
  static constexpr int kReceiverOffsetEnd = kReceiverOffset + kTaggedSize - 1;
  static constexpr int kInputOrDebugPosOffset = kReceiverOffsetEnd + 1;
  static constexpr int kInputOrDebugPosOffsetEnd = kInputOrDebugPosOffset + kTaggedSize - 1;
  static constexpr int kResumeModeOffset = kInputOrDebugPosOffsetEnd + 1;
  static constexpr int kResumeModeOffsetEnd = kResumeModeOffset + kTaggedSize - 1;
  static constexpr int kContinuationOffset = kResumeModeOffsetEnd + 1;
  static constexpr int kContinuationOffsetEnd = kContinuationOffset + kTaggedSize - 1;
  static constexpr int kParametersAndRegistersOffset = kContinuationOffsetEnd + 1;
  static constexpr int kParametersAndRegistersOffsetEnd = kParametersAndRegistersOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kParametersAndRegistersOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kParametersAndRegistersOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kParametersAndRegistersOffsetEnd + 1;
  static constexpr int kHeaderSize = kParametersAndRegistersOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSGeneratorObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSGeneratorObject, 
      DAlias>::value,
      "class TorqueGeneratedJSGeneratorObject should be used as direct base for JSGeneratorObject.");
  }
 protected:
  inline explicit TorqueGeneratedJSGeneratorObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSGeneratorObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSAsyncFunctionObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSAsyncFunctionObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSAsyncFunctionObject : public P {
  static_assert(std::is_same<JSAsyncFunctionObject, D>::value,
    "Use this class as direct base for JSAsyncFunctionObject.");
  static_assert(std::is_same<JSGeneratorObject, P>::value,
    "Pass in JSGeneratorObject as second template parameter for TorqueGeneratedJSAsyncFunctionObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSAsyncFunctionObject<D,P>;

inline JSPromise promise() const;
inline JSPromise promise(PtrComprCageBase cage_base) const;
inline void set_promise(JSPromise value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSAsyncFunctionObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kPromiseOffset = P::kHeaderSize;
  static constexpr int kPromiseOffsetEnd = kPromiseOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kPromiseOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kPromiseOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kPromiseOffsetEnd + 1;
  static constexpr int kHeaderSize = kPromiseOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSAsyncFunctionObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSAsyncFunctionObject, 
      DAlias>::value,
      "class TorqueGeneratedJSAsyncFunctionObject should be used as direct base for JSAsyncFunctionObject.");
  }
 protected:
  inline explicit TorqueGeneratedJSAsyncFunctionObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSAsyncFunctionObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSAsyncGeneratorObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSAsyncGeneratorObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSAsyncGeneratorObject : public P {
  static_assert(std::is_same<JSAsyncGeneratorObject, D>::value,
    "Use this class as direct base for JSAsyncGeneratorObject.");
  static_assert(std::is_same<JSGeneratorObject, P>::value,
    "Pass in JSGeneratorObject as second template parameter for TorqueGeneratedJSAsyncGeneratorObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSAsyncGeneratorObject<D,P>;

inline HeapObject queue() const;
inline HeapObject queue(PtrComprCageBase cage_base) const;
inline void set_queue(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int is_awaiting() const;
inline void set_is_awaiting(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSAsyncGeneratorObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kQueueOffset = P::kHeaderSize;
  static constexpr int kQueueOffsetEnd = kQueueOffset + kTaggedSize - 1;
  static constexpr int kIsAwaitingOffset = kQueueOffsetEnd + 1;
  static constexpr int kIsAwaitingOffsetEnd = kIsAwaitingOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kIsAwaitingOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kIsAwaitingOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kIsAwaitingOffsetEnd + 1;
  static constexpr int kHeaderSize = kIsAwaitingOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSAsyncGeneratorObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSAsyncGeneratorObject, 
      DAlias>::value,
      "class TorqueGeneratedJSAsyncGeneratorObject should be used as direct base for JSAsyncGeneratorObject.");
  }
 protected:
  inline explicit TorqueGeneratedJSAsyncGeneratorObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSAsyncGeneratorObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsAsyncGeneratorRequest() that avoids inlining.
V8_EXPORT_PRIVATE bool IsAsyncGeneratorRequest_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedAsyncGeneratorRequest : public P {
  static_assert(std::is_same<AsyncGeneratorRequest, D>::value,
    "Use this class as direct base for AsyncGeneratorRequest.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedAsyncGeneratorRequest.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedAsyncGeneratorRequest<D,P>;

  // Torque type: (Undefined | class AsyncGeneratorRequest)
inline HeapObject next() const;
inline HeapObject next(PtrComprCageBase cage_base) const;
inline void set_next(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int resume_mode() const;
inline void set_resume_mode(int value);

inline Object value() const;
inline Object value(PtrComprCageBase cage_base) const;
inline void set_value(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline JSPromise promise() const;
inline JSPromise promise(PtrComprCageBase cage_base) const;
inline void set_promise(JSPromise value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void AsyncGeneratorRequestVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kNextOffset = P::kHeaderSize;
  static constexpr int kNextOffsetEnd = kNextOffset + kTaggedSize - 1;
  static constexpr int kResumeModeOffset = kNextOffsetEnd + 1;
  static constexpr int kResumeModeOffsetEnd = kResumeModeOffset + kTaggedSize - 1;
  static constexpr int kValueOffset = kResumeModeOffsetEnd + 1;
  static constexpr int kValueOffsetEnd = kValueOffset + kTaggedSize - 1;
  static constexpr int kPromiseOffset = kValueOffsetEnd + 1;
  static constexpr int kPromiseOffsetEnd = kPromiseOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kPromiseOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kPromiseOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kPromiseOffsetEnd + 1;
  static constexpr int kHeaderSize = kPromiseOffsetEnd + 1;
  static constexpr int kSize = kPromiseOffsetEnd + 1;

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
  constexpr TorqueGeneratedAsyncGeneratorRequest() : P() {
    static_assert(std::is_base_of<TorqueGeneratedAsyncGeneratorRequest, 
      DAlias>::value,
      "class TorqueGeneratedAsyncGeneratorRequest should be used as direct base for AsyncGeneratorRequest.");
  }
 protected:
  inline explicit TorqueGeneratedAsyncGeneratorRequest(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedAsyncGeneratorRequest(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

