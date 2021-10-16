#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_NAME_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_NAME_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Name> Cast_Name_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Symbol> Cast_Symbol_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Uint32T> kNameEmptyHashField_0(compiler::CodeAssemblerState* state_);
TNode<BoolT> ContainsCachedArrayIndex_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_hash);
TNode<Uint32T> kArrayIndexValueBitsShift_0(compiler::CodeAssemblerState* state_);
TNode<Uint32T> kArrayIndexLengthBitsShift_0(compiler::CodeAssemblerState* state_);
TNode<Uint32T> TenToThe_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_exponent);
TNode<Uint32T> MakeArrayIndexHash_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_value, TNode<Uint32T> p_length);
TNode<Uint32T> LoadNameRawHashField_0(compiler::CodeAssemblerState* state_, TNode<Name> p_o);
void StoreNameRawHashField_0(compiler::CodeAssemblerState* state_, TNode<Name> p_o, TNode<Uint32T> p_v);
TNode<Uint32T> LoadSymbolFlags_0(compiler::CodeAssemblerState* state_, TNode<Symbol> p_o);
void StoreSymbolFlags_0(compiler::CodeAssemblerState* state_, TNode<Symbol> p_o, TNode<Uint32T> p_v);
TNode<PrimitiveHeapObject> LoadSymbolDescription_0(compiler::CodeAssemblerState* state_, TNode<Symbol> p_o);
void StoreSymbolDescription_0(compiler::CodeAssemblerState* state_, TNode<Symbol> p_o, TNode<PrimitiveHeapObject> p_v);
TNode<Name> DownCastForTorqueClass_Name_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<Symbol> DownCastForTorqueClass_Symbol_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_NAME_TQ_CSA_H_
