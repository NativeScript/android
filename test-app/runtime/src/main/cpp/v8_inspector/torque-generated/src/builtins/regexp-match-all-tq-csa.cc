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
#include "torque-generated/src/builtins/regexp-match-all-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/boolean-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/promise-abstract-operations-tq-csa.h"
#include "torque-generated/src/builtins/regexp-match-all-tq-csa.h"
#include "torque-generated/src/builtins/regexp-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/js-regexp-string-iterator-tq-csa.h"
#include "torque-generated/src/objects/js-regexp-tq-csa.h"
#include "torque-generated/src/objects/string-tq-csa.h"

namespace v8 {
namespace internal {
TNode<Object> RegExpPrototypeMatchAllImpl_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<NativeContext> p_nativeContext, TNode<Object> p_receiver, TNode<Object> p_string) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, BoolT, BoolT> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  TNode<String> tmp1;
  TNode<JSRegExp> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    CodeStubAssembler(state_).ThrowIfNotJSReceiver(TNode<Context>{p_context}, TNode<Object>{p_receiver}, MessageTemplate::kIncompatibleMethodReceiver, "RegExp.prototype.@@matchAll");
    tmp0 = UnsafeCast_JSReceiver_0(state_, TNode<Context>{p_context}, TNode<Object>{p_receiver});
    tmp1 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{p_string});
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_FastJSRegExp_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{tmp0}, &label3);
    ca_.Goto(&block4);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block5);
    }
  }

  TNode<JSFunction> tmp4;
  TNode<JSReceiver> tmp5;
  TNode<JSReceiver> tmp6;
  TNode<Object> tmp7;
  TNode<Object> tmp8;
  TNode<String> tmp9;
  TNode<JSReceiver> tmp10;
  TNode<Object> tmp11;
  TNode<Number> tmp12;
  TNode<JSReceiver> tmp13;
  TNode<String> tmp14;
  TNode<Smi> tmp15;
  TNode<Smi> tmp16;
  TNode<Smi> tmp17;
  TNode<BoolT> tmp18;
  TNode<String> tmp19;
  TNode<Smi> tmp20;
  TNode<Smi> tmp21;
  TNode<Smi> tmp22;
  TNode<BoolT> tmp23;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp4 = LoadRegExpFunction_0(state_, TNode<NativeContext>{p_nativeContext});
    tmp5 = CodeStubAssembler(state_).SpeciesConstructor(TNode<Context>{p_context}, TNode<Object>{tmp0}, TNode<JSReceiver>{tmp4});
    tmp6 = UnsafeCast_Constructor_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp5});
    tmp7 = FromConstexpr_JSAny_constexpr_string_0(state_, "flags");
    tmp8 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{tmp0}, TNode<Object>{tmp7});
    tmp9 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{tmp8});
    tmp10 = CodeStubAssembler(state_).Construct(TNode<Context>{p_context}, TNode<JSReceiver>{tmp6}, TNode<Object>{tmp0}, TNode<Object>{tmp9});
    tmp11 = RegExpBuiltinsAssembler(state_).SlowLoadLastIndex(TNode<Context>{p_context}, TNode<Object>{tmp0});
    tmp12 = CodeStubAssembler(state_).ToLength_Inline(TNode<Context>{p_context}, TNode<Object>{tmp11});
    tmp13 = UnsafeCast_JSReceiver_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp10});
    RegExpBuiltinsAssembler(state_).SlowStoreLastIndex(TNode<Context>{p_context}, TNode<Object>{tmp13}, TNode<Object>{tmp12});
    tmp14 = CodeStubAssembler(state_).StringConstant("g");
    tmp15 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp16 = ca_.CallStub<Smi>(Builtins::CallableFor(ca_.isolate(), Builtin::kStringIndexOf), p_context, tmp9, tmp14, tmp15);
    tmp17 = FromConstexpr_Smi_constexpr_int31_0(state_, -1);
    tmp18 = CodeStubAssembler(state_).SmiNotEqual(TNode<Smi>{tmp16}, TNode<Smi>{tmp17});
    tmp19 = CodeStubAssembler(state_).StringConstant("u");
    tmp20 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    tmp21 = ca_.CallStub<Smi>(Builtins::CallableFor(ca_.isolate(), Builtin::kStringIndexOf), p_context, tmp9, tmp19, tmp20);
    tmp22 = FromConstexpr_Smi_constexpr_int31_0(state_, -1);
    tmp23 = CodeStubAssembler(state_).SmiNotEqual(TNode<Smi>{tmp21}, TNode<Smi>{tmp22});
    ca_.Goto(&block2, tmp10, tmp18, tmp23);
  }

  TNode<IntPtrT> tmp24;
  TNode<PrimitiveHeapObject> tmp25;
  TNode<String> tmp26;
  TNode<Object> tmp27;
  TNode<JSRegExp> tmp28;
  TNode<JSRegExp> tmp29;
  TNode<Smi> tmp30;
  TNode<BoolT> tmp31;
  TNode<BoolT> tmp32;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp24 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp25 = CodeStubAssembler(state_).LoadReference<PrimitiveHeapObject>(CodeStubAssembler::Reference{tmp2, tmp24});
    tmp26 = FastFlagsGetter_0(state_, TNode<Context>{p_context}, TNode<JSRegExp>{tmp2});
    tmp27 = RegExpCreate_0(state_, TNode<Context>{p_context}, TNode<NativeContext>{p_nativeContext}, TNode<Object>{tmp25}, TNode<String>{tmp26});
    tmp28 = UnsafeCast_JSRegExp_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp27});
    tmp29 = UnsafeCast_FastJSRegExp_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp0});
    tmp30 = RegExpBuiltinsAssembler(state_).FastLoadLastIndex(TNode<JSRegExp>{tmp29});
    RegExpBuiltinsAssembler(state_).FastStoreLastIndex(TNode<JSRegExp>{tmp28}, TNode<Smi>{tmp30});
    tmp31 = RegExpBuiltinsAssembler(state_).FastFlagGetter(TNode<JSRegExp>{tmp28}, JSRegExp::Flag::kGlobal);
    tmp32 = RegExpBuiltinsAssembler(state_).FastFlagGetter(TNode<JSRegExp>{tmp28}, JSRegExp::Flag::kUnicode);
    ca_.Goto(&block2, tmp27, tmp31, tmp32);
  }

  TNode<Object> phi_bb2_6;
  TNode<BoolT> phi_bb2_7;
  TNode<BoolT> phi_bb2_8;
  TNode<Object> tmp33;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_6, &phi_bb2_7, &phi_bb2_8);
    tmp33 = RegExpMatchAllAssembler(state_).CreateRegExpStringIterator(TNode<NativeContext>{p_nativeContext}, TNode<Object>{phi_bb2_6}, TNode<String>{tmp1}, TNode<BoolT>{phi_bb2_7}, TNode<BoolT>{phi_bb2_8});
    ca_.Goto(&block10);
  }

    ca_.Bind(&block10);
  return TNode<Object>{tmp33};
}


TF_BUILTIN(RegExpPrototypeMatchAll, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kString);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = RegExpPrototypeMatchAllImpl_0(state_, TNode<Context>{parameter0}, TNode<NativeContext>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{parameter2});
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(RegExpStringIteratorPrototypeNext, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, BoolT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSRegExpStringIterator> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSRegExpStringIterator_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  TNode<Object> tmp2;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp2 = FromConstexpr_Object_constexpr_string_0(state_, "%RegExpStringIterator%.prototype.next");
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kIncompatibleMethodReceiver, TNode<Object>{tmp2}, TNode<Object>{parameter1});
  }

  TNode<IntPtrT> tmp3;
  TNode<Smi> tmp4;
  TNode<BoolT> tmp5;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 20);
    tmp4 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp0, tmp3});
    tmp5 = ca_.UncheckedCast<BoolT>(CodeStubAssembler(state_).DecodeWord32FromWord<base::BitField<bool, 1, 1, uintptr_t>>(ca_.UncheckedCast<WordT>(ca_.BitcastTaggedToWordForTagAndSmiBits(tmp4))));
    ca_.Branch(tmp5, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{});
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block6);
  }

  TNode<IntPtrT> tmp6;
  TNode<JSReceiver> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<String> tmp9;
  TNode<BoolT> tmp10;
  TNode<BoolT> tmp11;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp7 = CodeStubAssembler(state_).LoadReference<JSReceiver>(CodeStubAssembler::Reference{tmp0, tmp6});
    tmp8 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp9 = CodeStubAssembler(state_).LoadReference<String>(CodeStubAssembler::Reference{tmp0, tmp8});
    tmp10 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    tmp11 = IsFastRegExpPermissive_0(state_, TNode<Context>{parameter0}, TNode<HeapObject>{tmp7});
    ca_.Branch(tmp11, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  TNode<JSRegExp> tmp12;
  TNode<Number> tmp13;
  TNode<RegExpMatchInfo> tmp14;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp12 = UnsafeCast_JSRegExp_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp7});
    tmp13 = LoadLastIndexAsLength_0(state_, TNode<Context>{parameter0}, TNode<JSRegExp>{tmp12}, true);
    compiler::CodeAssemblerLabel label15(&ca_);
    tmp14 = RegExpPrototypeExecBodyWithoutResultFast_1(state_, TNode<Context>{parameter0}, TNode<JSRegExp>{tmp12}, TNode<String>{tmp9}, TNode<Number>{tmp13}, &label15);
    ca_.Goto(&block14);
    if (label15.is_used()) {
      ca_.Bind(&label15);
      ca_.Goto(&block15);
    }
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block10, ca_.Uninitialized<Object>());
  }

  TNode<JSArray> tmp16;
  TNode<BoolT> tmp17;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp16 = RegExpBuiltinsAssembler(state_).ConstructNewResultFromMatchInfo(TNode<Context>{parameter0}, TNode<JSRegExp>{tmp12}, TNode<RegExpMatchInfo>{tmp14}, TNode<String>{tmp9}, TNode<Number>{tmp13});
    tmp17 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block13, tmp16, tmp17);
  }

  TNode<Object> tmp18;
  TNode<Oddball> tmp19;
  TNode<BoolT> tmp20;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp18 = RegExpExec_0(state_, TNode<Context>{parameter0}, TNode<JSReceiver>{tmp7}, TNode<String>{tmp9});
    tmp19 = Null_0(state_);
    tmp20 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp18}, TNode<HeapObject>{tmp19});
    ca_.Branch(tmp20, &block16, std::vector<compiler::Node*>{}, &block17, std::vector<compiler::Node*>{});
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block10, tmp18);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block13, tmp18, tmp10);
  }

  TNode<Object> phi_bb13_6;
  TNode<BoolT> phi_bb13_7;
  TNode<BoolT> tmp21;
  TNode<BoolT> tmp22;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_6, &phi_bb13_7);
    tmp21 = ca_.UncheckedCast<BoolT>(CodeStubAssembler(state_).DecodeWord32FromWord<base::BitField<bool, 2, 1, uintptr_t>>(ca_.UncheckedCast<WordT>(ca_.BitcastTaggedToWordForTagAndSmiBits(tmp4))));
    tmp22 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp21});
    ca_.Branch(tmp22, &block18, std::vector<compiler::Node*>{}, &block19, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp23;
  TNode<Smi> tmp24;
  TNode<BoolT> tmp25;
  TNode<Smi> tmp26;
  TNode<Object> tmp27;
  TNode<Oddball> tmp28;
  TNode<JSObject> tmp29;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    tmp23 = FromConstexpr_intptr_constexpr_int31_0(state_, 20);
    tmp24 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp0, tmp23});
    tmp25 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    tmp26 = ca_.UncheckedCast<Smi>(ca_.BitcastWordToTaggedSigned(CodeStubAssembler(state_).UpdateWord32InWord<base::BitField<bool, 1, 1, uintptr_t>>(ca_.UncheckedCast<WordT>(ca_.BitcastTaggedToWordForTagAndSmiBits(tmp24)), ca_.UncheckedCast<Uint32T>(tmp25))));
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp0, tmp23}, tmp26);
    tmp27 = UnsafeCast_JSAny_0(state_, TNode<Context>{parameter0}, TNode<Object>{phi_bb13_6});
    tmp28 = False_0(state_);
    tmp29 = CodeStubAssembler(state_).AllocateJSIteratorResult(TNode<Context>{parameter0}, TNode<Object>{tmp27}, TNode<Oddball>{tmp28});
    CodeStubAssembler(state_).Return(tmp29);
  }

  if (block19.is_used()) {
    ca_.Bind(&block19);
    ca_.Branch(phi_bb13_7, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<JSArray> tmp30;
  TNode<IntPtrT> tmp31;
  TNode<FixedArrayBase> tmp32;
  TNode<FixedArray> tmp33;
  TNode<Object> tmp34;
  TNode<IntPtrT> tmp35;
  TNode<IntPtrT> tmp36;
  TNode<IntPtrT> tmp37;
  TNode<UintPtrT> tmp38;
  TNode<UintPtrT> tmp39;
  TNode<BoolT> tmp40;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp30 = UnsafeCast_JSRegExpResult_0(state_, TNode<Context>{parameter0}, TNode<Object>{phi_bb13_6});
    tmp31 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp32 = CodeStubAssembler(state_).LoadReference<FixedArrayBase>(CodeStubAssembler::Reference{tmp30, tmp31});
    tmp33 = UnsafeCast_FixedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp32});
    std::tie(tmp34, tmp35, tmp36) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{tmp33}).Flatten();
    tmp37 = Convert_intptr_constexpr_int31_0(state_, 0);
    tmp38 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp37});
    tmp39 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp36});
    tmp40 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp38}, TNode<UintPtrT>{tmp39});
    ca_.Branch(tmp40, &block30, std::vector<compiler::Node*>{}, &block31, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp41;
  TNode<IntPtrT> tmp42;
  TNode<Object> tmp43;
  TNode<IntPtrT> tmp44;
  TNode<Object> tmp45;
  TNode<String> tmp46;
  TNode<JSRegExp> tmp47;
  TNode<String> tmp48;
  TNode<BoolT> tmp49;
  if (block30.is_used()) {
    ca_.Bind(&block30);
    tmp41 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{tmp37});
    tmp42 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp35}, TNode<IntPtrT>{tmp41});
    std::tie(tmp43, tmp44) = NewReference_Object_0(state_, TNode<Object>{tmp34}, TNode<IntPtrT>{tmp42}).Flatten();
    tmp45 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp43, tmp44});
    tmp46 = UnsafeCast_String_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp45});
    tmp47 = UnsafeCast_JSRegExp_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp7});
    tmp48 = kEmptyString_0(state_);
    tmp49 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp46}, TNode<HeapObject>{tmp48});
    ca_.Branch(tmp49, &block37, std::vector<compiler::Node*>{}, &block38, std::vector<compiler::Node*>{});
  }

  if (block31.is_used()) {
    ca_.Bind(&block31);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Smi> tmp50;
  TNode<BoolT> tmp51;
  TNode<Smi> tmp52;
  if (block37.is_used()) {
    ca_.Bind(&block37);
    tmp50 = RegExpBuiltinsAssembler(state_).FastLoadLastIndex(TNode<JSRegExp>{tmp47});
    tmp51 = ca_.UncheckedCast<BoolT>(CodeStubAssembler(state_).DecodeWord32FromWord<base::BitField<bool, 3, 1, uintptr_t>>(ca_.UncheckedCast<WordT>(ca_.BitcastTaggedToWordForTagAndSmiBits(tmp4))));
    tmp52 = RegExpBuiltinsAssembler(state_).AdvanceStringIndexFast(TNode<String>{tmp9}, TNode<Smi>{tmp50}, TNode<BoolT>{tmp51});
    RegExpBuiltinsAssembler(state_).FastStoreLastIndex(TNode<JSRegExp>{tmp47}, TNode<Smi>{tmp52});
    ca_.Goto(&block38);
  }

  TNode<Oddball> tmp53;
  TNode<JSObject> tmp54;
  if (block38.is_used()) {
    ca_.Bind(&block38);
    tmp53 = False_0(state_);
    tmp54 = CodeStubAssembler(state_).AllocateJSIteratorResult(TNode<Context>{parameter0}, TNode<Object>{tmp30}, TNode<Oddball>{tmp53});
    CodeStubAssembler(state_).Return(tmp54);
  }

  TNode<Object> tmp55;
  TNode<Smi> tmp56;
  TNode<Object> tmp57;
  TNode<String> tmp58;
  TNode<String> tmp59;
  TNode<BoolT> tmp60;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp55 = UnsafeCast_JSAny_0(state_, TNode<Context>{parameter0}, TNode<Object>{phi_bb13_6});
    tmp56 = CodeStubAssembler(state_).SmiConstant(0);
    tmp57 = CodeStubAssembler(state_).GetProperty(TNode<Context>{parameter0}, TNode<Object>{tmp55}, TNode<Object>{tmp56});
    tmp58 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{tmp57});
    tmp59 = kEmptyString_0(state_);
    tmp60 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp58}, TNode<HeapObject>{tmp59});
    ca_.Branch(tmp60, &block43, std::vector<compiler::Node*>{}, &block44, std::vector<compiler::Node*>{});
  }

  TNode<Object> tmp61;
  TNode<Number> tmp62;
  TNode<BoolT> tmp63;
  TNode<Smi> tmp64;
  if (block43.is_used()) {
    ca_.Bind(&block43);
    tmp61 = RegExpBuiltinsAssembler(state_).SlowLoadLastIndex(TNode<Context>{parameter0}, TNode<Object>{tmp7});
    tmp62 = CodeStubAssembler(state_).ToLength_Inline(TNode<Context>{parameter0}, TNode<Object>{tmp61});
    tmp63 = ca_.UncheckedCast<BoolT>(CodeStubAssembler(state_).DecodeWord32FromWord<base::BitField<bool, 3, 1, uintptr_t>>(ca_.UncheckedCast<WordT>(ca_.BitcastTaggedToWordForTagAndSmiBits(tmp4))));
    tmp64 = RegExpBuiltinsAssembler(state_).AdvanceStringIndexSlow(TNode<String>{tmp9}, TNode<Number>{tmp62}, TNode<BoolT>{tmp63});
    RegExpBuiltinsAssembler(state_).SlowStoreLastIndex(TNode<Context>{parameter0}, TNode<Object>{tmp7}, TNode<Object>{tmp64});
    ca_.Goto(&block44);
  }

  TNode<Oddball> tmp65;
  TNode<JSObject> tmp66;
  if (block44.is_used()) {
    ca_.Bind(&block44);
    tmp65 = False_0(state_);
    tmp66 = CodeStubAssembler(state_).AllocateJSIteratorResult(TNode<Context>{parameter0}, TNode<Object>{tmp55}, TNode<Oddball>{tmp65});
    CodeStubAssembler(state_).Return(tmp66);
  }

  TNode<Object> phi_bb10_6;
  TNode<IntPtrT> tmp67;
  TNode<Smi> tmp68;
  TNode<BoolT> tmp69;
  TNode<Smi> tmp70;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_6);
    tmp67 = FromConstexpr_intptr_constexpr_int31_0(state_, 20);
    tmp68 = CodeStubAssembler(state_).LoadReference<Smi>(CodeStubAssembler::Reference{tmp0, tmp67});
    tmp69 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    tmp70 = ca_.UncheckedCast<Smi>(ca_.BitcastWordToTaggedSigned(CodeStubAssembler(state_).UpdateWord32InWord<base::BitField<bool, 1, 1, uintptr_t>>(ca_.UncheckedCast<WordT>(ca_.BitcastTaggedToWordForTagAndSmiBits(tmp68)), ca_.UncheckedCast<Uint32T>(tmp69))));
    CodeStubAssembler(state_).StoreReference<Smi>(CodeStubAssembler::Reference{tmp0, tmp67}, tmp70);
    ca_.Goto(&block6);
  }

  TNode<Oddball> tmp71;
  TNode<Oddball> tmp72;
  TNode<JSObject> tmp73;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp71 = Undefined_0(state_);
    tmp72 = True_0(state_);
    tmp73 = CodeStubAssembler(state_).AllocateJSIteratorResult(TNode<Context>{parameter0}, TNode<Object>{tmp71}, TNode<Oddball>{tmp72});
    CodeStubAssembler(state_).Return(tmp73);
  }
}

TNode<JSRegExp> UnsafeCast_JSRegExp_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSRegExp> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<JSRegExp>{tmp0};
}


TNode<JSRegExp> UnsafeCast_FastJSRegExp_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSRegExp> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<JSRegExp>{tmp0};
}


TNode<JSRegExpStringIterator> Cast_JSRegExpStringIterator_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o, compiler::CodeAssemblerLabel* label_CastError) {
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

  TNode<JSRegExpStringIterator> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_JSRegExpStringIterator_0(state_, TNode<HeapObject>{tmp0}, &label3);
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
  return TNode<JSRegExpStringIterator>{tmp2};
}


TNode<JSArray> UnsafeCast_JSRegExpResult_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
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


TNode<String> UnsafeCast_String_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<String>{tmp0};
}


} // namespace internal
} // namespace v8
