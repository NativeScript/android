#ifdef VERIFY_HEAP
#include "torque-generated/class-verifiers.h"

#include "src/objects/all-objects-inl.h"

// Has to be the last include (doesn't have include guards):
#include "src/objects/object-macros.h"
namespace v8 {
namespace internal {
#include "torque-generated/test/torque/test-torque-tq-inl.inc"
void TorqueGeneratedClassVerifiers::HeapObjectVerify(HeapObject o, Isolate* isolate) {
  CHECK(o.IsHeapObject());
  {
    Object map__value = o.map();
    Object::VerifyPointer(isolate, map__value);
    CHECK(map__value.IsMap());
  }
}
void TorqueGeneratedClassVerifiers::ContextVerify(Context o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsContext());
  {
    Object length__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, length__value);
    CHECK(length__value.IsSmi());
  }
  intptr_t elements__offset, elements__length;
  std::tie(std::ignore, elements__offset, elements__length) = TqRuntimeFieldSliceContextElements(o);
  CHECK_EQ(elements__offset, static_cast<int>(elements__offset));
  CHECK_EQ(elements__length, static_cast<int>(elements__length));
  for (int i = 0; i < static_cast<int>(elements__length); ++i) {
    Object elements__value = TaggedField<Object>::load(o, static_cast<int>(elements__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, elements__value);
  }
}
void TorqueGeneratedClassVerifiers::JSReceiverVerify(JSReceiver o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsJSReceiver());
  {
    Object properties_or_hash__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, properties_or_hash__value);
    CHECK(properties_or_hash__value.IsSmi() || properties_or_hash__value.IsFixedArrayBase() || properties_or_hash__value.IsPropertyArray() || properties_or_hash__value.IsSwissNameDictionary());
  }
}
void TorqueGeneratedClassVerifiers::PrimitiveHeapObjectVerify(PrimitiveHeapObject o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsPrimitiveHeapObject());
}
void TorqueGeneratedClassVerifiers::HeapNumberVerify(HeapNumber o, Isolate* isolate) {
  o.PrimitiveHeapObjectVerify(isolate);
  CHECK(o.IsHeapNumber());
}
void TorqueGeneratedClassVerifiers::BigIntBaseVerify(BigIntBase o, Isolate* isolate) {
  o.PrimitiveHeapObjectVerify(isolate);
  CHECK(o.IsBigIntBase());
}
void TorqueGeneratedClassVerifiers::NameVerify(Name o, Isolate* isolate) {
  o.PrimitiveHeapObjectVerify(isolate);
  CHECK(o.IsName());
}
void TorqueGeneratedClassVerifiers::StringVerify(String o, Isolate* isolate) {
  o.NameVerify(isolate);
  CHECK(o.IsString());
}
void TorqueGeneratedClassVerifiers::SymbolVerify(Symbol o, Isolate* isolate) {
  o.NameVerify(isolate);
  CHECK(o.IsSymbol());
  {
    Object description__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, description__value);
    CHECK(description__value.IsString() || description__value.IsUndefined());
  }
}
void TorqueGeneratedClassVerifiers::OddballVerify(Oddball o, Isolate* isolate) {
  o.PrimitiveHeapObjectVerify(isolate);
  CHECK(o.IsOddball());
  {
    Object to_string__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, to_string__value);
    CHECK(to_string__value.IsString());
  }
  {
    Object to_number__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, to_number__value);
    CHECK(to_number__value.IsSmi() || to_number__value.IsHeapNumber());
  }
  {
    Object type_of__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, type_of__value);
    CHECK(type_of__value.IsString());
  }
  {
    Object kind__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, kind__value);
    CHECK(kind__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::FixedArrayBaseVerify(FixedArrayBase o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsFixedArrayBase());
  {
    Object length__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, length__value);
    CHECK(length__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::FixedArrayVerify(FixedArray o, Isolate* isolate) {
  o.FixedArrayBaseVerify(isolate);
  CHECK(o.IsFixedArray());
  intptr_t objects__offset, objects__length;
  std::tie(std::ignore, objects__offset, objects__length) = TqRuntimeFieldSliceFixedArrayObjects(o);
  CHECK_EQ(objects__offset, static_cast<int>(objects__offset));
  CHECK_EQ(objects__length, static_cast<int>(objects__length));
  for (int i = 0; i < static_cast<int>(objects__length); ++i) {
    Object objects__value = TaggedField<Object>::load(o, static_cast<int>(objects__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, objects__value);
  }
}
void TorqueGeneratedClassVerifiers::JSObjectVerify(JSObject o, Isolate* isolate) {
  o.JSReceiverVerify(isolate);
  CHECK(o.IsJSObject());
  {
    Object elements__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, elements__value);
    CHECK(elements__value.IsFixedArrayBase());
  }
}
void TorqueGeneratedClassVerifiers::ByteArrayVerify(ByteArray o, Isolate* isolate) {
  o.FixedArrayBaseVerify(isolate);
  CHECK(o.IsByteArray());
}
void TorqueGeneratedClassVerifiers::WeakFixedArrayVerify(WeakFixedArray o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsWeakFixedArray());
  {
    Object length__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, length__value);
    CHECK(length__value.IsSmi());
  }
  intptr_t objects__offset, objects__length;
  std::tie(std::ignore, objects__offset, objects__length) = TqRuntimeFieldSliceWeakFixedArrayObjects(o);
  CHECK_EQ(objects__offset, static_cast<int>(objects__offset));
  CHECK_EQ(objects__length, static_cast<int>(objects__length));
  for (int i = 0; i < static_cast<int>(objects__length); ++i) {
    MaybeObject objects__value = TaggedField<MaybeObject>::load(o, static_cast<int>(objects__offset) + i * kTaggedSize);
    MaybeObject::VerifyMaybeObjectPointer(isolate, objects__value);
    CHECK(objects__value.IsCleared() || (!objects__value.IsWeak() && objects__value.GetHeapObjectOrSmi().IsHeapObject()) || (!objects__value.IsWeak() && objects__value.GetHeapObjectOrSmi().IsSmi()) || objects__value.IsWeak());
  }
}
void TorqueGeneratedClassVerifiers::ForeignVerify(Foreign o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsForeign());
}
void TorqueGeneratedClassVerifiers::JSProxyVerify(JSProxy o, Isolate* isolate) {
  o.JSReceiverVerify(isolate);
  CHECK(o.IsJSProxy());
  {
    Object target__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, target__value);
    CHECK(target__value.IsJSReceiver() || target__value.IsNull());
  }
  {
    Object handler__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, handler__value);
    CHECK(handler__value.IsJSReceiver() || handler__value.IsNull());
  }
}
void TorqueGeneratedClassVerifiers::JSFunctionOrBoundFunctionVerify(JSFunctionOrBoundFunction o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSFunctionOrBoundFunction());
}
void TorqueGeneratedClassVerifiers::JSFunctionVerify(JSFunction o, Isolate* isolate) {
  o.JSFunctionOrBoundFunctionVerify(isolate);
  CHECK(o.IsJSFunction());
  {
    Object shared_function_info__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, shared_function_info__value);
    CHECK(shared_function_info__value.IsSharedFunctionInfo());
  }
  {
    Object context__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, context__value);
    CHECK(context__value.IsContext());
  }
  {
    Object feedback_cell__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, feedback_cell__value);
    CHECK(feedback_cell__value.IsFeedbackCell());
  }
  {
    Object code__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, code__value);
    CHECK(code__value.IsCode());
  }
}
void TorqueGeneratedClassVerifiers::JSBoundFunctionVerify(JSBoundFunction o, Isolate* isolate) {
  o.JSFunctionOrBoundFunctionVerify(isolate);
  CHECK(o.IsJSBoundFunction());
  {
    Object bound_target_function__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, bound_target_function__value);
    CHECK(bound_target_function__value.IsCallableApiObject() || bound_target_function__value.IsCallableJSProxy() || bound_target_function__value.IsJSFunction() || bound_target_function__value.IsJSBoundFunction());
  }
  {
    Object bound_this__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, bound_this__value);
    CHECK(bound_this__value.IsJSReceiver() || bound_this__value.IsSmi() || bound_this__value.IsHeapNumber() || bound_this__value.IsBigInt() || bound_this__value.IsString() || bound_this__value.IsSymbol() || bound_this__value.IsTrue() || bound_this__value.IsFalse() || bound_this__value.IsNull() || bound_this__value.IsUndefined() || bound_this__value.IsSourceTextModule());
  }
  {
    Object bound_arguments__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, bound_arguments__value);
    CHECK(bound_arguments__value.IsFixedArray());
  }
}
void TorqueGeneratedClassVerifiers::JSCustomElementsObjectVerify(JSCustomElementsObject o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSCustomElementsObject());
}
void TorqueGeneratedClassVerifiers::JSSpecialObjectVerify(JSSpecialObject o, Isolate* isolate) {
  o.JSCustomElementsObjectVerify(isolate);
  CHECK(o.IsJSSpecialObject());
}
void TorqueGeneratedClassVerifiers::MapVerify(Map o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsMap());
  {
    Object prototype__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, prototype__value);
    CHECK(prototype__value.IsJSReceiver() || prototype__value.IsNull());
  }
  {
    Object constructor_or_back_pointer_or_native_context__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, constructor_or_back_pointer_or_native_context__value);
  }
  {
    Object instance_descriptors__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, instance_descriptors__value);
    CHECK(instance_descriptors__value.IsDescriptorArray());
  }
  {
    Object dependent_code__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, dependent_code__value);
    CHECK(dependent_code__value.IsDependentCode());
  }
  {
    Object prototype_validity_cell__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, prototype_validity_cell__value);
    CHECK(prototype_validity_cell__value.IsSmi() || prototype_validity_cell__value.IsCell());
  }
  {
    MaybeObject transitions_or_prototype_info__value = TaggedField<MaybeObject>::load(o, 36);
    MaybeObject::VerifyMaybeObjectPointer(isolate, transitions_or_prototype_info__value);
    CHECK(transitions_or_prototype_info__value.IsCleared() || (!transitions_or_prototype_info__value.IsWeak() && transitions_or_prototype_info__value.GetHeapObjectOrSmi().IsSmi()) || (!transitions_or_prototype_info__value.IsWeak() && transitions_or_prototype_info__value.GetHeapObjectOrSmi().IsTransitionArray()) || (!transitions_or_prototype_info__value.IsWeak() && transitions_or_prototype_info__value.GetHeapObjectOrSmi().IsMap()) || (transitions_or_prototype_info__value.IsWeak() && transitions_or_prototype_info__value.GetHeapObjectOrSmi().IsMap()) || (!transitions_or_prototype_info__value.IsWeak() && transitions_or_prototype_info__value.GetHeapObjectOrSmi().IsPrototypeInfo()));
  }
}
void TorqueGeneratedClassVerifiers::WeakCellVerify(WeakCell o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsWeakCell());
  {
    Object finalization_registry__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, finalization_registry__value);
    CHECK(finalization_registry__value.IsUndefined() || finalization_registry__value.IsJSFinalizationRegistry());
  }
  {
    Object target__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, target__value);
    CHECK(target__value.IsJSReceiver() || target__value.IsUndefined());
  }
  {
    Object unregister_token__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, unregister_token__value);
    CHECK(unregister_token__value.IsJSReceiver() || unregister_token__value.IsUndefined());
  }
  {
    Object holdings__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, holdings__value);
    CHECK(holdings__value.IsJSReceiver() || holdings__value.IsSmi() || holdings__value.IsHeapNumber() || holdings__value.IsBigInt() || holdings__value.IsString() || holdings__value.IsSymbol() || holdings__value.IsTrue() || holdings__value.IsFalse() || holdings__value.IsNull() || holdings__value.IsUndefined());
  }
  {
    Object prev__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, prev__value);
    CHECK(prev__value.IsUndefined() || prev__value.IsWeakCell());
  }
  {
    Object next__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, next__value);
    CHECK(next__value.IsUndefined() || next__value.IsWeakCell());
  }
  {
    Object key_list_prev__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, key_list_prev__value);
    CHECK(key_list_prev__value.IsUndefined() || key_list_prev__value.IsWeakCell());
  }
  {
    Object key_list_next__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, key_list_next__value);
    CHECK(key_list_next__value.IsUndefined() || key_list_next__value.IsWeakCell());
  }
}
void TorqueGeneratedClassVerifiers::JSPromiseVerify(JSPromise o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSPromise());
  {
    Object reactions_or_result__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, reactions_or_result__value);
    CHECK(reactions_or_result__value.IsJSReceiver() || reactions_or_result__value.IsSmi() || reactions_or_result__value.IsHeapNumber() || reactions_or_result__value.IsBigInt() || reactions_or_result__value.IsString() || reactions_or_result__value.IsSymbol() || reactions_or_result__value.IsTrue() || reactions_or_result__value.IsFalse() || reactions_or_result__value.IsNull() || reactions_or_result__value.IsUndefined() || reactions_or_result__value.IsPromiseReaction());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::StructVerify(Struct o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsStruct());
}
void TorqueGeneratedClassVerifiers::PromiseCapabilityVerify(PromiseCapability o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsPromiseCapability());
  {
    Object promise__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, promise__value);
    CHECK(promise__value.IsJSReceiver() || promise__value.IsUndefined());
  }
  {
    Object resolve__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, resolve__value);
  }
  {
    Object reject__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, reject__value);
  }
}
void TorqueGeneratedClassVerifiers::JSArrayBufferViewVerify(JSArrayBufferView o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSArrayBufferView());
  {
    Object buffer__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, buffer__value);
    CHECK(buffer__value.IsJSArrayBuffer());
  }
}
void TorqueGeneratedClassVerifiers::JSTypedArrayVerify(JSTypedArray o, Isolate* isolate) {
  o.JSArrayBufferViewVerify(isolate);
  CHECK(o.IsJSTypedArray());
  {
    Object base_pointer__value = TaggedField<Object>::load(o, 48);
    Object::VerifyPointer(isolate, base_pointer__value);
    CHECK(base_pointer__value.IsSmi() || base_pointer__value.IsByteArray());
  }
}
void TorqueGeneratedClassVerifiers::DataHandlerVerify(DataHandler o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsDataHandler());
  {
    Object smi_handler__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, smi_handler__value);
    CHECK(smi_handler__value.IsSmi() || smi_handler__value.IsCode());
  }
  {
    Object validity_cell__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, validity_cell__value);
    CHECK(validity_cell__value.IsSmi() || validity_cell__value.IsCell());
  }
}
void TorqueGeneratedClassVerifiers::AllocationMementoVerify(AllocationMemento o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsAllocationMemento());
  {
    Object allocation_site__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, allocation_site__value);
    CHECK(allocation_site__value.IsAllocationSite());
  }
}
void TorqueGeneratedClassVerifiers::CallHandlerInfoVerify(CallHandlerInfo o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsCallHandlerInfo());
  {
    Object callback__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, callback__value);
    CHECK(callback__value.IsUndefined() || callback__value.IsZero() || callback__value.IsNonNullForeign());
  }
  {
    Object js_callback__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, js_callback__value);
    CHECK(js_callback__value.IsUndefined() || js_callback__value.IsZero() || js_callback__value.IsNonNullForeign());
  }
  {
    Object data__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, data__value);
  }
}
void TorqueGeneratedClassVerifiers::InterceptorInfoVerify(InterceptorInfo o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsInterceptorInfo());
  {
    Object getter__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, getter__value);
    CHECK(getter__value.IsUndefined() || getter__value.IsZero() || getter__value.IsNonNullForeign());
  }
  {
    Object setter__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, setter__value);
    CHECK(setter__value.IsUndefined() || setter__value.IsZero() || setter__value.IsNonNullForeign());
  }
  {
    Object query__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, query__value);
    CHECK(query__value.IsUndefined() || query__value.IsZero() || query__value.IsNonNullForeign());
  }
  {
    Object descriptor__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, descriptor__value);
    CHECK(descriptor__value.IsUndefined() || descriptor__value.IsZero() || descriptor__value.IsNonNullForeign());
  }
  {
    Object deleter__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, deleter__value);
    CHECK(deleter__value.IsUndefined() || deleter__value.IsZero() || deleter__value.IsNonNullForeign());
  }
  {
    Object enumerator__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, enumerator__value);
    CHECK(enumerator__value.IsUndefined() || enumerator__value.IsZero() || enumerator__value.IsNonNullForeign());
  }
  {
    Object definer__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, definer__value);
    CHECK(definer__value.IsUndefined() || definer__value.IsZero() || definer__value.IsNonNullForeign());
  }
  {
    Object data__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, data__value);
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::AccessCheckInfoVerify(AccessCheckInfo o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsAccessCheckInfo());
  {
    Object callback__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, callback__value);
    CHECK(callback__value.IsUndefined() || callback__value.IsZero() || callback__value.IsForeign());
  }
  {
    Object named_interceptor__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, named_interceptor__value);
    CHECK(named_interceptor__value.IsUndefined() || named_interceptor__value.IsZero() || named_interceptor__value.IsInterceptorInfo());
  }
  {
    Object indexed_interceptor__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, indexed_interceptor__value);
    CHECK(indexed_interceptor__value.IsUndefined() || indexed_interceptor__value.IsZero() || indexed_interceptor__value.IsInterceptorInfo());
  }
  {
    Object data__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, data__value);
  }
}
void TorqueGeneratedClassVerifiers::AccessorInfoVerify(AccessorInfo o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsAccessorInfo());
  {
    Object name__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, name__value);
    CHECK(name__value.IsName());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
  {
    Object expected_receiver_type__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, expected_receiver_type__value);
  }
  {
    Object setter__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, setter__value);
    CHECK(setter__value.IsZero() || setter__value.IsNonNullForeign());
  }
  {
    Object getter__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, getter__value);
    CHECK(getter__value.IsZero() || getter__value.IsNonNullForeign());
  }
  {
    Object js_getter__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, js_getter__value);
    CHECK(js_getter__value.IsZero() || js_getter__value.IsNonNullForeign());
  }
  {
    Object data__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, data__value);
  }
}
void TorqueGeneratedClassVerifiers::JSArgumentsObjectVerify(JSArgumentsObject o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSArgumentsObject());
}
void TorqueGeneratedClassVerifiers::SloppyArgumentsElementsVerify(SloppyArgumentsElements o, Isolate* isolate) {
  o.FixedArrayBaseVerify(isolate);
  CHECK(o.IsSloppyArgumentsElements());
  {
    Object context__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, context__value);
    CHECK(context__value.IsContext());
  }
  {
    Object arguments__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, arguments__value);
    CHECK(arguments__value.IsFixedArray());
  }
  intptr_t mapped_entries__offset, mapped_entries__length;
  std::tie(std::ignore, mapped_entries__offset, mapped_entries__length) = TqRuntimeFieldSliceSloppyArgumentsElementsMappedEntries(o);
  CHECK_EQ(mapped_entries__offset, static_cast<int>(mapped_entries__offset));
  CHECK_EQ(mapped_entries__length, static_cast<int>(mapped_entries__length));
  for (int i = 0; i < static_cast<int>(mapped_entries__length); ++i) {
    Object mapped_entries__value = TaggedField<Object>::load(o, static_cast<int>(mapped_entries__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, mapped_entries__value);
    CHECK(mapped_entries__value.IsSmi() || mapped_entries__value.IsTheHole());
  }
}
void TorqueGeneratedClassVerifiers::AliasedArgumentsEntryVerify(AliasedArgumentsEntry o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsAliasedArgumentsEntry());
  {
    Object aliased_context_slot__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, aliased_context_slot__value);
    CHECK(aliased_context_slot__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::CellVerify(Cell o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsCell());
  {
    Object value__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, value__value);
  }
}
void TorqueGeneratedClassVerifiers::BytecodeArrayVerify(BytecodeArray o, Isolate* isolate) {
  o.FixedArrayBaseVerify(isolate);
  CHECK(o.IsBytecodeArray());
  {
    Object constant_pool__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, constant_pool__value);
    CHECK(constant_pool__value.IsFixedArray());
  }
  {
    Object handler_table__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, handler_table__value);
    CHECK(handler_table__value.IsByteArray());
  }
  {
    Object source_position_table__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, source_position_table__value);
    CHECK(source_position_table__value.IsUndefined() || source_position_table__value.IsByteArray() || source_position_table__value.IsException());
  }
}
void TorqueGeneratedClassVerifiers::ScopeInfoVerify(ScopeInfo o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsScopeInfo());
  {
    Object flags__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
  {
    Object parameter_count__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, parameter_count__value);
    CHECK(parameter_count__value.IsSmi());
  }
  {
    Object context_local_count__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, context_local_count__value);
    CHECK(context_local_count__value.IsSmi());
  }
  intptr_t context_local_names__offset, context_local_names__length;
  std::tie(std::ignore, context_local_names__offset, context_local_names__length) = TqRuntimeFieldSliceScopeInfoContextLocalNames(o);
  CHECK_EQ(context_local_names__offset, static_cast<int>(context_local_names__offset));
  CHECK_EQ(context_local_names__length, static_cast<int>(context_local_names__length));
  for (int i = 0; i < static_cast<int>(context_local_names__length); ++i) {
    Object context_local_names__value = TaggedField<Object>::load(o, static_cast<int>(context_local_names__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, context_local_names__value);
    CHECK(context_local_names__value.IsString());
  }
  intptr_t context_local_infos__offset, context_local_infos__length;
  std::tie(std::ignore, context_local_infos__offset, context_local_infos__length) = TqRuntimeFieldSliceScopeInfoContextLocalInfos(o);
  CHECK_EQ(context_local_infos__offset, static_cast<int>(context_local_infos__offset));
  CHECK_EQ(context_local_infos__length, static_cast<int>(context_local_infos__length));
  for (int i = 0; i < static_cast<int>(context_local_infos__length); ++i) {
    Object context_local_infos__value = TaggedField<Object>::load(o, static_cast<int>(context_local_infos__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, context_local_infos__value);
    CHECK(context_local_infos__value.IsSmi());
  }
  intptr_t saved_class_variable_info__offset, saved_class_variable_info__length;
  std::tie(std::ignore, saved_class_variable_info__offset, saved_class_variable_info__length) = TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo(o);
  CHECK_EQ(saved_class_variable_info__offset, static_cast<int>(saved_class_variable_info__offset));
  CHECK_EQ(saved_class_variable_info__length, static_cast<int>(saved_class_variable_info__length));
  for (int i = 0; i < static_cast<int>(saved_class_variable_info__length); ++i) {
    Object saved_class_variable_info__value = TaggedField<Object>::load(o, static_cast<int>(saved_class_variable_info__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, saved_class_variable_info__value);
    CHECK(saved_class_variable_info__value.IsSmi());
  }
  intptr_t receiver_info__offset, receiver_info__length;
  std::tie(std::ignore, receiver_info__offset, receiver_info__length) = TqRuntimeFieldSliceScopeInfoReceiverInfo(o);
  CHECK_EQ(receiver_info__offset, static_cast<int>(receiver_info__offset));
  CHECK_EQ(receiver_info__length, static_cast<int>(receiver_info__length));
  for (int i = 0; i < static_cast<int>(receiver_info__length); ++i) {
    Object receiver_info__value = TaggedField<Object>::load(o, static_cast<int>(receiver_info__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, receiver_info__value);
    CHECK(receiver_info__value.IsSmi());
  }
  intptr_t function_variable_info__offset, function_variable_info__length;
  std::tie(std::ignore, function_variable_info__offset, function_variable_info__length) = TqRuntimeFieldSliceScopeInfoFunctionVariableInfo(o);
  CHECK_EQ(function_variable_info__offset, static_cast<int>(function_variable_info__offset));
  CHECK_EQ(function_variable_info__length, static_cast<int>(function_variable_info__length));
  for (int i = 0; i < static_cast<int>(function_variable_info__length); ++i) {
    Object name__value = TaggedField<Object>::load(o, static_cast<int>(function_variable_info__offset) + 0 + i * 8);
    Object::VerifyPointer(isolate, name__value);
    CHECK(name__value.IsString() || name__value.IsZero());
    Object context_or_stack_slot_index__value = TaggedField<Object>::load(o, static_cast<int>(function_variable_info__offset) + 4 + i * 8);
    Object::VerifyPointer(isolate, context_or_stack_slot_index__value);
    CHECK(context_or_stack_slot_index__value.IsSmi());
  }
  intptr_t inferred_function_name__offset, inferred_function_name__length;
  std::tie(std::ignore, inferred_function_name__offset, inferred_function_name__length) = TqRuntimeFieldSliceScopeInfoInferredFunctionName(o);
  CHECK_EQ(inferred_function_name__offset, static_cast<int>(inferred_function_name__offset));
  CHECK_EQ(inferred_function_name__length, static_cast<int>(inferred_function_name__length));
  for (int i = 0; i < static_cast<int>(inferred_function_name__length); ++i) {
    Object inferred_function_name__value = TaggedField<Object>::load(o, static_cast<int>(inferred_function_name__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, inferred_function_name__value);
    CHECK(inferred_function_name__value.IsString() || inferred_function_name__value.IsUndefined());
  }
  intptr_t position_info__offset, position_info__length;
  std::tie(std::ignore, position_info__offset, position_info__length) = TqRuntimeFieldSliceScopeInfoPositionInfo(o);
  CHECK_EQ(position_info__offset, static_cast<int>(position_info__offset));
  CHECK_EQ(position_info__length, static_cast<int>(position_info__length));
  for (int i = 0; i < static_cast<int>(position_info__length); ++i) {
    Object start__value = TaggedField<Object>::load(o, static_cast<int>(position_info__offset) + 0 + i * 8);
    Object::VerifyPointer(isolate, start__value);
    CHECK(start__value.IsSmi());
    Object end__value = TaggedField<Object>::load(o, static_cast<int>(position_info__offset) + 4 + i * 8);
    Object::VerifyPointer(isolate, end__value);
    CHECK(end__value.IsSmi());
  }
  intptr_t outer_scope_info__offset, outer_scope_info__length;
  std::tie(std::ignore, outer_scope_info__offset, outer_scope_info__length) = TqRuntimeFieldSliceScopeInfoOuterScopeInfo(o);
  CHECK_EQ(outer_scope_info__offset, static_cast<int>(outer_scope_info__offset));
  CHECK_EQ(outer_scope_info__length, static_cast<int>(outer_scope_info__length));
  for (int i = 0; i < static_cast<int>(outer_scope_info__length); ++i) {
    Object outer_scope_info__value = TaggedField<Object>::load(o, static_cast<int>(outer_scope_info__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, outer_scope_info__value);
    CHECK(outer_scope_info__value.IsTheHole() || outer_scope_info__value.IsScopeInfo());
  }
  intptr_t locals_block_list__offset, locals_block_list__length;
  std::tie(std::ignore, locals_block_list__offset, locals_block_list__length) = TqRuntimeFieldSliceScopeInfoLocalsBlockList(o);
  CHECK_EQ(locals_block_list__offset, static_cast<int>(locals_block_list__offset));
  CHECK_EQ(locals_block_list__length, static_cast<int>(locals_block_list__length));
  for (int i = 0; i < static_cast<int>(locals_block_list__length); ++i) {
    Object locals_block_list__value = TaggedField<Object>::load(o, static_cast<int>(locals_block_list__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, locals_block_list__value);
    CHECK(locals_block_list__value.IsHashTable());
  }
  intptr_t module_info__offset, module_info__length;
  std::tie(std::ignore, module_info__offset, module_info__length) = TqRuntimeFieldSliceScopeInfoModuleInfo(o);
  CHECK_EQ(module_info__offset, static_cast<int>(module_info__offset));
  CHECK_EQ(module_info__length, static_cast<int>(module_info__length));
  for (int i = 0; i < static_cast<int>(module_info__length); ++i) {
    Object module_info__value = TaggedField<Object>::load(o, static_cast<int>(module_info__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, module_info__value);
    CHECK(module_info__value.IsSourceTextModuleInfo());
  }
  intptr_t module_variable_count__offset, module_variable_count__length;
  std::tie(std::ignore, module_variable_count__offset, module_variable_count__length) = TqRuntimeFieldSliceScopeInfoModuleVariableCount(o);
  CHECK_EQ(module_variable_count__offset, static_cast<int>(module_variable_count__offset));
  CHECK_EQ(module_variable_count__length, static_cast<int>(module_variable_count__length));
  for (int i = 0; i < static_cast<int>(module_variable_count__length); ++i) {
    Object module_variable_count__value = TaggedField<Object>::load(o, static_cast<int>(module_variable_count__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, module_variable_count__value);
    CHECK(module_variable_count__value.IsSmi());
  }
  intptr_t module_variables__offset, module_variables__length;
  std::tie(std::ignore, module_variables__offset, module_variables__length) = TqRuntimeFieldSliceScopeInfoModuleVariables(o);
  CHECK_EQ(module_variables__offset, static_cast<int>(module_variables__offset));
  CHECK_EQ(module_variables__length, static_cast<int>(module_variables__length));
  for (int i = 0; i < static_cast<int>(module_variables__length); ++i) {
    Object name__value = TaggedField<Object>::load(o, static_cast<int>(module_variables__offset) + 0 + i * 12);
    Object::VerifyPointer(isolate, name__value);
    CHECK(name__value.IsString());
    Object index__value = TaggedField<Object>::load(o, static_cast<int>(module_variables__offset) + 4 + i * 12);
    Object::VerifyPointer(isolate, index__value);
    CHECK(index__value.IsSmi());
    Object properties__value = TaggedField<Object>::load(o, static_cast<int>(module_variables__offset) + 8 + i * 12);
    Object::VerifyPointer(isolate, properties__value);
    CHECK(properties__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::FixedDoubleArrayVerify(FixedDoubleArray o, Isolate* isolate) {
  o.FixedArrayBaseVerify(isolate);
  CHECK(o.IsFixedDoubleArray());
}
void TorqueGeneratedClassVerifiers::RegExpMatchInfoVerify(RegExpMatchInfo o, Isolate* isolate) {
  o.FixedArrayVerify(isolate);
  CHECK(o.IsRegExpMatchInfo());
}
void TorqueGeneratedClassVerifiers::BreakPointVerify(BreakPoint o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsBreakPoint());
  {
    Object id__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, id__value);
    CHECK(id__value.IsSmi());
  }
  {
    Object condition__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, condition__value);
    CHECK(condition__value.IsString());
  }
}
void TorqueGeneratedClassVerifiers::BreakPointInfoVerify(BreakPointInfo o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsBreakPointInfo());
  {
    Object source_position__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, source_position__value);
    CHECK(source_position__value.IsSmi());
  }
  {
    Object break_points__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, break_points__value);
    CHECK(break_points__value.IsUndefined() || break_points__value.IsFixedArray() || break_points__value.IsBreakPoint());
  }
}
void TorqueGeneratedClassVerifiers::DebugInfoVerify(DebugInfo o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsDebugInfo());
  {
    Object shared__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, shared__value);
    CHECK(shared__value.IsSharedFunctionInfo());
  }
  {
    Object debugger_hints__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, debugger_hints__value);
    CHECK(debugger_hints__value.IsSmi());
  }
  {
    Object script__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, script__value);
    CHECK(script__value.IsUndefined() || script__value.IsScript());
  }
  {
    Object original_bytecode_array__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, original_bytecode_array__value);
    CHECK(original_bytecode_array__value.IsUndefined() || original_bytecode_array__value.IsBytecodeArray());
  }
  {
    Object debug_bytecode_array__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, debug_bytecode_array__value);
    CHECK(debug_bytecode_array__value.IsUndefined() || debug_bytecode_array__value.IsBytecodeArray());
  }
  {
    Object break_points__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, break_points__value);
    CHECK(break_points__value.IsFixedArray());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
  {
    Object coverage_info__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, coverage_info__value);
    CHECK(coverage_info__value.IsUndefined() || coverage_info__value.IsCoverageInfo());
  }
}
void TorqueGeneratedClassVerifiers::CoverageInfoVerify(CoverageInfo o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsCoverageInfo());
  intptr_t slots__offset, slots__length;
  std::tie(std::ignore, slots__offset, slots__length) = TqRuntimeFieldSliceCoverageInfoSlots(o);
  CHECK_EQ(slots__offset, static_cast<int>(slots__offset));
  CHECK_EQ(slots__length, static_cast<int>(slots__length));
  for (int i = 0; i < static_cast<int>(slots__length); ++i) {
  }
}
void TorqueGeneratedClassVerifiers::EnumCacheVerify(EnumCache o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsEnumCache());
  {
    Object keys__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, keys__value);
    CHECK(keys__value.IsFixedArray());
  }
  {
    Object indices__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, indices__value);
    CHECK(indices__value.IsFixedArray());
  }
}
void TorqueGeneratedClassVerifiers::AccessorPairVerify(AccessorPair o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsAccessorPair());
  {
    Object getter__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, getter__value);
  }
  {
    Object setter__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, setter__value);
  }
}
void TorqueGeneratedClassVerifiers::ClassPositionsVerify(ClassPositions o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsClassPositions());
  {
    Object start__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, start__value);
    CHECK(start__value.IsSmi());
  }
  {
    Object end__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, end__value);
    CHECK(end__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::DescriptorArrayVerify(DescriptorArray o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsDescriptorArray());
  {
    Object enum_cache__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, enum_cache__value);
    CHECK(enum_cache__value.IsEnumCache());
  }
  intptr_t descriptors__offset, descriptors__length;
  std::tie(std::ignore, descriptors__offset, descriptors__length) = TqRuntimeFieldSliceDescriptorArrayDescriptors(o);
  CHECK_EQ(descriptors__offset, static_cast<int>(descriptors__offset));
  CHECK_EQ(descriptors__length, static_cast<int>(descriptors__length));
  for (int i = 0; i < static_cast<int>(descriptors__length); ++i) {
    Object key__value = TaggedField<Object>::load(o, static_cast<int>(descriptors__offset) + 0 + i * 12);
    Object::VerifyPointer(isolate, key__value);
    CHECK(key__value.IsName() || key__value.IsUndefined());
    Object details__value = TaggedField<Object>::load(o, static_cast<int>(descriptors__offset) + 4 + i * 12);
    Object::VerifyPointer(isolate, details__value);
    CHECK(details__value.IsSmi() || details__value.IsUndefined());
    MaybeObject value__value = TaggedField<MaybeObject>::load(o, static_cast<int>(descriptors__offset) + 8 + i * 12);
    MaybeObject::VerifyMaybeObjectPointer(isolate, value__value);
    CHECK(value__value.IsCleared() || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsJSReceiver()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsSmi()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsHeapNumber()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsBigInt()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsString()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsSymbol()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsTrue()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsFalse()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsNull()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsUndefined()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsAccessorInfo()) || (value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsMap()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsAccessorPair()) || (!value__value.IsWeak() && value__value.GetHeapObjectOrSmi().IsClassPositions()));
  }
}
void TorqueGeneratedClassVerifiers::StrongDescriptorArrayVerify(StrongDescriptorArray o, Isolate* isolate) {
  o.DescriptorArrayVerify(isolate);
  CHECK(o.IsStrongDescriptorArray());
}
void TorqueGeneratedClassVerifiers::EmbedderDataArrayVerify(EmbedderDataArray o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsEmbedderDataArray());
  {
    Object length__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, length__value);
    CHECK(length__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::FeedbackCellVerify(FeedbackCell o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsFeedbackCell());
  {
    Object value__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, value__value);
    CHECK(value__value.IsUndefined() || value__value.IsFixedArray() || value__value.IsFeedbackVector());
  }
}
void TorqueGeneratedClassVerifiers::FeedbackVectorVerify(FeedbackVector o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsFeedbackVector());
  {
    Object shared_function_info__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, shared_function_info__value);
    CHECK(shared_function_info__value.IsSharedFunctionInfo());
  }
  {
    MaybeObject maybe_optimized_code__value = TaggedField<MaybeObject>::load(o, 24);
    MaybeObject::VerifyMaybeObjectPointer(isolate, maybe_optimized_code__value);
    CHECK(maybe_optimized_code__value.IsCleared() || (maybe_optimized_code__value.IsWeak() && maybe_optimized_code__value.GetHeapObjectOrSmi().IsCode()));
  }
  {
    Object closure_feedback_cell_array__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, closure_feedback_cell_array__value);
    CHECK(closure_feedback_cell_array__value.IsClosureFeedbackCellArray());
  }
  intptr_t raw_feedback_slots__offset, raw_feedback_slots__length;
  std::tie(std::ignore, raw_feedback_slots__offset, raw_feedback_slots__length) = TqRuntimeFieldSliceFeedbackVectorRawFeedbackSlots(o);
  CHECK_EQ(raw_feedback_slots__offset, static_cast<int>(raw_feedback_slots__offset));
  CHECK_EQ(raw_feedback_slots__length, static_cast<int>(raw_feedback_slots__length));
  for (int i = 0; i < static_cast<int>(raw_feedback_slots__length); ++i) {
    MaybeObject raw_feedback_slots__value = TaggedField<MaybeObject>::load(o, static_cast<int>(raw_feedback_slots__offset) + i * kTaggedSize);
    MaybeObject::VerifyMaybeObjectPointer(isolate, raw_feedback_slots__value);
    CHECK(raw_feedback_slots__value.IsCleared() || (!raw_feedback_slots__value.IsWeak() && raw_feedback_slots__value.GetHeapObjectOrSmi().IsHeapObject()) || (!raw_feedback_slots__value.IsWeak() && raw_feedback_slots__value.GetHeapObjectOrSmi().IsSmi()) || raw_feedback_slots__value.IsWeak());
  }
}
void TorqueGeneratedClassVerifiers::ArrayListVerify(ArrayList o, Isolate* isolate) {
  o.FixedArrayVerify(isolate);
  CHECK(o.IsArrayList());
}
void TorqueGeneratedClassVerifiers::TemplateListVerify(TemplateList o, Isolate* isolate) {
  o.FixedArrayVerify(isolate);
  CHECK(o.IsTemplateList());
}
void TorqueGeneratedClassVerifiers::WeakArrayListVerify(WeakArrayList o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsWeakArrayList());
  {
    Object capacity__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, capacity__value);
    CHECK(capacity__value.IsSmi());
  }
  {
    Object length__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, length__value);
    CHECK(length__value.IsSmi());
  }
  intptr_t objects__offset, objects__length;
  std::tie(std::ignore, objects__offset, objects__length) = TqRuntimeFieldSliceWeakArrayListObjects(o);
  CHECK_EQ(objects__offset, static_cast<int>(objects__offset));
  CHECK_EQ(objects__length, static_cast<int>(objects__length));
  for (int i = 0; i < static_cast<int>(objects__length); ++i) {
    MaybeObject objects__value = TaggedField<MaybeObject>::load(o, static_cast<int>(objects__offset) + i * kTaggedSize);
    MaybeObject::VerifyMaybeObjectPointer(isolate, objects__value);
    CHECK(objects__value.IsCleared() || (!objects__value.IsWeak() && objects__value.GetHeapObjectOrSmi().IsHeapObject()) || (!objects__value.IsWeak() && objects__value.GetHeapObjectOrSmi().IsSmi()) || objects__value.IsWeak());
  }
}
void TorqueGeneratedClassVerifiers::FreeSpaceVerify(FreeSpace o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsFreeSpace());
  {
    Object size__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, size__value);
    CHECK(size__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::JSArrayBufferVerify(JSArrayBuffer o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSArrayBuffer());
}
void TorqueGeneratedClassVerifiers::JSDataViewVerify(JSDataView o, Isolate* isolate) {
  o.JSArrayBufferViewVerify(isolate);
  CHECK(o.IsJSDataView());
}
void TorqueGeneratedClassVerifiers::JSArrayIteratorVerify(JSArrayIterator o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSArrayIterator());
  {
    Object iterated_object__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, iterated_object__value);
    CHECK(iterated_object__value.IsJSReceiver());
  }
  {
    Object next_index__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, next_index__value);
    CHECK(next_index__value.IsSmi() || next_index__value.IsHeapNumber());
  }
  {
    Object kind__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, kind__value);
    CHECK(kind__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::JSArrayVerify(JSArray o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSArray());
  {
    Object length__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, length__value);
    CHECK(length__value.IsSmi() || length__value.IsHeapNumber());
  }
}
void TorqueGeneratedClassVerifiers::JSCollectionIteratorVerify(JSCollectionIterator o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSCollectionIterator());
  {
    Object table__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, table__value);
  }
  {
    Object index__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, index__value);
  }
}
void TorqueGeneratedClassVerifiers::JSCollectionVerify(JSCollection o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSCollection());
  {
    Object table__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, table__value);
  }
}
void TorqueGeneratedClassVerifiers::JSSetVerify(JSSet o, Isolate* isolate) {
  o.JSCollectionVerify(isolate);
  CHECK(o.IsJSSet());
}
void TorqueGeneratedClassVerifiers::JSMapVerify(JSMap o, Isolate* isolate) {
  o.JSCollectionVerify(isolate);
  CHECK(o.IsJSMap());
}
void TorqueGeneratedClassVerifiers::JSWeakCollectionVerify(JSWeakCollection o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSWeakCollection());
  {
    Object table__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, table__value);
  }
}
void TorqueGeneratedClassVerifiers::JSWeakSetVerify(JSWeakSet o, Isolate* isolate) {
  o.JSWeakCollectionVerify(isolate);
  CHECK(o.IsJSWeakSet());
}
void TorqueGeneratedClassVerifiers::JSWeakMapVerify(JSWeakMap o, Isolate* isolate) {
  o.JSWeakCollectionVerify(isolate);
  CHECK(o.IsJSWeakMap());
}
void TorqueGeneratedClassVerifiers::JSGeneratorObjectVerify(JSGeneratorObject o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSGeneratorObject());
  {
    Object function__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, function__value);
    CHECK(function__value.IsJSFunction());
  }
  {
    Object context__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, context__value);
    CHECK(context__value.IsContext());
  }
  {
    Object receiver__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, receiver__value);
    CHECK(receiver__value.IsJSReceiver() || receiver__value.IsSmi() || receiver__value.IsHeapNumber() || receiver__value.IsBigInt() || receiver__value.IsString() || receiver__value.IsSymbol() || receiver__value.IsTrue() || receiver__value.IsFalse() || receiver__value.IsNull() || receiver__value.IsUndefined());
  }
  {
    Object input_or_debug_pos__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, input_or_debug_pos__value);
  }
  {
    Object resume_mode__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, resume_mode__value);
    CHECK(resume_mode__value.IsSmi());
  }
  {
    Object continuation__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, continuation__value);
    CHECK(continuation__value.IsSmi());
  }
  {
    Object parameters_and_registers__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, parameters_and_registers__value);
    CHECK(parameters_and_registers__value.IsFixedArray());
  }
}
void TorqueGeneratedClassVerifiers::JSAsyncFunctionObjectVerify(JSAsyncFunctionObject o, Isolate* isolate) {
  o.JSGeneratorObjectVerify(isolate);
  CHECK(o.IsJSAsyncFunctionObject());
  {
    Object promise__value = TaggedField<Object>::load(o, 40);
    Object::VerifyPointer(isolate, promise__value);
    CHECK(promise__value.IsJSPromise());
  }
}
void TorqueGeneratedClassVerifiers::JSAsyncGeneratorObjectVerify(JSAsyncGeneratorObject o, Isolate* isolate) {
  o.JSGeneratorObjectVerify(isolate);
  CHECK(o.IsJSAsyncGeneratorObject());
  {
    Object queue__value = TaggedField<Object>::load(o, 40);
    Object::VerifyPointer(isolate, queue__value);
    CHECK(queue__value.IsHeapObject());
  }
  {
    Object is_awaiting__value = TaggedField<Object>::load(o, 44);
    Object::VerifyPointer(isolate, is_awaiting__value);
    CHECK(is_awaiting__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::AsyncGeneratorRequestVerify(AsyncGeneratorRequest o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsAsyncGeneratorRequest());
  {
    Object next__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, next__value);
    CHECK(next__value.IsUndefined() || next__value.IsAsyncGeneratorRequest());
  }
  {
    Object resume_mode__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, resume_mode__value);
    CHECK(resume_mode__value.IsSmi());
  }
  {
    Object value__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, value__value);
  }
  {
    Object promise__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, promise__value);
    CHECK(promise__value.IsJSPromise());
  }
}
void TorqueGeneratedClassVerifiers::JSGlobalProxyVerify(JSGlobalProxy o, Isolate* isolate) {
  o.JSSpecialObjectVerify(isolate);
  CHECK(o.IsJSGlobalProxy());
  {
    Object native_context__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, native_context__value);
  }
}
void TorqueGeneratedClassVerifiers::JSGlobalObjectVerify(JSGlobalObject o, Isolate* isolate) {
  o.JSSpecialObjectVerify(isolate);
  CHECK(o.IsJSGlobalObject());
  {
    Object native_context__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, native_context__value);
    CHECK(native_context__value.IsNativeContext());
  }
  {
    Object global_proxy__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, global_proxy__value);
    CHECK(global_proxy__value.IsJSGlobalProxy());
  }
}
void TorqueGeneratedClassVerifiers::JSPrimitiveWrapperVerify(JSPrimitiveWrapper o, Isolate* isolate) {
  o.JSCustomElementsObjectVerify(isolate);
  CHECK(o.IsJSPrimitiveWrapper());
  {
    Object value__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, value__value);
    CHECK(value__value.IsJSReceiver() || value__value.IsSmi() || value__value.IsHeapNumber() || value__value.IsBigInt() || value__value.IsString() || value__value.IsSymbol() || value__value.IsTrue() || value__value.IsFalse() || value__value.IsNull() || value__value.IsUndefined());
  }
}
void TorqueGeneratedClassVerifiers::JSMessageObjectVerify(JSMessageObject o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSMessageObject());
  {
    Object message_type__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, message_type__value);
    CHECK(message_type__value.IsSmi());
  }
  {
    Object arguments__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, arguments__value);
  }
  {
    Object script__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, script__value);
    CHECK(script__value.IsScript());
  }
  {
    Object stack_frames__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, stack_frames__value);
  }
  {
    Object shared_info__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, shared_info__value);
    CHECK(shared_info__value.IsUndefined() || shared_info__value.IsSharedFunctionInfo());
  }
  {
    Object bytecode_offset__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, bytecode_offset__value);
    CHECK(bytecode_offset__value.IsSmi());
  }
  {
    Object start_position__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, start_position__value);
    CHECK(start_position__value.IsSmi());
  }
  {
    Object end_position__value = TaggedField<Object>::load(o, 40);
    Object::VerifyPointer(isolate, end_position__value);
    CHECK(end_position__value.IsSmi());
  }
  {
    Object error_level__value = TaggedField<Object>::load(o, 44);
    Object::VerifyPointer(isolate, error_level__value);
    CHECK(error_level__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::JSDateVerify(JSDate o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSDate());
  {
    Object value__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, value__value);
    CHECK(value__value.IsSmi() || value__value.IsHeapNumber() || value__value.IsUndefined());
  }
  {
    Object year__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, year__value);
    CHECK(year__value.IsSmi() || year__value.IsUndefined() || year__value.IsNaN());
  }
  {
    Object month__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, month__value);
    CHECK(month__value.IsSmi() || month__value.IsUndefined() || month__value.IsNaN());
  }
  {
    Object day__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, day__value);
    CHECK(day__value.IsSmi() || day__value.IsUndefined() || day__value.IsNaN());
  }
  {
    Object weekday__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, weekday__value);
    CHECK(weekday__value.IsSmi() || weekday__value.IsUndefined() || weekday__value.IsNaN());
  }
  {
    Object hour__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, hour__value);
    CHECK(hour__value.IsSmi() || hour__value.IsUndefined() || hour__value.IsNaN());
  }
  {
    Object min__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, min__value);
    CHECK(min__value.IsSmi() || min__value.IsUndefined() || min__value.IsNaN());
  }
  {
    Object sec__value = TaggedField<Object>::load(o, 40);
    Object::VerifyPointer(isolate, sec__value);
    CHECK(sec__value.IsSmi() || sec__value.IsUndefined() || sec__value.IsNaN());
  }
  {
    Object cache_stamp__value = TaggedField<Object>::load(o, 44);
    Object::VerifyPointer(isolate, cache_stamp__value);
    CHECK(cache_stamp__value.IsSmi() || cache_stamp__value.IsUndefined() || cache_stamp__value.IsNaN());
  }
}
void TorqueGeneratedClassVerifiers::JSAsyncFromSyncIteratorVerify(JSAsyncFromSyncIterator o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSAsyncFromSyncIterator());
  {
    Object sync_iterator__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, sync_iterator__value);
    CHECK(sync_iterator__value.IsJSReceiver());
  }
  {
    Object next__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, next__value);
  }
}
void TorqueGeneratedClassVerifiers::JSStringIteratorVerify(JSStringIterator o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSStringIterator());
  {
    Object string__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, string__value);
    CHECK(string__value.IsString());
  }
  {
    Object index__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, index__value);
    CHECK(index__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::JSRegExpStringIteratorVerify(JSRegExpStringIterator o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSRegExpStringIterator());
  {
    Object iterating_reg_exp__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, iterating_reg_exp__value);
    CHECK(iterating_reg_exp__value.IsJSReceiver());
  }
  {
    Object iterated_string__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, iterated_string__value);
    CHECK(iterated_string__value.IsString());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::JSRegExpVerify(JSRegExp o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSRegExp());
  {
    Object data__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, data__value);
    CHECK(data__value.IsUndefined() || data__value.IsFixedArray());
  }
  {
    Object source__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, source__value);
    CHECK(source__value.IsString() || source__value.IsUndefined());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsUndefined() || flags__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::JSFinalizationRegistryVerify(JSFinalizationRegistry o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSFinalizationRegistry());
  {
    Object native_context__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, native_context__value);
    CHECK(native_context__value.IsNativeContext());
  }
  {
    Object cleanup__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, cleanup__value);
    CHECK(cleanup__value.IsCallableApiObject() || cleanup__value.IsCallableJSProxy() || cleanup__value.IsJSFunction() || cleanup__value.IsJSBoundFunction());
  }
  {
    Object active_cells__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, active_cells__value);
    CHECK(active_cells__value.IsUndefined() || active_cells__value.IsWeakCell());
  }
  {
    Object cleared_cells__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, cleared_cells__value);
    CHECK(cleared_cells__value.IsUndefined() || cleared_cells__value.IsWeakCell());
  }
  {
    Object key_map__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, key_map__value);
  }
  {
    Object next_dirty__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, next_dirty__value);
    CHECK(next_dirty__value.IsUndefined() || next_dirty__value.IsJSFinalizationRegistry());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::JSWeakRefVerify(JSWeakRef o, Isolate* isolate) {
  o.JSObjectVerify(isolate);
  CHECK(o.IsJSWeakRef());
  {
    Object target__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, target__value);
    CHECK(target__value.IsJSReceiver() || target__value.IsUndefined());
  }
}
void TorqueGeneratedClassVerifiers::ArrayBoilerplateDescriptionVerify(ArrayBoilerplateDescription o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsArrayBoilerplateDescription());
  {
    Object flags__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
  {
    Object constant_elements__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, constant_elements__value);
    CHECK(constant_elements__value.IsFixedArrayBase());
  }
}
void TorqueGeneratedClassVerifiers::RegExpBoilerplateDescriptionVerify(RegExpBoilerplateDescription o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsRegExpBoilerplateDescription());
  {
    Object data__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, data__value);
    CHECK(data__value.IsFixedArray());
  }
  {
    Object source__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, source__value);
    CHECK(source__value.IsString());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::MegaDomHandlerVerify(MegaDomHandler o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsMegaDomHandler());
  {
    MaybeObject accessor__value = TaggedField<MaybeObject>::load(o, 4);
    MaybeObject::VerifyMaybeObjectPointer(isolate, accessor__value);
    CHECK(accessor__value.IsCleared() || (!accessor__value.IsWeak() && accessor__value.GetHeapObjectOrSmi().IsHeapObject()) || (!accessor__value.IsWeak() && accessor__value.GetHeapObjectOrSmi().IsSmi()) || accessor__value.IsWeak());
  }
  {
    MaybeObject context__value = TaggedField<MaybeObject>::load(o, 8);
    MaybeObject::VerifyMaybeObjectPointer(isolate, context__value);
    CHECK(context__value.IsCleared() || (!context__value.IsWeak() && context__value.GetHeapObjectOrSmi().IsHeapObject()) || (!context__value.IsWeak() && context__value.GetHeapObjectOrSmi().IsSmi()) || context__value.IsWeak());
  }
}
void TorqueGeneratedClassVerifiers::MicrotaskVerify(Microtask o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsMicrotask());
}
void TorqueGeneratedClassVerifiers::CallbackTaskVerify(CallbackTask o, Isolate* isolate) {
  o.MicrotaskVerify(isolate);
  CHECK(o.IsCallbackTask());
  {
    Object callback__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, callback__value);
    CHECK(callback__value.IsForeign());
  }
  {
    Object data__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, data__value);
    CHECK(data__value.IsForeign());
  }
}
void TorqueGeneratedClassVerifiers::CallableTaskVerify(CallableTask o, Isolate* isolate) {
  o.MicrotaskVerify(isolate);
  CHECK(o.IsCallableTask());
  {
    Object callable__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, callable__value);
    CHECK(callable__value.IsJSReceiver());
  }
  {
    Object context__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, context__value);
    CHECK(context__value.IsContext());
  }
}
void TorqueGeneratedClassVerifiers::ModuleVerify(Module o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsModule());
  {
    Object exports__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, exports__value);
    CHECK(exports__value.IsObjectHashTable());
  }
  {
    Object hash__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, hash__value);
    CHECK(hash__value.IsSmi());
  }
  {
    Object status__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, status__value);
    CHECK(status__value.IsSmi());
  }
  {
    Object module_namespace__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, module_namespace__value);
    CHECK(module_namespace__value.IsUndefined() || module_namespace__value.IsJSModuleNamespace());
  }
  {
    Object exception__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, exception__value);
  }
  {
    Object top_level_capability__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, top_level_capability__value);
    CHECK(top_level_capability__value.IsUndefined() || top_level_capability__value.IsJSPromise());
  }
}
void TorqueGeneratedClassVerifiers::JSModuleNamespaceVerify(JSModuleNamespace o, Isolate* isolate) {
  o.JSSpecialObjectVerify(isolate);
  CHECK(o.IsJSModuleNamespace());
  {
    Object module__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, module__value);
    CHECK(module__value.IsModule());
  }
}
void TorqueGeneratedClassVerifiers::SmallOrderedHashSetVerify(SmallOrderedHashSet o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsSmallOrderedHashSet());
  intptr_t data_table__offset, data_table__length;
  std::tie(std::ignore, data_table__offset, data_table__length) = TqRuntimeFieldSliceSmallOrderedHashSetDataTable(o);
  CHECK_EQ(data_table__offset, static_cast<int>(data_table__offset));
  CHECK_EQ(data_table__length, static_cast<int>(data_table__length));
  for (int i = 0; i < static_cast<int>(data_table__length); ++i) {
    Object data_table__value = TaggedField<Object>::load(o, static_cast<int>(data_table__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, data_table__value);
    CHECK(data_table__value.IsJSReceiver() || data_table__value.IsSmi() || data_table__value.IsHeapNumber() || data_table__value.IsBigInt() || data_table__value.IsString() || data_table__value.IsSymbol() || data_table__value.IsTrue() || data_table__value.IsFalse() || data_table__value.IsNull() || data_table__value.IsUndefined() || data_table__value.IsTheHole());
  }
}
void TorqueGeneratedClassVerifiers::SmallOrderedHashMapVerify(SmallOrderedHashMap o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsSmallOrderedHashMap());
  intptr_t data_table__offset, data_table__length;
  std::tie(std::ignore, data_table__offset, data_table__length) = TqRuntimeFieldSliceSmallOrderedHashMapDataTable(o);
  CHECK_EQ(data_table__offset, static_cast<int>(data_table__offset));
  CHECK_EQ(data_table__length, static_cast<int>(data_table__length));
  for (int i = 0; i < static_cast<int>(data_table__length); ++i) {
    Object key__value = TaggedField<Object>::load(o, static_cast<int>(data_table__offset) + 0 + i * 8);
    Object::VerifyPointer(isolate, key__value);
    CHECK(key__value.IsJSReceiver() || key__value.IsSmi() || key__value.IsHeapNumber() || key__value.IsBigInt() || key__value.IsString() || key__value.IsSymbol() || key__value.IsTrue() || key__value.IsFalse() || key__value.IsNull() || key__value.IsUndefined() || key__value.IsTheHole());
    Object value__value = TaggedField<Object>::load(o, static_cast<int>(data_table__offset) + 4 + i * 8);
    Object::VerifyPointer(isolate, value__value);
    CHECK(value__value.IsJSReceiver() || value__value.IsSmi() || value__value.IsHeapNumber() || value__value.IsBigInt() || value__value.IsString() || value__value.IsSymbol() || value__value.IsTrue() || value__value.IsFalse() || value__value.IsNull() || value__value.IsUndefined() || value__value.IsTheHole());
  }
}
void TorqueGeneratedClassVerifiers::SmallOrderedNameDictionaryVerify(SmallOrderedNameDictionary o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsSmallOrderedNameDictionary());
  intptr_t data_table__offset, data_table__length;
  std::tie(std::ignore, data_table__offset, data_table__length) = TqRuntimeFieldSliceSmallOrderedNameDictionaryDataTable(o);
  CHECK_EQ(data_table__offset, static_cast<int>(data_table__offset));
  CHECK_EQ(data_table__length, static_cast<int>(data_table__length));
  for (int i = 0; i < static_cast<int>(data_table__length); ++i) {
    Object key__value = TaggedField<Object>::load(o, static_cast<int>(data_table__offset) + 0 + i * 12);
    Object::VerifyPointer(isolate, key__value);
    CHECK(key__value.IsJSReceiver() || key__value.IsSmi() || key__value.IsHeapNumber() || key__value.IsBigInt() || key__value.IsString() || key__value.IsSymbol() || key__value.IsTrue() || key__value.IsFalse() || key__value.IsNull() || key__value.IsUndefined() || key__value.IsTheHole());
    Object value__value = TaggedField<Object>::load(o, static_cast<int>(data_table__offset) + 4 + i * 12);
    Object::VerifyPointer(isolate, value__value);
    CHECK(value__value.IsJSReceiver() || value__value.IsSmi() || value__value.IsHeapNumber() || value__value.IsBigInt() || value__value.IsString() || value__value.IsSymbol() || value__value.IsTrue() || value__value.IsFalse() || value__value.IsNull() || value__value.IsUndefined() || value__value.IsTheHole());
    Object property_details__value = TaggedField<Object>::load(o, static_cast<int>(data_table__offset) + 8 + i * 12);
    Object::VerifyPointer(isolate, property_details__value);
    CHECK(property_details__value.IsSmi() || property_details__value.IsTheHole());
  }
}
void TorqueGeneratedClassVerifiers::PromiseReactionVerify(PromiseReaction o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsPromiseReaction());
  {
    Object next__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, next__value);
    CHECK(next__value.IsZero() || next__value.IsPromiseReaction());
  }
  {
    Object reject_handler__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, reject_handler__value);
    CHECK(reject_handler__value.IsUndefined() || reject_handler__value.IsCallableApiObject() || reject_handler__value.IsCallableJSProxy() || reject_handler__value.IsJSFunction() || reject_handler__value.IsJSBoundFunction());
  }
  {
    Object fulfill_handler__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, fulfill_handler__value);
    CHECK(fulfill_handler__value.IsUndefined() || fulfill_handler__value.IsCallableApiObject() || fulfill_handler__value.IsCallableJSProxy() || fulfill_handler__value.IsJSFunction() || fulfill_handler__value.IsJSBoundFunction());
  }
  {
    Object promise_or_capability__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, promise_or_capability__value);
    CHECK(promise_or_capability__value.IsUndefined() || promise_or_capability__value.IsJSPromise() || promise_or_capability__value.IsPromiseCapability());
  }
  {
    Object continuation_preserved_embedder_data__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, continuation_preserved_embedder_data__value);
  }
}
void TorqueGeneratedClassVerifiers::PromiseReactionJobTaskVerify(PromiseReactionJobTask o, Isolate* isolate) {
  o.MicrotaskVerify(isolate);
  CHECK(o.IsPromiseReactionJobTask());
  {
    Object argument__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, argument__value);
  }
  {
    Object context__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, context__value);
    CHECK(context__value.IsContext());
  }
  {
    Object handler__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, handler__value);
    CHECK(handler__value.IsUndefined() || handler__value.IsCallableApiObject() || handler__value.IsCallableJSProxy() || handler__value.IsJSFunction() || handler__value.IsJSBoundFunction());
  }
  {
    Object promise_or_capability__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, promise_or_capability__value);
    CHECK(promise_or_capability__value.IsUndefined() || promise_or_capability__value.IsJSPromise() || promise_or_capability__value.IsPromiseCapability());
  }
  {
    Object continuation_preserved_embedder_data__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, continuation_preserved_embedder_data__value);
  }
}
void TorqueGeneratedClassVerifiers::PromiseFulfillReactionJobTaskVerify(PromiseFulfillReactionJobTask o, Isolate* isolate) {
  o.PromiseReactionJobTaskVerify(isolate);
  CHECK(o.IsPromiseFulfillReactionJobTask());
}
void TorqueGeneratedClassVerifiers::PromiseRejectReactionJobTaskVerify(PromiseRejectReactionJobTask o, Isolate* isolate) {
  o.PromiseReactionJobTaskVerify(isolate);
  CHECK(o.IsPromiseRejectReactionJobTask());
}
void TorqueGeneratedClassVerifiers::PromiseResolveThenableJobTaskVerify(PromiseResolveThenableJobTask o, Isolate* isolate) {
  o.MicrotaskVerify(isolate);
  CHECK(o.IsPromiseResolveThenableJobTask());
  {
    Object context__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, context__value);
    CHECK(context__value.IsContext());
  }
  {
    Object promise_to_resolve__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, promise_to_resolve__value);
    CHECK(promise_to_resolve__value.IsJSPromise());
  }
  {
    Object thenable__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, thenable__value);
    CHECK(thenable__value.IsJSReceiver());
  }
  {
    Object then__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, then__value);
    CHECK(then__value.IsJSReceiver());
  }
}
void TorqueGeneratedClassVerifiers::PropertyArrayVerify(PropertyArray o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsPropertyArray());
  {
    Object length_and_hash__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, length_and_hash__value);
    CHECK(length_and_hash__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::PropertyCellVerify(PropertyCell o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsPropertyCell());
  {
    Object name__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, name__value);
    CHECK(name__value.IsString() || name__value.IsPublicSymbol() || name__value.IsPrivateSymbol());
  }
  {
    Object property_details_raw__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, property_details_raw__value);
    CHECK(property_details_raw__value.IsSmi());
  }
  {
    Object value__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, value__value);
  }
  {
    Object dependent_code__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, dependent_code__value);
    CHECK(dependent_code__value.IsDependentCode());
  }
}
void TorqueGeneratedClassVerifiers::PropertyDescriptorObjectVerify(PropertyDescriptorObject o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsPropertyDescriptorObject());
  {
    Object flags__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
  {
    Object value__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, value__value);
    CHECK(value__value.IsJSReceiver() || value__value.IsSmi() || value__value.IsHeapNumber() || value__value.IsBigInt() || value__value.IsString() || value__value.IsSymbol() || value__value.IsTrue() || value__value.IsFalse() || value__value.IsNull() || value__value.IsUndefined() || value__value.IsTheHole());
  }
  {
    Object get__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, get__value);
    CHECK(get__value.IsJSReceiver() || get__value.IsSmi() || get__value.IsHeapNumber() || get__value.IsBigInt() || get__value.IsString() || get__value.IsSymbol() || get__value.IsTrue() || get__value.IsFalse() || get__value.IsNull() || get__value.IsUndefined() || get__value.IsTheHole());
  }
  {
    Object set__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, set__value);
    CHECK(set__value.IsJSReceiver() || set__value.IsSmi() || set__value.IsHeapNumber() || set__value.IsBigInt() || set__value.IsString() || set__value.IsSymbol() || set__value.IsTrue() || set__value.IsFalse() || set__value.IsNull() || set__value.IsUndefined() || set__value.IsTheHole());
  }
}
void TorqueGeneratedClassVerifiers::PrototypeInfoVerify(PrototypeInfo o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsPrototypeInfo());
  {
    Object module_namespace__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, module_namespace__value);
    CHECK(module_namespace__value.IsUndefined() || module_namespace__value.IsJSModuleNamespace());
  }
  {
    Object prototype_users__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, prototype_users__value);
    CHECK(prototype_users__value.IsZero() || prototype_users__value.IsWeakArrayList());
  }
  {
    Object prototype_chain_enum_cache__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, prototype_chain_enum_cache__value);
    CHECK(prototype_chain_enum_cache__value.IsUndefined() || prototype_chain_enum_cache__value.IsFixedArray() || prototype_chain_enum_cache__value.IsZero());
  }
  {
    Object registry_slot__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, registry_slot__value);
    CHECK(registry_slot__value.IsSmi());
  }
  {
    MaybeObject object_create_map__value = TaggedField<MaybeObject>::load(o, 20);
    MaybeObject::VerifyMaybeObjectPointer(isolate, object_create_map__value);
    CHECK(object_create_map__value.IsCleared() || (!object_create_map__value.IsWeak() && object_create_map__value.GetHeapObjectOrSmi().IsUndefined()) || (object_create_map__value.IsWeak() && object_create_map__value.GetHeapObjectOrSmi().IsMap()));
  }
  {
    Object bit_field__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, bit_field__value);
    CHECK(bit_field__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::ScriptVerify(Script o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsScript());
  {
    Object source__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, source__value);
    CHECK(source__value.IsString() || source__value.IsUndefined());
  }
  {
    Object name__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, name__value);
  }
  {
    Object line_offset__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, line_offset__value);
    CHECK(line_offset__value.IsSmi());
  }
  {
    Object column_offset__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, column_offset__value);
    CHECK(column_offset__value.IsSmi());
  }
  {
    Object context_data__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, context_data__value);
    CHECK(context_data__value.IsSmi() || context_data__value.IsSymbol() || context_data__value.IsUndefined());
  }
  {
    Object script_type__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, script_type__value);
    CHECK(script_type__value.IsSmi());
  }
  {
    Object line_ends__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, line_ends__value);
    CHECK(line_ends__value.IsUndefined() || line_ends__value.IsFixedArray());
  }
  {
    Object id__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, id__value);
    CHECK(id__value.IsSmi());
  }
  {
    Object eval_from_shared_or_wrapped_arguments_or_sfi_table__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, eval_from_shared_or_wrapped_arguments_or_sfi_table__value);
    CHECK(eval_from_shared_or_wrapped_arguments_or_sfi_table__value.IsUndefined() || eval_from_shared_or_wrapped_arguments_or_sfi_table__value.IsFixedArray() || eval_from_shared_or_wrapped_arguments_or_sfi_table__value.IsSharedFunctionInfo());
  }
  {
    Object eval_from_position__value = TaggedField<Object>::load(o, 40);
    Object::VerifyPointer(isolate, eval_from_position__value);
    CHECK(eval_from_position__value.IsSmi() || eval_from_position__value.IsForeign());
  }
  {
    Object shared_function_infos__value = TaggedField<Object>::load(o, 44);
    Object::VerifyPointer(isolate, shared_function_infos__value);
    CHECK(shared_function_infos__value.IsWeakFixedArray() || shared_function_infos__value.IsWeakArrayList());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 48);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
  {
    Object source_url__value = TaggedField<Object>::load(o, 52);
    Object::VerifyPointer(isolate, source_url__value);
    CHECK(source_url__value.IsString() || source_url__value.IsUndefined());
  }
  {
    Object source_mapping_url__value = TaggedField<Object>::load(o, 56);
    Object::VerifyPointer(isolate, source_mapping_url__value);
  }
  {
    Object host_defined_options__value = TaggedField<Object>::load(o, 60);
    Object::VerifyPointer(isolate, host_defined_options__value);
    CHECK(host_defined_options__value.IsFixedArray());
  }
}
void TorqueGeneratedClassVerifiers::PreparseDataVerify(PreparseData o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsPreparseData());
}
void TorqueGeneratedClassVerifiers::InterpreterDataVerify(InterpreterData o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsInterpreterData());
  {
    Object bytecode_array__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, bytecode_array__value);
    CHECK(bytecode_array__value.IsBytecodeArray());
  }
  {
    Object interpreter_trampoline__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, interpreter_trampoline__value);
    CHECK(interpreter_trampoline__value.IsCode());
  }
}
void TorqueGeneratedClassVerifiers::BaselineDataVerify(BaselineData o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsBaselineData());
  {
    Object baseline_code__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, baseline_code__value);
    CHECK(baseline_code__value.IsCode());
  }
  {
    Object data__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, data__value);
    CHECK(data__value.IsBytecodeArray() || data__value.IsInterpreterData());
  }
}
void TorqueGeneratedClassVerifiers::SharedFunctionInfoVerify(SharedFunctionInfo o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsSharedFunctionInfo());
  {
    Object function_data__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, function_data__value);
  }
  {
    Object name_or_scope_info__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, name_or_scope_info__value);
    CHECK(name_or_scope_info__value.IsString() || name_or_scope_info__value.IsNoSharedNameSentinel() || name_or_scope_info__value.IsScopeInfo());
  }
  {
    Object outer_scope_info_or_feedback_metadata__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, outer_scope_info_or_feedback_metadata__value);
    CHECK(outer_scope_info_or_feedback_metadata__value.IsHeapObject());
  }
  {
    Object script_or_debug_info__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, script_or_debug_info__value);
    CHECK(script_or_debug_info__value.IsUndefined() || script_or_debug_info__value.IsDebugInfo() || script_or_debug_info__value.IsScript());
  }
}
void TorqueGeneratedClassVerifiers::UncompiledDataVerify(UncompiledData o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsUncompiledData());
  {
    Object inferred_name__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, inferred_name__value);
    CHECK(inferred_name__value.IsString());
  }
}
void TorqueGeneratedClassVerifiers::UncompiledDataWithoutPreparseDataVerify(UncompiledDataWithoutPreparseData o, Isolate* isolate) {
  o.UncompiledDataVerify(isolate);
  CHECK(o.IsUncompiledDataWithoutPreparseData());
}
void TorqueGeneratedClassVerifiers::UncompiledDataWithPreparseDataVerify(UncompiledDataWithPreparseData o, Isolate* isolate) {
  o.UncompiledDataVerify(isolate);
  CHECK(o.IsUncompiledDataWithPreparseData());
  {
    Object preparse_data__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, preparse_data__value);
    CHECK(preparse_data__value.IsPreparseData());
  }
}
void TorqueGeneratedClassVerifiers::OnHeapBasicBlockProfilerDataVerify(OnHeapBasicBlockProfilerData o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsOnHeapBasicBlockProfilerData());
  {
    Object block_ids__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, block_ids__value);
    CHECK(block_ids__value.IsByteArray());
  }
  {
    Object counts__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, counts__value);
    CHECK(counts__value.IsByteArray());
  }
  {
    Object name__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, name__value);
    CHECK(name__value.IsString());
  }
  {
    Object schedule__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, schedule__value);
    CHECK(schedule__value.IsString());
  }
  {
    Object code__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, code__value);
    CHECK(code__value.IsString());
  }
  {
    Object hash__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, hash__value);
    CHECK(hash__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::SourceTextModuleVerify(SourceTextModule o, Isolate* isolate) {
  o.ModuleVerify(isolate);
  CHECK(o.IsSourceTextModule());
  {
    Object code__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, code__value);
    CHECK(code__value.IsJSFunction() || code__value.IsJSGeneratorObject() || code__value.IsSharedFunctionInfo());
  }
  {
    Object regular_exports__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, regular_exports__value);
    CHECK(regular_exports__value.IsFixedArray());
  }
  {
    Object regular_imports__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, regular_imports__value);
    CHECK(regular_imports__value.IsFixedArray());
  }
  {
    Object requested_modules__value = TaggedField<Object>::load(o, 40);
    Object::VerifyPointer(isolate, requested_modules__value);
    CHECK(requested_modules__value.IsFixedArray());
  }
  {
    Object import_meta__value = TaggedField<Object>::load(o, 44);
    Object::VerifyPointer(isolate, import_meta__value);
    CHECK(import_meta__value.IsJSObject() || import_meta__value.IsTheHole());
  }
  {
    Object cycle_root__value = TaggedField<Object>::load(o, 48);
    Object::VerifyPointer(isolate, cycle_root__value);
    CHECK(cycle_root__value.IsTheHole() || cycle_root__value.IsSourceTextModule());
  }
  {
    Object async_parent_modules__value = TaggedField<Object>::load(o, 52);
    Object::VerifyPointer(isolate, async_parent_modules__value);
    CHECK(async_parent_modules__value.IsArrayList());
  }
  {
    Object dfs_index__value = TaggedField<Object>::load(o, 56);
    Object::VerifyPointer(isolate, dfs_index__value);
    CHECK(dfs_index__value.IsSmi());
  }
  {
    Object dfs_ancestor_index__value = TaggedField<Object>::load(o, 60);
    Object::VerifyPointer(isolate, dfs_ancestor_index__value);
    CHECK(dfs_ancestor_index__value.IsSmi());
  }
  {
    Object pending_async_dependencies__value = TaggedField<Object>::load(o, 64);
    Object::VerifyPointer(isolate, pending_async_dependencies__value);
    CHECK(pending_async_dependencies__value.IsSmi());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 68);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::ModuleRequestVerify(ModuleRequest o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsModuleRequest());
  {
    Object specifier__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, specifier__value);
    CHECK(specifier__value.IsString());
  }
  {
    Object import_assertions__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, import_assertions__value);
    CHECK(import_assertions__value.IsFixedArray());
  }
  {
    Object position__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, position__value);
    CHECK(position__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::SourceTextModuleInfoEntryVerify(SourceTextModuleInfoEntry o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsSourceTextModuleInfoEntry());
  {
    Object export_name__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, export_name__value);
    CHECK(export_name__value.IsString() || export_name__value.IsUndefined());
  }
  {
    Object local_name__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, local_name__value);
    CHECK(local_name__value.IsString() || local_name__value.IsUndefined());
  }
  {
    Object import_name__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, import_name__value);
    CHECK(import_name__value.IsString() || import_name__value.IsUndefined());
  }
  {
    Object module_request__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, module_request__value);
    CHECK(module_request__value.IsSmi());
  }
  {
    Object cell_index__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, cell_index__value);
    CHECK(cell_index__value.IsSmi());
  }
  {
    Object beg_pos__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, beg_pos__value);
    CHECK(beg_pos__value.IsSmi());
  }
  {
    Object end_pos__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, end_pos__value);
    CHECK(end_pos__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::StackFrameInfoVerify(StackFrameInfo o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsStackFrameInfo());
  {
    Object receiver_or_instance__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, receiver_or_instance__value);
    CHECK(receiver_or_instance__value.IsJSReceiver() || receiver_or_instance__value.IsSmi() || receiver_or_instance__value.IsHeapNumber() || receiver_or_instance__value.IsBigInt() || receiver_or_instance__value.IsString() || receiver_or_instance__value.IsSymbol() || receiver_or_instance__value.IsTrue() || receiver_or_instance__value.IsFalse() || receiver_or_instance__value.IsNull() || receiver_or_instance__value.IsUndefined());
  }
  {
    Object function__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, function__value);
    CHECK(function__value.IsSmi() || function__value.IsJSFunction());
  }
  {
    Object code_object__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, code_object__value);
    CHECK(code_object__value.IsHeapObject());
  }
  {
    Object code_offset_or_source_position__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, code_offset_or_source_position__value);
    CHECK(code_offset_or_source_position__value.IsSmi());
  }
  {
    Object flags__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, flags__value);
    CHECK(flags__value.IsSmi());
  }
  {
    Object parameters__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, parameters__value);
    CHECK(parameters__value.IsFixedArray());
  }
}
void TorqueGeneratedClassVerifiers::ConsStringVerify(ConsString o, Isolate* isolate) {
  o.StringVerify(isolate);
  CHECK(o.IsConsString());
  {
    Object first__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, first__value);
    CHECK(first__value.IsString());
  }
  {
    Object second__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, second__value);
    CHECK(second__value.IsString());
  }
}
void TorqueGeneratedClassVerifiers::ExternalStringVerify(ExternalString o, Isolate* isolate) {
  o.StringVerify(isolate);
  CHECK(o.IsExternalString());
}
void TorqueGeneratedClassVerifiers::ExternalOneByteStringVerify(ExternalOneByteString o, Isolate* isolate) {
  o.ExternalStringVerify(isolate);
  CHECK(o.IsExternalOneByteString());
}
void TorqueGeneratedClassVerifiers::ExternalTwoByteStringVerify(ExternalTwoByteString o, Isolate* isolate) {
  o.ExternalStringVerify(isolate);
  CHECK(o.IsExternalTwoByteString());
}
void TorqueGeneratedClassVerifiers::InternalizedStringVerify(InternalizedString o, Isolate* isolate) {
  o.StringVerify(isolate);
  CHECK(o.IsInternalizedString());
}
void TorqueGeneratedClassVerifiers::SeqStringVerify(SeqString o, Isolate* isolate) {
  o.StringVerify(isolate);
  CHECK(o.IsSeqString());
}
void TorqueGeneratedClassVerifiers::SeqOneByteStringVerify(SeqOneByteString o, Isolate* isolate) {
  o.SeqStringVerify(isolate);
  CHECK(o.IsSeqOneByteString());
}
void TorqueGeneratedClassVerifiers::SeqTwoByteStringVerify(SeqTwoByteString o, Isolate* isolate) {
  o.SeqStringVerify(isolate);
  CHECK(o.IsSeqTwoByteString());
}
void TorqueGeneratedClassVerifiers::SlicedStringVerify(SlicedString o, Isolate* isolate) {
  o.StringVerify(isolate);
  CHECK(o.IsSlicedString());
  {
    Object parent__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, parent__value);
    CHECK(parent__value.IsString());
  }
  {
    Object offset__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, offset__value);
    CHECK(offset__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::ThinStringVerify(ThinString o, Isolate* isolate) {
  o.StringVerify(isolate);
  CHECK(o.IsThinString());
  {
    Object actual__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, actual__value);
    CHECK(actual__value.IsString());
  }
}
void TorqueGeneratedClassVerifiers::Tuple2Verify(Tuple2 o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsTuple2());
  {
    Object value1__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, value1__value);
  }
  {
    Object value2__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, value2__value);
  }
}
void TorqueGeneratedClassVerifiers::SyntheticModuleVerify(SyntheticModule o, Isolate* isolate) {
  o.ModuleVerify(isolate);
  CHECK(o.IsSyntheticModule());
  {
    Object name__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, name__value);
    CHECK(name__value.IsString());
  }
  {
    Object export_names__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, export_names__value);
    CHECK(export_names__value.IsFixedArray());
  }
  {
    Object evaluation_steps__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, evaluation_steps__value);
    CHECK(evaluation_steps__value.IsForeign());
  }
}
void TorqueGeneratedClassVerifiers::CachedTemplateObjectVerify(CachedTemplateObject o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsCachedTemplateObject());
  {
    Object slot_id__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, slot_id__value);
    CHECK(slot_id__value.IsSmi());
  }
  {
    Object template_object__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, template_object__value);
    CHECK(template_object__value.IsJSArray());
  }
  {
    Object next__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, next__value);
    CHECK(next__value.IsTheHole() || next__value.IsCachedTemplateObject());
  }
}
void TorqueGeneratedClassVerifiers::TemplateObjectDescriptionVerify(TemplateObjectDescription o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsTemplateObjectDescription());
  {
    Object raw_strings__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, raw_strings__value);
    CHECK(raw_strings__value.IsFixedArray());
  }
  {
    Object cooked_strings__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, cooked_strings__value);
    CHECK(cooked_strings__value.IsFixedArray());
  }
}
void TorqueGeneratedClassVerifiers::TemplateInfoVerify(TemplateInfo o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsTemplateInfo());
  {
    Object tag__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, tag__value);
    CHECK(tag__value.IsSmi());
  }
  {
    Object serial_number__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, serial_number__value);
    CHECK(serial_number__value.IsSmi());
  }
  {
    Object number_of_properties__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, number_of_properties__value);
    CHECK(number_of_properties__value.IsSmi());
  }
  {
    Object property_list__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, property_list__value);
    CHECK(property_list__value.IsUndefined() || property_list__value.IsTemplateList());
  }
  {
    Object property_accessors__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, property_accessors__value);
    CHECK(property_accessors__value.IsUndefined() || property_accessors__value.IsTemplateList());
  }
}
void TorqueGeneratedClassVerifiers::FunctionTemplateRareDataVerify(FunctionTemplateRareData o, Isolate* isolate) {
  o.StructVerify(isolate);
  CHECK(o.IsFunctionTemplateRareData());
  {
    Object prototype_template__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, prototype_template__value);
    CHECK(prototype_template__value.IsUndefined() || prototype_template__value.IsObjectTemplateInfo());
  }
  {
    Object prototype_provider_template__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, prototype_provider_template__value);
    CHECK(prototype_provider_template__value.IsUndefined() || prototype_provider_template__value.IsFunctionTemplateInfo());
  }
  {
    Object parent_template__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, parent_template__value);
    CHECK(parent_template__value.IsUndefined() || parent_template__value.IsFunctionTemplateInfo());
  }
  {
    Object named_property_handler__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, named_property_handler__value);
    CHECK(named_property_handler__value.IsUndefined() || named_property_handler__value.IsInterceptorInfo());
  }
  {
    Object indexed_property_handler__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, indexed_property_handler__value);
    CHECK(indexed_property_handler__value.IsUndefined() || indexed_property_handler__value.IsInterceptorInfo());
  }
  {
    Object instance_template__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, instance_template__value);
    CHECK(instance_template__value.IsUndefined() || instance_template__value.IsObjectTemplateInfo());
  }
  {
    Object instance_call_handler__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, instance_call_handler__value);
    CHECK(instance_call_handler__value.IsUndefined() || instance_call_handler__value.IsCallHandlerInfo());
  }
  {
    Object access_check_info__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, access_check_info__value);
    CHECK(access_check_info__value.IsUndefined() || access_check_info__value.IsAccessCheckInfo());
  }
  {
    Object c_function_overloads__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, c_function_overloads__value);
    CHECK(c_function_overloads__value.IsFixedArray());
  }
}
void TorqueGeneratedClassVerifiers::FunctionTemplateInfoVerify(FunctionTemplateInfo o, Isolate* isolate) {
  o.TemplateInfoVerify(isolate);
  CHECK(o.IsFunctionTemplateInfo());
  {
    Object call_code__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, call_code__value);
    CHECK(call_code__value.IsUndefined() || call_code__value.IsCallHandlerInfo());
  }
  {
    Object class_name__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, class_name__value);
    CHECK(class_name__value.IsString() || class_name__value.IsUndefined());
  }
  {
    Object signature__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, signature__value);
    CHECK(signature__value.IsUndefined() || signature__value.IsFunctionTemplateInfo());
  }
  {
    Object rare_data__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, rare_data__value);
    CHECK(rare_data__value.IsUndefined() || rare_data__value.IsFunctionTemplateRareData());
  }
  {
    Object shared_function_info__value = TaggedField<Object>::load(o, 40);
    Object::VerifyPointer(isolate, shared_function_info__value);
    CHECK(shared_function_info__value.IsUndefined() || shared_function_info__value.IsSharedFunctionInfo());
  }
  {
    Object flag__value = TaggedField<Object>::load(o, 44);
    Object::VerifyPointer(isolate, flag__value);
    CHECK(flag__value.IsSmi());
  }
  {
    Object length__value = TaggedField<Object>::load(o, 48);
    Object::VerifyPointer(isolate, length__value);
    CHECK(length__value.IsSmi());
  }
  {
    Object cached_property_name__value = TaggedField<Object>::load(o, 52);
    Object::VerifyPointer(isolate, cached_property_name__value);
  }
  {
    Object instance_type__value = TaggedField<Object>::load(o, 56);
    Object::VerifyPointer(isolate, instance_type__value);
    CHECK(instance_type__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::ObjectTemplateInfoVerify(ObjectTemplateInfo o, Isolate* isolate) {
  o.TemplateInfoVerify(isolate);
  CHECK(o.IsObjectTemplateInfo());
  {
    Object constructor__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, constructor__value);
    CHECK(constructor__value.IsUndefined() || constructor__value.IsFunctionTemplateInfo());
  }
  {
    Object data__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, data__value);
    CHECK(data__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::InternalClassVerify(InternalClass o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsInternalClass());
  {
    Object a__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, a__value);
    CHECK(a__value.IsSmi());
  }
  {
    Object b__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, b__value);
    CHECK(b__value.IsSmi() || b__value.IsHeapNumber());
  }
}
void TorqueGeneratedClassVerifiers::SmiPairVerify(SmiPair o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsSmiPair());
  {
    Object a__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, a__value);
    CHECK(a__value.IsSmi());
  }
  {
    Object b__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, b__value);
    CHECK(b__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::SmiBoxVerify(SmiBox o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsSmiBox());
  {
    Object value__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, value__value);
    CHECK(value__value.IsSmi());
  }
  {
    Object unrelated__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, unrelated__value);
    CHECK(unrelated__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::ExportedSubClassBaseVerify(ExportedSubClassBase o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsExportedSubClassBase());
  {
    Object a__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, a__value);
    CHECK(a__value.IsHeapObject());
  }
  {
    Object b__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, b__value);
    CHECK(b__value.IsHeapObject());
  }
}
void TorqueGeneratedClassVerifiers::ExportedSubClassVerify(ExportedSubClass o, Isolate* isolate) {
  o.ExportedSubClassBaseVerify(isolate);
  CHECK(o.IsExportedSubClass());
  {
    Object e_field__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, e_field__value);
    CHECK(e_field__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::AbstractInternalClassVerify(AbstractInternalClass o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsAbstractInternalClass());
}
void TorqueGeneratedClassVerifiers::AbstractInternalClassSubclass1Verify(AbstractInternalClassSubclass1 o, Isolate* isolate) {
  o.AbstractInternalClassVerify(isolate);
  CHECK(o.IsAbstractInternalClassSubclass1());
}
void TorqueGeneratedClassVerifiers::AbstractInternalClassSubclass2Verify(AbstractInternalClassSubclass2 o, Isolate* isolate) {
  o.AbstractInternalClassVerify(isolate);
  CHECK(o.IsAbstractInternalClassSubclass2());
}
void TorqueGeneratedClassVerifiers::InternalClassWithSmiElementsVerify(InternalClassWithSmiElements o, Isolate* isolate) {
  o.FixedArrayBaseVerify(isolate);
  CHECK(o.IsInternalClassWithSmiElements());
  {
    Object data__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, data__value);
    CHECK(data__value.IsSmi());
  }
  {
    Object object__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, object__value);
    CHECK(object__value.IsOddball());
  }
  intptr_t entries__offset, entries__length;
  std::tie(std::ignore, entries__offset, entries__length) = TqRuntimeFieldSliceInternalClassWithSmiElementsEntries(o);
  CHECK_EQ(entries__offset, static_cast<int>(entries__offset));
  CHECK_EQ(entries__length, static_cast<int>(entries__length));
  for (int i = 0; i < static_cast<int>(entries__length); ++i) {
    Object entries__value = TaggedField<Object>::load(o, static_cast<int>(entries__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, entries__value);
    CHECK(entries__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::InternalClassWithStructElementsVerify(InternalClassWithStructElements o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsInternalClassWithStructElements());
  {
    Object count__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, count__value);
    CHECK(count__value.IsSmi());
  }
  {
    Object data__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, data__value);
    CHECK(data__value.IsSmi());
  }
  {
    Object object__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, object__value);
  }
  intptr_t entries__offset, entries__length;
  std::tie(std::ignore, entries__offset, entries__length) = TqRuntimeFieldSliceInternalClassWithStructElementsEntries(o);
  CHECK_EQ(entries__offset, static_cast<int>(entries__offset));
  CHECK_EQ(entries__length, static_cast<int>(entries__length));
  for (int i = 0; i < static_cast<int>(entries__length); ++i) {
    Object entries__value = TaggedField<Object>::load(o, static_cast<int>(entries__offset) + i * kTaggedSize);
    Object::VerifyPointer(isolate, entries__value);
    CHECK(entries__value.IsSmi());
  }
  intptr_t more_entries__offset, more_entries__length;
  std::tie(std::ignore, more_entries__offset, more_entries__length) = TqRuntimeFieldSliceInternalClassWithStructElementsMoreEntries(o);
  CHECK_EQ(more_entries__offset, static_cast<int>(more_entries__offset));
  CHECK_EQ(more_entries__length, static_cast<int>(more_entries__length));
  for (int i = 0; i < static_cast<int>(more_entries__length); ++i) {
    Object a__value = TaggedField<Object>::load(o, static_cast<int>(more_entries__offset) + 0 + i * 8);
    Object::VerifyPointer(isolate, a__value);
    CHECK(a__value.IsSmi());
    Object b__value = TaggedField<Object>::load(o, static_cast<int>(more_entries__offset) + 4 + i * 8);
    Object::VerifyPointer(isolate, b__value);
    CHECK(b__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::ExportedSubClass2Verify(ExportedSubClass2 o, Isolate* isolate) {
  o.ExportedSubClassBaseVerify(isolate);
  CHECK(o.IsExportedSubClass2());
  {
    Object z_field__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, z_field__value);
    CHECK(z_field__value.IsSmi());
  }
}
void TorqueGeneratedClassVerifiers::SortStateVerify(SortState o, Isolate* isolate) {
  HeapObjectVerify(o, isolate);
  CHECK(o.IsSortState());
  {
    Object receiver__value = TaggedField<Object>::load(o, 4);
    Object::VerifyPointer(isolate, receiver__value);
    CHECK(receiver__value.IsJSReceiver());
  }
  {
    Object initialReceiverMap__value = TaggedField<Object>::load(o, 8);
    Object::VerifyPointer(isolate, initialReceiverMap__value);
    CHECK(initialReceiverMap__value.IsMap());
  }
  {
    Object initialReceiverLength__value = TaggedField<Object>::load(o, 12);
    Object::VerifyPointer(isolate, initialReceiverLength__value);
    CHECK(initialReceiverLength__value.IsSmi() || initialReceiverLength__value.IsHeapNumber());
  }
  {
    Object userCmpFn__value = TaggedField<Object>::load(o, 16);
    Object::VerifyPointer(isolate, userCmpFn__value);
    CHECK(userCmpFn__value.IsUndefined() || userCmpFn__value.IsCallableApiObject() || userCmpFn__value.IsCallableJSProxy() || userCmpFn__value.IsJSFunction() || userCmpFn__value.IsJSBoundFunction());
  }
  {
    Object sortComparePtr__value = TaggedField<Object>::load(o, 20);
    Object::VerifyPointer(isolate, sortComparePtr__value);
    CHECK(sortComparePtr__value.IsSmi());
  }
  {
    Object loadFn__value = TaggedField<Object>::load(o, 24);
    Object::VerifyPointer(isolate, loadFn__value);
    CHECK(loadFn__value.IsSmi());
  }
  {
    Object storeFn__value = TaggedField<Object>::load(o, 28);
    Object::VerifyPointer(isolate, storeFn__value);
    CHECK(storeFn__value.IsSmi());
  }
  {
    Object deleteFn__value = TaggedField<Object>::load(o, 32);
    Object::VerifyPointer(isolate, deleteFn__value);
    CHECK(deleteFn__value.IsSmi());
  }
  {
    Object canUseSameAccessorFn__value = TaggedField<Object>::load(o, 36);
    Object::VerifyPointer(isolate, canUseSameAccessorFn__value);
    CHECK(canUseSameAccessorFn__value.IsSmi());
  }
  {
    Object minGallop__value = TaggedField<Object>::load(o, 40);
    Object::VerifyPointer(isolate, minGallop__value);
    CHECK(minGallop__value.IsSmi());
  }
  {
    Object pendingRunsSize__value = TaggedField<Object>::load(o, 44);
    Object::VerifyPointer(isolate, pendingRunsSize__value);
    CHECK(pendingRunsSize__value.IsSmi());
  }
  {
    Object pendingRuns__value = TaggedField<Object>::load(o, 48);
    Object::VerifyPointer(isolate, pendingRuns__value);
    CHECK(pendingRuns__value.IsFixedArray());
  }
  {
    Object workArray__value = TaggedField<Object>::load(o, 52);
    Object::VerifyPointer(isolate, workArray__value);
    CHECK(workArray__value.IsFixedArray());
  }
  {
    Object tempArray__value = TaggedField<Object>::load(o, 56);
    Object::VerifyPointer(isolate, tempArray__value);
    CHECK(tempArray__value.IsFixedArray());
  }
  {
    Object sortLength__value = TaggedField<Object>::load(o, 60);
    Object::VerifyPointer(isolate, sortLength__value);
    CHECK(sortLength__value.IsSmi());
  }
  {
    Object numberOfUndefined__value = TaggedField<Object>::load(o, 64);
    Object::VerifyPointer(isolate, numberOfUndefined__value);
    CHECK(numberOfUndefined__value.IsSmi());
  }
}
}  // namespace internal
}  // namespace v8

#include "src/objects/object-macros-undef.h"
#endif  // VERIFY_HEAP
