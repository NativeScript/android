#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_MICROTASK_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_MICROTASK_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Microtask> Cast_Microtask_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<CallbackTask> Cast_CallbackTask_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<CallableTask> Cast_CallableTask_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Foreign> LoadCallbackTaskCallback_0(compiler::CodeAssemblerState* state_, TNode<CallbackTask> p_o);
void StoreCallbackTaskCallback_0(compiler::CodeAssemblerState* state_, TNode<CallbackTask> p_o, TNode<Foreign> p_v);
TNode<Foreign> LoadCallbackTaskData_0(compiler::CodeAssemblerState* state_, TNode<CallbackTask> p_o);
void StoreCallbackTaskData_0(compiler::CodeAssemblerState* state_, TNode<CallbackTask> p_o, TNode<Foreign> p_v);
TNode<JSReceiver> LoadCallableTaskCallable_0(compiler::CodeAssemblerState* state_, TNode<CallableTask> p_o);
void StoreCallableTaskCallable_0(compiler::CodeAssemblerState* state_, TNode<CallableTask> p_o, TNode<JSReceiver> p_v);
TNode<Context> LoadCallableTaskContext_0(compiler::CodeAssemblerState* state_, TNode<CallableTask> p_o);
void StoreCallableTaskContext_0(compiler::CodeAssemblerState* state_, TNode<CallableTask> p_o, TNode<Context> p_v);
TNode<Microtask> DownCastForTorqueClass_Microtask_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<CallbackTask> DownCastForTorqueClass_CallbackTask_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<CallableTask> DownCastForTorqueClass_CallableTask_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_MICROTASK_TQ_CSA_H_
