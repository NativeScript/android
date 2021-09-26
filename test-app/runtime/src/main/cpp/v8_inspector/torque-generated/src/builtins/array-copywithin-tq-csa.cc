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
#include "torque-generated/src/builtins/array-copywithin-tq-csa.h"
#include "torque-generated/src/builtins/array-copywithin-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"

namespace v8 {
namespace internal {
TNode<Number> ConvertToRelativeIndex_0(compiler::CodeAssemblerState* state_, TNode<Number> p_index, TNode<Number> p_length) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Number> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    tmp1 = NumberIsLessThan_0(state_, TNode<Number>{p_index}, TNode<Number>{tmp0});
    ca_.Branch(tmp1, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<Number> tmp2;
  TNode<Number> tmp3;
  TNode<Number> tmp4;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp2 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{p_index}, TNode<Number>{p_length});
    tmp3 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    tmp4 = Max_0(state_, TNode<Number>{tmp2}, TNode<Number>{tmp3});
    ca_.Goto(&block4, tmp4);
  }

  TNode<Number> tmp5;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp5 = Min_0(state_, TNode<Number>{p_index}, TNode<Number>{p_length});
    ca_.Goto(&block4, tmp5);
  }

  TNode<Number> phi_bb4_2;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_2);
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<Number>{phi_bb4_2};
}


TF_BUILTIN(ArrayPrototypeCopyWithin, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<Number> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Number, Number> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Number, Number> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Number, Number> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Number, Number> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Number, Number> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Number, Number> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Number, Number> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  TNode<Number> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<Number> tmp4;
  TNode<Number> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Object> tmp7;
  TNode<Number> tmp8;
  TNode<Number> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<Object> tmp11;
  TNode<Oddball> tmp12;
  TNode<BoolT> tmp13;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).ToObject_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = GetLengthProperty_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp3 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp2});
    tmp4 = ToInteger_Inline_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp3});
    tmp5 = ConvertToRelativeIndex_0(state_, TNode<Number>{tmp4}, TNode<Number>{tmp1});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp7 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp6});
    tmp8 = ToInteger_Inline_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp7});
    tmp9 = ConvertToRelativeIndex_0(state_, TNode<Number>{tmp8}, TNode<Number>{tmp1});
    tmp10 = FromConstexpr_intptr_constexpr_int31_0(state_, 2);
    tmp11 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp10});
    tmp12 = Undefined_0(state_);
    tmp13 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp11}, TNode<HeapObject>{tmp12});
    ca_.Branch(tmp13, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{tmp1});
  }

  TNode<IntPtrT> tmp14;
  TNode<Object> tmp15;
  TNode<Number> tmp16;
  if (block1.is_used()) {
    ca_.Bind(&block1);
    tmp14 = FromConstexpr_intptr_constexpr_int31_0(state_, 2);
    tmp15 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp14});
    tmp16 = ToInteger_Inline_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp15});
    ca_.Goto(&block2, tmp16);
  }

  TNode<Number> phi_bb2_11;
  TNode<Number> tmp17;
  TNode<Number> tmp18;
  TNode<Number> tmp19;
  TNode<Number> tmp20;
  TNode<Number> tmp21;
  TNode<BoolT> tmp22;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_11);
    tmp17 = ConvertToRelativeIndex_0(state_, TNode<Number>{phi_bb2_11}, TNode<Number>{tmp1});
    tmp18 = CodeStubAssembler(state_).NumberSub(TNode<Number>{tmp17}, TNode<Number>{tmp9});
    tmp19 = CodeStubAssembler(state_).NumberSub(TNode<Number>{tmp1}, TNode<Number>{tmp5});
    tmp20 = Min_0(state_, TNode<Number>{tmp18}, TNode<Number>{tmp19});
    tmp21 = FromConstexpr_Number_constexpr_int31_0(state_, 1);
    tmp22 = NumberIsLessThan_0(state_, TNode<Number>{tmp9}, TNode<Number>{tmp5});
    ca_.Branch(tmp22, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  TNode<Number> tmp23;
  TNode<BoolT> tmp24;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp23 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{tmp9}, TNode<Number>{tmp20});
    tmp24 = NumberIsLessThan_0(state_, TNode<Number>{tmp5}, TNode<Number>{tmp23});
    ca_.Goto(&block7, tmp24);
  }

  TNode<BoolT> tmp25;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp25 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block7, tmp25);
  }

  TNode<BoolT> phi_bb7_16;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_16);
    ca_.Branch(phi_bb7_16, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{tmp5, tmp9, tmp21});
  }

  TNode<Number> tmp26;
  TNode<Number> tmp27;
  TNode<Number> tmp28;
  TNode<Number> tmp29;
  TNode<Number> tmp30;
  TNode<Number> tmp31;
  TNode<Number> tmp32;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp26 = FromConstexpr_Number_constexpr_int31_0(state_, -1);
    tmp27 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{tmp9}, TNode<Number>{tmp20});
    tmp28 = FromConstexpr_Number_constexpr_int31_0(state_, 1);
    tmp29 = CodeStubAssembler(state_).NumberSub(TNode<Number>{tmp27}, TNode<Number>{tmp28});
    tmp30 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{tmp5}, TNode<Number>{tmp20});
    tmp31 = FromConstexpr_Number_constexpr_int31_0(state_, 1);
    tmp32 = CodeStubAssembler(state_).NumberSub(TNode<Number>{tmp30}, TNode<Number>{tmp31});
    ca_.Goto(&block4, tmp32, tmp29, tmp26);
  }

  TNode<Number> phi_bb4_8;
  TNode<Number> phi_bb4_10;
  TNode<Number> phi_bb4_14;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_8, &phi_bb4_10, &phi_bb4_14);
    ca_.Goto(&block10, phi_bb4_8, phi_bb4_10, tmp20);
  }

  TNode<Number> phi_bb10_8;
  TNode<Number> phi_bb10_10;
  TNode<Number> phi_bb10_13;
  TNode<Number> tmp33;
  TNode<BoolT> tmp34;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_8, &phi_bb10_10, &phi_bb10_13);
    tmp33 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    tmp34 = NumberIsGreaterThan_0(state_, TNode<Number>{phi_bb10_13}, TNode<Number>{tmp33});
    ca_.Branch(tmp34, &block8, std::vector<compiler::Node*>{phi_bb10_8, phi_bb10_10, phi_bb10_13}, &block9, std::vector<compiler::Node*>{phi_bb10_8, phi_bb10_10, phi_bb10_13});
  }

  TNode<Number> phi_bb8_8;
  TNode<Number> phi_bb8_10;
  TNode<Number> phi_bb8_13;
  TNode<Oddball> tmp35;
  TNode<Oddball> tmp36;
  TNode<BoolT> tmp37;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_8, &phi_bb8_10, &phi_bb8_13);
    tmp35 = ca_.CallStub<Oddball>(Builtins::CallableFor(ca_.isolate(), Builtin::kHasProperty), parameter0, tmp0, phi_bb8_10);
    tmp36 = True_0(state_);
    tmp37 = CodeStubAssembler(state_).TaggedEqual(TNode<HeapObject>{tmp35}, TNode<HeapObject>{tmp36});
    ca_.Branch(tmp37, &block11, std::vector<compiler::Node*>{phi_bb8_8, phi_bb8_10, phi_bb8_13}, &block12, std::vector<compiler::Node*>{phi_bb8_8, phi_bb8_10, phi_bb8_13});
  }

  TNode<Number> phi_bb11_8;
  TNode<Number> phi_bb11_10;
  TNode<Number> phi_bb11_13;
  TNode<Object> tmp38;
  TNode<Object> tmp39;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_8, &phi_bb11_10, &phi_bb11_13);
    tmp38 = CodeStubAssembler(state_).GetProperty(TNode<Context>{parameter0}, TNode<Object>{tmp0}, TNode<Object>{phi_bb11_10});
    tmp39 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kSetProperty), parameter0, tmp0, phi_bb11_8, tmp38);
    ca_.Goto(&block13, phi_bb11_8, phi_bb11_10, phi_bb11_13);
  }

  TNode<Number> phi_bb12_8;
  TNode<Number> phi_bb12_10;
  TNode<Number> phi_bb12_13;
  TNode<Smi> tmp40;
  TNode<Oddball> tmp41;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_8, &phi_bb12_10, &phi_bb12_13);
    tmp40 = FromConstexpr_LanguageModeSmi_constexpr_LanguageMode_0(state_, LanguageMode::kStrict);
    tmp41 = ca_.CallStub<Oddball>(Builtins::CallableFor(ca_.isolate(), Builtin::kDeleteProperty), parameter0, tmp0, phi_bb12_8, tmp40);
    ca_.Goto(&block13, phi_bb12_8, phi_bb12_10, phi_bb12_13);
  }

  TNode<Number> phi_bb13_8;
  TNode<Number> phi_bb13_10;
  TNode<Number> phi_bb13_13;
  TNode<Number> tmp42;
  TNode<Number> tmp43;
  TNode<Number> tmp44;
  TNode<Number> tmp45;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_8, &phi_bb13_10, &phi_bb13_13);
    tmp42 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{phi_bb13_10}, TNode<Number>{phi_bb4_14});
    tmp43 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{phi_bb13_8}, TNode<Number>{phi_bb4_14});
    tmp44 = FromConstexpr_Number_constexpr_int31_0(state_, 1);
    tmp45 = CodeStubAssembler(state_).NumberSub(TNode<Number>{phi_bb13_13}, TNode<Number>{tmp44});
    ca_.Goto(&block10, tmp43, tmp42, tmp45);
  }

  TNode<Number> phi_bb9_8;
  TNode<Number> phi_bb9_10;
  TNode<Number> phi_bb9_13;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_8, &phi_bb9_10, &phi_bb9_13);
    arguments.PopAndReturn(tmp0);
  }
}

} // namespace internal
} // namespace v8
