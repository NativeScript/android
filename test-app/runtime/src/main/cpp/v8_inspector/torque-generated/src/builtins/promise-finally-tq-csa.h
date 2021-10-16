#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_PROMISE_FINALLY_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_PROMISE_FINALLY_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<IntPtrT> FromConstexpr_PromiseValueThunkOrReasonContextSlot_constexpr_kValueSlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseValueThunkOrReasonContextSlot p_o);
TNode<IntPtrT> FromConstexpr_PromiseValueThunkOrReasonContextSlot_constexpr_kPromiseValueThunkOrReasonContextLength_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseValueThunkOrReasonContextSlot p_o);
TNode<IntPtrT> kValueSlot_0(compiler::CodeAssemblerState* state_);
TNode<IntPtrT> FromConstexpr_PromiseFinallyContextSlot_constexpr_kOnFinallySlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseFinallyContextSlot p_o);
TNode<IntPtrT> FromConstexpr_PromiseFinallyContextSlot_constexpr_kConstructorSlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseFinallyContextSlot p_o);
TNode<IntPtrT> FromConstexpr_PromiseFinallyContextSlot_constexpr_kPromiseFinallyContextLength_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseFinallyContextSlot p_o);
TNode<IntPtrT> kOnFinallySlot_0(compiler::CodeAssemblerState* state_);
TNode<IntPtrT> kConstructorSlot_0(compiler::CodeAssemblerState* state_);
TNode<JSFunction> CreateThrowerFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<Object> p_reason);
TNode<JSFunction> CreateValueThunkFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<Object> p_value);
TorqueStructPromiseFinallyFunctions_0 CreatePromiseFinallyFunctions_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<JSReceiver> p_onFinally, TNode<JSReceiver> p_constructor);
TorqueStructReference_JSAny_0 ContextSlot_PromiseValueThunkOrReasonContext_PromiseValueThunkOrReasonContext_JSAny_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index);
void InitContextSlot_PromiseValueThunkOrReasonContext_PromiseValueThunkOrReasonContext_JSAny_JSAny_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<Object> p_value);
TorqueStructReference_Callable_0 ContextSlot_PromiseFinallyContext_PromiseFinallyContext_Callable_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index);
TorqueStructReference_Constructor_0 ContextSlot_PromiseFinallyContext_PromiseFinallyContext_Constructor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index);
void InitContextSlot_PromiseFinallyContext_PromiseFinallyContext_Callable_Callable_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<JSReceiver> p_value);
void InitContextSlot_PromiseFinallyContext_PromiseFinallyContext_Constructor_Constructor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<JSReceiver> p_value);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_PROMISE_FINALLY_TQ_CSA_H_
