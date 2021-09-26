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
#include "torque-generated/src/builtins/promise-finally-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/promise-abstract-operations-tq-csa.h"
#include "torque-generated/src/builtins/promise-all-tq-csa.h"
#include "torque-generated/src/builtins/promise-constructor-tq-csa.h"
#include "torque-generated/src/builtins/promise-finally-tq-csa.h"
#include "torque-generated/src/builtins/promise-misc-tq-csa.h"
#include "torque-generated/src/builtins/promise-resolve-tq-csa.h"
#include "torque-generated/src/builtins/promise-then-tq-csa.h"
#include "torque-generated/src/builtins/promise-jobs-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/contexts-tq-csa.h"

namespace v8 {
namespace internal {
TNode<IntPtrT> FromConstexpr_PromiseValueThunkOrReasonContextSlot_constexpr_kValueSlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseValueThunkOrReasonContextSlot p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ca_.IntPtrConstant(CastToUnderlyingTypeIfEnum(p_o));
    tmp1 = (TNode<IntPtrT>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<IntPtrT>{tmp1};
}


TNode<IntPtrT> FromConstexpr_PromiseValueThunkOrReasonContextSlot_constexpr_kPromiseValueThunkOrReasonContextLength_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseValueThunkOrReasonContextSlot p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ca_.IntPtrConstant(CastToUnderlyingTypeIfEnum(p_o));
    tmp1 = (TNode<IntPtrT>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<IntPtrT>{tmp1};
}


TNode<IntPtrT> kValueSlot_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  TNode<IntPtrT> tmp0;
    tmp0 = FromConstexpr_PromiseValueThunkOrReasonContextSlot_constexpr_kValueSlot_0(state_, PromiseBuiltins::PromiseValueThunkOrReasonContextSlot::kValueSlot);
  TNode<IntPtrT> tmp1;
    tmp1 = (TNode<IntPtrT>{tmp0});
  return TNode<IntPtrT>{tmp1};}

TNode<IntPtrT> FromConstexpr_PromiseFinallyContextSlot_constexpr_kOnFinallySlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseFinallyContextSlot p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ca_.IntPtrConstant(CastToUnderlyingTypeIfEnum(p_o));
    tmp1 = (TNode<IntPtrT>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<IntPtrT>{tmp1};
}


TNode<IntPtrT> FromConstexpr_PromiseFinallyContextSlot_constexpr_kConstructorSlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseFinallyContextSlot p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ca_.IntPtrConstant(CastToUnderlyingTypeIfEnum(p_o));
    tmp1 = (TNode<IntPtrT>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<IntPtrT>{tmp1};
}


TNode<IntPtrT> FromConstexpr_PromiseFinallyContextSlot_constexpr_kPromiseFinallyContextLength_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseFinallyContextSlot p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ca_.IntPtrConstant(CastToUnderlyingTypeIfEnum(p_o));
    tmp1 = (TNode<IntPtrT>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<IntPtrT>{tmp1};
}


TNode<IntPtrT> kOnFinallySlot_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  TNode<IntPtrT> tmp0;
    tmp0 = FromConstexpr_PromiseFinallyContextSlot_constexpr_kOnFinallySlot_0(state_, PromiseBuiltins::PromiseFinallyContextSlot::kOnFinallySlot);
  TNode<IntPtrT> tmp1;
    tmp1 = (TNode<IntPtrT>{tmp0});
  return TNode<IntPtrT>{tmp1};}

TNode<IntPtrT> kConstructorSlot_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  TNode<IntPtrT> tmp0;
    tmp0 = FromConstexpr_PromiseFinallyContextSlot_constexpr_kConstructorSlot_0(state_, PromiseBuiltins::PromiseFinallyContextSlot::kConstructorSlot);
  TNode<IntPtrT> tmp1;
    tmp1 = (TNode<IntPtrT>{tmp0});
  return TNode<IntPtrT>{tmp1};}

TF_BUILTIN(PromiseValueThunkFinally, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Context> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Object> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<Object> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<Context>{parameter0});
    tmp1 = kValueSlot_0(state_);
    std::tie(tmp2, tmp3) = ContextSlot_PromiseValueThunkOrReasonContext_PromiseValueThunkOrReasonContext_JSAny_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp1}).Flatten();
    tmp4 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp2, tmp3});
    CodeStubAssembler(state_).Return(tmp4);
  }
}

TF_BUILTIN(PromiseThrowerFinally, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Context> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Object> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<Object> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<Context>{parameter0});
    tmp1 = kValueSlot_0(state_);
    std::tie(tmp2, tmp3) = ContextSlot_PromiseValueThunkOrReasonContext_PromiseValueThunkOrReasonContext_JSAny_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp1}).Flatten();
    tmp4 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp2, tmp3});
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrow, tmp0, tmp4);
    CodeStubAssembler(state_).Unreachable();
  }
}

TNode<JSFunction> CreateThrowerFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<Object> p_reason) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Context> tmp1;
  TNode<Context> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<Object> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Map> tmp7;
  TNode<SharedFunctionInfo> tmp8;
  TNode<JSFunction> tmp9;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_intptr_0(state_, PromiseBuiltins::PromiseValueThunkOrReasonContextSlot::kPromiseValueThunkOrReasonContextLength);
    tmp1 = AllocateSyntheticFunctionContext_1(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp0});
    tmp2 = (TNode<Context>{tmp1});
    tmp3 = kValueSlot_0(state_);
    InitContextSlot_PromiseValueThunkOrReasonContext_PromiseValueThunkOrReasonContext_JSAny_JSAny_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp3}, TNode<Object>{p_reason});
    tmp4 = STRICT_FUNCTION_WITHOUT_PROTOTYPE_MAP_INDEX_0(state_);
    std::tie(tmp5, tmp6) = ContextSlot_NativeContext_NativeContext_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp4}).Flatten();
    tmp7 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp5, tmp6});
    tmp8 = CodeStubAssembler(state_).PromiseThrowerFinallySharedFunConstant();
    tmp9 = CodeStubAssembler(state_).AllocateFunctionWithMapAndContext(TNode<Map>{tmp7}, TNode<SharedFunctionInfo>{tmp8}, TNode<Context>{tmp2});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<JSFunction>{tmp9};
}


TF_BUILTIN(PromiseCatchFinally, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kReason);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Context> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Object> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<JSReceiver> tmp4;
  TNode<Oddball> tmp5;
  TNode<Object> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<JSReceiver> tmp10;
  TNode<Object> tmp11;
  TNode<NativeContext> tmp12;
  TNode<JSFunction> tmp13;
  TNode<Object> tmp14;
  TNode<Object> tmp15;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<Context>{parameter0});
    tmp1 = kOnFinallySlot_0(state_);
    std::tie(tmp2, tmp3) = ContextSlot_PromiseFinallyContext_PromiseFinallyContext_Callable_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp1}).Flatten();
    tmp4 = CodeStubAssembler(state_).LoadReference<JSReceiver>(CodeStubAssembler::Reference{tmp2, tmp3});
    tmp5 = Undefined_0(state_);
    tmp6 = CodeStubAssembler(state_).Call(TNode<Context>{tmp0}, TNode<Object>{tmp4}, TNode<Object>{tmp5});
    tmp7 = kConstructorSlot_0(state_);
    std::tie(tmp8, tmp9) = ContextSlot_PromiseFinallyContext_PromiseFinallyContext_Constructor_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    tmp10 = CodeStubAssembler(state_).LoadReference<JSReceiver>(CodeStubAssembler::Reference{tmp8, tmp9});
    tmp11 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kPromiseResolve), tmp0, tmp10, tmp6);
    tmp12 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{tmp0});
    tmp13 = CreateThrowerFunction_0(state_, TNode<Context>{tmp0}, TNode<NativeContext>{tmp12}, TNode<Object>{parameter2});
    tmp14 = InvokeThen_0(state_, TNode<Context>{tmp0}, TNode<NativeContext>{tmp12}, TNode<Object>{tmp11}, TNode<Object>{tmp13});
    tmp15 = UnsafeCast_JSAny_0(state_, TNode<Context>{tmp0}, TNode<Object>{tmp14});
    CodeStubAssembler(state_).Return(tmp15);
  }
}

TNode<JSFunction> CreateValueThunkFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<Object> p_value) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Context> tmp1;
  TNode<Context> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<Object> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Map> tmp7;
  TNode<SharedFunctionInfo> tmp8;
  TNode<JSFunction> tmp9;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_intptr_0(state_, PromiseBuiltins::PromiseValueThunkOrReasonContextSlot::kPromiseValueThunkOrReasonContextLength);
    tmp1 = AllocateSyntheticFunctionContext_1(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp0});
    tmp2 = (TNode<Context>{tmp1});
    tmp3 = kValueSlot_0(state_);
    InitContextSlot_PromiseValueThunkOrReasonContext_PromiseValueThunkOrReasonContext_JSAny_JSAny_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp3}, TNode<Object>{p_value});
    tmp4 = STRICT_FUNCTION_WITHOUT_PROTOTYPE_MAP_INDEX_0(state_);
    std::tie(tmp5, tmp6) = ContextSlot_NativeContext_NativeContext_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp4}).Flatten();
    tmp7 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp5, tmp6});
    tmp8 = CodeStubAssembler(state_).PromiseValueThunkFinallySharedFunConstant();
    tmp9 = CodeStubAssembler(state_).AllocateFunctionWithMapAndContext(TNode<Map>{tmp7}, TNode<SharedFunctionInfo>{tmp8}, TNode<Context>{tmp2});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<JSFunction>{tmp9};
}


TF_BUILTIN(PromiseThenFinally, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kValue);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Context> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Object> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<JSReceiver> tmp4;
  TNode<Oddball> tmp5;
  TNode<Object> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<JSReceiver> tmp10;
  TNode<Object> tmp11;
  TNode<NativeContext> tmp12;
  TNode<JSFunction> tmp13;
  TNode<Object> tmp14;
  TNode<Object> tmp15;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<Context>{parameter0});
    tmp1 = kOnFinallySlot_0(state_);
    std::tie(tmp2, tmp3) = ContextSlot_PromiseFinallyContext_PromiseFinallyContext_Callable_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp1}).Flatten();
    tmp4 = CodeStubAssembler(state_).LoadReference<JSReceiver>(CodeStubAssembler::Reference{tmp2, tmp3});
    tmp5 = Undefined_0(state_);
    tmp6 = CodeStubAssembler(state_).Call(TNode<Context>{tmp0}, TNode<Object>{tmp4}, TNode<Object>{tmp5});
    tmp7 = kConstructorSlot_0(state_);
    std::tie(tmp8, tmp9) = ContextSlot_PromiseFinallyContext_PromiseFinallyContext_Constructor_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    tmp10 = CodeStubAssembler(state_).LoadReference<JSReceiver>(CodeStubAssembler::Reference{tmp8, tmp9});
    tmp11 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kPromiseResolve), tmp0, tmp10, tmp6);
    tmp12 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{tmp0});
    tmp13 = CreateValueThunkFunction_0(state_, TNode<Context>{tmp0}, TNode<NativeContext>{tmp12}, TNode<Object>{parameter2});
    tmp14 = InvokeThen_0(state_, TNode<Context>{tmp0}, TNode<NativeContext>{tmp12}, TNode<Object>{tmp11}, TNode<Object>{tmp13});
    tmp15 = UnsafeCast_JSAny_0(state_, TNode<Context>{tmp0}, TNode<Object>{tmp14});
    CodeStubAssembler(state_).Return(tmp15);
  }
}

TorqueStructPromiseFinallyFunctions_0 CreatePromiseFinallyFunctions_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<JSReceiver> p_onFinally, TNode<JSReceiver> p_constructor) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Context> tmp1;
  TNode<Context> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<Object> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Map> tmp8;
  TNode<SharedFunctionInfo> tmp9;
  TNode<JSFunction> tmp10;
  TNode<SharedFunctionInfo> tmp11;
  TNode<JSFunction> tmp12;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_intptr_0(state_, PromiseBuiltins::PromiseFinallyContextSlot::kPromiseFinallyContextLength);
    tmp1 = AllocateSyntheticFunctionContext_1(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp0});
    tmp2 = (TNode<Context>{tmp1});
    tmp3 = kOnFinallySlot_0(state_);
    InitContextSlot_PromiseFinallyContext_PromiseFinallyContext_Callable_Callable_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp3}, TNode<JSReceiver>{p_onFinally});
    tmp4 = kConstructorSlot_0(state_);
    InitContextSlot_PromiseFinallyContext_PromiseFinallyContext_Constructor_Constructor_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp4}, TNode<JSReceiver>{p_constructor});
    tmp5 = STRICT_FUNCTION_WITHOUT_PROTOTYPE_MAP_INDEX_0(state_);
    std::tie(tmp6, tmp7) = ContextSlot_NativeContext_NativeContext_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp5}).Flatten();
    tmp8 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp6, tmp7});
    tmp9 = CodeStubAssembler(state_).PromiseThenFinallySharedFunConstant();
    tmp10 = CodeStubAssembler(state_).AllocateFunctionWithMapAndContext(TNode<Map>{tmp8}, TNode<SharedFunctionInfo>{tmp9}, TNode<Context>{tmp2});
    tmp11 = CodeStubAssembler(state_).PromiseCatchFinallySharedFunConstant();
    tmp12 = CodeStubAssembler(state_).AllocateFunctionWithMapAndContext(TNode<Map>{tmp8}, TNode<SharedFunctionInfo>{tmp11}, TNode<Context>{tmp2});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TorqueStructPromiseFinallyFunctions_0{TNode<JSFunction>{tmp10}, TNode<JSFunction>{tmp12}};
}


TF_BUILTIN(PromisePrototypeFinally, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kOnFinally);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSReceiver> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
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
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCalledOnNonObject, "Promise.prototype.finally");
  }

  TNode<BoolT> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp2 = Is_NativeContext_Context_0(state_, TNode<Context>{parameter0}, TNode<Context>{parameter0});
    ca_.Branch(tmp2, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    {
      auto pos_stack = ca_.GetMacroSourcePositionStack();
      pos_stack.push_back({"src/builtins/promise-finally.tq", 174});
      CodeStubAssembler(state_).FailAssert("Torque assert 'Is<NativeContext>(context)' failed", pos_stack);
    }
  }

  TNode<NativeContext> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<Object> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<JSFunction> tmp7;
  TNode<JSReceiver> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Map> tmp10;
  TNode<BoolT> tmp11;
  TNode<BoolT> tmp12;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp3 = UnsafeCast_NativeContext_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter0});
    tmp4 = PROMISE_FUNCTION_INDEX_0(state_);
    std::tie(tmp5, tmp6) = NativeContextSlot_Context_JSFunction_0(state_, TNode<Context>{parameter0}, TNode<IntPtrT>{tmp4}).Flatten();
    tmp7 = CodeStubAssembler(state_).LoadReference<JSFunction>(CodeStubAssembler::Reference{tmp5, tmp6});
    tmp8 = UnsafeCast_Constructor_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp7});
    tmp9 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp10 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp0, tmp9});
    tmp11 = CodeStubAssembler(state_).IsJSPromiseMap(TNode<Map>{tmp10});
    tmp12 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp11});
    ca_.Branch(tmp12, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp13;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp13 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block11, tmp13);
  }

  TNode<BoolT> tmp14;
  TNode<BoolT> tmp15;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp14 = IsPromiseSpeciesLookupChainIntact_0(state_, TNode<NativeContext>{tmp3}, TNode<Map>{tmp10});
    tmp15 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp14});
    ca_.Goto(&block11, tmp15);
  }

  TNode<BoolT> phi_bb11_9;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_9);
    ca_.Branch(phi_bb11_9, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{tmp8});
  }

  TNode<JSReceiver> tmp16;
  TNode<JSReceiver> tmp17;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp16 = CodeStubAssembler(state_).SpeciesConstructor(TNode<Context>{parameter0}, TNode<Object>{tmp0}, TNode<JSReceiver>{tmp7});
    tmp17 = UnsafeCast_Constructor_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp16});
    ca_.Goto(&block8, tmp17);
  }

  TNode<JSReceiver> phi_bb8_6;
  TNode<JSReceiver> tmp18;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_6);
    compiler::CodeAssemblerLabel label19(&ca_);
    tmp18 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter2}, &label19);
    ca_.Goto(&block18);
    if (label19.is_used()) {
      ca_.Bind(&label19);
      ca_.Goto(&block19);
    }
  }

  if (block19.is_used()) {
    ca_.Bind(&block19);
    ca_.Goto(&block16, parameter2, parameter2);
  }

  TNode<JSFunction> tmp20;
  TNode<JSFunction> tmp21;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    std::tie(tmp20, tmp21) = CreatePromiseFinallyFunctions_0(state_, TNode<Context>{parameter0}, TNode<NativeContext>{tmp3}, TNode<JSReceiver>{tmp18}, TNode<JSReceiver>{phi_bb8_6}).Flatten();
    ca_.Goto(&block16, tmp20, tmp21);
  }

  TNode<Object> phi_bb16_8;
  TNode<Object> phi_bb16_9;
  TNode<Object> tmp22;
  TNode<Object> tmp23;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_8, &phi_bb16_9);
    tmp22 = InvokeThen_1(state_, TNode<Context>{parameter0}, TNode<NativeContext>{tmp3}, TNode<Object>{parameter1}, TNode<Object>{phi_bb16_8}, TNode<Object>{phi_bb16_9});
    tmp23 = UnsafeCast_JSAny_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp22});
    CodeStubAssembler(state_).Return(tmp23);
  }
}

TorqueStructReference_JSAny_0 ContextSlot_PromiseValueThunkOrReasonContext_PromiseValueThunkOrReasonContext_JSAny_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<BoolT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1, tmp2) = FieldSliceContextElements_0(state_, TNode<Context>{p_context}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_Object_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    std::tie(tmp10, tmp11) = ReferenceCast_JSAny_Object_0(state_, TorqueStructReference_Object_0{TNode<Object>{tmp8}, TNode<IntPtrT>{tmp9}, TorqueStructUnsafe_0{}}).Flatten();
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TorqueStructReference_JSAny_0{TNode<Object>{tmp10}, TNode<IntPtrT>{tmp11}, TorqueStructUnsafe_0{}};
}


void InitContextSlot_PromiseValueThunkOrReasonContext_PromiseValueThunkOrReasonContext_JSAny_JSAny_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<Object> p_value) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<BoolT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1, tmp2) = FieldSliceContextElements_0(state_, TNode<Context>{p_context}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp6 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_Object_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp8, tmp9}, p_value);
    ca_.Goto(&block20);
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block20);
}


TorqueStructReference_Callable_0 ContextSlot_PromiseFinallyContext_PromiseFinallyContext_Callable_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<BoolT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1, tmp2) = FieldSliceContextElements_0(state_, TNode<Context>{p_context}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_Object_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    std::tie(tmp10, tmp11) = ReferenceCast_Callable_Object_0(state_, TorqueStructReference_Object_0{TNode<Object>{tmp8}, TNode<IntPtrT>{tmp9}, TorqueStructUnsafe_0{}}).Flatten();
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TorqueStructReference_Callable_0{TNode<Object>{tmp10}, TNode<IntPtrT>{tmp11}, TorqueStructUnsafe_0{}};
}


TorqueStructReference_Constructor_0 ContextSlot_PromiseFinallyContext_PromiseFinallyContext_Constructor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<BoolT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1, tmp2) = FieldSliceContextElements_0(state_, TNode<Context>{p_context}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_Object_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    std::tie(tmp10, tmp11) = ReferenceCast_Constructor_Object_0(state_, TorqueStructReference_Object_0{TNode<Object>{tmp8}, TNode<IntPtrT>{tmp9}, TorqueStructUnsafe_0{}}).Flatten();
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TorqueStructReference_Constructor_0{TNode<Object>{tmp10}, TNode<IntPtrT>{tmp11}, TorqueStructUnsafe_0{}};
}


void InitContextSlot_PromiseFinallyContext_PromiseFinallyContext_Callable_Callable_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<JSReceiver> p_value) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<BoolT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1, tmp2) = FieldSliceContextElements_0(state_, TNode<Context>{p_context}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp6 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_Object_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp8, tmp9}, p_value);
    ca_.Goto(&block20);
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block20);
}


void InitContextSlot_PromiseFinallyContext_PromiseFinallyContext_Constructor_Constructor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<JSReceiver> p_value) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<BoolT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1, tmp2) = FieldSliceContextElements_0(state_, TNode<Context>{p_context}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp6 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_Object_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp8, tmp9}, p_value);
    ca_.Goto(&block20);
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block20);
}


} // namespace internal
} // namespace v8
