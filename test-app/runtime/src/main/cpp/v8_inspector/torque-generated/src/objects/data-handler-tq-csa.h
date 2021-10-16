#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_DATA_HANDLER_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_DATA_HANDLER_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<DataHandler> Cast_DataHandler_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Object> LoadDataHandlerSmiHandler_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o);
void StoreDataHandlerSmiHandler_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o, TNode<Object> p_v);
TNode<Object> LoadDataHandlerValidityCell_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o);
void StoreDataHandlerValidityCell_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o, TNode<Object> p_v);
TNode<MaybeObject> LoadDataHandlerData1_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o);
void StoreDataHandlerData1_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o, TNode<MaybeObject> p_v);
TNode<MaybeObject> LoadDataHandlerData2_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o);
void StoreDataHandlerData2_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o, TNode<MaybeObject> p_v);
TNode<MaybeObject> LoadDataHandlerData3_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o);
void StoreDataHandlerData3_0(compiler::CodeAssemblerState* state_, TNode<DataHandler> p_o, TNode<MaybeObject> p_v);
TNode<DataHandler> DownCastForTorqueClass_DataHandler_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_DATA_HANDLER_TQ_CSA_H_
