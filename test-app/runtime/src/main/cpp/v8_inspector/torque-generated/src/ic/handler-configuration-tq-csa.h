#ifndef V8_GEN_TORQUE_GENERATED_SRC_IC_HANDLER_CONFIGURATION_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_IC_HANDLER_CONFIGURATION_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<LoadHandler> Cast_LoadHandler_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<StoreHandler> Cast_StoreHandler_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<LoadHandler> DownCastForTorqueClass_LoadHandler_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<StoreHandler> DownCastForTorqueClass_StoreHandler_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_IC_HANDLER_CONFIGURATION_TQ_CSA_H_
