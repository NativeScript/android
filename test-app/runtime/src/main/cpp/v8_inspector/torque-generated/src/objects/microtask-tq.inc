class Microtask;
class CallbackTask;
class CallableTask;
// Alias for HeapObject::IsMicrotask() that avoids inlining.
V8_EXPORT_PRIVATE bool IsMicrotask_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedMicrotask : public P {
  static_assert(std::is_same<Microtask, D>::value,
    "Use this class as direct base for Microtask.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedMicrotask.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedMicrotask<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void MicrotaskVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedMicrotask() : P() {
    static_assert(std::is_base_of<TorqueGeneratedMicrotask, 
      DAlias>::value,
      "class TorqueGeneratedMicrotask should be used as direct base for Microtask.");
  }
 protected:
  inline explicit TorqueGeneratedMicrotask(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedMicrotask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsCallbackTask() that avoids inlining.
V8_EXPORT_PRIVATE bool IsCallbackTask_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedCallbackTask : public P {
  static_assert(std::is_same<CallbackTask, D>::value,
    "Use this class as direct base for CallbackTask.");
  static_assert(std::is_same<Microtask, P>::value,
    "Pass in Microtask as second template parameter for TorqueGeneratedCallbackTask.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedCallbackTask<D,P>;

inline Foreign callback() const;
inline Foreign callback(PtrComprCageBase cage_base) const;
inline void set_callback(Foreign value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Foreign data() const;
inline Foreign data(PtrComprCageBase cage_base) const;
inline void set_data(Foreign value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void CallbackTaskVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kCallbackOffset = P::kHeaderSize;
  static constexpr int kCallbackOffsetEnd = kCallbackOffset + kTaggedSize - 1;
  static constexpr int kDataOffset = kCallbackOffsetEnd + 1;
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
  constexpr TorqueGeneratedCallbackTask() : P() {
    static_assert(std::is_base_of<TorqueGeneratedCallbackTask, 
      DAlias>::value,
      "class TorqueGeneratedCallbackTask should be used as direct base for CallbackTask.");
  }
 protected:
  inline explicit TorqueGeneratedCallbackTask(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedCallbackTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsCallableTask() that avoids inlining.
V8_EXPORT_PRIVATE bool IsCallableTask_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedCallableTask : public P {
  static_assert(std::is_same<CallableTask, D>::value,
    "Use this class as direct base for CallableTask.");
  static_assert(std::is_same<Microtask, P>::value,
    "Pass in Microtask as second template parameter for TorqueGeneratedCallableTask.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedCallableTask<D,P>;

inline JSReceiver callable() const;
inline JSReceiver callable(PtrComprCageBase cage_base) const;
inline void set_callable(JSReceiver value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Context context() const;
inline Context context(PtrComprCageBase cage_base) const;
inline void set_context(Context value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void CallableTaskVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kCallableOffset = P::kHeaderSize;
  static constexpr int kCallableOffsetEnd = kCallableOffset + kTaggedSize - 1;
  static constexpr int kContextOffset = kCallableOffsetEnd + 1;
  static constexpr int kContextOffsetEnd = kContextOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kContextOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kContextOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kContextOffsetEnd + 1;
  static constexpr int kHeaderSize = kContextOffsetEnd + 1;
  static constexpr int kSize = kContextOffsetEnd + 1;

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
  constexpr TorqueGeneratedCallableTask() : P() {
    static_assert(std::is_base_of<TorqueGeneratedCallableTask, 
      DAlias>::value,
      "class TorqueGeneratedCallableTask should be used as direct base for CallableTask.");
  }
 protected:
  inline explicit TorqueGeneratedCallableTask(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedCallableTask(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

