#include "src/heap/factory-base.h"
#include "src/heap/factory-base-inl.h"
#include "src/heap/heap.h"
#include "src/heap/heap-inl.h"
#include "src/execution/isolate.h"
#include "src/objects/all-objects-inl.h"

namespace v8 {
namespace internal {

template <typename Impl>
Handle<SloppyArgumentsElements> TorqueGeneratedFactory<Impl>::NewSloppyArgumentsElements(int length, Handle<Context> context, Handle<FixedArray> arguments, AllocationType allocation_type) {
 int size = TorqueGeneratedSloppyArgumentsElements<SloppyArgumentsElements, FixedArrayBase>::SizeFor(length);
  Map map = factory()->read_only_roots().sloppy_arguments_elements_map();  HeapObject result =
    factory()->AllocateRawWithImmortalMap(size, allocation_type, map);
    WriteBarrierMode write_barrier_mode =
       allocation_type == AllocationType::kYoung
       ? SKIP_WRITE_BARRIER : UPDATE_WRITE_BARRIER;
  Handle<SloppyArgumentsElements> result_handle(SloppyArgumentsElements::cast(result), factory()->isolate());
  result_handle->TorqueGeneratedClass::set_length(length);
  result_handle->TorqueGeneratedClass::set_context(*context, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_arguments(*arguments, write_barrier_mode);
  return result_handle;
}

template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<SloppyArgumentsElements>TorqueGeneratedFactory<Factory>::NewSloppyArgumentsElements(int length, Handle<Context> context, Handle<FixedArray> arguments, AllocationType allocation_type);
template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<SloppyArgumentsElements>TorqueGeneratedFactory<LocalFactory>::NewSloppyArgumentsElements(int length, Handle<Context> context, Handle<FixedArray> arguments, AllocationType allocation_type);


template <typename Impl>
Handle<DescriptorArray> TorqueGeneratedFactory<Impl>::NewDescriptorArray(uint16_t number_of_all_descriptors, uint16_t number_of_descriptors, uint16_t raw_number_of_marked_descriptors, uint16_t filler16_bits, Handle<EnumCache> enum_cache, AllocationType allocation_type) {
 int size = TorqueGeneratedDescriptorArray<DescriptorArray, HeapObject>::SizeFor(number_of_all_descriptors);
  Map map = factory()->read_only_roots().descriptor_array_map();  HeapObject result =
    factory()->AllocateRawWithImmortalMap(size, allocation_type, map);
    WriteBarrierMode write_barrier_mode =
       allocation_type == AllocationType::kYoung
       ? SKIP_WRITE_BARRIER : UPDATE_WRITE_BARRIER;
  Handle<DescriptorArray> result_handle(DescriptorArray::cast(result), factory()->isolate());
  result_handle->TorqueGeneratedClass::set_number_of_all_descriptors(number_of_all_descriptors);
  result_handle->TorqueGeneratedClass::set_number_of_descriptors(number_of_descriptors);
  result_handle->TorqueGeneratedClass::set_raw_number_of_marked_descriptors(raw_number_of_marked_descriptors);
  result_handle->TorqueGeneratedClass::set_filler16_bits(filler16_bits);
  result_handle->TorqueGeneratedClass::set_enum_cache(*enum_cache, write_barrier_mode);
  return result_handle;
}

template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<DescriptorArray>TorqueGeneratedFactory<Factory>::NewDescriptorArray(uint16_t number_of_all_descriptors, uint16_t number_of_descriptors, uint16_t raw_number_of_marked_descriptors, uint16_t filler16_bits, Handle<EnumCache> enum_cache, AllocationType allocation_type);
template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<DescriptorArray>TorqueGeneratedFactory<LocalFactory>::NewDescriptorArray(uint16_t number_of_all_descriptors, uint16_t number_of_descriptors, uint16_t raw_number_of_marked_descriptors, uint16_t filler16_bits, Handle<EnumCache> enum_cache, AllocationType allocation_type);


template <typename Impl>
Handle<UncompiledDataWithoutPreparseData> TorqueGeneratedFactory<Impl>::NewUncompiledDataWithoutPreparseData(Handle<String> inferred_name, int32_t start_position, int32_t end_position, AllocationType allocation_type) {
 int size = TorqueGeneratedUncompiledDataWithoutPreparseData<UncompiledDataWithoutPreparseData, UncompiledData>::SizeFor();
  Map map = factory()->read_only_roots().uncompiled_data_without_preparse_data_map();  HeapObject result =
    factory()->AllocateRawWithImmortalMap(size, allocation_type, map);
    WriteBarrierMode write_barrier_mode =
       allocation_type == AllocationType::kYoung
       ? SKIP_WRITE_BARRIER : UPDATE_WRITE_BARRIER;
  Handle<UncompiledDataWithoutPreparseData> result_handle(UncompiledDataWithoutPreparseData::cast(result), factory()->isolate());
  result_handle->TorqueGeneratedClass::set_inferred_name(*inferred_name, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_start_position(start_position);
  result_handle->TorqueGeneratedClass::set_end_position(end_position);
  return result_handle;
}

template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<UncompiledDataWithoutPreparseData>TorqueGeneratedFactory<Factory>::NewUncompiledDataWithoutPreparseData(Handle<String> inferred_name, int32_t start_position, int32_t end_position, AllocationType allocation_type);
template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<UncompiledDataWithoutPreparseData>TorqueGeneratedFactory<LocalFactory>::NewUncompiledDataWithoutPreparseData(Handle<String> inferred_name, int32_t start_position, int32_t end_position, AllocationType allocation_type);


template <typename Impl>
Handle<UncompiledDataWithPreparseData> TorqueGeneratedFactory<Impl>::NewUncompiledDataWithPreparseData(Handle<String> inferred_name, int32_t start_position, int32_t end_position, Handle<PreparseData> preparse_data, AllocationType allocation_type) {
 int size = TorqueGeneratedUncompiledDataWithPreparseData<UncompiledDataWithPreparseData, UncompiledData>::SizeFor();
  Map map = factory()->read_only_roots().uncompiled_data_with_preparse_data_map();  HeapObject result =
    factory()->AllocateRawWithImmortalMap(size, allocation_type, map);
    WriteBarrierMode write_barrier_mode =
       allocation_type == AllocationType::kYoung
       ? SKIP_WRITE_BARRIER : UPDATE_WRITE_BARRIER;
  Handle<UncompiledDataWithPreparseData> result_handle(UncompiledDataWithPreparseData::cast(result), factory()->isolate());
  result_handle->TorqueGeneratedClass::set_inferred_name(*inferred_name, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_start_position(start_position);
  result_handle->TorqueGeneratedClass::set_end_position(end_position);
  result_handle->TorqueGeneratedClass::set_preparse_data(*preparse_data, write_barrier_mode);
  return result_handle;
}

template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<UncompiledDataWithPreparseData>TorqueGeneratedFactory<Factory>::NewUncompiledDataWithPreparseData(Handle<String> inferred_name, int32_t start_position, int32_t end_position, Handle<PreparseData> preparse_data, AllocationType allocation_type);
template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<UncompiledDataWithPreparseData>TorqueGeneratedFactory<LocalFactory>::NewUncompiledDataWithPreparseData(Handle<String> inferred_name, int32_t start_position, int32_t end_position, Handle<PreparseData> preparse_data, AllocationType allocation_type);


template <typename Impl>
Handle<OnHeapBasicBlockProfilerData> TorqueGeneratedFactory<Impl>::NewOnHeapBasicBlockProfilerData(Handle<ByteArray> block_ids, Handle<ByteArray> counts, Handle<String> name, Handle<String> schedule, Handle<String> code, int hash, AllocationType allocation_type) {
 int size = TorqueGeneratedOnHeapBasicBlockProfilerData<OnHeapBasicBlockProfilerData, HeapObject>::SizeFor();
  Map map = factory()->read_only_roots().on_heap_basic_block_profiler_data_map();  HeapObject result =
    factory()->AllocateRawWithImmortalMap(size, allocation_type, map);
    WriteBarrierMode write_barrier_mode =
       allocation_type == AllocationType::kYoung
       ? SKIP_WRITE_BARRIER : UPDATE_WRITE_BARRIER;
  Handle<OnHeapBasicBlockProfilerData> result_handle(OnHeapBasicBlockProfilerData::cast(result), factory()->isolate());
  result_handle->TorqueGeneratedClass::set_block_ids(*block_ids, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_counts(*counts, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_name(*name, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_schedule(*schedule, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_code(*code, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_hash(hash);
  return result_handle;
}

template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<OnHeapBasicBlockProfilerData>TorqueGeneratedFactory<Factory>::NewOnHeapBasicBlockProfilerData(Handle<ByteArray> block_ids, Handle<ByteArray> counts, Handle<String> name, Handle<String> schedule, Handle<String> code, int hash, AllocationType allocation_type);
template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<OnHeapBasicBlockProfilerData>TorqueGeneratedFactory<LocalFactory>::NewOnHeapBasicBlockProfilerData(Handle<ByteArray> block_ids, Handle<ByteArray> counts, Handle<String> name, Handle<String> schedule, Handle<String> code, int hash, AllocationType allocation_type);


template <typename Impl>
Handle<ExportedSubClassBase> TorqueGeneratedFactory<Impl>::NewExportedSubClassBase(Handle<HeapObject> a, Handle<HeapObject> b, AllocationType allocation_type) {
 int size = TorqueGeneratedExportedSubClassBase<ExportedSubClassBase, HeapObject>::SizeFor();
  Map map = factory()->read_only_roots().exported_sub_class_base_map();  HeapObject result =
    factory()->AllocateRawWithImmortalMap(size, allocation_type, map);
    WriteBarrierMode write_barrier_mode =
       allocation_type == AllocationType::kYoung
       ? SKIP_WRITE_BARRIER : UPDATE_WRITE_BARRIER;
  Handle<ExportedSubClassBase> result_handle(ExportedSubClassBase::cast(result), factory()->isolate());
  result_handle->TorqueGeneratedClass::set_a(*a, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_b(*b, write_barrier_mode);
  return result_handle;
}

template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<ExportedSubClassBase>TorqueGeneratedFactory<Factory>::NewExportedSubClassBase(Handle<HeapObject> a, Handle<HeapObject> b, AllocationType allocation_type);
template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<ExportedSubClassBase>TorqueGeneratedFactory<LocalFactory>::NewExportedSubClassBase(Handle<HeapObject> a, Handle<HeapObject> b, AllocationType allocation_type);


template <typename Impl>
Handle<ExportedSubClass> TorqueGeneratedFactory<Impl>::NewExportedSubClass(Handle<HeapObject> a, Handle<HeapObject> b, int32_t c_field, int32_t d_field, int e_field, AllocationType allocation_type) {
 int size = TorqueGeneratedExportedSubClass<ExportedSubClass, ExportedSubClassBase>::SizeFor();
  Map map = factory()->read_only_roots().exported_sub_class_map();  HeapObject result =
    factory()->AllocateRawWithImmortalMap(size, allocation_type, map);
    WriteBarrierMode write_barrier_mode =
       allocation_type == AllocationType::kYoung
       ? SKIP_WRITE_BARRIER : UPDATE_WRITE_BARRIER;
  Handle<ExportedSubClass> result_handle(ExportedSubClass::cast(result), factory()->isolate());
  result_handle->TorqueGeneratedClass::set_a(*a, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_b(*b, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_c_field(c_field);
  result_handle->TorqueGeneratedClass::set_d_field(d_field);
  result_handle->TorqueGeneratedClass::set_e_field(e_field);
  return result_handle;
}

template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<ExportedSubClass>TorqueGeneratedFactory<Factory>::NewExportedSubClass(Handle<HeapObject> a, Handle<HeapObject> b, int32_t c_field, int32_t d_field, int e_field, AllocationType allocation_type);
template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<ExportedSubClass>TorqueGeneratedFactory<LocalFactory>::NewExportedSubClass(Handle<HeapObject> a, Handle<HeapObject> b, int32_t c_field, int32_t d_field, int e_field, AllocationType allocation_type);


template <typename Impl>
Handle<ExportedSubClass2> TorqueGeneratedFactory<Impl>::NewExportedSubClass2(Handle<HeapObject> a, Handle<HeapObject> b, int32_t x_field, int32_t y_field, int z_field, AllocationType allocation_type) {
 int size = TorqueGeneratedExportedSubClass2<ExportedSubClass2, ExportedSubClassBase>::SizeFor();
  Map map = factory()->read_only_roots().exported_sub_class2_map();  HeapObject result =
    factory()->AllocateRawWithImmortalMap(size, allocation_type, map);
    WriteBarrierMode write_barrier_mode =
       allocation_type == AllocationType::kYoung
       ? SKIP_WRITE_BARRIER : UPDATE_WRITE_BARRIER;
  Handle<ExportedSubClass2> result_handle(ExportedSubClass2::cast(result), factory()->isolate());
  result_handle->TorqueGeneratedClass::set_a(*a, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_b(*b, write_barrier_mode);
  result_handle->TorqueGeneratedClass::set_x_field(x_field);
  result_handle->TorqueGeneratedClass::set_y_field(y_field);
  result_handle->TorqueGeneratedClass::set_z_field(z_field);
  return result_handle;
}

template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<ExportedSubClass2>TorqueGeneratedFactory<Factory>::NewExportedSubClass2(Handle<HeapObject> a, Handle<HeapObject> b, int32_t x_field, int32_t y_field, int z_field, AllocationType allocation_type);
template EXPORT_TEMPLATE_DEFINE(V8_EXPORT_PRIVATE) Handle<ExportedSubClass2>TorqueGeneratedFactory<LocalFactory>::NewExportedSubClass2(Handle<HeapObject> a, Handle<HeapObject> b, int32_t x_field, int32_t y_field, int z_field, AllocationType allocation_type);


}  // namespace internal
}  // namespace v8
