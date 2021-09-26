#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SCOPE_INFO_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SCOPE_INFO_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<ScopeInfo> kEmptyScopeInfo_0(compiler::CodeAssemblerState* state_);
TNode<Uint32T> FromConstexpr_ScopeType_constexpr_CLASS_SCOPE_0(compiler::CodeAssemblerState* state_, ScopeType p_o);
TNode<Uint32T> FromConstexpr_ScopeType_constexpr_EVAL_SCOPE_0(compiler::CodeAssemblerState* state_, ScopeType p_o);
TNode<Uint32T> FromConstexpr_ScopeType_constexpr_FUNCTION_SCOPE_0(compiler::CodeAssemblerState* state_, ScopeType p_o);
TNode<Uint32T> FromConstexpr_ScopeType_constexpr_MODULE_SCOPE_0(compiler::CodeAssemblerState* state_, ScopeType p_o);
TNode<Uint32T> FromConstexpr_ScopeType_constexpr_SCRIPT_SCOPE_0(compiler::CodeAssemblerState* state_, ScopeType p_o);
TNode<Uint32T> FromConstexpr_ScopeType_constexpr_CATCH_SCOPE_0(compiler::CodeAssemblerState* state_, ScopeType p_o);
TNode<Uint32T> FromConstexpr_ScopeType_constexpr_BLOCK_SCOPE_0(compiler::CodeAssemblerState* state_, ScopeType p_o);
TNode<Uint32T> FromConstexpr_ScopeType_constexpr_WITH_SCOPE_0(compiler::CodeAssemblerState* state_, ScopeType p_o);
TNode<Uint32T> FromConstexpr_VariableAllocationInfo_constexpr_NONE_0(compiler::CodeAssemblerState* state_, VariableAllocationInfo p_o);
TNode<Uint32T> FromConstexpr_VariableAllocationInfo_constexpr_STACK_0(compiler::CodeAssemblerState* state_, VariableAllocationInfo p_o);
TNode<Uint32T> FromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0(compiler::CodeAssemblerState* state_, VariableAllocationInfo p_o);
TNode<Uint32T> FromConstexpr_VariableAllocationInfo_constexpr_UNUSED_0(compiler::CodeAssemblerState* state_, VariableAllocationInfo p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kLet_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kConst_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kVar_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kTemporary_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kDynamic_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kDynamicGlobal_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kDynamicLocal_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kPrivateMethod_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kPrivateSetterOnly_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kPrivateGetterOnly_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_VariableMode_constexpr_kPrivateGetterAndSetter_0(compiler::CodeAssemblerState* state_, VariableMode p_o);
TNode<Uint32T> FromConstexpr_InitializationFlag_constexpr_kNeedsInitialization_0(compiler::CodeAssemblerState* state_, InitializationFlag p_o);
TNode<Uint32T> FromConstexpr_InitializationFlag_constexpr_kCreatedInitialized_0(compiler::CodeAssemblerState* state_, InitializationFlag p_o);
TNode<Uint32T> FromConstexpr_IsStaticFlag_constexpr_kNotStatic_0(compiler::CodeAssemblerState* state_, IsStaticFlag p_o);
TNode<Uint32T> FromConstexpr_IsStaticFlag_constexpr_kStatic_0(compiler::CodeAssemblerState* state_, IsStaticFlag p_o);
TNode<Uint32T> FromConstexpr_MaybeAssignedFlag_constexpr_kNotAssigned_0(compiler::CodeAssemblerState* state_, MaybeAssignedFlag p_o);
TNode<Uint32T> FromConstexpr_MaybeAssignedFlag_constexpr_kMaybeAssigned_0(compiler::CodeAssemblerState* state_, MaybeAssignedFlag p_o);
TNode<ScopeInfo> Cast_ScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<IntPtrT> IndexOfLocalName_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_scopeInfo, TNode<Name> p_name, compiler::CodeAssemblerLabel* label_NotFound);
TNode<Smi> LoadScopeInfoFlags_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<Smi> LoadScopeInfoParameterCount_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
void StoreScopeInfoParameterCount_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<Smi> p_v);
TNode<Smi> LoadScopeInfoContextLocalCount_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TorqueStructSlice_String_MutableReference_String_0 FieldSliceScopeInfoContextLocalNames_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<String> LoadScopeInfoContextLocalNames_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<IntPtrT> p_i);
void StoreScopeInfoContextLocalNames_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<IntPtrT> p_i, TNode<String> p_v);
TorqueStructSlice_SmiTagged_VariableProperties_MutableReference_SmiTagged_VariableProperties_0 FieldSliceScopeInfoContextLocalInfos_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<Smi> LoadScopeInfoContextLocalInfos_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<IntPtrT> p_i);
void StoreScopeInfoContextLocalInfos_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<IntPtrT> p_i, TNode<Smi> p_v);
TorqueStructSlice_Smi_MutableReference_Smi_0 FieldSliceScopeInfoSavedClassVariableInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<Smi> LoadScopeInfoSavedClassVariableInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
void StoreScopeInfoSavedClassVariableInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<Smi> p_v);
TorqueStructSlice_Smi_MutableReference_Smi_0 FieldSliceScopeInfoReceiverInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<Smi> LoadScopeInfoReceiverInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
void StoreScopeInfoReceiverInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<Smi> p_v);
TorqueStructSlice_FunctionVariableInfo_MutableReference_FunctionVariableInfo_0 FieldSliceScopeInfoFunctionVariableInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TorqueStructSlice_String_OR_Undefined_MutableReference_String_OR_Undefined_0 FieldSliceScopeInfoInferredFunctionName_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<PrimitiveHeapObject> LoadScopeInfoInferredFunctionName_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
void StoreScopeInfoInferredFunctionName_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<PrimitiveHeapObject> p_v);
TorqueStructSlice_PositionInfo_MutableReference_PositionInfo_0 FieldSliceScopeInfoPositionInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TorqueStructSlice_TheHole_OR_ScopeInfo_MutableReference_TheHole_OR_ScopeInfo_0 FieldSliceScopeInfoOuterScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<HeapObject> LoadScopeInfoOuterScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
void StoreScopeInfoOuterScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<HeapObject> p_v);
TorqueStructSlice_HashTable_MutableReference_HashTable_0 FieldSliceScopeInfoLocalsBlockList_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<FixedArray> LoadScopeInfoLocalsBlockList_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
void StoreScopeInfoLocalsBlockList_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<FixedArray> p_v);
TorqueStructSlice_SourceTextModuleInfo_MutableReference_SourceTextModuleInfo_0 FieldSliceScopeInfoModuleInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<FixedArray> LoadScopeInfoModuleInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
void StoreScopeInfoModuleInfo_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o, TNode<FixedArray> p_v);
TorqueStructSlice_Smi_ConstReference_Smi_0 FieldSliceScopeInfoModuleVariableCount_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<Smi> LoadScopeInfoModuleVariableCount_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TorqueStructSlice_ModuleVariable_MutableReference_ModuleVariable_0 FieldSliceScopeInfoModuleVariables_0(compiler::CodeAssemblerState* state_, TNode<ScopeInfo> p_o);
TNode<ScopeInfo> DownCastForTorqueClass_ScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TorqueStructSlice_String_MutableReference_String_0 NewMutableSlice_String_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_SmiTagged_VariableProperties_MutableReference_SmiTagged_VariableProperties_0 NewMutableSlice_SmiTagged_VariableProperties_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_Smi_MutableReference_Smi_0 NewMutableSlice_Smi_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_FunctionVariableInfo_MutableReference_FunctionVariableInfo_0 NewMutableSlice_FunctionVariableInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_String_OR_Undefined_MutableReference_String_OR_Undefined_0 NewMutableSlice_String_OR_Undefined_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_PositionInfo_MutableReference_PositionInfo_0 NewMutableSlice_PositionInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_TheHole_OR_ScopeInfo_MutableReference_TheHole_OR_ScopeInfo_0 NewMutableSlice_TheHole_OR_ScopeInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_HashTable_MutableReference_HashTable_0 NewMutableSlice_HashTable_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_SourceTextModuleInfo_MutableReference_SourceTextModuleInfo_0 NewMutableSlice_SourceTextModuleInfo_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_Smi_ConstReference_Smi_0 NewConstSlice_Smi_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
TorqueStructSlice_ModuleVariable_MutableReference_ModuleVariable_0 NewMutableSlice_ModuleVariable_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SCOPE_INFO_TQ_CSA_H_
