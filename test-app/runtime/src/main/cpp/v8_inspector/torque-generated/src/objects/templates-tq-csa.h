#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_TEMPLATES_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_TEMPLATES_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<TemplateInfo> Cast_TemplateInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<FunctionTemplateRareData> Cast_FunctionTemplateRareData_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<FunctionTemplateInfo> Cast_FunctionTemplateInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<ObjectTemplateInfo> Cast_ObjectTemplateInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Smi> LoadTemplateInfoTag_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o);
void StoreTemplateInfoTag_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o, TNode<Smi> p_v);
TNode<Smi> LoadTemplateInfoSerialNumber_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o);
void StoreTemplateInfoSerialNumber_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o, TNode<Smi> p_v);
TNode<Smi> LoadTemplateInfoNumberOfProperties_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o);
void StoreTemplateInfoNumberOfProperties_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o, TNode<Smi> p_v);
TNode<HeapObject> LoadTemplateInfoPropertyList_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o);
void StoreTemplateInfoPropertyList_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadTemplateInfoPropertyAccessors_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o);
void StoreTemplateInfoPropertyAccessors_0(compiler::CodeAssemblerState* state_, TNode<TemplateInfo> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateRareDataPrototypeTemplate_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o);
void StoreFunctionTemplateRareDataPrototypeTemplate_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateRareDataPrototypeProviderTemplate_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o);
void StoreFunctionTemplateRareDataPrototypeProviderTemplate_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateRareDataParentTemplate_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o);
void StoreFunctionTemplateRareDataParentTemplate_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateRareDataNamedPropertyHandler_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o);
void StoreFunctionTemplateRareDataNamedPropertyHandler_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateRareDataIndexedPropertyHandler_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o);
void StoreFunctionTemplateRareDataIndexedPropertyHandler_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateRareDataInstanceTemplate_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o);
void StoreFunctionTemplateRareDataInstanceTemplate_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateRareDataInstanceCallHandler_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o);
void StoreFunctionTemplateRareDataInstanceCallHandler_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateRareDataAccessCheckInfo_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o);
void StoreFunctionTemplateRareDataAccessCheckInfo_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o, TNode<HeapObject> p_v);
TNode<FixedArray> LoadFunctionTemplateRareDataCFunctionOverloads_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o);
void StoreFunctionTemplateRareDataCFunctionOverloads_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateRareData> p_o, TNode<FixedArray> p_v);
TNode<HeapObject> LoadFunctionTemplateInfoCallCode_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o);
void StoreFunctionTemplateInfoCallCode_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o, TNode<HeapObject> p_v);
TNode<PrimitiveHeapObject> LoadFunctionTemplateInfoClassName_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o);
void StoreFunctionTemplateInfoClassName_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o, TNode<PrimitiveHeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateInfoSignature_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o);
void StoreFunctionTemplateInfoSignature_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateInfoRareData_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o);
void StoreFunctionTemplateInfoRareData_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadFunctionTemplateInfoSharedFunctionInfo_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o);
void StoreFunctionTemplateInfoSharedFunctionInfo_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o, TNode<HeapObject> p_v);
TNode<Smi> LoadFunctionTemplateInfoFlag_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o);
void StoreFunctionTemplateInfoFlag_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o, TNode<Smi> p_v);
TNode<Smi> LoadFunctionTemplateInfoLength_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o);
void StoreFunctionTemplateInfoLength_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o, TNode<Smi> p_v);
TNode<Object> LoadFunctionTemplateInfoCachedPropertyName_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o);
void StoreFunctionTemplateInfoCachedPropertyName_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o, TNode<Object> p_v);
TNode<Smi> LoadFunctionTemplateInfoInstanceType_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o);
void StoreFunctionTemplateInfoInstanceType_0(compiler::CodeAssemblerState* state_, TNode<FunctionTemplateInfo> p_o, TNode<Smi> p_v);
TNode<HeapObject> LoadObjectTemplateInfoConstructor_0(compiler::CodeAssemblerState* state_, TNode<ObjectTemplateInfo> p_o);
void StoreObjectTemplateInfoConstructor_0(compiler::CodeAssemblerState* state_, TNode<ObjectTemplateInfo> p_o, TNode<HeapObject> p_v);
TNode<Smi> LoadObjectTemplateInfoData_0(compiler::CodeAssemblerState* state_, TNode<ObjectTemplateInfo> p_o);
void StoreObjectTemplateInfoData_0(compiler::CodeAssemblerState* state_, TNode<ObjectTemplateInfo> p_o, TNode<Smi> p_v);
TNode<TemplateInfo> DownCastForTorqueClass_TemplateInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<FunctionTemplateRareData> DownCastForTorqueClass_FunctionTemplateRareData_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<FunctionTemplateInfo> DownCastForTorqueClass_FunctionTemplateInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<ObjectTemplateInfo> DownCastForTorqueClass_ObjectTemplateInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_TEMPLATES_TQ_CSA_H_
