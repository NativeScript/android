#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_TORQUE_INTERNAL_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_TORQUE_INTERNAL_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<IntPtrT> AddIndexedFieldSizeToObjectSize_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_baseSize, TNode<IntPtrT> p_arrayLength, int32_t p_fieldSize);
TNode<IntPtrT> AlignTagged_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_x);
TNode<BoolT> IsTaggedAligned_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_x);
TNode<BoolT> ValidAllocationSize_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_sizeInBytes, TNode<Map> p_map);
CodeStubAssembler::AllocationFlag kAllocateBaseFlags_0(compiler::CodeAssemblerState* state_);
TNode<HeapObject> AllocateFromNew_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_sizeInBytes, TNode<Map> p_map, TNode<BoolT> p_pretenured);
void InitializeFieldsFromIterator_char8_UninitializedIterator_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char8_MutableReference_char8_0 p__target, TorqueStructUninitializedIterator_0 p__originIterator);
void InitializeFieldsFromIterator_char16_UninitializedIterator_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char16_MutableReference_char16_0 p__target, TorqueStructUninitializedIterator_0 p__originIterator);
TorqueStructfloat64_or_hole_0 LoadFloat64OrHole_0(compiler::CodeAssemblerState* state_, TorqueStructReference_float64_or_hole_0 p_r);
void StoreFloat64OrHole_0(compiler::CodeAssemblerState* state_, TorqueStructReference_float64_or_hole_0 p_r, TorqueStructfloat64_or_hole_0 p_value);
TNode<IntPtrT> TimesSizeOf_Object_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_Object_0 NewReference_Object_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_float64_or_hole_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_float64_or_hole_0 NewReference_float64_or_hole_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> Convert_intptr_constexpr_int31_0(compiler::CodeAssemblerState* state_, int31_t p_i);
TNode<IntPtrT> TimesSizeOf_Smi_OR_TheHole_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_Smi_OR_TheHole_0 NewReference_Smi_OR_TheHole_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_DescriptorEntry_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_DescriptorEntry_0 NewReference_DescriptorEntry_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_CoverageInfoSlot_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_CoverageInfoSlot_0 NewReference_CoverageInfoSlot_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_char8_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_char8_0 NewReference_char8_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_float64_0 NewReference_float64_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_String_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_String_0 NewReference_String_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_Smi_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_Smi_0 NewReference_Smi_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_InternalClassStructElement_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_InternalClassStructElement_0 NewReference_InternalClassStructElement_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_uint8_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_uint8_0 NewReference_uint8_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_MaybeObject_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_MaybeObject_0 NewReference_MaybeObject_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_SmiTagged_VariableProperties_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_SmiTagged_VariableProperties_0 NewReference_SmiTagged_VariableProperties_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_String_OR_Undefined_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_String_OR_Undefined_0 NewReference_String_OR_Undefined_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_TheHole_OR_ScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_TheHole_OR_ScopeInfo_0 NewReference_TheHole_OR_ScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_HashTable_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_HashTable_0 NewReference_HashTable_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_SourceTextModuleInfo_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_SourceTextModuleInfo_0 NewReference_SourceTextModuleInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0 NewReference_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_char16_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_char16_0 NewReference_char16_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_FunctionVariableInfo_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_FunctionVariableInfo_0 NewReference_FunctionVariableInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_PositionInfo_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_PositionInfo_0 NewReference_PositionInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_ModuleVariable_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_ModuleVariable_0 NewReference_ModuleVariable_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_HashMapEntry_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_HashMapEntry_0 NewReference_HashMapEntry_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<IntPtrT> TimesSizeOf_NameDictionaryEntry_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_i);
TorqueStructReference_NameDictionaryEntry_0 NewReference_NameDictionaryEntry_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<RawPtrT> Convert_RawPtr_RawPtr_char8_0(compiler::CodeAssemblerState* state_, TNode<RawPtrT> p_i);
TNode<RawPtrT> Convert_RawPtr_RawPtr_char16_0(compiler::CodeAssemblerState* state_, TNode<RawPtrT> p_i);
TorqueStructReference_JSPromise_0 NewReference_JSPromise_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_Boolean_0 NewReference_Boolean_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_PromiseCapability_0 NewReference_PromiseCapability_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<PromiseCapability> UnsafeCast_PromiseCapability_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TorqueStructReference_Map_0 NewReference_Map_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_FixedArray_0 NewReference_FixedArray_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_JSAny_0 NewReference_JSAny_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_Callable_0 NewReference_Callable_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_Constructor_0 NewReference_Constructor_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_HeapObject_0 NewReference_HeapObject_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_Null_OR_JSProxy_0 NewReference_Null_OR_JSProxy_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<HeapObject> UnsafeCast_Null_OR_JSProxy_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TorqueStructReference_ScopeInfo_0 NewReference_ScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<ScopeInfo> UnsafeCast_ScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TorqueStructReference_JSFunction_0 NewReference_JSFunction_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<JSFunction> UnsafeCast_JSFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TorqueStructReference_JSObject_0 NewReference_JSObject_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_Undefined_OR_FixedArray_0 NewReference_Undefined_OR_FixedArray_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<HeapObject> UnsafeCast_Undefined_OR_FixedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TorqueStructReference_RegExpMatchInfo_0 NewReference_RegExpMatchInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_FixedDoubleArray_0 NewReference_FixedDoubleArray_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TorqueStructReference_Undefined_OR_CallableApiObject_OR_CallableJSProxy_OR_JSFunction_OR_JSBoundFunction_0 NewReference_Undefined_OR_CallableApiObject_OR_CallableJSProxy_OR_JSFunction_OR_JSBoundFunction_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset);
TNode<HeapObject> UnsafeCast_Undefined_OR_CallableApiObject_OR_CallableJSProxy_OR_JSFunction_OR_JSBoundFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_TORQUE_INTERNAL_TQ_CSA_H_
