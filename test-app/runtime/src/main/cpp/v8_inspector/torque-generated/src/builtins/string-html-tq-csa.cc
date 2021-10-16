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
#include "torque-generated/src/builtins/string-html-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/frame-arguments-tq-csa.h"
#include "torque-generated/src/builtins/string-html-tq-csa.h"

namespace v8 {
namespace internal {
TF_BUILTIN(CreateHTML, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<Object> parameter1 = UncheckedParameter<Object>(Descriptor::kReceiver);
  USE(parameter1);
  TNode<String> parameter2 = UncheckedParameter<String>(Descriptor::kMethodName);
  USE(parameter2);
  TNode<String> parameter3 = UncheckedParameter<String>(Descriptor::kTagName);
  USE(parameter3);
  TNode<String> parameter4 = UncheckedParameter<String>(Descriptor::kAttr);
  USE(parameter4);
  TNode<Object> parameter5 = UncheckedParameter<Object>(Descriptor::kAttrValue);
  USE(parameter5);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<String> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<BoolT> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).ToThisString(TNode<Context>{parameter0}, TNode<Object>{parameter1}, TNode<String>{parameter2});
    tmp1 = FromConstexpr_String_constexpr_string_0(state_, "<");
    tmp2 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp1}, TNode<String>{parameter3});
    tmp3 = kEmptyString_0(state_);
    tmp4 = CodeStubAssembler(state_).TaggedNotEqual(TNode<Object>{parameter4}, TNode<HeapObject>{tmp3});
    ca_.Branch(tmp4, &block1, std::vector<compiler::Node*>{}, &block2, std::vector<compiler::Node*>{tmp2});
  }

  TNode<String> tmp5;
  TNode<String> tmp6;
  TNode<String> tmp7;
  TNode<String> tmp8;
  TNode<String> tmp9;
  TNode<String> tmp10;
  TNode<String> tmp11;
  TNode<String> tmp12;
  TNode<String> tmp13;
  TNode<String> tmp14;
  if (block1.is_used()) {
    ca_.Bind(&block1);
    tmp5 = CodeStubAssembler(state_).ToString_Inline(TNode<Context>{parameter0}, TNode<Object>{parameter5});
    tmp6 = TORQUE_CAST(CodeStubAssembler(state_).CallRuntime(Runtime::kStringEscapeQuotes, parameter0, tmp5)); 
    tmp7 = FromConstexpr_String_constexpr_string_0(state_, " ");
    tmp8 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp2}, TNode<String>{tmp7});
    tmp9 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp8}, TNode<String>{parameter4});
    tmp10 = FromConstexpr_String_constexpr_string_0(state_, "=\"");
    tmp11 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp9}, TNode<String>{tmp10});
    tmp12 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp11}, TNode<String>{tmp6});
    tmp13 = FromConstexpr_String_constexpr_string_0(state_, "\"");
    tmp14 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp12}, TNode<String>{tmp13});
    ca_.Goto(&block2, tmp14);
  }

  TNode<String> phi_bb2_7;
  TNode<String> tmp15;
  TNode<String> tmp16;
  TNode<String> tmp17;
  TNode<String> tmp18;
  TNode<String> tmp19;
  TNode<String> tmp20;
  TNode<String> tmp21;
  TNode<String> tmp22;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_7);
    tmp15 = FromConstexpr_String_constexpr_string_0(state_, ">");
    tmp16 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{phi_bb2_7}, TNode<String>{tmp15});
    tmp17 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp16}, TNode<String>{tmp0});
    tmp18 = FromConstexpr_String_constexpr_string_0(state_, "</");
    tmp19 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp17}, TNode<String>{tmp18});
    tmp20 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp19}, TNode<String>{parameter3});
    tmp21 = FromConstexpr_String_constexpr_string_0(state_, ">");
    tmp22 = StringAdd_0(state_, TNode<Context>{parameter0}, TNode<String>{tmp20}, TNode<String>{tmp21});
    CodeStubAssembler(state_).Return(tmp22);
  }
}

TF_BUILTIN(StringPrototypeAnchor, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  TNode<String> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.anchor");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "a");
    tmp4 = FromConstexpr_String_constexpr_string_0(state_, "name");
    tmp5 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp4, tmp1);
    arguments.PopAndReturn(tmp5);
  }
}

TF_BUILTIN(StringPrototypeBig, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = kEmptyString_0(state_);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.big");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "big");
    tmp4 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp0, tmp1);
    arguments.PopAndReturn(tmp4);
  }
}

TF_BUILTIN(StringPrototypeBlink, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = kEmptyString_0(state_);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.blink");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "blink");
    tmp4 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp0, tmp1);
    arguments.PopAndReturn(tmp4);
  }
}

TF_BUILTIN(StringPrototypeBold, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = kEmptyString_0(state_);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.bold");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "b");
    tmp4 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp0, tmp1);
    arguments.PopAndReturn(tmp4);
  }
}

TF_BUILTIN(StringPrototypeFontcolor, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  TNode<String> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.fontcolor");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "font");
    tmp4 = FromConstexpr_String_constexpr_string_0(state_, "color");
    tmp5 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp4, tmp1);
    arguments.PopAndReturn(tmp5);
  }
}

TF_BUILTIN(StringPrototypeFontsize, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  TNode<String> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.fontsize");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "font");
    tmp4 = FromConstexpr_String_constexpr_string_0(state_, "size");
    tmp5 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp4, tmp1);
    arguments.PopAndReturn(tmp5);
  }
}

TF_BUILTIN(StringPrototypeFixed, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = kEmptyString_0(state_);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.fixed");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "tt");
    tmp4 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp0, tmp1);
    arguments.PopAndReturn(tmp4);
  }
}

TF_BUILTIN(StringPrototypeItalics, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = kEmptyString_0(state_);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.italics");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "i");
    tmp4 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp0, tmp1);
    arguments.PopAndReturn(tmp4);
  }
}

TF_BUILTIN(StringPrototypeLink, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<IntPtrT> tmp0;
  TNode<Object> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  TNode<String> tmp5;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp1 = CodeStubAssembler(state_).GetArgumentValue(TorqueStructArguments{TNode<RawPtrT>{torque_arguments.frame}, TNode<RawPtrT>{torque_arguments.base}, TNode<IntPtrT>{torque_arguments.length}}, TNode<IntPtrT>{tmp0});
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.link");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "a");
    tmp4 = FromConstexpr_String_constexpr_string_0(state_, "href");
    tmp5 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp4, tmp1);
    arguments.PopAndReturn(tmp5);
  }
}

TF_BUILTIN(StringPrototypeSmall, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = kEmptyString_0(state_);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.small");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "small");
    tmp4 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp0, tmp1);
    arguments.PopAndReturn(tmp4);
  }
}

TF_BUILTIN(StringPrototypeStrike, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = kEmptyString_0(state_);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.strike");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "strike");
    tmp4 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp0, tmp1);
    arguments.PopAndReturn(tmp4);
  }
}

TF_BUILTIN(StringPrototypeSub, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = kEmptyString_0(state_);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.sub");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "sub");
    tmp4 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp0, tmp1);
    arguments.PopAndReturn(tmp4);
  }
}

TF_BUILTIN(StringPrototypeSup, CodeStubAssembler) {
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
    ca_.Goto(&block0);

  TNode<String> tmp0;
  TNode<String> tmp1;
  TNode<String> tmp2;
  TNode<String> tmp3;
  TNode<String> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = kEmptyString_0(state_);
    tmp1 = kEmptyString_0(state_);
    tmp2 = FromConstexpr_String_constexpr_string_0(state_, "String.prototype.sup");
    tmp3 = FromConstexpr_String_constexpr_string_0(state_, "sup");
    tmp4 = ca_.CallStub<String>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateHTML), parameter0, parameter1, tmp2, tmp3, tmp0, tmp1);
    arguments.PopAndReturn(tmp4);
  }
}

} // namespace internal
} // namespace v8
