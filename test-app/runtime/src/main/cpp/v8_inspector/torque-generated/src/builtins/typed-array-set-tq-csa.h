#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_TYPED_ARRAY_SET_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_TYPED_ARRAY_SET_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
const char* kBuiltinNameSet_0(compiler::CodeAssemblerState* state_);
void TypedArrayPrototypeSetArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, TNode<JSTypedArray> p_target, TNode<Object> p_arrayArg, TNode<UintPtrT> p_targetOffset, TNode<BoolT> p_targetOffsetOverflowed, compiler::CodeAssemblerLabel* label_IfOffsetOutOfBounds, compiler::CodeAssemblerLabel* label_IfDetached);
void TypedArrayPrototypeSetTypedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, TNode<JSTypedArray> p_target, TNode<JSTypedArray> p_typedArray, TNode<UintPtrT> p_targetOffset, TNode<BoolT> p_targetOffsetOverflowed, compiler::CodeAssemblerLabel* label_IfOffsetOutOfBounds);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_TYPED_ARRAY_SET_TQ_CSA_H_
