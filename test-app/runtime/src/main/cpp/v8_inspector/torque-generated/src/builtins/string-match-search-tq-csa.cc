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
#include "torque-generated/src/builtins/string-match-search-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frames-tq-csa.h"
#include "torque-generated/src/builtins/regexp-match-all-tq-csa.h"
#include "torque-generated/src/builtins/regexp-match-tq-csa.h"
#include "torque-generated/src/builtins/regexp-search-tq-csa.h"
#include "torque-generated/src/builtins/regexp-tq-csa.h"
#include "torque-generated/src/builtins/string-match-search-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(StringPrototypeMatch, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kRegexp);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = StringMatchSearch_StringMatchFunctor_0(state_, TNode<NativeContext>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{parameter2}, TorqueStructStringMatchFunctor_0{}, "String.prototype.match");
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TF_BUILTIN(StringPrototypeSearch, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<NativeContext> parameter0 = UncheckedParameter<NativeContext>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
USE(parameter1);
  TNode<Object> parameter2 = UncheckedParameter<Object>(Descriptor::kRegexp);
  USE(parameter2);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = StringMatchSearch_StringSearchFunctor_0(state_, TNode<NativeContext>{parameter0}, TNode<Object>{parameter1}, TNode<Object>{parameter2}, TorqueStructStringSearchFunctor_0{}, "String.prototype.search");
    CodeStubAssembler(state_).Return(tmp0);
  }
}

TNode<Object> StringMatchSearch_StringMatchFunctor_0(compiler::CodeAssemblerState* state_, TNode<NativeContext> p_context, TNode<Object> p_receiver, TNode<Object> p_regexp, TorqueStructStringMatchFunctor_0 p_functor, const char* p_methodName) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<String> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = RequireObjectCoercible_0(state_, TNode<Context>{p_context}, TNode<Object>{p_receiver}, p_methodName);
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_String_1(state_, TNode<Context>{p_context}, TNode<Object>{p_receiver}, &label2);
    ca_.Goto(&block4);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block3);
  }

  TNode<HeapObject> tmp3;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_HeapObject_1(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp}, &label4);
    ca_.Goto(&block6);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block7);
    }
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block3);
  }

  TNode<BoolT> tmp5;
  TNode<BoolT> tmp6;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp5 = IsFastRegExpForMatch_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{tmp3});
    tmp6 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp5});
    ca_.Branch(tmp6, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(&block3);
  }

  TNode<JSRegExp> tmp7;
  TNode<Object> tmp8;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp7 = UnsafeCast_FastJSRegExp_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp3});
    tmp8 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kRegExpMatchFast), p_context, tmp7, tmp1);
    ca_.Goto(&block1, tmp8);
  }

  TNode<Oddball> tmp9;
  TNode<BoolT> tmp10;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp9 = Undefined_0(state_);
    tmp10 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{p_regexp}, TNode<HeapObject>{tmp9});
    ca_.Branch(tmp10, &block14, std::vector<compiler::Node*>{}, &block15, std::vector<compiler::Node*>{});
  }

  TNode<Oddball> tmp11;
  TNode<BoolT> tmp12;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp11 = Null_0(state_);
    tmp12 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{p_regexp}, TNode<HeapObject>{tmp11});
    ca_.Goto(&block16, tmp12);
  }

  TNode<BoolT> tmp13;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp13 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block16, tmp13);
  }

  TNode<BoolT> phi_bb16_4;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_4);
    ca_.Branch(phi_bb16_4, &block12, std::vector<compiler::Node*>{}, &block13, std::vector<compiler::Node*>{});
  }

  TNode<Symbol> tmp14;
  TNode<JSReceiver> tmp15;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp14 = CodeStubAssembler(state_).MatchSymbolConstant();
    compiler::CodeAssemblerLabel label16(&ca_);
    tmp15 = GetMethod_3(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp}, TNode<Symbol>{tmp14}, &label16);
    ca_.Goto(&block20);
    if (label16.is_used()) {
      ca_.Bind(&label16);
      ca_.Goto(&block21);
    }
  }

  if (block21.is_used()) {
    ca_.Bind(&block21);
    ca_.Goto(&block13);
  }

  TNode<Object> tmp17;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp17 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp15}, TNode<Object>{p_regexp}, TNode<Object>{p_receiver});
    ca_.Goto(&block1, tmp17);
  }

  TNode<String> tmp18;
  TNode<String> tmp19;
  TNode<Object> tmp20;
  TNode<Symbol> tmp21;
  TNode<Object> tmp22;
  TNode<Object> tmp23;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp18 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{p_receiver});
    tmp19 = kEmptyString_0(state_);
    tmp20 = RegExpCreate_0(state_, TNode<Context>{p_context}, TNode<NativeContext>{p_context}, TNode<Object>{p_regexp}, TNode<String>{tmp19});
    tmp21 = CodeStubAssembler(state_).MatchSymbolConstant();
    tmp22 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{tmp20}, TNode<Object>{tmp21});
    tmp23 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp22}, TNode<Object>{tmp20}, TNode<Object>{tmp18});
    ca_.Goto(&block1, tmp23);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block23, phi_bb1_3);
  }

  TNode<Object> phi_bb23_3;
    ca_.Bind(&block23, &phi_bb23_3);
  return TNode<Object>{phi_bb23_3};
}


TNode<Object> StringMatchSearch_StringSearchFunctor_0(compiler::CodeAssemblerState* state_, TNode<NativeContext> p_context, TNode<Object> p_receiver, TNode<Object> p_regexp, TorqueStructStringSearchFunctor_0 p_functor, const char* p_methodName) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Object> tmp0;
  TNode<String> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = RequireObjectCoercible_0(state_, TNode<Context>{p_context}, TNode<Object>{p_receiver}, p_methodName);
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_String_1(state_, TNode<Context>{p_context}, TNode<Object>{p_receiver}, &label2);
    ca_.Goto(&block4);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block3);
  }

  TNode<HeapObject> tmp3;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_HeapObject_1(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp}, &label4);
    ca_.Goto(&block6);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block7);
    }
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block3);
  }

  TNode<BoolT> tmp5;
  TNode<BoolT> tmp6;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp5 = IsFastRegExpForSearch_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{tmp3});
    tmp6 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp5});
    ca_.Branch(tmp6, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(&block3);
  }

  TNode<JSRegExp> tmp7;
  TNode<Object> tmp8;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp7 = UnsafeCast_FastJSRegExp_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp3});
    tmp8 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kRegExpSearchFast), p_context, tmp7, tmp1);
    ca_.Goto(&block1, tmp8);
  }

  TNode<Oddball> tmp9;
  TNode<BoolT> tmp10;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp9 = Undefined_0(state_);
    tmp10 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{p_regexp}, TNode<HeapObject>{tmp9});
    ca_.Branch(tmp10, &block14, std::vector<compiler::Node*>{}, &block15, std::vector<compiler::Node*>{});
  }

  TNode<Oddball> tmp11;
  TNode<BoolT> tmp12;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp11 = Null_0(state_);
    tmp12 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{p_regexp}, TNode<HeapObject>{tmp11});
    ca_.Goto(&block16, tmp12);
  }

  TNode<BoolT> tmp13;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp13 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block16, tmp13);
  }

  TNode<BoolT> phi_bb16_4;
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_4);
    ca_.Branch(phi_bb16_4, &block12, std::vector<compiler::Node*>{}, &block13, std::vector<compiler::Node*>{});
  }

  TNode<Symbol> tmp14;
  TNode<JSReceiver> tmp15;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp14 = CodeStubAssembler(state_).SearchSymbolConstant();
    compiler::CodeAssemblerLabel label16(&ca_);
    tmp15 = GetMethod_3(state_, TNode<Context>{p_context}, TNode<Object>{p_regexp}, TNode<Symbol>{tmp14}, &label16);
    ca_.Goto(&block20);
    if (label16.is_used()) {
      ca_.Bind(&label16);
      ca_.Goto(&block21);
    }
  }

  if (block21.is_used()) {
    ca_.Bind(&block21);
    ca_.Goto(&block13);
  }

  TNode<Object> tmp17;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp17 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp15}, TNode<Object>{p_regexp}, TNode<Object>{p_receiver});
    ca_.Goto(&block1, tmp17);
  }

  TNode<String> tmp18;
  TNode<String> tmp19;
  TNode<Object> tmp20;
  TNode<Symbol> tmp21;
  TNode<Object> tmp22;
  TNode<Object> tmp23;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp18 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{p_context}, TNode<Object>{p_receiver});
    tmp19 = kEmptyString_0(state_);
    tmp20 = RegExpCreate_0(state_, TNode<Context>{p_context}, TNode<NativeContext>{p_context}, TNode<Object>{p_regexp}, TNode<String>{tmp19});
    tmp21 = CodeStubAssembler(state_).SearchSymbolConstant();
    tmp22 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{tmp20}, TNode<Object>{tmp21});
    tmp23 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp22}, TNode<Object>{tmp20}, TNode<Object>{tmp18});
    ca_.Goto(&block1, tmp23);
  }

  TNode<Object> phi_bb1_3;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_3);
    ca_.Goto(&block23, phi_bb1_3);
  }

  TNode<Object> phi_bb23_3;
    ca_.Bind(&block23, &phi_bb23_3);
  return TNode<Object>{phi_bb23_3};
}


} // namespace internal
} // namespace v8
