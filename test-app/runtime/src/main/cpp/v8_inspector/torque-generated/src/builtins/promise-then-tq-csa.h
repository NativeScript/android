#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_PROMISE_THEN_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_PROMISE_THEN_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<BoolT> IsPromiseSpeciesLookupChainIntact_0(compiler::CodeAssemblerState* state_, TNode<NativeContext> p_nativeContext, TNode<Map> p_promiseMap);
TNode<HeapObject> CastOrDefault_Callable_JSAny_Undefined_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_x, TNode<Oddball> p_default);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_PROMISE_THEN_TQ_CSA_H_
