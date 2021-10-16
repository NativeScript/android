#ifndef V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_NUMBER_TQ_CSA_H_
#define V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_NUMBER_TQ_CSA_H_
#include "src/builtins/torque-csa-header-includes.h"

namespace v8 {
namespace internal {
TNode<Uint32T> FromConstexpr_Operation_constexpr_kAdd_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kSubtract_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kMultiply_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kDivide_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kModulus_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kExponentiate_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kBitwiseAnd_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kBitwiseOr_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kBitwiseXor_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kShiftLeft_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kShiftRight_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kShiftRightLogical_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kBitwiseNot_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kNegate_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kIncrement_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kDecrement_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kEqual_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kStrictEqual_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kLessThan_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kLessThanOrEqual_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kGreaterThan_0(compiler::CodeAssemblerState* state_, Operation p_o);
TNode<Uint32T> FromConstexpr_Operation_constexpr_kGreaterThanOrEqual_0(compiler::CodeAssemblerState* state_, Operation p_o);
int32_t kAsciiZero_0(compiler::CodeAssemblerState* state_);
int32_t kAsciiLowerCaseA_0(compiler::CodeAssemblerState* state_);
TNode<Number> ThisNumberValue_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, const char* p_method);
TNode<Uint8T> ToCharCode_0(compiler::CodeAssemblerState* state_, TNode<Int32T> p_input);
TNode<String> NumberToStringSmi_0(compiler::CodeAssemblerState* state_, TNode<Int32T> p_x, TNode<Int32T> p_radix, compiler::CodeAssemblerLabel* label_Slow);
TNode<Object> ToNumericOrPrimitive_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_value);
void UnaryOp1_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_value, compiler::CodeAssemblerLabel* label_Number, compiler::TypedCodeAssemblerVariable<Number>* label_Number_parameter_0, compiler::CodeAssemblerLabel* label_BigInt, compiler::TypedCodeAssemblerVariable<BigInt>* label_BigInt_parameter_0);
void UnaryOp2_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_value, compiler::CodeAssemblerLabel* label_Smi, compiler::TypedCodeAssemblerVariable<Smi>* label_Smi_parameter_0, compiler::CodeAssemblerLabel* label_HeapNumber, compiler::TypedCodeAssemblerVariable<HeapNumber>* label_HeapNumber_parameter_0, compiler::CodeAssemblerLabel* label_BigInt, compiler::TypedCodeAssemblerVariable<BigInt>* label_BigInt_parameter_0);
void BinaryOp1_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_leftVal, TNode<Object> p_rightVal, compiler::CodeAssemblerLabel* label_Number, compiler::TypedCodeAssemblerVariable<Number>* label_Number_parameter_0, compiler::TypedCodeAssemblerVariable<Number>* label_Number_parameter_1, compiler::CodeAssemblerLabel* label_AtLeastOneBigInt, compiler::TypedCodeAssemblerVariable<Numeric>* label_AtLeastOneBigInt_parameter_0, compiler::TypedCodeAssemblerVariable<Numeric>* label_AtLeastOneBigInt_parameter_1);
void BinaryOp2_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_leftVal, TNode<Object> p_rightVal, compiler::CodeAssemblerLabel* label_Smis, compiler::TypedCodeAssemblerVariable<Smi>* label_Smis_parameter_0, compiler::TypedCodeAssemblerVariable<Smi>* label_Smis_parameter_1, compiler::CodeAssemblerLabel* label_Float64s, compiler::TypedCodeAssemblerVariable<Float64T>* label_Float64s_parameter_0, compiler::TypedCodeAssemblerVariable<Float64T>* label_Float64s_parameter_1, compiler::CodeAssemblerLabel* label_AtLeastOneBigInt, compiler::TypedCodeAssemblerVariable<Numeric>* label_AtLeastOneBigInt_parameter_0, compiler::TypedCodeAssemblerVariable<Numeric>* label_AtLeastOneBigInt_parameter_1);
TNode<Int32T> kMinimumDividend_0(compiler::CodeAssemblerState* state_);
TNode<Number> UnsafeCast_Number_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o);
TorqueStructReference_char8_0 UnsafeConstCast_char8_0(compiler::CodeAssemblerState* state_, TorqueStructReference_char8_0 p_r);
TNode<Smi> SmiTag_Operation_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_value);
} // namespace internal
} // namespace v8
#endif // V8_GEN_TORQUE_GENERATED_SRC_BUILTINS_NUMBER_TQ_CSA_H_
