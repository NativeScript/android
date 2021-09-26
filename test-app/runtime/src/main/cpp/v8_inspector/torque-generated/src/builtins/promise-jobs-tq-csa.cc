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
#include "torque-generated/src/builtins/promise-jobs-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/promise-abstract-operations-tq-csa.h"
#include "torque-generated/src/builtins/promise-constructor-tq-csa.h"
#include "torque-generated/src/builtins/promise-then-tq-csa.h"
#include "torque-generated/src/builtins/promise-jobs-tq-csa.h"
#include "torque-generated/src/objects/contexts-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(PromiseResolveThenableJob, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSPromise> parameter1 = UncheckedParameter<JSPromise>(Descriptor::kPromiseToResolve);
  USE(parameter1);
  TNode<JSReceiver> parameter2 = UncheckedParameter<JSReceiver>(Descriptor::kThenable);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kThen);
  USE(parameter3);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<NativeContext> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Object> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<JSFunction> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<Map> tmp6;
  TNode<BoolT> tmp7;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{parameter0});
    tmp1 = PROMISE_THEN_INDEX_0(state_);
    std::tie(tmp2, tmp3) = NativeContextSlot_Context_JSFunction_0(state_, TNode<Context>{parameter0}, TNode<IntPtrT>{tmp1}).Flatten();
    tmp4 = CodeStubAssembler(state_).LoadReference<JSFunction>(CodeStubAssembler::Reference{tmp2, tmp3});
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp6 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{parameter2, tmp5});
    tmp7 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{parameter3}, TNode<HeapObject>{tmp4});
    ca_.Branch(tmp7, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp8;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp8 = CodeStubAssembler(state_).IsJSPromiseMap(TNode<Map>{tmp6});
    ca_.Goto(&block5, tmp8);
  }

  TNode<BoolT> tmp9;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp9 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block5, tmp9);
  }

  TNode<BoolT> phi_bb5_8;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_8);
    ca_.Branch(phi_bb5_8, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp10;
  TNode<BoolT> tmp11;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp10 = CodeStubAssembler(state_).NeedsAnyPromiseHooks();
    tmp11 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp10});
    ca_.Goto(&block8, tmp11);
  }

  TNode<BoolT> tmp12;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp12 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block8, tmp12);
  }

  TNode<BoolT> phi_bb8_8;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_8);
    ca_.Branch(phi_bb8_8, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp13;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp13 = IsPromiseSpeciesLookupChainIntact_0(state_, TNode<NativeContext>{tmp0}, TNode<Map>{tmp6});
    ca_.Goto(&block11, tmp13);
  }

  TNode<BoolT> tmp14;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp14 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block11, tmp14);
  }

  TNode<BoolT> phi_bb11_8;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_8);
    ca_.Branch(phi_bb11_8, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  TNode<JSPromise> tmp15;
  TNode<Oddball> tmp16;
  TNode<Oddball> tmp17;
  TNode<Object> tmp18;
  if (block1.is_used()) {
    ca_.Bind(&block1);
    tmp15 = UnsafeCast_JSPromise_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter2});
    tmp16 = CodeStubAssembler(state_).UndefinedConstant();
    tmp17 = CodeStubAssembler(state_).UndefinedConstant();
    tmp18 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kPerformPromiseThen), parameter0, tmp15, tmp16, tmp17, parameter1);
    CodeStubAssembler(state_).Return(tmp18);
  }

  TNode<Oddball> tmp19;
  TNode<JSFunction> tmp20;
  TNode<JSFunction> tmp21;
  TNode<JSReceiver> tmp22;
      TNode<Object> tmp24;
  TNode<Object> tmp25;
      TNode<Object> tmp27;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp19 = False_0(state_);
    std::tie(tmp20, tmp21) = CreatePromiseResolvingFunctions_0(state_, TNode<Context>{parameter0}, TNode<JSPromise>{parameter1}, TNode<Oddball>{tmp19}, TNode<NativeContext>{tmp0}).Flatten();
    compiler::CodeAssemblerExceptionHandlerLabel catch23__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch23__label);
    tmp22 = UnsafeCast_Callable_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter3});
    }
    if (catch23__label.is_used()) {
      compiler::CodeAssemblerLabel catch23_skip(&ca_);
      ca_.Goto(&catch23_skip);
      ca_.Bind(&catch23__label, &tmp24);
      ca_.Goto(&block15);
      ca_.Bind(&catch23_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch26__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch26__label);
    tmp25 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp22}, TNode<Object>{parameter2}, TNode<Object>{tmp20}, TNode<Object>{tmp21});
    }
    if (catch26__label.is_used()) {
      compiler::CodeAssemblerLabel catch26_skip(&ca_);
      ca_.Goto(&catch26_skip);
      ca_.Bind(&catch26__label, &tmp27);
      ca_.Goto(&block16);
      ca_.Bind(&catch26_skip);
    }
    CodeStubAssembler(state_).Return(tmp25);
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block14, tmp24);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block14, tmp27);
  }

  TNode<Object> phi_bb14_11;
  TNode<JSReceiver> tmp28;
  TNode<Oddball> tmp29;
  TNode<Object> tmp30;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_11);
    tmp28 = UnsafeCast_Callable_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp21});
    tmp29 = Undefined_0(state_);
    tmp30 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp28}, TNode<Object>{tmp29}, TNode<Object>{phi_bb14_11});
    CodeStubAssembler(state_).Return(tmp30);
  }
}

} // namespace internal
} // namespace v8
