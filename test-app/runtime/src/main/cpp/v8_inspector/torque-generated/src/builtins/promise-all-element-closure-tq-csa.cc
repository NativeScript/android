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
#include "torque-generated/src/builtins/promise-all-element-closure-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/conversion-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/promise-abstract-operations-tq-csa.h"
#include "torque-generated/src/builtins/promise-all-element-closure-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/contexts-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-tq-csa.h"
#include "torque-generated/src/objects/js-objects-tq-csa.h"

namespace v8 {
namespace internal {
TNode<IntPtrT> FromConstexpr_PromiseAllResolveElementContextSlots_constexpr_kPromiseAllResolveElementRemainingSlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseAllResolveElementContextSlots p_o) {
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


TNode<IntPtrT> FromConstexpr_PromiseAllResolveElementContextSlots_constexpr_kPromiseAllResolveElementCapabilitySlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseAllResolveElementContextSlots p_o) {
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


TNode<IntPtrT> FromConstexpr_PromiseAllResolveElementContextSlots_constexpr_kPromiseAllResolveElementValuesSlot_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseAllResolveElementContextSlots p_o) {
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


TNode<IntPtrT> FromConstexpr_PromiseAllResolveElementContextSlots_constexpr_kPromiseAllResolveElementLength_0(compiler::CodeAssemblerState* state_, PromiseBuiltins::PromiseAllResolveElementContextSlots p_o) {
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


TNode<IntPtrT> kPromiseAllResolveElementRemainingSlot_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  TNode<IntPtrT> tmp0;
    tmp0 = FromConstexpr_PromiseAllResolveElementContextSlots_constexpr_kPromiseAllResolveElementRemainingSlot_0(state_, PromiseBuiltins::PromiseAllResolveElementContextSlots::kPromiseAllResolveElementRemainingSlot);
  TNode<IntPtrT> tmp1;
    tmp1 = (TNode<IntPtrT>{tmp0});
  return TNode<IntPtrT>{tmp1};}

TNode<IntPtrT> kPromiseAllResolveElementCapabilitySlot_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  TNode<IntPtrT> tmp0;
    tmp0 = FromConstexpr_PromiseAllResolveElementContextSlots_constexpr_kPromiseAllResolveElementCapabilitySlot_0(state_, PromiseBuiltins::PromiseAllResolveElementContextSlots::kPromiseAllResolveElementCapabilitySlot);
  TNode<IntPtrT> tmp1;
    tmp1 = (TNode<IntPtrT>{tmp0});
  return TNode<IntPtrT>{tmp1};}

TNode<IntPtrT> kPromiseAllResolveElementValuesSlot_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  TNode<IntPtrT> tmp0;
    tmp0 = FromConstexpr_PromiseAllResolveElementContextSlots_constexpr_kPromiseAllResolveElementValuesSlot_0(state_, PromiseBuiltins::PromiseAllResolveElementContextSlots::kPromiseAllResolveElementValuesSlot);
  TNode<IntPtrT> tmp1;
    tmp1 = (TNode<IntPtrT>{tmp0});
  return TNode<IntPtrT>{tmp1};}

TF_BUILTIN(PromiseAllResolveElementClosure, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<Context> tmp0;
  TNode<Object> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<Context>{parameter0});
    tmp1 = PromiseAllResolveElementClosure_PromiseAllWrapResultAsFulfilledFunctor_0(state_, TNode<Context>{tmp0}, TNode<Object>{parameter3}, TNode<JSFunction>{parameter2}, TorqueStructPromiseAllWrapResultAsFulfilledFunctor_0{}, false);
    CodeStubAssembler(state_).Return(tmp1);
  }
}

TF_BUILTIN(PromiseAllSettledResolveElementClosure, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<Context> tmp0;
  TNode<Object> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<Context>{parameter0});
    tmp1 = PromiseAllResolveElementClosure_PromiseAllSettledWrapResultAsFulfilledFunctor_0(state_, TNode<Context>{tmp0}, TNode<Object>{parameter3}, TNode<JSFunction>{parameter2}, TorqueStructPromiseAllSettledWrapResultAsFulfilledFunctor_0{}, true);
    CodeStubAssembler(state_).Return(tmp1);
  }
}

TF_BUILTIN(PromiseAllSettledRejectElementClosure, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<Context> tmp0;
  TNode<Object> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<Context>{parameter0});
    tmp1 = PromiseAllResolveElementClosure_PromiseAllSettledWrapResultAsRejectedFunctor_0(state_, TNode<Context>{tmp0}, TNode<Object>{parameter3}, TNode<JSFunction>{parameter2}, TorqueStructPromiseAllSettledWrapResultAsRejectedFunctor_0{}, true);
    CodeStubAssembler(state_).Return(tmp1);
  }
}

TNode<Object> PromiseAllResolveElementClosure_PromiseAllWrapResultAsFulfilledFunctor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_value, TNode<JSFunction> p_function, TorqueStructPromiseAllWrapResultAsFulfilledFunctor_0 p_wrapResultFunctor, bool p_hasResolveAndRejectClosures) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block44(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<NativeContext> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_NativeContext_0(state_, TNode<HeapObject>{p_context}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  TNode<NativeContext> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{ca_.UncheckedCast<Context>(p_context)});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    CodeStubAssembler(state_).StoreReference<Context>(CodeStubAssembler::Reference{p_function, tmp3}, tmp2);
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = CodeStubAssembler(state_).LoadJSReceiverIdentityHash(TNode<JSReceiver>{p_function}, &label5);
    ca_.Goto(&block16);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block17);
    }
  }

  TNode<Oddball> tmp6;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp6 = Undefined_0(state_);
    ca_.Goto(&block1, tmp6);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<Smi> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Object> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<FixedArray> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<IntPtrT> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<BoolT> tmp20;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp8 = CodeStubAssembler(state_).IntPtrSub(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp7});
    tmp9 = kPromiseAllResolveElementRemainingSlot_0(state_);
    std::tie(tmp10, tmp11) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp9}).Flatten();
    tmp12 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp10, tmp11});
    tmp13 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp14, tmp15) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp13}).Flatten();
    tmp16 = CodeStubAssembler(state_).LoadReference<FixedArray>(CodeStubAssembler::Reference{tmp14, tmp15});
    tmp17 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp18 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp8}, TNode<IntPtrT>{tmp17});
    tmp19 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp16});
    tmp20 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{tmp18}, TNode<IntPtrT>{tmp19});
    ca_.Branch(tmp20, &block22, std::vector<compiler::Node*>{}, &block23, std::vector<compiler::Node*>{tmp16});
  }

  TNode<IntPtrT> tmp21;
  TNode<IntPtrT> tmp22;
  TNode<FixedArray> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<Object> tmp25;
  TNode<IntPtrT> tmp26;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    tmp21 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp16});
    tmp22 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp23 = ExtractFixedArray_0(state_, TNode<FixedArray>{tmp16}, TNode<IntPtrT>{tmp22}, TNode<IntPtrT>{tmp21}, TNode<IntPtrT>{tmp18});
    tmp24 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp25, tmp26) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp24}).Flatten();
    CodeStubAssembler(state_).StoreReference<FixedArray>(CodeStubAssembler::Reference{tmp25, tmp26}, tmp23);
    ca_.Goto(&block23, tmp23);
  }

  TNode<FixedArray> phi_bb23_8;
  TNode<BoolT> tmp27;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_8);
    tmp27 = FromConstexpr_bool_constexpr_bool_0(state_, p_hasResolveAndRejectClosures);
    ca_.Branch(tmp27, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp28;
  TNode<IntPtrT> tmp29;
  TNode<IntPtrT> tmp30;
  TNode<UintPtrT> tmp31;
  TNode<UintPtrT> tmp32;
  TNode<BoolT> tmp33;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    std::tie(tmp28, tmp29, tmp30) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{phi_bb23_8}).Flatten();
    tmp31 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp8});
    tmp32 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp30});
    tmp33 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp31}, TNode<UintPtrT>{tmp32});
    ca_.Branch(tmp33, &block32, std::vector<compiler::Node*>{}, &block33, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp34;
  TNode<IntPtrT> tmp35;
  TNode<Object> tmp36;
  TNode<IntPtrT> tmp37;
  TNode<Object> tmp38;
  TNode<Oddball> tmp39;
  TNode<BoolT> tmp40;
  if (block32.is_used()) {
    ca_.Bind(&block32);
    tmp34 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp8});
    tmp35 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp29}, TNode<IntPtrT>{tmp34});
    std::tie(tmp36, tmp37) = NewReference_Object_0(state_, TNode<Object>{tmp28}, TNode<IntPtrT>{tmp35}).Flatten();
    tmp38 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp36, tmp37});
    tmp39 = TheHole_0(state_);
    tmp40 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp38}, TNode<HeapObject>{tmp39});
    ca_.Branch(tmp40, &block26, std::vector<compiler::Node*>{}, &block27, std::vector<compiler::Node*>{});
  }

  if (block33.is_used()) {
    ca_.Bind(&block33);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Oddball> tmp41;
  if (block26.is_used()) {
    ca_.Bind(&block26);
    tmp41 = Undefined_0(state_);
    ca_.Goto(&block1, tmp41);
  }

  if (block27.is_used()) {
    ca_.Bind(&block27);
    ca_.Goto(&block25);
  }

  TNode<Object> tmp42;
  TNode<IntPtrT> tmp43;
  TNode<IntPtrT> tmp44;
  TNode<UintPtrT> tmp45;
  TNode<UintPtrT> tmp46;
  TNode<BoolT> tmp47;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    std::tie(tmp42, tmp43, tmp44) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{phi_bb23_8}).Flatten();
    tmp45 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp8});
    tmp46 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp44});
    tmp47 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp45}, TNode<UintPtrT>{tmp46});
    ca_.Branch(tmp47, &block40, std::vector<compiler::Node*>{}, &block41, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp48;
  TNode<IntPtrT> tmp49;
  TNode<Object> tmp50;
  TNode<IntPtrT> tmp51;
  TNode<Smi> tmp52;
  TNode<Smi> tmp53;
  TNode<Smi> tmp54;
  TNode<BoolT> tmp55;
  if (block40.is_used()) {
    ca_.Bind(&block40);
    tmp48 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp8});
    tmp49 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp43}, TNode<IntPtrT>{tmp48});
    std::tie(tmp50, tmp51) = NewReference_Object_0(state_, TNode<Object>{tmp42}, TNode<IntPtrT>{tmp49}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp50, tmp51}, p_value);
    tmp52 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp53 = CodeStubAssembler(state_).SmiSub(TNode<Smi>{tmp12}, TNode<Smi>{tmp52});
    tmp54 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp55 = CodeStubAssembler(state_).SmiGreaterThanOrEqual(TNode<Smi>{tmp53}, TNode<Smi>{tmp54});
    ca_.Branch(tmp55, &block43, std::vector<compiler::Node*>{}, &block44, std::vector<compiler::Node*>{});
  }

  if (block41.is_used()) {
    ca_.Bind(&block41);
    CodeStubAssembler(state_).Unreachable();
  }

  if (block44.is_used()) {
    ca_.Bind(&block44);
    {
      auto pos_stack = ca_.GetMacroSourcePositionStack();
      pos_stack.push_back({"src/builtins/promise-all-element-closure.tq", 162});
      CodeStubAssembler(state_).FailAssert("Torque assert 'remainingElementsCount >= 0' failed", pos_stack);
    }
  }

  TNode<IntPtrT> tmp56;
  TNode<Object> tmp57;
  TNode<IntPtrT> tmp58;
  TNode<Smi> tmp59;
  TNode<BoolT> tmp60;
  if (block43.is_used()) {
    ca_.Bind(&block43);
    tmp56 = kPromiseAllResolveElementRemainingSlot_0(state_);
    std::tie(tmp57, tmp58) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp56}).Flatten();
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp57, tmp58}, tmp53);
    tmp59 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp60 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp53}, TNode<Smi>{tmp59});
    ca_.Branch(tmp60, &block45, std::vector<compiler::Node*>{}, &block46, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp61;
  TNode<Object> tmp62;
  TNode<IntPtrT> tmp63;
  TNode<PromiseCapability> tmp64;
  TNode<IntPtrT> tmp65;
  TNode<Object> tmp66;
  TNode<Object> tmp67;
  TNode<IntPtrT> tmp68;
  TNode<Object> tmp69;
  TNode<IntPtrT> tmp70;
  TNode<Map> tmp71;
  TNode<JSArray> tmp72;
  TNode<Oddball> tmp73;
  TNode<Object> tmp74;
  if (block45.is_used()) {
    ca_.Bind(&block45);
    tmp61 = kPromiseAllResolveElementCapabilitySlot_0(state_);
    std::tie(tmp62, tmp63) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_PromiseCapability_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp61}).Flatten();
    tmp64 = CodeStubAssembler(state_).LoadReference<PromiseCapability>(CodeStubAssembler::Reference{tmp62, tmp63});
    tmp65 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp66 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp64, tmp65});
    tmp67 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp66});
    tmp68 = JS_ARRAY_PACKED_ELEMENTS_MAP_INDEX_0(state_);
    std::tie(tmp69, tmp70) = NativeContextSlot_Map_0(state_, TNode<NativeContext>{tmp2}, TNode<IntPtrT>{tmp68}).Flatten();
    tmp71 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp69, tmp70});
    tmp72 = NewJSArray_0(state_, TNode<Context>{p_context}, TNode<Map>{tmp71}, TNode<FixedArrayBase>{phi_bb23_8});
    tmp73 = Undefined_0(state_);
    tmp74 = CodeStubAssembler(state_).Call(TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<Object>{tmp67}, TNode<Object>{tmp73}, TNode<Object>{tmp72});
    ca_.Goto(&block46);
  }

  TNode<Oddball> tmp75;
  if (block46.is_used()) {
    ca_.Bind(&block46);
    tmp75 = Undefined_0(state_);
    ca_.Goto(&block1, tmp75);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block47, phi_bb1_3);
  }

  TNode<Object> phi_bb47_3;
    ca_.Bind(&block47, &phi_bb47_3);
  return TNode<Object>{phi_bb47_3};
}


TNode<Object> PromiseAllResolveElementClosure_PromiseAllSettledWrapResultAsFulfilledFunctor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_value, TNode<JSFunction> p_function, TorqueStructPromiseAllSettledWrapResultAsFulfilledFunctor_0 p_wrapResultFunctor, bool p_hasResolveAndRejectClosures) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block44(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<NativeContext> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_NativeContext_0(state_, TNode<HeapObject>{p_context}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  TNode<NativeContext> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{ca_.UncheckedCast<Context>(p_context)});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    CodeStubAssembler(state_).StoreReference<Context>(CodeStubAssembler::Reference{p_function, tmp3}, tmp2);
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = CodeStubAssembler(state_).LoadJSReceiverIdentityHash(TNode<JSReceiver>{p_function}, &label5);
    ca_.Goto(&block16);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block17);
    }
  }

  TNode<Oddball> tmp6;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp6 = Undefined_0(state_);
    ca_.Goto(&block1, tmp6);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<Smi> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Object> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<FixedArray> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<IntPtrT> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<BoolT> tmp20;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp8 = CodeStubAssembler(state_).IntPtrSub(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp7});
    tmp9 = kPromiseAllResolveElementRemainingSlot_0(state_);
    std::tie(tmp10, tmp11) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp9}).Flatten();
    tmp12 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp10, tmp11});
    tmp13 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp14, tmp15) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp13}).Flatten();
    tmp16 = CodeStubAssembler(state_).LoadReference<FixedArray>(CodeStubAssembler::Reference{tmp14, tmp15});
    tmp17 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp18 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp8}, TNode<IntPtrT>{tmp17});
    tmp19 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp16});
    tmp20 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{tmp18}, TNode<IntPtrT>{tmp19});
    ca_.Branch(tmp20, &block22, std::vector<compiler::Node*>{}, &block23, std::vector<compiler::Node*>{tmp16});
  }

  TNode<IntPtrT> tmp21;
  TNode<IntPtrT> tmp22;
  TNode<FixedArray> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<Object> tmp25;
  TNode<IntPtrT> tmp26;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    tmp21 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp16});
    tmp22 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp23 = ExtractFixedArray_0(state_, TNode<FixedArray>{tmp16}, TNode<IntPtrT>{tmp22}, TNode<IntPtrT>{tmp21}, TNode<IntPtrT>{tmp18});
    tmp24 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp25, tmp26) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp24}).Flatten();
    CodeStubAssembler(state_).StoreReference<FixedArray>(CodeStubAssembler::Reference{tmp25, tmp26}, tmp23);
    ca_.Goto(&block23, tmp23);
  }

  TNode<FixedArray> phi_bb23_8;
  TNode<BoolT> tmp27;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_8);
    tmp27 = FromConstexpr_bool_constexpr_bool_0(state_, p_hasResolveAndRejectClosures);
    ca_.Branch(tmp27, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp28;
  TNode<IntPtrT> tmp29;
  TNode<IntPtrT> tmp30;
  TNode<UintPtrT> tmp31;
  TNode<UintPtrT> tmp32;
  TNode<BoolT> tmp33;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    std::tie(tmp28, tmp29, tmp30) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{phi_bb23_8}).Flatten();
    tmp31 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp8});
    tmp32 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp30});
    tmp33 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp31}, TNode<UintPtrT>{tmp32});
    ca_.Branch(tmp33, &block32, std::vector<compiler::Node*>{}, &block33, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp34;
  TNode<IntPtrT> tmp35;
  TNode<Object> tmp36;
  TNode<IntPtrT> tmp37;
  TNode<Object> tmp38;
  TNode<Oddball> tmp39;
  TNode<BoolT> tmp40;
  if (block32.is_used()) {
    ca_.Bind(&block32);
    tmp34 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp8});
    tmp35 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp29}, TNode<IntPtrT>{tmp34});
    std::tie(tmp36, tmp37) = NewReference_Object_0(state_, TNode<Object>{tmp28}, TNode<IntPtrT>{tmp35}).Flatten();
    tmp38 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp36, tmp37});
    tmp39 = TheHole_0(state_);
    tmp40 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp38}, TNode<HeapObject>{tmp39});
    ca_.Branch(tmp40, &block26, std::vector<compiler::Node*>{}, &block27, std::vector<compiler::Node*>{});
  }

  if (block33.is_used()) {
    ca_.Bind(&block33);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Oddball> tmp41;
  if (block26.is_used()) {
    ca_.Bind(&block26);
    tmp41 = Undefined_0(state_);
    ca_.Goto(&block1, tmp41);
  }

  if (block27.is_used()) {
    ca_.Bind(&block27);
    ca_.Goto(&block25);
  }

  TNode<IntPtrT> tmp42;
  TNode<Object> tmp43;
  TNode<IntPtrT> tmp44;
  TNode<JSFunction> tmp45;
  TNode<IntPtrT> tmp46;
  TNode<HeapObject> tmp47;
  TNode<Map> tmp48;
  TNode<JSObject> tmp49;
  TNode<String> tmp50;
  TNode<String> tmp51;
  TNode<Object> tmp52;
  TNode<String> tmp53;
  TNode<Object> tmp54;
  TNode<Object> tmp55;
  TNode<IntPtrT> tmp56;
  TNode<IntPtrT> tmp57;
  TNode<UintPtrT> tmp58;
  TNode<UintPtrT> tmp59;
  TNode<BoolT> tmp60;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp42 = OBJECT_FUNCTION_INDEX_0(state_);
    std::tie(tmp43, tmp44) = NativeContextSlot_JSFunction_0(state_, TNode<NativeContext>{tmp2}, TNode<IntPtrT>{tmp42}).Flatten();
    tmp45 = CodeStubAssembler(state_).LoadReference<JSFunction>(CodeStubAssembler::Reference{tmp43, tmp44});
    tmp46 = FromConstexpr_intptr_constexpr_int31_0(state_, 28);
    tmp47 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{tmp45, tmp46});
    tmp48 = UnsafeCast_Map_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp47});
    tmp49 = CodeStubAssembler(state_).AllocateJSObjectFromMap(TNode<Map>{tmp48});
    tmp50 = CodeStubAssembler(state_).StringConstant("status");
    tmp51 = CodeStubAssembler(state_).StringConstant("fulfilled");
    tmp52 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kFastCreateDataProperty), p_context, tmp49, tmp50, tmp51);
    tmp53 = CodeStubAssembler(state_).StringConstant("value");
    tmp54 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kFastCreateDataProperty), p_context, tmp49, tmp53, p_value);
    std::tie(tmp55, tmp56, tmp57) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{phi_bb23_8}).Flatten();
    tmp58 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp8});
    tmp59 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp57});
    tmp60 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp58}, TNode<UintPtrT>{tmp59});
    ca_.Branch(tmp60, &block40, std::vector<compiler::Node*>{}, &block41, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp61;
  TNode<IntPtrT> tmp62;
  TNode<Object> tmp63;
  TNode<IntPtrT> tmp64;
  TNode<Smi> tmp65;
  TNode<Smi> tmp66;
  TNode<Smi> tmp67;
  TNode<BoolT> tmp68;
  if (block40.is_used()) {
    ca_.Bind(&block40);
    tmp61 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp8});
    tmp62 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp56}, TNode<IntPtrT>{tmp61});
    std::tie(tmp63, tmp64) = NewReference_Object_0(state_, TNode<Object>{tmp55}, TNode<IntPtrT>{tmp62}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp63, tmp64}, tmp49);
    tmp65 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp66 = CodeStubAssembler(state_).SmiSub(TNode<Smi>{tmp12}, TNode<Smi>{tmp65});
    tmp67 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp68 = CodeStubAssembler(state_).SmiGreaterThanOrEqual(TNode<Smi>{tmp66}, TNode<Smi>{tmp67});
    ca_.Branch(tmp68, &block43, std::vector<compiler::Node*>{}, &block44, std::vector<compiler::Node*>{});
  }

  if (block41.is_used()) {
    ca_.Bind(&block41);
    CodeStubAssembler(state_).Unreachable();
  }

  if (block44.is_used()) {
    ca_.Bind(&block44);
    {
      auto pos_stack = ca_.GetMacroSourcePositionStack();
      pos_stack.push_back({"src/builtins/promise-all-element-closure.tq", 162});
      CodeStubAssembler(state_).FailAssert("Torque assert 'remainingElementsCount >= 0' failed", pos_stack);
    }
  }

  TNode<IntPtrT> tmp69;
  TNode<Object> tmp70;
  TNode<IntPtrT> tmp71;
  TNode<Smi> tmp72;
  TNode<BoolT> tmp73;
  if (block43.is_used()) {
    ca_.Bind(&block43);
    tmp69 = kPromiseAllResolveElementRemainingSlot_0(state_);
    std::tie(tmp70, tmp71) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp69}).Flatten();
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp70, tmp71}, tmp66);
    tmp72 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp73 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp66}, TNode<Smi>{tmp72});
    ca_.Branch(tmp73, &block45, std::vector<compiler::Node*>{}, &block46, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp74;
  TNode<Object> tmp75;
  TNode<IntPtrT> tmp76;
  TNode<PromiseCapability> tmp77;
  TNode<IntPtrT> tmp78;
  TNode<Object> tmp79;
  TNode<Object> tmp80;
  TNode<IntPtrT> tmp81;
  TNode<Object> tmp82;
  TNode<IntPtrT> tmp83;
  TNode<Map> tmp84;
  TNode<JSArray> tmp85;
  TNode<Oddball> tmp86;
  TNode<Object> tmp87;
  if (block45.is_used()) {
    ca_.Bind(&block45);
    tmp74 = kPromiseAllResolveElementCapabilitySlot_0(state_);
    std::tie(tmp75, tmp76) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_PromiseCapability_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp74}).Flatten();
    tmp77 = CodeStubAssembler(state_).LoadReference<PromiseCapability>(CodeStubAssembler::Reference{tmp75, tmp76});
    tmp78 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp79 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp77, tmp78});
    tmp80 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp79});
    tmp81 = JS_ARRAY_PACKED_ELEMENTS_MAP_INDEX_0(state_);
    std::tie(tmp82, tmp83) = NativeContextSlot_Map_0(state_, TNode<NativeContext>{tmp2}, TNode<IntPtrT>{tmp81}).Flatten();
    tmp84 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp82, tmp83});
    tmp85 = NewJSArray_0(state_, TNode<Context>{p_context}, TNode<Map>{tmp84}, TNode<FixedArrayBase>{phi_bb23_8});
    tmp86 = Undefined_0(state_);
    tmp87 = CodeStubAssembler(state_).Call(TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<Object>{tmp80}, TNode<Object>{tmp86}, TNode<Object>{tmp85});
    ca_.Goto(&block46);
  }

  TNode<Oddball> tmp88;
  if (block46.is_used()) {
    ca_.Bind(&block46);
    tmp88 = Undefined_0(state_);
    ca_.Goto(&block1, tmp88);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block47, phi_bb1_3);
  }

  TNode<Object> phi_bb47_3;
    ca_.Bind(&block47, &phi_bb47_3);
  return TNode<Object>{phi_bb47_3};
}


TNode<Object> PromiseAllResolveElementClosure_PromiseAllSettledWrapResultAsRejectedFunctor_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_value, TNode<JSFunction> p_function, TorqueStructPromiseAllSettledWrapResultAsRejectedFunctor_0 p_wrapResultFunctor, bool p_hasResolveAndRejectClosures) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block44(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<NativeContext> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_NativeContext_0(state_, TNode<HeapObject>{p_context}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  TNode<NativeContext> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{ca_.UncheckedCast<Context>(p_context)});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    CodeStubAssembler(state_).StoreReference<Context>(CodeStubAssembler::Reference{p_function, tmp3}, tmp2);
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = CodeStubAssembler(state_).LoadJSReceiverIdentityHash(TNode<JSReceiver>{p_function}, &label5);
    ca_.Goto(&block16);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block17);
    }
  }

  TNode<Oddball> tmp6;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp6 = Undefined_0(state_);
    ca_.Goto(&block1, tmp6);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<Smi> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Object> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<FixedArray> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<IntPtrT> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<BoolT> tmp20;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp8 = CodeStubAssembler(state_).IntPtrSub(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp7});
    tmp9 = kPromiseAllResolveElementRemainingSlot_0(state_);
    std::tie(tmp10, tmp11) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp9}).Flatten();
    tmp12 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp10, tmp11});
    tmp13 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp14, tmp15) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp13}).Flatten();
    tmp16 = CodeStubAssembler(state_).LoadReference<FixedArray>(CodeStubAssembler::Reference{tmp14, tmp15});
    tmp17 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp18 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp8}, TNode<IntPtrT>{tmp17});
    tmp19 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp16});
    tmp20 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{tmp18}, TNode<IntPtrT>{tmp19});
    ca_.Branch(tmp20, &block22, std::vector<compiler::Node*>{}, &block23, std::vector<compiler::Node*>{tmp16});
  }

  TNode<IntPtrT> tmp21;
  TNode<IntPtrT> tmp22;
  TNode<FixedArray> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<Object> tmp25;
  TNode<IntPtrT> tmp26;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    tmp21 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp16});
    tmp22 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp23 = ExtractFixedArray_0(state_, TNode<FixedArray>{tmp16}, TNode<IntPtrT>{tmp22}, TNode<IntPtrT>{tmp21}, TNode<IntPtrT>{tmp18});
    tmp24 = kPromiseAllResolveElementValuesSlot_0(state_);
    std::tie(tmp25, tmp26) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp24}).Flatten();
    CodeStubAssembler(state_).StoreReference<FixedArray>(CodeStubAssembler::Reference{tmp25, tmp26}, tmp23);
    ca_.Goto(&block23, tmp23);
  }

  TNode<FixedArray> phi_bb23_8;
  TNode<BoolT> tmp27;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_8);
    tmp27 = FromConstexpr_bool_constexpr_bool_0(state_, p_hasResolveAndRejectClosures);
    ca_.Branch(tmp27, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp28;
  TNode<IntPtrT> tmp29;
  TNode<IntPtrT> tmp30;
  TNode<UintPtrT> tmp31;
  TNode<UintPtrT> tmp32;
  TNode<BoolT> tmp33;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    std::tie(tmp28, tmp29, tmp30) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{phi_bb23_8}).Flatten();
    tmp31 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp8});
    tmp32 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp30});
    tmp33 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp31}, TNode<UintPtrT>{tmp32});
    ca_.Branch(tmp33, &block32, std::vector<compiler::Node*>{}, &block33, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp34;
  TNode<IntPtrT> tmp35;
  TNode<Object> tmp36;
  TNode<IntPtrT> tmp37;
  TNode<Object> tmp38;
  TNode<Oddball> tmp39;
  TNode<BoolT> tmp40;
  if (block32.is_used()) {
    ca_.Bind(&block32);
    tmp34 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp8});
    tmp35 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp29}, TNode<IntPtrT>{tmp34});
    std::tie(tmp36, tmp37) = NewReference_Object_0(state_, TNode<Object>{tmp28}, TNode<IntPtrT>{tmp35}).Flatten();
    tmp38 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp36, tmp37});
    tmp39 = TheHole_0(state_);
    tmp40 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp38}, TNode<HeapObject>{tmp39});
    ca_.Branch(tmp40, &block26, std::vector<compiler::Node*>{}, &block27, std::vector<compiler::Node*>{});
  }

  if (block33.is_used()) {
    ca_.Bind(&block33);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Oddball> tmp41;
  if (block26.is_used()) {
    ca_.Bind(&block26);
    tmp41 = Undefined_0(state_);
    ca_.Goto(&block1, tmp41);
  }

  if (block27.is_used()) {
    ca_.Bind(&block27);
    ca_.Goto(&block25);
  }

  TNode<IntPtrT> tmp42;
  TNode<Object> tmp43;
  TNode<IntPtrT> tmp44;
  TNode<JSFunction> tmp45;
  TNode<IntPtrT> tmp46;
  TNode<HeapObject> tmp47;
  TNode<Map> tmp48;
  TNode<JSObject> tmp49;
  TNode<String> tmp50;
  TNode<String> tmp51;
  TNode<Object> tmp52;
  TNode<String> tmp53;
  TNode<Object> tmp54;
  TNode<Object> tmp55;
  TNode<IntPtrT> tmp56;
  TNode<IntPtrT> tmp57;
  TNode<UintPtrT> tmp58;
  TNode<UintPtrT> tmp59;
  TNode<BoolT> tmp60;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp42 = OBJECT_FUNCTION_INDEX_0(state_);
    std::tie(tmp43, tmp44) = NativeContextSlot_JSFunction_0(state_, TNode<NativeContext>{tmp2}, TNode<IntPtrT>{tmp42}).Flatten();
    tmp45 = CodeStubAssembler(state_).LoadReference<JSFunction>(CodeStubAssembler::Reference{tmp43, tmp44});
    tmp46 = FromConstexpr_intptr_constexpr_int31_0(state_, 28);
    tmp47 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{tmp45, tmp46});
    tmp48 = UnsafeCast_Map_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp47});
    tmp49 = CodeStubAssembler(state_).AllocateJSObjectFromMap(TNode<Map>{tmp48});
    tmp50 = CodeStubAssembler(state_).StringConstant("status");
    tmp51 = CodeStubAssembler(state_).StringConstant("rejected");
    tmp52 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kFastCreateDataProperty), p_context, tmp49, tmp50, tmp51);
    tmp53 = CodeStubAssembler(state_).StringConstant("reason");
    tmp54 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kFastCreateDataProperty), p_context, tmp49, tmp53, p_value);
    std::tie(tmp55, tmp56, tmp57) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{phi_bb23_8}).Flatten();
    tmp58 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp8});
    tmp59 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp57});
    tmp60 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp58}, TNode<UintPtrT>{tmp59});
    ca_.Branch(tmp60, &block40, std::vector<compiler::Node*>{}, &block41, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp61;
  TNode<IntPtrT> tmp62;
  TNode<Object> tmp63;
  TNode<IntPtrT> tmp64;
  TNode<Smi> tmp65;
  TNode<Smi> tmp66;
  TNode<Smi> tmp67;
  TNode<BoolT> tmp68;
  if (block40.is_used()) {
    ca_.Bind(&block40);
    tmp61 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp8});
    tmp62 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp56}, TNode<IntPtrT>{tmp61});
    std::tie(tmp63, tmp64) = NewReference_Object_0(state_, TNode<Object>{tmp55}, TNode<IntPtrT>{tmp62}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp63, tmp64}, tmp49);
    tmp65 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp66 = CodeStubAssembler(state_).SmiSub(TNode<Smi>{tmp12}, TNode<Smi>{tmp65});
    tmp67 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp68 = CodeStubAssembler(state_).SmiGreaterThanOrEqual(TNode<Smi>{tmp66}, TNode<Smi>{tmp67});
    ca_.Branch(tmp68, &block43, std::vector<compiler::Node*>{}, &block44, std::vector<compiler::Node*>{});
  }

  if (block41.is_used()) {
    ca_.Bind(&block41);
    CodeStubAssembler(state_).Unreachable();
  }

  if (block44.is_used()) {
    ca_.Bind(&block44);
    {
      auto pos_stack = ca_.GetMacroSourcePositionStack();
      pos_stack.push_back({"src/builtins/promise-all-element-closure.tq", 162});
      CodeStubAssembler(state_).FailAssert("Torque assert 'remainingElementsCount >= 0' failed", pos_stack);
    }
  }

  TNode<IntPtrT> tmp69;
  TNode<Object> tmp70;
  TNode<IntPtrT> tmp71;
  TNode<Smi> tmp72;
  TNode<BoolT> tmp73;
  if (block43.is_used()) {
    ca_.Bind(&block43);
    tmp69 = kPromiseAllResolveElementRemainingSlot_0(state_);
    std::tie(tmp70, tmp71) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp69}).Flatten();
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp70, tmp71}, tmp66);
    tmp72 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp73 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp66}, TNode<Smi>{tmp72});
    ca_.Branch(tmp73, &block45, std::vector<compiler::Node*>{}, &block46, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp74;
  TNode<Object> tmp75;
  TNode<IntPtrT> tmp76;
  TNode<PromiseCapability> tmp77;
  TNode<IntPtrT> tmp78;
  TNode<Object> tmp79;
  TNode<Object> tmp80;
  TNode<IntPtrT> tmp81;
  TNode<Object> tmp82;
  TNode<IntPtrT> tmp83;
  TNode<Map> tmp84;
  TNode<JSArray> tmp85;
  TNode<Oddball> tmp86;
  TNode<Object> tmp87;
  if (block45.is_used()) {
    ca_.Bind(&block45);
    tmp74 = kPromiseAllResolveElementCapabilitySlot_0(state_);
    std::tie(tmp75, tmp76) = ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_PromiseCapability_0(state_, TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<IntPtrT>{tmp74}).Flatten();
    tmp77 = CodeStubAssembler(state_).LoadReference<PromiseCapability>(CodeStubAssembler::Reference{tmp75, tmp76});
    tmp78 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp79 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp77, tmp78});
    tmp80 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp79});
    tmp81 = JS_ARRAY_PACKED_ELEMENTS_MAP_INDEX_0(state_);
    std::tie(tmp82, tmp83) = NativeContextSlot_Map_0(state_, TNode<NativeContext>{tmp2}, TNode<IntPtrT>{tmp81}).Flatten();
    tmp84 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp82, tmp83});
    tmp85 = NewJSArray_0(state_, TNode<Context>{p_context}, TNode<Map>{tmp84}, TNode<FixedArrayBase>{phi_bb23_8});
    tmp86 = Undefined_0(state_);
    tmp87 = CodeStubAssembler(state_).Call(TNode<Context>{ca_.UncheckedCast<Context>(p_context)}, TNode<Object>{tmp80}, TNode<Object>{tmp86}, TNode<Object>{tmp85});
    ca_.Goto(&block46);
  }

  TNode<Oddball> tmp88;
  if (block46.is_used()) {
    ca_.Bind(&block46);
    tmp88 = Undefined_0(state_);
    ca_.Goto(&block1, tmp88);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block47, phi_bb1_3);
  }

  TNode<Object> phi_bb47_3;
    ca_.Bind(&block47, &phi_bb47_3);
  return TNode<Object>{phi_bb47_3};
}


TorqueStructReference_Smi_0 ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_Smi_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
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


TorqueStructReference_FixedArray_0 ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_FixedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
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


TorqueStructReference_PromiseCapability_0 ContextSlot_PromiseAllResolveElementContext_PromiseAllResolveElementContext_PromiseCapability_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
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


} // namespace internal
} // namespace v8
