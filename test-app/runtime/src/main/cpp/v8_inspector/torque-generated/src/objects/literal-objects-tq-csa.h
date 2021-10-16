#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_LITERAL_OBJECTS_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_LITERAL_OBJECTS_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<ArrayBoilerplateDescription> Cast_ArrayBoilerplateDescription_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<RegExpBoilerplateDescription> Cast_RegExpBoilerplateDescription_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Smi> LoadArrayBoilerplateDescriptionFlags_0(compiler::CodeAssemblerState* state_, TNode<ArrayBoilerplateDescription> p_o);
void StoreArrayBoilerplateDescriptionFlags_0(compiler::CodeAssemblerState* state_, TNode<ArrayBoilerplateDescription> p_o, TNode<Smi> p_v);
TNode<FixedArrayBase> LoadArrayBoilerplateDescriptionConstantElements_0(compiler::CodeAssemblerState* state_, TNode<ArrayBoilerplateDescription> p_o);
void StoreArrayBoilerplateDescriptionConstantElements_0(compiler::CodeAssemblerState* state_, TNode<ArrayBoilerplateDescription> p_o, TNode<FixedArrayBase> p_v);
TNode<FixedArray> LoadRegExpBoilerplateDescriptionData_0(compiler::CodeAssemblerState* state_, TNode<RegExpBoilerplateDescription> p_o);
void StoreRegExpBoilerplateDescriptionData_0(compiler::CodeAssemblerState* state_, TNode<RegExpBoilerplateDescription> p_o, TNode<FixedArray> p_v);
TNode<String> LoadRegExpBoilerplateDescriptionSource_0(compiler::CodeAssemblerState* state_, TNode<RegExpBoilerplateDescription> p_o);
void StoreRegExpBoilerplateDescriptionSource_0(compiler::CodeAssemblerState* state_, TNode<RegExpBoilerplateDescription> p_o, TNode<String> p_v);
TNode<Smi> LoadRegExpBoilerplateDescriptionFlags_0(compiler::CodeAssemblerState* state_, TNode<RegExpBoilerplateDescription> p_o);
void StoreRegExpBoilerplateDescriptionFlags_0(compiler::CodeAssemblerState* state_, TNode<RegExpBoilerplateDescription> p_o, TNode<Smi> p_v);
TNode<ArrayBoilerplateDescription> DownCastForTorqueClass_ArrayBoilerplateDescription_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<RegExpBoilerplateDescription> DownCastForTorqueClass_RegExpBoilerplateDescription_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_LITERAL_OBJECTS_TQ_CSA_H_
