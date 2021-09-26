#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_IC_DYNAMIC_CHECK_MAPS_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_IC_DYNAMIC_CHECK_MAPS_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Int32T> PerformPolymorphicCheck_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_expectedPolymorphicArray, TNode<Map> p_actualMap, TNode<Object> p_actualHandler);
TNode<Int32T> PerformMonomorphicCheck_0(compiler::CodeAssemblerState* state_, TNode<FeedbackVector> p_feedbackVector, TNode<IntPtrT> p_slotIndex, TNode<HeapObject> p_expectedMap, TNode<Map> p_actualMap, TNode<Object> p_actualHandler);
TNode<Int32T> DynamicCheckMaps_0(compiler::CodeAssemblerState* state_, TNode<Map> p_actualMap, TNode<IntPtrT> p_slotIndex, TNode<Object> p_actualHandler);
TNode<Int32T> DynamicCheckMapsWithFeedbackVector_0(compiler::CodeAssemblerState* state_, TNode<Map> p_actualMap, TNode<IntPtrT> p_slotIndex, TNode<Object> p_actualHandler, TNode<FeedbackVector> p_feedbackVector);
TNode<BoolT> Is_WeakFixedArray_HeapObject_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_o);
TNode<WeakFixedArray> UnsafeCast_WeakFixedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TNode<MaybeObject> UnsafeCast_WeakHeapObject_0(compiler::CodeAssemblerState* state_, TNode<MaybeObject> p_o);
TNode<Object> Cast_Object_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<MaybeObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_IC_DYNAMIC_CHECK_MAPS_TQ_CSA_H_
