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
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frames-tq-csa.h"

namespace v8 {
namespace internal {
TorqueStructFrameWithArgumentsInfo_0 GetFrameWithArgumentsInfo_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<RawPtrT> tmp0;
  TNode<RawPtrT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).LoadParentFramePointer();
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_FrameWithArguments_0(state_, TNode<Context>{p_context}, TNode<RawPtrT>{tmp0}, &label2);
    ca_.Goto(&block4);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<JSFunction> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<SharedFunctionInfo> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Uint16T> tmp7;
  TNode<Int32T> tmp8;
  TNode<BInt> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<BInt> tmp11;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp3 = LoadFunctionFromFrame_0(state_, TNode<RawPtrT>{tmp1});
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp5 = CodeStubAssembler(state_).LoadReference<SharedFunctionInfo>(CodeStubAssembler::Reference{tmp3, tmp4});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 22);
    tmp7 = CodeStubAssembler(state_).LoadReference<Uint16T>(CodeStubAssembler::Reference{tmp5, tmp6});
    tmp8 = Convert_int32_uint16_0(state_, TNode<Uint16T>{tmp7});
    tmp9 = Convert_bint_int32_0(state_, TNode<Int32T>{tmp8});
    tmp10 = LoadArgCFromFrame_0(state_, TNode<RawPtrT>{tmp1});
    tmp11 = Convert_bint_intptr_0(state_, TNode<IntPtrT>{tmp10});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TorqueStructFrameWithArgumentsInfo_0{TNode<RawPtrT>{tmp1}, TNode<BInt>{tmp11}, TNode<BInt>{tmp9}};
}


} // namespace internal
} // namespace v8
