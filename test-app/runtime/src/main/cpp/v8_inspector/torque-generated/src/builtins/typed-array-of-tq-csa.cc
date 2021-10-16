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
#include "torque-generated/src/builtins/typed-array-of-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-createtypedarray-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-of-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"

namespace v8 {
namespace internal {
const char* kBuiltinNameOf_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  return "%TypedArray%.of";}

TF_BUILTIN(TypedArrayOf, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, UintPtrT, UintPtrT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<UintPtrT> tmp0;
  TNode<JSReceiver> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{torque_arguments.length});
    compiler::CodeAssemblerLabel label2(&ca_);
    tmp1 = Cast_Constructor_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label2);
    ca_.Goto(&block5);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kNotConstructor, TNode<Object>{parameter1});
  }

  TNode<Number> tmp3;
  TNode<JSTypedArray> tmp4;
  TNode<Int32T> tmp5;
  TNode<BuiltinPtr> tmp6;
  TNode<BuiltinPtr> tmp7;
  TNode<BuiltinPtr> tmp8;
  TNode<UintPtrT> tmp9;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp3 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{tmp0});
    tmp4 = TypedArrayCreateByLength_0(state_, TNode<Context>{parameter0}, TNode<JSReceiver>{tmp1}, TNode<Number>{tmp3}, kBuiltinNameOf_0(state_));
    tmp5 = CodeStubAssembler(state_).LoadElementsKind(TNode<JSTypedArray>{tmp4});
    std::tie(tmp6, tmp7, tmp8) = GetTypedArrayAccessor_0(state_, TNode<Int32T>{tmp5}).Flatten();
    tmp9 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block9, tmp9);
  }

  TNode<UintPtrT> phi_bb9_11;
  TNode<BoolT> tmp10;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_11);
    tmp10 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb9_11}, TNode<UintPtrT>{tmp0});
    ca_.Branch(tmp10, &block7, std::vector<compiler::Node*>{phi_bb9_11}, &block8, std::vector<compiler::Node*>{phi_bb9_11});
  }

  TNode<UintPtrT> phi_bb7_11;
  TNode<IntPtrT> tmp11;
  TNode<Object> tmp12;
  TNode<Smi> tmp13;
  TNode<Smi> tmp14;
  TNode<BoolT> tmp15;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_11);
    tmp11 = CodeStubAssembler(state_).Signed(TNode<UintPtrT>{phi_bb7_11});
    tmp12 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp11});
tmp13 = TORQUE_CAST(CodeStubAssembler(state_).CallBuiltinPointer(Builtins::CallableFor(ca_.isolate(),ExampleBuiltinForTorqueFunctionPointerType(3)).descriptor(), tmp8, parameter0, tmp4, phi_bb7_11, tmp12));
    tmp14 = kStoreFailureArrayDetached_0(state_);
    tmp15 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp13}, TNode<Smi>{tmp14});
    ca_.Branch(tmp15, &block12, std::vector<compiler::Node*>{phi_bb7_11, phi_bb7_11, phi_bb7_11}, &block13, std::vector<compiler::Node*>{phi_bb7_11, phi_bb7_11, phi_bb7_11});
  }

  TNode<UintPtrT> phi_bb12_11;
  TNode<UintPtrT> phi_bb12_18;
  TNode<UintPtrT> phi_bb12_22;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_11, &phi_bb12_18, &phi_bb12_22);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kDetachedOperation, kBuiltinNameOf_0(state_));
  }

  TNode<UintPtrT> phi_bb13_11;
  TNode<UintPtrT> phi_bb13_18;
  TNode<UintPtrT> phi_bb13_22;
  TNode<UintPtrT> tmp16;
  TNode<UintPtrT> tmp17;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_11, &phi_bb13_18, &phi_bb13_22);
    tmp16 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp17 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb13_11}, TNode<UintPtrT>{tmp16});
    ca_.Goto(&block9, tmp17);
  }

  TNode<UintPtrT> phi_bb8_11;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_11);
    arguments.PopAndReturn(tmp4);
  }
}

} // namespace internal
} // namespace v8
