#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PROPERTY_ARRAY_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PROPERTY_ARRAY_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<PropertyArray> Cast_PropertyArray_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Smi> LoadPropertyArrayLengthAndHash_0(compiler::CodeAssemblerState* state_, TNode<PropertyArray> p_o);
void StorePropertyArrayLengthAndHash_0(compiler::CodeAssemblerState* state_, TNode<PropertyArray> p_o, TNode<Smi> p_v);
TNode<PropertyArray> DownCastForTorqueClass_PropertyArray_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PROPERTY_ARRAY_TQ_CSA_H_
