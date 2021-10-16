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
#include "torque-generated/src/builtins/string-endswith-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/regexp-tq-csa.h"
#include "torque-generated/src/builtins/string-endswith-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/string-tq-csa.h"

namespace v8 {
namespace internal {
TNode<BoolT> IsSubstringAt_0(compiler::CodeAssemblerState* state_, TNode<String> p_string, TNode<String> p_searchStr, TNode<IntPtrT> p_start) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TwoStringsToSlices_bool_IsSubstringAtFunctor_0(state_, TNode<String>{p_string}, TNode<String>{p_searchStr}, TorqueStructIsSubstringAtFunctor_0{TNode<IntPtrT>{p_start}});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<BoolT>{tmp0};
}


TF_BUILTIN(StringPrototypeEndsWith, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<String> tmp4;
  TNode<String> tmp5;
  TNode<BoolT> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp3 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp2});
    tmp4 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.endsWith");
    tmp5 = CodeStubAssembler(state_).ToThisString(TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<String>{tmp4});
    tmp6 = IsRegExp_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp1});
    ca_.Branch(tmp6, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kFirstArgumentNotRegExp, "String.prototype.endsWith");
  }

  TNode<String> tmp7;
  TNode<UintPtrT> tmp8;
  TNode<Oddball> tmp9;
  TNode<BoolT> tmp10;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp7 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{tmp1});
    tmp8 = LoadStringLengthAsUintPtr_0(state_, TNode<String>{tmp5});
    tmp9 = Undefined_0(state_);
    tmp10 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp3}, TNode<HeapObject>{tmp9});
    ca_.Branch(tmp10, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  TNode<UintPtrT> tmp11;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp11 = ClampToIndexRange_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp3}, TNode<UintPtrT>{tmp8});
    ca_.Goto(&block5, tmp11);
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block5, tmp8);
  }

  TNode<UintPtrT> phi_bb5_10;
  TNode<UintPtrT> tmp12;
  TNode<UintPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<BoolT> tmp16;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_10);
    tmp12 = LoadStringLengthAsUintPtr_0(state_, TNode<String>{tmp7});
    tmp13 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{phi_bb5_10}, TNode<UintPtrT>{tmp12});
    tmp14 = CodeStubAssembler(state_).Signed(TNode<UintPtrT>{tmp13});
    tmp15 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp16 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{tmp14}, TNode<IntPtrT>{tmp15});
    ca_.Branch(tmp16, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{});
  }

  TNode<Oddball> tmp17;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp17 = False_0(state_);
    arguments.PopAndReturn(tmp17);
  }

  TNode<BoolT> tmp18;
  TNode<Oddball> tmp19;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp18 = IsSubstringAt_0(state_, TNode<String>{tmp5}, TNode<String>{tmp7}, TNode<IntPtrT>{tmp14});
    tmp19 = Convert_Boolean_bool_0(state_, TNode<BoolT>{tmp18});
    arguments.PopAndReturn(tmp19);
  }
}

TNode<BoolT> TwoStringsToSlices_bool_IsSubstringAtFunctor_0(compiler::CodeAssemblerState* state_, TNode<String> p_s1, TNode<String> p_s2, TorqueStructIsSubstringAtFunctor_0 p_f) {
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
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT> block59(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT> block57(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, String> block63(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, String> block62(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block67(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block66(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block71(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block70(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block75(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block74(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block79(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block78(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block83(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block82(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block87(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block86(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block91(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block90(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block95(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block94(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block99(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block98(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block103(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block102(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block80(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block76(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT> block58(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, Object, IntPtrT, IntPtrT> block56(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, Object, IntPtrT, IntPtrT> block54(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT> block110(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT> block108(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, String> block114(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, String> block113(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block118(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block117(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block122(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block121(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block126(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block125(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block130(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block129(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block134(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block133(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block138(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block137(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block142(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block141(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block146(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block145(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block150(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block149(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block154(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String, IntPtrT> block153(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block131(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT, String> block127(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, String, IntPtrT> block109(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, Object, IntPtrT, IntPtrT> block107(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, IntPtrT, IntPtrT, Object, IntPtrT, IntPtrT> block105(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block155(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Int32T> tmp2;
  TNode<IntPtrT> tmp3;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp2 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_s1, tmp1});
    tmp3 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp2});
    ca_.Goto(&block8, p_s1, tmp0);
  }

  TNode<String> phi_bb8_5;
  TNode<IntPtrT> phi_bb8_6;
  TNode<BoolT> tmp4;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_5, &phi_bb8_6);
    tmp4 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp4, &block6, std::vector<compiler::Node*>{phi_bb8_5, phi_bb8_6}, &block7, std::vector<compiler::Node*>{phi_bb8_5, phi_bb8_6});
  }

  TNode<String> phi_bb6_5;
  TNode<IntPtrT> phi_bb6_6;
  TNode<SeqOneByteString> tmp5;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_5, &phi_bb6_6);
    compiler::CodeAssemblerLabel label6(&ca_);
    tmp5 = Cast_SeqOneByteString_1(state_, TNode<String>{phi_bb6_5}, &label6);
    ca_.Goto(&block11, phi_bb6_5, phi_bb6_6, phi_bb6_5, phi_bb6_5);
    if (label6.is_used()) {
      ca_.Bind(&label6);
      ca_.Goto(&block12, phi_bb6_5, phi_bb6_6, phi_bb6_5, phi_bb6_5);
    }
  }

  TNode<String> phi_bb12_5;
  TNode<IntPtrT> phi_bb12_6;
  TNode<String> phi_bb12_8;
  TNode<String> phi_bb12_9;
  TNode<SeqTwoByteString> tmp7;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_5, &phi_bb12_6, &phi_bb12_8, &phi_bb12_9);
    compiler::CodeAssemblerLabel label8(&ca_);
    tmp7 = Cast_SeqTwoByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb12_8)}, &label8);
    ca_.Goto(&block19, phi_bb12_5, phi_bb12_6, phi_bb12_8);
    if (label8.is_used()) {
      ca_.Bind(&label8);
      ca_.Goto(&block20, phi_bb12_5, phi_bb12_6, phi_bb12_8);
    }
  }

  TNode<String> phi_bb11_5;
  TNode<IntPtrT> phi_bb11_6;
  TNode<String> phi_bb11_8;
  TNode<String> phi_bb11_9;
  TNode<Object> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<Object> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_5, &phi_bb11_6, &phi_bb11_8, &phi_bb11_9);
    std::tie(tmp9, tmp10, tmp11) = FieldSliceSeqOneByteStringChars_0(state_, TNode<SeqOneByteString>{tmp5}).Flatten();
    compiler::CodeAssemblerLabel label15(&ca_);
    std::tie(tmp12, tmp13, tmp14) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{tmp9}, TNode<IntPtrT>{tmp10}, TNode<IntPtrT>{tmp11}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb11_6}, TNode<IntPtrT>{tmp3}, &label15).Flatten();
    ca_.Goto(&block15, phi_bb11_5, phi_bb11_6, phi_bb11_8, phi_bb11_6);
    if (label15.is_used()) {
      ca_.Bind(&label15);
      ca_.Goto(&block16, phi_bb11_5, phi_bb11_6, phi_bb11_8, phi_bb11_6);
    }
  }

  TNode<String> phi_bb16_5;
  TNode<IntPtrT> phi_bb16_6;
  TNode<String> phi_bb16_8;
  TNode<IntPtrT> phi_bb16_13;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_5, &phi_bb16_6, &phi_bb16_8, &phi_bb16_13);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb15_5;
  TNode<IntPtrT> phi_bb15_6;
  TNode<String> phi_bb15_8;
  TNode<IntPtrT> phi_bb15_13;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_5, &phi_bb15_6, &phi_bb15_8, &phi_bb15_13);
    ca_.Goto(&block5, tmp12, tmp13, tmp14);
  }

  TNode<String> phi_bb20_5;
  TNode<IntPtrT> phi_bb20_6;
  TNode<String> phi_bb20_8;
  TNode<ThinString> tmp16;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_5, &phi_bb20_6, &phi_bb20_8);
    compiler::CodeAssemblerLabel label17(&ca_);
    tmp16 = Cast_ThinString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb20_8)}, &label17);
    ca_.Goto(&block27, phi_bb20_5, phi_bb20_6, phi_bb20_8);
    if (label17.is_used()) {
      ca_.Bind(&label17);
      ca_.Goto(&block28, phi_bb20_5, phi_bb20_6, phi_bb20_8);
    }
  }

  TNode<String> phi_bb19_5;
  TNode<IntPtrT> phi_bb19_6;
  TNode<String> phi_bb19_8;
  TNode<Object> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<IntPtrT> tmp20;
  TNode<Object> tmp21;
  TNode<IntPtrT> tmp22;
  TNode<IntPtrT> tmp23;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_5, &phi_bb19_6, &phi_bb19_8);
    std::tie(tmp18, tmp19, tmp20) = FieldSliceSeqTwoByteStringChars_0(state_, TNode<SeqTwoByteString>{tmp7}).Flatten();
    compiler::CodeAssemblerLabel label24(&ca_);
    std::tie(tmp21, tmp22, tmp23) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{tmp18}, TNode<IntPtrT>{tmp19}, TNode<IntPtrT>{tmp20}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb19_6}, TNode<IntPtrT>{tmp3}, &label24).Flatten();
    ca_.Goto(&block23, phi_bb19_5, phi_bb19_6, phi_bb19_8, phi_bb19_6);
    if (label24.is_used()) {
      ca_.Bind(&label24);
      ca_.Goto(&block24, phi_bb19_5, phi_bb19_6, phi_bb19_8, phi_bb19_6);
    }
  }

  TNode<String> phi_bb24_5;
  TNode<IntPtrT> phi_bb24_6;
  TNode<String> phi_bb24_8;
  TNode<IntPtrT> phi_bb24_13;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_5, &phi_bb24_6, &phi_bb24_8, &phi_bb24_13);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb23_5;
  TNode<IntPtrT> phi_bb23_6;
  TNode<String> phi_bb23_8;
  TNode<IntPtrT> phi_bb23_13;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_5, &phi_bb23_6, &phi_bb23_8, &phi_bb23_13);
    ca_.Goto(&block3, tmp21, tmp22, tmp23);
  }

  TNode<String> phi_bb28_5;
  TNode<IntPtrT> phi_bb28_6;
  TNode<String> phi_bb28_8;
  TNode<ConsString> tmp25;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_5, &phi_bb28_6, &phi_bb28_8);
    compiler::CodeAssemblerLabel label26(&ca_);
    tmp25 = Cast_ConsString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb28_8)}, &label26);
    ca_.Goto(&block31, phi_bb28_5, phi_bb28_6, phi_bb28_8);
    if (label26.is_used()) {
      ca_.Bind(&label26);
      ca_.Goto(&block32, phi_bb28_5, phi_bb28_6, phi_bb28_8);
    }
  }

  TNode<String> phi_bb27_5;
  TNode<IntPtrT> phi_bb27_6;
  TNode<String> phi_bb27_8;
  TNode<IntPtrT> tmp27;
  TNode<String> tmp28;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_5, &phi_bb27_6, &phi_bb27_8);
    tmp27 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp28 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp16, tmp27});
    ca_.Goto(&block25, tmp28, phi_bb27_6, phi_bb27_8);
  }

  TNode<String> phi_bb32_5;
  TNode<IntPtrT> phi_bb32_6;
  TNode<String> phi_bb32_8;
  TNode<SlicedString> tmp29;
  if (block32.is_used()) {
    ca_.Bind(&block32, &phi_bb32_5, &phi_bb32_6, &phi_bb32_8);
    compiler::CodeAssemblerLabel label30(&ca_);
    tmp29 = Cast_SlicedString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb32_8)}, &label30);
    ca_.Goto(&block35, phi_bb32_5, phi_bb32_6, phi_bb32_8);
    if (label30.is_used()) {
      ca_.Bind(&label30);
      ca_.Goto(&block36, phi_bb32_5, phi_bb32_6, phi_bb32_8);
    }
  }

  TNode<String> phi_bb31_5;
  TNode<IntPtrT> phi_bb31_6;
  TNode<String> phi_bb31_8;
  TNode<String> tmp31;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_5, &phi_bb31_6, &phi_bb31_8);
    tmp31 = Flatten_1(state_, TNode<ConsString>{tmp25});
    ca_.Goto(&block29, tmp31, phi_bb31_6, phi_bb31_8);
  }

  TNode<String> phi_bb36_5;
  TNode<IntPtrT> phi_bb36_6;
  TNode<String> phi_bb36_8;
  TNode<ExternalOneByteString> tmp32;
  if (block36.is_used()) {
    ca_.Bind(&block36, &phi_bb36_5, &phi_bb36_6, &phi_bb36_8);
    compiler::CodeAssemblerLabel label33(&ca_);
    tmp32 = Cast_ExternalOneByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb36_8)}, &label33);
    ca_.Goto(&block39, phi_bb36_5, phi_bb36_6, phi_bb36_8);
    if (label33.is_used()) {
      ca_.Bind(&label33);
      ca_.Goto(&block40, phi_bb36_5, phi_bb36_6, phi_bb36_8);
    }
  }

  TNode<String> phi_bb35_5;
  TNode<IntPtrT> phi_bb35_6;
  TNode<String> phi_bb35_8;
  TNode<IntPtrT> tmp34;
  TNode<Smi> tmp35;
  TNode<IntPtrT> tmp36;
  TNode<IntPtrT> tmp37;
  TNode<IntPtrT> tmp38;
  TNode<String> tmp39;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_5, &phi_bb35_6, &phi_bb35_8);
    tmp34 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp35 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp29, tmp34});
    tmp36 = Convert_intptr_Smi_0(state_, TNode<Smi>{tmp35});
    tmp37 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb35_6}, TNode<IntPtrT>{tmp36});
    tmp38 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp39 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp29, tmp38});
    ca_.Goto(&block29, tmp39, tmp37, phi_bb35_8);
  }

  TNode<String> phi_bb40_5;
  TNode<IntPtrT> phi_bb40_6;
  TNode<String> phi_bb40_8;
  TNode<ExternalTwoByteString> tmp40;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_5, &phi_bb40_6, &phi_bb40_8);
    compiler::CodeAssemblerLabel label41(&ca_);
    tmp40 = Cast_ExternalTwoByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb40_8)}, &label41);
    ca_.Goto(&block47, phi_bb40_5, phi_bb40_6, phi_bb40_8);
    if (label41.is_used()) {
      ca_.Bind(&label41);
      ca_.Goto(&block48, phi_bb40_5, phi_bb40_6, phi_bb40_8);
    }
  }

  TNode<String> phi_bb39_5;
  TNode<IntPtrT> phi_bb39_6;
  TNode<String> phi_bb39_8;
  TNode<RawPtrT> tmp42;
  TNode<IntPtrT> tmp43;
  TNode<Int32T> tmp44;
  TNode<IntPtrT> tmp45;
  TNode<Object> tmp46;
  TNode<IntPtrT> tmp47;
  TNode<IntPtrT> tmp48;
  TNode<Object> tmp49;
  TNode<IntPtrT> tmp50;
  TNode<IntPtrT> tmp51;
  if (block39.is_used()) {
    ca_.Bind(&block39, &phi_bb39_5, &phi_bb39_6, &phi_bb39_8);
    tmp42 = Method_ExternalOneByteString_GetChars_0(state_, TNode<ExternalOneByteString>{tmp32});
    tmp43 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp44 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{tmp32, tmp43});
    tmp45 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp44});
    std::tie(tmp46, tmp47, tmp48) = NewOffHeapConstSlice_char8_0(state_, TNode<RawPtrT>{tmp42}, TNode<IntPtrT>{tmp45}).Flatten();
    compiler::CodeAssemblerLabel label52(&ca_);
    std::tie(tmp49, tmp50, tmp51) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{tmp46}, TNode<IntPtrT>{tmp47}, TNode<IntPtrT>{tmp48}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb39_6}, TNode<IntPtrT>{tmp3}, &label52).Flatten();
    ca_.Goto(&block43, phi_bb39_5, phi_bb39_6, phi_bb39_8, phi_bb39_6);
    if (label52.is_used()) {
      ca_.Bind(&label52);
      ca_.Goto(&block44, phi_bb39_5, phi_bb39_6, phi_bb39_8, phi_bb39_6);
    }
  }

  TNode<String> phi_bb44_5;
  TNode<IntPtrT> phi_bb44_6;
  TNode<String> phi_bb44_8;
  TNode<IntPtrT> phi_bb44_16;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_5, &phi_bb44_6, &phi_bb44_8, &phi_bb44_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb43_5;
  TNode<IntPtrT> phi_bb43_6;
  TNode<String> phi_bb43_8;
  TNode<IntPtrT> phi_bb43_16;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_5, &phi_bb43_6, &phi_bb43_8, &phi_bb43_16);
    ca_.Goto(&block5, tmp49, tmp50, tmp51);
  }

  TNode<String> phi_bb48_5;
  TNode<IntPtrT> phi_bb48_6;
  TNode<String> phi_bb48_8;
  if (block48.is_used()) {
    ca_.Bind(&block48, &phi_bb48_5, &phi_bb48_6, &phi_bb48_8);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb47_5;
  TNode<IntPtrT> phi_bb47_6;
  TNode<String> phi_bb47_8;
  TNode<RawPtrT> tmp53;
  TNode<IntPtrT> tmp54;
  TNode<Int32T> tmp55;
  TNode<IntPtrT> tmp56;
  TNode<Object> tmp57;
  TNode<IntPtrT> tmp58;
  TNode<IntPtrT> tmp59;
  TNode<Object> tmp60;
  TNode<IntPtrT> tmp61;
  TNode<IntPtrT> tmp62;
  if (block47.is_used()) {
    ca_.Bind(&block47, &phi_bb47_5, &phi_bb47_6, &phi_bb47_8);
    tmp53 = Method_ExternalTwoByteString_GetChars_0(state_, TNode<ExternalTwoByteString>{tmp40});
    tmp54 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp55 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{tmp40, tmp54});
    tmp56 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp55});
    std::tie(tmp57, tmp58, tmp59) = NewOffHeapConstSlice_char16_0(state_, TNode<RawPtrT>{tmp53}, TNode<IntPtrT>{tmp56}).Flatten();
    compiler::CodeAssemblerLabel label63(&ca_);
    std::tie(tmp60, tmp61, tmp62) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{tmp57}, TNode<IntPtrT>{tmp58}, TNode<IntPtrT>{tmp59}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb47_6}, TNode<IntPtrT>{tmp3}, &label63).Flatten();
    ca_.Goto(&block51, phi_bb47_5, phi_bb47_6, phi_bb47_8, phi_bb47_6);
    if (label63.is_used()) {
      ca_.Bind(&label63);
      ca_.Goto(&block52, phi_bb47_5, phi_bb47_6, phi_bb47_8, phi_bb47_6);
    }
  }

  TNode<String> phi_bb52_5;
  TNode<IntPtrT> phi_bb52_6;
  TNode<String> phi_bb52_8;
  TNode<IntPtrT> phi_bb52_16;
  if (block52.is_used()) {
    ca_.Bind(&block52, &phi_bb52_5, &phi_bb52_6, &phi_bb52_8, &phi_bb52_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> phi_bb51_5;
  TNode<IntPtrT> phi_bb51_6;
  TNode<String> phi_bb51_8;
  TNode<IntPtrT> phi_bb51_16;
  if (block51.is_used()) {
    ca_.Bind(&block51, &phi_bb51_5, &phi_bb51_6, &phi_bb51_8, &phi_bb51_16);
    ca_.Goto(&block3, tmp60, tmp61, tmp62);
  }

  TNode<String> phi_bb29_5;
  TNode<IntPtrT> phi_bb29_6;
  TNode<String> phi_bb29_8;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_5, &phi_bb29_6, &phi_bb29_8);
    ca_.Goto(&block25, phi_bb29_5, phi_bb29_6, phi_bb29_8);
  }

  TNode<String> phi_bb25_5;
  TNode<IntPtrT> phi_bb25_6;
  TNode<String> phi_bb25_8;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_5, &phi_bb25_6, &phi_bb25_8);
    ca_.Goto(&block8, phi_bb25_5, phi_bb25_6);
  }

  TNode<String> phi_bb7_5;
  TNode<IntPtrT> phi_bb7_6;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_5, &phi_bb7_6);
    VerifiedUnreachable_0(state_);
  }

  TNode<Object> phi_bb5_3;
  TNode<IntPtrT> phi_bb5_4;
  TNode<IntPtrT> phi_bb5_5;
  TNode<IntPtrT> tmp64;
  TNode<IntPtrT> tmp65;
  TNode<Int32T> tmp66;
  TNode<IntPtrT> tmp67;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_3, &phi_bb5_4, &phi_bb5_5);
    tmp64 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp65 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp66 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_s2, tmp65});
    tmp67 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp66});
    ca_.Goto(&block59, phi_bb5_3, phi_bb5_4, phi_bb5_5, p_s2, tmp64);
  }

  TNode<Object> phi_bb59_3;
  TNode<IntPtrT> phi_bb59_4;
  TNode<IntPtrT> phi_bb59_5;
  TNode<String> phi_bb59_8;
  TNode<IntPtrT> phi_bb59_9;
  TNode<BoolT> tmp68;
  if (block59.is_used()) {
    ca_.Bind(&block59, &phi_bb59_3, &phi_bb59_4, &phi_bb59_5, &phi_bb59_8, &phi_bb59_9);
    tmp68 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp68, &block57, std::vector<compiler::Node*>{phi_bb59_3, phi_bb59_4, phi_bb59_5, phi_bb59_8, phi_bb59_9}, &block58, std::vector<compiler::Node*>{phi_bb59_3, phi_bb59_4, phi_bb59_5, phi_bb59_8, phi_bb59_9});
  }

  TNode<Object> phi_bb57_3;
  TNode<IntPtrT> phi_bb57_4;
  TNode<IntPtrT> phi_bb57_5;
  TNode<String> phi_bb57_8;
  TNode<IntPtrT> phi_bb57_9;
  TNode<SeqOneByteString> tmp69;
  if (block57.is_used()) {
    ca_.Bind(&block57, &phi_bb57_3, &phi_bb57_4, &phi_bb57_5, &phi_bb57_8, &phi_bb57_9);
    compiler::CodeAssemblerLabel label70(&ca_);
    tmp69 = Cast_SeqOneByteString_1(state_, TNode<String>{phi_bb57_8}, &label70);
    ca_.Goto(&block62, phi_bb57_3, phi_bb57_4, phi_bb57_5, phi_bb57_8, phi_bb57_9, phi_bb57_8, phi_bb57_8);
    if (label70.is_used()) {
      ca_.Bind(&label70);
      ca_.Goto(&block63, phi_bb57_3, phi_bb57_4, phi_bb57_5, phi_bb57_8, phi_bb57_9, phi_bb57_8, phi_bb57_8);
    }
  }

  TNode<Object> phi_bb63_3;
  TNode<IntPtrT> phi_bb63_4;
  TNode<IntPtrT> phi_bb63_5;
  TNode<String> phi_bb63_8;
  TNode<IntPtrT> phi_bb63_9;
  TNode<String> phi_bb63_11;
  TNode<String> phi_bb63_12;
  TNode<SeqTwoByteString> tmp71;
  if (block63.is_used()) {
    ca_.Bind(&block63, &phi_bb63_3, &phi_bb63_4, &phi_bb63_5, &phi_bb63_8, &phi_bb63_9, &phi_bb63_11, &phi_bb63_12);
    compiler::CodeAssemblerLabel label72(&ca_);
    tmp71 = Cast_SeqTwoByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb63_11)}, &label72);
    ca_.Goto(&block70, phi_bb63_3, phi_bb63_4, phi_bb63_5, phi_bb63_8, phi_bb63_9, phi_bb63_11);
    if (label72.is_used()) {
      ca_.Bind(&label72);
      ca_.Goto(&block71, phi_bb63_3, phi_bb63_4, phi_bb63_5, phi_bb63_8, phi_bb63_9, phi_bb63_11);
    }
  }

  TNode<Object> phi_bb62_3;
  TNode<IntPtrT> phi_bb62_4;
  TNode<IntPtrT> phi_bb62_5;
  TNode<String> phi_bb62_8;
  TNode<IntPtrT> phi_bb62_9;
  TNode<String> phi_bb62_11;
  TNode<String> phi_bb62_12;
  TNode<Object> tmp73;
  TNode<IntPtrT> tmp74;
  TNode<IntPtrT> tmp75;
  TNode<Object> tmp76;
  TNode<IntPtrT> tmp77;
  TNode<IntPtrT> tmp78;
  if (block62.is_used()) {
    ca_.Bind(&block62, &phi_bb62_3, &phi_bb62_4, &phi_bb62_5, &phi_bb62_8, &phi_bb62_9, &phi_bb62_11, &phi_bb62_12);
    std::tie(tmp73, tmp74, tmp75) = FieldSliceSeqOneByteStringChars_0(state_, TNode<SeqOneByteString>{tmp69}).Flatten();
    compiler::CodeAssemblerLabel label79(&ca_);
    std::tie(tmp76, tmp77, tmp78) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{tmp73}, TNode<IntPtrT>{tmp74}, TNode<IntPtrT>{tmp75}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb62_9}, TNode<IntPtrT>{tmp67}, &label79).Flatten();
    ca_.Goto(&block66, phi_bb62_3, phi_bb62_4, phi_bb62_5, phi_bb62_8, phi_bb62_9, phi_bb62_11, phi_bb62_9);
    if (label79.is_used()) {
      ca_.Bind(&label79);
      ca_.Goto(&block67, phi_bb62_3, phi_bb62_4, phi_bb62_5, phi_bb62_8, phi_bb62_9, phi_bb62_11, phi_bb62_9);
    }
  }

  TNode<Object> phi_bb67_3;
  TNode<IntPtrT> phi_bb67_4;
  TNode<IntPtrT> phi_bb67_5;
  TNode<String> phi_bb67_8;
  TNode<IntPtrT> phi_bb67_9;
  TNode<String> phi_bb67_11;
  TNode<IntPtrT> phi_bb67_16;
  if (block67.is_used()) {
    ca_.Bind(&block67, &phi_bb67_3, &phi_bb67_4, &phi_bb67_5, &phi_bb67_8, &phi_bb67_9, &phi_bb67_11, &phi_bb67_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb66_3;
  TNode<IntPtrT> phi_bb66_4;
  TNode<IntPtrT> phi_bb66_5;
  TNode<String> phi_bb66_8;
  TNode<IntPtrT> phi_bb66_9;
  TNode<String> phi_bb66_11;
  TNode<IntPtrT> phi_bb66_16;
  if (block66.is_used()) {
    ca_.Bind(&block66, &phi_bb66_3, &phi_bb66_4, &phi_bb66_5, &phi_bb66_8, &phi_bb66_9, &phi_bb66_11, &phi_bb66_16);
    ca_.Goto(&block56, phi_bb66_3, phi_bb66_4, phi_bb66_5, tmp76, tmp77, tmp78);
  }

  TNode<Object> phi_bb71_3;
  TNode<IntPtrT> phi_bb71_4;
  TNode<IntPtrT> phi_bb71_5;
  TNode<String> phi_bb71_8;
  TNode<IntPtrT> phi_bb71_9;
  TNode<String> phi_bb71_11;
  TNode<ThinString> tmp80;
  if (block71.is_used()) {
    ca_.Bind(&block71, &phi_bb71_3, &phi_bb71_4, &phi_bb71_5, &phi_bb71_8, &phi_bb71_9, &phi_bb71_11);
    compiler::CodeAssemblerLabel label81(&ca_);
    tmp80 = Cast_ThinString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb71_11)}, &label81);
    ca_.Goto(&block78, phi_bb71_3, phi_bb71_4, phi_bb71_5, phi_bb71_8, phi_bb71_9, phi_bb71_11);
    if (label81.is_used()) {
      ca_.Bind(&label81);
      ca_.Goto(&block79, phi_bb71_3, phi_bb71_4, phi_bb71_5, phi_bb71_8, phi_bb71_9, phi_bb71_11);
    }
  }

  TNode<Object> phi_bb70_3;
  TNode<IntPtrT> phi_bb70_4;
  TNode<IntPtrT> phi_bb70_5;
  TNode<String> phi_bb70_8;
  TNode<IntPtrT> phi_bb70_9;
  TNode<String> phi_bb70_11;
  TNode<Object> tmp82;
  TNode<IntPtrT> tmp83;
  TNode<IntPtrT> tmp84;
  TNode<Object> tmp85;
  TNode<IntPtrT> tmp86;
  TNode<IntPtrT> tmp87;
  if (block70.is_used()) {
    ca_.Bind(&block70, &phi_bb70_3, &phi_bb70_4, &phi_bb70_5, &phi_bb70_8, &phi_bb70_9, &phi_bb70_11);
    std::tie(tmp82, tmp83, tmp84) = FieldSliceSeqTwoByteStringChars_0(state_, TNode<SeqTwoByteString>{tmp71}).Flatten();
    compiler::CodeAssemblerLabel label88(&ca_);
    std::tie(tmp85, tmp86, tmp87) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{tmp82}, TNode<IntPtrT>{tmp83}, TNode<IntPtrT>{tmp84}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb70_9}, TNode<IntPtrT>{tmp67}, &label88).Flatten();
    ca_.Goto(&block74, phi_bb70_3, phi_bb70_4, phi_bb70_5, phi_bb70_8, phi_bb70_9, phi_bb70_11, phi_bb70_9);
    if (label88.is_used()) {
      ca_.Bind(&label88);
      ca_.Goto(&block75, phi_bb70_3, phi_bb70_4, phi_bb70_5, phi_bb70_8, phi_bb70_9, phi_bb70_11, phi_bb70_9);
    }
  }

  TNode<Object> phi_bb75_3;
  TNode<IntPtrT> phi_bb75_4;
  TNode<IntPtrT> phi_bb75_5;
  TNode<String> phi_bb75_8;
  TNode<IntPtrT> phi_bb75_9;
  TNode<String> phi_bb75_11;
  TNode<IntPtrT> phi_bb75_16;
  if (block75.is_used()) {
    ca_.Bind(&block75, &phi_bb75_3, &phi_bb75_4, &phi_bb75_5, &phi_bb75_8, &phi_bb75_9, &phi_bb75_11, &phi_bb75_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb74_3;
  TNode<IntPtrT> phi_bb74_4;
  TNode<IntPtrT> phi_bb74_5;
  TNode<String> phi_bb74_8;
  TNode<IntPtrT> phi_bb74_9;
  TNode<String> phi_bb74_11;
  TNode<IntPtrT> phi_bb74_16;
  if (block74.is_used()) {
    ca_.Bind(&block74, &phi_bb74_3, &phi_bb74_4, &phi_bb74_5, &phi_bb74_8, &phi_bb74_9, &phi_bb74_11, &phi_bb74_16);
    ca_.Goto(&block54, phi_bb74_3, phi_bb74_4, phi_bb74_5, tmp85, tmp86, tmp87);
  }

  TNode<Object> phi_bb79_3;
  TNode<IntPtrT> phi_bb79_4;
  TNode<IntPtrT> phi_bb79_5;
  TNode<String> phi_bb79_8;
  TNode<IntPtrT> phi_bb79_9;
  TNode<String> phi_bb79_11;
  TNode<ConsString> tmp89;
  if (block79.is_used()) {
    ca_.Bind(&block79, &phi_bb79_3, &phi_bb79_4, &phi_bb79_5, &phi_bb79_8, &phi_bb79_9, &phi_bb79_11);
    compiler::CodeAssemblerLabel label90(&ca_);
    tmp89 = Cast_ConsString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb79_11)}, &label90);
    ca_.Goto(&block82, phi_bb79_3, phi_bb79_4, phi_bb79_5, phi_bb79_8, phi_bb79_9, phi_bb79_11);
    if (label90.is_used()) {
      ca_.Bind(&label90);
      ca_.Goto(&block83, phi_bb79_3, phi_bb79_4, phi_bb79_5, phi_bb79_8, phi_bb79_9, phi_bb79_11);
    }
  }

  TNode<Object> phi_bb78_3;
  TNode<IntPtrT> phi_bb78_4;
  TNode<IntPtrT> phi_bb78_5;
  TNode<String> phi_bb78_8;
  TNode<IntPtrT> phi_bb78_9;
  TNode<String> phi_bb78_11;
  TNode<IntPtrT> tmp91;
  TNode<String> tmp92;
  if (block78.is_used()) {
    ca_.Bind(&block78, &phi_bb78_3, &phi_bb78_4, &phi_bb78_5, &phi_bb78_8, &phi_bb78_9, &phi_bb78_11);
    tmp91 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp92 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp80, tmp91});
    ca_.Goto(&block76, phi_bb78_3, phi_bb78_4, phi_bb78_5, tmp92, phi_bb78_9, phi_bb78_11);
  }

  TNode<Object> phi_bb83_3;
  TNode<IntPtrT> phi_bb83_4;
  TNode<IntPtrT> phi_bb83_5;
  TNode<String> phi_bb83_8;
  TNode<IntPtrT> phi_bb83_9;
  TNode<String> phi_bb83_11;
  TNode<SlicedString> tmp93;
  if (block83.is_used()) {
    ca_.Bind(&block83, &phi_bb83_3, &phi_bb83_4, &phi_bb83_5, &phi_bb83_8, &phi_bb83_9, &phi_bb83_11);
    compiler::CodeAssemblerLabel label94(&ca_);
    tmp93 = Cast_SlicedString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb83_11)}, &label94);
    ca_.Goto(&block86, phi_bb83_3, phi_bb83_4, phi_bb83_5, phi_bb83_8, phi_bb83_9, phi_bb83_11);
    if (label94.is_used()) {
      ca_.Bind(&label94);
      ca_.Goto(&block87, phi_bb83_3, phi_bb83_4, phi_bb83_5, phi_bb83_8, phi_bb83_9, phi_bb83_11);
    }
  }

  TNode<Object> phi_bb82_3;
  TNode<IntPtrT> phi_bb82_4;
  TNode<IntPtrT> phi_bb82_5;
  TNode<String> phi_bb82_8;
  TNode<IntPtrT> phi_bb82_9;
  TNode<String> phi_bb82_11;
  TNode<String> tmp95;
  if (block82.is_used()) {
    ca_.Bind(&block82, &phi_bb82_3, &phi_bb82_4, &phi_bb82_5, &phi_bb82_8, &phi_bb82_9, &phi_bb82_11);
    tmp95 = Flatten_1(state_, TNode<ConsString>{tmp89});
    ca_.Goto(&block80, phi_bb82_3, phi_bb82_4, phi_bb82_5, tmp95, phi_bb82_9, phi_bb82_11);
  }

  TNode<Object> phi_bb87_3;
  TNode<IntPtrT> phi_bb87_4;
  TNode<IntPtrT> phi_bb87_5;
  TNode<String> phi_bb87_8;
  TNode<IntPtrT> phi_bb87_9;
  TNode<String> phi_bb87_11;
  TNode<ExternalOneByteString> tmp96;
  if (block87.is_used()) {
    ca_.Bind(&block87, &phi_bb87_3, &phi_bb87_4, &phi_bb87_5, &phi_bb87_8, &phi_bb87_9, &phi_bb87_11);
    compiler::CodeAssemblerLabel label97(&ca_);
    tmp96 = Cast_ExternalOneByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb87_11)}, &label97);
    ca_.Goto(&block90, phi_bb87_3, phi_bb87_4, phi_bb87_5, phi_bb87_8, phi_bb87_9, phi_bb87_11);
    if (label97.is_used()) {
      ca_.Bind(&label97);
      ca_.Goto(&block91, phi_bb87_3, phi_bb87_4, phi_bb87_5, phi_bb87_8, phi_bb87_9, phi_bb87_11);
    }
  }

  TNode<Object> phi_bb86_3;
  TNode<IntPtrT> phi_bb86_4;
  TNode<IntPtrT> phi_bb86_5;
  TNode<String> phi_bb86_8;
  TNode<IntPtrT> phi_bb86_9;
  TNode<String> phi_bb86_11;
  TNode<IntPtrT> tmp98;
  TNode<Smi> tmp99;
  TNode<IntPtrT> tmp100;
  TNode<IntPtrT> tmp101;
  TNode<IntPtrT> tmp102;
  TNode<String> tmp103;
  if (block86.is_used()) {
    ca_.Bind(&block86, &phi_bb86_3, &phi_bb86_4, &phi_bb86_5, &phi_bb86_8, &phi_bb86_9, &phi_bb86_11);
    tmp98 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp99 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp93, tmp98});
    tmp100 = Convert_intptr_Smi_0(state_, TNode<Smi>{tmp99});
    tmp101 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb86_9}, TNode<IntPtrT>{tmp100});
    tmp102 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp103 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp93, tmp102});
    ca_.Goto(&block80, phi_bb86_3, phi_bb86_4, phi_bb86_5, tmp103, tmp101, phi_bb86_11);
  }

  TNode<Object> phi_bb91_3;
  TNode<IntPtrT> phi_bb91_4;
  TNode<IntPtrT> phi_bb91_5;
  TNode<String> phi_bb91_8;
  TNode<IntPtrT> phi_bb91_9;
  TNode<String> phi_bb91_11;
  TNode<ExternalTwoByteString> tmp104;
  if (block91.is_used()) {
    ca_.Bind(&block91, &phi_bb91_3, &phi_bb91_4, &phi_bb91_5, &phi_bb91_8, &phi_bb91_9, &phi_bb91_11);
    compiler::CodeAssemblerLabel label105(&ca_);
    tmp104 = Cast_ExternalTwoByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb91_11)}, &label105);
    ca_.Goto(&block98, phi_bb91_3, phi_bb91_4, phi_bb91_5, phi_bb91_8, phi_bb91_9, phi_bb91_11);
    if (label105.is_used()) {
      ca_.Bind(&label105);
      ca_.Goto(&block99, phi_bb91_3, phi_bb91_4, phi_bb91_5, phi_bb91_8, phi_bb91_9, phi_bb91_11);
    }
  }

  TNode<Object> phi_bb90_3;
  TNode<IntPtrT> phi_bb90_4;
  TNode<IntPtrT> phi_bb90_5;
  TNode<String> phi_bb90_8;
  TNode<IntPtrT> phi_bb90_9;
  TNode<String> phi_bb90_11;
  TNode<RawPtrT> tmp106;
  TNode<IntPtrT> tmp107;
  TNode<Int32T> tmp108;
  TNode<IntPtrT> tmp109;
  TNode<Object> tmp110;
  TNode<IntPtrT> tmp111;
  TNode<IntPtrT> tmp112;
  TNode<Object> tmp113;
  TNode<IntPtrT> tmp114;
  TNode<IntPtrT> tmp115;
  if (block90.is_used()) {
    ca_.Bind(&block90, &phi_bb90_3, &phi_bb90_4, &phi_bb90_5, &phi_bb90_8, &phi_bb90_9, &phi_bb90_11);
    tmp106 = Method_ExternalOneByteString_GetChars_0(state_, TNode<ExternalOneByteString>{tmp96});
    tmp107 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp108 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{tmp96, tmp107});
    tmp109 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp108});
    std::tie(tmp110, tmp111, tmp112) = NewOffHeapConstSlice_char8_0(state_, TNode<RawPtrT>{tmp106}, TNode<IntPtrT>{tmp109}).Flatten();
    compiler::CodeAssemblerLabel label116(&ca_);
    std::tie(tmp113, tmp114, tmp115) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{tmp110}, TNode<IntPtrT>{tmp111}, TNode<IntPtrT>{tmp112}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb90_9}, TNode<IntPtrT>{tmp67}, &label116).Flatten();
    ca_.Goto(&block94, phi_bb90_3, phi_bb90_4, phi_bb90_5, phi_bb90_8, phi_bb90_9, phi_bb90_11, phi_bb90_9);
    if (label116.is_used()) {
      ca_.Bind(&label116);
      ca_.Goto(&block95, phi_bb90_3, phi_bb90_4, phi_bb90_5, phi_bb90_8, phi_bb90_9, phi_bb90_11, phi_bb90_9);
    }
  }

  TNode<Object> phi_bb95_3;
  TNode<IntPtrT> phi_bb95_4;
  TNode<IntPtrT> phi_bb95_5;
  TNode<String> phi_bb95_8;
  TNode<IntPtrT> phi_bb95_9;
  TNode<String> phi_bb95_11;
  TNode<IntPtrT> phi_bb95_19;
  if (block95.is_used()) {
    ca_.Bind(&block95, &phi_bb95_3, &phi_bb95_4, &phi_bb95_5, &phi_bb95_8, &phi_bb95_9, &phi_bb95_11, &phi_bb95_19);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb94_3;
  TNode<IntPtrT> phi_bb94_4;
  TNode<IntPtrT> phi_bb94_5;
  TNode<String> phi_bb94_8;
  TNode<IntPtrT> phi_bb94_9;
  TNode<String> phi_bb94_11;
  TNode<IntPtrT> phi_bb94_19;
  if (block94.is_used()) {
    ca_.Bind(&block94, &phi_bb94_3, &phi_bb94_4, &phi_bb94_5, &phi_bb94_8, &phi_bb94_9, &phi_bb94_11, &phi_bb94_19);
    ca_.Goto(&block56, phi_bb94_3, phi_bb94_4, phi_bb94_5, tmp113, tmp114, tmp115);
  }

  TNode<Object> phi_bb99_3;
  TNode<IntPtrT> phi_bb99_4;
  TNode<IntPtrT> phi_bb99_5;
  TNode<String> phi_bb99_8;
  TNode<IntPtrT> phi_bb99_9;
  TNode<String> phi_bb99_11;
  if (block99.is_used()) {
    ca_.Bind(&block99, &phi_bb99_3, &phi_bb99_4, &phi_bb99_5, &phi_bb99_8, &phi_bb99_9, &phi_bb99_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb98_3;
  TNode<IntPtrT> phi_bb98_4;
  TNode<IntPtrT> phi_bb98_5;
  TNode<String> phi_bb98_8;
  TNode<IntPtrT> phi_bb98_9;
  TNode<String> phi_bb98_11;
  TNode<RawPtrT> tmp117;
  TNode<IntPtrT> tmp118;
  TNode<Int32T> tmp119;
  TNode<IntPtrT> tmp120;
  TNode<Object> tmp121;
  TNode<IntPtrT> tmp122;
  TNode<IntPtrT> tmp123;
  TNode<Object> tmp124;
  TNode<IntPtrT> tmp125;
  TNode<IntPtrT> tmp126;
  if (block98.is_used()) {
    ca_.Bind(&block98, &phi_bb98_3, &phi_bb98_4, &phi_bb98_5, &phi_bb98_8, &phi_bb98_9, &phi_bb98_11);
    tmp117 = Method_ExternalTwoByteString_GetChars_0(state_, TNode<ExternalTwoByteString>{tmp104});
    tmp118 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp119 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{tmp104, tmp118});
    tmp120 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp119});
    std::tie(tmp121, tmp122, tmp123) = NewOffHeapConstSlice_char16_0(state_, TNode<RawPtrT>{tmp117}, TNode<IntPtrT>{tmp120}).Flatten();
    compiler::CodeAssemblerLabel label127(&ca_);
    std::tie(tmp124, tmp125, tmp126) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{tmp121}, TNode<IntPtrT>{tmp122}, TNode<IntPtrT>{tmp123}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb98_9}, TNode<IntPtrT>{tmp67}, &label127).Flatten();
    ca_.Goto(&block102, phi_bb98_3, phi_bb98_4, phi_bb98_5, phi_bb98_8, phi_bb98_9, phi_bb98_11, phi_bb98_9);
    if (label127.is_used()) {
      ca_.Bind(&label127);
      ca_.Goto(&block103, phi_bb98_3, phi_bb98_4, phi_bb98_5, phi_bb98_8, phi_bb98_9, phi_bb98_11, phi_bb98_9);
    }
  }

  TNode<Object> phi_bb103_3;
  TNode<IntPtrT> phi_bb103_4;
  TNode<IntPtrT> phi_bb103_5;
  TNode<String> phi_bb103_8;
  TNode<IntPtrT> phi_bb103_9;
  TNode<String> phi_bb103_11;
  TNode<IntPtrT> phi_bb103_19;
  if (block103.is_used()) {
    ca_.Bind(&block103, &phi_bb103_3, &phi_bb103_4, &phi_bb103_5, &phi_bb103_8, &phi_bb103_9, &phi_bb103_11, &phi_bb103_19);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb102_3;
  TNode<IntPtrT> phi_bb102_4;
  TNode<IntPtrT> phi_bb102_5;
  TNode<String> phi_bb102_8;
  TNode<IntPtrT> phi_bb102_9;
  TNode<String> phi_bb102_11;
  TNode<IntPtrT> phi_bb102_19;
  if (block102.is_used()) {
    ca_.Bind(&block102, &phi_bb102_3, &phi_bb102_4, &phi_bb102_5, &phi_bb102_8, &phi_bb102_9, &phi_bb102_11, &phi_bb102_19);
    ca_.Goto(&block54, phi_bb102_3, phi_bb102_4, phi_bb102_5, tmp124, tmp125, tmp126);
  }

  TNode<Object> phi_bb80_3;
  TNode<IntPtrT> phi_bb80_4;
  TNode<IntPtrT> phi_bb80_5;
  TNode<String> phi_bb80_8;
  TNode<IntPtrT> phi_bb80_9;
  TNode<String> phi_bb80_11;
  if (block80.is_used()) {
    ca_.Bind(&block80, &phi_bb80_3, &phi_bb80_4, &phi_bb80_5, &phi_bb80_8, &phi_bb80_9, &phi_bb80_11);
    ca_.Goto(&block76, phi_bb80_3, phi_bb80_4, phi_bb80_5, phi_bb80_8, phi_bb80_9, phi_bb80_11);
  }

  TNode<Object> phi_bb76_3;
  TNode<IntPtrT> phi_bb76_4;
  TNode<IntPtrT> phi_bb76_5;
  TNode<String> phi_bb76_8;
  TNode<IntPtrT> phi_bb76_9;
  TNode<String> phi_bb76_11;
  if (block76.is_used()) {
    ca_.Bind(&block76, &phi_bb76_3, &phi_bb76_4, &phi_bb76_5, &phi_bb76_8, &phi_bb76_9, &phi_bb76_11);
    ca_.Goto(&block59, phi_bb76_3, phi_bb76_4, phi_bb76_5, phi_bb76_8, phi_bb76_9);
  }

  TNode<Object> phi_bb58_3;
  TNode<IntPtrT> phi_bb58_4;
  TNode<IntPtrT> phi_bb58_5;
  TNode<String> phi_bb58_8;
  TNode<IntPtrT> phi_bb58_9;
  if (block58.is_used()) {
    ca_.Bind(&block58, &phi_bb58_3, &phi_bb58_4, &phi_bb58_5, &phi_bb58_8, &phi_bb58_9);
    VerifiedUnreachable_0(state_);
  }

  TNode<Object> phi_bb56_3;
  TNode<IntPtrT> phi_bb56_4;
  TNode<IntPtrT> phi_bb56_5;
  TNode<Object> phi_bb56_6;
  TNode<IntPtrT> phi_bb56_7;
  TNode<IntPtrT> phi_bb56_8;
  TNode<BoolT> tmp128;
  if (block56.is_used()) {
    ca_.Bind(&block56, &phi_bb56_3, &phi_bb56_4, &phi_bb56_5, &phi_bb56_6, &phi_bb56_7, &phi_bb56_8);
    tmp128 = Call_char8_char8_0(state_, TorqueStructIsSubstringAtFunctor_0{TNode<IntPtrT>{p_f.start}}, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{phi_bb56_3}, TNode<IntPtrT>{phi_bb56_4}, TNode<IntPtrT>{phi_bb56_5}, TorqueStructUnsafe_0{}}, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{phi_bb56_6}, TNode<IntPtrT>{phi_bb56_7}, TNode<IntPtrT>{phi_bb56_8}, TorqueStructUnsafe_0{}});
    ca_.Goto(&block1, tmp128);
  }

  TNode<Object> phi_bb54_3;
  TNode<IntPtrT> phi_bb54_4;
  TNode<IntPtrT> phi_bb54_5;
  TNode<Object> phi_bb54_6;
  TNode<IntPtrT> phi_bb54_7;
  TNode<IntPtrT> phi_bb54_8;
  TNode<BoolT> tmp129;
  if (block54.is_used()) {
    ca_.Bind(&block54, &phi_bb54_3, &phi_bb54_4, &phi_bb54_5, &phi_bb54_6, &phi_bb54_7, &phi_bb54_8);
    tmp129 = Call_char8_char16_0(state_, TorqueStructIsSubstringAtFunctor_0{TNode<IntPtrT>{p_f.start}}, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{phi_bb54_3}, TNode<IntPtrT>{phi_bb54_4}, TNode<IntPtrT>{phi_bb54_5}, TorqueStructUnsafe_0{}}, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{phi_bb54_6}, TNode<IntPtrT>{phi_bb54_7}, TNode<IntPtrT>{phi_bb54_8}, TorqueStructUnsafe_0{}});
    ca_.Goto(&block1, tmp129);
  }

  TNode<Object> phi_bb3_3;
  TNode<IntPtrT> phi_bb3_4;
  TNode<IntPtrT> phi_bb3_5;
  TNode<IntPtrT> tmp130;
  TNode<IntPtrT> tmp131;
  TNode<Int32T> tmp132;
  TNode<IntPtrT> tmp133;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_3, &phi_bb3_4, &phi_bb3_5);
    tmp130 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp131 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp132 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_s2, tmp131});
    tmp133 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp132});
    ca_.Goto(&block110, phi_bb3_3, phi_bb3_4, phi_bb3_5, p_s2, tmp130);
  }

  TNode<Object> phi_bb110_3;
  TNode<IntPtrT> phi_bb110_4;
  TNode<IntPtrT> phi_bb110_5;
  TNode<String> phi_bb110_8;
  TNode<IntPtrT> phi_bb110_9;
  TNode<BoolT> tmp134;
  if (block110.is_used()) {
    ca_.Bind(&block110, &phi_bb110_3, &phi_bb110_4, &phi_bb110_5, &phi_bb110_8, &phi_bb110_9);
    tmp134 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp134, &block108, std::vector<compiler::Node*>{phi_bb110_3, phi_bb110_4, phi_bb110_5, phi_bb110_8, phi_bb110_9}, &block109, std::vector<compiler::Node*>{phi_bb110_3, phi_bb110_4, phi_bb110_5, phi_bb110_8, phi_bb110_9});
  }

  TNode<Object> phi_bb108_3;
  TNode<IntPtrT> phi_bb108_4;
  TNode<IntPtrT> phi_bb108_5;
  TNode<String> phi_bb108_8;
  TNode<IntPtrT> phi_bb108_9;
  TNode<SeqOneByteString> tmp135;
  if (block108.is_used()) {
    ca_.Bind(&block108, &phi_bb108_3, &phi_bb108_4, &phi_bb108_5, &phi_bb108_8, &phi_bb108_9);
    compiler::CodeAssemblerLabel label136(&ca_);
    tmp135 = Cast_SeqOneByteString_1(state_, TNode<String>{phi_bb108_8}, &label136);
    ca_.Goto(&block113, phi_bb108_3, phi_bb108_4, phi_bb108_5, phi_bb108_8, phi_bb108_9, phi_bb108_8, phi_bb108_8);
    if (label136.is_used()) {
      ca_.Bind(&label136);
      ca_.Goto(&block114, phi_bb108_3, phi_bb108_4, phi_bb108_5, phi_bb108_8, phi_bb108_9, phi_bb108_8, phi_bb108_8);
    }
  }

  TNode<Object> phi_bb114_3;
  TNode<IntPtrT> phi_bb114_4;
  TNode<IntPtrT> phi_bb114_5;
  TNode<String> phi_bb114_8;
  TNode<IntPtrT> phi_bb114_9;
  TNode<String> phi_bb114_11;
  TNode<String> phi_bb114_12;
  TNode<SeqTwoByteString> tmp137;
  if (block114.is_used()) {
    ca_.Bind(&block114, &phi_bb114_3, &phi_bb114_4, &phi_bb114_5, &phi_bb114_8, &phi_bb114_9, &phi_bb114_11, &phi_bb114_12);
    compiler::CodeAssemblerLabel label138(&ca_);
    tmp137 = Cast_SeqTwoByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb114_11)}, &label138);
    ca_.Goto(&block121, phi_bb114_3, phi_bb114_4, phi_bb114_5, phi_bb114_8, phi_bb114_9, phi_bb114_11);
    if (label138.is_used()) {
      ca_.Bind(&label138);
      ca_.Goto(&block122, phi_bb114_3, phi_bb114_4, phi_bb114_5, phi_bb114_8, phi_bb114_9, phi_bb114_11);
    }
  }

  TNode<Object> phi_bb113_3;
  TNode<IntPtrT> phi_bb113_4;
  TNode<IntPtrT> phi_bb113_5;
  TNode<String> phi_bb113_8;
  TNode<IntPtrT> phi_bb113_9;
  TNode<String> phi_bb113_11;
  TNode<String> phi_bb113_12;
  TNode<Object> tmp139;
  TNode<IntPtrT> tmp140;
  TNode<IntPtrT> tmp141;
  TNode<Object> tmp142;
  TNode<IntPtrT> tmp143;
  TNode<IntPtrT> tmp144;
  if (block113.is_used()) {
    ca_.Bind(&block113, &phi_bb113_3, &phi_bb113_4, &phi_bb113_5, &phi_bb113_8, &phi_bb113_9, &phi_bb113_11, &phi_bb113_12);
    std::tie(tmp139, tmp140, tmp141) = FieldSliceSeqOneByteStringChars_0(state_, TNode<SeqOneByteString>{tmp135}).Flatten();
    compiler::CodeAssemblerLabel label145(&ca_);
    std::tie(tmp142, tmp143, tmp144) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{tmp139}, TNode<IntPtrT>{tmp140}, TNode<IntPtrT>{tmp141}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb113_9}, TNode<IntPtrT>{tmp133}, &label145).Flatten();
    ca_.Goto(&block117, phi_bb113_3, phi_bb113_4, phi_bb113_5, phi_bb113_8, phi_bb113_9, phi_bb113_11, phi_bb113_9);
    if (label145.is_used()) {
      ca_.Bind(&label145);
      ca_.Goto(&block118, phi_bb113_3, phi_bb113_4, phi_bb113_5, phi_bb113_8, phi_bb113_9, phi_bb113_11, phi_bb113_9);
    }
  }

  TNode<Object> phi_bb118_3;
  TNode<IntPtrT> phi_bb118_4;
  TNode<IntPtrT> phi_bb118_5;
  TNode<String> phi_bb118_8;
  TNode<IntPtrT> phi_bb118_9;
  TNode<String> phi_bb118_11;
  TNode<IntPtrT> phi_bb118_16;
  if (block118.is_used()) {
    ca_.Bind(&block118, &phi_bb118_3, &phi_bb118_4, &phi_bb118_5, &phi_bb118_8, &phi_bb118_9, &phi_bb118_11, &phi_bb118_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb117_3;
  TNode<IntPtrT> phi_bb117_4;
  TNode<IntPtrT> phi_bb117_5;
  TNode<String> phi_bb117_8;
  TNode<IntPtrT> phi_bb117_9;
  TNode<String> phi_bb117_11;
  TNode<IntPtrT> phi_bb117_16;
  if (block117.is_used()) {
    ca_.Bind(&block117, &phi_bb117_3, &phi_bb117_4, &phi_bb117_5, &phi_bb117_8, &phi_bb117_9, &phi_bb117_11, &phi_bb117_16);
    ca_.Goto(&block107, phi_bb117_3, phi_bb117_4, phi_bb117_5, tmp142, tmp143, tmp144);
  }

  TNode<Object> phi_bb122_3;
  TNode<IntPtrT> phi_bb122_4;
  TNode<IntPtrT> phi_bb122_5;
  TNode<String> phi_bb122_8;
  TNode<IntPtrT> phi_bb122_9;
  TNode<String> phi_bb122_11;
  TNode<ThinString> tmp146;
  if (block122.is_used()) {
    ca_.Bind(&block122, &phi_bb122_3, &phi_bb122_4, &phi_bb122_5, &phi_bb122_8, &phi_bb122_9, &phi_bb122_11);
    compiler::CodeAssemblerLabel label147(&ca_);
    tmp146 = Cast_ThinString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb122_11)}, &label147);
    ca_.Goto(&block129, phi_bb122_3, phi_bb122_4, phi_bb122_5, phi_bb122_8, phi_bb122_9, phi_bb122_11);
    if (label147.is_used()) {
      ca_.Bind(&label147);
      ca_.Goto(&block130, phi_bb122_3, phi_bb122_4, phi_bb122_5, phi_bb122_8, phi_bb122_9, phi_bb122_11);
    }
  }

  TNode<Object> phi_bb121_3;
  TNode<IntPtrT> phi_bb121_4;
  TNode<IntPtrT> phi_bb121_5;
  TNode<String> phi_bb121_8;
  TNode<IntPtrT> phi_bb121_9;
  TNode<String> phi_bb121_11;
  TNode<Object> tmp148;
  TNode<IntPtrT> tmp149;
  TNode<IntPtrT> tmp150;
  TNode<Object> tmp151;
  TNode<IntPtrT> tmp152;
  TNode<IntPtrT> tmp153;
  if (block121.is_used()) {
    ca_.Bind(&block121, &phi_bb121_3, &phi_bb121_4, &phi_bb121_5, &phi_bb121_8, &phi_bb121_9, &phi_bb121_11);
    std::tie(tmp148, tmp149, tmp150) = FieldSliceSeqTwoByteStringChars_0(state_, TNode<SeqTwoByteString>{tmp137}).Flatten();
    compiler::CodeAssemblerLabel label154(&ca_);
    std::tie(tmp151, tmp152, tmp153) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{tmp148}, TNode<IntPtrT>{tmp149}, TNode<IntPtrT>{tmp150}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb121_9}, TNode<IntPtrT>{tmp133}, &label154).Flatten();
    ca_.Goto(&block125, phi_bb121_3, phi_bb121_4, phi_bb121_5, phi_bb121_8, phi_bb121_9, phi_bb121_11, phi_bb121_9);
    if (label154.is_used()) {
      ca_.Bind(&label154);
      ca_.Goto(&block126, phi_bb121_3, phi_bb121_4, phi_bb121_5, phi_bb121_8, phi_bb121_9, phi_bb121_11, phi_bb121_9);
    }
  }

  TNode<Object> phi_bb126_3;
  TNode<IntPtrT> phi_bb126_4;
  TNode<IntPtrT> phi_bb126_5;
  TNode<String> phi_bb126_8;
  TNode<IntPtrT> phi_bb126_9;
  TNode<String> phi_bb126_11;
  TNode<IntPtrT> phi_bb126_16;
  if (block126.is_used()) {
    ca_.Bind(&block126, &phi_bb126_3, &phi_bb126_4, &phi_bb126_5, &phi_bb126_8, &phi_bb126_9, &phi_bb126_11, &phi_bb126_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb125_3;
  TNode<IntPtrT> phi_bb125_4;
  TNode<IntPtrT> phi_bb125_5;
  TNode<String> phi_bb125_8;
  TNode<IntPtrT> phi_bb125_9;
  TNode<String> phi_bb125_11;
  TNode<IntPtrT> phi_bb125_16;
  if (block125.is_used()) {
    ca_.Bind(&block125, &phi_bb125_3, &phi_bb125_4, &phi_bb125_5, &phi_bb125_8, &phi_bb125_9, &phi_bb125_11, &phi_bb125_16);
    ca_.Goto(&block105, phi_bb125_3, phi_bb125_4, phi_bb125_5, tmp151, tmp152, tmp153);
  }

  TNode<Object> phi_bb130_3;
  TNode<IntPtrT> phi_bb130_4;
  TNode<IntPtrT> phi_bb130_5;
  TNode<String> phi_bb130_8;
  TNode<IntPtrT> phi_bb130_9;
  TNode<String> phi_bb130_11;
  TNode<ConsString> tmp155;
  if (block130.is_used()) {
    ca_.Bind(&block130, &phi_bb130_3, &phi_bb130_4, &phi_bb130_5, &phi_bb130_8, &phi_bb130_9, &phi_bb130_11);
    compiler::CodeAssemblerLabel label156(&ca_);
    tmp155 = Cast_ConsString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb130_11)}, &label156);
    ca_.Goto(&block133, phi_bb130_3, phi_bb130_4, phi_bb130_5, phi_bb130_8, phi_bb130_9, phi_bb130_11);
    if (label156.is_used()) {
      ca_.Bind(&label156);
      ca_.Goto(&block134, phi_bb130_3, phi_bb130_4, phi_bb130_5, phi_bb130_8, phi_bb130_9, phi_bb130_11);
    }
  }

  TNode<Object> phi_bb129_3;
  TNode<IntPtrT> phi_bb129_4;
  TNode<IntPtrT> phi_bb129_5;
  TNode<String> phi_bb129_8;
  TNode<IntPtrT> phi_bb129_9;
  TNode<String> phi_bb129_11;
  TNode<IntPtrT> tmp157;
  TNode<String> tmp158;
  if (block129.is_used()) {
    ca_.Bind(&block129, &phi_bb129_3, &phi_bb129_4, &phi_bb129_5, &phi_bb129_8, &phi_bb129_9, &phi_bb129_11);
    tmp157 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp158 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp146, tmp157});
    ca_.Goto(&block127, phi_bb129_3, phi_bb129_4, phi_bb129_5, tmp158, phi_bb129_9, phi_bb129_11);
  }

  TNode<Object> phi_bb134_3;
  TNode<IntPtrT> phi_bb134_4;
  TNode<IntPtrT> phi_bb134_5;
  TNode<String> phi_bb134_8;
  TNode<IntPtrT> phi_bb134_9;
  TNode<String> phi_bb134_11;
  TNode<SlicedString> tmp159;
  if (block134.is_used()) {
    ca_.Bind(&block134, &phi_bb134_3, &phi_bb134_4, &phi_bb134_5, &phi_bb134_8, &phi_bb134_9, &phi_bb134_11);
    compiler::CodeAssemblerLabel label160(&ca_);
    tmp159 = Cast_SlicedString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb134_11)}, &label160);
    ca_.Goto(&block137, phi_bb134_3, phi_bb134_4, phi_bb134_5, phi_bb134_8, phi_bb134_9, phi_bb134_11);
    if (label160.is_used()) {
      ca_.Bind(&label160);
      ca_.Goto(&block138, phi_bb134_3, phi_bb134_4, phi_bb134_5, phi_bb134_8, phi_bb134_9, phi_bb134_11);
    }
  }

  TNode<Object> phi_bb133_3;
  TNode<IntPtrT> phi_bb133_4;
  TNode<IntPtrT> phi_bb133_5;
  TNode<String> phi_bb133_8;
  TNode<IntPtrT> phi_bb133_9;
  TNode<String> phi_bb133_11;
  TNode<String> tmp161;
  if (block133.is_used()) {
    ca_.Bind(&block133, &phi_bb133_3, &phi_bb133_4, &phi_bb133_5, &phi_bb133_8, &phi_bb133_9, &phi_bb133_11);
    tmp161 = Flatten_1(state_, TNode<ConsString>{tmp155});
    ca_.Goto(&block131, phi_bb133_3, phi_bb133_4, phi_bb133_5, tmp161, phi_bb133_9, phi_bb133_11);
  }

  TNode<Object> phi_bb138_3;
  TNode<IntPtrT> phi_bb138_4;
  TNode<IntPtrT> phi_bb138_5;
  TNode<String> phi_bb138_8;
  TNode<IntPtrT> phi_bb138_9;
  TNode<String> phi_bb138_11;
  TNode<ExternalOneByteString> tmp162;
  if (block138.is_used()) {
    ca_.Bind(&block138, &phi_bb138_3, &phi_bb138_4, &phi_bb138_5, &phi_bb138_8, &phi_bb138_9, &phi_bb138_11);
    compiler::CodeAssemblerLabel label163(&ca_);
    tmp162 = Cast_ExternalOneByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb138_11)}, &label163);
    ca_.Goto(&block141, phi_bb138_3, phi_bb138_4, phi_bb138_5, phi_bb138_8, phi_bb138_9, phi_bb138_11);
    if (label163.is_used()) {
      ca_.Bind(&label163);
      ca_.Goto(&block142, phi_bb138_3, phi_bb138_4, phi_bb138_5, phi_bb138_8, phi_bb138_9, phi_bb138_11);
    }
  }

  TNode<Object> phi_bb137_3;
  TNode<IntPtrT> phi_bb137_4;
  TNode<IntPtrT> phi_bb137_5;
  TNode<String> phi_bb137_8;
  TNode<IntPtrT> phi_bb137_9;
  TNode<String> phi_bb137_11;
  TNode<IntPtrT> tmp164;
  TNode<Smi> tmp165;
  TNode<IntPtrT> tmp166;
  TNode<IntPtrT> tmp167;
  TNode<IntPtrT> tmp168;
  TNode<String> tmp169;
  if (block137.is_used()) {
    ca_.Bind(&block137, &phi_bb137_3, &phi_bb137_4, &phi_bb137_5, &phi_bb137_8, &phi_bb137_9, &phi_bb137_11);
    tmp164 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp165 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp159, tmp164});
    tmp166 = Convert_intptr_Smi_0(state_, TNode<Smi>{tmp165});
    tmp167 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb137_9}, TNode<IntPtrT>{tmp166});
    tmp168 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp169 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp159, tmp168});
    ca_.Goto(&block131, phi_bb137_3, phi_bb137_4, phi_bb137_5, tmp169, tmp167, phi_bb137_11);
  }

  TNode<Object> phi_bb142_3;
  TNode<IntPtrT> phi_bb142_4;
  TNode<IntPtrT> phi_bb142_5;
  TNode<String> phi_bb142_8;
  TNode<IntPtrT> phi_bb142_9;
  TNode<String> phi_bb142_11;
  TNode<ExternalTwoByteString> tmp170;
  if (block142.is_used()) {
    ca_.Bind(&block142, &phi_bb142_3, &phi_bb142_4, &phi_bb142_5, &phi_bb142_8, &phi_bb142_9, &phi_bb142_11);
    compiler::CodeAssemblerLabel label171(&ca_);
    tmp170 = Cast_ExternalTwoByteString_1(state_, TNode<String>{ca_.UncheckedCast<String>(phi_bb142_11)}, &label171);
    ca_.Goto(&block149, phi_bb142_3, phi_bb142_4, phi_bb142_5, phi_bb142_8, phi_bb142_9, phi_bb142_11);
    if (label171.is_used()) {
      ca_.Bind(&label171);
      ca_.Goto(&block150, phi_bb142_3, phi_bb142_4, phi_bb142_5, phi_bb142_8, phi_bb142_9, phi_bb142_11);
    }
  }

  TNode<Object> phi_bb141_3;
  TNode<IntPtrT> phi_bb141_4;
  TNode<IntPtrT> phi_bb141_5;
  TNode<String> phi_bb141_8;
  TNode<IntPtrT> phi_bb141_9;
  TNode<String> phi_bb141_11;
  TNode<RawPtrT> tmp172;
  TNode<IntPtrT> tmp173;
  TNode<Int32T> tmp174;
  TNode<IntPtrT> tmp175;
  TNode<Object> tmp176;
  TNode<IntPtrT> tmp177;
  TNode<IntPtrT> tmp178;
  TNode<Object> tmp179;
  TNode<IntPtrT> tmp180;
  TNode<IntPtrT> tmp181;
  if (block141.is_used()) {
    ca_.Bind(&block141, &phi_bb141_3, &phi_bb141_4, &phi_bb141_5, &phi_bb141_8, &phi_bb141_9, &phi_bb141_11);
    tmp172 = Method_ExternalOneByteString_GetChars_0(state_, TNode<ExternalOneByteString>{tmp162});
    tmp173 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp174 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{tmp162, tmp173});
    tmp175 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp174});
    std::tie(tmp176, tmp177, tmp178) = NewOffHeapConstSlice_char8_0(state_, TNode<RawPtrT>{tmp172}, TNode<IntPtrT>{tmp175}).Flatten();
    compiler::CodeAssemblerLabel label182(&ca_);
    std::tie(tmp179, tmp180, tmp181) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{tmp176}, TNode<IntPtrT>{tmp177}, TNode<IntPtrT>{tmp178}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb141_9}, TNode<IntPtrT>{tmp133}, &label182).Flatten();
    ca_.Goto(&block145, phi_bb141_3, phi_bb141_4, phi_bb141_5, phi_bb141_8, phi_bb141_9, phi_bb141_11, phi_bb141_9);
    if (label182.is_used()) {
      ca_.Bind(&label182);
      ca_.Goto(&block146, phi_bb141_3, phi_bb141_4, phi_bb141_5, phi_bb141_8, phi_bb141_9, phi_bb141_11, phi_bb141_9);
    }
  }

  TNode<Object> phi_bb146_3;
  TNode<IntPtrT> phi_bb146_4;
  TNode<IntPtrT> phi_bb146_5;
  TNode<String> phi_bb146_8;
  TNode<IntPtrT> phi_bb146_9;
  TNode<String> phi_bb146_11;
  TNode<IntPtrT> phi_bb146_19;
  if (block146.is_used()) {
    ca_.Bind(&block146, &phi_bb146_3, &phi_bb146_4, &phi_bb146_5, &phi_bb146_8, &phi_bb146_9, &phi_bb146_11, &phi_bb146_19);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb145_3;
  TNode<IntPtrT> phi_bb145_4;
  TNode<IntPtrT> phi_bb145_5;
  TNode<String> phi_bb145_8;
  TNode<IntPtrT> phi_bb145_9;
  TNode<String> phi_bb145_11;
  TNode<IntPtrT> phi_bb145_19;
  if (block145.is_used()) {
    ca_.Bind(&block145, &phi_bb145_3, &phi_bb145_4, &phi_bb145_5, &phi_bb145_8, &phi_bb145_9, &phi_bb145_11, &phi_bb145_19);
    ca_.Goto(&block107, phi_bb145_3, phi_bb145_4, phi_bb145_5, tmp179, tmp180, tmp181);
  }

  TNode<Object> phi_bb150_3;
  TNode<IntPtrT> phi_bb150_4;
  TNode<IntPtrT> phi_bb150_5;
  TNode<String> phi_bb150_8;
  TNode<IntPtrT> phi_bb150_9;
  TNode<String> phi_bb150_11;
  if (block150.is_used()) {
    ca_.Bind(&block150, &phi_bb150_3, &phi_bb150_4, &phi_bb150_5, &phi_bb150_8, &phi_bb150_9, &phi_bb150_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb149_3;
  TNode<IntPtrT> phi_bb149_4;
  TNode<IntPtrT> phi_bb149_5;
  TNode<String> phi_bb149_8;
  TNode<IntPtrT> phi_bb149_9;
  TNode<String> phi_bb149_11;
  TNode<RawPtrT> tmp183;
  TNode<IntPtrT> tmp184;
  TNode<Int32T> tmp185;
  TNode<IntPtrT> tmp186;
  TNode<Object> tmp187;
  TNode<IntPtrT> tmp188;
  TNode<IntPtrT> tmp189;
  TNode<Object> tmp190;
  TNode<IntPtrT> tmp191;
  TNode<IntPtrT> tmp192;
  if (block149.is_used()) {
    ca_.Bind(&block149, &phi_bb149_3, &phi_bb149_4, &phi_bb149_5, &phi_bb149_8, &phi_bb149_9, &phi_bb149_11);
    tmp183 = Method_ExternalTwoByteString_GetChars_0(state_, TNode<ExternalTwoByteString>{tmp170});
    tmp184 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp185 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{tmp170, tmp184});
    tmp186 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp185});
    std::tie(tmp187, tmp188, tmp189) = NewOffHeapConstSlice_char16_0(state_, TNode<RawPtrT>{tmp183}, TNode<IntPtrT>{tmp186}).Flatten();
    compiler::CodeAssemblerLabel label193(&ca_);
    std::tie(tmp190, tmp191, tmp192) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{tmp187}, TNode<IntPtrT>{tmp188}, TNode<IntPtrT>{tmp189}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{phi_bb149_9}, TNode<IntPtrT>{tmp133}, &label193).Flatten();
    ca_.Goto(&block153, phi_bb149_3, phi_bb149_4, phi_bb149_5, phi_bb149_8, phi_bb149_9, phi_bb149_11, phi_bb149_9);
    if (label193.is_used()) {
      ca_.Bind(&label193);
      ca_.Goto(&block154, phi_bb149_3, phi_bb149_4, phi_bb149_5, phi_bb149_8, phi_bb149_9, phi_bb149_11, phi_bb149_9);
    }
  }

  TNode<Object> phi_bb154_3;
  TNode<IntPtrT> phi_bb154_4;
  TNode<IntPtrT> phi_bb154_5;
  TNode<String> phi_bb154_8;
  TNode<IntPtrT> phi_bb154_9;
  TNode<String> phi_bb154_11;
  TNode<IntPtrT> phi_bb154_19;
  if (block154.is_used()) {
    ca_.Bind(&block154, &phi_bb154_3, &phi_bb154_4, &phi_bb154_5, &phi_bb154_8, &phi_bb154_9, &phi_bb154_11, &phi_bb154_19);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb153_3;
  TNode<IntPtrT> phi_bb153_4;
  TNode<IntPtrT> phi_bb153_5;
  TNode<String> phi_bb153_8;
  TNode<IntPtrT> phi_bb153_9;
  TNode<String> phi_bb153_11;
  TNode<IntPtrT> phi_bb153_19;
  if (block153.is_used()) {
    ca_.Bind(&block153, &phi_bb153_3, &phi_bb153_4, &phi_bb153_5, &phi_bb153_8, &phi_bb153_9, &phi_bb153_11, &phi_bb153_19);
    ca_.Goto(&block105, phi_bb153_3, phi_bb153_4, phi_bb153_5, tmp190, tmp191, tmp192);
  }

  TNode<Object> phi_bb131_3;
  TNode<IntPtrT> phi_bb131_4;
  TNode<IntPtrT> phi_bb131_5;
  TNode<String> phi_bb131_8;
  TNode<IntPtrT> phi_bb131_9;
  TNode<String> phi_bb131_11;
  if (block131.is_used()) {
    ca_.Bind(&block131, &phi_bb131_3, &phi_bb131_4, &phi_bb131_5, &phi_bb131_8, &phi_bb131_9, &phi_bb131_11);
    ca_.Goto(&block127, phi_bb131_3, phi_bb131_4, phi_bb131_5, phi_bb131_8, phi_bb131_9, phi_bb131_11);
  }

  TNode<Object> phi_bb127_3;
  TNode<IntPtrT> phi_bb127_4;
  TNode<IntPtrT> phi_bb127_5;
  TNode<String> phi_bb127_8;
  TNode<IntPtrT> phi_bb127_9;
  TNode<String> phi_bb127_11;
  if (block127.is_used()) {
    ca_.Bind(&block127, &phi_bb127_3, &phi_bb127_4, &phi_bb127_5, &phi_bb127_8, &phi_bb127_9, &phi_bb127_11);
    ca_.Goto(&block110, phi_bb127_3, phi_bb127_4, phi_bb127_5, phi_bb127_8, phi_bb127_9);
  }

  TNode<Object> phi_bb109_3;
  TNode<IntPtrT> phi_bb109_4;
  TNode<IntPtrT> phi_bb109_5;
  TNode<String> phi_bb109_8;
  TNode<IntPtrT> phi_bb109_9;
  if (block109.is_used()) {
    ca_.Bind(&block109, &phi_bb109_3, &phi_bb109_4, &phi_bb109_5, &phi_bb109_8, &phi_bb109_9);
    VerifiedUnreachable_0(state_);
  }

  TNode<Object> phi_bb107_3;
  TNode<IntPtrT> phi_bb107_4;
  TNode<IntPtrT> phi_bb107_5;
  TNode<Object> phi_bb107_6;
  TNode<IntPtrT> phi_bb107_7;
  TNode<IntPtrT> phi_bb107_8;
  TNode<BoolT> tmp194;
  if (block107.is_used()) {
    ca_.Bind(&block107, &phi_bb107_3, &phi_bb107_4, &phi_bb107_5, &phi_bb107_6, &phi_bb107_7, &phi_bb107_8);
    tmp194 = Call_char16_char8_0(state_, TorqueStructIsSubstringAtFunctor_0{TNode<IntPtrT>{p_f.start}}, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{phi_bb107_3}, TNode<IntPtrT>{phi_bb107_4}, TNode<IntPtrT>{phi_bb107_5}, TorqueStructUnsafe_0{}}, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{phi_bb107_6}, TNode<IntPtrT>{phi_bb107_7}, TNode<IntPtrT>{phi_bb107_8}, TorqueStructUnsafe_0{}});
    ca_.Goto(&block1, tmp194);
  }

  TNode<Object> phi_bb105_3;
  TNode<IntPtrT> phi_bb105_4;
  TNode<IntPtrT> phi_bb105_5;
  TNode<Object> phi_bb105_6;
  TNode<IntPtrT> phi_bb105_7;
  TNode<IntPtrT> phi_bb105_8;
  TNode<BoolT> tmp195;
  if (block105.is_used()) {
    ca_.Bind(&block105, &phi_bb105_3, &phi_bb105_4, &phi_bb105_5, &phi_bb105_6, &phi_bb105_7, &phi_bb105_8);
    tmp195 = Call_char16_char16_0(state_, TorqueStructIsSubstringAtFunctor_0{TNode<IntPtrT>{p_f.start}}, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{phi_bb105_3}, TNode<IntPtrT>{phi_bb105_4}, TNode<IntPtrT>{phi_bb105_5}, TorqueStructUnsafe_0{}}, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{phi_bb105_6}, TNode<IntPtrT>{phi_bb105_7}, TNode<IntPtrT>{phi_bb105_8}, TorqueStructUnsafe_0{}});
    ca_.Goto(&block1, tmp195);
  }

  TNode<BoolT> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block155);
  }

    ca_.Bind(&block155);
  return TNode<BoolT>{phi_bb1_3};
}


TNode<BoolT> IsSubstringAt_char8_char8_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char8_ConstReference_char8_0 p_string, TorqueStructSlice_char8_ConstReference_char8_0 p_searchStr, TNode<IntPtrT> p_start) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label3(&ca_);
    std::tie(tmp0, tmp1, tmp2) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{p_string.object}, TNode<IntPtrT>{p_string.offset}, TNode<IntPtrT>{p_string.length}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{p_start}, TNode<IntPtrT>{p_searchStr.length}, &label3).Flatten();
    ca_.Goto(&block4);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block5);
    }
  }

  TNode<BoolT> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp4 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp5 = TimesSizeOf_char8_0(state_, TNode<IntPtrT>{tmp2});
    tmp6 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp5});
    tmp7 = TimesSizeOf_char8_0(state_, TNode<IntPtrT>{p_searchStr.length});
    tmp8 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{p_searchStr.offset}, TNode<IntPtrT>{tmp7});
    ca_.Goto(&block10, tmp1, p_searchStr.offset);
  }

  TNode<IntPtrT> phi_bb10_11;
  TNode<IntPtrT> phi_bb10_14;
  TNode<BoolT> tmp9;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_11, &phi_bb10_14);
    tmp9 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp9, &block8, std::vector<compiler::Node*>{phi_bb10_11, phi_bb10_14}, &block9, std::vector<compiler::Node*>{phi_bb10_11, phi_bb10_14});
  }

  TNode<IntPtrT> phi_bb8_11;
  TNode<IntPtrT> phi_bb8_14;
  TNode<BoolT> tmp10;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_11, &phi_bb8_14);
    tmp10 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb8_14}, TNode<IntPtrT>{tmp8});
    ca_.Branch(tmp10, &block15, std::vector<compiler::Node*>{phi_bb8_11, phi_bb8_14}, &block16, std::vector<compiler::Node*>{phi_bb8_11, phi_bb8_14});
  }

  TNode<IntPtrT> phi_bb15_11;
  TNode<IntPtrT> phi_bb15_14;
  TNode<BoolT> tmp11;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_11, &phi_bb15_14);
    tmp11 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp11);
  }

  TNode<IntPtrT> phi_bb16_11;
  TNode<IntPtrT> phi_bb16_14;
  TNode<Object> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<Uint8T> tmp16;
  TNode<BoolT> tmp17;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_11, &phi_bb16_14);
    std::tie(tmp12, tmp13) = NewReference_char8_0(state_, TNode<Object>{p_searchStr.object}, TNode<IntPtrT>{phi_bb16_14}).Flatten();
    tmp14 = FromConstexpr_intptr_constexpr_int31_0(state_, kUInt8Size);
    tmp15 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb16_14}, TNode<IntPtrT>{tmp14});
    tmp16 = CodeStubAssembler(state_).LoadReference<Uint8T>(CodeStubAssembler::Reference{tmp12, tmp13});
    tmp17 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb16_11}, TNode<IntPtrT>{tmp6});
    ca_.Branch(tmp17, &block23, std::vector<compiler::Node*>{phi_bb16_11}, &block24, std::vector<compiler::Node*>{phi_bb16_11});
  }

  TNode<IntPtrT> phi_bb23_11;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb24_11;
  TNode<Object> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<IntPtrT> tmp20;
  TNode<IntPtrT> tmp21;
  TNode<Uint8T> tmp22;
  TNode<BoolT> tmp23;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_11);
    std::tie(tmp18, tmp19) = NewReference_char8_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{phi_bb24_11}).Flatten();
    tmp20 = FromConstexpr_intptr_constexpr_int31_0(state_, kUInt8Size);
    tmp21 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb24_11}, TNode<IntPtrT>{tmp20});
    tmp22 = CodeStubAssembler(state_).LoadReference<Uint8T>(CodeStubAssembler::Reference{tmp18, tmp19});
    tmp23 = CodeStubAssembler(state_).Word32NotEqual(TNode<Uint32T>{tmp16}, TNode<Uint32T>{tmp22});
    ca_.Branch(tmp23, &block27, std::vector<compiler::Node*>{}, &block28, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp24;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp24 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp24);
  }

  if (block28.is_used()) {
    ca_.Bind(&block28);
    ca_.Goto(&block10, tmp21, tmp15);
  }

  TNode<IntPtrT> phi_bb9_11;
  TNode<IntPtrT> phi_bb9_14;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_11, &phi_bb9_14);
    VerifiedUnreachable_0(state_);
  }

  TNode<BoolT> phi_bb1_7;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_7);
    ca_.Goto(&block29, phi_bb1_7);
  }

  TNode<BoolT> phi_bb29_7;
    ca_.Bind(&block29, &phi_bb29_7);
  return TNode<BoolT>{phi_bb29_7};
}


TNode<BoolT> IsSubstringAt_char8_char16_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char8_ConstReference_char8_0 p_string, TorqueStructSlice_char16_ConstReference_char16_0 p_searchStr, TNode<IntPtrT> p_start) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label3(&ca_);
    std::tie(tmp0, tmp1, tmp2) = Subslice_char8_0(state_, TorqueStructSlice_char8_ConstReference_char8_0{TNode<Object>{p_string.object}, TNode<IntPtrT>{p_string.offset}, TNode<IntPtrT>{p_string.length}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{p_start}, TNode<IntPtrT>{p_searchStr.length}, &label3).Flatten();
    ca_.Goto(&block4);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block5);
    }
  }

  TNode<BoolT> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp4 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp5 = TimesSizeOf_char8_0(state_, TNode<IntPtrT>{tmp2});
    tmp6 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp5});
    tmp7 = TimesSizeOf_char16_0(state_, TNode<IntPtrT>{p_searchStr.length});
    tmp8 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{p_searchStr.offset}, TNode<IntPtrT>{tmp7});
    ca_.Goto(&block10, tmp1, p_searchStr.offset);
  }

  TNode<IntPtrT> phi_bb10_11;
  TNode<IntPtrT> phi_bb10_14;
  TNode<BoolT> tmp9;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_11, &phi_bb10_14);
    tmp9 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp9, &block8, std::vector<compiler::Node*>{phi_bb10_11, phi_bb10_14}, &block9, std::vector<compiler::Node*>{phi_bb10_11, phi_bb10_14});
  }

  TNode<IntPtrT> phi_bb8_11;
  TNode<IntPtrT> phi_bb8_14;
  TNode<BoolT> tmp10;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_11, &phi_bb8_14);
    tmp10 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb8_14}, TNode<IntPtrT>{tmp8});
    ca_.Branch(tmp10, &block15, std::vector<compiler::Node*>{phi_bb8_11, phi_bb8_14}, &block16, std::vector<compiler::Node*>{phi_bb8_11, phi_bb8_14});
  }

  TNode<IntPtrT> phi_bb15_11;
  TNode<IntPtrT> phi_bb15_14;
  TNode<BoolT> tmp11;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_11, &phi_bb15_14);
    tmp11 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp11);
  }

  TNode<IntPtrT> phi_bb16_11;
  TNode<IntPtrT> phi_bb16_14;
  TNode<Object> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<Uint16T> tmp16;
  TNode<BoolT> tmp17;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_11, &phi_bb16_14);
    std::tie(tmp12, tmp13) = NewReference_char16_0(state_, TNode<Object>{p_searchStr.object}, TNode<IntPtrT>{phi_bb16_14}).Flatten();
    tmp14 = FromConstexpr_intptr_constexpr_int31_0(state_, kUInt16Size);
    tmp15 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb16_14}, TNode<IntPtrT>{tmp14});
    tmp16 = CodeStubAssembler(state_).LoadReference<Uint16T>(CodeStubAssembler::Reference{tmp12, tmp13});
    tmp17 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb16_11}, TNode<IntPtrT>{tmp6});
    ca_.Branch(tmp17, &block23, std::vector<compiler::Node*>{phi_bb16_11}, &block24, std::vector<compiler::Node*>{phi_bb16_11});
  }

  TNode<IntPtrT> phi_bb23_11;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb24_11;
  TNode<Object> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<IntPtrT> tmp20;
  TNode<IntPtrT> tmp21;
  TNode<Uint8T> tmp22;
  TNode<BoolT> tmp23;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_11);
    std::tie(tmp18, tmp19) = NewReference_char8_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{phi_bb24_11}).Flatten();
    tmp20 = FromConstexpr_intptr_constexpr_int31_0(state_, kUInt8Size);
    tmp21 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb24_11}, TNode<IntPtrT>{tmp20});
    tmp22 = CodeStubAssembler(state_).LoadReference<Uint8T>(CodeStubAssembler::Reference{tmp18, tmp19});
    tmp23 = CodeStubAssembler(state_).Word32NotEqual(TNode<Uint32T>{tmp16}, TNode<Uint32T>{tmp22});
    ca_.Branch(tmp23, &block27, std::vector<compiler::Node*>{}, &block28, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp24;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp24 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp24);
  }

  if (block28.is_used()) {
    ca_.Bind(&block28);
    ca_.Goto(&block10, tmp21, tmp15);
  }

  TNode<IntPtrT> phi_bb9_11;
  TNode<IntPtrT> phi_bb9_14;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_11, &phi_bb9_14);
    VerifiedUnreachable_0(state_);
  }

  TNode<BoolT> phi_bb1_7;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_7);
    ca_.Goto(&block29, phi_bb1_7);
  }

  TNode<BoolT> phi_bb29_7;
    ca_.Bind(&block29, &phi_bb29_7);
  return TNode<BoolT>{phi_bb29_7};
}


TNode<BoolT> IsSubstringAt_char16_char8_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char16_ConstReference_char16_0 p_string, TorqueStructSlice_char8_ConstReference_char8_0 p_searchStr, TNode<IntPtrT> p_start) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label3(&ca_);
    std::tie(tmp0, tmp1, tmp2) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{p_string.object}, TNode<IntPtrT>{p_string.offset}, TNode<IntPtrT>{p_string.length}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{p_start}, TNode<IntPtrT>{p_searchStr.length}, &label3).Flatten();
    ca_.Goto(&block4);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block5);
    }
  }

  TNode<BoolT> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp4 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp5 = TimesSizeOf_char16_0(state_, TNode<IntPtrT>{tmp2});
    tmp6 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp5});
    tmp7 = TimesSizeOf_char8_0(state_, TNode<IntPtrT>{p_searchStr.length});
    tmp8 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{p_searchStr.offset}, TNode<IntPtrT>{tmp7});
    ca_.Goto(&block10, tmp1, p_searchStr.offset);
  }

  TNode<IntPtrT> phi_bb10_11;
  TNode<IntPtrT> phi_bb10_14;
  TNode<BoolT> tmp9;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_11, &phi_bb10_14);
    tmp9 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp9, &block8, std::vector<compiler::Node*>{phi_bb10_11, phi_bb10_14}, &block9, std::vector<compiler::Node*>{phi_bb10_11, phi_bb10_14});
  }

  TNode<IntPtrT> phi_bb8_11;
  TNode<IntPtrT> phi_bb8_14;
  TNode<BoolT> tmp10;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_11, &phi_bb8_14);
    tmp10 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb8_14}, TNode<IntPtrT>{tmp8});
    ca_.Branch(tmp10, &block15, std::vector<compiler::Node*>{phi_bb8_11, phi_bb8_14}, &block16, std::vector<compiler::Node*>{phi_bb8_11, phi_bb8_14});
  }

  TNode<IntPtrT> phi_bb15_11;
  TNode<IntPtrT> phi_bb15_14;
  TNode<BoolT> tmp11;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_11, &phi_bb15_14);
    tmp11 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp11);
  }

  TNode<IntPtrT> phi_bb16_11;
  TNode<IntPtrT> phi_bb16_14;
  TNode<Object> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<Uint8T> tmp16;
  TNode<BoolT> tmp17;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_11, &phi_bb16_14);
    std::tie(tmp12, tmp13) = NewReference_char8_0(state_, TNode<Object>{p_searchStr.object}, TNode<IntPtrT>{phi_bb16_14}).Flatten();
    tmp14 = FromConstexpr_intptr_constexpr_int31_0(state_, kUInt8Size);
    tmp15 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb16_14}, TNode<IntPtrT>{tmp14});
    tmp16 = CodeStubAssembler(state_).LoadReference<Uint8T>(CodeStubAssembler::Reference{tmp12, tmp13});
    tmp17 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb16_11}, TNode<IntPtrT>{tmp6});
    ca_.Branch(tmp17, &block23, std::vector<compiler::Node*>{phi_bb16_11}, &block24, std::vector<compiler::Node*>{phi_bb16_11});
  }

  TNode<IntPtrT> phi_bb23_11;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb24_11;
  TNode<Object> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<IntPtrT> tmp20;
  TNode<IntPtrT> tmp21;
  TNode<Uint16T> tmp22;
  TNode<BoolT> tmp23;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_11);
    std::tie(tmp18, tmp19) = NewReference_char16_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{phi_bb24_11}).Flatten();
    tmp20 = FromConstexpr_intptr_constexpr_int31_0(state_, kUInt16Size);
    tmp21 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb24_11}, TNode<IntPtrT>{tmp20});
    tmp22 = CodeStubAssembler(state_).LoadReference<Uint16T>(CodeStubAssembler::Reference{tmp18, tmp19});
    tmp23 = CodeStubAssembler(state_).Word32NotEqual(TNode<Uint32T>{tmp16}, TNode<Uint32T>{tmp22});
    ca_.Branch(tmp23, &block27, std::vector<compiler::Node*>{}, &block28, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp24;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp24 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp24);
  }

  if (block28.is_used()) {
    ca_.Bind(&block28);
    ca_.Goto(&block10, tmp21, tmp15);
  }

  TNode<IntPtrT> phi_bb9_11;
  TNode<IntPtrT> phi_bb9_14;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_11, &phi_bb9_14);
    VerifiedUnreachable_0(state_);
  }

  TNode<BoolT> phi_bb1_7;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_7);
    ca_.Goto(&block29, phi_bb1_7);
  }

  TNode<BoolT> phi_bb29_7;
    ca_.Bind(&block29, &phi_bb29_7);
  return TNode<BoolT>{phi_bb29_7};
}


TNode<BoolT> IsSubstringAt_char16_char16_0(compiler::CodeAssemblerState* state_, TorqueStructSlice_char16_ConstReference_char16_0 p_string, TorqueStructSlice_char16_ConstReference_char16_0 p_searchStr, TNode<IntPtrT> p_start) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label3(&ca_);
    std::tie(tmp0, tmp1, tmp2) = Subslice_char16_0(state_, TorqueStructSlice_char16_ConstReference_char16_0{TNode<Object>{p_string.object}, TNode<IntPtrT>{p_string.offset}, TNode<IntPtrT>{p_string.length}, TorqueStructUnsafe_0{}}, TNode<IntPtrT>{p_start}, TNode<IntPtrT>{p_searchStr.length}, &label3).Flatten();
    ca_.Goto(&block4);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block5);
    }
  }

  TNode<BoolT> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp4 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp5 = TimesSizeOf_char16_0(state_, TNode<IntPtrT>{tmp2});
    tmp6 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp5});
    tmp7 = TimesSizeOf_char16_0(state_, TNode<IntPtrT>{p_searchStr.length});
    tmp8 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{p_searchStr.offset}, TNode<IntPtrT>{tmp7});
    ca_.Goto(&block10, tmp1, p_searchStr.offset);
  }

  TNode<IntPtrT> phi_bb10_11;
  TNode<IntPtrT> phi_bb10_14;
  TNode<BoolT> tmp9;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_11, &phi_bb10_14);
    tmp9 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp9, &block8, std::vector<compiler::Node*>{phi_bb10_11, phi_bb10_14}, &block9, std::vector<compiler::Node*>{phi_bb10_11, phi_bb10_14});
  }

  TNode<IntPtrT> phi_bb8_11;
  TNode<IntPtrT> phi_bb8_14;
  TNode<BoolT> tmp10;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_11, &phi_bb8_14);
    tmp10 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb8_14}, TNode<IntPtrT>{tmp8});
    ca_.Branch(tmp10, &block15, std::vector<compiler::Node*>{phi_bb8_11, phi_bb8_14}, &block16, std::vector<compiler::Node*>{phi_bb8_11, phi_bb8_14});
  }

  TNode<IntPtrT> phi_bb15_11;
  TNode<IntPtrT> phi_bb15_14;
  TNode<BoolT> tmp11;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_11, &phi_bb15_14);
    tmp11 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp11);
  }

  TNode<IntPtrT> phi_bb16_11;
  TNode<IntPtrT> phi_bb16_14;
  TNode<Object> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<Uint16T> tmp16;
  TNode<BoolT> tmp17;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_11, &phi_bb16_14);
    std::tie(tmp12, tmp13) = NewReference_char16_0(state_, TNode<Object>{p_searchStr.object}, TNode<IntPtrT>{phi_bb16_14}).Flatten();
    tmp14 = FromConstexpr_intptr_constexpr_int31_0(state_, kUInt16Size);
    tmp15 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb16_14}, TNode<IntPtrT>{tmp14});
    tmp16 = CodeStubAssembler(state_).LoadReference<Uint16T>(CodeStubAssembler::Reference{tmp12, tmp13});
    tmp17 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb16_11}, TNode<IntPtrT>{tmp6});
    ca_.Branch(tmp17, &block23, std::vector<compiler::Node*>{phi_bb16_11}, &block24, std::vector<compiler::Node*>{phi_bb16_11});
  }

  TNode<IntPtrT> phi_bb23_11;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb24_11;
  TNode<Object> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<IntPtrT> tmp20;
  TNode<IntPtrT> tmp21;
  TNode<Uint16T> tmp22;
  TNode<BoolT> tmp23;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_11);
    std::tie(tmp18, tmp19) = NewReference_char16_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{phi_bb24_11}).Flatten();
    tmp20 = FromConstexpr_intptr_constexpr_int31_0(state_, kUInt16Size);
    tmp21 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb24_11}, TNode<IntPtrT>{tmp20});
    tmp22 = CodeStubAssembler(state_).LoadReference<Uint16T>(CodeStubAssembler::Reference{tmp18, tmp19});
    tmp23 = CodeStubAssembler(state_).Word32NotEqual(TNode<Uint32T>{tmp16}, TNode<Uint32T>{tmp22});
    ca_.Branch(tmp23, &block27, std::vector<compiler::Node*>{}, &block28, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp24;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp24 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp24);
  }

  if (block28.is_used()) {
    ca_.Bind(&block28);
    ca_.Goto(&block10, tmp21, tmp15);
  }

  TNode<IntPtrT> phi_bb9_11;
  TNode<IntPtrT> phi_bb9_14;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_11, &phi_bb9_14);
    VerifiedUnreachable_0(state_);
  }

  TNode<BoolT> phi_bb1_7;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_7);
    ca_.Goto(&block29, phi_bb1_7);
  }

  TNode<BoolT> phi_bb29_7;
    ca_.Bind(&block29, &phi_bb29_7);
  return TNode<BoolT>{phi_bb29_7};
}


} // namespace internal
} // namespace v8
