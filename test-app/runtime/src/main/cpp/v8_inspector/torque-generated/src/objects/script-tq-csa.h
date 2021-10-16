#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SCRIPT_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SCRIPT_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Script> Cast_Script_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<PrimitiveHeapObject> LoadScriptSource_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptSource_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<PrimitiveHeapObject> p_v);
TNode<Object> LoadScriptName_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptName_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<Object> p_v);
TNode<Smi> LoadScriptLineOffset_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptLineOffset_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<Smi> p_v);
TNode<Smi> LoadScriptColumnOffset_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptColumnOffset_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<Smi> p_v);
TNode<Object> LoadScriptContextData_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptContextData_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<Object> p_v);
TNode<Smi> LoadScriptScriptType_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptScriptType_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<Smi> p_v);
TNode<HeapObject> LoadScriptLineEnds_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptLineEnds_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<HeapObject> p_v);
TNode<Smi> LoadScriptId_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptId_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<Smi> p_v);
TNode<HeapObject> LoadScriptEvalFromSharedOrWrappedArgumentsOrSfiTable_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptEvalFromSharedOrWrappedArgumentsOrSfiTable_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<HeapObject> p_v);
TNode<Object> LoadScriptEvalFromPosition_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptEvalFromPosition_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<Object> p_v);
TNode<HeapObject> LoadScriptSharedFunctionInfos_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptSharedFunctionInfos_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<HeapObject> p_v);
TNode<Smi> LoadScriptFlags_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptFlags_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<Smi> p_v);
TNode<PrimitiveHeapObject> LoadScriptSourceUrl_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptSourceUrl_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<PrimitiveHeapObject> p_v);
TNode<Object> LoadScriptSourceMappingUrl_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptSourceMappingUrl_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<Object> p_v);
TNode<FixedArray> LoadScriptHostDefinedOptions_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o);
void StoreScriptHostDefinedOptions_0(compiler::CodeAssemblerState* state_, TNode<Script> p_o, TNode<FixedArray> p_v);
TNode<Script> DownCastForTorqueClass_Script_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SCRIPT_TQ_CSA_H_
