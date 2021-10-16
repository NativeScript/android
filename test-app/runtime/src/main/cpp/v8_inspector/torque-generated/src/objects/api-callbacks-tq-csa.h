#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_API_CALLBACKS_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_API_CALLBACKS_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<CallHandlerInfo> Cast_CallHandlerInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<InterceptorInfo> Cast_InterceptorInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<AccessCheckInfo> Cast_AccessCheckInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<AccessorInfo> Cast_AccessorInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Object> LoadCallHandlerInfoCallback_0(compiler::CodeAssemblerState* state_, TNode<CallHandlerInfo> p_o);
void StoreCallHandlerInfoCallback_0(compiler::CodeAssemblerState* state_, TNode<CallHandlerInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadCallHandlerInfoJsCallback_0(compiler::CodeAssemblerState* state_, TNode<CallHandlerInfo> p_o);
void StoreCallHandlerInfoJsCallback_0(compiler::CodeAssemblerState* state_, TNode<CallHandlerInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadCallHandlerInfoData_0(compiler::CodeAssemblerState* state_, TNode<CallHandlerInfo> p_o);
void StoreCallHandlerInfoData_0(compiler::CodeAssemblerState* state_, TNode<CallHandlerInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadInterceptorInfoGetter_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o);
void StoreInterceptorInfoGetter_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadInterceptorInfoSetter_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o);
void StoreInterceptorInfoSetter_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadInterceptorInfoQuery_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o);
void StoreInterceptorInfoQuery_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadInterceptorInfoDescriptor_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o);
void StoreInterceptorInfoDescriptor_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadInterceptorInfoDeleter_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o);
void StoreInterceptorInfoDeleter_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadInterceptorInfoEnumerator_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o);
void StoreInterceptorInfoEnumerator_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadInterceptorInfoDefiner_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o);
void StoreInterceptorInfoDefiner_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadInterceptorInfoData_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o);
void StoreInterceptorInfoData_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o, TNode<Object> p_v);
TNode<Smi> LoadInterceptorInfoFlags_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o);
void StoreInterceptorInfoFlags_0(compiler::CodeAssemblerState* state_, TNode<InterceptorInfo> p_o, TNode<Smi> p_v);
TNode<Object> LoadAccessCheckInfoCallback_0(compiler::CodeAssemblerState* state_, TNode<AccessCheckInfo> p_o);
void StoreAccessCheckInfoCallback_0(compiler::CodeAssemblerState* state_, TNode<AccessCheckInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadAccessCheckInfoNamedInterceptor_0(compiler::CodeAssemblerState* state_, TNode<AccessCheckInfo> p_o);
void StoreAccessCheckInfoNamedInterceptor_0(compiler::CodeAssemblerState* state_, TNode<AccessCheckInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadAccessCheckInfoIndexedInterceptor_0(compiler::CodeAssemblerState* state_, TNode<AccessCheckInfo> p_o);
void StoreAccessCheckInfoIndexedInterceptor_0(compiler::CodeAssemblerState* state_, TNode<AccessCheckInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadAccessCheckInfoData_0(compiler::CodeAssemblerState* state_, TNode<AccessCheckInfo> p_o);
void StoreAccessCheckInfoData_0(compiler::CodeAssemblerState* state_, TNode<AccessCheckInfo> p_o, TNode<Object> p_v);
TNode<Name> LoadAccessorInfoName_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o);
void StoreAccessorInfoName_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o, TNode<Name> p_v);
TNode<Smi> LoadAccessorInfoFlags_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o);
void StoreAccessorInfoFlags_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o, TNode<Smi> p_v);
TNode<Object> LoadAccessorInfoExpectedReceiverType_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o);
void StoreAccessorInfoExpectedReceiverType_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadAccessorInfoSetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o);
void StoreAccessorInfoSetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadAccessorInfoGetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o);
void StoreAccessorInfoGetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadAccessorInfoJsGetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o);
void StoreAccessorInfoJsGetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o, TNode<Object> p_v);
TNode<Object> LoadAccessorInfoData_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o);
void StoreAccessorInfoData_0(compiler::CodeAssemblerState* state_, TNode<AccessorInfo> p_o, TNode<Object> p_v);
TNode<CallHandlerInfo> DownCastForTorqueClass_CallHandlerInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<InterceptorInfo> DownCastForTorqueClass_InterceptorInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<AccessCheckInfo> DownCastForTorqueClass_AccessCheckInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<AccessorInfo> DownCastForTorqueClass_AccessorInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_API_CALLBACKS_TQ_CSA_H_
