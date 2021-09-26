class Script;
// Alias for HeapObject::IsScript() that avoids inlining.
V8_EXPORT_PRIVATE bool IsScript_NonInline(HeapObject o);
template <class D, class P>
class TorqueGeneratedScript : public P {
  static_assert(std::is_same<Script, D>::value,
    "Use this class as direct base for Script.");
  static_assert(std::is_same<Struct, P>::value,
    "Pass in Struct as second template parameter for TorqueGeneratedScript.");
 public: 
  using Super = P;
  using TorqueGeneratedClass = TorqueGeneratedScript<D,P>;

  // Torque type: (class String | Undefined)
inline PrimitiveHeapObject source() const;
inline PrimitiveHeapObject source(PtrComprCageBase cage_base) const;
inline void set_source(PrimitiveHeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object name() const;
inline Object name(PtrComprCageBase cage_base) const;
inline void set_name(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int line_offset() const;
inline void set_line_offset(int value);

inline int column_offset() const;
inline void set_column_offset(int value);

  // Torque type: (Smi | class Symbol | Undefined)
inline Object context_data() const;
inline Object context_data(PtrComprCageBase cage_base) const;
inline void set_context_data(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int script_type() const;
inline void set_script_type(int value);

  // Torque type: (Undefined | class FixedArray)
inline HeapObject line_ends() const;
inline HeapObject line_ends(PtrComprCageBase cage_base) const;
inline void set_line_ends(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int id() const;
inline void set_id(int value);

  // Torque type: (Undefined | class FixedArray | class SharedFunctionInfo)
inline HeapObject eval_from_shared_or_wrapped_arguments_or_sfi_table() const;
inline HeapObject eval_from_shared_or_wrapped_arguments_or_sfi_table(PtrComprCageBase cage_base) const;
inline void set_eval_from_shared_or_wrapped_arguments_or_sfi_table(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (Smi | class Foreign)
inline Object eval_from_position() const;
inline Object eval_from_position(PtrComprCageBase cage_base) const;
inline void set_eval_from_position(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

  // Torque type: (class WeakFixedArray | class WeakArrayList)
inline HeapObject shared_function_infos() const;
inline HeapObject shared_function_infos(PtrComprCageBase cage_base) const;
inline void set_shared_function_infos(HeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline int flags() const;
inline void set_flags(int value);

  // Torque type: (class String | Undefined)
inline PrimitiveHeapObject source_url() const;
inline PrimitiveHeapObject source_url(PtrComprCageBase cage_base) const;
inline void set_source_url(PrimitiveHeapObject value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline Object source_mapping_url() const;
inline Object source_mapping_url(PtrComprCageBase cage_base) const;
inline void set_source_mapping_url(Object value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

inline FixedArray host_defined_options() const;
inline FixedArray host_defined_options(PtrComprCageBase cage_base) const;
inline void set_host_defined_options(FixedArray value, WriteBarrierMode mode = UPDATE_WRITE_BARRIER);

V8_INLINE static D cast(Object object) {
  return D(object.ptr());
}

V8_INLINE static D unchecked_cast(Object object) {
  return bit_cast<D>(object);
}

#ifdef VERIFY_HEAP
V8_EXPORT_PRIVATE void ScriptVerify(Isolate* isolate);
#endif  // VERIFY_HEAP

  static constexpr int kStartOfStrongFieldsOffset = P::kHeaderSize;
  static constexpr int kSourceOffset = P::kHeaderSize;
  static constexpr int kSourceOffsetEnd = kSourceOffset + kTaggedSize - 1;
  static constexpr int kNameOffset = kSourceOffsetEnd + 1;
  static constexpr int kNameOffsetEnd = kNameOffset + kTaggedSize - 1;
  static constexpr int kLineOffsetOffset = kNameOffsetEnd + 1;
  static constexpr int kLineOffsetOffsetEnd = kLineOffsetOffset + kTaggedSize - 1;
  static constexpr int kColumnOffsetOffset = kLineOffsetOffsetEnd + 1;
  static constexpr int kColumnOffsetOffsetEnd = kColumnOffsetOffset + kTaggedSize - 1;
  static constexpr int kContextDataOffset = kColumnOffsetOffsetEnd + 1;
  static constexpr int kContextDataOffsetEnd = kContextDataOffset + kTaggedSize - 1;
  static constexpr int kScriptTypeOffset = kContextDataOffsetEnd + 1;
  static constexpr int kScriptTypeOffsetEnd = kScriptTypeOffset + kTaggedSize - 1;
  static constexpr int kLineEndsOffset = kScriptTypeOffsetEnd + 1;
  static constexpr int kLineEndsOffsetEnd = kLineEndsOffset + kTaggedSize - 1;
  static constexpr int kIdOffset = kLineEndsOffsetEnd + 1;
  static constexpr int kIdOffsetEnd = kIdOffset + kTaggedSize - 1;
  static constexpr int kEvalFromSharedOrWrappedArgumentsOrSfiTableOffset = kIdOffsetEnd + 1;
  static constexpr int kEvalFromSharedOrWrappedArgumentsOrSfiTableOffsetEnd = kEvalFromSharedOrWrappedArgumentsOrSfiTableOffset + kTaggedSize - 1;
  static constexpr int kEvalFromPositionOffset = kEvalFromSharedOrWrappedArgumentsOrSfiTableOffsetEnd + 1;
  static constexpr int kEvalFromPositionOffsetEnd = kEvalFromPositionOffset + kTaggedSize - 1;
  static constexpr int kSharedFunctionInfosOffset = kEvalFromPositionOffsetEnd + 1;
  static constexpr int kSharedFunctionInfosOffsetEnd = kSharedFunctionInfosOffset + kTaggedSize - 1;
  static constexpr int kFlagsOffset = kSharedFunctionInfosOffsetEnd + 1;
  static constexpr int kFlagsOffsetEnd = kFlagsOffset + kTaggedSize - 1;
  static constexpr int kSourceUrlOffset = kFlagsOffsetEnd + 1;
  static constexpr int kSourceUrlOffsetEnd = kSourceUrlOffset + kTaggedSize - 1;
  static constexpr int kSourceMappingUrlOffset = kSourceUrlOffsetEnd + 1;
  static constexpr int kSourceMappingUrlOffsetEnd = kSourceMappingUrlOffset + kTaggedSize - 1;
  static constexpr int kHostDefinedOptionsOffset = kSourceMappingUrlOffsetEnd + 1;
  static constexpr int kHostDefinedOptionsOffsetEnd = kHostDefinedOptionsOffset + kTaggedSize - 1;
  static constexpr int kEndOfStrongFieldsOffset = kHostDefinedOptionsOffsetEnd + 1;
  static constexpr int kStartOfWeakFieldsOffset = kHostDefinedOptionsOffsetEnd + 1;
  static constexpr int kEndOfWeakFieldsOffset = kHostDefinedOptionsOffsetEnd + 1;
  static constexpr int kHeaderSize = kHostDefinedOptionsOffsetEnd + 1;
  static constexpr int kSize = kHostDefinedOptionsOffsetEnd + 1;

V8_INLINE static constexpr int32_t SizeFor() {
    DCHECK(kHeaderSize == kSize && kHeaderSize == 64);
    int32_t size = kHeaderSize;
    return size;
}

V8_INLINE int32_t AllocatedSize() const {
    return SizeFor();
}

  friend class Factory;

 public:
  template <class DAlias = D>
  constexpr TorqueGeneratedScript() : P() {
    static_assert(std::is_base_of<TorqueGeneratedScript, 
      DAlias>::value,
      "class TorqueGeneratedScript should be used as direct base for Script.");
  }
 protected:
  inline explicit TorqueGeneratedScript(Address ptr);
  // Special-purpose constructor for subclasses that have fast paths where
  // their ptr() is a Smi.
  inline explicit TorqueGeneratedScript(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi);
};

