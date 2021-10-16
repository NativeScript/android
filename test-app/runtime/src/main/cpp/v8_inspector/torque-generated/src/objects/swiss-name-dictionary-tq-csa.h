#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SWISS_NAME_DICTIONARY_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SWISS_NAME_DICTIONARY_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<SwissNameDictionary> Cast_SwissNameDictionary_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<IntPtrT> SwissNameDictionaryCapacityFor_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_atLeastSpaceFor);
TNode<IntPtrT> SwissNameDictionaryMaxUsableCapacity_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity);
TNode<IntPtrT> SwissNameDictionarySizeFor_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity);
TNode<IntPtrT> SwissNameDictionaryMetaTableSizePerEntryFor_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity);
TNode<IntPtrT> SwissNameDictionaryMetaTableSizeFor_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity);
TNode<IntPtrT> SwissNameDictionaryDataTableStartOffsetMT_0(compiler::CodeAssemblerState* state_);
TNode<IntPtrT> SwissNameDictionaryCtrlTableStartOffsetMT_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity);
TorqueStructProbeSequence_0 Probe_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_hash, TNode<Uint32T> p_mask);
void SwissNameDictionaryDelete_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<IntPtrT> p_entry, compiler::CodeAssemblerLabel* label_Shrunk, compiler::TypedCodeAssemblerVariable<SwissNameDictionary>* label_Shrunk_parameter_0);
void SwissNameDictionaryFindEntrySIMD_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound);
void SwissNameDictionaryFindEntryPortable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound);
void SwissNameDictionaryAddSIMD_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout);
void SwissNameDictionaryAddPortable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout);
TNode<Uint32T> LoadSwissNameDictionaryHash_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o);
void StoreSwissNameDictionaryHash_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<Uint32T> p_v);
TNode<Int32T> LoadSwissNameDictionaryCapacity_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o);
TNode<ByteArray> LoadSwissNameDictionaryMetaTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o);
void StoreSwissNameDictionaryMetaTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<ByteArray> p_v);
TorqueStructSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_MutableReference_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0 FieldSliceSwissNameDictionaryDataTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o);
TNode<Object> LoadSwissNameDictionaryDataTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i);
void StoreSwissNameDictionaryDataTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Object> p_v);
TorqueStructSlice_uint8_MutableReference_uint8_0 FieldSliceSwissNameDictionaryCtrlTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o);
TNode<Uint8T> LoadSwissNameDictionaryCtrlTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i);
void StoreSwissNameDictionaryCtrlTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v);
TorqueStructSlice_uint8_MutableReference_uint8_0 FieldSliceSwissNameDictionaryPropertyDetailsTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o);
TNode<Uint8T> LoadSwissNameDictionaryPropertyDetailsTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i);
void StoreSwissNameDictionaryPropertyDetailsTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v);
TNode<SwissNameDictionary> DownCastForTorqueClass_SwissNameDictionary_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
void FindEntry_GroupSse2Loader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound);
void FindEntry_GroupPortableLoader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound);
void Add_GroupSse2Loader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout);
void Add_GroupPortableLoader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout);
TNode<Int32T> FindFirstEmpty_GroupSse2Loader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<IntPtrT> p_capacity, TNode<Uint32T> p_hash);
TNode<Int32T> FindFirstEmpty_GroupPortableLoader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<IntPtrT> p_capacity, TNode<Uint32T> p_hash);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SWISS_NAME_DICTIONARY_TQ_CSA_H_
