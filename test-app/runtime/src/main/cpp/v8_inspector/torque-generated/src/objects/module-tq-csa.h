#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_MODULE_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_MODULE_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Module> Cast_Module_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<JSModuleNamespace> Cast_JSModuleNamespace_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<ObjectHashTable> LoadModuleExports_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o);
void StoreModuleExports_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o, TNode<ObjectHashTable> p_v);
TNode<Smi> LoadModuleHash_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o);
void StoreModuleHash_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o, TNode<Smi> p_v);
TNode<Smi> LoadModuleStatus_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o);
void StoreModuleStatus_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o, TNode<Smi> p_v);
TNode<HeapObject> LoadModuleModuleNamespace_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o);
void StoreModuleModuleNamespace_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o, TNode<HeapObject> p_v);
TNode<Object> LoadModuleException_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o);
void StoreModuleException_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o, TNode<Object> p_v);
TNode<HeapObject> LoadModuleTopLevelCapability_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o);
void StoreModuleTopLevelCapability_0(compiler::CodeAssemblerState* state_, TNode<Module> p_o, TNode<HeapObject> p_v);
TNode<Module> LoadJSModuleNamespaceModule_0(compiler::CodeAssemblerState* state_, TNode<JSModuleNamespace> p_o);
void StoreJSModuleNamespaceModule_0(compiler::CodeAssemblerState* state_, TNode<JSModuleNamespace> p_o, TNode<Module> p_v);
TNode<Module> DownCastForTorqueClass_Module_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<JSModuleNamespace> DownCastForTorqueClass_JSModuleNamespace_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_MODULE_TQ_CSA_H_
