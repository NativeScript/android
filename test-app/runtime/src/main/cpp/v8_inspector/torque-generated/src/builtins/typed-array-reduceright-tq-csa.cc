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
#include "torque-generated/src/builtins/typed-array-reduceright-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-every-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-reduceright-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-buffer-tq-csa.h"

namespace v8 {
namespace internal {
const char* kBuiltinNameReduceRight_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  return "%TypedArray%.prototype.reduceRight";}

TNode<Object> ReduceRightAllElements_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSTypedArray> p_array, TNode<JSReceiver> p_callbackfn, TNode<Object> p_initialValue) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray, Object, UintPtrT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray, Object> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray, Object> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray, Object> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object, Object> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object, Object> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray, Object> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray, Object, Object, Object> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray, Object, Object, Object> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<JSTypedArray> tmp1;
  TNode<BuiltinPtr> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1, tmp2) = NewAttachedJSTypedArrayWitness_0(state_, TNode<JSTypedArray>{p_array}).Flatten();
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp4 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp1, tmp3});
    ca_.Goto(&block5, tmp1, p_initialValue, tmp4);
  }

  TNode<JSTypedArray> phi_bb5_5;
  TNode<Object> phi_bb5_8;
  TNode<UintPtrT> phi_bb5_9;
  TNode<UintPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  TNode<UintPtrT> tmp7;
  TNode<BoolT> tmp8;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_5, &phi_bb5_8, &phi_bb5_9);
    tmp5 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp6 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{phi_bb5_9}, TNode<UintPtrT>{tmp5});
    tmp7 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp8 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{phi_bb5_9}, TNode<UintPtrT>{tmp7});
    ca_.Branch(tmp8, &block3, std::vector<compiler::Node*>{phi_bb5_5, phi_bb5_8}, &block4, std::vector<compiler::Node*>{phi_bb5_5, phi_bb5_8});
  }

  TNode<JSTypedArray> phi_bb3_5;
  TNode<Object> phi_bb3_8;
  TNode<IntPtrT> tmp9;
  TNode<JSArrayBuffer> tmp10;
  TNode<BoolT> tmp11;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_5, &phi_bb3_8);
    tmp9 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp10 = CodeStubAssembler(state_).LoadReference<JSArrayBuffer>(CodeStubAssembler::Reference{tmp0, tmp9});
    tmp11 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{tmp10});
    ca_.Branch(tmp11, &block9, std::vector<compiler::Node*>{phi_bb3_5, phi_bb3_8}, &block10, std::vector<compiler::Node*>{phi_bb3_5, phi_bb3_8});
  }

  TNode<JSTypedArray> phi_bb9_5;
  TNode<Object> phi_bb9_8;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_5, &phi_bb9_8);
    ca_.Goto(&block4, phi_bb9_5, phi_bb9_8);
  }

  TNode<JSTypedArray> phi_bb10_5;
  TNode<Object> phi_bb10_8;
  TNode<JSTypedArray> tmp12;
  TNode<Numeric> tmp13;
  TNode<Oddball> tmp14;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_5, &phi_bb10_8);
    tmp12 = (TNode<JSTypedArray>{tmp0});
tmp13 = TORQUE_CAST(CodeStubAssembler(state_).CallBuiltinPointer(Builtins::CallableFor(ca_.isolate(),ExampleBuiltinForTorqueFunctionPointerType(1)).descriptor(), tmp2, TNode<Object>(), tmp12, tmp6));
    compiler::CodeAssemblerLabel label15(&ca_);
    tmp14 = Cast_TheHole_0(state_, TNode<Object>{phi_bb10_8}, &label15);
    ca_.Goto(&block14, phi_bb10_8, phi_bb10_8, phi_bb10_8);
    if (label15.is_used()) {
      ca_.Bind(&label15);
      ca_.Goto(&block15, phi_bb10_8, phi_bb10_8, phi_bb10_8);
    }
  }

  TNode<Object> phi_bb15_8;
  TNode<Object> phi_bb15_11;
  TNode<Object> phi_bb15_12;
  TNode<Oddball> tmp16;
  TNode<Number> tmp17;
  TNode<Object> tmp18;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_8, &phi_bb15_11, &phi_bb15_12);
    tmp16 = Undefined_0(state_);
    tmp17 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{tmp6});
    tmp18 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{p_callbackfn}, TNode<Object>{tmp16}, TNode<Object>{ca_.UncheckedCast<Object>(phi_bb15_11)}, TNode<Object>{tmp13}, TNode<Object>{tmp17}, TNode<Object>{tmp0});
    ca_.Goto(&block12, tmp18, phi_bb15_11);
  }

  TNode<Object> phi_bb14_8;
  TNode<Object> phi_bb14_11;
  TNode<Object> phi_bb14_12;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_8, &phi_bb14_11, &phi_bb14_12);
    ca_.Goto(&block12, tmp13, phi_bb14_11);
  }

  TNode<Object> phi_bb12_8;
  TNode<Object> phi_bb12_11;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_8, &phi_bb12_11);
    ca_.Goto(&block5, tmp12, phi_bb12_8, tmp6);
  }

  TNode<JSTypedArray> phi_bb4_5;
  TNode<Object> phi_bb4_8;
  TNode<Oddball> tmp19;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_5, &phi_bb4_8);
    compiler::CodeAssemblerLabel label20(&ca_);
    tmp19 = Cast_TheHole_0(state_, TNode<Object>{phi_bb4_8}, &label20);
    ca_.Goto(&block19, phi_bb4_5, phi_bb4_8, phi_bb4_8, phi_bb4_8);
    if (label20.is_used()) {
      ca_.Bind(&label20);
      ca_.Goto(&block20, phi_bb4_5, phi_bb4_8, phi_bb4_8, phi_bb4_8);
    }
  }

  TNode<JSTypedArray> phi_bb20_5;
  TNode<Object> phi_bb20_8;
  TNode<Object> phi_bb20_9;
  TNode<Object> phi_bb20_10;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_5, &phi_bb20_8, &phi_bb20_9, &phi_bb20_10);
    ca_.Goto(&block21);
  }

  TNode<JSTypedArray> phi_bb19_5;
  TNode<Object> phi_bb19_8;
  TNode<Object> phi_bb19_9;
  TNode<Object> phi_bb19_10;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_5, &phi_bb19_8, &phi_bb19_9, &phi_bb19_10);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kReduceNoInitial, kBuiltinNameReduceRight_0(state_));
  }

    ca_.Bind(&block21);
  return TNode<Object>{ca_.UncheckedCast<Object>(phi_bb20_9)};
}


TF_BUILTIN(TypedArrayPrototypeReduceRight, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSTypedArray_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block7);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block8);
    }
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kNotTypedArray, kBuiltinNameReduceRight_0(state_));
  }

  TNode<JSTypedArray> tmp2;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = EnsureAttached_0(state_, TNode<JSTypedArray>{tmp0}, &label3);
    ca_.Goto(&block9);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block10);
    }
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kDetachedOperation, kBuiltinNameReduceRight_0(state_));
  }

  TNode<IntPtrT> tmp4;
  TNode<Object> tmp5;
  TNode<JSReceiver> tmp6;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp5 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp4});
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp5}, &label7);
    ca_.Goto(&block11);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block12);
    }
  }

  TNode<IntPtrT> tmp8;
  TNode<Object> tmp9;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp8 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp9 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp8});
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCalledNonCallable, TNode<Object>{tmp9});
  }

  TNode<IntPtrT> tmp10;
  TNode<BoolT> tmp11;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp10 = FromConstexpr_intptr_constexpr_int31_0(state_, 2);
    tmp11 = CodeStubAssembler(state_).IntPtrGreaterThanOrEqual(TNode<IntPtrT>{torque_arguments.length}, TNode<IntPtrT>{tmp10});
    ca_.Branch(tmp11, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp12 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp13 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp12});
    ca_.Goto(&block15, tmp13);
  }

  TNode<Oddball> tmp14;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp14 = TheHole_0(state_);
    ca_.Goto(&block15, tmp14);
  }

  TNode<Object> phi_bb15_8;
  TNode<Object> tmp15;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_8);
    tmp15 = ReduceRightAllElements_0(state_, TNode<Context>{parameter0}, TNode<JSTypedArray>{tmp2}, TNode<JSReceiver>{tmp6}, TNode<Object>{phi_bb15_8});
    arguments.PopAndReturn(tmp15);
  }
}

} // namespace internal
} // namespace v8
