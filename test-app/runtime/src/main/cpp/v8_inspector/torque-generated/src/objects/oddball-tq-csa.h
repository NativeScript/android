#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_ODDBALL_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_ODDBALL_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Oddball> Cast_Oddball_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Float64T> LoadOddballToNumberRaw_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o);
void StoreOddballToNumberRaw_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o, TNode<Float64T> p_v);
TNode<String> LoadOddballToString_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o);
void StoreOddballToString_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o, TNode<String> p_v);
TNode<Number> LoadOddballToNumber_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o);
void StoreOddballToNumber_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o, TNode<Number> p_v);
TNode<String> LoadOddballTypeOf_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o);
void StoreOddballTypeOf_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o, TNode<String> p_v);
TNode<Smi> LoadOddballKind_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o);
void StoreOddballKind_0(compiler::CodeAssemblerState* state_, TNode<Oddball> p_o, TNode<Smi> p_v);
TNode<Oddball> DownCastForTorqueClass_Oddball_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_ODDBALL_TQ_CSA_H_
