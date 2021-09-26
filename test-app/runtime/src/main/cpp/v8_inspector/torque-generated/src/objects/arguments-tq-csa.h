#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_ARGUMENTS_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_ARGUMENTS_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<JSArgumentsObject> Cast_JSArgumentsObject_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<BoolT> IsJSArgumentsObjectWithLength_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TNode<SloppyArgumentsElements> Cast_SloppyArgumentsElements_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<AliasedArgumentsEntry> Cast_AliasedArgumentsEntry_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<JSArgumentsObject> NewJSStrictArgumentsObject_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FixedArray> p_elements);
TNode<JSArgumentsObject> NewJSSloppyArgumentsObject_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FixedArrayBase> p_elements, TNode<JSFunction> p_callee);
TNode<JSArgumentsObject> NewJSFastAliasedArgumentsObject_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FixedArrayBase> p_elements, TNode<Smi> p_length, TNode<JSFunction> p_callee);
TorqueStructParameterMapIterator_0 NewParameterMapIterator_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_formalParameterCount, TNode<IntPtrT> p_mappedCount);
TorqueStructParameterValueIterator_0 NewParameterValueIterator_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_mappedCount, TorqueStructArguments p_arguments);
TNode<JSArray> NewAllArguments_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<RawPtrT> p_frame, TNode<IntPtrT> p_argumentCount);
TNode<FixedArray> NewRestArgumentsElements_0(compiler::CodeAssemblerState* state_, TNode<RawPtrT> p_frame, TNode<IntPtrT> p_formalParameterCount, TNode<IntPtrT> p_argumentCount);
TNode<JSArray> NewRestArguments_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TorqueStructFrameWithArgumentsInfo_0 p_info);
TNode<FixedArray> NewStrictArgumentsElements_0(compiler::CodeAssemblerState* state_, TNode<RawPtrT> p_frame, TNode<IntPtrT> p_argumentCount);
TNode<JSArgumentsObject> NewStrictArguments_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TorqueStructFrameWithArgumentsInfo_0 p_info);
TNode<FixedArray> NewSloppyArgumentsElements_0(compiler::CodeAssemblerState* state_, TNode<RawPtrT> p_frame, TNode<IntPtrT> p_formalParameterCount, TNode<IntPtrT> p_argumentCount);
TNode<JSArgumentsObject> NewSloppyArguments_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TorqueStructFrameWithArgumentsInfo_0 p_info, TNode<JSFunction> p_callee);
TNode<JSArray> EmitFastNewAllArguments_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<RawPtrT> p_frame, TNode<IntPtrT> p_argc);
TNode<JSArray> EmitFastNewRestArguments_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSFunction> p__f);
TNode<JSArgumentsObject> EmitFastNewStrictArguments_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSFunction> p__f);
TNode<JSArgumentsObject> EmitFastNewSloppyArguments_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSFunction> p_f);
TorqueStructReference_Object_0 AccessSloppyArgumentsCommon_0(compiler::CodeAssemblerState* state_, TNode<JSObject> p_receiver, TNode<Object> p_keyObject, compiler::CodeAssemblerLabel* label_Bailout);
TNode<Object> SloppyArgumentsLoad_0(compiler::CodeAssemblerState* state_, TNode<JSObject> p_receiver, TNode<Object> p_keyObject, compiler::CodeAssemblerLabel* label_Bailout);
TNode<Object> SloppyArgumentsHas_0(compiler::CodeAssemblerState* state_, TNode<JSObject> p_receiver, TNode<Object> p_keyObject, compiler::CodeAssemblerLabel* label_Bailout);
TNode<Object> SloppyArgumentsStore_0(compiler::CodeAssemblerState* state_, TNode<JSObject> p_receiver, TNode<Object> p_keyObject, TNode<Object> p_value, compiler::CodeAssemblerLabel* label_Bailout);
TNode<Object> LoadJSSloppyArgumentsObjectLength_0(compiler::CodeAssemblerState* state_, TNode<JSArgumentsObject> p_o);
void StoreJSSloppyArgumentsObjectLength_0(compiler::CodeAssemblerState* state_, TNode<JSArgumentsObject> p_o, TNode<Object> p_v);
TNode<Object> LoadJSSloppyArgumentsObjectCallee_0(compiler::CodeAssemblerState* state_, TNode<JSArgumentsObject> p_o);
void StoreJSSloppyArgumentsObjectCallee_0(compiler::CodeAssemblerState* state_, TNode<JSArgumentsObject> p_o, TNode<Object> p_v);
TNode<Object> LoadJSStrictArgumentsObjectLength_0(compiler::CodeAssemblerState* state_, TNode<JSArgumentsObject> p_o);
void StoreJSStrictArgumentsObjectLength_0(compiler::CodeAssemblerState* state_, TNode<JSArgumentsObject> p_o, TNode<Object> p_v);
TNode<Context> LoadSloppyArgumentsElementsContext_0(compiler::CodeAssemblerState* state_, TNode<SloppyArgumentsElements> p_o);
void StoreSloppyArgumentsElementsContext_0(compiler::CodeAssemblerState* state_, TNode<SloppyArgumentsElements> p_o, TNode<Context> p_v);
TNode<FixedArray> LoadSloppyArgumentsElementsArguments_0(compiler::CodeAssemblerState* state_, TNode<SloppyArgumentsElements> p_o);
void StoreSloppyArgumentsElementsArguments_0(compiler::CodeAssemblerState* state_, TNode<SloppyArgumentsElements> p_o, TNode<FixedArray> p_v);
TorqueStructSlice_Smi_OR_TheHole_MutableReference_Smi_OR_TheHole_0 FieldSliceSloppyArgumentsElementsMappedEntries_0(compiler::CodeAssemblerState* state_, TNode<SloppyArgumentsElements> p_o);
TNode<Object> LoadSloppyArgumentsElementsMappedEntries_0(compiler::CodeAssemblerState* state_, TNode<SloppyArgumentsElements> p_o, TNode<IntPtrT> p_i);
void StoreSloppyArgumentsElementsMappedEntries_0(compiler::CodeAssemblerState* state_, TNode<SloppyArgumentsElements> p_o, TNode<IntPtrT> p_i, TNode<Object> p_v);
TNode<Smi> LoadAliasedArgumentsEntryAliasedContextSlot_0(compiler::CodeAssemblerState* state_, TNode<AliasedArgumentsEntry> p_o);
void StoreAliasedArgumentsEntryAliasedContextSlot_0(compiler::CodeAssemblerState* state_, TNode<AliasedArgumentsEntry> p_o, TNode<Smi> p_v);
TNode<JSArgumentsObject> DownCastForTorqueClass_JSArgumentsObject_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<BoolT> Is_JSArgumentsObjectWithLength_Object_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TNode<SloppyArgumentsElements> DownCastForTorqueClass_SloppyArgumentsElements_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<AliasedArgumentsEntry> DownCastForTorqueClass_AliasedArgumentsEntry_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<FixedArray> NewFixedArray_ParameterValueIterator_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_length, TorqueStructParameterValueIterator_0 p_it);
TNode<SloppyArgumentsElements> NewSloppyArgumentsElements_ParameterMapIterator_0(compiler::CodeAssemblerState* state_, TNode<Smi> p_length, TNode<Context> p_context, TNode<FixedArray> p_arguments, TorqueStructParameterMapIterator_0 p_it);
TNode<BoolT> OutOfBounds_Smi_Smi_0(compiler::CodeAssemblerState* state_, TNode<Smi> p_index, TNode<Smi> p_length);
TorqueStructSlice_Smi_OR_TheHole_MutableReference_Smi_OR_TheHole_0 NewMutableSlice_Smi_OR_TheHole_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
void InitializeFieldsFromIterator_Smi_OR_TheHole_ParameterMapIterator_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_Smi_OR_TheHole_MutableReference_Smi_OR_TheHole_0 p_target, TorqueStructParameterMapIterator_0 p_originIterator);
void InitializeFieldsFromIterator_Smi_OR_TheHole_ConstantIterator_TheHole_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_Smi_OR_TheHole_MutableReference_Smi_OR_TheHole_0 p_target, TorqueStructConstantIterator_TheHole_0 p_originIterator);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_ARGUMENTS_TQ_CSA_H_
