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
#include "torque-generated/src/builtins/array-unshift-tq-csa.h"
#include "torque-generated/src/builtins/array-reverse-tq-csa.h"
#include "torque-generated/src/builtins/array-unshift-tq-csa.h"
#include "torque-generated/src/builtins/array-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frames-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"

namespace v8 {
namespace internal {
TNode<Number> GenericArrayUnshift_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, TorqueStructArguments p_arguments) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Smi> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Smi> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Smi> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  TNode<Number> tmp1;
  TNode<Smi> tmp2;
  TNode<Smi> tmp3;
  TNode<BoolT> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).ToObject_Inline(TNode<Context>{p_context}, TNode<Object>{p_receiver});
    tmp1 = GetLengthProperty_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp0});
    tmp2 = Convert_Smi_intptr_0(state_, TNode<IntPtrT>{p_arguments.length});
    tmp3 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp4 = CodeStubAssembler(state_).SmiGreaterThan(TNode<Smi>{tmp2}, TNode<Smi>{tmp3});
    ca_.Branch(tmp4, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<Number> tmp5;
  TNode<Number> tmp6;
  TNode<BoolT> tmp7;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp5 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{tmp1}, TNode<Number>{tmp2});
    tmp6 = FromConstexpr_Number_constexpr_float64_0(state_, kMaxSafeInteger);
    tmp7 = NumberIsGreaterThan_0(state_, TNode<Number>{tmp5}, TNode<Number>{tmp6});
    ca_.Branch(tmp7, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kInvalidArrayLength);
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block8, tmp1);
  }

  TNode<Number> phi_bb8_8;
  TNode<Number> tmp8;
  TNode<BoolT> tmp9;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_8);
    tmp8 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    tmp9 = NumberIsGreaterThan_0(state_, TNode<Number>{phi_bb8_8}, TNode<Number>{tmp8});
    ca_.Branch(tmp9, &block6, std::vector<compiler::Node*>{phi_bb8_8}, &block7, std::vector<compiler::Node*>{phi_bb8_8});
  }

  TNode<Number> phi_bb6_8;
  TNode<Number> tmp10;
  TNode<Number> tmp11;
  TNode<Number> tmp12;
  TNode<Number> tmp13;
  TNode<Number> tmp14;
  TNode<Oddball> tmp15;
  TNode<Oddball> tmp16;
  TNode<BoolT> tmp17;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_8);
    tmp10 = FromConstexpr_Number_constexpr_int31_0(state_, 1);
    tmp11 = CodeStubAssembler(state_).NumberSub(TNode<Number>{phi_bb6_8}, TNode<Number>{tmp10});
    tmp12 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{phi_bb6_8}, TNode<Number>{tmp2});
    tmp13 = FromConstexpr_Number_constexpr_int31_0(state_, 1);
    tmp14 = CodeStubAssembler(state_).NumberSub(TNode<Number>{tmp12}, TNode<Number>{tmp13});
    tmp15 = ca_.CallStub<Oddball>(Builtins::CallableFor(ca_.isolate(), Builtin::kHasProperty), p_context, tmp0, tmp11);
    tmp16 = True_0(state_);
    tmp17 = CodeStubAssembler(state_).TaggedEqual(TNode<HeapObject>{tmp15}, TNode<HeapObject>{tmp16});
    ca_.Branch(tmp17, &block9, std::vector<compiler::Node*>{phi_bb6_8}, &block10, std::vector<compiler::Node*>{phi_bb6_8});
  }

  TNode<Number> phi_bb9_8;
  TNode<Object> tmp18;
  TNode<Object> tmp19;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_8);
    tmp18 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{tmp0}, TNode<Object>{tmp11});
    tmp19 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kSetProperty), p_context, tmp0, tmp14, tmp18);
    ca_.Goto(&block11, phi_bb9_8);
  }

  TNode<Number> phi_bb10_8;
  TNode<Smi> tmp20;
  TNode<Oddball> tmp21;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_8);
    tmp20 = FromConstexpr_LanguageModeSmi_constexpr_LanguageMode_0(state_, LanguageMode::kStrict);
    tmp21 = ca_.CallStub<Oddball>(Builtins::CallableFor(ca_.isolate(), Builtin::kDeleteProperty), p_context, tmp0, tmp14, tmp20);
    ca_.Goto(&block11, phi_bb10_8);
  }

  TNode<Number> phi_bb11_8;
  TNode<Number> tmp22;
  TNode<Number> tmp23;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_8);
    tmp22 = FromConstexpr_Number_constexpr_int31_0(state_, 1);
    tmp23 = CodeStubAssembler(state_).NumberSub(TNode<Number>{phi_bb11_8}, TNode<Number>{tmp22});
    ca_.Goto(&block8, tmp23);
  }

  TNode<Number> phi_bb7_8;
  TNode<Smi> tmp24;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_8);
    tmp24 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    ca_.Goto(&block14, phi_bb7_8, tmp24);
  }

  TNode<Number> phi_bb14_8;
  TNode<Smi> phi_bb14_9;
  TNode<BoolT> tmp25;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_8, &phi_bb14_9);
    tmp25 = CodeStubAssembler(state_).SmiLessThan(TNode<Smi>{phi_bb14_9}, TNode<Smi>{tmp2});
    ca_.Branch(tmp25, &block12, std::vector<compiler::Node*>{phi_bb14_8, phi_bb14_9}, &block13, std::vector<compiler::Node*>{phi_bb14_8, phi_bb14_9});
  }

  TNode<Number> phi_bb12_8;
  TNode<Smi> phi_bb12_9;
  TNode<IntPtrT> tmp26;
  TNode<Object> tmp27;
  TNode<Object> tmp28;
  TNode<Smi> tmp29;
  TNode<Smi> tmp30;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_8, &phi_bb12_9);
    tmp26 = Convert_intptr_Smi_0(state_, TNode<Smi>{phi_bb12_9});
    tmp27 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{p_arguments.frame}, TNode<RawPtrT>{p_arguments.base}, TNode<IntPtrT>{p_arguments.length}}, TNode<IntPtrT>{tmp26});
    tmp28 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kSetProperty), p_context, tmp0, phi_bb12_9, tmp27);
    tmp29 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp30 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb12_9}, TNode<Smi>{tmp29});
    ca_.Goto(&block14, phi_bb12_8, tmp30);
  }

  TNode<Number> phi_bb13_8;
  TNode<Smi> phi_bb13_9;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_8, &phi_bb13_9);
    ca_.Goto(&block3);
  }

  TNode<Number> tmp31;
  TNode<String> tmp32;
  TNode<Object> tmp33;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp31 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{tmp1}, TNode<Number>{tmp2});
    tmp32 = kLengthString_0(state_);
    tmp33 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kSetProperty), p_context, tmp0, tmp32, tmp31);
    ca_.Goto(&block15);
  }

    ca_.Bind(&block15);
  return TNode<Number>{tmp31};
}


TF_BUILTIN(ArrayPrototypeUnshift, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_FastJSArray_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block2);
  }

  TNode<IntPtrT> tmp2;
  TNode<Map> tmp3;
  TNode<BoolT> tmp4;
  TNode<BoolT> tmp5;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    EnsureWriteableFastElements_0(state_, TNode<Context>{parameter0}, TNode<JSArray>{tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp3 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp0, tmp2});
    tmp4 = CodeStubAssembler(state_).IsExtensibleMap(TNode<Map>{tmp3});
    tmp5 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp4});
    ca_.Branch(tmp5, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block2);
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    compiler::CodeAssemblerLabel label6(&ca_);
    EnsureArrayLengthWritable_0(state_, TNode<Context>{parameter0}, TNode<Map>{tmp3}, &label6);
    ca_.Goto(&block7);
    if (label6.is_used()) {
      ca_.Bind(&label6);
      ca_.Goto(&block8);
    }
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(&block2);
  }

  TNode<JSFunction> tmp7;
  TNode<Oddball> tmp8;
  TNode<Int32T> tmp9;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp7 = LoadTargetFromFrame_0(state_);
    tmp8 = Undefined_0(state_);
    tmp9 = Convert_int32_intptr_0(state_, TNode<IntPtrT>{torque_arguments.length});
   CodeStubAssembler(state_).TailCallBuiltin(Builtin::kArrayUnshift, parameter0, tmp7, tmp8, tmp9);
  }

  TNode<Number> tmp10;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp10 = GenericArrayUnshift_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}});
    arguments.PopAndReturn(tmp10);
  }
}

} // namespace internal
} // namespace v8
