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
#include "torque-generated/src/builtins/string-replaceall-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/builtins-string-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/regexp-match-all-tq-csa.h"
#include "torque-generated/src/builtins/regexp-tq-csa.h"
#include "torque-generated/src/builtins/string-replaceall-tq-csa.h"
#include "torque-generated/src/objects/js-regexp-tq-csa.h"
#include "torque-generated/src/objects/string-tq-csa.h"

namespace v8 {
namespace internal {
void ThrowIfNotGlobal_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_searchValue) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSRegExp> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_FastJSRegExp_1(state_, TNode<Context>{p_context}, TNode<Object>{p_searchValue}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  TNode<Object> tmp2;
  TNode<Object> tmp3;
  TNode<Object> tmp4;
  TNode<String> tmp5;
  TNode<String> tmp6;
  TNode<Smi> tmp7;
  TNode<Smi> tmp8;
  TNode<Smi> tmp9;
  TNode<BoolT> tmp10;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = FromConstexpr_JSAny_constexpr_string_0(state_, "flags");
    tmp3 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{p_searchValue}, TNode<Object>{tmp2});
    tmp4 = RequireObjectCoercible_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp3}, "String.prototype.replaceAll");
    tmp5 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{tmp3});
    tmp6 = CodeStubAssembler(state_).StringConstant("g");
    tmp7 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp8 = ca_.CallStub<Smi>(Builtins::CallableFor(ca_.isolate(), Builtin::kStringIndexOf), p_context, tmp5, tmp6, tmp7);
    tmp9 = FromConstexpr_Smi_constexpr_int31_0(state_, -1);
    tmp10 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp8}, TNode<Smi>{tmp9});
    ca_.Goto(&block2, tmp10);
  }

  TNode<BoolT> tmp11;
  TNode<BoolT> tmp12;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp11 = RegExpBuiltinsAssembler(state_).FastFlagGetterGlobal(TNode<JSRegExp>{tmp0});
    tmp12 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp11});
    ca_.Goto(&block2, tmp12);
  }

  TNode<BoolT> phi_bb2_2;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_2);
    ca_.Branch(phi_bb2_2, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kRegExpGlobalInvokedOnNonGlobal, "String.prototype.replaceAll");
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block8);
  }

    ca_.Bind(&block8);
}


TF_BUILTIN(StringPrototypeReplaceAll, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kSearchValue);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kReplaceValue);
  USE(parameter3);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, String, Smi> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, String, Smi> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, String, Smi> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, String, Smi> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, String, Smi, String> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, String, Smi> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, String, Smi> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, String, Smi> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<Oddball> tmp1;
  TNode<BoolT> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = RequireObjectCoercible_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, "String.prototype.replaceAll");
    tmp1 = Undefined_0(state_);
    tmp2 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{parameter2}, TNode<HeapObject>{tmp1});
    ca_.Branch(tmp2, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  TNode<Oddball> tmp3;
  TNode<BoolT> tmp4;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = Null_0(state_);
    tmp4 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{parameter2}, TNode<HeapObject>{tmp3});
    ca_.Goto(&block5, tmp4);
  }

  TNode<BoolT> tmp5;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp5 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block5, tmp5);
  }

  TNode<BoolT> phi_bb5_5;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_5);
    ca_.Branch(phi_bb5_5, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp6;
  if (block1.is_used()) {
    ca_.Bind(&block1);
    tmp6 = IsRegExp_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter2});
    ca_.Branch(tmp6, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ThrowIfNotGlobal_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter2});
    ca_.Goto(&block7);
  }

  TNode<Symbol> tmp7;
  TNode<JSReceiver> tmp8;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp7 = CodeStubAssembler(state_).ReplaceSymbolConstant();
    compiler::CodeAssemblerLabel label9(&ca_);
    tmp8 = GetMethod_3(state_, TNode<Context>{parameter0}, TNode<Object>{parameter2}, TNode<Symbol>{tmp7}, &label9);
    ca_.Goto(&block10);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block11);
    }
  }

  if (block11.is_used()) {
    ca_.Bind(&block11);
    ca_.Goto(&block2);
  }

  TNode<Object> tmp10;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp10 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp8}, TNode<Object>{parameter2}, TNode<Object>{parameter1}, TNode<Object>{parameter3});
    CodeStubAssembler(state_).Return(tmp10);
  }

  TNode<String> tmp11;
  TNode<String> tmp12;
  TNode<BoolT> tmp13;
  TNode<BoolT> tmp14;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp11 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp12 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter2});
    tmp13 = Is_Callable_JSAny_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter3});
    tmp14 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp13});
    ca_.Branch(tmp14, &block12, std::vector<compiler::Node*>{}, &block13, std::vector<compiler::Node*>{parameter3});
  }

  TNode<String> tmp15;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp15 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter3});
    ca_.Goto(&block13, tmp15);
  }

  TNode<Object> phi_bb13_6;
  TNode<Smi> tmp16;
  TNode<Smi> tmp17;
  TNode<Smi> tmp18;
  TNode<Smi> tmp19;
  TNode<String> tmp20;
  TNode<Smi> tmp21;
  TNode<Smi> tmp22;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_6);
    tmp16 = CodeStubAssembler(state_).LoadStringLengthAsSmi(TNode<String>{tmp12});
    tmp17 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp18 = CodeStubAssembler(state_).SmiMax(TNode<Smi>{tmp17}, TNode<Smi>{tmp16});
    tmp19 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp20 = kEmptyString_0(state_);
    tmp21 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp22 = AbstractStringIndexOf_4(state_, TNode<Context>{parameter0}, TNode<String>{tmp11}, TNode<String>{tmp12}, TNode<Smi>{tmp21});
    ca_.Goto(&block16, tmp19, tmp20, tmp22);
  }

  TNode<Smi> phi_bb16_10;
  TNode<String> phi_bb16_11;
  TNode<Smi> phi_bb16_12;
  TNode<Smi> tmp23;
  TNode<BoolT> tmp24;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_10, &phi_bb16_11, &phi_bb16_12);
    tmp23 = FromConstexpr_Smi_constexpr_int31_0(state_, -1);
    tmp24 = CodeStubAssembler(state_).SmiNotEqual(TNode<Smi>{phi_bb16_12}, TNode<Smi>{tmp23});
    ca_.Branch(tmp24, &block14, std::vector<compiler::Node*>{phi_bb16_10, phi_bb16_11, phi_bb16_12}, &block15, std::vector<compiler::Node*>{phi_bb16_10, phi_bb16_11, phi_bb16_12});
  }

  TNode<Smi> phi_bb14_10;
  TNode<String> phi_bb14_11;
  TNode<Smi> phi_bb14_12;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_10, &phi_bb14_11, &phi_bb14_12);
    ca_.Branch(tmp13, &block17, std::vector<compiler::Node*>{phi_bb14_10, phi_bb14_11, phi_bb14_12}, &block18, std::vector<compiler::Node*>{phi_bb14_10, phi_bb14_11, phi_bb14_12});
  }

  TNode<Smi> phi_bb17_10;
  TNode<String> phi_bb17_11;
  TNode<Smi> phi_bb17_12;
  TNode<JSReceiver> tmp25;
  TNode<Oddball> tmp26;
  TNode<Object> tmp27;
  TNode<String> tmp28;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_10, &phi_bb17_11, &phi_bb17_12);
    tmp25 = UnsafeCast_Callable_0(state_, TNode<Context>{parameter0}, TNode<Object>{phi_bb13_6});
    tmp26 = Undefined_0(state_);
    tmp27 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp25}, TNode<Object>{tmp26}, TNode<Object>{tmp12}, TNode<Object>{phi_bb17_12}, TNode<Object>{tmp11});
    tmp28 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{tmp27});
    ca_.Goto(&block19, phi_bb17_10, phi_bb17_11, phi_bb17_12, tmp28);
  }

  TNode<Smi> phi_bb18_10;
  TNode<String> phi_bb18_11;
  TNode<Smi> phi_bb18_12;
  TNode<String> tmp29;
  TNode<Smi> tmp30;
  TNode<String> tmp31;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_10, &phi_bb18_11, &phi_bb18_12);
    tmp29 = UnsafeCast_String_0(state_, TNode<Context>{parameter0}, TNode<Object>{phi_bb13_6});
    tmp30 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb18_12}, TNode<Smi>{tmp16});
    tmp31 = StringBuiltinsAssembler(state_).GetSubstitution(TNode<Context>{parameter0}, TNode<String>{tmp11}, TNode<Smi>{phi_bb18_12}, TNode<Smi>{tmp30}, TNode<String>{tmp29});
    ca_.Goto(&block19, phi_bb18_10, phi_bb18_11, phi_bb18_12, tmp31);
  }

  TNode<Smi> phi_bb19_10;
  TNode<String> phi_bb19_11;
  TNode<Smi> phi_bb19_12;
  TNode<String> phi_bb19_13;
  TNode<IntPtrT> tmp32;
  TNode<UintPtrT> tmp33;
  TNode<IntPtrT> tmp34;
  TNode<UintPtrT> tmp35;
  TNode<String> tmp36;
  TNode<String> tmp37;
  TNode<String> tmp38;
  TNode<Smi> tmp39;
  TNode<Smi> tmp40;
  TNode<Smi> tmp41;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_10, &phi_bb19_11, &phi_bb19_12, &phi_bb19_13);
    tmp32 = CodeStubAssembler(state_).SmiUntag(TNode<Smi>{phi_bb19_10});
    tmp33 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{tmp32});
    tmp34 = CodeStubAssembler(state_).SmiUntag(TNode<Smi>{phi_bb19_12});
    tmp35 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{tmp34});
    tmp36 = StringBuiltinsAssembler(state_).SubString(TNode<String>{tmp11}, TNode<UintPtrT>{tmp33}, TNode<UintPtrT>{tmp35});
    tmp37 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{phi_bb19_11}, TNode<String>{tmp36});
    tmp38 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp37}, TNode<String>{phi_bb19_13});
    tmp39 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb19_12}, TNode<Smi>{tmp16});
    tmp40 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb19_12}, TNode<Smi>{tmp18});
    tmp41 = AbstractStringIndexOf_4(state_, TNode<Context>{parameter0}, TNode<String>{tmp11}, TNode<String>{tmp12}, TNode<Smi>{tmp40});
    ca_.Goto(&block16, tmp39, tmp38, tmp41);
  }

  TNode<Smi> phi_bb15_10;
  TNode<String> phi_bb15_11;
  TNode<Smi> phi_bb15_12;
  TNode<Smi> tmp42;
  TNode<BoolT> tmp43;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_10, &phi_bb15_11, &phi_bb15_12);
    tmp42 = CodeStubAssembler(state_).LoadStringLengthAsSmi(TNode<String>{tmp11});
    tmp43 = CodeStubAssembler(state_).SmiLessThan(TNode<Smi>{phi_bb15_10}, TNode<Smi>{tmp42});
    ca_.Branch(tmp43, &block20, std::vector<compiler::Node*>{phi_bb15_10, phi_bb15_11, phi_bb15_12}, &block21, std::vector<compiler::Node*>{phi_bb15_10, phi_bb15_11, phi_bb15_12});
  }

  TNode<Smi> phi_bb20_10;
  TNode<String> phi_bb20_11;
  TNode<Smi> phi_bb20_12;
  TNode<IntPtrT> tmp44;
  TNode<UintPtrT> tmp45;
  TNode<IntPtrT> tmp46;
  TNode<UintPtrT> tmp47;
  TNode<String> tmp48;
  TNode<String> tmp49;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_10, &phi_bb20_11, &phi_bb20_12);
    tmp44 = CodeStubAssembler(state_).SmiUntag(TNode<Smi>{phi_bb20_10});
    tmp45 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{tmp44});
    tmp46 = CodeStubAssembler(state_).LoadStringLengthAsWord(TNode<String>{tmp11});
    tmp47 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{tmp46});
    tmp48 = StringBuiltinsAssembler(state_).SubString(TNode<String>{tmp11}, TNode<UintPtrT>{tmp45}, TNode<UintPtrT>{tmp47});
    tmp49 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{phi_bb20_11}, TNode<String>{tmp48});
    ca_.Goto(&block21, phi_bb20_10, tmp49, phi_bb20_12);
  }

  TNode<Smi> phi_bb21_10;
  TNode<String> phi_bb21_11;
  TNode<Smi> phi_bb21_12;
  if (block21.is_used()) {
    ca_.Bind(&block21, &phi_bb21_10, &phi_bb21_11, &phi_bb21_12);
    CodeStubAssembler(state_).Return(phi_bb21_11);
  }
}

} // namespace internal
} // namespace v8
