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
#include "torque-generated/src/builtins/array-reduce-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-reduce-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/objects/js-array-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(ArrayReducePreLoopEagerDeoptContinuation, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kCallback);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kLength);
  USE(parameter3);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSReceiver_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<JSReceiver> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter2}, &label3);
    ca_.Goto(&block7);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block8);
    }
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Number> tmp4;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_Number_0(state_, TNode<Object>{parameter3}, &label5);
    ca_.Goto(&block11);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block12);
    }
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Oddball> tmp6;
  TNode<Number> tmp7;
  TNode<Object> tmp8;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp6 = TheHole_0(state_);
    tmp7 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    tmp8 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kArrayReduceLoopContinuation), parameter0, tmp0, tmp2, tmp6, tmp0, tmp7, tmp4);
    CodeStubAssembler(state_).Return(tmp8);
  }
}

TF_BUILTIN(ArrayReduceLoopEagerDeoptContinuation, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kCallback);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kInitialK);
  USE(parameter3);
  TNode<Object> parameter4 = UncheckedParameter<Object>(Descriptor::kLength);
  USE(parameter4);
  TNode<Object> parameter5 = UncheckedParameter<Object>(Descriptor::kAccumulator);
  USE(parameter5);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSReceiver_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<JSReceiver> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter2}, &label3);
    ca_.Goto(&block7);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block8);
    }
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Number> tmp4;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_Number_0(state_, TNode<Object>{parameter3}, &label5);
    ca_.Goto(&block11);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block12);
    }
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Number> tmp6;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = Cast_Number_0(state_, TNode<Object>{parameter4}, &label7);
    ca_.Goto(&block15);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block16);
    }
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> tmp8;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp8 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kArrayReduceLoopContinuation), parameter0, tmp0, tmp2, parameter5, tmp0, tmp4, tmp6);
    CodeStubAssembler(state_).Return(tmp8);
  }
}

TF_BUILTIN(ArrayReduceLoopLazyDeoptContinuation, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kCallback);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kInitialK);
  USE(parameter3);
  TNode<Object> parameter4 = UncheckedParameter<Object>(Descriptor::kLength);
  USE(parameter4);
  TNode<Object> parameter5 = UncheckedParameter<Object>(Descriptor::kResult);
  USE(parameter5);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSReceiver_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<JSReceiver> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter2}, &label3);
    ca_.Goto(&block7);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block8);
    }
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Number> tmp4;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_Number_0(state_, TNode<Object>{parameter3}, &label5);
    ca_.Goto(&block11);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block12);
    }
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Number> tmp6;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = Cast_Number_0(state_, TNode<Object>{parameter4}, &label7);
    ca_.Goto(&block15);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block16);
    }
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> tmp8;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp8 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kArrayReduceLoopContinuation), parameter0, tmp0, tmp2, parameter5, tmp0, tmp4, tmp6);
    CodeStubAssembler(state_).Return(tmp8);
  }
}

TF_BUILTIN(ArrayReduceLoopContinuation, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<JSReceiver> parameter2 = UncheckedParameter<JSReceiver>(Descriptor::kCallbackfn);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kInitialAccumulator);
  USE(parameter3);
  TNode<JSReceiver> parameter4 = UncheckedParameter<JSReceiver>(Descriptor::kO);
  USE(parameter4);
  TNode<Number> parameter5 = UncheckedParameter<Number>(Descriptor::kInitialK);
  USE(parameter5);
  TNode<Number> parameter6 = UncheckedParameter<Number>(Descriptor::kLength);
  USE(parameter6);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Number> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Number> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Number> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Number, Object, Object> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Number, Object, Object> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Number, Object> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Number> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Number> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object, Object> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object, Object> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    ca_.Goto(&block3, parameter3, parameter5);
  }

  TNode<Object> phi_bb3_7;
  TNode<Number> phi_bb3_8;
  TNode<BoolT> tmp0;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_7, &phi_bb3_8);
    tmp0 = NumberIsLessThan_0(state_, TNode<Number>{phi_bb3_8}, TNode<Number>{parameter6});
    ca_.Branch(tmp0, &block1, std::vector<compiler::Node*>{phi_bb3_7, phi_bb3_8}, &block2, std::vector<compiler::Node*>{phi_bb3_7, phi_bb3_8});
  }

  TNode<Object> phi_bb1_7;
  TNode<Number> phi_bb1_8;
  TNode<Oddball> tmp1;
  TNode<Oddball> tmp2;
  TNode<BoolT> tmp3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_7, &phi_bb1_8);
    tmp1 = CodeStubAssembler(state_).HasProperty_Inline(TNode<Context>{parameter0}, TNode<JSReceiver>{parameter4}, TNode<Object>{phi_bb1_8});
    tmp2 = True_0(state_);
    tmp3 = CodeStubAssembler(state_).TaggedEqual(TNode<HeapObject>{tmp1}, TNode<HeapObject>{tmp2});
    ca_.Branch(tmp3, &block5, std::vector<compiler::Node*>{phi_bb1_7, phi_bb1_8}, &block6, std::vector<compiler::Node*>{phi_bb1_7, phi_bb1_8});
  }

  TNode<Object> phi_bb5_7;
  TNode<Number> phi_bb5_8;
  TNode<Object> tmp4;
  TNode<Oddball> tmp5;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_7, &phi_bb5_8);
    tmp4 = CodeStubAssembler(state_).GetProperty(TNode<Context>{parameter0}, TNode<Object>{parameter4}, TNode<Object>{phi_bb5_8});
    compiler::CodeAssemblerLabel label6(&ca_);
    tmp5 = Cast_TheHole_0(state_, TNode<Object>{phi_bb5_7}, &label6);
    ca_.Goto(&block9, phi_bb5_7, phi_bb5_8, phi_bb5_7, phi_bb5_7);
    if (label6.is_used()) {
      ca_.Bind(&label6);
      ca_.Goto(&block10, phi_bb5_7, phi_bb5_8, phi_bb5_7, phi_bb5_7);
    }
  }

  TNode<Object> phi_bb10_7;
  TNode<Number> phi_bb10_8;
  TNode<Object> phi_bb10_11;
  TNode<Object> phi_bb10_12;
  TNode<Oddball> tmp7;
  TNode<Object> tmp8;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_7, &phi_bb10_8, &phi_bb10_11, &phi_bb10_12);
    tmp7 = Undefined_0(state_);
    tmp8 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{parameter2}, TNode<Object>{tmp7}, TNode<Object>{ca_.UncheckedCast<Object>(phi_bb10_11)}, TNode<Object>{tmp4}, TNode<Object>{phi_bb10_8}, TNode<Object>{parameter4});
    ca_.Goto(&block7, tmp8, phi_bb10_8, phi_bb10_11);
  }

  TNode<Object> phi_bb9_7;
  TNode<Number> phi_bb9_8;
  TNode<Object> phi_bb9_11;
  TNode<Object> phi_bb9_12;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_7, &phi_bb9_8, &phi_bb9_11, &phi_bb9_12);
    ca_.Goto(&block7, tmp4, phi_bb9_8, phi_bb9_11);
  }

  TNode<Object> phi_bb7_7;
  TNode<Number> phi_bb7_8;
  TNode<Object> phi_bb7_11;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_7, &phi_bb7_8, &phi_bb7_11);
    ca_.Goto(&block6, phi_bb7_7, phi_bb7_8);
  }

  TNode<Object> phi_bb6_7;
  TNode<Number> phi_bb6_8;
  TNode<Number> tmp9;
  TNode<Number> tmp10;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_7, &phi_bb6_8);
    tmp9 = FromConstexpr_Number_constexpr_int31_0(state_, 1);
    tmp10 = CodeStubAssembler(state_).NumberAdd(TNode<Number>{phi_bb6_8}, TNode<Number>{tmp9});
    ca_.Goto(&block3, phi_bb6_7, tmp10);
  }

  TNode<Object> phi_bb2_7;
  TNode<Number> phi_bb2_8;
  TNode<Oddball> tmp11;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_7, &phi_bb2_8);
    compiler::CodeAssemblerLabel label12(&ca_);
    tmp11 = Cast_TheHole_0(state_, TNode<Object>{phi_bb2_7}, &label12);
    ca_.Goto(&block13, phi_bb2_7, phi_bb2_7, phi_bb2_7);
    if (label12.is_used()) {
      ca_.Bind(&label12);
      ca_.Goto(&block14, phi_bb2_7, phi_bb2_7, phi_bb2_7);
    }
  }

  TNode<Object> phi_bb14_7;
  TNode<Object> phi_bb14_8;
  TNode<Object> phi_bb14_9;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_7, &phi_bb14_8, &phi_bb14_9);
    CodeStubAssembler(state_).Return(ca_.UncheckedCast<Object>(phi_bb14_8));
  }

  TNode<Object> phi_bb13_7;
  TNode<Object> phi_bb13_8;
  TNode<Object> phi_bb13_9;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_7, &phi_bb13_8, &phi_bb13_9);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kReduceNoInitial, "Array.prototype.reduce");
  }
}

TNode<Object> FastArrayReduce_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_o, TNode<Number> p_len, TNode<JSReceiver> p_callbackfn, TNode<Object> p_initialAccumulator, compiler::CodeAssemblerLabel* label_Bailout, compiler::TypedCodeAssemblerVariable<Number>* label_Bailout_parameter_0, compiler::TypedCodeAssemblerVariable<Object>* label_Bailout_parameter_1) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Smi> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Smi> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Smi> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Smi> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Smi> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Smi> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Smi> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Smi, Smi> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Smi, Smi, Smi> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Smi, Smi, Smi> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Smi, Smi> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Smi, Smi, Smi> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Smi, Smi, Smi> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Smi, Smi, Object> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Object, Object> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Object, Object> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi, Object> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Smi> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Smi> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Object, Object> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, JSArray, Object, Object> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number, Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Smi> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_Smi_0(state_, TNode<Object>{p_len}, &label1);
    ca_.Goto(&block5);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block6);
    }
  }

  TNode<Number> tmp2;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp2 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    ca_.Goto(&block1, tmp2, p_initialAccumulator);
  }

  TNode<JSArray> tmp3;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_FastJSArrayForRead_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{p_o}, &label4);
    ca_.Goto(&block9);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block10);
    }
  }

  TNode<Number> tmp5;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp5 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    ca_.Goto(&block1, tmp5, p_initialAccumulator);
  }

  TNode<JSArray> tmp6;
  TNode<JSArray> tmp7;
  TNode<Map> tmp8;
  TNode<BoolT> tmp9;
  TNode<Smi> tmp10;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    std::tie(tmp6, tmp7, tmp8, tmp9) = NewFastJSArrayForReadWitness_0(state_, TNode<JSArray>{tmp3}).Flatten();
    tmp10 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    ca_.Goto(&block13, p_initialAccumulator, tmp7, tmp10);
  }

  TNode<Object> phi_bb13_5;
  TNode<JSArray> phi_bb13_8;
  TNode<Smi> phi_bb13_11;
  TNode<BoolT> tmp11;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_5, &phi_bb13_8, &phi_bb13_11);
    tmp11 = NumberIsLessThan_0(state_, TNode<Number>{phi_bb13_11}, TNode<Number>{p_len});
    ca_.Branch(tmp11, &block11, std::vector<compiler::Node*>{phi_bb13_5, phi_bb13_8, phi_bb13_11}, &block12, std::vector<compiler::Node*>{phi_bb13_5, phi_bb13_8, phi_bb13_11});
  }

  TNode<Object> phi_bb11_5;
  TNode<JSArray> phi_bb11_8;
  TNode<Smi> phi_bb11_11;
  TNode<IntPtrT> tmp12;
  TNode<Map> tmp13;
  TNode<BoolT> tmp14;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_5, &phi_bb11_8, &phi_bb11_11);
    tmp12 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp13 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp6, tmp12});
    tmp14 = CodeStubAssembler(state_).TaggedNotEqual(TNode<HeapObject>{tmp13}, TNode<HeapObject>{tmp8});
    ca_.Branch(tmp14, &block18, std::vector<compiler::Node*>{phi_bb11_5, phi_bb11_8, phi_bb11_11}, &block19, std::vector<compiler::Node*>{phi_bb11_5, phi_bb11_8, phi_bb11_11});
  }

  TNode<Object> phi_bb18_5;
  TNode<JSArray> phi_bb18_8;
  TNode<Smi> phi_bb18_11;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_5, &phi_bb18_8, &phi_bb18_11);
    ca_.Goto(&block16, phi_bb18_5, phi_bb18_8, phi_bb18_11);
  }

  TNode<Object> phi_bb19_5;
  TNode<JSArray> phi_bb19_8;
  TNode<Smi> phi_bb19_11;
  TNode<BoolT> tmp15;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_5, &phi_bb19_8, &phi_bb19_11);
    tmp15 = CodeStubAssembler(state_).IsNoElementsProtectorCellInvalid();
    ca_.Branch(tmp15, &block20, std::vector<compiler::Node*>{phi_bb19_5, phi_bb19_8, phi_bb19_11}, &block21, std::vector<compiler::Node*>{phi_bb19_5, phi_bb19_8, phi_bb19_11});
  }

  TNode<Object> phi_bb20_5;
  TNode<JSArray> phi_bb20_8;
  TNode<Smi> phi_bb20_11;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_5, &phi_bb20_8, &phi_bb20_11);
    ca_.Goto(&block16, phi_bb20_5, phi_bb20_8, phi_bb20_11);
  }

  TNode<Object> phi_bb21_5;
  TNode<JSArray> phi_bb21_8;
  TNode<Smi> phi_bb21_11;
  TNode<JSArray> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<Number> tmp18;
  TNode<BoolT> tmp19;
  if (block21.is_used()) {
    ca_.Bind(&block21, &phi_bb21_5, &phi_bb21_8, &phi_bb21_11);
    tmp16 = (TNode<JSArray>{tmp6});
    tmp17 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp18 = CodeStubAssembler(state_).LoadReference<Number>(CodeStubAssembler::Reference{tmp16, tmp17});
    tmp19 = NumberIsGreaterThanOrEqual_0(state_, TNode<Number>{phi_bb21_11}, TNode<Number>{tmp18});
    ca_.Branch(tmp19, &block22, std::vector<compiler::Node*>{phi_bb21_5, phi_bb21_11}, &block23, std::vector<compiler::Node*>{phi_bb21_5, phi_bb21_11});
  }

  TNode<Object> phi_bb16_5;
  TNode<JSArray> phi_bb16_8;
  TNode<Smi> phi_bb16_11;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_5, &phi_bb16_8, &phi_bb16_11);
    ca_.Goto(&block1, phi_bb16_11, phi_bb16_5);
  }

  TNode<Object> phi_bb22_5;
  TNode<Smi> phi_bb22_11;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_5, &phi_bb22_11);
    ca_.Goto(&block1, phi_bb22_11, phi_bb22_5);
  }

  TNode<Object> phi_bb23_5;
  TNode<Smi> phi_bb23_11;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_5, &phi_bb23_11);
    ca_.Branch(tmp9, &block28, std::vector<compiler::Node*>{phi_bb23_5, phi_bb23_11, phi_bb23_11, phi_bb23_11}, &block29, std::vector<compiler::Node*>{phi_bb23_5, phi_bb23_11, phi_bb23_11, phi_bb23_11});
  }

  TNode<Object> phi_bb28_5;
  TNode<Smi> phi_bb28_11;
  TNode<Smi> phi_bb28_12;
  TNode<Smi> phi_bb28_15;
  TNode<Object> tmp20;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_5, &phi_bb28_11, &phi_bb28_12, &phi_bb28_15);
    compiler::CodeAssemblerLabel label21(&ca_);
    tmp20 = LoadElementNoHole_FixedDoubleArray_0(state_, TNode<Context>{p_context}, TNode<JSArray>{tmp16}, TNode<Smi>{phi_bb28_15}, &label21);
    ca_.Goto(&block31, phi_bb28_5, phi_bb28_11, phi_bb28_12, phi_bb28_15, phi_bb28_15);
    if (label21.is_used()) {
      ca_.Bind(&label21);
      ca_.Goto(&block32, phi_bb28_5, phi_bb28_11, phi_bb28_12, phi_bb28_15, phi_bb28_15);
    }
  }

  TNode<Object> phi_bb32_5;
  TNode<Smi> phi_bb32_11;
  TNode<Smi> phi_bb32_12;
  TNode<Smi> phi_bb32_15;
  TNode<Smi> phi_bb32_17;
  if (block32.is_used()) {
    ca_.Bind(&block32, &phi_bb32_5, &phi_bb32_11, &phi_bb32_12, &phi_bb32_15, &phi_bb32_17);
    ca_.Goto(&block26, phi_bb32_5, phi_bb32_11);
  }

  TNode<Object> phi_bb31_5;
  TNode<Smi> phi_bb31_11;
  TNode<Smi> phi_bb31_12;
  TNode<Smi> phi_bb31_15;
  TNode<Smi> phi_bb31_17;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_5, &phi_bb31_11, &phi_bb31_12, &phi_bb31_15, &phi_bb31_17);
    ca_.Goto(&block27, phi_bb31_5, phi_bb31_11, phi_bb31_12, phi_bb31_15, tmp20);
  }

  TNode<Object> phi_bb29_5;
  TNode<Smi> phi_bb29_11;
  TNode<Smi> phi_bb29_12;
  TNode<Smi> phi_bb29_15;
  TNode<Object> tmp22;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_5, &phi_bb29_11, &phi_bb29_12, &phi_bb29_15);
    compiler::CodeAssemblerLabel label23(&ca_);
    tmp22 = LoadElementNoHole_FixedArray_0(state_, TNode<Context>{p_context}, TNode<JSArray>{tmp16}, TNode<Smi>{phi_bb29_15}, &label23);
    ca_.Goto(&block33, phi_bb29_5, phi_bb29_11, phi_bb29_12, phi_bb29_15, phi_bb29_15);
    if (label23.is_used()) {
      ca_.Bind(&label23);
      ca_.Goto(&block34, phi_bb29_5, phi_bb29_11, phi_bb29_12, phi_bb29_15, phi_bb29_15);
    }
  }

  TNode<Object> phi_bb34_5;
  TNode<Smi> phi_bb34_11;
  TNode<Smi> phi_bb34_12;
  TNode<Smi> phi_bb34_15;
  TNode<Smi> phi_bb34_17;
  if (block34.is_used()) {
    ca_.Bind(&block34, &phi_bb34_5, &phi_bb34_11, &phi_bb34_12, &phi_bb34_15, &phi_bb34_17);
    ca_.Goto(&block26, phi_bb34_5, phi_bb34_11);
  }

  TNode<Object> phi_bb33_5;
  TNode<Smi> phi_bb33_11;
  TNode<Smi> phi_bb33_12;
  TNode<Smi> phi_bb33_15;
  TNode<Smi> phi_bb33_17;
  if (block33.is_used()) {
    ca_.Bind(&block33, &phi_bb33_5, &phi_bb33_11, &phi_bb33_12, &phi_bb33_15, &phi_bb33_17);
    ca_.Goto(&block27, phi_bb33_5, phi_bb33_11, phi_bb33_12, phi_bb33_15, tmp22);
  }

  TNode<Object> phi_bb27_5;
  TNode<Smi> phi_bb27_11;
  TNode<Smi> phi_bb27_12;
  TNode<Smi> phi_bb27_15;
  TNode<Object> phi_bb27_16;
  TNode<Oddball> tmp24;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_5, &phi_bb27_11, &phi_bb27_12, &phi_bb27_15, &phi_bb27_16);
    compiler::CodeAssemblerLabel label25(&ca_);
    tmp24 = Cast_TheHole_0(state_, TNode<Object>{phi_bb27_5}, &label25);
    ca_.Goto(&block37, phi_bb27_5, phi_bb27_11, phi_bb27_5, phi_bb27_5);
    if (label25.is_used()) {
      ca_.Bind(&label25);
      ca_.Goto(&block38, phi_bb27_5, phi_bb27_11, phi_bb27_5, phi_bb27_5);
    }
  }

  TNode<Object> phi_bb26_5;
  TNode<Smi> phi_bb26_11;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_5, &phi_bb26_11);
    ca_.Goto(&block14, phi_bb26_5, phi_bb26_11);
  }

  TNode<Object> phi_bb38_5;
  TNode<Smi> phi_bb38_11;
  TNode<Object> phi_bb38_13;
  TNode<Object> phi_bb38_14;
  TNode<Oddball> tmp26;
  TNode<Object> tmp27;
  if (block38.is_used()) {
    ca_.Bind(&block38, &phi_bb38_5, &phi_bb38_11, &phi_bb38_13, &phi_bb38_14);
    tmp26 = Undefined_0(state_);
    tmp27 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{p_callbackfn}, TNode<Object>{tmp26}, TNode<Object>{ca_.UncheckedCast<Object>(phi_bb38_13)}, TNode<Object>{phi_bb27_16}, TNode<Object>{phi_bb38_11}, TNode<Object>{tmp16});
    ca_.Goto(&block35, tmp27, phi_bb38_11, phi_bb38_13);
  }

  TNode<Object> phi_bb37_5;
  TNode<Smi> phi_bb37_11;
  TNode<Object> phi_bb37_13;
  TNode<Object> phi_bb37_14;
  if (block37.is_used()) {
    ca_.Bind(&block37, &phi_bb37_5, &phi_bb37_11, &phi_bb37_13, &phi_bb37_14);
    ca_.Goto(&block35, phi_bb27_16, phi_bb37_11, phi_bb37_13);
  }

  TNode<Object> phi_bb35_5;
  TNode<Smi> phi_bb35_11;
  TNode<Object> phi_bb35_13;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_5, &phi_bb35_11, &phi_bb35_13);
    ca_.Goto(&block14, phi_bb35_5, phi_bb35_11);
  }

  TNode<Object> phi_bb14_5;
  TNode<Smi> phi_bb14_11;
  TNode<Smi> tmp28;
  TNode<Smi> tmp29;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_5, &phi_bb14_11);
    tmp28 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp29 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb14_11}, TNode<Smi>{tmp28});
    ca_.Goto(&block13, phi_bb14_5, tmp16, tmp29);
  }

  TNode<Object> phi_bb12_5;
  TNode<JSArray> phi_bb12_8;
  TNode<Smi> phi_bb12_11;
  TNode<Oddball> tmp30;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_5, &phi_bb12_8, &phi_bb12_11);
    compiler::CodeAssemblerLabel label31(&ca_);
    tmp30 = Cast_TheHole_0(state_, TNode<Object>{phi_bb12_5}, &label31);
    ca_.Goto(&block42, phi_bb12_5, phi_bb12_8, phi_bb12_5, phi_bb12_5);
    if (label31.is_used()) {
      ca_.Bind(&label31);
      ca_.Goto(&block43, phi_bb12_5, phi_bb12_8, phi_bb12_5, phi_bb12_5);
    }
  }

  TNode<Object> phi_bb43_5;
  TNode<JSArray> phi_bb43_8;
  TNode<Object> phi_bb43_11;
  TNode<Object> phi_bb43_12;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_5, &phi_bb43_8, &phi_bb43_11, &phi_bb43_12);
    ca_.Goto(&block44);
  }

  TNode<Object> phi_bb42_5;
  TNode<JSArray> phi_bb42_8;
  TNode<Object> phi_bb42_11;
  TNode<Object> phi_bb42_12;
  if (block42.is_used()) {
    ca_.Bind(&block42, &phi_bb42_5, &phi_bb42_8, &phi_bb42_11, &phi_bb42_12);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kReduceNoInitial, "Array.prototype.reduce");
  }

  TNode<Number> phi_bb1_0;
  TNode<Object> phi_bb1_1;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_0, &phi_bb1_1);
    *label_Bailout_parameter_1 = phi_bb1_1;
    *label_Bailout_parameter_0 = phi_bb1_0;
    ca_.Goto(label_Bailout);
  }

    ca_.Bind(&block44);
  return TNode<Object>{ca_.UncheckedCast<Object>(phi_bb43_11)};
}


TF_BUILTIN(ArrayReduce, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<JSReceiver> tmp1;
  TNode<Number> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<BoolT> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = RequireObjectCoercible_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, "Array.prototype.reduce");
    tmp1 = CodeStubAssembler(state_).ToObject_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp2 = GetLengthProperty_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp1});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp4 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{torque_arguments.length}, TNode<IntPtrT>{tmp3});
    ca_.Branch(tmp4, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block2);
  }

  TNode<IntPtrT> tmp5;
  TNode<Object> tmp6;
  TNode<JSReceiver> tmp7;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp6 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp5});
    compiler::CodeAssemblerLabel label8(&ca_);
    tmp7 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp6}, &label8);
    ca_.Goto(&block5);
    if (label8.is_used()) {
      ca_.Bind(&label8);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block2);
  }

  TNode<IntPtrT> tmp9;
  TNode<BoolT> tmp10;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp9 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp10 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{torque_arguments.length}, TNode<IntPtrT>{tmp9});
    ca_.Branch(tmp10, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp11;
  TNode<Object> tmp12;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp12 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp11});
    ca_.Goto(&block9, tmp12);
  }

  TNode<Oddball> tmp13;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp13 = TheHole_0(state_);
    ca_.Goto(&block9, tmp13);
  }

  TNode<Object> phi_bb9_8;
  TNode<Object> tmp14;
    compiler::TypedCodeAssemblerVariable<Number> tmp16(&ca_);
    compiler::TypedCodeAssemblerVariable<Object> tmp17(&ca_);
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_8);
    compiler::CodeAssemblerLabel label15(&ca_);
    tmp14 = FastArrayReduce_0(state_, TNode<Context>{parameter0}, TNode<JSReceiver>{tmp1}, TNode<Number>{tmp2}, TNode<JSReceiver>{tmp7}, TNode<Object>{phi_bb9_8}, &label15, &tmp16, &tmp17);
    ca_.Goto(&block13);
    if (label15.is_used()) {
      ca_.Bind(&label15);
      ca_.Goto(&block14);
    }
  }

  TNode<Object> tmp18;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp18 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kArrayReduceLoopContinuation), parameter0, tmp1, tmp7, tmp17.value(), tmp1, tmp16.value(), tmp2);
    arguments.PopAndReturn(tmp18);
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    arguments.PopAndReturn(tmp14);
  }

  TNode<IntPtrT> tmp19;
  TNode<Object> tmp20;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp19 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp20 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp19});
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCalledNonCallable, TNode<Object>{tmp20});
  }
}

} // namespace internal
} // namespace v8
