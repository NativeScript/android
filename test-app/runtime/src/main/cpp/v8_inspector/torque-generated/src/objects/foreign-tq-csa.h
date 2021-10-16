#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_FOREIGN_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_FOREIGN_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Foreign> Cast_Foreign_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<ExternalPointerT> LoadForeignForeignAddress_0(compiler::CodeAssemblerState* state_, TNode<Foreign> p_o);
void StoreForeignForeignAddress_0(compiler::CodeAssemblerState* state_, TNode<Foreign> p_o, TNode<ExternalPointerT> p_v);
TNode<Foreign> DownCastForTorqueClass_Foreign_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_FOREIGN_TQ_CSA_H_
