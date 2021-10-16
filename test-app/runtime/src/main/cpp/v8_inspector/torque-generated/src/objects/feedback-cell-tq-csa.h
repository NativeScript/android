#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_FEEDBACK_CELL_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_FEEDBACK_CELL_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<FeedbackCell> Cast_FeedbackCell_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<HeapObject> LoadFeedbackCellValue_0(compiler::CodeAssemblerState* state_, TNode<FeedbackCell> p_o);
void StoreFeedbackCellValue_0(compiler::CodeAssemblerState* state_, TNode<FeedbackCell> p_o, TNode<HeapObject> p_v);
TNode<Int32T> LoadFeedbackCellInterruptBudget_0(compiler::CodeAssemblerState* state_, TNode<FeedbackCell> p_o);
void StoreFeedbackCellInterruptBudget_0(compiler::CodeAssemblerState* state_, TNode<FeedbackCell> p_o, TNode<Int32T> p_v);
TNode<FeedbackCell> DownCastForTorqueClass_FeedbackCell_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_FEEDBACK_CELL_TQ_CSA_H_
