#ifndef V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_REGEXP_STRING_ITERATOR_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_REGEXP_STRING_ITERATOR_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<JSRegExpStringIterator> Cast_JSRegExpStringIterator_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError);
TNode<JSReceiver> LoadJSRegExpStringIteratorIteratingRegExp_0(compiler::CodeAssemblerState* state_, TNode<JSRegExpStringIterator> p_o);
void StoreJSRegExpStringIteratorIteratingRegExp_0(compiler::CodeAssemblerState* state_, TNode<JSRegExpStringIterator> p_o, TNode<JSReceiver> p_v);
TNode<String> LoadJSRegExpStringIteratorIteratedString_0(compiler::CodeAssemblerState* state_, TNode<JSRegExpStringIterator> p_o);
void StoreJSRegExpStringIteratorIteratedString_0(compiler::CodeAssemblerState* state_, TNode<JSRegExpStringIterator> p_o, TNode<String> p_v);
TNode<Smi> LoadJSRegExpStringIteratorFlags_0(compiler::CodeAssemblerState* state_, TNode<JSRegExpStringIterator> p_o);
void StoreJSRegExpStringIteratorFlags_0(compiler::CodeAssemblerState* state_, TNode<JSRegExpStringIterator> p_o, TNode<Smi> p_v);
TNode<JSRegExpStringIterator> DownCastForTorqueClass_JSRegExpStringIterator_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_OBJECTS_JS_REGEXP_STRING_ITERATOR_TQ_CSA_H_
