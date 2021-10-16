#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_EMBEDDER_DATA_ARRAY_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_EMBEDDER_DATA_ARRAY_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<EmbedderDataArray> Cast_EmbedderDataArray_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Smi> LoadEmbedderDataArrayLength_0(compiler::CodeAssemblerState* state_, TNode<EmbedderDataArray> p_o);
void StoreEmbedderDataArrayLength_0(compiler::CodeAssemblerState* state_, TNode<EmbedderDataArray> p_o, TNode<Smi> p_v);
TNode<EmbedderDataArray> DownCastForTorqueClass_EmbedderDataArray_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_EMBEDDER_DATA_ARRAY_TQ_CSA_H_
