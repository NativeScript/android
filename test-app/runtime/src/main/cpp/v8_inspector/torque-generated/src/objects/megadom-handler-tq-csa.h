#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_MEGADOM_HANDLER_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_MEGADOM_HANDLER_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<MegaDomHandler> Cast_MegaDomHandler_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<MaybeObject> LoadMegaDomHandlerAccessor_0(compiler::CodeAssemblerState* state_, TNode<MegaDomHandler> p_o);
void StoreMegaDomHandlerAccessor_0(compiler::CodeAssemblerState* state_, TNode<MegaDomHandler> p_o, TNode<MaybeObject> p_v);
TNode<MaybeObject> LoadMegaDomHandlerContext_0(compiler::CodeAssemblerState* state_, TNode<MegaDomHandler> p_o);
void StoreMegaDomHandlerContext_0(compiler::CodeAssemblerState* state_, TNode<MegaDomHandler> p_o, TNode<MaybeObject> p_v);
TNode<MegaDomHandler> DownCastForTorqueClass_MegaDomHandler_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_MEGADOM_HANDLER_TQ_CSA_H_
