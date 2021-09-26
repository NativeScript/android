#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PROTOTYPE_INFO_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PROTOTYPE_INFO_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<PrototypeInfo> Cast_PrototypeInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<HeapObject> LoadPrototypeInfoModuleNamespace_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o);
void StorePrototypeInfoModuleNamespace_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o, TNode<HeapObject> p_v);
TNode<Object> LoadPrototypeInfoPrototypeUsers_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o);
void StorePrototypeInfoPrototypeUsers_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadPrototypeInfoPrototypeChainEnumCache_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o);
void StorePrototypeInfoPrototypeChainEnumCache_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o, TNode<Object> p_v);
TNode<Smi> LoadPrototypeInfoRegistrySlot_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o);
void StorePrototypeInfoRegistrySlot_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o, TNode<Smi> p_v);
TNode<MaybeObject> LoadPrototypeInfoObjectCreateMap_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o);
void StorePrototypeInfoObjectCreateMap_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o, TNode<MaybeObject> p_v);
TNode<Smi> LoadPrototypeInfoBitField_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o);
void StorePrototypeInfoBitField_0(compiler::CodeAssemblerState* state_, TNode<PrototypeInfo> p_o, TNode<Smi> p_v);
TNode<PrototypeInfo> DownCastForTorqueClass_PrototypeInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PROTOTYPE_INFO_TQ_CSA_H_
