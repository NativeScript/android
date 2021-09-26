#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_REGEXP_MATCH_INFO_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_REGEXP_MATCH_INFO_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
int31_t GetStartOfCaptureIndex_0(compiler::CodeAssemblerState* state_, int31_t p_captureIndex);
TNode<Smi> Method_RegExpMatchInfo_GetStartOfCapture_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<RegExpMatchInfo> p_this, int31_t p_captureIndex);
TNode<Smi> Method_RegExpMatchInfo_GetEndOfCapture_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<RegExpMatchInfo> p_this, int31_t p_captureIndex);
TNode<Smi> Method_RegExpMatchInfo_NumberOfCaptures_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<RegExpMatchInfo> p_this);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_REGEXP_MATCH_INFO_TQ_CSA_H_
