#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PRIMITIVE_HEAP_OBJECT_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PRIMITIVE_HEAP_OBJECT_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<PrimitiveHeapObject> Cast_PrimitiveHeapObject_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<PrimitiveHeapObject> DownCastForTorqueClass_PrimitiveHeapObject_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PRIMITIVE_HEAP_OBJECT_TQ_CSA_H_
