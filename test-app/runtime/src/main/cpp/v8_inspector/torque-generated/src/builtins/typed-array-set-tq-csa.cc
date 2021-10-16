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
#include "torque-generated/src/builtins/typed-array-set-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-every-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-set-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"

namespace v8 {
namespace internal {
const char* kBuiltinNameSet_0(compiler::CodeAssemblerState* state_) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

    ca_.Bind(&block0);
  return "%TypedArray%.prototype.set";}

TF_BUILTIN(TypedArrayPrototypeSet, CodeStubAssembler) {
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
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT, UintPtrT, BoolT> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT, UintPtrT, BoolT> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT, UintPtrT, BoolT> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT, UintPtrT, BoolT> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT, UintPtrT, BoolT> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kDeferred);
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
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kNotTypedArray, kBuiltinNameSet_0(state_));
  }

  TNode<BoolT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<BoolT> tmp5;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp2 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    tmp3 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp5 = CodeStubAssembler(state_).IntPtrGreaterThan(TNode<IntPtrT>{torque_arguments.length}, TNode<IntPtrT>{tmp4});
    ca_.Branch(tmp5, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp6;
  TNode<Object> tmp7;
  TNode<UintPtrT> tmp8;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 1);
    tmp7 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp6});
    compiler::CodeAssemblerLabel label9(&ca_);
    compiler::CodeAssemblerLabel label10(&ca_);
    compiler::CodeAssemblerLabel label11(&ca_);
    tmp8 = ToUintPtr_0(state_, TNode<Context>{parameter0}, TNode<Object>{tmp7}, &label9, &label10, &label11);
    ca_.Goto(&block14);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block15);
    }
    if (label10.is_used()) {
      ca_.Bind(&label10);
      ca_.Goto(&block16);
    }
    if (label11.is_used()) {
      ca_.Bind(&label11);
      ca_.Goto(&block17);
    }
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    ca_.Goto(&block8);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block13);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block13);
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block12, tmp2, tmp8);
  }

  TNode<BoolT> tmp12;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp12 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block12, tmp12, tmp3);
  }

  TNode<BoolT> phi_bb12_6;
  TNode<UintPtrT> phi_bb12_7;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_6, &phi_bb12_7);
    ca_.Goto(&block11, phi_bb12_6, phi_bb12_7);
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    ca_.Goto(&block11, tmp2, tmp3);
  }

  TNode<BoolT> phi_bb11_6;
  TNode<UintPtrT> phi_bb11_7;
  TNode<JSTypedArray> tmp13;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_6, &phi_bb11_7);
    compiler::CodeAssemblerLabel label14(&ca_);
    tmp13 = EnsureAttached_0(state_, TNode<JSTypedArray>{tmp0}, &label14);
    ca_.Goto(&block18, phi_bb11_6, phi_bb11_7);
    if (label14.is_used()) {
      ca_.Bind(&label14);
      ca_.Goto(&block19, phi_bb11_6, phi_bb11_7);
    }
  }

  TNode<BoolT> phi_bb19_6;
  TNode<UintPtrT> phi_bb19_7;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_6, &phi_bb19_7);
    ca_.Goto(&block6);
  }

  TNode<BoolT> phi_bb18_6;
  TNode<UintPtrT> phi_bb18_7;
  TNode<IntPtrT> tmp15;
  TNode<Object> tmp16;
  TNode<JSTypedArray> tmp17;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_6, &phi_bb18_7);
    tmp15 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp16 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp15});
    compiler::CodeAssemblerLabel label18(&ca_);
    tmp17 = Cast_JSTypedArray_1(state_, TNode<Context>{parameter0}, TNode<Object>{tmp16}, &label18);
    ca_.Goto(&block22, phi_bb18_6, phi_bb18_7);
    if (label18.is_used()) {
      ca_.Bind(&label18);
      ca_.Goto(&block23, phi_bb18_6, phi_bb18_7);
    }
  }

  TNode<BoolT> phi_bb23_6;
  TNode<UintPtrT> phi_bb23_7;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_6, &phi_bb23_7);
    compiler::CodeAssemblerLabel label19(&ca_);
    compiler::CodeAssemblerLabel label20(&ca_);
    TypedArrayPrototypeSetArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<JSTypedArray>{tmp13}, TNode<Object>{tmp16}, TNode<UintPtrT>{phi_bb23_7}, TNode<BoolT>{phi_bb23_6}, &label19, &label20);
    ca_.Goto(&block28, phi_bb23_6, phi_bb23_7, phi_bb23_7, phi_bb23_6);
    if (label19.is_used()) {
      ca_.Bind(&label19);
      ca_.Goto(&block29, phi_bb23_6, phi_bb23_7, phi_bb23_7, phi_bb23_6);
    }
    if (label20.is_used()) {
      ca_.Bind(&label20);
      ca_.Goto(&block30, phi_bb23_6, phi_bb23_7, phi_bb23_7, phi_bb23_6);
    }
  }

  TNode<BoolT> phi_bb22_6;
  TNode<UintPtrT> phi_bb22_7;
  TNode<JSTypedArray> tmp21;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_6, &phi_bb22_7);
    compiler::CodeAssemblerLabel label22(&ca_);
    tmp21 = EnsureAttached_0(state_, TNode<JSTypedArray>{tmp17}, &label22);
    ca_.Goto(&block24, phi_bb22_6, phi_bb22_7);
    if (label22.is_used()) {
      ca_.Bind(&label22);
      ca_.Goto(&block25, phi_bb22_6, phi_bb22_7);
    }
  }

  TNode<BoolT> phi_bb25_6;
  TNode<UintPtrT> phi_bb25_7;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_6, &phi_bb25_7);
    ca_.Goto(&block6);
  }

  TNode<BoolT> phi_bb24_6;
  TNode<UintPtrT> phi_bb24_7;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_6, &phi_bb24_7);
    compiler::CodeAssemblerLabel label23(&ca_);
    TypedArrayPrototypeSetTypedArray_0(state_, TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<JSTypedArray>{tmp13}, TNode<JSTypedArray>{tmp21}, TNode<UintPtrT>{phi_bb24_7}, TNode<BoolT>{phi_bb24_6}, &label23);
    ca_.Goto(&block26, phi_bb24_6, phi_bb24_7, phi_bb24_7, phi_bb24_6);
    if (label23.is_used()) {
      ca_.Bind(&label23);
      ca_.Goto(&block27, phi_bb24_6, phi_bb24_7, phi_bb24_7, phi_bb24_6);
    }
  }

  TNode<BoolT> phi_bb27_6;
  TNode<UintPtrT> phi_bb27_7;
  TNode<UintPtrT> phi_bb27_14;
  TNode<BoolT> phi_bb27_15;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_6, &phi_bb27_7, &phi_bb27_14, &phi_bb27_15);
    ca_.Goto(&block8);
  }

  TNode<BoolT> phi_bb26_6;
  TNode<UintPtrT> phi_bb26_7;
  TNode<UintPtrT> phi_bb26_14;
  TNode<BoolT> phi_bb26_15;
  TNode<Oddball> tmp24;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_6, &phi_bb26_7, &phi_bb26_14, &phi_bb26_15);
    tmp24 = Undefined_0(state_);
    arguments.PopAndReturn(tmp24);
  }

  TNode<BoolT> phi_bb29_6;
  TNode<UintPtrT> phi_bb29_7;
  TNode<UintPtrT> phi_bb29_12;
  TNode<BoolT> phi_bb29_13;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_6, &phi_bb29_7, &phi_bb29_12, &phi_bb29_13);
    ca_.Goto(&block8);
  }

  TNode<BoolT> phi_bb30_6;
  TNode<UintPtrT> phi_bb30_7;
  TNode<UintPtrT> phi_bb30_12;
  TNode<BoolT> phi_bb30_13;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_6, &phi_bb30_7, &phi_bb30_12, &phi_bb30_13);
    ca_.Goto(&block6);
  }

  TNode<BoolT> phi_bb28_6;
  TNode<UintPtrT> phi_bb28_7;
  TNode<UintPtrT> phi_bb28_12;
  TNode<BoolT> phi_bb28_13;
  TNode<Oddball> tmp25;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_6, &phi_bb28_7, &phi_bb28_12, &phi_bb28_13);
    tmp25 = Undefined_0(state_);
    arguments.PopAndReturn(tmp25);
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{parameter0}, MessageTemplate::kTypedArraySetOffsetOutOfBounds);
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{parameter0}, MessageTemplate::kDetachedOperation, kBuiltinNameSet_0(state_));
  }
}

void TypedArrayPrototypeSetArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, TNode<JSTypedArray> p_target, TNode<Object> p_arrayArg, TNode<UintPtrT> p_targetOffset, TNode<BoolT> p_targetOffsetOverflowed, compiler::CodeAssemblerLabel* label_IfOffsetOutOfBounds, compiler::CodeAssemblerLabel* label_IfDetached) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Number> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_Number_0(state_, TNode<Object>{p_arrayArg}, &label1);
    ca_.Goto(&block6);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block7);
    }
  }

  TNode<JSReceiver> tmp2;
  TNode<Number> tmp3;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp2 = CodeStubAssembler(state_).ToObject_Inline(TNode<Context>{p_context}, TNode<Object>{p_arrayArg});
    tmp3 = GetLengthProperty_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp2});
    ca_.Branch(p_targetOffsetOverflowed, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kInvalidArgument);
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    ca_.Goto(&block1);
  }

  TNode<IntPtrT> tmp4;
  TNode<UintPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp5 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{p_target, tmp4});
    compiler::CodeAssemblerLabel label7(&ca_);
    tmp6 = ChangeSafeIntegerNumberToUintPtr_0(state_, TNode<Number>{tmp3}, &label7);
    ca_.Goto(&block10);
    if (label7.is_used()) {
      ca_.Bind(&label7);
      ca_.Goto(&block11);
    }
  }

  if (block11.is_used()) {
    ca_.Bind(&block11);
    ca_.Goto(&block1);
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    compiler::CodeAssemblerLabel label8(&ca_);
    CheckIntegerIndexAdditionOverflow_0(state_, TNode<UintPtrT>{tmp6}, TNode<UintPtrT>{p_targetOffset}, TNode<UintPtrT>{tmp5}, &label8);
    ca_.Goto(&block12);
    if (label8.is_used()) {
      ca_.Bind(&label8);
      ca_.Goto(&block13);
    }
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block1);
  }

  TNode<UintPtrT> tmp9;
  TNode<BoolT> tmp10;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp9 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp10 = CodeStubAssembler(state_).WordEqual(TNode<UintPtrT>{tmp6}, TNode<UintPtrT>{tmp9});
    ca_.Branch(tmp10, &block14, std::vector<compiler::Node*>{}, &block15, std::vector<compiler::Node*>{});
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    ca_.Goto(&block3);
  }

  TNode<Int32T> tmp11;
  TNode<BoolT> tmp12;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp11 = CodeStubAssembler(state_).LoadElementsKind(TNode<JSTypedArray>{p_target});
    tmp12 = TypedArrayBuiltinsAssembler(state_).IsBigInt64ElementsKind(TNode<Int32T>{tmp11});
    ca_.Branch(tmp12, &block18, std::vector<compiler::Node*>{}, &block19, std::vector<compiler::Node*>{});
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    ca_.Goto(&block17);
  }

  TNode<JSArray> tmp13;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    compiler::CodeAssemblerLabel label14(&ca_);
    tmp13 = Cast_FastJSArray_0(state_, TNode<Context>{p_context}, TNode<HeapObject>{tmp2}, &label14);
    ca_.Goto(&block22);
    if (label14.is_used()) {
      ca_.Bind(&label14);
      ca_.Goto(&block23);
    }
  }

  if (block23.is_used()) {
    ca_.Bind(&block23);
    ca_.Goto(&block17);
  }

  TNode<IntPtrT> tmp15;
  TNode<Map> tmp16;
  TNode<Int32T> tmp17;
  TNode<BoolT> tmp18;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    tmp15 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp16 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{tmp13, tmp15});
    tmp17 = CodeStubAssembler(state_).LoadMapElementsKind(TNode<Map>{tmp16});
    tmp18 = CodeStubAssembler(state_).IsElementsKindInRange(TNode<Int32T>{tmp17}, ElementsKind::PACKED_SMI_ELEMENTS, ElementsKind::HOLEY_SMI_ELEMENTS);
    ca_.Branch(tmp18, &block26, std::vector<compiler::Node*>{}, &block27, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp19;
  if (block26.is_used()) {
    ca_.Bind(&block26);
    tmp19 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block28, tmp19);
  }

  TNode<BoolT> tmp20;
  if (block27.is_used()) {
    ca_.Bind(&block27);
    tmp20 = CodeStubAssembler(state_).IsElementsKindInRange(TNode<Int32T>{tmp17}, ElementsKind::PACKED_DOUBLE_ELEMENTS, ElementsKind::HOLEY_DOUBLE_ELEMENTS);
    ca_.Goto(&block28, tmp20);
  }

  TNode<BoolT> phi_bb28_13;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_13);
    ca_.Branch(phi_bb28_13, &block24, std::vector<compiler::Node*>{}, &block25, std::vector<compiler::Node*>{});
  }

  TNode<JSTypedArray> tmp21;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    compiler::CodeAssemblerLabel label22(&ca_);
    tmp21 = EnsureAttached_0(state_, TNode<JSTypedArray>{p_target}, &label22);
    ca_.Goto(&block30);
    if (label22.is_used()) {
      ca_.Bind(&label22);
      ca_.Goto(&block31);
    }
  }

  if (block31.is_used()) {
    ca_.Bind(&block31);
    ca_.Goto(label_IfDetached);
  }

  if (block30.is_used()) {
    ca_.Bind(&block30);
    TypedArrayBuiltinsAssembler(state_).CallCCopyFastNumberJSArrayElementsToTypedArray(TNode<Context>{p_context}, TNode<JSArray>{tmp13}, TNode<JSTypedArray>{tmp21}, TNode<UintPtrT>{tmp6}, TNode<UintPtrT>{p_targetOffset});
    ca_.Goto(&block16);
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    ca_.Goto(&block17);
  }

  TNode<Number> tmp23;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp23 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{p_targetOffset});
    CodeStubAssembler(state_).CallRuntime(Runtime::kTypedArraySet, p_context, p_target, tmp2, tmp3, tmp23);
    ca_.Goto(&block16);
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block3);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block32);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(label_IfOffsetOutOfBounds);
  }

    ca_.Bind(&block32);
}


void TypedArrayPrototypeSetTypedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Object> p_receiver, TNode<JSTypedArray> p_target, TNode<JSTypedArray> p_typedArray, TNode<UintPtrT> p_targetOffset, TNode<BoolT> p_targetOffsetOverflowed, compiler::CodeAssemblerLabel* label_IfOffsetOutOfBounds) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block40(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block44(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    ca_.Branch(p_targetOffsetOverflowed, &block3, std::vector<compiler::Node*>{}, &block4, std::vector<compiler::Node*>{});
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block1);
  }

  TNode<IntPtrT> tmp0;
  TNode<UintPtrT> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp1 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{p_target, tmp0});
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp3 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{p_typedArray, tmp2});
    compiler::CodeAssemblerLabel label4(&ca_);
    CheckIntegerIndexAdditionOverflow_0(state_, TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{p_targetOffset}, TNode<UintPtrT>{tmp1}, &label4);
    ca_.Goto(&block5);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block6);
    }
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block1);
  }

  TNode<UintPtrT> tmp5;
  TNode<Int32T> tmp6;
  TNode<Int32T> tmp7;
  TNode<BoolT> tmp8;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    std::tie(tmp5, tmp6) = TypedArrayBuiltinsAssembler(state_).GetTypedArrayElementsInfo(TNode<JSTypedArray>{p_target}).Flatten();
    tmp7 = CodeStubAssembler(state_).LoadElementsKind(TNode<JSTypedArray>{p_typedArray});
    tmp8 = ElementsKindNotEqual_0(state_, TNode<Int32T>{tmp7}, TNode<Int32T>{tmp6});
    ca_.Branch(tmp8, &block9, std::vector<compiler::Node*>{}, &block10, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp9;
  TNode<BoolT> tmp10;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp9 = TypedArrayBuiltinsAssembler(state_).IsUint8ElementsKind(TNode<Int32T>{tmp7});
    tmp10 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp9});
    ca_.Branch(tmp10, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp11;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp11 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block15, tmp11);
  }

  TNode<BoolT> tmp12;
  TNode<BoolT> tmp13;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp12 = TypedArrayBuiltinsAssembler(state_).IsUint8ElementsKind(TNode<Int32T>{tmp6});
    tmp13 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp12});
    ca_.Goto(&block15, tmp13);
  }

  TNode<BoolT> phi_bb15_12;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_12);
    ca_.Branch(phi_bb15_12, &block11, std::vector<compiler::Node*>{}, &block12, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp14;
  TNode<BoolT> tmp15;
  TNode<BoolT> tmp16;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp14 = TypedArrayBuiltinsAssembler(state_).IsBigInt64ElementsKind(TNode<Int32T>{tmp7});
    tmp15 = TypedArrayBuiltinsAssembler(state_).IsBigInt64ElementsKind(TNode<Int32T>{tmp6});
    tmp16 = CodeStubAssembler(state_).Word32NotEqual(TNode<BoolT>{tmp14}, TNode<BoolT>{tmp15});
    ca_.Branch(tmp16, &block40, std::vector<compiler::Node*>{}, &block41, std::vector<compiler::Node*>{});
  }

  if (block12.is_used()) {
    ca_.Bind(&block12);
    ca_.Goto(&block10);
  }

  TNode<UintPtrT> tmp17;
  TNode<BoolT> tmp18;
  if (block10.is_used()) {
    ca_.Bind(&block10);
    tmp17 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp18 = CodeStubAssembler(state_).WordEqual(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp17});
    ca_.Branch(tmp18, &block16, std::vector<compiler::Node*>{}, &block17, std::vector<compiler::Node*>{});
  }

  if (block16.is_used()) {
    ca_.Bind(&block16);
    ca_.Goto(&block2);
  }

  TNode<UintPtrT> tmp19;
  TNode<BoolT> tmp20;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp19 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSTypedArray::kMaxLength);
    tmp20 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp19});
    ca_.Branch(tmp20, &block21, std::vector<compiler::Node*>{}, &block22, std::vector<compiler::Node*>{});
  }

  if (block21.is_used()) {
    ca_.Bind(&block21);
    ca_.Goto(&block19);
  }

  TNode<UintPtrT> tmp21;
  TNode<UintPtrT> tmp22;
  TNode<BoolT> tmp23;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    tmp21 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSArrayBuffer::kMaxByteLength);
    tmp22 = CodeStubAssembler(state_).WordShr(TNode<UintPtrT>{tmp21}, TNode<UintPtrT>{tmp5});
    tmp23 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp22});
    ca_.Branch(tmp23, &block23, std::vector<compiler::Node*>{}, &block24, std::vector<compiler::Node*>{});
  }

  if (block23.is_used()) {
    ca_.Bind(&block23);
    ca_.Goto(&block19);
  }

  TNode<UintPtrT> tmp24;
  TNode<UintPtrT> tmp25;
  TNode<BoolT> tmp26;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp24 = CodeStubAssembler(state_).WordShl(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp5});
    tmp25 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSTypedArray::kMaxLength);
    tmp26 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{p_targetOffset}, TNode<UintPtrT>{tmp25});
    ca_.Branch(tmp26, &block28, std::vector<compiler::Node*>{}, &block29, std::vector<compiler::Node*>{});
  }

  if (block19.is_used()) {
    ca_.Bind(&block19);
    CodeStubAssembler(state_).Unreachable();
  }

  if (block28.is_used()) {
    ca_.Bind(&block28);
    ca_.Goto(&block26);
  }

  TNode<UintPtrT> tmp27;
  TNode<UintPtrT> tmp28;
  TNode<BoolT> tmp29;
  if (block29.is_used()) {
    ca_.Bind(&block29);
    tmp27 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSArrayBuffer::kMaxByteLength);
    tmp28 = CodeStubAssembler(state_).WordShr(TNode<UintPtrT>{tmp27}, TNode<UintPtrT>{tmp5});
    tmp29 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{p_targetOffset}, TNode<UintPtrT>{tmp28});
    ca_.Branch(tmp29, &block30, std::vector<compiler::Node*>{}, &block31, std::vector<compiler::Node*>{});
  }

  if (block30.is_used()) {
    ca_.Bind(&block30);
    ca_.Goto(&block26);
  }

  TNode<UintPtrT> tmp30;
  TNode<RawPtrT> tmp31;
  TNode<IntPtrT> tmp32;
  TNode<RawPtrT> tmp33;
  TNode<RawPtrT> tmp34;
  if (block31.is_used()) {
    ca_.Bind(&block31);
    tmp30 = CodeStubAssembler(state_).WordShl(TNode<UintPtrT>{p_targetOffset}, TNode<UintPtrT>{tmp5});
    tmp31 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{p_target});
    tmp32 = Convert_intptr_uintptr_0(state_, TNode<UintPtrT>{tmp30});
    tmp33 = CodeStubAssembler(state_).RawPtrAdd(TNode<RawPtrT>{tmp31}, TNode<IntPtrT>{tmp32});
    tmp34 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{p_typedArray});
    TypedArrayBuiltinsAssembler(state_).CallCMemmove(TNode<RawPtrT>{tmp33}, TNode<RawPtrT>{tmp34}, TNode<UintPtrT>{tmp24});
    ca_.Goto(&block7);
  }

  if (block26.is_used()) {
    ca_.Bind(&block26);
    CodeStubAssembler(state_).Unreachable();
  }

  if (block40.is_used()) {
    ca_.Bind(&block40);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kBigIntMixedTypes);
  }

  TNode<UintPtrT> tmp35;
  TNode<BoolT> tmp36;
  if (block41.is_used()) {
    ca_.Bind(&block41);
    tmp35 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp36 = CodeStubAssembler(state_).WordEqual(TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{tmp35});
    ca_.Branch(tmp36, &block42, std::vector<compiler::Node*>{}, &block43, std::vector<compiler::Node*>{});
  }

  if (block42.is_used()) {
    ca_.Bind(&block42);
    ca_.Goto(&block2);
  }

  if (block43.is_used()) {
    ca_.Bind(&block43);
    TypedArrayBuiltinsAssembler(state_).CallCCopyTypedArrayElementsToTypedArray(TNode<JSTypedArray>{p_typedArray}, TNode<JSTypedArray>{p_target}, TNode<UintPtrT>{tmp3}, TNode<UintPtrT>{p_targetOffset});
    ca_.Goto(&block7);
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block2);
  }

  if (block2.is_used()) {
    ca_.Bind(&block2);
    ca_.Goto(&block44);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(label_IfOffsetOutOfBounds);
  }

    ca_.Bind(&block44);
}


} // namespace internal
} // namespace v8
