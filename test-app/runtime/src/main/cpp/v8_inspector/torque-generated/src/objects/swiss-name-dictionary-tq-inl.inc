inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSwissNameDictionaryDataTable(SwissNameDictionary p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSwissNameDictionaryCtrlTable(SwissNameDictionary p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSwissNameDictionaryPropertyDetailsTable(SwissNameDictionary p_o);
inline intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int31_0(int31_t p_i);
inline intptr_t TqRuntimeConvert_intptr_int32_0(int32_t p_i);
inline intptr_t TqRuntimeConvert_intptr_intptr_0(intptr_t p_i);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int32_0(int32_t p_i);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_uint8_0(Object p_object, intptr_t p_offset, intptr_t p_length);
#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSwissNameDictionaryDataTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSwissNameDictionaryDataTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSwissNameDictionaryDataTable(SwissNameDictionary p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  int32_t tmp1{}; USE(tmp1);
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
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  tmp1 = (p_o).ReadField<int32_t>(tmp0);
  tmp2 = TqRuntimeConvert_intptr_int32_0(tmp1);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(2);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp2, tmp3);
  tmp5 = TqRuntimeConvert_intptr_intptr_0(tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(16);
  std::tie(tmp7, tmp8, tmp9) = TqRuntimeNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(p_o, tmp6, tmp5);
  goto block2;

  block2:
  return std::make_tuple(tmp7, tmp8, tmp9);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSwissNameDictionaryDataTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSwissNameDictionaryCtrlTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSwissNameDictionaryCtrlTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSwissNameDictionaryCtrlTable(SwissNameDictionary p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  int32_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  Object tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceSwissNameDictionaryDataTable(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  tmp7 = (p_o).ReadField<int32_t>(tmp6);
  tmp8 = TqRuntimeConvert_intptr_int32_0(tmp7);
  tmp9 = TqRuntimeFromConstexpr_intptr_constexpr_int32_0(swiss_table::Group::kWidth);
  tmp10 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp8, tmp9);
  tmp11 = TqRuntimeConvert_intptr_intptr_0(tmp10);
  std::tie(tmp12, tmp13, tmp14) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp5, tmp11);
  goto block2;

  block2:
  return std::make_tuple(tmp12, tmp13, tmp14);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSwissNameDictionaryCtrlTable

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSwissNameDictionaryPropertyDetailsTable
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSwissNameDictionaryPropertyDetailsTable
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceSwissNameDictionaryPropertyDetailsTable(SwissNameDictionary p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  int32_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  Object tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  intptr_t tmp12{}; USE(tmp12);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceSwissNameDictionaryCtrlTable(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(1);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  tmp7 = (p_o).ReadField<int32_t>(tmp6);
  tmp8 = TqRuntimeConvert_intptr_int32_0(tmp7);
  tmp9 = TqRuntimeConvert_intptr_intptr_0(tmp8);
  std::tie(tmp10, tmp11, tmp12) = TqRuntimeNewMutableSlice_uint8_0(p_o, tmp5, tmp9);
  goto block2;

  block2:
  return std::make_tuple(tmp10, tmp11, tmp12);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceSwissNameDictionaryPropertyDetailsTable

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

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_int32_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_int32_0
intptr_t TqRuntimeConvert_intptr_int32_0(int32_t p_i) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = TorqueRuntimeMacroShims::CodeStubAssembler::ChangeInt32ToIntPtr(p_i);
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_int32_0

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

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_intptr_constexpr_int32_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_intptr_constexpr_int32_0
intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int32_0(int32_t p_i) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_intptr_constexpr_int32_0

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

