#include "src/ast/ast.h"
#include "src/builtins/builtins-array-gen.h"
#include "src/builtins/builtins-bigint-gen.h"
#include "src/builtins/builtins-collections-gen.h"
#include "src/builtins/builtins-constructor-gen.h"
#include "src/builtins/builtins-data-view-gen.h"
#include "src/builtins/builtins-iterator-gen.h"
#include "src/builtins/builtins-promise-gen.h"
#include "src/builtins/builtins-promise.h"
#include "src/builtins/builtins-proxy-gen.h"
#include "src/builtins/builtins-regexp-gen.h"
#include "src/builtins/builtins-string-gen.h"
#include "src/builtins/builtins-typed-array-gen.h"
#include "src/builtins/builtins-utils-gen.h"
#include "src/builtins/builtins.h"
#include "src/codegen/code-factory.h"
#include "src/heap/factory-inl.h"
#include "src/ic/binary-op-assembler.h"
#include "src/ic/handler-configuration-inl.h"
#include "src/objects/arguments.h"
#include "src/objects/bigint.h"
#include "src/objects/elements-kind.h"
#include "src/objects/free-space.h"
#include "src/objects/js-function.h"
#include "src/objects/js-generator.h"
#include "src/objects/js-objects.h"
#include "src/objects/js-promise.h"
#include "src/objects/js-regexp-string-iterator.h"
#include "src/objects/js-weak-refs.h"
#include "src/objects/objects.h"
#include "src/objects/ordered-hash-table.h"
#include "src/objects/property-array.h"
#include "src/objects/property-descriptor-object.h"
#include "src/objects/source-text-module.h"
#include "src/objects/stack-frame-info.h"
#include "src/objects/swiss-hash-table-helpers.h"
#include "src/objects/swiss-name-dictionary.h"
#include "src/objects/synthetic-module.h"
#include "src/objects/template-objects.h"
#include "src/objects/torque-defined-classes.h"
#include "src/torque/runtime-support.h"
// Required Builtins:
#include "torque-generated/src/builtins/string-trim-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/builtins-string-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/string-trim-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/string-tq-csa.h"

namespace v8 {
namespace internal {
TNode<Uint32T> FromConstexpr_TrimMode_constexpr_kTrim_0(compiler::CodeAssemblerState* state_, String::TrimMode p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Uint32T> tmp0;
  TNode<Uint32T> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ca_.Uint32Constant(CastToUnderlyingTypeIfEnum(p_o));
    tmp1 = (TNode<Uint32T>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Uint32T>{tmp1};
}


TNode<Uint32T> FromConstexpr_TrimMode_constexpr_kTrimStart_0(compiler::CodeAssemblerState* state_, String::TrimMode p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Uint32T> tmp0;
  TNode<Uint32T> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ca_.Uint32Constant(CastToUnderlyingTypeIfEnum(p_o));
    tmp1 = (TNode<Uint32T>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Uint32T>{tmp1};
}


TNode<Uint32T> FromConstexpr_TrimMode_constexpr_kTrimEnd_0(compiler::CodeAssemblerState* state_, String::TrimMode p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Uint32T> tmp0;
  TNode<Uint32T> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ca_.Uint32Constant(CastToUnderlyingTypeIfEnum(p_o));
    tmp1 = (TNode<Uint32T>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Uint32T>{tmp1};
}


TNode<BoolT> IsWhiteSpaceOrLineTerminator_0(compiler::CodeAssemblerState* state_, TNode<Uint16T> p_charCode) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Uint32T> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_uint32_constexpr_int31_0(state_, 32);
    tmp1 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp0});
    ca_.Branch(tmp1, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp2;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp2 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp2);
  }

  TNode<Int32T> tmp3;
  TNode<Int32T> tmp4;
  TNode<Int32T> tmp5;
  TNode<Uint32T> tmp6;
  TNode<Uint32T> tmp7;
  TNode<BoolT> tmp8;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = Convert_int32_char8_OR_char16_0(state_, TNode<Uint16T>{p_charCode});
    tmp4 = FromConstexpr_int32_constexpr_int31_0(state_, 14);
    tmp5 = CodeStubAssembler(state_).Int32Sub(TNode<Int32T>{tmp3}, TNode<Int32T>{tmp4});
    tmp6 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp5});
    tmp7 = FromConstexpr_uint32_constexpr_int31_0(state_, 146);
    tmp8 = CodeStubAssembler(state_).Uint32LessThan(TNode<Uint32T>{tmp6}, TNode<Uint32T>{tmp7});
    ca_.Branch(tmp8, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp9;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp9 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp9);
  }

  TNode<Uint32T> tmp10;
  TNode<BoolT> tmp11;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp10 = FromConstexpr_uint32_constexpr_int31_0(state_, 9);
    tmp11 = CodeStubAssembler(state_).Uint32LessThan(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp10});
    ca_.Branch(tmp11, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp12;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp12 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp12);
  }

  TNode<Uint32T> tmp13;
  TNode<BoolT> tmp14;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp13 = FromConstexpr_uint32_constexpr_int31_0(state_, 13);
    tmp14 = CodeStubAssembler(state_).Uint32LessThanOrEqual(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp13});
    ca_.Branch(tmp14, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp15;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp15 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp15);
  }

  TNode<Uint32T> tmp16;
  TNode<BoolT> tmp17;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp16 = FromConstexpr_uint32_constexpr_int31_0(state_, 160);
    tmp17 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp16});
    ca_.Branch(tmp17, &block10, std::vector<compiler::Node*>{}, &block11, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp18;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp18 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp18);
  }

  TNode<Uint32T> tmp19;
  TNode<BoolT> tmp20;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp19 = FromConstexpr_uint32_constexpr_int31_0(state_, 5760);
    tmp20 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp19});
    ca_.Branch(tmp20, &block12, std::vector<compiler::Node*>{}, &block13, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp21;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp21 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp21);
  }

  TNode<Uint32T> tmp22;
  TNode<BoolT> tmp23;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp22 = FromConstexpr_uint32_constexpr_int31_0(state_, 8192);
    tmp23 = CodeStubAssembler(state_).Uint32LessThan(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp22});
    ca_.Branch(tmp23, &block14, std::vector<compiler::Node*>{}, &block15, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp24;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp24 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp24);
  }

  TNode<Uint32T> tmp25;
  TNode<BoolT> tmp26;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp25 = FromConstexpr_uint32_constexpr_int31_0(state_, 8202);
    tmp26 = CodeStubAssembler(state_).Uint32LessThanOrEqual(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp25});
    ca_.Branch(tmp26, &block16, std::vector<compiler::Node*>{}, &block17, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp27;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp27 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp27);
  }

  TNode<Uint32T> tmp28;
  TNode<BoolT> tmp29;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp28 = FromConstexpr_uint32_constexpr_int31_0(state_, 8232);
    tmp29 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp28});
    ca_.Branch(tmp29, &block18, std::vector<compiler::Node*>{}, &block19, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp30;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    tmp30 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp30);
  }

  TNode<Uint32T> tmp31;
  TNode<BoolT> tmp32;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    tmp31 = FromConstexpr_uint32_constexpr_int31_0(state_, 8233);
    tmp32 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp31});
    ca_.Branch(tmp32, &block20, std::vector<compiler::Node*>{}, &block21, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp33;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp33 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp33);
  }

  TNode<Uint32T> tmp34;
  TNode<BoolT> tmp35;
  if (block21.is_used()) {
    ca_.Bind(&block21);
    tmp34 = FromConstexpr_uint32_constexpr_int31_0(state_, 8239);
    tmp35 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp34});
    ca_.Branch(tmp35, &block22, std::vector<compiler::Node*>{}, &block23, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp36;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    tmp36 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp36);
  }

  TNode<Uint32T> tmp37;
  TNode<BoolT> tmp38;
  if (block23.is_used()) {
    ca_.Bind(&block23);
    tmp37 = FromConstexpr_uint32_constexpr_int31_0(state_, 8287);
    tmp38 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp37});
    ca_.Branch(tmp38, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp39;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp39 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp39);
  }

  TNode<Uint32T> tmp40;
  TNode<BoolT> tmp41;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp40 = FromConstexpr_uint32_constexpr_int31_0(state_, 65279);
    tmp41 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp40});
    ca_.Branch(tmp41, &block26, std::vector<compiler::Node*>{}, &block27, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp42;
  if (block26.is_used()) {
    ca_.Bind(&block26);
    tmp42 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp42);
  }

  TNode<Uint32T> tmp43;
  TNode<BoolT> tmp44;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp43 = FromConstexpr_uint32_constexpr_int31_0(state_, 12288);
    tmp44 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{p_charCode}, TNode<Uint32T>{tmp43});
    ca_.Branch(tmp44, &block28, std::vector<compiler::Node*>{}, &block29, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp45;
  if (block28.is_used()) {
    ca_.Bind(&block28);
    tmp45 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp45);
  }

  TNode<BoolT> tmp46;
  if (block29.is_used()) {
    ca_.Bind(&block29);
    tmp46 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp46);
  }

  TNode<BoolT> phi_bb1_1;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_1);
    ca_.Goto(&block30, phi_bb1_1);
  }

  TNode<BoolT> phi_bb30_1;
    ca_.Bind(&block30, &phi_bb30_1);
  return TNode<BoolT>{phi_bb30_1};
}


TNode<String> StringTrim_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, TorqueStructArguments p__arguments, const char* p_methodName, String::TrimMode p_variant) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, String> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, String> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, IntPtrT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, IntPtrT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block39(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, IntPtrT> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, IntPtrT> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block48(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, IntPtrT> block52(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String, IntPtrT> block51(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT, String> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, IntPtrT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block53(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<Int32T> tmp4;
  TNode<IntPtrT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_String_constexpr_string_0(state_, p_methodName);
    tmp1 = CodeStubAssembler(state_).ToThisString(TNode<Context>{p_context}, TNode<Object>{p_receiver}, TNode<String>{tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp4 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{tmp1, tmp3});
    tmp5 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp4});
    ca_.Goto(&block8, tmp1, tmp2);
  }

  TNode<String> phi_bb8_8;
  TNode<IntPtrT> phi_bb8_9;
  TNode<BoolT> tmp6;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_8, &phi_bb8_9);
    tmp6 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp6, &block6, std::vector<compiler::Node*>{phi_bb8_8, phi_bb8_9}, &block7, std::vector<compiler::Node*>{phi_bb8_8, phi_bb8_9});
  }

  TNode<String> phi_bb6_8;
  TNode<IntPtrT> phi_bb6_9;
  TNode<SeqOneByteString> tmp7;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_8, &phi_bb6_9);
    compiler::CodeAssemblerLabel label8(&ca_);
    tmp7 = Cast_SeqOneByteString_1(state_, TNode<String>{phi_bb6_8}, &label8);
    ca_.Goto(&block11, phi_bb6_8, phi_bb6_9, phi_bb6_8, phi_bb6_8);
    if (label8.is_used()) {
      ca_.Bind(&label8);
      ca_.Goto(&block12, phi_bb6_8, phi_bb6_9, phi_bb6_8, phi_bb6_8);
    }
  }

  TNode<String> phi_bb12_8;
  TNode<IntPtrT> phi_bb12_9;
  TNode<String> phi_bb12_11;
  TNode<String> phi_bb12_12;
  TNode<SeqTwoByteString> tmp9;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_8, &phi_bb12_9, &phi_bb12_11, &phi_bb12_12);
    compiler::CodeAssemblerLabel label10(&ca_);
    tmp9 = Cast_SeqTwoByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb12_11)}, &label10);
    ca_.Goto(&block19, phi_bb12_8, phi_bb12_9, phi_bb12_11);
    if (label10.is_used()) {
      ca_.Bind(&label10);
      ca_.Goto(&block20, phi_bb12_8, phi_bb12_9, phi_bb12_11);
    }
  }

  TNode<String> phi_bb11_8;
  TNode<IntPtrT> phi_bb11_9;
  TNode<String> phi_bb11_11;
  TNode<String> phi_bb11_12;
  TNode<Object> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Object> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<IntPtrT> tmp16;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_8, &phi_bb11_9, &phi_bb11_11, &phi_bb11_12);
    std::tie(tmp11, tmp12, tmp13) = FieldSliceSeqOneByteStringChars_0(state_, TNode<SeqOneByteString>{tmp7}).Flatten();
    compiler::CodeAssemblerLabel label17(&ca_);
    std::tie(tmp14, tmp15, tmp16) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{tmp11}, TNode<IntPtrT>{tmp12}, TNode<IntPtrT>{tmp13}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb11_9}, TNode<IntPtrT>{tmp5}, &label17).Flatten();
    ca_.Goto(&block15, phi_bb11_8, phi_bb11_9, phi_bb11_11, phi_bb11_9);
    if (label17.is_used()) {
      ca_.Bind(&label17);
      ca_.Goto(&block16, phi_bb11_8, phi_bb11_9, phi_bb11_11, phi_bb11_9);
    }
  }

  TNode<String> phi_bb16_8;
  TNode<IntPtrT> phi_bb16_9;
  TNode<String> phi_bb16_11;
  TNode<IntPtrT> phi_bb16_16;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_8, &phi_bb16_9, &phi_bb16_11, &phi_bb16_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb15_8;
  TNode<IntPtrT> phi_bb15_9;
  TNode<String> phi_bb15_11;
  TNode<IntPtrT> phi_bb15_16;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_8, &phi_bb15_9, &phi_bb15_11, &phi_bb15_16);
    ca_.Goto(&block5, tmp14, tmp15, tmp16);
  }

  TNode<String> phi_bb20_8;
  TNode<IntPtrT> phi_bb20_9;
  TNode<String> phi_bb20_11;
  TNode<ThinString> tmp18;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_8, &phi_bb20_9, &phi_bb20_11);
    compiler::CodeAssemblerLabel label19(&ca_);
    tmp18 = Cast_ThinString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb20_11)}, &label19);
    ca_.Goto(&block27, phi_bb20_8, phi_bb20_9, phi_bb20_11);
    if (label19.is_used()) {
      ca_.Bind(&label19);
      ca_.Goto(&block28, phi_bb20_8, phi_bb20_9, phi_bb20_11);
    }
  }

  TNode<String> phi_bb19_8;
  TNode<IntPtrT> phi_bb19_9;
  TNode<String> phi_bb19_11;
  TNode<Object> tmp20;
  TNode<IntPtrT> tmp21;
  TNode<IntPtrT> tmp22;
  TNode<Object> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<IntPtrT> tmp25;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_8, &phi_bb19_9, &phi_bb19_11);
    std::tie(tmp20, tmp21, tmp22) = FieldSliceSeqTwoByteStringChars_0(state_, TNode<SeqTwoByteString>{tmp9}).Flatten();
    compiler::CodeAssemblerLabel label26(&ca_);
    std::tie(tmp23, tmp24, tmp25) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{tmp20}, TNode<IntPtrT>{tmp21}, TNode<IntPtrT>{tmp22}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb19_9}, TNode<IntPtrT>{tmp5}, &label26).Flatten();
    ca_.Goto(&block23, phi_bb19_8, phi_bb19_9, phi_bb19_11, phi_bb19_9);
    if (label26.is_used()) {
      ca_.Bind(&label26);
      ca_.Goto(&block24, phi_bb19_8, phi_bb19_9, phi_bb19_11, phi_bb19_9);
    }
  }

  TNode<String> phi_bb24_8;
  TNode<IntPtrT> phi_bb24_9;
  TNode<String> phi_bb24_11;
  TNode<IntPtrT> phi_bb24_16;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_8, &phi_bb24_9, &phi_bb24_11, &phi_bb24_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb23_8;
  TNode<IntPtrT> phi_bb23_9;
  TNode<String> phi_bb23_11;
  TNode<IntPtrT> phi_bb23_16;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_8, &phi_bb23_9, &phi_bb23_11, &phi_bb23_16);
    ca_.Goto(&block3, tmp23, tmp24, tmp25);
  }

  TNode<String> phi_bb28_8;
  TNode<IntPtrT> phi_bb28_9;
  TNode<String> phi_bb28_11;
  TNode<ConsString> tmp27;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_8, &phi_bb28_9, &phi_bb28_11);
    compiler::CodeAssemblerLabel label28(&ca_);
    tmp27 = Cast_ConsString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb28_11)}, &label28);
    ca_.Goto(&block31, phi_bb28_8, phi_bb28_9, phi_bb28_11);
    if (label28.is_used()) {
      ca_.Bind(&label28);
      ca_.Goto(&block32, phi_bb28_8, phi_bb28_9, phi_bb28_11);
    }
  }

  TNode<String> phi_bb27_8;
  TNode<IntPtrT> phi_bb27_9;
  TNode<String> phi_bb27_11;
  TNode<IntPtrT> tmp29;
  TNode<String> tmp30;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_8, &phi_bb27_9, &phi_bb27_11);
    tmp29 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp30 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp18, tmp29});
    ca_.Goto(&block25, tmp30, phi_bb27_9, phi_bb27_11);
  }

  TNode<String> phi_bb32_8;
  TNode<IntPtrT> phi_bb32_9;
  TNode<String> phi_bb32_11;
  TNode<SlicedString> tmp31;
  if (block32.is_used()) {
    ca_.Bind(&block32, &phi_bb32_8, &phi_bb32_9, &phi_bb32_11);
    compiler::CodeAssemblerLabel label32(&ca_);
    tmp31 = Cast_SlicedString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb32_11)}, &label32);
    ca_.Goto(&block35, phi_bb32_8, phi_bb32_9, phi_bb32_11);
    if (label32.is_used()) {
      ca_.Bind(&label32);
      ca_.Goto(&block36, phi_bb32_8, phi_bb32_9, phi_bb32_11);
    }
  }

  TNode<String> phi_bb31_8;
  TNode<IntPtrT> phi_bb31_9;
  TNode<String> phi_bb31_11;
  TNode<String> tmp33;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_8, &phi_bb31_9, &phi_bb31_11);
    tmp33 = Flatten_1(state_, TNode<ConsString>{tmp27});
    ca_.Goto(&block29, tmp33, phi_bb31_9, phi_bb31_11);
  }

  TNode<String> phi_bb36_8;
  TNode<IntPtrT> phi_bb36_9;
  TNode<String> phi_bb36_11;
  TNode<ExternalOneByteString> tmp34;
  if (block36.is_used()) {
    ca_.Bind(&block36, &phi_bb36_8, &phi_bb36_9, &phi_bb36_11);
    compiler::CodeAssemblerLabel label35(&ca_);
    tmp34 = Cast_ExternalOneByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb36_11)}, &label35);
    ca_.Goto(&block39, phi_bb36_8, phi_bb36_9, phi_bb36_11);
    if (label35.is_used()) {
      ca_.Bind(&label35);
      ca_.Goto(&block40, phi_bb36_8, phi_bb36_9, phi_bb36_11);
    }
  }

  TNode<String> phi_bb35_8;
  TNode<IntPtrT> phi_bb35_9;
  TNode<String> phi_bb35_11;
  TNode<IntPtrT> tmp36;
  TNode<Smi> tmp37;
  TNode<IntPtrT> tmp38;
  TNode<IntPtrT> tmp39;
  TNode<IntPtrT> tmp40;
  TNode<String> tmp41;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_8, &phi_bb35_9, &phi_bb35_11);
    tmp36 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp37 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp31, tmp36});
    tmp38 = Convert_intptr_Smi_0(state_, TNode<Smi>{tmp37});
    tmp39 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb35_9}, TNode<IntPtrT>{tmp38});
    tmp40 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp41 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp31, tmp40});
    ca_.Goto(&block29, tmp41, tmp39, phi_bb35_11);
  }

  TNode<String> phi_bb40_8;
  TNode<IntPtrT> phi_bb40_9;
  TNode<String> phi_bb40_11;
  TNode<ExternalTwoByteString> tmp42;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_8, &phi_bb40_9, &phi_bb40_11);
    compiler::CodeAssemblerLabel label43(&ca_);
    tmp42 = Cast_ExternalTwoByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb40_11)}, &label43);
    ca_.Goto(&block47, phi_bb40_8, phi_bb40_9, phi_bb40_11);
    if (label43.is_used()) {
      ca_.Bind(&label43);
      ca_.Goto(&block48, phi_bb40_8, phi_bb40_9, phi_bb40_11);
    }
  }

  TNode<String> phi_bb39_8;
  TNode<IntPtrT> phi_bb39_9;
  TNode<String> phi_bb39_11;
  TNode<RawPtrT> tmp44;
  TNode<IntPtrT> tmp45;
  TNode<Int32T> tmp46;
  TNode<IntPtrT> tmp47;
  TNode<Object> tmp48;
  TNode<IntPtrT> tmp49;
  TNode<IntPtrT> tmp50;
  TNode<Object> tmp51;
  TNode<IntPtrT> tmp52;
  TNode<IntPtrT> tmp53;
  if (block39.is_used()) {
    ca_.Bind(&block39, &phi_bb39_8, &phi_bb39_9, &phi_bb39_11);
    tmp44 = Method_ExternalOneByteString_GetChars_0(state_, TNode<ExternalOneByteString>{tmp34});
    tmp45 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp46 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{tmp34, tmp45});
    tmp47 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp46});
    std::tie(tmp48, tmp49, tmp50) = NewOffHeapConstSlice_char8_0(state_, TNode<RawPtrT>{tmp44}, TNode<IntPtrT>{tmp47}).Flatten();
    compiler::CodeAssemblerLabel label54(&ca_);
    std::tie(tmp51, tmp52, tmp53) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{tmp48}, TNode<IntPtrT>{tmp49}, TNode<IntPtrT>{tmp50}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb39_9}, TNode<IntPtrT>{tmp5}, &label54).Flatten();
    ca_.Goto(&block43, phi_bb39_8, phi_bb39_9, phi_bb39_11, phi_bb39_9);
    if (label54.is_used()) {
      ca_.Bind(&label54);
      ca_.Goto(&block44, phi_bb39_8, phi_bb39_9, phi_bb39_11, phi_bb39_9);
    }
  }

  TNode<String> phi_bb44_8;
  TNode<IntPtrT> phi_bb44_9;
  TNode<String> phi_bb44_11;
  TNode<IntPtrT> phi_bb44_19;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_8, &phi_bb44_9, &phi_bb44_11, &phi_bb44_19);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb43_8;
  TNode<IntPtrT> phi_bb43_9;
  TNode<String> phi_bb43_11;
  TNode<IntPtrT> phi_bb43_19;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_8, &phi_bb43_9, &phi_bb43_11, &phi_bb43_19);
    ca_.Goto(&block5, tmp51, tmp52, tmp53);
  }

  TNode<String> phi_bb48_8;
  TNode<IntPtrT> phi_bb48_9;
  TNode<String> phi_bb48_11;
  if (block48.is_used()) {
    ca_.Bind(&block48, &phi_bb48_8, &phi_bb48_9, &phi_bb48_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb47_8;
  TNode<IntPtrT> phi_bb47_9;
  TNode<String> phi_bb47_11;
  TNode<RawPtrT> tmp55;
  TNode<IntPtrT> tmp56;
  TNode<Int32T> tmp57;
  TNode<IntPtrT> tmp58;
  TNode<Object> tmp59;
  TNode<IntPtrT> tmp60;
  TNode<IntPtrT> tmp61;
  TNode<Object> tmp62;
  TNode<IntPtrT> tmp63;
  TNode<IntPtrT> tmp64;
  if (block47.is_used()) {
    ca_.Bind(&block47, &phi_bb47_8, &phi_bb47_9, &phi_bb47_11);
    tmp55 = Method_ExternalTwoByteString_GetChars_0(state_, TNode<ExternalTwoByteString>{tmp42});
    tmp56 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp57 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{tmp42, tmp56});
    tmp58 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp57});
    std::tie(tmp59, tmp60, tmp61) = NewOffHeapConstSlice_char16_0(state_, TNode<RawPtrT>{tmp55}, TNode<IntPtrT>{tmp58}).Flatten();
    compiler::CodeAssemblerLabel label65(&ca_);
    std::tie(tmp62, tmp63, tmp64) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{tmp59}, TNode<IntPtrT>{tmp60}, TNode<IntPtrT>{tmp61}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb47_9}, TNode<IntPtrT>{tmp5}, &label65).Flatten();
    ca_.Goto(&block51, phi_bb47_8, phi_bb47_9, phi_bb47_11, phi_bb47_9);
    if (label65.is_used()) {
      ca_.Bind(&label65);
      ca_.Goto(&block52, phi_bb47_8, phi_bb47_9, phi_bb47_11, phi_bb47_9);
    }
  }

  TNode<String> phi_bb52_8;
  TNode<IntPtrT> phi_bb52_9;
  TNode<String> phi_bb52_11;
  TNode<IntPtrT> phi_bb52_19;
  if (block52.is_used()) {
    ca_.Bind(&block52, &phi_bb52_8, &phi_bb52_9, &phi_bb52_11, &phi_bb52_19);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb51_8;
  TNode<IntPtrT> phi_bb51_9;
  TNode<String> phi_bb51_11;
  TNode<IntPtrT> phi_bb51_19;
  if (block51.is_used()) {
    ca_.Bind(&block51, &phi_bb51_8, &phi_bb51_9, &phi_bb51_11, &phi_bb51_19);
    ca_.Goto(&block3, tmp62, tmp63, tmp64);
  }

  TNode<String> phi_bb29_8;
  TNode<IntPtrT> phi_bb29_9;
  TNode<String> phi_bb29_11;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_8, &phi_bb29_9, &phi_bb29_11);
    ca_.Goto(&block25, phi_bb29_8, phi_bb29_9, phi_bb29_11);
  }

  TNode<String> phi_bb25_8;
  TNode<IntPtrT> phi_bb25_9;
  TNode<String> phi_bb25_11;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_8, &phi_bb25_9, &phi_bb25_11);
    ca_.Goto(&block8, phi_bb25_8, phi_bb25_9);
  }

  TNode<String> phi_bb7_8;
  TNode<IntPtrT> phi_bb7_9;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_8, &phi_bb7_9);
    VerifiedUnreachable_0(state_);
  }

  TNode<Object> phi_bb5_6;
  TNode<IntPtrT> phi_bb5_7;
  TNode<IntPtrT> phi_bb5_8;
  TNode<String> tmp66;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_6, &phi_bb5_7, &phi_bb5_8);
    tmp66 = StringTrimBody_char8_0(state_, TNode<Context>{p_context}, TNode<String>{tmp1}, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{phi_bb5_6}, TNode<IntPtrT>{phi_bb5_7}, TNode<IntPtrT>{phi_bb5_8}, TorqueStructUnsafe_0{}}, p_variant);
    ca_.Goto(&block1, tmp66);
  }

  TNode<Object> phi_bb3_6;
  TNode<IntPtrT> phi_bb3_7;
  TNode<IntPtrT> phi_bb3_8;
  TNode<String> tmp67;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_6, &phi_bb3_7, &phi_bb3_8);
    tmp67 = StringTrimBody_char16_0(state_, TNode<Context>{p_context}, TNode<String>{tmp1}, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{phi_bb3_6}, TNode<IntPtrT>{phi_bb3_7}, TNode<IntPtrT>{phi_bb3_8}, TorqueStructUnsafe_0{}}, p_variant);
    ca_.Goto(&block1, tmp67);
  }

  TNode<String> phi_bb1_5;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_5);
    ca_.Goto(&block53);
  }

    ca_.Bind(&block53);
  return TNode<String>{phi_bb1_5};
}


TF_BUILTIN(StringPrototypeTrim, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
   TNode<Word32T> argc = UncheckedParameter<Word32T>(Descriptor::kJSActualArgumentsCount);
  TNode<IntPtrT> arguments_length(ChangeInt32ToIntPtr(UncheckedCast<Int32T>(argc)));
  TNode<RawPtrT> arguments_frame = UncheckedCast<RawPtrT>(LoadFramePointer());
  TorqueStructArguments torque_arguments(GetFrameArguments(arguments_frame, arguments_length));
  CodeStubArguments arguments(this, torque_arguments);
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = arguments.GetReceiver();
USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = StringTrim_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, "String.prototype.trim", String::TrimMode::kTrim);
    arguments.PopAndReturn(tmp0);
  }
}

TF_BUILTIN(StringPrototypeTrimStart, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
   TNode<Word32T> argc = UncheckedParameter<Word32T>(Descriptor::kJSActualArgumentsCount);
  TNode<IntPtrT> arguments_length(ChangeInt32ToIntPtr(UncheckedCast<Int32T>(argc)));
  TNode<RawPtrT> arguments_frame = UncheckedCast<RawPtrT>(LoadFramePointer());
  TorqueStructArguments torque_arguments(GetFrameArguments(arguments_frame, arguments_length));
  CodeStubArguments arguments(this, torque_arguments);
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = arguments.GetReceiver();
USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = StringTrim_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, "String.prototype.trimLeft", String::TrimMode::kTrimStart);
    arguments.PopAndReturn(tmp0);
  }
}

TF_BUILTIN(StringPrototypeTrimEnd, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
   TNode<Word32T> argc = UncheckedParameter<Word32T>(Descriptor::kJSActualArgumentsCount);
  TNode<IntPtrT> arguments_length(ChangeInt32ToIntPtr(UncheckedCast<Int32T>(argc)));
  TNode<RawPtrT> arguments_frame = UncheckedCast<RawPtrT>(LoadFramePointer());
  TorqueStructArguments torque_arguments(GetFrameArguments(arguments_frame, arguments_length));
  CodeStubArguments arguments(this, torque_arguments);
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = arguments.GetReceiver();
USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = StringTrim_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, "String.prototype.trimRight", String::TrimMode::kTrimEnd);
    arguments.PopAndReturn(tmp0);
  }
}

TNode<String> StringTrimBody_char8_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<String> p_string, TorqueStructSlice_char8_ConstReference_char8_0 p_slice, String::TrimMode p_variant) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<BoolT> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadStringLengthAsWord(TNode<String>{p_string});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp3 = CodeStubAssembler(state_).IntPtrSub(TNode<IntPtrT>{tmp0}, TNode<IntPtrT>{tmp2});
    tmp4 = FromConstexpr_bool_constexpr_bool_0(state_, ((CodeStubAssembler(state_).ConstexprUint32Equal(p_variant, String::TrimMode::kTrim)) || (CodeStubAssembler(state_).ConstexprUint32Equal(p_variant, String::TrimMode::kTrimStart))));
    ca_.Branch(tmp4, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{tmp1});
  }

  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<BoolT> tmp7;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp6 = StringTrimLoop_char8_0(state_, TNode<Context>{p_context}, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{p_slice.object}, TNode<IntPtrT>{p_slice.offset}, TNode<IntPtrT>{p_slice.length}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp0}, TNode<IntPtrT>{tmp5});
    tmp7 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp6}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp7, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp8;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp8 = kEmptyString_0(state_);
    ca_.Goto(&block1, tmp8);
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block3, tmp6);
  }

  TNode<IntPtrT> phi_bb3_6;
  TNode<BoolT> tmp9;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_6);
    tmp9 = FromConstexpr_bool_constexpr_bool_0(state_, ((CodeStubAssembler(state_).ConstexprUint32Equal(p_variant, String::TrimMode::kTrim)) || (CodeStubAssembler(state_).ConstexprUint32Equal(p_variant, String::TrimMode::kTrimEnd))));
    ca_.Branch(tmp9, &block6, std::vector<compiler::Node*>{phi_bb3_6}, &block7, std::vector<compiler::Node*>{phi_bb3_6, tmp3});
  }

  TNode<IntPtrT> phi_bb6_6;
  TNode<IntPtrT> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<BoolT> tmp14;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_6);
    tmp10 = FromConstexpr_intptr_constexpr_int31_0(state_, -1);
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, -1);
    tmp12 = StringTrimLoop_char8_0(state_, TNode<Context>{p_context}, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{p_slice.object}, TNode<IntPtrT>{p_slice.offset}, TNode<IntPtrT>{p_slice.length}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp10}, TNode<IntPtrT>{tmp11});
    tmp13 = FromConstexpr_intptr_constexpr_int31_0(state_, -1);
    tmp14 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp12}, TNode<IntPtrT>{tmp13});
    ca_.Branch(tmp14, &block8, std::vector<compiler::Node*>{phi_bb6_6}, &block9, std::vector<compiler::Node*>{phi_bb6_6});
  }

  TNode<IntPtrT> phi_bb8_6;
  TNode<String> tmp15;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_6);
    tmp15 = kEmptyString_0(state_);
    ca_.Goto(&block1, tmp15);
  }

  TNode<IntPtrT> phi_bb9_6;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_6);
    ca_.Goto(&block7, phi_bb9_6, tmp12);
  }

  TNode<IntPtrT> phi_bb7_6;
  TNode<IntPtrT> phi_bb7_7;
  TNode<UintPtrT> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<IntPtrT> tmp18;
  TNode<UintPtrT> tmp19;
  TNode<String> tmp20;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_6, &phi_bb7_7);
    tmp16 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{phi_bb7_6});
    tmp17 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp18 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb7_7}, TNode<IntPtrT>{tmp17});
    tmp19 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{tmp18});
    tmp20 = StringBuiltinsAssembler(state_).SubString(TNode<String>{p_string}, TNode<UintPtrT>{tmp16}, TNode<UintPtrT>{tmp19});
    ca_.Goto(&block1, tmp20);
  }

  TNode<String> phi_bb1_5;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_5);
    ca_.Goto(&block10);
  }

    ca_.Bind(&block10);
  return TNode<String>{phi_bb1_5};
}


TNode<String> StringTrimBody_char16_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<String> p_string, TorqueStructSlice_char16_ConstReference_char16_0 p_slice, String::TrimMode p_variant) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<BoolT> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadStringLengthAsWord(TNode<String>{p_string});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp3 = CodeStubAssembler(state_).IntPtrSub(TNode<IntPtrT>{tmp0}, TNode<IntPtrT>{tmp2});
    tmp4 = FromConstexpr_bool_constexpr_bool_0(state_, ((CodeStubAssembler(state_).ConstexprUint32Equal(p_variant, String::TrimMode::kTrim)) || (CodeStubAssembler(state_).ConstexprUint32Equal(p_variant, String::TrimMode::kTrimStart))));
    ca_.Branch(tmp4, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{tmp1});
  }

  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<BoolT> tmp7;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp6 = StringTrimLoop_char16_0(state_, TNode<Context>{p_context}, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{p_slice.object}, TNode<IntPtrT>{p_slice.offset}, TNode<IntPtrT>{p_slice.length}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp0}, TNode<IntPtrT>{tmp5});
    tmp7 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp6}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp7, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp8;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp8 = kEmptyString_0(state_);
    ca_.Goto(&block1, tmp8);
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block3, tmp6);
  }

  TNode<IntPtrT> phi_bb3_6;
  TNode<BoolT> tmp9;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_6);
    tmp9 = FromConstexpr_bool_constexpr_bool_0(state_, ((CodeStubAssembler(state_).ConstexprUint32Equal(p_variant, String::TrimMode::kTrim)) || (CodeStubAssembler(state_).ConstexprUint32Equal(p_variant, String::TrimMode::kTrimEnd))));
    ca_.Branch(tmp9, &block6, std::vector<compiler::Node*>{phi_bb3_6}, &block7, std::vector<compiler::Node*>{phi_bb3_6, tmp3});
  }

  TNode<IntPtrT> phi_bb6_6;
  TNode<IntPtrT> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<BoolT> tmp14;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_6);
    tmp10 = FromConstexpr_intptr_constexpr_int31_0(state_, -1);
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, -1);
    tmp12 = StringTrimLoop_char16_0(state_, TNode<Context>{p_context}, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{p_slice.object}, TNode<IntPtrT>{p_slice.offset}, TNode<IntPtrT>{p_slice.length}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp10}, TNode<IntPtrT>{tmp11});
    tmp13 = FromConstexpr_intptr_constexpr_int31_0(state_, -1);
    tmp14 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp12}, TNode<IntPtrT>{tmp13});
    ca_.Branch(tmp14, &block8, std::vector<compiler::Node*>{phi_bb6_6}, &block9, std::vector<compiler::Node*>{phi_bb6_6});
  }

  TNode<IntPtrT> phi_bb8_6;
  TNode<String> tmp15;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_6);
    tmp15 = kEmptyString_0(state_);
    ca_.Goto(&block1, tmp15);
  }

  TNode<IntPtrT> phi_bb9_6;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_6);
    ca_.Goto(&block7, phi_bb9_6, tmp12);
  }

  TNode<IntPtrT> phi_bb7_6;
  TNode<IntPtrT> phi_bb7_7;
  TNode<UintPtrT> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<IntPtrT> tmp18;
  TNode<UintPtrT> tmp19;
  TNode<String> tmp20;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_6, &phi_bb7_7);
    tmp16 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{phi_bb7_6});
    tmp17 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp18 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb7_7}, TNode<IntPtrT>{tmp17});
    tmp19 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{tmp18});
    tmp20 = StringBuiltinsAssembler(state_).SubString(TNode<String>{p_string}, TNode<UintPtrT>{tmp16}, TNode<UintPtrT>{tmp19});
    ca_.Goto(&block1, tmp20);
  }

  TNode<String> phi_bb1_5;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_5);
    ca_.Goto(&block10);
  }

    ca_.Bind(&block10);
  return TNode<String>{phi_bb1_5};
}


TNode<IntPtrT> StringTrimLoop_char8_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TorqueStructSlice_char8_ConstReference_char8_0 p_stringSlice, TNode<IntPtrT> p_startIndex, TNode<IntPtrT> p_endIndex, TNode<IntPtrT> p_increment) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    ca_.Goto(&block4, p_startIndex);
  }

  TNode<IntPtrT> phi_bb4_7;
  TNode<BoolT> tmp0;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_7);
    tmp0 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp0, &block2, std::vector<compiler::Node*>{phi_bb4_7}, &block3, std::vector<compiler::Node*>{phi_bb4_7});
  }

  TNode<IntPtrT> phi_bb2_7;
  TNode<BoolT> tmp1;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_7);
    tmp1 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb2_7}, TNode<IntPtrT>{p_endIndex});
    ca_.Branch(tmp1, &block5, std::vector<compiler::Node*>{phi_bb2_7}, &block6, std::vector<compiler::Node*>{phi_bb2_7});
  }

  TNode<IntPtrT> phi_bb5_7;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_7);
    ca_.Goto(&block1, phi_bb5_7);
  }

  TNode<IntPtrT> phi_bb6_7;
  TNode<UintPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<BoolT> tmp4;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_7);
    tmp2 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb6_7});
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_stringSlice.length});
    tmp4 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp2}, TNode<UintPtrT>{tmp3});
    ca_.Branch(tmp4, &block11, std::vector<compiler::Node*>{phi_bb6_7, phi_bb6_7, phi_bb6_7, phi_bb6_7, phi_bb6_7}, &block12, std::vector<compiler::Node*>{phi_bb6_7, phi_bb6_7, phi_bb6_7, phi_bb6_7, phi_bb6_7});
  }

  TNode<IntPtrT> phi_bb11_7;
  TNode<IntPtrT> phi_bb11_11;
  TNode<IntPtrT> phi_bb11_12;
  TNode<IntPtrT> phi_bb11_16;
  TNode<IntPtrT> phi_bb11_17;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Object> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<Uint8T> tmp9;
  TNode<BoolT> tmp10;
  TNode<BoolT> tmp11;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_7, &phi_bb11_11, &phi_bb11_12, &phi_bb11_16, &phi_bb11_17);
    tmp5 = TimesSizeOf_char8_0(state_, TNode<IntPtrT>{phi_bb11_17});
    tmp6 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{p_stringSlice.offset}, TNode<IntPtrT>{tmp5});
    std::tie(tmp7, tmp8) = NewReference_char8_0(state_, TNode<Object>{p_stringSlice.object}, TNode<IntPtrT>{tmp6}).Flatten();
    tmp9 = CodeStubAssembler(state_).LoadReference<Uint8T>(CodeStubAssembler::Reference{tmp7, tmp8});
    tmp10 = IsWhiteSpaceOrLineTerminator_0(state_, TNode<Uint16T>{tmp9});
    tmp11 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp10});
    ca_.Branch(tmp11, &block14, std::vector<compiler::Node*>{phi_bb11_7}, &block15, std::vector<compiler::Node*>{phi_bb11_7});
  }

  TNode<IntPtrT> phi_bb12_7;
  TNode<IntPtrT> phi_bb12_11;
  TNode<IntPtrT> phi_bb12_12;
  TNode<IntPtrT> phi_bb12_16;
  TNode<IntPtrT> phi_bb12_17;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_7, &phi_bb12_11, &phi_bb12_12, &phi_bb12_16, &phi_bb12_17);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb14_7;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_7);
    ca_.Goto(&block1, phi_bb14_7);
  }

  TNode<IntPtrT> phi_bb15_7;
  TNode<IntPtrT> tmp12;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_7);
    tmp12 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb15_7}, TNode<IntPtrT>{p_increment});
    ca_.Goto(&block4, tmp12);
  }

  TNode<IntPtrT> phi_bb3_7;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_7);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb1_7;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_7);
    ca_.Goto(&block16, phi_bb1_7);
  }

  TNode<IntPtrT> phi_bb16_7;
    ca_.Bind(&block16, &phi_bb16_7);
  return TNode<IntPtrT>{phi_bb16_7};
}


TNode<IntPtrT> StringTrimLoop_char16_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TorqueStructSlice_char16_ConstReference_char16_0 p_stringSlice, TNode<IntPtrT> p_startIndex, TNode<IntPtrT> p_endIndex, TNode<IntPtrT> p_increment) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    ca_.Goto(&block4, p_startIndex);
  }

  TNode<IntPtrT> phi_bb4_7;
  TNode<BoolT> tmp0;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_7);
    tmp0 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp0, &block2, std::vector<compiler::Node*>{phi_bb4_7}, &block3, std::vector<compiler::Node*>{phi_bb4_7});
  }

  TNode<IntPtrT> phi_bb2_7;
  TNode<BoolT> tmp1;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_7);
    tmp1 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb2_7}, TNode<IntPtrT>{p_endIndex});
    ca_.Branch(tmp1, &block5, std::vector<compiler::Node*>{phi_bb2_7}, &block6, std::vector<compiler::Node*>{phi_bb2_7});
  }

  TNode<IntPtrT> phi_bb5_7;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_7);
    ca_.Goto(&block1, phi_bb5_7);
  }

  TNode<IntPtrT> phi_bb6_7;
  TNode<UintPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<BoolT> tmp4;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_7);
    tmp2 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb6_7});
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_stringSlice.length});
    tmp4 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp2}, TNode<UintPtrT>{tmp3});
    ca_.Branch(tmp4, &block11, std::vector<compiler::Node*>{phi_bb6_7, phi_bb6_7, phi_bb6_7, phi_bb6_7, phi_bb6_7}, &block12, std::vector<compiler::Node*>{phi_bb6_7, phi_bb6_7, phi_bb6_7, phi_bb6_7, phi_bb6_7});
  }

  TNode<IntPtrT> phi_bb11_7;
  TNode<IntPtrT> phi_bb11_11;
  TNode<IntPtrT> phi_bb11_12;
  TNode<IntPtrT> phi_bb11_16;
  TNode<IntPtrT> phi_bb11_17;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Object> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<Uint16T> tmp9;
  TNode<BoolT> tmp10;
  TNode<BoolT> tmp11;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_7, &phi_bb11_11, &phi_bb11_12, &phi_bb11_16, &phi_bb11_17);
    tmp5 = TimesSizeOf_char16_0(state_, TNode<IntPtrT>{phi_bb11_17});
    tmp6 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{p_stringSlice.offset}, TNode<IntPtrT>{tmp5});
    std::tie(tmp7, tmp8) = NewReference_char16_0(state_, TNode<Object>{p_stringSlice.object}, TNode<IntPtrT>{tmp6}).Flatten();
    tmp9 = CodeStubAssembler(state_).LoadReference<Uint16T>(CodeStubAssembler::Reference{tmp7, tmp8});
    tmp10 = IsWhiteSpaceOrLineTerminator_0(state_, TNode<Uint16T>{tmp9});
    tmp11 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp10});
    ca_.Branch(tmp11, &block14, std::vector<compiler::Node*>{phi_bb11_7}, &block15, std::vector<compiler::Node*>{phi_bb11_7});
  }

  TNode<IntPtrT> phi_bb12_7;
  TNode<IntPtrT> phi_bb12_11;
  TNode<IntPtrT> phi_bb12_12;
  TNode<IntPtrT> phi_bb12_16;
  TNode<IntPtrT> phi_bb12_17;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_7, &phi_bb12_11, &phi_bb12_12, &phi_bb12_16, &phi_bb12_17);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb14_7;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_7);
    ca_.Goto(&block1, phi_bb14_7);
  }

  TNode<IntPtrT> phi_bb15_7;
  TNode<IntPtrT> tmp12;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_7);
    tmp12 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb15_7}, TNode<IntPtrT>{p_increment});
    ca_.Goto(&block4, tmp12);
  }

  TNode<IntPtrT> phi_bb3_7;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_7);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb1_7;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_7);
    ca_.Goto(&block16, phi_bb1_7);
  }

  TNode<IntPtrT> phi_bb16_7;
    ca_.Bind(&block16, &phi_bb16_7);
  return TNode<IntPtrT>{phi_bb16_7};
}


} // namespace internal
} // namespace v8
