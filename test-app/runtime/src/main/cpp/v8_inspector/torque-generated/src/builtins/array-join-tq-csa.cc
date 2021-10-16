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
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/builtins-string-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"
#include "torque-generated/src/objects/contexts-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-buffer-tq-csa.h"
#include "torque-generated/src/objects/js-array-tq-csa.h"
#include "torque-generated/src/objects/string-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(LoadJoinElement_DictionaryElements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSArray> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<FixedArrayBase> tmp2;
  TNode<NumberDictionary> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<Object> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp2 = CodeStubAssembler(state_).LoadReference<FixedArrayBase>(CodeStubAssembler::Reference{tmp0, tmp1});
    tmp3 = UnsafeCast_NumberDictionary_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp2});
    tmp4 = CodeStubAssembler(state_).Signed(TNode<UintPtrT>{parameter2});
    compiler::CodeAssemblerLabel label6(&ca_);
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp5 = CodeStubAssembler(state_).BasicLoadNumberDictionaryElement(TNode<NumberDictionary>{tmp3}, TNode<IntPtrT>{tmp4}, &label6, &label7);
    ca_.Goto(&block5);
    if (label6.is_used()) {
      ca_.Bind(&label6);
      ca_.Goto(&block6);
    }
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block7);
    }
  }

  TNode<Number> tmp8;
  TNode<Object> tmp9;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp8 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{parameter2});
    tmp9 = CodeStubAssembler(state_).GetProperty(TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{tmp8});
    CodeStubAssembler(state_).Return(tmp9);
  }

  TNode<String> tmp10;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp10 = kEmptyString_0(state_);
    CodeStubAssembler(state_).Return(tmp10);
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    CodeStubAssembler(state_).Return(tmp5);
  }
}

TF_BUILTIN(LoadJoinElement_FastSmiOrObjectElements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSArray> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<FixedArrayBase> tmp2;
  TNode<FixedArray> tmp3;
  TNode<Object> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<UintPtrT> tmp8;
  TNode<UintPtrT> tmp9;
  TNode<BoolT> tmp10;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp2 = CodeStubAssembler(state_).LoadReference<FixedArrayBase>(CodeStubAssembler::Reference{tmp0, tmp1});
    tmp3 = UnsafeCast_FixedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp2});
    std::tie(tmp4, tmp5, tmp6) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp3}).Flatten();
    tmp7 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{parameter2});
    tmp8 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp7});
    tmp9 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp6});
    tmp10 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp8}, TNode<UintPtrT>{tmp9});
    ca_.Branch(tmp10, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<Object> tmp15;
  TNode<Oddball> tmp16;
  TNode<BoolT> tmp17;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp11 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp7});
    tmp12 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp5}, TNode<IntPtrT>{tmp11});
    std::tie(tmp13, tmp14) = NewReference_Object_0(state_, TNode<Object>{tmp4}, TNode<IntPtrT>{tmp12}).Flatten();
    tmp15 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp13, tmp14});
    tmp16 = TheHole_0(state_);
    tmp17 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp15}, TNode<HeapObject>{tmp16});
    ca_.Branch(tmp17, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> tmp18;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp18 = kEmptyString_0(state_);
    ca_.Goto(&block10, tmp18);
  }

  TNode<Object> tmp19;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp19 = UnsafeCast_JSAny_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp15});
    ca_.Goto(&block10, tmp19);
  }

  TNode<Object> phi_bb10_6;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_6);
    CodeStubAssembler(state_).Return(phi_bb10_6);
  }
}

TF_BUILTIN(LoadJoinElement_FastDoubleElements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSArray> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<FixedArrayBase> tmp2;
  TNode<FixedDoubleArray> tmp3;
  TNode<Object> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<UintPtrT> tmp8;
  TNode<UintPtrT> tmp9;
  TNode<BoolT> tmp10;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp2 = CodeStubAssembler(state_).LoadReference<FixedArrayBase>(CodeStubAssembler::Reference{tmp0, tmp1});
    tmp3 = UnsafeCast_FixedDoubleArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp2});
    std::tie(tmp4, tmp5, tmp6) = FieldSliceFixedDoubleArrayFloats_0(state_, TNode<FixedDoubleArray>{tmp3}).Flatten();
    tmp7 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{parameter2});
    tmp8 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp7});
    tmp9 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp6});
    tmp10 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp8}, TNode<UintPtrT>{tmp9});
    ca_.Branch(tmp10, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<BoolT> tmp15;
  TNode<Float64T> tmp16;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp11 = TimesSizeOf_float64_or_hole_0(state_, TNode<IntPtrT>{tmp7});
    tmp12 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp5}, TNode<IntPtrT>{tmp11});
    std::tie(tmp13, tmp14) = NewReference_float64_or_hole_0(state_, TNode<Object>{tmp4}, TNode<IntPtrT>{tmp12}).Flatten();
    std::tie(tmp15, tmp16) = LoadFloat64OrHole_0(state_, TorqueStructReference_float64_or_hole_0{TNode<Object>{tmp13}, TNode<IntPtrT>{tmp14}, TorqueStructUnsafe_0{}}).Flatten();
    ca_.Branch(tmp15, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> tmp17;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp17 = kEmptyString_0(state_);
    CodeStubAssembler(state_).Return(tmp17);
  }

  TNode<HeapNumber> tmp18;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp18 = CodeStubAssembler(state_).AllocateHeapNumberWithValue(TNode<Float64T>{tmp16});
    CodeStubAssembler(state_).Return(tmp18);
  }
}

TF_BUILTIN(ConvertToLocaleString, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kElement);
  USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kLocales);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kOptions);
  USE(parameter3);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).IsNullOrUndefined(TNode<Object>{parameter1});
    ca_.Branch(tmp0, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp1;
  if (block1.is_used()) {
    ca_.Bind(&block1);
    tmp1 = kEmptyString_0(state_);
    CodeStubAssembler(state_).Return(tmp1);
  }

  TNode<Object> tmp2;
  TNode<Object> tmp3;
  TNode<JSReceiver> tmp4;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp2 = FromConstexpr_JSAny_constexpr_string_0(state_, "toLocaleString");
    tmp3 = CodeStubAssembler(state_).GetProperty(TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{tmp2});
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp3}, &label5);
    ca_.Goto(&block5);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCalledNonCallable, TNode<Object>{tmp3});
  }

  TNode<BoolT> tmp6;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp6 = CodeStubAssembler(state_).IsNullOrUndefined(TNode<Object>{parameter2});
    ca_.Branch(tmp6, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp7;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp7 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp4}, TNode<Object>{parameter1});
    ca_.Goto(&block9, tmp7);
  }

  TNode<BoolT> tmp8;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp8 = CodeStubAssembler(state_).IsNullOrUndefined(TNode<Object>{parameter3});
    ca_.Branch(tmp8, &block10, std::vector<compiler::Node*>{}, &block11, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp9;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp9 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp4}, TNode<Object>{parameter1}, TNode<Object>{parameter2});
    ca_.Goto(&block12, tmp9);
  }

  TNode<Object> tmp10;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp10 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp4}, TNode<Object>{parameter1}, TNode<Object>{parameter2}, TNode<Object>{parameter3});
    ca_.Goto(&block12, tmp10);
  }

  TNode<Object> phi_bb12_6;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_6);
    ca_.Goto(&block9, phi_bb12_6);
  }

  TNode<Object> phi_bb9_6;
  TNode<String> tmp11;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_6);
    tmp11 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{phi_bb9_6});
    CodeStubAssembler(state_).Return(tmp11);
  }
}

TNode<BoolT> CannotUseSameArrayAccessor_JSArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<BuiltinPtr> p_loadFn, TNode<JSReceiver> p_receiver, TNode<Map> p_originalMap, TNode<Number> p_originalLen) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{p_loadFn}, TNode<Smi>{ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinElement_GenericElementsAccessor_0))});
    ca_.Branch(tmp0, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp1;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp1 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp1);
  }

  TNode<JSArray> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<Map> tmp4;
  TNode<BoolT> tmp5;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp2 = UnsafeCast_JSArray_0(state_, TNode<Context>{p_context}, TNode<Object>{p_receiver});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp4 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp2, tmp3});
    tmp5 = CodeStubAssembler(state_).TaggedNotEqual(TNode<HeapObject>{p_originalMap}, TNode<HeapObject>{tmp4});
    ca_.Branch(tmp5, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp6;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp6 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp6);
  }

  TNode<IntPtrT> tmp7;
  TNode<Number> tmp8;
  TNode<BoolT> tmp9;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp8 = CodeStubAssembler(state_).LoadReference<Number>(CodeStubAssembler::Reference{tmp2, tmp7});
    tmp9 = IsNumberNotEqual_0(state_, TNode<Number>{p_originalLen}, TNode<Number>{tmp8});
    ca_.Branch(tmp9, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp10;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp10 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp10);
  }

  TNode<BoolT> tmp11;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp11 = CodeStubAssembler(state_).IsNoElementsProtectorCellInvalid();
    ca_.Branch(tmp11, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp12;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp12 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp12);
  }

  TNode<BoolT> tmp13;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp13 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp13);
  }

  TNode<BoolT> phi_bb1_5;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_5);
    ca_.Goto(&block10, phi_bb1_5);
  }

  TNode<BoolT> phi_bb10_5;
    ca_.Bind(&block10, &phi_bb10_5);
  return TNode<BoolT>{phi_bb10_5};
}


TNode<BoolT> CannotUseSameArrayAccessor_JSTypedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<BuiltinPtr> p__loadFn, TNode<JSReceiver> p_receiver, TNode<Map> p__initialMap, TNode<Number> p__initialLen) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<JSArrayBuffer> tmp2;
  TNode<BoolT> tmp3;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{p_context}, TNode<Object>{p_receiver});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp2 = CodeStubAssembler(state_).LoadReference<JSArrayBuffer>(CodeStubAssembler::Reference{tmp0, tmp1});
    tmp3 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{tmp2});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<BoolT>{tmp3};
}


TNode<IntPtrT> AddStringLength_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_lenA, TNode<IntPtrT> p_lenB) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = CodeStubAssembler(state_).TryIntPtrAdd(TNode<IntPtrT>{p_lenA}, TNode<IntPtrT>{p_lenB}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block3);
  }

  TNode<IntPtrT> tmp2;
  TNode<BoolT> tmp3;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, String::kMaxLength);
    tmp3 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{tmp0}, TNode<IntPtrT>{tmp2});
    ca_.Branch(tmp3, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block3);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block8);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowInvalidStringLength, p_context);
    CodeStubAssembler(state_).Unreachable();
  }

    ca_.Bind(&block8);
  return TNode<IntPtrT>{tmp0};
}


TorqueStructBuffer_0 NewBuffer_0(compiler::CodeAssemblerState* state_, TNode<UintPtrT> p_len, TNode<String> p_sep) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<UintPtrT> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_uintptr_constexpr_int31_0(state_, FixedArray::kMaxRegularLength);
    tmp1 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{p_len}, TNode<UintPtrT>{tmp0});
    ca_.Branch(tmp1, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp2;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, FixedArray::kMaxRegularLength);
    ca_.Goto(&block4, tmp2);
  }

  TNode<IntPtrT> tmp3;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = CodeStubAssembler(state_).Signed(TNode<UintPtrT>{p_len});
    ca_.Goto(&block4, tmp3);
  }

  TNode<IntPtrT> phi_bb4_2;
  TNode<FixedArray> tmp4;
  TNode<Uint16T> tmp5;
  TNode<BoolT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_2);
    tmp4 = CodeStubAssembler(state_).AllocateZeroedFixedArray(TNode<IntPtrT>{phi_bb4_2});
    tmp5 = CodeStubAssembler(state_).LoadInstanceType(TNode<HeapObject>{p_sep});
    tmp6 = CodeStubAssembler(state_).IsOneByteStringInstanceType(TNode<Uint16T>{tmp5});
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp8 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block10);
  }

    ca_.Bind(&block10);
  return TorqueStructBuffer_0{TNode<FixedArray>{tmp4}, TNode<IntPtrT>{tmp7}, TNode<IntPtrT>{tmp8}, TNode<BoolT>{tmp6}};
}


TNode<String> BufferJoin_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TorqueStructBuffer_0 p_buffer, TNode<String> p_sep) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{p_buffer.totalStringLength}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp1, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp2;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp2 = kEmptyString_0(state_);
    ca_.Goto(&block1, tmp2);
  }

  TNode<IntPtrT> tmp3;
  TNode<BoolT> tmp4;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp4 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{p_buffer.index}, TNode<IntPtrT>{tmp3});
    ca_.Branch(tmp4, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<UintPtrT> tmp9;
  TNode<UintPtrT> tmp10;
  TNode<BoolT> tmp11;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    std::tie(tmp5, tmp6, tmp7) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_buffer.fixedArray}).Flatten();
    tmp8 = Convert_intptr_constexpr_int31_0(state_, 0);
    tmp9 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp8});
    tmp10 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp7});
    tmp11 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp9}, TNode<UintPtrT>{tmp10});
    ca_.Branch(tmp11, &block14, std::vector<compiler::Node*>{}, &block15, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Object> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<Object> tmp16;
  TNode<String> tmp17;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp12 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp8});
    tmp13 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp6}, TNode<IntPtrT>{tmp12});
    std::tie(tmp14, tmp15) = NewReference_Object_0(state_, TNode<Object>{tmp5}, TNode<IntPtrT>{tmp13}).Flatten();
    tmp16 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp14, tmp15});
    compiler::CodeAssemblerLabel label18(&ca_);
    tmp17 = Cast_String_1(state_, TNode<Context>{p_context}, TNode<Object>{tmp16}, &label18);
    ca_.Goto(&block19);
    if (label18.is_used()) {
      ca_.Bind(&label18);
      ca_.Goto(&block20);
    }
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Number> tmp19;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    compiler::CodeAssemblerLabel label20(&ca_);
    tmp19 = Cast_Number_0(state_, TNode<Object>{ca_.UncheckedCast<Object>(tmp16)}, &label20);
    ca_.Goto(&block23);
    if (label20.is_used()) {
      ca_.Bind(&label20);
      ca_.Goto(&block24);
    }
  }

  if (block19.is_used()) {
    ca_.Bind(&block19);
    ca_.Goto(&block1, tmp17);
  }

  if (block24.is_used()) {
    ca_.Bind(&block24);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> tmp21;
  if (block23.is_used()) {
    ca_.Bind(&block23);
    tmp21 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kStringRepeat), p_context, p_sep, tmp19);
    ca_.Goto(&block1, tmp21);
  }

  TNode<UintPtrT> tmp22;
  TNode<Uint32T> tmp23;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp22 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{p_buffer.totalStringLength});
    tmp23 = Convert_uint32_uintptr_0(state_, TNode<UintPtrT>{tmp22});
    ca_.Branch(p_buffer.isOneByte, &block25, std::vector<compiler::Node*>{}, &block26, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp24;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp24 = AllocateSeqOneByteString_0(state_, TNode<Uint32T>{tmp23});
    ca_.Goto(&block27, tmp24);
  }

  TNode<String> tmp25;
  if (block26.is_used()) {
    ca_.Bind(&block26);
    tmp25 = AllocateSeqTwoByteString_0(state_, TNode<Uint32T>{tmp23});
    ca_.Goto(&block27, tmp25);
  }

  TNode<String> phi_bb27_7;
  TNode<String> tmp26;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_7);
    tmp26 = ArrayBuiltinsAssembler(state_).CallJSArrayArrayJoinConcatToSequentialString(TNode<FixedArray>{p_buffer.fixedArray}, TNode<IntPtrT>{p_buffer.index}, TNode<String>{p_sep}, TNode<String>{phi_bb27_7});
    ca_.Goto(&block1, tmp26);
  }

  TNode<String> phi_bb1_6;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_6);
    ca_.Goto(&block29, phi_bb1_6);
  }

  TNode<String> phi_bb29_6;
    ca_.Bind(&block29, &phi_bb29_6);
  return TNode<String>{phi_bb29_6};
}


TNode<Object> ArrayJoin_JSArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, bool p_useToLocaleString, TNode<JSReceiver> p_receiver, TNode<String> p_sep, TNode<Number> p_lenNumber, TNode<Object> p_locales, TNode<Object> p_options) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Map> tmp1;
  TNode<Int32T> tmp2;
  TNode<JSArray> tmp3;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{p_receiver, tmp0});
    tmp2 = CodeStubAssembler(state_).LoadMapElementsKind(TNode<Map>{tmp1});
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_JSArray_0(state_, TNode<HeapObject>{p_receiver}, &label4);
    ca_.Goto(&block4);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block3);
  }

  TNode<IntPtrT> tmp5;
  TNode<Number> tmp6;
  TNode<BoolT> tmp7;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp6 = CodeStubAssembler(state_).LoadReference<Number>(CodeStubAssembler::Reference{tmp3, tmp5});
    tmp7 = IsNumberNotEqual_0(state_, TNode<Number>{tmp6}, TNode<Number>{p_lenNumber});
    ca_.Branch(tmp7, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block3);
  }

  TNode<BoolT> tmp8;
  TNode<BoolT> tmp9;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp8 = CodeStubAssembler(state_).IsPrototypeInitialArrayPrototype(TNode<Context>{p_context}, TNode<Map>{tmp1});
    tmp9 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp8});
    ca_.Branch(tmp9, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(&block3);
  }

  TNode<BoolT> tmp10;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp10 = CodeStubAssembler(state_).IsNoElementsProtectorCellInvalid();
    ca_.Branch(tmp10, &block10, std::vector<compiler::Node*>{}, &block11, std::vector<compiler::Node*>{});
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    ca_.Goto(&block3);
  }

  TNode<BoolT> tmp11;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp11 = CodeStubAssembler(state_).IsElementsKindLessThanOrEqual(TNode<Int32T>{tmp2}, ElementsKind::HOLEY_ELEMENTS);
    ca_.Branch(tmp11, &block12, std::vector<compiler::Node*>{}, &block13, std::vector<compiler::Node*>{});
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    ca_.Goto(&block14, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinElement_FastSmiOrObjectElements_0)));
  }

  TNode<BoolT> tmp12;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp12 = CodeStubAssembler(state_).IsElementsKindLessThanOrEqual(TNode<Int32T>{tmp2}, ElementsKind::HOLEY_DOUBLE_ELEMENTS);
    ca_.Branch(tmp12, &block15, std::vector<compiler::Node*>{}, &block16, std::vector<compiler::Node*>{});
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block17, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinElement_FastDoubleElements_0)));
  }

  TNode<Int32T> tmp13;
  TNode<BoolT> tmp14;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp13 = FromConstexpr_ElementsKind_constexpr_DICTIONARY_ELEMENTS_0(state_, ElementsKind::DICTIONARY_ELEMENTS);
    tmp14 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp13});
    ca_.Branch(tmp14, &block18, std::vector<compiler::Node*>{}, &block19, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp15;
  TNode<FixedArrayBase> tmp16;
  TNode<NumberDictionary> tmp17;
  TNode<Smi> tmp18;
  TNode<Smi> tmp19;
  TNode<BoolT> tmp20;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    tmp15 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp16 = CodeStubAssembler(state_).LoadReference<FixedArrayBase>(CodeStubAssembler::Reference{tmp3, tmp15});
    tmp17 = UnsafeCast_NumberDictionary_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp16});
    tmp18 = CodeStubAssembler(state_).GetNumberDictionaryNumberOfElements(TNode<NumberDictionary>{tmp17});
    tmp19 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp20 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp18}, TNode<Smi>{tmp19});
    ca_.Branch(tmp20, &block21, std::vector<compiler::Node*>{}, &block22, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp21;
  TNode<BoolT> tmp22;
  if (block21.is_used()) {
    ca_.Bind(&block21);
    tmp21 = kEmptyString_0(state_);
    tmp22 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{p_sep}, TNode<HeapObject>{tmp21});
    ca_.Branch(tmp22, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp23;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp23 = kEmptyString_0(state_);
    ca_.Goto(&block1, tmp23);
  }

  TNode<Number> tmp24;
  TNode<Number> tmp25;
  TNode<Smi> tmp26;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp24 = FromConstexpr_Number_constexpr_int31_0(state_, 1);
    tmp25 = CodeStubAssembler(state_).NumberSub(TNode<Number>{p_lenNumber}, TNode<Number>{tmp24});
    compiler::CodeAssemblerLabel label27(&ca_);
    tmp26 = Cast_Smi_0(state_, TNode<Object>{tmp25}, &label27);
    ca_.Goto(&block28);
    if (label27.is_used()) {
      ca_.Bind(&label27);
      ca_.Goto(&block29);
    }
  }

  if (block29.is_used()) {
    ca_.Bind(&block29);
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowInvalidStringLength, p_context);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> tmp28;
  if (block28.is_used()) {
    ca_.Bind(&block28);
    tmp28 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kStringRepeat), p_context, p_sep, tmp26);
    ca_.Goto(&block1, tmp28);
  }

  if (block22.is_used()) {
    ca_.Bind(&block22);
    ca_.Goto(&block17, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinElement_DictionaryElements_0)));
  }

  if (block19.is_used()) {
    ca_.Bind(&block19);
    ca_.Goto(&block3);
  }

  TNode<BuiltinPtr> phi_bb17_8;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_8);
    ca_.Goto(&block14, phi_bb17_8);
  }

  TNode<BuiltinPtr> phi_bb14_8;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_8);
    ca_.Goto(&block2, phi_bb14_8);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block2, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinElement_GenericElementsAccessor_0)));
  }

  TNode<BuiltinPtr> phi_bb2_8;
  TNode<String> tmp29;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_8);
    tmp29 = ArrayJoinImpl_JSArray_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_receiver}, TNode<String>{p_sep}, TNode<Number>{p_lenNumber}, p_useToLocaleString, TNode<Object>{p_locales}, TNode<Object>{p_options}, TNode<BuiltinPtr>{phi_bb2_8});
    ca_.Goto(&block1, tmp29);
  }

  TNode<Object> phi_bb1_6;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_6);
    ca_.Goto(&block30, phi_bb1_6);
  }

  TNode<Object> phi_bb30_6;
    ca_.Bind(&block30, &phi_bb30_6);
  return TNode<Object>{phi_bb30_6};
}


TNode<Object> ArrayJoin_JSTypedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, bool p_useToLocaleString, TNode<JSReceiver> p_receiver, TNode<String> p_sep, TNode<Number> p_lenNumber, TNode<Object> p_locales, TNode<Object> p_options) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BuiltinPtr> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Map> tmp1;
  TNode<Int32T> tmp2;
  TNode<BoolT> tmp3;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{p_receiver, tmp0});
    tmp2 = CodeStubAssembler(state_).LoadMapElementsKind(TNode<Map>{tmp1});
    tmp3 = CodeStubAssembler(state_).IsElementsKindGreaterThan(TNode<Int32T>{tmp2}, ElementsKind::UINT32_ELEMENTS);
    ca_.Branch(tmp3, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<Int32T> tmp4;
  TNode<BoolT> tmp5;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp4 = FromConstexpr_ElementsKind_constexpr_INT32_ELEMENTS_0(state_, ElementsKind::INT32_ELEMENTS);
    tmp5 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp4});
    ca_.Branch(tmp5, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block7, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_Int32Elements_0)));
  }

  TNode<Int32T> tmp6;
  TNode<BoolT> tmp7;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp6 = FromConstexpr_ElementsKind_constexpr_FLOAT32_ELEMENTS_0(state_, ElementsKind::FLOAT32_ELEMENTS);
    tmp7 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp6});
    ca_.Branch(tmp7, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(&block10, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_Float32Elements_0)));
  }

  TNode<Int32T> tmp8;
  TNode<BoolT> tmp9;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp8 = FromConstexpr_ElementsKind_constexpr_FLOAT64_ELEMENTS_0(state_, ElementsKind::FLOAT64_ELEMENTS);
    tmp9 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp8});
    ca_.Branch(tmp9, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  if (block11.is_used()) {
    ca_.Bind(&block11);
    ca_.Goto(&block13, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_Float64Elements_0)));
  }

  TNode<Int32T> tmp10;
  TNode<BoolT> tmp11;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp10 = FromConstexpr_ElementsKind_constexpr_UINT8_CLAMPED_ELEMENTS_0(state_, ElementsKind::UINT8_CLAMPED_ELEMENTS);
    tmp11 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp10});
    ca_.Branch(tmp11, &block14, std::vector<compiler::Node*>{}, &block15, std::vector<compiler::Node*>{});
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block16, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_Uint8ClampedElements_0)));
  }

  TNode<Int32T> tmp12;
  TNode<BoolT> tmp13;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp12 = FromConstexpr_ElementsKind_constexpr_BIGUINT64_ELEMENTS_0(state_, ElementsKind::BIGUINT64_ELEMENTS);
    tmp13 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp12});
    ca_.Branch(tmp13, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block19, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_BigUint64Elements_0)));
  }

  TNode<Int32T> tmp14;
  TNode<BoolT> tmp15;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    tmp14 = FromConstexpr_ElementsKind_constexpr_BIGINT64_ELEMENTS_0(state_, ElementsKind::BIGINT64_ELEMENTS);
    tmp15 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp14});
    ca_.Branch(tmp15, &block20, std::vector<compiler::Node*>{}, &block21, std::vector<compiler::Node*>{});
  }

  if (block20.is_used()) {
    ca_.Bind(&block20);
    ca_.Goto(&block19, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_BigInt64Elements_0)));
  }

  if (block21.is_used()) {
    ca_.Bind(&block21);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BuiltinPtr> phi_bb19_8;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_8);
    ca_.Goto(&block16, phi_bb19_8);
  }

  TNode<BuiltinPtr> phi_bb16_8;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_8);
    ca_.Goto(&block13, phi_bb16_8);
  }

  TNode<BuiltinPtr> phi_bb13_8;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_8);
    ca_.Goto(&block10, phi_bb13_8);
  }

  TNode<BuiltinPtr> phi_bb10_8;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_8);
    ca_.Goto(&block7, phi_bb10_8);
  }

  TNode<BuiltinPtr> phi_bb7_8;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_8);
    ca_.Goto(&block4, phi_bb7_8);
  }

  TNode<Int32T> tmp16;
  TNode<BoolT> tmp17;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp16 = FromConstexpr_ElementsKind_constexpr_UINT8_ELEMENTS_0(state_, ElementsKind::UINT8_ELEMENTS);
    tmp17 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp16});
    ca_.Branch(tmp17, &block23, std::vector<compiler::Node*>{}, &block24, std::vector<compiler::Node*>{});
  }

  if (block23.is_used()) {
    ca_.Bind(&block23);
    ca_.Goto(&block25, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_Uint8Elements_0)));
  }

  TNode<Int32T> tmp18;
  TNode<BoolT> tmp19;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp18 = FromConstexpr_ElementsKind_constexpr_INT8_ELEMENTS_0(state_, ElementsKind::INT8_ELEMENTS);
    tmp19 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp18});
    ca_.Branch(tmp19, &block26, std::vector<compiler::Node*>{}, &block27, std::vector<compiler::Node*>{});
  }

  if (block26.is_used()) {
    ca_.Bind(&block26);
    ca_.Goto(&block28, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_Int8Elements_0)));
  }

  TNode<Int32T> tmp20;
  TNode<BoolT> tmp21;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp20 = FromConstexpr_ElementsKind_constexpr_UINT16_ELEMENTS_0(state_, ElementsKind::UINT16_ELEMENTS);
    tmp21 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp20});
    ca_.Branch(tmp21, &block29, std::vector<compiler::Node*>{}, &block30, std::vector<compiler::Node*>{});
  }

  if (block29.is_used()) {
    ca_.Bind(&block29);
    ca_.Goto(&block31, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_Uint16Elements_0)));
  }

  TNode<Int32T> tmp22;
  TNode<BoolT> tmp23;
  if (block30.is_used()) {
    ca_.Bind(&block30);
    tmp22 = FromConstexpr_ElementsKind_constexpr_INT16_ELEMENTS_0(state_, ElementsKind::INT16_ELEMENTS);
    tmp23 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp22});
    ca_.Branch(tmp23, &block32, std::vector<compiler::Node*>{}, &block33, std::vector<compiler::Node*>{});
  }

  if (block32.is_used()) {
    ca_.Bind(&block32);
    ca_.Goto(&block34, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_Int16Elements_0)));
  }

  TNode<Int32T> tmp24;
  TNode<BoolT> tmp25;
  if (block33.is_used()) {
    ca_.Bind(&block33);
    tmp24 = FromConstexpr_ElementsKind_constexpr_UINT32_ELEMENTS_0(state_, ElementsKind::UINT32_ELEMENTS);
    tmp25 = CodeStubAssembler(state_).ElementsKindEqual(TNode<Int32T>{tmp2}, TNode<Int32T>{tmp24});
    ca_.Branch(tmp25, &block35, std::vector<compiler::Node*>{}, &block36, std::vector<compiler::Node*>{});
  }

  if (block35.is_used()) {
    ca_.Bind(&block35);
    ca_.Goto(&block34, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinTypedElement_Uint32Elements_0)));
  }

  if (block36.is_used()) {
    ca_.Bind(&block36);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BuiltinPtr> phi_bb34_8;
  if (block34.is_used()) {
    ca_.Bind(&block34, &phi_bb34_8);
    ca_.Goto(&block31, phi_bb34_8);
  }

  TNode<BuiltinPtr> phi_bb31_8;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_8);
    ca_.Goto(&block28, phi_bb31_8);
  }

  TNode<BuiltinPtr> phi_bb28_8;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_8);
    ca_.Goto(&block25, phi_bb28_8);
  }

  TNode<BuiltinPtr> phi_bb25_8;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_8);
    ca_.Goto(&block4, phi_bb25_8);
  }

  TNode<BuiltinPtr> phi_bb4_8;
  TNode<String> tmp26;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_8);
    tmp26 = ArrayJoinImpl_JSTypedArray_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_receiver}, TNode<String>{p_sep}, TNode<Number>{p_lenNumber}, p_useToLocaleString, TNode<Object>{p_locales}, TNode<Object>{p_options}, TNode<BuiltinPtr>{phi_bb4_8});
    ca_.Goto(&block38);
  }

    ca_.Bind(&block38);
  return TNode<Object>{tmp26};
}


TNode<FixedArray> LoadJoinStack_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, compiler::CodeAssemblerLabel* label_IfUninitialized) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<HeapObject> tmp3;
  TNode<Oddball> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ARRAY_JOIN_STACK_INDEX_0(state_);
    std::tie(tmp1, tmp2) = NativeContextSlot_Context_Undefined_OR_FixedArray_0(state_, TNode<Context>{p_context}, TNode<IntPtrT>{tmp0}).Flatten();
    tmp3 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{tmp1, tmp2});
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_Undefined_2(state_, TNode<HeapObject>{tmp3}, &label5);
    ca_.Goto(&block5);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block7);
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(label_IfUninitialized);
  }

    ca_.Bind(&block7);
  return TNode<FixedArray>{ca_.UncheckedCast<FixedArray>(tmp3)};
}


void SetJoinStack_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<FixedArray> p_stack) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ARRAY_JOIN_STACK_INDEX_0(state_);
    std::tie(tmp1, tmp2) = NativeContextSlot_Context_Undefined_OR_FixedArray_0(state_, TNode<Context>{p_context}, TNode<IntPtrT>{tmp0}).Flatten();
    CodeStubAssembler(state_).StoreReference<HeapObject>(CodeStubAssembler::Reference{tmp1, tmp2}, p_stack);
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
}


TF_BUILTIN(JoinStackPush, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<FixedArray> parameter1 = UncheckedParameter<FixedArray>(Descriptor::kStack);
  USE(parameter1);
  TNode<JSReceiver> parameter2 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{parameter1});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block3, tmp1);
  }

  TNode<IntPtrT> phi_bb3_4;
  TNode<BoolT> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_4);
    tmp2 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{phi_bb3_4}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp2, &block1, std::vector<compiler::Node*>{phi_bb3_4}, &block2, std::vector<compiler::Node*>{phi_bb3_4});
  }

  TNode<IntPtrT> phi_bb1_4;
  TNode<Object> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  TNode<UintPtrT> tmp7;
  TNode<BoolT> tmp8;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_4);
    std::tie(tmp3, tmp4, tmp5) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{parameter1}).Flatten();
    tmp6 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb1_4});
    tmp7 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp5});
    tmp8 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp6}, TNode<UintPtrT>{tmp7});
    ca_.Branch(tmp8, &block9, std::vector<compiler::Node*>{phi_bb1_4, phi_bb1_4, phi_bb1_4, phi_bb1_4, phi_bb1_4}, &block10, std::vector<compiler::Node*>{phi_bb1_4, phi_bb1_4, phi_bb1_4, phi_bb1_4, phi_bb1_4});
  }

  TNode<IntPtrT> phi_bb9_4;
  TNode<IntPtrT> phi_bb9_9;
  TNode<IntPtrT> phi_bb9_10;
  TNode<IntPtrT> phi_bb9_14;
  TNode<IntPtrT> phi_bb9_15;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<Object> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  TNode<Oddball> tmp14;
  TNode<BoolT> tmp15;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_4, &phi_bb9_9, &phi_bb9_10, &phi_bb9_14, &phi_bb9_15);
    tmp9 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb9_15});
    tmp10 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp9});
    std::tie(tmp11, tmp12) = NewReference_Object_0(state_, TNode<Object>{tmp3}, TNode<IntPtrT>{tmp10}).Flatten();
    tmp13 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp11, tmp12});
    tmp14 = TheHole_0(state_);
    tmp15 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp13}, TNode<HeapObject>{tmp14});
    ca_.Branch(tmp15, &block12, std::vector<compiler::Node*>{phi_bb9_4}, &block13, std::vector<compiler::Node*>{phi_bb9_4});
  }

  TNode<IntPtrT> phi_bb10_4;
  TNode<IntPtrT> phi_bb10_9;
  TNode<IntPtrT> phi_bb10_10;
  TNode<IntPtrT> phi_bb10_14;
  TNode<IntPtrT> phi_bb10_15;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_4, &phi_bb10_9, &phi_bb10_10, &phi_bb10_14, &phi_bb10_15);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb12_4;
  TNode<Object> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<IntPtrT> tmp18;
  TNode<UintPtrT> tmp19;
  TNode<UintPtrT> tmp20;
  TNode<BoolT> tmp21;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_4);
    std::tie(tmp16, tmp17, tmp18) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{parameter1}).Flatten();
    tmp19 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb12_4});
    tmp20 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp18});
    tmp21 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp19}, TNode<UintPtrT>{tmp20});
    ca_.Branch(tmp21, &block18, std::vector<compiler::Node*>{phi_bb12_4, phi_bb12_4, phi_bb12_4, phi_bb12_4, phi_bb12_4}, &block19, std::vector<compiler::Node*>{phi_bb12_4, phi_bb12_4, phi_bb12_4, phi_bb12_4, phi_bb12_4});
  }

  TNode<IntPtrT> phi_bb18_4;
  TNode<IntPtrT> phi_bb18_10;
  TNode<IntPtrT> phi_bb18_11;
  TNode<IntPtrT> phi_bb18_15;
  TNode<IntPtrT> phi_bb18_16;
  TNode<IntPtrT> tmp22;
  TNode<IntPtrT> tmp23;
  TNode<Object> tmp24;
  TNode<IntPtrT> tmp25;
  TNode<Oddball> tmp26;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_4, &phi_bb18_10, &phi_bb18_11, &phi_bb18_15, &phi_bb18_16);
    tmp22 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb18_16});
    tmp23 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp17}, TNode<IntPtrT>{tmp22});
    std::tie(tmp24, tmp25) = NewReference_Object_0(state_, TNode<Object>{tmp16}, TNode<IntPtrT>{tmp23}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp24, tmp25}, parameter2);
    tmp26 = True_0(state_);
    CodeStubAssembler(state_).Return(tmp26);
  }

  TNode<IntPtrT> phi_bb19_4;
  TNode<IntPtrT> phi_bb19_10;
  TNode<IntPtrT> phi_bb19_11;
  TNode<IntPtrT> phi_bb19_15;
  TNode<IntPtrT> phi_bb19_16;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_4, &phi_bb19_10, &phi_bb19_11, &phi_bb19_15, &phi_bb19_16);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb13_4;
  TNode<BoolT> tmp27;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_4);
    tmp27 = CodeStubAssembler(state_).TaggedEqual(TNode<HeapObject>{parameter2}, TNode<Object>{tmp13});
    ca_.Branch(tmp27, &block21, std::vector<compiler::Node*>{phi_bb13_4}, &block22, std::vector<compiler::Node*>{phi_bb13_4});
  }

  TNode<IntPtrT> phi_bb21_4;
  TNode<Oddball> tmp28;
  if (block21.is_used()) {
    ca_.Bind(&block21, &phi_bb21_4);
    tmp28 = False_0(state_);
    CodeStubAssembler(state_).Return(tmp28);
  }

  TNode<IntPtrT> phi_bb22_4;
  TNode<IntPtrT> tmp29;
  TNode<IntPtrT> tmp30;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_4);
    tmp29 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp30 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb22_4}, TNode<IntPtrT>{tmp29});
    ca_.Goto(&block3, tmp30);
  }

  TNode<IntPtrT> phi_bb2_4;
  TNode<FixedArray> tmp31;
  TNode<Oddball> tmp32;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_4);
    tmp31 = StoreAndGrowFixedArray_JSReceiver_0(state_, TNode<FixedArray>{parameter1}, TNode<IntPtrT>{tmp0}, TNode<JSReceiver>{parameter2});
    SetJoinStack_0(state_, TNode<Context>{parameter0}, TNode<FixedArray>{tmp31});
    tmp32 = True_0(state_);
    CodeStubAssembler(state_).Return(tmp32);
  }
}

TNode<BoolT> JoinStackPushInline_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_receiver) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FixedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = LoadJoinStack_0(state_, TNode<Context>{p_context}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  TNode<IntPtrT> tmp2;
  TNode<FixedArray> tmp3;
  TNode<Object> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<UintPtrT> tmp8;
  TNode<UintPtrT> tmp9;
  TNode<BoolT> tmp10;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, JSArray::kMinJoinStackSize);
    tmp3 = CodeStubAssembler(state_).AllocateFixedArrayWithHoles(TNode<IntPtrT>{tmp2}, CodeStubAssembler::AllocationFlag::kNone);
    std::tie(tmp4, tmp5, tmp6) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp3}).Flatten();
    tmp7 = Convert_intptr_constexpr_int31_0(state_, 0);
    tmp8 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp7});
    tmp9 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp6});
    tmp10 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp8}, TNode<UintPtrT>{tmp9});
    ca_.Branch(tmp10, &block29, std::vector<compiler::Node*>{}, &block30, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<UintPtrT> tmp15;
  TNode<UintPtrT> tmp16;
  TNode<BoolT> tmp17;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    std::tie(tmp11, tmp12, tmp13) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp0}).Flatten();
    tmp14 = Convert_intptr_constexpr_int31_0(state_, 0);
    tmp15 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp14});
    tmp16 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp13});
    tmp17 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp15}, TNode<UintPtrT>{tmp16});
    ca_.Branch(tmp17, &block12, std::vector<compiler::Node*>{}, &block13, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<Object> tmp20;
  TNode<IntPtrT> tmp21;
  TNode<Object> tmp22;
  TNode<Oddball> tmp23;
  TNode<BoolT> tmp24;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp18 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp14});
    tmp19 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp12}, TNode<IntPtrT>{tmp18});
    std::tie(tmp20, tmp21) = NewReference_Object_0(state_, TNode<Object>{tmp11}, TNode<IntPtrT>{tmp19}).Flatten();
    tmp22 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp20, tmp21});
    tmp23 = TheHole_0(state_);
    tmp24 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp22}, TNode<HeapObject>{tmp23});
    ca_.Branch(tmp24, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> tmp25;
  TNode<IntPtrT> tmp26;
  TNode<IntPtrT> tmp27;
  TNode<IntPtrT> tmp28;
  TNode<UintPtrT> tmp29;
  TNode<UintPtrT> tmp30;
  TNode<BoolT> tmp31;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    std::tie(tmp25, tmp26, tmp27) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp0}).Flatten();
    tmp28 = Convert_intptr_constexpr_int31_0(state_, 0);
    tmp29 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp28});
    tmp30 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp27});
    tmp31 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp29}, TNode<UintPtrT>{tmp30});
    ca_.Branch(tmp31, &block20, std::vector<compiler::Node*>{}, &block21, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp32;
  TNode<IntPtrT> tmp33;
  TNode<Object> tmp34;
  TNode<IntPtrT> tmp35;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp32 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp28});
    tmp33 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp26}, TNode<IntPtrT>{tmp32});
    std::tie(tmp34, tmp35) = NewReference_Object_0(state_, TNode<Object>{tmp25}, TNode<IntPtrT>{tmp33}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp34, tmp35}, p_receiver);
    ca_.Goto(&block15);
  }

  if (block21.is_used()) {
    ca_.Bind(&block21);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Oddball> tmp36;
  TNode<Oddball> tmp37;
  TNode<BoolT> tmp38;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp36 = ca_.CallStub<Oddball>(Builtins::CallableFor(ca_.isolate(), Builtin::kJoinStackPush), p_context, tmp0, p_receiver);
    tmp37 = False_0(state_);
    tmp38 = CodeStubAssembler(state_).TaggedEqual(TNode<HeapObject>{tmp36}, TNode<HeapObject>{tmp37});
    ca_.Branch(tmp38, &block23, std::vector<compiler::Node*>{}, &block24, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp39;
  if (block23.is_used()) {
    ca_.Bind(&block23);
    tmp39 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block1, tmp39);
  }

  if (block24.is_used()) {
    ca_.Bind(&block24);
    ca_.Goto(&block15);
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block2);
  }

  TNode<IntPtrT> tmp40;
  TNode<IntPtrT> tmp41;
  TNode<Object> tmp42;
  TNode<IntPtrT> tmp43;
  if (block29.is_used()) {
    ca_.Bind(&block29);
    tmp40 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp7});
    tmp41 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp5}, TNode<IntPtrT>{tmp40});
    std::tie(tmp42, tmp43) = NewReference_Object_0(state_, TNode<Object>{tmp4}, TNode<IntPtrT>{tmp41}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp42, tmp43}, p_receiver);
    SetJoinStack_0(state_, TNode<Context>{p_context}, TNode<FixedArray>{tmp3});
    ca_.Goto(&block2);
  }

  if (block30.is_used()) {
    ca_.Bind(&block30);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BoolT> tmp44;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp44 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block1, tmp44);
  }

  TNode<BoolT> phi_bb1_2;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_2);
    ca_.Goto(&block32, phi_bb1_2);
  }

  TNode<BoolT> phi_bb32_2;
    ca_.Bind(&block32, &phi_bb32_2);
  return TNode<BoolT>{phi_bb32_2};
}


TF_BUILTIN(JoinStackPop, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<FixedArray> parameter1 = UncheckedParameter<FixedArray>(Descriptor::kStack);
  USE(parameter1);
  TNode<JSReceiver> parameter2 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BoolT> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block14(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{parameter1});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block3, tmp1);
  }

  TNode<IntPtrT> phi_bb3_4;
  TNode<BoolT> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_4);
    tmp2 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{phi_bb3_4}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp2, &block1, std::vector<compiler::Node*>{phi_bb3_4}, &block2, std::vector<compiler::Node*>{phi_bb3_4});
  }

  TNode<IntPtrT> phi_bb1_4;
  TNode<Object> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  TNode<UintPtrT> tmp7;
  TNode<BoolT> tmp8;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_4);
    std::tie(tmp3, tmp4, tmp5) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{parameter1}).Flatten();
    tmp6 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb1_4});
    tmp7 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp5});
    tmp8 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp6}, TNode<UintPtrT>{tmp7});
    ca_.Branch(tmp8, &block11, std::vector<compiler::Node*>{phi_bb1_4, phi_bb1_4, phi_bb1_4, phi_bb1_4, phi_bb1_4}, &block12, std::vector<compiler::Node*>{phi_bb1_4, phi_bb1_4, phi_bb1_4, phi_bb1_4, phi_bb1_4});
  }

  TNode<IntPtrT> phi_bb11_4;
  TNode<IntPtrT> phi_bb11_9;
  TNode<IntPtrT> phi_bb11_10;
  TNode<IntPtrT> phi_bb11_14;
  TNode<IntPtrT> phi_bb11_15;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<Object> tmp11;
  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  TNode<BoolT> tmp14;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_4, &phi_bb11_9, &phi_bb11_10, &phi_bb11_14, &phi_bb11_15);
    tmp9 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb11_15});
    tmp10 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp9});
    std::tie(tmp11, tmp12) = NewReference_Object_0(state_, TNode<Object>{tmp3}, TNode<IntPtrT>{tmp10}).Flatten();
    tmp13 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp11, tmp12});
    tmp14 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp13}, TNode<HeapObject>{parameter2});
    ca_.Branch(tmp14, &block5, std::vector<compiler::Node*>{phi_bb11_4}, &block6, std::vector<compiler::Node*>{phi_bb11_4});
  }

  TNode<IntPtrT> phi_bb12_4;
  TNode<IntPtrT> phi_bb12_9;
  TNode<IntPtrT> phi_bb12_10;
  TNode<IntPtrT> phi_bb12_14;
  TNode<IntPtrT> phi_bb12_15;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_4, &phi_bb12_9, &phi_bb12_10, &phi_bb12_14, &phi_bb12_15);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb5_4;
  TNode<IntPtrT> tmp15;
  TNode<BoolT> tmp16;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_4);
    tmp15 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp16 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb5_4}, TNode<IntPtrT>{tmp15});
    ca_.Branch(tmp16, &block16, std::vector<compiler::Node*>{phi_bb5_4}, &block17, std::vector<compiler::Node*>{phi_bb5_4});
  }

  TNode<IntPtrT> phi_bb16_4;
  TNode<IntPtrT> tmp17;
  TNode<BoolT> tmp18;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_4);
    tmp17 = FromConstexpr_intptr_constexpr_int31_0(state_, JSArray::kMinJoinStackSize);
    tmp18 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{tmp0}, TNode<IntPtrT>{tmp17});
    ca_.Goto(&block18, phi_bb16_4, tmp18);
  }

  TNode<IntPtrT> phi_bb17_4;
  TNode<BoolT> tmp19;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_4);
    tmp19 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block18, phi_bb17_4, tmp19);
  }

  TNode<IntPtrT> phi_bb18_4;
  TNode<BoolT> phi_bb18_6;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_4, &phi_bb18_6);
    ca_.Branch(phi_bb18_6, &block14, std::vector<compiler::Node*>{phi_bb18_4}, &block15, std::vector<compiler::Node*>{phi_bb18_4});
  }

  TNode<IntPtrT> phi_bb14_4;
  TNode<IntPtrT> tmp20;
  TNode<FixedArray> tmp21;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_4);
    tmp20 = FromConstexpr_intptr_constexpr_int31_0(state_, JSArray::kMinJoinStackSize);
    tmp21 = CodeStubAssembler(state_).AllocateFixedArrayWithHoles(TNode<IntPtrT>{tmp20}, CodeStubAssembler::AllocationFlag::kNone);
    SetJoinStack_0(state_, TNode<Context>{parameter0}, TNode<FixedArray>{tmp21});
    ca_.Goto(&block19, phi_bb14_4);
  }

  TNode<IntPtrT> phi_bb15_4;
  TNode<Object> tmp22;
  TNode<IntPtrT> tmp23;
  TNode<IntPtrT> tmp24;
  TNode<UintPtrT> tmp25;
  TNode<UintPtrT> tmp26;
  TNode<BoolT> tmp27;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_4);
    std::tie(tmp22, tmp23, tmp24) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{parameter1}).Flatten();
    tmp25 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb15_4});
    tmp26 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp24});
    tmp27 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp25}, TNode<UintPtrT>{tmp26});
    ca_.Branch(tmp27, &block24, std::vector<compiler::Node*>{phi_bb15_4, phi_bb15_4, phi_bb15_4, phi_bb15_4, phi_bb15_4}, &block25, std::vector<compiler::Node*>{phi_bb15_4, phi_bb15_4, phi_bb15_4, phi_bb15_4, phi_bb15_4});
  }

  TNode<IntPtrT> phi_bb24_4;
  TNode<IntPtrT> phi_bb24_9;
  TNode<IntPtrT> phi_bb24_10;
  TNode<IntPtrT> phi_bb24_14;
  TNode<IntPtrT> phi_bb24_15;
  TNode<IntPtrT> tmp28;
  TNode<IntPtrT> tmp29;
  TNode<Object> tmp30;
  TNode<IntPtrT> tmp31;
  TNode<Oddball> tmp32;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_4, &phi_bb24_9, &phi_bb24_10, &phi_bb24_14, &phi_bb24_15);
    tmp28 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb24_15});
    tmp29 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp23}, TNode<IntPtrT>{tmp28});
    std::tie(tmp30, tmp31) = NewReference_Object_0(state_, TNode<Object>{tmp22}, TNode<IntPtrT>{tmp29}).Flatten();
    tmp32 = TheHole_0(state_);
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp30, tmp31}, tmp32);
    ca_.Goto(&block19, phi_bb24_4);
  }

  TNode<IntPtrT> phi_bb25_4;
  TNode<IntPtrT> phi_bb25_9;
  TNode<IntPtrT> phi_bb25_10;
  TNode<IntPtrT> phi_bb25_14;
  TNode<IntPtrT> phi_bb25_15;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_4, &phi_bb25_9, &phi_bb25_10, &phi_bb25_14, &phi_bb25_15);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb19_4;
  TNode<Oddball> tmp33;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_4);
    tmp33 = Undefined_0(state_);
    CodeStubAssembler(state_).Return(tmp33);
  }

  TNode<IntPtrT> phi_bb6_4;
  TNode<IntPtrT> tmp34;
  TNode<IntPtrT> tmp35;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_4);
    tmp34 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp35 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb6_4}, TNode<IntPtrT>{tmp34});
    ca_.Goto(&block3, tmp35);
  }

  TNode<IntPtrT> phi_bb2_4;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_4);
    CodeStubAssembler(state_).Unreachable();
  }
}

void JoinStackPopInline_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_receiver) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FixedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = LoadJoinStack_0(state_, TNode<Context>{p_context}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<UintPtrT> tmp7;
  TNode<UintPtrT> tmp8;
  TNode<BoolT> tmp9;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp2 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{tmp0});
    std::tie(tmp3, tmp4, tmp5) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp0}).Flatten();
    tmp6 = Convert_intptr_constexpr_int31_0(state_, 0);
    tmp7 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp6});
    tmp8 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp5});
    tmp9 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp7}, TNode<UintPtrT>{tmp8});
    ca_.Branch(tmp9, &block12, std::vector<compiler::Node*>{}, &block13, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<Object> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Object> tmp14;
  TNode<BoolT> tmp15;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp10 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp6});
    tmp11 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp10});
    std::tie(tmp12, tmp13) = NewReference_Object_0(state_, TNode<Object>{tmp3}, TNode<IntPtrT>{tmp11}).Flatten();
    tmp14 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp12, tmp13});
    tmp15 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp14}, TNode<HeapObject>{p_receiver});
    ca_.Branch(tmp15, &block15, std::vector<compiler::Node*>{}, &block16, std::vector<compiler::Node*>{});
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp16;
  TNode<BoolT> tmp17;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp16 = FromConstexpr_intptr_constexpr_int31_0(state_, JSArray::kMinJoinStackSize);
    tmp17 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp2}, TNode<IntPtrT>{tmp16});
    ca_.Goto(&block17, tmp17);
  }

  TNode<BoolT> tmp18;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp18 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block17, tmp18);
  }

  TNode<BoolT> phi_bb17_5;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_5);
    ca_.Branch(phi_bb17_5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<Oddball> tmp19;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp19 = TheHole_0(state_);
    CodeStubAssembler(state_).StoreFixedArrayElement(TNode<FixedArray>{tmp0}, 0, TNode<Object>{tmp19}, SKIP_WRITE_BARRIER);
    ca_.Goto(&block18);
  }

  TNode<Object> tmp20;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp20 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kJoinStackPop), p_context, tmp0, p_receiver);
    ca_.Goto(&block18);
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    ca_.Goto(&block19);
  }

    ca_.Bind(&block19);
}


TF_BUILTIN(ArrayPrototypeJoin, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<JSReceiver> tmp2;
  TNode<Number> tmp3;
  TNode<Number> tmp4;
  TNode<BoolT> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = CodeStubAssembler(state_).ToObject_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp3 = GetLengthProperty_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp2});
    tmp4 = FromConstexpr_Number_constexpr_uint32_0(state_, JSArray::kMaxArrayLength);
    tmp5 = NumberIsGreaterThan_0(state_, TNode<Number>{tmp3}, TNode<Number>{tmp4});
    ca_.Branch(tmp5, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kInvalidArrayLength);
  }

  TNode<Oddball> tmp6;
  TNode<Oddball> tmp7;
  TNode<Object> tmp8;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp6 = Undefined_0(state_);
    tmp7 = Undefined_0(state_);
    tmp8 = CycleProtectedArrayJoin_JSArray_0(state_, TNode<Context>{parameter0}, false, TNode<JSReceiver>{tmp2}, TNode<Number>{tmp3}, TNode<Object>{tmp1}, TNode<Object>{tmp6}, TNode<Object>{tmp7});
    arguments.PopAndReturn(tmp8);
  }
}

TF_BUILTIN(ArrayPrototypeToLocaleString, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<JSReceiver> tmp4;
  TNode<Number> tmp5;
  TNode<Number> tmp6;
  TNode<BoolT> tmp7;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp3 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp2});
    tmp4 = CodeStubAssembler(state_).ToObject_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp5 = GetLengthProperty_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp4});
    tmp6 = FromConstexpr_Number_constexpr_uint32_0(state_, JSArray::kMaxArrayLength);
    tmp7 = NumberIsGreaterThan_0(state_, TNode<Number>{tmp5}, TNode<Number>{tmp6});
    ca_.Branch(tmp7, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kInvalidArrayLength);
  }

  TNode<Object> tmp8;
  TNode<Object> tmp9;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp8 = FromConstexpr_JSAny_constexpr_string_0(state_, ",");
    tmp9 = CycleProtectedArrayJoin_JSArray_0(state_, TNode<Context>{parameter0}, true, TNode<JSReceiver>{tmp4}, TNode<Number>{tmp5}, TNode<Object>{tmp8}, TNode<Object>{tmp1}, TNode<Object>{tmp3});
    arguments.PopAndReturn(tmp9);
  }
}

TF_BUILTIN(ArrayPrototypeToString, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  TNode<Object> tmp1;
  TNode<Object> tmp2;
  TNode<JSReceiver> tmp3;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).ToObject_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = FromConstexpr_JSAny_constexpr_string_0(state_, "join");
    tmp2 = CodeStubAssembler(state_).GetProperty(TNode<Context>{parameter0}, TNode<Object>{tmp0}, TNode<Object>{tmp1});
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp2}, &label4);
    ca_.Goto(&block3);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block4);
    }
  }

  TNode<String> tmp5;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp5 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kObjectToString), parameter0, tmp0);
    arguments.PopAndReturn(tmp5);
  }

  TNode<Object> tmp6;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp6 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp3}, TNode<Object>{tmp0});
    arguments.PopAndReturn(tmp6);
  }
}

TF_BUILTIN(TypedArrayPrototypeJoin, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<JSTypedArray> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<Number> tmp5;
  TNode<Oddball> tmp6;
  TNode<Oddball> tmp7;
  TNode<Object> tmp8;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = TypedArrayBuiltinsAssembler(state_).ValidateTypedArray(TNode<Context>{parameter0}, TNode<Object>{parameter1}, "%TypedArray%.prototype.join");
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp4 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp2, tmp3});
    tmp5 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{tmp4});
    tmp6 = Undefined_0(state_);
    tmp7 = Undefined_0(state_);
    tmp8 = CycleProtectedArrayJoin_JSTypedArray_0(state_, TNode<Context>{parameter0}, false, TNode<JSReceiver>{tmp2}, TNode<Number>{tmp5}, TNode<Object>{tmp1}, TNode<Object>{tmp6}, TNode<Object>{tmp7});
    arguments.PopAndReturn(tmp8);
  }
}

TF_BUILTIN(TypedArrayPrototypeToLocaleString, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<JSTypedArray> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  TNode<Number> tmp7;
  TNode<Object> tmp8;
  TNode<Object> tmp9;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp3 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp2});
    tmp4 = TypedArrayBuiltinsAssembler(state_).ValidateTypedArray(TNode<Context>{parameter0}, TNode<Object>{parameter1}, "%TypedArray%.prototype.toLocaleString");
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp6 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp4, tmp5});
    tmp7 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{tmp6});
    tmp8 = FromConstexpr_JSAny_constexpr_string_0(state_, ",");
    tmp9 = CycleProtectedArrayJoin_JSTypedArray_0(state_, TNode<Context>{parameter0}, true, TNode<JSReceiver>{tmp4}, TNode<Number>{tmp7}, TNode<Object>{tmp8}, TNode<Object>{tmp1}, TNode<Object>{tmp3});
    arguments.PopAndReturn(tmp9);
  }
}

TNode<JSArray> UnsafeCast_JSArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<JSArray>{tmp0};
}


TNode<NumberDictionary> UnsafeCast_NumberDictionary_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<NumberDictionary> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<NumberDictionary>{tmp0};
}


TNode<FixedArray> UnsafeCast_FixedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FixedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<FixedArray>{tmp0};
}


TNode<Object> UnsafeCast_JSAny_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = (TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<Object>{tmp0};
}


TNode<FixedDoubleArray> UnsafeCast_FixedDoubleArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<FixedDoubleArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<FixedDoubleArray>{tmp0};
}


TF_BUILTIN(LoadJoinElement_GenericElementsAccessor_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Number> tmp0;
  TNode<Object> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{parameter2});
    tmp1 = CodeStubAssembler(state_).GetProperty(TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{tmp0});
    CodeStubAssembler(state_).Return(tmp1);
  }
}

TNode<JSTypedArray> UnsafeCast_JSTypedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<JSTypedArray>{tmp0};
}


TNode<String> Cast_String_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o, compiler::CodeAssemblerLabel* label_CastError) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<HeapObject> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = CodeStubAssembler(state_).TaggedToHeapObject(TNode<Object>{p_o}, &label1);
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

  TNode<String> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_String_0(state_, TNode<HeapObject>{tmp0}, &label3);
    ca_.Goto(&block5);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block1);
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block7);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(label_CastError);
  }

    ca_.Bind(&block7);
  return TNode<String>{tmp2};
}


TNode<String> ArrayJoinImpl_JSArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_receiver, TNode<String> p_sep, TNode<Number> p_lengthNumber, bool p_useToLocaleString, TNode<Object> p_locales, TNode<Object> p_options, TNode<BuiltinPtr> p_initialLoadFn) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block5(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String, BoolT> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block33(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, BoolT, String, String, String> block35(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, BoolT, String, String, String> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, BoolT> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block39(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, IntPtrT> block43(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, IntPtrT> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, IntPtrT> block45(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, IntPtrT> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Map> tmp1;
  TNode<UintPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<FixedArray> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<BoolT> tmp8;
  TNode<UintPtrT> tmp9;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{p_receiver, tmp0});
    tmp2 = Convert_uintptr_Number_0(state_, TNode<Number>{p_lengthNumber});
    tmp3 = CodeStubAssembler(state_).LoadStringLengthAsWord(TNode<String>{p_sep});
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    std::tie(tmp5, tmp6, tmp7, tmp8) = NewBuffer_0(state_, TNode<UintPtrT>{tmp2}, TNode<String>{p_sep}).Flatten();
    tmp9 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block4, tmp4, p_initialLoadFn, tmp5, tmp6, tmp7, tmp8, tmp9);
  }

  TNode<IntPtrT> phi_bb4_10;
  TNode<BuiltinPtr> phi_bb4_11;
  TNode<FixedArray> phi_bb4_12;
  TNode<IntPtrT> phi_bb4_13;
  TNode<IntPtrT> phi_bb4_14;
  TNode<BoolT> phi_bb4_15;
  TNode<UintPtrT> phi_bb4_16;
  TNode<BoolT> tmp10;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_10, &phi_bb4_11, &phi_bb4_12, &phi_bb4_13, &phi_bb4_14, &phi_bb4_15, &phi_bb4_16);
    tmp10 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb4_16}, TNode<UintPtrT>{tmp2});
    ca_.Branch(tmp10, &block2, std::vector<compiler::Node*>{phi_bb4_10, phi_bb4_11, phi_bb4_12, phi_bb4_13, phi_bb4_14, phi_bb4_15, phi_bb4_16}, &block3, std::vector<compiler::Node*>{phi_bb4_10, phi_bb4_11, phi_bb4_12, phi_bb4_13, phi_bb4_14, phi_bb4_15, phi_bb4_16});
  }

  TNode<IntPtrT> phi_bb2_10;
  TNode<BuiltinPtr> phi_bb2_11;
  TNode<FixedArray> phi_bb2_12;
  TNode<IntPtrT> phi_bb2_13;
  TNode<IntPtrT> phi_bb2_14;
  TNode<BoolT> phi_bb2_15;
  TNode<UintPtrT> phi_bb2_16;
  TNode<BoolT> tmp11;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_10, &phi_bb2_11, &phi_bb2_12, &phi_bb2_13, &phi_bb2_14, &phi_bb2_15, &phi_bb2_16);
    tmp11 = CannotUseSameArrayAccessor_JSArray_0(state_, TNode<Context>{p_context}, TNode<BuiltinPtr>{phi_bb2_11}, TNode<JSReceiver>{p_receiver}, TNode<Map>{tmp1}, TNode<Number>{p_lengthNumber});
    ca_.Branch(tmp11, &block5, std::vector<compiler::Node*>{phi_bb2_10, phi_bb2_11, phi_bb2_12, phi_bb2_13, phi_bb2_14, phi_bb2_15, phi_bb2_16}, &block6, std::vector<compiler::Node*>{phi_bb2_10, phi_bb2_11, phi_bb2_12, phi_bb2_13, phi_bb2_14, phi_bb2_15, phi_bb2_16});
  }

  TNode<IntPtrT> phi_bb5_10;
  TNode<BuiltinPtr> phi_bb5_11;
  TNode<FixedArray> phi_bb5_12;
  TNode<IntPtrT> phi_bb5_13;
  TNode<IntPtrT> phi_bb5_14;
  TNode<BoolT> phi_bb5_15;
  TNode<UintPtrT> phi_bb5_16;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_10, &phi_bb5_11, &phi_bb5_12, &phi_bb5_13, &phi_bb5_14, &phi_bb5_15, &phi_bb5_16);
    ca_.Goto(&block6, phi_bb5_10, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinElement_GenericElementsAccessor_0)), phi_bb5_12, phi_bb5_13, phi_bb5_14, phi_bb5_15, phi_bb5_16);
  }

  TNode<IntPtrT> phi_bb6_10;
  TNode<BuiltinPtr> phi_bb6_11;
  TNode<FixedArray> phi_bb6_12;
  TNode<IntPtrT> phi_bb6_13;
  TNode<IntPtrT> phi_bb6_14;
  TNode<BoolT> phi_bb6_15;
  TNode<UintPtrT> phi_bb6_16;
  TNode<UintPtrT> tmp12;
  TNode<BoolT> tmp13;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_10, &phi_bb6_11, &phi_bb6_12, &phi_bb6_13, &phi_bb6_14, &phi_bb6_15, &phi_bb6_16);
    tmp12 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp13 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{phi_bb6_16}, TNode<UintPtrT>{tmp12});
    ca_.Branch(tmp13, &block7, std::vector<compiler::Node*>{phi_bb6_10, phi_bb6_12, phi_bb6_13, phi_bb6_14, phi_bb6_15, phi_bb6_16}, &block8, std::vector<compiler::Node*>{phi_bb6_10, phi_bb6_12, phi_bb6_13, phi_bb6_14, phi_bb6_15, phi_bb6_16});
  }

  TNode<IntPtrT> phi_bb7_10;
  TNode<FixedArray> phi_bb7_12;
  TNode<IntPtrT> phi_bb7_13;
  TNode<IntPtrT> phi_bb7_14;
  TNode<BoolT> phi_bb7_15;
  TNode<UintPtrT> phi_bb7_16;
  TNode<IntPtrT> tmp14;
  TNode<IntPtrT> tmp15;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_10, &phi_bb7_12, &phi_bb7_13, &phi_bb7_14, &phi_bb7_15, &phi_bb7_16);
    tmp14 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp15 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb7_10}, TNode<IntPtrT>{tmp14});
    ca_.Goto(&block8, tmp15, phi_bb7_12, phi_bb7_13, phi_bb7_14, phi_bb7_15, phi_bb7_16);
  }

  TNode<IntPtrT> phi_bb8_10;
  TNode<FixedArray> phi_bb8_12;
  TNode<IntPtrT> phi_bb8_13;
  TNode<IntPtrT> phi_bb8_14;
  TNode<BoolT> phi_bb8_15;
  TNode<UintPtrT> phi_bb8_16;
  TNode<UintPtrT> tmp16;
  TNode<UintPtrT> tmp17;
  TNode<Object> tmp18;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_10, &phi_bb8_12, &phi_bb8_13, &phi_bb8_14, &phi_bb8_15, &phi_bb8_16);
    tmp16 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp17 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb8_16}, TNode<UintPtrT>{tmp16});
tmp18 = CodeStubAssembler(state_).CallBuiltinPointer(Builtins::CallableFor(ca_.isolate(),ExampleBuiltinForTorqueFunctionPointerType(0)).descriptor(), phi_bb6_11, p_context, p_receiver, phi_bb8_16);
    if ((p_useToLocaleString)) {
      ca_.Goto(&block9, phi_bb8_12, phi_bb8_13, phi_bb8_14, phi_bb8_15);
    } else {
      ca_.Goto(&block10, phi_bb8_12, phi_bb8_13, phi_bb8_14, phi_bb8_15);
    }
  }

  TNode<FixedArray> phi_bb9_12;
  TNode<IntPtrT> phi_bb9_13;
  TNode<IntPtrT> phi_bb9_14;
  TNode<BoolT> phi_bb9_15;
  TNode<String> tmp19;
  TNode<String> tmp20;
  TNode<BoolT> tmp21;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_12, &phi_bb9_13, &phi_bb9_14, &phi_bb9_15);
    tmp19 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kConvertToLocaleString), p_context, tmp18, p_locales, p_options);
    tmp20 = kEmptyString_0(state_);
    tmp21 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp19}, TNode<HeapObject>{tmp20});
    ca_.Branch(tmp21, &block12, std::vector<compiler::Node*>{phi_bb9_12, phi_bb9_13, phi_bb9_14, phi_bb9_15}, &block13, std::vector<compiler::Node*>{phi_bb9_12, phi_bb9_13, phi_bb9_14, phi_bb9_15});
  }

  TNode<FixedArray> phi_bb12_12;
  TNode<IntPtrT> phi_bb12_13;
  TNode<IntPtrT> phi_bb12_14;
  TNode<BoolT> phi_bb12_15;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_12, &phi_bb12_13, &phi_bb12_14, &phi_bb12_15);
    ca_.Goto(&block4, phi_bb8_10, phi_bb6_11, phi_bb12_12, phi_bb12_13, phi_bb12_14, phi_bb12_15, tmp17);
  }

  TNode<FixedArray> phi_bb13_12;
  TNode<IntPtrT> phi_bb13_13;
  TNode<IntPtrT> phi_bb13_14;
  TNode<BoolT> phi_bb13_15;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_12, &phi_bb13_13, &phi_bb13_14, &phi_bb13_15);
    ca_.Goto(&block11, phi_bb13_12, phi_bb13_13, phi_bb13_14, phi_bb13_15, tmp19);
  }

  TNode<FixedArray> phi_bb10_12;
  TNode<IntPtrT> phi_bb10_13;
  TNode<IntPtrT> phi_bb10_14;
  TNode<BoolT> phi_bb10_15;
  TNode<String> tmp22;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_12, &phi_bb10_13, &phi_bb10_14, &phi_bb10_15);
    compiler::CodeAssemblerLabel label23(&ca_);
    tmp22 = Cast_String_1(state_, TNode<Context>{p_context}, TNode<Object>{tmp18}, &label23);
    ca_.Goto(&block16, phi_bb10_12, phi_bb10_13, phi_bb10_14, phi_bb10_15);
    if (label23.is_used()) {
      ca_.Bind(&label23);
      ca_.Goto(&block17, phi_bb10_12, phi_bb10_13, phi_bb10_14, phi_bb10_15);
    }
  }

  TNode<FixedArray> phi_bb17_12;
  TNode<IntPtrT> phi_bb17_13;
  TNode<IntPtrT> phi_bb17_14;
  TNode<BoolT> phi_bb17_15;
  TNode<Number> tmp24;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_12, &phi_bb17_13, &phi_bb17_14, &phi_bb17_15);
    compiler::CodeAssemblerLabel label25(&ca_);
    tmp24 = Cast_Number_0(state_, TNode<Object>{ca_.UncheckedCast<Object>(tmp18)}, &label25);
    ca_.Goto(&block22, phi_bb17_12, phi_bb17_13, phi_bb17_14, phi_bb17_15);
    if (label25.is_used()) {
      ca_.Bind(&label25);
      ca_.Goto(&block23, phi_bb17_12, phi_bb17_13, phi_bb17_14, phi_bb17_15);
    }
  }

  TNode<FixedArray> phi_bb16_12;
  TNode<IntPtrT> phi_bb16_13;
  TNode<IntPtrT> phi_bb16_14;
  TNode<BoolT> phi_bb16_15;
  TNode<String> tmp26;
  TNode<BoolT> tmp27;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_12, &phi_bb16_13, &phi_bb16_14, &phi_bb16_15);
    tmp26 = kEmptyString_0(state_);
    tmp27 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp22}, TNode<HeapObject>{tmp26});
    ca_.Branch(tmp27, &block18, std::vector<compiler::Node*>{phi_bb16_12, phi_bb16_13, phi_bb16_14, phi_bb16_15}, &block19, std::vector<compiler::Node*>{phi_bb16_12, phi_bb16_13, phi_bb16_14, phi_bb16_15});
  }

  TNode<FixedArray> phi_bb18_12;
  TNode<IntPtrT> phi_bb18_13;
  TNode<IntPtrT> phi_bb18_14;
  TNode<BoolT> phi_bb18_15;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_12, &phi_bb18_13, &phi_bb18_14, &phi_bb18_15);
    ca_.Goto(&block4, phi_bb8_10, phi_bb6_11, phi_bb18_12, phi_bb18_13, phi_bb18_14, phi_bb18_15, tmp17);
  }

  TNode<FixedArray> phi_bb19_12;
  TNode<IntPtrT> phi_bb19_13;
  TNode<IntPtrT> phi_bb19_14;
  TNode<BoolT> phi_bb19_15;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_12, &phi_bb19_13, &phi_bb19_14, &phi_bb19_15);
    ca_.Goto(&block14, phi_bb19_12, phi_bb19_13, phi_bb19_14, phi_bb19_15, tmp22);
  }

  TNode<FixedArray> phi_bb23_12;
  TNode<IntPtrT> phi_bb23_13;
  TNode<IntPtrT> phi_bb23_14;
  TNode<BoolT> phi_bb23_15;
  TNode<BoolT> tmp28;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_12, &phi_bb23_13, &phi_bb23_14, &phi_bb23_15);
    tmp28 = CodeStubAssembler(state_).IsNullOrUndefined(TNode<Object>{ca_.UncheckedCast<HeapObject>(tmp18)});
    ca_.Branch(tmp28, &block24, std::vector<compiler::Node*>{phi_bb23_12, phi_bb23_13, phi_bb23_14, phi_bb23_15}, &block25, std::vector<compiler::Node*>{phi_bb23_12, phi_bb23_13, phi_bb23_14, phi_bb23_15});
  }

  TNode<FixedArray> phi_bb22_12;
  TNode<IntPtrT> phi_bb22_13;
  TNode<IntPtrT> phi_bb22_14;
  TNode<BoolT> phi_bb22_15;
  TNode<String> tmp29;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_12, &phi_bb22_13, &phi_bb22_14, &phi_bb22_15);
    tmp29 = CodeStubAssembler(state_).NumberToString(TNode<Number>{tmp24});
    ca_.Goto(&block20, phi_bb22_12, phi_bb22_13, phi_bb22_14, phi_bb22_15, tmp29);
  }

  TNode<FixedArray> phi_bb24_12;
  TNode<IntPtrT> phi_bb24_13;
  TNode<IntPtrT> phi_bb24_14;
  TNode<BoolT> phi_bb24_15;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_12, &phi_bb24_13, &phi_bb24_14, &phi_bb24_15);
    ca_.Goto(&block4, phi_bb8_10, phi_bb6_11, phi_bb24_12, phi_bb24_13, phi_bb24_14, phi_bb24_15, tmp17);
  }

  TNode<FixedArray> phi_bb25_12;
  TNode<IntPtrT> phi_bb25_13;
  TNode<IntPtrT> phi_bb25_14;
  TNode<BoolT> phi_bb25_15;
  TNode<String> tmp30;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_12, &phi_bb25_13, &phi_bb25_14, &phi_bb25_15);
    tmp30 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kToString), p_context, ca_.UncheckedCast<HeapObject>(tmp18));
    ca_.Goto(&block20, phi_bb25_12, phi_bb25_13, phi_bb25_14, phi_bb25_15, tmp30);
  }

  TNode<FixedArray> phi_bb20_12;
  TNode<IntPtrT> phi_bb20_13;
  TNode<IntPtrT> phi_bb20_14;
  TNode<BoolT> phi_bb20_15;
  TNode<String> phi_bb20_18;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_12, &phi_bb20_13, &phi_bb20_14, &phi_bb20_15, &phi_bb20_18);
    ca_.Goto(&block14, phi_bb20_12, phi_bb20_13, phi_bb20_14, phi_bb20_15, phi_bb20_18);
  }

  TNode<FixedArray> phi_bb14_12;
  TNode<IntPtrT> phi_bb14_13;
  TNode<IntPtrT> phi_bb14_14;
  TNode<BoolT> phi_bb14_15;
  TNode<String> phi_bb14_18;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_12, &phi_bb14_13, &phi_bb14_14, &phi_bb14_15, &phi_bb14_18);
    ca_.Goto(&block11, phi_bb14_12, phi_bb14_13, phi_bb14_14, phi_bb14_15, phi_bb14_18);
  }

  TNode<FixedArray> phi_bb11_12;
  TNode<IntPtrT> phi_bb11_13;
  TNode<IntPtrT> phi_bb11_14;
  TNode<BoolT> phi_bb11_15;
  TNode<String> phi_bb11_18;
  TNode<IntPtrT> tmp31;
  TNode<BoolT> tmp32;
  TNode<IntPtrT> tmp33;
  TNode<BoolT> tmp34;
  TNode<BoolT> tmp35;
  TNode<IntPtrT> tmp36;
  TNode<BoolT> tmp37;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_12, &phi_bb11_13, &phi_bb11_14, &phi_bb11_15, &phi_bb11_18);
    tmp31 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp32 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb11_13}, TNode<IntPtrT>{tmp31});
    tmp33 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp34 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{phi_bb8_10}, TNode<IntPtrT>{tmp33});
    tmp35 = CodeStubAssembler(state_).Word32Or(TNode<BoolT>{tmp32}, TNode<BoolT>{tmp34});
    tmp36 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp37 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb8_10}, TNode<IntPtrT>{tmp36});
    ca_.Branch(tmp37, &block30, std::vector<compiler::Node*>{phi_bb11_12, phi_bb11_13, phi_bb11_14, phi_bb11_15, phi_bb11_18, phi_bb11_18, phi_bb11_18}, &block31, std::vector<compiler::Node*>{phi_bb11_12, phi_bb11_13, phi_bb11_14, phi_bb11_15, phi_bb11_18, phi_bb11_18, phi_bb11_18});
  }

  TNode<FixedArray> phi_bb30_12;
  TNode<IntPtrT> phi_bb30_13;
  TNode<IntPtrT> phi_bb30_14;
  TNode<BoolT> phi_bb30_15;
  TNode<String> phi_bb30_18;
  TNode<String> phi_bb30_19;
  TNode<String> phi_bb30_24;
  TNode<BoolT> tmp38;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_12, &phi_bb30_13, &phi_bb30_14, &phi_bb30_15, &phi_bb30_18, &phi_bb30_19, &phi_bb30_24);
    tmp38 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block32, phi_bb30_12, phi_bb30_13, phi_bb30_14, phi_bb30_15, phi_bb30_18, phi_bb30_19, phi_bb30_24, tmp38);
  }

  TNode<FixedArray> phi_bb31_12;
  TNode<IntPtrT> phi_bb31_13;
  TNode<IntPtrT> phi_bb31_14;
  TNode<BoolT> phi_bb31_15;
  TNode<String> phi_bb31_18;
  TNode<String> phi_bb31_19;
  TNode<String> phi_bb31_24;
  TNode<IntPtrT> tmp39;
  TNode<BoolT> tmp40;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_12, &phi_bb31_13, &phi_bb31_14, &phi_bb31_15, &phi_bb31_18, &phi_bb31_19, &phi_bb31_24);
    tmp39 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp40 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp39});
    ca_.Goto(&block32, phi_bb31_12, phi_bb31_13, phi_bb31_14, phi_bb31_15, phi_bb31_18, phi_bb31_19, phi_bb31_24, tmp40);
  }

  TNode<FixedArray> phi_bb32_12;
  TNode<IntPtrT> phi_bb32_13;
  TNode<IntPtrT> phi_bb32_14;
  TNode<BoolT> phi_bb32_15;
  TNode<String> phi_bb32_18;
  TNode<String> phi_bb32_19;
  TNode<String> phi_bb32_24;
  TNode<BoolT> phi_bb32_37;
  if (block32.is_used()) {
    ca_.Bind(&block32, &phi_bb32_12, &phi_bb32_13, &phi_bb32_14, &phi_bb32_15, &phi_bb32_18, &phi_bb32_19, &phi_bb32_24, &phi_bb32_37);
    ca_.Branch(phi_bb32_37, &block28, std::vector<compiler::Node*>{phi_bb32_12, phi_bb32_13, phi_bb32_14, phi_bb32_15, phi_bb32_18, phi_bb32_19, phi_bb32_24}, &block29, std::vector<compiler::Node*>{phi_bb32_12, phi_bb32_13, phi_bb32_14, phi_bb32_15, phi_bb32_18, phi_bb32_19, phi_bb32_24});
  }

  TNode<FixedArray> phi_bb28_12;
  TNode<IntPtrT> phi_bb28_13;
  TNode<IntPtrT> phi_bb28_14;
  TNode<BoolT> phi_bb28_15;
  TNode<String> phi_bb28_18;
  TNode<String> phi_bb28_19;
  TNode<String> phi_bb28_24;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_12, &phi_bb28_13, &phi_bb28_14, &phi_bb28_15, &phi_bb28_18, &phi_bb28_19, &phi_bb28_24);
    ca_.Goto(&block27, phi_bb28_12, phi_bb28_13, phi_bb28_14, phi_bb28_15, phi_bb28_18, phi_bb28_19, phi_bb28_24);
  }

  TNode<FixedArray> phi_bb29_12;
  TNode<IntPtrT> phi_bb29_13;
  TNode<IntPtrT> phi_bb29_14;
  TNode<BoolT> phi_bb29_15;
  TNode<String> phi_bb29_18;
  TNode<String> phi_bb29_19;
  TNode<String> phi_bb29_24;
  TNode<IntPtrT> tmp41;
  TNode<IntPtrT> tmp42;
  TNode<BoolT> tmp43;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_12, &phi_bb29_13, &phi_bb29_14, &phi_bb29_15, &phi_bb29_18, &phi_bb29_19, &phi_bb29_24);
    tmp41 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{phi_bb8_10});
    tmp42 = CodeStubAssembler(state_).IntPtrDiv(TNode<IntPtrT>{tmp41}, TNode<IntPtrT>{tmp3});
    tmp43 = CodeStubAssembler(state_).WordNotEqual(TNode<IntPtrT>{tmp42}, TNode<IntPtrT>{phi_bb8_10});
    ca_.Branch(tmp43, &block33, std::vector<compiler::Node*>{phi_bb29_12, phi_bb29_13, phi_bb29_14, phi_bb29_15, phi_bb29_18, phi_bb29_19, phi_bb29_24}, &block34, std::vector<compiler::Node*>{phi_bb29_12, phi_bb29_13, phi_bb29_14, phi_bb29_15, phi_bb29_18, phi_bb29_19, phi_bb29_24});
  }

  TNode<FixedArray> phi_bb33_12;
  TNode<IntPtrT> phi_bb33_13;
  TNode<IntPtrT> phi_bb33_14;
  TNode<BoolT> phi_bb33_15;
  TNode<String> phi_bb33_18;
  TNode<String> phi_bb33_19;
  TNode<String> phi_bb33_24;
  if (block33.is_used()) {
    ca_.Bind(&block33, &phi_bb33_12, &phi_bb33_13, &phi_bb33_14, &phi_bb33_15, &phi_bb33_18, &phi_bb33_19, &phi_bb33_24);
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowInvalidStringLength, p_context);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<FixedArray> phi_bb34_12;
  TNode<IntPtrT> phi_bb34_13;
  TNode<IntPtrT> phi_bb34_14;
  TNode<BoolT> phi_bb34_15;
  TNode<String> phi_bb34_18;
  TNode<String> phi_bb34_19;
  TNode<String> phi_bb34_24;
  TNode<IntPtrT> tmp44;
  if (block34.is_used()) {
    ca_.Bind(&block34, &phi_bb34_12, &phi_bb34_13, &phi_bb34_14, &phi_bb34_15, &phi_bb34_18, &phi_bb34_19, &phi_bb34_24);
    tmp44 = AddStringLength_0(state_, TNode<Context>{p_context}, TNode<IntPtrT>{phi_bb34_14}, TNode<IntPtrT>{tmp41});
    ca_.Branch(tmp35, &block35, std::vector<compiler::Node*>{phi_bb34_12, phi_bb34_13, phi_bb34_15, phi_bb34_18, phi_bb34_19, phi_bb34_24}, &block36, std::vector<compiler::Node*>{phi_bb34_12, phi_bb34_13, phi_bb34_15, phi_bb34_18, phi_bb34_19, phi_bb34_24});
  }

  TNode<FixedArray> phi_bb35_12;
  TNode<IntPtrT> phi_bb35_13;
  TNode<BoolT> phi_bb35_15;
  TNode<String> phi_bb35_18;
  TNode<String> phi_bb35_19;
  TNode<String> phi_bb35_24;
  TNode<IntPtrT> tmp45;
  TNode<IntPtrT> tmp46;
  TNode<Smi> tmp47;
  TNode<FixedArray> tmp48;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_12, &phi_bb35_13, &phi_bb35_15, &phi_bb35_18, &phi_bb35_19, &phi_bb35_24);
    tmp45 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp46 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb35_13}, TNode<IntPtrT>{tmp45});
    tmp47 = Convert_Smi_intptr_0(state_, TNode<IntPtrT>{phi_bb8_10});
    tmp48 = StoreAndGrowFixedArray_Smi_0(state_, TNode<FixedArray>{phi_bb35_12}, TNode<IntPtrT>{phi_bb35_13}, TNode<Smi>{tmp47});
    ca_.Goto(&block36, tmp48, tmp46, phi_bb35_15, phi_bb35_18, phi_bb35_19, phi_bb35_24);
  }

  TNode<FixedArray> phi_bb36_12;
  TNode<IntPtrT> phi_bb36_13;
  TNode<BoolT> phi_bb36_15;
  TNode<String> phi_bb36_18;
  TNode<String> phi_bb36_19;
  TNode<String> phi_bb36_24;
  if (block36.is_used()) {
    ca_.Bind(&block36, &phi_bb36_12, &phi_bb36_13, &phi_bb36_15, &phi_bb36_18, &phi_bb36_19, &phi_bb36_24);
    ca_.Goto(&block27, phi_bb36_12, phi_bb36_13, tmp44, phi_bb36_15, phi_bb36_18, phi_bb36_19, phi_bb36_24);
  }

  TNode<FixedArray> phi_bb27_12;
  TNode<IntPtrT> phi_bb27_13;
  TNode<IntPtrT> phi_bb27_14;
  TNode<BoolT> phi_bb27_15;
  TNode<String> phi_bb27_18;
  TNode<String> phi_bb27_19;
  TNode<String> phi_bb27_24;
  TNode<IntPtrT> tmp49;
  TNode<IntPtrT> tmp50;
  TNode<IntPtrT> tmp51;
  TNode<IntPtrT> tmp52;
  TNode<FixedArray> tmp53;
  TNode<Uint16T> tmp54;
  TNode<BoolT> tmp55;
  TNode<BoolT> tmp56;
  TNode<IntPtrT> tmp57;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_12, &phi_bb27_13, &phi_bb27_14, &phi_bb27_15, &phi_bb27_18, &phi_bb27_19, &phi_bb27_24);
    tmp49 = CodeStubAssembler(state_).LoadStringLengthAsWord(TNode<String>{phi_bb27_24});
    tmp50 = AddStringLength_0(state_, TNode<Context>{p_context}, TNode<IntPtrT>{phi_bb27_14}, TNode<IntPtrT>{tmp49});
    tmp51 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp52 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb27_13}, TNode<IntPtrT>{tmp51});
    tmp53 = StoreAndGrowFixedArray_String_0(state_, TNode<FixedArray>{phi_bb27_12}, TNode<IntPtrT>{phi_bb27_13}, TNode<String>{phi_bb27_24});
    tmp54 = CodeStubAssembler(state_).LoadInstanceType(TNode<HeapObject>{phi_bb27_24});
    tmp55 = CodeStubAssembler(state_).IsOneByteStringInstanceType(TNode<Uint16T>{tmp54});
    tmp56 = CodeStubAssembler(state_).Word32And(TNode<BoolT>{tmp55}, TNode<BoolT>{phi_bb27_15});
    tmp57 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block4, tmp57, phi_bb6_11, tmp53, tmp52, tmp50, tmp56, tmp17);
  }

  TNode<IntPtrT> phi_bb3_10;
  TNode<BuiltinPtr> phi_bb3_11;
  TNode<FixedArray> phi_bb3_12;
  TNode<IntPtrT> phi_bb3_13;
  TNode<IntPtrT> phi_bb3_14;
  TNode<BoolT> phi_bb3_15;
  TNode<UintPtrT> phi_bb3_16;
  TNode<BoolT> tmp58;
  TNode<IntPtrT> tmp59;
  TNode<BoolT> tmp60;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_10, &phi_bb3_11, &phi_bb3_12, &phi_bb3_13, &phi_bb3_14, &phi_bb3_15, &phi_bb3_16);
    tmp58 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    tmp59 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp60 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb3_10}, TNode<IntPtrT>{tmp59});
    ca_.Branch(tmp60, &block40, std::vector<compiler::Node*>{phi_bb3_10, phi_bb3_11, phi_bb3_12, phi_bb3_13, phi_bb3_14, phi_bb3_15, phi_bb3_16, phi_bb3_10, phi_bb3_10}, &block41, std::vector<compiler::Node*>{phi_bb3_10, phi_bb3_11, phi_bb3_12, phi_bb3_13, phi_bb3_14, phi_bb3_15, phi_bb3_16, phi_bb3_10, phi_bb3_10});
  }

  TNode<IntPtrT> phi_bb40_10;
  TNode<BuiltinPtr> phi_bb40_11;
  TNode<FixedArray> phi_bb40_12;
  TNode<IntPtrT> phi_bb40_13;
  TNode<IntPtrT> phi_bb40_14;
  TNode<BoolT> phi_bb40_15;
  TNode<UintPtrT> phi_bb40_16;
  TNode<IntPtrT> phi_bb40_17;
  TNode<IntPtrT> phi_bb40_21;
  TNode<BoolT> tmp61;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_10, &phi_bb40_11, &phi_bb40_12, &phi_bb40_13, &phi_bb40_14, &phi_bb40_15, &phi_bb40_16, &phi_bb40_17, &phi_bb40_21);
    tmp61 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block42, phi_bb40_10, phi_bb40_11, phi_bb40_12, phi_bb40_13, phi_bb40_14, phi_bb40_15, phi_bb40_16, phi_bb40_17, phi_bb40_21, tmp61);
  }

  TNode<IntPtrT> phi_bb41_10;
  TNode<BuiltinPtr> phi_bb41_11;
  TNode<FixedArray> phi_bb41_12;
  TNode<IntPtrT> phi_bb41_13;
  TNode<IntPtrT> phi_bb41_14;
  TNode<BoolT> phi_bb41_15;
  TNode<UintPtrT> phi_bb41_16;
  TNode<IntPtrT> phi_bb41_17;
  TNode<IntPtrT> phi_bb41_21;
  TNode<IntPtrT> tmp62;
  TNode<BoolT> tmp63;
  if (block41.is_used()) {
    ca_.Bind(&block41, &phi_bb41_10, &phi_bb41_11, &phi_bb41_12, &phi_bb41_13, &phi_bb41_14, &phi_bb41_15, &phi_bb41_16, &phi_bb41_17, &phi_bb41_21);
    tmp62 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp63 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp62});
    ca_.Goto(&block42, phi_bb41_10, phi_bb41_11, phi_bb41_12, phi_bb41_13, phi_bb41_14, phi_bb41_15, phi_bb41_16, phi_bb41_17, phi_bb41_21, tmp63);
  }

  TNode<IntPtrT> phi_bb42_10;
  TNode<BuiltinPtr> phi_bb42_11;
  TNode<FixedArray> phi_bb42_12;
  TNode<IntPtrT> phi_bb42_13;
  TNode<IntPtrT> phi_bb42_14;
  TNode<BoolT> phi_bb42_15;
  TNode<UintPtrT> phi_bb42_16;
  TNode<IntPtrT> phi_bb42_17;
  TNode<IntPtrT> phi_bb42_21;
  TNode<BoolT> phi_bb42_25;
  if (block42.is_used()) {
    ca_.Bind(&block42, &phi_bb42_10, &phi_bb42_11, &phi_bb42_12, &phi_bb42_13, &phi_bb42_14, &phi_bb42_15, &phi_bb42_16, &phi_bb42_17, &phi_bb42_21, &phi_bb42_25);
    ca_.Branch(phi_bb42_25, &block38, std::vector<compiler::Node*>{phi_bb42_10, phi_bb42_11, phi_bb42_12, phi_bb42_13, phi_bb42_14, phi_bb42_15, phi_bb42_16, phi_bb42_17, phi_bb42_21}, &block39, std::vector<compiler::Node*>{phi_bb42_10, phi_bb42_11, phi_bb42_12, phi_bb42_13, phi_bb42_14, phi_bb42_15, phi_bb42_16, phi_bb42_17, phi_bb42_21});
  }

  TNode<IntPtrT> phi_bb38_10;
  TNode<BuiltinPtr> phi_bb38_11;
  TNode<FixedArray> phi_bb38_12;
  TNode<IntPtrT> phi_bb38_13;
  TNode<IntPtrT> phi_bb38_14;
  TNode<BoolT> phi_bb38_15;
  TNode<UintPtrT> phi_bb38_16;
  TNode<IntPtrT> phi_bb38_17;
  TNode<IntPtrT> phi_bb38_21;
  if (block38.is_used()) {
    ca_.Bind(&block38, &phi_bb38_10, &phi_bb38_11, &phi_bb38_12, &phi_bb38_13, &phi_bb38_14, &phi_bb38_15, &phi_bb38_16, &phi_bb38_17, &phi_bb38_21);
    ca_.Goto(&block37, phi_bb38_10, phi_bb38_11, phi_bb38_12, phi_bb38_13, phi_bb38_14, phi_bb38_15, phi_bb38_16, phi_bb38_17, phi_bb38_21);
  }

  TNode<IntPtrT> phi_bb39_10;
  TNode<BuiltinPtr> phi_bb39_11;
  TNode<FixedArray> phi_bb39_12;
  TNode<IntPtrT> phi_bb39_13;
  TNode<IntPtrT> phi_bb39_14;
  TNode<BoolT> phi_bb39_15;
  TNode<UintPtrT> phi_bb39_16;
  TNode<IntPtrT> phi_bb39_17;
  TNode<IntPtrT> phi_bb39_21;
  TNode<IntPtrT> tmp64;
  TNode<IntPtrT> tmp65;
  TNode<BoolT> tmp66;
  if (block39.is_used()) {
    ca_.Bind(&block39, &phi_bb39_10, &phi_bb39_11, &phi_bb39_12, &phi_bb39_13, &phi_bb39_14, &phi_bb39_15, &phi_bb39_16, &phi_bb39_17, &phi_bb39_21);
    tmp64 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{phi_bb39_21});
    tmp65 = CodeStubAssembler(state_).IntPtrDiv(TNode<IntPtrT>{tmp64}, TNode<IntPtrT>{tmp3});
    tmp66 = CodeStubAssembler(state_).WordNotEqual(TNode<IntPtrT>{tmp65}, TNode<IntPtrT>{phi_bb39_21});
    ca_.Branch(tmp66, &block43, std::vector<compiler::Node*>{phi_bb39_10, phi_bb39_11, phi_bb39_12, phi_bb39_13, phi_bb39_14, phi_bb39_15, phi_bb39_16, phi_bb39_17, phi_bb39_21, phi_bb39_21}, &block44, std::vector<compiler::Node*>{phi_bb39_10, phi_bb39_11, phi_bb39_12, phi_bb39_13, phi_bb39_14, phi_bb39_15, phi_bb39_16, phi_bb39_17, phi_bb39_21, phi_bb39_21});
  }

  TNode<IntPtrT> phi_bb43_10;
  TNode<BuiltinPtr> phi_bb43_11;
  TNode<FixedArray> phi_bb43_12;
  TNode<IntPtrT> phi_bb43_13;
  TNode<IntPtrT> phi_bb43_14;
  TNode<BoolT> phi_bb43_15;
  TNode<UintPtrT> phi_bb43_16;
  TNode<IntPtrT> phi_bb43_17;
  TNode<IntPtrT> phi_bb43_21;
  TNode<IntPtrT> phi_bb43_24;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_10, &phi_bb43_11, &phi_bb43_12, &phi_bb43_13, &phi_bb43_14, &phi_bb43_15, &phi_bb43_16, &phi_bb43_17, &phi_bb43_21, &phi_bb43_24);
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowInvalidStringLength, p_context);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb44_10;
  TNode<BuiltinPtr> phi_bb44_11;
  TNode<FixedArray> phi_bb44_12;
  TNode<IntPtrT> phi_bb44_13;
  TNode<IntPtrT> phi_bb44_14;
  TNode<BoolT> phi_bb44_15;
  TNode<UintPtrT> phi_bb44_16;
  TNode<IntPtrT> phi_bb44_17;
  TNode<IntPtrT> phi_bb44_21;
  TNode<IntPtrT> phi_bb44_24;
  TNode<IntPtrT> tmp67;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_10, &phi_bb44_11, &phi_bb44_12, &phi_bb44_13, &phi_bb44_14, &phi_bb44_15, &phi_bb44_16, &phi_bb44_17, &phi_bb44_21, &phi_bb44_24);
    tmp67 = AddStringLength_0(state_, TNode<Context>{p_context}, TNode<IntPtrT>{phi_bb44_14}, TNode<IntPtrT>{tmp64});
    ca_.Branch(tmp58, &block45, std::vector<compiler::Node*>{phi_bb44_10, phi_bb44_11, phi_bb44_12, phi_bb44_13, phi_bb44_15, phi_bb44_16, phi_bb44_17, phi_bb44_21, phi_bb44_24}, &block46, std::vector<compiler::Node*>{phi_bb44_10, phi_bb44_11, phi_bb44_12, phi_bb44_13, phi_bb44_15, phi_bb44_16, phi_bb44_17, phi_bb44_21, phi_bb44_24});
  }

  TNode<IntPtrT> phi_bb45_10;
  TNode<BuiltinPtr> phi_bb45_11;
  TNode<FixedArray> phi_bb45_12;
  TNode<IntPtrT> phi_bb45_13;
  TNode<BoolT> phi_bb45_15;
  TNode<UintPtrT> phi_bb45_16;
  TNode<IntPtrT> phi_bb45_17;
  TNode<IntPtrT> phi_bb45_21;
  TNode<IntPtrT> phi_bb45_24;
  TNode<IntPtrT> tmp68;
  TNode<IntPtrT> tmp69;
  TNode<Smi> tmp70;
  TNode<FixedArray> tmp71;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_10, &phi_bb45_11, &phi_bb45_12, &phi_bb45_13, &phi_bb45_15, &phi_bb45_16, &phi_bb45_17, &phi_bb45_21, &phi_bb45_24);
    tmp68 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp69 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb45_13}, TNode<IntPtrT>{tmp68});
    tmp70 = Convert_Smi_intptr_0(state_, TNode<IntPtrT>{phi_bb45_24});
    tmp71 = StoreAndGrowFixedArray_Smi_0(state_, TNode<FixedArray>{phi_bb45_12}, TNode<IntPtrT>{phi_bb45_13}, TNode<Smi>{tmp70});
    ca_.Goto(&block46, phi_bb45_10, phi_bb45_11, tmp71, tmp69, phi_bb45_15, phi_bb45_16, phi_bb45_17, phi_bb45_21, phi_bb45_24);
  }

  TNode<IntPtrT> phi_bb46_10;
  TNode<BuiltinPtr> phi_bb46_11;
  TNode<FixedArray> phi_bb46_12;
  TNode<IntPtrT> phi_bb46_13;
  TNode<BoolT> phi_bb46_15;
  TNode<UintPtrT> phi_bb46_16;
  TNode<IntPtrT> phi_bb46_17;
  TNode<IntPtrT> phi_bb46_21;
  TNode<IntPtrT> phi_bb46_24;
  if (block46.is_used()) {
    ca_.Bind(&block46, &phi_bb46_10, &phi_bb46_11, &phi_bb46_12, &phi_bb46_13, &phi_bb46_15, &phi_bb46_16, &phi_bb46_17, &phi_bb46_21, &phi_bb46_24);
    ca_.Goto(&block37, phi_bb46_10, phi_bb46_11, phi_bb46_12, phi_bb46_13, tmp67, phi_bb46_15, phi_bb46_16, phi_bb46_17, phi_bb46_21);
  }

  TNode<IntPtrT> phi_bb37_10;
  TNode<BuiltinPtr> phi_bb37_11;
  TNode<FixedArray> phi_bb37_12;
  TNode<IntPtrT> phi_bb37_13;
  TNode<IntPtrT> phi_bb37_14;
  TNode<BoolT> phi_bb37_15;
  TNode<UintPtrT> phi_bb37_16;
  TNode<IntPtrT> phi_bb37_17;
  TNode<IntPtrT> phi_bb37_21;
  TNode<String> tmp72;
  if (block37.is_used()) {
    ca_.Bind(&block37, &phi_bb37_10, &phi_bb37_11, &phi_bb37_12, &phi_bb37_13, &phi_bb37_14, &phi_bb37_15, &phi_bb37_16, &phi_bb37_17, &phi_bb37_21);
    tmp72 = BufferJoin_0(state_, TNode<Context>{p_context}, TorqueStructBuffer_0{TNode<FixedArray>{phi_bb37_12}, TNode<IntPtrT>{phi_bb37_13}, TNode<IntPtrT>{phi_bb37_14}, TNode<BoolT>{phi_bb37_15}}, TNode<String>{p_sep});
    ca_.Goto(&block47);
  }

    ca_.Bind(&block47);
  return TNode<String>{tmp72};
}


TF_BUILTIN(LoadJoinTypedElement_Int32Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_Int32Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_Float32Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_Float32Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_Float64Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_Float64Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_Uint8ClampedElements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_Uint8ClampedElements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_BigUint64Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_BigUint64Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_BigInt64Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_BigInt64Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_Uint8Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_Uint8Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_Int8Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_Int8Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_Uint16Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_Uint16Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_Int16Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_Int16Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TF_BUILTIN(LoadJoinTypedElement_Uint32Elements_0, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<UintPtrT> parameter2 = UncheckedParameter<UintPtrT>(Descriptor::kK);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  TNode<RawPtrT> tmp1;
  TNode<Numeric> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1});
    tmp1 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp2 = CodeStubAssembler(state_).LoadFixedTypedArrayElementAsTagged(TNode<RawPtrT>{tmp1}, TNode<UintPtrT>{parameter2}, (KindForArrayType_Uint32Elements_0(state_)));
    CodeStubAssembler(state_).Return(tmp2);
  }
}

TNode<String> ArrayJoinImpl_JSTypedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_receiver, TNode<String> p_sep, TNode<Number> p_lengthNumber, bool p_useToLocaleString, TNode<Object> p_locales, TNode<Object> p_options, TNode<BuiltinPtr> p_initialLoadFn) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block5(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String, BoolT> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block33(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, BoolT, String, String, String> block35(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, BoolT, String, String, String> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, BoolT, String, String, String> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, BoolT> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block39(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, IntPtrT> block43(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, IntPtrT> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, IntPtrT> block45(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT, IntPtrT> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT, BuiltinPtr, FixedArray, IntPtrT, IntPtrT, BoolT, UintPtrT, IntPtrT, IntPtrT> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Map> tmp1;
  TNode<UintPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<FixedArray> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<BoolT> tmp8;
  TNode<UintPtrT> tmp9;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{p_receiver, tmp0});
    tmp2 = Convert_uintptr_Number_0(state_, TNode<Number>{p_lengthNumber});
    tmp3 = CodeStubAssembler(state_).LoadStringLengthAsWord(TNode<String>{p_sep});
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    std::tie(tmp5, tmp6, tmp7, tmp8) = NewBuffer_0(state_, TNode<UintPtrT>{tmp2}, TNode<String>{p_sep}).Flatten();
    tmp9 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block4, tmp4, p_initialLoadFn, tmp5, tmp6, tmp7, tmp8, tmp9);
  }

  TNode<IntPtrT> phi_bb4_10;
  TNode<BuiltinPtr> phi_bb4_11;
  TNode<FixedArray> phi_bb4_12;
  TNode<IntPtrT> phi_bb4_13;
  TNode<IntPtrT> phi_bb4_14;
  TNode<BoolT> phi_bb4_15;
  TNode<UintPtrT> phi_bb4_16;
  TNode<BoolT> tmp10;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_10, &phi_bb4_11, &phi_bb4_12, &phi_bb4_13, &phi_bb4_14, &phi_bb4_15, &phi_bb4_16);
    tmp10 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb4_16}, TNode<UintPtrT>{tmp2});
    ca_.Branch(tmp10, &block2, std::vector<compiler::Node*>{phi_bb4_10, phi_bb4_11, phi_bb4_12, phi_bb4_13, phi_bb4_14, phi_bb4_15, phi_bb4_16}, &block3, std::vector<compiler::Node*>{phi_bb4_10, phi_bb4_11, phi_bb4_12, phi_bb4_13, phi_bb4_14, phi_bb4_15, phi_bb4_16});
  }

  TNode<IntPtrT> phi_bb2_10;
  TNode<BuiltinPtr> phi_bb2_11;
  TNode<FixedArray> phi_bb2_12;
  TNode<IntPtrT> phi_bb2_13;
  TNode<IntPtrT> phi_bb2_14;
  TNode<BoolT> phi_bb2_15;
  TNode<UintPtrT> phi_bb2_16;
  TNode<BoolT> tmp11;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_10, &phi_bb2_11, &phi_bb2_12, &phi_bb2_13, &phi_bb2_14, &phi_bb2_15, &phi_bb2_16);
    tmp11 = CannotUseSameArrayAccessor_JSTypedArray_0(state_, TNode<Context>{p_context}, TNode<BuiltinPtr>{phi_bb2_11}, TNode<JSReceiver>{p_receiver}, TNode<Map>{tmp1}, TNode<Number>{p_lengthNumber});
    ca_.Branch(tmp11, &block5, std::vector<compiler::Node*>{phi_bb2_10, phi_bb2_11, phi_bb2_12, phi_bb2_13, phi_bb2_14, phi_bb2_15, phi_bb2_16}, &block6, std::vector<compiler::Node*>{phi_bb2_10, phi_bb2_11, phi_bb2_12, phi_bb2_13, phi_bb2_14, phi_bb2_15, phi_bb2_16});
  }

  TNode<IntPtrT> phi_bb5_10;
  TNode<BuiltinPtr> phi_bb5_11;
  TNode<FixedArray> phi_bb5_12;
  TNode<IntPtrT> phi_bb5_13;
  TNode<IntPtrT> phi_bb5_14;
  TNode<BoolT> phi_bb5_15;
  TNode<UintPtrT> phi_bb5_16;
  if (block5.is_used()) {
    ca_.Bind(&block5, &phi_bb5_10, &phi_bb5_11, &phi_bb5_12, &phi_bb5_13, &phi_bb5_14, &phi_bb5_15, &phi_bb5_16);
    ca_.Goto(&block6, phi_bb5_10, ca_.UncheckedCast<BuiltinPtr>(ca_.SmiConstant(Builtin::kLoadJoinElement_GenericElementsAccessor_0)), phi_bb5_12, phi_bb5_13, phi_bb5_14, phi_bb5_15, phi_bb5_16);
  }

  TNode<IntPtrT> phi_bb6_10;
  TNode<BuiltinPtr> phi_bb6_11;
  TNode<FixedArray> phi_bb6_12;
  TNode<IntPtrT> phi_bb6_13;
  TNode<IntPtrT> phi_bb6_14;
  TNode<BoolT> phi_bb6_15;
  TNode<UintPtrT> phi_bb6_16;
  TNode<UintPtrT> tmp12;
  TNode<BoolT> tmp13;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_10, &phi_bb6_11, &phi_bb6_12, &phi_bb6_13, &phi_bb6_14, &phi_bb6_15, &phi_bb6_16);
    tmp12 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp13 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{phi_bb6_16}, TNode<UintPtrT>{tmp12});
    ca_.Branch(tmp13, &block7, std::vector<compiler::Node*>{phi_bb6_10, phi_bb6_12, phi_bb6_13, phi_bb6_14, phi_bb6_15, phi_bb6_16}, &block8, std::vector<compiler::Node*>{phi_bb6_10, phi_bb6_12, phi_bb6_13, phi_bb6_14, phi_bb6_15, phi_bb6_16});
  }

  TNode<IntPtrT> phi_bb7_10;
  TNode<FixedArray> phi_bb7_12;
  TNode<IntPtrT> phi_bb7_13;
  TNode<IntPtrT> phi_bb7_14;
  TNode<BoolT> phi_bb7_15;
  TNode<UintPtrT> phi_bb7_16;
  TNode<IntPtrT> tmp14;
  TNode<IntPtrT> tmp15;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_10, &phi_bb7_12, &phi_bb7_13, &phi_bb7_14, &phi_bb7_15, &phi_bb7_16);
    tmp14 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp15 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb7_10}, TNode<IntPtrT>{tmp14});
    ca_.Goto(&block8, tmp15, phi_bb7_12, phi_bb7_13, phi_bb7_14, phi_bb7_15, phi_bb7_16);
  }

  TNode<IntPtrT> phi_bb8_10;
  TNode<FixedArray> phi_bb8_12;
  TNode<IntPtrT> phi_bb8_13;
  TNode<IntPtrT> phi_bb8_14;
  TNode<BoolT> phi_bb8_15;
  TNode<UintPtrT> phi_bb8_16;
  TNode<UintPtrT> tmp16;
  TNode<UintPtrT> tmp17;
  TNode<Object> tmp18;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_10, &phi_bb8_12, &phi_bb8_13, &phi_bb8_14, &phi_bb8_15, &phi_bb8_16);
    tmp16 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp17 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb8_16}, TNode<UintPtrT>{tmp16});
tmp18 = CodeStubAssembler(state_).CallBuiltinPointer(Builtins::CallableFor(ca_.isolate(),ExampleBuiltinForTorqueFunctionPointerType(0)).descriptor(), phi_bb6_11, p_context, p_receiver, phi_bb8_16);
    if ((p_useToLocaleString)) {
      ca_.Goto(&block9, phi_bb8_12, phi_bb8_13, phi_bb8_14, phi_bb8_15);
    } else {
      ca_.Goto(&block10, phi_bb8_12, phi_bb8_13, phi_bb8_14, phi_bb8_15);
    }
  }

  TNode<FixedArray> phi_bb9_12;
  TNode<IntPtrT> phi_bb9_13;
  TNode<IntPtrT> phi_bb9_14;
  TNode<BoolT> phi_bb9_15;
  TNode<String> tmp19;
  TNode<String> tmp20;
  TNode<BoolT> tmp21;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_12, &phi_bb9_13, &phi_bb9_14, &phi_bb9_15);
    tmp19 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kConvertToLocaleString), p_context, tmp18, p_locales, p_options);
    tmp20 = kEmptyString_0(state_);
    tmp21 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp19}, TNode<HeapObject>{tmp20});
    ca_.Branch(tmp21, &block12, std::vector<compiler::Node*>{phi_bb9_12, phi_bb9_13, phi_bb9_14, phi_bb9_15}, &block13, std::vector<compiler::Node*>{phi_bb9_12, phi_bb9_13, phi_bb9_14, phi_bb9_15});
  }

  TNode<FixedArray> phi_bb12_12;
  TNode<IntPtrT> phi_bb12_13;
  TNode<IntPtrT> phi_bb12_14;
  TNode<BoolT> phi_bb12_15;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_12, &phi_bb12_13, &phi_bb12_14, &phi_bb12_15);
    ca_.Goto(&block4, phi_bb8_10, phi_bb6_11, phi_bb12_12, phi_bb12_13, phi_bb12_14, phi_bb12_15, tmp17);
  }

  TNode<FixedArray> phi_bb13_12;
  TNode<IntPtrT> phi_bb13_13;
  TNode<IntPtrT> phi_bb13_14;
  TNode<BoolT> phi_bb13_15;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_12, &phi_bb13_13, &phi_bb13_14, &phi_bb13_15);
    ca_.Goto(&block11, phi_bb13_12, phi_bb13_13, phi_bb13_14, phi_bb13_15, tmp19);
  }

  TNode<FixedArray> phi_bb10_12;
  TNode<IntPtrT> phi_bb10_13;
  TNode<IntPtrT> phi_bb10_14;
  TNode<BoolT> phi_bb10_15;
  TNode<String> tmp22;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_12, &phi_bb10_13, &phi_bb10_14, &phi_bb10_15);
    compiler::CodeAssemblerLabel label23(&ca_);
    tmp22 = Cast_String_1(state_, TNode<Context>{p_context}, TNode<Object>{tmp18}, &label23);
    ca_.Goto(&block16, phi_bb10_12, phi_bb10_13, phi_bb10_14, phi_bb10_15);
    if (label23.is_used()) {
      ca_.Bind(&label23);
      ca_.Goto(&block17, phi_bb10_12, phi_bb10_13, phi_bb10_14, phi_bb10_15);
    }
  }

  TNode<FixedArray> phi_bb17_12;
  TNode<IntPtrT> phi_bb17_13;
  TNode<IntPtrT> phi_bb17_14;
  TNode<BoolT> phi_bb17_15;
  TNode<Number> tmp24;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_12, &phi_bb17_13, &phi_bb17_14, &phi_bb17_15);
    compiler::CodeAssemblerLabel label25(&ca_);
    tmp24 = Cast_Number_0(state_, TNode<Object>{ca_.UncheckedCast<Object>(tmp18)}, &label25);
    ca_.Goto(&block22, phi_bb17_12, phi_bb17_13, phi_bb17_14, phi_bb17_15);
    if (label25.is_used()) {
      ca_.Bind(&label25);
      ca_.Goto(&block23, phi_bb17_12, phi_bb17_13, phi_bb17_14, phi_bb17_15);
    }
  }

  TNode<FixedArray> phi_bb16_12;
  TNode<IntPtrT> phi_bb16_13;
  TNode<IntPtrT> phi_bb16_14;
  TNode<BoolT> phi_bb16_15;
  TNode<String> tmp26;
  TNode<BoolT> tmp27;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_12, &phi_bb16_13, &phi_bb16_14, &phi_bb16_15);
    tmp26 = kEmptyString_0(state_);
    tmp27 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp22}, TNode<HeapObject>{tmp26});
    ca_.Branch(tmp27, &block18, std::vector<compiler::Node*>{phi_bb16_12, phi_bb16_13, phi_bb16_14, phi_bb16_15}, &block19, std::vector<compiler::Node*>{phi_bb16_12, phi_bb16_13, phi_bb16_14, phi_bb16_15});
  }

  TNode<FixedArray> phi_bb18_12;
  TNode<IntPtrT> phi_bb18_13;
  TNode<IntPtrT> phi_bb18_14;
  TNode<BoolT> phi_bb18_15;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_12, &phi_bb18_13, &phi_bb18_14, &phi_bb18_15);
    ca_.Goto(&block4, phi_bb8_10, phi_bb6_11, phi_bb18_12, phi_bb18_13, phi_bb18_14, phi_bb18_15, tmp17);
  }

  TNode<FixedArray> phi_bb19_12;
  TNode<IntPtrT> phi_bb19_13;
  TNode<IntPtrT> phi_bb19_14;
  TNode<BoolT> phi_bb19_15;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_12, &phi_bb19_13, &phi_bb19_14, &phi_bb19_15);
    ca_.Goto(&block14, phi_bb19_12, phi_bb19_13, phi_bb19_14, phi_bb19_15, tmp22);
  }

  TNode<FixedArray> phi_bb23_12;
  TNode<IntPtrT> phi_bb23_13;
  TNode<IntPtrT> phi_bb23_14;
  TNode<BoolT> phi_bb23_15;
  TNode<BoolT> tmp28;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_12, &phi_bb23_13, &phi_bb23_14, &phi_bb23_15);
    tmp28 = CodeStubAssembler(state_).IsNullOrUndefined(TNode<Object>{ca_.UncheckedCast<HeapObject>(tmp18)});
    ca_.Branch(tmp28, &block24, std::vector<compiler::Node*>{phi_bb23_12, phi_bb23_13, phi_bb23_14, phi_bb23_15}, &block25, std::vector<compiler::Node*>{phi_bb23_12, phi_bb23_13, phi_bb23_14, phi_bb23_15});
  }

  TNode<FixedArray> phi_bb22_12;
  TNode<IntPtrT> phi_bb22_13;
  TNode<IntPtrT> phi_bb22_14;
  TNode<BoolT> phi_bb22_15;
  TNode<String> tmp29;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_12, &phi_bb22_13, &phi_bb22_14, &phi_bb22_15);
    tmp29 = CodeStubAssembler(state_).NumberToString(TNode<Number>{tmp24});
    ca_.Goto(&block20, phi_bb22_12, phi_bb22_13, phi_bb22_14, phi_bb22_15, tmp29);
  }

  TNode<FixedArray> phi_bb24_12;
  TNode<IntPtrT> phi_bb24_13;
  TNode<IntPtrT> phi_bb24_14;
  TNode<BoolT> phi_bb24_15;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_12, &phi_bb24_13, &phi_bb24_14, &phi_bb24_15);
    ca_.Goto(&block4, phi_bb8_10, phi_bb6_11, phi_bb24_12, phi_bb24_13, phi_bb24_14, phi_bb24_15, tmp17);
  }

  TNode<FixedArray> phi_bb25_12;
  TNode<IntPtrT> phi_bb25_13;
  TNode<IntPtrT> phi_bb25_14;
  TNode<BoolT> phi_bb25_15;
  TNode<String> tmp30;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_12, &phi_bb25_13, &phi_bb25_14, &phi_bb25_15);
    tmp30 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kToString), p_context, ca_.UncheckedCast<HeapObject>(tmp18));
    ca_.Goto(&block20, phi_bb25_12, phi_bb25_13, phi_bb25_14, phi_bb25_15, tmp30);
  }

  TNode<FixedArray> phi_bb20_12;
  TNode<IntPtrT> phi_bb20_13;
  TNode<IntPtrT> phi_bb20_14;
  TNode<BoolT> phi_bb20_15;
  TNode<String> phi_bb20_18;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_12, &phi_bb20_13, &phi_bb20_14, &phi_bb20_15, &phi_bb20_18);
    ca_.Goto(&block14, phi_bb20_12, phi_bb20_13, phi_bb20_14, phi_bb20_15, phi_bb20_18);
  }

  TNode<FixedArray> phi_bb14_12;
  TNode<IntPtrT> phi_bb14_13;
  TNode<IntPtrT> phi_bb14_14;
  TNode<BoolT> phi_bb14_15;
  TNode<String> phi_bb14_18;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_12, &phi_bb14_13, &phi_bb14_14, &phi_bb14_15, &phi_bb14_18);
    ca_.Goto(&block11, phi_bb14_12, phi_bb14_13, phi_bb14_14, phi_bb14_15, phi_bb14_18);
  }

  TNode<FixedArray> phi_bb11_12;
  TNode<IntPtrT> phi_bb11_13;
  TNode<IntPtrT> phi_bb11_14;
  TNode<BoolT> phi_bb11_15;
  TNode<String> phi_bb11_18;
  TNode<IntPtrT> tmp31;
  TNode<BoolT> tmp32;
  TNode<IntPtrT> tmp33;
  TNode<BoolT> tmp34;
  TNode<BoolT> tmp35;
  TNode<IntPtrT> tmp36;
  TNode<BoolT> tmp37;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_12, &phi_bb11_13, &phi_bb11_14, &phi_bb11_15, &phi_bb11_18);
    tmp31 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp32 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb11_13}, TNode<IntPtrT>{tmp31});
    tmp33 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp34 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{phi_bb8_10}, TNode<IntPtrT>{tmp33});
    tmp35 = CodeStubAssembler(state_).Word32Or(TNode<BoolT>{tmp32}, TNode<BoolT>{tmp34});
    tmp36 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp37 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb8_10}, TNode<IntPtrT>{tmp36});
    ca_.Branch(tmp37, &block30, std::vector<compiler::Node*>{phi_bb11_12, phi_bb11_13, phi_bb11_14, phi_bb11_15, phi_bb11_18, phi_bb11_18, phi_bb11_18}, &block31, std::vector<compiler::Node*>{phi_bb11_12, phi_bb11_13, phi_bb11_14, phi_bb11_15, phi_bb11_18, phi_bb11_18, phi_bb11_18});
  }

  TNode<FixedArray> phi_bb30_12;
  TNode<IntPtrT> phi_bb30_13;
  TNode<IntPtrT> phi_bb30_14;
  TNode<BoolT> phi_bb30_15;
  TNode<String> phi_bb30_18;
  TNode<String> phi_bb30_19;
  TNode<String> phi_bb30_24;
  TNode<BoolT> tmp38;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_12, &phi_bb30_13, &phi_bb30_14, &phi_bb30_15, &phi_bb30_18, &phi_bb30_19, &phi_bb30_24);
    tmp38 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block32, phi_bb30_12, phi_bb30_13, phi_bb30_14, phi_bb30_15, phi_bb30_18, phi_bb30_19, phi_bb30_24, tmp38);
  }

  TNode<FixedArray> phi_bb31_12;
  TNode<IntPtrT> phi_bb31_13;
  TNode<IntPtrT> phi_bb31_14;
  TNode<BoolT> phi_bb31_15;
  TNode<String> phi_bb31_18;
  TNode<String> phi_bb31_19;
  TNode<String> phi_bb31_24;
  TNode<IntPtrT> tmp39;
  TNode<BoolT> tmp40;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_12, &phi_bb31_13, &phi_bb31_14, &phi_bb31_15, &phi_bb31_18, &phi_bb31_19, &phi_bb31_24);
    tmp39 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp40 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp39});
    ca_.Goto(&block32, phi_bb31_12, phi_bb31_13, phi_bb31_14, phi_bb31_15, phi_bb31_18, phi_bb31_19, phi_bb31_24, tmp40);
  }

  TNode<FixedArray> phi_bb32_12;
  TNode<IntPtrT> phi_bb32_13;
  TNode<IntPtrT> phi_bb32_14;
  TNode<BoolT> phi_bb32_15;
  TNode<String> phi_bb32_18;
  TNode<String> phi_bb32_19;
  TNode<String> phi_bb32_24;
  TNode<BoolT> phi_bb32_37;
  if (block32.is_used()) {
    ca_.Bind(&block32, &phi_bb32_12, &phi_bb32_13, &phi_bb32_14, &phi_bb32_15, &phi_bb32_18, &phi_bb32_19, &phi_bb32_24, &phi_bb32_37);
    ca_.Branch(phi_bb32_37, &block28, std::vector<compiler::Node*>{phi_bb32_12, phi_bb32_13, phi_bb32_14, phi_bb32_15, phi_bb32_18, phi_bb32_19, phi_bb32_24}, &block29, std::vector<compiler::Node*>{phi_bb32_12, phi_bb32_13, phi_bb32_14, phi_bb32_15, phi_bb32_18, phi_bb32_19, phi_bb32_24});
  }

  TNode<FixedArray> phi_bb28_12;
  TNode<IntPtrT> phi_bb28_13;
  TNode<IntPtrT> phi_bb28_14;
  TNode<BoolT> phi_bb28_15;
  TNode<String> phi_bb28_18;
  TNode<String> phi_bb28_19;
  TNode<String> phi_bb28_24;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_12, &phi_bb28_13, &phi_bb28_14, &phi_bb28_15, &phi_bb28_18, &phi_bb28_19, &phi_bb28_24);
    ca_.Goto(&block27, phi_bb28_12, phi_bb28_13, phi_bb28_14, phi_bb28_15, phi_bb28_18, phi_bb28_19, phi_bb28_24);
  }

  TNode<FixedArray> phi_bb29_12;
  TNode<IntPtrT> phi_bb29_13;
  TNode<IntPtrT> phi_bb29_14;
  TNode<BoolT> phi_bb29_15;
  TNode<String> phi_bb29_18;
  TNode<String> phi_bb29_19;
  TNode<String> phi_bb29_24;
  TNode<IntPtrT> tmp41;
  TNode<IntPtrT> tmp42;
  TNode<BoolT> tmp43;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_12, &phi_bb29_13, &phi_bb29_14, &phi_bb29_15, &phi_bb29_18, &phi_bb29_19, &phi_bb29_24);
    tmp41 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{phi_bb8_10});
    tmp42 = CodeStubAssembler(state_).IntPtrDiv(TNode<IntPtrT>{tmp41}, TNode<IntPtrT>{tmp3});
    tmp43 = CodeStubAssembler(state_).WordNotEqual(TNode<IntPtrT>{tmp42}, TNode<IntPtrT>{phi_bb8_10});
    ca_.Branch(tmp43, &block33, std::vector<compiler::Node*>{phi_bb29_12, phi_bb29_13, phi_bb29_14, phi_bb29_15, phi_bb29_18, phi_bb29_19, phi_bb29_24}, &block34, std::vector<compiler::Node*>{phi_bb29_12, phi_bb29_13, phi_bb29_14, phi_bb29_15, phi_bb29_18, phi_bb29_19, phi_bb29_24});
  }

  TNode<FixedArray> phi_bb33_12;
  TNode<IntPtrT> phi_bb33_13;
  TNode<IntPtrT> phi_bb33_14;
  TNode<BoolT> phi_bb33_15;
  TNode<String> phi_bb33_18;
  TNode<String> phi_bb33_19;
  TNode<String> phi_bb33_24;
  if (block33.is_used()) {
    ca_.Bind(&block33, &phi_bb33_12, &phi_bb33_13, &phi_bb33_14, &phi_bb33_15, &phi_bb33_18, &phi_bb33_19, &phi_bb33_24);
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowInvalidStringLength, p_context);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<FixedArray> phi_bb34_12;
  TNode<IntPtrT> phi_bb34_13;
  TNode<IntPtrT> phi_bb34_14;
  TNode<BoolT> phi_bb34_15;
  TNode<String> phi_bb34_18;
  TNode<String> phi_bb34_19;
  TNode<String> phi_bb34_24;
  TNode<IntPtrT> tmp44;
  if (block34.is_used()) {
    ca_.Bind(&block34, &phi_bb34_12, &phi_bb34_13, &phi_bb34_14, &phi_bb34_15, &phi_bb34_18, &phi_bb34_19, &phi_bb34_24);
    tmp44 = AddStringLength_0(state_, TNode<Context>{p_context}, TNode<IntPtrT>{phi_bb34_14}, TNode<IntPtrT>{tmp41});
    ca_.Branch(tmp35, &block35, std::vector<compiler::Node*>{phi_bb34_12, phi_bb34_13, phi_bb34_15, phi_bb34_18, phi_bb34_19, phi_bb34_24}, &block36, std::vector<compiler::Node*>{phi_bb34_12, phi_bb34_13, phi_bb34_15, phi_bb34_18, phi_bb34_19, phi_bb34_24});
  }

  TNode<FixedArray> phi_bb35_12;
  TNode<IntPtrT> phi_bb35_13;
  TNode<BoolT> phi_bb35_15;
  TNode<String> phi_bb35_18;
  TNode<String> phi_bb35_19;
  TNode<String> phi_bb35_24;
  TNode<IntPtrT> tmp45;
  TNode<IntPtrT> tmp46;
  TNode<Smi> tmp47;
  TNode<FixedArray> tmp48;
  if (block35.is_used()) {
    ca_.Bind(&block35, &phi_bb35_12, &phi_bb35_13, &phi_bb35_15, &phi_bb35_18, &phi_bb35_19, &phi_bb35_24);
    tmp45 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp46 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb35_13}, TNode<IntPtrT>{tmp45});
    tmp47 = Convert_Smi_intptr_0(state_, TNode<IntPtrT>{phi_bb8_10});
    tmp48 = StoreAndGrowFixedArray_Smi_0(state_, TNode<FixedArray>{phi_bb35_12}, TNode<IntPtrT>{phi_bb35_13}, TNode<Smi>{tmp47});
    ca_.Goto(&block36, tmp48, tmp46, phi_bb35_15, phi_bb35_18, phi_bb35_19, phi_bb35_24);
  }

  TNode<FixedArray> phi_bb36_12;
  TNode<IntPtrT> phi_bb36_13;
  TNode<BoolT> phi_bb36_15;
  TNode<String> phi_bb36_18;
  TNode<String> phi_bb36_19;
  TNode<String> phi_bb36_24;
  if (block36.is_used()) {
    ca_.Bind(&block36, &phi_bb36_12, &phi_bb36_13, &phi_bb36_15, &phi_bb36_18, &phi_bb36_19, &phi_bb36_24);
    ca_.Goto(&block27, phi_bb36_12, phi_bb36_13, tmp44, phi_bb36_15, phi_bb36_18, phi_bb36_19, phi_bb36_24);
  }

  TNode<FixedArray> phi_bb27_12;
  TNode<IntPtrT> phi_bb27_13;
  TNode<IntPtrT> phi_bb27_14;
  TNode<BoolT> phi_bb27_15;
  TNode<String> phi_bb27_18;
  TNode<String> phi_bb27_19;
  TNode<String> phi_bb27_24;
  TNode<IntPtrT> tmp49;
  TNode<IntPtrT> tmp50;
  TNode<IntPtrT> tmp51;
  TNode<IntPtrT> tmp52;
  TNode<FixedArray> tmp53;
  TNode<Uint16T> tmp54;
  TNode<BoolT> tmp55;
  TNode<BoolT> tmp56;
  TNode<IntPtrT> tmp57;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_12, &phi_bb27_13, &phi_bb27_14, &phi_bb27_15, &phi_bb27_18, &phi_bb27_19, &phi_bb27_24);
    tmp49 = CodeStubAssembler(state_).LoadStringLengthAsWord(TNode<String>{phi_bb27_24});
    tmp50 = AddStringLength_0(state_, TNode<Context>{p_context}, TNode<IntPtrT>{phi_bb27_14}, TNode<IntPtrT>{tmp49});
    tmp51 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp52 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb27_13}, TNode<IntPtrT>{tmp51});
    tmp53 = StoreAndGrowFixedArray_String_0(state_, TNode<FixedArray>{phi_bb27_12}, TNode<IntPtrT>{phi_bb27_13}, TNode<String>{phi_bb27_24});
    tmp54 = CodeStubAssembler(state_).LoadInstanceType(TNode<HeapObject>{phi_bb27_24});
    tmp55 = CodeStubAssembler(state_).IsOneByteStringInstanceType(TNode<Uint16T>{tmp54});
    tmp56 = CodeStubAssembler(state_).Word32And(TNode<BoolT>{tmp55}, TNode<BoolT>{phi_bb27_15});
    tmp57 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block4, tmp57, phi_bb6_11, tmp53, tmp52, tmp50, tmp56, tmp17);
  }

  TNode<IntPtrT> phi_bb3_10;
  TNode<BuiltinPtr> phi_bb3_11;
  TNode<FixedArray> phi_bb3_12;
  TNode<IntPtrT> phi_bb3_13;
  TNode<IntPtrT> phi_bb3_14;
  TNode<BoolT> phi_bb3_15;
  TNode<UintPtrT> phi_bb3_16;
  TNode<BoolT> tmp58;
  TNode<IntPtrT> tmp59;
  TNode<BoolT> tmp60;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_10, &phi_bb3_11, &phi_bb3_12, &phi_bb3_13, &phi_bb3_14, &phi_bb3_15, &phi_bb3_16);
    tmp58 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    tmp59 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp60 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb3_10}, TNode<IntPtrT>{tmp59});
    ca_.Branch(tmp60, &block40, std::vector<compiler::Node*>{phi_bb3_10, phi_bb3_11, phi_bb3_12, phi_bb3_13, phi_bb3_14, phi_bb3_15, phi_bb3_16, phi_bb3_10, phi_bb3_10}, &block41, std::vector<compiler::Node*>{phi_bb3_10, phi_bb3_11, phi_bb3_12, phi_bb3_13, phi_bb3_14, phi_bb3_15, phi_bb3_16, phi_bb3_10, phi_bb3_10});
  }

  TNode<IntPtrT> phi_bb40_10;
  TNode<BuiltinPtr> phi_bb40_11;
  TNode<FixedArray> phi_bb40_12;
  TNode<IntPtrT> phi_bb40_13;
  TNode<IntPtrT> phi_bb40_14;
  TNode<BoolT> phi_bb40_15;
  TNode<UintPtrT> phi_bb40_16;
  TNode<IntPtrT> phi_bb40_17;
  TNode<IntPtrT> phi_bb40_21;
  TNode<BoolT> tmp61;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_10, &phi_bb40_11, &phi_bb40_12, &phi_bb40_13, &phi_bb40_14, &phi_bb40_15, &phi_bb40_16, &phi_bb40_17, &phi_bb40_21);
    tmp61 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block42, phi_bb40_10, phi_bb40_11, phi_bb40_12, phi_bb40_13, phi_bb40_14, phi_bb40_15, phi_bb40_16, phi_bb40_17, phi_bb40_21, tmp61);
  }

  TNode<IntPtrT> phi_bb41_10;
  TNode<BuiltinPtr> phi_bb41_11;
  TNode<FixedArray> phi_bb41_12;
  TNode<IntPtrT> phi_bb41_13;
  TNode<IntPtrT> phi_bb41_14;
  TNode<BoolT> phi_bb41_15;
  TNode<UintPtrT> phi_bb41_16;
  TNode<IntPtrT> phi_bb41_17;
  TNode<IntPtrT> phi_bb41_21;
  TNode<IntPtrT> tmp62;
  TNode<BoolT> tmp63;
  if (block41.is_used()) {
    ca_.Bind(&block41, &phi_bb41_10, &phi_bb41_11, &phi_bb41_12, &phi_bb41_13, &phi_bb41_14, &phi_bb41_15, &phi_bb41_16, &phi_bb41_17, &phi_bb41_21);
    tmp62 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp63 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp62});
    ca_.Goto(&block42, phi_bb41_10, phi_bb41_11, phi_bb41_12, phi_bb41_13, phi_bb41_14, phi_bb41_15, phi_bb41_16, phi_bb41_17, phi_bb41_21, tmp63);
  }

  TNode<IntPtrT> phi_bb42_10;
  TNode<BuiltinPtr> phi_bb42_11;
  TNode<FixedArray> phi_bb42_12;
  TNode<IntPtrT> phi_bb42_13;
  TNode<IntPtrT> phi_bb42_14;
  TNode<BoolT> phi_bb42_15;
  TNode<UintPtrT> phi_bb42_16;
  TNode<IntPtrT> phi_bb42_17;
  TNode<IntPtrT> phi_bb42_21;
  TNode<BoolT> phi_bb42_25;
  if (block42.is_used()) {
    ca_.Bind(&block42, &phi_bb42_10, &phi_bb42_11, &phi_bb42_12, &phi_bb42_13, &phi_bb42_14, &phi_bb42_15, &phi_bb42_16, &phi_bb42_17, &phi_bb42_21, &phi_bb42_25);
    ca_.Branch(phi_bb42_25, &block38, std::vector<compiler::Node*>{phi_bb42_10, phi_bb42_11, phi_bb42_12, phi_bb42_13, phi_bb42_14, phi_bb42_15, phi_bb42_16, phi_bb42_17, phi_bb42_21}, &block39, std::vector<compiler::Node*>{phi_bb42_10, phi_bb42_11, phi_bb42_12, phi_bb42_13, phi_bb42_14, phi_bb42_15, phi_bb42_16, phi_bb42_17, phi_bb42_21});
  }

  TNode<IntPtrT> phi_bb38_10;
  TNode<BuiltinPtr> phi_bb38_11;
  TNode<FixedArray> phi_bb38_12;
  TNode<IntPtrT> phi_bb38_13;
  TNode<IntPtrT> phi_bb38_14;
  TNode<BoolT> phi_bb38_15;
  TNode<UintPtrT> phi_bb38_16;
  TNode<IntPtrT> phi_bb38_17;
  TNode<IntPtrT> phi_bb38_21;
  if (block38.is_used()) {
    ca_.Bind(&block38, &phi_bb38_10, &phi_bb38_11, &phi_bb38_12, &phi_bb38_13, &phi_bb38_14, &phi_bb38_15, &phi_bb38_16, &phi_bb38_17, &phi_bb38_21);
    ca_.Goto(&block37, phi_bb38_10, phi_bb38_11, phi_bb38_12, phi_bb38_13, phi_bb38_14, phi_bb38_15, phi_bb38_16, phi_bb38_17, phi_bb38_21);
  }

  TNode<IntPtrT> phi_bb39_10;
  TNode<BuiltinPtr> phi_bb39_11;
  TNode<FixedArray> phi_bb39_12;
  TNode<IntPtrT> phi_bb39_13;
  TNode<IntPtrT> phi_bb39_14;
  TNode<BoolT> phi_bb39_15;
  TNode<UintPtrT> phi_bb39_16;
  TNode<IntPtrT> phi_bb39_17;
  TNode<IntPtrT> phi_bb39_21;
  TNode<IntPtrT> tmp64;
  TNode<IntPtrT> tmp65;
  TNode<BoolT> tmp66;
  if (block39.is_used()) {
    ca_.Bind(&block39, &phi_bb39_10, &phi_bb39_11, &phi_bb39_12, &phi_bb39_13, &phi_bb39_14, &phi_bb39_15, &phi_bb39_16, &phi_bb39_17, &phi_bb39_21);
    tmp64 = CodeStubAssembler(state_).IntPtrMul(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{phi_bb39_21});
    tmp65 = CodeStubAssembler(state_).IntPtrDiv(TNode<IntPtrT>{tmp64}, TNode<IntPtrT>{tmp3});
    tmp66 = CodeStubAssembler(state_).WordNotEqual(TNode<IntPtrT>{tmp65}, TNode<IntPtrT>{phi_bb39_21});
    ca_.Branch(tmp66, &block43, std::vector<compiler::Node*>{phi_bb39_10, phi_bb39_11, phi_bb39_12, phi_bb39_13, phi_bb39_14, phi_bb39_15, phi_bb39_16, phi_bb39_17, phi_bb39_21, phi_bb39_21}, &block44, std::vector<compiler::Node*>{phi_bb39_10, phi_bb39_11, phi_bb39_12, phi_bb39_13, phi_bb39_14, phi_bb39_15, phi_bb39_16, phi_bb39_17, phi_bb39_21, phi_bb39_21});
  }

  TNode<IntPtrT> phi_bb43_10;
  TNode<BuiltinPtr> phi_bb43_11;
  TNode<FixedArray> phi_bb43_12;
  TNode<IntPtrT> phi_bb43_13;
  TNode<IntPtrT> phi_bb43_14;
  TNode<BoolT> phi_bb43_15;
  TNode<UintPtrT> phi_bb43_16;
  TNode<IntPtrT> phi_bb43_17;
  TNode<IntPtrT> phi_bb43_21;
  TNode<IntPtrT> phi_bb43_24;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_10, &phi_bb43_11, &phi_bb43_12, &phi_bb43_13, &phi_bb43_14, &phi_bb43_15, &phi_bb43_16, &phi_bb43_17, &phi_bb43_21, &phi_bb43_24);
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowInvalidStringLength, p_context);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> phi_bb44_10;
  TNode<BuiltinPtr> phi_bb44_11;
  TNode<FixedArray> phi_bb44_12;
  TNode<IntPtrT> phi_bb44_13;
  TNode<IntPtrT> phi_bb44_14;
  TNode<BoolT> phi_bb44_15;
  TNode<UintPtrT> phi_bb44_16;
  TNode<IntPtrT> phi_bb44_17;
  TNode<IntPtrT> phi_bb44_21;
  TNode<IntPtrT> phi_bb44_24;
  TNode<IntPtrT> tmp67;
  if (block44.is_used()) {
    ca_.Bind(&block44, &phi_bb44_10, &phi_bb44_11, &phi_bb44_12, &phi_bb44_13, &phi_bb44_14, &phi_bb44_15, &phi_bb44_16, &phi_bb44_17, &phi_bb44_21, &phi_bb44_24);
    tmp67 = AddStringLength_0(state_, TNode<Context>{p_context}, TNode<IntPtrT>{phi_bb44_14}, TNode<IntPtrT>{tmp64});
    ca_.Branch(tmp58, &block45, std::vector<compiler::Node*>{phi_bb44_10, phi_bb44_11, phi_bb44_12, phi_bb44_13, phi_bb44_15, phi_bb44_16, phi_bb44_17, phi_bb44_21, phi_bb44_24}, &block46, std::vector<compiler::Node*>{phi_bb44_10, phi_bb44_11, phi_bb44_12, phi_bb44_13, phi_bb44_15, phi_bb44_16, phi_bb44_17, phi_bb44_21, phi_bb44_24});
  }

  TNode<IntPtrT> phi_bb45_10;
  TNode<BuiltinPtr> phi_bb45_11;
  TNode<FixedArray> phi_bb45_12;
  TNode<IntPtrT> phi_bb45_13;
  TNode<BoolT> phi_bb45_15;
  TNode<UintPtrT> phi_bb45_16;
  TNode<IntPtrT> phi_bb45_17;
  TNode<IntPtrT> phi_bb45_21;
  TNode<IntPtrT> phi_bb45_24;
  TNode<IntPtrT> tmp68;
  TNode<IntPtrT> tmp69;
  TNode<Smi> tmp70;
  TNode<FixedArray> tmp71;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_10, &phi_bb45_11, &phi_bb45_12, &phi_bb45_13, &phi_bb45_15, &phi_bb45_16, &phi_bb45_17, &phi_bb45_21, &phi_bb45_24);
    tmp68 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp69 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb45_13}, TNode<IntPtrT>{tmp68});
    tmp70 = Convert_Smi_intptr_0(state_, TNode<IntPtrT>{phi_bb45_24});
    tmp71 = StoreAndGrowFixedArray_Smi_0(state_, TNode<FixedArray>{phi_bb45_12}, TNode<IntPtrT>{phi_bb45_13}, TNode<Smi>{tmp70});
    ca_.Goto(&block46, phi_bb45_10, phi_bb45_11, tmp71, tmp69, phi_bb45_15, phi_bb45_16, phi_bb45_17, phi_bb45_21, phi_bb45_24);
  }

  TNode<IntPtrT> phi_bb46_10;
  TNode<BuiltinPtr> phi_bb46_11;
  TNode<FixedArray> phi_bb46_12;
  TNode<IntPtrT> phi_bb46_13;
  TNode<BoolT> phi_bb46_15;
  TNode<UintPtrT> phi_bb46_16;
  TNode<IntPtrT> phi_bb46_17;
  TNode<IntPtrT> phi_bb46_21;
  TNode<IntPtrT> phi_bb46_24;
  if (block46.is_used()) {
    ca_.Bind(&block46, &phi_bb46_10, &phi_bb46_11, &phi_bb46_12, &phi_bb46_13, &phi_bb46_15, &phi_bb46_16, &phi_bb46_17, &phi_bb46_21, &phi_bb46_24);
    ca_.Goto(&block37, phi_bb46_10, phi_bb46_11, phi_bb46_12, phi_bb46_13, tmp67, phi_bb46_15, phi_bb46_16, phi_bb46_17, phi_bb46_21);
  }

  TNode<IntPtrT> phi_bb37_10;
  TNode<BuiltinPtr> phi_bb37_11;
  TNode<FixedArray> phi_bb37_12;
  TNode<IntPtrT> phi_bb37_13;
  TNode<IntPtrT> phi_bb37_14;
  TNode<BoolT> phi_bb37_15;
  TNode<UintPtrT> phi_bb37_16;
  TNode<IntPtrT> phi_bb37_17;
  TNode<IntPtrT> phi_bb37_21;
  TNode<String> tmp72;
  if (block37.is_used()) {
    ca_.Bind(&block37, &phi_bb37_10, &phi_bb37_11, &phi_bb37_12, &phi_bb37_13, &phi_bb37_14, &phi_bb37_15, &phi_bb37_16, &phi_bb37_17, &phi_bb37_21);
    tmp72 = BufferJoin_0(state_, TNode<Context>{p_context}, TorqueStructBuffer_0{TNode<FixedArray>{phi_bb37_12}, TNode<IntPtrT>{phi_bb37_13}, TNode<IntPtrT>{phi_bb37_14}, TNode<BoolT>{phi_bb37_15}}, TNode<String>{p_sep});
    ca_.Goto(&block47);
  }

    ca_.Bind(&block47);
  return TNode<String>{tmp72};
}


TorqueStructReference_Undefined_OR_FixedArray_0 NativeContextSlot_Context_Undefined_OR_FixedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<IntPtrT> p_index) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    std::tie(tmp0, tmp1) = NativeContextSlot_Undefined_OR_FixedArray_0(state_, TNode<Context>{p_context}, TNode<IntPtrT>{p_index}).Flatten();
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TorqueStructReference_Undefined_OR_FixedArray_0{TNode<Object>{tmp0}, TNode<IntPtrT>{tmp1}, TorqueStructUnsafe_0{}};
}


TNode<FixedArray> StoreAndGrowFixedArray_JSReceiver_0(compiler::CodeAssemblerState* state_, TNode<FixedArray> p_fixedArray, TNode<IntPtrT> p_index, TNode<JSReceiver> p_element) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{p_fixedArray});
    tmp1 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{p_index}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp1, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<UintPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  TNode<BoolT> tmp7;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    std::tie(tmp2, tmp3, tmp4) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_fixedArray}).Flatten();
    tmp5 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp6 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp4});
    tmp7 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp5}, TNode<UintPtrT>{tmp6});
    ca_.Branch(tmp7, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp8 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp9 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp8});
    std::tie(tmp10, tmp11) = NewReference_Object_0(state_, TNode<Object>{tmp2}, TNode<IntPtrT>{tmp9}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp10, tmp11}, p_element);
    ca_.Goto(&block1, p_fixedArray);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<FixedArray> tmp14;
  TNode<Object> tmp15;
  TNode<IntPtrT> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<UintPtrT> tmp18;
  TNode<UintPtrT> tmp19;
  TNode<BoolT> tmp20;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp12 = CodeStubAssembler(state_).CalculateNewElementsCapacity(TNode<IntPtrT>{tmp0});
    tmp13 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp14 = ExtractFixedArray_0(state_, TNode<FixedArray>{p_fixedArray}, TNode<IntPtrT>{tmp13}, TNode<IntPtrT>{tmp0}, TNode<IntPtrT>{tmp12});
    std::tie(tmp15, tmp16, tmp17) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp14}).Flatten();
    tmp18 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp19 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp17});
    tmp20 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp18}, TNode<UintPtrT>{tmp19});
    ca_.Branch(tmp20, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp21;
  TNode<IntPtrT> tmp22;
  TNode<Object> tmp23;
  TNode<IntPtrT> tmp24;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp21 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp22 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp16}, TNode<IntPtrT>{tmp21});
    std::tie(tmp23, tmp24) = NewReference_Object_0(state_, TNode<Object>{tmp15}, TNode<IntPtrT>{tmp22}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp23, tmp24}, p_element);
    ca_.Goto(&block1, tmp14);
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<FixedArray> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block27);
  }

    ca_.Bind(&block27);
  return TNode<FixedArray>{phi_bb1_3};
}


TNode<Object> CycleProtectedArrayJoin_JSArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, bool p_useToLocaleString, TNode<JSReceiver> p_o, TNode<Number> p_len, TNode<Object> p_sepObj, TNode<Object> p_locales, TNode<Object> p_options) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block13(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Oddball> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = Undefined_0(state_);
    tmp1 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{p_sepObj}, TNode<HeapObject>{tmp0});
    ca_.Branch(tmp1, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp2;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, ",");
    ca_.Goto(&block4, tmp2);
  }

  TNode<String> tmp3;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{p_sepObj});
    ca_.Goto(&block4, tmp3);
  }

  TNode<String> phi_bb4_6;
  TNode<Number> tmp4;
  TNode<BoolT> tmp5;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_6);
    tmp4 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    tmp5 = NumberIsGreaterThan_0(state_, TNode<Number>{p_len}, TNode<Number>{tmp4});
    ca_.Branch(tmp5, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp6;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp6 = JoinStackPushInline_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_o});
    ca_.Goto(&block10, tmp6);
  }

  TNode<BoolT> tmp7;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp7 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block10, tmp7);
  }

  TNode<BoolT> phi_bb10_8;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_8);
    ca_.Branch(phi_bb10_8, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp8;
      TNode<Object> tmp10;
      TNode<Object> tmp12;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    compiler::CodeAssemblerExceptionHandlerLabel catch9__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch9__label);
    tmp8 = ArrayJoin_JSArray_0(state_, TNode<Context>{p_context}, p_useToLocaleString, TNode<JSReceiver>{p_o}, TNode<String>{phi_bb4_6}, TNode<Number>{p_len}, TNode<Object>{p_locales}, TNode<Object>{p_options});
    }
    if (catch9__label.is_used()) {
      compiler::CodeAssemblerLabel catch9_skip(&ca_);
      ca_.Goto(&catch9_skip);
      ca_.Bind(&catch9__label, &tmp10);
      ca_.Goto(&block14);
      ca_.Bind(&catch9_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch11__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch11__label);
    JoinStackPopInline_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_o});
    }
    if (catch11__label.is_used()) {
      compiler::CodeAssemblerLabel catch11_skip(&ca_);
      ca_.Goto(&catch11_skip);
      ca_.Bind(&catch11__label, &tmp12);
      ca_.Goto(&block15);
      ca_.Bind(&catch11_skip);
    }
    ca_.Goto(&block1, tmp8);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block13, tmp10);
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block13, tmp12);
  }

  TNode<Object> phi_bb13_7;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_7);
    JoinStackPopInline_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_o});
    CodeStubAssembler(state_).CallRuntime(Runtime::kReThrow, p_context, phi_bb13_7);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> tmp13;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp13 = kEmptyString_0(state_);
    ca_.Goto(&block1, tmp13);
  }

  TNode<Object> phi_bb1_6;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_6);
    ca_.Goto(&block16);
  }

    ca_.Bind(&block16);
  return TNode<Object>{phi_bb1_6};
}


TNode<Object> CycleProtectedArrayJoin_JSTypedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, bool p_useToLocaleString, TNode<JSReceiver> p_o, TNode<Number> p_len, TNode<Object> p_sepObj, TNode<Object> p_locales, TNode<Object> p_options) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block13(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Oddball> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = Undefined_0(state_);
    tmp1 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{p_sepObj}, TNode<HeapObject>{tmp0});
    ca_.Branch(tmp1, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp2;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, ",");
    ca_.Goto(&block4, tmp2);
  }

  TNode<String> tmp3;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{p_sepObj});
    ca_.Goto(&block4, tmp3);
  }

  TNode<String> phi_bb4_6;
  TNode<Number> tmp4;
  TNode<BoolT> tmp5;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_6);
    tmp4 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    tmp5 = NumberIsGreaterThan_0(state_, TNode<Number>{p_len}, TNode<Number>{tmp4});
    ca_.Branch(tmp5, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp6;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp6 = JoinStackPushInline_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_o});
    ca_.Goto(&block10, tmp6);
  }

  TNode<BoolT> tmp7;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp7 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block10, tmp7);
  }

  TNode<BoolT> phi_bb10_8;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_8);
    ca_.Branch(phi_bb10_8, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp8;
      TNode<Object> tmp10;
      TNode<Object> tmp12;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    compiler::CodeAssemblerExceptionHandlerLabel catch9__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch9__label);
    tmp8 = ArrayJoin_JSTypedArray_0(state_, TNode<Context>{p_context}, p_useToLocaleString, TNode<JSReceiver>{p_o}, TNode<String>{phi_bb4_6}, TNode<Number>{p_len}, TNode<Object>{p_locales}, TNode<Object>{p_options});
    }
    if (catch9__label.is_used()) {
      compiler::CodeAssemblerLabel catch9_skip(&ca_);
      ca_.Goto(&catch9_skip);
      ca_.Bind(&catch9__label, &tmp10);
      ca_.Goto(&block14);
      ca_.Bind(&catch9_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch11__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch11__label);
    JoinStackPopInline_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_o});
    }
    if (catch11__label.is_used()) {
      compiler::CodeAssemblerLabel catch11_skip(&ca_);
      ca_.Goto(&catch11_skip);
      ca_.Bind(&catch11__label, &tmp12);
      ca_.Goto(&block15);
      ca_.Bind(&catch11_skip);
    }
    ca_.Goto(&block1, tmp8);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block13, tmp10);
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block13, tmp12);
  }

  TNode<Object> phi_bb13_7;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_7);
    JoinStackPopInline_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_o});
    CodeStubAssembler(state_).CallRuntime(Runtime::kReThrow, p_context, phi_bb13_7);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<String> tmp13;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp13 = kEmptyString_0(state_);
    ca_.Goto(&block1, tmp13);
  }

  TNode<Object> phi_bb1_6;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_6);
    ca_.Goto(&block16);
  }

    ca_.Bind(&block16);
  return TNode<Object>{phi_bb1_6};
}


TNode<FixedArray> StoreAndGrowFixedArray_Smi_0(compiler::CodeAssemblerState* state_, TNode<FixedArray> p_fixedArray, TNode<IntPtrT> p_index, TNode<Smi> p_element) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{p_fixedArray});
    tmp1 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{p_index}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp1, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<UintPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  TNode<BoolT> tmp7;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    std::tie(tmp2, tmp3, tmp4) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_fixedArray}).Flatten();
    tmp5 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp6 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp4});
    tmp7 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp5}, TNode<UintPtrT>{tmp6});
    ca_.Branch(tmp7, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp8 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp9 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp8});
    std::tie(tmp10, tmp11) = NewReference_Object_0(state_, TNode<Object>{tmp2}, TNode<IntPtrT>{tmp9}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp10, tmp11}, p_element);
    ca_.Goto(&block1, p_fixedArray);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<FixedArray> tmp14;
  TNode<Object> tmp15;
  TNode<IntPtrT> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<UintPtrT> tmp18;
  TNode<UintPtrT> tmp19;
  TNode<BoolT> tmp20;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp12 = CodeStubAssembler(state_).CalculateNewElementsCapacity(TNode<IntPtrT>{tmp0});
    tmp13 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp14 = ExtractFixedArray_0(state_, TNode<FixedArray>{p_fixedArray}, TNode<IntPtrT>{tmp13}, TNode<IntPtrT>{tmp0}, TNode<IntPtrT>{tmp12});
    std::tie(tmp15, tmp16, tmp17) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp14}).Flatten();
    tmp18 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp19 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp17});
    tmp20 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp18}, TNode<UintPtrT>{tmp19});
    ca_.Branch(tmp20, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp21;
  TNode<IntPtrT> tmp22;
  TNode<Object> tmp23;
  TNode<IntPtrT> tmp24;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp21 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp22 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp16}, TNode<IntPtrT>{tmp21});
    std::tie(tmp23, tmp24) = NewReference_Object_0(state_, TNode<Object>{tmp15}, TNode<IntPtrT>{tmp22}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp23, tmp24}, p_element);
    ca_.Goto(&block1, tmp14);
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<FixedArray> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block27);
  }

    ca_.Bind(&block27);
  return TNode<FixedArray>{phi_bb1_3};
}


TNode<FixedArray> StoreAndGrowFixedArray_String_0(compiler::CodeAssemblerState* state_, TNode<FixedArray> p_fixedArray, TNode<IntPtrT> p_index, TNode<String> p_element) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadAndUntagFixedArrayBaseLength(TNode<FixedArrayBase>{p_fixedArray});
    tmp1 = CodeStubAssembler(state_).IntPtrLessThan(TNode<IntPtrT>{p_index}, TNode<IntPtrT>{tmp0});
    ca_.Branch(tmp1, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<UintPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  TNode<BoolT> tmp7;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    std::tie(tmp2, tmp3, tmp4) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{p_fixedArray}).Flatten();
    tmp5 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp6 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp4});
    tmp7 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp5}, TNode<UintPtrT>{tmp6});
    ca_.Branch(tmp7, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<IntPtrT> tmp11;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp8 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp9 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp3}, TNode<IntPtrT>{tmp8});
    std::tie(tmp10, tmp11) = NewReference_Object_0(state_, TNode<Object>{tmp2}, TNode<IntPtrT>{tmp9}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp10, tmp11}, p_element);
    ca_.Goto(&block1, p_fixedArray);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<IntPtrT> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<FixedArray> tmp14;
  TNode<Object> tmp15;
  TNode<IntPtrT> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<UintPtrT> tmp18;
  TNode<UintPtrT> tmp19;
  TNode<BoolT> tmp20;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp12 = CodeStubAssembler(state_).CalculateNewElementsCapacity(TNode<IntPtrT>{tmp0});
    tmp13 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp14 = ExtractFixedArray_0(state_, TNode<FixedArray>{p_fixedArray}, TNode<IntPtrT>{tmp13}, TNode<IntPtrT>{tmp0}, TNode<IntPtrT>{tmp12});
    std::tie(tmp15, tmp16, tmp17) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp14}).Flatten();
    tmp18 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{p_index});
    tmp19 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp17});
    tmp20 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp18}, TNode<UintPtrT>{tmp19});
    ca_.Branch(tmp20, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp21;
  TNode<IntPtrT> tmp22;
  TNode<Object> tmp23;
  TNode<IntPtrT> tmp24;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp21 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{p_index});
    tmp22 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp16}, TNode<IntPtrT>{tmp21});
    std::tie(tmp23, tmp24) = NewReference_Object_0(state_, TNode<Object>{tmp15}, TNode<IntPtrT>{tmp22}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp23, tmp24}, p_element);
    ca_.Goto(&block1, tmp14);
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<FixedArray> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block27);
  }

    ca_.Bind(&block27);
  return TNode<FixedArray>{phi_bb1_3};
}


} // namespace internal
} // namespace v8
