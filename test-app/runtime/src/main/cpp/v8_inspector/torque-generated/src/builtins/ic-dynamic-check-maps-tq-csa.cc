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
#include "torque-generated/src/builtins/ic-dynamic-check-maps-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/ic-dynamic-check-maps-tq-csa.h"
#include "torque-generated/src/builtins/ic-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"

namespace v8 {
namespace internal {
TNode<Int32T> PerformPolymorphicCheck_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_expectedPolymorphicArray, TNode<Map> p_actualMap, TNode<Object> p_actualHandler) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Int32T> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Int32T> block48(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<WeakFixedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_WeakFixedArray_0(state_, TNode<HeapObject>{p_expectedPolymorphicArray}, &label1);
    ca_.Goto(&block7);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block8);
    }
  }

  TNode<BoolT> tmp2;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp2 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block4, tmp2);
  }

  TNode<BoolT> tmp3;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp3 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block4, tmp3);
  }

  TNode<BoolT> phi_bb4_5;
  TNode<BoolT> tmp4;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_5);
    tmp4 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{phi_bb4_5});
    ca_.Branch(tmp4, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<Int32T> tmp5;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp5 = FromConstexpr_int32_constexpr_int32_0(state_, static_cast<int>(DynamicCheckMapsStatus::kDeopt));
    ca_.Goto(&block1, tmp5);
  }

  TNode<WeakFixedArray> tmp6;
  TNode<MaybeObject> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp6 = TORQUE_CAST(TNode<Object>{p_expectedPolymorphicArray});
    tmp7 = CodeStubAssembler(state_).MakeWeak(TNode<HeapObject>{p_actualMap});
    tmp8 = CodeStubAssembler(state_).LoadAndUntagWeakFixedArrayLength(TNode<WeakFixedArray>{tmp6});
    tmp9 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block28, tmp9);
  }

  TNode<IntPtrT> phi_bb28_6;
  TNode<BoolT> tmp10;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_6);
    tmp10 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{phi_bb28_6}, TNode<IntPtrT>{tmp8});
    ca_.Branch(tmp10, &block26, std::vector<compiler::Node*>{phi_bb28_6}, &block27, std::vector<compiler::Node*>{phi_bb28_6});
  }

  TNode<IntPtrT> phi_bb26_6;
  TNode<MaybeObject> tmp11;
  TNode<MaybeObject> tmp12;
  TNode<BoolT> tmp13;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_6);
    tmp11 = CodeStubAssembler(state_).LoadWeakFixedArrayElement(TNode<WeakFixedArray>{tmp6}, TNode<IntPtrT>{phi_bb26_6});
    tmp12 = UnsafeCast_WeakHeapObject_0(state_, TNode<MaybeObject>{tmp11});
    tmp13 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{tmp12}, TNode<MaybeObject>{tmp7});
    ca_.Branch(tmp13, &block30, std::vector<compiler::Node*>{phi_bb26_6}, &block31, std::vector<compiler::Node*>{phi_bb26_6});
  }

  TNode<IntPtrT> phi_bb30_6;
  TNode<IntPtrT> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<MaybeObject> tmp16;
  TNode<MaybeObject> tmp17;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_6);
    tmp14 = CodeStubAssembler(state_).FeedbackIteratorHandlerOffset();
    tmp15 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb30_6}, TNode<IntPtrT>{tmp14});
    tmp16 = CodeStubAssembler(state_).LoadWeakFixedArrayElement(TNode<WeakFixedArray>{tmp6}, TNode<IntPtrT>{tmp15});
    compiler::CodeAssemblerLabel label18(&ca_);
    tmp17 = Cast_WeakHeapObject_0(state_, TNode<MaybeObject>{tmp16}, &label18);
    ca_.Goto(&block41, phi_bb30_6);
    if (label18.is_used()) {
      ca_.Bind(&label18);
      ca_.Goto(&block42, phi_bb30_6);
    }
  }

  TNode<IntPtrT> phi_bb42_6;
  TNode<Object> tmp19;
  if (block42.is_used()) {
    ca_.Bind(&block42, &phi_bb42_6);
    compiler::CodeAssemblerLabel label20(&ca_);
    tmp19 = Cast_Object_0(state_, TNode<Object>{ca_.UncheckedCast<Object>(tmp16)}, &label20);
    ca_.Goto(&block43, phi_bb42_6);
    if (label20.is_used()) {
      ca_.Bind(&label20);
      ca_.Goto(&block44, phi_bb42_6);
    }
  }

  TNode<IntPtrT> phi_bb41_6;
  if (block41.is_used()) {
    ca_.Bind(&block41, &phi_bb41_6);
    ca_.Goto(&block37, phi_bb41_6);
  }

  TNode<IntPtrT> phi_bb44_6;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_6);
    ca_.Goto(&block37, phi_bb44_6);
  }

  TNode<IntPtrT> phi_bb43_6;
  TNode<BoolT> tmp21;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_6);
    tmp21 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{tmp19}, TNode<MaybeObject>{p_actualHandler});
    ca_.Branch(tmp21, &block45, std::vector<compiler::Node*>{phi_bb43_6}, &block46, std::vector<compiler::Node*>{phi_bb43_6});
  }

  TNode<IntPtrT> phi_bb37_6;
  if (block37.is_used()) {
    ca_.Bind(&block37, &phi_bb37_6);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb45_6;
  TNode<Int32T> tmp22;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_6);
    tmp22 = FromConstexpr_int32_constexpr_int32_0(state_, static_cast<int>(DynamicCheckMapsStatus::kSuccess));
    ca_.Goto(&block1, tmp22);
  }

  TNode<IntPtrT> phi_bb46_6;
  TNode<Int32T> tmp23;
  if (block46.is_used()) {
    ca_.Bind(&block46, &phi_bb46_6);
    tmp23 = FromConstexpr_int32_constexpr_int32_0(state_, static_cast<int>(DynamicCheckMapsStatus::kDeopt));
    ca_.Goto(&block1, tmp23);
  }

  TNode<IntPtrT> phi_bb31_6;
  TNode<IntPtrT> tmp24;
  TNode<IntPtrT> tmp25;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_6);
    tmp24 = CodeStubAssembler(state_).FeedbackIteratorEntrySize();
    tmp25 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb31_6}, TNode<IntPtrT>{tmp24});
    ca_.Goto(&block28, tmp25);
  }

  TNode<IntPtrT> phi_bb27_6;
  TNode<Int32T> tmp26;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_6);
    tmp26 = FromConstexpr_int32_constexpr_int32_0(state_, static_cast<int>(DynamicCheckMapsStatus::kBailout));
    ca_.Goto(&block1, tmp26);
  }

  TNode<Int32T> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block48, phi_bb1_3);
  }

  TNode<Int32T> phi_bb48_3;
    ca_.Bind(&block48, &phi_bb48_3);
  return TNode<Int32T>{phi_bb48_3};
}


TNode<Int32T> PerformMonomorphicCheck_0(compiler::CodeAssemblerState* state_, TNode<FeedbackVector> p_feedbackVector, TNode<IntPtrT> p_slotIndex, TNode<HeapObject> p_expectedMap, TNode<Map> p_actualMap, TNode<Object> p_actualHandler) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Int32T> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Int32T> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{p_expectedMap}, TNode<HeapObject>{p_actualMap});
    ca_.Branch(tmp0, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<MaybeObject> tmp3;
  TNode<MaybeObject> tmp4;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp2 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{p_slotIndex}, TNode<IntPtrT>{tmp1});
    tmp3 = CodeStubAssembler(state_).LoadFeedbackVectorSlot(TNode<FeedbackVector>{p_feedbackVector}, TNode<IntPtrT>{tmp2});
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_WeakHeapObject_0(state_, TNode<MaybeObject>{tmp3}, &label5);
    ca_.Goto(&block13);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block14);
    }
  }

  TNode<Object> tmp6;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = Cast_Object_0(state_, TNode<Object>{ca_.UncheckedCast<Object>(tmp3)}, &label7);
    ca_.Goto(&block15);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block16);
    }
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block9);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block9);
  }

  TNode<BoolT> tmp8;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp8 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{p_actualHandler}, TNode<MaybeObject>{tmp6});
    ca_.Branch(tmp8, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  if (block9.is_used()) {
    ca_.Bind(&block9);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Int32T> tmp9;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp9 = FromConstexpr_int32_constexpr_int32_0(state_, static_cast<int>(DynamicCheckMapsStatus::kSuccess));
    ca_.Goto(&block1, tmp9);
  }

  TNode<Int32T> tmp10;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    tmp10 = FromConstexpr_int32_constexpr_int32_0(state_, static_cast<int>(DynamicCheckMapsStatus::kDeopt));
    ca_.Goto(&block1, tmp10);
  }

  TNode<Int32T> tmp11;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp11 = FromConstexpr_int32_constexpr_int32_0(state_, static_cast<int>(DynamicCheckMapsStatus::kBailout));
    ca_.Goto(&block1, tmp11);
  }

  TNode<Int32T> phi_bb1_5;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_5);
    ca_.Goto(&block19, phi_bb1_5);
  }

  TNode<Int32T> phi_bb19_5;
    ca_.Bind(&block19, &phi_bb19_5);
  return TNode<Int32T>{phi_bb19_5};
}


TNode<Int32T> DynamicCheckMaps_0(compiler::CodeAssemblerState* state_, TNode<Map> p_actualMap, TNode<IntPtrT> p_slotIndex, TNode<Object> p_actualHandler) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FeedbackVector> tmp0;
  TNode<Int32T> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadFeedbackVectorForStubWithTrampoline();
    tmp1 = DynamicCheckMapsWithFeedbackVector_0(state_, TNode<Map>{p_actualMap}, TNode<IntPtrT>{p_slotIndex}, TNode<Object>{p_actualHandler}, TNode<FeedbackVector>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Int32T>{tmp1};
}


TNode<Int32T> DynamicCheckMapsWithFeedbackVector_0(compiler::CodeAssemblerState* state_, TNode<Map> p_actualMap, TNode<IntPtrT> p_slotIndex, TNode<Object> p_actualHandler, TNode<FeedbackVector> p_feedbackVector) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Int32T> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<MaybeObject> tmp0;
  TNode<HeapObject> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadFeedbackVectorSlot(TNode<FeedbackVector>{p_feedbackVector}, TNode<IntPtrT>{p_slotIndex});
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = CodeStubAssembler(state_).GetHeapObjectIfStrong(TNode<MaybeObject>{tmp0}, &label2);
    ca_.Goto(&block6);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block7);
    }
  }

  TNode<HeapObject> tmp3;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = CodeStubAssembler(state_).GetHeapObjectAssumeWeak(TNode<MaybeObject>{tmp0}, &label4);
    ca_.Goto(&block8);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block9);
    }
  }

  TNode<Int32T> tmp5;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp5 = PerformPolymorphicCheck_0(state_, TNode<HeapObject>{tmp1}, TNode<Map>{p_actualMap}, TNode<Object>{p_actualHandler});
    ca_.Goto(&block1, tmp5);
  }

  TNode<Int32T> tmp6;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp6 = FromConstexpr_int32_constexpr_int32_0(state_, static_cast<int>(DynamicCheckMapsStatus::kDeopt));
    ca_.Goto(&block1, tmp6);
  }

  TNode<Int32T> tmp7;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp7 = PerformMonomorphicCheck_0(state_, TNode<FeedbackVector>{p_feedbackVector}, TNode<IntPtrT>{p_slotIndex}, TNode<HeapObject>{tmp3}, TNode<Map>{p_actualMap}, TNode<Object>{p_actualHandler});
    ca_.Goto(&block1, tmp7);
  }

  TNode<Int32T> phi_bb1_4;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_4);
    ca_.Goto(&block10);
  }

    ca_.Bind(&block10);
  return TNode<Int32T>{phi_bb1_4};
}


TNode<BoolT> Is_WeakFixedArray_HeapObject_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<HeapObject> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<WeakFixedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_WeakFixedArray_0(state_, TNode<HeapObject>{p_o}, &label1);
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


TNode<WeakFixedArray> UnsafeCast_WeakFixedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<WeakFixedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<WeakFixedArray>{tmp0};
}


TNode<MaybeObject> UnsafeCast_WeakHeapObject_0(compiler::CodeAssemblerState* state_, TNode<MaybeObject> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<MaybeObject> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<MaybeObject>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<MaybeObject>{tmp0};
}


TNode<Object> Cast_Object_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<MaybeObject> p_o, compiler::CodeAssemblerLabel* label_CastError) {
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

  TNode<Object> tmp2;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Object_0(state_, TNode<Object>{ca_.UncheckedCast<Object>(p_o)}, &label3);
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
  return TNode<Object>{tmp2};
}


} // namespace internal
} // namespace v8
