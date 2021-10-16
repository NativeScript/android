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
#include "torque-generated/src/builtins/promise-all-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/array-splice-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/iterator-tq-csa.h"
#include "torque-generated/src/builtins/promise-abstract-operations-tq-csa.h"
#include "torque-generated/src/builtins/promise-all-tq-csa.h"
#include "torque-generated/src/builtins/promise-all-element-closure-tq-csa.h"
#include "torque-generated/src/builtins/promise-any-tq-csa.h"
#include "torque-generated/src/builtins/promise-constructor-tq-csa.h"
#include "torque-generated/src/builtins/promise-misc-tq-csa.h"
#include "torque-generated/src/builtins/promise-resolve-tq-csa.h"
#include "torque-generated/src/builtins/promise-jobs-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/contexts-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-tq-csa.h"

namespace v8 {
namespace internal {
TNode<Context> CreatePromiseAllResolveElementContext_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<PromiseCapability> p_capability, TNode<NativeContext> p_nativeContext) {
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
  TNode<FixedArray> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_intptr_0(state_, PromiseBuiltins::PromiseAllResolveElementContextSlots::kPromiseAllResolveElementLength);
    tmp1 = AllocateSyntheticFunctionContext_1(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp0});
    tmp2 = (TNode<Context>{tmp1});
    tmp3 = kPromiseAllResolveElementRemainingSlot_0(state_);
    InitContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_constexpr_int31_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp3}, 1);
    tmp4 = kPromiseAllResolveElementCapabilitySlot_0(state_);
    InitContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_PromiseCapability_PromiseCapability_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp4}, TNode<PromiseCapability>{p_capability});
    tmp5 = kPromiseAllResolveElementValuesSlot_0(state_);
    tmp6 = kEmptyFixedArray_0(state_);
    InitContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_EmptyFixedArray_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp5}, TNode<FixedArray>{tmp6});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Context>{tmp2};
}


TNode<JSFunction> CreatePromiseAllResolveElementFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Context> p_resolveElementContext, TNode<Smi> p_index, TNode<NativeContext> p_nativeContext, TNode<SharedFunctionInfo> p_resolveFunction) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Map> tmp3;
  TNode<JSFunction> tmp4;
  TNode<IntPtrT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = STRICT_FUNCTION_WITHOUT_PROTOTYPE_MAP_INDEX_0(state_);
    std::tie(tmp1, tmp2) = ContextSlot_NativeContext_NativeContext_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp0}).Flatten();
    tmp3 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp1, tmp2});
    tmp4 = CodeStubAssembler(state_).AllocateFunctionWithMapAndContext(TNode<Map>{tmp3}, TNode<SharedFunctionInfo>{p_resolveFunction}, TNode<Context>{p_resolveElementContext});
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp4, tmp5}, p_index);
    ca_.Goto(&block14);
  }

    ca_.Bind(&block14);
  return TNode<JSFunction>{tmp4};
}


TNode<Context> CreatePromiseResolvingFunctionsContext_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSPromise> p_promise, TNode<Oddball> p_debugEvent, TNode<NativeContext> p_nativeContext) {
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
  TNode<Oddball> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<BoolT> tmp11;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_intptr_0(state_, PromiseBuiltins::PromiseResolvingFunctionContextSlot::kPromiseContextLength);
    tmp1 = AllocateSyntheticFunctionContext_1(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp0});
    tmp2 = (TNode<Context>{tmp1});
    tmp3 = kPromiseSlot_0(state_);
    InitContextSlot_PromiseResolvingFunctionContext_PromiseResolvingFunctionContext_JSPromise_JSPromise_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp3}, TNode<JSPromise>{p_promise});
    tmp4 = kAlreadyResolvedSlot_0(state_);
    tmp5 = False_0(state_);
    InitContextSlot_PromiseResolvingFunctionContext_PromiseResolvingFunctionContext_Boolean_False_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp4}, TNode<Oddball>{tmp5});
    tmp6 = kDebugEventSlot_0(state_);
    InitContextSlot_PromiseResolvingFunctionContext_PromiseResolvingFunctionContext_Boolean_Boolean_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp6}, TNode<Oddball>{p_debugEvent});
    tmp7 = FromConstexpr_intptr_constexpr_intptr_0(state_, Context::Field::MIN_CONTEXT_SLOTS);
    tmp8 = FromConstexpr_intptr_constexpr_int31_0(state_, 3);
    tmp9 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp7}, TNode<IntPtrT>{tmp8});
    tmp10 = FromConstexpr_intptr_constexpr_intptr_0(state_, PromiseBuiltins::PromiseResolvingFunctionContextSlot::kPromiseContextLength);
    tmp11 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp10}, TNode<IntPtrT>{tmp9});
    CodeStubAssembler(state_).StaticAssert(TNode<BoolT>{tmp11}, "static_assert(PromiseResolvingFunctionContextSlot::kPromiseContextLength ==\n      ContextSlot::MIN_CONTEXT_SLOTS + 3) at src/builtins/promise-all.tq:77:3");
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Context>{tmp2};
}


TNode<BoolT> IsPromiseThenLookupChainIntact_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<Map> p_receiverMap) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = IsForceSlowPath_0(state_);
    ca_.Branch(tmp0, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp1;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp1 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp1);
  }

  TNode<BoolT> tmp2;
  TNode<BoolT> tmp3;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp2 = CodeStubAssembler(state_).IsJSPromiseMap(TNode<Map>{p_receiverMap});
    tmp3 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp2});
    ca_.Branch(tmp3, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp4;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp4 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<HeapObject> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<JSObject> tmp10;
  TNode<BoolT> tmp11;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp6 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{p_receiverMap, tmp5});
    tmp7 = PROMISE_PROTOTYPE_INDEX_0(state_);
    std::tie(tmp8, tmp9) = NativeContextSlot_JSObject_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp7}).Flatten();
    tmp10 = CodeStubAssembler(state_).LoadReference<JSObject>(CodeStubAssembler::Reference{tmp8, tmp9});
    tmp11 = CodeStubAssembler(state_).TaggedNotEqual(TNode<HeapObject>{tmp6}, TNode<HeapObject>{tmp10});
    ca_.Branch(tmp11, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp12;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp12 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp12);
  }

  TNode<BoolT> tmp13;
  TNode<BoolT> tmp14;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp13 = CodeStubAssembler(state_).IsPromiseThenProtectorCellInvalid();
    tmp14 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp13});
    ca_.Goto(&block1, tmp14);
  }

  TNode<BoolT> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block8, phi_bb1_3);
  }

  TNode<BoolT> phi_bb8_3;
    ca_.Bind(&block8, &phi_bb8_3);
  return TNode<BoolT>{phi_bb8_3};
}


TF_BUILTIN(PromiseAll, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kIterable);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = GeneratePromiseAll_PromiseAllResolveElementFunctor_PromiseAllRejectElementFunctor_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{parameter2}, TorqueStructPromiseAllResolveElementFunctor_0{}, TorqueStructPromiseAllRejectElementFunctor_0{});
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(PromiseAllSettled, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kIterable);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = GeneratePromiseAll_PromiseAllSettledResolveElementFunctor_PromiseAllSettledRejectElementFunctor_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{parameter2}, TorqueStructPromiseAllSettledResolveElementFunctor_0{}, TorqueStructPromiseAllSettledRejectElementFunctor_0{});
    CodeStubAssembler(state_).Return(tmp0);
  }
}

void InitContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_constexpr_int31_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, int31_t p_value) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Smi> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<UintPtrT> tmp5;
  TNode<BoolT> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_Smi_constexpr_int31_0(state_, p_value);
    std::tie(tmp1, tmp2, tmp3) = FieldSliceContextElements_0(state_, TNode<Context>{p_context}).Flatten();
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp5 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp3});
    tmp6 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp4}, TNode<UintPtrT>{tmp5});
    ca_.Branch(tmp6, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<Object> tmp9;
  TNode<IntPtrT> tmp10;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp7 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp8 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp7});
    std::tie(tmp9, tmp10) = NewReference_Object_0(state_, TNode<Object>{tmp1}, TNode<IntPtrT>{tmp8}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp9, tmp10}, tmp0);
    ca_.Goto(&block20);
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block20);
}


void InitContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_PromiseCapability_PromiseCapability_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<PromiseCapability> p_value) {
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


void InitContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_EmptyFixedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<FixedArray> p_value) {
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


TorqueStructReference_Map_0 ContextSlot_NativeContext_NativeContext_Map_0(compiler::CodeAssemblerState* state_, TNode<NativeContext> p_context, TNode<IntPtrT> p_index) {
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
    std::tie(tmp10, tmp11) = ReferenceCast_Map_Object_0(state_, TorqueStructReference_Object_0{TNode<Object>{tmp8}, TNode<IntPtrT>{tmp9}, TorqueStructUnsafe_0{}}).Flatten();
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TorqueStructReference_Map_0{TNode<Object>{tmp10}, TNode<IntPtrT>{tmp11}, TorqueStructUnsafe_0{}};
}


void InitContextSlot_PromiseResolvingFunctionContext_PromiseResolvingFunctionContext_JSPromise_JSPromise_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<JSPromise> p_value) {
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


void InitContextSlot_PromiseResolvingFunctionContext_PromiseResolvingFunctionContext_Boolean_False_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<Oddball> p_value) {
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


void InitContextSlot_PromiseResolvingFunctionContext_PromiseResolvingFunctionContext_Boolean_Boolean_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<Oddball> p_value) {
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


TorqueStructReference_JSObject_0 NativeContextSlot_JSObject_0(compiler::CodeAssemblerState* state_, TNode<NativeContext> p_context, TNode<IntPtrT> p_index) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1) = ContextSlot_NativeContext_NativeContext_JSObject_0(state_, TNode<NativeContext>{p_context}, TNode<IntPtrT>{p_index}).Flatten();
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TorqueStructReference_JSObject_0{TNode<Object>{tmp0}, TNode<IntPtrT>{tmp1}, TorqueStructUnsafe_0{}};
}


TNode<Object> GeneratePromiseAll_PromiseAllResolveElementFunctor_PromiseAllRejectElementFunctor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, TNode<Object> p_iterable, TorqueStructPromiseAllResolveElementFunctor_0 p_createResolveElementFunctor, TorqueStructPromiseAllRejectElementFunctor_0 p_createRejectElementFunctor) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block13(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block11(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<NativeContext> tmp0;
  TNode<JSReceiver> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{p_context});
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_JSReceiver_1(state_, TNode<Context>{p_context}, TNode<Object>{p_receiver}, &label2);
    ca_.Goto(&block4);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kCalledOnNonObject, "Promise.all");
  }

  TNode<Oddball> tmp3;
  TNode<PromiseCapability> tmp4;
  TNode<JSReceiver> tmp5;
  TNode<Object> tmp6;
      TNode<Object> tmp8;
  TNode<JSReceiver> tmp9;
  TNode<Object> tmp10;
      TNode<Object> tmp12;
  TNode<Object> tmp13;
    compiler::TypedCodeAssemblerVariable<Object> tmp15(&ca_);
      TNode<Object> tmp17;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp3 = False_0(state_);
    tmp4 = ca_.CallStub<PromiseCapability>(Builtins::CallableFor(ca_.isolate(), Builtin::kNewPromiseCapability), p_context, tmp1, tmp3);
    tmp5 = UnsafeCast_Constructor_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp1});
    compiler::CodeAssemblerExceptionHandlerLabel catch7__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch7__label);
    tmp6 = GetPromiseResolve_0(state_, TNode<Context>{p_context}, TNode<NativeContext>{tmp0}, TNode<JSReceiver>{tmp5});
    }
    if (catch7__label.is_used()) {
      compiler::CodeAssemblerLabel catch7_skip(&ca_);
      ca_.Goto(&catch7_skip);
      ca_.Bind(&catch7__label, &tmp8);
      ca_.Goto(&block14);
      ca_.Bind(&catch7_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch11__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch11__label);
    std::tie(tmp9, tmp10) = IteratorBuiltinsAssembler(state_).GetIterator(TNode<Context>{p_context}, TNode<Object>{p_iterable}).Flatten();
    }
    if (catch11__label.is_used()) {
      compiler::CodeAssemblerLabel catch11_skip(&ca_);
      ca_.Goto(&catch11_skip);
      ca_.Bind(&catch11__label, &tmp12);
      ca_.Goto(&block15);
      ca_.Bind(&catch11_skip);
    }
    compiler::CodeAssemblerLabel label14(&ca_);
    compiler::CodeAssemblerExceptionHandlerLabel catch16__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch16__label);
    tmp13 = PerformPromiseAll_PromiseAllResolveElementFunctor_PromiseAllRejectElementFunctor_0(state_, TNode<Context>{p_context}, TNode<NativeContext>{tmp0}, TorqueStructIteratorRecord{TNode<JSReceiver>{tmp9}, TNode<Object>{tmp10}}, TNode<JSReceiver>{tmp5}, TNode<PromiseCapability>{tmp4}, TNode<Object>{tmp6}, TorqueStructPromiseAllResolveElementFunctor_0{}, TorqueStructPromiseAllRejectElementFunctor_0{}, &label14, &tmp15);
    }
    if (catch16__label.is_used()) {
      compiler::CodeAssemblerLabel catch16_skip(&ca_);
      ca_.Goto(&catch16_skip);
      ca_.Bind(&catch16__label, &tmp17);
      ca_.Goto(&block18);
      ca_.Bind(&catch16_skip);
    }
    ca_.Goto(&block16);
    if (label14.is_used()) {
      ca_.Bind(&label14);
      ca_.Goto(&block17);
    }
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block13, tmp8);
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block13, tmp12);
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    ca_.Goto(&block13, tmp17);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block11, tmp15.value());
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block1, tmp13);
  }

  TNode<Object> phi_bb13_7;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_7);
    ca_.Goto(&block11, phi_bb13_7);
  }

  TNode<Object> phi_bb11_7;
  TNode<Object> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<Object> tmp20;
  TNode<Object> tmp21;
  TNode<Oddball> tmp22;
  TNode<Object> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<HeapObject> tmp25;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_7);
    tmp18 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{phi_bb11_7});
    tmp19 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp20 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp4, tmp19});
    tmp21 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp20});
    tmp22 = Undefined_0(state_);
    tmp23 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp21}, TNode<Object>{tmp22}, TNode<Object>{tmp18});
    tmp24 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp25 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{tmp4, tmp24});
    ca_.Goto(&block1, tmp25);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block19);
  }

    ca_.Bind(&block19);
  return TNode<Object>{phi_bb1_3};
}


TNode<Object> GeneratePromiseAll_PromiseAllSettledResolveElementFunctor_PromiseAllSettledRejectElementFunctor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, TNode<Object> p_iterable, TorqueStructPromiseAllSettledResolveElementFunctor_0 p_createResolveElementFunctor, TorqueStructPromiseAllSettledRejectElementFunctor_0 p_createRejectElementFunctor) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block13(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block11(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<NativeContext> tmp0;
  TNode<JSReceiver> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{p_context});
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_JSReceiver_1(state_, TNode<Context>{p_context}, TNode<Object>{p_receiver}, &label2);
    ca_.Goto(&block4);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kCalledOnNonObject, "Promise.all");
  }

  TNode<Oddball> tmp3;
  TNode<PromiseCapability> tmp4;
  TNode<JSReceiver> tmp5;
  TNode<Object> tmp6;
      TNode<Object> tmp8;
  TNode<JSReceiver> tmp9;
  TNode<Object> tmp10;
      TNode<Object> tmp12;
  TNode<Object> tmp13;
    compiler::TypedCodeAssemblerVariable<Object> tmp15(&ca_);
      TNode<Object> tmp17;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp3 = False_0(state_);
    tmp4 = ca_.CallStub<PromiseCapability>(Builtins::CallableFor(ca_.isolate(), Builtin::kNewPromiseCapability), p_context, tmp1, tmp3);
    tmp5 = UnsafeCast_Constructor_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp1});
    compiler::CodeAssemblerExceptionHandlerLabel catch7__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch7__label);
    tmp6 = GetPromiseResolve_0(state_, TNode<Context>{p_context}, TNode<NativeContext>{tmp0}, TNode<JSReceiver>{tmp5});
    }
    if (catch7__label.is_used()) {
      compiler::CodeAssemblerLabel catch7_skip(&ca_);
      ca_.Goto(&catch7_skip);
      ca_.Bind(&catch7__label, &tmp8);
      ca_.Goto(&block14);
      ca_.Bind(&catch7_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch11__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch11__label);
    std::tie(tmp9, tmp10) = IteratorBuiltinsAssembler(state_).GetIterator(TNode<Context>{p_context}, TNode<Object>{p_iterable}).Flatten();
    }
    if (catch11__label.is_used()) {
      compiler::CodeAssemblerLabel catch11_skip(&ca_);
      ca_.Goto(&catch11_skip);
      ca_.Bind(&catch11__label, &tmp12);
      ca_.Goto(&block15);
      ca_.Bind(&catch11_skip);
    }
    compiler::CodeAssemblerLabel label14(&ca_);
    compiler::CodeAssemblerExceptionHandlerLabel catch16__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch16__label);
    tmp13 = PerformPromiseAll_PromiseAllSettledResolveElementFunctor_PromiseAllSettledRejectElementFunctor_0(state_, TNode<Context>{p_context}, TNode<NativeContext>{tmp0}, TorqueStructIteratorRecord{TNode<JSReceiver>{tmp9}, TNode<Object>{tmp10}}, TNode<JSReceiver>{tmp5}, TNode<PromiseCapability>{tmp4}, TNode<Object>{tmp6}, TorqueStructPromiseAllSettledResolveElementFunctor_0{}, TorqueStructPromiseAllSettledRejectElementFunctor_0{}, &label14, &tmp15);
    }
    if (catch16__label.is_used()) {
      compiler::CodeAssemblerLabel catch16_skip(&ca_);
      ca_.Goto(&catch16_skip);
      ca_.Bind(&catch16__label, &tmp17);
      ca_.Goto(&block18);
      ca_.Bind(&catch16_skip);
    }
    ca_.Goto(&block16);
    if (label14.is_used()) {
      ca_.Bind(&label14);
      ca_.Goto(&block17);
    }
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block13, tmp8);
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block13, tmp12);
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    ca_.Goto(&block13, tmp17);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block11, tmp15.value());
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block1, tmp13);
  }

  TNode<Object> phi_bb13_7;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_7);
    ca_.Goto(&block11, phi_bb13_7);
  }

  TNode<Object> phi_bb11_7;
  TNode<Object> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<Object> tmp20;
  TNode<Object> tmp21;
  TNode<Oddball> tmp22;
  TNode<Object> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<HeapObject> tmp25;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_7);
    tmp18 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{phi_bb11_7});
    tmp19 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp20 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp4, tmp19});
    tmp21 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp20});
    tmp22 = Undefined_0(state_);
    tmp23 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp21}, TNode<Object>{tmp22}, TNode<Object>{tmp18});
    tmp24 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp25 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{tmp4, tmp24});
    ca_.Goto(&block1, tmp25);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block19);
  }

    ca_.Bind(&block19);
  return TNode<Object>{phi_bb1_3};
}


TNode<Object> PerformPromiseAll_PromiseAllResolveElementFunctor_PromiseAllRejectElementFunctor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TorqueStructIteratorRecord p_iter, TNode<JSReceiver> p_constructor, TNode<PromiseCapability> p_capability, TNode<Object> p_promiseResolveFunction, TorqueStructPromiseAllResolveElementFunctor_0 p_createResolveElementFunctor, TorqueStructPromiseAllRejectElementFunctor_0 p_createRejectElementFunctor, compiler::CodeAssemblerLabel* label_Reject, compiler::TypedCodeAssemblerVariable<Object>* label_Reject_parameter_0) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block13(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block20(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block21(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Object> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi, Smi> block24(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi> block25(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block26(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block27(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block28(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block29(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block30(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block35(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block39(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block40(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block44(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block45(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block49(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block50(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block48(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block51(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block54(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block52(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block55(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block56(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block57(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block58(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block53(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block60(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block61(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block62(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block65(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block66(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block69(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block67(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block70(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block68(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block63(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block71(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block64(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block72(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block73(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block59(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi, Smi> block74(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi> block75(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Object> block8(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block77(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block76(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block78(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block81(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block82(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block79(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block80(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block83(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<HeapObject> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<Object> tmp5;
  TNode<BoolT> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp1 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{p_capability, tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp3 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{p_capability, tmp2});
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp5 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{p_capability, tmp4});
    tmp6 = CodeStubAssembler(state_).IsDebugActive();
    ca_.Branch(tmp6, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  TNode<Symbol> tmp7;
  TNode<Oddball> tmp8;
  TNode<Object> tmp9;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp7 = kPromiseForwardingHandlerSymbol_0(state_);
    tmp8 = True_0(state_);
    tmp9 = CodeStubAssembler(state_).SetPropertyStrict(TNode<Context>{p_context}, TNode<Object>{tmp5}, TNode<Object>{tmp7}, TNode<Object>{tmp8});
    ca_.Goto(&block4);
  }

  TNode<Context> tmp10;
  TNode<Smi> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  TNode<IntPtrT> tmp14;
      TNode<Object> tmp16;
  TNode<Map> tmp17;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp10 = CreatePromiseAllResolveElementContext_0(state_, TNode<Context>{p_context}, TNode<PromiseCapability>{p_capability}, TNode<NativeContext>{p_nativeContext});
    tmp11 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp12 = ITERATOR_RESULT_MAP_INDEX_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch15__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch15__label);
    std::tie(tmp13, tmp14) = NativeContextSlot_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp12}).Flatten();
    }
    if (catch15__label.is_used()) {
      compiler::CodeAssemblerLabel catch15_skip(&ca_);
      ca_.Goto(&catch15_skip);
      ca_.Bind(&catch15__label, &tmp16);
      ca_.Goto(&block9);
      ca_.Bind(&catch15_skip);
    }
    tmp17 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp13, tmp14});
    ca_.Goto(&block12, tmp11);
  }

  if (block9.is_used()) {
    ca_.Bind(&block9);
    ca_.Goto(&block8, tmp11, tmp16);
  }

  TNode<Smi> phi_bb12_11;
  TNode<BoolT> tmp18;
      TNode<Object> tmp20;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch19__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch19__label);
    tmp18 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch19__label.is_used()) {
      compiler::CodeAssemblerLabel catch19_skip(&ca_);
      ca_.Goto(&catch19_skip);
      ca_.Bind(&catch19__label, &tmp20);
      ca_.Goto(&block13, phi_bb12_11);
      ca_.Bind(&catch19_skip);
    }
    ca_.Branch(tmp18, &block10, std::vector<compiler::Node*>{phi_bb12_11}, &block11, std::vector<compiler::Node*>{phi_bb12_11});
  }

  TNode<Smi> phi_bb13_11;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_11);
    ca_.Goto(&block8, phi_bb13_11, tmp20);
  }

  TNode<Smi> phi_bb10_11;
  TNode<JSReceiver> tmp21;
      TNode<Object> tmp24;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_11);
    compiler::CodeAssemblerLabel label22(&ca_);
    compiler::CodeAssemblerExceptionHandlerLabel catch23__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch23__label);
    tmp21 = IteratorBuiltinsAssembler(state_).IteratorStep(TNode<Context>{p_context}, TorqueStructIteratorRecord{TNode<JSReceiver>{p_iter.object}, TNode<Object>{p_iter.next}}, TNode<Map>{tmp17}, &label22);
    }
    if (catch23__label.is_used()) {
      compiler::CodeAssemblerLabel catch23_skip(&ca_);
      ca_.Goto(&catch23_skip);
      ca_.Bind(&catch23__label, &tmp24);
      ca_.Goto(&block20, phi_bb10_11);
      ca_.Bind(&catch23_skip);
    }
    ca_.Goto(&block18, phi_bb10_11);
    if (label22.is_used()) {
      ca_.Bind(&label22);
      ca_.Goto(&block19, phi_bb10_11);
    }
  }

  TNode<Smi> phi_bb20_11;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_11);
    ca_.Goto(&block15, phi_bb20_11, tmp24);
  }

  TNode<Smi> phi_bb19_11;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_11);
    ca_.Goto(&block5, phi_bb19_11);
  }

  TNode<Smi> phi_bb18_11;
  TNode<Object> tmp25;
      TNode<Object> tmp27;
  TNode<Smi> tmp28;
      TNode<Object> tmp30;
  TNode<BoolT> tmp31;
      TNode<Object> tmp33;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch26__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch26__label);
    tmp25 = IteratorBuiltinsAssembler(state_).IteratorValue(TNode<Context>{p_context}, TNode<JSReceiver>{tmp21}, TNode<Map>{tmp17});
    }
    if (catch26__label.is_used()) {
      compiler::CodeAssemblerLabel catch26_skip(&ca_);
      ca_.Goto(&catch26_skip);
      ca_.Bind(&catch26__label, &tmp27);
      ca_.Goto(&block21, phi_bb18_11);
      ca_.Bind(&catch26_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch29__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch29__label);
    tmp28 = FromConstexpr_Smi_constexpr_int31_0(state_, PropertyArray::HashField::kMax);
    }
    if (catch29__label.is_used()) {
      compiler::CodeAssemblerLabel catch29_skip(&ca_);
      ca_.Goto(&catch29_skip);
      ca_.Bind(&catch29__label, &tmp30);
      ca_.Goto(&block24, phi_bb18_11, phi_bb18_11, phi_bb18_11);
      ca_.Bind(&catch29_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch32__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch32__label);
    tmp31 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{phi_bb18_11}, TNode<Smi>{tmp28});
    }
    if (catch32__label.is_used()) {
      compiler::CodeAssemblerLabel catch32_skip(&ca_);
      ca_.Goto(&catch32_skip);
      ca_.Bind(&catch32__label, &tmp33);
      ca_.Goto(&block25, phi_bb18_11, phi_bb18_11);
      ca_.Bind(&catch32_skip);
    }
    ca_.Branch(tmp31, &block22, std::vector<compiler::Node*>{phi_bb18_11}, &block23, std::vector<compiler::Node*>{phi_bb18_11});
  }

  TNode<Smi> phi_bb21_11;
  if (block21.is_used()) {
    ca_.Bind(&block21, &phi_bb21_11);
    ca_.Goto(&block15, phi_bb21_11, tmp27);
  }

  TNode<Smi> phi_bb15_11;
  TNode<Object> phi_bb15_14;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_11, &phi_bb15_14);
    ca_.Goto(&block1, phi_bb15_14);
  }

  TNode<Smi> phi_bb24_11;
  TNode<Smi> phi_bb24_14;
  TNode<Smi> phi_bb24_15;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_11, &phi_bb24_14, &phi_bb24_15);
    ca_.Goto(&block8, phi_bb24_11, tmp30);
  }

  TNode<Smi> phi_bb25_11;
  TNode<Smi> phi_bb25_14;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_11, &phi_bb25_14);
    ca_.Goto(&block8, phi_bb25_11, tmp33);
  }

  TNode<Smi> phi_bb22_11;
  TNode<Object> tmp34;
      TNode<Object> tmp36;
      TNode<Object> tmp38;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch35__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch35__label);
    tmp34 = FromConstexpr_Object_constexpr_string_0(state_, "all");
    }
    if (catch35__label.is_used()) {
      compiler::CodeAssemblerLabel catch35_skip(&ca_);
      ca_.Goto(&catch35_skip);
      ca_.Bind(&catch35__label, &tmp36);
      ca_.Goto(&block26, phi_bb22_11);
      ca_.Bind(&catch35_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch37__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch37__label);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{p_context}, MessageTemplate::kTooManyElementsInPromiseCombinator, TNode<Object>{tmp34});
    }
    if (catch37__label.is_used()) {
      compiler::CodeAssemblerLabel catch37_skip(&ca_);
      ca_.Bind(&catch37__label, &tmp38);
      ca_.Goto(&block27, phi_bb22_11);
    }
  }

  TNode<Smi> phi_bb26_11;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_11);
    ca_.Goto(&block8, phi_bb26_11, tmp36);
  }

  TNode<Smi> phi_bb27_11;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_11);
    ca_.Goto(&block8, phi_bb27_11, tmp38);
  }

  TNode<Smi> phi_bb23_11;
  TNode<IntPtrT> tmp39;
  TNode<Object> tmp40;
  TNode<IntPtrT> tmp41;
      TNode<Object> tmp43;
  TNode<Smi> tmp44;
  TNode<Smi> tmp45;
      TNode<Object> tmp47;
  TNode<Smi> tmp48;
      TNode<Object> tmp50;
  TNode<SharedFunctionInfo> tmp51;
  TNode<JSFunction> tmp52;
  TNode<IntPtrT> tmp53;
  TNode<Object> tmp54;
  TNode<JSReceiver> tmp55;
  TNode<Oddball> tmp56;
  TNode<BoolT> tmp57;
      TNode<Object> tmp59;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_11);
    tmp39 = kPromiseAllResolveElementRemainingSlot_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch42__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch42__label);
    std::tie(tmp40, tmp41) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{tmp10}, TNode<IntPtrT>{tmp39}).Flatten();
    }
    if (catch42__label.is_used()) {
      compiler::CodeAssemblerLabel catch42_skip(&ca_);
      ca_.Goto(&catch42_skip);
      ca_.Bind(&catch42__label, &tmp43);
      ca_.Goto(&block28, phi_bb23_11);
      ca_.Bind(&catch42_skip);
    }
    tmp44 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp40, tmp41});
    compiler::CodeAssemblerExceptionHandlerLabel catch46__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch46__label);
    tmp45 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    }
    if (catch46__label.is_used()) {
      compiler::CodeAssemblerLabel catch46_skip(&ca_);
      ca_.Goto(&catch46_skip);
      ca_.Bind(&catch46__label, &tmp47);
      ca_.Goto(&block29, phi_bb23_11);
      ca_.Bind(&catch46_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch49__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch49__label);
    tmp48 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{tmp44}, TNode<Smi>{tmp45});
    }
    if (catch49__label.is_used()) {
      compiler::CodeAssemblerLabel catch49_skip(&ca_);
      ca_.Goto(&catch49_skip);
      ca_.Bind(&catch49__label, &tmp50);
      ca_.Goto(&block30, phi_bb23_11);
      ca_.Bind(&catch49_skip);
    }
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp40, tmp41}, tmp48);
    tmp51 = CodeStubAssembler(state_).PromiseAllResolveElementSharedFunConstant();
    tmp52 = CreatePromiseAllResolveElementFunction_0(state_, TNode<Context>{p_context}, TNode<Context>{tmp10}, TNode<Smi>{phi_bb23_11}, TNode<NativeContext>{p_nativeContext}, TNode<SharedFunctionInfo>{tmp51});
    tmp53 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp54 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{p_capability, tmp53});
    tmp55 = UnsafeCast_Callable_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp54});
    tmp56 = Undefined_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch58__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch58__label);
    tmp57 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{p_promiseResolveFunction}, TNode<HeapObject>{tmp56});
    }
    if (catch58__label.is_used()) {
      compiler::CodeAssemblerLabel catch58_skip(&ca_);
      ca_.Goto(&catch58_skip);
      ca_.Bind(&catch58__label, &tmp59);
      ca_.Goto(&block35, phi_bb23_11);
      ca_.Bind(&catch58_skip);
    }
    ca_.Branch(tmp57, &block36, std::vector<compiler::Node*>{phi_bb23_11}, &block37, std::vector<compiler::Node*>{phi_bb23_11});
  }

  TNode<Smi> phi_bb28_11;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_11);
    ca_.Goto(&block8, phi_bb28_11, tmp43);
  }

  TNode<Smi> phi_bb29_11;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_11);
    ca_.Goto(&block8, phi_bb29_11, tmp47);
  }

  TNode<Smi> phi_bb30_11;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_11);
    ca_.Goto(&block8, phi_bb30_11, tmp50);
  }

  TNode<Smi> phi_bb35_11;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_11);
    ca_.Goto(&block8, phi_bb35_11, tmp59);
  }

  TNode<Smi> phi_bb36_11;
  TNode<BoolT> tmp60;
      TNode<Object> tmp62;
  if (block36.is_used()) {
    ca_.Bind(&block36, &phi_bb36_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch61__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch61__label);
    tmp60 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch61__label.is_used()) {
      compiler::CodeAssemblerLabel catch61_skip(&ca_);
      ca_.Goto(&catch61_skip);
      ca_.Bind(&catch61__label, &tmp62);
      ca_.Goto(&block39, phi_bb36_11);
      ca_.Bind(&catch61_skip);
    }
    ca_.Goto(&block38, phi_bb36_11, tmp60);
  }

  TNode<Smi> phi_bb39_11;
  if (block39.is_used()) {
    ca_.Bind(&block39, &phi_bb39_11);
    ca_.Goto(&block8, phi_bb39_11, tmp62);
  }

  TNode<Smi> phi_bb37_11;
  TNode<BoolT> tmp63;
      TNode<Object> tmp65;
  if (block37.is_used()) {
    ca_.Bind(&block37, &phi_bb37_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch64__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch64__label);
    tmp63 = CodeStubAssembler(state_).NeedsAnyPromiseHooks();
    }
    if (catch64__label.is_used()) {
      compiler::CodeAssemblerLabel catch64_skip(&ca_);
      ca_.Goto(&catch64_skip);
      ca_.Bind(&catch64__label, &tmp65);
      ca_.Goto(&block40, phi_bb37_11);
      ca_.Bind(&catch64_skip);
    }
    ca_.Goto(&block38, phi_bb37_11, tmp63);
  }

  TNode<Smi> phi_bb40_11;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_11);
    ca_.Goto(&block8, phi_bb40_11, tmp65);
  }

  TNode<Smi> phi_bb38_11;
  TNode<BoolT> phi_bb38_17;
  if (block38.is_used()) {
    ca_.Bind(&block38, &phi_bb38_11, &phi_bb38_17);
    ca_.Branch(phi_bb38_17, &block41, std::vector<compiler::Node*>{phi_bb38_11}, &block42, std::vector<compiler::Node*>{phi_bb38_11});
  }

  TNode<Smi> phi_bb41_11;
  TNode<BoolT> tmp66;
      TNode<Object> tmp68;
  if (block41.is_used()) {
    ca_.Bind(&block41, &phi_bb41_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch67__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch67__label);
    tmp66 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch67__label.is_used()) {
      compiler::CodeAssemblerLabel catch67_skip(&ca_);
      ca_.Goto(&catch67_skip);
      ca_.Bind(&catch67__label, &tmp68);
      ca_.Goto(&block44, phi_bb41_11);
      ca_.Bind(&catch67_skip);
    }
    ca_.Goto(&block43, phi_bb41_11, tmp66);
  }

  TNode<Smi> phi_bb44_11;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_11);
    ca_.Goto(&block8, phi_bb44_11, tmp68);
  }

  TNode<Smi> phi_bb42_11;
  TNode<BoolT> tmp69;
      TNode<Object> tmp71;
  if (block42.is_used()) {
    ca_.Bind(&block42, &phi_bb42_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch70__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch70__label);
    tmp69 = CodeStubAssembler(state_).IsPromiseSpeciesProtectorCellInvalid();
    }
    if (catch70__label.is_used()) {
      compiler::CodeAssemblerLabel catch70_skip(&ca_);
      ca_.Goto(&catch70_skip);
      ca_.Bind(&catch70__label, &tmp71);
      ca_.Goto(&block45, phi_bb42_11);
      ca_.Bind(&catch70_skip);
    }
    ca_.Goto(&block43, phi_bb42_11, tmp69);
  }

  TNode<Smi> phi_bb45_11;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_11);
    ca_.Goto(&block8, phi_bb45_11, tmp71);
  }

  TNode<Smi> phi_bb43_11;
  TNode<BoolT> phi_bb43_17;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_11, &phi_bb43_17);
    ca_.Branch(phi_bb43_17, &block46, std::vector<compiler::Node*>{phi_bb43_11}, &block47, std::vector<compiler::Node*>{phi_bb43_11});
  }

  TNode<Smi> phi_bb46_11;
  TNode<BoolT> tmp72;
      TNode<Object> tmp74;
  if (block46.is_used()) {
    ca_.Bind(&block46, &phi_bb46_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch73__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch73__label);
    tmp72 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch73__label.is_used()) {
      compiler::CodeAssemblerLabel catch73_skip(&ca_);
      ca_.Goto(&catch73_skip);
      ca_.Bind(&catch73__label, &tmp74);
      ca_.Goto(&block49, phi_bb46_11);
      ca_.Bind(&catch73_skip);
    }
    ca_.Goto(&block48, phi_bb46_11, tmp72);
  }

  TNode<Smi> phi_bb49_11;
  if (block49.is_used()) {
    ca_.Bind(&block49, &phi_bb49_11);
    ca_.Goto(&block8, phi_bb49_11, tmp74);
  }

  TNode<Smi> phi_bb47_11;
  TNode<BoolT> tmp75;
      TNode<Object> tmp77;
  if (block47.is_used()) {
    ca_.Bind(&block47, &phi_bb47_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch76__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch76__label);
    tmp75 = Is_Smi_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp25});
    }
    if (catch76__label.is_used()) {
      compiler::CodeAssemblerLabel catch76_skip(&ca_);
      ca_.Goto(&catch76_skip);
      ca_.Bind(&catch76__label, &tmp77);
      ca_.Goto(&block50, phi_bb47_11);
      ca_.Bind(&catch76_skip);
    }
    ca_.Goto(&block48, phi_bb47_11, tmp75);
  }

  TNode<Smi> phi_bb50_11;
  if (block50.is_used()) {
    ca_.Bind(&block50, &phi_bb50_11);
    ca_.Goto(&block8, phi_bb50_11, tmp77);
  }

  TNode<Smi> phi_bb48_11;
  TNode<BoolT> phi_bb48_17;
  if (block48.is_used()) {
    ca_.Bind(&block48, &phi_bb48_11, &phi_bb48_17);
    ca_.Branch(phi_bb48_17, &block51, std::vector<compiler::Node*>{phi_bb48_11}, &block52, std::vector<compiler::Node*>{phi_bb48_11});
  }

  TNode<Smi> phi_bb51_11;
  TNode<BoolT> tmp78;
      TNode<Object> tmp80;
  if (block51.is_used()) {
    ca_.Bind(&block51, &phi_bb51_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch79__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch79__label);
    tmp78 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch79__label.is_used()) {
      compiler::CodeAssemblerLabel catch79_skip(&ca_);
      ca_.Goto(&catch79_skip);
      ca_.Bind(&catch79__label, &tmp80);
      ca_.Goto(&block54, phi_bb51_11);
      ca_.Bind(&catch79_skip);
    }
    ca_.Goto(&block53, phi_bb51_11, tmp78);
  }

  TNode<Smi> phi_bb54_11;
  if (block54.is_used()) {
    ca_.Bind(&block54, &phi_bb54_11);
    ca_.Goto(&block8, phi_bb54_11, tmp80);
  }

  TNode<Smi> phi_bb52_11;
  TNode<HeapObject> tmp81;
      TNode<Object> tmp83;
  TNode<IntPtrT> tmp84;
      TNode<Object> tmp86;
  TNode<Map> tmp87;
  TNode<BoolT> tmp88;
      TNode<Object> tmp90;
  TNode<BoolT> tmp91;
      TNode<Object> tmp93;
  if (block52.is_used()) {
    ca_.Bind(&block52, &phi_bb52_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch82__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch82__label);
    tmp81 = UnsafeCast_HeapObject_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp25});
    }
    if (catch82__label.is_used()) {
      compiler::CodeAssemblerLabel catch82_skip(&ca_);
      ca_.Goto(&catch82_skip);
      ca_.Bind(&catch82__label, &tmp83);
      ca_.Goto(&block55, phi_bb52_11);
      ca_.Bind(&catch82_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch85__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch85__label);
    tmp84 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    }
    if (catch85__label.is_used()) {
      compiler::CodeAssemblerLabel catch85_skip(&ca_);
      ca_.Goto(&catch85_skip);
      ca_.Bind(&catch85__label, &tmp86);
      ca_.Goto(&block56, phi_bb52_11);
      ca_.Bind(&catch85_skip);
    }
    tmp87 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp81, tmp84});
    compiler::CodeAssemblerExceptionHandlerLabel catch89__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch89__label);
    tmp88 = IsPromiseThenLookupChainIntact_0(state_, TNode<Context>{p_context}, TNode<NativeContext>{p_nativeContext}, TNode<Map>{tmp87});
    }
    if (catch89__label.is_used()) {
      compiler::CodeAssemblerLabel catch89_skip(&ca_);
      ca_.Goto(&catch89_skip);
      ca_.Bind(&catch89__label, &tmp90);
      ca_.Goto(&block57, phi_bb52_11);
      ca_.Bind(&catch89_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch92__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch92__label);
    tmp91 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp88});
    }
    if (catch92__label.is_used()) {
      compiler::CodeAssemblerLabel catch92_skip(&ca_);
      ca_.Goto(&catch92_skip);
      ca_.Bind(&catch92__label, &tmp93);
      ca_.Goto(&block58, phi_bb52_11);
      ca_.Bind(&catch92_skip);
    }
    ca_.Goto(&block53, phi_bb52_11, tmp91);
  }

  TNode<Smi> phi_bb55_11;
  if (block55.is_used()) {
    ca_.Bind(&block55, &phi_bb55_11);
    ca_.Goto(&block8, phi_bb55_11, tmp83);
  }

  TNode<Smi> phi_bb56_11;
  if (block56.is_used()) {
    ca_.Bind(&block56, &phi_bb56_11);
    ca_.Goto(&block8, phi_bb56_11, tmp86);
  }

  TNode<Smi> phi_bb57_11;
  if (block57.is_used()) {
    ca_.Bind(&block57, &phi_bb57_11);
    ca_.Goto(&block8, phi_bb57_11, tmp90);
  }

  TNode<Smi> phi_bb58_11;
  if (block58.is_used()) {
    ca_.Bind(&block58, &phi_bb58_11);
    ca_.Goto(&block8, phi_bb58_11, tmp93);
  }

  TNode<Smi> phi_bb53_11;
  TNode<BoolT> phi_bb53_17;
  if (block53.is_used()) {
    ca_.Bind(&block53, &phi_bb53_11, &phi_bb53_17);
    ca_.Branch(phi_bb53_17, &block33, std::vector<compiler::Node*>{phi_bb53_11}, &block34, std::vector<compiler::Node*>{phi_bb53_11});
  }

  TNode<Smi> phi_bb33_11;
  TNode<Object> tmp94;
      TNode<Object> tmp96;
  TNode<String> tmp97;
  TNode<Object> tmp98;
      TNode<Object> tmp100;
  TNode<Object> tmp101;
      TNode<Object> tmp103;
  TNode<BoolT> tmp104;
      TNode<Object> tmp106;
  if (block33.is_used()) {
    ca_.Bind(&block33, &phi_bb33_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch95__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch95__label);
    tmp94 = CallResolve_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_constructor}, TNode<Object>{p_promiseResolveFunction}, TNode<Object>{tmp25});
    }
    if (catch95__label.is_used()) {
      compiler::CodeAssemblerLabel catch95_skip(&ca_);
      ca_.Goto(&catch95_skip);
      ca_.Bind(&catch95__label, &tmp96);
      ca_.Goto(&block60, phi_bb33_11);
      ca_.Bind(&catch95_skip);
    }
    tmp97 = kThenString_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch99__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch99__label);
    tmp98 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{tmp94}, TNode<Object>{tmp97});
    }
    if (catch99__label.is_used()) {
      compiler::CodeAssemblerLabel catch99_skip(&ca_);
      ca_.Goto(&catch99_skip);
      ca_.Bind(&catch99__label, &tmp100);
      ca_.Goto(&block61, phi_bb33_11);
      ca_.Bind(&catch99_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch102__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch102__label);
    tmp101 = CodeStubAssembler(state_).Call(TNode<Context>{p_nativeContext}, TNode<Object>{tmp98}, TNode<Object>{tmp94}, TNode<Object>{tmp52}, TNode<Object>{tmp55});
    }
    if (catch102__label.is_used()) {
      compiler::CodeAssemblerLabel catch102_skip(&ca_);
      ca_.Goto(&catch102_skip);
      ca_.Bind(&catch102__label, &tmp103);
      ca_.Goto(&block62, phi_bb33_11);
      ca_.Bind(&catch102_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch105__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch105__label);
    tmp104 = CodeStubAssembler(state_).IsDebugActive();
    }
    if (catch105__label.is_used()) {
      compiler::CodeAssemblerLabel catch105_skip(&ca_);
      ca_.Goto(&catch105_skip);
      ca_.Bind(&catch105__label, &tmp106);
      ca_.Goto(&block65, phi_bb33_11);
      ca_.Bind(&catch105_skip);
    }
    ca_.Branch(tmp104, &block66, std::vector<compiler::Node*>{phi_bb33_11}, &block67, std::vector<compiler::Node*>{phi_bb33_11});
  }

  TNode<Smi> phi_bb60_11;
  if (block60.is_used()) {
    ca_.Bind(&block60, &phi_bb60_11);
    ca_.Goto(&block8, phi_bb60_11, tmp96);
  }

  TNode<Smi> phi_bb61_11;
  if (block61.is_used()) {
    ca_.Bind(&block61, &phi_bb61_11);
    ca_.Goto(&block8, phi_bb61_11, tmp100);
  }

  TNode<Smi> phi_bb62_11;
  if (block62.is_used()) {
    ca_.Bind(&block62, &phi_bb62_11);
    ca_.Goto(&block8, phi_bb62_11, tmp103);
  }

  TNode<Smi> phi_bb65_11;
  if (block65.is_used()) {
    ca_.Bind(&block65, &phi_bb65_11);
    ca_.Goto(&block8, phi_bb65_11, tmp106);
  }

  TNode<Smi> phi_bb66_11;
  TNode<BoolT> tmp107;
      TNode<Object> tmp109;
  if (block66.is_used()) {
    ca_.Bind(&block66, &phi_bb66_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch108__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch108__label);
    tmp107 = Is_JSPromise_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp101});
    }
    if (catch108__label.is_used()) {
      compiler::CodeAssemblerLabel catch108_skip(&ca_);
      ca_.Goto(&catch108_skip);
      ca_.Bind(&catch108__label, &tmp109);
      ca_.Goto(&block69, phi_bb66_11);
      ca_.Bind(&catch108_skip);
    }
    ca_.Goto(&block68, phi_bb66_11, tmp107);
  }

  TNode<Smi> phi_bb69_11;
  if (block69.is_used()) {
    ca_.Bind(&block69, &phi_bb69_11);
    ca_.Goto(&block8, phi_bb69_11, tmp109);
  }

  TNode<Smi> phi_bb67_11;
  TNode<BoolT> tmp110;
      TNode<Object> tmp112;
  if (block67.is_used()) {
    ca_.Bind(&block67, &phi_bb67_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch111__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch111__label);
    tmp110 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    }
    if (catch111__label.is_used()) {
      compiler::CodeAssemblerLabel catch111_skip(&ca_);
      ca_.Goto(&catch111_skip);
      ca_.Bind(&catch111__label, &tmp112);
      ca_.Goto(&block70, phi_bb67_11);
      ca_.Bind(&catch111_skip);
    }
    ca_.Goto(&block68, phi_bb67_11, tmp110);
  }

  TNode<Smi> phi_bb70_11;
  if (block70.is_used()) {
    ca_.Bind(&block70, &phi_bb70_11);
    ca_.Goto(&block8, phi_bb70_11, tmp112);
  }

  TNode<Smi> phi_bb68_11;
  TNode<BoolT> phi_bb68_20;
  if (block68.is_used()) {
    ca_.Bind(&block68, &phi_bb68_11, &phi_bb68_20);
    ca_.Branch(phi_bb68_20, &block63, std::vector<compiler::Node*>{phi_bb68_11}, &block64, std::vector<compiler::Node*>{phi_bb68_11});
  }

  TNode<Smi> phi_bb63_11;
  TNode<Symbol> tmp113;
  TNode<Object> tmp114;
      TNode<Object> tmp116;
  if (block63.is_used()) {
    ca_.Bind(&block63, &phi_bb63_11);
    tmp113 = kPromiseHandledBySymbol_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch115__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch115__label);
    tmp114 = CodeStubAssembler(state_).SetPropertyStrict(TNode<Context>{p_context}, TNode<Object>{tmp101}, TNode<Object>{tmp113}, TNode<Object>{tmp1});
    }
    if (catch115__label.is_used()) {
      compiler::CodeAssemblerLabel catch115_skip(&ca_);
      ca_.Goto(&catch115_skip);
      ca_.Bind(&catch115__label, &tmp116);
      ca_.Goto(&block71, phi_bb63_11);
      ca_.Bind(&catch115_skip);
    }
    ca_.Goto(&block64, phi_bb63_11);
  }

  TNode<Smi> phi_bb71_11;
  if (block71.is_used()) {
    ca_.Bind(&block71, &phi_bb71_11);
    ca_.Goto(&block8, phi_bb71_11, tmp116);
  }

  TNode<Smi> phi_bb64_11;
  if (block64.is_used()) {
    ca_.Bind(&block64, &phi_bb64_11);
    ca_.Goto(&block59, phi_bb64_11);
  }

  TNode<Smi> phi_bb34_11;
  TNode<JSPromise> tmp117;
      TNode<Object> tmp119;
  TNode<Oddball> tmp120;
      TNode<Object> tmp122;
  if (block34.is_used()) {
    ca_.Bind(&block34, &phi_bb34_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch118__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch118__label);
    tmp117 = UnsafeCast_JSPromise_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp25});
    }
    if (catch118__label.is_used()) {
      compiler::CodeAssemblerLabel catch118_skip(&ca_);
      ca_.Goto(&catch118_skip);
      ca_.Bind(&catch118__label, &tmp119);
      ca_.Goto(&block72, phi_bb34_11);
      ca_.Bind(&catch118_skip);
    }
    tmp120 = Undefined_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch121__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch121__label);
    PerformPromiseThenImpl_0(state_, TNode<Context>{p_context}, TNode<JSPromise>{tmp117}, TNode<HeapObject>{tmp52}, TNode<HeapObject>{tmp55}, TNode<HeapObject>{tmp120});
    }
    if (catch121__label.is_used()) {
      compiler::CodeAssemblerLabel catch121_skip(&ca_);
      ca_.Goto(&catch121_skip);
      ca_.Bind(&catch121__label, &tmp122);
      ca_.Goto(&block73, phi_bb34_11);
      ca_.Bind(&catch121_skip);
    }
    ca_.Goto(&block59, phi_bb34_11);
  }

  TNode<Smi> phi_bb72_11;
  if (block72.is_used()) {
    ca_.Bind(&block72, &phi_bb72_11);
    ca_.Goto(&block8, phi_bb72_11, tmp119);
  }

  TNode<Smi> phi_bb73_11;
  if (block73.is_used()) {
    ca_.Bind(&block73, &phi_bb73_11);
    ca_.Goto(&block8, phi_bb73_11, tmp122);
  }

  TNode<Smi> phi_bb59_11;
  TNode<Smi> tmp123;
      TNode<Object> tmp125;
  TNode<Smi> tmp126;
      TNode<Object> tmp128;
  if (block59.is_used()) {
    ca_.Bind(&block59, &phi_bb59_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch124__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch124__label);
    tmp123 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    }
    if (catch124__label.is_used()) {
      compiler::CodeAssemblerLabel catch124_skip(&ca_);
      ca_.Goto(&catch124_skip);
      ca_.Bind(&catch124__label, &tmp125);
      ca_.Goto(&block74, phi_bb59_11, phi_bb59_11, phi_bb59_11);
      ca_.Bind(&catch124_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch127__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch127__label);
    tmp126 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb59_11}, TNode<Smi>{tmp123});
    }
    if (catch127__label.is_used()) {
      compiler::CodeAssemblerLabel catch127_skip(&ca_);
      ca_.Goto(&catch127_skip);
      ca_.Bind(&catch127__label, &tmp128);
      ca_.Goto(&block75, phi_bb59_11, phi_bb59_11);
      ca_.Bind(&catch127_skip);
    }
    ca_.Goto(&block12, tmp126);
  }

  TNode<Smi> phi_bb74_11;
  TNode<Smi> phi_bb74_16;
  TNode<Smi> phi_bb74_17;
  if (block74.is_used()) {
    ca_.Bind(&block74, &phi_bb74_11, &phi_bb74_16, &phi_bb74_17);
    ca_.Goto(&block8, phi_bb74_11, tmp125);
  }

  TNode<Smi> phi_bb75_11;
  TNode<Smi> phi_bb75_16;
  if (block75.is_used()) {
    ca_.Bind(&block75, &phi_bb75_11, &phi_bb75_16);
    ca_.Goto(&block8, phi_bb75_11, tmp128);
  }

  TNode<Smi> phi_bb11_11;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_11);
    ca_.Goto(&block5, phi_bb11_11);
  }

  TNode<Smi> phi_bb8_11;
  TNode<Object> phi_bb8_12;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_11, &phi_bb8_12);
    IteratorCloseOnException_0(state_, TNode<Context>{p_context}, TorqueStructIteratorRecord{TNode<JSReceiver>{p_iter.object}, TNode<Object>{p_iter.next}});
    ca_.Goto(&block1, phi_bb8_12);
  }

  TNode<Smi> phi_bb5_11;
  TNode<IntPtrT> tmp129;
  TNode<Object> tmp130;
  TNode<IntPtrT> tmp131;
  TNode<Smi> tmp132;
  TNode<Smi> tmp133;
  TNode<Smi> tmp134;
  TNode<Smi> tmp135;
  TNode<BoolT> tmp136;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_11);
    tmp129 = kPromiseAllResolveElementRemainingSlot_0(state_);
    std::tie(tmp130, tmp131) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{tmp10}, TNode<IntPtrT>{tmp129}).Flatten();
    tmp132 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp130, tmp131});
    tmp133 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp134 = CodeStubAssembler(state_).SmiSub(TNode<Smi>{tmp132}, TNode<Smi>{tmp133});
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp130, tmp131}, tmp134);
    tmp135 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp136 = CodeStubAssembler(state_).SmiGreaterThanOrEqual(TNode<Smi>{tmp134}, TNode<Smi>{tmp135});
    ca_.Branch(tmp136, &block76, std::vector<compiler::Node*>{phi_bb5_11}, &block77, std::vector<compiler::Node*>{phi_bb5_11});
  }

  TNode<Smi> phi_bb77_11;
  if (block77.is_used()) {
    ca_.Bind(&block77, &phi_bb77_11);
    {
      auto pos_stack = ca_.GetMacroSourcePositionStack();
      pos_stack.push_back({"src/builtins/promise-all.tq", 278});
      CodeStubAssembler(state_).FailAssert("Torque assert 'remainingElementsCount >= 0' failed", pos_stack);
    }
  }

  TNode<Smi> phi_bb76_11;
  TNode<Smi> tmp137;
  TNode<BoolT> tmp138;
  if (block76.is_used()) {
    ca_.Bind(&block76, &phi_bb76_11);
    tmp137 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp138 = CodeStubAssembler(state_).SmiGreaterThan(TNode<Smi>{tmp134}, TNode<Smi>{tmp137});
    ca_.Branch(tmp138, &block78, std::vector<compiler::Node*>{phi_bb76_11}, &block79, std::vector<compiler::Node*>{phi_bb76_11});
  }

  TNode<Smi> phi_bb78_11;
  TNode<IntPtrT> tmp139;
  TNode<Object> tmp140;
  TNode<IntPtrT> tmp141;
  TNode<FixedArray> tmp142;
  TNode<IntPtrT> tmp143;
  TNode<IntPtrT> tmp144;
  TNode<IntPtrT> tmp145;
  TNode<IntPtrT> tmp146;
  TNode<BoolT> tmp147;
  if (block78.is_used()) {
    ca_.Bind(&block78, &phi_bb78_11);
    tmp139 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp140, tmp141) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{tmp10}, TNode<IntPtrT>{tmp139}).Flatten();
    tmp142 = CodeStubAssembler(state_).LoadReference<FixedArray>(CodeStubAssembler::Reference{tmp140, tmp141});
    tmp143 = CodeStubAssembler(state_).SmiUntag(TNode<Smi>{phi_bb78_11});
    tmp144 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp145 = CodeStubAssembler(state_).IntPtrSub(TNode<IntPtrT>{tmp143}, TNode<IntPtrT>{tmp144});
    tmp146 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp142});
    tmp147 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{tmp146}, TNode<IntPtrT>{tmp145});
    ca_.Branch(tmp147, &block81, std::vector<compiler::Node*>{phi_bb78_11}, &block82, std::vector<compiler::Node*>{phi_bb78_11});
  }

  TNode<Smi> phi_bb81_11;
  TNode<IntPtrT> tmp148;
  TNode<FixedArray> tmp149;
  if (block81.is_used()) {
    ca_.Bind(&block81, &phi_bb81_11);
    tmp148 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp149 = ExtractFixedArray_0(state_, TNode<FixedArray>{tmp142}, TNode<IntPtrT>{tmp148}, TNode<IntPtrT>{tmp146}, TNode<IntPtrT>{tmp145});
    CodeStubAssembler(state_).StoreReference<FixedArray>(CodeStubAssembler::Reference{tmp140, tmp141}, tmp149);
    ca_.Goto(&block82, phi_bb81_11);
  }

  TNode<Smi> phi_bb82_11;
  if (block82.is_used()) {
    ca_.Bind(&block82, &phi_bb82_11);
    ca_.Goto(&block80, phi_bb82_11);
  }

  TNode<Smi> phi_bb79_11;
  TNode<IntPtrT> tmp150;
  TNode<Object> tmp151;
  TNode<IntPtrT> tmp152;
  TNode<FixedArray> tmp153;
  TNode<IntPtrT> tmp154;
  TNode<Object> tmp155;
  TNode<IntPtrT> tmp156;
  TNode<Map> tmp157;
  TNode<JSArray> tmp158;
  TNode<Object> tmp159;
  TNode<Oddball> tmp160;
  TNode<Object> tmp161;
  if (block79.is_used()) {
    ca_.Bind(&block79, &phi_bb79_11);
    tmp150 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp151, tmp152) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{tmp10}, TNode<IntPtrT>{tmp150}).Flatten();
    tmp153 = CodeStubAssembler(state_).LoadReference<FixedArray>(CodeStubAssembler::Reference{tmp151, tmp152});
    tmp154 = JS_ARRAY_PACKED_ELEMENTS_MAP_INDEX_0(state_);
    std::tie(tmp155, tmp156) = NativeContextSlot_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp154}).Flatten();
    tmp157 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp155, tmp156});
    tmp158 = NewJSArray_0(state_, TNode<Context>{p_context}, TNode<Map>{tmp157}, TNode<FixedArrayBase>{tmp153});
    tmp159 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp3});
    tmp160 = Undefined_0(state_);
    tmp161 = CodeStubAssembler(state_).Call(TNode<Context>{p_nativeContext}, TNode<Object>{tmp159}, TNode<Object>{tmp160}, TNode<Object>{tmp158});
    ca_.Goto(&block80, phi_bb79_11);
  }

  TNode<Smi> phi_bb80_11;
  if (block80.is_used()) {
    ca_.Bind(&block80, &phi_bb80_11);
    ca_.Goto(&block83);
  }

  TNode<Object> phi_bb1_0;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_0);
    *label_Reject_parameter_0 = phi_bb1_0;
    ca_.Goto(label_Reject);
  }

    ca_.Bind(&block83);
  return TNode<Object>{tmp1};
}


TNode<Object> PerformPromiseAll_PromiseAllSettledResolveElementFunctor_PromiseAllSettledRejectElementFunctor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TorqueStructIteratorRecord p_iter, TNode<JSReceiver> p_constructor, TNode<PromiseCapability> p_capability, TNode<Object> p_promiseResolveFunction, TorqueStructPromiseAllSettledResolveElementFunctor_0 p_createResolveElementFunctor, TorqueStructPromiseAllSettledRejectElementFunctor_0 p_createRejectElementFunctor, compiler::CodeAssemblerLabel* label_Reject, compiler::TypedCodeAssemblerVariable<Object>* label_Reject_parameter_0) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block13(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block20(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block21(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Object> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi, Smi> block24(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi> block25(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block26(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block27(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block28(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block29(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block30(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block35(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block39(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block40(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block44(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block45(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block49(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block50(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block48(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block51(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block54(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block52(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block55(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block56(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block57(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block58(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block53(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block60(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block61(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block62(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block65(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block66(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block69(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block67(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block70(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, BoolT> block68(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block63(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block71(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block64(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block72(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block73(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block59(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi, Smi> block74(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi> block75(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Object> block8(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block77(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block76(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block78(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block81(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block82(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block79(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block80(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block83(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<HeapObject> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<Object> tmp5;
  TNode<BoolT> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp1 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{p_capability, tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp3 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{p_capability, tmp2});
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp5 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{p_capability, tmp4});
    tmp6 = CodeStubAssembler(state_).IsDebugActive();
    ca_.Branch(tmp6, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  TNode<Symbol> tmp7;
  TNode<Oddball> tmp8;
  TNode<Object> tmp9;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp7 = kPromiseForwardingHandlerSymbol_0(state_);
    tmp8 = True_0(state_);
    tmp9 = CodeStubAssembler(state_).SetPropertyStrict(TNode<Context>{p_context}, TNode<Object>{tmp5}, TNode<Object>{tmp7}, TNode<Object>{tmp8});
    ca_.Goto(&block4);
  }

  TNode<Context> tmp10;
  TNode<Smi> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  TNode<IntPtrT> tmp14;
      TNode<Object> tmp16;
  TNode<Map> tmp17;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp10 = CreatePromiseAllResolveElementContext_0(state_, TNode<Context>{p_context}, TNode<PromiseCapability>{p_capability}, TNode<NativeContext>{p_nativeContext});
    tmp11 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp12 = ITERATOR_RESULT_MAP_INDEX_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch15__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch15__label);
    std::tie(tmp13, tmp14) = NativeContextSlot_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp12}).Flatten();
    }
    if (catch15__label.is_used()) {
      compiler::CodeAssemblerLabel catch15_skip(&ca_);
      ca_.Goto(&catch15_skip);
      ca_.Bind(&catch15__label, &tmp16);
      ca_.Goto(&block9);
      ca_.Bind(&catch15_skip);
    }
    tmp17 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp13, tmp14});
    ca_.Goto(&block12, tmp11);
  }

  if (block9.is_used()) {
    ca_.Bind(&block9);
    ca_.Goto(&block8, tmp11, tmp16);
  }

  TNode<Smi> phi_bb12_11;
  TNode<BoolT> tmp18;
      TNode<Object> tmp20;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch19__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch19__label);
    tmp18 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch19__label.is_used()) {
      compiler::CodeAssemblerLabel catch19_skip(&ca_);
      ca_.Goto(&catch19_skip);
      ca_.Bind(&catch19__label, &tmp20);
      ca_.Goto(&block13, phi_bb12_11);
      ca_.Bind(&catch19_skip);
    }
    ca_.Branch(tmp18, &block10, std::vector<compiler::Node*>{phi_bb12_11}, &block11, std::vector<compiler::Node*>{phi_bb12_11});
  }

  TNode<Smi> phi_bb13_11;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_11);
    ca_.Goto(&block8, phi_bb13_11, tmp20);
  }

  TNode<Smi> phi_bb10_11;
  TNode<JSReceiver> tmp21;
      TNode<Object> tmp24;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_11);
    compiler::CodeAssemblerLabel label22(&ca_);
    compiler::CodeAssemblerExceptionHandlerLabel catch23__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch23__label);
    tmp21 = IteratorBuiltinsAssembler(state_).IteratorStep(TNode<Context>{p_context}, TorqueStructIteratorRecord{TNode<JSReceiver>{p_iter.object}, TNode<Object>{p_iter.next}}, TNode<Map>{tmp17}, &label22);
    }
    if (catch23__label.is_used()) {
      compiler::CodeAssemblerLabel catch23_skip(&ca_);
      ca_.Goto(&catch23_skip);
      ca_.Bind(&catch23__label, &tmp24);
      ca_.Goto(&block20, phi_bb10_11);
      ca_.Bind(&catch23_skip);
    }
    ca_.Goto(&block18, phi_bb10_11);
    if (label22.is_used()) {
      ca_.Bind(&label22);
      ca_.Goto(&block19, phi_bb10_11);
    }
  }

  TNode<Smi> phi_bb20_11;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_11);
    ca_.Goto(&block15, phi_bb20_11, tmp24);
  }

  TNode<Smi> phi_bb19_11;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_11);
    ca_.Goto(&block5, phi_bb19_11);
  }

  TNode<Smi> phi_bb18_11;
  TNode<Object> tmp25;
      TNode<Object> tmp27;
  TNode<Smi> tmp28;
      TNode<Object> tmp30;
  TNode<BoolT> tmp31;
      TNode<Object> tmp33;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch26__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch26__label);
    tmp25 = IteratorBuiltinsAssembler(state_).IteratorValue(TNode<Context>{p_context}, TNode<JSReceiver>{tmp21}, TNode<Map>{tmp17});
    }
    if (catch26__label.is_used()) {
      compiler::CodeAssemblerLabel catch26_skip(&ca_);
      ca_.Goto(&catch26_skip);
      ca_.Bind(&catch26__label, &tmp27);
      ca_.Goto(&block21, phi_bb18_11);
      ca_.Bind(&catch26_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch29__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch29__label);
    tmp28 = FromConstexpr_Smi_constexpr_int31_0(state_, PropertyArray::HashField::kMax);
    }
    if (catch29__label.is_used()) {
      compiler::CodeAssemblerLabel catch29_skip(&ca_);
      ca_.Goto(&catch29_skip);
      ca_.Bind(&catch29__label, &tmp30);
      ca_.Goto(&block24, phi_bb18_11, phi_bb18_11, phi_bb18_11);
      ca_.Bind(&catch29_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch32__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch32__label);
    tmp31 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{phi_bb18_11}, TNode<Smi>{tmp28});
    }
    if (catch32__label.is_used()) {
      compiler::CodeAssemblerLabel catch32_skip(&ca_);
      ca_.Goto(&catch32_skip);
      ca_.Bind(&catch32__label, &tmp33);
      ca_.Goto(&block25, phi_bb18_11, phi_bb18_11);
      ca_.Bind(&catch32_skip);
    }
    ca_.Branch(tmp31, &block22, std::vector<compiler::Node*>{phi_bb18_11}, &block23, std::vector<compiler::Node*>{phi_bb18_11});
  }

  TNode<Smi> phi_bb21_11;
  if (block21.is_used()) {
    ca_.Bind(&block21, &phi_bb21_11);
    ca_.Goto(&block15, phi_bb21_11, tmp27);
  }

  TNode<Smi> phi_bb15_11;
  TNode<Object> phi_bb15_14;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_11, &phi_bb15_14);
    ca_.Goto(&block1, phi_bb15_14);
  }

  TNode<Smi> phi_bb24_11;
  TNode<Smi> phi_bb24_14;
  TNode<Smi> phi_bb24_15;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_11, &phi_bb24_14, &phi_bb24_15);
    ca_.Goto(&block8, phi_bb24_11, tmp30);
  }

  TNode<Smi> phi_bb25_11;
  TNode<Smi> phi_bb25_14;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_11, &phi_bb25_14);
    ca_.Goto(&block8, phi_bb25_11, tmp33);
  }

  TNode<Smi> phi_bb22_11;
  TNode<Object> tmp34;
      TNode<Object> tmp36;
      TNode<Object> tmp38;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch35__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch35__label);
    tmp34 = FromConstexpr_Object_constexpr_string_0(state_, "all");
    }
    if (catch35__label.is_used()) {
      compiler::CodeAssemblerLabel catch35_skip(&ca_);
      ca_.Goto(&catch35_skip);
      ca_.Bind(&catch35__label, &tmp36);
      ca_.Goto(&block26, phi_bb22_11);
      ca_.Bind(&catch35_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch37__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch37__label);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{p_context}, MessageTemplate::kTooManyElementsInPromiseCombinator, TNode<Object>{tmp34});
    }
    if (catch37__label.is_used()) {
      compiler::CodeAssemblerLabel catch37_skip(&ca_);
      ca_.Bind(&catch37__label, &tmp38);
      ca_.Goto(&block27, phi_bb22_11);
    }
  }

  TNode<Smi> phi_bb26_11;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_11);
    ca_.Goto(&block8, phi_bb26_11, tmp36);
  }

  TNode<Smi> phi_bb27_11;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_11);
    ca_.Goto(&block8, phi_bb27_11, tmp38);
  }

  TNode<Smi> phi_bb23_11;
  TNode<IntPtrT> tmp39;
  TNode<Object> tmp40;
  TNode<IntPtrT> tmp41;
      TNode<Object> tmp43;
  TNode<Smi> tmp44;
  TNode<Smi> tmp45;
      TNode<Object> tmp47;
  TNode<Smi> tmp48;
      TNode<Object> tmp50;
  TNode<SharedFunctionInfo> tmp51;
  TNode<JSFunction> tmp52;
  TNode<SharedFunctionInfo> tmp53;
  TNode<JSFunction> tmp54;
  TNode<Oddball> tmp55;
  TNode<BoolT> tmp56;
      TNode<Object> tmp58;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_11);
    tmp39 = kPromiseAllResolveElementRemainingSlot_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch42__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch42__label);
    std::tie(tmp40, tmp41) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{tmp10}, TNode<IntPtrT>{tmp39}).Flatten();
    }
    if (catch42__label.is_used()) {
      compiler::CodeAssemblerLabel catch42_skip(&ca_);
      ca_.Goto(&catch42_skip);
      ca_.Bind(&catch42__label, &tmp43);
      ca_.Goto(&block28, phi_bb23_11);
      ca_.Bind(&catch42_skip);
    }
    tmp44 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp40, tmp41});
    compiler::CodeAssemblerExceptionHandlerLabel catch46__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch46__label);
    tmp45 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    }
    if (catch46__label.is_used()) {
      compiler::CodeAssemblerLabel catch46_skip(&ca_);
      ca_.Goto(&catch46_skip);
      ca_.Bind(&catch46__label, &tmp47);
      ca_.Goto(&block29, phi_bb23_11);
      ca_.Bind(&catch46_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch49__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch49__label);
    tmp48 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{tmp44}, TNode<Smi>{tmp45});
    }
    if (catch49__label.is_used()) {
      compiler::CodeAssemblerLabel catch49_skip(&ca_);
      ca_.Goto(&catch49_skip);
      ca_.Bind(&catch49__label, &tmp50);
      ca_.Goto(&block30, phi_bb23_11);
      ca_.Bind(&catch49_skip);
    }
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp40, tmp41}, tmp48);
    tmp51 = CodeStubAssembler(state_).PromiseAllSettledResolveElementSharedFunConstant();
    tmp52 = CreatePromiseAllResolveElementFunction_0(state_, TNode<Context>{p_context}, TNode<Context>{tmp10}, TNode<Smi>{phi_bb23_11}, TNode<NativeContext>{p_nativeContext}, TNode<SharedFunctionInfo>{tmp51});
    tmp53 = CodeStubAssembler(state_).PromiseAllSettledRejectElementSharedFunConstant();
    tmp54 = CreatePromiseAllResolveElementFunction_0(state_, TNode<Context>{p_context}, TNode<Context>{tmp10}, TNode<Smi>{phi_bb23_11}, TNode<NativeContext>{p_nativeContext}, TNode<SharedFunctionInfo>{tmp53});
    tmp55 = Undefined_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch57__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch57__label);
    tmp56 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{p_promiseResolveFunction}, TNode<HeapObject>{tmp55});
    }
    if (catch57__label.is_used()) {
      compiler::CodeAssemblerLabel catch57_skip(&ca_);
      ca_.Goto(&catch57_skip);
      ca_.Bind(&catch57__label, &tmp58);
      ca_.Goto(&block35, phi_bb23_11);
      ca_.Bind(&catch57_skip);
    }
    ca_.Branch(tmp56, &block36, std::vector<compiler::Node*>{phi_bb23_11}, &block37, std::vector<compiler::Node*>{phi_bb23_11});
  }

  TNode<Smi> phi_bb28_11;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_11);
    ca_.Goto(&block8, phi_bb28_11, tmp43);
  }

  TNode<Smi> phi_bb29_11;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_11);
    ca_.Goto(&block8, phi_bb29_11, tmp47);
  }

  TNode<Smi> phi_bb30_11;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_11);
    ca_.Goto(&block8, phi_bb30_11, tmp50);
  }

  TNode<Smi> phi_bb35_11;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_11);
    ca_.Goto(&block8, phi_bb35_11, tmp58);
  }

  TNode<Smi> phi_bb36_11;
  TNode<BoolT> tmp59;
      TNode<Object> tmp61;
  if (block36.is_used()) {
    ca_.Bind(&block36, &phi_bb36_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch60__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch60__label);
    tmp59 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch60__label.is_used()) {
      compiler::CodeAssemblerLabel catch60_skip(&ca_);
      ca_.Goto(&catch60_skip);
      ca_.Bind(&catch60__label, &tmp61);
      ca_.Goto(&block39, phi_bb36_11);
      ca_.Bind(&catch60_skip);
    }
    ca_.Goto(&block38, phi_bb36_11, tmp59);
  }

  TNode<Smi> phi_bb39_11;
  if (block39.is_used()) {
    ca_.Bind(&block39, &phi_bb39_11);
    ca_.Goto(&block8, phi_bb39_11, tmp61);
  }

  TNode<Smi> phi_bb37_11;
  TNode<BoolT> tmp62;
      TNode<Object> tmp64;
  if (block37.is_used()) {
    ca_.Bind(&block37, &phi_bb37_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch63__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch63__label);
    tmp62 = CodeStubAssembler(state_).NeedsAnyPromiseHooks();
    }
    if (catch63__label.is_used()) {
      compiler::CodeAssemblerLabel catch63_skip(&ca_);
      ca_.Goto(&catch63_skip);
      ca_.Bind(&catch63__label, &tmp64);
      ca_.Goto(&block40, phi_bb37_11);
      ca_.Bind(&catch63_skip);
    }
    ca_.Goto(&block38, phi_bb37_11, tmp62);
  }

  TNode<Smi> phi_bb40_11;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_11);
    ca_.Goto(&block8, phi_bb40_11, tmp64);
  }

  TNode<Smi> phi_bb38_11;
  TNode<BoolT> phi_bb38_17;
  if (block38.is_used()) {
    ca_.Bind(&block38, &phi_bb38_11, &phi_bb38_17);
    ca_.Branch(phi_bb38_17, &block41, std::vector<compiler::Node*>{phi_bb38_11}, &block42, std::vector<compiler::Node*>{phi_bb38_11});
  }

  TNode<Smi> phi_bb41_11;
  TNode<BoolT> tmp65;
      TNode<Object> tmp67;
  if (block41.is_used()) {
    ca_.Bind(&block41, &phi_bb41_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch66__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch66__label);
    tmp65 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch66__label.is_used()) {
      compiler::CodeAssemblerLabel catch66_skip(&ca_);
      ca_.Goto(&catch66_skip);
      ca_.Bind(&catch66__label, &tmp67);
      ca_.Goto(&block44, phi_bb41_11);
      ca_.Bind(&catch66_skip);
    }
    ca_.Goto(&block43, phi_bb41_11, tmp65);
  }

  TNode<Smi> phi_bb44_11;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_11);
    ca_.Goto(&block8, phi_bb44_11, tmp67);
  }

  TNode<Smi> phi_bb42_11;
  TNode<BoolT> tmp68;
      TNode<Object> tmp70;
  if (block42.is_used()) {
    ca_.Bind(&block42, &phi_bb42_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch69__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch69__label);
    tmp68 = CodeStubAssembler(state_).IsPromiseSpeciesProtectorCellInvalid();
    }
    if (catch69__label.is_used()) {
      compiler::CodeAssemblerLabel catch69_skip(&ca_);
      ca_.Goto(&catch69_skip);
      ca_.Bind(&catch69__label, &tmp70);
      ca_.Goto(&block45, phi_bb42_11);
      ca_.Bind(&catch69_skip);
    }
    ca_.Goto(&block43, phi_bb42_11, tmp68);
  }

  TNode<Smi> phi_bb45_11;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_11);
    ca_.Goto(&block8, phi_bb45_11, tmp70);
  }

  TNode<Smi> phi_bb43_11;
  TNode<BoolT> phi_bb43_17;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_11, &phi_bb43_17);
    ca_.Branch(phi_bb43_17, &block46, std::vector<compiler::Node*>{phi_bb43_11}, &block47, std::vector<compiler::Node*>{phi_bb43_11});
  }

  TNode<Smi> phi_bb46_11;
  TNode<BoolT> tmp71;
      TNode<Object> tmp73;
  if (block46.is_used()) {
    ca_.Bind(&block46, &phi_bb46_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch72__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch72__label);
    tmp71 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch72__label.is_used()) {
      compiler::CodeAssemblerLabel catch72_skip(&ca_);
      ca_.Goto(&catch72_skip);
      ca_.Bind(&catch72__label, &tmp73);
      ca_.Goto(&block49, phi_bb46_11);
      ca_.Bind(&catch72_skip);
    }
    ca_.Goto(&block48, phi_bb46_11, tmp71);
  }

  TNode<Smi> phi_bb49_11;
  if (block49.is_used()) {
    ca_.Bind(&block49, &phi_bb49_11);
    ca_.Goto(&block8, phi_bb49_11, tmp73);
  }

  TNode<Smi> phi_bb47_11;
  TNode<BoolT> tmp74;
      TNode<Object> tmp76;
  if (block47.is_used()) {
    ca_.Bind(&block47, &phi_bb47_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch75__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch75__label);
    tmp74 = Is_Smi_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp25});
    }
    if (catch75__label.is_used()) {
      compiler::CodeAssemblerLabel catch75_skip(&ca_);
      ca_.Goto(&catch75_skip);
      ca_.Bind(&catch75__label, &tmp76);
      ca_.Goto(&block50, phi_bb47_11);
      ca_.Bind(&catch75_skip);
    }
    ca_.Goto(&block48, phi_bb47_11, tmp74);
  }

  TNode<Smi> phi_bb50_11;
  if (block50.is_used()) {
    ca_.Bind(&block50, &phi_bb50_11);
    ca_.Goto(&block8, phi_bb50_11, tmp76);
  }

  TNode<Smi> phi_bb48_11;
  TNode<BoolT> phi_bb48_17;
  if (block48.is_used()) {
    ca_.Bind(&block48, &phi_bb48_11, &phi_bb48_17);
    ca_.Branch(phi_bb48_17, &block51, std::vector<compiler::Node*>{phi_bb48_11}, &block52, std::vector<compiler::Node*>{phi_bb48_11});
  }

  TNode<Smi> phi_bb51_11;
  TNode<BoolT> tmp77;
      TNode<Object> tmp79;
  if (block51.is_used()) {
    ca_.Bind(&block51, &phi_bb51_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch78__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch78__label);
    tmp77 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch78__label.is_used()) {
      compiler::CodeAssemblerLabel catch78_skip(&ca_);
      ca_.Goto(&catch78_skip);
      ca_.Bind(&catch78__label, &tmp79);
      ca_.Goto(&block54, phi_bb51_11);
      ca_.Bind(&catch78_skip);
    }
    ca_.Goto(&block53, phi_bb51_11, tmp77);
  }

  TNode<Smi> phi_bb54_11;
  if (block54.is_used()) {
    ca_.Bind(&block54, &phi_bb54_11);
    ca_.Goto(&block8, phi_bb54_11, tmp79);
  }

  TNode<Smi> phi_bb52_11;
  TNode<HeapObject> tmp80;
      TNode<Object> tmp82;
  TNode<IntPtrT> tmp83;
      TNode<Object> tmp85;
  TNode<Map> tmp86;
  TNode<BoolT> tmp87;
      TNode<Object> tmp89;
  TNode<BoolT> tmp90;
      TNode<Object> tmp92;
  if (block52.is_used()) {
    ca_.Bind(&block52, &phi_bb52_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch81__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch81__label);
    tmp80 = UnsafeCast_HeapObject_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp25});
    }
    if (catch81__label.is_used()) {
      compiler::CodeAssemblerLabel catch81_skip(&ca_);
      ca_.Goto(&catch81_skip);
      ca_.Bind(&catch81__label, &tmp82);
      ca_.Goto(&block55, phi_bb52_11);
      ca_.Bind(&catch81_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch84__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch84__label);
    tmp83 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    }
    if (catch84__label.is_used()) {
      compiler::CodeAssemblerLabel catch84_skip(&ca_);
      ca_.Goto(&catch84_skip);
      ca_.Bind(&catch84__label, &tmp85);
      ca_.Goto(&block56, phi_bb52_11);
      ca_.Bind(&catch84_skip);
    }
    tmp86 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp80, tmp83});
    compiler::CodeAssemblerExceptionHandlerLabel catch88__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch88__label);
    tmp87 = IsPromiseThenLookupChainIntact_0(state_, TNode<Context>{p_context}, TNode<NativeContext>{p_nativeContext}, TNode<Map>{tmp86});
    }
    if (catch88__label.is_used()) {
      compiler::CodeAssemblerLabel catch88_skip(&ca_);
      ca_.Goto(&catch88_skip);
      ca_.Bind(&catch88__label, &tmp89);
      ca_.Goto(&block57, phi_bb52_11);
      ca_.Bind(&catch88_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch91__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch91__label);
    tmp90 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp87});
    }
    if (catch91__label.is_used()) {
      compiler::CodeAssemblerLabel catch91_skip(&ca_);
      ca_.Goto(&catch91_skip);
      ca_.Bind(&catch91__label, &tmp92);
      ca_.Goto(&block58, phi_bb52_11);
      ca_.Bind(&catch91_skip);
    }
    ca_.Goto(&block53, phi_bb52_11, tmp90);
  }

  TNode<Smi> phi_bb55_11;
  if (block55.is_used()) {
    ca_.Bind(&block55, &phi_bb55_11);
    ca_.Goto(&block8, phi_bb55_11, tmp82);
  }

  TNode<Smi> phi_bb56_11;
  if (block56.is_used()) {
    ca_.Bind(&block56, &phi_bb56_11);
    ca_.Goto(&block8, phi_bb56_11, tmp85);
  }

  TNode<Smi> phi_bb57_11;
  if (block57.is_used()) {
    ca_.Bind(&block57, &phi_bb57_11);
    ca_.Goto(&block8, phi_bb57_11, tmp89);
  }

  TNode<Smi> phi_bb58_11;
  if (block58.is_used()) {
    ca_.Bind(&block58, &phi_bb58_11);
    ca_.Goto(&block8, phi_bb58_11, tmp92);
  }

  TNode<Smi> phi_bb53_11;
  TNode<BoolT> phi_bb53_17;
  if (block53.is_used()) {
    ca_.Bind(&block53, &phi_bb53_11, &phi_bb53_17);
    ca_.Branch(phi_bb53_17, &block33, std::vector<compiler::Node*>{phi_bb53_11}, &block34, std::vector<compiler::Node*>{phi_bb53_11});
  }

  TNode<Smi> phi_bb33_11;
  TNode<Object> tmp93;
      TNode<Object> tmp95;
  TNode<String> tmp96;
  TNode<Object> tmp97;
      TNode<Object> tmp99;
  TNode<Object> tmp100;
      TNode<Object> tmp102;
  TNode<BoolT> tmp103;
      TNode<Object> tmp105;
  if (block33.is_used()) {
    ca_.Bind(&block33, &phi_bb33_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch94__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch94__label);
    tmp93 = CallResolve_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_constructor}, TNode<Object>{p_promiseResolveFunction}, TNode<Object>{tmp25});
    }
    if (catch94__label.is_used()) {
      compiler::CodeAssemblerLabel catch94_skip(&ca_);
      ca_.Goto(&catch94_skip);
      ca_.Bind(&catch94__label, &tmp95);
      ca_.Goto(&block60, phi_bb33_11);
      ca_.Bind(&catch94_skip);
    }
    tmp96 = kThenString_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch98__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch98__label);
    tmp97 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{tmp93}, TNode<Object>{tmp96});
    }
    if (catch98__label.is_used()) {
      compiler::CodeAssemblerLabel catch98_skip(&ca_);
      ca_.Goto(&catch98_skip);
      ca_.Bind(&catch98__label, &tmp99);
      ca_.Goto(&block61, phi_bb33_11);
      ca_.Bind(&catch98_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch101__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch101__label);
    tmp100 = CodeStubAssembler(state_).Call(TNode<Context>{p_nativeContext}, TNode<Object>{tmp97}, TNode<Object>{tmp93}, TNode<Object>{tmp52}, TNode<Object>{tmp54});
    }
    if (catch101__label.is_used()) {
      compiler::CodeAssemblerLabel catch101_skip(&ca_);
      ca_.Goto(&catch101_skip);
      ca_.Bind(&catch101__label, &tmp102);
      ca_.Goto(&block62, phi_bb33_11);
      ca_.Bind(&catch101_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch104__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch104__label);
    tmp103 = CodeStubAssembler(state_).IsDebugActive();
    }
    if (catch104__label.is_used()) {
      compiler::CodeAssemblerLabel catch104_skip(&ca_);
      ca_.Goto(&catch104_skip);
      ca_.Bind(&catch104__label, &tmp105);
      ca_.Goto(&block65, phi_bb33_11);
      ca_.Bind(&catch104_skip);
    }
    ca_.Branch(tmp103, &block66, std::vector<compiler::Node*>{phi_bb33_11}, &block67, std::vector<compiler::Node*>{phi_bb33_11});
  }

  TNode<Smi> phi_bb60_11;
  if (block60.is_used()) {
    ca_.Bind(&block60, &phi_bb60_11);
    ca_.Goto(&block8, phi_bb60_11, tmp95);
  }

  TNode<Smi> phi_bb61_11;
  if (block61.is_used()) {
    ca_.Bind(&block61, &phi_bb61_11);
    ca_.Goto(&block8, phi_bb61_11, tmp99);
  }

  TNode<Smi> phi_bb62_11;
  if (block62.is_used()) {
    ca_.Bind(&block62, &phi_bb62_11);
    ca_.Goto(&block8, phi_bb62_11, tmp102);
  }

  TNode<Smi> phi_bb65_11;
  if (block65.is_used()) {
    ca_.Bind(&block65, &phi_bb65_11);
    ca_.Goto(&block8, phi_bb65_11, tmp105);
  }

  TNode<Smi> phi_bb66_11;
  TNode<BoolT> tmp106;
      TNode<Object> tmp108;
  if (block66.is_used()) {
    ca_.Bind(&block66, &phi_bb66_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch107__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch107__label);
    tmp106 = Is_JSPromise_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp100});
    }
    if (catch107__label.is_used()) {
      compiler::CodeAssemblerLabel catch107_skip(&ca_);
      ca_.Goto(&catch107_skip);
      ca_.Bind(&catch107__label, &tmp108);
      ca_.Goto(&block69, phi_bb66_11);
      ca_.Bind(&catch107_skip);
    }
    ca_.Goto(&block68, phi_bb66_11, tmp106);
  }

  TNode<Smi> phi_bb69_11;
  if (block69.is_used()) {
    ca_.Bind(&block69, &phi_bb69_11);
    ca_.Goto(&block8, phi_bb69_11, tmp108);
  }

  TNode<Smi> phi_bb67_11;
  TNode<BoolT> tmp109;
      TNode<Object> tmp111;
  if (block67.is_used()) {
    ca_.Bind(&block67, &phi_bb67_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch110__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch110__label);
    tmp109 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    }
    if (catch110__label.is_used()) {
      compiler::CodeAssemblerLabel catch110_skip(&ca_);
      ca_.Goto(&catch110_skip);
      ca_.Bind(&catch110__label, &tmp111);
      ca_.Goto(&block70, phi_bb67_11);
      ca_.Bind(&catch110_skip);
    }
    ca_.Goto(&block68, phi_bb67_11, tmp109);
  }

  TNode<Smi> phi_bb70_11;
  if (block70.is_used()) {
    ca_.Bind(&block70, &phi_bb70_11);
    ca_.Goto(&block8, phi_bb70_11, tmp111);
  }

  TNode<Smi> phi_bb68_11;
  TNode<BoolT> phi_bb68_20;
  if (block68.is_used()) {
    ca_.Bind(&block68, &phi_bb68_11, &phi_bb68_20);
    ca_.Branch(phi_bb68_20, &block63, std::vector<compiler::Node*>{phi_bb68_11}, &block64, std::vector<compiler::Node*>{phi_bb68_11});
  }

  TNode<Smi> phi_bb63_11;
  TNode<Symbol> tmp112;
  TNode<Object> tmp113;
      TNode<Object> tmp115;
  if (block63.is_used()) {
    ca_.Bind(&block63, &phi_bb63_11);
    tmp112 = kPromiseHandledBySymbol_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch114__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch114__label);
    tmp113 = CodeStubAssembler(state_).SetPropertyStrict(TNode<Context>{p_context}, TNode<Object>{tmp100}, TNode<Object>{tmp112}, TNode<Object>{tmp1});
    }
    if (catch114__label.is_used()) {
      compiler::CodeAssemblerLabel catch114_skip(&ca_);
      ca_.Goto(&catch114_skip);
      ca_.Bind(&catch114__label, &tmp115);
      ca_.Goto(&block71, phi_bb63_11);
      ca_.Bind(&catch114_skip);
    }
    ca_.Goto(&block64, phi_bb63_11);
  }

  TNode<Smi> phi_bb71_11;
  if (block71.is_used()) {
    ca_.Bind(&block71, &phi_bb71_11);
    ca_.Goto(&block8, phi_bb71_11, tmp115);
  }

  TNode<Smi> phi_bb64_11;
  if (block64.is_used()) {
    ca_.Bind(&block64, &phi_bb64_11);
    ca_.Goto(&block59, phi_bb64_11);
  }

  TNode<Smi> phi_bb34_11;
  TNode<JSPromise> tmp116;
      TNode<Object> tmp118;
  TNode<Oddball> tmp119;
      TNode<Object> tmp121;
  if (block34.is_used()) {
    ca_.Bind(&block34, &phi_bb34_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch117__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch117__label);
    tmp116 = UnsafeCast_JSPromise_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp25});
    }
    if (catch117__label.is_used()) {
      compiler::CodeAssemblerLabel catch117_skip(&ca_);
      ca_.Goto(&catch117_skip);
      ca_.Bind(&catch117__label, &tmp118);
      ca_.Goto(&block72, phi_bb34_11);
      ca_.Bind(&catch117_skip);
    }
    tmp119 = Undefined_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch120__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch120__label);
    PerformPromiseThenImpl_0(state_, TNode<Context>{p_context}, TNode<JSPromise>{tmp116}, TNode<HeapObject>{tmp52}, TNode<HeapObject>{tmp54}, TNode<HeapObject>{tmp119});
    }
    if (catch120__label.is_used()) {
      compiler::CodeAssemblerLabel catch120_skip(&ca_);
      ca_.Goto(&catch120_skip);
      ca_.Bind(&catch120__label, &tmp121);
      ca_.Goto(&block73, phi_bb34_11);
      ca_.Bind(&catch120_skip);
    }
    ca_.Goto(&block59, phi_bb34_11);
  }

  TNode<Smi> phi_bb72_11;
  if (block72.is_used()) {
    ca_.Bind(&block72, &phi_bb72_11);
    ca_.Goto(&block8, phi_bb72_11, tmp118);
  }

  TNode<Smi> phi_bb73_11;
  if (block73.is_used()) {
    ca_.Bind(&block73, &phi_bb73_11);
    ca_.Goto(&block8, phi_bb73_11, tmp121);
  }

  TNode<Smi> phi_bb59_11;
  TNode<Smi> tmp122;
      TNode<Object> tmp124;
  TNode<Smi> tmp125;
      TNode<Object> tmp127;
  if (block59.is_used()) {
    ca_.Bind(&block59, &phi_bb59_11);
    compiler::CodeAssemblerExceptionHandlerLabel catch123__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch123__label);
    tmp122 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    }
    if (catch123__label.is_used()) {
      compiler::CodeAssemblerLabel catch123_skip(&ca_);
      ca_.Goto(&catch123_skip);
      ca_.Bind(&catch123__label, &tmp124);
      ca_.Goto(&block74, phi_bb59_11, phi_bb59_11, phi_bb59_11);
      ca_.Bind(&catch123_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch126__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch126__label);
    tmp125 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb59_11}, TNode<Smi>{tmp122});
    }
    if (catch126__label.is_used()) {
      compiler::CodeAssemblerLabel catch126_skip(&ca_);
      ca_.Goto(&catch126_skip);
      ca_.Bind(&catch126__label, &tmp127);
      ca_.Goto(&block75, phi_bb59_11, phi_bb59_11);
      ca_.Bind(&catch126_skip);
    }
    ca_.Goto(&block12, tmp125);
  }

  TNode<Smi> phi_bb74_11;
  TNode<Smi> phi_bb74_16;
  TNode<Smi> phi_bb74_17;
  if (block74.is_used()) {
    ca_.Bind(&block74, &phi_bb74_11, &phi_bb74_16, &phi_bb74_17);
    ca_.Goto(&block8, phi_bb74_11, tmp124);
  }

  TNode<Smi> phi_bb75_11;
  TNode<Smi> phi_bb75_16;
  if (block75.is_used()) {
    ca_.Bind(&block75, &phi_bb75_11, &phi_bb75_16);
    ca_.Goto(&block8, phi_bb75_11, tmp127);
  }

  TNode<Smi> phi_bb11_11;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_11);
    ca_.Goto(&block5, phi_bb11_11);
  }

  TNode<Smi> phi_bb8_11;
  TNode<Object> phi_bb8_12;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_11, &phi_bb8_12);
    IteratorCloseOnException_0(state_, TNode<Context>{p_context}, TorqueStructIteratorRecord{TNode<JSReceiver>{p_iter.object}, TNode<Object>{p_iter.next}});
    ca_.Goto(&block1, phi_bb8_12);
  }

  TNode<Smi> phi_bb5_11;
  TNode<IntPtrT> tmp128;
  TNode<Object> tmp129;
  TNode<IntPtrT> tmp130;
  TNode<Smi> tmp131;
  TNode<Smi> tmp132;
  TNode<Smi> tmp133;
  TNode<Smi> tmp134;
  TNode<BoolT> tmp135;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_11);
    tmp128 = kPromiseAllResolveElementRemainingSlot_0(state_);
    std::tie(tmp129, tmp130) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{tmp10}, TNode<IntPtrT>{tmp128}).Flatten();
    tmp131 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp129, tmp130});
    tmp132 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp133 = CodeStubAssembler(state_).SmiSub(TNode<Smi>{tmp131}, TNode<Smi>{tmp132});
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp129, tmp130}, tmp133);
    tmp134 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp135 = CodeStubAssembler(state_).SmiGreaterThanOrEqual(TNode<Smi>{tmp133}, TNode<Smi>{tmp134});
    ca_.Branch(tmp135, &block76, std::vector<compiler::Node*>{phi_bb5_11}, &block77, std::vector<compiler::Node*>{phi_bb5_11});
  }

  TNode<Smi> phi_bb77_11;
  if (block77.is_used()) {
    ca_.Bind(&block77, &phi_bb77_11);
    {
      auto pos_stack = ca_.GetMacroSourcePositionStack();
      pos_stack.push_back({"src/builtins/promise-all.tq", 278});
      CodeStubAssembler(state_).FailAssert("Torque assert 'remainingElementsCount >= 0' failed", pos_stack);
    }
  }

  TNode<Smi> phi_bb76_11;
  TNode<Smi> tmp136;
  TNode<BoolT> tmp137;
  if (block76.is_used()) {
    ca_.Bind(&block76, &phi_bb76_11);
    tmp136 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp137 = CodeStubAssembler(state_).SmiGreaterThan(TNode<Smi>{tmp133}, TNode<Smi>{tmp136});
    ca_.Branch(tmp137, &block78, std::vector<compiler::Node*>{phi_bb76_11}, &block79, std::vector<compiler::Node*>{phi_bb76_11});
  }

  TNode<Smi> phi_bb78_11;
  TNode<IntPtrT> tmp138;
  TNode<Object> tmp139;
  TNode<IntPtrT> tmp140;
  TNode<FixedArray> tmp141;
  TNode<IntPtrT> tmp142;
  TNode<IntPtrT> tmp143;
  TNode<IntPtrT> tmp144;
  TNode<IntPtrT> tmp145;
  TNode<BoolT> tmp146;
  if (block78.is_used()) {
    ca_.Bind(&block78, &phi_bb78_11);
    tmp138 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp139, tmp140) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{tmp10}, TNode<IntPtrT>{tmp138}).Flatten();
    tmp141 = CodeStubAssembler(state_).LoadReference<FixedArray>(CodeStubAssembler::Reference{tmp139, tmp140});
    tmp142 = CodeStubAssembler(state_).SmiUntag(TNode<Smi>{phi_bb78_11});
    tmp143 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp144 = CodeStubAssembler(state_).IntPtrSub(TNode<IntPtrT>{tmp142}, TNode<IntPtrT>{tmp143});
    tmp145 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp141});
    tmp146 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{tmp145}, TNode<IntPtrT>{tmp144});
    ca_.Branch(tmp146, &block81, std::vector<compiler::Node*>{phi_bb78_11}, &block82, std::vector<compiler::Node*>{phi_bb78_11});
  }

  TNode<Smi> phi_bb81_11;
  TNode<IntPtrT> tmp147;
  TNode<FixedArray> tmp148;
  if (block81.is_used()) {
    ca_.Bind(&block81, &phi_bb81_11);
    tmp147 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp148 = ExtractFixedArray_0(state_, TNode<FixedArray>{tmp141}, TNode<IntPtrT>{tmp147}, TNode<IntPtrT>{tmp145}, TNode<IntPtrT>{tmp144});
    CodeStubAssembler(state_).StoreReference<FixedArray>(CodeStubAssembler::Reference{tmp139, tmp140}, tmp148);
    ca_.Goto(&block82, phi_bb81_11);
  }

  TNode<Smi> phi_bb82_11;
  if (block82.is_used()) {
    ca_.Bind(&block82, &phi_bb82_11);
    ca_.Goto(&block80, phi_bb82_11);
  }

  TNode<Smi> phi_bb79_11;
  TNode<IntPtrT> tmp149;
  TNode<Object> tmp150;
  TNode<IntPtrT> tmp151;
  TNode<FixedArray> tmp152;
  TNode<IntPtrT> tmp153;
  TNode<Object> tmp154;
  TNode<IntPtrT> tmp155;
  TNode<Map> tmp156;
  TNode<JSArray> tmp157;
  TNode<Object> tmp158;
  TNode<Oddball> tmp159;
  TNode<Object> tmp160;
  if (block79.is_used()) {
    ca_.Bind(&block79, &phi_bb79_11);
    tmp149 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp150, tmp151) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{tmp10}, TNode<IntPtrT>{tmp149}).Flatten();
    tmp152 = CodeStubAssembler(state_).LoadReference<FixedArray>(CodeStubAssembler::Reference{tmp150, tmp151});
    tmp153 = JS_ARRAY_PACKED_ELEMENTS_MAP_INDEX_0(state_);
    std::tie(tmp154, tmp155) = NativeContextSlot_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp153}).Flatten();
    tmp156 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp154, tmp155});
    tmp157 = NewJSArray_0(state_, TNode<Context>{p_context}, TNode<Map>{tmp156}, TNode<FixedArrayBase>{tmp152});
    tmp158 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp3});
    tmp159 = Undefined_0(state_);
    tmp160 = CodeStubAssembler(state_).Call(TNode<Context>{p_nativeContext}, TNode<Object>{tmp158}, TNode<Object>{tmp159}, TNode<Object>{tmp157});
    ca_.Goto(&block80, phi_bb79_11);
  }

  TNode<Smi> phi_bb80_11;
  if (block80.is_used()) {
    ca_.Bind(&block80, &phi_bb80_11);
    ca_.Goto(&block83);
  }

  TNode<Object> phi_bb1_0;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_0);
    *label_Reject_parameter_0 = phi_bb1_0;
    ca_.Goto(label_Reject);
  }

    ca_.Bind(&block83);
  return TNode<Object>{tmp1};
}


} // namespace internal
} // namespace v8
