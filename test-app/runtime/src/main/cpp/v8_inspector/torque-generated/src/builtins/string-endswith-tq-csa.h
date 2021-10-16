#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_STRING_ENDSWITH_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_STRING_ENDSWITH_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<BoolT> IsSubstringAt_0(compiler::CodeAssemblerState* state_, TNode<String> p_string, TNode<String> p_searchStr, TNode<IntPtrT> p_start);
TNode<BoolT> TwoStringsToSlices_bool_IsSubstringAtFunctor_0(compiler::CodeAssemblerState* state_, TNode<String> p_s1, TNode<String> p_s2, TorqueStructIsSubstringAtFunctor_0 p_f);
TNode<BoolT> IsSubstringAt_char8_char8_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char8_ConstReference_char8_0 p_string, TorqueStructSlice_char8_ConstReference_char8_0 p_searchStr, TNode<IntPtrT> p_start);
TNode<BoolT> IsSubstringAt_char8_char16_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char8_ConstReference_char8_0 p_string, TorqueStructSlice_char16_ConstReference_char16_0 p_searchStr, TNode<IntPtrT> p_start);
TNode<BoolT> IsSubstringAt_char16_char8_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char16_ConstReference_char16_0 p_string, TorqueStructSlice_char8_ConstReference_char8_0 p_searchStr, TNode<IntPtrT> p_start);
TNode<BoolT> IsSubstringAt_char16_char16_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char16_ConstReference_char16_0 p_string, TorqueStructSlice_char16_ConstReference_char16_0 p_searchStr, TNode<IntPtrT> p_start);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_STRING_ENDSWITH_TQ_CSA_H_
