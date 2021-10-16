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
#include "torque-generated/src/builtins/promise-reaction-job-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/promise-abstract-operations-tq-csa.h"
#include "torque-generated/src/builtins/promise-reaction-job-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/js-promise-tq-csa.h"
#include "torque-generated/src/objects/promise-tq-csa.h"

namespace v8 {
namespace internal {
TNode<Object> RejectPromiseReactionJob_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_promiseOrCapability, TNode<Object> p_reason, PromiseReaction::Type p_reactionType) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    if (((CodeStubAssembler(state_).ConstexprInt31Equal(p_reactionType, PromiseReaction::kReject)))) {
      ca_.Goto(&block2);
    } else {
      ca_.Goto(&block3);
    }
  }

  TNode<JSPromise> tmp0;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSPromise_0(state_, TNode<HeapObject>{p_promiseOrCapability}, &label1);
    ca_.Goto(&block7);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block8);
    }
  }

  TNode<Oddball> tmp2;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Undefined_2(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(p_promiseOrCapability)}, &label3);
    ca_.Goto(&block11);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block12);
    }
  }

  TNode<Oddball> tmp4;
  TNode<Object> tmp5;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp4 = False_0(state_);
    tmp5 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kRejectPromise), p_context, tmp0, p_reason, tmp4);
    ca_.Goto(&block1, tmp5);
  }

  TNode<IntPtrT> tmp6;
  TNode<Object> tmp7;
  TNode<JSReceiver> tmp8;
  TNode<Oddball> tmp9;
  TNode<Object> tmp10;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp7 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{ca_.UncheckedCast<PromiseCapability>(p_promiseOrCapability), tmp6});
    tmp8 = UnsafeCast_Callable_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp7});
    tmp9 = Undefined_0(state_);
    tmp10 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp8}, TNode<Object>{tmp9}, TNode<Object>{p_reason});
    ca_.Goto(&block1, tmp10);
  }

  TNode<Oddball> tmp11;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp11 = Undefined_0(state_);
    ca_.Goto(&block1, tmp11);
  }

  TNode<BoolT> tmp12;
  TNode<Oddball> tmp13;
  TNode<Object> tmp14;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp12 = FromConstexpr_bool_constexpr_bool_0(state_, (CodeStubAssembler(state_).ConstexprInt31Equal(p_reactionType, PromiseReaction::kFulfill)));
    CodeStubAssembler(state_).StaticAssert(TNode<BoolT>{tmp12}, "static_assert(reactionType == kPromiseReactionFulfill) at src/builtins/promise-reaction-job.tq:33:5");
    tmp13 = Undefined_0(state_);
    tmp14 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kPromiseRejectReactionJob), p_context, p_reason, tmp13, p_promiseOrCapability);
    ca_.Goto(&block1, tmp14);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block13);
  }

    ca_.Bind(&block13);
  return TNode<Object>{phi_bb1_3};
}


TNode<Object> FuflfillPromiseReactionJob_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_promiseOrCapability, TNode<Object> p_result, PromiseReaction::Type p_reactionType) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSPromise> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSPromise_0(state_, TNode<HeapObject>{p_promiseOrCapability}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  TNode<Oddball> tmp2;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Undefined_2(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(p_promiseOrCapability)}, &label3);
    ca_.Goto(&block8);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block9);
    }
  }

  TNode<Object> tmp4;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp4 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kResolvePromise), p_context, tmp0, p_result);
    ca_.Goto(&block1, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<Object> tmp6;
  TNode<JSReceiver> tmp7;
  TNode<Oddball> tmp8;
  TNode<Object> tmp9;
      TNode<Object> tmp11;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp6 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{ca_.UncheckedCast<PromiseCapability>(p_promiseOrCapability), tmp5});
    tmp7 = UnsafeCast_Callable_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp6});
    tmp8 = Undefined_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch10__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch10__label);
    tmp9 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp7}, TNode<Object>{tmp8}, TNode<Object>{p_result});
    }
    if (catch10__label.is_used()) {
      compiler::CodeAssemblerLabel catch10_skip(&ca_);
      ca_.Goto(&catch10_skip);
      ca_.Bind(&catch10__label, &tmp11);
      ca_.Goto(&block12);
      ca_.Bind(&catch10_skip);
    }
    ca_.Goto(&block1, tmp9);
  }

  TNode<Oddball> tmp12;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp12 = Undefined_0(state_);
    ca_.Goto(&block1, tmp12);
  }

  TNode<Object> tmp13;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp13 = RejectPromiseReactionJob_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{p_promiseOrCapability}, TNode<Object>{tmp11}, p_reactionType);
    ca_.Goto(&block1, tmp13);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block13);
  }

    ca_.Bind(&block13);
  return TNode<Object>{phi_bb1_3};
}


TNode<Object> PromiseReactionJob_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_argument, TNode<HeapObject> p_handler, TNode<HeapObject> p_promiseOrCapability, PromiseReaction::Type p_reactionType) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Oddball> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = Undefined_0(state_);
    tmp1 = CodeStubAssembler(state_).TaggedEqual(TNode<HeapObject>{p_handler}, TNode<HeapObject>{tmp0});
    ca_.Branch(tmp1, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  if (block2.is_used()) {
    ca_.Bind(&block2);
    if (((CodeStubAssembler(state_).ConstexprInt31Equal(p_reactionType, PromiseReaction::kFulfill)))) {
      ca_.Goto(&block5);
    } else {
      ca_.Goto(&block6);
    }
  }

  TNode<Object> tmp2;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = FuflfillPromiseReactionJob_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{p_promiseOrCapability}, TNode<Object>{p_argument}, p_reactionType);
    ca_.Goto(&block1, tmp2);
  }

  TNode<BoolT> tmp3;
  TNode<Object> tmp4;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp3 = FromConstexpr_bool_constexpr_bool_0(state_, (CodeStubAssembler(state_).ConstexprInt31Equal(p_reactionType, PromiseReaction::kReject)));
    CodeStubAssembler(state_).StaticAssert(TNode<BoolT>{tmp3}, "static_assert(reactionType == kPromiseReactionReject) at src/builtins/promise-reaction-job.tq:82:7");
    tmp4 = RejectPromiseReactionJob_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{p_promiseOrCapability}, TNode<Object>{p_argument}, p_reactionType);
    ca_.Goto(&block1, tmp4);
  }

  TNode<JSReceiver> tmp5;
      TNode<Object> tmp7;
  TNode<Oddball> tmp8;
  TNode<Object> tmp9;
      TNode<Object> tmp11;
  TNode<Oddball> tmp12;
  TNode<BoolT> tmp13;
      TNode<Object> tmp15;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    compiler::CodeAssemblerExceptionHandlerLabel catch6__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch6__label);
    tmp5 = UnsafeCast_Callable_0(state_, TNode<Context>{p_context}, TNode<Object>{p_handler});
    }
    if (catch6__label.is_used()) {
      compiler::CodeAssemblerLabel catch6_skip(&ca_);
      ca_.Goto(&catch6_skip);
      ca_.Bind(&catch6__label, &tmp7);
      ca_.Goto(&block10);
      ca_.Bind(&catch6_skip);
    }
    tmp8 = Undefined_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch10__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch10__label);
    tmp9 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp5}, TNode<Object>{tmp8}, TNode<Object>{p_argument});
    }
    if (catch10__label.is_used()) {
      compiler::CodeAssemblerLabel catch10_skip(&ca_);
      ca_.Goto(&catch10_skip);
      ca_.Bind(&catch10__label, &tmp11);
      ca_.Goto(&block11);
      ca_.Bind(&catch10_skip);
    }
    tmp12 = Undefined_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch14__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch14__label);
    tmp13 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{p_promiseOrCapability}, TNode<HeapObject>{tmp12});
    }
    if (catch14__label.is_used()) {
      compiler::CodeAssemblerLabel catch14_skip(&ca_);
      ca_.Goto(&catch14_skip);
      ca_.Bind(&catch14__label, &tmp15);
      ca_.Goto(&block14);
      ca_.Bind(&catch14_skip);
    }
    ca_.Branch(tmp13, &block12, std::vector<compiler::Node*>{}, &block13, std::vector<compiler::Node*>{});
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    ca_.Goto(&block9, tmp7);
  }

  if (block11.is_used()) {
    ca_.Bind(&block11);
    ca_.Goto(&block9, tmp11);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block9, tmp15);
  }

  TNode<Oddball> tmp16;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp16 = Undefined_0(state_);
    ca_.Goto(&block1, tmp16);
  }

  TNode<Object> tmp17;
      TNode<Object> tmp19;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    compiler::CodeAssemblerExceptionHandlerLabel catch18__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch18__label);
    tmp17 = FuflfillPromiseReactionJob_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{p_promiseOrCapability}, TNode<Object>{tmp9}, p_reactionType);
    }
    if (catch18__label.is_used()) {
      compiler::CodeAssemblerLabel catch18_skip(&ca_);
      ca_.Goto(&catch18_skip);
      ca_.Bind(&catch18__label, &tmp19);
      ca_.Goto(&block16);
      ca_.Bind(&catch18_skip);
    }
    ca_.Goto(&block1, tmp17);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block9, tmp19);
  }

  TNode<Object> phi_bb9_4;
  TNode<Object> tmp20;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_4);
    tmp20 = RejectPromiseReactionJob_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{p_promiseOrCapability}, TNode<Object>{phi_bb9_4}, p_reactionType);
    ca_.Goto(&block1, tmp20);
  }

  TNode<Object> phi_bb1_4;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_4);
    ca_.Goto(&block17);
  }

    ca_.Bind(&block17);
  return TNode<Object>{phi_bb1_4};
}


TF_BUILTIN(PromiseFulfillReactionJob, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kValue);
  USE(parameter1);
  TNode<HeapObject> parameter2 = UncheckedParameter<HeapObject>(Descriptor::kHandler);
  USE(parameter2);
  TNode<HeapObject> parameter3 = UncheckedParameter<HeapObject>(Descriptor::kPromiseOrCapability);
  USE(parameter3);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = PromiseReactionJob_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<HeapObject>{parameter2}, TNode<HeapObject>{parameter3}, PromiseReaction::kFulfill);
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(PromiseRejectReactionJob, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReason);
  USE(parameter1);
  TNode<HeapObject> parameter2 = UncheckedParameter<HeapObject>(Descriptor::kHandler);
  USE(parameter2);
  TNode<HeapObject> parameter3 = UncheckedParameter<HeapObject>(Descriptor::kPromiseOrCapability);
  USE(parameter3);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = PromiseReactionJob_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<HeapObject>{parameter2}, TNode<HeapObject>{parameter3}, PromiseReaction::kReject);
    CodeStubAssembler(state_).Return(tmp0);
  }
}

} // namespace internal
} // namespace v8
