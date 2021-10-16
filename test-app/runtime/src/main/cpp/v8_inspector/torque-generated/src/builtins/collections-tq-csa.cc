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
#include "torque-generated/src/builtins/collections-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-findindex-tq-csa.h"
#include "torque-generated/src/builtins/array-reverse-tq-csa.h"
#include "torque-generated/src/builtins/array-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/collections-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"

namespace v8 {
namespace internal {
TorqueStructKeyValuePair LoadKeyValuePairNoSideEffects_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o, compiler::CodeAssemblerLabel* label_MayHaveSideEffects) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_FastJSArray_1(state_, TNode<Context>{p_context}, TNode<Object>{p_o}, &label1);
    ca_.Goto(&block5);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block6);
    }
  }

  TNode<JSReceiver> tmp2;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_JSReceiver_1(state_, TNode<Context>{p_context}, TNode<Object>{ca_.UncheckedCast<Object>(p_o)}, &label3);
    ca_.Goto(&block33);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block34);
    }
  }

  TNode<Smi> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<FixedArrayBase> tmp6;
  TNode<FixedArray> tmp7;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp4 = CodeStubAssembler(state_).LoadFastJSArrayLength(TNode<JSArray>{tmp0});
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp6 = CodeStubAssembler(state_).LoadReference<FixedArrayBase>(CodeStubAssembler::Reference{tmp0, tmp5});
    compiler::CodeAssemblerLabel label8(&ca_);
    tmp7 = Cast_FixedArray_0(state_, TNode<HeapObject>{tmp6}, &label8);
    ca_.Goto(&block9);
    if (label8.is_used()) {
      ca_.Bind(&label8);
      ca_.Goto(&block10);
    }
  }

  TNode<FixedDoubleArray> tmp9;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    compiler::CodeAssemblerLabel label10(&ca_);
    tmp9 = Cast_FixedDoubleArray_0(state_, TNode<HeapObject>{ca_.UncheckedCast<FixedArrayBase>(tmp6)}, &label10);
    ca_.Goto(&block21);
    if (label10.is_used()) {
      ca_.Bind(&label10);
      ca_.Goto(&block22);
    }
  }

  TNode<Smi> tmp11;
  TNode<BoolT> tmp12;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp11 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp12 = CodeStubAssembler(state_).SmiGreaterThan(TNode<Smi>{tmp4}, TNode<Smi>{tmp11});
    ca_.Branch(tmp12, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  TNode<Smi> tmp13;
  TNode<Object> tmp14;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp13 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp14 = LoadElementOrUndefined_0(state_, TNode<Context>{p_context}, TNode<FixedArray>{tmp7}, TNode<Smi>{tmp13});
    ca_.Goto(&block13, tmp14);
  }

  TNode<Oddball> tmp15;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp15 = Undefined_0(state_);
    ca_.Goto(&block13, tmp15);
  }

  TNode<Object> phi_bb13_7;
  TNode<Smi> tmp16;
  TNode<BoolT> tmp17;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_7);
    tmp16 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp17 = CodeStubAssembler(state_).SmiGreaterThan(TNode<Smi>{tmp4}, TNode<Smi>{tmp16});
    ca_.Branch(tmp17, &block15, std::vector<compiler::Node*>{}, &block16, std::vector<compiler::Node*>{});
  }

  TNode<Smi> tmp18;
  TNode<Object> tmp19;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp18 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp19 = LoadElementOrUndefined_0(state_, TNode<Context>{p_context}, TNode<FixedArray>{tmp7}, TNode<Smi>{tmp18});
    ca_.Goto(&block17, tmp19);
  }

  TNode<Oddball> tmp20;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp20 = Undefined_0(state_);
    ca_.Goto(&block17, tmp20);
  }

  TNode<Object> phi_bb17_8;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_8);
    ca_.Goto(&block2, phi_bb13_7, phi_bb17_8);
  }

  if (block22.is_used()) {
    ca_.Bind(&block22);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Smi> tmp21;
  TNode<BoolT> tmp22;
  if (block21.is_used()) {
    ca_.Bind(&block21);
    tmp21 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp22 = CodeStubAssembler(state_).SmiGreaterThan(TNode<Smi>{tmp4}, TNode<Smi>{tmp21});
    ca_.Branch(tmp22, &block23, std::vector<compiler::Node*>{}, &block24, std::vector<compiler::Node*>{});
  }

  TNode<Smi> tmp23;
  TNode<Object> tmp24;
  if (block23.is_used()) {
    ca_.Bind(&block23);
    tmp23 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp24 = LoadElementOrUndefined_1(state_, TNode<FixedDoubleArray>{tmp9}, TNode<Smi>{tmp23});
    ca_.Goto(&block25, tmp24);
  }

  TNode<Oddball> tmp25;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp25 = Undefined_0(state_);
    ca_.Goto(&block25, tmp25);
  }

  TNode<Object> phi_bb25_7;
  TNode<Smi> tmp26;
  TNode<BoolT> tmp27;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_7);
    tmp26 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp27 = CodeStubAssembler(state_).SmiGreaterThan(TNode<Smi>{tmp4}, TNode<Smi>{tmp26});
    ca_.Branch(tmp27, &block27, std::vector<compiler::Node*>{}, &block28, std::vector<compiler::Node*>{});
  }

  TNode<Smi> tmp28;
  TNode<Object> tmp29;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp28 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp29 = LoadElementOrUndefined_1(state_, TNode<FixedDoubleArray>{tmp9}, TNode<Smi>{tmp28});
    ca_.Goto(&block29, tmp29);
  }

  TNode<Oddball> tmp30;
  if (block28.is_used()) {
    ca_.Bind(&block28);
    tmp30 = Undefined_0(state_);
    ca_.Goto(&block29, tmp30);
  }

  TNode<Object> phi_bb29_8;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_8);
    ca_.Goto(&block2, phi_bb25_7, phi_bb29_8);
  }

  if (block34.is_used()) {
    ca_.Bind(&block34);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kIteratorValueNotAnObject, TNode<Object>{ca_.UncheckedCast<Object>(p_o)});
  }

  if (block33.is_used()) {
    ca_.Bind(&block33);
    ca_.Goto(label_MayHaveSideEffects);
  }

  TNode<Object> phi_bb2_2;
  TNode<Object> phi_bb2_3;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_2, &phi_bb2_3);
    ca_.Goto(&block35);
  }

    ca_.Bind(&block35);
  return TorqueStructKeyValuePair{TNode<Object>{phi_bb2_2}, TNode<Object>{phi_bb2_3}};
}


TorqueStructKeyValuePair LoadKeyValuePair_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<Object> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label2(&ca_);
    std::tie(tmp0, tmp1) = LoadKeyValuePairNoSideEffects_0(state_, TNode<Context>{p_context}, TNode<Object>{p_o}, &label2).Flatten();
    ca_.Goto(&block4);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block5);
    }
  }

  TNode<Smi> tmp3;
  TNode<Object> tmp4;
  TNode<Smi> tmp5;
  TNode<Object> tmp6;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp3 = Convert_Smi_constexpr_int31_0(state_, 0);
    tmp4 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{p_o}, TNode<Object>{tmp3});
    tmp5 = Convert_Smi_constexpr_int31_0(state_, 1);
    tmp6 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{p_o}, TNode<Object>{tmp5});
    ca_.Goto(&block1, tmp4, tmp6);
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block1, tmp0, tmp1);
  }

  TNode<Object> phi_bb1_2;
  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_2, &phi_bb1_3);
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TorqueStructKeyValuePair{TNode<Object>{phi_bb1_2}, TNode<Object>{phi_bb1_3}};
}


} // namespace internal
} // namespace v8
