#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_FREE_SPACE_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_FREE_SPACE_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<FreeSpace> Cast_FreeSpace_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Smi> LoadFreeSpaceSize_0(compiler::CodeAssemblerState* state_, TNode<FreeSpace> p_o);
void StoreFreeSpaceSize_0(compiler::CodeAssemblerState* state_, TNode<FreeSpace> p_o, TNode<Smi> p_v);
TNode<MaybeObject> LoadFreeSpaceNext_0(compiler::CodeAssemblerState* state_, TNode<FreeSpace> p_o);
void StoreFreeSpaceNext_0(compiler::CodeAssemblerState* state_, TNode<FreeSpace> p_o, TNode<MaybeObject> p_v);
TNode<FreeSpace> DownCastForTorqueClass_FreeSpace_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_FREE_SPACE_TQ_CSA_H_
