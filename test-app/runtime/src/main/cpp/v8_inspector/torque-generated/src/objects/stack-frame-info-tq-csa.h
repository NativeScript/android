#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_STACK_FRAME_INFO_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_STACK_FRAME_INFO_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<StackFrameInfo> Cast_StackFrameInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Object> LoadStackFrameInfoReceiverOrInstance_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o);
void StoreStackFrameInfoReceiverOrInstance_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadStackFrameInfoFunction_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o);
void StoreStackFrameInfoFunction_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o, TNode<Object> p_v);
TNode<HeapObject> LoadStackFrameInfoCodeObject_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o);
void StoreStackFrameInfoCodeObject_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o, TNode<HeapObject> p_v);
TNode<Smi> LoadStackFrameInfoCodeOffsetOrSourcePosition_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o);
void StoreStackFrameInfoCodeOffsetOrSourcePosition_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o, TNode<Smi> p_v);
TNode<Smi> LoadStackFrameInfoFlags_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o);
void StoreStackFrameInfoFlags_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o, TNode<Smi> p_v);
TNode<FixedArray> LoadStackFrameInfoParameters_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o);
void StoreStackFrameInfoParameters_0(compiler::CodeAssemblerState* state_, TNode<StackFrameInfo> p_o, TNode<FixedArray> p_v);
TNode<StackFrameInfo> DownCastForTorqueClass_StackFrameInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_STACK_FRAME_INFO_TQ_CSA_H_
