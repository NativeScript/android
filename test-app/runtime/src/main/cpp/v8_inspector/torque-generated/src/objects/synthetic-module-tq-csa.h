#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SYNTHETIC_MODULE_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SYNTHETIC_MODULE_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<SyntheticModule> Cast_SyntheticModule_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<String> LoadSyntheticModuleName_0(compiler::CodeAssemblerState* state_, TNode<SyntheticModule> p_o);
void StoreSyntheticModuleName_0(compiler::CodeAssemblerState* state_, TNode<SyntheticModule> p_o, TNode<String> p_v);
TNode<FixedArray> LoadSyntheticModuleExportNames_0(compiler::CodeAssemblerState* state_, TNode<SyntheticModule> p_o);
void StoreSyntheticModuleExportNames_0(compiler::CodeAssemblerState* state_, TNode<SyntheticModule> p_o, TNode<FixedArray> p_v);
TNode<Foreign> LoadSyntheticModuleEvaluationSteps_0(compiler::CodeAssemblerState* state_, TNode<SyntheticModule> p_o);
void StoreSyntheticModuleEvaluationSteps_0(compiler::CodeAssemblerState* state_, TNode<SyntheticModule> p_o, TNode<Foreign> p_v);
TNode<SyntheticModule> DownCastForTorqueClass_SyntheticModule_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SYNTHETIC_MODULE_TQ_CSA_H_
