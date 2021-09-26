#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_CODE_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_CODE_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<BytecodeArray> Cast_BytecodeArray_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<CodeDataContainer> Cast_CodeDataContainer_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<FixedArray> LoadBytecodeArrayConstantPool_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o);
void StoreBytecodeArrayConstantPool_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o, TNode<FixedArray> p_v);
TNode<ByteArray> LoadBytecodeArrayHandlerTable_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o);
void StoreBytecodeArrayHandlerTable_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o, TNode<ByteArray> p_v);
TNode<HeapObject> LoadBytecodeArraySourcePositionTable_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o);
void StoreBytecodeArraySourcePositionTable_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o, TNode<HeapObject> p_v);
TNode<Int32T> LoadBytecodeArrayFrameSize_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o);
void StoreBytecodeArrayFrameSize_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o, TNode<Int32T> p_v);
TNode<Int32T> LoadBytecodeArrayParameterSize_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o);
void StoreBytecodeArrayParameterSize_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o, TNode<Int32T> p_v);
TNode<Int32T> LoadBytecodeArrayIncomingNewTargetOrGeneratorRegister_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o);
void StoreBytecodeArrayIncomingNewTargetOrGeneratorRegister_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o, TNode<Int32T> p_v);
TNode<Int8T> LoadBytecodeArrayOsrNestingLevel_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o);
void StoreBytecodeArrayOsrNestingLevel_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o, TNode<Int8T> p_v);
TNode<Int8T> LoadBytecodeArrayBytecodeAge_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o);
void StoreBytecodeArrayBytecodeAge_0(compiler::CodeAssemblerState* state_, TNode<BytecodeArray> p_o, TNode<Int8T> p_v);
TNode<BytecodeArray> DownCastForTorqueClass_BytecodeArray_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<CodeDataContainer> DownCastForTorqueClass_CodeDataContainer_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_CODE_TQ_CSA_H_
