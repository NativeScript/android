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
#include "torque-generated/src/builtins/promise-any-tq-csa.h"
#include "torque-generated/src/builtins/aggregate-error-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/array-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/iterator-tq-csa.h"
#include "torque-generated/src/builtins/promise-abstract-operations-tq-csa.h"
#include "torque-generated/src/builtins/promise-all-tq-csa.h"
#include "torque-generated/src/builtins/promise-all-element-closure-tq-csa.h"
#include "torque-generated/src/builtins/promise-any-tq-csa.h"
#include "torque-generated/src/builtins/promise-constructor-tq-csa.h"
#include "torque-generated/src/builtins/promise-misc-tq-csa.h"
#include "torque-generated/src/builtins/promise-resolve-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/contexts-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"

namespace v8 {
namespace internal {
TNode<IntPtrT> FromConstexpr_PromiseAnyRejectElementContextSlots_constexpr_kPromiseAnyRejectElementRemainingSlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseAnyRejectElementContextSlots p_o) {
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


TNode<IntPtrT> FromConstexpr_PromiseAnyRejectElementContextSlots_constexpr_kPromiseAnyRejectElementCapabilitySlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseAnyRejectElementContextSlots p_o) {
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


TNode<IntPtrT> FromConstexpr_PromiseAnyRejectElementContextSlots_constexpr_kPromiseAnyRejectElementErrorsSlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseAnyRejectElementContextSlots p_o) {
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


TNode<IntPtrT> FromConstexpr_PromiseAnyRejectElementContextSlots_constexpr_kPromiseAnyRejectElementLength_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseAnyRejectElementContextSlots p_o) {
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


TNode<IntPtrT> kPromiseAnyRejectElementRemainingSlot_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  TNode<IntPtrT> tmp0;
    tmp0 = FromConstexpr_PromiseAnyRejectElementContextSlots_constexpr_kPromiseAnyRejectElementRemainingSlot_0(state_, PromiseBuiltins::PromiseAnyRejectElementContextSlots::kPromiseAnyRejectElementRemainingSlot);
  TNode<IntPtrT> tmp1;
    tmp1 = (TNode<IntPtrT>{tmp0});
  return TNode<IntPtrT>{tmp1};}

TNode<IntPtrT> kPromiseAnyRejectElementCapabilitySlot_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  TNode<IntPtrT> tmp0;
    tmp0 = FromConstexpr_PromiseAnyRejectElementContextSlots_constexpr_kPromiseAnyRejectElementCapabilitySlot_0(state_, PromiseBuiltins::PromiseAnyRejectElementContextSlots::kPromiseAnyRejectElementCapabilitySlot);
  TNode<IntPtrT> tmp1;
    tmp1 = (TNode<IntPtrT>{tmp0});
  return TNode<IntPtrT>{tmp1};}

TNode<IntPtrT> kPromiseAnyRejectElementErrorsSlot_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  TNode<IntPtrT> tmp0;
    tmp0 = FromConstexpr_PromiseAnyRejectElementContextSlots_constexpr_kPromiseAnyRejectElementErrorsSlot_0(state_, PromiseBuiltins::PromiseAnyRejectElementContextSlots::kPromiseAnyRejectElementErrorsSlot);
  TNode<IntPtrT> tmp1;
    tmp1 = (TNode<IntPtrT>{tmp0});
  return TNode<IntPtrT>{tmp1};}

TNode<Context> CreatePromiseAnyRejectElementContext_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<PromiseCapability> p_capability, TNode<NativeContext> p_nativeContext) {
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
    tmp0 = FromConstexpr_intptr_constexpr_intptr_0(state_, PromiseBuiltins::PromiseAnyRejectElementContextSlots::kPromiseAnyRejectElementLength);
    tmp1 = AllocateSyntheticFunctionContext_1(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp0});
    tmp2 = (TNode<Context>{tmp1});
    tmp3 = kPromiseAnyRejectElementRemainingSlot_0(state_);
    InitContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_Smi_constexpr_int31_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp3}, 1);
    tmp4 = kPromiseAnyRejectElementCapabilitySlot_0(state_);
    InitContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_PromiseCapability_PromiseCapability_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp4}, TNode<PromiseCapability>{p_capability});
    tmp5 = kPromiseAnyRejectElementErrorsSlot_0(state_);
    tmp6 = kEmptyFixedArray_0(state_);
    InitContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_FixedArray_EmptyFixedArray_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp5}, TNode<FixedArray>{tmp6});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Context>{tmp2};
}


TNode<JSFunction> CreatePromiseAnyRejectElementFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Context> p_rejectElementContext, TNode<Smi> p_index, TNode<NativeContext> p_nativeContext) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Map> tmp3;
  TNode<SharedFunctionInfo> tmp4;
  TNode<JSFunction> tmp5;
  TNode<IntPtrT> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = STRICT_FUNCTION_WITHOUT_PROTOTYPE_MAP_INDEX_0(state_);
    std::tie(tmp1, tmp2) = ContextSlot_NativeContext_NativeContext_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp0}).Flatten();
    tmp3 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp1, tmp2});
    tmp4 = CodeStubAssembler(state_).PromiseAnyRejectElementSharedFunConstant();
    tmp5 = CodeStubAssembler(state_).AllocateFunctionWithMapAndContext(TNode<Map>{tmp3}, TNode<SharedFunctionInfo>{tmp4}, TNode<Context>{p_rejectElementContext});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp5, tmp6}, p_index);
    ca_.Goto(&block14);
  }

    ca_.Bind(&block14);
  return TNode<JSFunction>{tmp5};
}


TF_BUILTIN(PromiseAnyRejectElementClosure, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<JSFunction> parameter2 = UncheckedParameter<JSFunction>(Descriptor::kJSTarget);
USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kValue);
  USE(parameter3);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = IsNativeContext_0(state_, TNode<HeapObject>{parameter0});
    ca_.Branch(tmp0, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  TNode<Oddball> tmp1;
  if (block1.is_used()) {
    ca_.Bind(&block1);
    tmp1 = Undefined_0(state_);
    CodeStubAssembler(state_).Return(tmp1);
  }

  TNode<Context> tmp2;
  TNode<NativeContext> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp2 = (TNode<Context>{parameter0});
    tmp3 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{tmp2});
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    CodeStubAssembler(state_).StoreReference<Context>(CodeStubAssembler::Reference{parameter2, tmp4}, tmp3);
    compiler::CodeAssemblerLabel label6(&ca_);
    tmp5 = CodeStubAssembler(state_).LoadJSReceiverIdentityHash(TNode<JSReceiver>{parameter2}, &label6);
    ca_.Goto(&block13);
    if (label6.is_used()) {
      ca_.Bind(&label6);
      ca_.Goto(&block14);
    }
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<FixedArray> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Object> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<Smi> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<IntPtrT> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<IntPtrT> tmp20;
  TNode<IntPtrT> tmp21;
  TNode<BoolT> tmp22;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp8 = CodeStubAssembler(state_).IntPtrSub(TNode<IntPtrT>{tmp5}, TNode<IntPtrT>{tmp7});
    tmp9 = kPromiseAnyRejectElementErrorsSlot_0(state_);
    std::tie(tmp10, tmp11) = ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_FixedArray_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp9}).Flatten();
    tmp12 = CodeStubAssembler(state_).LoadReference<FixedArray>(CodeStubAssembler::Reference{tmp10, tmp11});
    tmp13 = kPromiseAnyRejectElementRemainingSlot_0(state_);
    std::tie(tmp14, tmp15) = ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_Smi_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp13}).Flatten();
    tmp16 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp14, tmp15});
    tmp17 = CodeStubAssembler(state_).SmiUntag(TNode<Smi>{tmp16});
    tmp18 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp19 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp8}, TNode<IntPtrT>{tmp18});
    tmp20 = CodeStubAssembler(state_).IntPtrMax(TNode<IntPtrT>{tmp17}, TNode<IntPtrT>{tmp19});
    tmp21 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp12});
    tmp22 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{tmp20}, TNode<IntPtrT>{tmp21});
    ca_.Branch(tmp22, &block19, std::vector<compiler::Node*>{}, &block20, std::vector<compiler::Node*>{tmp12});
  }

  TNode<IntPtrT> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<FixedArray> tmp25;
  TNode<IntPtrT> tmp26;
  TNode<Object> tmp27;
  TNode<IntPtrT> tmp28;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    tmp23 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp12});
    tmp24 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp25 = ExtractFixedArray_0(state_, TNode<FixedArray>{tmp12}, TNode<IntPtrT>{tmp24}, TNode<IntPtrT>{tmp23}, TNode<IntPtrT>{tmp20});
    tmp26 = kPromiseAnyRejectElementErrorsSlot_0(state_);
    std::tie(tmp27, tmp28) = ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_FixedArray_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp26}).Flatten();
    CodeStubAssembler(state_).StoreReference<FixedArray>(CodeStubAssembler::Reference{tmp27, tmp28}, tmp25);
    ca_.Goto(&block20, tmp25);
  }

  TNode<FixedArray> phi_bb20_8;
  TNode<Object> tmp29;
  TNode<IntPtrT> tmp30;
  TNode<IntPtrT> tmp31;
  TNode<UintPtrT> tmp32;
  TNode<UintPtrT> tmp33;
  TNode<BoolT> tmp34;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_8);
    std::tie(tmp29, tmp30, tmp31) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{phi_bb20_8}).Flatten();
    tmp32 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp8});
    tmp33 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp31});
    tmp34 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp32}, TNode<UintPtrT>{tmp33});
    ca_.Branch(tmp34, &block25, std::vector<compiler::Node*>{}, &block26, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp35;
  TNode<IntPtrT> tmp36;
  TNode<Object> tmp37;
  TNode<IntPtrT> tmp38;
  TNode<Smi> tmp39;
  TNode<Smi> tmp40;
  TNode<IntPtrT> tmp41;
  TNode<Object> tmp42;
  TNode<IntPtrT> tmp43;
  TNode<Smi> tmp44;
  TNode<BoolT> tmp45;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp35 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp8});
    tmp36 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp30}, TNode<IntPtrT>{tmp35});
    std::tie(tmp37, tmp38) = NewReference_Object_0(state_, TNode<Object>{tmp29}, TNode<IntPtrT>{tmp36}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp37, tmp38}, parameter3);
    tmp39 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp40 = CodeStubAssembler(state_).SmiSub(TNode<Smi>{tmp16}, TNode<Smi>{tmp39});
    tmp41 = kPromiseAnyRejectElementRemainingSlot_0(state_);
    std::tie(tmp42, tmp43) = ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_Smi_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp41}).Flatten();
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp42, tmp43}, tmp40);
    tmp44 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp45 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp40}, TNode<Smi>{tmp44});
    ca_.Branch(tmp45, &block28, std::vector<compiler::Node*>{}, &block29, std::vector<compiler::Node*>{});
  }

  if (block26.is_used()) {
    ca_.Bind(&block26);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<JSObject> tmp46;
  TNode<IntPtrT> tmp47;
  TNode<Object> tmp48;
  TNode<IntPtrT> tmp49;
  TNode<PromiseCapability> tmp50;
  TNode<IntPtrT> tmp51;
  TNode<Object> tmp52;
  TNode<JSReceiver> tmp53;
  TNode<Oddball> tmp54;
  TNode<Object> tmp55;
  if (block28.is_used()) {
    ca_.Bind(&block28);
    tmp46 = ConstructAggregateError_0(state_, TNode<Context>{tmp2}, TNode<FixedArray>{phi_bb20_8});
    tmp47 = kPromiseAnyRejectElementCapabilitySlot_0(state_);
    std::tie(tmp48, tmp49) = ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_PromiseCapability_0(state_, TNode<Context>{tmp2}, TNode<IntPtrT>{tmp47}).Flatten();
    tmp50 = CodeStubAssembler(state_).LoadReference<PromiseCapability>(CodeStubAssembler::Reference{tmp48, tmp49});
    tmp51 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp52 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp50, tmp51});
    tmp53 = UnsafeCast_Callable_0(state_, TNode<Context>{tmp2}, TNode<Object>{tmp52});
    tmp54 = Undefined_0(state_);
    tmp55 = CodeStubAssembler(state_).Call(TNode<Context>{tmp2}, TNode<Object>{tmp53}, TNode<Object>{tmp54}, TNode<Object>{tmp46});
    ca_.Goto(&block29);
  }

  TNode<Oddball> tmp56;
  if (block29.is_used()) {
    ca_.Bind(&block29);
    tmp56 = Undefined_0(state_);
    CodeStubAssembler(state_).Return(tmp56);
  }
}

TNode<Object> PerformPromiseAny_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TorqueStructIteratorRecord p_iteratorRecord, TNode<JSReceiver> p_constructor, TNode<PromiseCapability> p_resultCapability, TNode<Object> p_promiseResolveFunction, compiler::CodeAssemblerLabel* label_Reject, compiler::TypedCodeAssemblerVariable<Object>* label_Reject_parameter_0) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block11(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block18(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block19(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Object> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi, Smi> block22(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi> block23(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block24(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block25(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block26(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi> block27(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block28(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block29(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block30(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block31(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block32(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block33(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block34(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block35(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi, Smi> block36(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Smi> block37(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block40(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block44(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block45(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block38(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block46(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block47(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block48(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block39(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi, Object> block6(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block49(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Smi> block50(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block51(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Context> tmp0;
  TNode<Smi> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<IntPtrT> tmp4;
      TNode<Object> tmp6;
  TNode<Map> tmp7;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CreatePromiseAnyRejectElementContext_0(state_, TNode<Context>{p_context}, TNode<PromiseCapability>{p_resultCapability}, TNode<NativeContext>{p_nativeContext});
    tmp1 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp2 = ITERATOR_RESULT_MAP_INDEX_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch5__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch5__label);
    std::tie(tmp3, tmp4) = NativeContextSlot_Map_0(state_, TNode<NativeContext>{p_nativeContext}, TNode<IntPtrT>{tmp2}).Flatten();
    }
    if (catch5__label.is_used()) {
      compiler::CodeAssemblerLabel catch5_skip(&ca_);
      ca_.Goto(&catch5_skip);
      ca_.Bind(&catch5__label, &tmp6);
      ca_.Goto(&block7);
      ca_.Bind(&catch5_skip);
    }
    tmp7 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp3, tmp4});
    ca_.Goto(&block10, tmp1);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block6, tmp1, tmp6);
  }

  TNode<Smi> phi_bb10_8;
  TNode<BoolT> tmp8;
      TNode<Object> tmp10;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_8);
    compiler::CodeAssemblerExceptionHandlerLabel catch9__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch9__label);
    tmp8 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    }
    if (catch9__label.is_used()) {
      compiler::CodeAssemblerLabel catch9_skip(&ca_);
      ca_.Goto(&catch9_skip);
      ca_.Bind(&catch9__label, &tmp10);
      ca_.Goto(&block11, phi_bb10_8);
      ca_.Bind(&catch9_skip);
    }
    ca_.Branch(tmp8, &block8, std::vector<compiler::Node*>{phi_bb10_8}, &block9, std::vector<compiler::Node*>{phi_bb10_8});
  }

  TNode<Smi> phi_bb11_8;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_8);
    ca_.Goto(&block6, phi_bb11_8, tmp10);
  }

  TNode<Smi> phi_bb8_8;
  TNode<JSReceiver> tmp11;
      TNode<Object> tmp14;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_8);
    compiler::CodeAssemblerLabel label12(&ca_);
    compiler::CodeAssemblerExceptionHandlerLabel catch13__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch13__label);
    tmp11 = IteratorBuiltinsAssembler(state_).IteratorStep(TNode<Context>{p_context}, TorqueStructIteratorRecord{TNode<JSReceiver>{p_iteratorRecord.object}, TNode<Object>{p_iteratorRecord.next}}, TNode<Map>{tmp7}, &label12);
    }
    if (catch13__label.is_used()) {
      compiler::CodeAssemblerLabel catch13_skip(&ca_);
      ca_.Goto(&catch13_skip);
      ca_.Bind(&catch13__label, &tmp14);
      ca_.Goto(&block18, phi_bb8_8);
      ca_.Bind(&catch13_skip);
    }
    ca_.Goto(&block16, phi_bb8_8);
    if (label12.is_used()) {
      ca_.Bind(&label12);
      ca_.Goto(&block17, phi_bb8_8);
    }
  }

  TNode<Smi> phi_bb18_8;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_8);
    ca_.Goto(&block13, phi_bb18_8, tmp14);
  }

  TNode<Smi> phi_bb17_8;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_8);
    ca_.Goto(&block3, phi_bb17_8);
  }

  TNode<Smi> phi_bb16_8;
  TNode<Object> tmp15;
      TNode<Object> tmp17;
  TNode<Smi> tmp18;
      TNode<Object> tmp20;
  TNode<BoolT> tmp21;
      TNode<Object> tmp23;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_8);
    compiler::CodeAssemblerExceptionHandlerLabel catch16__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch16__label);
    tmp15 = IteratorBuiltinsAssembler(state_).IteratorValue(TNode<Context>{p_context}, TNode<JSReceiver>{tmp11}, TNode<Map>{tmp7});
    }
    if (catch16__label.is_used()) {
      compiler::CodeAssemblerLabel catch16_skip(&ca_);
      ca_.Goto(&catch16_skip);
      ca_.Bind(&catch16__label, &tmp17);
      ca_.Goto(&block19, phi_bb16_8);
      ca_.Bind(&catch16_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch19__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch19__label);
    tmp18 = FromConstexpr_Smi_constexpr_int31_0(state_, PropertyArray::HashField::kMax);
    }
    if (catch19__label.is_used()) {
      compiler::CodeAssemblerLabel catch19_skip(&ca_);
      ca_.Goto(&catch19_skip);
      ca_.Bind(&catch19__label, &tmp20);
      ca_.Goto(&block22, phi_bb16_8, phi_bb16_8, phi_bb16_8);
      ca_.Bind(&catch19_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch22__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch22__label);
    tmp21 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{phi_bb16_8}, TNode<Smi>{tmp18});
    }
    if (catch22__label.is_used()) {
      compiler::CodeAssemblerLabel catch22_skip(&ca_);
      ca_.Goto(&catch22_skip);
      ca_.Bind(&catch22__label, &tmp23);
      ca_.Goto(&block23, phi_bb16_8, phi_bb16_8);
      ca_.Bind(&catch22_skip);
    }
    ca_.Branch(tmp21, &block20, std::vector<compiler::Node*>{phi_bb16_8}, &block21, std::vector<compiler::Node*>{phi_bb16_8});
  }

  TNode<Smi> phi_bb19_8;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_8);
    ca_.Goto(&block13, phi_bb19_8, tmp17);
  }

  TNode<Smi> phi_bb13_8;
  TNode<Object> phi_bb13_11;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_8, &phi_bb13_11);
    ca_.Goto(&block1, phi_bb13_11);
  }

  TNode<Smi> phi_bb22_8;
  TNode<Smi> phi_bb22_11;
  TNode<Smi> phi_bb22_12;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_8, &phi_bb22_11, &phi_bb22_12);
    ca_.Goto(&block6, phi_bb22_8, tmp20);
  }

  TNode<Smi> phi_bb23_8;
  TNode<Smi> phi_bb23_11;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_8, &phi_bb23_11);
    ca_.Goto(&block6, phi_bb23_8, tmp23);
  }

  TNode<Smi> phi_bb20_8;
  TNode<Object> tmp24;
      TNode<Object> tmp26;
      TNode<Object> tmp28;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_8);
    compiler::CodeAssemblerExceptionHandlerLabel catch25__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch25__label);
    tmp24 = FromConstexpr_Object_constexpr_string_0(state_, "any");
    }
    if (catch25__label.is_used()) {
      compiler::CodeAssemblerLabel catch25_skip(&ca_);
      ca_.Goto(&catch25_skip);
      ca_.Bind(&catch25__label, &tmp26);
      ca_.Goto(&block24, phi_bb20_8);
      ca_.Bind(&catch25_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch27__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch27__label);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{p_context}, MessageTemplate::kTooManyElementsInPromiseCombinator, TNode<Object>{tmp24});
    }
    if (catch27__label.is_used()) {
      compiler::CodeAssemblerLabel catch27_skip(&ca_);
      ca_.Bind(&catch27__label, &tmp28);
      ca_.Goto(&block25, phi_bb20_8);
    }
  }

  TNode<Smi> phi_bb24_8;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_8);
    ca_.Goto(&block6, phi_bb24_8, tmp26);
  }

  TNode<Smi> phi_bb25_8;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_8);
    ca_.Goto(&block6, phi_bb25_8, tmp28);
  }

  TNode<Smi> phi_bb21_8;
  TNode<Object> tmp29;
      TNode<Object> tmp31;
  TNode<JSFunction> tmp32;
      TNode<Object> tmp34;
  TNode<IntPtrT> tmp35;
  TNode<Object> tmp36;
  TNode<IntPtrT> tmp37;
      TNode<Object> tmp39;
  TNode<Smi> tmp40;
  TNode<IntPtrT> tmp41;
  TNode<Object> tmp42;
  TNode<IntPtrT> tmp43;
      TNode<Object> tmp45;
  TNode<Smi> tmp46;
      TNode<Object> tmp48;
  TNode<Smi> tmp49;
      TNode<Object> tmp51;
  TNode<String> tmp52;
  TNode<Object> tmp53;
      TNode<Object> tmp55;
  TNode<IntPtrT> tmp56;
      TNode<Object> tmp58;
  TNode<Object> tmp59;
  TNode<Object> tmp60;
      TNode<Object> tmp62;
  TNode<Object> tmp63;
      TNode<Object> tmp65;
  TNode<Smi> tmp66;
      TNode<Object> tmp68;
  TNode<Smi> tmp69;
      TNode<Object> tmp71;
  TNode<BoolT> tmp72;
      TNode<Object> tmp74;
  if (block21.is_used()) {
    ca_.Bind(&block21, &phi_bb21_8);
    compiler::CodeAssemblerExceptionHandlerLabel catch30__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch30__label);
    tmp29 = CallResolve_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_constructor}, TNode<Object>{p_promiseResolveFunction}, TNode<Object>{tmp15});
    }
    if (catch30__label.is_used()) {
      compiler::CodeAssemblerLabel catch30_skip(&ca_);
      ca_.Goto(&catch30_skip);
      ca_.Bind(&catch30__label, &tmp31);
      ca_.Goto(&block26, phi_bb21_8);
      ca_.Bind(&catch30_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch33__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch33__label);
    tmp32 = CreatePromiseAnyRejectElementFunction_0(state_, TNode<Context>{p_context}, TNode<Context>{tmp0}, TNode<Smi>{phi_bb21_8}, TNode<NativeContext>{p_nativeContext});
    }
    if (catch33__label.is_used()) {
      compiler::CodeAssemblerLabel catch33_skip(&ca_);
      ca_.Goto(&catch33_skip);
      ca_.Bind(&catch33__label, &tmp34);
      ca_.Goto(&block27, phi_bb21_8, phi_bb21_8);
      ca_.Bind(&catch33_skip);
    }
    tmp35 = kPromiseAnyRejectElementRemainingSlot_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch38__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch38__label);
    std::tie(tmp36, tmp37) = ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_Smi_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp35}).Flatten();
    }
    if (catch38__label.is_used()) {
      compiler::CodeAssemblerLabel catch38_skip(&ca_);
      ca_.Goto(&catch38_skip);
      ca_.Bind(&catch38__label, &tmp39);
      ca_.Goto(&block28, phi_bb21_8);
      ca_.Bind(&catch38_skip);
    }
    tmp40 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp36, tmp37});
    tmp41 = kPromiseAnyRejectElementRemainingSlot_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch44__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch44__label);
    std::tie(tmp42, tmp43) = ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_Smi_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp41}).Flatten();
    }
    if (catch44__label.is_used()) {
      compiler::CodeAssemblerLabel catch44_skip(&ca_);
      ca_.Goto(&catch44_skip);
      ca_.Bind(&catch44__label, &tmp45);
      ca_.Goto(&block29, phi_bb21_8);
      ca_.Bind(&catch44_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch47__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch47__label);
    tmp46 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    }
    if (catch47__label.is_used()) {
      compiler::CodeAssemblerLabel catch47_skip(&ca_);
      ca_.Goto(&catch47_skip);
      ca_.Bind(&catch47__label, &tmp48);
      ca_.Goto(&block30, phi_bb21_8);
      ca_.Bind(&catch47_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch50__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch50__label);
    tmp49 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{tmp40}, TNode<Smi>{tmp46});
    }
    if (catch50__label.is_used()) {
      compiler::CodeAssemblerLabel catch50_skip(&ca_);
      ca_.Goto(&catch50_skip);
      ca_.Bind(&catch50__label, &tmp51);
      ca_.Goto(&block31, phi_bb21_8);
      ca_.Bind(&catch50_skip);
    }
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp42, tmp43}, tmp49);
    tmp52 = kThenString_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch54__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch54__label);
    tmp53 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{tmp29}, TNode<Object>{tmp52});
    }
    if (catch54__label.is_used()) {
      compiler::CodeAssemblerLabel catch54_skip(&ca_);
      ca_.Goto(&catch54_skip);
      ca_.Bind(&catch54__label, &tmp55);
      ca_.Goto(&block32, phi_bb21_8);
      ca_.Bind(&catch54_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch57__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch57__label);
    tmp56 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    }
    if (catch57__label.is_used()) {
      compiler::CodeAssemblerLabel catch57_skip(&ca_);
      ca_.Goto(&catch57_skip);
      ca_.Bind(&catch57__label, &tmp58);
      ca_.Goto(&block33, phi_bb21_8);
      ca_.Bind(&catch57_skip);
    }
    tmp59 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{p_resultCapability, tmp56});
    compiler::CodeAssemblerExceptionHandlerLabel catch61__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch61__label);
    tmp60 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp59});
    }
    if (catch61__label.is_used()) {
      compiler::CodeAssemblerLabel catch61_skip(&ca_);
      ca_.Goto(&catch61_skip);
      ca_.Bind(&catch61__label, &tmp62);
      ca_.Goto(&block34, phi_bb21_8);
      ca_.Bind(&catch61_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch64__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch64__label);
    tmp63 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp53}, TNode<Object>{tmp29}, TNode<Object>{tmp60}, TNode<Object>{tmp32});
    }
    if (catch64__label.is_used()) {
      compiler::CodeAssemblerLabel catch64_skip(&ca_);
      ca_.Goto(&catch64_skip);
      ca_.Bind(&catch64__label, &tmp65);
      ca_.Goto(&block35, phi_bb21_8);
      ca_.Bind(&catch64_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch67__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch67__label);
    tmp66 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    }
    if (catch67__label.is_used()) {
      compiler::CodeAssemblerLabel catch67_skip(&ca_);
      ca_.Goto(&catch67_skip);
      ca_.Bind(&catch67__label, &tmp68);
      ca_.Goto(&block36, phi_bb21_8, phi_bb21_8, phi_bb21_8);
      ca_.Bind(&catch67_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch70__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch70__label);
    tmp69 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb21_8}, TNode<Smi>{tmp66});
    }
    if (catch70__label.is_used()) {
      compiler::CodeAssemblerLabel catch70_skip(&ca_);
      ca_.Goto(&catch70_skip);
      ca_.Bind(&catch70__label, &tmp71);
      ca_.Goto(&block37, phi_bb21_8, phi_bb21_8);
      ca_.Bind(&catch70_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch73__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch73__label);
    tmp72 = CodeStubAssembler(state_).IsDebugActive();
    }
    if (catch73__label.is_used()) {
      compiler::CodeAssemblerLabel catch73_skip(&ca_);
      ca_.Goto(&catch73_skip);
      ca_.Bind(&catch73__label, &tmp74);
      ca_.Goto(&block40);
      ca_.Bind(&catch73_skip);
    }
    ca_.Branch(tmp72, &block41, std::vector<compiler::Node*>{}, &block42, std::vector<compiler::Node*>{});
  }

  TNode<Smi> phi_bb26_8;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_8);
    ca_.Goto(&block6, phi_bb26_8, tmp31);
  }

  TNode<Smi> phi_bb27_8;
  TNode<Smi> phi_bb27_13;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_8, &phi_bb27_13);
    ca_.Goto(&block6, phi_bb27_8, tmp34);
  }

  TNode<Smi> phi_bb28_8;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_8);
    ca_.Goto(&block6, phi_bb28_8, tmp39);
  }

  TNode<Smi> phi_bb29_8;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_8);
    ca_.Goto(&block6, phi_bb29_8, tmp45);
  }

  TNode<Smi> phi_bb30_8;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_8);
    ca_.Goto(&block6, phi_bb30_8, tmp48);
  }

  TNode<Smi> phi_bb31_8;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_8);
    ca_.Goto(&block6, phi_bb31_8, tmp51);
  }

  TNode<Smi> phi_bb32_8;
  if (block32.is_used()) {
    ca_.Bind(&block32, &phi_bb32_8);
    ca_.Goto(&block6, phi_bb32_8, tmp55);
  }

  TNode<Smi> phi_bb33_8;
  if (block33.is_used()) {
    ca_.Bind(&block33, &phi_bb33_8);
    ca_.Goto(&block6, phi_bb33_8, tmp58);
  }

  TNode<Smi> phi_bb34_8;
  if (block34.is_used()) {
    ca_.Bind(&block34, &phi_bb34_8);
    ca_.Goto(&block6, phi_bb34_8, tmp62);
  }

  TNode<Smi> phi_bb35_8;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_8);
    ca_.Goto(&block6, phi_bb35_8, tmp65);
  }

  TNode<Smi> phi_bb36_8;
  TNode<Smi> phi_bb36_16;
  TNode<Smi> phi_bb36_17;
  if (block36.is_used()) {
    ca_.Bind(&block36, &phi_bb36_8, &phi_bb36_16, &phi_bb36_17);
    ca_.Goto(&block6, phi_bb36_8, tmp68);
  }

  TNode<Smi> phi_bb37_8;
  TNode<Smi> phi_bb37_16;
  if (block37.is_used()) {
    ca_.Bind(&block37, &phi_bb37_8, &phi_bb37_16);
    ca_.Goto(&block6, phi_bb37_8, tmp71);
  }

  if (block40.is_used()) {
    ca_.Bind(&block40);
    ca_.Goto(&block6, tmp69, tmp74);
  }

  TNode<BoolT> tmp75;
      TNode<Object> tmp77;
  if (block41.is_used()) {
    ca_.Bind(&block41);
    compiler::CodeAssemblerExceptionHandlerLabel catch76__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch76__label);
    tmp75 = Is_JSPromise_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp63});
    }
    if (catch76__label.is_used()) {
      compiler::CodeAssemblerLabel catch76_skip(&ca_);
      ca_.Goto(&catch76_skip);
      ca_.Bind(&catch76__label, &tmp77);
      ca_.Goto(&block44);
      ca_.Bind(&catch76_skip);
    }
    ca_.Goto(&block43, tmp75);
  }

  if (block44.is_used()) {
    ca_.Bind(&block44);
    ca_.Goto(&block6, tmp69, tmp77);
  }

  TNode<BoolT> tmp78;
      TNode<Object> tmp80;
  if (block42.is_used()) {
    ca_.Bind(&block42);
    compiler::CodeAssemblerExceptionHandlerLabel catch79__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch79__label);
    tmp78 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    }
    if (catch79__label.is_used()) {
      compiler::CodeAssemblerLabel catch79_skip(&ca_);
      ca_.Goto(&catch79_skip);
      ca_.Bind(&catch79__label, &tmp80);
      ca_.Goto(&block45);
      ca_.Bind(&catch79_skip);
    }
    ca_.Goto(&block43, tmp78);
  }

  if (block45.is_used()) {
    ca_.Bind(&block45);
    ca_.Goto(&block6, tmp69, tmp80);
  }

  TNode<BoolT> phi_bb43_17;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_17);
    ca_.Branch(phi_bb43_17, &block38, std::vector<compiler::Node*>{}, &block39, std::vector<compiler::Node*>{});
  }

  TNode<Symbol> tmp81;
  TNode<IntPtrT> tmp82;
      TNode<Object> tmp84;
  TNode<HeapObject> tmp85;
  TNode<Object> tmp86;
      TNode<Object> tmp88;
  TNode<Symbol> tmp89;
  TNode<Oddball> tmp90;
  TNode<Object> tmp91;
      TNode<Object> tmp93;
  if (block38.is_used()) {
    ca_.Bind(&block38);
    tmp81 = kPromiseHandledBySymbol_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch83__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch83__label);
    tmp82 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    }
    if (catch83__label.is_used()) {
      compiler::CodeAssemblerLabel catch83_skip(&ca_);
      ca_.Goto(&catch83_skip);
      ca_.Bind(&catch83__label, &tmp84);
      ca_.Goto(&block46);
      ca_.Bind(&catch83_skip);
    }
    tmp85 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{p_resultCapability, tmp82});
    compiler::CodeAssemblerExceptionHandlerLabel catch87__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch87__label);
    tmp86 = CodeStubAssembler(state_).SetPropertyStrict(TNode<Context>{p_context}, TNode<Object>{tmp63}, TNode<Object>{tmp81}, TNode<Object>{tmp85});
    }
    if (catch87__label.is_used()) {
      compiler::CodeAssemblerLabel catch87_skip(&ca_);
      ca_.Goto(&catch87_skip);
      ca_.Bind(&catch87__label, &tmp88);
      ca_.Goto(&block47);
      ca_.Bind(&catch87_skip);
    }
    tmp89 = kPromiseForwardingHandlerSymbol_0(state_);
    tmp90 = True_0(state_);
    compiler::CodeAssemblerExceptionHandlerLabel catch92__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch92__label);
    tmp91 = CodeStubAssembler(state_).SetPropertyStrict(TNode<Context>{p_context}, TNode<Object>{tmp32}, TNode<Object>{tmp89}, TNode<Object>{tmp90});
    }
    if (catch92__label.is_used()) {
      compiler::CodeAssemblerLabel catch92_skip(&ca_);
      ca_.Goto(&catch92_skip);
      ca_.Bind(&catch92__label, &tmp93);
      ca_.Goto(&block48);
      ca_.Bind(&catch92_skip);
    }
    ca_.Goto(&block39);
  }

  if (block46.is_used()) {
    ca_.Bind(&block46);
    ca_.Goto(&block6, tmp69, tmp84);
  }

  if (block47.is_used()) {
    ca_.Bind(&block47);
    ca_.Goto(&block6, tmp69, tmp88);
  }

  if (block48.is_used()) {
    ca_.Bind(&block48);
    ca_.Goto(&block6, tmp69, tmp93);
  }

  if (block39.is_used()) {
    ca_.Bind(&block39);
    ca_.Goto(&block10, tmp69);
  }

  TNode<Smi> phi_bb9_8;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_8);
    ca_.Goto(&block3, phi_bb9_8);
  }

  TNode<Smi> phi_bb6_8;
  TNode<Object> phi_bb6_9;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_8, &phi_bb6_9);
    IteratorCloseOnException_0(state_, TNode<Context>{p_context}, TorqueStructIteratorRecord{TNode<JSReceiver>{p_iteratorRecord.object}, TNode<Object>{p_iteratorRecord.next}});
    ca_.Goto(&block1, phi_bb6_9);
  }

  TNode<Smi> phi_bb3_8;
  TNode<IntPtrT> tmp94;
  TNode<Object> tmp95;
  TNode<IntPtrT> tmp96;
  TNode<Smi> tmp97;
  TNode<Smi> tmp98;
  TNode<Smi> tmp99;
  TNode<Smi> tmp100;
  TNode<BoolT> tmp101;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_8);
    tmp94 = kPromiseAnyRejectElementRemainingSlot_0(state_);
    std::tie(tmp95, tmp96) = ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_Smi_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp94}).Flatten();
    tmp97 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp95, tmp96});
    tmp98 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp99 = CodeStubAssembler(state_).SmiSub(TNode<Smi>{tmp97}, TNode<Smi>{tmp98});
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp95, tmp96}, tmp99);
    tmp100 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp101 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp99}, TNode<Smi>{tmp100});
    ca_.Branch(tmp101, &block49, std::vector<compiler::Node*>{phi_bb3_8}, &block50, std::vector<compiler::Node*>{phi_bb3_8});
  }

  TNode<Smi> phi_bb49_8;
  TNode<IntPtrT> tmp102;
  TNode<Object> tmp103;
  TNode<IntPtrT> tmp104;
  TNode<FixedArray> tmp105;
  TNode<JSObject> tmp106;
  if (block49.is_used()) {
    ca_.Bind(&block49, &phi_bb49_8);
    tmp102 = kPromiseAnyRejectElementErrorsSlot_0(state_);
    std::tie(tmp103, tmp104) = ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_FixedArray_0(state_, TNode<Context>{tmp0}, TNode<IntPtrT>{tmp102}).Flatten();
    tmp105 = CodeStubAssembler(state_).LoadReference<FixedArray>(CodeStubAssembler::Reference{tmp103, tmp104});
    tmp106 = ConstructAggregateError_0(state_, TNode<Context>{p_context}, TNode<FixedArray>{tmp105});
    ca_.Goto(&block1, tmp106);
  }

  TNode<Smi> phi_bb50_8;
  TNode<IntPtrT> tmp107;
  TNode<HeapObject> tmp108;
  if (block50.is_used()) {
    ca_.Bind(&block50, &phi_bb50_8);
    tmp107 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp108 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{p_resultCapability, tmp107});
    ca_.Goto(&block51);
  }

  TNode<Object> phi_bb1_0;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_0);
    *label_Reject_parameter_0 = phi_bb1_0;
    ca_.Goto(label_Reject);
  }

    ca_.Bind(&block51);
  return TNode<Object>{tmp108};
}


TF_BUILTIN(PromiseAny, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kIterable);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block12(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block10(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    ca_.Goto(&block0);

  TNode<NativeContext> tmp0;
  TNode<JSReceiver> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{parameter0});
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_JSReceiver_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label2);
    ca_.Goto(&block3);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCalledOnNonObject, "Promise.any");
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
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = False_0(state_);
    tmp4 = ca_.CallStub<PromiseCapability>(Builtins::CallableFor(ca_.isolate(), Builtin::kNewPromiseCapability), parameter0, tmp1, tmp3);
    tmp5 = UnsafeCast_Constructor_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp1});
    compiler::CodeAssemblerExceptionHandlerLabel catch7__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch7__label);
    tmp6 = GetPromiseResolve_0(state_, TNode<Context>{parameter0}, TNode<NativeContext>{tmp0}, TNode<JSReceiver>{tmp5});
    }
    if (catch7__label.is_used()) {
      compiler::CodeAssemblerLabel catch7_skip(&ca_);
      ca_.Goto(&catch7_skip);
      ca_.Bind(&catch7__label, &tmp8);
      ca_.Goto(&block13);
      ca_.Bind(&catch7_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch11__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch11__label);
    std::tie(tmp9, tmp10) = IteratorBuiltinsAssembler(state_).GetIterator(TNode<Context>{parameter0}, TNode<Object>{parameter2}).Flatten();
    }
    if (catch11__label.is_used()) {
      compiler::CodeAssemblerLabel catch11_skip(&ca_);
      ca_.Goto(&catch11_skip);
      ca_.Bind(&catch11__label, &tmp12);
      ca_.Goto(&block14);
      ca_.Bind(&catch11_skip);
    }
    compiler::CodeAssemblerLabel label14(&ca_);
    compiler::CodeAssemblerExceptionHandlerLabel catch16__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch16__label);
    tmp13 = PerformPromiseAny_0(state_, TNode<Context>{parameter0}, TNode<NativeContext>{tmp0}, TorqueStructIteratorRecord{TNode<JSReceiver>{tmp9}, TNode<Object>{tmp10}}, TNode<JSReceiver>{tmp5}, TNode<PromiseCapability>{tmp4}, TNode<Object>{tmp6}, &label14, &tmp15);
    }
    if (catch16__label.is_used()) {
      compiler::CodeAssemblerLabel catch16_skip(&ca_);
      ca_.Goto(&catch16_skip);
      ca_.Bind(&catch16__label, &tmp17);
      ca_.Goto(&block17);
      ca_.Bind(&catch16_skip);
    }
    ca_.Goto(&block15);
    if (label14.is_used()) {
      ca_.Bind(&label14);
      ca_.Goto(&block16);
    }
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block12, tmp8);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block12, tmp12);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block12, tmp17);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block10, tmp15.value());
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    CodeStubAssembler(state_).Return(tmp13);
  }

  TNode<Object> phi_bb12_7;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_7);
    ca_.Goto(&block10, phi_bb12_7);
  }

  TNode<Object> phi_bb10_7;
  TNode<IntPtrT> tmp18;
  TNode<Object> tmp19;
  TNode<JSReceiver> tmp20;
  TNode<Oddball> tmp21;
  TNode<Object> tmp22;
  TNode<Object> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<HeapObject> tmp25;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_7);
    tmp18 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp19 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp4, tmp18});
    tmp20 = UnsafeCast_Callable_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp19});
    tmp21 = Undefined_0(state_);
    tmp22 = UnsafeCast_JSAny_0(state_, TNode<Context>{parameter0}, TNode<Object>{phi_bb10_7});
    tmp23 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp20}, TNode<Object>{tmp21}, TNode<Object>{tmp22});
    tmp24 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp25 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{tmp4, tmp24});
    CodeStubAssembler(state_).Return(tmp25);
  }
}

TNode<JSObject> ConstructAggregateError_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FixedArray> p_errors) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Smi> tmp0;
  TNode<JSObject> tmp1;
  TNode<JSArray> tmp2;
  TNode<String> tmp3;
  TNode<Smi> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).SmiConstant(MessageTemplate::kAllPromisesRejected);
    tmp1 = TORQUE_CAST(CodeStubAssembler(state_).CallRuntime(Runtime::kConstructInternalAggregateErrorHelper, p_context, tmp0)); 
    tmp2 = CreateJSArrayWithElements_0(state_, TNode<Context>{p_context}, TNode<FixedArray>{p_errors});
    tmp3 = CodeStubAssembler(state_).ErrorsStringConstant();
    tmp4 = CodeStubAssembler(state_).SmiConstant(PropertyAttributes::DONT_ENUM);
    CodeStubAssembler(state_).CallRuntime(Runtime::kSetOwnPropertyIgnoreAttributes, p_context, tmp1, tmp3, tmp2, tmp4);
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<JSObject>{tmp1};
}


void InitContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_Smi_constexpr_int31_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, int31_t p_value) {
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


void InitContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_PromiseCapability_PromiseCapability_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<PromiseCapability> p_value) {
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


void InitContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_FixedArray_EmptyFixedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index, TNode<FixedArray> p_value) {
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


TorqueStructReference_FixedArray_0 ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_FixedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
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
    std::tie(tmp10, tmp11) = ReferenceCast_FixedArray_Object_0(state_, TorqueStructReference_Object_0{TNode<Object>{tmp8}, TNode<IntPtrT>{tmp9}, TorqueStructUnsafe_0{}}).Flatten();
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TorqueStructReference_FixedArray_0{TNode<Object>{tmp10}, TNode<IntPtrT>{tmp11}, TorqueStructUnsafe_0{}};
}


TorqueStructReference_Smi_0 ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_Smi_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
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
    std::tie(tmp10, tmp11) = ReferenceCast_Smi_Object_0(state_, TorqueStructReference_Object_0{TNode<Object>{tmp8}, TNode<IntPtrT>{tmp9}, TorqueStructUnsafe_0{}}).Flatten();
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TorqueStructReference_Smi_0{TNode<Object>{tmp10}, TNode<IntPtrT>{tmp11}, TorqueStructUnsafe_0{}};
}


TorqueStructReference_PromiseCapability_0 ContextSlot_PromiseAnyRejectElementContext_PromiseAnyRejectElementContext_PromiseCapability_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
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
    std::tie(tmp10, tmp11) = ReferenceCast_PromiseCapability_Object_0(state_, TorqueStructReference_Object_0{TNode<Object>{tmp8}, TNode<IntPtrT>{tmp9}, TorqueStructUnsafe_0{}}).Flatten();
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TorqueStructReference_PromiseCapability_0{TNode<Object>{tmp10}, TNode<IntPtrT>{tmp11}, TorqueStructUnsafe_0{}};
}


TNode<BoolT> Is_JSPromise_JSAny_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSPromise> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSPromise_1(state_, TNode<Context>{p_context}, TNode<Object>{p_o}, &label1);
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


TNode<BoolT> Is_Constructor_JSReceiver_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_Constructor_0(state_, TNode<HeapObject>{p_o}, &label1);
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
