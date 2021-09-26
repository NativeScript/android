#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PROPERTY_CELL_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PROPERTY_CELL_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<PropertyCell> Cast_PropertyCell_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Name> LoadPropertyCellName_0(compiler::CodeAssemblerState* state_, TNode<PropertyCell> p_o);
void StorePropertyCellName_0(compiler::CodeAssemblerState* state_, TNode<PropertyCell> p_o, TNode<Name> p_v);
TNode<Smi> LoadPropertyCellPropertyDetailsRaw_0(compiler::CodeAssemblerState* state_, TNode<PropertyCell> p_o);
void StorePropertyCellPropertyDetailsRaw_0(compiler::CodeAssemblerState* state_, TNode<PropertyCell> p_o, TNode<Smi> p_v);
TNode<Object> LoadPropertyCellValue_0(compiler::CodeAssemblerState* state_, TNode<PropertyCell> p_o);
void StorePropertyCellValue_0(compiler::CodeAssemblerState* state_, TNode<PropertyCell> p_o, TNode<Object> p_v);
TNode<WeakFixedArray> LoadPropertyCellDependentCode_0(compiler::CodeAssemblerState* state_, TNode<PropertyCell> p_o);
void StorePropertyCellDependentCode_0(compiler::CodeAssemblerState* state_, TNode<PropertyCell> p_o, TNode<WeakFixedArray> p_v);
TNode<PropertyCell> DownCastForTorqueClass_PropertyCell_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_PROPERTY_CELL_TQ_CSA_H_
