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
#include "torque-generated/src/builtins/typed-array-subarray-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-createtypedarray-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-every-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(TypedArrayPrototypeSubArray, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSTypedArray_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kIncompatibleMethodReceiver, "%TypedArray%.prototype.subarray");
  }

  TNode<JSArrayBuffer> tmp2;
  TNode<IntPtrT> tmp3;
  TNode<UintPtrT> tmp4;
  TNode<IntPtrT> tmp5;
  TNode<Object> tmp6;
  TNode<Oddball> tmp7;
  TNode<BoolT> tmp8;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp2 = CodeStubAssembler(state_).GetTypedArrayBuffer(TNode<Context>{parameter0}, TNode<JSTypedArray>{tmp0});
    tmp3 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp4 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp0, tmp3});
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp6 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp5});
    tmp7 = Undefined_0(state_);
    tmp8 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp6}, TNode<HeapObject>{tmp7});
    ca_.Branch(tmp8, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  TNode<UintPtrT> tmp9;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp9 = ConvertToRelativeIndex_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp6}, TNode<UintPtrT>{tmp4});
    ca_.Goto(&block7, tmp9);
  }

  TNode<UintPtrT> tmp10;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp10 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block7, tmp10);
  }

  TNode<UintPtrT> phi_bb7_9;
  TNode<IntPtrT> tmp11;
  TNode<Object> tmp12;
  TNode<Oddball> tmp13;
  TNode<BoolT> tmp14;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_9);
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp12 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp11});
    tmp13 = Undefined_0(state_);
    tmp14 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp12}, TNode<HeapObject>{tmp13});
    ca_.Branch(tmp14, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<UintPtrT> tmp15;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp15 = ConvertToRelativeIndex_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp12}, TNode<UintPtrT>{tmp4});
    ca_.Goto(&block11, tmp15);
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    ca_.Goto(&block11, tmp4);
  }

  TNode<UintPtrT> phi_bb11_11;
  TNode<UintPtrT> tmp16;
  TNode<IntPtrT> tmp17;
  TNode<IntPtrT> tmp18;
  TNode<IntPtrT> tmp19;
  TNode<UintPtrT> tmp20;
  TNode<UintPtrT> tmp21;
  TNode<Int32T> tmp22;
  TNode<IntPtrT> tmp23;
  TNode<UintPtrT> tmp24;
  TNode<UintPtrT> tmp25;
  TNode<BoolT> tmp26;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_11);
    tmp16 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{phi_bb11_11}, TNode<UintPtrT>{phi_bb7_9});
    tmp17 = CodeStubAssembler(state_).Signed(TNode<UintPtrT>{tmp16});
    tmp18 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp19 = CodeStubAssembler(state_).IntPtrMax(TNode<IntPtrT>{tmp17}, TNode<IntPtrT>{tmp18});
    tmp20 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{tmp19});
    std::tie(tmp21, tmp22) = TypedArrayBuiltinsAssembler(state_).GetTypedArrayElementsInfo(TNode<JSTypedArray>{tmp0}).Flatten();
    tmp23 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp24 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp0, tmp23});
    tmp25 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSTypedArray::kMaxLength);
    tmp26 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{phi_bb7_9}, TNode<UintPtrT>{tmp25});
    ca_.Branch(tmp26, &block16, std::vector<compiler::Node*>{}, &block17, std::vector<compiler::Node*>{});
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block14);
  }

  TNode<UintPtrT> tmp27;
  TNode<UintPtrT> tmp28;
  TNode<BoolT> tmp29;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp27 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSArrayBuffer::kMaxByteLength);
    tmp28 = CodeStubAssembler(state_).WordShr(TNode<UintPtrT>{tmp27}, TNode<UintPtrT>{tmp21});
    tmp29 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{phi_bb7_9}, TNode<UintPtrT>{tmp28});
    ca_.Branch(tmp29, &block18, std::vector<compiler::Node*>{}, &block19, std::vector<compiler::Node*>{});
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    ca_.Goto(&block14);
  }

  TNode<UintPtrT> tmp30;
  TNode<UintPtrT> tmp31;
  TNode<JSTypedArray> tmp32;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    tmp30 = CodeStubAssembler(state_).WordShl(TNode<UintPtrT>{phi_bb7_9}, TNode<UintPtrT>{tmp21});
    tmp31 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{tmp24}, TNode<UintPtrT>{tmp30});
    tmp32 = TypedArraySpeciesCreateByBuffer_0(state_, TNode<Context>{parameter0}, "%TypedArray%.prototype.subarray", TNode<JSTypedArray>{tmp0}, TNode<JSArrayBuffer>{tmp2}, TNode<UintPtrT>{tmp31}, TNode<UintPtrT>{tmp20});
    arguments.PopAndReturn(tmp32);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{parameter0}, MessageTemplate::kInvalidArrayBufferLength);
  }
}

} // namespace internal
} // namespace v8
