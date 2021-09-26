#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_PROXY_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_PROXY_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<JSProxy> Cast_JSProxy_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<JSObject> NewJSProxyRevocableResult_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSProxy> p_proxy, TNode<JSFunction> p_revoke);
TNode<HeapObject> LoadJSProxyTarget_0(compiler::CodeAssemblerState* state_, TNode<JSProxy> p_o);
void StoreJSProxyTarget_0(compiler::CodeAssemblerState* state_, TNode<JSProxy> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadJSProxyHandler_0(compiler::CodeAssemblerState* state_, TNode<JSProxy> p_o);
void StoreJSProxyHandler_0(compiler::CodeAssemblerState* state_, TNode<JSProxy> p_o, TNode<HeapObject> p_v);
TNode<Object> LoadJSProxyRevocableResultProxy_0(compiler::CodeAssemblerState* state_, TNode<JSObject> p_o);
void StoreJSProxyRevocableResultProxy_0(compiler::CodeAssemblerState* state_, TNode<JSObject> p_o, TNode<Object> p_v);
TNode<Object> LoadJSProxyRevocableResultRevoke_0(compiler::CodeAssemblerState* state_, TNode<JSObject> p_o);
void StoreJSProxyRevocableResultRevoke_0(compiler::CodeAssemblerState* state_, TNode<JSObject> p_o, TNode<Object> p_v);
TNode<JSProxy> DownCastForTorqueClass_JSProxy_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_PROXY_TQ_CSA_H_
