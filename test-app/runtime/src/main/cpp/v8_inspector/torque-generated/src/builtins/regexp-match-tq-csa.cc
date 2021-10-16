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
#include "torque-generated/src/builtins/regexp-match-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/array-reverse-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/boolean-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/growable-fixed-array-tq-csa.h"
#include "torque-generated/src/builtins/number-tq-csa.h"
#include "torque-generated/src/builtins/regexp-exec-tq-csa.h"
#include "torque-generated/src/builtins/regexp-match-all-tq-csa.h"
#include "torque-generated/src/builtins/regexp-match-tq-csa.h"
#include "torque-generated/src/builtins/regexp-replace-tq-csa.h"
#include "torque-generated/src/builtins/regexp-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-tq-csa.h"
#include "torque-generated/src/objects/regexp-match-info-tq-csa.h"
#include "torque-generated/src/objects/string-tq-csa.h"

namespace v8 {
namespace internal {
TNode<Object> RegExpPrototypeMatchBody_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_regexp, TNode<String> p_string, bool p_isFastPath) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block49(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block48(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block50(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block51(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String> block52(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block57(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block58(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block59(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block60(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, HeapObject> block61(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String, Object> block83(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String, Object> block84(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String, String, Object, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block102(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String, String, Object, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block103(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String> block105(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String> block106(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String> block107(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String> block108(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String, Object> block109(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String> block114(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String> block115(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String, String> block116(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block39(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block121(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    if ((p_isFastPath)) {
      ca_.Goto(&block2);
    } else {
      ca_.Goto(&block3);
    }
  }

  if (block2.is_used()) {
    ca_.Bind(&block2);
    ca_.Goto(&block4);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block4);
  }

  TNode<BoolT> tmp0;
  TNode<BoolT> tmp1;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp0 = RegExpBuiltinsAssembler(state_).FlagGetter(TNode<Context>{p_context}, TNode<Object>{p_regexp}, JSRegExp::Flag::kGlobal, p_isFastPath);
    tmp1 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp0});
    ca_.Branch(tmp1, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp2;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp2 = FromConstexpr_bool_constexpr_bool_0(state_, p_isFastPath);
    ca_.Branch(tmp2, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp3;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp3 = RegExpPrototypeExecBodyFast_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_regexp}, TNode<String>{p_string});
    ca_.Goto(&block13, tmp3);
  }

  TNode<Object> tmp4;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp4 = RegExpExec_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_regexp}, TNode<String>{p_string});
    ca_.Goto(&block13, tmp4);
  }

  TNode<Object> phi_bb13_4;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_4);
    ca_.Goto(&block1, phi_bb13_4);
  }

  TNode<BoolT> tmp5;
  TNode<Number> tmp6;
  TNode<FixedArray> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<BoolT> tmp10;
  TNode<String> tmp11;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp5 = RegExpBuiltinsAssembler(state_).FlagGetter(TNode<Context>{p_context}, TNode<Object>{p_regexp}, JSRegExp::Flag::kUnicode, p_isFastPath);
    tmp6 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    StoreLastIndex_0(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp}, TNode<Number>{tmp6}, p_isFastPath);
    std::tie(tmp7, tmp8, tmp9) = NewGrowableFixedArray_0(state_).Flatten();
    tmp10 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    tmp11 = CodeStubAssembler(state_).EmptyStringConstant();
    if ((p_isFastPath)) {
      ca_.Goto(&block19);
    } else {
      ca_.Goto(&block20);
    }
  }

  TNode<JSRegExp> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<HeapObject> tmp14;
  TNode<FixedArray> tmp15;
  TNode<Object> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<IntPtrT> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<UintPtrT> tmp20;
  TNode<UintPtrT> tmp21;
  TNode<BoolT> tmp22;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    tmp12 = UnsafeCast_JSRegExp_0(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp});
    tmp13 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp14 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{tmp12, tmp13});
    tmp15 = UnsafeCast_FixedArray_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp14});
    std::tie(tmp16, tmp17, tmp18) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp15}).Flatten();
    tmp19 = Convert_intptr_constexpr_int31_0(state_, JSRegExp::kTagIndex);
    tmp20 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp19});
    tmp21 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp18});
    tmp22 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp20}, TNode<UintPtrT>{tmp21});
    ca_.Branch(tmp22, &block28, std::vector<compiler::Node*>{}, &block29, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<Object> tmp25;
  TNode<IntPtrT> tmp26;
  TNode<Object> tmp27;
  TNode<Smi> tmp28;
  TNode<Smi> tmp29;
  TNode<BoolT> tmp30;
  if (block28.is_used()) {
    ca_.Bind(&block28);
    tmp23 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp19});
    tmp24 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp17}, TNode<IntPtrT>{tmp23});
    std::tie(tmp25, tmp26) = NewReference_Object_0(state_, TNode<Object>{tmp16}, TNode<IntPtrT>{tmp24}).Flatten();
    tmp27 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp25, tmp26});
    tmp28 = UnsafeCast_Smi_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp27});
    tmp29 = FromConstexpr_Smi_constexpr_int31_0(state_, JSRegExp::ATOM);
    tmp30 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp28}, TNode<Smi>{tmp29});
    ca_.Branch(tmp30, &block22, std::vector<compiler::Node*>{}, &block23, std::vector<compiler::Node*>{tmp10, tmp11});
  }

  if (block29.is_used()) {
    ca_.Bind(&block29);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> tmp31;
  TNode<IntPtrT> tmp32;
  TNode<IntPtrT> tmp33;
  TNode<IntPtrT> tmp34;
  TNode<UintPtrT> tmp35;
  TNode<UintPtrT> tmp36;
  TNode<BoolT> tmp37;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    std::tie(tmp31, tmp32, tmp33) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp15}).Flatten();
    tmp34 = Convert_intptr_constexpr_int31_0(state_, JSRegExp::kAtomPatternIndex);
    tmp35 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp34});
    tmp36 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp33});
    tmp37 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp35}, TNode<UintPtrT>{tmp36});
    ca_.Branch(tmp37, &block35, std::vector<compiler::Node*>{}, &block36, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp38;
  TNode<IntPtrT> tmp39;
  TNode<Object> tmp40;
  TNode<IntPtrT> tmp41;
  TNode<Object> tmp42;
  TNode<String> tmp43;
  TNode<BoolT> tmp44;
  if (block35.is_used()) {
    ca_.Bind(&block35);
    tmp38 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp34});
    tmp39 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp32}, TNode<IntPtrT>{tmp38});
    std::tie(tmp40, tmp41) = NewReference_Object_0(state_, TNode<Object>{tmp31}, TNode<IntPtrT>{tmp39}).Flatten();
    tmp42 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp40, tmp41});
    tmp43 = UnsafeCast_String_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp42});
    tmp44 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block23, tmp44, tmp43);
  }

  if (block36.is_used()) {
    ca_.Bind(&block36);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BoolT> phi_bb23_8;
  TNode<String> phi_bb23_9;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_8, &phi_bb23_9);
    ca_.Goto(&block21, phi_bb23_8, phi_bb23_9);
  }

  if (block20.is_used()) {
    ca_.Bind(&block20);
    ca_.Goto(&block21, tmp10, tmp11);
  }

  TNode<BoolT> phi_bb21_8;
  TNode<String> phi_bb21_9;
  if (block21.is_used()) {
    ca_.Bind(&block21, &phi_bb21_8, &phi_bb21_9);
    ca_.Goto(&block40, tmp7, tmp8, tmp9, phi_bb21_8, phi_bb21_9);
  }

  TNode<FixedArray> phi_bb40_5;
  TNode<IntPtrT> phi_bb40_6;
  TNode<IntPtrT> phi_bb40_7;
  TNode<BoolT> phi_bb40_8;
  TNode<String> phi_bb40_9;
  TNode<BoolT> tmp45;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_5, &phi_bb40_6, &phi_bb40_7, &phi_bb40_8, &phi_bb40_9);
    tmp45 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp45, &block38, std::vector<compiler::Node*>{phi_bb40_5, phi_bb40_6, phi_bb40_7, phi_bb40_8, phi_bb40_9}, &block39, std::vector<compiler::Node*>{phi_bb40_5, phi_bb40_6, phi_bb40_7, phi_bb40_8, phi_bb40_9});
  }

  TNode<FixedArray> phi_bb38_5;
  TNode<IntPtrT> phi_bb38_6;
  TNode<IntPtrT> phi_bb38_7;
  TNode<BoolT> phi_bb38_8;
  TNode<String> phi_bb38_9;
  TNode<String> tmp46;
  if (block38.is_used()) {
    ca_.Bind(&block38, &phi_bb38_5, &phi_bb38_6, &phi_bb38_7, &phi_bb38_8, &phi_bb38_9);
    tmp46 = CodeStubAssembler(state_).EmptyStringConstant();
    if ((p_isFastPath)) {
      ca_.Goto(&block45, phi_bb38_5, phi_bb38_6, phi_bb38_7, phi_bb38_8, phi_bb38_9);
    } else {
      ca_.Goto(&block46, phi_bb38_5, phi_bb38_6, phi_bb38_7, phi_bb38_8, phi_bb38_9);
    }
  }

  TNode<FixedArray> phi_bb45_5;
  TNode<IntPtrT> phi_bb45_6;
  TNode<IntPtrT> phi_bb45_7;
  TNode<BoolT> phi_bb45_8;
  TNode<String> phi_bb45_9;
  TNode<JSRegExp> tmp47;
  TNode<RegExpMatchInfo> tmp48;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_5, &phi_bb45_6, &phi_bb45_7, &phi_bb45_8, &phi_bb45_9);
    tmp47 = UnsafeCast_JSRegExp_0(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp});
    compiler::CodeAssemblerLabel label49(&ca_);
    tmp48 = RegExpPrototypeExecBodyWithoutResultFast_0(state_, TNode<Context>{p_context}, TNode<JSRegExp>{tmp47}, TNode<String>{p_string}, &label49);
    ca_.Goto(&block48, phi_bb45_5, phi_bb45_6, phi_bb45_7, phi_bb45_8, phi_bb45_9);
    if (label49.is_used()) {
      ca_.Bind(&label49);
      ca_.Goto(&block49, phi_bb45_5, phi_bb45_6, phi_bb45_7, phi_bb45_8, phi_bb45_9);
    }
  }

  TNode<FixedArray> phi_bb49_5;
  TNode<IntPtrT> phi_bb49_6;
  TNode<IntPtrT> phi_bb49_7;
  TNode<BoolT> phi_bb49_8;
  TNode<String> phi_bb49_9;
  if (block49.is_used()) {
    ca_.Bind(&block49, &phi_bb49_5, &phi_bb49_6, &phi_bb49_7, &phi_bb49_8, &phi_bb49_9);
    ca_.Goto(&block44, phi_bb49_5, phi_bb49_6, phi_bb49_7, phi_bb49_8, phi_bb49_9);
  }

  TNode<FixedArray> phi_bb48_5;
  TNode<IntPtrT> phi_bb48_6;
  TNode<IntPtrT> phi_bb48_7;
  TNode<BoolT> phi_bb48_8;
  TNode<String> phi_bb48_9;
  if (block48.is_used()) {
    ca_.Bind(&block48, &phi_bb48_5, &phi_bb48_6, &phi_bb48_7, &phi_bb48_8, &phi_bb48_9);
    ca_.Branch(phi_bb48_8, &block50, std::vector<compiler::Node*>{phi_bb48_5, phi_bb48_6, phi_bb48_7, phi_bb48_8, phi_bb48_9}, &block51, std::vector<compiler::Node*>{phi_bb48_5, phi_bb48_6, phi_bb48_7, phi_bb48_8, phi_bb48_9});
  }

  TNode<FixedArray> phi_bb50_5;
  TNode<IntPtrT> phi_bb50_6;
  TNode<IntPtrT> phi_bb50_7;
  TNode<BoolT> phi_bb50_8;
  TNode<String> phi_bb50_9;
  if (block50.is_used()) {
    ca_.Bind(&block50, &phi_bb50_5, &phi_bb50_6, &phi_bb50_7, &phi_bb50_8, &phi_bb50_9);
    ca_.Goto(&block52, phi_bb50_5, phi_bb50_6, phi_bb50_7, phi_bb50_8, phi_bb50_9, phi_bb50_9);
  }

  TNode<FixedArray> phi_bb51_5;
  TNode<IntPtrT> phi_bb51_6;
  TNode<IntPtrT> phi_bb51_7;
  TNode<BoolT> phi_bb51_8;
  TNode<String> phi_bb51_9;
  TNode<Object> tmp50;
  TNode<Object> tmp51;
  TNode<Smi> tmp52;
  TNode<Smi> tmp53;
  TNode<String> tmp54;
  if (block51.is_used()) {
    ca_.Bind(&block51, &phi_bb51_5, &phi_bb51_6, &phi_bb51_7, &phi_bb51_8, &phi_bb51_9);
    tmp50 = CodeStubAssembler(state_).UnsafeLoadFixedArrayElement(TNode<RegExpMatchInfo>{tmp48}, RegExpMatchInfo::kFirstCaptureIndex);
    tmp51 = CodeStubAssembler(state_).UnsafeLoadFixedArrayElement(TNode<RegExpMatchInfo>{tmp48}, (CodeStubAssembler(state_).ConstexprInt31Add(RegExpMatchInfo::kFirstCaptureIndex, 1)));
    tmp52 = UnsafeCast_Smi_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp50});
    tmp53 = UnsafeCast_Smi_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp51});
    tmp54 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kSubString), p_context, p_string, tmp52, tmp53);
    ca_.Goto(&block52, phi_bb51_5, phi_bb51_6, phi_bb51_7, phi_bb51_8, phi_bb51_9, tmp54);
  }

  TNode<FixedArray> phi_bb52_5;
  TNode<IntPtrT> phi_bb52_6;
  TNode<IntPtrT> phi_bb52_7;
  TNode<BoolT> phi_bb52_8;
  TNode<String> phi_bb52_9;
  TNode<String> phi_bb52_10;
  if (block52.is_used()) {
    ca_.Bind(&block52, &phi_bb52_5, &phi_bb52_6, &phi_bb52_7, &phi_bb52_8, &phi_bb52_9, &phi_bb52_10);
    ca_.Goto(&block47, phi_bb52_5, phi_bb52_6, phi_bb52_7, phi_bb52_8, phi_bb52_9, phi_bb52_10);
  }

  TNode<FixedArray> phi_bb46_5;
  TNode<IntPtrT> phi_bb46_6;
  TNode<IntPtrT> phi_bb46_7;
  TNode<BoolT> phi_bb46_8;
  TNode<String> phi_bb46_9;
  TNode<Object> tmp55;
  TNode<Oddball> tmp56;
  TNode<BoolT> tmp57;
  if (block46.is_used()) {
    ca_.Bind(&block46, &phi_bb46_5, &phi_bb46_6, &phi_bb46_7, &phi_bb46_8, &phi_bb46_9);
    tmp55 = RegExpExec_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_regexp}, TNode<String>{p_string});
    tmp56 = Null_0(state_);
    tmp57 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp55}, TNode<HeapObject>{tmp56});
    ca_.Branch(tmp57, &block57, std::vector<compiler::Node*>{phi_bb46_5, phi_bb46_6, phi_bb46_7, phi_bb46_8, phi_bb46_9}, &block58, std::vector<compiler::Node*>{phi_bb46_5, phi_bb46_6, phi_bb46_7, phi_bb46_8, phi_bb46_9});
  }

  TNode<FixedArray> phi_bb57_5;
  TNode<IntPtrT> phi_bb57_6;
  TNode<IntPtrT> phi_bb57_7;
  TNode<BoolT> phi_bb57_8;
  TNode<String> phi_bb57_9;
  if (block57.is_used()) {
    ca_.Bind(&block57, &phi_bb57_5, &phi_bb57_6, &phi_bb57_7, &phi_bb57_8, &phi_bb57_9);
    ca_.Goto(&block44, phi_bb57_5, phi_bb57_6, phi_bb57_7, phi_bb57_8, phi_bb57_9);
  }

  TNode<FixedArray> phi_bb58_5;
  TNode<IntPtrT> phi_bb58_6;
  TNode<IntPtrT> phi_bb58_7;
  TNode<BoolT> phi_bb58_8;
  TNode<String> phi_bb58_9;
  TNode<Smi> tmp58;
  TNode<Object> tmp59;
  TNode<String> tmp60;
  if (block58.is_used()) {
    ca_.Bind(&block58, &phi_bb58_5, &phi_bb58_6, &phi_bb58_7, &phi_bb58_8, &phi_bb58_9);
    tmp58 = CodeStubAssembler(state_).SmiConstant(0);
    tmp59 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{tmp55}, TNode<Object>{tmp58});
    tmp60 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{tmp59});
    ca_.Goto(&block47, phi_bb58_5, phi_bb58_6, phi_bb58_7, phi_bb58_8, phi_bb58_9, tmp60);
  }

  TNode<FixedArray> phi_bb47_5;
  TNode<IntPtrT> phi_bb47_6;
  TNode<IntPtrT> phi_bb47_7;
  TNode<BoolT> phi_bb47_8;
  TNode<String> phi_bb47_9;
  TNode<String> phi_bb47_10;
  TNode<BoolT> tmp61;
  if (block47.is_used()) {
    ca_.Bind(&block47, &phi_bb47_5, &phi_bb47_6, &phi_bb47_7, &phi_bb47_8, &phi_bb47_9, &phi_bb47_10);
    tmp61 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb47_6}, TNode<IntPtrT>{phi_bb47_7});
    ca_.Branch(tmp61, &block83, std::vector<compiler::Node*>{phi_bb47_5, phi_bb47_6, phi_bb47_7, phi_bb47_8, phi_bb47_9, phi_bb47_10, phi_bb47_10, phi_bb47_10}, &block84, std::vector<compiler::Node*>{phi_bb47_5, phi_bb47_6, phi_bb47_7, phi_bb47_8, phi_bb47_9, phi_bb47_10, phi_bb47_10, phi_bb47_10});
  }

  TNode<FixedArray> phi_bb44_5;
  TNode<IntPtrT> phi_bb44_6;
  TNode<IntPtrT> phi_bb44_7;
  TNode<BoolT> phi_bb44_8;
  TNode<String> phi_bb44_9;
  TNode<IntPtrT> tmp62;
  TNode<BoolT> tmp63;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_5, &phi_bb44_6, &phi_bb44_7, &phi_bb44_8, &phi_bb44_9);
    tmp62 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp63 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb44_7}, TNode<IntPtrT>{tmp62});
    ca_.Branch(tmp63, &block59, std::vector<compiler::Node*>{phi_bb44_5, phi_bb44_6, phi_bb44_7, phi_bb44_8, phi_bb44_9}, &block60, std::vector<compiler::Node*>{phi_bb44_5, phi_bb44_6, phi_bb44_7, phi_bb44_8, phi_bb44_9});
  }

  TNode<FixedArray> phi_bb59_5;
  TNode<IntPtrT> phi_bb59_6;
  TNode<IntPtrT> phi_bb59_7;
  TNode<BoolT> phi_bb59_8;
  TNode<String> phi_bb59_9;
  TNode<Oddball> tmp64;
  if (block59.is_used()) {
    ca_.Bind(&block59, &phi_bb59_5, &phi_bb59_6, &phi_bb59_7, &phi_bb59_8, &phi_bb59_9);
    tmp64 = Null_0(state_);
    ca_.Goto(&block61, phi_bb59_5, phi_bb59_6, phi_bb59_7, phi_bb59_8, phi_bb59_9, tmp64);
  }

  TNode<FixedArray> phi_bb60_5;
  TNode<IntPtrT> phi_bb60_6;
  TNode<IntPtrT> phi_bb60_7;
  TNode<BoolT> phi_bb60_8;
  TNode<String> phi_bb60_9;
  TNode<NativeContext> tmp65;
  TNode<Map> tmp66;
  TNode<IntPtrT> tmp67;
  TNode<FixedArray> tmp68;
  TNode<Smi> tmp69;
  TNode<JSArray> tmp70;
  if (block60.is_used()) {
    ca_.Bind(&block60, &phi_bb60_5, &phi_bb60_6, &phi_bb60_7, &phi_bb60_8, &phi_bb60_9);
    tmp65 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{p_context});
    tmp66 = CodeStubAssembler(state_).LoadJSArrayElementsMap(ElementsKind::PACKED_ELEMENTS, TNode<NativeContext>{tmp65});
    tmp67 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp68 = ExtractFixedArray_0(state_, TNode<FixedArray>{phi_bb60_5}, TNode<IntPtrT>{tmp67}, TNode<IntPtrT>{phi_bb60_7}, TNode<IntPtrT>{phi_bb60_7});
    tmp69 = Convert_Smi_intptr_0(state_, TNode<IntPtrT>{phi_bb60_7});
    tmp70 = CodeStubAssembler(state_).AllocateJSArray(TNode<Map>{tmp66}, TNode<FixedArrayBase>{tmp68}, TNode<Smi>{tmp69});
    ca_.Goto(&block61, phi_bb60_5, phi_bb60_6, phi_bb60_7, phi_bb60_8, phi_bb60_9, tmp70);
  }

  TNode<FixedArray> phi_bb61_5;
  TNode<IntPtrT> phi_bb61_6;
  TNode<IntPtrT> phi_bb61_7;
  TNode<BoolT> phi_bb61_8;
  TNode<String> phi_bb61_9;
  TNode<HeapObject> phi_bb61_11;
  if (block61.is_used()) {
    ca_.Bind(&block61, &phi_bb61_5, &phi_bb61_6, &phi_bb61_7, &phi_bb61_8, &phi_bb61_9, &phi_bb61_11);
    ca_.Goto(&block1, phi_bb61_11);
  }

  TNode<FixedArray> phi_bb83_5;
  TNode<IntPtrT> phi_bb83_6;
  TNode<IntPtrT> phi_bb83_7;
  TNode<BoolT> phi_bb83_8;
  TNode<String> phi_bb83_9;
  TNode<String> phi_bb83_10;
  TNode<String> phi_bb83_11;
  TNode<Object> phi_bb83_12;
  TNode<IntPtrT> tmp71;
  TNode<IntPtrT> tmp72;
  TNode<IntPtrT> tmp73;
  TNode<IntPtrT> tmp74;
  TNode<IntPtrT> tmp75;
  TNode<IntPtrT> tmp76;
  TNode<FixedArray> tmp77;
  if (block83.is_used()) {
    ca_.Bind(&block83, &phi_bb83_5, &phi_bb83_6, &phi_bb83_7, &phi_bb83_8, &phi_bb83_9, &phi_bb83_10, &phi_bb83_11, &phi_bb83_12);
    tmp71 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp72 = CodeStubAssembler(state_).WordSar(TNode<IntPtrT>{phi_bb83_6}, TNode<IntPtrT>{tmp71});
    tmp73 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb83_6}, TNode<IntPtrT>{tmp72});
    tmp74 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp75 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp73}, TNode<IntPtrT>{tmp74});
    tmp76 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp77 = ExtractFixedArray_0(state_, TNode<FixedArray>{phi_bb83_5}, TNode<IntPtrT>{tmp76}, TNode<IntPtrT>{phi_bb83_7}, TNode<IntPtrT>{tmp75});
    ca_.Goto(&block84, tmp77, tmp75, phi_bb83_7, phi_bb83_8, phi_bb83_9, phi_bb83_10, phi_bb83_11, phi_bb83_12);
  }

  TNode<FixedArray> phi_bb84_5;
  TNode<IntPtrT> phi_bb84_6;
  TNode<IntPtrT> phi_bb84_7;
  TNode<BoolT> phi_bb84_8;
  TNode<String> phi_bb84_9;
  TNode<String> phi_bb84_10;
  TNode<String> phi_bb84_11;
  TNode<Object> phi_bb84_12;
  TNode<Object> tmp78;
  TNode<IntPtrT> tmp79;
  TNode<IntPtrT> tmp80;
  TNode<IntPtrT> tmp81;
  TNode<IntPtrT> tmp82;
  TNode<UintPtrT> tmp83;
  TNode<UintPtrT> tmp84;
  TNode<BoolT> tmp85;
  if (block84.is_used()) {
    ca_.Bind(&block84, &phi_bb84_5, &phi_bb84_6, &phi_bb84_7, &phi_bb84_8, &phi_bb84_9, &phi_bb84_10, &phi_bb84_11, &phi_bb84_12);
    std::tie(tmp78, tmp79, tmp80) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{phi_bb84_5}).Flatten();
    tmp81 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp82 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb84_7}, TNode<IntPtrT>{tmp81});
    tmp83 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb84_7});
    tmp84 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp80});
    tmp85 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp83}, TNode<UintPtrT>{tmp84});
    ca_.Branch(tmp85, &block102, std::vector<compiler::Node*>{phi_bb84_8, phi_bb84_9, phi_bb84_10, phi_bb84_11, phi_bb84_12, phi_bb84_7, phi_bb84_7, phi_bb84_7, phi_bb84_7}, &block103, std::vector<compiler::Node*>{phi_bb84_8, phi_bb84_9, phi_bb84_10, phi_bb84_11, phi_bb84_12, phi_bb84_7, phi_bb84_7, phi_bb84_7, phi_bb84_7});
  }

  TNode<BoolT> phi_bb102_8;
  TNode<String> phi_bb102_9;
  TNode<String> phi_bb102_10;
  TNode<String> phi_bb102_11;
  TNode<Object> phi_bb102_12;
  TNode<IntPtrT> phi_bb102_17;
  TNode<IntPtrT> phi_bb102_18;
  TNode<IntPtrT> phi_bb102_22;
  TNode<IntPtrT> phi_bb102_23;
  TNode<IntPtrT> tmp86;
  TNode<IntPtrT> tmp87;
  TNode<Object> tmp88;
  TNode<IntPtrT> tmp89;
  TNode<Smi> tmp90;
  TNode<Smi> tmp91;
  TNode<BoolT> tmp92;
  if (block102.is_used()) {
    ca_.Bind(&block102, &phi_bb102_8, &phi_bb102_9, &phi_bb102_10, &phi_bb102_11, &phi_bb102_12, &phi_bb102_17, &phi_bb102_18, &phi_bb102_22, &phi_bb102_23);
    tmp86 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb102_23});
    tmp87 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp79}, TNode<IntPtrT>{tmp86});
    std::tie(tmp88, tmp89) = NewReference_Object_0(state_, TNode<Object>{tmp78}, TNode<IntPtrT>{tmp87}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp88, tmp89}, phi_bb102_12);
    tmp90 = CodeStubAssembler(state_).LoadStringLengthAsSmi(TNode<String>{phi_bb102_10});
    tmp91 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp92 = CodeStubAssembler(state_).SmiNotEqual(TNode<Smi>{tmp90}, TNode<Smi>{tmp91});
    ca_.Branch(tmp92, &block105, std::vector<compiler::Node*>{phi_bb102_8, phi_bb102_9, phi_bb102_10}, &block106, std::vector<compiler::Node*>{phi_bb102_8, phi_bb102_9, phi_bb102_10});
  }

  TNode<BoolT> phi_bb103_8;
  TNode<String> phi_bb103_9;
  TNode<String> phi_bb103_10;
  TNode<String> phi_bb103_11;
  TNode<Object> phi_bb103_12;
  TNode<IntPtrT> phi_bb103_17;
  TNode<IntPtrT> phi_bb103_18;
  TNode<IntPtrT> phi_bb103_22;
  TNode<IntPtrT> phi_bb103_23;
  if (block103.is_used()) {
    ca_.Bind(&block103, &phi_bb103_8, &phi_bb103_9, &phi_bb103_10, &phi_bb103_11, &phi_bb103_12, &phi_bb103_17, &phi_bb103_18, &phi_bb103_22, &phi_bb103_23);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BoolT> phi_bb105_8;
  TNode<String> phi_bb105_9;
  TNode<String> phi_bb105_10;
  if (block105.is_used()) {
    ca_.Bind(&block105, &phi_bb105_8, &phi_bb105_9, &phi_bb105_10);
    ca_.Goto(&block40, phi_bb84_5, phi_bb84_6, tmp82, phi_bb105_8, phi_bb105_9);
  }

  TNode<BoolT> phi_bb106_8;
  TNode<String> phi_bb106_9;
  TNode<String> phi_bb106_10;
  TNode<Object> tmp93;
  if (block106.is_used()) {
    ca_.Bind(&block106, &phi_bb106_8, &phi_bb106_9, &phi_bb106_10);
    tmp93 = LoadLastIndex_0(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp}, p_isFastPath);
    if ((p_isFastPath)) {
      ca_.Goto(&block107, phi_bb106_8, phi_bb106_9, phi_bb106_10);
    } else {
      ca_.Goto(&block108, phi_bb106_8, phi_bb106_9, phi_bb106_10);
    }
  }

  TNode<BoolT> phi_bb107_8;
  TNode<String> phi_bb107_9;
  TNode<String> phi_bb107_10;
  if (block107.is_used()) {
    ca_.Bind(&block107, &phi_bb107_8, &phi_bb107_9, &phi_bb107_10);
    ca_.Goto(&block109, phi_bb107_8, phi_bb107_9, phi_bb107_10, tmp93);
  }

  TNode<BoolT> phi_bb108_8;
  TNode<String> phi_bb108_9;
  TNode<String> phi_bb108_10;
  TNode<Number> tmp94;
  if (block108.is_used()) {
    ca_.Bind(&block108, &phi_bb108_8, &phi_bb108_9, &phi_bb108_10);
    tmp94 = CodeStubAssembler(state_).ToLength_Inline(TNode<Context>{p_context}, TNode<Object>{tmp93});
    ca_.Goto(&block109, phi_bb108_8, phi_bb108_9, phi_bb108_10, tmp94);
  }

  TNode<BoolT> phi_bb109_8;
  TNode<String> phi_bb109_9;
  TNode<String> phi_bb109_10;
  TNode<Object> phi_bb109_12;
  TNode<Number> tmp95;
  TNode<Number> tmp96;
  if (block109.is_used()) {
    ca_.Bind(&block109, &phi_bb109_8, &phi_bb109_9, &phi_bb109_10, &phi_bb109_12);
    tmp95 = UnsafeCast_Number_0(state_, TNode<Context>{p_context}, TNode<Object>{phi_bb109_12});
    tmp96 = RegExpBuiltinsAssembler(state_).AdvanceStringIndex(TNode<String>{p_string}, TNode<Number>{tmp95}, TNode<BoolT>{tmp5}, p_isFastPath);
    if ((p_isFastPath)) {
      ca_.Goto(&block114, phi_bb109_8, phi_bb109_9, phi_bb109_10);
    } else {
      ca_.Goto(&block115, phi_bb109_8, phi_bb109_9, phi_bb109_10);
    }
  }

  TNode<BoolT> phi_bb114_8;
  TNode<String> phi_bb114_9;
  TNode<String> phi_bb114_10;
  if (block114.is_used()) {
    ca_.Bind(&block114, &phi_bb114_8, &phi_bb114_9, &phi_bb114_10);
    StaticAssertStringLengthFitsSmi_0(state_);
    ca_.Goto(&block116, phi_bb114_8, phi_bb114_9, phi_bb114_10);
  }

  TNode<BoolT> phi_bb115_8;
  TNode<String> phi_bb115_9;
  TNode<String> phi_bb115_10;
  if (block115.is_used()) {
    ca_.Bind(&block115, &phi_bb115_8, &phi_bb115_9, &phi_bb115_10);
    ca_.Goto(&block116, phi_bb115_8, phi_bb115_9, phi_bb115_10);
  }

  TNode<BoolT> phi_bb116_8;
  TNode<String> phi_bb116_9;
  TNode<String> phi_bb116_10;
  if (block116.is_used()) {
    ca_.Bind(&block116, &phi_bb116_8, &phi_bb116_9, &phi_bb116_10);
    StoreLastIndex_0(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp}, TNode<Number>{tmp96}, p_isFastPath);
    ca_.Goto(&block40, phi_bb84_5, phi_bb84_6, tmp82, phi_bb116_8, phi_bb116_9);
  }

  TNode<FixedArray> phi_bb39_5;
  TNode<IntPtrT> phi_bb39_6;
  TNode<IntPtrT> phi_bb39_7;
  TNode<BoolT> phi_bb39_8;
  TNode<String> phi_bb39_9;
  if (block39.is_used()) {
    ca_.Bind(&block39, &phi_bb39_5, &phi_bb39_6, &phi_bb39_7, &phi_bb39_8, &phi_bb39_9);
    VerifiedUnreachable_0(state_);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block121, phi_bb1_3);
  }

  TNode<Object> phi_bb121_3;
    ca_.Bind(&block121, &phi_bb121_3);
  return TNode<Object>{phi_bb121_3};
}


TNode<Object> FastRegExpPrototypeMatchBody_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSRegExp> p_receiver, TNode<String> p_string) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = RegExpPrototypeMatchBody_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_receiver}, TNode<String>{p_string}, true);
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Object>{tmp0};
}


TNode<Object> SlowRegExpPrototypeMatchBody_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_receiver, TNode<String> p_string) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = RegExpPrototypeMatchBody_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_receiver}, TNode<String>{p_string}, false);
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Object>{tmp0};
}


TF_BUILTIN(RegExpMatchFast, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSRegExp> parameter1 = UncheckedParameter<JSRegExp>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<String> parameter2 = UncheckedParameter<String>(Descriptor::kString);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FastRegExpPrototypeMatchBody_0(state_, TNode<Context>{parameter0}, TNode<JSRegExp>{parameter1}, TNode<String>{parameter2});
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(RegExpPrototypeMatch, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kString);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  TNode<String> tmp1;
  TNode<JSRegExp> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    CodeStubAssembler(state_).ThrowIfNotJSReceiver(TNode<Context>{parameter0}, TNode<Object>{parameter1}, MessageTemplate::kIncompatibleMethodReceiver, "RegExp.prototype.@@match");
    tmp0 = UnsafeCast_JSReceiver_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter2});
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_FastJSRegExp_0(state_, TNode<Context>{parameter0}, TNode<HeapObject>{tmp0}, &label3);
    ca_.Goto(&block3);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block4);
    }
  }

  TNode<Object> tmp4;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp4 = SlowRegExpPrototypeMatchBody_0(state_, TNode<Context>{parameter0}, TNode<JSReceiver>{tmp0}, TNode<String>{tmp1});
    CodeStubAssembler(state_).Return(tmp4);
  }

  TNode<Object> tmp5;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp5 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kRegExpMatchFast), parameter0, tmp2, tmp1);
    CodeStubAssembler(state_).Return(tmp5);
  }
}

TNode<BoolT> Is_FastJSRegExp_JSReceiver_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_o) {
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
