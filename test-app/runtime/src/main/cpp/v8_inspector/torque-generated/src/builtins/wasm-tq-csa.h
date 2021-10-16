#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_WASM_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_WASM_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<NativeContext> LoadContextFromFrame_0(compiler::CodeAssemblerState* state_);
TNode<HeapObject> WasmAllocateArray_0(compiler::CodeAssemblerState* state_, TNode<Map> p_rtt, TNode<Uint32T> p_length, TNode<Uint32T> p_elementSize, CodeStubAssembler::InitializationMode p_initializationMode);
TNode<IntPtrT> Convert_intptr_constexpr_int32_0(compiler::CodeAssemblerState* state_, int32_t p_i);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_WASM_TQ_CSA_H_
