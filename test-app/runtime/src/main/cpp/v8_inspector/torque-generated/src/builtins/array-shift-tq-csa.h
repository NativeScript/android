#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_ARRAY_SHIFT_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_ARRAY_SHIFT_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Object> TryFastArrayShift_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, compiler::CodeAssemblerLabel* label_Slow, compiler::CodeAssemblerLabel* label_Runtime);
TNode<Object> GenericArrayShift_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_ARRAY_SHIFT_TQ_CSA_H_
