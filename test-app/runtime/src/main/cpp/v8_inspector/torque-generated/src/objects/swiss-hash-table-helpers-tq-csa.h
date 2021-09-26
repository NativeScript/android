#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SWISS_HASH_TABLE_HELPERS_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SWISS_HASH_TABLE_HELPERS_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Uint64T> kByteMaskShift_0(compiler::CodeAssemblerState* state_);
TNode<Uint32T> H1_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_hash);
TNode<Uint32T> H2_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_hash);
TNode<Uint64T> ClearLowestSetBit_uint64_0(compiler::CodeAssemblerState* state_, TNode<Uint64T> p_value);
TNode<Int32T> Convert_int32_int32_0(compiler::CodeAssemblerState* state_, TNode<Int32T> p_i);
TNode<Uint32T> ClearLowestSetBit_uint32_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_value);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SWISS_HASH_TABLE_HELPERS_TQ_CSA_H_
