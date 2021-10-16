#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_STRING_TRIM_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_STRING_TRIM_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Uint32T> FromConstexpr_TrimMode_constexpr_kTrim_0(compiler::CodeAssemblerState* state_, String::TrimMode p_o);
TNode<Uint32T> FromConstexpr_TrimMode_constexpr_kTrimStart_0(compiler::CodeAssemblerState* state_, String::TrimMode p_o);
TNode<Uint32T> FromConstexpr_TrimMode_constexpr_kTrimEnd_0(compiler::CodeAssemblerState* state_, String::TrimMode p_o);
TNode<BoolT> IsWhiteSpaceOrLineTerminator_0(compiler::CodeAssemblerState* state_, TNode<Uint16T> p_charCode);
TNode<String> StringTrim_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, TorqueStructArguments p__arguments, const char* p_methodName, String::TrimMode p_variant);
TNode<String> StringTrimBody_char8_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<String> p_string, TorqueStructSlice_char8_ConstReference_char8_0 p_slice, String::TrimMode p_variant);
TNode<String> StringTrimBody_char16_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<String> p_string, TorqueStructSlice_char16_ConstReference_char16_0 p_slice, String::TrimMode p_variant);
TNode<IntPtrT> StringTrimLoop_char8_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TorqueStructSlice_char8_ConstReference_char8_0 p_stringSlice, TNode<IntPtrT> p_startIndex, TNode<IntPtrT> p_endIndex, TNode<IntPtrT> p_increment);
TNode<IntPtrT> StringTrimLoop_char16_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TorqueStructSlice_char16_ConstReference_char16_0 p_stringSlice, TNode<IntPtrT> p_startIndex, TNode<IntPtrT> p_endIndex, TNode<IntPtrT> p_increment);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_STRING_TRIM_TQ_CSA_H_
