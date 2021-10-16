#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_WEAK_REFS_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_WEAK_REFS_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<JSFinalizationRegistry> Cast_JSFinalizationRegistry_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<WeakCell> Cast_WeakCell_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<JSWeakRef> Cast_JSWeakRef_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<HeapObject> LoadWeakCellFinalizationRegistry_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o);
void StoreWeakCellFinalizationRegistry_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadWeakCellTarget_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o);
void StoreWeakCellTarget_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadWeakCellUnregisterToken_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o);
void StoreWeakCellUnregisterToken_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o, TNode<HeapObject> p_v);
TNode<Object> LoadWeakCellHoldings_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o);
void StoreWeakCellHoldings_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o, TNode<Object> p_v);
TNode<HeapObject> LoadWeakCellPrev_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o);
void StoreWeakCellPrev_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadWeakCellNext_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o);
void StoreWeakCellNext_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadWeakCellKeyListPrev_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o);
void StoreWeakCellKeyListPrev_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadWeakCellKeyListNext_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o);
void StoreWeakCellKeyListNext_0(compiler::CodeAssemblerState* state_, TNode<WeakCell> p_o, TNode<HeapObject> p_v);
TNode<NativeContext> LoadJSFinalizationRegistryNativeContext_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o);
void StoreJSFinalizationRegistryNativeContext_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o, TNode<NativeContext> p_v);
TNode<JSReceiver> LoadJSFinalizationRegistryCleanup_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o);
void StoreJSFinalizationRegistryCleanup_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o, TNode<JSReceiver> p_v);
TNode<HeapObject> LoadJSFinalizationRegistryActiveCells_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o);
void StoreJSFinalizationRegistryActiveCells_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadJSFinalizationRegistryClearedCells_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o);
void StoreJSFinalizationRegistryClearedCells_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o, TNode<HeapObject> p_v);
TNode<Object> LoadJSFinalizationRegistryKeyMap_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o);
void StoreJSFinalizationRegistryKeyMap_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o, TNode<Object> p_v);
TNode<HeapObject> LoadJSFinalizationRegistryNextDirty_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o);
void StoreJSFinalizationRegistryNextDirty_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o, TNode<HeapObject> p_v);
TNode<Smi> LoadJSFinalizationRegistryFlags_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o);
void StoreJSFinalizationRegistryFlags_0(compiler::CodeAssemblerState* state_, TNode<JSFinalizationRegistry> p_o, TNode<Smi> p_v);
TNode<HeapObject> LoadJSWeakRefTarget_0(compiler::CodeAssemblerState* state_, TNode<JSWeakRef> p_o);
void StoreJSWeakRefTarget_0(compiler::CodeAssemblerState* state_, TNode<JSWeakRef> p_o, TNode<HeapObject> p_v);
TNode<JSFinalizationRegistry> DownCastForTorqueClass_JSFinalizationRegistry_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<WeakCell> DownCastForTorqueClass_WeakCell_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<JSWeakRef> DownCastForTorqueClass_JSWeakRef_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_WEAK_REFS_TQ_CSA_H_
