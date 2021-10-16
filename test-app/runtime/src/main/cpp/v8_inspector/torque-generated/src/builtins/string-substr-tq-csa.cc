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
#include "torque-generated/src/builtins/string-substr-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/builtins-string-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(StringPrototypeSubstr, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<UintPtrT> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<Object> tmp4;
  TNode<Oddball> tmp5;
  TNode<BoolT> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.substr");
    tmp1 = CodeStubAssembler(state_).ToThisString(TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<String>{tmp0});
    tmp2 = LoadStringLengthAsUintPtr_0(state_, TNode<String>{tmp1});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp4 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp3});
    tmp5 = Undefined_0(state_);
    tmp6 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp4}, TNode<HeapObject>{tmp5});
    ca_.Branch(tmp6, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  TNode<UintPtrT> tmp7;
  if (block1.is_used()) {
    ca_.Bind(&block1);
    tmp7 = ConvertToRelativeIndex_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp4}, TNode<UintPtrT>{tmp2});
    ca_.Goto(&block3, tmp7);
  }

  TNode<UintPtrT> tmp8;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp8 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block3, tmp8);
  }

  TNode<UintPtrT> phi_bb3_8;
  TNode<IntPtrT> tmp9;
  TNode<Object> tmp10;
  TNode<UintPtrT> tmp11;
  TNode<Oddball> tmp12;
  TNode<BoolT> tmp13;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_8);
    tmp9 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp10 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp9});
    tmp11 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{tmp2}, TNode<UintPtrT>{phi_bb3_8});
    tmp12 = Undefined_0(state_);
    tmp13 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp10}, TNode<HeapObject>{tmp12});
    ca_.Branch(tmp13, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<UintPtrT> tmp14;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp14 = ClampToIndexRange_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp10}, TNode<UintPtrT>{tmp11});
    ca_.Goto(&block11, tmp14);
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    ca_.Goto(&block11, tmp11);
  }

  TNode<UintPtrT> phi_bb11_11;
  TNode<UintPtrT> tmp15;
  TNode<BoolT> tmp16;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_11);
    tmp15 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp16 = CodeStubAssembler(state_).WordEqual(TNode<UintPtrT>{phi_bb11_11}, TNode<UintPtrT>{tmp15});
    ca_.Branch(tmp16, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  TNode<String> tmp17;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp17 = CodeStubAssembler(state_).EmptyStringConstant();
    arguments.PopAndReturn(tmp17);
  }

  TNode<UintPtrT> tmp18;
  TNode<String> tmp19;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp18 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb3_8}, TNode<UintPtrT>{phi_bb11_11});
    tmp19 = StringBuiltinsAssembler(state_).SubString(TNode<String>{tmp1}, TNode<UintPtrT>{phi_bb3_8}, TNode<UintPtrT>{tmp18});
    arguments.PopAndReturn(tmp19);
  }
}

} // namespace internal
} // namespace v8
