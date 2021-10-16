#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_TEMPLATE_OBJECTS_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_TEMPLATE_OBJECTS_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<CachedTemplateObject> Cast_CachedTemplateObject_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<TemplateObjectDescription> Cast_TemplateObjectDescription_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Smi> LoadCachedTemplateObjectSlotId_0(compiler::CodeAssemblerState* state_, TNode<CachedTemplateObject> p_o);
void StoreCachedTemplateObjectSlotId_0(compiler::CodeAssemblerState* state_, TNode<CachedTemplateObject> p_o, TNode<Smi> p_v);
TNode<JSArray> LoadCachedTemplateObjectTemplateObject_0(compiler::CodeAssemblerState* state_, TNode<CachedTemplateObject> p_o);
void StoreCachedTemplateObjectTemplateObject_0(compiler::CodeAssemblerState* state_, TNode<CachedTemplateObject> p_o, TNode<JSArray> p_v);
TNode<HeapObject> LoadCachedTemplateObjectNext_0(compiler::CodeAssemblerState* state_, TNode<CachedTemplateObject> p_o);
void StoreCachedTemplateObjectNext_0(compiler::CodeAssemblerState* state_, TNode<CachedTemplateObject> p_o, TNode<HeapObject> p_v);
TNode<FixedArray> LoadTemplateObjectDescriptionRawStrings_0(compiler::CodeAssemblerState* state_, TNode<TemplateObjectDescription> p_o);
void StoreTemplateObjectDescriptionRawStrings_0(compiler::CodeAssemblerState* state_, TNode<TemplateObjectDescription> p_o, TNode<FixedArray> p_v);
TNode<FixedArray> LoadTemplateObjectDescriptionCookedStrings_0(compiler::CodeAssemblerState* state_, TNode<TemplateObjectDescription> p_o);
void StoreTemplateObjectDescriptionCookedStrings_0(compiler::CodeAssemblerState* state_, TNode<TemplateObjectDescription> p_o, TNode<FixedArray> p_v);
TNode<CachedTemplateObject> DownCastForTorqueClass_CachedTemplateObject_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<TemplateObjectDescription> DownCastForTorqueClass_TemplateObjectDescription_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_TEMPLATE_OBJECTS_TQ_CSA_H_
