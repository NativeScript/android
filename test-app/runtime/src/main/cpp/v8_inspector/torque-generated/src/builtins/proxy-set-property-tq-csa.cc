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
#include "torque-generated/src/builtins/proxy-set-property-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/boolean-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/proxy-delete-property-tq-csa.h"
#include "torque-generated/src/builtins/proxy-set-property-tq-csa.h"
#include "torque-generated/src/builtins/proxy-tq-csa.h"
#include "torque-generated/src/objects/js-objects-tq-csa.h"
#include "torque-generated/src/objects/name-tq-csa.h"

namespace v8 {
namespace internal {
void CallThrowTypeErrorIfStrict_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, MessageTemplate p_message) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Smi> tmp0;
  TNode<Oddball> tmp1;
  TNode<Oddball> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).SmiConstant(p_message);
    tmp1 = Null_0(state_);
    tmp2 = Null_0(state_);
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowTypeErrorIfStrict, p_context, tmp0, tmp1, tmp2);
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
}


TF_BUILTIN(ProxySetProperty, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSProxy> parameter1 = UncheckedParameter<JSProxy>(Descriptor::kProxy);
  USE(parameter1);
  TNode<Name> parameter2 = UncheckedParameter<Name>(Descriptor::kName);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kValue);
  USE(parameter3);
  TNode<Object> parameter4 = UncheckedParameter<Object>(Descriptor::kReceiverValue);
  USE(parameter4);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Symbol> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_PrivateSymbol_1(state_, TNode<HeapObject>{parameter2}, &label1);
    ca_.Goto(&block11);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block12);
    }
  }

  TNode<IntPtrT> tmp2;
  TNode<HeapObject> tmp3;
  TNode<JSReceiver> tmp4;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp3 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{parameter1, tmp2});
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_JSReceiver_0(state_, TNode<HeapObject>{tmp3}, &label5);
    ca_.Goto(&block24);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block25);
    }
  }

  TNode<Oddball> tmp6;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    CallThrowTypeErrorIfStrict_0(state_, TNode<Context>{parameter0}, MessageTemplate::kProxyPrivate);
    tmp6 = Undefined_0(state_);
    CodeStubAssembler(state_).Return(tmp6);
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kProxyRevoked, "set");
  }

  TNode<IntPtrT> tmp7;
  TNode<HeapObject> tmp8;
  TNode<JSReceiver> tmp9;
  TNode<JSReceiver> tmp10;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp8 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{parameter1, tmp7});
    tmp9 = UnsafeCast_JSReceiver_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp8});
    compiler::CodeAssemblerLabel label11(&ca_);
    tmp10 = GetMethod_2(state_, TNode<Context>{parameter0}, TNode<Object>{tmp4}, "set", &label11);
    ca_.Goto(&block28);
    if (label11.is_used()) {
      ca_.Bind(&label11);
      ca_.Goto(&block29);
    }
  }

  if (block29.is_used()) {
    ca_.Bind(&block29);
    CodeStubAssembler(state_).CallRuntime(Runtime::kSetPropertyWithReceiver, parameter0, tmp9, parameter2, parameter3, parameter4);
    CodeStubAssembler(state_).Return(parameter3);
  }

  TNode<Object> tmp12;
  TNode<BoolT> tmp13;
  if (block28.is_used()) {
    ca_.Bind(&block28);
    tmp12 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp10}, TNode<Object>{tmp4}, TNode<Object>{tmp9}, TNode<Object>{ca_.UncheckedCast<Name>(parameter2)}, TNode<Object>{parameter3}, TNode<Object>{parameter4});
    tmp13 = ToBoolean_0(state_, TNode<Object>{tmp12});
    ca_.Branch(tmp13, &block30, std::vector<compiler::Node*>{}, &block31, std::vector<compiler::Node*>{});
  }

  if (block30.is_used()) {
    ca_.Bind(&block30);
    ProxiesCodeStubAssembler(state_).CheckGetSetTrapResult(TNode<Context>{parameter0}, TNode<JSReceiver>{tmp9}, TNode<JSProxy>{parameter1}, TNode<Name>{parameter2}, TNode<Object>{parameter3}, JSProxy::AccessKind::kSet);
    CodeStubAssembler(state_).Return(parameter3);
  }

  TNode<Smi> tmp14;
  TNode<Object> tmp15;
  if (block31.is_used()) {
    ca_.Bind(&block31);
    tmp14 = CodeStubAssembler(state_).SmiConstant(MessageTemplate::kProxyTrapReturnedFalsishFor);
    tmp15 = FromConstexpr_Object_constexpr_string_0(state_, "set");
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowTypeErrorIfStrict, parameter0, tmp14, tmp15, parameter2);
    CodeStubAssembler(state_).Return(parameter3);
  }
}

TNode<BoolT> Is_Name_AnyName_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Name> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Name> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_Name_0(state_, TNode<HeapObject>{p_o}, &label1);
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


} // namespace internal
} // namespace v8
