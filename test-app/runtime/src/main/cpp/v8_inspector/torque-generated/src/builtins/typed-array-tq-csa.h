#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_TYPED_ARRAY_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_TYPED_ARRAY_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Smi> kStoreSucceded_0(compiler::CodeAssemblerState* state_);
TNode<Smi> kStoreFailureArrayDetached_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_0(compiler::CodeAssemblerState* state_, TNode<Int32T> p_elementsKind);
TNode<JSTypedArray> EnsureAttached_0(compiler::CodeAssemblerState* state_, TNode<JSTypedArray> p_array, compiler::CodeAssemblerLabel* label_Detached);
TorqueStructAttachedJSTypedArrayWitness_0 NewAttachedJSTypedArrayWitness_0(compiler::CodeAssemblerState* state_, TNode<JSTypedArray> p_array);
ElementsKind KindForArrayType_Uint8Elements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_Int8Elements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_Uint16Elements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_Int16Elements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_Uint32Elements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_Int32Elements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_Float32Elements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_Float64Elements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_Uint8ClampedElements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_BigUint64Elements_0(compiler::CodeAssemblerState* state_);
ElementsKind KindForArrayType_BigInt64Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_Int32Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_Float32Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_Float64Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_Uint8ClampedElements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_BigUint64Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_BigInt64Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_Uint8Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_Int8Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_Uint16Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_Int16Elements_0(compiler::CodeAssemblerState* state_);
TorqueStructTypedArrayAccessor_0 GetTypedArrayAccessor_Uint32Elements_0(compiler::CodeAssemblerState* state_);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_TYPED_ARRAY_TQ_CSA_H_
