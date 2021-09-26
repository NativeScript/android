#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_INTERNAL_COVERAGE_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_INTERNAL_COVERAGE_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<CoverageInfo> GetCoverageInfo_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSFunction> p_function, compiler::CodeAssemblerLabel* label_IfNoCoverageInfo);
void IncrementBlockCount_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<CoverageInfo> p_coverageInfo, TNode<Smi> p_slot);
TNode<CoverageInfo> UnsafeCast_CoverageInfo_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_INTERNAL_COVERAGE_TQ_CSA_H_
