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
#include "torque-generated/src/builtins/string-repeat-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/string-repeat-tq-csa.h"
#include "torque-generated/src/objects/heap-number-tq-csa.h"

namespace v8 {
namespace internal {
const char* kBuiltinName_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  return "String.prototype.repeat";}

TF_BUILTIN(StringRepeat, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<String> parameter1 = UncheckedParameter<String>(Descriptor::kString);
  USE(parameter1);
  TNode<Smi> parameter2 = UncheckedParameter<Smi>(Descriptor::kCount);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, String, IntPtrT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, String, IntPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, String, IntPtrT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, String, IntPtrT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String, String, IntPtrT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<IntPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = Convert_intptr_Smi_0(state_, TNode<Smi>{parameter2});
    ca_.Goto(&block11, tmp0, parameter1, tmp1);
  }

  TNode<String> phi_bb11_3;
  TNode<String> phi_bb11_4;
  TNode<IntPtrT> phi_bb11_5;
  TNode<BoolT> tmp2;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_3, &phi_bb11_4, &phi_bb11_5);
    tmp2 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp2, &block9, std::vector<compiler::Node*>{phi_bb11_3, phi_bb11_4, phi_bb11_5}, &block10, std::vector<compiler::Node*>{phi_bb11_3, phi_bb11_4, phi_bb11_5});
  }

  TNode<String> phi_bb9_3;
  TNode<String> phi_bb9_4;
  TNode<IntPtrT> phi_bb9_5;
  TNode<IntPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<BoolT> tmp6;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_3, &phi_bb9_4, &phi_bb9_5);
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp4 = CodeStubAssembler(state_).WordAnd(TNode<IntPtrT>{phi_bb9_5}, TNode<IntPtrT>{tmp3});
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp6 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp4}, TNode<IntPtrT>{tmp5});
    ca_.Branch(tmp6, &block12, std::vector<compiler::Node*>{phi_bb9_3, phi_bb9_4, phi_bb9_5}, &block13, std::vector<compiler::Node*>{phi_bb9_3, phi_bb9_4, phi_bb9_5});
  }

  TNode<String> phi_bb12_3;
  TNode<String> phi_bb12_4;
  TNode<IntPtrT> phi_bb12_5;
  TNode<String> tmp7;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_3, &phi_bb12_4, &phi_bb12_5);
    tmp7 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{phi_bb12_3}, TNode<String>{phi_bb12_4});
    ca_.Goto(&block13, tmp7, phi_bb12_4, phi_bb12_5);
  }

  TNode<String> phi_bb13_3;
  TNode<String> phi_bb13_4;
  TNode<IntPtrT> phi_bb13_5;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<BoolT> tmp11;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_3, &phi_bb13_4, &phi_bb13_5);
    tmp8 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp9 = CodeStubAssembler(state_).WordSar(TNode<IntPtrT>{phi_bb13_5}, TNode<IntPtrT>{tmp8});
    tmp10 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp11 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{tmp9}, TNode<IntPtrT>{tmp10});
    ca_.Branch(tmp11, &block14, std::vector<compiler::Node*>{phi_bb13_4}, &block15, std::vector<compiler::Node*>{phi_bb13_4});
  }

  TNode<String> phi_bb14_4;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_4);
    ca_.Goto(&block10, phi_bb13_3, phi_bb14_4, tmp9);
  }

  TNode<String> phi_bb15_4;
  TNode<String> tmp12;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_4);
    tmp12 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{phi_bb15_4}, TNode<String>{phi_bb15_4});
    ca_.Goto(&block11, phi_bb13_3, tmp12, tmp9);
  }

  TNode<String> phi_bb10_3;
  TNode<String> phi_bb10_4;
  TNode<IntPtrT> phi_bb10_5;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_3, &phi_bb10_4, &phi_bb10_5);
    CodeStubAssembler(state_).Return(phi_bb10_3);
  }
}

TF_BUILTIN(StringPrototypeRepeat, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kCount);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<Number> tmp2;
  TNode<Smi> tmp3;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_String_constexpr_string_0(state_, kBuiltinName_0(state_));
    tmp1 = CodeStubAssembler(state_).ToThisString(TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<String>{tmp0});
    tmp2 = ToInteger_Inline_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter2});
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_Smi_0(state_, TNode<Object>{tmp2}, &label4);
    ca_.Goto(&block9);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block10);
    }
  }

  TNode<Float64T> tmp5;
  TNode<Float64T> tmp6;
  TNode<BoolT> tmp7;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp5 = LoadHeapNumberValue_0(state_, TNode<HeapNumber>{ca_.UncheckedCast<HeapNumber>(tmp2)});
    tmp6 = FromConstexpr_float64_constexpr_float64_0(state_, V8_INFINITY);
    tmp7 = CodeStubAssembler(state_).Float64Equal(TNode<Float64T>{tmp5}, TNode<Float64T>{tmp6});
    ca_.Branch(tmp7, &block26, std::vector<compiler::Node*>{}, &block27, std::vector<compiler::Node*>{});
  }

  TNode<Smi> tmp8;
  TNode<BoolT> tmp9;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp8 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp9 = CodeStubAssembler(state_).SmiLessThan(TNode<Smi>{tmp3}, TNode<Smi>{tmp8});
    ca_.Branch(tmp9, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  if (block11.is_used()) {
    ca_.Bind(&block11);
    ca_.Goto(&block4);
  }

  TNode<Smi> tmp10;
  TNode<BoolT> tmp11;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp10 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp11 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp3}, TNode<Smi>{tmp10});
    ca_.Branch(tmp11, &block15, std::vector<compiler::Node*>{}, &block16, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp12;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp12 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block17, tmp12);
  }

  TNode<Uint32T> tmp13;
  TNode<Uint32T> tmp14;
  TNode<BoolT> tmp15;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp13 = CodeStubAssembler(state_).LoadStringLengthAsWord32(TNode<String>{tmp1});
    tmp14 = FromConstexpr_uint32_constexpr_int31_0(state_, 0);
    tmp15 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{tmp13}, TNode<Uint32T>{tmp14});
    ca_.Goto(&block17, tmp15);
  }

  TNode<BoolT> phi_bb17_7;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_7);
    ca_.Branch(phi_bb17_7, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block6);
  }

  TNode<Smi> tmp16;
  TNode<BoolT> tmp17;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp16 = FromConstexpr_Smi_constexpr_int31_0(state_, String::kMaxLength);
    tmp17 = CodeStubAssembler(state_).SmiGreaterThan(TNode<Smi>{tmp3}, TNode<Smi>{tmp16});
    ca_.Branch(tmp17, &block18, std::vector<compiler::Node*>{}, &block19, std::vector<compiler::Node*>{});
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    ca_.Goto(&block2);
  }

  TNode<String> tmp18;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    tmp18 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kStringRepeat), parameter0, tmp1, tmp3);
    CodeStubAssembler(state_).Return(tmp18);
  }

  TNode<BoolT> tmp19;
  if (block26.is_used()) {
    ca_.Bind(&block26);
    tmp19 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block28, tmp19);
  }

  TNode<Float64T> tmp20;
  TNode<BoolT> tmp21;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp20 = FromConstexpr_float64_constexpr_int31_0(state_, 0);
    tmp21 = CodeStubAssembler(state_).Float64LessThan(TNode<Float64T>{tmp5}, TNode<Float64T>{tmp20});
    ca_.Goto(&block28, tmp21);
  }

  TNode<BoolT> phi_bb28_8;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_8);
    ca_.Branch(phi_bb28_8, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  if (block24.is_used()) {
    ca_.Bind(&block24);
    ca_.Goto(&block4);
  }

  TNode<Uint32T> tmp22;
  TNode<Uint32T> tmp23;
  TNode<BoolT> tmp24;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp22 = CodeStubAssembler(state_).LoadStringLengthAsWord32(TNode<String>{tmp1});
    tmp23 = FromConstexpr_uint32_constexpr_int31_0(state_, 0);
    tmp24 = CodeStubAssembler(state_).Word32Equal(TNode<Uint32T>{tmp22}, TNode<Uint32T>{tmp23});
    ca_.Branch(tmp24, &block29, std::vector<compiler::Node*>{}, &block30, std::vector<compiler::Node*>{});
  }

  if (block29.is_used()) {
    ca_.Bind(&block29);
    ca_.Goto(&block6);
  }

  if (block30.is_used()) {
    ca_.Bind(&block30);
    ca_.Goto(&block2);
  }

  TNode<String> tmp25;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp25 = kEmptyString_0(state_);
    CodeStubAssembler(state_).Return(tmp25);
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{parameter0}, MessageTemplate::kInvalidCountValue, TNode<Object>{parameter2});
  }

  if (block2.is_used()) {
    ca_.Bind(&block2);
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowInvalidStringLength, parameter0);
    CodeStubAssembler(state_).Unreachable();
  }
}

} // namespace internal
} // namespace v8
