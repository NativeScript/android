#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_CONVERSION_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_CONVERSION_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Object> TryGetExoticToPrimitive_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_input, compiler::CodeAssemblerLabel* label_OrdinaryToPrimitive);
TNode<Object> CallExoticToPrimitive_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_input, TNode<Object> p_exoticToPrimitive, TNode<String> p_hint);
TNode<Object> TryToPrimitiveMethod_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_input, TNode<String> p_name, compiler::CodeAssemblerLabel* label_Continue);
TNode<Name> Cast_Name_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<Map> UnsafeCast_Map_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_CONVERSION_TQ_CSA_H_
