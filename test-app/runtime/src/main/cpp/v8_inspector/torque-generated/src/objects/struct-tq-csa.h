#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_STRUCT_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_STRUCT_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Struct> Cast_Struct_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Tuple2> Cast_Tuple2_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<ClassPositions> Cast_ClassPositions_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<AccessorPair> Cast_AccessorPair_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Object> LoadAccessorPairGetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorPair> p_o);
void StoreAccessorPairGetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorPair> p_o, TNode<Object> p_v);
TNode<Object> LoadAccessorPairSetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorPair> p_o);
void StoreAccessorPairSetter_0(compiler::CodeAssemblerState* state_, TNode<AccessorPair> p_o, TNode<Object> p_v);
TNode<Smi> LoadClassPositionsStart_0(compiler::CodeAssemblerState* state_, TNode<ClassPositions> p_o);
void StoreClassPositionsStart_0(compiler::CodeAssemblerState* state_, TNode<ClassPositions> p_o, TNode<Smi> p_v);
TNode<Smi> LoadClassPositionsEnd_0(compiler::CodeAssemblerState* state_, TNode<ClassPositions> p_o);
void StoreClassPositionsEnd_0(compiler::CodeAssemblerState* state_, TNode<ClassPositions> p_o, TNode<Smi> p_v);
TNode<Object> LoadTuple2Value1_0(compiler::CodeAssemblerState* state_, TNode<Tuple2> p_o);
void StoreTuple2Value1_0(compiler::CodeAssemblerState* state_, TNode<Tuple2> p_o, TNode<Object> p_v);
TNode<Object> LoadTuple2Value2_0(compiler::CodeAssemblerState* state_, TNode<Tuple2> p_o);
void StoreTuple2Value2_0(compiler::CodeAssemblerState* state_, TNode<Tuple2> p_o, TNode<Object> p_v);
TNode<Struct> DownCastForTorqueClass_Struct_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<Tuple2> DownCastForTorqueClass_Tuple2_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<ClassPositions> DownCastForTorqueClass_ClassPositions_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<AccessorPair> DownCastForTorqueClass_AccessorPair_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_STRUCT_TQ_CSA_H_
