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
#include "torque-generated/src/builtins/proxy-is-extensible-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/object-tq-csa.h"
#include "torque-generated/src/builtins/proxy-delete-property-tq-csa.h"
#include "torque-generated/src/objects/js-objects-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(ProxyIsExtensible, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSProxy> parameter1 = UncheckedParameter<JSProxy>(Descriptor::kProxy);
  USE(parameter1);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<HeapObject> tmp1;
  TNode<JSReceiver> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    CodeStubAssembler(state_).PerformStackCheck(TNode<Context>{parameter0});
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp1 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{parameter1, tmp0});
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_JSReceiver_0(state_, TNode<HeapObject>{tmp1}, &label3);
    ca_.Goto(&block12);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block13);
    }
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kProxyRevoked, "isExtensible");
  }

  TNode<IntPtrT> tmp4;
  TNode<HeapObject> tmp5;
  TNode<JSReceiver> tmp6;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    tmp5 = CodeStubAssembler(state_).LoadReference<HeapObject>(CodeStubAssembler::Reference{parameter1, tmp4});
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = GetMethod_2(state_, TNode<Context>{parameter0}, TNode<Object>{tmp2}, "isExtensible", &label7);
    ca_.Goto(&block16);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block17);
    }
  }

  TNode<Object> tmp8;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp8 = ObjectIsExtensibleImpl_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp5});
    CodeStubAssembler(state_).Return(tmp8);
  }

  TNode<Object> tmp9;
  TNode<BoolT> tmp10;
  TNode<Object> tmp11;
  TNode<BoolT> tmp12;
  TNode<BoolT> tmp13;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp9 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp6}, TNode<Object>{tmp2}, TNode<Object>{tmp5});
    tmp10 = ToBoolean_0(state_, TNode<Object>{tmp9});
    tmp11 = ObjectIsExtensibleImpl_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp5});
    tmp12 = ToBoolean_0(state_, TNode<Object>{tmp11});
    tmp13 = CodeStubAssembler(state_).Word32NotEqual(TNode<BoolT>{tmp10}, TNode<BoolT>{tmp12});
    ca_.Branch(tmp13, &block18, std::vector<compiler::Node*>{}, &block19, std::vector<compiler::Node*>{});
  }

  TNode<Oddball> tmp14;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    tmp14 = CodeStubAssembler(state_).SelectBooleanConstant(TNode<BoolT>{tmp12});
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kProxyIsExtensibleInconsistent, TNode<Object>{tmp14});
  }

  TNode<Oddball> tmp15;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    tmp15 = CodeStubAssembler(state_).SelectBooleanConstant(TNode<BoolT>{tmp10});
    CodeStubAssembler(state_).Return(tmp15);
  }
}

} // namespace internal
} // namespace v8
