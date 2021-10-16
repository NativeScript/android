inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashSetPadding(SmallOrderedHashSet p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashSetDataTable(SmallOrderedHashSet p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashSetHashTable(SmallOrderedHashSet p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashSetChainTable(SmallOrderedHashSet p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashMapPadding(SmallOrderedHashMap p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashMapDataTable(SmallOrderedHashMap p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashMapHashTable(SmallOrderedHashMap p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashMapChainTable(SmallOrderedHashMap p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedNameDictionaryPadding1(SmallOrderedNameDictionary p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedNameDictionaryDataTable(SmallOrderedNameDictionary p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedNameDictionaryHashTable(SmallOrderedNameDictionary p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedNameDictionaryChainTable(SmallOrderedNameDictionary p_o);
inline intptr_t TqRuntimeConvert_intptr_constexpr_int31_0(int31_t p_i);
inline intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int31_0(int31_t p_i);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_uint8_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline intptr_t TqRuntimeConvert_intptr_uint8_0(uint8_t p_ui);
inline intptr_t TqRuntimeConvert_intptr_intptr_0(intptr_t p_i);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_HashMapEntry_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_NameDictionaryEntry_0(Object p_object, intptr_t p_offset, intptr_t p_length);
#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetPadding
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetPadding
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashSetPadding(SmallOrderedHashSet p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  Object tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  goto block0;

  block0:
  tmp0 = TqRuntimeConvert_intptr_constexpr_int31_0(1);
  tmp1 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(7);
  std::tie(tmp2, tmp3, tmp4) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp1, tmp0);
  goto block2;

  block2:
  return std::make_tuple(tmp2, tmp3, tmp4);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetPadding

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetDataTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetDataTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashSetDataTable(SmallOrderedHashSet p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  uint8_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Object tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  goto block0;

  block0:
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(6);
  tmp1 = (p_o).ReadField<uint8_t>(tmp0);
  tmp2 = TqRuntimeConvert_intptr_uint8_0(tmp1);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(SmallOrderedHashTable<int>::kLoadFactor);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp2, tmp3);
  tmp5 = TqRuntimeConvert_intptr_intptr_0(tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  std::tie(tmp7, tmp8, tmp9) = TqRuntimeNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(p_o, tmp6, tmp5);
  goto block2;

  block2:
  return std::make_tuple(tmp7, tmp8, tmp9);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetDataTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetHashTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetHashTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashSetHashTable(SmallOrderedHashSet p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  Object tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceSmallOrderedHashSetDataTable(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(6);
  tmp7 = (p_o).ReadField<uint8_t>(tmp6);
  tmp8 = TqRuntimeConvert_intptr_uint8_0(tmp7);
  std::tie(tmp9, tmp10, tmp11) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp5, tmp8);
  goto block2;

  block2:
  return std::make_tuple(tmp9, tmp10, tmp11);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetHashTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetChainTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetChainTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashSetChainTable(SmallOrderedHashSet p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  Object tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceSmallOrderedHashSetHashTable(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(1);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(6);
  tmp7 = (p_o).ReadField<uint8_t>(tmp6);
  tmp8 = TqRuntimeConvert_intptr_uint8_0(tmp7);
  tmp9 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(SmallOrderedHashTable<int>::kLoadFactor);
  tmp10 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp8, tmp9);
  tmp11 = TqRuntimeConvert_intptr_intptr_0(tmp10);
  std::tie(tmp12, tmp13, tmp14) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp5, tmp11);
  goto block2;

  block2:
  return std::make_tuple(tmp12, tmp13, tmp14);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashSetChainTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapPadding
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapPadding
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashMapPadding(SmallOrderedHashMap p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  Object tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  goto block0;

  block0:
  tmp0 = TqRuntimeConvert_intptr_constexpr_int31_0(1);
  tmp1 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(7);
  std::tie(tmp2, tmp3, tmp4) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp1, tmp0);
  goto block2;

  block2:
  return std::make_tuple(tmp2, tmp3, tmp4);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapPadding

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapDataTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapDataTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashMapDataTable(SmallOrderedHashMap p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  uint8_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Object tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  goto block0;

  block0:
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(6);
  tmp1 = (p_o).ReadField<uint8_t>(tmp0);
  tmp2 = TqRuntimeConvert_intptr_uint8_0(tmp1);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(SmallOrderedHashTable<int>::kLoadFactor);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp2, tmp3);
  tmp5 = TqRuntimeConvert_intptr_intptr_0(tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  std::tie(tmp7, tmp8, tmp9) = TqRuntimeNewMutableSlice_HashMapEntry_0(p_o, tmp6, tmp5);
  goto block2;

  block2:
  return std::make_tuple(tmp7, tmp8, tmp9);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapDataTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapHashTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapHashTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashMapHashTable(SmallOrderedHashMap p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  Object tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceSmallOrderedHashMapDataTable(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(6);
  tmp7 = (p_o).ReadField<uint8_t>(tmp6);
  tmp8 = TqRuntimeConvert_intptr_uint8_0(tmp7);
  std::tie(tmp9, tmp10, tmp11) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp5, tmp8);
  goto block2;

  block2:
  return std::make_tuple(tmp9, tmp10, tmp11);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapHashTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapChainTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapChainTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedHashMapChainTable(SmallOrderedHashMap p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  Object tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceSmallOrderedHashMapHashTable(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(1);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(6);
  tmp7 = (p_o).ReadField<uint8_t>(tmp6);
  tmp8 = TqRuntimeConvert_intptr_uint8_0(tmp7);
  tmp9 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(SmallOrderedHashTable<int>::kLoadFactor);
  tmp10 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp8, tmp9);
  tmp11 = TqRuntimeConvert_intptr_intptr_0(tmp10);
  std::tie(tmp12, tmp13, tmp14) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp5, tmp11);
  goto block2;

  block2:
  return std::make_tuple(tmp12, tmp13, tmp14);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedHashMapChainTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryPadding1
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryPadding1
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedNameDictionaryPadding1(SmallOrderedNameDictionary p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  Object tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  goto block0;

  block0:
  tmp0 = TqRuntimeConvert_intptr_constexpr_int31_0(1);
  tmp1 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(11);
  std::tie(tmp2, tmp3, tmp4) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp1, tmp0);
  goto block2;

  block2:
  return std::make_tuple(tmp2, tmp3, tmp4);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryPadding1

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryDataTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryDataTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedNameDictionaryDataTable(SmallOrderedNameDictionary p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  uint8_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Object tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  goto block0;

  block0:
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(10);
  tmp1 = (p_o).ReadField<uint8_t>(tmp0);
  tmp2 = TqRuntimeConvert_intptr_uint8_0(tmp1);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(SmallOrderedHashTable<int>::kLoadFactor);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp2, tmp3);
  tmp5 = TqRuntimeConvert_intptr_intptr_0(tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(12);
  std::tie(tmp7, tmp8, tmp9) = TqRuntimeNewMutableSlice_NameDictionaryEntry_0(p_o, tmp6, tmp5);
  goto block2;

  block2:
  return std::make_tuple(tmp7, tmp8, tmp9);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryDataTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryHashTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryHashTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedNameDictionaryHashTable(SmallOrderedNameDictionary p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  Object tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceSmallOrderedNameDictionaryDataTable(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(12);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(10);
  tmp7 = (p_o).ReadField<uint8_t>(tmp6);
  tmp8 = TqRuntimeConvert_intptr_uint8_0(tmp7);
  std::tie(tmp9, tmp10, tmp11) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp5, tmp8);
  goto block2;

  block2:
  return std::make_tuple(tmp9, tmp10, tmp11);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryHashTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryChainTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryChainTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSmallOrderedNameDictionaryChainTable(SmallOrderedNameDictionary p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  Object tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceSmallOrderedNameDictionaryHashTable(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(1);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(10);
  tmp7 = (p_o).ReadField<uint8_t>(tmp6);
  tmp8 = TqRuntimeConvert_intptr_uint8_0(tmp7);
  std::tie(tmp9, tmp10, tmp11) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp5, tmp8);
  goto block2;

  block2:
  return std::make_tuple(tmp9, tmp10, tmp11);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSmallOrderedNameDictionaryChainTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_constexpr_int31_0
intptr_t TqRuntimeConvert_intptr_constexpr_int31_0(int31_t p_i) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(p_i);
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_intptr_constexpr_int31_0
intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int31_0(int31_t p_i) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_uint8_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_uint8_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_uint8_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return std::make_tuple(tmp0, tmp1, tmp2);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_uint8_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_uint8_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_uint8_0
intptr_t TqRuntimeConvert_intptr_uint8_0(uint8_t p_ui) {
  DisallowGarbageCollection no_gc;
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = TorqueRuntimeMacroShims::CodeStubAssembler::ChangeUint32ToWord(p_ui);
  tmp1 = TorqueRuntimeMacroShims::CodeStubAssembler::Signed(tmp0);
  goto block2;

  block2:
  return tmp1;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_uint8_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_intptr_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_intptr_0
intptr_t TqRuntimeConvert_intptr_intptr_0(intptr_t p_i) {
  DisallowGarbageCollection no_gc;
  goto block0;

  block0:
  goto block2;

  block2:
  return p_i;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_intptr_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return std::make_tuple(tmp0, tmp1, tmp2);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_HashMapEntry_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_HashMapEntry_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_HashMapEntry_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return std::make_tuple(tmp0, tmp1, tmp2);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_HashMapEntry_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_NameDictionaryEntry_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_NameDictionaryEntry_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_NameDictionaryEntry_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return std::make_tuple(tmp0, tmp1, tmp2);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_NameDictionaryEntry_0

