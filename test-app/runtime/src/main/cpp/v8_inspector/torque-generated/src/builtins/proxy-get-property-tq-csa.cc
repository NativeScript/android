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
#include "torque-generated/src/builtins/proxy-get-property-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/proxy-delete-property-tq-csa.h"
#include "torque-generated/src/builtins/proxy-get-property-tq-csa.h"
#include "torque-generated/src/builtins/proxy-tq-csa.h"
#include "torque-generated/src/objects/js-objects-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(ProxyGetProperty, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSProxy> parameter1 = UncheckedParameter<JSProxy>(Descriptor::kProxy);
  USE(parameter1);
  TNode<Name> parameter2 = UncheckedParameter<Name>(Descriptor::kName);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kReceiverValue);
  USE(parameter3);
  TNode<Smi> parameter4 = UncheckedParameter<Smi>(Descriptor::kOnNonExistent);
  USE(parameter4);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<HeapObject> tmp1;
  TNode<Oddball> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    CodeStubAssembler(state_).PerformStackCheck(TNode<Context>{parameter0});
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp1 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{parameter1, tmp0});
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Null_0(state_, TNode<HeapObject>{tmp1}, &label3);
    ca_.Goto(&block15);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block16);
    }
  }

  TNode<IntPtrT> tmp4;
  TNode<HeapObject> tmp5;
  TNode<JSReceiver> tmp6;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp5 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{parameter1, tmp4});
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = Cast_JSReceiver_0(state_, TNode<HeapObject>{tmp5}, &label7);
    ca_.Goto(&block19);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block20);
    }
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kProxyRevoked, "get");
  }

  if (block20.is_used()) {
    ca_.Bind(&block20);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<JSReceiver> tmp8;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    compiler::CodeAssemblerLabel label9(&ca_);
    tmp8 = GetMethod_2(state_, TNode<Context>{parameter0}, TNode<Object>{ca_.UncheckedCast<JSReceiver>(tmp1)}, "get", &label9);
    ca_.Goto(&block23);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block24);
    }
  }

  TNode<Object> tmp10;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp10 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kGetPropertyWithReceiver), parameter0, tmp6, parameter2, parameter3, parameter4);
    CodeStubAssembler(state_).Return(tmp10);
  }

  TNode<Object> tmp11;
  if (block23.is_used()) {
    ca_.Bind(&block23);
    tmp11 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp8}, TNode<Object>{ca_.UncheckedCast<JSReceiver>(tmp1)}, TNode<Object>{tmp6}, TNode<Object>{parameter2}, TNode<Object>{parameter3});
    ProxiesCodeStubAssembler(state_).CheckGetSetTrapResult(TNode<Context>{parameter0}, TNode<JSReceiver>{tmp6}, TNode<JSProxy>{parameter1}, TNode<Name>{parameter2}, TNode<Object>{tmp11}, JSProxy::AccessKind::kGet);
    CodeStubAssembler(state_).Return(tmp11);
  }
}

} // namespace internal
} // namespace v8
