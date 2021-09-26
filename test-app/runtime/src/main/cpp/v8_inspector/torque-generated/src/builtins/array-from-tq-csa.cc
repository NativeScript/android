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
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/array-every-tq-csa.h"
#include "torque-generated/src/builtins/array-from-tq-csa.h"
#include "torque-generated/src/builtins/array-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/iterator-tq-csa.h"
#include "torque-generated/src/builtins/promise-abstract-operations-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(ArrayFrom, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<Object> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, JSReceiver> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block39(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block46(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi, Smi> block47(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi, Object> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi, Object> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi, Smi> block50(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block54(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block53(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, JSReceiver> block51(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block57(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block55(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block58(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block59(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi, Object> block60(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, Smi> block56(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).HasBuiltinSubclassingFlag();
    ca_.Branch(tmp0, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{});
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(&block3, parameter1);
  }

  TNode<JSFunction> tmp1;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp1 = GetArrayFunction_0(state_, TNode<Context>{parameter0});
    ca_.Goto(&block3, tmp1);
  }

  TNode<Object> phi_bb3_5;
  TNode<IntPtrT> tmp2;
  TNode<BoolT> tmp3;
  if (block3.is_used()) {
    ca_.Bind(&block3, &phi_bb3_5);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp3 = CodeStubAssembler(state_).WordEqual(TNode<IntPtrT>{torque_arguments.length}, TNode<IntPtrT>{tmp2});
    ca_.Branch(tmp3, &block7, std::vector<compiler::Node*>{}, &block8, std::vector<compiler::Node*>{});
  }

  TNode<JSFunction> tmp4;
  TNode<BoolT> tmp5;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp4 = GetArrayFunction_0(state_, TNode<Context>{parameter0});
    tmp5 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{phi_bb3_5}, TNode<HeapObject>{tmp4});
    ca_.Goto(&block9, tmp5);
  }

  TNode<BoolT> tmp6;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp6 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block9, tmp6);
  }

  TNode<BoolT> phi_bb9_7;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_7);
    ca_.Branch(phi_bb9_7, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp7;
  TNode<Object> tmp8;
  TNode<JSArray> tmp9;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp8 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp7});
    compiler::CodeAssemblerLabel label10(&ca_);
    tmp9 = IteratorBuiltinsAssembler(state_).FastIterableToList(TNode<Context>{parameter0}, TNode<Object>{tmp8}, &label10);
    ca_.Goto(&block12);
    if (label10.is_used()) {
      ca_.Bind(&label10);
      ca_.Goto(&block13);
    }
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block6);
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    arguments.PopAndReturn(tmp9);
  }

  TNode<IntPtrT> tmp11;
  TNode<Object> tmp12;
  TNode<IntPtrT> tmp13;
  TNode<Object> tmp14;
  TNode<IntPtrT> tmp15;
  TNode<Object> tmp16;
  TNode<Oddball> tmp17;
  TNode<BoolT> tmp18;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp12 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp11});
    tmp13 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp14 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp13});
    tmp15 = FromConstexpr_intptr_constexpr_int31_0(state_, 2);
    tmp16 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp15});
    tmp17 = Undefined_0(state_);
    tmp18 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{tmp14}, TNode<HeapObject>{tmp17});
    ca_.Branch(tmp18, &block14, std::vector<compiler::Node*>{}, &block15, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp19;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp19 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block16, tmp19);
  }

  TNode<BoolT> tmp20;
  TNode<BoolT> tmp21;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp20 = Is_Callable_JSAny_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp14});
    tmp21 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp20});
    ca_.Branch(tmp21, &block17, std::vector<compiler::Node*>{}, &block18, std::vector<compiler::Node*>{});
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kCalledNonCallable, TNode<Object>{tmp14});
  }

  TNode<BoolT> tmp22;
  if (block18.is_used()) {
    ca_.Bind(&block18);
    tmp22 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block16, tmp22);
  }

  TNode<BoolT> phi_bb16_9;
  TNode<Symbol> tmp23;
  TNode<JSReceiver> tmp24;
    compiler::TypedCodeAssemblerVariable<Object> tmp27(&ca_);
  if (block16.is_used()) {
    ca_.Bind(&block16, &phi_bb16_9);
    tmp23 = CodeStubAssembler(state_).IteratorSymbolConstant();
    compiler::CodeAssemblerLabel label25(&ca_);
    compiler::CodeAssemblerLabel label26(&ca_);
    tmp24 = GetMethod_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp12}, TNode<Name>{tmp23}, &label25, &label26, &tmp27);
    ca_.Goto(&block23, phi_bb16_9);
    if (label25.is_used()) {
      ca_.Bind(&label25);
      ca_.Goto(&block24, phi_bb16_9);
    }
    if (label26.is_used()) {
      ca_.Bind(&label26);
      ca_.Goto(&block25, phi_bb16_9);
    }
  }

  TNode<BoolT> phi_bb24_9;
  TNode<JSReceiver> tmp28;
  TNode<Number> tmp29;
  TNode<JSReceiver> tmp30;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_9);
    tmp28 = CodeStubAssembler(state_).ToObject_Inline(TNode<Context>{parameter0}, TNode<Object>{tmp12});
    tmp29 = GetLengthProperty_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp28});
    compiler::CodeAssemblerLabel label31(&ca_);
    tmp30 = Cast_Constructor_1(state_, TNode<Context>{parameter0}, TNode<Object>{phi_bb3_5}, &label31);
    ca_.Goto(&block53, phi_bb24_9);
    if (label31.is_used()) {
      ca_.Bind(&label31);
      ca_.Goto(&block54, phi_bb24_9);
    }
  }

  TNode<BoolT> phi_bb25_9;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_9);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kIteratorSymbolNonCallable);
  }

  TNode<BoolT> phi_bb23_9;
  TNode<JSReceiver> tmp32;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_9);
    compiler::CodeAssemblerLabel label33(&ca_);
    tmp32 = Cast_Constructor_1(state_, TNode<Context>{parameter0}, TNode<Object>{phi_bb3_5}, &label33);
    ca_.Goto(&block28, phi_bb23_9);
    if (label33.is_used()) {
      ca_.Bind(&label33);
      ca_.Goto(&block29, phi_bb23_9);
    }
  }

  TNode<BoolT> phi_bb29_9;
  TNode<Number> tmp34;
  TNode<JSArray> tmp35;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_9);
    tmp34 = FromConstexpr_Number_constexpr_int31_0(state_, 0);
    tmp35 = CodeStubAssembler(state_).ArrayCreate(TNode<Context>{parameter0}, TNode<Number>{tmp34});
    ca_.Goto(&block26, phi_bb29_9, tmp35);
  }

  TNode<BoolT> phi_bb28_9;
  TNode<JSReceiver> tmp36;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_9);
    tmp36 = CodeStubAssembler(state_).Construct(TNode<Context>{parameter0}, TNode<JSReceiver>{tmp32});
    ca_.Goto(&block26, phi_bb28_9, tmp36);
  }

  TNode<BoolT> phi_bb26_9;
  TNode<JSReceiver> phi_bb26_11;
  TNode<JSReceiver> tmp37;
  TNode<Object> tmp38;
  TNode<Map> tmp39;
  TNode<Smi> tmp40;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_9, &phi_bb26_11);
    std::tie(tmp37, tmp38) = IteratorBuiltinsAssembler(state_).GetIterator(TNode<Context>{parameter0}, TNode<Object>{tmp12}, TNode<Object>{tmp24}).Flatten();
    tmp39 = GetIteratorResultMap_0(state_, TNode<Context>{parameter0});
    tmp40 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    ca_.Goto(&block32, phi_bb26_9, tmp40);
  }

  TNode<BoolT> phi_bb32_9;
  TNode<Smi> phi_bb32_15;
  TNode<BoolT> tmp41;
  if (block32.is_used()) {
    ca_.Bind(&block32, &phi_bb32_9, &phi_bb32_15);
    tmp41 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Branch(tmp41, &block30, std::vector<compiler::Node*>{phi_bb32_9, phi_bb32_15}, &block31, std::vector<compiler::Node*>{phi_bb32_9, phi_bb32_15});
  }

  TNode<BoolT> phi_bb30_9;
  TNode<Smi> phi_bb30_15;
  TNode<JSReceiver> tmp42;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_9, &phi_bb30_15);
    compiler::CodeAssemblerLabel label43(&ca_);
    tmp42 = IteratorBuiltinsAssembler(state_).IteratorStep(TNode<Context>{parameter0}, TorqueStructIteratorRecord{TNode<JSReceiver>{tmp37}, TNode<Object>{tmp38}}, TNode<Map>{tmp39}, &label43);
    ca_.Goto(&block39, phi_bb30_9, phi_bb30_15);
    if (label43.is_used()) {
      ca_.Bind(&label43);
      ca_.Goto(&block40, phi_bb30_9, phi_bb30_15);
    }
  }

  TNode<BoolT> phi_bb40_9;
  TNode<Smi> phi_bb40_15;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_9, &phi_bb40_15);
    CodeStubAssembler(state_).SetPropertyLength(TNode<Context>{parameter0}, TNode<Object>{phi_bb26_11}, TNode<Number>{phi_bb40_15});
    arguments.PopAndReturn(phi_bb26_11);
  }

  TNode<BoolT> phi_bb39_9;
  TNode<Smi> phi_bb39_15;
  TNode<Object> tmp44;
  if (block39.is_used()) {
    ca_.Bind(&block39, &phi_bb39_9, &phi_bb39_15);
    tmp44 = IteratorBuiltinsAssembler(state_).IteratorValue(TNode<Context>{parameter0}, TNode<JSReceiver>{tmp42}, TNode<Map>{tmp39});
    ca_.Branch(phi_bb39_9, &block41, std::vector<compiler::Node*>{phi_bb39_9, phi_bb39_15}, &block42, std::vector<compiler::Node*>{phi_bb39_9, phi_bb39_15});
  }

  TNode<BoolT> phi_bb41_9;
  TNode<Smi> phi_bb41_15;
  TNode<JSReceiver> tmp45;
      TNode<Object> tmp47;
  TNode<Object> tmp48;
      TNode<Object> tmp50;
  if (block41.is_used()) {
    ca_.Bind(&block41, &phi_bb41_9, &phi_bb41_15);
    compiler::CodeAssemblerExceptionHandlerLabel catch46__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch46__label);
    tmp45 = UnsafeCast_Callable_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp14});
    }
    if (catch46__label.is_used()) {
      compiler::CodeAssemblerLabel catch46_skip(&ca_);
      ca_.Goto(&catch46_skip);
      ca_.Bind(&catch46__label, &tmp47);
      ca_.Goto(&block46, phi_bb41_9, phi_bb41_15);
      ca_.Bind(&catch46_skip);
    }
    compiler::CodeAssemblerExceptionHandlerLabel catch49__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch49__label);
    tmp48 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp45}, TNode<Object>{tmp16}, TNode<Object>{tmp44}, TNode<Object>{phi_bb41_15});
    }
    if (catch49__label.is_used()) {
      compiler::CodeAssemblerLabel catch49_skip(&ca_);
      ca_.Goto(&catch49_skip);
      ca_.Bind(&catch49__label, &tmp50);
      ca_.Goto(&block47, phi_bb41_9, phi_bb41_15, phi_bb41_15);
      ca_.Bind(&catch49_skip);
    }
    ca_.Goto(&block43, phi_bb41_9, phi_bb41_15, tmp48);
  }

  TNode<BoolT> phi_bb46_9;
  TNode<Smi> phi_bb46_15;
  if (block46.is_used()) {
    ca_.Bind(&block46, &phi_bb46_9, &phi_bb46_15);
    ca_.Goto(&block45, phi_bb46_9, phi_bb46_15, tmp47);
  }

  TNode<BoolT> phi_bb47_9;
  TNode<Smi> phi_bb47_15;
  TNode<Smi> phi_bb47_23;
  if (block47.is_used()) {
    ca_.Bind(&block47, &phi_bb47_9, &phi_bb47_15, &phi_bb47_23);
    ca_.Goto(&block45, phi_bb47_9, phi_bb47_15, tmp50);
  }

  TNode<BoolT> phi_bb45_9;
  TNode<Smi> phi_bb45_15;
  TNode<Object> phi_bb45_19;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_9, &phi_bb45_15, &phi_bb45_19);
    IteratorCloseOnException_0(state_, TNode<Context>{parameter0}, TorqueStructIteratorRecord{TNode<JSReceiver>{tmp37}, TNode<Object>{tmp38}});
    CodeStubAssembler(state_).CallRuntime(Runtime::kReThrow, parameter0, phi_bb45_19);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BoolT> phi_bb42_9;
  TNode<Smi> phi_bb42_15;
  if (block42.is_used()) {
    ca_.Bind(&block42, &phi_bb42_9, &phi_bb42_15);
    ca_.Goto(&block43, phi_bb42_9, phi_bb42_15, tmp44);
  }

  TNode<BoolT> phi_bb43_9;
  TNode<Smi> phi_bb43_15;
  TNode<Object> phi_bb43_18;
  TNode<Object> tmp51;
      TNode<Object> tmp53;
  TNode<Smi> tmp54;
  TNode<Smi> tmp55;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_9, &phi_bb43_15, &phi_bb43_18);
    compiler::CodeAssemblerExceptionHandlerLabel catch52__label(&ca_, compiler::CodeAssemblerLabel::kDeferred);
    { compiler::ScopedExceptionHandler s(&ca_, &catch52__label);
    tmp51 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kFastCreateDataProperty), parameter0, phi_bb26_11, phi_bb43_15, phi_bb43_18);
    }
    if (catch52__label.is_used()) {
      compiler::CodeAssemblerLabel catch52_skip(&ca_);
      ca_.Goto(&catch52_skip);
      ca_.Bind(&catch52__label, &tmp53);
      ca_.Goto(&block50, phi_bb43_9, phi_bb43_15, phi_bb43_15);
      ca_.Bind(&catch52_skip);
    }
    tmp54 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp55 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb43_15}, TNode<Smi>{tmp54});
    ca_.Goto(&block32, phi_bb43_9, tmp55);
  }

  TNode<BoolT> phi_bb50_9;
  TNode<Smi> phi_bb50_15;
  TNode<Smi> phi_bb50_20;
  if (block50.is_used()) {
    ca_.Bind(&block50, &phi_bb50_9, &phi_bb50_15, &phi_bb50_20);
    IteratorCloseOnException_0(state_, TNode<Context>{parameter0}, TorqueStructIteratorRecord{TNode<JSReceiver>{tmp37}, TNode<Object>{tmp38}});
    CodeStubAssembler(state_).CallRuntime(Runtime::kReThrow, parameter0, tmp53);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BoolT> phi_bb31_9;
  TNode<Smi> phi_bb31_15;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_9, &phi_bb31_15);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BoolT> phi_bb54_9;
  TNode<JSArray> tmp56;
  if (block54.is_used()) {
    ca_.Bind(&block54, &phi_bb54_9);
    tmp56 = CodeStubAssembler(state_).ArrayCreate(TNode<Context>{parameter0}, TNode<Number>{tmp29});
    ca_.Goto(&block51, phi_bb54_9, tmp56);
  }

  TNode<BoolT> phi_bb53_9;
  TNode<JSReceiver> tmp57;
  if (block53.is_used()) {
    ca_.Bind(&block53, &phi_bb53_9);
    tmp57 = CodeStubAssembler(state_).Construct(TNode<Context>{parameter0}, TNode<JSReceiver>{tmp30}, TNode<Object>{tmp29});
    ca_.Goto(&block51, phi_bb53_9, tmp57);
  }

  TNode<BoolT> phi_bb51_9;
  TNode<JSReceiver> phi_bb51_12;
  TNode<Smi> tmp58;
  if (block51.is_used()) {
    ca_.Bind(&block51, &phi_bb51_9, &phi_bb51_12);
    tmp58 = FromConstexpr_Smi_constexpr_int31_0(state_, 0);
    ca_.Goto(&block57, phi_bb51_9, tmp58);
  }

  TNode<BoolT> phi_bb57_9;
  TNode<Smi> phi_bb57_13;
  TNode<BoolT> tmp59;
  if (block57.is_used()) {
    ca_.Bind(&block57, &phi_bb57_9, &phi_bb57_13);
    tmp59 = NumberIsLessThan_0(state_, TNode<Number>{phi_bb57_13}, TNode<Number>{tmp29});
    ca_.Branch(tmp59, &block55, std::vector<compiler::Node*>{phi_bb57_9, phi_bb57_13}, &block56, std::vector<compiler::Node*>{phi_bb57_9, phi_bb57_13});
  }

  TNode<BoolT> phi_bb55_9;
  TNode<Smi> phi_bb55_13;
  TNode<Object> tmp60;
  if (block55.is_used()) {
    ca_.Bind(&block55, &phi_bb55_9, &phi_bb55_13);
    tmp60 = CodeStubAssembler(state_).GetProperty(TNode<Context>{parameter0}, TNode<Object>{tmp28}, TNode<Object>{phi_bb55_13});
    ca_.Branch(phi_bb55_9, &block58, std::vector<compiler::Node*>{phi_bb55_9, phi_bb55_13}, &block59, std::vector<compiler::Node*>{phi_bb55_9, phi_bb55_13});
  }

  TNode<BoolT> phi_bb58_9;
  TNode<Smi> phi_bb58_13;
  TNode<JSReceiver> tmp61;
  TNode<Object> tmp62;
  if (block58.is_used()) {
    ca_.Bind(&block58, &phi_bb58_9, &phi_bb58_13);
    tmp61 = UnsafeCast_Callable_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp14});
    tmp62 = CodeStubAssembler(state_).Call(TNode<Context>{parameter0}, TNode<Object>{tmp61}, TNode<Object>{tmp16}, TNode<Object>{tmp60}, TNode<Object>{phi_bb58_13});
    ca_.Goto(&block60, phi_bb58_9, phi_bb58_13, tmp62);
  }

  TNode<BoolT> phi_bb59_9;
  TNode<Smi> phi_bb59_13;
  if (block59.is_used()) {
    ca_.Bind(&block59, &phi_bb59_9, &phi_bb59_13);
    ca_.Goto(&block60, phi_bb59_9, phi_bb59_13, tmp60);
  }

  TNode<BoolT> phi_bb60_9;
  TNode<Smi> phi_bb60_13;
  TNode<Object> phi_bb60_15;
  TNode<Object> tmp63;
  TNode<Smi> tmp64;
  TNode<Smi> tmp65;
  if (block60.is_used()) {
    ca_.Bind(&block60, &phi_bb60_9, &phi_bb60_13, &phi_bb60_15);
    tmp63 = ca_.CallStub<Object>(Builtins::CallableFor(ca_.isolate(), Builtin::kFastCreateDataProperty), parameter0, phi_bb51_12, phi_bb60_13, phi_bb60_15);
    tmp64 = FromConstexpr_Smi_constexpr_int31_0(state_, 1);
    tmp65 = CodeStubAssembler(state_).SmiAdd(TNode<Smi>{phi_bb60_13}, TNode<Smi>{tmp64});
    ca_.Goto(&block57, phi_bb60_9, tmp65);
  }

  TNode<BoolT> phi_bb56_9;
  TNode<Smi> phi_bb56_13;
  if (block56.is_used()) {
    ca_.Bind(&block56, &phi_bb56_9, &phi_bb56_13);
    CodeStubAssembler(state_).SetPropertyLength(TNode<Context>{parameter0}, TNode<Object>{phi_bb51_12}, TNode<Number>{tmp29});
    arguments.PopAndReturn(phi_bb51_12);
  }
}

TNode<BoolT> Is_Callable_JSAny_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_Callable_1(state_, TNode<Context>{p_context}, TNode<Object>{p_o}, &label1);
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


TNode<JSReceiver> Cast_Constructor_1(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o, compiler::CodeAssemblerLabel* label_CastError) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<HeapObject> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = CodeStubAssembler(state_).TaggedToHeapObject(TNode<Object>{p_o}, &label1);
    ca_.Goto(&block3);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block4);
    }
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block1);
  }

  TNode<JSReceiver> tmp2;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_Constructor_0(state_, TNode<HeapObject>{tmp0}, &label3);
    ca_.Goto(&block5);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block1);
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block7);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(label_CastError);
  }

    ca_.Bind(&block7);
  return TNode<JSReceiver>{tmp2};
}


TNode<JSReceiver> UnsafeCast_Callable_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_o) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TORQUE_CAST(TNode<Object>{p_o});
    ca_.Goto(&block6);
  }

    ca_.Bind(&block6);
  return TNode<JSReceiver>{tmp0};
}


} // namespace internal
} // namespace v8
