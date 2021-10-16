#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_ALLOCATION_SITE_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_ALLOCATION_SITE_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<AllocationSite> Cast_AllocationSite_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<AllocationMemento> Cast_AllocationMemento_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<AllocationSite> LoadAllocationMementoAllocationSite_0(compiler::CodeAssemblerState* state_, TNode<AllocationMemento> p_o);
void StoreAllocationMementoAllocationSite_0(compiler::CodeAssemblerState* state_, TNode<AllocationMemento> p_o, TNode<AllocationSite> p_v);
TNode<AllocationSite> DownCastForTorqueClass_AllocationSite_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<AllocationMemento> DownCastForTorqueClass_AllocationMemento_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_ALLOCATION_SITE_TQ_CSA_H_
