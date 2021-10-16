#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_REGEXP_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_REGEXP_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<BoolT> IsFastRegExpForMatch_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_o);
TNode<BoolT> IsFastRegExpForSearch_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_o);
TNode<BoolT> IsFastRegExpStrict_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_o);
TNode<BoolT> IsFastRegExpPermissive_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_o);
TNode<Object> RegExpExec_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_receiver, TNode<String> p_string);
TNode<RegExpMatchInfo> RegExpPrototypeExecBodyWithoutResult_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSRegExp> p_regexp, TNode<String> p_string, TNode<Number> p_regexpLastIndex, bool p_isFastPath, compiler::CodeAssemblerLabel* label_IfDidNotMatch);
TNode<RegExpMatchInfo> RegExpPrototypeExecBodyWithoutResultFast_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSRegExp> p_regexp, TNode<String> p_string, compiler::CodeAssemblerLabel* label_IfDidNotMatch);
TNode<RegExpMatchInfo> RegExpPrototypeExecBodyWithoutResultFast_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSRegExp> p_regexp, TNode<String> p_string, TNode<Number> p_lastIndex, compiler::CodeAssemblerLabel* label_IfDidNotMatch);
TNode<Object> RegExpPrototypeExecBody_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_receiver, TNode<String> p_string, bool p_isFastPath);
TNode<JSFunction> LoadRegExpFunction_0(compiler::CodeAssemblerState* state_, TNode<NativeContext> p_nativeContext);
TNode<BoolT> HasInitialRegExpMap_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_o);
TNode<BoolT> IsReceiverInitialRegExpPrototype_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver);
int31_t kNoCounterFlagGetter_0(compiler::CodeAssemblerState* state_);
TNode<Object> FlagGetter_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, JSRegExp::Flag p_flag, int31_t p_counter, const char* p_methodName);
TNode<String> FastFlagsGetter_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSRegExp> p_receiver);
TNode<String> SlowFlagsGetter_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver);
TNode<Object> LoadLastIndex_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_regexp, bool p_isFastPath);
TNode<Number> LoadLastIndexAsLength_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSRegExp> p_regexp, bool p_isFastPath);
void StoreLastIndex_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_regexp, TNode<Number> p_value, bool p_isFastPath);
TNode<BoolT> IsRegExp_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_obj);
TNode<Object> RegExpCreate_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<Object> p_maybeString, TNode<String> p_flags);
TNode<Object> RegExpCreate_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Map> p_initialMap, TNode<Object> p_maybeString, TNode<String> p_flags);
TNode<JSRegExp> Cast_FastJSRegExp_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o, compiler::CodeAssemblerLabel* label_CastError);
TNode<Smi> UnsafeCast_PositiveSmi_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TNode<BoolT> Is_JSRegExp_JSReceiver_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_o);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_REGEXP_TQ_CSA_H_
