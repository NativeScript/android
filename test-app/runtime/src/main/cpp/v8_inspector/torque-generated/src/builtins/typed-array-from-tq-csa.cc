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
#include "torque-generated/src/builtins/typed-array-from-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/array-isarray-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-createtypedarray-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-every-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-from-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-buffer-tq-csa.h"
#include "torque-generated/src/objects/js-function-tq-csa.h"

namespace v8 {
namespace internal {
const char* kBuiltinNameFrom_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  return "%TypedArray%.from";}

TF_BUILTIN(TypedArrayFrom, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block36(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block35(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block39(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object> block47(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object> block48(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object> block52(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object> block51(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object, UintPtrT> block55(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object, UintPtrT> block53(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object, UintPtrT> block57(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object, UintPtrT> block58(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object, UintPtrT, Object> block59(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object, UintPtrT, UintPtrT, UintPtrT> block61(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object, UintPtrT, UintPtrT, UintPtrT> block62(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT, Object, UintPtrT> block54(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Object> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<Object> tmp5;
  TNode<JSReceiver> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp3 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp2});
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 2);
    tmp5 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp4});
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = Cast_Constructor_1(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, &label7);
    ca_.Goto(&block5);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kNotConstructor, TNode<Object>{parameter1});
  }

  TNode<Oddball> tmp8;
  TNode<BoolT> tmp9;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp8 = Undefined_0(state_);
    tmp9 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{tmp3}, TNode<HeapObject>{tmp8});
    ca_.Branch(tmp9, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp10;
  TNode<BoolT> tmp11;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp10 = Is_Callable_JSAny_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp3});
    tmp11 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp10});
    ca_.Goto(&block11, tmp11);
  }

  TNode<BoolT> tmp12;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp12 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block11, tmp12);
  }

  TNode<BoolT> phi_bb11_11;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_11);
    ca_.Branch(phi_bb11_11, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{});
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCalledNonCallable, TNode<Object>{tmp3});
  }

  TNode<Symbol> tmp13;
  TNode<JSReceiver> tmp14;
    compiler::TypedCodeAssemblerVariable<Object> tmp17(&ca_);
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp13 = CodeStubAssembler(state_).IteratorSymbolConstant();
    compiler::CodeAssemblerLabel label15(&ca_);
    compiler::CodeAssemblerLabel label16(&ca_);
    tmp14 = GetMethod_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp1}, TNode<Name>{tmp13}, &label15, &label16, &tmp17);
    ca_.Goto(&block16);
    if (label15.is_used()) {
      ca_.Bind(&label15);
      ca_.Goto(&block17);
    }
    if (label16.is_used()) {
      ca_.Bind(&label16);
      ca_.Goto(&block18);
    }
  }

  TNode<JSReceiver> tmp18;
  TNode<Number> tmp19;
  TNode<UintPtrT> tmp20;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp18 = CodeStubAssembler(state_).ToObject_Inline(TNode<Context>{parameter0}, TNode<Object>{tmp1});
    tmp19 = GetLengthProperty_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp18});
    compiler::CodeAssemblerLabel label21(&ca_);
    tmp20 = ChangeSafeIntegerNumberToUintPtr_0(state_, TNode<Number>{tmp19}, &label21);
    ca_.Goto(&block43);
    if (label21.is_used()) {
      ca_.Bind(&label21);
      ca_.Goto(&block44);
    }
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kIteratorSymbolNonCallable);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Branch(tmp9, &block21, std::vector<compiler::Node*>{}, &block22, std::vector<compiler::Node*>{});
  }

  if (block21.is_used()) {
    ca_.Bind(&block21);
    ca_.Goto(&block20);
  }

  TNode<JSFunction> tmp22;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    compiler::CodeAssemblerLabel label23(&ca_);
    tmp22 = Cast_JSFunction_0(state_, TNode<HeapObject>{tmp14}, &label23);
    ca_.Goto(&block23);
    if (label23.is_used()) {
      ca_.Bind(&label23);
      ca_.Goto(&block24);
    }
  }

  if (block24.is_used()) {
    ca_.Bind(&block24);
    ca_.Goto(&block20);
  }

  TNode<BoolT> tmp24;
  if (block23.is_used()) {
    ca_.Bind(&block23);
    tmp24 = CodeStubAssembler(state_).IsArrayIteratorProtectorCellInvalid();
    ca_.Branch(tmp24, &block25, std::vector<compiler::Node*>{}, &block26, std::vector<compiler::Node*>{});
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    ca_.Goto(&block20);
  }

  TNode<JSArray> tmp25;
  if (block26.is_used()) {
    ca_.Bind(&block26);
    compiler::CodeAssemblerLabel label26(&ca_);
    tmp25 = Cast_JSArray_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp1}, &label26);
    ca_.Goto(&block29);
    if (label26.is_used()) {
      ca_.Bind(&label26);
      ca_.Goto(&block30);
    }
  }

  TNode<JSTypedArray> tmp27;
  if (block30.is_used()) {
    ca_.Bind(&block30);
    compiler::CodeAssemblerLabel label28(&ca_);
    tmp27 = Cast_JSTypedArray_1(state_, TNode<Context>{parameter0}, TNode<Object>{ca_.UncheckedCast<Object>(tmp1)}, &label28);
    ca_.Goto(&block35);
    if (label28.is_used()) {
      ca_.Bind(&label28);
      ca_.Goto(&block36);
    }
  }

  TNode<IntPtrT> tmp29;
  TNode<SharedFunctionInfo> tmp30;
  TNode<IntPtrT> tmp31;
  TNode<Object> tmp32;
  TNode<Smi> tmp33;
  TNode<BoolT> tmp34;
  TNode<BoolT> tmp35;
  if (block29.is_used()) {
    ca_.Bind(&block29);
    tmp29 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp30 = CodeStubAssembler(state_).LoadReference<SharedFunctionInfo>(CodeStubAssembler::Reference{tmp22, tmp29});
    tmp31 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp32 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp30, tmp31});
    tmp33 = CodeStubAssembler(state_).SmiConstant(Builtin::kArrayPrototypeValues);
    tmp34 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{tmp32}, TNode<MaybeObject>{tmp33});
    tmp35 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp34});
    ca_.Branch(tmp35, &block31, std::vector<compiler::Node*>{}, &block32, std::vector<compiler::Node*>{});
  }

  if (block31.is_used()) {
    ca_.Bind(&block31);
    ca_.Goto(&block20);
  }

  TNode<IntPtrT> tmp36;
  TNode<Number> tmp37;
  TNode<UintPtrT> tmp38;
  if (block32.is_used()) {
    ca_.Bind(&block32);
    tmp36 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp37 = CodeStubAssembler(state_).LoadReference<Number>(CodeStubAssembler::Reference{tmp25, tmp36});
    tmp38 = Convert_uintptr_Number_0(state_, TNode<Number>{tmp37});
    ca_.Goto(&block27, tmp38);
  }

  if (block36.is_used()) {
    ca_.Bind(&block36);
    ca_.Goto(&block20);
  }

  TNode<IntPtrT> tmp39;
  TNode<JSArrayBuffer> tmp40;
  TNode<BoolT> tmp41;
  if (block35.is_used()) {
    ca_.Bind(&block35);
    tmp39 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp40 = CodeStubAssembler(state_).LoadReference<JSArrayBuffer>(CodeStubAssembler::Reference{tmp27, tmp39});
    tmp41 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{tmp40});
    ca_.Branch(tmp41, &block37, std::vector<compiler::Node*>{}, &block38, std::vector<compiler::Node*>{});
  }

  if (block37.is_used()) {
    ca_.Bind(&block37);
    ca_.Goto(&block20);
  }

  TNode<IntPtrT> tmp42;
  TNode<SharedFunctionInfo> tmp43;
  TNode<IntPtrT> tmp44;
  TNode<Object> tmp45;
  TNode<Smi> tmp46;
  TNode<BoolT> tmp47;
  TNode<BoolT> tmp48;
  if (block38.is_used()) {
    ca_.Bind(&block38);
    tmp42 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp43 = CodeStubAssembler(state_).LoadReference<SharedFunctionInfo>(CodeStubAssembler::Reference{tmp22, tmp42});
    tmp44 = FromConstexpr_intptr_constexpr_int31_0(state_, 4);
    tmp45 = CodeStubAssembler(state_).LoadReference<Object>(CodeStubAssembler::Reference{tmp43, tmp44});
    tmp46 = CodeStubAssembler(state_).SmiConstant(Builtin::kTypedArrayPrototypeValues);
    tmp47 = CodeStubAssembler(state_).TaggedEqual(TNode<MaybeObject>{tmp45}, TNode<MaybeObject>{tmp46});
    tmp48 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp47});
    ca_.Branch(tmp48, &block39, std::vector<compiler::Node*>{}, &block40, std::vector<compiler::Node*>{});
  }

  if (block39.is_used()) {
    ca_.Bind(&block39);
    ca_.Goto(&block20);
  }

  TNode<IntPtrT> tmp49;
  TNode<UintPtrT> tmp50;
  if (block40.is_used()) {
    ca_.Bind(&block40);
    tmp49 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp50 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp27, tmp49});
    ca_.Goto(&block27, tmp50);
  }

  TNode<UintPtrT> phi_bb27_10;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_10);
    ca_.Goto(&block19, phi_bb27_10, tmp1);
  }

  TNode<JSArray> tmp51;
  TNode<IntPtrT> tmp52;
  TNode<Number> tmp53;
  TNode<UintPtrT> tmp54;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp51 = ca_.CallStub<JSArray>(Builtins::CallableFor(ca_.isolate(), Builtin::kIterableToList), parameter0, tmp1, tmp14);
    tmp52 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp53 = CodeStubAssembler(state_).LoadReference<Number>(CodeStubAssembler::Reference{tmp51, tmp52});
    tmp54 = Convert_uintptr_Number_0(state_, TNode<Number>{tmp53});
    ca_.Goto(&block19, tmp54, tmp51);
  }

  TNode<UintPtrT> phi_bb19_10;
  TNode<Object> phi_bb19_11;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_10, &phi_bb19_11);
    ca_.Goto(&block14, phi_bb19_10, phi_bb19_11);
  }

  if (block44.is_used()) {
    ca_.Bind(&block44);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{parameter0}, MessageTemplate::kInvalidTypedArrayLength, TNode<Object>{tmp19});
  }

  if (block43.is_used()) {
    ca_.Bind(&block43);
    ca_.Goto(&block14, tmp20, tmp18);
  }

  TNode<UintPtrT> phi_bb14_10;
  TNode<Object> phi_bb14_11;
  TNode<Number> tmp55;
  TNode<JSTypedArray> tmp56;
  TNode<BoolT> tmp57;
  if (block14.is_used()) {
    ca_.Bind(&block14, &phi_bb14_10, &phi_bb14_11);
    tmp55 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{phi_bb14_10});
    tmp56 = TypedArrayCreateByLength_0(state_, TNode<Context>{parameter0}, TNode<JSReceiver>{tmp6}, TNode<Number>{tmp55}, kBuiltinNameFrom_0(state_));
    tmp57 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp9});
    ca_.Branch(tmp57, &block45, std::vector<compiler::Node*>{phi_bb14_10, phi_bb14_11}, &block46, std::vector<compiler::Node*>{phi_bb14_10, phi_bb14_11});
  }

  TNode<UintPtrT> phi_bb45_10;
  TNode<Object> phi_bb45_11;
  TNode<UintPtrT> tmp58;
  TNode<BoolT> tmp59;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_10, &phi_bb45_11);
    tmp58 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp59 = CodeStubAssembler(state_).WordNotEqual(TNode<UintPtrT>{phi_bb45_10}, TNode<UintPtrT>{tmp58});
    ca_.Branch(tmp59, &block47, std::vector<compiler::Node*>{phi_bb45_10, phi_bb45_11}, &block48, std::vector<compiler::Node*>{phi_bb45_10, phi_bb45_11});
  }

  TNode<UintPtrT> phi_bb47_10;
  TNode<Object> phi_bb47_11;
  if (block47.is_used()) {
    ca_.Bind(&block47, &phi_bb47_10, &phi_bb47_11);
    CodeStubAssembler(state_).CallRuntime(Runtime::kTypedArrayCopyElements, parameter0, tmp56, phi_bb47_11, tmp55);
    ca_.Goto(&block48, phi_bb47_10, phi_bb47_11);
  }

  TNode<UintPtrT> phi_bb48_10;
  TNode<Object> phi_bb48_11;
  if (block48.is_used()) {
    ca_.Bind(&block48, &phi_bb48_10, &phi_bb48_11);
    arguments.PopAndReturn(tmp56);
  }

  TNode<UintPtrT> phi_bb46_10;
  TNode<Object> phi_bb46_11;
  TNode<JSReceiver> tmp60;
  if (block46.is_used()) {
    ca_.Bind(&block46, &phi_bb46_10, &phi_bb46_11);
    compiler::CodeAssemblerLabel label61(&ca_);
    tmp60 = Cast_Callable_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp3}, &label61);
    ca_.Goto(&block51, phi_bb46_10, phi_bb46_11);
    if (label61.is_used()) {
      ca_.Bind(&label61);
      ca_.Goto(&block52, phi_bb46_10, phi_bb46_11);
    }
  }

  TNode<UintPtrT> phi_bb52_10;
  TNode<Object> phi_bb52_11;
  if (block52.is_used()) {
    ca_.Bind(&block52, &phi_bb52_10, &phi_bb52_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<UintPtrT> phi_bb51_10;
  TNode<Object> phi_bb51_11;
  TNode<Int32T> tmp62;
  TNode<BuiltinPtr> tmp63;
  TNode<BuiltinPtr> tmp64;
  TNode<BuiltinPtr> tmp65;
  TNode<UintPtrT> tmp66;
  if (block51.is_used()) {
    ca_.Bind(&block51, &phi_bb51_10, &phi_bb51_11);
    tmp62 = CodeStubAssembler(state_).LoadElementsKind(TNode<JSTypedArray>{tmp56});
    std::tie(tmp63, tmp64, tmp65) = GetTypedArrayAccessor_0(state_, TNode<Int32T>{tmp62}).Flatten();
    tmp66 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block55, phi_bb51_10, phi_bb51_11, tmp66);
  }

  TNode<UintPtrT> phi_bb55_10;
  TNode<Object> phi_bb55_11;
  TNode<UintPtrT> phi_bb55_18;
  TNode<BoolT> tmp67;
  if (block55.is_used()) {
    ca_.Bind(&block55, &phi_bb55_10, &phi_bb55_11, &phi_bb55_18);
    tmp67 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb55_18}, TNode<UintPtrT>{phi_bb55_10});
    ca_.Branch(tmp67, &block53, std::vector<compiler::Node*>{phi_bb55_10, phi_bb55_11, phi_bb55_18}, &block54, std::vector<compiler::Node*>{phi_bb55_10, phi_bb55_11, phi_bb55_18});
  }

  TNode<UintPtrT> phi_bb53_10;
  TNode<Object> phi_bb53_11;
  TNode<UintPtrT> phi_bb53_18;
  TNode<Number> tmp68;
  TNode<Object> tmp69;
  if (block53.is_used()) {
    ca_.Bind(&block53, &phi_bb53_10, &phi_bb53_11, &phi_bb53_18);
    tmp68 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{phi_bb53_18});
    tmp69 = CodeStubAssembler(state_).GetProperty(TNode<Context>{parameter0}, TNode<Object>{phi_bb53_11}, TNode<Object>{tmp68});
    ca_.Branch(tmp9, &block57, std::vector<compiler::Node*>{phi_bb53_10, phi_bb53_11, phi_bb53_18}, &block58, std::vector<compiler::Node*>{phi_bb53_10, phi_bb53_11, phi_bb53_18});
  }

  TNode<UintPtrT> phi_bb57_10;
  TNode<Object> phi_bb57_11;
  TNode<UintPtrT> phi_bb57_18;
  TNode<Object> tmp70;
  if (block57.is_used()) {
    ca_.Bind(&block57, &phi_bb57_10, &phi_bb57_11, &phi_bb57_18);
    tmp70 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp60}, TNode<Object>{tmp5}, TNode<Object>{tmp69}, TNode<Object>{tmp68});
    ca_.Goto(&block59, phi_bb57_10, phi_bb57_11, phi_bb57_18, tmp70);
  }

  TNode<UintPtrT> phi_bb58_10;
  TNode<Object> phi_bb58_11;
  TNode<UintPtrT> phi_bb58_18;
  if (block58.is_used()) {
    ca_.Bind(&block58, &phi_bb58_10, &phi_bb58_11, &phi_bb58_18);
    ca_.Goto(&block59, phi_bb58_10, phi_bb58_11, phi_bb58_18, tmp69);
  }

  TNode<UintPtrT> phi_bb59_10;
  TNode<Object> phi_bb59_11;
  TNode<UintPtrT> phi_bb59_18;
  TNode<Object> phi_bb59_21;
  TNode<Smi> tmp71;
  TNode<Smi> tmp72;
  TNode<BoolT> tmp73;
  if (block59.is_used()) {
    ca_.Bind(&block59, &phi_bb59_10, &phi_bb59_11, &phi_bb59_18, &phi_bb59_21);
tmp71 = TORQUE_CAST(CodeStubAssembler(state_).CallBuiltinPointer(Builtins::CallableFor(ca_.isolate(),ExampleBuiltinForTorqueFunctionPointerType(3)).descriptor(), tmp65, parameter0, tmp56, phi_bb59_18, phi_bb59_21));
    tmp72 = kStoreFailureArrayDetached_0(state_);
    tmp73 = CodeStubAssembler(state_).SmiEqual(TNode<Smi>{tmp71}, TNode<Smi>{tmp72});
    ca_.Branch(tmp73, &block61, std::vector<compiler::Node*>{phi_bb59_10, phi_bb59_11, phi_bb59_18, phi_bb59_18, phi_bb59_18}, &block62, std::vector<compiler::Node*>{phi_bb59_10, phi_bb59_11, phi_bb59_18, phi_bb59_18, phi_bb59_18});
  }

  TNode<UintPtrT> phi_bb61_10;
  TNode<Object> phi_bb61_11;
  TNode<UintPtrT> phi_bb61_18;
  TNode<UintPtrT> phi_bb61_27;
  TNode<UintPtrT> phi_bb61_31;
  if (block61.is_used()) {
    ca_.Bind(&block61, &phi_bb61_10, &phi_bb61_11, &phi_bb61_18, &phi_bb61_27, &phi_bb61_31);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kDetachedOperation, kBuiltinNameFrom_0(state_));
  }

  TNode<UintPtrT> phi_bb62_10;
  TNode<Object> phi_bb62_11;
  TNode<UintPtrT> phi_bb62_18;
  TNode<UintPtrT> phi_bb62_27;
  TNode<UintPtrT> phi_bb62_31;
  TNode<UintPtrT> tmp74;
  TNode<UintPtrT> tmp75;
  if (block62.is_used()) {
    ca_.Bind(&block62, &phi_bb62_10, &phi_bb62_11, &phi_bb62_18, &phi_bb62_27, &phi_bb62_31);
    tmp74 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp75 = CodeStubAssembler(state_).UintPtrAdd(TNode<UintPtrT>{phi_bb62_18}, TNode<UintPtrT>{tmp74});
    ca_.Goto(&block55, phi_bb62_10, phi_bb62_11, tmp75);
  }

  TNode<UintPtrT> phi_bb54_10;
  TNode<Object> phi_bb54_11;
  TNode<UintPtrT> phi_bb54_18;
  if (block54.is_used()) {
    ca_.Bind(&block54, &phi_bb54_10, &phi_bb54_11, &phi_bb54_18);
    arguments.PopAndReturn(tmp56);
  }
}

} // namespace internal
} // namespace v8
