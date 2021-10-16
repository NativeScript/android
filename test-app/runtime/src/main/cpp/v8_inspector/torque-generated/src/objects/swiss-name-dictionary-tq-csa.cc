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
#include "torque-generated/src/objects/swiss-name-dictionary-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/builtins-string-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/contexts-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/ordered-hash-table-tq-csa.h"
#include "torque-generated/src/objects/string-tq-csa.h"
#include "torque-generated/src/objects/swiss-hash-table-helpers-tq-csa.h"
#include "torque-generated/src/objects/swiss-name-dictionary-tq-csa.h"

namespace v8 {
namespace internal {
TNode<SwissNameDictionary> Cast_SwissNameDictionary_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_obj, compiler::CodeAssemblerLabel* label_CastError) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<SwissNameDictionary> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = DownCastForTorqueClass_SwissNameDictionary_0(state_, TNode<HeapObject>{p_obj}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(label_CastError);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block5);
  }

    ca_.Bind(&block5);
  return TNode<SwissNameDictionary>{tmp0};
}


TNode<IntPtrT> SwissNameDictionaryCapacityFor_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_atLeastSpaceFor) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp1 = CodeStubAssembler(state_).IntPtrLessThanOrEqual(TNode<IntPtrT>{p_atLeastSpaceFor}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp1, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp2;
  TNode<BoolT> tmp3;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp3 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{p_atLeastSpaceFor}, TNode<IntPtrT>{tmp2});
    ca_.Branch(tmp3, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp4;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block1, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<BoolT> tmp6;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp5 = FromConstexpr_intptr_constexpr_int32_0(state_, SwissNameDictionary::kInitialCapacity);
    tmp6 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{p_atLeastSpaceFor}, TNode<IntPtrT>{tmp5});
    ca_.Branch(tmp6, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp7;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    ca_.Goto(&block1, tmp7);
  }

  TNode<BoolT> tmp8;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp8 = FromConstexpr_bool_constexpr_bool_0(state_, (CodeStubAssembler(state_).ConstexprInt32Equal(swiss_table::Group::kWidth, 16)));
    ca_.Branch(tmp8, &block10, std::vector<compiler::Node*>{}, &block11, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp9;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp9 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    ca_.Goto(&block1, tmp9);
  }

  TNode<BoolT> tmp10;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp10 = FromConstexpr_bool_constexpr_bool_0(state_, (CodeStubAssembler(state_).ConstexprInt32Equal(swiss_table::Group::kWidth, 8)));
    ca_.Branch(tmp10, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp11;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    ca_.Goto(&block1, tmp11);
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    ca_.Goto(&block3);
  }

  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<IntPtrT> tmp15;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp12 = FromConstexpr_intptr_constexpr_int31_0(state_, 7);
    tmp13 = CodeStubAssembler(state_).IntPtrDiv(TNode<IntPtrT>{p_atLeastSpaceFor}, TNode<IntPtrT>{tmp12});
    tmp14 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{p_atLeastSpaceFor}, TNode<IntPtrT>{tmp13});
    tmp15 = CodeStubAssembler(state_).IntPtrRoundUpToPowerOfTwo32(TNode<IntPtrT>{tmp14});
    ca_.Goto(&block1, tmp15);
  }

  TNode<IntPtrT> phi_bb1_1;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_1);
    ca_.Goto(&block23);
  }

    ca_.Bind(&block23);
  return TNode<IntPtrT>{phi_bb1_1};
}


TNode<IntPtrT> SwissNameDictionaryMaxUsableCapacity_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_bool_constexpr_bool_0(state_, (CodeStubAssembler(state_).ConstexprInt32Equal(swiss_table::Group::kWidth, 8)));
    ca_.Branch(tmp0, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp1;
  TNode<BoolT> tmp2;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp2 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{p_capacity}, TNode<IntPtrT>{tmp1});
    ca_.Goto(&block13, tmp2);
  }

  TNode<BoolT> tmp3;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp3 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block13, tmp3);
  }

  TNode<BoolT> phi_bb13_2;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_2);
    ca_.Branch(phi_bb13_2, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp4;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 3);
    ca_.Goto(&block1, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp6 = CodeStubAssembler(state_).IntPtrDiv(TNode<IntPtrT>{p_capacity}, TNode<IntPtrT>{tmp5});
    tmp7 = CodeStubAssembler(state_).IntPtrSub(TNode<IntPtrT>{p_capacity}, TNode<IntPtrT>{tmp6});
    ca_.Goto(&block1, tmp7);
  }

  TNode<IntPtrT> phi_bb1_1;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_1);
    ca_.Goto(&block14);
  }

    ca_.Bind(&block14);
  return TNode<IntPtrT>{phi_bb1_1};
}


TNode<IntPtrT> SwissNameDictionarySizeFor_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, (CodeStubAssembler(state_).ConstexprInt31Add((CodeStubAssembler(state_).ConstexprInt31Mul(2, kTaggedSize)), 2)));
    tmp1 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{p_capacity}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int32_0(state_, swiss_table::Group::kWidth);
    tmp3 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp2});
    tmp4 = FromConstexpr_intptr_constexpr_int32_0(state_, (CodeStubAssembler(state_).ConstexprInt32Add((CodeStubAssembler(state_).ConstexprInt32Add(HeapObject::kHeaderSize, 8)), kTaggedSize)));
    tmp5 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp3});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<IntPtrT>{tmp5};
}


TNode<IntPtrT> SwissNameDictionaryMetaTableSizePerEntryFor_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int32_0(state_, SwissNameDictionary::kMax1ByteMetaTableCapacity);
    tmp1 = CodeStubAssembler(state_).IntPtrLessThanOrEqual(TNode<IntPtrT>{p_capacity}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp1, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp2;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    ca_.Goto(&block1, tmp2);
  }

  TNode<IntPtrT> tmp3;
  TNode<BoolT> tmp4;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = FromConstexpr_intptr_constexpr_int32_0(state_, SwissNameDictionary::kMax2ByteMetaTableCapacity);
    tmp4 = CodeStubAssembler(state_).IntPtrLessThanOrEqual(TNode<IntPtrT>{p_capacity}, TNode<IntPtrT>{tmp3});
    ca_.Branch(tmp4, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp5;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 2);
    ca_.Goto(&block1, tmp5);
  }

  TNode<IntPtrT> tmp6;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    ca_.Goto(&block1, tmp6);
  }

  TNode<IntPtrT> phi_bb1_1;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_1);
    ca_.Goto(&block8, phi_bb1_1);
  }

  TNode<IntPtrT> phi_bb8_1;
    ca_.Bind(&block8, &phi_bb8_1);
  return TNode<IntPtrT>{phi_bb8_1};
}


TNode<IntPtrT> SwissNameDictionaryMetaTableSizeFor_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = SwissNameDictionaryMetaTableSizePerEntryFor_0(state_, TNode<IntPtrT>{p_capacity});
    tmp1 = SwissNameDictionaryMaxUsableCapacity_0(state_, TNode<IntPtrT>{p_capacity});
    tmp2 = Convert_intptr_intptr_0(state_, TNode<IntPtrT>{tmp1});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 2);
    tmp4 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<IntPtrT>{tmp5};
}


TNode<IntPtrT> SwissNameDictionaryDataTableStartOffsetMT_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_intptr_0(state_, SwissNameDictionary::DataTableStartOffset() - kHeapObjectTag);
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<IntPtrT>{tmp0};
}


TNode<IntPtrT> SwissNameDictionaryCtrlTableStartOffsetMT_0(compiler::CodeAssemblerState* state_, TNode<IntPtrT> p_capacity) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, kTaggedSize);
    tmp1 = FromConstexpr_intptr_constexpr_intptr_0(state_, SwissNameDictionary::kDataTableEntryCount);
    tmp2 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp0});
    tmp3 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{p_capacity});
    tmp4 = FromConstexpr_intptr_constexpr_intptr_0(state_, SwissNameDictionary::DataTableStartOffset() - kHeapObjectTag);
    tmp5 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp3});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<IntPtrT>{tmp5};
}


TorqueStructProbeSequence_0 Probe_0(compiler::CodeAssemblerState* state_, TNode<Uint32T> p_hash, TNode<Uint32T> p_mask) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Uint32T> tmp0;
  TNode<Uint32T> tmp1;
  TNode<Uint32T> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = H1_0(state_, TNode<Uint32T>{p_hash});
    tmp1 = CodeStubAssembler(state_).Word32And(TNode<Uint32T>{tmp0}, TNode<Uint32T>{p_mask});
    tmp2 = FromConstexpr_uint32_constexpr_int31_0(state_, 0);
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TorqueStructProbeSequence_0{TNode<Uint32T>{p_mask}, TNode<Uint32T>{tmp1}, TNode<Uint32T>{tmp2}};
}


void SwissNameDictionaryDelete_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<IntPtrT> p_entry, compiler::CodeAssemblerLabel* label_Shrunk, compiler::TypedCodeAssemblerVariable<SwissNameDictionary>* label_Shrunk_parameter_0) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Int32T> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<ByteArray> tmp4;
  TNode<Uint32T> tmp5;
  TNode<Oddball> tmp6;
  TNode<Oddball> tmp7;
  TNode<Uint8T> tmp8;
  TNode<Int32T> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<BoolT> tmp13;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp1 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_table, tmp0});
    tmp2 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp1});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp4 = CodeStubAssembler(state_).LoadReference<ByteArray>(CodeStubAssembler::Reference{p_table, tmp3});
    tmp5 = CodeStubAssembler(state_).SwissNameDictionaryUpdateCountsForDeletion(TNode<ByteArray>{tmp4}, TNode<IntPtrT>{tmp2});
    tmp6 = TheHole_0(state_);
    tmp7 = TheHole_0(state_);
    CodeStubAssembler(state_).StoreSwissNameDictionaryKeyAndValue(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{p_entry}, TNode<Object>{tmp6}, TNode<Object>{tmp7});
    tmp8 = FromConstexpr_uint8_constexpr_uint8_0(state_, static_cast<uint8_t>(swiss_table::Ctrl::kDeleted));
    CodeStubAssembler(state_).SwissNameDictionarySetCtrl(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{p_entry}, TNode<Uint8T>{tmp8});
    tmp9 = CodeStubAssembler(state_).Signed(TNode<Uint32T>{tmp5});
    tmp10 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp9});
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, 2);
    tmp12 = CodeStubAssembler(state_).WordSar(TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp11});
    tmp13 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{tmp10}, TNode<IntPtrT>{tmp12});
    ca_.Branch(tmp13, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  TNode<Smi> tmp14;
  TNode<SwissNameDictionary> tmp15;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp14 = kNoContext_0(state_);
    tmp15 = TORQUE_CAST(CodeStubAssembler(state_).CallRuntime(Runtime::kShrinkSwissNameDictionary, tmp14, p_table)); 
    *label_Shrunk_parameter_0 = tmp15;
    ca_.Goto(label_Shrunk);
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block5);
  }

    ca_.Bind(&block5);
}


void SwissNameDictionaryFindEntrySIMD_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    compiler::TypedCodeAssemblerVariable<IntPtrT> tmp1(&ca_);
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label0(&ca_);
    compiler::CodeAssemblerLabel label2(&ca_);
    FindEntry_GroupSse2Loader_0(state_, TNode<SwissNameDictionary>{p_table}, TNode<Name>{p_key}, &label0, &tmp1, &label2);
    if (label0.is_used()) {
      ca_.Bind(&label0);
      ca_.Goto(&block3);
    }
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block4);
    }
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    *label_Found_parameter_0 = tmp1.value();
    ca_.Goto(label_Found);
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(label_NotFound);
  }
}


void SwissNameDictionaryFindEntryPortable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    compiler::TypedCodeAssemblerVariable<IntPtrT> tmp1(&ca_);
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label0(&ca_);
    compiler::CodeAssemblerLabel label2(&ca_);
    FindEntry_GroupPortableLoader_0(state_, TNode<SwissNameDictionary>{p_table}, TNode<Name>{p_key}, &label0, &tmp1, &label2);
    if (label0.is_used()) {
      ca_.Bind(&label0);
      ca_.Goto(&block3);
    }
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block4);
    }
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    *label_Found_parameter_0 = tmp1.value();
    ca_.Goto(label_Found);
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(label_NotFound);
  }
}


void SwissNameDictionaryAddSIMD_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label0(&ca_);
    Add_GroupSse2Loader_0(state_, TNode<SwissNameDictionary>{p_table}, TNode<Name>{p_key}, TNode<Object>{p_value}, TNode<Uint8T>{p_propertyDetails}, &label0);
    ca_.Goto(&block3);
    if (label0.is_used()) {
      ca_.Bind(&label0);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(label_Bailout);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block5);
  }

    ca_.Bind(&block5);
}


void SwissNameDictionaryAddPortable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label0(&ca_);
    Add_GroupPortableLoader_0(state_, TNode<SwissNameDictionary>{p_table}, TNode<Name>{p_key}, TNode<Object>{p_value}, TNode<Uint8T>{p_propertyDetails}, &label0);
    ca_.Goto(&block3);
    if (label0.is_used()) {
      ca_.Bind(&label0);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(label_Bailout);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block5);
  }

    ca_.Bind(&block5);
}


TNode<Uint32T> LoadSwissNameDictionaryHash_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Uint32T> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp1 = CodeStubAssembler(state_).LoadReference<Uint32T>(CodeStubAssembler::Reference{p_o, tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Uint32T>{tmp1};
}


void StoreSwissNameDictionaryHash_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<Uint32T> p_v) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    CodeStubAssembler(state_).StoreReference<Uint32T>(CodeStubAssembler::Reference{p_o, tmp0}, p_v);
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
}


TNode<Int32T> LoadSwissNameDictionaryCapacity_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Int32T> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp1 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_o, tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<Int32T>{tmp1};
}


TNode<ByteArray> LoadSwissNameDictionaryMetaTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<ByteArray> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp1 = CodeStubAssembler(state_).LoadReference<ByteArray>(CodeStubAssembler::Reference{p_o, tmp0});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<ByteArray>{tmp1};
}


void StoreSwissNameDictionaryMetaTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<ByteArray> p_v) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    CodeStubAssembler(state_).StoreReference<ByteArray>(CodeStubAssembler::Reference{p_o, tmp0}, p_v);
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
}


TorqueStructSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_MutableReference_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0 FieldSliceSwissNameDictionaryDataTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Int32T> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Object> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp1 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_o, tmp0});
    tmp2 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp1});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 2);
    tmp4 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp3});
    tmp5 = Convert_intptr_intptr_0(state_, TNode<IntPtrT>{tmp4});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    std::tie(tmp7, tmp8, tmp9) = NewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(state_, TNode<Object>{p_o}, TNode<IntPtrT>{tmp6}, TNode<IntPtrT>{tmp5}).Flatten();
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TorqueStructSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_MutableReference_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0{TNode<Object>{tmp7}, TNode<IntPtrT>{tmp8}, TNode<IntPtrT>{tmp9}, TorqueStructUnsafe_0{}};
}


TNode<Object> LoadSwissNameDictionaryDataTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i) {
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
    std::tie(tmp0, tmp1, tmp2) = FieldSliceSwissNameDictionaryDataTable_0(state_, TNode<SwissNameDictionary>{p_o}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_i});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = TimesSizeOf_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(state_, TNode<IntPtrT>{p_i});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    tmp10 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp8, tmp9});
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TNode<Object>{tmp10};
}


void StoreSwissNameDictionaryDataTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Object> p_v) {
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
    std::tie(tmp0, tmp1, tmp2) = FieldSliceSwissNameDictionaryDataTable_0(state_, TNode<SwissNameDictionary>{p_o}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_i});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = TimesSizeOf_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(state_, TNode<IntPtrT>{p_i});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp8, tmp9}, p_v);
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
}


TorqueStructSlice_uint8_MutableReference_uint8_0 FieldSliceSwissNameDictionaryCtrlTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Int32T> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<Object> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1, tmp2) = FieldSliceSwissNameDictionaryDataTable_0(state_, TNode<SwissNameDictionary>{p_o}).Flatten();
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp4 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp4});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp7 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_o, tmp6});
    tmp8 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp7});
    tmp9 = FromConstexpr_intptr_constexpr_int32_0(state_, swiss_table::Group::kWidth);
    tmp10 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp8}, TNode<IntPtrT>{tmp9});
    tmp11 = Convert_intptr_intptr_0(state_, TNode<IntPtrT>{tmp10});
    std::tie(tmp12, tmp13, tmp14) = NewMutableSlice_uint8_0(state_, TNode<Object>{p_o}, TNode<IntPtrT>{tmp5}, TNode<IntPtrT>{tmp11}).Flatten();
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TorqueStructSlice_uint8_MutableReference_uint8_0{TNode<Object>{tmp12}, TNode<IntPtrT>{tmp13}, TNode<IntPtrT>{tmp14}, TorqueStructUnsafe_0{}};
}


TNode<Uint8T> LoadSwissNameDictionaryCtrlTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i) {
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
    std::tie(tmp0, tmp1, tmp2) = FieldSliceSwissNameDictionaryCtrlTable_0(state_, TNode<SwissNameDictionary>{p_o}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_i});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Uint8T> tmp10;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = TimesSizeOf_uint8_0(state_, TNode<IntPtrT>{p_i});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_uint8_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    tmp10 = CodeStubAssembler(state_).LoadReference<Uint8T>(CodeStubAssembler::Reference{tmp8, tmp9});
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TNode<Uint8T>{tmp10};
}


void StoreSwissNameDictionaryCtrlTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
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
    std::tie(tmp0, tmp1, tmp2) = FieldSliceSwissNameDictionaryCtrlTable_0(state_, TNode<SwissNameDictionary>{p_o}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_i});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = TimesSizeOf_uint8_0(state_, TNode<IntPtrT>{p_i});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_uint8_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    CodeStubAssembler(state_).StoreReference<Uint8T>(CodeStubAssembler::Reference{tmp8, tmp9}, p_v);
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
}


TorqueStructSlice_uint8_MutableReference_uint8_0 FieldSliceSwissNameDictionaryPropertyDetailsTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Int32T> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1, tmp2) = FieldSliceSwissNameDictionaryCtrlTable_0(state_, TNode<SwissNameDictionary>{p_o}).Flatten();
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp4 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp4});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp7 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_o, tmp6});
    tmp8 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp7});
    tmp9 = Convert_intptr_intptr_0(state_, TNode<IntPtrT>{tmp8});
    std::tie(tmp10, tmp11, tmp12) = NewMutableSlice_uint8_0(state_, TNode<Object>{p_o}, TNode<IntPtrT>{tmp5}, TNode<IntPtrT>{tmp9}).Flatten();
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TorqueStructSlice_uint8_MutableReference_uint8_0{TNode<Object>{tmp10}, TNode<IntPtrT>{tmp11}, TNode<IntPtrT>{tmp12}, TorqueStructUnsafe_0{}};
}


TNode<Uint8T> LoadSwissNameDictionaryPropertyDetailsTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i) {
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
    std::tie(tmp0, tmp1, tmp2) = FieldSliceSwissNameDictionaryPropertyDetailsTable_0(state_, TNode<SwissNameDictionary>{p_o}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_i});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Uint8T> tmp10;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = TimesSizeOf_uint8_0(state_, TNode<IntPtrT>{p_i});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_uint8_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    tmp10 = CodeStubAssembler(state_).LoadReference<Uint8T>(CodeStubAssembler::Reference{tmp8, tmp9});
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
  return TNode<Uint8T>{tmp10};
}


void StoreSwissNameDictionaryPropertyDetailsTable_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_o, TNode<IntPtrT> p_i, TNode<Uint8T> p_v) {
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
    std::tie(tmp0, tmp1, tmp2) = FieldSliceSwissNameDictionaryPropertyDetailsTable_0(state_, TNode<SwissNameDictionary>{p_o}).Flatten();
    tmp3 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_i});
    tmp4 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp2});
    tmp5 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp4});
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<IntPtrT> tmp9;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = TimesSizeOf_uint8_0(state_, TNode<IntPtrT>{p_i});
    tmp7 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp1}, TNode<IntPtrT>{tmp6});
    std::tie(tmp8, tmp9) = NewReference_uint8_0(state_, TNode<Object>{tmp0}, TNode<IntPtrT>{tmp7}).Flatten();
    CodeStubAssembler(state_).StoreReference<Uint8T>(CodeStubAssembler::Reference{tmp8, tmp9}, p_v);
    ca_.Goto(&block9);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block9);
}


TNode<SwissNameDictionary> DownCastForTorqueClass_SwissNameDictionary_0(compiler::CodeAssemblerState* state_, TNode<HeapObject> p_o, compiler::CodeAssemblerLabel* label_CastError) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Map> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{p_o, tmp0});
    if (((CodeStubAssembler(state_).ConstexprInt31Equal(static_cast<InstanceType>(175), static_cast<InstanceType>(175))))) {
      ca_.Goto(&block3);
    } else {
      ca_.Goto(&block4);
    }
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    if ((CodeStubAssembler(state_).ClassHasMapConstant<SwissNameDictionary>())) {
      ca_.Goto(&block6);
    } else {
      ca_.Goto(&block7);
    }
  }

  TNode<Map> tmp2;
  TNode<BoolT> tmp3;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp2 = CodeStubAssembler(state_).GetClassMapConstant<SwissNameDictionary>();
    tmp3 = CodeStubAssembler(state_).TaggedNotEqual(TNode<HeapObject>{tmp1}, TNode<HeapObject>{tmp2});
    ca_.Branch(tmp3, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  if (block9.is_used()) {
    ca_.Bind(&block9);
    ca_.Goto(&block1);
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    ca_.Goto(&block8);
  }

  TNode<IntPtrT> tmp4;
  TNode<Uint16T> tmp5;
  TNode<Uint32T> tmp6;
  TNode<BoolT> tmp7;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp5 = CodeStubAssembler(state_).LoadReference<Uint16T>(CodeStubAssembler::Reference{tmp1, tmp4});
    tmp6 = FromConstexpr_uint32_constexpr_uint32_0(state_, static_cast<InstanceType>(175));
    tmp7 = CodeStubAssembler(state_).Word32NotEqual(TNode<Uint32T>{tmp5}, TNode<Uint32T>{tmp6});
    ca_.Branch(tmp7, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  if (block11.is_used()) {
    ca_.Bind(&block11);
    ca_.Goto(&block1);
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    ca_.Goto(&block8);
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(&block5);
  }

  TNode<Int32T> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Uint16T> tmp10;
  TNode<Uint16T> tmp11;
  TNode<Int32T> tmp12;
  TNode<Uint16T> tmp13;
  TNode<Uint16T> tmp14;
  TNode<Int32T> tmp15;
  TNode<Int32T> tmp16;
  TNode<Uint32T> tmp17;
  TNode<Uint32T> tmp18;
  TNode<BoolT> tmp19;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp8 = FromConstexpr_int32_constexpr_int32_0(state_, (CodeStubAssembler(state_).ConstexprUint32Sub(static_cast<InstanceType>(175), static_cast<InstanceType>(175))));
    tmp9 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp10 = CodeStubAssembler(state_).LoadReference<Uint16T>(CodeStubAssembler::Reference{tmp1, tmp9});
    tmp11 = Convert_uint16_InstanceType_0(state_, TNode<Uint16T>{tmp10});
    tmp12 = Convert_int32_uint16_0(state_, TNode<Uint16T>{tmp11});
    tmp13 = FromConstexpr_InstanceType_constexpr_InstanceType_0(state_, static_cast<InstanceType>(175));
    tmp14 = Convert_uint16_InstanceType_0(state_, TNode<Uint16T>{tmp13});
    tmp15 = Convert_int32_uint16_0(state_, TNode<Uint16T>{tmp14});
    tmp16 = CodeStubAssembler(state_).Int32Sub(TNode<Int32T>{tmp12}, TNode<Int32T>{tmp15});
    tmp17 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp16});
    tmp18 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp8});
    tmp19 = CodeStubAssembler(state_).Uint32GreaterThan(TNode<Uint32T>{tmp17}, TNode<Uint32T>{tmp18});
    ca_.Branch(tmp19, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block1);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block5);
  }

  TNode<SwissNameDictionary> tmp20;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp20 = TORQUE_CAST(TNode<HeapObject>{p_o});
    ca_.Goto(&block15);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(label_CastError);
  }

    ca_.Bind(&block15);
  return TNode<SwissNameDictionary>{tmp20};
}


void FindEntry_GroupSse2Loader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint32T> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint32T> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint32T> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint32T> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint32T> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint32T> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint32T> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Uint32T> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Int32T> tmp2;
  TNode<Int32T> tmp3;
  TNode<BoolT> tmp4;
  TNode<Int32T> tmp5;
  TNode<Int32T> tmp6;
  TNode<Int32T> tmp7;
  TNode<Int32T> tmp8;
  TNode<Uint32T> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Uint32T> tmp14;
  TNode<Uint32T> tmp15;
  TNode<Uint32T> tmp16;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadNameHash(TNode<Name>{p_key});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp2 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_table, tmp1});
    tmp3 = FromConstexpr_int32_constexpr_int31_0(state_, 0);
    tmp4 = CodeStubAssembler(state_).Word32Equal(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp3});
    tmp5 = Convert_int32_bool_0(state_, TNode<BoolT>{tmp4});
    tmp6 = CodeStubAssembler(state_).Word32Or(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp5});
    tmp7 = FromConstexpr_int32_constexpr_int31_0(state_, 1);
    tmp8 = CodeStubAssembler(state_).Int32Sub(TNode<Int32T>{tmp6}, TNode<Int32T>{tmp7});
    tmp9 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp8});
    tmp10 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp2});
    tmp11 = SwissNameDictionaryCtrlTableStartOffsetMT_0(state_, TNode<IntPtrT>{tmp10});
    tmp12 = CodeStubAssembler(state_).BitcastTaggedToWord(TNode<Object>{p_table});
    tmp13 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp11}, TNode<IntPtrT>{tmp12});
    std::tie(tmp14, tmp15, tmp16) = Probe_0(state_, TNode<Uint32T>{tmp0}, TNode<Uint32T>{tmp9}).Flatten();
    ca_.Goto(&block5, tmp15, tmp16);
  }

  TNode<Uint32T> phi_bb5_8;
  TNode<Uint32T> phi_bb5_9;
  TNode<BoolT> tmp17;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_8, &phi_bb5_9);
    tmp17 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp17, &block3, std::vector<compiler::Node*>{phi_bb5_8, phi_bb5_9}, &block4, std::vector<compiler::Node*>{phi_bb5_8, phi_bb5_9});
  }

  TNode<Uint32T> phi_bb3_8;
  TNode<Uint32T> phi_bb3_9;
  TNode<IntPtrT> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<Simd128T> tmp20;
  TNode<I8x16T> tmp21;
  TNode<Uint32T> tmp22;
  TNode<Int32T> tmp23;
  TNode<I8x16T> tmp24;
  TNode<I8x16T> tmp25;
  TNode<Int32T> tmp26;
  TNode<Uint32T> tmp27;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_8, &phi_bb3_9);
    tmp18 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{phi_bb3_8});
    tmp19 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp13}, TNode<IntPtrT>{tmp18});
    tmp20 = CodeStubAssembler(state_).LoadSimd128(TNode<IntPtrT>{tmp19});
    tmp21 = Convert_I8X16_Simd128_0(state_, TNode<Simd128T>{tmp20});
    tmp22 = H2_0(state_, TNode<Uint32T>{tmp0});
    tmp23 = CodeStubAssembler(state_).Signed(TNode<Uint32T>{tmp22});
    tmp24 = CodeStubAssembler(state_).I8x16Splat(TNode<Int32T>{tmp23});
    tmp25 = CodeStubAssembler(state_).I8x16Eq(TNode<I8x16T>{tmp24}, TNode<I8x16T>{tmp21});
    tmp26 = CodeStubAssembler(state_).I8x16BitMask(TNode<I8x16T>{tmp25});
    tmp27 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp26});
    ca_.Goto(&block10, phi_bb3_8, phi_bb3_9, tmp27);
  }

  TNode<Uint32T> phi_bb10_8;
  TNode<Uint32T> phi_bb10_9;
  TNode<Uint32T> phi_bb10_11;
  TNode<Uint32T> tmp28;
  TNode<BoolT> tmp29;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_8, &phi_bb10_9, &phi_bb10_11);
    tmp28 = FromConstexpr_uint32_constexpr_int31_0(state_, 0);
    tmp29 = CodeStubAssembler(state_).Word32NotEqual(TNode<Uint32T>{phi_bb10_11}, TNode<Uint32T>{tmp28});
    ca_.Branch(tmp29, &block8, std::vector<compiler::Node*>{phi_bb10_8, phi_bb10_9, phi_bb10_11}, &block9, std::vector<compiler::Node*>{phi_bb10_8, phi_bb10_9, phi_bb10_11});
  }

  TNode<Uint32T> phi_bb8_8;
  TNode<Uint32T> phi_bb8_9;
  TNode<Uint32T> phi_bb8_11;
  TNode<Int32T> tmp30;
  TNode<Int32T> tmp31;
  TNode<Uint32T> tmp32;
  TNode<Uint32T> tmp33;
  TNode<Uint32T> tmp34;
  TNode<IntPtrT> tmp35;
  TNode<Name> tmp36;
  TNode<BoolT> tmp37;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_8, &phi_bb8_9, &phi_bb8_11);
    tmp30 = CodeStubAssembler(state_).CountTrailingZeros32(TNode<Uint32T>{phi_bb8_11});
    tmp31 = Convert_int32_int32_0(state_, TNode<Int32T>{tmp30});
    tmp32 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp31});
    tmp33 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb8_8}, TNode<Uint32T>{tmp32});
    tmp34 = CodeStubAssembler(state_).Word32And(TNode<Uint32T>{tmp33}, TNode<Uint32T>{tmp14});
    tmp35 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{tmp34});
    tmp36 = CodeStubAssembler(state_).LoadSwissNameDictionaryKey(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp35});
    tmp37 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{p_key}, TNode<MaybeObject>{tmp36});
    ca_.Branch(tmp37, &block14, std::vector<compiler::Node*>{phi_bb8_8, phi_bb8_9, phi_bb8_11}, &block15, std::vector<compiler::Node*>{phi_bb8_8, phi_bb8_9, phi_bb8_11});
  }

  TNode<Uint32T> phi_bb14_8;
  TNode<Uint32T> phi_bb14_9;
  TNode<Uint32T> phi_bb14_11;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_8, &phi_bb14_9, &phi_bb14_11);
    *label_Found_parameter_0 = tmp35;
    ca_.Goto(label_Found);
  }

  TNode<Uint32T> phi_bb15_8;
  TNode<Uint32T> phi_bb15_9;
  TNode<Uint32T> phi_bb15_11;
  TNode<Uint32T> tmp38;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_8, &phi_bb15_9, &phi_bb15_11);
    tmp38 = ClearLowestSetBit_uint32_0(state_, TNode<Uint32T>{phi_bb15_11});
    ca_.Goto(&block10, phi_bb15_8, phi_bb15_9, tmp38);
  }

  TNode<Uint32T> phi_bb9_8;
  TNode<Uint32T> phi_bb9_9;
  TNode<Uint32T> phi_bb9_11;
  TNode<Uint8T> tmp39;
  TNode<Int32T> tmp40;
  TNode<I8x16T> tmp41;
  TNode<I8x16T> tmp42;
  TNode<Int32T> tmp43;
  TNode<Uint32T> tmp44;
  TNode<Uint32T> tmp45;
  TNode<BoolT> tmp46;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_8, &phi_bb9_9, &phi_bb9_11);
    tmp39 = FromConstexpr_uint8_constexpr_uint8_0(state_, static_cast<uint8_t>(swiss_table::Ctrl::kEmpty));
    tmp40 = Convert_int32_uint8_0(state_, TNode<Uint8T>{tmp39});
    tmp41 = CodeStubAssembler(state_).I8x16Splat(TNode<Int32T>{tmp40});
    tmp42 = CodeStubAssembler(state_).I8x16Eq(TNode<I8x16T>{tmp41}, TNode<I8x16T>{tmp21});
    tmp43 = CodeStubAssembler(state_).I8x16BitMask(TNode<I8x16T>{tmp42});
    tmp44 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp43});
    tmp45 = FromConstexpr_uint32_constexpr_int31_0(state_, 0);
    tmp46 = CodeStubAssembler(state_).Word32NotEqual(TNode<Uint32T>{tmp44}, TNode<Uint32T>{tmp45});
    ca_.Branch(tmp46, &block17, std::vector<compiler::Node*>{phi_bb9_8, phi_bb9_9, phi_bb9_11}, &block18, std::vector<compiler::Node*>{phi_bb9_8, phi_bb9_9, phi_bb9_11});
  }

  TNode<Uint32T> phi_bb17_8;
  TNode<Uint32T> phi_bb17_9;
  TNode<Uint32T> phi_bb17_11;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_8, &phi_bb17_9, &phi_bb17_11);
    ca_.Goto(label_NotFound);
  }

  TNode<Uint32T> phi_bb18_8;
  TNode<Uint32T> phi_bb18_9;
  TNode<Uint32T> phi_bb18_11;
  TNode<Int32T> tmp47;
  TNode<Uint32T> tmp48;
  TNode<Uint32T> tmp49;
  TNode<Uint32T> tmp50;
  TNode<Uint32T> tmp51;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_8, &phi_bb18_9, &phi_bb18_11);
    tmp47 = FromConstexpr_int32_constexpr_int32_0(state_, swiss_table::Group::kWidth);
    tmp48 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp47});
    tmp49 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb18_9}, TNode<Uint32T>{tmp48});
    tmp50 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb18_8}, TNode<Uint32T>{tmp49});
    tmp51 = CodeStubAssembler(state_).Word32And(TNode<Uint32T>{tmp50}, TNode<Uint32T>{tmp14});
    ca_.Goto(&block5, tmp51, tmp49);
  }

  TNode<Uint32T> phi_bb4_8;
  TNode<Uint32T> phi_bb4_9;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_8, &phi_bb4_9);
    CodeStubAssembler(state_).Unreachable();
  }
}


void FindEntry_GroupPortableLoader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, compiler::CodeAssemblerLabel* label_Found, compiler::TypedCodeAssemblerVariable<IntPtrT>* label_Found_parameter_0, compiler::CodeAssemblerLabel* label_NotFound) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint64T> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint64T> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint64T> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint64T> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint64T> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint64T> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T, Uint64T> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Uint32T> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Int32T> tmp2;
  TNode<Int32T> tmp3;
  TNode<BoolT> tmp4;
  TNode<Int32T> tmp5;
  TNode<Int32T> tmp6;
  TNode<Int32T> tmp7;
  TNode<Int32T> tmp8;
  TNode<Uint32T> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Uint32T> tmp14;
  TNode<Uint32T> tmp15;
  TNode<Uint32T> tmp16;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadNameHash(TNode<Name>{p_key});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp2 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_table, tmp1});
    tmp3 = FromConstexpr_int32_constexpr_int31_0(state_, 0);
    tmp4 = CodeStubAssembler(state_).Word32Equal(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp3});
    tmp5 = Convert_int32_bool_0(state_, TNode<BoolT>{tmp4});
    tmp6 = CodeStubAssembler(state_).Word32Or(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp5});
    tmp7 = FromConstexpr_int32_constexpr_int31_0(state_, 1);
    tmp8 = CodeStubAssembler(state_).Int32Sub(TNode<Int32T>{tmp6}, TNode<Int32T>{tmp7});
    tmp9 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp8});
    tmp10 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp2});
    tmp11 = SwissNameDictionaryCtrlTableStartOffsetMT_0(state_, TNode<IntPtrT>{tmp10});
    tmp12 = CodeStubAssembler(state_).BitcastTaggedToWord(TNode<Object>{p_table});
    tmp13 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp11}, TNode<IntPtrT>{tmp12});
    std::tie(tmp14, tmp15, tmp16) = Probe_0(state_, TNode<Uint32T>{tmp0}, TNode<Uint32T>{tmp9}).Flatten();
    ca_.Goto(&block5, tmp15, tmp16);
  }

  TNode<Uint32T> phi_bb5_8;
  TNode<Uint32T> phi_bb5_9;
  TNode<BoolT> tmp17;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_8, &phi_bb5_9);
    tmp17 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp17, &block3, std::vector<compiler::Node*>{phi_bb5_8, phi_bb5_9}, &block4, std::vector<compiler::Node*>{phi_bb5_8, phi_bb5_9});
  }

  TNode<Uint32T> phi_bb3_8;
  TNode<Uint32T> phi_bb3_9;
  TNode<IntPtrT> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<Uint64T> tmp20;
  TNode<Uint32T> tmp21;
  TNode<Uint64T> tmp22;
  TNode<Uint64T> tmp23;
  TNode<Uint64T> tmp24;
  TNode<Uint64T> tmp25;
  TNode<Uint64T> tmp26;
  TNode<Uint64T> tmp27;
  TNode<Uint64T> tmp28;
  TNode<Uint64T> tmp29;
  TNode<Uint64T> tmp30;
  TNode<Uint64T> tmp31;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_8, &phi_bb3_9);
    tmp18 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{phi_bb3_8});
    tmp19 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp13}, TNode<IntPtrT>{tmp18});
    tmp20 = CodeStubAssembler(state_).LoadSwissNameDictionaryCtrlTableGroup(TNode<IntPtrT>{tmp19});
    tmp21 = H2_0(state_, TNode<Uint32T>{tmp0});
    tmp22 = Convert_uint64_uint32_0(state_, TNode<Uint32T>{tmp21});
    tmp23 = FromConstexpr_uint64_constexpr_uint64_0(state_, swiss_table::GroupPortableImpl::kLsbs);
    tmp24 = CodeStubAssembler(state_).Uint64Mul(TNode<Uint64T>{tmp23}, TNode<Uint64T>{tmp22});
    tmp25 = CodeStubAssembler(state_).Word64Xor(TNode<Uint64T>{tmp20}, TNode<Uint64T>{tmp24});
    tmp26 = FromConstexpr_uint64_constexpr_uint64_0(state_, swiss_table::GroupPortableImpl::kLsbs);
    tmp27 = CodeStubAssembler(state_).Uint64Sub(TNode<Uint64T>{tmp25}, TNode<Uint64T>{tmp26});
    tmp28 = CodeStubAssembler(state_).Word64Not(TNode<Uint64T>{tmp25});
    tmp29 = CodeStubAssembler(state_).Word64And(TNode<Uint64T>{tmp27}, TNode<Uint64T>{tmp28});
    tmp30 = FromConstexpr_uint64_constexpr_uint64_0(state_, swiss_table::GroupPortableImpl::kMsbs);
    tmp31 = CodeStubAssembler(state_).Word64And(TNode<Uint64T>{tmp29}, TNode<Uint64T>{tmp30});
    ca_.Goto(&block10, phi_bb3_8, phi_bb3_9, tmp31);
  }

  TNode<Uint32T> phi_bb10_8;
  TNode<Uint32T> phi_bb10_9;
  TNode<Uint64T> phi_bb10_11;
  TNode<Uint64T> tmp32;
  TNode<BoolT> tmp33;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_8, &phi_bb10_9, &phi_bb10_11);
    tmp32 = FromConstexpr_uint64_constexpr_int31_0(state_, 0);
    tmp33 = CodeStubAssembler(state_).Word64NotEqual(TNode<Uint64T>{phi_bb10_11}, TNode<Uint64T>{tmp32});
    ca_.Branch(tmp33, &block8, std::vector<compiler::Node*>{phi_bb10_8, phi_bb10_9, phi_bb10_11}, &block9, std::vector<compiler::Node*>{phi_bb10_8, phi_bb10_9, phi_bb10_11});
  }

  TNode<Uint32T> phi_bb8_8;
  TNode<Uint32T> phi_bb8_9;
  TNode<Uint64T> phi_bb8_11;
  TNode<Int64T> tmp34;
  TNode<Uint64T> tmp35;
  TNode<Int64T> tmp36;
  TNode<Int64T> tmp37;
  TNode<Int32T> tmp38;
  TNode<Uint32T> tmp39;
  TNode<Uint32T> tmp40;
  TNode<Uint32T> tmp41;
  TNode<IntPtrT> tmp42;
  TNode<Name> tmp43;
  TNode<BoolT> tmp44;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_8, &phi_bb8_9, &phi_bb8_11);
    tmp34 = CodeStubAssembler(state_).CountTrailingZeros64(TNode<Uint64T>{phi_bb8_11});
    tmp35 = kByteMaskShift_0(state_);
    tmp36 = CodeStubAssembler(state_).Signed(TNode<Uint64T>{tmp35});
    tmp37 = CodeStubAssembler(state_).Word64Sar(TNode<Int64T>{tmp34}, TNode<Int64T>{tmp36});
    tmp38 = Convert_int32_int64_0(state_, TNode<Int64T>{tmp37});
    tmp39 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp38});
    tmp40 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb8_8}, TNode<Uint32T>{tmp39});
    tmp41 = CodeStubAssembler(state_).Word32And(TNode<Uint32T>{tmp40}, TNode<Uint32T>{tmp14});
    tmp42 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{tmp41});
    tmp43 = CodeStubAssembler(state_).LoadSwissNameDictionaryKey(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp42});
    tmp44 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{p_key}, TNode<MaybeObject>{tmp43});
    ca_.Branch(tmp44, &block14, std::vector<compiler::Node*>{phi_bb8_8, phi_bb8_9, phi_bb8_11}, &block15, std::vector<compiler::Node*>{phi_bb8_8, phi_bb8_9, phi_bb8_11});
  }

  TNode<Uint32T> phi_bb14_8;
  TNode<Uint32T> phi_bb14_9;
  TNode<Uint64T> phi_bb14_11;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_8, &phi_bb14_9, &phi_bb14_11);
    *label_Found_parameter_0 = tmp42;
    ca_.Goto(label_Found);
  }

  TNode<Uint32T> phi_bb15_8;
  TNode<Uint32T> phi_bb15_9;
  TNode<Uint64T> phi_bb15_11;
  TNode<Uint64T> tmp45;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_8, &phi_bb15_9, &phi_bb15_11);
    tmp45 = ClearLowestSetBit_uint64_0(state_, TNode<Uint64T>{phi_bb15_11});
    ca_.Goto(&block10, phi_bb15_8, phi_bb15_9, tmp45);
  }

  TNode<Uint32T> phi_bb9_8;
  TNode<Uint32T> phi_bb9_9;
  TNode<Uint64T> phi_bb9_11;
  TNode<Uint64T> tmp46;
  TNode<Uint64T> tmp47;
  TNode<Uint64T> tmp48;
  TNode<Uint64T> tmp49;
  TNode<Uint64T> tmp50;
  TNode<Uint64T> tmp51;
  TNode<Uint64T> tmp52;
  TNode<BoolT> tmp53;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_8, &phi_bb9_9, &phi_bb9_11);
    tmp46 = CodeStubAssembler(state_).Word64Not(TNode<Uint64T>{tmp20});
    tmp47 = FromConstexpr_uint64_constexpr_int31_0(state_, 6);
    tmp48 = CodeStubAssembler(state_).Word64Shl(TNode<Uint64T>{tmp46}, TNode<Uint64T>{tmp47});
    tmp49 = CodeStubAssembler(state_).Word64And(TNode<Uint64T>{tmp20}, TNode<Uint64T>{tmp48});
    tmp50 = FromConstexpr_uint64_constexpr_uint64_0(state_, swiss_table::GroupPortableImpl::kMsbs);
    tmp51 = CodeStubAssembler(state_).Word64And(TNode<Uint64T>{tmp49}, TNode<Uint64T>{tmp50});
    tmp52 = FromConstexpr_uint64_constexpr_int31_0(state_, 0);
    tmp53 = CodeStubAssembler(state_).Word64NotEqual(TNode<Uint64T>{tmp51}, TNode<Uint64T>{tmp52});
    ca_.Branch(tmp53, &block17, std::vector<compiler::Node*>{phi_bb9_8, phi_bb9_9, phi_bb9_11}, &block18, std::vector<compiler::Node*>{phi_bb9_8, phi_bb9_9, phi_bb9_11});
  }

  TNode<Uint32T> phi_bb17_8;
  TNode<Uint32T> phi_bb17_9;
  TNode<Uint64T> phi_bb17_11;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_8, &phi_bb17_9, &phi_bb17_11);
    ca_.Goto(label_NotFound);
  }

  TNode<Uint32T> phi_bb18_8;
  TNode<Uint32T> phi_bb18_9;
  TNode<Uint64T> phi_bb18_11;
  TNode<Int32T> tmp54;
  TNode<Uint32T> tmp55;
  TNode<Uint32T> tmp56;
  TNode<Uint32T> tmp57;
  TNode<Uint32T> tmp58;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_8, &phi_bb18_9, &phi_bb18_11);
    tmp54 = FromConstexpr_int32_constexpr_int32_0(state_, swiss_table::Group::kWidth);
    tmp55 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp54});
    tmp56 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb18_9}, TNode<Uint32T>{tmp55});
    tmp57 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb18_8}, TNode<Uint32T>{tmp56});
    tmp58 = CodeStubAssembler(state_).Word32And(TNode<Uint32T>{tmp57}, TNode<Uint32T>{tmp14});
    ca_.Goto(&block5, tmp58, tmp56);
  }

  TNode<Uint32T> phi_bb4_8;
  TNode<Uint32T> phi_bb4_9;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_8, &phi_bb4_9);
    CodeStubAssembler(state_).Unreachable();
  }
}


void Add_GroupSse2Loader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Int32T> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<Int32T> tmp4;
  TNode<Uint32T> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<ByteArray> tmp7;
  TNode<Uint32T> tmp8;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp1 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_table, tmp0});
    tmp2 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp1});
    tmp3 = SwissNameDictionaryMaxUsableCapacity_0(state_, TNode<IntPtrT>{tmp2});
    tmp4 = Convert_int32_intptr_0(state_, TNode<IntPtrT>{tmp3});
    tmp5 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp4});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp7 = CodeStubAssembler(state_).LoadReference<ByteArray>(CodeStubAssembler::Reference{p_table, tmp6});
    compiler::CodeAssemblerLabel label9(&ca_);
    tmp8 = CodeStubAssembler(state_).SwissNameDictionaryIncreaseElementCountOrBailout(TNode<ByteArray>{tmp7}, TNode<IntPtrT>{tmp2}, TNode<Uint32T>{tmp5}, &label9);
    ca_.Goto(&block5);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(label_Bailout);
  }

  TNode<Uint32T> tmp10;
  TNode<Int32T> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Uint32T> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<Uint8T> tmp16;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp10 = CodeStubAssembler(state_).LoadNameHash(TNode<Name>{p_key});
    tmp11 = FindFirstEmpty_GroupSse2Loader_0(state_, TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp2}, TNode<Uint32T>{tmp10});
    tmp12 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp11});
    CodeStubAssembler(state_).StoreSwissNameDictionaryKeyAndValue(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp12}, TNode<Object>{p_key}, TNode<Object>{p_value});
    tmp13 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{tmp8});
    CodeStubAssembler(state_).StoreSwissNameDictionaryEnumToEntryMapping(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp13}, TNode<Int32T>{tmp11});
    tmp14 = H2_0(state_, TNode<Uint32T>{tmp10});
    tmp15 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{tmp14});
    tmp16 = Convert_uint8_intptr_0(state_, TNode<IntPtrT>{tmp15});
    CodeStubAssembler(state_).SwissNameDictionarySetCtrl(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp12}, TNode<Uint8T>{tmp16});
    CodeStubAssembler(state_).StoreSwissNameDictionaryPropertyDetails(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp12}, TNode<Uint8T>{p_propertyDetails});
    ca_.Goto(&block7);
  }

    ca_.Bind(&block7);
}


void Add_GroupPortableLoader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<Name> p_key, TNode<Object> p_value, TNode<Uint8T> p_propertyDetails, compiler::CodeAssemblerLabel* label_Bailout) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Int32T> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<Int32T> tmp4;
  TNode<Uint32T> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<ByteArray> tmp7;
  TNode<Uint32T> tmp8;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp1 = CodeStubAssembler(state_).LoadReference<Int32T>(CodeStubAssembler::Reference{p_table, tmp0});
    tmp2 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp1});
    tmp3 = SwissNameDictionaryMaxUsableCapacity_0(state_, TNode<IntPtrT>{tmp2});
    tmp4 = Convert_int32_intptr_0(state_, TNode<IntPtrT>{tmp3});
    tmp5 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp4});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp7 = CodeStubAssembler(state_).LoadReference<ByteArray>(CodeStubAssembler::Reference{p_table, tmp6});
    compiler::CodeAssemblerLabel label9(&ca_);
    tmp8 = CodeStubAssembler(state_).SwissNameDictionaryIncreaseElementCountOrBailout(TNode<ByteArray>{tmp7}, TNode<IntPtrT>{tmp2}, TNode<Uint32T>{tmp5}, &label9);
    ca_.Goto(&block5);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(label_Bailout);
  }

  TNode<Uint32T> tmp10;
  TNode<Int32T> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Uint32T> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<Uint8T> tmp16;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp10 = CodeStubAssembler(state_).LoadNameHash(TNode<Name>{p_key});
    tmp11 = FindFirstEmpty_GroupPortableLoader_0(state_, TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp2}, TNode<Uint32T>{tmp10});
    tmp12 = Convert_intptr_int32_0(state_, TNode<Int32T>{tmp11});
    CodeStubAssembler(state_).StoreSwissNameDictionaryKeyAndValue(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp12}, TNode<Object>{p_key}, TNode<Object>{p_value});
    tmp13 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{tmp8});
    CodeStubAssembler(state_).StoreSwissNameDictionaryEnumToEntryMapping(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp13}, TNode<Int32T>{tmp11});
    tmp14 = H2_0(state_, TNode<Uint32T>{tmp10});
    tmp15 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{tmp14});
    tmp16 = Convert_uint8_intptr_0(state_, TNode<IntPtrT>{tmp15});
    CodeStubAssembler(state_).SwissNameDictionarySetCtrl(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp12}, TNode<Uint8T>{tmp16});
    CodeStubAssembler(state_).StoreSwissNameDictionaryPropertyDetails(TNode<SwissNameDictionary>{p_table}, TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp12}, TNode<Uint8T>{p_propertyDetails});
    ca_.Goto(&block7);
  }

    ca_.Bind(&block7);
}


TNode<Int32T> FindFirstEmpty_GroupSse2Loader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<IntPtrT> p_capacity, TNode<Uint32T> p_hash) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Int32T> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<BoolT> tmp2;
  TNode<Int32T> tmp3;
  TNode<Int32T> tmp4;
  TNode<Int32T> tmp5;
  TNode<Int32T> tmp6;
  TNode<Uint32T> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<Uint32T> tmp11;
  TNode<Uint32T> tmp12;
  TNode<Uint32T> tmp13;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = Convert_int32_intptr_0(state_, TNode<IntPtrT>{p_capacity});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp2 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{p_capacity}, TNode<IntPtrT>{tmp1});
    tmp3 = Convert_int32_bool_0(state_, TNode<BoolT>{tmp2});
    tmp4 = CodeStubAssembler(state_).Word32Or(TNode<Int32T>{tmp0}, TNode<Int32T>{tmp3});
    tmp5 = FromConstexpr_int32_constexpr_int31_0(state_, 1);
    tmp6 = CodeStubAssembler(state_).Int32Sub(TNode<Int32T>{tmp4}, TNode<Int32T>{tmp5});
    tmp7 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp6});
    tmp8 = SwissNameDictionaryCtrlTableStartOffsetMT_0(state_, TNode<IntPtrT>{p_capacity});
    tmp9 = CodeStubAssembler(state_).BitcastTaggedToWord(TNode<Object>{p_table});
    tmp10 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp8}, TNode<IntPtrT>{tmp9});
    std::tie(tmp11, tmp12, tmp13) = Probe_0(state_, TNode<Uint32T>{p_hash}, TNode<Uint32T>{tmp7}).Flatten();
    ca_.Goto(&block4, tmp12, tmp13);
  }

  TNode<Uint32T> phi_bb4_7;
  TNode<Uint32T> phi_bb4_8;
  TNode<BoolT> tmp14;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_7, &phi_bb4_8);
    tmp14 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp14, &block2, std::vector<compiler::Node*>{phi_bb4_7, phi_bb4_8}, &block3, std::vector<compiler::Node*>{phi_bb4_7, phi_bb4_8});
  }

  TNode<Uint32T> phi_bb2_7;
  TNode<Uint32T> phi_bb2_8;
  TNode<IntPtrT> tmp15;
  TNode<IntPtrT> tmp16;
  TNode<Simd128T> tmp17;
  TNode<I8x16T> tmp18;
  TNode<Uint8T> tmp19;
  TNode<Int32T> tmp20;
  TNode<I8x16T> tmp21;
  TNode<I8x16T> tmp22;
  TNode<Int32T> tmp23;
  TNode<Uint32T> tmp24;
  TNode<Uint32T> tmp25;
  TNode<BoolT> tmp26;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_7, &phi_bb2_8);
    tmp15 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{phi_bb2_7});
    tmp16 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp10}, TNode<IntPtrT>{tmp15});
    tmp17 = CodeStubAssembler(state_).LoadSimd128(TNode<IntPtrT>{tmp16});
    tmp18 = Convert_I8X16_Simd128_0(state_, TNode<Simd128T>{tmp17});
    tmp19 = FromConstexpr_uint8_constexpr_uint8_0(state_, static_cast<uint8_t>(swiss_table::Ctrl::kEmpty));
    tmp20 = Convert_int32_uint8_0(state_, TNode<Uint8T>{tmp19});
    tmp21 = CodeStubAssembler(state_).I8x16Splat(TNode<Int32T>{tmp20});
    tmp22 = CodeStubAssembler(state_).I8x16Eq(TNode<I8x16T>{tmp21}, TNode<I8x16T>{tmp18});
    tmp23 = CodeStubAssembler(state_).I8x16BitMask(TNode<I8x16T>{tmp22});
    tmp24 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp23});
    tmp25 = FromConstexpr_uint32_constexpr_int31_0(state_, 0);
    tmp26 = CodeStubAssembler(state_).Word32NotEqual(TNode<Uint32T>{tmp24}, TNode<Uint32T>{tmp25});
    ca_.Branch(tmp26, &block7, std::vector<compiler::Node*>{phi_bb2_7, phi_bb2_8}, &block8, std::vector<compiler::Node*>{phi_bb2_7, phi_bb2_8});
  }

  TNode<Uint32T> phi_bb7_7;
  TNode<Uint32T> phi_bb7_8;
  TNode<Int32T> tmp27;
  TNode<Int32T> tmp28;
  TNode<Uint32T> tmp29;
  TNode<Uint32T> tmp30;
  TNode<Uint32T> tmp31;
  TNode<Int32T> tmp32;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_7, &phi_bb7_8);
    tmp27 = CodeStubAssembler(state_).CountTrailingZeros32(TNode<Uint32T>{tmp24});
    tmp28 = Convert_int32_int32_0(state_, TNode<Int32T>{tmp27});
    tmp29 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp28});
    tmp30 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb7_7}, TNode<Uint32T>{tmp29});
    tmp31 = CodeStubAssembler(state_).Word32And(TNode<Uint32T>{tmp30}, TNode<Uint32T>{tmp11});
    tmp32 = CodeStubAssembler(state_).Signed(TNode<Uint32T>{tmp31});
    ca_.Goto(&block13);
  }

  TNode<Uint32T> phi_bb8_7;
  TNode<Uint32T> phi_bb8_8;
  TNode<Int32T> tmp33;
  TNode<Uint32T> tmp34;
  TNode<Uint32T> tmp35;
  TNode<Uint32T> tmp36;
  TNode<Uint32T> tmp37;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_7, &phi_bb8_8);
    tmp33 = FromConstexpr_int32_constexpr_int32_0(state_, swiss_table::Group::kWidth);
    tmp34 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp33});
    tmp35 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb8_8}, TNode<Uint32T>{tmp34});
    tmp36 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb8_7}, TNode<Uint32T>{tmp35});
    tmp37 = CodeStubAssembler(state_).Word32And(TNode<Uint32T>{tmp36}, TNode<Uint32T>{tmp11});
    ca_.Goto(&block4, tmp37, tmp35);
  }

  TNode<Uint32T> phi_bb3_7;
  TNode<Uint32T> phi_bb3_8;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_7, &phi_bb3_8);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block13);
  return TNode<Int32T>{tmp32};
}


TNode<Int32T> FindFirstEmpty_GroupPortableLoader_0(compiler::CodeAssemblerState* state_, TNode<SwissNameDictionary> p_table, TNode<IntPtrT> p_capacity, TNode<Uint32T> p_hash) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Uint32T, Uint32T> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Int32T> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<BoolT> tmp2;
  TNode<Int32T> tmp3;
  TNode<Int32T> tmp4;
  TNode<Int32T> tmp5;
  TNode<Int32T> tmp6;
  TNode<Uint32T> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<Uint32T> tmp11;
  TNode<Uint32T> tmp12;
  TNode<Uint32T> tmp13;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = Convert_int32_intptr_0(state_, TNode<IntPtrT>{p_capacity});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp2 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{p_capacity}, TNode<IntPtrT>{tmp1});
    tmp3 = Convert_int32_bool_0(state_, TNode<BoolT>{tmp2});
    tmp4 = CodeStubAssembler(state_).Word32Or(TNode<Int32T>{tmp0}, TNode<Int32T>{tmp3});
    tmp5 = FromConstexpr_int32_constexpr_int31_0(state_, 1);
    tmp6 = CodeStubAssembler(state_).Int32Sub(TNode<Int32T>{tmp4}, TNode<Int32T>{tmp5});
    tmp7 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp6});
    tmp8 = SwissNameDictionaryCtrlTableStartOffsetMT_0(state_, TNode<IntPtrT>{p_capacity});
    tmp9 = CodeStubAssembler(state_).BitcastTaggedToWord(TNode<Object>{p_table});
    tmp10 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp8}, TNode<IntPtrT>{tmp9});
    std::tie(tmp11, tmp12, tmp13) = Probe_0(state_, TNode<Uint32T>{p_hash}, TNode<Uint32T>{tmp7}).Flatten();
    ca_.Goto(&block4, tmp12, tmp13);
  }

  TNode<Uint32T> phi_bb4_7;
  TNode<Uint32T> phi_bb4_8;
  TNode<BoolT> tmp14;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_7, &phi_bb4_8);
    tmp14 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp14, &block2, std::vector<compiler::Node*>{phi_bb4_7, phi_bb4_8}, &block3, std::vector<compiler::Node*>{phi_bb4_7, phi_bb4_8});
  }

  TNode<Uint32T> phi_bb2_7;
  TNode<Uint32T> phi_bb2_8;
  TNode<IntPtrT> tmp15;
  TNode<IntPtrT> tmp16;
  TNode<Uint64T> tmp17;
  TNode<Uint64T> tmp18;
  TNode<Uint64T> tmp19;
  TNode<Uint64T> tmp20;
  TNode<Uint64T> tmp21;
  TNode<Uint64T> tmp22;
  TNode<Uint64T> tmp23;
  TNode<Uint64T> tmp24;
  TNode<BoolT> tmp25;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_7, &phi_bb2_8);
    tmp15 = Convert_intptr_uint32_0(state_, TNode<Uint32T>{phi_bb2_7});
    tmp16 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp10}, TNode<IntPtrT>{tmp15});
    tmp17 = CodeStubAssembler(state_).LoadSwissNameDictionaryCtrlTableGroup(TNode<IntPtrT>{tmp16});
    tmp18 = CodeStubAssembler(state_).Word64Not(TNode<Uint64T>{tmp17});
    tmp19 = FromConstexpr_uint64_constexpr_int31_0(state_, 6);
    tmp20 = CodeStubAssembler(state_).Word64Shl(TNode<Uint64T>{tmp18}, TNode<Uint64T>{tmp19});
    tmp21 = CodeStubAssembler(state_).Word64And(TNode<Uint64T>{tmp17}, TNode<Uint64T>{tmp20});
    tmp22 = FromConstexpr_uint64_constexpr_uint64_0(state_, swiss_table::GroupPortableImpl::kMsbs);
    tmp23 = CodeStubAssembler(state_).Word64And(TNode<Uint64T>{tmp21}, TNode<Uint64T>{tmp22});
    tmp24 = FromConstexpr_uint64_constexpr_int31_0(state_, 0);
    tmp25 = CodeStubAssembler(state_).Word64NotEqual(TNode<Uint64T>{tmp23}, TNode<Uint64T>{tmp24});
    ca_.Branch(tmp25, &block7, std::vector<compiler::Node*>{phi_bb2_7, phi_bb2_8}, &block8, std::vector<compiler::Node*>{phi_bb2_7, phi_bb2_8});
  }

  TNode<Uint32T> phi_bb7_7;
  TNode<Uint32T> phi_bb7_8;
  TNode<Int64T> tmp26;
  TNode<Uint64T> tmp27;
  TNode<Int64T> tmp28;
  TNode<Int64T> tmp29;
  TNode<Int32T> tmp30;
  TNode<Uint32T> tmp31;
  TNode<Uint32T> tmp32;
  TNode<Uint32T> tmp33;
  TNode<Int32T> tmp34;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_7, &phi_bb7_8);
    tmp26 = CodeStubAssembler(state_).CountTrailingZeros64(TNode<Uint64T>{tmp23});
    tmp27 = kByteMaskShift_0(state_);
    tmp28 = CodeStubAssembler(state_).Signed(TNode<Uint64T>{tmp27});
    tmp29 = CodeStubAssembler(state_).Word64Sar(TNode<Int64T>{tmp26}, TNode<Int64T>{tmp28});
    tmp30 = Convert_int32_int64_0(state_, TNode<Int64T>{tmp29});
    tmp31 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp30});
    tmp32 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb7_7}, TNode<Uint32T>{tmp31});
    tmp33 = CodeStubAssembler(state_).Word32And(TNode<Uint32T>{tmp32}, TNode<Uint32T>{tmp11});
    tmp34 = CodeStubAssembler(state_).Signed(TNode<Uint32T>{tmp33});
    ca_.Goto(&block13);
  }

  TNode<Uint32T> phi_bb8_7;
  TNode<Uint32T> phi_bb8_8;
  TNode<Int32T> tmp35;
  TNode<Uint32T> tmp36;
  TNode<Uint32T> tmp37;
  TNode<Uint32T> tmp38;
  TNode<Uint32T> tmp39;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_7, &phi_bb8_8);
    tmp35 = FromConstexpr_int32_constexpr_int32_0(state_, swiss_table::Group::kWidth);
    tmp36 = CodeStubAssembler(state_).Unsigned(TNode<Int32T>{tmp35});
    tmp37 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb8_8}, TNode<Uint32T>{tmp36});
    tmp38 = CodeStubAssembler(state_).Uint32Add(TNode<Uint32T>{phi_bb8_7}, TNode<Uint32T>{tmp37});
    tmp39 = CodeStubAssembler(state_).Word32And(TNode<Uint32T>{tmp38}, TNode<Uint32T>{tmp11});
    ca_.Goto(&block4, tmp39, tmp37);
  }

  TNode<Uint32T> phi_bb3_7;
  TNode<Uint32T> phi_bb3_8;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_7, &phi_bb3_8);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block13);
  return TNode<Int32T>{tmp34};
}


} // namespace internal
} // namespace v8
