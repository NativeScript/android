#ifdef OBJECT_PRINT
#include <iosfwd>

#include "src/objects/all-objects-inl.h"

namespace v8 {
namespace internal {
template <>
void TorqueGeneratedContext<Context, HeapObject>::ContextPrint(std::ostream& os) {
  this->PrintHeader(os, "Context");
  os << "\n - length: " << this->length();
  os << '\n';
}

template <>
void TorqueGeneratedOddball<Oddball, PrimitiveHeapObject>::OddballPrint(std::ostream& os) {
  this->PrintHeader(os, "Oddball");
  os << "\n - to_number_raw: " << this->to_number_raw();
  os << "\n - to_string: " << Brief(this->to_string());
  os << "\n - to_number: " << Brief(this->to_number());
  os << "\n - type_of: " << Brief(this->type_of());
  os << "\n - kind: " << this->kind();
  os << '\n';
}

template <>
void TorqueGeneratedStruct<Struct, HeapObject>::StructPrint(std::ostream& os) {
  this->PrintHeader(os, "Struct");
  os << '\n';
}

template <>
void TorqueGeneratedInterceptorInfo<InterceptorInfo, Struct>::InterceptorInfoPrint(std::ostream& os) {
  this->PrintHeader(os, "InterceptorInfo");
  os << "\n - getter: " << Brief(this->getter());
  os << "\n - setter: " << Brief(this->setter());
  os << "\n - query: " << Brief(this->query());
  os << "\n - descriptor: " << Brief(this->descriptor());
  os << "\n - deleter: " << Brief(this->deleter());
  os << "\n - enumerator: " << Brief(this->enumerator());
  os << "\n - definer: " << Brief(this->definer());
  os << "\n - data: " << Brief(this->data());
  os << "\n - flags: " << this->flags();
  os << '\n';
}

template <>
void TorqueGeneratedAccessCheckInfo<AccessCheckInfo, Struct>::AccessCheckInfoPrint(std::ostream& os) {
  this->PrintHeader(os, "AccessCheckInfo");
  os << "\n - callback: " << Brief(this->callback());
  os << "\n - named_interceptor: " << Brief(this->named_interceptor());
  os << "\n - indexed_interceptor: " << Brief(this->indexed_interceptor());
  os << "\n - data: " << Brief(this->data());
  os << '\n';
}

template <>
void TorqueGeneratedAccessorInfo<AccessorInfo, Struct>::AccessorInfoPrint(std::ostream& os) {
  this->PrintHeader(os, "AccessorInfo");
  os << "\n - name: " << Brief(this->name());
  os << "\n - flags: " << this->flags();
  os << "\n - expected_receiver_type: " << Brief(this->expected_receiver_type());
  os << "\n - setter: " << Brief(this->setter());
  os << "\n - getter: " << Brief(this->getter());
  os << "\n - js_getter: " << Brief(this->js_getter());
  os << "\n - data: " << Brief(this->data());
  os << '\n';
}

template <>
void TorqueGeneratedSloppyArgumentsElements<SloppyArgumentsElements, FixedArrayBase>::SloppyArgumentsElementsPrint(std::ostream& os) {
  this->PrintHeader(os, "SloppyArgumentsElements");
  os << "\n - length: " << this->length();
  os << "\n - context: " << Brief(this->context());
  os << "\n - arguments: " << Brief(this->arguments());
  os << '\n';
}

template <>
void TorqueGeneratedAliasedArgumentsEntry<AliasedArgumentsEntry, Struct>::AliasedArgumentsEntryPrint(std::ostream& os) {
  this->PrintHeader(os, "AliasedArgumentsEntry");
  os << "\n - aliased_context_slot: " << this->aliased_context_slot();
  os << '\n';
}

template <>
void TorqueGeneratedCell<Cell, HeapObject>::CellPrint(std::ostream& os) {
  this->PrintHeader(os, "Cell");
  os << "\n - value: " << Brief(this->value());
  os << '\n';
}

template <>
void TorqueGeneratedBreakPoint<BreakPoint, Struct>::BreakPointPrint(std::ostream& os) {
  this->PrintHeader(os, "BreakPoint");
  os << "\n - id: " << this->id();
  os << "\n - condition: " << Brief(this->condition());
  os << '\n';
}

template <>
void TorqueGeneratedBreakPointInfo<BreakPointInfo, Struct>::BreakPointInfoPrint(std::ostream& os) {
  this->PrintHeader(os, "BreakPointInfo");
  os << "\n - source_position: " << this->source_position();
  os << "\n - break_points: " << Brief(this->break_points());
  os << '\n';
}

template <>
void TorqueGeneratedDebugInfo<DebugInfo, Struct>::DebugInfoPrint(std::ostream& os) {
  this->PrintHeader(os, "DebugInfo");
  os << "\n - shared: " << Brief(this->shared());
  os << "\n - debugger_hints: " << this->debugger_hints();
  os << "\n - script: " << Brief(this->script());
  os << "\n - original_bytecode_array: " << Brief(this->original_bytecode_array(kAcquireLoad));
  os << "\n - debug_bytecode_array: " << Brief(this->debug_bytecode_array(kAcquireLoad));
  os << "\n - break_points: " << Brief(this->break_points());
  os << "\n - flags: " << this->flags(kRelaxedLoad);
  os << "\n - coverage_info: " << Brief(this->coverage_info());
  os << '\n';
}

template <>
void TorqueGeneratedEnumCache<EnumCache, Struct>::EnumCachePrint(std::ostream& os) {
  this->PrintHeader(os, "EnumCache");
  os << "\n - keys: " << Brief(this->keys());
  os << "\n - indices: " << Brief(this->indices());
  os << '\n';
}

template <>
void TorqueGeneratedDescriptorArray<DescriptorArray, HeapObject>::DescriptorArrayPrint(std::ostream& os) {
  this->PrintHeader(os, "DescriptorArray");
  os << "\n - number_of_all_descriptors: " << this->number_of_all_descriptors();
  os << "\n - number_of_descriptors: " << this->number_of_descriptors();
  os << "\n - raw_number_of_marked_descriptors: " << this->raw_number_of_marked_descriptors();
  os << "\n - filler16_bits: " << this->filler16_bits();
  os << "\n - enum_cache: " << Brief(this->enum_cache());
  os << '\n';
}

template <>
void TorqueGeneratedStrongDescriptorArray<StrongDescriptorArray, DescriptorArray>::StrongDescriptorArrayPrint(std::ostream& os) {
  this->PrintHeader(os, "StrongDescriptorArray");
  os << "\n - number_of_all_descriptors: " << this->number_of_all_descriptors();
  os << "\n - number_of_descriptors: " << this->number_of_descriptors();
  os << "\n - raw_number_of_marked_descriptors: " << this->raw_number_of_marked_descriptors();
  os << "\n - filler16_bits: " << this->filler16_bits();
  os << "\n - enum_cache: " << Brief(this->enum_cache());
  os << '\n';
}

template <>
void TorqueGeneratedMegaDomHandler<MegaDomHandler, HeapObject>::MegaDomHandlerPrint(std::ostream& os) {
  this->PrintHeader(os, "MegaDomHandler");
  os << "\n - accessor: " << Brief(this->accessor());
  os << "\n - context: " << Brief(this->context());
  os << '\n';
}

template <>
void TorqueGeneratedPropertyDescriptorObject<PropertyDescriptorObject, Struct>::PropertyDescriptorObjectPrint(std::ostream& os) {
  this->PrintHeader(os, "PropertyDescriptorObject");
  os << "\n - flags: " << this->flags();
  os << "\n - value: " << Brief(this->value());
  os << "\n - get: " << Brief(this->get());
  os << "\n - set: " << Brief(this->set());
  os << '\n';
}

template <>
void TorqueGeneratedBaselineData<BaselineData, Struct>::BaselineDataPrint(std::ostream& os) {
  this->PrintHeader(os, "BaselineData");
  os << "\n - baseline_code: " << Brief(this->baseline_code());
  os << "\n - data: " << Brief(this->data());
  os << '\n';
}

template <>
void TorqueGeneratedSharedFunctionInfo<SharedFunctionInfo, HeapObject>::SharedFunctionInfoPrint(std::ostream& os) {
  this->PrintHeader(os, "SharedFunctionInfo");
  os << "\n - function_data: " << Brief(this->function_data());
  os << "\n - name_or_scope_info: " << Brief(this->name_or_scope_info());
  os << "\n - outer_scope_info_or_feedback_metadata: " << Brief(this->outer_scope_info_or_feedback_metadata());
  os << "\n - script_or_debug_info: " << Brief(this->script_or_debug_info());
  os << "\n - length: " << this->length();
  os << "\n - formal_parameter_count: " << this->formal_parameter_count();
  os << "\n - function_token_offset: " << this->function_token_offset();
  os << "\n - expected_nof_properties: " << this->expected_nof_properties();
  os << "\n - flags2: " << this->flags2();
  os << "\n - flags: " << this->flags();
  os << "\n - function_literal_id: " << this->function_literal_id();
  os << '\n';
}

template <>
void TorqueGeneratedUncompiledData<UncompiledData, HeapObject>::UncompiledDataPrint(std::ostream& os) {
  this->PrintHeader(os, "UncompiledData");
  os << "\n - inferred_name: " << Brief(this->inferred_name());
  os << "\n - start_position: " << this->start_position();
  os << "\n - end_position: " << this->end_position();
  os << '\n';
}

template <>
void TorqueGeneratedUncompiledDataWithoutPreparseData<UncompiledDataWithoutPreparseData, UncompiledData>::UncompiledDataWithoutPreparseDataPrint(std::ostream& os) {
  this->PrintHeader(os, "UncompiledDataWithoutPreparseData");
  os << "\n - inferred_name: " << Brief(this->inferred_name());
  os << "\n - start_position: " << this->start_position();
  os << "\n - end_position: " << this->end_position();
  os << '\n';
}

template <>
void TorqueGeneratedUncompiledDataWithPreparseData<UncompiledDataWithPreparseData, UncompiledData>::UncompiledDataWithPreparseDataPrint(std::ostream& os) {
  this->PrintHeader(os, "UncompiledDataWithPreparseData");
  os << "\n - inferred_name: " << Brief(this->inferred_name());
  os << "\n - start_position: " << this->start_position();
  os << "\n - end_position: " << this->end_position();
  os << "\n - preparse_data: " << Brief(this->preparse_data());
  os << '\n';
}

template <>
void TorqueGeneratedOnHeapBasicBlockProfilerData<OnHeapBasicBlockProfilerData, HeapObject>::OnHeapBasicBlockProfilerDataPrint(std::ostream& os) {
  this->PrintHeader(os, "OnHeapBasicBlockProfilerData");
  os << "\n - block_ids: " << Brief(this->block_ids());
  os << "\n - counts: " << Brief(this->counts());
  os << "\n - name: " << Brief(this->name());
  os << "\n - schedule: " << Brief(this->schedule());
  os << "\n - code: " << Brief(this->code());
  os << "\n - hash: " << this->hash();
  os << '\n';
}

template <>
void TorqueGeneratedModuleRequest<ModuleRequest, Struct>::ModuleRequestPrint(std::ostream& os) {
  this->PrintHeader(os, "ModuleRequest");
  os << "\n - specifier: " << Brief(this->specifier());
  os << "\n - import_assertions: " << Brief(this->import_assertions());
  os << "\n - position: " << this->position();
  os << '\n';
}

template <>
void TorqueGeneratedTuple2<Tuple2, Struct>::Tuple2Print(std::ostream& os) {
  this->PrintHeader(os, "Tuple2");
  os << "\n - value1: " << Brief(this->value1());
  os << "\n - value2: " << Brief(this->value2());
  os << '\n';
}

template <>
void TorqueGeneratedCachedTemplateObject<CachedTemplateObject, Struct>::CachedTemplateObjectPrint(std::ostream& os) {
  this->PrintHeader(os, "CachedTemplateObject");
  os << "\n - slot_id: " << this->slot_id();
  os << "\n - template_object: " << Brief(this->template_object());
  os << "\n - next: " << Brief(this->next());
  os << '\n';
}

template <>
void TorqueGeneratedTemplateObjectDescription<TemplateObjectDescription, Struct>::TemplateObjectDescriptionPrint(std::ostream& os) {
  this->PrintHeader(os, "TemplateObjectDescription");
  os << "\n - raw_strings: " << Brief(this->raw_strings());
  os << "\n - cooked_strings: " << Brief(this->cooked_strings());
  os << '\n';
}

template <>
void TorqueGeneratedFunctionTemplateRareData<FunctionTemplateRareData, Struct>::FunctionTemplateRareDataPrint(std::ostream& os) {
  this->PrintHeader(os, "FunctionTemplateRareData");
  os << "\n - prototype_template: " << Brief(this->prototype_template());
  os << "\n - prototype_provider_template: " << Brief(this->prototype_provider_template());
  os << "\n - parent_template: " << Brief(this->parent_template());
  os << "\n - named_property_handler: " << Brief(this->named_property_handler());
  os << "\n - indexed_property_handler: " << Brief(this->indexed_property_handler());
  os << "\n - instance_template: " << Brief(this->instance_template());
  os << "\n - instance_call_handler: " << Brief(this->instance_call_handler());
  os << "\n - access_check_info: " << Brief(this->access_check_info());
  os << "\n - c_function_overloads: " << Brief(this->c_function_overloads());
  os << '\n';
}

template <>
void TorqueGeneratedInternalClass<InternalClass, HeapObject>::InternalClassPrint(std::ostream& os) {
  this->PrintHeader(os, "InternalClass");
  os << "\n - a: " << this->a();
  os << "\n - b: " << Brief(this->b());
  os << '\n';
}

template <>
void TorqueGeneratedSmiPair<SmiPair, HeapObject>::SmiPairPrint(std::ostream& os) {
  this->PrintHeader(os, "SmiPair");
  os << "\n - a: " << this->a();
  os << "\n - b: " << this->b();
  os << '\n';
}

template <>
void TorqueGeneratedSmiBox<SmiBox, HeapObject>::SmiBoxPrint(std::ostream& os) {
  this->PrintHeader(os, "SmiBox");
  os << "\n - value: " << this->value();
  os << "\n - unrelated: " << this->unrelated();
  os << '\n';
}

template <>
void TorqueGeneratedExportedSubClassBase<ExportedSubClassBase, HeapObject>::ExportedSubClassBasePrint(std::ostream& os) {
  this->PrintHeader(os, "ExportedSubClassBase");
  os << "\n - a: " << Brief(this->a());
  os << "\n - b: " << Brief(this->b());
  os << '\n';
}

template <>
void TorqueGeneratedExportedSubClass<ExportedSubClass, ExportedSubClassBase>::ExportedSubClassPrint(std::ostream& os) {
  this->PrintHeader(os, "ExportedSubClass");
  os << "\n - a: " << Brief(this->a());
  os << "\n - b: " << Brief(this->b());
  os << "\n - c_field: " << this->c_field();
  os << "\n - d_field: " << this->d_field();
  os << "\n - e_field: " << this->e_field();
  os << '\n';
}

template <>
void TorqueGeneratedAbstractInternalClass<AbstractInternalClass, HeapObject>::AbstractInternalClassPrint(std::ostream& os) {
  this->PrintHeader(os, "AbstractInternalClass");
  os << '\n';
}

template <>
void TorqueGeneratedAbstractInternalClassSubclass1<AbstractInternalClassSubclass1, AbstractInternalClass>::AbstractInternalClassSubclass1Print(std::ostream& os) {
  this->PrintHeader(os, "AbstractInternalClassSubclass1");
  os << '\n';
}

template <>
void TorqueGeneratedAbstractInternalClassSubclass2<AbstractInternalClassSubclass2, AbstractInternalClass>::AbstractInternalClassSubclass2Print(std::ostream& os) {
  this->PrintHeader(os, "AbstractInternalClassSubclass2");
  os << '\n';
}

template <>
void TorqueGeneratedInternalClassWithSmiElements<InternalClassWithSmiElements, FixedArrayBase>::InternalClassWithSmiElementsPrint(std::ostream& os) {
  this->PrintHeader(os, "InternalClassWithSmiElements");
  os << "\n - length: " << this->length();
  os << "\n - data: " << this->data();
  os << "\n - object: " << Brief(this->object());
  os << '\n';
}

template <>
void TorqueGeneratedInternalClassWithStructElements<InternalClassWithStructElements, HeapObject>::InternalClassWithStructElementsPrint(std::ostream& os) {
  this->PrintHeader(os, "InternalClassWithStructElements");
  os << "\n - dummy1: " << this->dummy1();
  os << "\n - dummy2: " << this->dummy2();
  os << "\n - count: " << this->count();
  os << "\n - data: " << this->data();
  os << "\n - object: " << Brief(this->object());
  os << '\n';
}

template <>
void TorqueGeneratedExportedSubClass2<ExportedSubClass2, ExportedSubClassBase>::ExportedSubClass2Print(std::ostream& os) {
  this->PrintHeader(os, "ExportedSubClass2");
  os << "\n - a: " << Brief(this->a());
  os << "\n - b: " << Brief(this->b());
  os << "\n - x_field: " << this->x_field();
  os << "\n - y_field: " << this->y_field();
  os << "\n - z_field: " << this->z_field();
  os << '\n';
}

template <>
void TorqueGeneratedSortState<SortState, HeapObject>::SortStatePrint(std::ostream& os) {
  this->PrintHeader(os, "SortState");
  os << "\n - receiver: " << Brief(this->receiver());
  os << "\n - initialReceiverMap: " << Brief(this->initialReceiverMap());
  os << "\n - initialReceiverLength: " << Brief(this->initialReceiverLength());
  os << "\n - userCmpFn: " << Brief(this->userCmpFn());
  os << "\n - sortComparePtr: " << this->sortComparePtr();
  os << "\n - loadFn: " << this->loadFn();
  os << "\n - storeFn: " << this->storeFn();
  os << "\n - deleteFn: " << this->deleteFn();
  os << "\n - canUseSameAccessorFn: " << this->canUseSameAccessorFn();
  os << "\n - minGallop: " << this->minGallop();
  os << "\n - pendingRunsSize: " << this->pendingRunsSize();
  os << "\n - pendingRuns: " << Brief(this->pendingRuns());
  os << "\n - workArray: " << Brief(this->workArray());
  os << "\n - tempArray: " << Brief(this->tempArray());
  os << "\n - sortLength: " << this->sortLength();
  os << "\n - numberOfUndefined: " << this->numberOfUndefined();
  os << '\n';
}

}  // namespace internal
}  // namespace v8
#endif  // OBJECT_PRINT
