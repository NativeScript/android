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
#include "torque-generated/src/builtins/regexp-replace-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-isarray-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/array-reverse-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/regexp-match-all-tq-csa.h"
#include "torque-generated/src/builtins/regexp-replace-tq-csa.h"
#include "torque-generated/src/builtins/regexp-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-tq-csa.h"
#include "torque-generated/src/objects/js-regexp-tq-csa.h"
#include "torque-generated/src/objects/regexp-match-info-tq-csa.h"
#include "torque-generated/src/objects/string-tq-csa.h"

namespace v8 {
namespace internal {
void RegExpReplaceCallableNoExplicitCaptures_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FixedArray> p_matchesElements, TNode<IntPtrT> p_matchesLength, TNode<String> p_string, TNode<JSReceiver> p_replaceFn) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Smi> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block4, tmp0, tmp1);
  }

  TNode<Smi> phi_bb4_5;
  TNode<IntPtrT> phi_bb4_6;
  TNode<BoolT> tmp2;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_5, &phi_bb4_6);
    tmp2 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{phi_bb4_6}, TNode<IntPtrT>{p_matchesLength});
    ca_.Branch(tmp2, &block2, std::vector<compiler::Node*>{phi_bb4_5, phi_bb4_6}, &block3, std::vector<compiler::Node*>{phi_bb4_5, phi_bb4_6});
  }

  TNode<Smi> phi_bb2_5;
  TNode<IntPtrT> phi_bb2_6;
  TNode<Object> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  TNode<UintPtrT> tmp7;
  TNode<BoolT> tmp8;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_5, &phi_bb2_6);
    std::tie(tmp3, tmp4, tmp5) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_matchesElements}).Flatten();
    tmp6 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb2_6});
    tmp7 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp5});
    tmp8 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp6}, TNode<UintPtrT>{tmp7});
    ca_.Branch(tmp8, &block10, std::vector<compiler::Node*>{phi_bb2_5, phi_bb2_6, phi_bb2_6, phi_bb2_6, phi_bb2_6, phi_bb2_6}, &block11, std::vector<compiler::Node*>{phi_bb2_5, phi_bb2_6, phi_bb2_6, phi_bb2_6, phi_bb2_6, phi_bb2_6});
  }

  TNode<Smi> phi_bb10_5;
  TNode<IntPtrT> phi_bb10_6;
  TNode<IntPtrT> phi_bb10_11;
  TNode<IntPtrT> phi_bb10_12;
  TNode<IntPtrT> phi_bb10_16;
  TNode<IntPtrT> phi_bb10_17;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<Object> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  TNode<Smi> tmp14;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_5, &phi_bb10_6, &phi_bb10_11, &phi_bb10_12, &phi_bb10_16, &phi_bb10_17);
    tmp9 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb10_17});
    tmp10 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp9});
    std::tie(tmp11, tmp12) = NewReference_Object_0(state_, TNode<Object>{tmp3}, TNode<IntPtrT>{tmp10}).Flatten();
    tmp13 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp11, tmp12});
    compiler::CodeAssemblerLabel label15(&ca_);
    tmp14 = Cast_Smi_0(state_, TNode<Object>{tmp13}, &label15);
    ca_.Goto(&block15, phi_bb10_5, phi_bb10_6);
    if (label15.is_used()) {
      ca_.Bind(&label15);
      ca_.Goto(&block16, phi_bb10_5, phi_bb10_6);
    }
  }

  TNode<Smi> phi_bb11_5;
  TNode<IntPtrT> phi_bb11_6;
  TNode<IntPtrT> phi_bb11_11;
  TNode<IntPtrT> phi_bb11_12;
  TNode<IntPtrT> phi_bb11_16;
  TNode<IntPtrT> phi_bb11_17;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_5, &phi_bb11_6, &phi_bb11_11, &phi_bb11_12, &phi_bb11_16, &phi_bb11_17);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Smi> phi_bb16_5;
  TNode<IntPtrT> phi_bb16_6;
  TNode<String> tmp16;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_5, &phi_bb16_6);
    compiler::CodeAssemblerLabel label17(&ca_);
    tmp16 = Cast_String_0(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(tmp13)}, &label17);
    ca_.Goto(&block29, phi_bb16_5, phi_bb16_6);
    if (label17.is_used()) {
      ca_.Bind(&label17);
      ca_.Goto(&block30, phi_bb16_5, phi_bb16_6);
    }
  }

  TNode<Smi> phi_bb15_5;
  TNode<IntPtrT> phi_bb15_6;
  TNode<Smi> tmp18;
  TNode<BoolT> tmp19;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_5, &phi_bb15_6);
    tmp18 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp19 = CodeStubAssembler(state_).SmiGreaterThan(TNode<Smi>{tmp14}, TNode<Smi>{tmp18});
    ca_.Branch(tmp19, &block17, std::vector<compiler::Node*>{phi_bb15_5, phi_bb15_6}, &block18, std::vector<compiler::Node*>{phi_bb15_5, phi_bb15_6});
  }

  TNode<Smi> phi_bb17_5;
  TNode<IntPtrT> phi_bb17_6;
  TNode<IntPtrT> tmp20;
  TNode<IntPtrT> tmp21;
  TNode<IntPtrT> tmp22;
  TNode<IntPtrT> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<IntPtrT> tmp25;
  TNode<Smi> tmp26;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_5, &phi_bb17_6);
    tmp20 = Convert_intptr_Smi_0(state_, TNode<Smi>{tmp14});
    tmp21 = FromConstexpr_intptr_constexpr_int31_0(state_, 11);
    tmp22 = CodeStubAssembler(state_).WordSar(TNode<IntPtrT>{tmp20}, TNode<IntPtrT>{tmp21});
    tmp23 = FromConstexpr_intptr_constexpr_int31_0(state_, 2047);
    tmp24 = CodeStubAssembler(state_).WordAnd(TNode<IntPtrT>{tmp20}, TNode<IntPtrT>{tmp23});
    tmp25 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp22}, TNode<IntPtrT>{tmp24});
    tmp26 = Convert_Smi_intptr_0(state_, TNode<IntPtrT>{tmp25});
    ca_.Goto(&block19, tmp26, phi_bb17_6);
  }

  TNode<Smi> phi_bb18_5;
  TNode<IntPtrT> phi_bb18_6;
  TNode<Object> tmp27;
  TNode<IntPtrT> tmp28;
  TNode<IntPtrT> tmp29;
  TNode<IntPtrT> tmp30;
  TNode<IntPtrT> tmp31;
  TNode<UintPtrT> tmp32;
  TNode<UintPtrT> tmp33;
  TNode<BoolT> tmp34;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_5, &phi_bb18_6);
    std::tie(tmp27, tmp28, tmp29) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_matchesElements}).Flatten();
    tmp30 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp31 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb18_6}, TNode<IntPtrT>{tmp30});
    tmp32 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp31});
    tmp33 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp29});
    tmp34 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp32}, TNode<UintPtrT>{tmp33});
    ca_.Branch(tmp34, &block24, std::vector<compiler::Node*>{phi_bb18_5}, &block25, std::vector<compiler::Node*>{phi_bb18_5});
  }

  TNode<Smi> phi_bb24_5;
  TNode<IntPtrT> tmp35;
  TNode<IntPtrT> tmp36;
  TNode<Object> tmp37;
  TNode<IntPtrT> tmp38;
  TNode<Object> tmp39;
  TNode<Smi> tmp40;
  TNode<Smi> tmp41;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_5);
    tmp35 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp31});
    tmp36 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp28}, TNode<IntPtrT>{tmp35});
    std::tie(tmp37, tmp38) = NewReference_Object_0(state_, TNode<Object>{tmp27}, TNode<IntPtrT>{tmp36}).Flatten();
    tmp39 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp37, tmp38});
    tmp40 = UnsafeCast_Smi_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp39});
    tmp41 = CodeStubAssembler(state_).SmiSub(TNode<Smi>{tmp40}, TNode<Smi>{tmp14});
    ca_.Goto(&block19, tmp41, tmp31);
  }

  TNode<Smi> phi_bb25_5;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_5);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Smi> phi_bb19_5;
  TNode<IntPtrT> phi_bb19_6;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_5, &phi_bb19_6);
    ca_.Goto(&block13, phi_bb19_5, phi_bb19_6);
  }

  TNode<Smi> phi_bb30_5;
  TNode<IntPtrT> phi_bb30_6;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_5, &phi_bb30_6);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Smi> phi_bb29_5;
  TNode<IntPtrT> phi_bb29_6;
  TNode<Oddball> tmp42;
  TNode<Object> tmp43;
  TNode<String> tmp44;
  TNode<Object> tmp45;
  TNode<IntPtrT> tmp46;
  TNode<IntPtrT> tmp47;
  TNode<UintPtrT> tmp48;
  TNode<UintPtrT> tmp49;
  TNode<BoolT> tmp50;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_5, &phi_bb29_6);
    tmp42 = Undefined_0(state_);
    tmp43 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{p_replaceFn}, TNode<Object>{tmp42}, TNode<Object>{tmp16}, TNode<Object>{phi_bb29_5}, TNode<Object>{p_string});
    tmp44 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{tmp43});
    std::tie(tmp45, tmp46, tmp47) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_matchesElements}).Flatten();
    tmp48 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb29_6});
    tmp49 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp47});
    tmp50 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp48}, TNode<UintPtrT>{tmp49});
    ca_.Branch(tmp50, &block35, std::vector<compiler::Node*>{phi_bb29_5, phi_bb29_6, phi_bb29_6, phi_bb29_6, phi_bb29_6, phi_bb29_6}, &block36, std::vector<compiler::Node*>{phi_bb29_5, phi_bb29_6, phi_bb29_6, phi_bb29_6, phi_bb29_6, phi_bb29_6});
  }

  TNode<Smi> phi_bb35_5;
  TNode<IntPtrT> phi_bb35_6;
  TNode<IntPtrT> phi_bb35_15;
  TNode<IntPtrT> phi_bb35_16;
  TNode<IntPtrT> phi_bb35_20;
  TNode<IntPtrT> phi_bb35_21;
  TNode<IntPtrT> tmp51;
  TNode<IntPtrT> tmp52;
  TNode<Object> tmp53;
  TNode<IntPtrT> tmp54;
  TNode<Smi> tmp55;
  TNode<Smi> tmp56;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_5, &phi_bb35_6, &phi_bb35_15, &phi_bb35_16, &phi_bb35_20, &phi_bb35_21);
    tmp51 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb35_21});
    tmp52 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp46}, TNode<IntPtrT>{tmp51});
    std::tie(tmp53, tmp54) = NewReference_Object_0(state_, TNode<Object>{tmp45}, TNode<IntPtrT>{tmp52}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp53, tmp54}, tmp44);
    tmp55 = CodeStubAssembler(state_).LoadStringLengthAsSmi(TNode<String>{tmp16});
    tmp56 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb35_5}, TNode<Smi>{tmp55});
    ca_.Goto(&block13, tmp56, phi_bb35_6);
  }

  TNode<Smi> phi_bb36_5;
  TNode<IntPtrT> phi_bb36_6;
  TNode<IntPtrT> phi_bb36_15;
  TNode<IntPtrT> phi_bb36_16;
  TNode<IntPtrT> phi_bb36_20;
  TNode<IntPtrT> phi_bb36_21;
  if (block36.is_used()) {
    ca_.Bind(&block36, &phi_bb36_5, &phi_bb36_6, &phi_bb36_15, &phi_bb36_16, &phi_bb36_20, &phi_bb36_21);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Smi> phi_bb13_5;
  TNode<IntPtrT> phi_bb13_6;
  TNode<IntPtrT> tmp57;
  TNode<IntPtrT> tmp58;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_5, &phi_bb13_6);
    tmp57 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp58 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb13_6}, TNode<IntPtrT>{tmp57});
    ca_.Goto(&block4, phi_bb13_5, tmp58);
  }

  TNode<Smi> phi_bb3_5;
  TNode<IntPtrT> phi_bb3_6;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_5, &phi_bb3_6);
    ca_.Goto(&block38);
  }

    ca_.Bind(&block38);
}


void RegExpReplaceCallableWithExplicitCaptures_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FixedArray> p_matchesElements, TNode<IntPtrT> p_matchesLength, TNode<JSReceiver> p_replaceFn) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block4, tmp0);
  }

  TNode<IntPtrT> phi_bb4_4;
  TNode<BoolT> tmp1;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_4);
    tmp1 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{phi_bb4_4}, TNode<IntPtrT>{p_matchesLength});
    ca_.Branch(tmp1, &block2, std::vector<compiler::Node*>{phi_bb4_4}, &block3, std::vector<compiler::Node*>{phi_bb4_4});
  }

  TNode<IntPtrT> phi_bb2_4;
  TNode<Object> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<UintPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  TNode<BoolT> tmp7;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_4);
    std::tie(tmp2, tmp3, tmp4) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_matchesElements}).Flatten();
    tmp5 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb2_4});
    tmp6 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp4});
    tmp7 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp5}, TNode<UintPtrT>{tmp6});
    ca_.Branch(tmp7, &block12, std::vector<compiler::Node*>{phi_bb2_4, phi_bb2_4, phi_bb2_4, phi_bb2_4, phi_bb2_4}, &block13, std::vector<compiler::Node*>{phi_bb2_4, phi_bb2_4, phi_bb2_4, phi_bb2_4, phi_bb2_4});
  }

  TNode<IntPtrT> phi_bb12_4;
  TNode<IntPtrT> phi_bb12_9;
  TNode<IntPtrT> phi_bb12_10;
  TNode<IntPtrT> phi_bb12_14;
  TNode<IntPtrT> phi_bb12_15;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<Object> tmp12;
  TNode<JSArray> tmp13;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_4, &phi_bb12_9, &phi_bb12_10, &phi_bb12_14, &phi_bb12_15);
    tmp8 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb12_15});
    tmp9 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp8});
    std::tie(tmp10, tmp11) = NewReference_Object_0(state_, TNode<Object>{tmp2}, TNode<IntPtrT>{tmp9}).Flatten();
    tmp12 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp10, tmp11});
    compiler::CodeAssemblerLabel label14(&ca_);
    tmp13 = Cast_JSArray_1(state_, TNode<Context>{p_context}, TNode<Object>{tmp12}, &label14);
    ca_.Goto(&block15, phi_bb12_4);
    if (label14.is_used()) {
      ca_.Bind(&label14);
      ca_.Goto(&block16, phi_bb12_4);
    }
  }

  TNode<IntPtrT> phi_bb13_4;
  TNode<IntPtrT> phi_bb13_9;
  TNode<IntPtrT> phi_bb13_10;
  TNode<IntPtrT> phi_bb13_14;
  TNode<IntPtrT> phi_bb13_15;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_4, &phi_bb13_9, &phi_bb13_10, &phi_bb13_14, &phi_bb13_15);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb16_4;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_4);
    ca_.Goto(&block5, phi_bb16_4);
  }

  TNode<IntPtrT> phi_bb15_4;
  TNode<JSReceiver> tmp15;
  TNode<Oddball> tmp16;
  TNode<Oddball> tmp17;
  TNode<Object> tmp18;
  TNode<Object> tmp19;
  TNode<IntPtrT> tmp20;
  TNode<IntPtrT> tmp21;
  TNode<UintPtrT> tmp22;
  TNode<UintPtrT> tmp23;
  TNode<BoolT> tmp24;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_4);
    tmp15 = GetReflectApply_0(state_, TNode<Context>{p_context});
    tmp16 = Undefined_0(state_);
    tmp17 = Undefined_0(state_);
    tmp18 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp15}, TNode<Object>{tmp16}, TNode<Object>{p_replaceFn}, TNode<Object>{tmp17}, TNode<Object>{tmp13});
    std::tie(tmp19, tmp20, tmp21) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_matchesElements}).Flatten();
    tmp22 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb15_4});
    tmp23 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp21});
    tmp24 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp22}, TNode<UintPtrT>{tmp23});
    ca_.Branch(tmp24, &block21, std::vector<compiler::Node*>{phi_bb15_4, phi_bb15_4, phi_bb15_4, phi_bb15_4, phi_bb15_4}, &block22, std::vector<compiler::Node*>{phi_bb15_4, phi_bb15_4, phi_bb15_4, phi_bb15_4, phi_bb15_4});
  }

  TNode<IntPtrT> phi_bb21_4;
  TNode<IntPtrT> phi_bb21_11;
  TNode<IntPtrT> phi_bb21_12;
  TNode<IntPtrT> phi_bb21_16;
  TNode<IntPtrT> phi_bb21_17;
  TNode<IntPtrT> tmp25;
  TNode<IntPtrT> tmp26;
  TNode<Object> tmp27;
  TNode<IntPtrT> tmp28;
  TNode<String> tmp29;
  if (block21.is_used()) {
    ca_.Bind(&block21, &phi_bb21_4, &phi_bb21_11, &phi_bb21_12, &phi_bb21_16, &phi_bb21_17);
    tmp25 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb21_17});
    tmp26 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp20}, TNode<IntPtrT>{tmp25});
    std::tie(tmp27, tmp28) = NewReference_Object_0(state_, TNode<Object>{tmp19}, TNode<IntPtrT>{tmp26}).Flatten();
    tmp29 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{tmp18});
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp27, tmp28}, tmp29);
    ca_.Goto(&block5, phi_bb21_4);
  }

  TNode<IntPtrT> phi_bb22_4;
  TNode<IntPtrT> phi_bb22_11;
  TNode<IntPtrT> phi_bb22_12;
  TNode<IntPtrT> phi_bb22_16;
  TNode<IntPtrT> phi_bb22_17;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_4, &phi_bb22_11, &phi_bb22_12, &phi_bb22_16, &phi_bb22_17);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb5_4;
  TNode<IntPtrT> tmp30;
  TNode<IntPtrT> tmp31;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_4);
    tmp30 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp31 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb5_4}, TNode<IntPtrT>{tmp30});
    ca_.Goto(&block4, tmp31);
  }

  TNode<IntPtrT> phi_bb3_4;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_4);
    ca_.Goto(&block24);
  }

    ca_.Bind(&block24);
}


TNode<String> RegExpReplaceFastGlobalCallable_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSRegExp> p_regexp, TNode<String> p_string, TNode<JSReceiver> p_replaceFn) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Smi> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Smi> tmp2;
  TNode<RegExpMatchInfo> tmp3;
  TNode<Map> tmp4;
  TNode<JSArray> tmp5;
  TNode<HeapObject> tmp6;
  TNode<Smi> tmp7;
  TNode<Oddball> tmp8;
  TNode<BoolT> tmp9;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    RegExpBuiltinsAssembler(state_).FastStoreLastIndex(TNode<JSRegExp>{p_regexp}, TNode<Smi>{tmp0});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp2 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp3 = GetRegExpLastMatchInfo_0(state_, TNode<Context>{p_context});
    tmp4 = GetFastPackedElementsJSArrayMap_0(state_, TNode<Context>{p_context});
    tmp5 = CodeStubAssembler(state_).AllocateJSArray(ElementsKind::PACKED_ELEMENTS, TNode<Map>{tmp4}, TNode<IntPtrT>{tmp1}, TNode<Smi>{tmp2});
    tmp6 = TORQUE_CAST(CodeStubAssembler(state_).CallRuntime(Runtime::kRegExpExecMultiple, p_context, p_regexp, p_string, tmp3, tmp5)); 
    tmp7 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    RegExpBuiltinsAssembler(state_).FastStoreLastIndex(TNode<JSRegExp>{p_regexp}, TNode<Smi>{tmp7});
    tmp8 = Null_0(state_);
    tmp9 = CodeStubAssembler(state_).TaggedEqual(TNode<HeapObject>{tmp6}, TNode<HeapObject>{tmp8});
    ca_.Branch(tmp9, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  if (block2.is_used()) {
    ca_.Bind(&block2);
    ca_.Goto(&block1, p_string);
  }

  TNode<JSArray> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<Number> tmp12;
  TNode<Smi> tmp13;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp10 = UnsafeCast_JSArray_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp6});
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp12 = CodeStubAssembler(state_).LoadReference<Number>(CodeStubAssembler::Reference{tmp10, tmp11});
    compiler::CodeAssemblerLabel label14(&ca_);
    tmp13 = Cast_Smi_0(state_, TNode<Object>{tmp12}, &label14);
    ca_.Goto(&block6);
    if (label14.is_used()) {
      ca_.Bind(&label14);
      ca_.Goto(&block7);
    }
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp15;
  TNode<IntPtrT> tmp16;
  TNode<FixedArrayBase> tmp17;
  TNode<FixedArray> tmp18;
  TNode<RegExpMatchInfo> tmp19;
  TNode<Smi> tmp20;
  TNode<Smi> tmp21;
  TNode<BoolT> tmp22;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp15 = Convert_intptr_Smi_0(state_, TNode<Smi>{tmp13});
    tmp16 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp17 = CodeStubAssembler(state_).LoadReference<FixedArrayBase>(CodeStubAssembler::Reference{tmp10, tmp16});
    tmp18 = UnsafeCast_FixedArray_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp17});
    tmp19 = GetRegExpLastMatchInfo_0(state_, TNode<Context>{p_context});
    tmp20 = Method_RegExpMatchInfo_NumberOfCaptures_0(state_, TNode<Context>{p_context}, TNode<RegExpMatchInfo>{tmp19});
    tmp21 = FromConstexpr_Smi_constexpr_int31_0(state_, 2);
    tmp22 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp20}, TNode<Smi>{tmp21});
    ca_.Branch(tmp22, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    RegExpReplaceCallableNoExplicitCaptures_0(state_, TNode<Context>{p_context}, TNode<FixedArray>{tmp18}, TNode<IntPtrT>{tmp15}, TNode<String>{p_string}, TNode<JSReceiver>{p_replaceFn});
    ca_.Goto(&block10);
  }

  if (block9.is_used()) {
    ca_.Bind(&block9);
    RegExpReplaceCallableWithExplicitCaptures_0(state_, TNode<Context>{p_context}, TNode<FixedArray>{tmp18}, TNode<IntPtrT>{tmp15}, TNode<JSReceiver>{p_replaceFn});
    ca_.Goto(&block10);
  }

  TNode<String> tmp23;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp23 = TORQUE_CAST(CodeStubAssembler(state_).CallRuntime(Runtime::kStringBuilderConcat, p_context, tmp10, tmp13, p_string)); 
    ca_.Goto(&block1, tmp23);
  }

  TNode<String> phi_bb1_4;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_4);
    ca_.Goto(&block11, phi_bb1_4);
  }

  TNode<String> phi_bb11_4;
    ca_.Bind(&block11, &phi_bb11_4);
  return TNode<String>{phi_bb11_4};
}


TNode<String> RegExpReplaceFastString_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSRegExp> p_regexp, TNode<String> p_string, TNode<String> p_replaceString) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, Smi> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, Smi> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, Smi> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, Smi> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, Smi> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<Smi> tmp1;
  TNode<BoolT> tmp2;
  TNode<Smi> tmp3;
  TNode<JSRegExp> tmp4;
  TNode<BoolT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp2 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    tmp3 = CodeStubAssembler(state_).LoadStringLengthAsSmi(TNode<String>{p_replaceString});
    tmp4 = UnsafeCast_FastJSRegExp_0(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp});
    tmp5 = RegExpBuiltinsAssembler(state_).FastFlagGetterGlobal(TNode<JSRegExp>{tmp4});
    ca_.Branch(tmp5, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{tmp2});
  }

  TNode<BoolT> tmp6;
  TNode<Smi> tmp7;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp6 = RegExpBuiltinsAssembler(state_).FastFlagGetterUnicode(TNode<JSRegExp>{tmp4});
    tmp7 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    RegExpBuiltinsAssembler(state_).FastStoreLastIndex(TNode<JSRegExp>{tmp4}, TNode<Smi>{tmp7});
    ca_.Goto(&block3, tmp6);
  }

  TNode<BoolT> phi_bb3_6;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_6);
    ca_.Goto(&block6, tmp0, tmp1);
  }

  TNode<String> phi_bb6_4;
  TNode<Smi> phi_bb6_5;
  TNode<BoolT> tmp8;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_4, &phi_bb6_5);
    tmp8 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp8, &block4, std::vector<compiler::Node*>{phi_bb6_4, phi_bb6_5}, &block5, std::vector<compiler::Node*>{phi_bb6_4, phi_bb6_5});
  }

  TNode<String> phi_bb4_4;
  TNode<Smi> phi_bb4_5;
  TNode<RegExpMatchInfo> tmp9;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_4, &phi_bb4_5);
    compiler::CodeAssemblerLabel label10(&ca_);
    tmp9 = RegExpPrototypeExecBodyWithoutResultFast_0(state_, TNode<Context>{p_context}, TNode<JSRegExp>{p_regexp}, TNode<String>{p_string}, &label10);
    ca_.Goto(&block9, phi_bb4_4, phi_bb4_5);
    if (label10.is_used()) {
      ca_.Bind(&label10);
      ca_.Goto(&block10, phi_bb4_4, phi_bb4_5);
    }
  }

  TNode<String> phi_bb10_4;
  TNode<Smi> phi_bb10_5;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_4, &phi_bb10_5);
    ca_.Goto(&block5, phi_bb10_4, phi_bb10_5);
  }

  TNode<String> phi_bb9_4;
  TNode<Smi> phi_bb9_5;
  TNode<Smi> tmp11;
  TNode<Smi> tmp12;
  TNode<String> tmp13;
  TNode<String> tmp14;
  TNode<Smi> tmp15;
  TNode<BoolT> tmp16;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_4, &phi_bb9_5);
    tmp11 = Method_RegExpMatchInfo_GetStartOfCapture_0(state_, TNode<Context>{p_context}, TNode<RegExpMatchInfo>{tmp9}, 0);
    tmp12 = Method_RegExpMatchInfo_GetEndOfCapture_0(state_, TNode<Context>{p_context}, TNode<RegExpMatchInfo>{tmp9}, 0);
    tmp13 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kSubString), p_context, p_string, phi_bb9_5, tmp11);
    tmp14 = StringAdd_0(state_, TNode<Context>{p_context}, TNode<String>{phi_bb9_4}, TNode<String>{tmp13});
    tmp15 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp16 = CodeStubAssembler(state_).SmiNotEqual(TNode<Smi>{tmp3}, TNode<Smi>{tmp15});
    ca_.Branch(tmp16, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{tmp14});
  }

  TNode<String> tmp17;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp17 = StringAdd_0(state_, TNode<Context>{p_context}, TNode<String>{tmp14}, TNode<String>{p_replaceString});
    ca_.Goto(&block12, tmp17);
  }

  TNode<String> phi_bb12_4;
  TNode<BoolT> tmp18;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_4);
    tmp18 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp5});
    ca_.Branch(tmp18, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block5, phi_bb12_4, tmp12);
  }

  TNode<BoolT> tmp19;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp19 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp12}, TNode<Smi>{tmp11});
    ca_.Branch(tmp19, &block15, std::vector<compiler::Node*>{}, &block16, std::vector<compiler::Node*>{});
  }

  TNode<JSRegExp> tmp20;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    compiler::CodeAssemblerLabel label21(&ca_);
    tmp20 = Cast_FastJSRegExp_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{p_regexp}, &label21);
    ca_.Goto(&block19);
    if (label21.is_used()) {
      ca_.Bind(&label21);
      ca_.Goto(&block20);
    }
  }

  TNode<Object> tmp22;
  TNode<Number> tmp23;
  TNode<Smi> tmp24;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp22 = RegExpBuiltinsAssembler(state_).SlowLoadLastIndex(TNode<Context>{p_context}, TNode<Object>{p_regexp});
    tmp23 = CodeStubAssembler(state_).ToLength_Inline(TNode<Context>{p_context}, TNode<Object>{tmp22});
    tmp24 = RegExpBuiltinsAssembler(state_).AdvanceStringIndexSlow(TNode<String>{p_string}, TNode<Number>{tmp23}, TNode<BoolT>{phi_bb3_6});
    RegExpBuiltinsAssembler(state_).SlowStoreLastIndex(TNode<Context>{p_context}, TNode<Object>{p_regexp}, TNode<Object>{tmp24});
    ca_.Goto(&block17);
  }

  TNode<Smi> tmp25;
  TNode<Smi> tmp26;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    tmp25 = RegExpBuiltinsAssembler(state_).FastLoadLastIndex(TNode<JSRegExp>{tmp20});
    tmp26 = RegExpBuiltinsAssembler(state_).AdvanceStringIndexFast(TNode<String>{p_string}, TNode<Smi>{tmp25}, TNode<BoolT>{phi_bb3_6});
    RegExpBuiltinsAssembler(state_).FastStoreLastIndex(TNode<JSRegExp>{tmp20}, TNode<Smi>{tmp26});
    ca_.Goto(&block17);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block16);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block6, phi_bb12_4, tmp12);
  }

  TNode<String> phi_bb5_4;
  TNode<Smi> phi_bb5_5;
  TNode<Smi> tmp27;
  TNode<String> tmp28;
  TNode<String> tmp29;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_4, &phi_bb5_5);
    tmp27 = CodeStubAssembler(state_).LoadStringLengthAsSmi(TNode<String>{p_string});
    tmp28 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kSubString), p_context, p_string, phi_bb5_5, tmp27);
    tmp29 = StringAdd_0(state_, TNode<Context>{p_context}, TNode<String>{phi_bb5_4}, TNode<String>{tmp28});
    ca_.Goto(&block21);
  }

    ca_.Bind(&block21);
  return TNode<String>{tmp29};
}


TF_BUILTIN(RegExpReplace, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSRegExp> parameter1 = UncheckedParameter<JSRegExp>(Descriptor::kRegexp);
  USE(parameter1);
  TNode<String> parameter2 = UncheckedParameter<String>(Descriptor::kString);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kReplaceValue);
  USE(parameter3);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter3}, &label1);
    ca_.Goto(&block7);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block8);
    }
  }

  TNode<String> tmp2;
  TNode<JSRegExp> tmp3;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp2 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter3});
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_FastJSRegExp_0(state_, TNode<Context>{parameter0}, TNode<HeapObject>{parameter1}, &label4);
    ca_.Goto(&block15);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block16);
    }
  }

  TNode<BoolT> tmp5;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp5 = RegExpBuiltinsAssembler(state_).FastFlagGetterGlobal(TNode<JSRegExp>{parameter1});
    ca_.Branch(tmp5, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp6;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp6 = RegExpReplaceFastGlobalCallable_0(state_, TNode<Context>{parameter0}, TNode<JSRegExp>{parameter1}, TNode<String>{parameter2}, TNode<JSReceiver>{tmp0});
    ca_.Goto(&block11, tmp6);
  }

  TNode<String> tmp7;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp7 = TORQUE_CAST(CodeStubAssembler(state_).CallRuntime(Runtime::kStringReplaceNonGlobalRegExpWithFunction, parameter0, parameter2, parameter1, tmp0)); 
    ca_.Goto(&block11, tmp7);
  }

  TNode<String> phi_bb11_6;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_6);
    CodeStubAssembler(state_).Return(phi_bb11_6);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block14);
  }

  TNode<String> tmp8;
  TNode<Smi> tmp9;
  TNode<Smi> tmp10;
  TNode<Smi> tmp11;
  TNode<BoolT> tmp12;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp8 = CodeStubAssembler(state_).SingleCharacterStringConstant("$");
    tmp9 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp10 = ca_.CallStub<Smi>(Builtins::CallableFor(ca_.isolate(), Builtin::kStringIndexOf), parameter0, tmp2, tmp8, tmp9);
    tmp11 = FromConstexpr_Smi_constexpr_int31_0(state_, -1);
    tmp12 = CodeStubAssembler(state_).SmiNotEqual(TNode<Smi>{tmp10}, TNode<Smi>{tmp11});
    ca_.Branch(tmp12, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block14);
  }

  TNode<String> tmp13;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    tmp13 = RegExpReplaceFastString_0(state_, TNode<Context>{parameter0}, TNode<JSRegExp>{tmp3}, TNode<String>{parameter2}, TNode<String>{tmp2});
    CodeStubAssembler(state_).Return(tmp13);
  }

  TNode<String> tmp14;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp14 = TORQUE_CAST(CodeStubAssembler(state_).CallRuntime(Runtime::kRegExpReplaceRT, parameter0, parameter1, parameter2, tmp2)); 
    CodeStubAssembler(state_).Return(tmp14);
  }
}

TF_BUILTIN(RegExpPrototypeReplace, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<JSReceiver> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp3 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp2});
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_JSReceiver_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label5);
    ca_.Goto(&block3);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kIncompatibleMethodReceiver, "RegExp.prototype.@@replace");
  }

  TNode<String> tmp6;
  TNode<JSRegExp> tmp7;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp6 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{tmp1});
    compiler::CodeAssemblerLabel label8(&ca_);
    tmp7 = Cast_FastJSRegExp_0(state_, TNode<Context>{parameter0}, TNode<HeapObject>{tmp4}, &label8);
    ca_.Goto(&block7);
    if (label8.is_used()) {
      ca_.Bind(&label8);
      ca_.Goto(&block8);
    }
  }

  TNode<Smi> tmp9;
  TNode<String> tmp10;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp9 = CodeStubAssembler(state_).SmiConstant(v8::Isolate::kRegExpReplaceCalledOnSlowRegExp);
    CodeStubAssembler(state_).CallRuntime(Runtime::kIncrementUseCounter, parameter0, tmp9);
    tmp10 = TORQUE_CAST(CodeStubAssembler(state_).CallRuntime(Runtime::kRegExpReplaceRT, parameter0, tmp4, tmp6, tmp3)); 
    arguments.PopAndReturn(tmp10);
  }

  TNode<String> tmp11;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp11 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kRegExpReplace), parameter0, tmp7, tmp6, tmp3);
    arguments.PopAndReturn(tmp11);
  }
}

TNode<BoolT> Is_FastJSRegExp_FastJSRegExp_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSRegExp> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSRegExp> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_FastJSRegExp_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{p_o}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  TNode<BoolT> tmp2;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp2);
  }

  TNode<BoolT> tmp3;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp3 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp3);
  }

  TNode<BoolT> phi_bb1_2;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_2);
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<BoolT>{phi_bb1_2};
}


} // namespace internal
} // namespace v8
