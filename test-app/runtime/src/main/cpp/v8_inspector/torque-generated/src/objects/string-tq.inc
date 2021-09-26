class String;
class ConsString;
class ExternalString;
class ExternalOneByteString;
class ExternalTwoByteString;
class InternalizedString;
class SeqString;
class SeqOneByteString;
class SeqTwoByteString;
class SlicedString;
class ThinString;
// Alias for HeapObject::IsString() that avoids inlining.
V8_EXPORT_PRIVATE bool IsString_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedString : public P {
  static_assert(std::is_same<String, D>::value,
    "Use this class as direct base for String.");
  static_assert(std::is_same<Name, P>::value,
    "Pass in Name as second template parameter for TorqueGeneratedString.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedString<D,P>;

inline int32_t length() const;
inline void set_length(int32_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void StringVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kLengthOffset = P::kHeaderSize;
  static constexpr int kLengthOffsetEnd = kLengthOffset + kInt32Size - 1;
  static constexpr int kStartOfWeakFieldsOffset = kLengthOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kLengthOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kLengthOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kLengthOffsetEnd + 1;
  static constexpr int kHeaderSize = kLengthOffsetEnd + 1;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedString() : P() {
    static_assert(std::is_base_of<TorqueGeneratedString, 
      DAlias>::value,
      "class TorqueGeneratedString should be used as direct base for String.");
  }
 protected:
  inline explicit TorqueGeneratedString(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedString(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsConsString() that avoids inlining.
V8_EXPORT_PRIVATE bool IsConsString_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedConsString : public P {
  static_assert(std::is_same<ConsString, D>::value,
    "Use this class as direct base for ConsString.");
  static_assert(std::is_same<String, P>::value,
    "Pass in String as second template parameter for TorqueGeneratedConsString.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedConsString<D,P>;

inline String first() const;
inline String first(PtrComprCageBase cage_base) const;
inline void set_first(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline String second() const;
inline String second(PtrComprCageBase cage_base) const;
inline void set_second(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void ConsStringVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kFirstOffset = P::kHeaderSize;
  static constexpr int kFirstOffsetEnd = kFirstOffset + kTaggedSize - 1;
  static constexpr int kSecondOffset = kFirstOffsetEnd + 1;
  static constexpr int kSecondOffsetEnd = kSecondOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kSecondOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kSecondOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kSecondOffsetEnd + 1;
  static constexpr int kHeaderSize = kSecondOffsetEnd + 1;
  static constexpr int kSize = kSecondOffsetEnd + 1;

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
  constexpr TorqueGeneratedConsString() : P() {
    static_assert(std::is_base_of<TorqueGeneratedConsString, 
      DAlias>::value,
      "class TorqueGeneratedConsString should be used as direct base for ConsString.");
  }
 protected:
  inline explicit TorqueGeneratedConsString(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedConsString(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsInternalizedString() that avoids inlining.
V8_EXPORT_PRIVATE bool IsInternalizedString_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedInternalizedString : public P {
  static_assert(std::is_same<InternalizedString, D>::value,
    "Use this class as direct base for InternalizedString.");
  static_assert(std::is_same<String, P>::value,
    "Pass in String as second template parameter for TorqueGeneratedInternalizedString.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedInternalizedString<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void InternalizedStringVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;
  static constexpr int kSize = P::kHeaderSize;

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
  constexpr TorqueGeneratedInternalizedString() : P() {
    static_assert(std::is_base_of<TorqueGeneratedInternalizedString, 
      DAlias>::value,
      "class TorqueGeneratedInternalizedString should be used as direct base for InternalizedString.");
  }
 protected:
  inline explicit TorqueGeneratedInternalizedString(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedInternalizedString(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsSeqString() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSeqString_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSeqString : public P {
  static_assert(std::is_same<SeqString, D>::value,
    "Use this class as direct base for SeqString.");
  static_assert(std::is_same<String, P>::value,
    "Pass in String as second template parameter for TorqueGeneratedSeqString.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSeqString<D,P>;

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SeqStringVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedSeqString() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSeqString, 
      DAlias>::value,
      "class TorqueGeneratedSeqString should be used as direct base for SeqString.");
  }
 protected:
  inline explicit TorqueGeneratedSeqString(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSeqString(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsSeqOneByteString() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSeqOneByteString_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSeqOneByteString : public P {
  static_assert(std::is_same<SeqOneByteString, D>::value,
    "Use this class as direct base for SeqOneByteString.");
  static_assert(std::is_same<SeqString, P>::value,
    "Pass in SeqString as second template parameter for TorqueGeneratedSeqOneByteString.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSeqOneByteString<D,P>;

inline char chars(int i) const;
inline void set_chars(int i, char value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SeqOneByteStringVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kHeaderSize = P::kHeaderSize;
  static constexpr int kCharsOffset = P::kHeaderSize;
  static constexpr int kCharsOffsetEnd = kCharsOffset + 0 - 1;
  static constexpr int kStartOfWeakFieldsOffset = kCharsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kCharsOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kCharsOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kCharsOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor(int length) {
    int32_t size = kHeaderSize;
    size += length * 1;
    size = OBJECT_POINTER_ALIGN(size);
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor(this->length());
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedSeqOneByteString() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSeqOneByteString, 
      DAlias>::value,
      "class TorqueGeneratedSeqOneByteString should be used as direct base for SeqOneByteString.");
  }
 protected:
  inline explicit TorqueGeneratedSeqOneByteString(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSeqOneByteString(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsSeqTwoByteString() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSeqTwoByteString_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSeqTwoByteString : public P {
  static_assert(std::is_same<SeqTwoByteString, D>::value,
    "Use this class as direct base for SeqTwoByteString.");
  static_assert(std::is_same<SeqString, P>::value,
    "Pass in SeqString as second template parameter for TorqueGeneratedSeqTwoByteString.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSeqTwoByteString<D,P>;

inline char16_t chars(int i) const;
inline void set_chars(int i, char16_t value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SeqTwoByteStringVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kHeaderSize = P::kHeaderSize;
  static constexpr int kCharsOffset = P::kHeaderSize;
  static constexpr int kCharsOffsetEnd = kCharsOffset + 0 - 1;
  static constexpr int kStartOfWeakFieldsOffset = kCharsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kCharsOffsetEnd + 1;
  static constexpr int kStartOfStrongFieldsOffset = kCharsOffsetEnd + 1;
  static constexpr int kEndOfStrongFieldsOffset = kCharsOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor(int length) {
    int32_t size = kHeaderSize;
    size += length * 2;
    size = OBJECT_POINTER_ALIGN(size);
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor(this->length());
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedSeqTwoByteString() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSeqTwoByteString, 
      DAlias>::value,
      "class TorqueGeneratedSeqTwoByteString should be used as direct base for SeqTwoByteString.");
  }
 protected:
  inline explicit TorqueGeneratedSeqTwoByteString(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSeqTwoByteString(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsSlicedString() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSlicedString_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSlicedString : public P {
  static_assert(std::is_same<SlicedString, D>::value,
    "Use this class as direct base for SlicedString.");
  static_assert(std::is_same<String, P>::value,
    "Pass in String as second template parameter for TorqueGeneratedSlicedString.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSlicedString<D,P>;

inline String parent() const;
inline String parent(PtrComprCageBase cage_base) const;
inline void set_parent(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int offset() const;
inline void set_offset(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SlicedStringVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kParentOffset = P::kHeaderSize;
  static constexpr int kParentOffsetEnd = kParentOffset + kTaggedSize - 1;
  static constexpr int kOffsetOffset = kParentOffsetEnd + 1;
  static constexpr int kOffsetOffsetEnd = kOffsetOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kOffsetOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kOffsetOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kOffsetOffsetEnd + 1;
  static constexpr int kHeaderSize = kOffsetOffsetEnd + 1;
  static constexpr int kSize = kOffsetOffsetEnd + 1;

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
  constexpr TorqueGeneratedSlicedString() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSlicedString, 
      DAlias>::value,
      "class TorqueGeneratedSlicedString should be used as direct base for SlicedString.");
  }
 protected:
  inline explicit TorqueGeneratedSlicedString(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSlicedString(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

// Alias for HeapObject::IsThinString() that avoids inlining.
V8_EXPORT_PRIVATE bool IsThinString_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedThinString : public P {
  static_assert(std::is_same<ThinString, D>::value,
    "Use this class as direct base for ThinString.");
  static_assert(std::is_same<String, P>::value,
    "Pass in String as second template parameter for TorqueGeneratedThinString.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedThinString<D,P>;

inline String actual() const;
inline String actual(PtrComprCageBase cage_base) const;
inline void set_actual(String value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void ThinStringVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kActualOffset = P::kHeaderSize;
  static constexpr int kActualOffsetEnd = kActualOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kActualOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kActualOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kActualOffsetEnd + 1;
  static constexpr int kHeaderSize = kActualOffsetEnd + 1;
  static constexpr int kSize = kActualOffsetEnd + 1;

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
  constexpr TorqueGeneratedThinString() : P() {
    static_assert(std::is_base_of<TorqueGeneratedThinString, 
      DAlias>::value,
      "class TorqueGeneratedThinString should be used as direct base for ThinString.");
  }
 protected:
  inline explicit TorqueGeneratedThinString(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedThinString(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

