#ifndef V8_GEN_TORQUE_GENERATED_BIT_FIELDS_H_
#define V8_GEN_TORQUE_GENERATED_BIT_FIELDS_H_

#include "src/base/bit-field.h"

namespace v8 {
namespace internal {
#define DEFINE_TORQUE_GENERATED_INTERCEPTOR_INFO_FLAGS() \
  using CanInterceptSymbolsBit = base::BitField<bool, 0, 1, uint32_t>; \
  using AllCanReadBit = base::BitField<bool, 1, 1, uint32_t>; \
  using NonMaskingBit = base::BitField<bool, 2, 1, uint32_t>; \
  using NamedBit = base::BitField<bool, 3, 1, uint32_t>; \
  using HasNoSideEffectBit = base::BitField<bool, 4, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kCanInterceptSymbols = 1 << 0, \
    kAllCanRead = 1 << 1, \
    kNonMasking = 1 << 2, \
    kNamed = 1 << 3, \
    kHasNoSideEffect = 1 << 4, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 5; \

#define DEFINE_TORQUE_GENERATED_ACCESSOR_INFO_FLAGS() \
  using AllCanReadBit = base::BitField<bool, 0, 1, uint32_t>; \
  using AllCanWriteBit = base::BitField<bool, 1, 1, uint32_t>; \
  using IsSpecialDataPropertyBit = base::BitField<bool, 2, 1, uint32_t>; \
  using IsSloppyBit = base::BitField<bool, 3, 1, uint32_t>; \
  using ReplaceOnAccessBit = base::BitField<bool, 4, 1, uint32_t>; \
  using GetterSideEffectTypeBits = base::BitField<SideEffectType, 5, 2, uint32_t>; \
  using SetterSideEffectTypeBits = base::BitField<SideEffectType, 7, 2, uint32_t>; \
  using InitialAttributesBits = base::BitField<PropertyAttributes, 9, 3, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_DEBUG_INFO_FLAGS() \
  using HasBreakInfoBit = base::BitField<bool, 0, 1, uint32_t>; \
  using PreparedForDebugExecutionBit = base::BitField<bool, 1, 1, uint32_t>; \
  using HasCoverageInfoBit = base::BitField<bool, 2, 1, uint32_t>; \
  using BreakAtEntryBit = base::BitField<bool, 3, 1, uint32_t>; \
  using CanBreakAtEntryBit = base::BitField<bool, 4, 1, uint32_t>; \
  using DebugExecutionModeBit = base::BitField<bool, 5, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kHasBreakInfo = 1 << 0, \
    kPreparedForDebugExecution = 1 << 1, \
    kHasCoverageInfo = 1 << 2, \
    kBreakAtEntry = 1 << 3, \
    kCanBreakAtEntry = 1 << 4, \
    kDebugExecutionMode = 1 << 5, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 6; \

#define DEFINE_TORQUE_GENERATED_DEBUGGER_HINTS() \
  using SideEffectStateBits = base::BitField<int32_t, 0, 2, uint32_t>; \
  using DebugIsBlackboxedBit = base::BitField<bool, 2, 1, uint32_t>; \
  using ComputedDebugIsBlackboxedBit = base::BitField<bool, 3, 1, uint32_t>; \
  using DebuggingIdBits = base::BitField<int32_t, 4, 20, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_FEEDBACK_VECTOR_FLAGS() \
  using OptimizationMarkerBits = base::BitField<OptimizationMarker, 0, 3, uint32_t>; \
  using OptimizationTierBits = base::BitField<OptimizationTier, 3, 2, uint32_t>; \
  using GlobalTicksAtLastRuntimeProfilerInterruptBits = base::BitField<uint32_t, 5, 24, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_JS_ARRAY_BUFFER_FLAGS() \
  using IsExternalBit = base::BitField<bool, 0, 1, uint32_t>; \
  using IsDetachableBit = base::BitField<bool, 1, 1, uint32_t>; \
  using WasDetachedBit = base::BitField<bool, 2, 1, uint32_t>; \
  using IsAsmJsMemoryBit = base::BitField<bool, 3, 1, uint32_t>; \
  using IsSharedBit = base::BitField<bool, 4, 1, uint32_t>; \
  using IsResizableBit = base::BitField<bool, 5, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kIsExternal = 1 << 0, \
    kIsDetachable = 1 << 1, \
    kWasDetached = 1 << 2, \
    kIsAsmJsMemory = 1 << 3, \
    kIsShared = 1 << 4, \
    kIsResizable = 1 << 5, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 6; \

#define DEFINE_TORQUE_GENERATED_JS_TYPED_ARRAY_FLAGS() \
  using IsLengthTrackingBit = base::BitField<bool, 0, 1, uint32_t>; \
  using IsBackedByRabBit = base::BitField<bool, 1, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kIsLengthTracking = 1 << 0, \
    kIsBackedByRab = 1 << 1, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 2; \

#define DEFINE_TORQUE_GENERATED_JS_PROMISE_FLAGS() \
  using StatusBits = base::BitField<Promise::PromiseState, 0, 2, uint32_t>; \
  using HasHandlerBit = base::BitField<bool, 2, 1, uint32_t>; \
  using HandledHintBit = base::BitField<bool, 3, 1, uint32_t>; \
  using IsSilentBit = base::BitField<bool, 4, 1, uint32_t>; \
  using AsyncTaskIdBits = base::BitField<int32_t, 5, 22, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_JS_REG_EXP_STRING_ITERATOR_FLAGS() \
  using DoneBit = base::BitField<bool, 0, 1, uint32_t>; \
  using GlobalBit = base::BitField<bool, 1, 1, uint32_t>; \
  using UnicodeBit = base::BitField<bool, 2, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kDone = 1 << 0, \
    kGlobal = 1 << 1, \
    kUnicode = 1 << 2, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 3; \

#define DEFINE_TORQUE_GENERATED_JS_REG_EXP_FLAGS() \
  using GlobalBit = base::BitField<bool, 0, 1, uint32_t>; \
  using IgnoreCaseBit = base::BitField<bool, 1, 1, uint32_t>; \
  using MultilineBit = base::BitField<bool, 2, 1, uint32_t>; \
  using StickyBit = base::BitField<bool, 3, 1, uint32_t>; \
  using UnicodeBit = base::BitField<bool, 4, 1, uint32_t>; \
  using DotAllBit = base::BitField<bool, 5, 1, uint32_t>; \
  using LinearBit = base::BitField<bool, 6, 1, uint32_t>; \
  using HasIndicesBit = base::BitField<bool, 7, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kGlobal = 1 << 0, \
    kIgnoreCase = 1 << 1, \
    kMultiline = 1 << 2, \
    kSticky = 1 << 3, \
    kUnicode = 1 << 4, \
    kDotAll = 1 << 5, \
    kLinear = 1 << 6, \
    kHasIndices = 1 << 7, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 8; \

#define DEFINE_TORQUE_GENERATED_FINALIZATION_REGISTRY_FLAGS() \
  using ScheduledForCleanupBit = base::BitField<bool, 0, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kScheduledForCleanup = 1 << 0, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 1; \

#define DEFINE_TORQUE_GENERATED_MAP_BIT_FIELDS1() \
  using HasNonInstancePrototypeBit = base::BitField<bool, 0, 1, uint8_t>; \
  using IsCallableBit = base::BitField<bool, 1, 1, uint8_t>; \
  using HasNamedInterceptorBit = base::BitField<bool, 2, 1, uint8_t>; \
  using HasIndexedInterceptorBit = base::BitField<bool, 3, 1, uint8_t>; \
  using IsUndetectableBit = base::BitField<bool, 4, 1, uint8_t>; \
  using IsAccessCheckNeededBit = base::BitField<bool, 5, 1, uint8_t>; \
  using IsConstructorBit = base::BitField<bool, 6, 1, uint8_t>; \
  using HasPrototypeSlotBit = base::BitField<bool, 7, 1, uint8_t>; \
  enum Flag { \
    kNone = 0, \
    kHasNonInstancePrototype = 1 << 0, \
    kIsCallable = 1 << 1, \
    kHasNamedInterceptor = 1 << 2, \
    kHasIndexedInterceptor = 1 << 3, \
    kIsUndetectable = 1 << 4, \
    kIsAccessCheckNeeded = 1 << 5, \
    kIsConstructor = 1 << 6, \
    kHasPrototypeSlot = 1 << 7, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 8; \

#define DEFINE_TORQUE_GENERATED_MAP_BIT_FIELDS2() \
  using NewTargetIsBaseBit = base::BitField<bool, 0, 1, uint8_t>; \
  using IsImmutablePrototypeBit = base::BitField<bool, 1, 1, uint8_t>; \
  using ElementsKindBits = base::BitField<ElementsKind, 2, 6, uint8_t>; \

#define DEFINE_TORQUE_GENERATED_MAP_BIT_FIELDS3() \
  using EnumLengthBits = base::BitField<int32_t, 0, 10, uint32_t>; \
  using NumberOfOwnDescriptorsBits = base::BitField<int32_t, 10, 10, uint32_t>; \
  using IsPrototypeMapBit = base::BitField<bool, 20, 1, uint32_t>; \
  using IsDictionaryMapBit = base::BitField<bool, 21, 1, uint32_t>; \
  using OwnsDescriptorsBit = base::BitField<bool, 22, 1, uint32_t>; \
  using IsInRetainedMapListBit = base::BitField<bool, 23, 1, uint32_t>; \
  using IsDeprecatedBit = base::BitField<bool, 24, 1, uint32_t>; \
  using IsUnstableBit = base::BitField<bool, 25, 1, uint32_t>; \
  using IsMigrationTargetBit = base::BitField<bool, 26, 1, uint32_t>; \
  using IsExtensibleBit = base::BitField<bool, 27, 1, uint32_t>; \
  using MayHaveInterestingSymbolsBit = base::BitField<bool, 28, 1, uint32_t>; \
  using ConstructionCounterBits = base::BitField<int32_t, 29, 3, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_NAME_HASH() \
  using HashNotComputedBit = base::BitField<bool, 0, 1, uint32_t>; \
  using IsNotIntegerIndexMaskBit = base::BitField<bool, 1, 1, uint32_t>; \
  using ArrayIndexValueBits = base::BitField<uint32_t, 2, 24, uint32_t>; \
  using ArrayIndexLengthBits = base::BitField<uint32_t, 26, 6, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_SYMBOL_FLAGS() \
  using IsPrivateBit = base::BitField<bool, 0, 1, uint32_t>; \
  using IsWellKnownSymbolBit = base::BitField<bool, 1, 1, uint32_t>; \
  using IsInPublicSymbolTableBit = base::BitField<bool, 2, 1, uint32_t>; \
  using IsInterestingSymbolBit = base::BitField<bool, 3, 1, uint32_t>; \
  using IsPrivateNameBit = base::BitField<bool, 4, 1, uint32_t>; \
  using IsPrivateBrandBit = base::BitField<bool, 5, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kIsPrivate = 1 << 0, \
    kIsWellKnownSymbol = 1 << 1, \
    kIsInPublicSymbolTable = 1 << 2, \
    kIsInterestingSymbol = 1 << 3, \
    kIsPrivateName = 1 << 4, \
    kIsPrivateBrand = 1 << 5, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 6; \

#define DEFINE_TORQUE_GENERATED_PROPERTY_DESCRIPTOR_OBJECT_FLAGS() \
  using IsEnumerableBit = base::BitField<bool, 0, 1, uint32_t>; \
  using HasEnumerableBit = base::BitField<bool, 1, 1, uint32_t>; \
  using IsConfigurableBit = base::BitField<bool, 2, 1, uint32_t>; \
  using HasConfigurableBit = base::BitField<bool, 3, 1, uint32_t>; \
  using IsWritableBit = base::BitField<bool, 4, 1, uint32_t>; \
  using HasWritableBit = base::BitField<bool, 5, 1, uint32_t>; \
  using HasValueBit = base::BitField<bool, 6, 1, uint32_t>; \
  using HasGetBit = base::BitField<bool, 7, 1, uint32_t>; \
  using HasSetBit = base::BitField<bool, 8, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kIsEnumerable = 1 << 0, \
    kHasEnumerable = 1 << 1, \
    kIsConfigurable = 1 << 2, \
    kHasConfigurable = 1 << 3, \
    kIsWritable = 1 << 4, \
    kHasWritable = 1 << 5, \
    kHasValue = 1 << 6, \
    kHasGet = 1 << 7, \
    kHasSet = 1 << 8, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 9; \

#define DEFINE_TORQUE_GENERATED_PROTOTYPE_INFO_FLAGS() \
  using ShouldBeFastBit = base::BitField<bool, 0, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kShouldBeFast = 1 << 0, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 1; \

#define DEFINE_TORQUE_GENERATED_SCOPE_FLAGS() \
  using ScopeTypeBits = base::BitField<ScopeType, 0, 4, uint32_t>; \
  using SloppyEvalCanExtendVarsBit = base::BitField<bool, 4, 1, uint32_t>; \
  using LanguageModeBit = base::BitField<LanguageMode, 5, 1, uint32_t>; \
  using DeclarationScopeBit = base::BitField<bool, 6, 1, uint32_t>; \
  using ReceiverVariableBits = base::BitField<VariableAllocationInfo, 7, 2, uint32_t>; \
  using HasClassBrandBit = base::BitField<bool, 9, 1, uint32_t>; \
  using HasSavedClassVariableIndexBit = base::BitField<bool, 10, 1, uint32_t>; \
  using HasNewTargetBit = base::BitField<bool, 11, 1, uint32_t>; \
  using FunctionVariableBits = base::BitField<VariableAllocationInfo, 12, 2, uint32_t>; \
  using HasInferredFunctionNameBit = base::BitField<bool, 14, 1, uint32_t>; \
  using IsAsmModuleBit = base::BitField<bool, 15, 1, uint32_t>; \
  using HasSimpleParametersBit = base::BitField<bool, 16, 1, uint32_t>; \
  using FunctionKindBits = base::BitField<FunctionKind, 17, 5, uint32_t>; \
  using HasOuterScopeInfoBit = base::BitField<bool, 22, 1, uint32_t>; \
  using IsDebugEvaluateScopeBit = base::BitField<bool, 23, 1, uint32_t>; \
  using ForceContextAllocationBit = base::BitField<bool, 24, 1, uint32_t>; \
  using PrivateNameLookupSkipsOuterClassBit = base::BitField<bool, 25, 1, uint32_t>; \
  using HasContextExtensionSlotBit = base::BitField<bool, 26, 1, uint32_t>; \
  using IsReplModeScopeBit = base::BitField<bool, 27, 1, uint32_t>; \
  using HasLocalsBlockListBit = base::BitField<bool, 28, 1, uint32_t>; \
  using IsEmptyBit = base::BitField<bool, 29, 1, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_VARIABLE_PROPERTIES() \
  using VariableModeBits = base::BitField<VariableMode, 0, 4, uint32_t>; \
  using InitFlagBit = base::BitField<InitializationFlag, 4, 1, uint32_t>; \
  using MaybeAssignedFlagBit = base::BitField<MaybeAssignedFlag, 5, 1, uint32_t>; \
  using ParameterNumberBits = base::BitField<uint32_t, 6, 16, uint32_t>; \
  using IsStaticFlagBit = base::BitField<IsStaticFlag, 22, 1, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_SCRIPT_FLAGS() \
  using CompilationTypeBit = base::BitField<Script::CompilationType, 0, 1, uint32_t>; \
  using CompilationStateBit = base::BitField<Script::CompilationState, 1, 1, uint32_t>; \
  using IsReplModeBit = base::BitField<bool, 2, 1, uint32_t>; \
  using OriginOptionsBits = base::BitField<int32_t, 3, 4, uint32_t>; \
  using BreakOnEntryBit = base::BitField<bool, 7, 1, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_SHARED_FUNCTION_INFO_FLAGS() \
  using FunctionKindBits = base::BitField<FunctionKind, 0, 5, uint32_t>; \
  using IsNativeBit = base::BitField<bool, 5, 1, uint32_t>; \
  using IsStrictBit = base::BitField<bool, 6, 1, uint32_t>; \
  using FunctionSyntaxKindBits = base::BitField<FunctionSyntaxKind, 7, 3, uint32_t>; \
  using IsClassConstructorBit = base::BitField<bool, 10, 1, uint32_t>; \
  using HasDuplicateParametersBit = base::BitField<bool, 11, 1, uint32_t>; \
  using AllowLazyCompilationBit = base::BitField<bool, 12, 1, uint32_t>; \
  using IsAsmWasmBrokenBit = base::BitField<bool, 13, 1, uint32_t>; \
  using FunctionMapIndexBits = base::BitField<uint32_t, 14, 5, uint32_t>; \
  using DisabledOptimizationReasonBits = base::BitField<BailoutReason, 19, 4, uint32_t>; \
  using RequiresInstanceMembersInitializerBit = base::BitField<bool, 23, 1, uint32_t>; \
  using ConstructAsBuiltinBit = base::BitField<bool, 24, 1, uint32_t>; \
  using NameShouldPrintAsAnonymousBit = base::BitField<bool, 25, 1, uint32_t>; \
  using HasReportedBinaryCoverageBit = base::BitField<bool, 26, 1, uint32_t>; \
  using IsTopLevelBit = base::BitField<bool, 27, 1, uint32_t>; \
  using PropertiesAreFinalBit = base::BitField<bool, 28, 1, uint32_t>; \
  using PrivateNameLookupSkipsOuterClassBit = base::BitField<bool, 29, 1, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_SHARED_FUNCTION_INFO_FLAGS2() \
  using ClassScopeHasPrivateBrandBit = base::BitField<bool, 0, 1, uint8_t>; \
  using HasStaticPrivateMethodsOrAccessorsBit = base::BitField<bool, 1, 1, uint8_t>; \
  enum Flag { \
    kNone = 0, \
    kClassScopeHasPrivateBrand = 1 << 0, \
    kHasStaticPrivateMethodsOrAccessors = 1 << 1, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 2; \

#define DEFINE_TORQUE_GENERATED_SOURCE_TEXT_MODULE_FLAGS() \
  using AsyncBit = base::BitField<bool, 0, 1, uint32_t>; \
  using AsyncEvaluatingOrdinalBits = base::BitField<uint32_t, 1, 30, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_STACK_FRAME_INFO_FLAGS() \
  using IsWasmBit = base::BitField<bool, 0, 1, uint32_t>; \
  using IsAsmJsWasmBit = base::BitField<bool, 1, 1, uint32_t>; \
  using IsStrictBit = base::BitField<bool, 2, 1, uint32_t>; \
  using IsConstructorBit = base::BitField<bool, 3, 1, uint32_t>; \
  using IsAsmJsAtNumberConversionBit = base::BitField<bool, 4, 1, uint32_t>; \
  using IsAsyncBit = base::BitField<bool, 5, 1, uint32_t>; \
  using IsSourcePositionComputedBit = base::BitField<bool, 6, 1, uint32_t>; \
  enum Flag { \
    kNone = 0, \
    kIsWasm = 1 << 0, \
    kIsAsmJsWasm = 1 << 1, \
    kIsStrict = 1 << 2, \
    kIsConstructor = 1 << 3, \
    kIsAsmJsAtNumberConversion = 1 << 4, \
    kIsAsync = 1 << 5, \
    kIsSourcePositionComputed = 1 << 6, \
  }; \
  using Flags = base::Flags<Flag>; \
  static constexpr int kFlagCount = 7; \

#define DEFINE_TORQUE_GENERATED_STRING_INSTANCE_TYPE() \
  using RepresentationBits = base::BitField<StringRepresentationTag, 0, 3, uint16_t>; \
  using IsOneByteBit = base::BitField<bool, 3, 1, uint16_t>; \
  using IsUncachedBit = base::BitField<bool, 4, 1, uint16_t>; \
  using IsNotInternalizedBit = base::BitField<bool, 5, 1, uint16_t>; \

#define DEFINE_TORQUE_GENERATED_FUNCTION_TEMPLATE_INFO_FLAGS() \
  using UndetectableBit = base::BitField<bool, 0, 1, uint32_t>; \
  using NeedsAccessCheckBit = base::BitField<bool, 1, 1, uint32_t>; \
  using ReadOnlyPrototypeBit = base::BitField<bool, 2, 1, uint32_t>; \
  using RemovePrototypeBit = base::BitField<bool, 3, 1, uint32_t>; \
  using AcceptAnyReceiverBit = base::BitField<bool, 4, 1, uint32_t>; \
  using PublishedBit = base::BitField<bool, 5, 1, uint32_t>; \
  using AllowedReceiverRangeStartBits = base::BitField<int32_t, 6, 12, uint32_t>; \
  using AllowedReceiverRangeEndBits = base::BitField<int32_t, 18, 12, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_OBJECT_TEMPLATE_INFO_FLAGS() \
  using IsImmutablePrototypeBit = base::BitField<bool, 0, 1, uint32_t>; \
  using IsCodeKindBit = base::BitField<bool, 1, 1, uint32_t>; \
  using EmbedderFieldCountBits = base::BitField<int32_t, 2, 28, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_TEST_BIT_FIELD_STRUCT() \
  using ABit = base::BitField<bool, 0, 1, uint8_t>; \
  using BBits = base::BitField<uint16_t, 1, 3, uint8_t>; \
  using CBits = base::BitField<uint32_t, 4, 3, uint8_t>; \
  using DBit = base::BitField<bool, 7, 1, uint8_t>; \

#define DEFINE_TORQUE_GENERATED_TEST_BIT_FIELD_STRUCT2() \
  using ABits = base::BitField<uintptr_t, 0, 5, uint32_t>; \
  using BBits = base::BitField<uintptr_t, 5, 6, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_TEST_BIT_FIELD_STRUCT3() \
  using CBit = base::BitField<bool, 0, 1, uintptr_t>; \
  using DBits = base::BitField<uint32_t, 1, 9, uintptr_t>; \
  using EBits = base::BitField<uintptr_t, 10, 17, uintptr_t>; \

#define DEFINE_TORQUE_GENERATED_TEST_BIT_FIELD_STRUCT4() \
  using ABit = base::BitField<bool, 0, 1, uint32_t>; \
  using BBits = base::BitField<int32_t, 1, 3, uint32_t>; \
  using CBit = base::BitField<bool, 4, 1, uint32_t>; \

#define DEFINE_TORQUE_GENERATED_TEST_BIT_FIELD_STRUCT5() \
  using BBits = base::BitField<int32_t, 0, 19, uint32_t>; \
  using ABit = base::BitField<bool, 19, 1, uint32_t>; \
  using CBit = base::BitField<bool, 20, 1, uint32_t>; \

}  // namespace internal
}  // namespace v8
#endif  // V8_GEN_TORQUE_GENERATED_BIT_FIELDS_H_
