#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_REGEXP_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_REGEXP_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<JSRegExp> Cast_JSRegExp_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<HeapObject> LoadJSRegExpData_0(compiler::CodeAssemblerState* state_, TNode<JSRegExp> p_o);
void StoreJSRegExpData_0(compiler::CodeAssemblerState* state_, TNode<JSRegExp> p_o, TNode<HeapObject> p_v);
TNode<PrimitiveHeapObject> LoadJSRegExpSource_0(compiler::CodeAssemblerState* state_, TNode<JSRegExp> p_o);
void StoreJSRegExpSource_0(compiler::CodeAssemblerState* state_, TNode<JSRegExp> p_o, TNode<PrimitiveHeapObject> p_v);
TNode<Object> LoadJSRegExpFlags_0(compiler::CodeAssemblerState* state_, TNode<JSRegExp> p_o);
void StoreJSRegExpFlags_0(compiler::CodeAssemblerState* state_, TNode<JSRegExp> p_o, TNode<Object> p_v);
TNode<Object> LoadJSRegExpResultIndex_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o);
void StoreJSRegExpResultIndex_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o, TNode<Object> p_v);
TNode<Object> LoadJSRegExpResultInput_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o);
void StoreJSRegExpResultInput_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o, TNode<Object> p_v);
TNode<Object> LoadJSRegExpResultGroups_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o);
void StoreJSRegExpResultGroups_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o, TNode<Object> p_v);
TNode<HeapObject> LoadJSRegExpResultNames_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o);
void StoreJSRegExpResultNames_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o, TNode<HeapObject> p_v);
TNode<String> LoadJSRegExpResultRegexpInput_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o);
void StoreJSRegExpResultRegexpInput_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o, TNode<String> p_v);
TNode<Smi> LoadJSRegExpResultRegexpLastIndex_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o);
void StoreJSRegExpResultRegexpLastIndex_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o, TNode<Smi> p_v);
TNode<Object> LoadJSRegExpResultWithIndicesIndices_0(compiler::CodeAssemblerState* state_, TNode<JSRegExpResult> p_o);
void StoreJSRegExpResultWithIndicesIndices_0(compiler::CodeAssemblerState* state_, TNode<JSRegExpResult> p_o, TNode<Object> p_v);
TNode<Object> LoadJSRegExpResultIndicesGroups_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o);
void StoreJSRegExpResultIndicesGroups_0(compiler::CodeAssemblerState* state_, TNode<JSArray> p_o, TNode<Object> p_v);
TNode<JSRegExp> DownCastForTorqueClass_JSRegExp_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_REGEXP_TQ_CSA_H_
