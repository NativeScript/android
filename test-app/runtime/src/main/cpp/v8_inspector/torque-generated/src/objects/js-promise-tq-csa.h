#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_PROMISE_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_PROMISE_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<JSPromise> Cast_JSPromise_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Object> LoadJSPromiseReactionsOrResult_0(compiler::CodeAssemblerState* state_, TNode<JSPromise> p_o);
void StoreJSPromiseReactionsOrResult_0(compiler::CodeAssemblerState* state_, TNode<JSPromise> p_o, TNode<Object> p_v);
TNode<Smi> LoadJSPromiseFlags_0(compiler::CodeAssemblerState* state_, TNode<JSPromise> p_o);
void StoreJSPromiseFlags_0(compiler::CodeAssemblerState* state_, TNode<JSPromise> p_o, TNode<Smi> p_v);
TNode<Int32T> Method_JSPromise_Status_0(compiler::CodeAssemblerState* state_, TNode<JSPromise> p_this);
void Method_JSPromise_SetStatus_0(compiler::CodeAssemblerState* state_, TNode<JSPromise> p_this, Promise::PromiseState p_status);
TNode<BoolT> Method_JSPromise_HasHandler_0(compiler::CodeAssemblerState* state_, TNode<JSPromise> p_this);
void Method_JSPromise_SetHasHandler_0(compiler::CodeAssemblerState* state_, TNode<JSPromise> p_this);
TNode<JSPromise> DownCastForTorqueClass_JSPromise_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_PROMISE_TQ_CSA_H_
