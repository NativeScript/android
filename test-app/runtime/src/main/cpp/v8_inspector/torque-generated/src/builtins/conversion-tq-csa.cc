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
#include "torque-generated/src/builtins/conversion-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/boolean-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/conversion-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/math-tq-csa.h"
#include "torque-generated/src/objects/contexts-tq-csa.h"
#include "torque-generated/src/objects/heap-number-tq-csa.h"
#include "torque-generated/src/objects/js-objects-tq-csa.h"
#include "torque-generated/src/objects/name-tq-csa.h"
#include "torque-generated/src/objects/oddball-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(StringToNumber, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<String> parameter1 = UncheckedParameter<String>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Number> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).StringToNumber(TNode<String>{parameter1});
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(NonNumberToNumber, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<HeapObject> parameter1 = UncheckedParameter<HeapObject>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Number> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).NonNumberToNumber(TNode<Context>{parameter0}, TNode<HeapObject>{parameter1});
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(NonNumberToNumeric, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<HeapObject> parameter1 = UncheckedParameter<HeapObject>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Numeric> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).NonNumberToNumeric(TNode<Context>{parameter0}, TNode<HeapObject>{parameter1});
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(ToNumeric, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Number> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_Number_0(state_, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  TNode<Numeric> tmp2;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp2 = ca_.CallStub<Numeric>(Builtins::CallableFor(ca_.isolate(), Builtin::kNonNumberToNumeric), parameter0, ca_.UncheckedCast<HeapObject>(parameter1));
    CodeStubAssembler(state_).Return(tmp2);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(NumberToString, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Number> parameter1 = UncheckedParameter<Number>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).NumberToString(TNode<Number>{parameter1});
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(ToBoolean, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Object> parameter0 = UncheckedParameter<Object>(Descriptor::kInput);
  USE(parameter0);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label0(&ca_);
    compiler::CodeAssemblerLabel label1(&ca_);
    CodeStubAssembler(state_).BranchIfToBooleanIsTrue(TNode<Object>{parameter0}, &label0, &label1);
    if (label0.is_used()) {
      ca_.Bind(&label0);
      ca_.Goto(&block5);
    }
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block6);
    }
  }

  TNode<Oddball> tmp2;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = CodeStubAssembler(state_).TrueConstant();
    CodeStubAssembler(state_).Return(tmp2);
  }

  TNode<Oddball> tmp3;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp3 = CodeStubAssembler(state_).FalseConstant();
    CodeStubAssembler(state_).Return(tmp3);
  }
}

TF_BUILTIN(ToBooleanForBaselineJump, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Object> parameter0 = UncheckedParameter<Object>(Descriptor::kInput);
  USE(parameter0);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label0(&ca_);
    compiler::CodeAssemblerLabel label1(&ca_);
    CodeStubAssembler(state_).BranchIfToBooleanIsTrue(TNode<Object>{parameter0}, &label0, &label1);
    if (label0.is_used()) {
      ca_.Bind(&label0);
      ca_.Goto(&block5);
    }
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block6);
    }
  }

  TNode<Smi> tmp2;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    CodeStubAssembler(state_).Return(parameter0, tmp2);
  }

  TNode<Smi> tmp3;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp3 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    CodeStubAssembler(state_).Return(parameter0, tmp3);
  }
}

TF_BUILTIN(ToLength, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object, Object> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object, Object> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    ca_.Goto(&block3, parameter1);
  }

  TNode<Object> phi_bb3_2;
  TNode<BoolT> tmp0;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_2);
    tmp0 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp0, &block1, std::vector<compiler::Node*>{phi_bb3_2}, &block2, std::vector<compiler::Node*>{phi_bb3_2});
  }

  TNode<Object> phi_bb1_2;
  TNode<Smi> tmp1;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_2);
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_Smi_0(state_, TNode<Object>{phi_bb1_2}, &label2);
    ca_.Goto(&block6, phi_bb1_2, phi_bb1_2, phi_bb1_2);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block7, phi_bb1_2, phi_bb1_2, phi_bb1_2);
    }
  }

  TNode<Object> phi_bb7_2;
  TNode<Object> phi_bb7_3;
  TNode<Object> phi_bb7_4;
  TNode<HeapNumber> tmp3;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_2, &phi_bb7_3, &phi_bb7_4);
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_HeapNumber_0(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(phi_bb7_3)}, &label4);
    ca_.Goto(&block12, phi_bb7_2, phi_bb7_3);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block13, phi_bb7_2, phi_bb7_3);
    }
  }

  TNode<Object> phi_bb6_2;
  TNode<Object> phi_bb6_3;
  TNode<Object> phi_bb6_4;
  TNode<Smi> tmp5;
  TNode<BoolT> tmp6;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_2, &phi_bb6_3, &phi_bb6_4);
    tmp5 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp6 = CodeStubAssembler(state_).SmiLessThan(TNode<Smi>{tmp1}, TNode<Smi>{tmp5});
    ca_.Branch(tmp6, &block8, std::vector<compiler::Node*>{phi_bb6_2, phi_bb6_3}, &block9, std::vector<compiler::Node*>{phi_bb6_2, phi_bb6_3});
  }

  TNode<Object> phi_bb8_2;
  TNode<Object> phi_bb8_3;
  TNode<Number> tmp7;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_2, &phi_bb8_3);
    tmp7 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    CodeStubAssembler(state_).Return(tmp7);
  }

  TNode<Object> phi_bb9_2;
  TNode<Object> phi_bb9_3;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_2, &phi_bb9_3);
    CodeStubAssembler(state_).Return(tmp1);
  }

  TNode<Object> phi_bb13_2;
  TNode<Object> phi_bb13_3;
  TNode<Number> tmp8;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_2, &phi_bb13_3);
    tmp8 = CodeStubAssembler(state_).NonNumberToNumber(TNode<Context>{parameter0}, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(phi_bb13_3)});
    ca_.Goto(&block3, tmp8);
  }

  TNode<Object> phi_bb12_2;
  TNode<Object> phi_bb12_3;
  TNode<Float64T> tmp9;
  TNode<Float64T> tmp10;
  TNode<BoolT> tmp11;
  TNode<BoolT> tmp12;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_2, &phi_bb12_3);
    tmp9 = Convert_float64_HeapNumber_0(state_, TNode<HeapNumber>{tmp3});
    tmp10 = FromConstexpr_float64_constexpr_int31_0(state_, 0);
    tmp11 = CodeStubAssembler(state_).Float64GreaterThan(TNode<Float64T>{tmp9}, TNode<Float64T>{tmp10});
    tmp12 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp11});
    ca_.Branch(tmp12, &block14, std::vector<compiler::Node*>{phi_bb12_2, phi_bb12_3}, &block15, std::vector<compiler::Node*>{phi_bb12_2, phi_bb12_3});
  }

  TNode<Object> phi_bb14_2;
  TNode<Object> phi_bb14_3;
  TNode<Number> tmp13;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_2, &phi_bb14_3);
    tmp13 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    CodeStubAssembler(state_).Return(tmp13);
  }

  TNode<Object> phi_bb15_2;
  TNode<Object> phi_bb15_3;
  TNode<Float64T> tmp14;
  TNode<BoolT> tmp15;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_2, &phi_bb15_3);
    tmp14 = FromConstexpr_float64_constexpr_float64_0(state_, kMaxSafeInteger);
    tmp15 = CodeStubAssembler(state_).Float64GreaterThan(TNode<Float64T>{tmp9}, TNode<Float64T>{tmp14});
    ca_.Branch(tmp15, &block16, std::vector<compiler::Node*>{phi_bb15_2, phi_bb15_3}, &block17, std::vector<compiler::Node*>{phi_bb15_2, phi_bb15_3});
  }

  TNode<Object> phi_bb16_2;
  TNode<Object> phi_bb16_3;
  TNode<Number> tmp16;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_2, &phi_bb16_3);
    tmp16 = FromConstexpr_Number_constexpr_float64_0(state_, kMaxSafeInteger);
    CodeStubAssembler(state_).Return(tmp16);
  }

  TNode<Object> phi_bb17_2;
  TNode<Object> phi_bb17_3;
  TNode<Float64T> tmp17;
  TNode<Number> tmp18;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_2, &phi_bb17_3);
    tmp17 = CodeStubAssembler(state_).Float64Floor(TNode<Float64T>{tmp9});
    tmp18 = CodeStubAssembler(state_).ChangeFloat64ToTagged(TNode<Float64T>{tmp17});
    CodeStubAssembler(state_).Return(tmp18);
  }

  TNode<Object> phi_bb2_2;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_2);
    VerifiedUnreachable_0(state_);
  }
}

TF_BUILTIN(ToName, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object, Object> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object, Object> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    ca_.Goto(&block3, parameter1);
  }

  TNode<Object> phi_bb3_2;
  TNode<BoolT> tmp0;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_2);
    tmp0 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp0, &block1, std::vector<compiler::Node*>{phi_bb3_2}, &block2, std::vector<compiler::Node*>{phi_bb3_2});
  }

  TNode<Object> phi_bb1_2;
  TNode<Name> tmp1;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_2);
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_Name_1(state_, TNode<Context>{parameter0}, TNode<Object>{phi_bb1_2}, &label2);
    ca_.Goto(&block6, phi_bb1_2, phi_bb1_2, phi_bb1_2);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block7, phi_bb1_2, phi_bb1_2, phi_bb1_2);
    }
  }

  TNode<Object> phi_bb7_2;
  TNode<Object> phi_bb7_3;
  TNode<Object> phi_bb7_4;
  TNode<Number> tmp3;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_2, &phi_bb7_3, &phi_bb7_4);
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_Number_0(state_, TNode<Object>{ca_.UncheckedCast<Object>(phi_bb7_3)}, &label4);
    ca_.Goto(&block10, phi_bb7_2, phi_bb7_3);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block11, phi_bb7_2, phi_bb7_3);
    }
  }

  TNode<Object> phi_bb6_2;
  TNode<Object> phi_bb6_3;
  TNode<Object> phi_bb6_4;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_2, &phi_bb6_3, &phi_bb6_4);
    CodeStubAssembler(state_).Return(tmp1);
  }

  TNode<Object> phi_bb11_2;
  TNode<Object> phi_bb11_3;
  TNode<BigInt> tmp5;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_2, &phi_bb11_3);
    compiler::CodeAssemblerLabel label6(&ca_);
    tmp5 = Cast_BigInt_0(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(phi_bb11_3)}, &label6);
    ca_.Goto(&block14, phi_bb11_2, phi_bb11_3);
    if (label6.is_used()) {
      ca_.Bind(&label6);
      ca_.Goto(&block15, phi_bb11_2, phi_bb11_3);
    }
  }

  TNode<Object> phi_bb10_2;
  TNode<Object> phi_bb10_3;
  TNode<String> tmp7;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_2, &phi_bb10_3);
    tmp7 = CodeStubAssembler(state_).NumberToString(TNode<Number>{tmp3});
    CodeStubAssembler(state_).Return(tmp7);
  }

  TNode<Object> phi_bb15_2;
  TNode<Object> phi_bb15_3;
  TNode<Oddball> tmp8;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_2, &phi_bb15_3);
    compiler::CodeAssemblerLabel label9(&ca_);
    tmp8 = Cast_Oddball_0(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(phi_bb15_3)}, &label9);
    ca_.Goto(&block18, phi_bb15_2, phi_bb15_3);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block19, phi_bb15_2, phi_bb15_3);
    }
  }

  TNode<Object> phi_bb14_2;
  TNode<Object> phi_bb14_3;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_2, &phi_bb14_3);
    CodeStubAssembler(state_).TailCallRuntime(Runtime::kToString, parameter0, tmp5);
  }

  TNode<Object> phi_bb19_2;
  TNode<Object> phi_bb19_3;
  TNode<Object> tmp10;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_2, &phi_bb19_3);
    tmp10 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kNonPrimitiveToPrimitive_String), parameter0, ca_.UncheckedCast<JSReceiver>(phi_bb19_3));
    ca_.Goto(&block3, tmp10);
  }

  TNode<Object> phi_bb18_2;
  TNode<Object> phi_bb18_3;
  TNode<IntPtrT> tmp11;
  TNode<String> tmp12;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_2, &phi_bb18_3);
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp12 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp8, tmp11});
    CodeStubAssembler(state_).Return(tmp12);
  }

  TNode<Object> phi_bb2_2;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_2);
    VerifiedUnreachable_0(state_);
  }
}

TF_BUILTIN(ToObject, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<IntPtrT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Smi> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_Smi_0(state_, TNode<Object>{parameter1}, &label1);
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
    tmp2 = Cast_JSReceiver_0(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(parameter1)}, &label3);
    ca_.Goto(&block9);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block10);
    }
  }

  TNode<IntPtrT> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp4 = NUMBER_FUNCTION_INDEX_0(state_);
    ca_.Goto(&block2, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<Map> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<Uint8T> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<BoolT> tmp11;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp6 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{ca_.UncheckedCast<PrimitiveHeapObject>(parameter1), tmp5});
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 5);
    tmp8 = CodeStubAssembler(state_).LoadReference<Uint8T>(CodeStubAssembler::Reference{tmp6, tmp7});
    tmp9 = Convert_intptr_uint8_0(state_, TNode<Uint8T>{tmp8});
    tmp10 = FromConstexpr_intptr_constexpr_int31_0(state_, Map::kNoConstructorFunctionIndex);
    tmp11 = CodeStubAssembler(state_).WordNotEqual(TNode<IntPtrT>{tmp9}, TNode<IntPtrT>{tmp10});
    ca_.Branch(tmp11, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  if (block9.is_used()) {
    ca_.Bind(&block9);
    CodeStubAssembler(state_).Return(tmp2);
  }

  TNode<IntPtrT> tmp12;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp12 = (TNode<IntPtrT>{tmp9});
    ca_.Goto(&block2, tmp12);
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kUndefinedOrNullToObject, "ToObject");
  }

  TNode<IntPtrT> phi_bb2_2;
  TNode<Object> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<JSFunction> tmp15;
  TNode<IntPtrT> tmp16;
  TNode<HeapObject> tmp17;
  TNode<Map> tmp18;
  TNode<JSObject> tmp19;
  TNode<JSPrimitiveWrapper> tmp20;
  TNode<IntPtrT> tmp21;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_2);
    std::tie(tmp13, tmp14) = NativeContextSlot_Context_JSFunction_0(state_, TNode<Context>{parameter0}, TNode<IntPtrT>{phi_bb2_2}).Flatten();
    tmp15 = CodeStubAssembler(state_).LoadReference<JSFunction>(CodeStubAssembler::Reference{tmp13, tmp14});
    tmp16 = FromConstexpr_intptr_constexpr_int31_0(state_, 28);
    tmp17 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{tmp15, tmp16});
    tmp18 = UnsafeCast_Map_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp17});
    tmp19 = AllocateFastOrSlowJSObjectFromMap_0(state_, TNode<Context>{parameter0}, TNode<Map>{tmp18});
    tmp20 = UnsafeCast_JSPrimitiveWrapper_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp19});
    tmp21 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp20, tmp21}, parameter1);
    CodeStubAssembler(state_).Return(tmp20);
  }
}

TNode<Object> TryGetExoticToPrimitive_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_input, compiler::CodeAssemblerLabel* label_OrdinaryToPrimitive) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Symbol> tmp0;
  TNode<Object> tmp1;
  TNode<BoolT> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).ToPrimitiveSymbolConstant();
    tmp1 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{p_input}, TNode<Object>{tmp0});
    tmp2 = CodeStubAssembler(state_).IsNullOrUndefined(TNode<Object>{tmp1});
    ca_.Branch(tmp2, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(label_OrdinaryToPrimitive);
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block5);
  }

    ca_.Bind(&block5);
  return TNode<Object>{tmp1};
}


TNode<Object> CallExoticToPrimitive_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_input, TNode<Object> p_exoticToPrimitive, TNode<String> p_hint) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<Object> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{p_exoticToPrimitive}, TNode<Object>{p_input}, TNode<Object>{p_hint});
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_JSPrimitive_0(state_, TNode<Object>{tmp0}, &label2);
    ca_.Goto(&block4);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kCannotConvertToPrimitive);
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<Object>{tmp1};
}


TF_BUILTIN(NonPrimitiveToPrimitive_Default, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = TryGetExoticToPrimitive_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  TNode<Object> tmp2;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp2 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kOrdinaryToPrimitive_Number), parameter0, parameter1);
    CodeStubAssembler(state_).Return(tmp2);
  }

  TNode<String> tmp3;
  TNode<Object> tmp4;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = CodeStubAssembler(state_).DefaultStringConstant();
    tmp4 = CallExoticToPrimitive_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{tmp0}, TNode<String>{tmp3});
    CodeStubAssembler(state_).Return(tmp4);
  }
}

TF_BUILTIN(NonPrimitiveToPrimitive_Number, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = TryGetExoticToPrimitive_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  TNode<Object> tmp2;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp2 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kOrdinaryToPrimitive_Number), parameter0, parameter1);
    CodeStubAssembler(state_).Return(tmp2);
  }

  TNode<String> tmp3;
  TNode<Object> tmp4;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = CodeStubAssembler(state_).NumberStringConstant();
    tmp4 = CallExoticToPrimitive_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{tmp0}, TNode<String>{tmp3});
    CodeStubAssembler(state_).Return(tmp4);
  }
}

TF_BUILTIN(NonPrimitiveToPrimitive_String, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSReceiver> parameter1 = UncheckedParameter<JSReceiver>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = TryGetExoticToPrimitive_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  TNode<Object> tmp2;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp2 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kOrdinaryToPrimitive_String), parameter0, parameter1);
    CodeStubAssembler(state_).Return(tmp2);
  }

  TNode<String> tmp3;
  TNode<Object> tmp4;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = CodeStubAssembler(state_).StringStringConstant();
    tmp4 = CallExoticToPrimitive_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{tmp0}, TNode<String>{tmp3});
    CodeStubAssembler(state_).Return(tmp4);
  }
}

TNode<Object> TryToPrimitiveMethod_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_input, TNode<String> p_name, compiler::CodeAssemblerLabel* label_Continue) {
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

  TNode<Object> tmp0;
  TNode<JSReceiver> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{p_input}, TNode<Object>{p_name});
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_Callable_1(state_, TNode<Context>{p_context}, TNode<Object>{tmp0}, &label2);
    ca_.Goto(&block5);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block1);
  }

  TNode<Object> tmp3;
  TNode<Object> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp3 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp0}, TNode<Object>{p_input});
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_JSPrimitive_0(state_, TNode<Object>{tmp3}, &label5);
    ca_.Goto(&block7);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block8);
    }
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
    ca_.Goto(label_Continue);
  }

    ca_.Bind(&block9);
  return TNode<Object>{tmp4};
}


TF_BUILTIN(OrdinaryToPrimitive_Number, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<Object> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).ValueOfStringConstant();
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = TryToPrimitiveMethod_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<String>{tmp0}, &label2);
    ca_.Goto(&block5);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block6);
    }
  }

  TNode<String> tmp3;
  TNode<Object> tmp4;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp3 = CodeStubAssembler(state_).ToStringStringConstant();
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = TryToPrimitiveMethod_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<String>{tmp3}, &label5);
    ca_.Goto(&block7);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block8);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    CodeStubAssembler(state_).Return(tmp1);
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCannotConvertToPrimitive);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Return(tmp4);
  }
}

TF_BUILTIN(OrdinaryToPrimitive_String, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kInput);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<Object> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).ToStringStringConstant();
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = TryToPrimitiveMethod_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<String>{tmp0}, &label2);
    ca_.Goto(&block5);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block6);
    }
  }

  TNode<String> tmp3;
  TNode<Object> tmp4;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp3 = CodeStubAssembler(state_).ValueOfStringConstant();
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = TryToPrimitiveMethod_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<String>{tmp3}, &label5);
    ca_.Goto(&block7);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block8);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    CodeStubAssembler(state_).Return(tmp1);
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCannotConvertToPrimitive);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).Return(tmp4);
  }
}

TNode<Name> Cast_Name_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o, compiler::CodeAssemblerLabel* label_CastError) {
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

  TNode<Name> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Name_0(state_, TNode<HeapObject>{tmp0}, &label3);
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
  return TNode<Name>{tmp2};
}


TNode<Map> UnsafeCast_Map_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Map> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<Map>{tmp0};
}


} // namespace internal
} // namespace v8
