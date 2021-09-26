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
#include "torque-generated/src/builtins/typed-array-sort-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-sort-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-buffer-tq-csa.h"

namespace v8 {
namespace internal {
const char* kBuiltinNameSort_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  return "%TypedArray%.prototype.sort";}

TNode<Number> CallCompare_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSTypedArray> p_array, TNode<JSReceiver> p_comparefn, TNode<Object> p_a, TNode<Object> p_b) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Number> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Oddball> tmp0;
  TNode<Object> tmp1;
  TNode<Number> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<JSArrayBuffer> tmp4;
  TNode<BoolT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = Undefined_0(state_);
    tmp1 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{p_comparefn}, TNode<Object>{tmp0}, TNode<Object>{p_a}, TNode<Object>{p_b});
    tmp2 = CodeStubAssembler(state_).ToNumber_Inline(TNode<Context>{p_context}, TNode<Object>{tmp1});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp4 = CodeStubAssembler(state_).LoadReference<JSArrayBuffer>(CodeStubAssembler::Reference{p_array, tmp3});
    tmp5 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{tmp4});
    ca_.Branch(tmp5, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  if (block2.is_used()) {
    ca_.Bind(&block2);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kDetachedOperation, kBuiltinNameSort_0(state_));
  }

  TNode<BoolT> tmp6;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp6 = NumberIsNaN_0(state_, TNode<Number>{tmp2});
    ca_.Branch(tmp6, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  TNode<Number> tmp7;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp7 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    ca_.Goto(&block1, tmp7);
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block1, tmp2);
  }

  TNode<Number> phi_bb1_5;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_5);
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<Number>{phi_bb1_5};
}


void TypedArrayMerge_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSTypedArray> p_array, TNode<JSReceiver> p_comparefn, TNode<FixedArray> p_source, TNode<UintPtrT> p_from, TNode<UintPtrT> p_middle, TNode<UintPtrT> p_to, TNode<FixedArray> p_target) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, BoolT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block50(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block51(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block57(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block58(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block68(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block69(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block75(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT, UintPtrT> block76(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block78(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    ca_.Goto(&block4, p_from, p_middle, p_from);
  }

  TNode<UintPtrT> phi_bb4_8;
  TNode<UintPtrT> phi_bb4_9;
  TNode<UintPtrT> phi_bb4_10;
  TNode<BoolT> tmp0;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_8, &phi_bb4_9, &phi_bb4_10);
    tmp0 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb4_10}, TNode<UintPtrT>{p_to});
    ca_.Branch(tmp0, &block2, std::vector<compiler::Node*>{phi_bb4_8, phi_bb4_9, phi_bb4_10}, &block3, std::vector<compiler::Node*>{phi_bb4_8, phi_bb4_9, phi_bb4_10});
  }

  TNode<UintPtrT> phi_bb2_8;
  TNode<UintPtrT> phi_bb2_9;
  TNode<UintPtrT> phi_bb2_10;
  TNode<BoolT> tmp1;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_8, &phi_bb2_9, &phi_bb2_10);
    tmp1 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb2_8}, TNode<UintPtrT>{p_middle});
    ca_.Branch(tmp1, &block8, std::vector<compiler::Node*>{phi_bb2_8, phi_bb2_9, phi_bb2_10}, &block9, std::vector<compiler::Node*>{phi_bb2_8, phi_bb2_9, phi_bb2_10});
  }

  TNode<UintPtrT> phi_bb8_8;
  TNode<UintPtrT> phi_bb8_9;
  TNode<UintPtrT> phi_bb8_10;
  TNode<BoolT> tmp2;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_8, &phi_bb8_9, &phi_bb8_10);
    tmp2 = CodeStubAssembler(state_).UintPtrGreaterThanOrEqual(TNode<UintPtrT>{phi_bb8_9}, TNode<UintPtrT>{p_to});
    ca_.Goto(&block10, phi_bb8_8, phi_bb8_9, phi_bb8_10, tmp2);
  }

  TNode<UintPtrT> phi_bb9_8;
  TNode<UintPtrT> phi_bb9_9;
  TNode<UintPtrT> phi_bb9_10;
  TNode<BoolT> tmp3;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_8, &phi_bb9_9, &phi_bb9_10);
    tmp3 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block10, phi_bb9_8, phi_bb9_9, phi_bb9_10, tmp3);
  }

  TNode<UintPtrT> phi_bb10_8;
  TNode<UintPtrT> phi_bb10_9;
  TNode<UintPtrT> phi_bb10_10;
  TNode<BoolT> phi_bb10_12;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_8, &phi_bb10_9, &phi_bb10_10, &phi_bb10_12);
    ca_.Branch(phi_bb10_12, &block6, std::vector<compiler::Node*>{phi_bb10_8, phi_bb10_9, phi_bb10_10}, &block7, std::vector<compiler::Node*>{phi_bb10_8, phi_bb10_9, phi_bb10_10});
  }

  TNode<UintPtrT> phi_bb6_8;
  TNode<UintPtrT> phi_bb6_9;
  TNode<UintPtrT> phi_bb6_10;
  TNode<Object> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<UintPtrT> tmp8;
  TNode<UintPtrT> tmp9;
  TNode<BoolT> tmp10;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_8, &phi_bb6_9, &phi_bb6_10);
    std::tie(tmp4, tmp5, tmp6) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_target}).Flatten();
    tmp7 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb6_10});
    tmp8 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp7});
    tmp9 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp6});
    tmp10 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp8}, TNode<UintPtrT>{tmp9});
    ca_.Branch(tmp10, &block16, std::vector<compiler::Node*>{phi_bb6_8, phi_bb6_9, phi_bb6_10, phi_bb6_10, phi_bb6_10}, &block17, std::vector<compiler::Node*>{phi_bb6_8, phi_bb6_9, phi_bb6_10, phi_bb6_10, phi_bb6_10});
  }

  TNode<UintPtrT> phi_bb16_8;
  TNode<UintPtrT> phi_bb16_9;
  TNode<UintPtrT> phi_bb16_10;
  TNode<UintPtrT> phi_bb16_15;
  TNode<UintPtrT> phi_bb16_16;
  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<Object> tmp15;
  TNode<IntPtrT> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<UintPtrT> tmp18;
  TNode<UintPtrT> tmp19;
  TNode<IntPtrT> tmp20;
  TNode<UintPtrT> tmp21;
  TNode<UintPtrT> tmp22;
  TNode<BoolT> tmp23;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_8, &phi_bb16_9, &phi_bb16_10, &phi_bb16_15, &phi_bb16_16);
    tmp11 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp7});
    tmp12 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp5}, TNode<IntPtrT>{tmp11});
    std::tie(tmp13, tmp14) = NewReference_Object_0(state_, TNode<Object>{tmp4}, TNode<IntPtrT>{tmp12}).Flatten();
    std::tie(tmp15, tmp16, tmp17) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_source}).Flatten();
    tmp18 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp19 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb16_8}, TNode<UintPtrT>{tmp18});
    tmp20 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb16_8});
    tmp21 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp20});
    tmp22 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp17});
    tmp23 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp21}, TNode<UintPtrT>{tmp22});
    ca_.Branch(tmp23, &block23, std::vector<compiler::Node*>{phi_bb16_9, phi_bb16_10, phi_bb16_15, phi_bb16_16, phi_bb16_8, phi_bb16_8}, &block24, std::vector<compiler::Node*>{phi_bb16_9, phi_bb16_10, phi_bb16_15, phi_bb16_16, phi_bb16_8, phi_bb16_8});
  }

  TNode<UintPtrT> phi_bb17_8;
  TNode<UintPtrT> phi_bb17_9;
  TNode<UintPtrT> phi_bb17_10;
  TNode<UintPtrT> phi_bb17_15;
  TNode<UintPtrT> phi_bb17_16;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_8, &phi_bb17_9, &phi_bb17_10, &phi_bb17_15, &phi_bb17_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb23_9;
  TNode<UintPtrT> phi_bb23_10;
  TNode<UintPtrT> phi_bb23_15;
  TNode<UintPtrT> phi_bb23_16;
  TNode<UintPtrT> phi_bb23_23;
  TNode<UintPtrT> phi_bb23_24;
  TNode<IntPtrT> tmp24;
  TNode<IntPtrT> tmp25;
  TNode<Object> tmp26;
  TNode<IntPtrT> tmp27;
  TNode<Object> tmp28;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_9, &phi_bb23_10, &phi_bb23_15, &phi_bb23_16, &phi_bb23_23, &phi_bb23_24);
    tmp24 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp20});
    tmp25 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp16}, TNode<IntPtrT>{tmp24});
    std::tie(tmp26, tmp27) = NewReference_Object_0(state_, TNode<Object>{tmp15}, TNode<IntPtrT>{tmp25}).Flatten();
    tmp28 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp26, tmp27});
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp13, tmp14}, tmp28);
    ca_.Goto(&block11, tmp19, phi_bb23_9, phi_bb23_10);
  }

  TNode<UintPtrT> phi_bb24_9;
  TNode<UintPtrT> phi_bb24_10;
  TNode<UintPtrT> phi_bb24_15;
  TNode<UintPtrT> phi_bb24_16;
  TNode<UintPtrT> phi_bb24_23;
  TNode<UintPtrT> phi_bb24_24;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_9, &phi_bb24_10, &phi_bb24_15, &phi_bb24_16, &phi_bb24_23, &phi_bb24_24);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb7_8;
  TNode<UintPtrT> phi_bb7_9;
  TNode<UintPtrT> phi_bb7_10;
  TNode<BoolT> tmp29;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_8, &phi_bb7_9, &phi_bb7_10);
    tmp29 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb7_8}, TNode<UintPtrT>{p_middle});
    ca_.Branch(tmp29, &block26, std::vector<compiler::Node*>{phi_bb7_8, phi_bb7_9, phi_bb7_10}, &block27, std::vector<compiler::Node*>{phi_bb7_8, phi_bb7_9, phi_bb7_10});
  }

  TNode<UintPtrT> phi_bb26_8;
  TNode<UintPtrT> phi_bb26_9;
  TNode<UintPtrT> phi_bb26_10;
  TNode<Object> tmp30;
  TNode<IntPtrT> tmp31;
  TNode<IntPtrT> tmp32;
  TNode<IntPtrT> tmp33;
  TNode<UintPtrT> tmp34;
  TNode<UintPtrT> tmp35;
  TNode<BoolT> tmp36;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_8, &phi_bb26_9, &phi_bb26_10);
    std::tie(tmp30, tmp31, tmp32) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_source}).Flatten();
    tmp33 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb26_8});
    tmp34 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp33});
    tmp35 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp32});
    tmp36 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp34}, TNode<UintPtrT>{tmp35});
    ca_.Branch(tmp36, &block33, std::vector<compiler::Node*>{phi_bb26_8, phi_bb26_9, phi_bb26_10, phi_bb26_8, phi_bb26_8}, &block34, std::vector<compiler::Node*>{phi_bb26_8, phi_bb26_9, phi_bb26_10, phi_bb26_8, phi_bb26_8});
  }

  TNode<UintPtrT> phi_bb33_8;
  TNode<UintPtrT> phi_bb33_9;
  TNode<UintPtrT> phi_bb33_10;
  TNode<UintPtrT> phi_bb33_15;
  TNode<UintPtrT> phi_bb33_16;
  TNode<IntPtrT> tmp37;
  TNode<IntPtrT> tmp38;
  TNode<Object> tmp39;
  TNode<IntPtrT> tmp40;
  TNode<Object> tmp41;
  TNode<Object> tmp42;
  TNode<Object> tmp43;
  TNode<IntPtrT> tmp44;
  TNode<IntPtrT> tmp45;
  TNode<IntPtrT> tmp46;
  TNode<UintPtrT> tmp47;
  TNode<UintPtrT> tmp48;
  TNode<BoolT> tmp49;
  if (block33.is_used()) {
    ca_.Bind(&block33, &phi_bb33_8, &phi_bb33_9, &phi_bb33_10, &phi_bb33_15, &phi_bb33_16);
    tmp37 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp33});
    tmp38 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp31}, TNode<IntPtrT>{tmp37});
    std::tie(tmp39, tmp40) = NewReference_Object_0(state_, TNode<Object>{tmp30}, TNode<IntPtrT>{tmp38}).Flatten();
    tmp41 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp39, tmp40});
    tmp42 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp41});
    std::tie(tmp43, tmp44, tmp45) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_source}).Flatten();
    tmp46 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb33_9});
    tmp47 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp46});
    tmp48 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp45});
    tmp49 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp47}, TNode<UintPtrT>{tmp48});
    ca_.Branch(tmp49, &block40, std::vector<compiler::Node*>{phi_bb33_8, phi_bb33_9, phi_bb33_10, phi_bb33_9, phi_bb33_9}, &block41, std::vector<compiler::Node*>{phi_bb33_8, phi_bb33_9, phi_bb33_10, phi_bb33_9, phi_bb33_9});
  }

  TNode<UintPtrT> phi_bb34_8;
  TNode<UintPtrT> phi_bb34_9;
  TNode<UintPtrT> phi_bb34_10;
  TNode<UintPtrT> phi_bb34_15;
  TNode<UintPtrT> phi_bb34_16;
  if (block34.is_used()) {
    ca_.Bind(&block34, &phi_bb34_8, &phi_bb34_9, &phi_bb34_10, &phi_bb34_15, &phi_bb34_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb40_8;
  TNode<UintPtrT> phi_bb40_9;
  TNode<UintPtrT> phi_bb40_10;
  TNode<UintPtrT> phi_bb40_16;
  TNode<UintPtrT> phi_bb40_17;
  TNode<IntPtrT> tmp50;
  TNode<IntPtrT> tmp51;
  TNode<Object> tmp52;
  TNode<IntPtrT> tmp53;
  TNode<Object> tmp54;
  TNode<Object> tmp55;
  TNode<Number> tmp56;
  TNode<Number> tmp57;
  TNode<BoolT> tmp58;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_8, &phi_bb40_9, &phi_bb40_10, &phi_bb40_16, &phi_bb40_17);
    tmp50 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp46});
    tmp51 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp44}, TNode<IntPtrT>{tmp50});
    std::tie(tmp52, tmp53) = NewReference_Object_0(state_, TNode<Object>{tmp43}, TNode<IntPtrT>{tmp51}).Flatten();
    tmp54 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp52, tmp53});
    tmp55 = UnsafeCast_JSAny_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp54});
    tmp56 = CallCompare_0(state_, TNode<Context>{p_context}, TNode<JSTypedArray>{p_array}, TNode<JSReceiver>{p_comparefn}, TNode<Object>{tmp42}, TNode<Object>{tmp55});
    tmp57 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    tmp58 = NumberIsLessThanOrEqual_0(state_, TNode<Number>{tmp56}, TNode<Number>{tmp57});
    ca_.Branch(tmp58, &block43, std::vector<compiler::Node*>{phi_bb40_8, phi_bb40_9, phi_bb40_10}, &block44, std::vector<compiler::Node*>{phi_bb40_8, phi_bb40_9, phi_bb40_10});
  }

  TNode<UintPtrT> phi_bb41_8;
  TNode<UintPtrT> phi_bb41_9;
  TNode<UintPtrT> phi_bb41_10;
  TNode<UintPtrT> phi_bb41_16;
  TNode<UintPtrT> phi_bb41_17;
  if (block41.is_used()) {
    ca_.Bind(&block41, &phi_bb41_8, &phi_bb41_9, &phi_bb41_10, &phi_bb41_16, &phi_bb41_17);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb43_8;
  TNode<UintPtrT> phi_bb43_9;
  TNode<UintPtrT> phi_bb43_10;
  TNode<Object> tmp59;
  TNode<IntPtrT> tmp60;
  TNode<IntPtrT> tmp61;
  TNode<IntPtrT> tmp62;
  TNode<UintPtrT> tmp63;
  TNode<UintPtrT> tmp64;
  TNode<BoolT> tmp65;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_8, &phi_bb43_9, &phi_bb43_10);
    std::tie(tmp59, tmp60, tmp61) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_target}).Flatten();
    tmp62 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb43_10});
    tmp63 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp62});
    tmp64 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp61});
    tmp65 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp63}, TNode<UintPtrT>{tmp64});
    ca_.Branch(tmp65, &block50, std::vector<compiler::Node*>{phi_bb43_8, phi_bb43_9, phi_bb43_10, phi_bb43_10, phi_bb43_10}, &block51, std::vector<compiler::Node*>{phi_bb43_8, phi_bb43_9, phi_bb43_10, phi_bb43_10, phi_bb43_10});
  }

  TNode<UintPtrT> phi_bb50_8;
  TNode<UintPtrT> phi_bb50_9;
  TNode<UintPtrT> phi_bb50_10;
  TNode<UintPtrT> phi_bb50_17;
  TNode<UintPtrT> phi_bb50_18;
  TNode<IntPtrT> tmp66;
  TNode<IntPtrT> tmp67;
  TNode<Object> tmp68;
  TNode<IntPtrT> tmp69;
  TNode<UintPtrT> tmp70;
  TNode<UintPtrT> tmp71;
  if (block50.is_used()) {
    ca_.Bind(&block50, &phi_bb50_8, &phi_bb50_9, &phi_bb50_10, &phi_bb50_17, &phi_bb50_18);
    tmp66 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp62});
    tmp67 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp60}, TNode<IntPtrT>{tmp66});
    std::tie(tmp68, tmp69) = NewReference_Object_0(state_, TNode<Object>{tmp59}, TNode<IntPtrT>{tmp67}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp68, tmp69}, tmp42);
    tmp70 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp71 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb50_8}, TNode<UintPtrT>{tmp70});
    ca_.Goto(&block45, tmp71, phi_bb50_9, phi_bb50_10);
  }

  TNode<UintPtrT> phi_bb51_8;
  TNode<UintPtrT> phi_bb51_9;
  TNode<UintPtrT> phi_bb51_10;
  TNode<UintPtrT> phi_bb51_17;
  TNode<UintPtrT> phi_bb51_18;
  if (block51.is_used()) {
    ca_.Bind(&block51, &phi_bb51_8, &phi_bb51_9, &phi_bb51_10, &phi_bb51_17, &phi_bb51_18);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb44_8;
  TNode<UintPtrT> phi_bb44_9;
  TNode<UintPtrT> phi_bb44_10;
  TNode<Object> tmp72;
  TNode<IntPtrT> tmp73;
  TNode<IntPtrT> tmp74;
  TNode<IntPtrT> tmp75;
  TNode<UintPtrT> tmp76;
  TNode<UintPtrT> tmp77;
  TNode<BoolT> tmp78;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_8, &phi_bb44_9, &phi_bb44_10);
    std::tie(tmp72, tmp73, tmp74) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_target}).Flatten();
    tmp75 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb44_10});
    tmp76 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp75});
    tmp77 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp74});
    tmp78 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp76}, TNode<UintPtrT>{tmp77});
    ca_.Branch(tmp78, &block57, std::vector<compiler::Node*>{phi_bb44_8, phi_bb44_9, phi_bb44_10, phi_bb44_10, phi_bb44_10}, &block58, std::vector<compiler::Node*>{phi_bb44_8, phi_bb44_9, phi_bb44_10, phi_bb44_10, phi_bb44_10});
  }

  TNode<UintPtrT> phi_bb57_8;
  TNode<UintPtrT> phi_bb57_9;
  TNode<UintPtrT> phi_bb57_10;
  TNode<UintPtrT> phi_bb57_17;
  TNode<UintPtrT> phi_bb57_18;
  TNode<IntPtrT> tmp79;
  TNode<IntPtrT> tmp80;
  TNode<Object> tmp81;
  TNode<IntPtrT> tmp82;
  TNode<UintPtrT> tmp83;
  TNode<UintPtrT> tmp84;
  if (block57.is_used()) {
    ca_.Bind(&block57, &phi_bb57_8, &phi_bb57_9, &phi_bb57_10, &phi_bb57_17, &phi_bb57_18);
    tmp79 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp75});
    tmp80 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp73}, TNode<IntPtrT>{tmp79});
    std::tie(tmp81, tmp82) = NewReference_Object_0(state_, TNode<Object>{tmp72}, TNode<IntPtrT>{tmp80}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp81, tmp82}, tmp55);
    tmp83 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp84 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb57_9}, TNode<UintPtrT>{tmp83});
    ca_.Goto(&block45, phi_bb57_8, tmp84, phi_bb57_10);
  }

  TNode<UintPtrT> phi_bb58_8;
  TNode<UintPtrT> phi_bb58_9;
  TNode<UintPtrT> phi_bb58_10;
  TNode<UintPtrT> phi_bb58_17;
  TNode<UintPtrT> phi_bb58_18;
  if (block58.is_used()) {
    ca_.Bind(&block58, &phi_bb58_8, &phi_bb58_9, &phi_bb58_10, &phi_bb58_17, &phi_bb58_18);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb45_8;
  TNode<UintPtrT> phi_bb45_9;
  TNode<UintPtrT> phi_bb45_10;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_8, &phi_bb45_9, &phi_bb45_10);
    ca_.Goto(&block28, phi_bb45_8, phi_bb45_9, phi_bb45_10);
  }

  TNode<UintPtrT> phi_bb27_8;
  TNode<UintPtrT> phi_bb27_9;
  TNode<UintPtrT> phi_bb27_10;
  TNode<Object> tmp85;
  TNode<IntPtrT> tmp86;
  TNode<IntPtrT> tmp87;
  TNode<IntPtrT> tmp88;
  TNode<UintPtrT> tmp89;
  TNode<UintPtrT> tmp90;
  TNode<BoolT> tmp91;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_8, &phi_bb27_9, &phi_bb27_10);
    std::tie(tmp85, tmp86, tmp87) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_target}).Flatten();
    tmp88 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb27_10});
    tmp89 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp88});
    tmp90 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp87});
    tmp91 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp89}, TNode<UintPtrT>{tmp90});
    ca_.Branch(tmp91, &block68, std::vector<compiler::Node*>{phi_bb27_8, phi_bb27_9, phi_bb27_10, phi_bb27_10, phi_bb27_10}, &block69, std::vector<compiler::Node*>{phi_bb27_8, phi_bb27_9, phi_bb27_10, phi_bb27_10, phi_bb27_10});
  }

  TNode<UintPtrT> phi_bb68_8;
  TNode<UintPtrT> phi_bb68_9;
  TNode<UintPtrT> phi_bb68_10;
  TNode<UintPtrT> phi_bb68_15;
  TNode<UintPtrT> phi_bb68_16;
  TNode<IntPtrT> tmp92;
  TNode<IntPtrT> tmp93;
  TNode<Object> tmp94;
  TNode<IntPtrT> tmp95;
  TNode<Object> tmp96;
  TNode<IntPtrT> tmp97;
  TNode<IntPtrT> tmp98;
  TNode<UintPtrT> tmp99;
  TNode<UintPtrT> tmp100;
  TNode<IntPtrT> tmp101;
  TNode<UintPtrT> tmp102;
  TNode<UintPtrT> tmp103;
  TNode<BoolT> tmp104;
  if (block68.is_used()) {
    ca_.Bind(&block68, &phi_bb68_8, &phi_bb68_9, &phi_bb68_10, &phi_bb68_15, &phi_bb68_16);
    tmp92 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp88});
    tmp93 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp86}, TNode<IntPtrT>{tmp92});
    std::tie(tmp94, tmp95) = NewReference_Object_0(state_, TNode<Object>{tmp85}, TNode<IntPtrT>{tmp93}).Flatten();
    std::tie(tmp96, tmp97, tmp98) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_source}).Flatten();
    tmp99 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp100 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb68_9}, TNode<UintPtrT>{tmp99});
    tmp101 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb68_9});
    tmp102 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp101});
    tmp103 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp98});
    tmp104 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp102}, TNode<UintPtrT>{tmp103});
    ca_.Branch(tmp104, &block75, std::vector<compiler::Node*>{phi_bb68_8, phi_bb68_10, phi_bb68_15, phi_bb68_16, phi_bb68_9, phi_bb68_9}, &block76, std::vector<compiler::Node*>{phi_bb68_8, phi_bb68_10, phi_bb68_15, phi_bb68_16, phi_bb68_9, phi_bb68_9});
  }

  TNode<UintPtrT> phi_bb69_8;
  TNode<UintPtrT> phi_bb69_9;
  TNode<UintPtrT> phi_bb69_10;
  TNode<UintPtrT> phi_bb69_15;
  TNode<UintPtrT> phi_bb69_16;
  if (block69.is_used()) {
    ca_.Bind(&block69, &phi_bb69_8, &phi_bb69_9, &phi_bb69_10, &phi_bb69_15, &phi_bb69_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb75_8;
  TNode<UintPtrT> phi_bb75_10;
  TNode<UintPtrT> phi_bb75_15;
  TNode<UintPtrT> phi_bb75_16;
  TNode<UintPtrT> phi_bb75_23;
  TNode<UintPtrT> phi_bb75_24;
  TNode<IntPtrT> tmp105;
  TNode<IntPtrT> tmp106;
  TNode<Object> tmp107;
  TNode<IntPtrT> tmp108;
  TNode<Object> tmp109;
  if (block75.is_used()) {
    ca_.Bind(&block75, &phi_bb75_8, &phi_bb75_10, &phi_bb75_15, &phi_bb75_16, &phi_bb75_23, &phi_bb75_24);
    tmp105 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp101});
    tmp106 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp97}, TNode<IntPtrT>{tmp105});
    std::tie(tmp107, tmp108) = NewReference_Object_0(state_, TNode<Object>{tmp96}, TNode<IntPtrT>{tmp106}).Flatten();
    tmp109 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp107, tmp108});
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp94, tmp95}, tmp109);
    ca_.Goto(&block28, phi_bb75_8, tmp100, phi_bb75_10);
  }

  TNode<UintPtrT> phi_bb76_8;
  TNode<UintPtrT> phi_bb76_10;
  TNode<UintPtrT> phi_bb76_15;
  TNode<UintPtrT> phi_bb76_16;
  TNode<UintPtrT> phi_bb76_23;
  TNode<UintPtrT> phi_bb76_24;
  if (block76.is_used()) {
    ca_.Bind(&block76, &phi_bb76_8, &phi_bb76_10, &phi_bb76_15, &phi_bb76_16, &phi_bb76_23, &phi_bb76_24);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb28_8;
  TNode<UintPtrT> phi_bb28_9;
  TNode<UintPtrT> phi_bb28_10;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_8, &phi_bb28_9, &phi_bb28_10);
    ca_.Goto(&block11, phi_bb28_8, phi_bb28_9, phi_bb28_10);
  }

  TNode<UintPtrT> phi_bb11_8;
  TNode<UintPtrT> phi_bb11_9;
  TNode<UintPtrT> phi_bb11_10;
  TNode<UintPtrT> tmp110;
  TNode<UintPtrT> tmp111;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_8, &phi_bb11_9, &phi_bb11_10);
    tmp110 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp111 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb11_10}, TNode<UintPtrT>{tmp110});
    ca_.Goto(&block4, phi_bb11_8, phi_bb11_9, tmp111);
  }

  TNode<UintPtrT> phi_bb3_8;
  TNode<UintPtrT> phi_bb3_9;
  TNode<UintPtrT> phi_bb3_10;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_8, &phi_bb3_9, &phi_bb3_10);
    ca_.Goto(&block78);
  }

    ca_.Bind(&block78);
}


TF_BUILTIN(TypedArrayMergeSort, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<FixedArray> parameter1 = UncheckedParameter<FixedArray>(Descriptor::kSource);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kFrom);
  USE(parameter2);
  TNode<UintPtrT> parameter3 = UncheckedParameter<UintPtrT>(Descriptor::kTo);
  USE(parameter3);
  TNode<FixedArray> parameter4 = UncheckedParameter<FixedArray>(Descriptor::kTarget);
  USE(parameter4);
  TNode<JSTypedArray> parameter5 = UncheckedParameter<JSTypedArray>(Descriptor::kArray);
  USE(parameter5);
  TNode<JSReceiver> parameter6 = UncheckedParameter<JSReceiver>(Descriptor::kComparefn);
  USE(parameter6);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<UintPtrT> tmp0;
  TNode<UintPtrT> tmp1;
  TNode<UintPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<UintPtrT> tmp5;
  TNode<BoolT> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{parameter3}, TNode<UintPtrT>{parameter2});
    tmp1 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp2 = CodeStubAssembler(state_).WordShr(TNode<UintPtrT>{tmp0}, TNode<UintPtrT>{tmp1});
    tmp3 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{parameter2}, TNode<UintPtrT>{tmp2});
    tmp4 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{parameter2});
    tmp5 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp6 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp4}, TNode<UintPtrT>{tmp5});
    ca_.Branch(tmp6, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp7;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp7 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kTypedArrayMergeSort), parameter0, parameter4, parameter2, tmp3, parameter1, parameter5, parameter6);
    ca_.Goto(&block6);
  }

  TNode<UintPtrT> tmp8;
  TNode<UintPtrT> tmp9;
  TNode<BoolT> tmp10;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp8 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{parameter3}, TNode<UintPtrT>{tmp3});
    tmp9 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp10 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp8}, TNode<UintPtrT>{tmp9});
    ca_.Branch(tmp10, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp11;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp11 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kTypedArrayMergeSort), parameter0, parameter4, tmp3, parameter3, parameter1, parameter5, parameter6);
    ca_.Goto(&block8);
  }

  TNode<Oddball> tmp12;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    TypedArrayMerge_0(state_, TNode<Context>{parameter0}, TNode<JSTypedArray>{parameter5}, TNode<JSReceiver>{parameter6}, TNode<FixedArray>{parameter1}, TNode<UintPtrT>{parameter2}, TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{parameter3}, TNode<FixedArray>{parameter4});
    tmp12 = Undefined_0(state_);
    CodeStubAssembler(state_).Return(tmp12);
  }
}

TF_BUILTIN(TypedArrayPrototypeSort, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
   TNode<Word32T> argc = UncheckedParameter<Word32T>(Descriptor::kJSActualArgumentsCount);
  TNode<IntPtrT> arguments_length(ChangeInt32ToIntPtr(UncheckedCast<Int32T>(argc)));
  TNode<RawPtrT> arguments_frame = UncheckedCast<RawPtrT>(LoadFramePointer());
  TorqueStructArguments torque_arguments(GetFrameArguments(arguments_frame, arguments_length));
  CodeStubArguments arguments(this, torque_arguments);
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = arguments.GetReceiver();
USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT, UintPtrT> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<Oddball> tmp2;
  TNode<BoolT> tmp3;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = Undefined_0(state_);
    tmp3 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp1}, TNode<HeapObject>{tmp2});
    ca_.Branch(tmp3, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp4;
  TNode<BoolT> tmp5;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp4 = Is_Callable_JSAny_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp1});
    tmp5 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp4});
    ca_.Goto(&block5, tmp5);
  }

  TNode<BoolT> tmp6;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp6 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block5, tmp6);
  }

  TNode<BoolT> phi_bb5_7;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_7);
    ca_.Branch(phi_bb5_7, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kBadSortComparisonFunction, TNode<Object>{tmp1});
  }

  TNode<JSTypedArray> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<UintPtrT> tmp9;
  TNode<UintPtrT> tmp10;
  TNode<BoolT> tmp11;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp7 = TypedArrayBuiltinsAssembler(state_).ValidateTypedArray(TNode<Context>{parameter0}, TNode<Object>{parameter1}, kBuiltinNameSort_0(state_));
    tmp8 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp9 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp7, tmp8});
    tmp10 = FromConstexpr_uintptr_constexpr_int31_0(state_, 2);
    tmp11 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp9}, TNode<UintPtrT>{tmp10});
    ca_.Branch(tmp11, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    arguments.PopAndReturn(tmp7);
  }

  TNode<Oddball> tmp12;
  TNode<BoolT> tmp13;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp12 = Undefined_0(state_);
    tmp13 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp1}, TNode<HeapObject>{tmp12});
    ca_.Branch(tmp13, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  TNode<JSTypedArray> tmp14;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp14 = TORQUE_CAST(CodeStubAssembler(state_).CallRuntime(Runtime::kTypedArraySortFast, parameter0, parameter1)); 
    arguments.PopAndReturn(tmp14);
  }

  TNode<UintPtrT> tmp15;
  TNode<BoolT> tmp16;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp15 = FromConstexpr_uintptr_constexpr_int31_0(state_, FixedArray::kMaxLength);
    tmp16 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp9}, TNode<UintPtrT>{tmp15});
    ca_.Branch(tmp16, &block10, std::vector<compiler::Node*>{}, &block11, std::vector<compiler::Node*>{});
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kTypedArrayTooLargeToSort);
  }

  TNode<JSReceiver> tmp17;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    compiler::CodeAssemblerLabel label18(&ca_);
    tmp17 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp1}, &label18);
    ca_.Goto(&block14);
    if (label18.is_used()) {
      ca_.Bind(&label18);
      ca_.Goto(&block15);
    }
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Int32T> tmp19;
  TNode<BuiltinPtr> tmp20;
  TNode<BuiltinPtr> tmp21;
  TNode<BuiltinPtr> tmp22;
  TNode<IntPtrT> tmp23;
  TNode<FixedArray> tmp24;
  TNode<IntPtrT> tmp25;
  TNode<FixedArray> tmp26;
  TNode<UintPtrT> tmp27;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp19 = CodeStubAssembler(state_).LoadElementsKind(TNode<JSTypedArray>{tmp7});
    std::tie(tmp20, tmp21, tmp22) = GetTypedArrayAccessor_0(state_, TNode<Int32T>{tmp19}).Flatten();
    tmp23 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{tmp9});
    tmp24 = CodeStubAssembler(state_).AllocateZeroedFixedArray(TNode<IntPtrT>{tmp23});
    tmp25 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{tmp9});
    tmp26 = CodeStubAssembler(state_).AllocateZeroedFixedArray(TNode<IntPtrT>{tmp25});
    tmp27 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block18, tmp27);
  }

  TNode<UintPtrT> phi_bb18_15;
  TNode<BoolT> tmp28;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_15);
    tmp28 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb18_15}, TNode<UintPtrT>{tmp9});
    ca_.Branch(tmp28, &block16, std::vector<compiler::Node*>{phi_bb18_15}, &block17, std::vector<compiler::Node*>{phi_bb18_15});
  }

  TNode<UintPtrT> phi_bb16_15;
  TNode<Numeric> tmp29;
  TNode<Object> tmp30;
  TNode<IntPtrT> tmp31;
  TNode<IntPtrT> tmp32;
  TNode<IntPtrT> tmp33;
  TNode<UintPtrT> tmp34;
  TNode<UintPtrT> tmp35;
  TNode<BoolT> tmp36;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_15);
tmp29 = TORQUE_CAST(CodeStubAssembler(state_).CallBuiltinPointer(Builtins::CallableFor(ca_.isolate(),ExampleBuiltinForTorqueFunctionPointerType(1)).descriptor(), tmp20, TNode<Object>(), tmp7, phi_bb16_15));
    std::tie(tmp30, tmp31, tmp32) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp24}).Flatten();
    tmp33 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb16_15});
    tmp34 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp33});
    tmp35 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp32});
    tmp36 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp34}, TNode<UintPtrT>{tmp35});
    ca_.Branch(tmp36, &block25, std::vector<compiler::Node*>{phi_bb16_15, phi_bb16_15, phi_bb16_15}, &block26, std::vector<compiler::Node*>{phi_bb16_15, phi_bb16_15, phi_bb16_15});
  }

  TNode<UintPtrT> phi_bb25_15;
  TNode<UintPtrT> phi_bb25_21;
  TNode<UintPtrT> phi_bb25_22;
  TNode<IntPtrT> tmp37;
  TNode<IntPtrT> tmp38;
  TNode<Object> tmp39;
  TNode<IntPtrT> tmp40;
  TNode<Object> tmp41;
  TNode<IntPtrT> tmp42;
  TNode<IntPtrT> tmp43;
  TNode<IntPtrT> tmp44;
  TNode<UintPtrT> tmp45;
  TNode<UintPtrT> tmp46;
  TNode<BoolT> tmp47;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_15, &phi_bb25_21, &phi_bb25_22);
    tmp37 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp33});
    tmp38 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp31}, TNode<IntPtrT>{tmp37});
    std::tie(tmp39, tmp40) = NewReference_Object_0(state_, TNode<Object>{tmp30}, TNode<IntPtrT>{tmp38}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp39, tmp40}, tmp29);
    std::tie(tmp41, tmp42, tmp43) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp26}).Flatten();
    tmp44 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb25_15});
    tmp45 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp44});
    tmp46 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp43});
    tmp47 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp45}, TNode<UintPtrT>{tmp46});
    ca_.Branch(tmp47, &block32, std::vector<compiler::Node*>{phi_bb25_15, phi_bb25_15, phi_bb25_15}, &block33, std::vector<compiler::Node*>{phi_bb25_15, phi_bb25_15, phi_bb25_15});
  }

  TNode<UintPtrT> phi_bb26_15;
  TNode<UintPtrT> phi_bb26_21;
  TNode<UintPtrT> phi_bb26_22;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_15, &phi_bb26_21, &phi_bb26_22);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb32_15;
  TNode<UintPtrT> phi_bb32_21;
  TNode<UintPtrT> phi_bb32_22;
  TNode<IntPtrT> tmp48;
  TNode<IntPtrT> tmp49;
  TNode<Object> tmp50;
  TNode<IntPtrT> tmp51;
  TNode<UintPtrT> tmp52;
  TNode<UintPtrT> tmp53;
  if (block32.is_used()) {
    ca_.Bind(&block32, &phi_bb32_15, &phi_bb32_21, &phi_bb32_22);
    tmp48 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp44});
    tmp49 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp42}, TNode<IntPtrT>{tmp48});
    std::tie(tmp50, tmp51) = NewReference_Object_0(state_, TNode<Object>{tmp41}, TNode<IntPtrT>{tmp49}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp50, tmp51}, tmp29);
    tmp52 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp53 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb32_15}, TNode<UintPtrT>{tmp52});
    ca_.Goto(&block18, tmp53);
  }

  TNode<UintPtrT> phi_bb33_15;
  TNode<UintPtrT> phi_bb33_21;
  TNode<UintPtrT> phi_bb33_22;
  if (block33.is_used()) {
    ca_.Bind(&block33, &phi_bb33_15, &phi_bb33_21, &phi_bb33_22);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb17_15;
  TNode<UintPtrT> tmp54;
  TNode<Object> tmp55;
  TNode<UintPtrT> tmp56;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_15);
    tmp54 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp55 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kTypedArrayMergeSort), parameter0, tmp26, tmp54, tmp9, tmp24, tmp7, tmp17);
    tmp56 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block37, tmp56);
  }

  TNode<UintPtrT> phi_bb37_15;
  TNode<BoolT> tmp57;
  if (block37.is_used()) {
    ca_.Bind(&block37, &phi_bb37_15);
    tmp57 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb37_15}, TNode<UintPtrT>{tmp9});
    ca_.Branch(tmp57, &block35, std::vector<compiler::Node*>{phi_bb37_15}, &block36, std::vector<compiler::Node*>{phi_bb37_15});
  }

  TNode<UintPtrT> phi_bb35_15;
  TNode<Object> tmp58;
  TNode<IntPtrT> tmp59;
  TNode<IntPtrT> tmp60;
  TNode<IntPtrT> tmp61;
  TNode<UintPtrT> tmp62;
  TNode<UintPtrT> tmp63;
  TNode<BoolT> tmp64;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_15);
    std::tie(tmp58, tmp59, tmp60) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp24}).Flatten();
    tmp61 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{phi_bb35_15});
    tmp62 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp61});
    tmp63 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp60});
    tmp64 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp62}, TNode<UintPtrT>{tmp63});
    ca_.Branch(tmp64, &block43, std::vector<compiler::Node*>{phi_bb35_15, phi_bb35_15, phi_bb35_15, phi_bb35_15}, &block44, std::vector<compiler::Node*>{phi_bb35_15, phi_bb35_15, phi_bb35_15, phi_bb35_15});
  }

  TNode<UintPtrT> phi_bb43_15;
  TNode<UintPtrT> phi_bb43_21;
  TNode<UintPtrT> phi_bb43_26;
  TNode<UintPtrT> phi_bb43_27;
  TNode<IntPtrT> tmp65;
  TNode<IntPtrT> tmp66;
  TNode<Object> tmp67;
  TNode<IntPtrT> tmp68;
  TNode<Object> tmp69;
  TNode<Numeric> tmp70;
  TNode<Smi> tmp71;
  TNode<UintPtrT> tmp72;
  TNode<UintPtrT> tmp73;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_15, &phi_bb43_21, &phi_bb43_26, &phi_bb43_27);
    tmp65 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp61});
    tmp66 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp59}, TNode<IntPtrT>{tmp65});
    std::tie(tmp67, tmp68) = NewReference_Object_0(state_, TNode<Object>{tmp58}, TNode<IntPtrT>{tmp66}).Flatten();
    tmp69 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp67, tmp68});
    tmp70 = UnsafeCast_Numeric_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp69});
tmp71 = TORQUE_CAST(CodeStubAssembler(state_).CallBuiltinPointer(Builtins::CallableFor(ca_.isolate(),ExampleBuiltinForTorqueFunctionPointerType(2)).descriptor(), tmp21, parameter0, tmp7, phi_bb43_21, tmp70));
    tmp72 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp73 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb43_15}, TNode<UintPtrT>{tmp72});
    ca_.Goto(&block37, tmp73);
  }

  TNode<UintPtrT> phi_bb44_15;
  TNode<UintPtrT> phi_bb44_21;
  TNode<UintPtrT> phi_bb44_26;
  TNode<UintPtrT> phi_bb44_27;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_15, &phi_bb44_21, &phi_bb44_26, &phi_bb44_27);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb36_15;
  if (block36.is_used()) {
    ca_.Bind(&block36, &phi_bb36_15);
    arguments.PopAndReturn(tmp7);
  }
}

TNode<Numeric> UnsafeCast_Numeric_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Numeric> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<Numeric>{tmp0};
}


} // namespace internal
} // namespace v8
