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
#include "torque-generated/src/builtins/typed-array-filter-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/growable-fixed-array-tq-csa.h"
#include "torque-generated/src/builtins/torque-internal-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-createtypedarray-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-every-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-filter-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"
#include "torque-generated/src/objects/fixed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-buffer-tq-csa.h"
#include "torque-generated/src/objects/js-array-tq-csa.h"

namespace v8 {
namespace internal {
const char* kBuiltinNameFilter_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  return "%TypedArray%.prototype.filter";}

TF_BUILTIN(TypedArrayPrototypeFilter, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, JSTypedArray, UintPtrT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, JSTypedArray, UintPtrT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, JSTypedArray, UintPtrT> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, JSTypedArray, UintPtrT> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, UintPtrT> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, UintPtrT> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, UintPtrT> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block49(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, IntPtrT, IntPtrT, IntPtrT, IntPtrT> block50(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, UintPtrT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<FixedArray, IntPtrT, IntPtrT, JSTypedArray, UintPtrT> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_JSTypedArray_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label1);
    ca_.Goto(&block5);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kNotTypedArray, kBuiltinNameFilter_0(state_));
  }

  TNode<JSTypedArray> tmp2;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = EnsureAttached_0(state_, TNode<JSTypedArray>{tmp0}, &label3);
    ca_.Goto(&block7);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block8);
    }
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(&block2);
  }

  TNode<IntPtrT> tmp4;
  TNode<UintPtrT> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Object> tmp7;
  TNode<JSReceiver> tmp8;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp5 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp2, tmp4});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp7 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp6});
    compiler::CodeAssemblerLabel label9(&ca_);
    tmp8 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp7}, &label9);
    ca_.Goto(&block11);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block12);
    }
  }

  TNode<IntPtrT> tmp10;
  TNode<Object> tmp11;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp10 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp11 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp10});
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCalledNonCallable, TNode<Object>{tmp11});
  }

  TNode<IntPtrT> tmp12;
  TNode<Object> tmp13;
  TNode<FixedArray> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<IntPtrT> tmp16;
  TNode<JSTypedArray> tmp17;
  TNode<JSTypedArray> tmp18;
  TNode<BuiltinPtr> tmp19;
  TNode<UintPtrT> tmp20;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp12 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp13 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp12});
    std::tie(tmp14, tmp15, tmp16) = NewGrowableFixedArray_0(state_).Flatten();
    std::tie(tmp17, tmp18, tmp19) = NewAttachedJSTypedArrayWitness_0(state_, TNode<JSTypedArray>{tmp2}).Flatten();
    tmp20 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block15, tmp14, tmp15, tmp16, tmp18, tmp20);
  }

  TNode<FixedArray> phi_bb15_10;
  TNode<IntPtrT> phi_bb15_11;
  TNode<IntPtrT> phi_bb15_12;
  TNode<JSTypedArray> phi_bb15_14;
  TNode<UintPtrT> phi_bb15_16;
  TNode<BoolT> tmp21;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_10, &phi_bb15_11, &phi_bb15_12, &phi_bb15_14, &phi_bb15_16);
    tmp21 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb15_16}, TNode<UintPtrT>{tmp5});
    ca_.Branch(tmp21, &block13, std::vector<compiler::Node*>{phi_bb15_10, phi_bb15_11, phi_bb15_12, phi_bb15_14, phi_bb15_16}, &block14, std::vector<compiler::Node*>{phi_bb15_10, phi_bb15_11, phi_bb15_12, phi_bb15_14, phi_bb15_16});
  }

  TNode<FixedArray> phi_bb13_10;
  TNode<IntPtrT> phi_bb13_11;
  TNode<IntPtrT> phi_bb13_12;
  TNode<JSTypedArray> phi_bb13_14;
  TNode<UintPtrT> phi_bb13_16;
  TNode<IntPtrT> tmp22;
  TNode<JSArrayBuffer> tmp23;
  TNode<BoolT> tmp24;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_10, &phi_bb13_11, &phi_bb13_12, &phi_bb13_14, &phi_bb13_16);
    tmp22 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp23 = CodeStubAssembler(state_).LoadReference<JSArrayBuffer>(CodeStubAssembler::Reference{tmp17, tmp22});
    tmp24 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{tmp23});
    ca_.Branch(tmp24, &block18, std::vector<compiler::Node*>{phi_bb13_10, phi_bb13_11, phi_bb13_12, phi_bb13_14, phi_bb13_16}, &block19, std::vector<compiler::Node*>{phi_bb13_10, phi_bb13_11, phi_bb13_12, phi_bb13_14, phi_bb13_16});
  }

  TNode<FixedArray> phi_bb18_10;
  TNode<IntPtrT> phi_bb18_11;
  TNode<IntPtrT> phi_bb18_12;
  TNode<JSTypedArray> phi_bb18_14;
  TNode<UintPtrT> phi_bb18_16;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_10, &phi_bb18_11, &phi_bb18_12, &phi_bb18_14, &phi_bb18_16);
    ca_.Goto(&block2);
  }

  TNode<FixedArray> phi_bb19_10;
  TNode<IntPtrT> phi_bb19_11;
  TNode<IntPtrT> phi_bb19_12;
  TNode<JSTypedArray> phi_bb19_14;
  TNode<UintPtrT> phi_bb19_16;
  TNode<JSTypedArray> tmp25;
  TNode<Numeric> tmp26;
  TNode<Number> tmp27;
  TNode<Object> tmp28;
  TNode<BoolT> tmp29;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_10, &phi_bb19_11, &phi_bb19_12, &phi_bb19_14, &phi_bb19_16);
    tmp25 = (TNode<JSTypedArray>{tmp17});
tmp26 = TORQUE_CAST(CodeStubAssembler(state_).CallBuiltinPointer(Builtins::CallableFor(ca_.isolate(),ExampleBuiltinForTorqueFunctionPointerType(1)).descriptor(), tmp19, TNode<Object>(), tmp25, phi_bb19_16));
    tmp27 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{phi_bb19_16});
    tmp28 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp8}, TNode<Object>{tmp13}, TNode<Object>{tmp26}, TNode<Object>{tmp27}, TNode<Object>{tmp17});
    tmp29 = ToBoolean_0(state_, TNode<Object>{tmp28});
    ca_.Branch(tmp29, &block22, std::vector<compiler::Node*>{phi_bb19_10, phi_bb19_11, phi_bb19_12, phi_bb19_16}, &block23, std::vector<compiler::Node*>{phi_bb19_10, phi_bb19_11, phi_bb19_12, phi_bb19_16});
  }

  TNode<FixedArray> phi_bb22_10;
  TNode<IntPtrT> phi_bb22_11;
  TNode<IntPtrT> phi_bb22_12;
  TNode<UintPtrT> phi_bb22_16;
  TNode<BoolT> tmp30;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_10, &phi_bb22_11, &phi_bb22_12, &phi_bb22_16);
    tmp30 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{phi_bb22_11}, TNode<IntPtrT>{phi_bb22_12});
    ca_.Branch(tmp30, &block30, std::vector<compiler::Node*>{phi_bb22_10, phi_bb22_11, phi_bb22_12, phi_bb22_16}, &block31, std::vector<compiler::Node*>{phi_bb22_10, phi_bb22_11, phi_bb22_12, phi_bb22_16});
  }

  TNode<FixedArray> phi_bb30_10;
  TNode<IntPtrT> phi_bb30_11;
  TNode<IntPtrT> phi_bb30_12;
  TNode<UintPtrT> phi_bb30_16;
  TNode<IntPtrT> tmp31;
  TNode<IntPtrT> tmp32;
  TNode<IntPtrT> tmp33;
  TNode<IntPtrT> tmp34;
  TNode<IntPtrT> tmp35;
  TNode<IntPtrT> tmp36;
  TNode<FixedArray> tmp37;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_10, &phi_bb30_11, &phi_bb30_12, &phi_bb30_16);
    tmp31 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp32 = CodeStubAssembler(state_).WordSar(TNode<IntPtrT>{phi_bb30_11}, TNode<IntPtrT>{tmp31});
    tmp33 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb30_11}, TNode<IntPtrT>{tmp32});
    tmp34 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    tmp35 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp33}, TNode<IntPtrT>{tmp34});
    tmp36 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp37 = ExtractFixedArray_0(state_, TNode<FixedArray>{phi_bb30_10}, TNode<IntPtrT>{tmp36}, TNode<IntPtrT>{phi_bb30_12}, TNode<IntPtrT>{tmp35});
    ca_.Goto(&block31, tmp37, tmp35, phi_bb30_12, phi_bb30_16);
  }

  TNode<FixedArray> phi_bb31_10;
  TNode<IntPtrT> phi_bb31_11;
  TNode<IntPtrT> phi_bb31_12;
  TNode<UintPtrT> phi_bb31_16;
  TNode<Object> tmp38;
  TNode<IntPtrT> tmp39;
  TNode<IntPtrT> tmp40;
  TNode<IntPtrT> tmp41;
  TNode<IntPtrT> tmp42;
  TNode<UintPtrT> tmp43;
  TNode<UintPtrT> tmp44;
  TNode<BoolT> tmp45;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_10, &phi_bb31_11, &phi_bb31_12, &phi_bb31_16);
    std::tie(tmp38, tmp39, tmp40) = FieldSliceFixedArrayObjects_0(state_, TNode<FixedArray>{phi_bb31_10}).Flatten();
    tmp41 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp42 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{phi_bb31_12}, TNode<IntPtrT>{tmp41});
    tmp43 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{phi_bb31_12});
    tmp44 = Convert_uintptr_intptr_0(state_, TNode<IntPtrT>{tmp40});
    tmp45 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp43}, TNode<UintPtrT>{tmp44});
    ca_.Branch(tmp45, &block49, std::vector<compiler::Node*>{phi_bb31_16, phi_bb31_12, phi_bb31_12, phi_bb31_12, phi_bb31_12}, &block50, std::vector<compiler::Node*>{phi_bb31_16, phi_bb31_12, phi_bb31_12, phi_bb31_12, phi_bb31_12});
  }

  TNode<UintPtrT> phi_bb49_16;
  TNode<IntPtrT> phi_bb49_25;
  TNode<IntPtrT> phi_bb49_26;
  TNode<IntPtrT> phi_bb49_30;
  TNode<IntPtrT> phi_bb49_31;
  TNode<IntPtrT> tmp46;
  TNode<IntPtrT> tmp47;
  TNode<Object> tmp48;
  TNode<IntPtrT> tmp49;
  if (block49.is_used()) {
    ca_.Bind(&block49, &phi_bb49_16, &phi_bb49_25, &phi_bb49_26, &phi_bb49_30, &phi_bb49_31);
    tmp46 = TimesSizeOf_Object_0(state_, TNode<IntPtrT>{phi_bb49_31});
    tmp47 = CodeStubAssembler(state_).IntPtrAdd(TNode<IntPtrT>{tmp39}, TNode<IntPtrT>{tmp46});
    std::tie(tmp48, tmp49) = NewReference_Object_0(state_, TNode<Object>{tmp38}, TNode<IntPtrT>{tmp47}).Flatten();
    CodeStubAssembler(state_).StoreReference<Object>(CodeStubAssembler::Reference{tmp48, tmp49}, tmp26);
    ca_.Goto(&block23, phi_bb31_10, phi_bb31_11, tmp42, phi_bb49_16);
  }

  TNode<UintPtrT> phi_bb50_16;
  TNode<IntPtrT> phi_bb50_25;
  TNode<IntPtrT> phi_bb50_26;
  TNode<IntPtrT> phi_bb50_30;
  TNode<IntPtrT> phi_bb50_31;
  if (block50.is_used()) {
    ca_.Bind(&block50, &phi_bb50_16, &phi_bb50_25, &phi_bb50_26, &phi_bb50_30, &phi_bb50_31);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<FixedArray> phi_bb23_10;
  TNode<IntPtrT> phi_bb23_11;
  TNode<IntPtrT> phi_bb23_12;
  TNode<UintPtrT> phi_bb23_16;
  TNode<UintPtrT> tmp50;
  TNode<UintPtrT> tmp51;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_10, &phi_bb23_11, &phi_bb23_12, &phi_bb23_16);
    tmp50 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp51 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb23_16}, TNode<UintPtrT>{tmp50});
    ca_.Goto(&block15, phi_bb23_10, phi_bb23_11, phi_bb23_12, tmp25, tmp51);
  }

  TNode<FixedArray> phi_bb14_10;
  TNode<IntPtrT> phi_bb14_11;
  TNode<IntPtrT> phi_bb14_12;
  TNode<JSTypedArray> phi_bb14_14;
  TNode<UintPtrT> phi_bb14_16;
  TNode<UintPtrT> tmp52;
  TNode<JSTypedArray> tmp53;
  TNode<UintPtrT> tmp54;
  TNode<Number> tmp55;
  TNode<NativeContext> tmp56;
  TNode<Map> tmp57;
  TNode<IntPtrT> tmp58;
  TNode<FixedArray> tmp59;
  TNode<Smi> tmp60;
  TNode<JSArray> tmp61;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_10, &phi_bb14_11, &phi_bb14_12, &phi_bb14_14, &phi_bb14_16);
    tmp52 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{phi_bb14_12});
    tmp53 = TypedArraySpeciesCreateByLength_0(state_, TNode<Context>{parameter0}, kBuiltinNameFilter_0(state_), TNode<JSTypedArray>{tmp0}, TNode<UintPtrT>{tmp52});
    tmp54 = CodeStubAssembler(state_).Unsigned(TNode<IntPtrT>{phi_bb14_12});
    tmp55 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{tmp54});
    tmp56 = CodeStubAssembler(state_).LoadNativeContext(TNode<Context>{parameter0});
    tmp57 = CodeStubAssembler(state_).LoadJSArrayElementsMap(ElementsKind::PACKED_ELEMENTS, TNode<NativeContext>{tmp56});
    tmp58 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp59 = ExtractFixedArray_0(state_, TNode<FixedArray>{phi_bb14_10}, TNode<IntPtrT>{tmp58}, TNode<IntPtrT>{phi_bb14_12}, TNode<IntPtrT>{phi_bb14_12});
    tmp60 = Convert_Smi_intptr_0(state_, TNode<IntPtrT>{phi_bb14_12});
    tmp61 = CodeStubAssembler(state_).AllocateJSArray(TNode<Map>{tmp57}, TNode<FixedArrayBase>{tmp59}, TNode<Smi>{tmp60});
    CodeStubAssembler(state_).CallRuntime(Runtime::kTypedArrayCopyElements, parameter0, tmp53, tmp61, tmp55);
    arguments.PopAndReturn(tmp53);
  }

  if (block2.is_used()) {
    ca_.Bind(&block2);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kDetachedOperation, kBuiltinNameFilter_0(state_));
  }
}

} // namespace internal
} // namespace v8
