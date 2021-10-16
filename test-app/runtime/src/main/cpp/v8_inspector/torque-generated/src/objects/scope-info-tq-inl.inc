inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoContextLocalNames(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoContextLocalInfos(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoReceiverInfo(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoFunctionVariableInfo(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoInferredFunctionName(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoPositionInfo(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoOuterScopeInfo(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoLocalsBlockList(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoModuleInfo(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoModuleVariableCount(ScopeInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoModuleVariables(ScopeInfo p_o);
inline intptr_t TqRuntimeFromConstexpr_intptr_constexpr_int31_0(int31_t p_i);
inline intptr_t TqRuntimeConvert_intptr_Smi_0(Smi p_s);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_String_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_SmiTagged_VariableProperties_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline int31_t TqRuntimeFromConstexpr_int31_constexpr_int31_0(int31_t p_i);
inline intptr_t TqRuntimeConvert_intptr_int31_0(int31_t p_i);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_Smi_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline uint32_t TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_STACK_0(VariableAllocationInfo p_o);
inline bool TqRuntimeFromConstexpr_bool_constexpr_bool_0(bool p_b);
inline uint32_t TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0(VariableAllocationInfo p_o);
inline uint32_t TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_NONE_0(VariableAllocationInfo p_o);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_FunctionVariableInfo_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_String_OR_Undefined_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline uint32_t TqRuntimeFromConstexpr_uint32_constexpr_uint32_0(uint32_t p_i);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_PositionInfo_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_TheHole_OR_ScopeInfo_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_HashTable_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_SourceTextModuleInfo_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewConstSlice_Smi_0(Object p_object, intptr_t p_offset, intptr_t p_length);
inline intptr_t TqRuntimeConvert_intptr_constexpr_int31_0(int31_t p_i);
inline uintptr_t TqRuntimeConvert_uintptr_intptr_0(intptr_t p_i);
inline intptr_t TqRuntimeTimesSizeOf_Smi_0(intptr_t p_i);
inline std::tuple<Object, intptr_t> TqRuntimeNewReference_Smi_0(Object p_object, intptr_t p_offset);
inline Smi TqRuntimeFromConstexpr_Smi_constexpr_int31_0(int31_t p_i);
inline std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_ModuleVariable_0(Object p_object, intptr_t p_offset, intptr_t p_length);
#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoContextLocalNames
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoContextLocalNames
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoContextLocalNames(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  Smi tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  Object tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(12);
  tmp1 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp0));
  tmp2 = TqRuntimeConvert_intptr_Smi_0(tmp1);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(16);
  std::tie(tmp4, tmp5, tmp6) = TqRuntimeNewMutableSlice_String_0(p_o, tmp3, tmp2);
  goto block2;

  block2:
  return std::make_tuple(tmp4, tmp5, tmp6);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoContextLocalNames

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoContextLocalInfos
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoContextLocalInfos
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoContextLocalInfos(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  Object tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoContextLocalNames(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(12);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = TqRuntimeConvert_intptr_Smi_0(tmp7);
  std::tie(tmp9, tmp10, tmp11) = TqRuntimeNewMutableSlice_SmiTagged_VariableProperties_0(p_o, tmp5, tmp8);
  goto block2;

  block2:
  return std::make_tuple(tmp9, tmp10, tmp11);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoContextLocalInfos

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  bool tmp8{}; USE(tmp8);
  int31_t tmp9{}; USE(tmp9);
  int31_t tmp10{}; USE(tmp10);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp11{}; USE(tmp11);
  Object tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoContextLocalInfos(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<bool, 10, 1, uint32_t>::decode(tmp7.value()));
  if (tmp8) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  tmp9 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(1);
  phi_bb4_8 = tmp9;
  goto block4;

  block3:
  tmp10 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(0);
  phi_bb4_8 = tmp10;
  goto block4;

  block4:
  tmp11 = TqRuntimeConvert_intptr_int31_0(phi_bb4_8);
  std::tie(tmp12, tmp13, tmp14) = TqRuntimeNewMutableSlice_Smi_0(p_o, tmp5, tmp11);
  goto block6;

  block6:
  return std::make_tuple(tmp12, tmp13, tmp14);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoReceiverInfo
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoReceiverInfo
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoReceiverInfo(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  bool tmp11{}; USE(tmp11);
  intptr_t tmp12{}; USE(tmp12);
  Smi tmp13{}; USE(tmp13);
  uint32_t tmp14{}; USE(tmp14);
  uint32_t tmp15{}; USE(tmp15);
  bool tmp16{}; USE(tmp16);
  bool phi_bb8_9{}; USE(phi_bb8_9);
  int31_t tmp17{}; USE(tmp17);
  int31_t tmp18{}; USE(tmp18);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp19{}; USE(tmp19);
  Object tmp20{}; USE(tmp20);
  intptr_t tmp21{}; USE(tmp21);
  intptr_t tmp22{}; USE(tmp22);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<VariableAllocationInfo, 7, 2, uint32_t>::decode(tmp7.value()));
  tmp9 = TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_STACK_0(VariableAllocationInfo::STACK);
  tmp10 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32Equal(tmp8, tmp9);
  if (tmp10) {
    goto block6;
  } else {
    goto block7;
  }

  block6:
  tmp11 = TqRuntimeFromConstexpr_bool_constexpr_bool_0(true);
  phi_bb8_9 = tmp11;
  goto block8;

  block7:
  tmp12 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp13 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp12));
  tmp14 = CastToUnderlyingTypeIfEnum(base::BitField<VariableAllocationInfo, 7, 2, uint32_t>::decode(tmp13.value()));
  tmp15 = TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0(VariableAllocationInfo::CONTEXT);
  tmp16 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32Equal(tmp14, tmp15);
  phi_bb8_9 = tmp16;
  goto block8;

  block8:
  if (phi_bb8_9) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  tmp17 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(1);
  phi_bb4_8 = tmp17;
  goto block4;

  block3:
  tmp18 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(0);
  phi_bb4_8 = tmp18;
  goto block4;

  block4:
  tmp19 = TqRuntimeConvert_intptr_int31_0(phi_bb4_8);
  std::tie(tmp20, tmp21, tmp22) = TqRuntimeNewMutableSlice_Smi_0(p_o, tmp5, tmp19);
  goto block9;

  block9:
  return std::make_tuple(tmp20, tmp21, tmp22);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoReceiverInfo

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoFunctionVariableInfo
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoFunctionVariableInfo
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoFunctionVariableInfo(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  int31_t tmp11{}; USE(tmp11);
  int31_t tmp12{}; USE(tmp12);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp13{}; USE(tmp13);
  Object tmp14{}; USE(tmp14);
  intptr_t tmp15{}; USE(tmp15);
  intptr_t tmp16{}; USE(tmp16);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoReceiverInfo(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<VariableAllocationInfo, 12, 2, uint32_t>::decode(tmp7.value()));
  tmp9 = TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_NONE_0(VariableAllocationInfo::NONE);
  tmp10 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32NotEqual(tmp8, tmp9);
  if (tmp10) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  tmp11 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(1);
  phi_bb4_8 = tmp11;
  goto block4;

  block3:
  tmp12 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(0);
  phi_bb4_8 = tmp12;
  goto block4;

  block4:
  tmp13 = TqRuntimeConvert_intptr_int31_0(phi_bb4_8);
  std::tie(tmp14, tmp15, tmp16) = TqRuntimeNewMutableSlice_FunctionVariableInfo_0(p_o, tmp5, tmp13);
  goto block6;

  block6:
  return std::make_tuple(tmp14, tmp15, tmp16);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoFunctionVariableInfo

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoInferredFunctionName
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoInferredFunctionName
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoInferredFunctionName(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  bool tmp8{}; USE(tmp8);
  int31_t tmp9{}; USE(tmp9);
  int31_t tmp10{}; USE(tmp10);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp11{}; USE(tmp11);
  Object tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoFunctionVariableInfo(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<bool, 14, 1, uint32_t>::decode(tmp7.value()));
  if (tmp8) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  tmp9 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(1);
  phi_bb4_8 = tmp9;
  goto block4;

  block3:
  tmp10 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(0);
  phi_bb4_8 = tmp10;
  goto block4;

  block4:
  tmp11 = TqRuntimeConvert_intptr_int31_0(phi_bb4_8);
  std::tie(tmp12, tmp13, tmp14) = TqRuntimeNewMutableSlice_String_OR_Undefined_0(p_o, tmp5, tmp11);
  goto block6;

  block6:
  return std::make_tuple(tmp12, tmp13, tmp14);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoInferredFunctionName

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoPositionInfo
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoPositionInfo
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoPositionInfo(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  bool tmp11{}; USE(tmp11);
  intptr_t tmp12{}; USE(tmp12);
  Smi tmp13{}; USE(tmp13);
  uint32_t tmp14{}; USE(tmp14);
  uint32_t tmp15{}; USE(tmp15);
  bool tmp16{}; USE(tmp16);
  bool phi_bb8_9{}; USE(phi_bb8_9);
  bool tmp17{}; USE(tmp17);
  intptr_t tmp18{}; USE(tmp18);
  Smi tmp19{}; USE(tmp19);
  uint32_t tmp20{}; USE(tmp20);
  uint32_t tmp21{}; USE(tmp21);
  bool tmp22{}; USE(tmp22);
  bool phi_bb11_9{}; USE(phi_bb11_9);
  bool tmp23{}; USE(tmp23);
  intptr_t tmp24{}; USE(tmp24);
  Smi tmp25{}; USE(tmp25);
  uint32_t tmp26{}; USE(tmp26);
  uint32_t tmp27{}; USE(tmp27);
  bool tmp28{}; USE(tmp28);
  bool phi_bb14_9{}; USE(phi_bb14_9);
  int31_t tmp29{}; USE(tmp29);
  int31_t tmp30{}; USE(tmp30);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp31{}; USE(tmp31);
  Object tmp32{}; USE(tmp32);
  intptr_t tmp33{}; USE(tmp33);
  intptr_t tmp34{}; USE(tmp34);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoInferredFunctionName(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(tmp7.value()));
  tmp9 = TqRuntimeFromConstexpr_uint32_constexpr_uint32_0(ScopeType::FUNCTION_SCOPE);
  tmp10 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32Equal(tmp8, tmp9);
  if (tmp10) {
    goto block6;
  } else {
    goto block7;
  }

  block6:
  tmp11 = TqRuntimeFromConstexpr_bool_constexpr_bool_0(true);
  phi_bb8_9 = tmp11;
  goto block8;

  block7:
  tmp12 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp13 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp12));
  tmp14 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(tmp13.value()));
  tmp15 = TqRuntimeFromConstexpr_uint32_constexpr_uint32_0(ScopeType::SCRIPT_SCOPE);
  tmp16 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32Equal(tmp14, tmp15);
  phi_bb8_9 = tmp16;
  goto block8;

  block8:
  if (phi_bb8_9) {
    goto block9;
  } else {
    goto block10;
  }

  block9:
  tmp17 = TqRuntimeFromConstexpr_bool_constexpr_bool_0(true);
  phi_bb11_9 = tmp17;
  goto block11;

  block10:
  tmp18 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp19 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp18));
  tmp20 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(tmp19.value()));
  tmp21 = TqRuntimeFromConstexpr_uint32_constexpr_uint32_0(ScopeType::EVAL_SCOPE);
  tmp22 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32Equal(tmp20, tmp21);
  phi_bb11_9 = tmp22;
  goto block11;

  block11:
  if (phi_bb11_9) {
    goto block12;
  } else {
    goto block13;
  }

  block12:
  tmp23 = TqRuntimeFromConstexpr_bool_constexpr_bool_0(true);
  phi_bb14_9 = tmp23;
  goto block14;

  block13:
  tmp24 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp25 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp24));
  tmp26 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(tmp25.value()));
  tmp27 = TqRuntimeFromConstexpr_uint32_constexpr_uint32_0(ScopeType::MODULE_SCOPE);
  tmp28 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32Equal(tmp26, tmp27);
  phi_bb14_9 = tmp28;
  goto block14;

  block14:
  if (phi_bb14_9) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  tmp29 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(1);
  phi_bb4_8 = tmp29;
  goto block4;

  block3:
  tmp30 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(0);
  phi_bb4_8 = tmp30;
  goto block4;

  block4:
  tmp31 = TqRuntimeConvert_intptr_int31_0(phi_bb4_8);
  std::tie(tmp32, tmp33, tmp34) = TqRuntimeNewMutableSlice_PositionInfo_0(p_o, tmp5, tmp31);
  goto block15;

  block15:
  return std::make_tuple(tmp32, tmp33, tmp34);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoPositionInfo

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoOuterScopeInfo
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoOuterScopeInfo
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoOuterScopeInfo(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  bool tmp8{}; USE(tmp8);
  int31_t tmp9{}; USE(tmp9);
  int31_t tmp10{}; USE(tmp10);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp11{}; USE(tmp11);
  Object tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoPositionInfo(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(8);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<bool, 22, 1, uint32_t>::decode(tmp7.value()));
  if (tmp8) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  tmp9 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(1);
  phi_bb4_8 = tmp9;
  goto block4;

  block3:
  tmp10 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(0);
  phi_bb4_8 = tmp10;
  goto block4;

  block4:
  tmp11 = TqRuntimeConvert_intptr_int31_0(phi_bb4_8);
  std::tie(tmp12, tmp13, tmp14) = TqRuntimeNewMutableSlice_TheHole_OR_ScopeInfo_0(p_o, tmp5, tmp11);
  goto block6;

  block6:
  return std::make_tuple(tmp12, tmp13, tmp14);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoOuterScopeInfo

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoLocalsBlockList
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoLocalsBlockList
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoLocalsBlockList(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  bool tmp8{}; USE(tmp8);
  int31_t tmp9{}; USE(tmp9);
  int31_t tmp10{}; USE(tmp10);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp11{}; USE(tmp11);
  Object tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoOuterScopeInfo(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<bool, 28, 1, uint32_t>::decode(tmp7.value()));
  if (tmp8) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  tmp9 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(1);
  phi_bb4_8 = tmp9;
  goto block4;

  block3:
  tmp10 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(0);
  phi_bb4_8 = tmp10;
  goto block4;

  block4:
  tmp11 = TqRuntimeConvert_intptr_int31_0(phi_bb4_8);
  std::tie(tmp12, tmp13, tmp14) = TqRuntimeNewMutableSlice_HashTable_0(p_o, tmp5, tmp11);
  goto block6;

  block6:
  return std::make_tuple(tmp12, tmp13, tmp14);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoLocalsBlockList

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoModuleInfo
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoModuleInfo
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoModuleInfo(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  int31_t tmp11{}; USE(tmp11);
  int31_t tmp12{}; USE(tmp12);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp13{}; USE(tmp13);
  Object tmp14{}; USE(tmp14);
  intptr_t tmp15{}; USE(tmp15);
  intptr_t tmp16{}; USE(tmp16);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoLocalsBlockList(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(tmp7.value()));
  tmp9 = TqRuntimeFromConstexpr_uint32_constexpr_uint32_0(ScopeType::MODULE_SCOPE);
  tmp10 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32Equal(tmp8, tmp9);
  if (tmp10) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  tmp11 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(1);
  phi_bb4_8 = tmp11;
  goto block4;

  block3:
  tmp12 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(0);
  phi_bb4_8 = tmp12;
  goto block4;

  block4:
  tmp13 = TqRuntimeConvert_intptr_int31_0(phi_bb4_8);
  std::tie(tmp14, tmp15, tmp16) = TqRuntimeNewMutableSlice_SourceTextModuleInfo_0(p_o, tmp5, tmp13);
  goto block6;

  block6:
  return std::make_tuple(tmp14, tmp15, tmp16);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoModuleInfo

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoModuleVariableCount
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoModuleVariableCount
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoModuleVariableCount(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  int31_t tmp11{}; USE(tmp11);
  int31_t tmp12{}; USE(tmp12);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp13{}; USE(tmp13);
  Object tmp14{}; USE(tmp14);
  intptr_t tmp15{}; USE(tmp15);
  intptr_t tmp16{}; USE(tmp16);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoModuleInfo(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(tmp7.value()));
  tmp9 = TqRuntimeFromConstexpr_uint32_constexpr_uint32_0(ScopeType::MODULE_SCOPE);
  tmp10 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32Equal(tmp8, tmp9);
  if (tmp10) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  tmp11 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(1);
  phi_bb4_8 = tmp11;
  goto block4;

  block3:
  tmp12 = TqRuntimeFromConstexpr_int31_constexpr_int31_0(0);
  phi_bb4_8 = tmp12;
  goto block4;

  block4:
  tmp13 = TqRuntimeConvert_intptr_int31_0(phi_bb4_8);
  std::tie(tmp14, tmp15, tmp16) = TqRuntimeNewConstSlice_Smi_0(p_o, tmp5, tmp13);
  goto block6;

  block6:
  return std::make_tuple(tmp14, tmp15, tmp16);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoModuleVariableCount

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoModuleVariables
#define V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoModuleVariables
std::tuple<Object, intptr_t, intptr_t> TqRuntimeFieldSliceScopeInfoModuleVariables(ScopeInfo p_o) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  Smi tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  Object tmp11{}; USE(tmp11);
  intptr_t tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  uintptr_t tmp15{}; USE(tmp15);
  uintptr_t tmp16{}; USE(tmp16);
  bool tmp17{}; USE(tmp17);
  intptr_t tmp18{}; USE(tmp18);
  intptr_t tmp19{}; USE(tmp19);
  Object tmp20{}; USE(tmp20);
  intptr_t tmp21{}; USE(tmp21);
  Smi tmp22{}; USE(tmp22);
  Smi tmp23{}; USE(tmp23);
  Smi phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp24{}; USE(tmp24);
  Object tmp25{}; USE(tmp25);
  intptr_t tmp26{}; USE(tmp26);
  intptr_t tmp27{}; USE(tmp27);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = TqRuntimeFieldSliceScopeInfoModuleVariableCount(p_o);
  tmp3 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp4 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(tmp3, tmp2);
  tmp5 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp1, tmp4);
  tmp6 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(4);
  tmp7 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&p_o), static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(tmp7.value()));
  tmp9 = TqRuntimeFromConstexpr_uint32_constexpr_uint32_0(ScopeType::MODULE_SCOPE);
  tmp10 = TorqueRuntimeMacroShims::CodeStubAssembler::Word32Equal(tmp8, tmp9);
  if (tmp10) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  std::tie(tmp11, tmp12, tmp13) = TqRuntimeFieldSliceScopeInfoModuleVariableCount(p_o);
  tmp14 = TqRuntimeConvert_intptr_constexpr_int31_0(0);
  tmp15 = TqRuntimeConvert_uintptr_intptr_0(tmp14);
  tmp16 = TqRuntimeConvert_uintptr_intptr_0(tmp13);
  tmp17 = TorqueRuntimeMacroShims::CodeStubAssembler::UintPtrLessThan(tmp15, tmp16);
  if (tmp17) {
    goto block10;
  } else {
    goto block11;
  }

  block10:
  tmp18 = TqRuntimeTimesSizeOf_Smi_0(tmp14);
  tmp19 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrAdd(tmp12, tmp18);
  std::tie(tmp20, tmp21) = TqRuntimeNewReference_Smi_0(tmp11, tmp19);
  tmp22 = TaggedField<Smi>::load(*static_cast<HeapObject*>(&tmp20), static_cast<int>(tmp21));
  phi_bb4_8 = tmp22;
  goto block4;

  block11:
  UNREACHABLE();

  block3:
  tmp23 = TqRuntimeFromConstexpr_Smi_constexpr_int31_0(0);
  phi_bb4_8 = tmp23;
  goto block4;

  block4:
  tmp24 = TqRuntimeConvert_intptr_Smi_0(phi_bb4_8);
  std::tie(tmp25, tmp26, tmp27) = TqRuntimeNewMutableSlice_ModuleVariable_0(p_o, tmp5, tmp24);
  goto block13;

  block13:
  return std::make_tuple(tmp25, tmp26, tmp27);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFieldSliceScopeInfoModuleVariables

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

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_Smi_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_Smi_0
intptr_t TqRuntimeConvert_intptr_Smi_0(Smi p_s) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = TorqueRuntimeMacroShims::CodeStubAssembler::SmiUntag(p_s);
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_String_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_String_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_String_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_String_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_SmiTagged_VariableProperties_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_SmiTagged_VariableProperties_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_SmiTagged_VariableProperties_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_SmiTagged_VariableProperties_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_int31_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_int31_constexpr_int31_0
int31_t TqRuntimeFromConstexpr_int31_constexpr_int31_0(int31_t p_i) {
  DisallowGarbageCollection no_gc;
  int31_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_int31_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_int31_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_int31_0
intptr_t TqRuntimeConvert_intptr_int31_0(int31_t p_i) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = TorqueRuntimeMacroShims::CodeStubAssembler::ChangeInt32ToIntPtr(p_i);
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_intptr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_Smi_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_Smi_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_Smi_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_STACK_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_STACK_0
uint32_t TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_STACK_0(VariableAllocationInfo p_o) {
  DisallowGarbageCollection no_gc;
  uint32_t tmp0{}; USE(tmp0);
  uint32_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_o));
  tmp1 = (tmp0);
  goto block2;

  block2:
  return tmp1;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_STACK_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_bool_constexpr_bool_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_bool_constexpr_bool_0
bool TqRuntimeFromConstexpr_bool_constexpr_bool_0(bool p_b) {
  DisallowGarbageCollection no_gc;
  bool tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = TorqueRuntimeMacroShims::CodeStubAssembler::BoolConstant(p_b);
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_bool_constexpr_bool_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0
uint32_t TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0(VariableAllocationInfo p_o) {
  DisallowGarbageCollection no_gc;
  uint32_t tmp0{}; USE(tmp0);
  uint32_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_o));
  tmp1 = (tmp0);
  goto block2;

  block2:
  return tmp1;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_NONE_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_NONE_0
uint32_t TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_NONE_0(VariableAllocationInfo p_o) {
  DisallowGarbageCollection no_gc;
  uint32_t tmp0{}; USE(tmp0);
  uint32_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_o));
  tmp1 = (tmp0);
  goto block2;

  block2:
  return tmp1;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_VariableAllocationInfo_constexpr_NONE_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_FunctionVariableInfo_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_FunctionVariableInfo_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_FunctionVariableInfo_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_FunctionVariableInfo_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_String_OR_Undefined_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_String_OR_Undefined_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_String_OR_Undefined_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_String_OR_Undefined_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_uint32_constexpr_uint32_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_uint32_constexpr_uint32_0
uint32_t TqRuntimeFromConstexpr_uint32_constexpr_uint32_0(uint32_t p_i) {
  DisallowGarbageCollection no_gc;
  int32_t tmp0{}; USE(tmp0);
  uint32_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  tmp1 = TorqueRuntimeMacroShims::CodeStubAssembler::Unsigned(tmp0);
  goto block2;

  block2:
  return tmp1;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_uint32_constexpr_uint32_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_PositionInfo_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_PositionInfo_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_PositionInfo_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_PositionInfo_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_TheHole_OR_ScopeInfo_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_TheHole_OR_ScopeInfo_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_TheHole_OR_ScopeInfo_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_TheHole_OR_ScopeInfo_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_HashTable_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_HashTable_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_HashTable_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_HashTable_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_SourceTextModuleInfo_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_SourceTextModuleInfo_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_SourceTextModuleInfo_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_SourceTextModuleInfo_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewConstSlice_Smi_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewConstSlice_Smi_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewConstSlice_Smi_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewConstSlice_Smi_0

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

#ifndef V8_INTERNAL_DEFINED_TqRuntimeConvert_uintptr_intptr_0
#define V8_INTERNAL_DEFINED_TqRuntimeConvert_uintptr_intptr_0
uintptr_t TqRuntimeConvert_uintptr_intptr_0(intptr_t p_i) {
  DisallowGarbageCollection no_gc;
  uintptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = TorqueRuntimeMacroShims::CodeStubAssembler::Unsigned(p_i);
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeConvert_uintptr_intptr_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeTimesSizeOf_Smi_0
#define V8_INTERNAL_DEFINED_TqRuntimeTimesSizeOf_Smi_0
intptr_t TqRuntimeTimesSizeOf_Smi_0(intptr_t p_i) {
  DisallowGarbageCollection no_gc;
  intptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = TqRuntimeFromConstexpr_intptr_constexpr_int31_0(kTaggedSize);
  tmp1 = TorqueRuntimeMacroShims::CodeStubAssembler::IntPtrMul(p_i, tmp0);
  goto block2;

  block2:
  return tmp1;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeTimesSizeOf_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewReference_Smi_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewReference_Smi_0
std::tuple<Object, intptr_t> TqRuntimeNewReference_Smi_0(Object p_object, intptr_t p_offset) {
  DisallowGarbageCollection no_gc;
  Object tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  std::tie(tmp0, tmp1) = (std::make_tuple(p_object, p_offset));
  goto block2;

  block2:
  return std::make_tuple(tmp0, tmp1);
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewReference_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_Smi_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_Smi_constexpr_int31_0
Smi TqRuntimeFromConstexpr_Smi_constexpr_int31_0(int31_t p_i) {
  DisallowGarbageCollection no_gc;
  Smi tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = Smi::FromInt(CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return tmp0;
}

#endif // V8_INTERNAL_DEFINED_TqRuntimeFromConstexpr_Smi_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_ModuleVariable_0
#define V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_ModuleVariable_0
std::tuple<Object, intptr_t, intptr_t> TqRuntimeNewMutableSlice_ModuleVariable_0(Object p_object, intptr_t p_offset, intptr_t p_length) {
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

#endif // V8_INTERNAL_DEFINED_TqRuntimeNewMutableSlice_ModuleVariable_0

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::flags() const {
  int value;
  value = TaggedField<Smi>::load(*this, kFlagsOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_flags(int value) {
  WRITE_FIELD(*this, kFlagsOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::parameter_count() const {
  int value;
  value = TaggedField<Smi>::load(*this, kParameterCountOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_parameter_count(int value) {
  WRITE_FIELD(*this, kParameterCountOffset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::context_local_count() const {
  int value;
  value = TaggedField<Smi>::load(*this, kContextLocalCountOffset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_context_local_count(int value) {
  WRITE_FIELD(*this, kContextLocalCountOffset, Smi::FromInt(value));
}

template<class D, class P>
String TorqueGeneratedScopeInfo<D, P>::context_local_names(int i) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScopeInfo::context_local_names(cage_base, i);
}

template<class D, class P>
String TorqueGeneratedScopeInfo<D, P>::context_local_names(PtrComprCageBase cage_base, int i) const {
  String value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->context_local_count());
  int offset = kContextLocalNamesOffset + i * kTaggedSize;
  value = TaggedField<String>::load(cage_base, *this, offset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_context_local_names(int i, String value, WriteBarrierMode mode) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->context_local_count());
  int offset = kContextLocalNamesOffset + i * kTaggedSize;
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::context_local_infos(int i) const {
  int value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->context_local_count());
  int offset = ContextLocalInfosOffset() + i * kTaggedSize;
  value = TaggedField<Smi>::load(*this, offset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_context_local_infos(int i, int value) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, this ->context_local_count());
  int offset = ContextLocalInfosOffset() + i * kTaggedSize;
  WRITE_FIELD(*this, offset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::saved_class_variable_info() const {
  int value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo(*static_cast<const D*>(this)))));
  int offset = SavedClassVariableInfoOffset() + 0 * kTaggedSize;
  value = TaggedField<Smi>::load(*this, offset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_saved_class_variable_info(int value) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo(*static_cast<const D*>(this)))));
  int offset = SavedClassVariableInfoOffset() + 0 * kTaggedSize;
  WRITE_FIELD(*this, offset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::receiver_info() const {
  int value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoReceiverInfo(*static_cast<const D*>(this)))));
  int offset = ReceiverInfoOffset() + 0 * kTaggedSize;
  value = TaggedField<Smi>::load(*this, offset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_receiver_info(int value) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoReceiverInfo(*static_cast<const D*>(this)))));
  int offset = ReceiverInfoOffset() + 0 * kTaggedSize;
  WRITE_FIELD(*this, offset, Smi::FromInt(value));
}

template<class D, class P>
Object TorqueGeneratedScopeInfo<D, P>::function_variable_info_name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScopeInfo::function_variable_info_name(cage_base);
}

template<class D, class P>
Object TorqueGeneratedScopeInfo<D, P>::function_variable_info_name(PtrComprCageBase cage_base) const {
  Object value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoFunctionVariableInfo(*static_cast<const D*>(this)))));
  int offset = FunctionVariableInfoOffset() + 0 + 0 * 8;
  value = TaggedField<Object>::load(cage_base, *this, offset);
  DCHECK(value.IsString() || value.IsZero());
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_function_variable_info_name(Object value, WriteBarrierMode mode) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoFunctionVariableInfo(*static_cast<const D*>(this)))));
  int offset = FunctionVariableInfoOffset() + 0 + 0 * 8;
  SLOW_DCHECK(value.IsString() || value.IsZero());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::function_variable_info_context_or_stack_slot_index() const {
  int value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoFunctionVariableInfo(*static_cast<const D*>(this)))));
  int offset = FunctionVariableInfoOffset() + 4 + 0 * 8;
  value = TaggedField<Smi>::load(*this, offset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_function_variable_info_context_or_stack_slot_index(int value) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoFunctionVariableInfo(*static_cast<const D*>(this)))));
  int offset = FunctionVariableInfoOffset() + 4 + 0 * 8;
  WRITE_FIELD(*this, offset, Smi::FromInt(value));
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedScopeInfo<D, P>::inferred_function_name() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScopeInfo::inferred_function_name(cage_base);
}

template<class D, class P>
PrimitiveHeapObject TorqueGeneratedScopeInfo<D, P>::inferred_function_name(PtrComprCageBase cage_base) const {
  PrimitiveHeapObject value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoInferredFunctionName(*static_cast<const D*>(this)))));
  int offset = InferredFunctionNameOffset() + 0 * kTaggedSize;
  value = TaggedField<PrimitiveHeapObject>::load(cage_base, *this, offset);
  DCHECK(value.IsString() || value.IsUndefined());
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_inferred_function_name(PrimitiveHeapObject value, WriteBarrierMode mode) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoInferredFunctionName(*static_cast<const D*>(this)))));
  int offset = InferredFunctionNameOffset() + 0 * kTaggedSize;
  SLOW_DCHECK(value.IsString() || value.IsUndefined());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::position_info_start() const {
  int value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoPositionInfo(*static_cast<const D*>(this)))));
  int offset = PositionInfoOffset() + 0 + 0 * 8;
  value = TaggedField<Smi>::load(*this, offset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_position_info_start(int value) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoPositionInfo(*static_cast<const D*>(this)))));
  int offset = PositionInfoOffset() + 0 + 0 * 8;
  WRITE_FIELD(*this, offset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::position_info_end() const {
  int value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoPositionInfo(*static_cast<const D*>(this)))));
  int offset = PositionInfoOffset() + 4 + 0 * 8;
  value = TaggedField<Smi>::load(*this, offset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_position_info_end(int value) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoPositionInfo(*static_cast<const D*>(this)))));
  int offset = PositionInfoOffset() + 4 + 0 * 8;
  WRITE_FIELD(*this, offset, Smi::FromInt(value));
}

template<class D, class P>
HeapObject TorqueGeneratedScopeInfo<D, P>::outer_scope_info() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScopeInfo::outer_scope_info(cage_base);
}

template<class D, class P>
HeapObject TorqueGeneratedScopeInfo<D, P>::outer_scope_info(PtrComprCageBase cage_base) const {
  HeapObject value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoOuterScopeInfo(*static_cast<const D*>(this)))));
  int offset = OuterScopeInfoOffset() + 0 * kTaggedSize;
  value = TaggedField<HeapObject>::load(cage_base, *this, offset);
  DCHECK(value.IsTheHole() || value.IsScopeInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_outer_scope_info(HeapObject value, WriteBarrierMode mode) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoOuterScopeInfo(*static_cast<const D*>(this)))));
  int offset = OuterScopeInfoOffset() + 0 * kTaggedSize;
  SLOW_DCHECK(value.IsTheHole() || value.IsScopeInfo());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedScopeInfo<D, P>::locals_block_list() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScopeInfo::locals_block_list(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedScopeInfo<D, P>::locals_block_list(PtrComprCageBase cage_base) const {
  FixedArray value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoLocalsBlockList(*static_cast<const D*>(this)))));
  int offset = LocalsBlockListOffset() + 0 * kTaggedSize;
  value = TaggedField<FixedArray>::load(cage_base, *this, offset);
  DCHECK(value.IsHashTable());
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_locals_block_list(FixedArray value, WriteBarrierMode mode) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoLocalsBlockList(*static_cast<const D*>(this)))));
  int offset = LocalsBlockListOffset() + 0 * kTaggedSize;
  SLOW_DCHECK(value.IsHashTable());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
FixedArray TorqueGeneratedScopeInfo<D, P>::module_info() const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScopeInfo::module_info(cage_base);
}

template<class D, class P>
FixedArray TorqueGeneratedScopeInfo<D, P>::module_info(PtrComprCageBase cage_base) const {
  FixedArray value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleInfo(*static_cast<const D*>(this)))));
  int offset = ModuleInfoOffset() + 0 * kTaggedSize;
  value = TaggedField<FixedArray>::load(cage_base, *this, offset);
  DCHECK(value.IsSourceTextModuleInfo());
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_module_info(FixedArray value, WriteBarrierMode mode) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleInfo(*static_cast<const D*>(this)))));
  int offset = ModuleInfoOffset() + 0 * kTaggedSize;
  SLOW_DCHECK(value.IsSourceTextModuleInfo());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::module_variable_count() const {
  int value;
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleVariableCount(*static_cast<const D*>(this)))));
  int offset = ModuleVariableCountOffset() + 0 * kTaggedSize;
  value = TaggedField<Smi>::load(*this, offset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_module_variable_count(int value) {
  DCHECK_GE(0, 0);
  DCHECK_LT(0, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleVariableCount(*static_cast<const D*>(this)))));
  int offset = ModuleVariableCountOffset() + 0 * kTaggedSize;
  WRITE_FIELD(*this, offset, Smi::FromInt(value));
}

template<class D, class P>
String TorqueGeneratedScopeInfo<D, P>::module_variables_name(int i) const {
  PtrComprCageBase cage_base = GetPtrComprCageBase(*this);
  return TorqueGeneratedScopeInfo::module_variables_name(cage_base, i);
}

template<class D, class P>
String TorqueGeneratedScopeInfo<D, P>::module_variables_name(PtrComprCageBase cage_base, int i) const {
  String value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleVariables(*static_cast<const D*>(this)))));
  int offset = ModuleVariablesOffset() + 0 + i * 12;
  value = TaggedField<String>::load(cage_base, *this, offset);
  DCHECK(value.IsString());
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_module_variables_name(int i, String value, WriteBarrierMode mode) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleVariables(*static_cast<const D*>(this)))));
  int offset = ModuleVariablesOffset() + 0 + i * 12;
  SLOW_DCHECK(value.IsString());
  WRITE_FIELD(*this, offset, value);
  CONDITIONAL_WRITE_BARRIER(*this, offset, value, mode);
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::module_variables_index(int i) const {
  int value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleVariables(*static_cast<const D*>(this)))));
  int offset = ModuleVariablesOffset() + 4 + i * 12;
  value = TaggedField<Smi>::load(*this, offset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_module_variables_index(int i, int value) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleVariables(*static_cast<const D*>(this)))));
  int offset = ModuleVariablesOffset() + 4 + i * 12;
  WRITE_FIELD(*this, offset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::module_variables_properties(int i) const {
  int value;
  DCHECK_GE(i, 0);
  DCHECK_LT(i, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleVariables(*static_cast<const D*>(this)))));
  int offset = ModuleVariablesOffset() + 8 + i * 12;
  value = TaggedField<Smi>::load(*this, offset).value();
  return value;
}

template<class D, class P>
void TorqueGeneratedScopeInfo<D, P>::set_module_variables_properties(int i, int value) {
  DCHECK_GE(i, 0);
  DCHECK_LT(i, static_cast<int>(std::get<2>(TqRuntimeFieldSliceScopeInfoModuleVariables(*static_cast<const D*>(this)))));
  int offset = ModuleVariablesOffset() + 8 + i * 12;
  WRITE_FIELD(*this, offset, Smi::FromInt(value));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::ContextLocalInfosOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoContextLocalInfos(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::SavedClassVariableInfoOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoSavedClassVariableInfo(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::ReceiverInfoOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoReceiverInfo(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::FunctionVariableInfoOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoFunctionVariableInfo(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::InferredFunctionNameOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoInferredFunctionName(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::PositionInfoOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoPositionInfo(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::OuterScopeInfoOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoOuterScopeInfo(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::LocalsBlockListOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoLocalsBlockList(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::ModuleInfoOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoModuleInfo(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::ModuleVariableCountOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoModuleVariableCount(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::ModuleVariablesOffset() const {
  return static_cast<int>(std::get<1>(TqRuntimeFieldSliceScopeInfoModuleVariables(*static_cast<const D*>(this))));
}

template<class D, class P>
int TorqueGeneratedScopeInfo<D, P>::AllocatedSize() const {
  auto slice = TqRuntimeFieldSliceScopeInfoModuleVariables(*static_cast<const D*>(this));
  return static_cast<int>(std::get<1>(slice)) + 12 * static_cast<int>(std::get<2>(slice));
}

template<class D, class P>
inline TorqueGeneratedScopeInfo<D, P>::TorqueGeneratedScopeInfo(Address ptr)
  : P(ptr) {
  SLOW_DCHECK(IsScopeInfo_NonInline(*this));
}
template<class D, class P>
inline TorqueGeneratedScopeInfo<D, P>::TorqueGeneratedScopeInfo(Address ptr, HeapObject::AllowInlineSmiStorage allow_smi)
  : P(ptr, allow_smi) {
  SLOW_DCHECK((allow_smi == HeapObject::AllowInlineSmiStorage::kAllowBeingASmi && this->IsSmi()) || IsScopeInfo_NonInline(*this));
}
