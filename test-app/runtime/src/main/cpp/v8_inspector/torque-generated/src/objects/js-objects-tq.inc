class JSReceiver;
class JSObject;
class JSCustomElementsObject;
class JSSpecialObject;
class JSGlobalProxy;
class JSGlobalObject;
class JSPrimitiveWrapper;
class JSMessageObject;
class JSDate;
class JSAsyncFromSyncIterator;
class JSStringIterator;
// Alias for HeapObject::IsJSObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSObject : public P {
  static_assert(std::is_same<JSObject, D>::value,
    "Use this class as direct base for JSObject.");
  static_assert(std::is_same<JSReceiver, P>::value,
    "Pass in JSReceiver as second template parameter for TorqueGeneratedJSObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSObject<D,P>;

inline FixedArrayBase elements() const;
inline FixedArrayBase elements(PtrComprCageBase cage_base) const;
inline void set_elements(FixedArrayBase value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kElementsOffset = P::kHeaderSize;
  static constexpr int kElementsOffsetEnd = kElementsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kElementsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kElementsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kElementsOffsetEnd + 1;
  static constexpr int kHeaderSize = kElementsOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSObject, 
      DAlias>::value,
      "class TorqueGeneratedJSObject should be used as direct base for JSObject.");
  }
 protected:
  inline explicit TorqueGeneratedJSObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSCustomElementsObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSCustomElementsObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSCustomElementsObject : public P {
  static_assert(std::is_same<JSCustomElementsObject, D>::value,
    "Use this class as direct base for JSCustomElementsObject.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSCustomElementsObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSCustomElementsObject<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSCustomElementsObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSCustomElementsObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSCustomElementsObject, 
      DAlias>::value,
      "class TorqueGeneratedJSCustomElementsObject should be used as direct base for JSCustomElementsObject.");
  }
 protected:
  inline explicit TorqueGeneratedJSCustomElementsObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSCustomElementsObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSSpecialObject() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSSpecialObject_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSSpecialObject : public P {
  static_assert(std::is_same<JSSpecialObject, D>::value,
    "Use this class as direct base for JSSpecialObject.");
  static_assert(std::is_same<JSCustomElementsObject, P>::value,
    "Pass in JSCustomElementsObject as second template parameter for TorqueGeneratedJSSpecialObject.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSSpecialObject<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSSpecialObjectVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSSpecialObject() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSSpecialObject, 
      DAlias>::value,
      "class TorqueGeneratedJSSpecialObject should be used as direct base for JSSpecialObject.");
  }
 protected:
  inline explicit TorqueGeneratedJSSpecialObject(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSSpecialObject(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSGlobalProxy() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSGlobalProxy_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSGlobalProxy : public P {
  static_assert(std::is_same<JSGlobalProxy, D>::value,
    "Use this class as direct base for JSGlobalProxy.");
  static_assert(std::is_same<JSSpecialObject, P>::value,
    "Pass in JSSpecialObject as second template parameter for TorqueGeneratedJSGlobalProxy.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSGlobalProxy<D,P>;

inline Object native_context() const;
inline Object native_context(PtrComprCageBase cage_base) const;
inline void set_native_context(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSGlobalProxyVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kNativeContextOffset = P::kHeaderSize;
  static constexpr int kNativeContextOffsetEnd = kNativeContextOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kNativeContextOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kNativeContextOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kNativeContextOffsetEnd + 1;
  static constexpr int kHeaderSize = kNativeContextOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSGlobalProxy() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSGlobalProxy, 
      DAlias>::value,
      "class TorqueGeneratedJSGlobalProxy should be used as direct base for JSGlobalProxy.");
  }
 protected:
  inline explicit TorqueGeneratedJSGlobalProxy(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSGlobalProxy(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSPrimitiveWrapper() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSPrimitiveWrapper_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSPrimitiveWrapper : public P {
  static_assert(std::is_same<JSPrimitiveWrapper, D>::value,
    "Use this class as direct base for JSPrimitiveWrapper.");
  static_assert(std::is_same<JSCustomElementsObject, P>::value,
    "Pass in JSCustomElementsObject as second template parameter for TorqueGeneratedJSPrimitiveWrapper.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSPrimitiveWrapper<D,P>;

  // Torque type: JSAny
inline Object value() const;
inline Object value(PtrComprCageBase cage_base) const;
inline void set_value(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSPrimitiveWrapperVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kValueOffset = P::kHeaderSize;
  static constexpr int kValueOffsetEnd = kValueOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kValueOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kValueOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kValueOffsetEnd + 1;
  static constexpr int kHeaderSize = kValueOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSPrimitiveWrapper() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSPrimitiveWrapper, 
      DAlias>::value,
      "class TorqueGeneratedJSPrimitiveWrapper should be used as direct base for JSPrimitiveWrapper.");
  }
 protected:
  inline explicit TorqueGeneratedJSPrimitiveWrapper(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSPrimitiveWrapper(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSDate() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSDate_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSDate : public P {
  static_assert(std::is_same<JSDate, D>::value,
    "Use this class as direct base for JSDate.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSDate.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSDate<D,P>;

  // Torque type: NumberOrUndefined
inline Object value() const;
inline Object value(PtrComprCageBase cage_base) const;
inline void set_value(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | Undefined | NaN)
inline Object year() const;
inline Object year(PtrComprCageBase cage_base) const;
inline void set_year(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | Undefined | NaN)
inline Object month() const;
inline Object month(PtrComprCageBase cage_base) const;
inline void set_month(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | Undefined | NaN)
inline Object day() const;
inline Object day(PtrComprCageBase cage_base) const;
inline void set_day(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | Undefined | NaN)
inline Object weekday() const;
inline Object weekday(PtrComprCageBase cage_base) const;
inline void set_weekday(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | Undefined | NaN)
inline Object hour() const;
inline Object hour(PtrComprCageBase cage_base) const;
inline void set_hour(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | Undefined | NaN)
inline Object min() const;
inline Object min(PtrComprCageBase cage_base) const;
inline void set_min(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | Undefined | NaN)
inline Object sec() const;
inline Object sec(PtrComprCageBase cage_base) const;
inline void set_sec(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | Undefined | NaN)
inline Object cache_stamp() const;
inline Object cache_stamp(PtrComprCageBase cage_base) const;
inline void set_cache_stamp(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSDateVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kValueOffset = P::kHeaderSize;
  static constexpr int kValueOffsetEnd = kValueOffset + kTaggedSize - 1;
  static constexpr int kYearOffset = kValueOffsetEnd + 1;
  static constexpr int kYearOffsetEnd = kYearOffset + kTaggedSize - 1;
  static constexpr int kMonthOffset = kYearOffsetEnd + 1;
  static constexpr int kMonthOffsetEnd = kMonthOffset + kTaggedSize - 1;
  static constexpr int kDayOffset = kMonthOffsetEnd + 1;
  static constexpr int kDayOffsetEnd = kDayOffset + kTaggedSize - 1;
  static constexpr int kWeekdayOffset = kDayOffsetEnd + 1;
  static constexpr int kWeekdayOffsetEnd = kWeekdayOffset + kTaggedSize - 1;
  static constexpr int kHourOffset = kWeekdayOffsetEnd + 1;
  static constexpr int kHourOffsetEnd = kHourOffset + kTaggedSize - 1;
  static constexpr int kMinOffset = kHourOffsetEnd + 1;
  static constexpr int kMinOffsetEnd = kMinOffset + kTaggedSize - 1;
  static constexpr int kSecOffset = kMinOffsetEnd + 1;
  static constexpr int kSecOffsetEnd = kSecOffset + kTaggedSize - 1;
  static constexpr int kCacheStampOffset = kSecOffsetEnd + 1;
  static constexpr int kCacheStampOffsetEnd = kCacheStampOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kCacheStampOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kCacheStampOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kCacheStampOffsetEnd + 1;
  static constexpr int kHeaderSize = kCacheStampOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSDate() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSDate, 
      DAlias>::value,
      "class TorqueGeneratedJSDate should be used as direct base for JSDate.");
  }
 protected:
  inline explicit TorqueGeneratedJSDate(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSDate(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSAsyncFromSyncIterator() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSAsyncFromSyncIterator_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSAsyncFromSyncIterator : public P {
  static_assert(std::is_same<JSAsyncFromSyncIterator, D>::value,
    "Use this class as direct base for JSAsyncFromSyncIterator.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSAsyncFromSyncIterator.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSAsyncFromSyncIterator<D,P>;

inline JSReceiver sync_iterator() const;
inline JSReceiver sync_iterator(PtrComprCageBase cage_base) const;
inline void set_sync_iterator(JSReceiver value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object next() const;
inline Object next(PtrComprCageBase cage_base) const;
inline void set_next(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSAsyncFromSyncIteratorVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kSyncIteratorOffset = P::kHeaderSize;
  static constexpr int kSyncIteratorOffsetEnd = kSyncIteratorOffset + kTaggedSize - 1;
  static constexpr int kNextOffset = kSyncIteratorOffsetEnd + 1;
  static constexpr int kNextOffsetEnd = kNextOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kNextOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kNextOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kNextOffsetEnd + 1;
  static constexpr int kHeaderSize = kNextOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSAsyncFromSyncIterator() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSAsyncFromSyncIterator, 
      DAlias>::value,
      "class TorqueGeneratedJSAsyncFromSyncIterator should be used as direct base for JSAsyncFromSyncIterator.");
  }
 protected:
  inline explicit TorqueGeneratedJSAsyncFromSyncIterator(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSAsyncFromSyncIterator(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsJSStringIterator() that avoids inlining.
V8_EXPORT_PRIVATE bool IsJSStringIterator_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedJSStringIterator : public P {
  static_assert(std::is_same<JSStringIterator, D>::value,
    "Use this class as direct base for JSStringIterator.");
  static_assert(std::is_same<JSObject, P>::value,
    "Pass in JSObject as second template parameter for TorqueGeneratedJSStringIterator.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedJSStringIterator<D,P>;

inline String string() const;
inline String string(PtrComprCageBase cage_base) const;
inline void set_string(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int index() const;
inline void set_index(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void JSStringIteratorVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kStringOffset = P::kHeaderSize;
  static constexpr int kStringOffsetEnd = kStringOffset + kTaggedSize - 1;
  static constexpr int kIndexOffset = kStringOffsetEnd + 1;
  static constexpr int kIndexOffsetEnd = kIndexOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kIndexOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kIndexOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kIndexOffsetEnd + 1;
  static constexpr int kHeaderSize = kIndexOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedJSStringIterator() : P() {
    static_assert(std::is_base_of<TorqueGeneratedJSStringIterator, 
      DAlias>::value,
      "class TorqueGeneratedJSStringIterator should be used as direct base for JSStringIterator.");
  }
 protected:
  inline explicit TorqueGeneratedJSStringIterator(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedJSStringIterator(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

