#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_DEBUG_OBJECTS_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_DEBUG_OBJECTS_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<BreakPoint> Cast_BreakPoint_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<BreakPointInfo> Cast_BreakPointInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<DebugInfo> Cast_DebugInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<CoverageInfo> Cast_CoverageInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<Smi> LoadBreakPointId_0(compiler::CodeAssemblerState* state_, TNode<BreakPoint> p_o);
void StoreBreakPointId_0(compiler::CodeAssemblerState* state_, TNode<BreakPoint> p_o, TNode<Smi> p_v);
TNode<String> LoadBreakPointCondition_0(compiler::CodeAssemblerState* state_, TNode<BreakPoint> p_o);
void StoreBreakPointCondition_0(compiler::CodeAssemblerState* state_, TNode<BreakPoint> p_o, TNode<String> p_v);
TNode<Smi> LoadBreakPointInfoSourcePosition_0(compiler::CodeAssemblerState* state_, TNode<BreakPointInfo> p_o);
void StoreBreakPointInfoSourcePosition_0(compiler::CodeAssemblerState* state_, TNode<BreakPointInfo> p_o, TNode<Smi> p_v);
TNode<HeapObject> LoadBreakPointInfoBreakPoints_0(compiler::CodeAssemblerState* state_, TNode<BreakPointInfo> p_o);
void StoreBreakPointInfoBreakPoints_0(compiler::CodeAssemblerState* state_, TNode<BreakPointInfo> p_o, TNode<HeapObject> p_v);
TNode<SharedFunctionInfo> LoadDebugInfoShared_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o);
void StoreDebugInfoShared_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o, TNode<SharedFunctionInfo> p_v);
TNode<Smi> LoadDebugInfoDebuggerHints_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o);
void StoreDebugInfoDebuggerHints_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o, TNode<Smi> p_v);
TNode<HeapObject> LoadDebugInfoScript_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o);
void StoreDebugInfoScript_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadDebugInfoOriginalBytecodeArray_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o);
void StoreDebugInfoOriginalBytecodeArray_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o, TNode<HeapObject> p_v);
TNode<HeapObject> LoadDebugInfoDebugBytecodeArray_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o);
void StoreDebugInfoDebugBytecodeArray_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o, TNode<HeapObject> p_v);
TNode<FixedArray> LoadDebugInfoBreakPoints_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o);
void StoreDebugInfoBreakPoints_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o, TNode<FixedArray> p_v);
TNode<Smi> LoadDebugInfoFlags_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o);
void StoreDebugInfoFlags_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o, TNode<Smi> p_v);
TNode<HeapObject> LoadDebugInfoCoverageInfo_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o);
void StoreDebugInfoCoverageInfo_0(compiler::CodeAssemblerState* state_, TNode<DebugInfo> p_o, TNode<HeapObject> p_v);
TNode<Int32T> LoadCoverageInfoSlotCount_0(compiler::CodeAssemblerState* state_, TNode<CoverageInfo> p_o);
TorqueStructSlice_CoverageInfoSlot_MutableReference_CoverageInfoSlot_0 FieldSliceCoverageInfoSlots_0(compiler::CodeAssemblerState* state_, TNode<CoverageInfo> p_o);
TNode<BreakPoint> DownCastForTorqueClass_BreakPoint_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<BreakPointInfo> DownCastForTorqueClass_BreakPointInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<DebugInfo> DownCastForTorqueClass_DebugInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<CoverageInfo> DownCastForTorqueClass_CoverageInfo_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
TorqueStructSlice_CoverageInfoSlot_MutableReference_CoverageInfoSlot_0 NewMutableSlice_CoverageInfoSlot_0(compiler::CodeAssemblerState* state_, TNode<Object> p_object, TNode<IntPtrT> p_offset, TNode<IntPtrT> p_length);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_DEBUG_OBJECTS_TQ_CSA_H_
