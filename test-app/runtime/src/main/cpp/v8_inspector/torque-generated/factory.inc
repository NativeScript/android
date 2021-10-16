Handle<SloppyArgumentsElements> NewSloppyArgumentsElements(int length, Handle<Context> context, Handle<FixedArray> arguments, AllocationType allocation_type);
Handle<DescriptorArray> NewDescriptorArray(uint16_t number_of_all_descriptors, uint16_t number_of_descriptors, uint16_t raw_number_of_marked_descriptors, uint16_t filler16_bits, Handle<EnumCache> enum_cache, AllocationType allocation_type);
Handle<UncompiledDataWithoutPreparseData> NewUncompiledDataWithoutPreparseData(Handle<String> inferred_name, int32_t start_position, int32_t end_position, AllocationType allocation_type);
Handle<UncompiledDataWithPreparseData> NewUncompiledDataWithPreparseData(Handle<String> inferred_name, int32_t start_position, int32_t end_position, Handle<PreparseData> preparse_data, AllocationType allocation_type);
Handle<OnHeapBasicBlockProfilerData> NewOnHeapBasicBlockProfilerData(Handle<ByteArray> block_ids, Handle<ByteArray> counts, Handle<String> name, Handle<String> schedule, Handle<String> code, int hash, AllocationType allocation_type);
Handle<ExportedSubClassBase> NewExportedSubClassBase(Handle<HeapObject> a, Handle<HeapObject> b, AllocationType allocation_type);
Handle<ExportedSubClass> NewExportedSubClass(Handle<HeapObject> a, Handle<HeapObject> b, int32_t c_field, int32_t d_field, int e_field, AllocationType allocation_type);
Handle<ExportedSubClass2> NewExportedSubClass2(Handle<HeapObject> a, Handle<HeapObject> b, int32_t x_field, int32_t y_field, int z_field, AllocationType allocation_type);
