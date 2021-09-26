#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_HEAP_NUMBER_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_HEAP_NUMBER_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<HeapNumber> Cast_HeapNumber_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Float64T> LoadHeapNumberValue_0(compiler::CodeAssemblerState* state_, TNode<HeapNumber> p_o);
void StoreHeapNumberValue_0(compiler::CodeAssemblerState* state_, TNode<HeapNumber> p_o, TNode<Float64T> p_v);
TNode<HeapNumber> DownCastForTorqueClass_HeapNumber_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_HEAP_NUMBER_TQ_CSA_H_
