#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_INTERNAL_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_INTERNAL_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Uint32T> FromConstexpr_ForInFeedback_constexpr_kAny_0(compiler::CodeAssemblerState* state_, ForInFeedback p_o);
TNode<Object> ForInNextSlow_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<UintPtrT> p_slot, TNode<HeapObject> p_receiver, TNode<Object> p_key, TNode<Object> p_cacheType, TNode<HeapObject> p_maybeFeedbackVector, UpdateFeedbackMode p_guaranteedFeedback);
TNode<JSArray> Cast_JSArray_2(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<MaybeObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<Smi> SmiTag_ForInFeedback_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_value);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_INTERNAL_TQ_CSA_H_
