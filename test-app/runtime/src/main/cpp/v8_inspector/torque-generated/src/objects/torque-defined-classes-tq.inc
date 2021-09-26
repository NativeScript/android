class StrongDescriptorArray;
class InternalClass;
class SmiPair;
class SmiBox;
class ExportedSubClassBase;
class ExportedSubClass;
class AbstractInternalClass;
class AbstractInternalClassSubclass1;
class AbstractInternalClassSubclass2;
class InternalClassWithSmiElements;
class InternalClassWithStructElements;
class ExportedSubClass2;
class SortState;
// Alias for HeapObject::IsStrongDescriptorArray() that avoids inlining.
V8_EXPORT_PRIVATE bool IsStrongDescriptorArray_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedStrongDescriptorArray : public P {
  static_assert(std::is_same<StrongDescriptorArray, D>::value,
    "Use this class as direct base for StrongDescriptorArray.");
  static_assert(std::is_same<DescriptorArray, P>::value,
    "Pass in DescriptorArray as second template parameter for TorqueGeneratedStrongDescriptorArray.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedStrongDescriptorArray<D,P>;

 protected: // not extern or @export
 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(StrongDescriptorArray)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void StrongDescriptorArrayVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;

V8_INLINE static constexpr int32_t SizeFor(int number_of_all_descriptors) {
    int32_t size = kHeaderSize;
    size += number_of_all_descriptors * 12;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor(this->number_of_all_descriptors());
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedStrongDescriptorArray() : P() {
    static_assert(std::is_base_of<TorqueGeneratedStrongDescriptorArray, 
      DAlias>::value,
      "class TorqueGeneratedStrongDescriptorArray should be used as direct base for StrongDescriptorArray.");
  }
 protected:
  inline explicit TorqueGeneratedStrongDescriptorArray(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedStrongDescriptorArray(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class StrongDescriptorArray : public TorqueGeneratedStrongDescriptorArray<StrongDescriptorArray, DescriptorArray> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(StrongDescriptorArray)
};

// Alias for HeapObject::IsInternalClass() that avoids inlining.
V8_EXPORT_PRIVATE bool IsInternalClass_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedInternalClass : public P {
  static_assert(std::is_same<InternalClass, D>::value,
    "Use this class as direct base for InternalClass.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedInternalClass.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedInternalClass<D,P>;

 protected: // not extern or @export
inline int a() const;
inline void set_a(int value);

  // Torque type: Number
inline Object b() const;
inline Object b(PtrComprCageBase cage_base) const;
inline void set_b(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(InternalClass)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void InternalClassVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kAOffset = P::kHeaderSize;
  static constexpr int kAOffsetEnd = kAOffset + kTaggedSize - 1;
  static constexpr int kBOffset = kAOffsetEnd + 1;
  static constexpr int kBOffsetEnd = kBOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kBOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kBOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kBOffsetEnd + 1;
  static constexpr int kHeaderSize = kBOffsetEnd + 1;
  static constexpr int kSize = kBOffsetEnd + 1;

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
  constexpr TorqueGeneratedInternalClass() : P() {
    static_assert(std::is_base_of<TorqueGeneratedInternalClass, 
      DAlias>::value,
      "class TorqueGeneratedInternalClass should be used as direct base for InternalClass.");
  }
 protected:
  inline explicit TorqueGeneratedInternalClass(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedInternalClass(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class InternalClass : public TorqueGeneratedInternalClass<InternalClass, HeapObject> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(InternalClass)
};

// Alias for HeapObject::IsSmiPair() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSmiPair_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSmiPair : public P {
  static_assert(std::is_same<SmiPair, D>::value,
    "Use this class as direct base for SmiPair.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedSmiPair.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSmiPair<D,P>;

 protected: // not extern or @export
inline int a() const;
inline void set_a(int value);

inline int b() const;
inline void set_b(int value);

 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(SmiPair)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SmiPairVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kAOffset = P::kHeaderSize;
  static constexpr int kAOffsetEnd = kAOffset + kTaggedSize - 1;
  static constexpr int kBOffset = kAOffsetEnd + 1;
  static constexpr int kBOffsetEnd = kBOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kBOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kBOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kBOffsetEnd + 1;
  static constexpr int kHeaderSize = kBOffsetEnd + 1;
  static constexpr int kSize = kBOffsetEnd + 1;

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
  constexpr TorqueGeneratedSmiPair() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSmiPair, 
      DAlias>::value,
      "class TorqueGeneratedSmiPair should be used as direct base for SmiPair.");
  }
 protected:
  inline explicit TorqueGeneratedSmiPair(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSmiPair(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class SmiPair : public TorqueGeneratedSmiPair<SmiPair, HeapObject> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(SmiPair)
};

// Alias for HeapObject::IsSmiBox() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSmiBox_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSmiBox : public P {
  static_assert(std::is_same<SmiBox, D>::value,
    "Use this class as direct base for SmiBox.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedSmiBox.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSmiBox<D,P>;

 protected: // not extern or @export
inline int value() const;
inline void set_value(int value);

inline int unrelated() const;
inline void set_unrelated(int value);

 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(SmiBox)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SmiBoxVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kValueOffset = P::kHeaderSize;
  static constexpr int kValueOffsetEnd = kValueOffset + kTaggedSize - 1;
  static constexpr int kUnrelatedOffset = kValueOffsetEnd + 1;
  static constexpr int kUnrelatedOffsetEnd = kUnrelatedOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kUnrelatedOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kUnrelatedOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kUnrelatedOffsetEnd + 1;
  static constexpr int kHeaderSize = kUnrelatedOffsetEnd + 1;
  static constexpr int kSize = kUnrelatedOffsetEnd + 1;

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
  constexpr TorqueGeneratedSmiBox() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSmiBox, 
      DAlias>::value,
      "class TorqueGeneratedSmiBox should be used as direct base for SmiBox.");
  }
 protected:
  inline explicit TorqueGeneratedSmiBox(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSmiBox(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class SmiBox : public TorqueGeneratedSmiBox<SmiBox, HeapObject> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(SmiBox)
};

// Alias for HeapObject::IsExportedSubClassBase() that avoids inlining.
V8_EXPORT_PRIVATE bool IsExportedSubClassBase_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedExportedSubClassBase : public P {
  static_assert(std::is_same<ExportedSubClassBase, D>::value,
    "Use this class as direct base for ExportedSubClassBase.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedExportedSubClassBase.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedExportedSubClassBase<D,P>;

inline HeapObject a() const;
inline HeapObject a(PtrComprCageBase cage_base) const;
inline void set_a(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline HeapObject b() const;
inline HeapObject b(PtrComprCageBase cage_base) const;
inline void set_b(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(ExportedSubClassBase)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void ExportedSubClassBaseVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kAOffset = P::kHeaderSize;
  static constexpr int kAOffsetEnd = kAOffset + kTaggedSize - 1;
  static constexpr int kBOffset = kAOffsetEnd + 1;
  static constexpr int kBOffsetEnd = kBOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kBOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kBOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kBOffsetEnd + 1;
  static constexpr int kHeaderSize = kBOffsetEnd + 1;
  static constexpr int kSize = kBOffsetEnd + 1;

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
  constexpr TorqueGeneratedExportedSubClassBase() : P() {
    static_assert(std::is_base_of<TorqueGeneratedExportedSubClassBase, 
      DAlias>::value,
      "class TorqueGeneratedExportedSubClassBase should be used as direct base for ExportedSubClassBase.");
  }
 protected:
  inline explicit TorqueGeneratedExportedSubClassBase(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedExportedSubClassBase(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class ExportedSubClassBase : public TorqueGeneratedExportedSubClassBase<ExportedSubClassBase, HeapObject> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(ExportedSubClassBase)
};

// Alias for HeapObject::IsExportedSubClass() that avoids inlining.
V8_EXPORT_PRIVATE bool IsExportedSubClass_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedExportedSubClass : public P {
  static_assert(std::is_same<ExportedSubClass, D>::value,
    "Use this class as direct base for ExportedSubClass.");
  static_assert(std::is_same<ExportedSubClassBase, P>::value,
    "Pass in ExportedSubClassBase as second template parameter for TorqueGeneratedExportedSubClass.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedExportedSubClass<D,P>;

inline int32_t c_field() const;
inline void set_c_field(int32_t value);

inline int32_t d_field() const;
inline void set_d_field(int32_t value);

inline int e_field() const;
inline void set_e_field(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(ExportedSubClass)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void ExportedSubClassVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kCFieldOffset = P::kHeaderSize;
  static constexpr int kCFieldOffsetEnd = kCFieldOffset + kInt32Size - 1;
  static constexpr int kDFieldOffset = kCFieldOffsetEnd + 1;
  static constexpr int kDFieldOffsetEnd = kDFieldOffset + kInt32Size - 1;
  static constexpr int kStartOfStrongFieldsOffset = kDFieldOffsetEnd + 1;
  static constexpr int kEFieldOffset = kDFieldOffsetEnd + 1;
  static constexpr int kEFieldOffsetEnd = kEFieldOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kEFieldOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kEFieldOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kEFieldOffsetEnd + 1;
  static constexpr int kHeaderSize = kEFieldOffsetEnd + 1;
  static constexpr int kSize = kEFieldOffsetEnd + 1;

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
  constexpr TorqueGeneratedExportedSubClass() : P() {
    static_assert(std::is_base_of<TorqueGeneratedExportedSubClass, 
      DAlias>::value,
      "class TorqueGeneratedExportedSubClass should be used as direct base for ExportedSubClass.");
  }
 protected:
  inline explicit TorqueGeneratedExportedSubClass(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedExportedSubClass(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class ExportedSubClass : public TorqueGeneratedExportedSubClass<ExportedSubClass, ExportedSubClassBase> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(ExportedSubClass)
};

// Alias for HeapObject::IsAbstractInternalClass() that avoids inlining.
V8_EXPORT_PRIVATE bool IsAbstractInternalClass_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedAbstractInternalClass : public P {
  static_assert(std::is_same<AbstractInternalClass, D>::value,
    "Use this class as direct base for AbstractInternalClass.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedAbstractInternalClass.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedAbstractInternalClass<D,P>;

 protected: // not extern or @export
 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(AbstractInternalClass)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void AbstractInternalClassVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedAbstractInternalClass() : P() {
    static_assert(std::is_base_of<TorqueGeneratedAbstractInternalClass, 
      DAlias>::value,
      "class TorqueGeneratedAbstractInternalClass should be used as direct base for AbstractInternalClass.");
  }
 protected:
  inline explicit TorqueGeneratedAbstractInternalClass(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedAbstractInternalClass(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class AbstractInternalClass : public TorqueGeneratedAbstractInternalClass<AbstractInternalClass, HeapObject> {
 public:
  TQ_OBJECT_CONSTRUCTORS(AbstractInternalClass)
};

// Alias for HeapObject::IsAbstractInternalClassSubclass1() that avoids inlining.
V8_EXPORT_PRIVATE bool IsAbstractInternalClassSubclass1_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedAbstractInternalClassSubclass1 : public P {
  static_assert(std::is_same<AbstractInternalClassSubclass1, D>::value,
    "Use this class as direct base for AbstractInternalClassSubclass1.");
  static_assert(std::is_same<AbstractInternalClass, P>::value,
    "Pass in AbstractInternalClass as second template parameter for TorqueGeneratedAbstractInternalClassSubclass1.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedAbstractInternalClassSubclass1<D,P>;

 protected: // not extern or @export
 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(AbstractInternalClassSubclass1)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void AbstractInternalClassSubclass1Verify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;
  static constexpr int kSize = P::kHeaderSize;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 4);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedAbstractInternalClassSubclass1() : P() {
    static_assert(std::is_base_of<TorqueGeneratedAbstractInternalClassSubclass1, 
      DAlias>::value,
      "class TorqueGeneratedAbstractInternalClassSubclass1 should be used as direct base for AbstractInternalClassSubclass1.");
  }
 protected:
  inline explicit TorqueGeneratedAbstractInternalClassSubclass1(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedAbstractInternalClassSubclass1(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class AbstractInternalClassSubclass1 : public TorqueGeneratedAbstractInternalClassSubclass1<AbstractInternalClassSubclass1, AbstractInternalClass> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(AbstractInternalClassSubclass1)
};

// Alias for HeapObject::IsAbstractInternalClassSubclass2() that avoids inlining.
V8_EXPORT_PRIVATE bool IsAbstractInternalClassSubclass2_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedAbstractInternalClassSubclass2 : public P {
  static_assert(std::is_same<AbstractInternalClassSubclass2, D>::value,
    "Use this class as direct base for AbstractInternalClassSubclass2.");
  static_assert(std::is_same<AbstractInternalClass, P>::value,
    "Pass in AbstractInternalClass as second template parameter for TorqueGeneratedAbstractInternalClassSubclass2.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedAbstractInternalClassSubclass2<D,P>;

 protected: // not extern or @export
 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(AbstractInternalClassSubclass2)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void AbstractInternalClassSubclass2Verify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfWeakFieldsOffset = P::kHeaderSize;
  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kEndOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kHeaderSize = P::kHeaderSize;
  static constexpr int kSize = P::kHeaderSize;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 4);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedAbstractInternalClassSubclass2() : P() {
    static_assert(std::is_base_of<TorqueGeneratedAbstractInternalClassSubclass2, 
      DAlias>::value,
      "class TorqueGeneratedAbstractInternalClassSubclass2 should be used as direct base for AbstractInternalClassSubclass2.");
  }
 protected:
  inline explicit TorqueGeneratedAbstractInternalClassSubclass2(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedAbstractInternalClassSubclass2(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class AbstractInternalClassSubclass2 : public TorqueGeneratedAbstractInternalClassSubclass2<AbstractInternalClassSubclass2, AbstractInternalClass> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(AbstractInternalClassSubclass2)
};

// Alias for HeapObject::IsInternalClassWithSmiElements() that avoids inlining.
V8_EXPORT_PRIVATE bool IsInternalClassWithSmiElements_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedInternalClassWithSmiElements : public P {
  static_assert(std::is_same<InternalClassWithSmiElements, D>::value,
    "Use this class as direct base for InternalClassWithSmiElements.");
  static_assert(std::is_same<FixedArrayBase, P>::value,
    "Pass in FixedArrayBase as second template parameter for TorqueGeneratedInternalClassWithSmiElements.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedInternalClassWithSmiElements<D,P>;

 protected: // not extern or @export
inline int data() const;
inline void set_data(int value);

inline Oddball object() const;
inline Oddball object(PtrComprCageBase cage_base) const;
inline void set_object(Oddball value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int entries(int i) const;
inline void set_entries(int i, int value);

 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(InternalClassWithSmiElements)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void InternalClassWithSmiElementsVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kDataOffset = P::kHeaderSize;
  static constexpr int kDataOffsetEnd = kDataOffset + kTaggedSize - 1;
  static constexpr int kObjectOffset = kDataOffsetEnd + 1;
  static constexpr int kObjectOffsetEnd = kObjectOffset + kTaggedSize - 1;
  static constexpr int kHeaderSize = kObjectOffsetEnd + 1;
  static constexpr int kEntriesOffset = kObjectOffsetEnd + 1;
  static constexpr int kEntriesOffsetEnd = kEntriesOffset + 0 - 1;
  static constexpr int kEndOfStrongFieldsOffset = kEntriesOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kEntriesOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kEntriesOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor(int length) {
    int32_t size = kHeaderSize;
    size += length * 4;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor(this->length());
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedInternalClassWithSmiElements() : P() {
    static_assert(std::is_base_of<TorqueGeneratedInternalClassWithSmiElements, 
      DAlias>::value,
      "class TorqueGeneratedInternalClassWithSmiElements should be used as direct base for InternalClassWithSmiElements.");
  }
 protected:
  inline explicit TorqueGeneratedInternalClassWithSmiElements(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedInternalClassWithSmiElements(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class InternalClassWithSmiElements : public TorqueGeneratedInternalClassWithSmiElements<InternalClassWithSmiElements, FixedArrayBase> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(InternalClassWithSmiElements)
};

// Alias for HeapObject::IsInternalClassWithStructElements() that avoids inlining.
V8_EXPORT_PRIVATE bool IsInternalClassWithStructElements_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedInternalClassWithStructElements : public P {
  static_assert(std::is_same<InternalClassWithStructElements, D>::value,
    "Use this class as direct base for InternalClassWithStructElements.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedInternalClassWithStructElements.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedInternalClassWithStructElements<D,P>;

 protected: // not extern or @export
inline int32_t dummy1() const;
inline void set_dummy1(int32_t value);

inline int32_t dummy2() const;
inline void set_dummy2(int32_t value);

inline int count() const;
inline void set_count(int value);

inline int data() const;
inline void set_data(int value);

inline Object object() const;
inline Object object(PtrComprCageBase cage_base) const;
inline void set_object(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int entries(int i) const;
inline void set_entries(int i, int value);

inline int more_entries_a(int i) const;
inline void set_more_entries_a(int i, int value);

inline int more_entries_b(int i) const;
inline void set_more_entries_b(int i, int value);

 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(InternalClassWithStructElements)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void InternalClassWithStructElementsVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kDummy1Offset = P::kHeaderSize;
  static constexpr int kDummy1OffsetEnd = kDummy1Offset + kInt32Size - 1;
  static constexpr int kDummy2Offset = kDummy1OffsetEnd + 1;
  static constexpr int kDummy2OffsetEnd = kDummy2Offset + kInt32Size - 1;
  static constexpr int kStartOfStrongFieldsOffset = kDummy2OffsetEnd + 1;
  static constexpr int kCountOffset = kDummy2OffsetEnd + 1;
  static constexpr int kCountOffsetEnd = kCountOffset + kTaggedSize - 1;
  static constexpr int kDataOffset = kCountOffsetEnd + 1;
  static constexpr int kDataOffsetEnd = kDataOffset + kTaggedSize - 1;
  static constexpr int kObjectOffset = kDataOffsetEnd + 1;
  static constexpr int kObjectOffsetEnd = kObjectOffset + kTaggedSize - 1;
  static constexpr int kHeaderSize = kObjectOffsetEnd + 1;
  static constexpr int kEntriesOffset = kObjectOffsetEnd + 1;
  static constexpr int kEntriesOffsetEnd = kEntriesOffset + 0 - 1;
inline int MoreEntriesOffset() const;
  static constexpr int kEndOfStrongFieldsOffset = kEntriesOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kEntriesOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kEntriesOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor(int count) {
    int32_t size = kHeaderSize;
    size += count * 4;
    size += count * 8;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor(this->count());
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedInternalClassWithStructElements() : P() {
    static_assert(std::is_base_of<TorqueGeneratedInternalClassWithStructElements, 
      DAlias>::value,
      "class TorqueGeneratedInternalClassWithStructElements should be used as direct base for InternalClassWithStructElements.");
  }
 protected:
  inline explicit TorqueGeneratedInternalClassWithStructElements(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedInternalClassWithStructElements(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class InternalClassWithStructElements : public TorqueGeneratedInternalClassWithStructElements<InternalClassWithStructElements, HeapObject> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(InternalClassWithStructElements)
};

// Alias for HeapObject::IsExportedSubClass2() that avoids inlining.
V8_EXPORT_PRIVATE bool IsExportedSubClass2_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedExportedSubClass2 : public P {
  static_assert(std::is_same<ExportedSubClass2, D>::value,
    "Use this class as direct base for ExportedSubClass2.");
  static_assert(std::is_same<ExportedSubClassBase, P>::value,
    "Pass in ExportedSubClassBase as second template parameter for TorqueGeneratedExportedSubClass2.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedExportedSubClass2<D,P>;

inline int32_t x_field() const;
inline void set_x_field(int32_t value);

inline int32_t y_field() const;
inline void set_y_field(int32_t value);

inline int z_field() const;
inline void set_z_field(int value);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(ExportedSubClass2)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void ExportedSubClass2Verify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kXFieldOffset = P::kHeaderSize;
  static constexpr int kXFieldOffsetEnd = kXFieldOffset + kInt32Size - 1;
  static constexpr int kYFieldOffset = kXFieldOffsetEnd + 1;
  static constexpr int kYFieldOffsetEnd = kYFieldOffset + kInt32Size - 1;
  static constexpr int kStartOfStrongFieldsOffset = kYFieldOffsetEnd + 1;
  static constexpr int kZFieldOffset = kYFieldOffsetEnd + 1;
  static constexpr int kZFieldOffsetEnd = kZFieldOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kZFieldOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kZFieldOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kZFieldOffsetEnd + 1;
  static constexpr int kHeaderSize = kZFieldOffsetEnd + 1;
  static constexpr int kSize = kZFieldOffsetEnd + 1;

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
  constexpr TorqueGeneratedExportedSubClass2() : P() {
    static_assert(std::is_base_of<TorqueGeneratedExportedSubClass2, 
      DAlias>::value,
      "class TorqueGeneratedExportedSubClass2 should be used as direct base for ExportedSubClass2.");
  }
 protected:
  inline explicit TorqueGeneratedExportedSubClass2(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedExportedSubClass2(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class ExportedSubClass2 : public TorqueGeneratedExportedSubClass2<ExportedSubClass2, ExportedSubClassBase> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(ExportedSubClass2)
};

// Alias for HeapObject::IsSortState() that avoids inlining.
V8_EXPORT_PRIVATE bool IsSortState_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedSortState : public P {
  static_assert(std::is_same<SortState, D>::value,
    "Use this class as direct base for SortState.");
  static_assert(std::is_same<HeapObject, P>::value,
    "Pass in HeapObject as second template parameter for TorqueGeneratedSortState.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedSortState<D,P>;

 protected: // not extern or @export
inline JSReceiver receiver() const;
inline JSReceiver receiver(PtrComprCageBase cage_base) const;
inline void set_receiver(JSReceiver value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Map initialReceiverMap() const;
inline Map initialReceiverMap(PtrComprCageBase cage_base) const;
inline void set_initialReceiverMap(Map value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: Number
inline Object initialReceiverLength() const;
inline Object initialReceiverLength(PtrComprCageBase cage_base) const;
inline void set_initialReceiverLength(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Undefined | CallableApiObject | CallableJSProxy | class JSFunction | class JSBoundFunction)
inline HeapObject userCmpFn() const;
inline HeapObject userCmpFn(PtrComprCageBase cage_base) const;
inline void set_userCmpFn(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int sortComparePtr() const;
inline void set_sortComparePtr(int value);

inline int loadFn() const;
inline void set_loadFn(int value);

inline int storeFn() const;
inline void set_storeFn(int value);

inline int deleteFn() const;
inline void set_deleteFn(int value);

inline int canUseSameAccessorFn() const;
inline void set_canUseSameAccessorFn(int value);

inline int minGallop() const;
inline void set_minGallop(int value);

inline int pendingRunsSize() const;
inline void set_pendingRunsSize(int value);

inline FixedArray pendingRuns() const;
inline FixedArray pendingRuns(PtrComprCageBase cage_base) const;
inline void set_pendingRuns(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray workArray() const;
inline FixedArray workArray(PtrComprCageBase cage_base) const;
inline void set_workArray(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray tempArray() const;
inline FixedArray tempArray(PtrComprCageBase cage_base) const;
inline void set_tempArray(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int sortLength() const;
inline void set_sortLength(int value);

inline int numberOfUndefined() const;
inline void set_numberOfUndefined(int value);

 public:
V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}


  DECL_PRINTER(SortState)
#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void SortStateVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kReceiverOffset = P::kHeaderSize;
  static constexpr int kReceiverOffsetEnd = kReceiverOffset + kTaggedSize - 1;
  static constexpr int kInitialReceiverMapOffset = kReceiverOffsetEnd + 1;
  static constexpr int kInitialReceiverMapOffsetEnd = kInitialReceiverMapOffset + kTaggedSize - 1;
  static constexpr int kInitialReceiverLengthOffset = kInitialReceiverMapOffsetEnd + 1;
  static constexpr int kInitialReceiverLengthOffsetEnd = kInitialReceiverLengthOffset + kTaggedSize - 1;
  static constexpr int kUserCmpFnOffset = kInitialReceiverLengthOffsetEnd + 1;
  static constexpr int kUserCmpFnOffsetEnd = kUserCmpFnOffset + kTaggedSize - 1;
  static constexpr int kSortComparePtrOffset = kUserCmpFnOffsetEnd + 1;
  static constexpr int kSortComparePtrOffsetEnd = kSortComparePtrOffset + kTaggedSize - 1;
  static constexpr int kLoadFnOffset = kSortComparePtrOffsetEnd + 1;
  static constexpr int kLoadFnOffsetEnd = kLoadFnOffset + kTaggedSize - 1;
  static constexpr int kStoreFnOffset = kLoadFnOffsetEnd + 1;
  static constexpr int kStoreFnOffsetEnd = kStoreFnOffset + kTaggedSize - 1;
  static constexpr int kDeleteFnOffset = kStoreFnOffsetEnd + 1;
  static constexpr int kDeleteFnOffsetEnd = kDeleteFnOffset + kTaggedSize - 1;
  static constexpr int kCanUseSameAccessorFnOffset = kDeleteFnOffsetEnd + 1;
  static constexpr int kCanUseSameAccessorFnOffsetEnd = kCanUseSameAccessorFnOffset + kTaggedSize - 1;
  static constexpr int kMinGallopOffset = kCanUseSameAccessorFnOffsetEnd + 1;
  static constexpr int kMinGallopOffsetEnd = kMinGallopOffset + kTaggedSize - 1;
  static constexpr int kPendingRunsSizeOffset = kMinGallopOffsetEnd + 1;
  static constexpr int kPendingRunsSizeOffsetEnd = kPendingRunsSizeOffset + kTaggedSize - 1;
  static constexpr int kPendingRunsOffset = kPendingRunsSizeOffsetEnd + 1;
  static constexpr int kPendingRunsOffsetEnd = kPendingRunsOffset + kTaggedSize - 1;
  static constexpr int kWorkArrayOffset = kPendingRunsOffsetEnd + 1;
  static constexpr int kWorkArrayOffsetEnd = kWorkArrayOffset + kTaggedSize - 1;
  static constexpr int kTempArrayOffset = kWorkArrayOffsetEnd + 1;
  static constexpr int kTempArrayOffsetEnd = kTempArrayOffset + kTaggedSize - 1;
  static constexpr int kSortLengthOffset = kTempArrayOffsetEnd + 1;
  static constexpr int kSortLengthOffsetEnd = kSortLengthOffset + kTaggedSize - 1;
  static constexpr int kNumberOfUndefinedOffset = kSortLengthOffsetEnd + 1;
  static constexpr int kNumberOfUndefinedOffsetEnd = kNumberOfUndefinedOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kNumberOfUndefinedOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kNumberOfUndefinedOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kNumberOfUndefinedOffsetEnd + 1;
  static constexpr int kHeaderSize = kNumberOfUndefinedOffsetEnd + 1;
  static constexpr int kSize = kNumberOfUndefinedOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 68);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedSortState() : P() {
    static_assert(std::is_base_of<TorqueGeneratedSortState, 
      DAlias>::value,
      "class TorqueGeneratedSortState should be used as direct base for SortState.");
  }
 protected:
  inline explicit TorqueGeneratedSortState(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedSortState(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

class SortState : public TorqueGeneratedSortState<SortState, HeapObject> {
 public:
  class BodyDescriptor;
  TQ_OBJECT_CONSTRUCTORS(SortState)
};

