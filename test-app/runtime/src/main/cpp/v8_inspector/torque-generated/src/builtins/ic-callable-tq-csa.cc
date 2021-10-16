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
#include "torque-generated/src/builtins/ic-callable-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frames-tq-csa.h"
#include "torque-generated/src/builtins/ic-callable-tq-csa.h"
#include "torque-generated/src/builtins/ic-tq-csa.h"
#include "torque-generated/src/objects/feedback-vector-tq-csa.h"
#include "torque-generated/src/objects/js-function-tq-csa.h"

namespace v8 {
namespace internal {
TNode<BoolT> IsMonomorphic_0(compiler::CodeAssemblerState* state_, TNode<MaybeObject> p_feedback, TNode<Object> p_target) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).IsWeakReferenceToObject(TNode<MaybeObject>{p_feedback}, TNode<Object>{p_target});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<BoolT>{tmp0};
}


TNode<BoolT> InSameNativeContext_0(compiler::CodeAssemblerState* state_, TNode<Context> p_lhs, TNode<Context> p_rhs) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<NativeContext> tmp0;
  TNode<NativeContext> tmp1;
  TNode<BoolT> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{p_lhs});
    tmp1 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{p_rhs});
    tmp2 = CodeStubAssembler(state_).TaggedEqual(TNode<HeapObject>{tmp0}, TNode<HeapObject>{tmp1});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<BoolT>{tmp2};
}


TNode<HeapObject> MaybeObjectToStrong_0(compiler::CodeAssemblerState* state_, TNode<MaybeObject> p_maybeObject, compiler::CodeAssemblerLabel* label_IfCleared) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<MaybeObject> tmp0;
  TNode<HeapObject> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<MaybeObject>{p_maybeObject});
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = WeakToStrong_HeapObject_0(state_, TNode<MaybeObject>{tmp0}, &label2);
    ca_.Goto(&block7);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block8);
    }
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(label_IfCleared);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block9);
  }

    ca_.Bind(&block9);
  return TNode<HeapObject>{tmp1};
}


void TryInitializeAsMonomorphic_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_maybeTarget, TNode<FeedbackVector> p_feedbackVector, TNode<UintPtrT> p_slotId, compiler::CodeAssemblerLabel* label_TransitionToMegamorphic) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<HeapObject> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<HeapObject> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<HeapObject> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<HeapObject, HeapObject> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<HeapObject, HeapObject> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<HeapObject> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<HeapObject> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<HeapObject> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_HeapObject_1(state_, TNode<Context>{p_context}, TNode<Object>{p_maybeTarget}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block1);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block7, tmp0);
  }

  TNode<HeapObject> phi_bb7_5;
  TNode<BoolT> tmp2;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_5);
    tmp2 = Is_JSBoundFunction_HeapObject_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{phi_bb7_5});
    ca_.Branch(tmp2, &block5, std::vector<compiler::Node*>{phi_bb7_5}, &block6, std::vector<compiler::Node*>{phi_bb7_5});
  }

  TNode<HeapObject> phi_bb5_5;
  TNode<JSBoundFunction> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<JSReceiver> tmp5;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_5);
    tmp3 = UnsafeCast_JSBoundFunction_0(state_, TNode<Context>{p_context}, TNode<Object>{phi_bb5_5});
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp5 = CodeStubAssembler(state_).LoadReference<JSReceiver>(CodeStubAssembler::Reference{tmp3, tmp4});
    ca_.Goto(&block7, tmp5);
  }

  TNode<HeapObject> phi_bb6_5;
  TNode<JSFunction> tmp6;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_5);
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = Cast_JSFunction_0(state_, TNode<HeapObject>{phi_bb6_5}, &label7);
    ca_.Goto(&block8, phi_bb6_5, phi_bb6_5);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block9, phi_bb6_5, phi_bb6_5);
    }
  }

  TNode<HeapObject> phi_bb9_5;
  TNode<HeapObject> phi_bb9_6;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_5, &phi_bb9_6);
    ca_.Goto(&block1);
  }

  TNode<HeapObject> phi_bb8_5;
  TNode<HeapObject> phi_bb8_6;
  TNode<IntPtrT> tmp8;
  TNode<Context> tmp9;
  TNode<BoolT> tmp10;
  TNode<BoolT> tmp11;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_5, &phi_bb8_6);
    tmp8 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp9 = CodeStubAssembler(state_).LoadReference<Context>(CodeStubAssembler::Reference{tmp6, tmp8});
    tmp10 = InSameNativeContext_0(state_, TNode<Context>{tmp9}, TNode<Context>{p_context});
    tmp11 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp10});
    ca_.Branch(tmp11, &block10, std::vector<compiler::Node*>{phi_bb8_5}, &block11, std::vector<compiler::Node*>{phi_bb8_5});
  }

  TNode<HeapObject> phi_bb10_5;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_5);
    ca_.Goto(&block1);
  }

  TNode<HeapObject> phi_bb11_5;
  TNode<MaybeObject> tmp12;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_5);
    tmp12 = CodeStubAssembler(state_).StoreWeakReferenceInFeedbackVector(TNode<FeedbackVector>{p_feedbackVector}, TNode<UintPtrT>{p_slotId}, TNode<HeapObject>{tmp0});
    CodeStubAssembler(state_).ReportFeedbackUpdate(TNode<FeedbackVector>{p_feedbackVector}, TNode<UintPtrT>{p_slotId}, "Call:Initialize");
    ca_.Goto(&block12);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(label_TransitionToMegamorphic);
  }

    ca_.Bind(&block12);
}


void TransitionToMegamorphic_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FeedbackVector> p_feedbackVector, TNode<UintPtrT> p_slotId) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Symbol> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kMegamorphicSymbol_0(state_);
    CodeStubAssembler(state_).StoreFeedbackVectorSlot(TNode<FeedbackVector>{p_feedbackVector}, TNode<UintPtrT>{p_slotId}, TNode<MaybeObject>{tmp0});
    CodeStubAssembler(state_).ReportFeedbackUpdate(TNode<FeedbackVector>{p_feedbackVector}, TNode<UintPtrT>{p_slotId}, "Call:TransitionMegamorphic");
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
}


TNode<BoolT> TaggedEqualPrototypeApplyFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_target) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSFunction> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = GetPrototypeApplyFunction_0(state_, TNode<Context>{p_context});
    tmp1 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{p_target}, TNode<HeapObject>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<BoolT>{tmp1};
}


TNode<BoolT> FeedbackValueIsReceiver_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FeedbackVector> p_feedbackVector, TNode<UintPtrT> p_slotId) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<MaybeObject> tmp0;
  TNode<Smi> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadFeedbackVectorSlot(TNode<FeedbackVector>{p_feedbackVector}, TNode<UintPtrT>{p_slotId}, kTaggedSize);
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_Smi_1(state_, TNode<Context>{p_context}, TNode<MaybeObject>{tmp0}, &label2);
    ca_.Goto(&block4);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block5);
    }
  }

  TNode<BoolT> tmp3;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp3 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp3);
  }

  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<BoolT> tmp8;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp4 = CodeStubAssembler(state_).SmiUntag(TNode<Smi>{tmp1});
    tmp5 = CodeStubAssembler(state_).IntPtrConstant(FeedbackNexus::CallFeedbackContentField::kMask);
    tmp6 = CodeStubAssembler(state_).WordAnd(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp5});
    tmp7 = CodeStubAssembler(state_).IntPtrConstant(0);
    tmp8 = CodeStubAssembler(state_).WordNotEqual(TNode<IntPtrT>{tmp6}, TNode<IntPtrT>{tmp7});
    ca_.Goto(&block1, tmp8);
  }

  TNode<BoolT> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<BoolT>{phi_bb1_3};
}


void SetCallFeedbackContent_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FeedbackVector> p_feedbackVector, TNode<UintPtrT> p_slotId, CallFeedbackContent p_callFeedbackContent) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<MaybeObject> tmp0;
  TNode<Smi> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadFeedbackVectorSlot(TNode<FeedbackVector>{p_feedbackVector}, TNode<UintPtrT>{p_slotId}, kTaggedSize);
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_Smi_1(state_, TNode<Context>{p_context}, TNode<MaybeObject>{tmp0}, &label2);
    ca_.Goto(&block4);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block1);
  }

  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Uint32T> tmp7;
  TNode<Uint32T> tmp8;
  TNode<Uint32T> tmp9;
  TNode<Int32T> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<Smi> tmp13;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp3 = CodeStubAssembler(state_).SmiUntag(TNode<Smi>{tmp1});
    tmp4 = CodeStubAssembler(state_).IntPtrConstant(FeedbackNexus::CallFeedbackContentField::kMask);
    tmp5 = CodeStubAssembler(state_).WordNot(TNode<IntPtrT>{tmp4});
    tmp6 = CodeStubAssembler(state_).WordAnd(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp5});
    tmp7 = FromConstexpr_uint32_constexpr_uint32_0(state_, static_cast<uint32_t>(p_callFeedbackContent));
    tmp8 = FromConstexpr_uint32_constexpr_uint32_0(state_, FeedbackNexus::CallFeedbackContentField::kShift);
    tmp9 = CodeStubAssembler(state_).Word32Shl(TNode<Uint32T>{tmp7}, TNode<Uint32T>{tmp8});
    tmp10 = CodeStubAssembler(state_).Signed(TNode<Uint32T>{tmp9});
    tmp11 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp10});
    tmp12 = CodeStubAssembler(state_).WordOr(TNode<IntPtrT>{tmp6}, TNode<IntPtrT>{tmp11});
    tmp13 = CodeStubAssembler(state_).SmiTag(TNode<IntPtrT>{tmp12});
    CodeStubAssembler(state_).StoreFeedbackVectorSlot(TNode<FeedbackVector>{p_feedbackVector}, TNode<UintPtrT>{p_slotId}, TNode<MaybeObject>{tmp13}, SKIP_WRITE_BARRIER, kTaggedSize);
    CodeStubAssembler(state_).ReportFeedbackUpdate(TNode<FeedbackVector>{p_feedbackVector}, TNode<UintPtrT>{p_slotId}, "Call:SetCallFeedbackContent");
    ca_.Goto(&block1);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
}


void CollectCallFeedback_0(compiler::CodeAssemblerState* state_, TNode<Object> p_maybeTarget, std::function<TNode<Object>()> p_maybeReceiver, TNode<Context> p_context, TNode<HeapObject> p_maybeFeedbackVector, TNode<UintPtrT> p_slotId) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block39(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block53(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block52(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block54(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FeedbackVector> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_FeedbackVector_0(state_, TNode<HeapObject>{p_maybeFeedbackVector}, &label1);
    ca_.Goto(&block11);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block12);
    }
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    ca_.Goto(&block1);
  }

  TNode<MaybeObject> tmp2;
  TNode<BoolT> tmp3;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    CodeStubAssembler(state_).IncrementCallCount(TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    tmp2 = CodeStubAssembler(state_).LoadFeedbackVectorSlot(TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    tmp3 = IsMonomorphic_0(state_, TNode<MaybeObject>{tmp2}, TNode<Object>{p_maybeTarget});
    ca_.Branch(tmp3, &block19, std::vector<compiler::Node*>{}, &block20, std::vector<compiler::Node*>{});
  }

  if (block19.is_used()) {
    ca_.Bind(&block19);
    ca_.Goto(&block1);
  }

  TNode<BoolT> tmp4;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp4 = IsMegamorphic_0(state_, TNode<MaybeObject>{tmp2});
    ca_.Branch(tmp4, &block21, std::vector<compiler::Node*>{}, &block22, std::vector<compiler::Node*>{});
  }

  if (block21.is_used()) {
    ca_.Bind(&block21);
    ca_.Goto(&block1);
  }

  TNode<BoolT> tmp5;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    tmp5 = IsUninitialized_0(state_, TNode<MaybeObject>{tmp2});
    ca_.Branch(tmp5, &block23, std::vector<compiler::Node*>{}, &block24, std::vector<compiler::Node*>{});
  }

  if (block23.is_used()) {
    ca_.Bind(&block23);
    ca_.Goto(&block16);
  }

  TNode<HeapObject> tmp6;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = MaybeObjectToStrong_0(state_, TNode<MaybeObject>{tmp2}, &label7);
    ca_.Goto(&block25);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block26);
    }
  }

  if (block26.is_used()) {
    ca_.Bind(&block26);
    SetCallFeedbackContent_0(state_, TNode<Context>{p_context}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, CallFeedbackContent::kTarget);
    ca_.Goto(&block16);
  }

  TNode<BoolT> tmp8;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp8 = FeedbackValueIsReceiver_0(state_, TNode<Context>{p_context}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    ca_.Branch(tmp8, &block29, std::vector<compiler::Node*>{}, &block30, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp9;
  if (block29.is_used()) {
    ca_.Bind(&block29);
    tmp9 = TaggedEqualPrototypeApplyFunction_0(state_, TNode<Context>{p_context}, TNode<Object>{p_maybeTarget});
    ca_.Goto(&block31, tmp9);
  }

  TNode<BoolT> tmp10;
  if (block30.is_used()) {
    ca_.Bind(&block30);
    tmp10 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block31, tmp10);
  }

  TNode<BoolT> phi_bb31_9;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_9);
    ca_.Branch(phi_bb31_9, &block27, std::vector<compiler::Node*>{}, &block28, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp11;
  TNode<BoolT> tmp12;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp11 = CodeStubAssembler(state_).RunLazy(std::function<TNode<Object>()>{p_maybeReceiver});
    tmp12 = IsMonomorphic_0(state_, TNode<MaybeObject>{tmp2}, TNode<Object>{tmp11});
    ca_.Branch(tmp12, &block32, std::vector<compiler::Node*>{}, &block33, std::vector<compiler::Node*>{});
  }

  if (block32.is_used()) {
    ca_.Bind(&block32);
    ca_.Goto(&block1);
  }

  if (block33.is_used()) {
    ca_.Bind(&block33);
    SetCallFeedbackContent_0(state_, TNode<Context>{p_context}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, CallFeedbackContent::kTarget);
    compiler::CodeAssemblerLabel label13(&ca_);
    TryInitializeAsMonomorphic_0(state_, TNode<Context>{p_context}, TNode<Object>{p_maybeTarget}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, &label13);
    ca_.Goto(&block35);
    if (label13.is_used()) {
      ca_.Bind(&label13);
      ca_.Goto(&block36);
    }
  }

  if (block36.is_used()) {
    ca_.Bind(&block36);
    ca_.Goto(&block14);
  }

  if (block35.is_used()) {
    ca_.Bind(&block35);
    ca_.Goto(&block1);
  }

  TNode<JSFunction> tmp14;
  if (block28.is_used()) {
    ca_.Bind(&block28);
    compiler::CodeAssemblerLabel label15(&ca_);
    tmp14 = Cast_JSFunction_1(state_, TNode<Context>{p_context}, TNode<Object>{p_maybeTarget}, &label15);
    ca_.Goto(&block37);
    if (label15.is_used()) {
      ca_.Bind(&label15);
      ca_.Goto(&block38);
    }
  }

  if (block38.is_used()) {
    ca_.Bind(&block38);
    ca_.Goto(&block14);
  }

  TNode<IntPtrT> tmp16;
  TNode<FeedbackCell> tmp17;
  TNode<BoolT> tmp18;
  if (block37.is_used()) {
    ca_.Bind(&block37);
    tmp16 = FromConstexpr_intptr_constexpr_int31_0(state_, 20);
    tmp17 = CodeStubAssembler(state_).LoadReference<FeedbackCell>(CodeStubAssembler::Reference{tmp14, tmp16});
    tmp18 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{tmp6}, TNode<MaybeObject>{tmp17});
    ca_.Branch(tmp18, &block39, std::vector<compiler::Node*>{}, &block40, std::vector<compiler::Node*>{});
  }

  if (block39.is_used()) {
    ca_.Bind(&block39);
    ca_.Goto(&block1);
  }

  TNode<JSFunction> tmp19;
  if (block40.is_used()) {
    ca_.Bind(&block40);
    compiler::CodeAssemblerLabel label20(&ca_);
    tmp19 = Cast_JSFunction_0(state_, TNode<HeapObject>{tmp6}, &label20);
    ca_.Goto(&block41);
    if (label20.is_used()) {
      ca_.Bind(&label20);
      ca_.Goto(&block42);
    }
  }

  if (block42.is_used()) {
    ca_.Bind(&block42);
    ca_.Goto(&block14);
  }

  TNode<IntPtrT> tmp21;
  TNode<FeedbackCell> tmp22;
  TNode<BoolT> tmp23;
  TNode<BoolT> tmp24;
  if (block41.is_used()) {
    ca_.Bind(&block41);
    tmp21 = FromConstexpr_intptr_constexpr_int31_0(state_, 20);
    tmp22 = CodeStubAssembler(state_).LoadReference<FeedbackCell>(CodeStubAssembler::Reference{tmp19, tmp21});
    tmp23 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{tmp22}, TNode<MaybeObject>{tmp17});
    tmp24 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp23});
    ca_.Branch(tmp24, &block43, std::vector<compiler::Node*>{}, &block44, std::vector<compiler::Node*>{});
  }

  if (block43.is_used()) {
    ca_.Bind(&block43);
    ca_.Goto(&block14);
  }

  TNode<MaybeObject> tmp25;
  if (block44.is_used()) {
    ca_.Bind(&block44);
    tmp25 = CodeStubAssembler(state_).StoreWeakReferenceInFeedbackVector(TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, TNode<HeapObject>{tmp22});
    CodeStubAssembler(state_).ReportFeedbackUpdate(TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, "Call:FeedbackVectorCell");
    ca_.Goto(&block15);
  }

  TNode<BoolT> tmp26;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp26 = TaggedEqualPrototypeApplyFunction_0(state_, TNode<Context>{p_context}, TNode<Object>{p_maybeTarget});
    ca_.Branch(tmp26, &block45, std::vector<compiler::Node*>{}, &block46, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp27;
  if (block45.is_used()) {
    ca_.Bind(&block45);
    tmp27 = CodeStubAssembler(state_).RunLazy(std::function<TNode<Object>()>{p_maybeReceiver});
    SetCallFeedbackContent_0(state_, TNode<Context>{p_context}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, CallFeedbackContent::kReceiver);
    ca_.Goto(&block47, tmp27);
  }

  if (block46.is_used()) {
    ca_.Bind(&block46);
    ca_.Goto(&block47, p_maybeTarget);
  }

  TNode<Object> phi_bb47_6;
  if (block47.is_used()) {
    ca_.Bind(&block47, &phi_bb47_6);
    compiler::CodeAssemblerLabel label28(&ca_);
    TryInitializeAsMonomorphic_0(state_, TNode<Context>{p_context}, TNode<Object>{phi_bb47_6}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, &label28);
    ca_.Goto(&block52);
    if (label28.is_used()) {
      ca_.Bind(&label28);
      ca_.Goto(&block53);
    }
  }

  if (block53.is_used()) {
    ca_.Bind(&block53);
    ca_.Goto(&block14);
  }

  if (block52.is_used()) {
    ca_.Bind(&block52);
    ca_.Goto(&block15);
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block13);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    TransitionToMegamorphic_0(state_, TNode<Context>{p_context}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    ca_.Goto(&block13);
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block1);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(&block54);
  }

    ca_.Bind(&block54);
}


void CollectInstanceOfFeedback_0(compiler::CodeAssemblerState* state_, TNode<Object> p_maybeTarget, TNode<Context> p_context, TNode<HeapObject> p_maybeFeedbackVector, TNode<UintPtrT> p_slotId) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FeedbackVector> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_FeedbackVector_0(state_, TNode<HeapObject>{p_maybeFeedbackVector}, &label1);
    ca_.Goto(&block11);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block12);
    }
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    ca_.Goto(&block1);
  }

  TNode<MaybeObject> tmp2;
  TNode<BoolT> tmp3;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp2 = CodeStubAssembler(state_).LoadFeedbackVectorSlot(TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    tmp3 = IsMonomorphic_0(state_, TNode<MaybeObject>{tmp2}, TNode<Object>{p_maybeTarget});
    ca_.Branch(tmp3, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block1);
  }

  TNode<BoolT> tmp4;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    tmp4 = IsMegamorphic_0(state_, TNode<MaybeObject>{tmp2});
    ca_.Branch(tmp4, &block19, std::vector<compiler::Node*>{}, &block20, std::vector<compiler::Node*>{});
  }

  if (block19.is_used()) {
    ca_.Bind(&block19);
    ca_.Goto(&block1);
  }

  TNode<BoolT> tmp5;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp5 = IsUninitialized_0(state_, TNode<MaybeObject>{tmp2});
    ca_.Branch(tmp5, &block21, std::vector<compiler::Node*>{}, &block22, std::vector<compiler::Node*>{});
  }

  if (block21.is_used()) {
    ca_.Bind(&block21);
    ca_.Goto(&block16);
  }

  TNode<HeapObject> tmp6;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = MaybeObjectToStrong_0(state_, TNode<MaybeObject>{tmp2}, &label7);
    ca_.Goto(&block23);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block24);
    }
  }

  if (block24.is_used()) {
    ca_.Bind(&block24);
    ca_.Goto(&block16);
  }

  if (block23.is_used()) {
    ca_.Bind(&block23);
    ca_.Goto(&block14);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    compiler::CodeAssemblerLabel label8(&ca_);
    TryInitializeAsMonomorphic_0(state_, TNode<Context>{p_context}, TNode<Object>{p_maybeTarget}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, &label8);
    ca_.Goto(&block25);
    if (label8.is_used()) {
      ca_.Bind(&label8);
      ca_.Goto(&block26);
    }
  }

  if (block26.is_used()) {
    ca_.Bind(&block26);
    ca_.Goto(&block14);
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    ca_.Goto(&block13);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    TransitionToMegamorphic_0(state_, TNode<Context>{p_context}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    ca_.Goto(&block13);
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block1);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(&block27);
  }

    ca_.Bind(&block27);
}


TNode<BoolT> BothTaggedEqualArrayFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_first, TNode<Object> p_second) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{p_first}, TNode<MaybeObject>{p_second});
    ca_.Branch(tmp0, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<JSFunction> tmp1;
  TNode<BoolT> tmp2;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp1 = GetArrayFunction_0(state_, TNode<Context>{p_context});
    tmp2 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{p_second}, TNode<HeapObject>{tmp1});
    ca_.Goto(&block4, tmp2);
  }

  TNode<BoolT> tmp3;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block4, tmp3);
  }

  TNode<BoolT> phi_bb4_4;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_4);
    ca_.Goto(&block5);
  }

    ca_.Bind(&block5);
  return TNode<BoolT>{phi_bb4_4};
}


TNode<FeedbackVector> CastFeedbackVector_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_maybeFeedbackVector, UpdateFeedbackMode p_updateFeedbackMode, compiler::CodeAssemblerLabel* label_Fallback) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FeedbackVector> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FeedbackVector> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    if (((CodeStubAssembler(state_).UpdateFeedbackModeEqual(p_updateFeedbackMode, UpdateFeedbackMode::kGuaranteedFeedback)))) {
      ca_.Goto(&block3);
    } else {
      ca_.Goto(&block4);
    }
  }

  TNode<FeedbackVector> tmp0;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp0 = TORQUE_CAST(TNode<Object>{p_maybeFeedbackVector});
    ca_.Goto(&block2, tmp0);
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    if (((CodeStubAssembler(state_).UpdateFeedbackModeEqual(p_updateFeedbackMode, UpdateFeedbackMode::kOptionalFeedback)))) {
      ca_.Goto(&block19);
    } else {
      ca_.Goto(&block20);
    }
  }

  TNode<FeedbackVector> tmp1;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_FeedbackVector_0(state_, TNode<HeapObject>{p_maybeFeedbackVector}, &label2);
    ca_.Goto(&block24);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block25);
    }
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    ca_.Goto(label_Fallback);
  }

  if (block24.is_used()) {
    ca_.Bind(&block24);
    ca_.Goto(&block2, tmp1);
  }

  if (block20.is_used()) {
    ca_.Bind(&block20);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<FeedbackVector> phi_bb2_1;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_1);
    ca_.Goto(&block26, phi_bb2_1);
  }

  TNode<FeedbackVector> phi_bb26_1;
    ca_.Bind(&block26, &phi_bb26_1);
  return TNode<FeedbackVector>{phi_bb26_1};
}


void CollectConstructFeedback_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_target, TNode<Object> p_newTarget, TNode<HeapObject> p_maybeFeedbackVector, TNode<UintPtrT> p_slotId, UpdateFeedbackMode p_updateFeedbackMode, compiler::CodeAssemblerLabel* label_ConstructGeneric, compiler::CodeAssemblerLabel* label_ConstructArray, compiler::TypedCodeAssemblerVariable<AllocationSite>* label_ConstructArray_parameter_0) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<AllocationSite> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FeedbackVector> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = CastFeedbackVector_0(state_, TNode<HeapObject>{p_maybeFeedbackVector}, p_updateFeedbackMode, &label1);
    ca_.Goto(&block12);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block13);
    }
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block1);
  }

  TNode<MaybeObject> tmp2;
  TNode<BoolT> tmp3;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    CodeStubAssembler(state_).IncrementCallCount(TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    tmp2 = CodeStubAssembler(state_).LoadFeedbackVectorSlot(TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    tmp3 = IsMonomorphic_0(state_, TNode<MaybeObject>{tmp2}, TNode<Object>{p_newTarget});
    ca_.Branch(tmp3, &block18, std::vector<compiler::Node*>{}, &block19, std::vector<compiler::Node*>{});
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    ca_.Goto(&block1);
  }

  TNode<BoolT> tmp4;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    tmp4 = IsMegamorphic_0(state_, TNode<MaybeObject>{tmp2});
    ca_.Branch(tmp4, &block20, std::vector<compiler::Node*>{}, &block21, std::vector<compiler::Node*>{});
  }

  if (block20.is_used()) {
    ca_.Bind(&block20);
    ca_.Goto(&block1);
  }

  TNode<BoolT> tmp5;
  if (block21.is_used()) {
    ca_.Bind(&block21);
    tmp5 = IsUninitialized_0(state_, TNode<MaybeObject>{tmp2});
    ca_.Branch(tmp5, &block22, std::vector<compiler::Node*>{}, &block23, std::vector<compiler::Node*>{});
  }

  if (block22.is_used()) {
    ca_.Bind(&block22);
    ca_.Goto(&block17);
  }

  TNode<BoolT> tmp6;
  TNode<BoolT> tmp7;
  if (block23.is_used()) {
    ca_.Bind(&block23);
    tmp6 = CodeStubAssembler(state_).IsWeakOrCleared(TNode<MaybeObject>{tmp2});
    tmp7 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp6});
    ca_.Branch(tmp7, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp8;
  TNode<BoolT> tmp9;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp8 = TORQUE_CAST(TNode<MaybeObject>{tmp2});
    tmp9 = Is_AllocationSite_Object_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp8});
    ca_.Branch(tmp9, &block26, std::vector<compiler::Node*>{}, &block27, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp10;
  if (block26.is_used()) {
    ca_.Bind(&block26);
    tmp10 = BothTaggedEqualArrayFunction_0(state_, TNode<Context>{p_context}, TNode<Object>{p_target}, TNode<Object>{p_newTarget});
    ca_.Branch(tmp10, &block28, std::vector<compiler::Node*>{}, &block29, std::vector<compiler::Node*>{});
  }

  TNode<AllocationSite> tmp11;
  if (block28.is_used()) {
    ca_.Bind(&block28);
    tmp11 = UnsafeCast_AllocationSite_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp8});
    ca_.Goto(&block2, tmp11);
  }

  if (block29.is_used()) {
    ca_.Bind(&block29);
    ca_.Goto(&block15);
  }

  if (block27.is_used()) {
    ca_.Bind(&block27);
    ca_.Goto(&block25);
  }

  TNode<HeapObject> tmp12;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    compiler::CodeAssemblerLabel label13(&ca_);
    tmp12 = MaybeObjectToStrong_0(state_, TNode<MaybeObject>{tmp2}, &label13);
    ca_.Goto(&block30);
    if (label13.is_used()) {
      ca_.Bind(&label13);
      ca_.Goto(&block31);
    }
  }

  if (block31.is_used()) {
    ca_.Bind(&block31);
    ca_.Goto(&block17);
  }

  if (block30.is_used()) {
    ca_.Bind(&block30);
    ca_.Goto(&block15);
  }

  TNode<BoolT> tmp14;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp14 = BothTaggedEqualArrayFunction_0(state_, TNode<Context>{p_context}, TNode<Object>{p_target}, TNode<Object>{p_newTarget});
    ca_.Branch(tmp14, &block32, std::vector<compiler::Node*>{}, &block33, std::vector<compiler::Node*>{});
  }

  TNode<AllocationSite> tmp15;
  if (block32.is_used()) {
    ca_.Bind(&block32);
    tmp15 = CodeStubAssembler(state_).CreateAllocationSiteInFeedbackVector(TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    CodeStubAssembler(state_).ReportFeedbackUpdate(TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, "Construct:CreateAllocationSite");
    ca_.Goto(&block2, tmp15);
  }

  if (block33.is_used()) {
    ca_.Bind(&block33);
    compiler::CodeAssemblerLabel label16(&ca_);
    TryInitializeAsMonomorphic_0(state_, TNode<Context>{p_context}, TNode<Object>{p_newTarget}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId}, &label16);
    ca_.Goto(&block34);
    if (label16.is_used()) {
      ca_.Bind(&label16);
      ca_.Goto(&block35);
    }
  }

  if (block35.is_used()) {
    ca_.Bind(&block35);
    ca_.Goto(&block15);
  }

  if (block34.is_used()) {
    ca_.Bind(&block34);
    ca_.Goto(&block14);
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    TransitionToMegamorphic_0(state_, TNode<Context>{p_context}, TNode<FeedbackVector>{tmp0}, TNode<UintPtrT>{p_slotId});
    ca_.Goto(&block14);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block1);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(label_ConstructGeneric);
  }

  TNode<AllocationSite> phi_bb2_0;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_0);
    *label_ConstructArray_parameter_0 = phi_bb2_0;
    ca_.Goto(label_ConstructArray);
  }
}


TNode<HeapObject> WeakToStrong_HeapObject_0(compiler::CodeAssemblerState* state_, TNode<MaybeObject> p_x, compiler::CodeAssemblerLabel* label_ClearedWeakPointer) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<HeapObject> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = CodeStubAssembler(state_).GetHeapObjectAssumeWeak(TNode<MaybeObject>{p_x}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(label_ClearedWeakPointer);
  }

  TNode<HeapObject> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp2 = (TNode<HeapObject>{tmp0});
    ca_.Goto(&block5);
  }

    ca_.Bind(&block5);
  return TNode<HeapObject>{tmp2};
}


TNode<BoolT> Is_JSBoundFunction_HeapObject_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSBoundFunction> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSBoundFunction_0(state_, TNode<HeapObject>{p_o}, &label1);
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


TNode<JSBoundFunction> UnsafeCast_JSBoundFunction_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSBoundFunction> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<JSBoundFunction>{tmp0};
}


TNode<Smi> Cast_Smi_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<MaybeObject> p_o, compiler::CodeAssemblerLabel* label_CastError) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<MaybeObject> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_WeakHeapObject_0(state_, TNode<MaybeObject>{p_o}, &label1);
    ca_.Goto(&block5);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block6);
    }
  }

  TNode<Smi> tmp2;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Smi_0(state_, TNode<Object>{ca_.UncheckedCast<Object>(p_o)}, &label3);
    ca_.Goto(&block7);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block8);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block1);
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(&block1);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block9);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(label_CastError);
  }

    ca_.Bind(&block9);
  return TNode<Smi>{tmp2};
}


TNode<BoolT> Is_FeedbackVector_Undefined_OR_FeedbackVector_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FeedbackVector> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_FeedbackVector_0(state_, TNode<HeapObject>{p_o}, &label1);
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


TNode<FeedbackVector> UnsafeCast_FeedbackVector_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FeedbackVector> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<FeedbackVector>{tmp0};
}


TNode<BoolT> Is_AllocationSite_Object_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<AllocationSite> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_AllocationSite_1(state_, TNode<Context>{p_context}, TNode<Object>{p_o}, &label1);
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


TNode<AllocationSite> UnsafeCast_AllocationSite_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<AllocationSite> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<AllocationSite>{tmp0};
}


} // namespace internal
} // namespace v8
