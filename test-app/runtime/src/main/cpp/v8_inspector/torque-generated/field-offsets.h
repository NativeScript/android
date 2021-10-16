#ifndef V8_GEN_TORQUE_GENERATED_FIELD_OFFSETS_H_
#define V8_GEN_TORQUE_GENERATED_FIELD_OFFSETS_H_

#define TORQUE_GENERATED_HEAP_OBJECT_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kMapOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_JS_RECEIVER_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kPropertiesOrHashOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_BIG_INT_BASE_FIELDS(V) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kHeaderSize, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_JS_FUNCTION_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kSharedFunctionInfoOffset, kTaggedSize) \
V(kContextOffset, kTaggedSize) \
V(kFeedbackCellOffset, kTaggedSize) \
V(kCodeOffset, kTaggedSize) \
V(kPrototypeOrInitialMapOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_MAP_FIELDS(V) \
V(kInstanceSizeInWordsOffset, kUInt8Size) \
V(kInObjectPropertiesStartOrConstructorFunctionIndexOffset, kUInt8Size) \
V(kUsedOrUnusedInstanceSizeInWordsOffset, kUInt8Size) \
V(kVisitorIdOffset, kUInt8Size) \
V(kInstanceTypeOffset, kUInt16Size) \
V(kBitFieldOffset, kUInt8Size) \
V(kBitField2Offset, kUInt8Size) \
V(kBitField3Offset, kInt32Size) \
V(kOptionalPaddingOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kPrototypeOffset, kTaggedSize) \
V(kConstructorOrBackPointerOrNativeContextOffset, kTaggedSize) \
V(kInstanceDescriptorsOffset, kTaggedSize) \
V(kDependentCodeOffset, kTaggedSize) \
V(kPrototypeValidityCellOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kTransitionsOrPrototypeInfoOffset, kTaggedSize) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_DATA_HANDLER_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kSmiHandlerOffset, kTaggedSize) \
V(kValidityCellOffset, kTaggedSize) \
V(kData1Offset, kTaggedSize) \
V(kData2Offset, kTaggedSize) \
V(kData3Offset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_JS_SLOPPY_ARGUMENTS_OBJECT_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kLengthOffset, kTaggedSize) \
V(kCalleeOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_JS_STRICT_ARGUMENTS_OBJECT_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kLengthOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_MUTABLE_BIG_INT_FIELDS(V) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kHeaderSize, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_BYTECODE_ARRAY_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kConstantPoolOffset, kTaggedSize) \
V(kHandlerTableOffset, kTaggedSize) \
V(kSourcePositionTableOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kFrameSizeOffset, kInt32Size) \
V(kParameterSizeOffset, kInt32Size) \
V(kIncomingNewTargetOrGeneratorRegisterOffset, kInt32Size) \
V(kOsrNestingLevelOffset, kUInt8Size) \
V(kBytecodeAgeOffset, kUInt8Size) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_REG_EXP_MATCH_INFO_FIELDS(V) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_JS_ARRAY_ITERATOR_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kIteratedObjectOffset, kTaggedSize) \
V(kNextIndexOffset, kTaggedSize) \
V(kKindOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_JS_ARRAY_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kLengthOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_JS_GLOBAL_OBJECT_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kNativeContextOffset, kTaggedSize) \
V(kGlobalProxyOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_JS_MESSAGE_OBJECT_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kMessageTypeOffset, kTaggedSize) \
V(kArgumentsOffset, kTaggedSize) \
V(kScriptOffset, kTaggedSize) \
V(kStackFramesOffset, kTaggedSize) \
V(kSharedInfoOffset, kTaggedSize) \
V(kBytecodeOffsetOffset, kTaggedSize) \
V(kStartPositionOffset, kTaggedSize) \
V(kEndPositionOffset, kTaggedSize) \
V(kErrorLevelOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_JS_PROXY_REVOCABLE_RESULT_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kProxyOffset, kTaggedSize) \
V(kRevokeOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_JS_REG_EXP_RESULT_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kIndexOffset, kTaggedSize) \
V(kInputOffset, kTaggedSize) \
V(kGroupsOffset, kTaggedSize) \
V(kNamesOffset, kTaggedSize) \
V(kRegexpInputOffset, kTaggedSize) \
V(kRegexpLastIndexOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_JS_REG_EXP_RESULT_WITH_INDICES_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kIndicesOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_JS_REG_EXP_RESULT_INDICES_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kGroupsOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_JS_FINALIZATION_REGISTRY_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kNativeContextOffset, kTaggedSize) \
V(kCleanupOffset, kTaggedSize) \
V(kActiveCellsOffset, kTaggedSize) \
V(kClearedCellsOffset, kTaggedSize) \
V(kKeyMapOffset, kTaggedSize) \
V(kNextDirtyOffset, kTaggedSize) \
V(kFlagsOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_MODULE_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kExportsOffset, kTaggedSize) \
V(kHashOffset, kTaggedSize) \
V(kStatusOffset, kTaggedSize) \
V(kModuleNamespaceOffset, kTaggedSize) \
V(kExceptionOffset, kTaggedSize) \
V(kTopLevelCapabilityOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_SMALL_ORDERED_HASH_TABLE_FIELDS(V) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_SMALL_ORDERED_HASH_SET_FIELDS(V) \
V(kNumberOfElementsOffset, kUInt8Size) \
V(kNumberOfDeletedElementsOffset, kUInt8Size) \
V(kNumberOfBucketsOffset, kUInt8Size) \
V(kHeaderSize, 0) \
V(kPaddingOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kDataTableOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \

#define TORQUE_GENERATED_SMALL_ORDERED_HASH_MAP_FIELDS(V) \
V(kNumberOfElementsOffset, kUInt8Size) \
V(kNumberOfDeletedElementsOffset, kUInt8Size) \
V(kNumberOfBucketsOffset, kUInt8Size) \
V(kHeaderSize, 0) \
V(kPaddingOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kDataTableOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \

#define TORQUE_GENERATED_SMALL_ORDERED_NAME_DICTIONARY_FIELDS(V) \
V(kHashOffset, kInt32Size) \
V(kPadding0Offset, 0) \
V(kNumberOfElementsOffset, kUInt8Size) \
V(kNumberOfDeletedElementsOffset, kUInt8Size) \
V(kNumberOfBucketsOffset, kUInt8Size) \
V(kHeaderSize, 0) \
V(kPadding1Offset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kDataTableOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \

#define TORQUE_GENERATED_PROPERTY_ARRAY_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kLengthAndHashOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_PROPERTY_CELL_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kNameOffset, kTaggedSize) \
V(kPropertyDetailsRawOffset, kTaggedSize) \
V(kValueOffset, kTaggedSize) \
V(kDependentCodeOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_INTERPRETER_DATA_FIELDS(V) \
V(kStartOfStrongFieldsOffset, 0) \
V(kBytecodeArrayOffset, kTaggedSize) \
V(kInterpreterTrampolineOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kHeaderSize, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_EXTERNAL_STRING_FIELDS(V) \
V(kResourceOffset, kExternalPointerSize) \
V(kResourceDataOffset, kExternalPointerSize) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kHeaderSize, 0) \

#define TORQUE_GENERATED_EXTERNAL_ONE_BYTE_STRING_FIELDS(V) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kHeaderSize, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_EXTERNAL_TWO_BYTE_STRING_FIELDS(V) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kHeaderSize, 0) \
V(kSize, 0) \

#define TORQUE_GENERATED_SWISS_NAME_DICTIONARY_FIELDS(V) \
V(kHashOffset, kInt32Size) \
V(kCapacityOffset, kInt32Size) \
V(kStartOfStrongFieldsOffset, 0) \
V(kMetaTableOffset, kTaggedSize) \
V(kHeaderSize, 0) \
V(kDataTableOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \

#define TORQUE_INSTANCE_TYPE_TO_BODY_DESCRIPTOR_LIST(V)\
V(ODDBALL_TYPE,Oddball)\
V(FIXED_ARRAY_TYPE,FixedArray)\
V(WEAK_FIXED_ARRAY_TYPE,WeakFixedArray)\
V(SLOPPY_ARGUMENTS_ELEMENTS_TYPE,SloppyArgumentsElements)\
V(SCOPE_INFO_TYPE,ScopeInfo)\
V(DESCRIPTOR_ARRAY_TYPE,DescriptorArray)\
V(STRONG_DESCRIPTOR_ARRAY_TYPE,StrongDescriptorArray)\
V(FEEDBACK_VECTOR_TYPE,FeedbackVector)\
V(WEAK_ARRAY_LIST_TYPE,WeakArrayList)\
V(MEGA_DOM_HANDLER_TYPE,MegaDomHandler)\
V(SHARED_FUNCTION_INFO_TYPE,SharedFunctionInfo)\
V(UNCOMPILED_DATA_WITHOUT_PREPARSE_DATA_TYPE,UncompiledDataWithoutPreparseData)\
V(UNCOMPILED_DATA_WITH_PREPARSE_DATA_TYPE,UncompiledDataWithPreparseData)\
V(ON_HEAP_BASIC_BLOCK_PROFILER_DATA_TYPE,OnHeapBasicBlockProfilerData)\
V(INTERNAL_CLASS_TYPE,InternalClass)\
V(SMI_PAIR_TYPE,SmiPair)\
V(SMI_BOX_TYPE,SmiBox)\
V(EXPORTED_SUB_CLASS_BASE_TYPE,ExportedSubClassBase)\
V(EXPORTED_SUB_CLASS_TYPE,ExportedSubClass)\
V(ABSTRACT_INTERNAL_CLASS_SUBCLASS1_TYPE,AbstractInternalClassSubclass1)\
V(ABSTRACT_INTERNAL_CLASS_SUBCLASS2_TYPE,AbstractInternalClassSubclass2)\
V(INTERNAL_CLASS_WITH_SMI_ELEMENTS_TYPE,InternalClassWithSmiElements)\
V(INTERNAL_CLASS_WITH_STRUCT_ELEMENTS_TYPE,InternalClassWithStructElements)\
V(EXPORTED_SUB_CLASS2_TYPE,ExportedSubClass2)\
V(SORT_STATE_TYPE,SortState)\

#define TORQUE_DATA_ONLY_VISITOR_ID_LIST(V)\

#define TORQUE_POINTER_VISITOR_ID_LIST(V)\
V(Context)\
V(Oddball)\
V(FixedArray)\
V(WeakFixedArray)\
V(SloppyArgumentsElements)\
V(ScopeInfo)\
V(DescriptorArray)\
V(StrongDescriptorArray)\
V(FeedbackVector)\
V(WeakArrayList)\
V(MegaDomHandler)\
V(SharedFunctionInfo)\
V(UncompiledDataWithoutPreparseData)\
V(UncompiledDataWithPreparseData)\
V(OnHeapBasicBlockProfilerData)\
V(ConsString)\
V(SeqOneByteString)\
V(SeqTwoByteString)\
V(SlicedString)\
V(ThinString)\
V(InternalClass)\
V(SmiPair)\
V(SmiBox)\
V(ExportedSubClassBase)\
V(ExportedSubClass)\
V(AbstractInternalClassSubclass1)\
V(AbstractInternalClassSubclass2)\
V(InternalClassWithSmiElements)\
V(InternalClassWithStructElements)\
V(ExportedSubClass2)\
V(SortState)\

#endif  // V8_GEN_TORQUE_GENERATED_FIELD_OFFSETS_H_
