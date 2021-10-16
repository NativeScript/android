class ArrayEveryLoopContinuationDescriptor : public StaticCallInterfaceDescriptor<ArrayEveryLoopContinuationDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kCallbackfn, kThisArg, kArray, kO, kInitialK, kLength, kInitialTo)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ArrayEveryLoopContinuationDescriptor)
};

class ArrayFilterLoopContinuationDescriptor : public StaticCallInterfaceDescriptor<ArrayFilterLoopContinuationDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kCallbackfn, kThisArg, kArray, kO, kInitialK, kLength, kInitialTo)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ArrayFilterLoopContinuationDescriptor)
};

class ArrayFindLoopContinuationDescriptor : public StaticCallInterfaceDescriptor<ArrayFindLoopContinuationDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kCallbackfn, kThisArg, kO, kInitialK, kLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ArrayFindLoopContinuationDescriptor)
};

class ArrayFindIndexLoopContinuationDescriptor : public StaticCallInterfaceDescriptor<ArrayFindIndexLoopContinuationDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kCallbackfn, kThisArg, kO, kInitialK, kLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ArrayFindIndexLoopContinuationDescriptor)
};

class ArrayForEachLoopContinuationDescriptor : public StaticCallInterfaceDescriptor<ArrayForEachLoopContinuationDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kCallbackfn, kThisArg, kArray, kO, kInitialK, kLen, kTo)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ArrayForEachLoopContinuationDescriptor)
};

class LoadJoinElement_DictionaryElements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinElement_DictionaryElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinElement_DictionaryElements_0Descriptor)
};

class LoadJoinElement_FastSmiOrObjectElements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinElement_FastSmiOrObjectElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinElement_FastSmiOrObjectElements_0Descriptor)
};

class LoadJoinElement_FastDoubleElements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinElement_FastDoubleElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinElement_FastDoubleElements_0Descriptor)
};

class ConvertToLocaleStringDescriptor : public StaticCallInterfaceDescriptor<ConvertToLocaleStringDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kElement, kLocales, kOptions)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ConvertToLocaleStringDescriptor)
};

class JoinStackPushDescriptor : public StaticCallInterfaceDescriptor<JoinStackPushDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kStack, kReceiver)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(JoinStackPushDescriptor)
};

class JoinStackPopDescriptor : public StaticCallInterfaceDescriptor<JoinStackPopDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kStack, kReceiver)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(JoinStackPopDescriptor)
};

class ArrayMapLoopContinuationDescriptor : public StaticCallInterfaceDescriptor<ArrayMapLoopContinuationDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kCallbackfn, kThisArg, kArray, kO, kInitialK, kLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ArrayMapLoopContinuationDescriptor)
};

class ArrayReduceRightLoopContinuationDescriptor : public StaticCallInterfaceDescriptor<ArrayReduceRightLoopContinuationDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kCallbackfn, kInitialAccumulator, kO, kInitialK, kLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ArrayReduceRightLoopContinuationDescriptor)
};

class ArrayReduceLoopContinuationDescriptor : public StaticCallInterfaceDescriptor<ArrayReduceLoopContinuationDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kCallbackfn, kInitialAccumulator, kO, kInitialK, kLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ArrayReduceLoopContinuationDescriptor)
};

class ArraySomeLoopContinuationDescriptor : public StaticCallInterfaceDescriptor<ArraySomeLoopContinuationDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kCallbackfn, kThisArg, kArray, kO, kInitialK, kLength, kInitialTo)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ArraySomeLoopContinuationDescriptor)
};

class ToIntegerDescriptor : public StaticCallInterfaceDescriptor<ToIntegerDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ToIntegerDescriptor)
};

class FastCreateDataPropertyDescriptor : public StaticCallInterfaceDescriptor<FastCreateDataPropertyDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kKey, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(FastCreateDataPropertyDescriptor)
};

class CheckNumberInRangeDescriptor : public StaticCallInterfaceDescriptor<CheckNumberInRangeDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kValue, kMin, kMax, kNodeId)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(CheckNumberInRangeDescriptor)
};

class CheckSameObjectDescriptor : public StaticCallInterfaceDescriptor<CheckSameObjectDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLhs, kRhs)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CheckSameObjectDescriptor)
};

class BigIntAddNoThrowDescriptor : public StaticCallInterfaceDescriptor<BigIntAddNoThrowDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kX, kY)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(BigIntAddNoThrowDescriptor)
};

class BigIntAddDescriptor : public StaticCallInterfaceDescriptor<BigIntAddDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kXNum, kYNum)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(BigIntAddDescriptor)
};

class BigIntSubtractNoThrowDescriptor : public StaticCallInterfaceDescriptor<BigIntSubtractNoThrowDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kX, kY)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(BigIntSubtractNoThrowDescriptor)
};

class BigIntSubtractDescriptor : public StaticCallInterfaceDescriptor<BigIntSubtractDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kXNum, kYNum)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(BigIntSubtractDescriptor)
};

class BigIntUnaryMinusDescriptor : public StaticCallInterfaceDescriptor<BigIntUnaryMinusDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kBigint)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(BigIntUnaryMinusDescriptor)
};

class ToStringDescriptor : public StaticCallInterfaceDescriptor<ToStringDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kO)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ToStringDescriptor)
};

class StringToListDescriptor : public StaticCallInterfaceDescriptor<StringToListDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kString)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(StringToListDescriptor)
};

class StringAddConvertLeftDescriptor : public StaticCallInterfaceDescriptor<StringAddConvertLeftDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(StringAddConvertLeftDescriptor)
};

class StringAddConvertRightDescriptor : public StaticCallInterfaceDescriptor<StringAddConvertRightDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StringAddConvertRightDescriptor)
};

class StringCharAtDescriptor : public StaticCallInterfaceDescriptor<StringCharAtDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kPosition)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(StringCharAtDescriptor)
};

class FastNewClosureBaselineDescriptor : public StaticCallInterfaceDescriptor<FastNewClosureBaselineDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kSharedFunctionInfo, kFeedbackCell)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(FastNewClosureBaselineDescriptor)
};

class FastNewFunctionContextEvalDescriptor : public StaticCallInterfaceDescriptor<FastNewFunctionContextEvalDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kScopeInfo, kSlots)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineTypeOf<Uint32T>::value)
  DECLARE_DEFAULT_DESCRIPTOR(FastNewFunctionContextEvalDescriptor)
};

class FastNewFunctionContextFunctionDescriptor : public StaticCallInterfaceDescriptor<FastNewFunctionContextFunctionDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kScopeInfo, kSlots)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineTypeOf<Uint32T>::value)
  DECLARE_DEFAULT_DESCRIPTOR(FastNewFunctionContextFunctionDescriptor)
};

class CreateRegExpLiteralDescriptor : public StaticCallInterfaceDescriptor<CreateRegExpLiteralDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kMaybeFeedbackVector, kSlot, kPattern, kFlags)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(CreateRegExpLiteralDescriptor)
};

class CreateShallowArrayLiteralDescriptor : public StaticCallInterfaceDescriptor<CreateShallowArrayLiteralDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kMaybeFeedbackVector, kSlot, kConstantElements, kFlags)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(CreateShallowArrayLiteralDescriptor)
};

class CreateEmptyArrayLiteralDescriptor : public StaticCallInterfaceDescriptor<CreateEmptyArrayLiteralDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kFeedbackVector, kSlot)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CreateEmptyArrayLiteralDescriptor)
};

class CreateShallowObjectLiteralDescriptor : public StaticCallInterfaceDescriptor<CreateShallowObjectLiteralDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kMaybeFeedbackVector, kSlot, kDesc, kFlags)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(CreateShallowObjectLiteralDescriptor)
};

class CreateEmptyLiteralObjectDescriptor : public StaticCallInterfaceDescriptor<CreateEmptyLiteralObjectDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CreateEmptyLiteralObjectDescriptor)
};

class StringToNumberDescriptor : public StaticCallInterfaceDescriptor<StringToNumberDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(StringToNumberDescriptor)
};

class NonNumberToNumberDescriptor : public StaticCallInterfaceDescriptor<NonNumberToNumberDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(NonNumberToNumberDescriptor)
};

class NonNumberToNumericDescriptor : public StaticCallInterfaceDescriptor<NonNumberToNumericDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(NonNumberToNumericDescriptor)
};

class ToNumericDescriptor : public StaticCallInterfaceDescriptor<ToNumericDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ToNumericDescriptor)
};

class NumberToStringDescriptor : public StaticCallInterfaceDescriptor<NumberToStringDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(NumberToStringDescriptor)
};

class ToBooleanDescriptor : public StaticCallInterfaceDescriptor<ToBooleanDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ToBooleanDescriptor)
};

class ToBooleanForBaselineJumpDescriptor : public StaticCallInterfaceDescriptor<ToBooleanForBaselineJumpDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(2, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedSigned(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ToBooleanForBaselineJumpDescriptor)
};

class ToLengthDescriptor : public StaticCallInterfaceDescriptor<ToLengthDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ToLengthDescriptor)
};

class ToNameDescriptor : public StaticCallInterfaceDescriptor<ToNameDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ToNameDescriptor)
};

class ToObjectDescriptor : public StaticCallInterfaceDescriptor<ToObjectDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ToObjectDescriptor)
};

class NonPrimitiveToPrimitive_DefaultDescriptor : public StaticCallInterfaceDescriptor<NonPrimitiveToPrimitive_DefaultDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(NonPrimitiveToPrimitive_DefaultDescriptor)
};

class NonPrimitiveToPrimitive_NumberDescriptor : public StaticCallInterfaceDescriptor<NonPrimitiveToPrimitive_NumberDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(NonPrimitiveToPrimitive_NumberDescriptor)
};

class NonPrimitiveToPrimitive_StringDescriptor : public StaticCallInterfaceDescriptor<NonPrimitiveToPrimitive_StringDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(NonPrimitiveToPrimitive_StringDescriptor)
};

class OrdinaryToPrimitive_NumberDescriptor : public StaticCallInterfaceDescriptor<OrdinaryToPrimitive_NumberDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(OrdinaryToPrimitive_NumberDescriptor)
};

class OrdinaryToPrimitive_StringDescriptor : public StaticCallInterfaceDescriptor<OrdinaryToPrimitive_StringDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(OrdinaryToPrimitive_StringDescriptor)
};

class IncBlockCounterDescriptor : public StaticCallInterfaceDescriptor<IncBlockCounterDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kFunction, kCoverageArraySlotIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(IncBlockCounterDescriptor)
};

class GetTemplateObjectDescriptor : public StaticCallInterfaceDescriptor<GetTemplateObjectDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kShared, kDescription, kSlot, kMaybeFeedbackVector)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(GetTemplateObjectDescriptor)
};

class BytecodeBudgetInterruptFromCodeDescriptor : public StaticCallInterfaceDescriptor<BytecodeBudgetInterruptFromCodeDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kFeedbackCell)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(BytecodeBudgetInterruptFromCodeDescriptor)
};

class ForInNextDescriptor : public StaticCallInterfaceDescriptor<ForInNextDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSlot, kReceiver, kCacheArray, kCacheType, kCacheIndex, kFeedbackVector)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineTypeOf<UintPtrT>::value, MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedSigned(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(ForInNextDescriptor)
};

class GetImportMetaObjectBaselineDescriptor : public StaticCallInterfaceDescriptor<GetImportMetaObjectBaselineDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(GetImportMetaObjectBaselineDescriptor)
};

class GetIteratorWithFeedbackDescriptor : public StaticCallInterfaceDescriptor<GetIteratorWithFeedbackDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kLoadSlot, kCallSlot, kMaybeFeedbackVector)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(GetIteratorWithFeedbackDescriptor)
};

class GetIteratorBaselineDescriptor : public StaticCallInterfaceDescriptor<GetIteratorBaselineDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kReceiver, kLoadSlot, kCallSlot)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(GetIteratorBaselineDescriptor)
};

class CreateAsyncFromSyncIteratorBaselineDescriptor : public StaticCallInterfaceDescriptor<CreateAsyncFromSyncIteratorBaselineDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kSyncIterator)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CreateAsyncFromSyncIteratorBaselineDescriptor)
};

class CallIteratorWithFeedbackDescriptor : public StaticCallInterfaceDescriptor<CallIteratorWithFeedbackDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kIteratorMethod, kCallSlot, kFeedback)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::TaggedSigned(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(CallIteratorWithFeedbackDescriptor)
};

class ParseIntDescriptor : public StaticCallInterfaceDescriptor<ParseIntDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kInput, kRadix)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ParseIntDescriptor)
};

class AddDescriptor : public StaticCallInterfaceDescriptor<AddDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeftArg, kRightArg)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(AddDescriptor)
};

class SubtractDescriptor : public StaticCallInterfaceDescriptor<SubtractDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(SubtractDescriptor)
};

class MultiplyDescriptor : public StaticCallInterfaceDescriptor<MultiplyDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(MultiplyDescriptor)
};

class DivideDescriptor : public StaticCallInterfaceDescriptor<DivideDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(DivideDescriptor)
};

class ModulusDescriptor : public StaticCallInterfaceDescriptor<ModulusDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ModulusDescriptor)
};

class ExponentiateDescriptor : public StaticCallInterfaceDescriptor<ExponentiateDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ExponentiateDescriptor)
};

class NegateDescriptor : public StaticCallInterfaceDescriptor<NegateDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(NegateDescriptor)
};

class BitwiseNotDescriptor : public StaticCallInterfaceDescriptor<BitwiseNotDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(BitwiseNotDescriptor)
};

class DecrementDescriptor : public StaticCallInterfaceDescriptor<DecrementDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(DecrementDescriptor)
};

class IncrementDescriptor : public StaticCallInterfaceDescriptor<IncrementDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(IncrementDescriptor)
};

class ShiftLeftDescriptor : public StaticCallInterfaceDescriptor<ShiftLeftDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ShiftLeftDescriptor)
};

class ShiftRightDescriptor : public StaticCallInterfaceDescriptor<ShiftRightDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ShiftRightDescriptor)
};

class ShiftRightLogicalDescriptor : public StaticCallInterfaceDescriptor<ShiftRightLogicalDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ShiftRightLogicalDescriptor)
};

class BitwiseAndDescriptor : public StaticCallInterfaceDescriptor<BitwiseAndDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(BitwiseAndDescriptor)
};

class BitwiseOrDescriptor : public StaticCallInterfaceDescriptor<BitwiseOrDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(BitwiseOrDescriptor)
};

class BitwiseXorDescriptor : public StaticCallInterfaceDescriptor<BitwiseXorDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(BitwiseXorDescriptor)
};

class LessThanDescriptor : public StaticCallInterfaceDescriptor<LessThanDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(LessThanDescriptor)
};

class LessThanOrEqualDescriptor : public StaticCallInterfaceDescriptor<LessThanOrEqualDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(LessThanOrEqualDescriptor)
};

class GreaterThanDescriptor : public StaticCallInterfaceDescriptor<GreaterThanDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(GreaterThanDescriptor)
};

class GreaterThanOrEqualDescriptor : public StaticCallInterfaceDescriptor<GreaterThanOrEqualDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(GreaterThanOrEqualDescriptor)
};

class EqualDescriptor : public StaticCallInterfaceDescriptor<EqualDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(EqualDescriptor)
};

class StrictEqualDescriptor : public StaticCallInterfaceDescriptor<StrictEqualDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kLeft, kRight)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StrictEqualDescriptor)
};

class CreateObjectWithoutPropertiesDescriptor : public StaticCallInterfaceDescriptor<CreateObjectWithoutPropertiesDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kPrototype)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CreateObjectWithoutPropertiesDescriptor)
};

class FulfillPromiseDescriptor : public StaticCallInterfaceDescriptor<FulfillPromiseDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kPromise, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(FulfillPromiseDescriptor)
};

class RejectPromiseDescriptor : public StaticCallInterfaceDescriptor<RejectPromiseDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kPromise, kReason, kDebugEvent)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(RejectPromiseDescriptor)
};

class NewPromiseCapabilityDescriptor : public StaticCallInterfaceDescriptor<NewPromiseCapabilityDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kMaybeConstructor, kDebugEvent)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(NewPromiseCapabilityDescriptor)
};

class PerformPromiseThenDescriptor : public StaticCallInterfaceDescriptor<PerformPromiseThenDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kPromise, kOnFulfilled, kOnRejected, kResultPromise)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(PerformPromiseThenDescriptor)
};

class PromiseFulfillReactionJobDescriptor : public StaticCallInterfaceDescriptor<PromiseFulfillReactionJobDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kValue, kHandler, kPromiseOrCapability)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(PromiseFulfillReactionJobDescriptor)
};

class PromiseRejectReactionJobDescriptor : public StaticCallInterfaceDescriptor<PromiseRejectReactionJobDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReason, kHandler, kPromiseOrCapability)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(PromiseRejectReactionJobDescriptor)
};

class PromiseResolveDescriptor : public StaticCallInterfaceDescriptor<PromiseResolveDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kConstructor, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(PromiseResolveDescriptor)
};

class ResolvePromiseDescriptor : public StaticCallInterfaceDescriptor<ResolvePromiseDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kPromise, kResolution)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ResolvePromiseDescriptor)
};

class PromiseResolveThenableJobDescriptor : public StaticCallInterfaceDescriptor<PromiseResolveThenableJobDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kPromiseToResolve, kThenable, kThen)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(PromiseResolveThenableJobDescriptor)
};

class ProxyDeletePropertyDescriptor : public StaticCallInterfaceDescriptor<ProxyDeletePropertyDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kProxy, kName, kLanguageMode)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(ProxyDeletePropertyDescriptor)
};

class ProxyGetPropertyDescriptor : public StaticCallInterfaceDescriptor<ProxyGetPropertyDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kProxy, kName, kReceiverValue, kOnNonExistent)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(ProxyGetPropertyDescriptor)
};

class ProxyGetPrototypeOfDescriptor : public StaticCallInterfaceDescriptor<ProxyGetPrototypeOfDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kProxy)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(ProxyGetPrototypeOfDescriptor)
};

class ProxyHasPropertyDescriptor : public StaticCallInterfaceDescriptor<ProxyHasPropertyDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kProxy, kName)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(ProxyHasPropertyDescriptor)
};

class ProxyIsExtensibleDescriptor : public StaticCallInterfaceDescriptor<ProxyIsExtensibleDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kProxy)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(ProxyIsExtensibleDescriptor)
};

class ProxyPreventExtensionsDescriptor : public StaticCallInterfaceDescriptor<ProxyPreventExtensionsDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kProxy, kDoThrow)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(ProxyPreventExtensionsDescriptor)
};

class ProxySetPropertyDescriptor : public StaticCallInterfaceDescriptor<ProxySetPropertyDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kProxy, kName, kValue, kReceiverValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(ProxySetPropertyDescriptor)
};

class ProxySetPrototypeOfDescriptor : public StaticCallInterfaceDescriptor<ProxySetPrototypeOfDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kProxy, kProto, kDoThrow)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(ProxySetPrototypeOfDescriptor)
};

class RegExpPrototypeExecSlowDescriptor : public StaticCallInterfaceDescriptor<RegExpPrototypeExecSlowDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kRegexp, kString)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(RegExpPrototypeExecSlowDescriptor)
};

class RegExpMatchFastDescriptor : public StaticCallInterfaceDescriptor<RegExpMatchFastDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kString)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(RegExpMatchFastDescriptor)
};

class RegExpReplaceDescriptor : public StaticCallInterfaceDescriptor<RegExpReplaceDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kRegexp, kString, kReplaceValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(RegExpReplaceDescriptor)
};

class RegExpSearchFastDescriptor : public StaticCallInterfaceDescriptor<RegExpSearchFastDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kString)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(RegExpSearchFastDescriptor)
};

class RegExpSplitDescriptor : public StaticCallInterfaceDescriptor<RegExpSplitDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kRegexp, kString, kLimit)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(RegExpSplitDescriptor)
};

class RegExpPrototypeTestFastDescriptor : public StaticCallInterfaceDescriptor<RegExpPrototypeTestFastDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kString)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(RegExpPrototypeTestFastDescriptor)
};

class CreateHTMLDescriptor : public StaticCallInterfaceDescriptor<CreateHTMLDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kMethodName, kTagName, kAttr, kAttrValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CreateHTMLDescriptor)
};

class StringRepeatDescriptor : public StaticCallInterfaceDescriptor<StringRepeatDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kString, kCount)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(StringRepeatDescriptor)
};

class CreateTypedArrayDescriptor : public StaticCallInterfaceDescriptor<CreateTypedArrayDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTarget, kNewTarget, kArg1, kArg2, kArg3)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CreateTypedArrayDescriptor)
};

class TypedArrayMergeSortDescriptor : public StaticCallInterfaceDescriptor<TypedArrayMergeSortDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSource, kFrom, kTo, kTarget, kArray, kComparefn)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineTypeOf<UintPtrT>::value, MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(TypedArrayMergeSortDescriptor)
};

class NewSloppyArgumentsElementsDescriptor : public StaticCallInterfaceDescriptor<NewSloppyArgumentsElementsDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kFrame, kFormalParameterCount, kArgumentCount)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineTypeOf<RawPtrT>::value, MachineTypeOf<IntPtrT>::value, MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(NewSloppyArgumentsElementsDescriptor)
};

class NewStrictArgumentsElementsDescriptor : public StaticCallInterfaceDescriptor<NewStrictArgumentsElementsDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kFrame, kFormalParameterCount, kArgumentCount)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineTypeOf<RawPtrT>::value, MachineTypeOf<IntPtrT>::value, MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(NewStrictArgumentsElementsDescriptor)
};

class NewRestArgumentsElementsDescriptor : public StaticCallInterfaceDescriptor<NewRestArgumentsElementsDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kFrame, kFormalParameterCount, kArgumentCount)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineTypeOf<RawPtrT>::value, MachineTypeOf<IntPtrT>::value, MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(NewRestArgumentsElementsDescriptor)
};

class FastNewSloppyArgumentsDescriptor : public StaticCallInterfaceDescriptor<FastNewSloppyArgumentsDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kF)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(FastNewSloppyArgumentsDescriptor)
};

class FastNewStrictArgumentsDescriptor : public StaticCallInterfaceDescriptor<FastNewStrictArgumentsDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kF)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(FastNewStrictArgumentsDescriptor)
};

class FastNewRestArgumentsDescriptor : public StaticCallInterfaceDescriptor<FastNewRestArgumentsDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kF)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(FastNewRestArgumentsDescriptor)
};

class StringSlowFlattenDescriptor : public StaticCallInterfaceDescriptor<StringSlowFlattenDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kCons)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(StringSlowFlattenDescriptor)
};

class StringIndexOfDescriptor : public StaticCallInterfaceDescriptor<StringIndexOfDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kS, kSearchString, kStart)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(StringIndexOfDescriptor)
};

class GenericBuiltinTest_JSAny_0Descriptor : public StaticCallInterfaceDescriptor<GenericBuiltinTest_JSAny_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kParam)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(GenericBuiltinTest_JSAny_0Descriptor)
};

class TestHelperPlus1Descriptor : public StaticCallInterfaceDescriptor<TestHelperPlus1Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kX)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(TestHelperPlus1Descriptor)
};

class TestHelperPlus2Descriptor : public StaticCallInterfaceDescriptor<TestHelperPlus2Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kX)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(TestHelperPlus2Descriptor)
};

class NewSmiBoxDescriptor : public StaticCallInterfaceDescriptor<NewSmiBoxDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(NewSmiBoxDescriptor)
};

class ReturnTwoValuesDescriptor : public StaticCallInterfaceDescriptor<ReturnTwoValuesDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(2, kValue, kObj)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(ReturnTwoValuesDescriptor)
};

class Load_FastSmiElements_0Descriptor : public StaticCallInterfaceDescriptor<Load_FastSmiElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(Load_FastSmiElements_0Descriptor)
};

class Load_FastObjectElements_0Descriptor : public StaticCallInterfaceDescriptor<Load_FastObjectElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(Load_FastObjectElements_0Descriptor)
};

class Load_FastDoubleElements_0Descriptor : public StaticCallInterfaceDescriptor<Load_FastDoubleElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(Load_FastDoubleElements_0Descriptor)
};

class Store_FastSmiElements_0Descriptor : public StaticCallInterfaceDescriptor<Store_FastSmiElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(Store_FastSmiElements_0Descriptor)
};

class Store_FastObjectElements_0Descriptor : public StaticCallInterfaceDescriptor<Store_FastObjectElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(Store_FastObjectElements_0Descriptor)
};

class Store_FastDoubleElements_0Descriptor : public StaticCallInterfaceDescriptor<Store_FastDoubleElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(Store_FastDoubleElements_0Descriptor)
};

class Delete_FastSmiElements_0Descriptor : public StaticCallInterfaceDescriptor<Delete_FastSmiElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(Delete_FastSmiElements_0Descriptor)
};

class Delete_FastObjectElements_0Descriptor : public StaticCallInterfaceDescriptor<Delete_FastObjectElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(Delete_FastObjectElements_0Descriptor)
};

class Delete_FastDoubleElements_0Descriptor : public StaticCallInterfaceDescriptor<Delete_FastDoubleElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(Delete_FastDoubleElements_0Descriptor)
};

class SortCompareDefaultDescriptor : public StaticCallInterfaceDescriptor<SortCompareDefaultDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kComparefn, kX, kY)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(SortCompareDefaultDescriptor)
};

class SortCompareUserFnDescriptor : public StaticCallInterfaceDescriptor<SortCompareUserFnDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kComparefn, kX, kY)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(SortCompareUserFnDescriptor)
};

class CanUseSameAccessor_GenericElementsAccessor_0Descriptor : public StaticCallInterfaceDescriptor<CanUseSameAccessor_GenericElementsAccessor_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kInitialReceiverMap, kInitialReceiverLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CanUseSameAccessor_GenericElementsAccessor_0Descriptor)
};

class CopyDescriptor : public StaticCallInterfaceDescriptor<CopyDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSource, kSrcPos, kTarget, kDstPos, kLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(CopyDescriptor)
};

class MergeAtDescriptor : public StaticCallInterfaceDescriptor<MergeAtDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kI)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(MergeAtDescriptor)
};

class GallopLeftDescriptor : public StaticCallInterfaceDescriptor<GallopLeftDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kArray, kKey, kBase, kLength, kHint)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedSigned(), MachineType::TaggedSigned(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(GallopLeftDescriptor)
};

class GallopRightDescriptor : public StaticCallInterfaceDescriptor<GallopRightDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kArray, kKey, kBase, kLength, kHint)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged(), MachineType::TaggedSigned(), MachineType::TaggedSigned(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(GallopRightDescriptor)
};

class ArrayTimSortDescriptor : public StaticCallInterfaceDescriptor<ArrayTimSortDescriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer())
  DECLARE_DEFAULT_DESCRIPTOR(ArrayTimSortDescriptor)
};

class LoadJoinElement_GenericElementsAccessor_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinElement_GenericElementsAccessor_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinElement_GenericElementsAccessor_0Descriptor)
};

class LoadJoinTypedElement_Int32Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_Int32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_Int32Elements_0Descriptor)
};

class LoadJoinTypedElement_Float32Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_Float32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_Float32Elements_0Descriptor)
};

class LoadJoinTypedElement_Float64Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_Float64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_Float64Elements_0Descriptor)
};

class LoadJoinTypedElement_Uint8ClampedElements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_Uint8ClampedElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_Uint8ClampedElements_0Descriptor)
};

class LoadJoinTypedElement_BigUint64Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_BigUint64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_BigUint64Elements_0Descriptor)
};

class LoadJoinTypedElement_BigInt64Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_BigInt64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_BigInt64Elements_0Descriptor)
};

class LoadJoinTypedElement_Uint8Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_Uint8Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_Uint8Elements_0Descriptor)
};

class LoadJoinTypedElement_Int8Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_Int8Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_Int8Elements_0Descriptor)
};

class LoadJoinTypedElement_Uint16Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_Uint16Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_Uint16Elements_0Descriptor)
};

class LoadJoinTypedElement_Int16Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_Int16Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_Int16Elements_0Descriptor)
};

class LoadJoinTypedElement_Uint32Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadJoinTypedElement_Uint32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kK)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadJoinTypedElement_Uint32Elements_0Descriptor)
};

class GenericBuiltinTest_Smi_0Descriptor : public StaticCallInterfaceDescriptor<GenericBuiltinTest_Smi_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kParam)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(GenericBuiltinTest_Smi_0Descriptor)
};

class CanUseSameAccessor_FastDoubleElements_0Descriptor : public StaticCallInterfaceDescriptor<CanUseSameAccessor_FastDoubleElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kInitialReceiverMap, kInitialReceiverLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CanUseSameAccessor_FastDoubleElements_0Descriptor)
};

class CanUseSameAccessor_FastSmiElements_0Descriptor : public StaticCallInterfaceDescriptor<CanUseSameAccessor_FastSmiElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kInitialReceiverMap, kInitialReceiverLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CanUseSameAccessor_FastSmiElements_0Descriptor)
};

class CanUseSameAccessor_FastObjectElements_0Descriptor : public StaticCallInterfaceDescriptor<CanUseSameAccessor_FastObjectElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kReceiver, kInitialReceiverMap, kInitialReceiverLength)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::TaggedPointer(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(CanUseSameAccessor_FastObjectElements_0Descriptor)
};

class Load_GenericElementsAccessor_0Descriptor : public StaticCallInterfaceDescriptor<Load_GenericElementsAccessor_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(Load_GenericElementsAccessor_0Descriptor)
};

class Store_GenericElementsAccessor_0Descriptor : public StaticCallInterfaceDescriptor<Store_GenericElementsAccessor_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned(), MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(Store_GenericElementsAccessor_0Descriptor)
};

class Delete_GenericElementsAccessor_0Descriptor : public StaticCallInterfaceDescriptor<Delete_GenericElementsAccessor_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kSortState, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineType::TaggedSigned())
  DECLARE_DEFAULT_DESCRIPTOR(Delete_GenericElementsAccessor_0Descriptor)
};

class LoadTypedElement_Int32Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_Int32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_Int32Elements_0Descriptor)
};

class StoreTypedElementNumeric_Int32Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_Int32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_Int32Elements_0Descriptor)
};

class StoreTypedElementJSAny_Int32Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_Int32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_Int32Elements_0Descriptor)
};

class LoadTypedElement_Float32Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_Float32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_Float32Elements_0Descriptor)
};

class StoreTypedElementNumeric_Float32Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_Float32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_Float32Elements_0Descriptor)
};

class StoreTypedElementJSAny_Float32Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_Float32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_Float32Elements_0Descriptor)
};

class LoadTypedElement_Float64Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_Float64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_Float64Elements_0Descriptor)
};

class StoreTypedElementNumeric_Float64Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_Float64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_Float64Elements_0Descriptor)
};

class StoreTypedElementJSAny_Float64Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_Float64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_Float64Elements_0Descriptor)
};

class LoadTypedElement_Uint8ClampedElements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_Uint8ClampedElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_Uint8ClampedElements_0Descriptor)
};

class StoreTypedElementNumeric_Uint8ClampedElements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_Uint8ClampedElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_Uint8ClampedElements_0Descriptor)
};

class StoreTypedElementJSAny_Uint8ClampedElements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_Uint8ClampedElements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_Uint8ClampedElements_0Descriptor)
};

class LoadTypedElement_BigUint64Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_BigUint64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_BigUint64Elements_0Descriptor)
};

class StoreTypedElementNumeric_BigUint64Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_BigUint64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_BigUint64Elements_0Descriptor)
};

class StoreTypedElementJSAny_BigUint64Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_BigUint64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_BigUint64Elements_0Descriptor)
};

class LoadTypedElement_BigInt64Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_BigInt64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_BigInt64Elements_0Descriptor)
};

class StoreTypedElementNumeric_BigInt64Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_BigInt64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_BigInt64Elements_0Descriptor)
};

class StoreTypedElementJSAny_BigInt64Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_BigInt64Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_BigInt64Elements_0Descriptor)
};

class LoadTypedElement_Uint8Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_Uint8Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_Uint8Elements_0Descriptor)
};

class StoreTypedElementNumeric_Uint8Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_Uint8Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_Uint8Elements_0Descriptor)
};

class StoreTypedElementJSAny_Uint8Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_Uint8Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_Uint8Elements_0Descriptor)
};

class LoadTypedElement_Int8Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_Int8Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_Int8Elements_0Descriptor)
};

class StoreTypedElementNumeric_Int8Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_Int8Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_Int8Elements_0Descriptor)
};

class StoreTypedElementJSAny_Int8Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_Int8Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_Int8Elements_0Descriptor)
};

class LoadTypedElement_Uint16Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_Uint16Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_Uint16Elements_0Descriptor)
};

class StoreTypedElementNumeric_Uint16Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_Uint16Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_Uint16Elements_0Descriptor)
};

class StoreTypedElementJSAny_Uint16Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_Uint16Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_Uint16Elements_0Descriptor)
};

class LoadTypedElement_Int16Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_Int16Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_Int16Elements_0Descriptor)
};

class StoreTypedElementNumeric_Int16Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_Int16Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_Int16Elements_0Descriptor)
};

class StoreTypedElementJSAny_Int16Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_Int16Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_Int16Elements_0Descriptor)
};

class LoadTypedElement_Uint32Elements_0Descriptor : public StaticCallInterfaceDescriptor<LoadTypedElement_Uint32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS_NO_CONTEXT(1, kArray, kIndex)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::AnyTagged(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value)
  DECLARE_DEFAULT_DESCRIPTOR(LoadTypedElement_Uint32Elements_0Descriptor)
};

class StoreTypedElementNumeric_Uint32Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementNumeric_Uint32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementNumeric_Uint32Elements_0Descriptor)
};

class StoreTypedElementJSAny_Uint32Elements_0Descriptor : public StaticCallInterfaceDescriptor<StoreTypedElementJSAny_Uint32Elements_0Descriptor> {
 public:
  DEFINE_RESULT_AND_PARAMETERS(1, kTypedArray, kIndex, kValue)
  DEFINE_RESULT_AND_PARAMETER_TYPES(MachineType::TaggedSigned(), MachineType::TaggedPointer(), MachineTypeOf<UintPtrT>::value, MachineType::AnyTagged())
  DECLARE_DEFAULT_DESCRIPTOR(StoreTypedElementJSAny_Uint32Elements_0Descriptor)
};

