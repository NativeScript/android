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
#include "torque-generated/src/builtins/typed-array-createtypedarray-tq-csa.h"
#include "torque-generated/src/builtins/array-join-tq-csa.h"
#include "torque-generated/src/builtins/arraybuffer-tq-csa.h"
#include "torque-generated/src/builtins/base-tq-csa.h"
#include "torque-generated/src/builtins/cast-tq-csa.h"
#include "torque-generated/src/builtins/convert-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-createtypedarray-tq-csa.h"
#include "torque-generated/src/builtins/typed-array-tq-csa.h"
#include "torque-generated/src/objects/js-array-buffer-tq-csa.h"
#include "torque-generated/src/objects/js-objects-tq-csa.h"

namespace v8 {
namespace internal {
TNode<JSTypedArray> AllocateTypedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, bool p_isOnHeap, TNode<Map> p_map, TNode<JSArrayBuffer> p_buffer, TNode<UintPtrT> p_byteOffset, TNode<UintPtrT> p_byteLength, TNode<UintPtrT> p_length, TNode<BoolT> p_isLengthTracking) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<ByteArray> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  if (block0.is_used()) {
    ca_.Bind(&block0);
    if ((p_isOnHeap)) {
      ca_.Goto(&block2);
    } else {
      ca_.Goto(&block3);
    }
  }

  TNode<ByteArray> tmp0;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp0 = CodeStubAssembler(state_).AllocateByteArray(TNode<UintPtrT>{p_byteLength});
    ca_.Goto(&block4, tmp0);
  }

  TNode<ByteArray> tmp1;
  TNode<RawPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp1 = kEmptyByteArray_0(state_);
    tmp2 = CodeStubAssembler(state_).LoadJSArrayBufferBackingStorePtr(TNode<JSArrayBuffer>{p_buffer});
    tmp3 = Convert_uintptr_RawPtr_0(state_, TNode<RawPtrT>{tmp2});
    ca_.Goto(&block4, tmp1);
  }

  TNode<ByteArray> phi_bb4_7;
  TNode<JSObject> tmp4;
  TNode<JSTypedArray> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<IntPtrT> tmp7;
  TNode<IntPtrT> tmp8;
  TNode<IntPtrT> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<IntPtrT> tmp11;
  TNode<Uint32T> tmp12;
  TNode<Uint32T> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<BoolT> tmp15;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_7);
    tmp4 = AllocateFastOrSlowJSObjectFromMap_0(state_, TNode<Context>{p_context}, TNode<Map>{p_map});
    tmp5 = UnsafeCast_JSTypedArray_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp4});
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 8);
    CodeStubAssembler(state_).StoreReference<FixedArrayBase>(CodeStubAssembler::Reference{tmp5, tmp6}, phi_bb4_7);
    tmp7 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    CodeStubAssembler(state_).StoreReference<JSArrayBuffer>(CodeStubAssembler::Reference{tmp5, tmp7}, p_buffer);
    tmp8 = FromConstexpr_intptr_constexpr_int31_0(state_, 16);
    CodeStubAssembler(state_).StoreReference<UintPtrT>(CodeStubAssembler::Reference{tmp5, tmp8}, p_byteOffset);
    tmp9 = FromConstexpr_intptr_constexpr_int31_0(state_, 24);
    CodeStubAssembler(state_).StoreReference<UintPtrT>(CodeStubAssembler::Reference{tmp5, tmp9}, p_byteLength);
    tmp10 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    CodeStubAssembler(state_).StoreReference<UintPtrT>(CodeStubAssembler::Reference{tmp5, tmp10}, p_length);
    tmp11 = FromConstexpr_intptr_constexpr_int31_0(state_, 52);
    tmp12 = CodeStubAssembler(state_).LoadReference<Uint32T>(CodeStubAssembler::Reference{tmp5, tmp11});
    tmp13 = ca_.UncheckedCast<Uint32T>(CodeStubAssembler(state_).UpdateWord32<base::BitField<bool, 0, 1, uint32_t>>(ca_.UncheckedCast<Word32T>(tmp12), ca_.UncheckedCast<Uint32T>(p_isLengthTracking)));
    CodeStubAssembler(state_).StoreReference<Uint32T>(CodeStubAssembler::Reference{tmp5, tmp11}, tmp13);
    tmp14 = FromConstexpr_intptr_constexpr_int31_0(state_, 52);
    tmp15 = IsResizableArrayBuffer_0(state_, TNode<JSArrayBuffer>{p_buffer});
    ca_.Branch(tmp15, &block20, std::vector<compiler::Node*>{}, &block21, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp16;
  TNode<BoolT> tmp17;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp16 = IsSharedArrayBuffer_0(state_, TNode<JSArrayBuffer>{p_buffer});
    tmp17 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp16});
    ca_.Goto(&block22, tmp17);
  }

  TNode<BoolT> tmp18;
  if (block21.is_used()) {
    ca_.Bind(&block21);
    tmp18 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block22, tmp18);
  }

  TNode<BoolT> phi_bb22_13;
  TNode<Uint32T> tmp19;
  TNode<Uint32T> tmp20;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_13);
    tmp19 = CodeStubAssembler(state_).LoadReference<Uint32T>(CodeStubAssembler::Reference{tmp5, tmp14});
    tmp20 = ca_.UncheckedCast<Uint32T>(CodeStubAssembler(state_).UpdateWord32<base::BitField<bool, 1, 1, uint32_t>>(ca_.UncheckedCast<Word32T>(tmp19), ca_.UncheckedCast<Uint32T>(phi_bb22_13)));
    CodeStubAssembler(state_).StoreReference<Uint32T>(CodeStubAssembler::Reference{tmp5, tmp14}, tmp20);
    TypedArrayBuiltinsAssembler(state_).AllocateJSTypedArrayExternalPointerEntry(TNode<JSTypedArray>{tmp5});
    if ((p_isOnHeap)) {
      ca_.Goto(&block23);
    } else {
      ca_.Goto(&block24);
    }
  }

  if (block23.is_used()) {
    ca_.Bind(&block23);
    TypedArrayBuiltinsAssembler(state_).SetJSTypedArrayOnHeapDataPtr(TNode<JSTypedArray>{tmp5}, TNode<ByteArray>{phi_bb4_7}, TNode<UintPtrT>{p_byteOffset});
    ca_.Goto(&block25);
  }

  TNode<RawPtrT> tmp21;
  if (block24.is_used()) {
    ca_.Bind(&block24);
    tmp21 = CodeStubAssembler(state_).LoadJSArrayBufferBackingStorePtr(TNode<JSArrayBuffer>{p_buffer});
    TypedArrayBuiltinsAssembler(state_).SetJSTypedArrayOffHeapDataPtr(TNode<JSTypedArray>{tmp5}, TNode<RawPtrT>{tmp21}, TNode<UintPtrT>{p_byteOffset});
    ca_.Goto(&block25);
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    TypedArrayBuiltinsAssembler(state_).SetupTypedArrayEmbedderFields(TNode<JSTypedArray>{tmp5});
    ca_.Goto(&block30);
  }

    ca_.Bind(&block30);
  return TNode<JSTypedArray>{tmp5};
}


TNode<JSTypedArray> TypedArrayInitialize_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, bool p_initialize, TNode<Map> p_map, TNode<UintPtrT> p_length, TorqueStructTypedArrayElementsInfo p_elementsInfo, TNode<JSReceiver> p_bufferConstructor, compiler::CodeAssemblerLabel* label_IfRangeError) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object, Object> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<UintPtrT> tmp0;
  TNode<BoolT> tmp1;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSTypedArray::kMaxLength);
    tmp1 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{p_length}, TNode<UintPtrT>{tmp0});
    ca_.Branch(tmp1, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block1);
  }

  TNode<UintPtrT> tmp2;
  TNode<UintPtrT> tmp3;
  TNode<BoolT> tmp4;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSArrayBuffer::kMaxByteLength);
    tmp3 = CodeStubAssembler(state_).WordShr(TNode<UintPtrT>{tmp2}, TNode<UintPtrT>{p_elementsInfo.sizeLog2});
    tmp4 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{p_length}, TNode<UintPtrT>{tmp3});
    ca_.Branch(tmp4, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block1);
  }

  TNode<UintPtrT> tmp5;
  TNode<Number> tmp6;
  TNode<JSReceiver> tmp7;
  TNode<UintPtrT> tmp8;
  TNode<BoolT> tmp9;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp5 = CodeStubAssembler(state_).WordShl(TNode<UintPtrT>{p_length}, TNode<UintPtrT>{p_elementsInfo.sizeLog2});
    tmp6 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{tmp5});
    tmp7 = GetArrayBufferFunction_0(state_, TNode<Context>{p_context});
    tmp8 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp9 = CodeStubAssembler(state_).TaggedNotEqual(TNode<HeapObject>{p_bufferConstructor}, TNode<HeapObject>{tmp7});
    ca_.Branch(tmp9, &block12, std::vector<compiler::Node*>{}, &block13, std::vector<compiler::Node*>{});
  }

  TNode<JSReceiver> tmp10;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    tmp10 = CodeStubAssembler(state_).ConstructWithTarget(TNode<Context>{p_context}, TNode<JSReceiver>{tmp7}, TNode<JSReceiver>{p_bufferConstructor}, TNode<Object>{tmp6});
    ca_.Goto(&block9, tmp10);
  }

  TNode<UintPtrT> tmp11;
  TNode<BoolT> tmp12;
  if (block13.is_used()) {
    ca_.Bind(&block13);
    tmp11 = FromConstexpr_uintptr_constexpr_int31_0(state_, JSTypedArray::kMaxSizeInHeap);
    tmp12 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp5}, TNode<UintPtrT>{tmp11});
    ca_.Branch(tmp12, &block14, std::vector<compiler::Node*>{}, &block15, std::vector<compiler::Node*>{});
  }

  if (block14.is_used()) {
    ca_.Bind(&block14);
    if ((p_initialize)) {
      ca_.Goto(&block19);
    } else {
      ca_.Goto(&block20);
    }
  }

  TNode<JSArrayBuffer> tmp13;
  TNode<BoolT> tmp14;
  TNode<JSTypedArray> tmp15;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp13 = TypedArrayBuiltinsAssembler(state_).AllocateEmptyOnHeapBuffer(TNode<Context>{p_context}, TNode<UintPtrT>{tmp5});
    tmp14 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    tmp15 = AllocateTypedArray_0(state_, TNode<Context>{p_context}, true, TNode<Map>{p_map}, TNode<JSArrayBuffer>{tmp13}, TNode<UintPtrT>{tmp8}, TNode<UintPtrT>{tmp5}, TNode<UintPtrT>{p_length}, TNode<BoolT>{tmp14});
    if ((p_initialize)) {
      ca_.Goto(&block16);
    } else {
      ca_.Goto(&block17);
    }
  }

  TNode<RawPtrT> tmp16;
  TNode<IntPtrT> tmp17;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp16 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp15});
    tmp17 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    TypedArrayBuiltinsAssembler(state_).CallCMemset(TNode<RawPtrT>{tmp16}, TNode<IntPtrT>{tmp17}, TNode<UintPtrT>{tmp5});
    ca_.Goto(&block18);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block18);
  }

  if (block18.is_used()) {
    ca_.Bind(&block18);
    ca_.Goto(&block2, tmp15);
  }

  TNode<JSReceiver> tmp18;
  if (block19.is_used()) {
    ca_.Bind(&block19);
    tmp18 = CodeStubAssembler(state_).Construct(TNode<Context>{p_context}, TNode<JSReceiver>{tmp7}, TNode<Object>{tmp6});
    ca_.Goto(&block9, tmp18);
  }

  TNode<JSFunction> tmp19;
  TNode<Oddball> tmp20;
  TNode<Object> tmp21;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    tmp19 = GetArrayBufferNoInitFunction_0(state_, TNode<Context>{p_context});
    tmp20 = Undefined_0(state_);
    tmp21 = CodeStubAssembler(state_).Call(TNode<Context>{p_context}, TNode<Object>{tmp19}, TNode<Object>{tmp20}, TNode<Object>{tmp6});
    ca_.Goto(&block9, tmp21);
  }

  TNode<Object> phi_bb9_10;
  TNode<JSArrayBuffer> tmp22;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_10);
    compiler::CodeAssemblerLabel label23(&ca_);
    tmp22 = Cast_JSArrayBuffer_1(state_, TNode<Context>{p_context}, TNode<Object>{phi_bb9_10}, &label23);
    ca_.Goto(&block24, phi_bb9_10, phi_bb9_10);
    if (label23.is_used()) {
      ca_.Bind(&label23);
      ca_.Goto(&block25, phi_bb9_10, phi_bb9_10);
    }
  }

  TNode<Object> phi_bb25_10;
  TNode<Object> phi_bb25_11;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_10, &phi_bb25_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Object> phi_bb24_10;
  TNode<Object> phi_bb24_11;
  TNode<BoolT> tmp24;
  TNode<JSTypedArray> tmp25;
  if (block24.is_used()) {
    ca_.Bind(&block24, &phi_bb24_10, &phi_bb24_11);
    tmp24 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    tmp25 = AllocateTypedArray_0(state_, TNode<Context>{p_context}, false, TNode<Map>{p_map}, TNode<JSArrayBuffer>{tmp22}, TNode<UintPtrT>{tmp8}, TNode<UintPtrT>{tmp5}, TNode<UintPtrT>{p_length}, TNode<BoolT>{tmp24});
    ca_.Goto(&block2, tmp25);
  }

  TNode<JSTypedArray> phi_bb2_6;
  if (block2.is_used()) {
    ca_.Bind(&block2, &phi_bb2_6);
    ca_.Goto(&block26, phi_bb2_6);
  }

  if (block1.is_used()) {
    ca_.Bind(&block1);
    ca_.Goto(label_IfRangeError);
  }

  TNode<JSTypedArray> phi_bb26_6;
    ca_.Bind(&block26, &phi_bb26_6);
  return TNode<JSTypedArray>{phi_bb26_6};
}


TNode<JSTypedArray> ConstructByLength_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Map> p_map, TNode<Object> p_lengthObj, TorqueStructTypedArrayElementsInfo p_elementsInfo) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<UintPtrT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = ToIndex_0(state_, TNode<Context>{p_context}, TNode<Object>{p_lengthObj}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  if (block5.is_used()) {
    ca_.Bind(&block5);
    ca_.Goto(&block3);
  }

  TNode<JSReceiver> tmp2;
  TNode<JSTypedArray> tmp3;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    tmp2 = GetArrayBufferFunction_0(state_, TNode<Context>{p_context});
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = TypedArrayInitialize_0(state_, TNode<Context>{p_context}, true, TNode<Map>{p_map}, TNode<UintPtrT>{tmp0}, TorqueStructTypedArrayElementsInfo{TNode<UintPtrT>{p_elementsInfo.sizeLog2}, TNode<Int32T>{p_elementsInfo.kind}}, TNode<JSReceiver>{tmp2}, &label4);
    ca_.Goto(&block6);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block7);
    }
  }

  if (block7.is_used()) {
    ca_.Bind(&block7);
    ca_.Goto(&block3);
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block8);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{p_context}, MessageTemplate::kInvalidTypedArrayLength, TNode<Object>{p_lengthObj});
  }

    ca_.Bind(&block8);
  return TNode<JSTypedArray>{tmp3};
}


TNode<JSTypedArray> ConstructByArrayLike_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<Map> p_map, TNode<HeapObject> p_arrayLike, TNode<UintPtrT> p_length, TorqueStructTypedArrayElementsInfo p_elementsInfo, TNode<JSReceiver> p_bufferConstructor) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSTypedArray> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = TypedArrayInitialize_0(state_, TNode<Context>{p_context}, false, TNode<Map>{p_map}, TNode<UintPtrT>{p_length}, TorqueStructTypedArrayElementsInfo{TNode<UintPtrT>{p_elementsInfo.sizeLog2}, TNode<Int32T>{p_elementsInfo.kind}}, TNode<JSReceiver>{p_bufferConstructor}, &label1);
    ca_.Goto(&block4);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block5);
    }
  }

  TNode<Number> tmp2;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp2 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{p_length});
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{p_context}, MessageTemplate::kInvalidTypedArrayLength, TNode<Object>{tmp2});
  }

  TNode<JSTypedArray> tmp3;
  if (block4.is_used()) {
    ca_.Bind(&block4);
    compiler::CodeAssemblerLabel label4(&ca_);
    tmp3 = Cast_JSTypedArray_0(state_, TNode<HeapObject>{p_arrayLike}, &label4);
    ca_.Goto(&block8);
    if (label4.is_used()) {
      ca_.Bind(&label4);
      ca_.Goto(&block9);
    }
  }

  if (block9.is_used()) {
    ca_.Bind(&block9);
    ca_.Goto(&block7);
  }

  TNode<IntPtrT> tmp5;
  TNode<JSArrayBuffer> tmp6;
  TNode<BoolT> tmp7;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp6 = CodeStubAssembler(state_).LoadReference<JSArrayBuffer>(CodeStubAssembler::Reference{tmp3, tmp5});
    tmp7 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{tmp6});
    ca_.Branch(tmp7, &block10, std::vector<compiler::Node*>{}, &block11, std::vector<compiler::Node*>{});
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kDetachedOperation, "Construct");
  }

  TNode<Int32T> tmp8;
  TNode<BoolT> tmp9;
  if (block11.is_used()) {
    ca_.Bind(&block11);
    tmp8 = CodeStubAssembler(state_).LoadElementsKind(TNode<JSTypedArray>{tmp3});
    tmp9 = ElementsKindNotEqual_0(state_, TNode<Int32T>{tmp8}, TNode<Int32T>{p_elementsInfo.kind});
    ca_.Branch(tmp9, &block13, std::vector<compiler::Node*>{}, &block14, std::vector<compiler::Node*>{});
  }

  if (block13.is_used()) {
    ca_.Bind(&block13);
    ca_.Goto(&block7);
  }

  TNode<UintPtrT> tmp10;
  TNode<BoolT> tmp11;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp10 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp11 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{p_length}, TNode<UintPtrT>{tmp10});
    ca_.Branch(tmp11, &block16, std::vector<compiler::Node*>{}, &block17, std::vector<compiler::Node*>{});
  }

  TNode<IntPtrT> tmp12;
  TNode<UintPtrT> tmp13;
  TNode<IntPtrT> tmp14;
  TNode<JSArrayBuffer> tmp15;
  TNode<BoolT> tmp16;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp12 = FromConstexpr_intptr_constexpr_int31_0(state_, 24);
    tmp13 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp0, tmp12});
    tmp14 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp15 = CodeStubAssembler(state_).LoadReference<JSArrayBuffer>(CodeStubAssembler::Reference{tmp3, tmp14});
    tmp16 = IsSharedArrayBuffer_0(state_, TNode<JSArrayBuffer>{tmp15});
    ca_.Branch(tmp16, &block22, std::vector<compiler::Node*>{}, &block23, std::vector<compiler::Node*>{});
  }

  TNode<RawPtrT> tmp17;
  TNode<RawPtrT> tmp18;
  if (block22.is_used()) {
    ca_.Bind(&block22);
    tmp17 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp18 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp3});
    TypedArrayBuiltinsAssembler(state_).CallCRelaxedMemcpy(TNode<RawPtrT>{tmp17}, TNode<RawPtrT>{tmp18}, TNode<UintPtrT>{tmp13});
    ca_.Goto(&block24);
  }

  TNode<RawPtrT> tmp19;
  TNode<RawPtrT> tmp20;
  if (block23.is_used()) {
    ca_.Bind(&block23);
    tmp19 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp0});
    tmp20 = CodeStubAssembler(state_).LoadJSTypedArrayDataPtr(TNode<JSTypedArray>{tmp3});
    TypedArrayBuiltinsAssembler(state_).CallCMemcpy(TNode<RawPtrT>{tmp19}, TNode<RawPtrT>{tmp20}, TNode<UintPtrT>{tmp13});
    ca_.Goto(&block24);
  }

  if (block24.is_used()) {
    ca_.Bind(&block24);
    ca_.Goto(&block17);
  }

  if (block17.is_used()) {
    ca_.Bind(&block17);
    ca_.Goto(&block6);
  }

  TNode<UintPtrT> tmp21;
  TNode<BoolT> tmp22;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp21 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp22 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{p_length}, TNode<UintPtrT>{tmp21});
    ca_.Branch(tmp22, &block25, std::vector<compiler::Node*>{}, &block26, std::vector<compiler::Node*>{});
  }

  TNode<Number> tmp23;
  if (block25.is_used()) {
    ca_.Bind(&block25);
    tmp23 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{p_length});
    CodeStubAssembler(state_).CallRuntime(Runtime::kTypedArrayCopyElements, p_context, tmp0, p_arrayLike, tmp23);
    ca_.Goto(&block26);
  }

  if (block26.is_used()) {
    ca_.Bind(&block26);
    ca_.Goto(&block6);
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block27);
  }

    ca_.Bind(&block27);
  return TNode<JSTypedArray>{tmp0};
}


void ConstructByIterable_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_iterable, TNode<JSReceiver> p_iteratorFn, compiler::CodeAssemblerLabel* label_IfConstructByArrayLike, compiler::TypedCodeAssemblerVariable<JSArray>* label_IfConstructByArrayLike_parameter_0, compiler::TypedCodeAssemblerVariable<UintPtrT>* label_IfConstructByArrayLike_parameter_1, compiler::TypedCodeAssemblerVariable<JSReceiver>* label_IfConstructByArrayLike_parameter_2) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSArray> tmp0;
  TNode<UintPtrT> tmp1;
  TNode<JSReceiver> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = ca_.CallStub<JSArray>(Builtins::CallableFor(ca_.isolate(), Builtin::kIterableToListMayPreserveHoles), p_context, p_iterable, p_iteratorFn);
    tmp1 = LoadJSArrayLengthAsUintPtr_0(state_, TNode<JSArray>{tmp0});
    tmp2 = GetArrayBufferFunction_0(state_, TNode<Context>{p_context});
    *label_IfConstructByArrayLike_parameter_2 = tmp2;
    *label_IfConstructByArrayLike_parameter_1 = tmp1;
    *label_IfConstructByArrayLike_parameter_0 = tmp0;
    ca_.Goto(label_IfConstructByArrayLike);
  }
}


void ConstructByTypedArray_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSTypedArray> p_srcTypedArray, compiler::CodeAssemblerLabel* label_IfConstructByArrayLike, compiler::TypedCodeAssemblerVariable<JSTypedArray>* label_IfConstructByArrayLike_parameter_0, compiler::TypedCodeAssemblerVariable<UintPtrT>* label_IfConstructByArrayLike_parameter_1, compiler::TypedCodeAssemblerVariable<JSReceiver>* label_IfConstructByArrayLike_parameter_2) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<UintPtrT> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSReceiver, UintPtrT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<JSArrayBuffer> tmp2;
  TNode<BoolT> tmp3;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = GetArrayBufferFunction_0(state_, TNode<Context>{p_context});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp2 = CodeStubAssembler(state_).LoadReference<JSArrayBuffer>(CodeStubAssembler::Reference{p_srcTypedArray, tmp1});
    tmp3 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{tmp2});
    ca_.Branch(tmp3, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<UintPtrT> tmp4;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp4 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block4, tmp4);
  }

  TNode<IntPtrT> tmp5;
  TNode<UintPtrT> tmp6;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp5 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp6 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{p_srcTypedArray, tmp5});
    ca_.Goto(&block4, tmp6);
  }

  TNode<UintPtrT> phi_bb4_4;
  TNode<BoolT> tmp7;
  TNode<BoolT> tmp8;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_4);
    tmp7 = IsSharedArrayBuffer_0(state_, TNode<JSArrayBuffer>{tmp2});
    tmp8 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp7});
    ca_.Branch(tmp8, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{tmp0, phi_bb4_4});
  }

  TNode<JSReceiver> tmp9;
  TNode<BoolT> tmp10;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp9 = CodeStubAssembler(state_).SpeciesConstructor(TNode<Context>{p_context}, TNode<Object>{tmp2}, TNode<JSReceiver>{tmp0});
    tmp10 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{tmp2});
    ca_.Branch(tmp10, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{phi_bb4_4});
  }

  TNode<UintPtrT> tmp11;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp11 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    ca_.Goto(&block9, tmp11);
  }

  TNode<UintPtrT> phi_bb9_4;
  if (block9.is_used()) {
    ca_.Bind(&block9, &phi_bb9_4);
    ca_.Goto(&block7, tmp9, phi_bb9_4);
  }

  TNode<JSReceiver> phi_bb7_2;
  TNode<UintPtrT> phi_bb7_4;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_2, &phi_bb7_4);
    *label_IfConstructByArrayLike_parameter_2 = phi_bb7_2;
    *label_IfConstructByArrayLike_parameter_1 = phi_bb7_4;
    *label_IfConstructByArrayLike_parameter_0 = p_srcTypedArray;
    ca_.Goto(label_IfConstructByArrayLike);
  }
}


TNode<JSTypedArray> ConstructByArrayBuffer_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSFunction> p_target, TNode<JSReceiver> p_newTarget, TNode<JSArrayBuffer> p_buffer, TNode<Object> p_byteOffset, TNode<Object> p_length) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Map, BoolT> block13(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block22(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block27(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block28(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block31(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block32(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, BoolT> block33(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block29(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block38(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block37(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block30(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT> block34(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block39(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block42(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block43(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block45(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block46(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block48(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block49(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block40(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block51(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block52(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block53(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block54(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block57(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block58(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, BoolT> block59(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block55(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block56(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, UintPtrT, UintPtrT> block41(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT, String> block19(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block17(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<BoolT> block15(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block60(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<BoolT> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = IsResizableArrayBuffer_0(state_, TNode<JSArrayBuffer>{p_buffer});
    ca_.Branch(tmp0, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  TNode<Oddball> tmp1;
  TNode<BoolT> tmp2;
  if (block2.is_used()) {
    ca_.Bind(&block2);
    tmp1 = Undefined_0(state_);
    tmp2 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{p_length}, TNode<HeapObject>{tmp1});
    ca_.Goto(&block4, tmp2);
  }

  TNode<BoolT> tmp3;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp3 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block4, tmp3);
  }

  TNode<BoolT> phi_bb4_8;
  TNode<BoolT> tmp4;
  if (block4.is_used()) {
    ca_.Bind(&block4, &phi_bb4_8);
    tmp4 = IsResizableArrayBuffer_0(state_, TNode<JSArrayBuffer>{p_buffer});
    ca_.Branch(tmp4, &block5, std::vector<compiler::Node*>{}, &block6, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp5;
  TNode<BoolT> tmp6;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp5 = IsSharedArrayBuffer_0(state_, TNode<JSArrayBuffer>{p_buffer});
    tmp6 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp5});
    ca_.Branch(tmp6, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  TNode<BoolT> tmp7;
  if (block8.is_used()) {
    ca_.Bind(&block8);
    tmp7 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block10, tmp7);
  }

  if (block9.is_used()) {
    ca_.Bind(&block9);
    ca_.Goto(&block10, phi_bb4_8);
  }

  TNode<BoolT> phi_bb10_10;
  if (block10.is_used()) {
    ca_.Bind(&block10, &phi_bb10_10);
    ca_.Goto(&block7, phi_bb10_10);
  }

  TNode<BoolT> tmp8;
  if (block6.is_used()) {
    ca_.Bind(&block6);
    tmp8 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block7, tmp8);
  }

  TNode<BoolT> phi_bb7_9;
  if (block7.is_used()) {
    ca_.Bind(&block7, &phi_bb7_9);
    ca_.Branch(phi_bb7_9, &block11, std::vector<compiler::Node*>{phi_bb7_9}, &block12, std::vector<compiler::Node*>{phi_bb7_9});
  }

  TNode<BoolT> phi_bb11_8;
  TNode<Map> tmp9;
  if (block11.is_used()) {
    ca_.Bind(&block11, &phi_bb11_8);
    tmp9 = GetDerivedRabGsabMap_0(state_, TNode<Context>{p_context}, TNode<JSFunction>{p_target}, TNode<JSReceiver>{p_newTarget});
    ca_.Goto(&block13, tmp9, phi_bb11_8);
  }

  TNode<BoolT> phi_bb12_8;
  TNode<Map> tmp10;
  if (block12.is_used()) {
    ca_.Bind(&block12, &phi_bb12_8);
    tmp10 = GetDerivedMap_0(state_, TNode<Context>{p_context}, TNode<JSFunction>{p_target}, TNode<JSReceiver>{p_newTarget});
    ca_.Goto(&block13, tmp10, phi_bb12_8);
  }

  TNode<Map> phi_bb13_6;
  TNode<BoolT> phi_bb13_8;
  TNode<UintPtrT> tmp11;
  TNode<Int32T> tmp12;
  TNode<UintPtrT> tmp13;
  if (block13.is_used()) {
    ca_.Bind(&block13, &phi_bb13_6, &phi_bb13_8);
    std::tie(tmp11, tmp12) = TypedArrayBuiltinsAssembler(state_).GetTypedArrayElementsInfo(TNode<Map>{phi_bb13_6}).Flatten();
    compiler::CodeAssemblerLabel label14(&ca_);
    tmp13 = ToIndex_0(state_, TNode<Context>{p_context}, TNode<Object>{p_byteOffset}, &label14);
    ca_.Goto(&block20, phi_bb13_8);
    if (label14.is_used()) {
      ca_.Bind(&label14);
      ca_.Goto(&block21, phi_bb13_8);
    }
  }

  TNode<BoolT> phi_bb21_8;
  if (block21.is_used()) {
    ca_.Bind(&block21, &phi_bb21_8);
    ca_.Goto(&block15, phi_bb21_8);
  }

  TNode<BoolT> phi_bb20_8;
  TNode<UintPtrT> tmp15;
  TNode<UintPtrT> tmp16;
  TNode<UintPtrT> tmp17;
  TNode<UintPtrT> tmp18;
  TNode<UintPtrT> tmp19;
  TNode<UintPtrT> tmp20;
  TNode<BoolT> tmp21;
  if (block20.is_used()) {
    ca_.Bind(&block20, &phi_bb20_8);
    tmp15 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp16 = CodeStubAssembler(state_).WordShl(TNode<UintPtrT>{tmp15}, TNode<UintPtrT>{tmp11});
    tmp17 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp18 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{tmp16}, TNode<UintPtrT>{tmp17});
    tmp19 = CodeStubAssembler(state_).WordAnd(TNode<UintPtrT>{tmp13}, TNode<UintPtrT>{tmp18});
    tmp20 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp21 = CodeStubAssembler(state_).WordNotEqual(TNode<UintPtrT>{tmp19}, TNode<UintPtrT>{tmp20});
    ca_.Branch(tmp21, &block22, std::vector<compiler::Node*>{phi_bb20_8}, &block23, std::vector<compiler::Node*>{phi_bb20_8});
  }

  TNode<BoolT> phi_bb22_8;
  TNode<String> tmp22;
  if (block22.is_used()) {
    ca_.Bind(&block22, &phi_bb22_8);
    tmp22 = FromConstexpr_String_constexpr_string_0(state_, "start offset");
    ca_.Goto(&block19, phi_bb22_8, tmp22);
  }

  TNode<BoolT> phi_bb23_8;
  TNode<UintPtrT> tmp23;
  if (block23.is_used()) {
    ca_.Bind(&block23, &phi_bb23_8);
    compiler::CodeAssemblerLabel label24(&ca_);
    tmp23 = ToIndex_0(state_, TNode<Context>{p_context}, TNode<Object>{p_length}, &label24);
    ca_.Goto(&block25, phi_bb23_8);
    if (label24.is_used()) {
      ca_.Bind(&label24);
      ca_.Goto(&block26, phi_bb23_8);
    }
  }

  TNode<BoolT> phi_bb26_8;
  if (block26.is_used()) {
    ca_.Bind(&block26, &phi_bb26_8);
    ca_.Goto(&block17, phi_bb26_8);
  }

  TNode<BoolT> phi_bb25_8;
  TNode<BoolT> tmp25;
  if (block25.is_used()) {
    ca_.Bind(&block25, &phi_bb25_8);
    tmp25 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{p_buffer});
    ca_.Branch(tmp25, &block27, std::vector<compiler::Node*>{phi_bb25_8}, &block28, std::vector<compiler::Node*>{phi_bb25_8});
  }

  TNode<BoolT> phi_bb27_8;
  if (block27.is_used()) {
    ca_.Bind(&block27, &phi_bb27_8);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kDetachedOperation, "Construct");
  }

  TNode<BoolT> phi_bb28_8;
  TNode<BoolT> tmp26;
  if (block28.is_used()) {
    ca_.Bind(&block28, &phi_bb28_8);
    tmp26 = IsResizableArrayBuffer_0(state_, TNode<JSArrayBuffer>{p_buffer});
    ca_.Branch(tmp26, &block31, std::vector<compiler::Node*>{phi_bb28_8}, &block32, std::vector<compiler::Node*>{phi_bb28_8});
  }

  TNode<BoolT> phi_bb31_8;
  TNode<BoolT> tmp27;
  if (block31.is_used()) {
    ca_.Bind(&block31, &phi_bb31_8);
    tmp27 = IsSharedArrayBuffer_0(state_, TNode<JSArrayBuffer>{p_buffer});
    ca_.Goto(&block33, phi_bb31_8, tmp27);
  }

  TNode<BoolT> phi_bb32_8;
  TNode<BoolT> tmp28;
  if (block32.is_used()) {
    ca_.Bind(&block32, &phi_bb32_8);
    tmp28 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    ca_.Goto(&block33, phi_bb32_8, tmp28);
  }

  TNode<BoolT> phi_bb33_8;
  TNode<BoolT> phi_bb33_16;
  if (block33.is_used()) {
    ca_.Bind(&block33, &phi_bb33_8, &phi_bb33_16);
    ca_.Branch(phi_bb33_16, &block29, std::vector<compiler::Node*>{phi_bb33_8}, &block30, std::vector<compiler::Node*>{phi_bb33_8});
  }

  TNode<BoolT> phi_bb29_8;
  TNode<Object> tmp29;
  TNode<UintPtrT> tmp30;
  if (block29.is_used()) {
    ca_.Bind(&block29, &phi_bb29_8);
    tmp29 = CodeStubAssembler(state_).CallRuntime(Runtime::kGrowableSharedArrayBufferByteLength, p_context, p_buffer); 
    compiler::CodeAssemblerLabel label31(&ca_);
    tmp30 = ToIndex_0(state_, TNode<Context>{p_context}, TNode<Object>{tmp29}, &label31);
    ca_.Goto(&block37, phi_bb29_8);
    if (label31.is_used()) {
      ca_.Bind(&label31);
      ca_.Goto(&block38, phi_bb29_8);
    }
  }

  TNode<BoolT> phi_bb38_8;
  if (block38.is_used()) {
    ca_.Bind(&block38, &phi_bb38_8);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BoolT> phi_bb37_8;
  if (block37.is_used()) {
    ca_.Bind(&block37, &phi_bb37_8);
    ca_.Goto(&block34, phi_bb37_8, tmp30);
  }

  TNode<BoolT> phi_bb30_8;
  TNode<IntPtrT> tmp32;
  TNode<UintPtrT> tmp33;
  if (block30.is_used()) {
    ca_.Bind(&block30, &phi_bb30_8);
    tmp32 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp33 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{p_buffer, tmp32});
    ca_.Goto(&block34, phi_bb30_8, tmp33);
  }

  TNode<BoolT> phi_bb34_8;
  TNode<UintPtrT> phi_bb34_14;
  TNode<Oddball> tmp34;
  TNode<BoolT> tmp35;
  if (block34.is_used()) {
    ca_.Bind(&block34, &phi_bb34_8, &phi_bb34_14);
    tmp34 = Undefined_0(state_);
    tmp35 = CodeStubAssembler(state_).TaggedEqual(TNode<Object>{p_length}, TNode<HeapObject>{tmp34});
    ca_.Branch(tmp35, &block39, std::vector<compiler::Node*>{phi_bb34_8}, &block40, std::vector<compiler::Node*>{phi_bb34_8});
  }

  TNode<BoolT> phi_bb39_8;
  TNode<UintPtrT> tmp36;
  TNode<UintPtrT> tmp37;
  TNode<UintPtrT> tmp38;
  TNode<UintPtrT> tmp39;
  TNode<UintPtrT> tmp40;
  TNode<UintPtrT> tmp41;
  TNode<BoolT> tmp42;
  if (block39.is_used()) {
    ca_.Bind(&block39, &phi_bb39_8);
    tmp36 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp37 = CodeStubAssembler(state_).WordShl(TNode<UintPtrT>{tmp36}, TNode<UintPtrT>{tmp11});
    tmp38 = FromConstexpr_uintptr_constexpr_int31_0(state_, 1);
    tmp39 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{tmp37}, TNode<UintPtrT>{tmp38});
    tmp40 = CodeStubAssembler(state_).WordAnd(TNode<UintPtrT>{phi_bb34_14}, TNode<UintPtrT>{tmp39});
    tmp41 = FromConstexpr_uintptr_constexpr_int31_0(state_, 0);
    tmp42 = CodeStubAssembler(state_).WordNotEqual(TNode<UintPtrT>{tmp40}, TNode<UintPtrT>{tmp41});
    ca_.Branch(tmp42, &block42, std::vector<compiler::Node*>{phi_bb39_8}, &block43, std::vector<compiler::Node*>{phi_bb39_8});
  }

  TNode<BoolT> phi_bb42_8;
  TNode<String> tmp43;
  if (block42.is_used()) {
    ca_.Bind(&block42, &phi_bb42_8);
    tmp43 = FromConstexpr_String_constexpr_string_0(state_, "byte length");
    ca_.Goto(&block19, phi_bb42_8, tmp43);
  }

  TNode<BoolT> phi_bb43_8;
  TNode<BoolT> tmp44;
  if (block43.is_used()) {
    ca_.Bind(&block43, &phi_bb43_8);
    tmp44 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb34_14}, TNode<UintPtrT>{tmp13});
    ca_.Branch(tmp44, &block45, std::vector<compiler::Node*>{phi_bb43_8}, &block46, std::vector<compiler::Node*>{phi_bb43_8});
  }

  TNode<BoolT> phi_bb45_8;
  if (block45.is_used()) {
    ca_.Bind(&block45, &phi_bb45_8);
    ca_.Goto(&block15, phi_bb45_8);
  }

  TNode<BoolT> phi_bb46_8;
  TNode<UintPtrT> tmp45;
  TNode<UintPtrT> tmp46;
  TNode<UintPtrT> tmp47;
  TNode<BoolT> tmp48;
  if (block46.is_used()) {
    ca_.Bind(&block46, &phi_bb46_8);
    tmp45 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{phi_bb34_14}, TNode<UintPtrT>{tmp13});
    tmp46 = CodeStubAssembler(state_).WordShr(TNode<UintPtrT>{tmp45}, TNode<UintPtrT>{tmp11});
    tmp47 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSTypedArray::kMaxLength);
    tmp48 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp46}, TNode<UintPtrT>{tmp47});
    ca_.Branch(tmp48, &block48, std::vector<compiler::Node*>{phi_bb46_8}, &block49, std::vector<compiler::Node*>{phi_bb46_8});
  }

  TNode<BoolT> phi_bb48_8;
  if (block48.is_used()) {
    ca_.Bind(&block48, &phi_bb48_8);
    ca_.Goto(&block15, phi_bb48_8);
  }

  TNode<BoolT> phi_bb49_8;
  if (block49.is_used()) {
    ca_.Bind(&block49, &phi_bb49_8);
    ca_.Goto(&block41, phi_bb49_8, tmp46, tmp45);
  }

  TNode<BoolT> phi_bb40_8;
  TNode<UintPtrT> tmp49;
  TNode<BoolT> tmp50;
  if (block40.is_used()) {
    ca_.Bind(&block40, &phi_bb40_8);
    tmp49 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSTypedArray::kMaxLength);
    tmp50 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp23}, TNode<UintPtrT>{tmp49});
    ca_.Branch(tmp50, &block51, std::vector<compiler::Node*>{phi_bb40_8}, &block52, std::vector<compiler::Node*>{phi_bb40_8});
  }

  TNode<BoolT> phi_bb51_8;
  if (block51.is_used()) {
    ca_.Bind(&block51, &phi_bb51_8);
    ca_.Goto(&block17, phi_bb51_8);
  }

  TNode<BoolT> phi_bb52_8;
  TNode<UintPtrT> tmp51;
  TNode<UintPtrT> tmp52;
  TNode<BoolT> tmp53;
  if (block52.is_used()) {
    ca_.Bind(&block52, &phi_bb52_8);
    tmp51 = FromConstexpr_uintptr_constexpr_uintptr_0(state_, JSArrayBuffer::kMaxByteLength);
    tmp52 = CodeStubAssembler(state_).WordShr(TNode<UintPtrT>{tmp51}, TNode<UintPtrT>{tmp11});
    tmp53 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp23}, TNode<UintPtrT>{tmp52});
    ca_.Branch(tmp53, &block53, std::vector<compiler::Node*>{phi_bb52_8}, &block54, std::vector<compiler::Node*>{phi_bb52_8});
  }

  TNode<BoolT> phi_bb53_8;
  if (block53.is_used()) {
    ca_.Bind(&block53, &phi_bb53_8);
    ca_.Goto(&block17, phi_bb53_8);
  }

  TNode<BoolT> phi_bb54_8;
  TNode<UintPtrT> tmp54;
  TNode<BoolT> tmp55;
  if (block54.is_used()) {
    ca_.Bind(&block54, &phi_bb54_8);
    tmp54 = CodeStubAssembler(state_).WordShl(TNode<UintPtrT>{tmp23}, TNode<UintPtrT>{tmp11});
    tmp55 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{phi_bb34_14}, TNode<UintPtrT>{tmp54});
    ca_.Branch(tmp55, &block57, std::vector<compiler::Node*>{phi_bb54_8}, &block58, std::vector<compiler::Node*>{phi_bb54_8});
  }

  TNode<BoolT> phi_bb57_8;
  TNode<BoolT> tmp56;
  if (block57.is_used()) {
    ca_.Bind(&block57, &phi_bb57_8);
    tmp56 = FromConstexpr_bool_constexpr_bool_0(state_, true);
    ca_.Goto(&block59, phi_bb57_8, tmp56);
  }

  TNode<BoolT> phi_bb58_8;
  TNode<UintPtrT> tmp57;
  TNode<BoolT> tmp58;
  if (block58.is_used()) {
    ca_.Bind(&block58, &phi_bb58_8);
    tmp57 = CodeStubAssembler(state_).UintPtrSub(TNode<UintPtrT>{phi_bb34_14}, TNode<UintPtrT>{tmp54});
    tmp58 = CodeStubAssembler(state_).UintPtrGreaterThan(TNode<UintPtrT>{tmp13}, TNode<UintPtrT>{tmp57});
    ca_.Goto(&block59, phi_bb58_8, tmp58);
  }

  TNode<BoolT> phi_bb59_8;
  TNode<BoolT> phi_bb59_16;
  if (block59.is_used()) {
    ca_.Bind(&block59, &phi_bb59_8, &phi_bb59_16);
    ca_.Branch(phi_bb59_16, &block55, std::vector<compiler::Node*>{phi_bb59_8}, &block56, std::vector<compiler::Node*>{phi_bb59_8});
  }

  TNode<BoolT> phi_bb55_8;
  if (block55.is_used()) {
    ca_.Bind(&block55, &phi_bb55_8);
    ca_.Goto(&block17, phi_bb55_8);
  }

  TNode<BoolT> phi_bb56_8;
  if (block56.is_used()) {
    ca_.Bind(&block56, &phi_bb56_8);
    ca_.Goto(&block41, phi_bb56_8, tmp23, tmp54);
  }

  TNode<BoolT> phi_bb41_8;
  TNode<UintPtrT> phi_bb41_12;
  TNode<UintPtrT> phi_bb41_13;
  TNode<JSTypedArray> tmp59;
  if (block41.is_used()) {
    ca_.Bind(&block41, &phi_bb41_8, &phi_bb41_12, &phi_bb41_13);
    tmp59 = AllocateTypedArray_0(state_, TNode<Context>{p_context}, false, TNode<Map>{phi_bb13_6}, TNode<JSArrayBuffer>{p_buffer}, TNode<UintPtrT>{tmp13}, TNode<UintPtrT>{phi_bb41_13}, TNode<UintPtrT>{phi_bb41_12}, TNode<BoolT>{phi_bb4_8});
    ca_.Goto(&block60);
  }

  TNode<BoolT> phi_bb19_8;
  TNode<String> phi_bb19_11;
  if (block19.is_used()) {
    ca_.Bind(&block19, &phi_bb19_8, &phi_bb19_11);
    CodeStubAssembler(state_).CallRuntime(Runtime::kThrowInvalidTypedArrayAlignment, p_context, phi_bb13_6, phi_bb19_11);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<BoolT> phi_bb17_8;
  if (block17.is_used()) {
    ca_.Bind(&block17, &phi_bb17_8);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{p_context}, MessageTemplate::kInvalidTypedArrayLength, TNode<Object>{p_length});
  }

  TNode<BoolT> phi_bb15_8;
  if (block15.is_used()) {
    ca_.Bind(&block15, &phi_bb15_8);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{p_context}, MessageTemplate::kInvalidOffset, TNode<Object>{p_byteOffset});
  }

    ca_.Bind(&block60);
  return TNode<JSTypedArray>{tmp59};
}


TNode<JSTypedArray> TypedArrayCreateByLength_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_constructor, TNode<Number> p_length, const char* p_methodName) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSReceiver> tmp0;
  TNode<JSTypedArray> tmp1;
  TNode<IntPtrT> tmp2;
  TNode<Uint32T> tmp3;
  TNode<BoolT> tmp4;
  TNode<Uint32T> tmp5;
  TNode<IntPtrT> tmp6;
  TNode<Uint32T> tmp7;
  TNode<BoolT> tmp8;
  TNode<Uint32T> tmp9;
  TNode<IntPtrT> tmp10;
  TNode<JSArrayBuffer> tmp11;
  TNode<BoolT> tmp12;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).Construct(TNode<Context>{p_context}, TNode<JSReceiver>{p_constructor}, TNode<Object>{p_length});
    tmp1 = TypedArrayBuiltinsAssembler(state_).ValidateTypedArray(TNode<Context>{p_context}, TNode<Object>{tmp0}, p_methodName);
    tmp2 = FromConstexpr_intptr_constexpr_int31_0(state_, 52);
    tmp3 = CodeStubAssembler(state_).LoadReference<Uint32T>(CodeStubAssembler::Reference{tmp1, tmp2});
    tmp4 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    tmp5 = ca_.UncheckedCast<Uint32T>(CodeStubAssembler(state_).UpdateWord32<base::BitField<bool, 0, 1, uint32_t>>(ca_.UncheckedCast<Word32T>(tmp3), ca_.UncheckedCast<Uint32T>(tmp4)));
    CodeStubAssembler(state_).StoreReference<Uint32T>(CodeStubAssembler::Reference{tmp1, tmp2}, tmp5);
    tmp6 = FromConstexpr_intptr_constexpr_int31_0(state_, 52);
    tmp7 = CodeStubAssembler(state_).LoadReference<Uint32T>(CodeStubAssembler::Reference{tmp1, tmp6});
    tmp8 = FromConstexpr_bool_constexpr_bool_0(state_, false);
    tmp9 = ca_.UncheckedCast<Uint32T>(CodeStubAssembler(state_).UpdateWord32<base::BitField<bool, 1, 1, uint32_t>>(ca_.UncheckedCast<Word32T>(tmp7), ca_.UncheckedCast<Uint32T>(tmp8)));
    CodeStubAssembler(state_).StoreReference<Uint32T>(CodeStubAssembler::Reference{tmp1, tmp6}, tmp9);
    tmp10 = FromConstexpr_intptr_constexpr_int31_0(state_, 12);
    tmp11 = CodeStubAssembler(state_).LoadReference<JSArrayBuffer>(CodeStubAssembler::Reference{tmp1, tmp10});
    tmp12 = IsDetachedBuffer_0(state_, TNode<JSArrayBuffer>{tmp11});
    ca_.Branch(tmp12, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kDetachedOperation, p_methodName);
  }

  TNode<IntPtrT> tmp13;
  TNode<UintPtrT> tmp14;
  TNode<UintPtrT> tmp15;
  TNode<BoolT> tmp16;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp13 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp14 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp1, tmp13});
    tmp15 = Convert_uintptr_Number_0(state_, TNode<Number>{p_length});
    tmp16 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp14}, TNode<UintPtrT>{tmp15});
    ca_.Branch(tmp16, &block8, std::vector<compiler::Node*>{}, &block9, std::vector<compiler::Node*>{});
  }

  if (block8.is_used()) {
    ca_.Bind(&block8);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kTypedArrayTooShort);
  }

  if (block9.is_used()) {
    ca_.Bind(&block9);
    ca_.Goto(&block10);
  }

    ca_.Bind(&block10);
  return TNode<JSTypedArray>{tmp1};
}


void ConstructByJSReceiver_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, TNode<JSReceiver> p_obj, compiler::CodeAssemblerLabel* label_IfConstructByArrayLike, compiler::TypedCodeAssemblerVariable<JSReceiver>* label_IfConstructByArrayLike_parameter_0, compiler::TypedCodeAssemblerVariable<UintPtrT>* label_IfConstructByArrayLike_parameter_1, compiler::TypedCodeAssemblerVariable<JSReceiver>* label_IfConstructByArrayLike_parameter_2) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block9(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block10(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSReceiver, UintPtrT, JSReceiver> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Symbol> tmp0;
  TNode<JSReceiver> tmp1;
    compiler::TypedCodeAssemblerVariable<Object> tmp4(&ca_);
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = CodeStubAssembler(state_).IteratorSymbolConstant();
    compiler::CodeAssemblerLabel label2(&ca_);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp1 = GetMethod_0(state_, TNode<Context>{p_context}, TNode<Object>{p_obj}, TNode<Name>{tmp0}, &label2, &label3, &tmp4);
    ca_.Goto(&block8);
    if (label2.is_used()) {
      ca_.Bind(&label2);
      ca_.Goto(&block9);
    }
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block10);
    }
  }

  TNode<String> tmp5;
  TNode<Object> tmp6;
  TNode<Number> tmp7;
  TNode<UintPtrT> tmp8;
  if (block9.is_used()) {
    ca_.Bind(&block9);
    tmp5 = kLengthString_0(state_);
    tmp6 = CodeStubAssembler(state_).GetProperty(TNode<Context>{p_context}, TNode<Object>{p_obj}, TNode<Object>{tmp5});
    tmp7 = CodeStubAssembler(state_).ToLength_Inline(TNode<Context>{p_context}, TNode<Object>{tmp6});
    compiler::CodeAssemblerLabel label9(&ca_);
    tmp8 = ChangeSafeIntegerNumberToUintPtr_0(state_, TNode<Number>{tmp7}, &label9);
    ca_.Goto(&block14);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block15);
    }
  }

  if (block10.is_used()) {
    ca_.Bind(&block10);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kIteratorSymbolNonCallable);
  }

    compiler::TypedCodeAssemblerVariable<JSArray> tmp11(&ca_);
    compiler::TypedCodeAssemblerVariable<UintPtrT> tmp12(&ca_);
    compiler::TypedCodeAssemblerVariable<JSReceiver> tmp13(&ca_);
  if (block8.is_used()) {
    ca_.Bind(&block8);
    compiler::CodeAssemblerLabel label10(&ca_);
    ConstructByIterable_0(state_, TNode<Context>{p_context}, TNode<JSReceiver>{p_obj}, TNode<JSReceiver>{tmp1}, &label10, &tmp11, &tmp12, &tmp13);
    if (label10.is_used()) {
      ca_.Bind(&label10);
      ca_.Goto(&block11);
    }
  }

  if (block11.is_used()) {
    ca_.Bind(&block11);
    ca_.Goto(&block1, tmp11.value(), tmp12.value(), tmp13.value());
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    CodeStubAssembler(state_).ThrowRangeError(TNode<Context>{p_context}, MessageTemplate::kInvalidTypedArrayLength, TNode<Object>{tmp7});
  }

  TNode<JSReceiver> tmp14;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp14 = GetArrayBufferFunction_0(state_, TNode<Context>{p_context});
    ca_.Goto(&block1, p_obj, tmp8, tmp14);
  }

  TNode<JSReceiver> phi_bb1_0;
  TNode<UintPtrT> phi_bb1_1;
  TNode<JSReceiver> phi_bb1_2;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_0, &phi_bb1_1, &phi_bb1_2);
    *label_IfConstructByArrayLike_parameter_2 = phi_bb1_2;
    *label_IfConstructByArrayLike_parameter_1 = phi_bb1_1;
    *label_IfConstructByArrayLike_parameter_0 = phi_bb1_0;
    ca_.Goto(label_IfConstructByArrayLike);
  }
}


TF_BUILTIN(CreateTypedArray, CodeStubAssembler) {
  compiler::CodeAssemblerState* state_ = state();  compiler::CodeAssembler ca_(state());
  TNode<Context> parameter0 = UncheckedParameter<Context>(Descriptor::kContext);
  USE(parameter0);
  TNode<JSFunction> parameter1 = UncheckedParameter<JSFunction>(Descriptor::kTarget);
  USE(parameter1);
  TNode<JSReceiver> parameter2 = UncheckedParameter<JSReceiver>(Descriptor::kNewTarget);
  USE(parameter2);
  TNode<Object> parameter3 = UncheckedParameter<Object>(Descriptor::kArg1);
  USE(parameter3);
  TNode<Object> parameter4 = UncheckedParameter<Object>(Descriptor::kArg2);
  USE(parameter4);
  TNode<Object> parameter5 = UncheckedParameter<Object>(Descriptor::kArg3);
  USE(parameter5);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block12(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block11(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block20(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block19(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block21(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block25(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block24(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block26(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block8(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSReceiver, UintPtrT, JSReceiver> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Smi> tmp0;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    compiler::CodeAssemblerLabel label1(&ca_);
    tmp0 = Cast_Smi_0(state_, TNode<Object>{parameter3}, &label1);
    ca_.Goto(&block11);
    if (label1.is_used()) {
      ca_.Bind(&label1);
      ca_.Goto(&block12);
    }
  }

  TNode<JSArrayBuffer> tmp2;
  if (block12.is_used()) {
    ca_.Bind(&block12);
    compiler::CodeAssemblerLabel label3(&ca_);
    tmp2 = Cast_JSArrayBuffer_0(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(parameter3)}, &label3);
    ca_.Goto(&block15);
    if (label3.is_used()) {
      ca_.Bind(&label3);
      ca_.Goto(&block16);
    }
  }

  if (block11.is_used()) {
    ca_.Bind(&block11);
    ca_.Goto(&block8, tmp0);
  }

  TNode<JSTypedArray> tmp4;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    compiler::CodeAssemblerLabel label5(&ca_);
    tmp4 = Cast_JSTypedArray_0(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(parameter3)}, &label5);
    ca_.Goto(&block19);
    if (label5.is_used()) {
      ca_.Bind(&label5);
      ca_.Goto(&block20);
    }
  }

  TNode<JSTypedArray> tmp6;
  if (block15.is_used()) {
    ca_.Bind(&block15);
    tmp6 = ConstructByArrayBuffer_0(state_, TNode<Context>{parameter0}, TNode<JSFunction>{parameter1}, TNode<JSReceiver>{parameter2}, TNode<JSArrayBuffer>{tmp2}, TNode<Object>{parameter4}, TNode<Object>{parameter5});
    CodeStubAssembler(state_).Return(tmp6);
  }

  TNode<JSReceiver> tmp7;
  if (block20.is_used()) {
    ca_.Bind(&block20);
    compiler::CodeAssemblerLabel label8(&ca_);
    tmp7 = Cast_JSReceiver_0(state_, TNode<HeapObject>{ca_.UncheckedCast<HeapObject>(parameter3)}, &label8);
    ca_.Goto(&block24);
    if (label8.is_used()) {
      ca_.Bind(&label8);
      ca_.Goto(&block25);
    }
  }

    compiler::TypedCodeAssemblerVariable<JSTypedArray> tmp10(&ca_);
    compiler::TypedCodeAssemblerVariable<UintPtrT> tmp11(&ca_);
    compiler::TypedCodeAssemblerVariable<JSReceiver> tmp12(&ca_);
  if (block19.is_used()) {
    ca_.Bind(&block19);
    compiler::CodeAssemblerLabel label9(&ca_);
    ConstructByTypedArray_0(state_, TNode<Context>{parameter0}, TNode<JSTypedArray>{tmp4}, &label9, &tmp10, &tmp11, &tmp12);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block21);
    }
  }

  if (block21.is_used()) {
    ca_.Bind(&block21);
    ca_.Goto(&block6, tmp10.value(), tmp11.value(), tmp12.value());
  }

  if (block25.is_used()) {
    ca_.Bind(&block25);
    ca_.Goto(&block8, ca_.UncheckedCast<PrimitiveHeapObject>(parameter3));
  }

    compiler::TypedCodeAssemblerVariable<JSReceiver> tmp14(&ca_);
    compiler::TypedCodeAssemblerVariable<UintPtrT> tmp15(&ca_);
    compiler::TypedCodeAssemblerVariable<JSReceiver> tmp16(&ca_);
  if (block24.is_used()) {
    ca_.Bind(&block24);
    compiler::CodeAssemblerLabel label13(&ca_);
    ConstructByJSReceiver_0(state_, TNode<Context>{parameter0}, TNode<JSReceiver>{tmp7}, &label13, &tmp14, &tmp15, &tmp16);
    if (label13.is_used()) {
      ca_.Bind(&label13);
      ca_.Goto(&block26);
    }
  }

  if (block26.is_used()) {
    ca_.Bind(&block26);
    ca_.Goto(&block6, tmp14.value(), tmp15.value(), tmp16.value());
  }

  TNode<Object> phi_bb8_6;
  TNode<Map> tmp17;
  TNode<UintPtrT> tmp18;
  TNode<Int32T> tmp19;
  TNode<JSTypedArray> tmp20;
  if (block8.is_used()) {
    ca_.Bind(&block8, &phi_bb8_6);
    tmp17 = GetDerivedMap_0(state_, TNode<Context>{parameter0}, TNode<JSFunction>{parameter1}, TNode<JSReceiver>{parameter2});
    std::tie(tmp18, tmp19) = TypedArrayBuiltinsAssembler(state_).GetTypedArrayElementsInfo(TNode<Map>{tmp17}).Flatten();
    tmp20 = ConstructByLength_0(state_, TNode<Context>{parameter0}, TNode<Map>{tmp17}, TNode<Object>{phi_bb8_6}, TorqueStructTypedArrayElementsInfo{TNode<UintPtrT>{tmp18}, TNode<Int32T>{tmp19}});
    CodeStubAssembler(state_).Return(tmp20);
  }

  TNode<JSReceiver> phi_bb6_6;
  TNode<UintPtrT> phi_bb6_7;
  TNode<JSReceiver> phi_bb6_8;
  TNode<Map> tmp21;
  TNode<UintPtrT> tmp22;
  TNode<Int32T> tmp23;
  TNode<JSTypedArray> tmp24;
  if (block6.is_used()) {
    ca_.Bind(&block6, &phi_bb6_6, &phi_bb6_7, &phi_bb6_8);
    tmp21 = GetDerivedMap_0(state_, TNode<Context>{parameter0}, TNode<JSFunction>{parameter1}, TNode<JSReceiver>{parameter2});
    std::tie(tmp22, tmp23) = TypedArrayBuiltinsAssembler(state_).GetTypedArrayElementsInfo(TNode<Map>{tmp21}).Flatten();
    tmp24 = ConstructByArrayLike_0(state_, TNode<Context>{parameter0}, TNode<Map>{tmp21}, TNode<HeapObject>{phi_bb6_6}, TNode<UintPtrT>{phi_bb6_7}, TorqueStructTypedArrayElementsInfo{TNode<UintPtrT>{tmp22}, TNode<Int32T>{tmp23}}, TNode<JSReceiver>{phi_bb6_8});
    CodeStubAssembler(state_).Return(tmp24);
  }
}

TNode<JSTypedArray> TypedArraySpeciesCreate_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, const char* p_methodName, int31_t p_numArgs, TNode<JSTypedArray> p_exemplar, TNode<Object> p_arg0, TNode<Object> p_arg1, TNode<Object> p_arg2) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block5(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block6(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block7(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block15(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block14(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block16(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block17(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<Object> block18(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray> block1(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<JSTypedArray> block23(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<JSFunction> tmp0;
  TNode<IntPtrT> tmp1;
  TNode<Map> tmp2;
  TNode<BoolT> tmp3;
  TNode<BoolT> tmp4;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = TypedArrayBuiltinsAssembler(state_).GetDefaultConstructor(TNode<Context>{p_context}, TNode<JSTypedArray>{p_exemplar});
    tmp1 = FromConstexpr_intptr_constexpr_int31_0(state_, 0);
    tmp2 = CodeStubAssembler(state_).LoadReference<Map>(CodeStubAssembler::Reference{p_exemplar, tmp1});
    tmp3 = CodeStubAssembler(state_).IsPrototypeTypedArrayPrototype(TNode<Context>{p_context}, TNode<Map>{tmp2});
    tmp4 = CodeStubAssembler(state_).Word32BinaryNot(TNode<BoolT>{tmp3});
    ca_.Branch(tmp4, &block4, std::vector<compiler::Node*>{}, &block5, std::vector<compiler::Node*>{});
  }

  if (block4.is_used()) {
    ca_.Bind(&block4);
    ca_.Goto(&block3);
  }

  TNode<BoolT> tmp5;
  if (block5.is_used()) {
    ca_.Bind(&block5);
    tmp5 = CodeStubAssembler(state_).IsTypedArraySpeciesProtectorCellInvalid();
    ca_.Branch(tmp5, &block6, std::vector<compiler::Node*>{}, &block7, std::vector<compiler::Node*>{});
  }

  if (block6.is_used()) {
    ca_.Bind(&block6);
    ca_.Goto(&block3);
  }

  TNode<JSTypedArray> tmp6;
  if (block7.is_used()) {
    ca_.Bind(&block7);
    tmp6 = ca_.CallStub<JSTypedArray>(Builtins::CallableFor(ca_.isolate(), Builtin::kCreateTypedArray), p_context, tmp0, tmp0, p_arg0, p_arg1, p_arg2);
    ca_.Goto(&block1, tmp6);
  }

  TNode<JSReceiver> tmp7;
  TNode<JSReceiver> tmp8;
  if (block3.is_used()) {
    ca_.Bind(&block3);
    tmp7 = CodeStubAssembler(state_).SpeciesConstructor(TNode<Context>{p_context}, TNode<Object>{p_exemplar}, TNode<JSReceiver>{tmp0});
    compiler::CodeAssemblerLabel label9(&ca_);
    tmp8 = Cast_Constructor_0(state_, TNode<HeapObject>{tmp7}, &label9);
    ca_.Goto(&block14);
    if (label9.is_used()) {
      ca_.Bind(&label9);
      ca_.Goto(&block15);
    }
  }

  if (block15.is_used()) {
    ca_.Bind(&block15);
    CodeStubAssembler(state_).Unreachable();
  }

  TNode<Oddball> tmp10;
  if (block14.is_used()) {
    ca_.Bind(&block14);
    tmp10 = Undefined_0(state_);
    if (((CodeStubAssembler(state_).ConstexprInt31Equal(p_numArgs, 1)))) {
      ca_.Goto(&block16);
    } else {
      ca_.Goto(&block17);
    }
  }

  TNode<JSReceiver> tmp11;
  if (block16.is_used()) {
    ca_.Bind(&block16);
    tmp11 = CodeStubAssembler(state_).Construct(TNode<Context>{p_context}, TNode<JSReceiver>{tmp8}, TNode<Object>{p_arg0});
    ca_.Goto(&block18, tmp11);
  }

  TNode<JSReceiver> tmp12;
  if (block17.is_used()) {
    ca_.Bind(&block17);
    tmp12 = CodeStubAssembler(state_).Construct(TNode<Context>{p_context}, TNode<JSReceiver>{tmp8}, TNode<Object>{p_arg0}, TNode<Object>{p_arg1}, TNode<Object>{p_arg2});
    ca_.Goto(&block18, tmp12);
  }

  TNode<Object> phi_bb18_7;
  TNode<JSTypedArray> tmp13;
  if (block18.is_used()) {
    ca_.Bind(&block18, &phi_bb18_7);
    tmp13 = TypedArrayBuiltinsAssembler(state_).ValidateTypedArray(TNode<Context>{p_context}, TNode<Object>{phi_bb18_7}, p_methodName);
    ca_.Goto(&block1, tmp13);
  }

  TNode<JSTypedArray> phi_bb1_5;
  if (block1.is_used()) {
    ca_.Bind(&block1, &phi_bb1_5);
    ca_.Goto(&block23, phi_bb1_5);
  }

  TNode<JSTypedArray> phi_bb23_5;
    ca_.Bind(&block23, &phi_bb23_5);
  return TNode<JSTypedArray>{phi_bb23_5};
}


TNode<JSTypedArray> TypedArraySpeciesCreateByLength_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, const char* p_methodName, TNode<JSTypedArray> p_exemplar, TNode<UintPtrT> p_length) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block3(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block4(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Number> tmp0;
  TNode<Oddball> tmp1;
  TNode<Oddball> tmp2;
  TNode<JSTypedArray> tmp3;
  TNode<IntPtrT> tmp4;
  TNode<UintPtrT> tmp5;
  TNode<BoolT> tmp6;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{p_length});
    tmp1 = Undefined_0(state_);
    tmp2 = Undefined_0(state_);
    tmp3 = TypedArraySpeciesCreate_0(state_, TNode<Context>{p_context}, p_methodName, 1, TNode<JSTypedArray>{p_exemplar}, TNode<Object>{tmp0}, TNode<Object>{tmp1}, TNode<Object>{tmp2});
    tmp4 = FromConstexpr_intptr_constexpr_int31_0(state_, 32);
    tmp5 = CodeStubAssembler(state_).LoadReference<UintPtrT>(CodeStubAssembler::Reference{tmp3, tmp4});
    tmp6 = CodeStubAssembler(state_).UintPtrLessThan(TNode<UintPtrT>{tmp5}, TNode<UintPtrT>{p_length});
    ca_.Branch(tmp6, &block2, std::vector<compiler::Node*>{}, &block3, std::vector<compiler::Node*>{});
  }

  if (block2.is_used()) {
    ca_.Bind(&block2);
    CodeStubAssembler(state_).ThrowTypeError(TNode<Context>{p_context}, MessageTemplate::kTypedArrayTooShort);
  }

  if (block3.is_used()) {
    ca_.Bind(&block3);
    ca_.Goto(&block4);
  }

    ca_.Bind(&block4);
  return TNode<JSTypedArray>{tmp3};
}


TNode<JSTypedArray> TypedArraySpeciesCreateByBuffer_0(compiler::CodeAssemblerState* state_, TNode<Context> p_context, const char* p_methodName, TNode<JSTypedArray> p_exemplar, TNode<JSArrayBuffer> p_buffer, TNode<UintPtrT> p_beginByteOffset, TNode<UintPtrT> p_newLength) {
  compiler::CodeAssembler ca_(state_);
  compiler::CodeAssembler::SourcePositionScope pos_scope(&ca_);
  compiler::CodeAssemblerParameterizedLabel<> block0(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
  compiler::CodeAssemblerParameterizedLabel<> block2(&ca_, compiler::CodeAssemblerLabel::kNonDeferred);
    ca_.Goto(&block0);

  TNode<Number> tmp0;
  TNode<Number> tmp1;
  TNode<JSTypedArray> tmp2;
  if (block0.is_used()) {
    ca_.Bind(&block0);
    tmp0 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{p_beginByteOffset});
    tmp1 = Convert_Number_uintptr_0(state_, TNode<UintPtrT>{p_newLength});
    tmp2 = TypedArraySpeciesCreate_0(state_, TNode<Context>{p_context}, p_methodName, 3, TNode<JSTypedArray>{p_exemplar}, TNode<Object>{p_buffer}, TNode<Object>{tmp0}, TNode<Object>{tmp1});
    ca_.Goto(&block2);
  }

    ca_.Bind(&block2);
  return TNode<JSTypedArray>{tmp2};
}


} // namespace internal
} // namespace v8
