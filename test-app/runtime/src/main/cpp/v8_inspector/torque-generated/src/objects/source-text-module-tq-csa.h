#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SOURCE_TEXT_MODULE_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SOURCE_TEXT_MODULE_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<SourceTextModule> Cast_SourceTextModule_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<ModuleRequest> Cast_ModuleRequest_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<SourceTextModuleInfoEntry> Cast_SourceTextModuleInfoEntry_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<HeapObject> LoadSourceTextModuleCode_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleCode_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<HeapObject> p_v);
TNode<FixedArray> LoadSourceTextModuleRegularExports_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleRegularExports_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<FixedArray> p_v);
TNode<FixedArray> LoadSourceTextModuleRegularImports_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleRegularImports_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<FixedArray> p_v);
TNode<FixedArray> LoadSourceTextModuleRequestedModules_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleRequestedModules_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<FixedArray> p_v);
TNode<HeapObject> LoadSourceTextModuleImportMeta_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleImportMeta_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadSourceTextModuleCycleRoot_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleCycleRoot_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<HeapObject> p_v);
TNode<ArrayList> LoadSourceTextModuleAsyncParentModules_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleAsyncParentModules_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<ArrayList> p_v);
TNode<Smi> LoadSourceTextModuleDfsIndex_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleDfsIndex_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<Smi> p_v);
TNode<Smi> LoadSourceTextModuleDfsAncestorIndex_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleDfsAncestorIndex_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<Smi> p_v);
TNode<Smi> LoadSourceTextModulePendingAsyncDependencies_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModulePendingAsyncDependencies_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<Smi> p_v);
TNode<Smi> LoadSourceTextModuleFlags_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o);
void StoreSourceTextModuleFlags_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModule> p_o, TNode<Smi> p_v);
TNode<String> LoadModuleRequestSpecifier_0(compiler::CodeAssemblerState* state_, TNode<ModuleRequest> p_o);
void StoreModuleRequestSpecifier_0(compiler::CodeAssemblerState* state_, TNode<ModuleRequest> p_o, TNode<String> p_v);
TNode<FixedArray> LoadModuleRequestImportAssertions_0(compiler::CodeAssemblerState* state_, TNode<ModuleRequest> p_o);
void StoreModuleRequestImportAssertions_0(compiler::CodeAssemblerState* state_, TNode<ModuleRequest> p_o, TNode<FixedArray> p_v);
TNode<Smi> LoadModuleRequestPosition_0(compiler::CodeAssemblerState* state_, TNode<ModuleRequest> p_o);
void StoreModuleRequestPosition_0(compiler::CodeAssemblerState* state_, TNode<ModuleRequest> p_o, TNode<Smi> p_v);
TNode<PrimitiveHeapObject> LoadSourceTextModuleInfoEntryExportName_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o);
void StoreSourceTextModuleInfoEntryExportName_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o, TNode<PrimitiveHeapObject> p_v);
TNode<PrimitiveHeapObject> LoadSourceTextModuleInfoEntryLocalName_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o);
void StoreSourceTextModuleInfoEntryLocalName_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o, TNode<PrimitiveHeapObject> p_v);
TNode<PrimitiveHeapObject> LoadSourceTextModuleInfoEntryImportName_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o);
void StoreSourceTextModuleInfoEntryImportName_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o, TNode<PrimitiveHeapObject> p_v);
TNode<Smi> LoadSourceTextModuleInfoEntryModuleRequest_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o);
void StoreSourceTextModuleInfoEntryModuleRequest_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o, TNode<Smi> p_v);
TNode<Smi> LoadSourceTextModuleInfoEntryCellIndex_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o);
void StoreSourceTextModuleInfoEntryCellIndex_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o, TNode<Smi> p_v);
TNode<Smi> LoadSourceTextModuleInfoEntryBegPos_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o);
void StoreSourceTextModuleInfoEntryBegPos_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o, TNode<Smi> p_v);
TNode<Smi> LoadSourceTextModuleInfoEntryEndPos_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o);
void StoreSourceTextModuleInfoEntryEndPos_0(compiler::CodeAssemblerState* state_, TNode<SourceTextModuleInfoEntry> p_o, TNode<Smi> p_v);
TNode<SourceTextModule> DownCastForTorqueClass_SourceTextModule_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<ModuleRequest> DownCastForTorqueClass_ModuleRequest_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<SourceTextModuleInfoEntry> DownCastForTorqueClass_SourceTextModuleInfoEntry_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_SOURCE_TEXT_MODULE_TQ_CSA_H_
