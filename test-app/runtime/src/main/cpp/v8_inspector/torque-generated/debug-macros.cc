#include "torque-generated/debug-macros.h"

#include "src/objects/swiss-name-dictionary.h"
#include "src/objects/ordered-hash-table.h"
#include "tools/debug_helper/debug-macro-shims.h"
#include "include/v8-internal.h"

namespace v8 {
namespace internal {
namespace debug_helper_internal {

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceContextElements
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceContextElements
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceContextElements(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_Object_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceContextElements

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceFixedArrayObjects
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceFixedArrayObjects
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceFixedArrayObjects(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_Object_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceFixedArrayObjects

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceByteArrayBytes
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceByteArrayBytes
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceByteArrayBytes(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceByteArrayBytes

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceWeakFixedArrayObjects
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceWeakFixedArrayObjects
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceWeakFixedArrayObjects(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_MaybeObject_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceWeakFixedArrayObjects

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSloppyArgumentsElementsMappedEntries
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSloppyArgumentsElementsMappedEntries
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSloppyArgumentsElementsMappedEntries(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 16));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_Smi_OR_TheHole_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSloppyArgumentsElementsMappedEntries

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoContextLocalNames
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoContextLocalNames
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoContextLocalNames(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 12));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 16));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_String_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoContextLocalNames

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoContextLocalInfos
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoContextLocalInfos
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoContextLocalInfos(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  uintptr_t tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoContextLocalNames(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 12));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_Smi_0(accessor, tmp7));
  ASSIGN_OR_RETURN(std::tie(tmp9, tmp10, tmp11), TqDebugNewMutableSlice_SmiTagged_VariableProperties_0(accessor, p_o, tmp5, tmp8));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp9, tmp10, tmp11)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoContextLocalInfos

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoSavedClassVariableInfo
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoSavedClassVariableInfo
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoSavedClassVariableInfo(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  bool tmp8{}; USE(tmp8);
  int31_t tmp9{}; USE(tmp9);
  int31_t tmp10{}; USE(tmp10);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp11{}; USE(tmp11);
  uintptr_t tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoContextLocalInfos(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<bool, 10, 1, uint32_t>::decode(Internals::SmiValue(tmp7)));
  if (tmp8) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 1));
  phi_bb4_8 = tmp9;
  goto block4;

  block3:
  ASSIGN_OR_RETURN(tmp10, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp10;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp11, TqDebugConvert_intptr_int31_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp12, tmp13, tmp14), TqDebugNewMutableSlice_Smi_0(accessor, p_o, tmp5, tmp11));
  goto block6;

  block6:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp12, tmp13, tmp14)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoSavedClassVariableInfo

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoReceiverInfo
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoReceiverInfo
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoReceiverInfo(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  bool tmp11{}; USE(tmp11);
  intptr_t tmp12{}; USE(tmp12);
  uintptr_t tmp13{}; USE(tmp13);
  uint32_t tmp14{}; USE(tmp14);
  uint32_t tmp15{}; USE(tmp15);
  bool tmp16{}; USE(tmp16);
  bool phi_bb8_9{}; USE(phi_bb8_9);
  int31_t tmp17{}; USE(tmp17);
  int31_t tmp18{}; USE(tmp18);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp19{}; USE(tmp19);
  uintptr_t tmp20{}; USE(tmp20);
  intptr_t tmp21{}; USE(tmp21);
  intptr_t tmp22{}; USE(tmp22);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoSavedClassVariableInfo(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<VariableAllocationInfo, 7, 2, uint32_t>::decode(Internals::SmiValue(tmp7)));
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_VariableAllocationInfo_constexpr_STACK_0(accessor, VariableAllocationInfo::STACK));
  ASSIGN_OR_RETURN(tmp10, TorqueDebugMacroShims::CodeStubAssembler::Word32Equal(accessor, tmp8, tmp9));
  if (tmp10) {
    goto block6;
  } else {
    goto block7;
  }

  block6:
  ASSIGN_OR_RETURN(tmp11, TqDebugFromConstexpr_bool_constexpr_bool_0(accessor, true));
  phi_bb8_9 = tmp11;
  goto block8;

  block7:
  ASSIGN_OR_RETURN(tmp12, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp13, accessor, p_o, static_cast<int>(tmp12));
  tmp14 = CastToUnderlyingTypeIfEnum(base::BitField<VariableAllocationInfo, 7, 2, uint32_t>::decode(Internals::SmiValue(tmp13)));
  ASSIGN_OR_RETURN(tmp15, TqDebugFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0(accessor, VariableAllocationInfo::CONTEXT));
  ASSIGN_OR_RETURN(tmp16, TorqueDebugMacroShims::CodeStubAssembler::Word32Equal(accessor, tmp14, tmp15));
  phi_bb8_9 = tmp16;
  goto block8;

  block8:
  if (phi_bb8_9) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(tmp17, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 1));
  phi_bb4_8 = tmp17;
  goto block4;

  block3:
  ASSIGN_OR_RETURN(tmp18, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp18;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp19, TqDebugConvert_intptr_int31_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp20, tmp21, tmp22), TqDebugNewMutableSlice_Smi_0(accessor, p_o, tmp5, tmp19));
  goto block9;

  block9:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp20, tmp21, tmp22)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoReceiverInfo

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoFunctionVariableInfo
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoFunctionVariableInfo
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoFunctionVariableInfo(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  int31_t tmp11{}; USE(tmp11);
  int31_t tmp12{}; USE(tmp12);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp13{}; USE(tmp13);
  uintptr_t tmp14{}; USE(tmp14);
  intptr_t tmp15{}; USE(tmp15);
  intptr_t tmp16{}; USE(tmp16);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoReceiverInfo(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<VariableAllocationInfo, 12, 2, uint32_t>::decode(Internals::SmiValue(tmp7)));
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_VariableAllocationInfo_constexpr_NONE_0(accessor, VariableAllocationInfo::NONE));
  ASSIGN_OR_RETURN(tmp10, TorqueDebugMacroShims::CodeStubAssembler::Word32NotEqual(accessor, tmp8, tmp9));
  if (tmp10) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(tmp11, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 1));
  phi_bb4_8 = tmp11;
  goto block4;

  block3:
  ASSIGN_OR_RETURN(tmp12, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp12;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp13, TqDebugConvert_intptr_int31_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp14, tmp15, tmp16), TqDebugNewMutableSlice_FunctionVariableInfo_0(accessor, p_o, tmp5, tmp13));
  goto block6;

  block6:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp14, tmp15, tmp16)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoFunctionVariableInfo

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoInferredFunctionName
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoInferredFunctionName
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoInferredFunctionName(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  bool tmp8{}; USE(tmp8);
  int31_t tmp9{}; USE(tmp9);
  int31_t tmp10{}; USE(tmp10);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp11{}; USE(tmp11);
  uintptr_t tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoFunctionVariableInfo(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<bool, 14, 1, uint32_t>::decode(Internals::SmiValue(tmp7)));
  if (tmp8) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 1));
  phi_bb4_8 = tmp9;
  goto block4;

  block3:
  ASSIGN_OR_RETURN(tmp10, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp10;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp11, TqDebugConvert_intptr_int31_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp12, tmp13, tmp14), TqDebugNewMutableSlice_String_OR_Undefined_0(accessor, p_o, tmp5, tmp11));
  goto block6;

  block6:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp12, tmp13, tmp14)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoInferredFunctionName

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoPositionInfo
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoPositionInfo
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoPositionInfo(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  bool tmp11{}; USE(tmp11);
  intptr_t tmp12{}; USE(tmp12);
  uintptr_t tmp13{}; USE(tmp13);
  uint32_t tmp14{}; USE(tmp14);
  uint32_t tmp15{}; USE(tmp15);
  bool tmp16{}; USE(tmp16);
  bool phi_bb8_9{}; USE(phi_bb8_9);
  bool tmp17{}; USE(tmp17);
  intptr_t tmp18{}; USE(tmp18);
  uintptr_t tmp19{}; USE(tmp19);
  uint32_t tmp20{}; USE(tmp20);
  uint32_t tmp21{}; USE(tmp21);
  bool tmp22{}; USE(tmp22);
  bool phi_bb11_9{}; USE(phi_bb11_9);
  bool tmp23{}; USE(tmp23);
  intptr_t tmp24{}; USE(tmp24);
  uintptr_t tmp25{}; USE(tmp25);
  uint32_t tmp26{}; USE(tmp26);
  uint32_t tmp27{}; USE(tmp27);
  bool tmp28{}; USE(tmp28);
  bool phi_bb14_9{}; USE(phi_bb14_9);
  int31_t tmp29{}; USE(tmp29);
  int31_t tmp30{}; USE(tmp30);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp31{}; USE(tmp31);
  uintptr_t tmp32{}; USE(tmp32);
  intptr_t tmp33{}; USE(tmp33);
  intptr_t tmp34{}; USE(tmp34);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoInferredFunctionName(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(Internals::SmiValue(tmp7)));
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_uint32_constexpr_uint32_0(accessor, ScopeType::FUNCTION_SCOPE));
  ASSIGN_OR_RETURN(tmp10, TorqueDebugMacroShims::CodeStubAssembler::Word32Equal(accessor, tmp8, tmp9));
  if (tmp10) {
    goto block6;
  } else {
    goto block7;
  }

  block6:
  ASSIGN_OR_RETURN(tmp11, TqDebugFromConstexpr_bool_constexpr_bool_0(accessor, true));
  phi_bb8_9 = tmp11;
  goto block8;

  block7:
  ASSIGN_OR_RETURN(tmp12, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp13, accessor, p_o, static_cast<int>(tmp12));
  tmp14 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(Internals::SmiValue(tmp13)));
  ASSIGN_OR_RETURN(tmp15, TqDebugFromConstexpr_uint32_constexpr_uint32_0(accessor, ScopeType::SCRIPT_SCOPE));
  ASSIGN_OR_RETURN(tmp16, TorqueDebugMacroShims::CodeStubAssembler::Word32Equal(accessor, tmp14, tmp15));
  phi_bb8_9 = tmp16;
  goto block8;

  block8:
  if (phi_bb8_9) {
    goto block9;
  } else {
    goto block10;
  }

  block9:
  ASSIGN_OR_RETURN(tmp17, TqDebugFromConstexpr_bool_constexpr_bool_0(accessor, true));
  phi_bb11_9 = tmp17;
  goto block11;

  block10:
  ASSIGN_OR_RETURN(tmp18, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp19, accessor, p_o, static_cast<int>(tmp18));
  tmp20 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(Internals::SmiValue(tmp19)));
  ASSIGN_OR_RETURN(tmp21, TqDebugFromConstexpr_uint32_constexpr_uint32_0(accessor, ScopeType::EVAL_SCOPE));
  ASSIGN_OR_RETURN(tmp22, TorqueDebugMacroShims::CodeStubAssembler::Word32Equal(accessor, tmp20, tmp21));
  phi_bb11_9 = tmp22;
  goto block11;

  block11:
  if (phi_bb11_9) {
    goto block12;
  } else {
    goto block13;
  }

  block12:
  ASSIGN_OR_RETURN(tmp23, TqDebugFromConstexpr_bool_constexpr_bool_0(accessor, true));
  phi_bb14_9 = tmp23;
  goto block14;

  block13:
  ASSIGN_OR_RETURN(tmp24, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp25, accessor, p_o, static_cast<int>(tmp24));
  tmp26 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(Internals::SmiValue(tmp25)));
  ASSIGN_OR_RETURN(tmp27, TqDebugFromConstexpr_uint32_constexpr_uint32_0(accessor, ScopeType::MODULE_SCOPE));
  ASSIGN_OR_RETURN(tmp28, TorqueDebugMacroShims::CodeStubAssembler::Word32Equal(accessor, tmp26, tmp27));
  phi_bb14_9 = tmp28;
  goto block14;

  block14:
  if (phi_bb14_9) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(tmp29, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 1));
  phi_bb4_8 = tmp29;
  goto block4;

  block3:
  ASSIGN_OR_RETURN(tmp30, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp30;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp31, TqDebugConvert_intptr_int31_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp32, tmp33, tmp34), TqDebugNewMutableSlice_PositionInfo_0(accessor, p_o, tmp5, tmp31));
  goto block15;

  block15:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp32, tmp33, tmp34)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoPositionInfo

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoOuterScopeInfo
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoOuterScopeInfo
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoOuterScopeInfo(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  bool tmp8{}; USE(tmp8);
  int31_t tmp9{}; USE(tmp9);
  int31_t tmp10{}; USE(tmp10);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp11{}; USE(tmp11);
  uintptr_t tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoPositionInfo(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<bool, 22, 1, uint32_t>::decode(Internals::SmiValue(tmp7)));
  if (tmp8) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 1));
  phi_bb4_8 = tmp9;
  goto block4;

  block3:
  ASSIGN_OR_RETURN(tmp10, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp10;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp11, TqDebugConvert_intptr_int31_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp12, tmp13, tmp14), TqDebugNewMutableSlice_TheHole_OR_ScopeInfo_0(accessor, p_o, tmp5, tmp11));
  goto block6;

  block6:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp12, tmp13, tmp14)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoOuterScopeInfo

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoLocalsBlockList
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoLocalsBlockList
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoLocalsBlockList(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  bool tmp8{}; USE(tmp8);
  int31_t tmp9{}; USE(tmp9);
  int31_t tmp10{}; USE(tmp10);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp11{}; USE(tmp11);
  uintptr_t tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoOuterScopeInfo(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<bool, 28, 1, uint32_t>::decode(Internals::SmiValue(tmp7)));
  if (tmp8) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 1));
  phi_bb4_8 = tmp9;
  goto block4;

  block3:
  ASSIGN_OR_RETURN(tmp10, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp10;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp11, TqDebugConvert_intptr_int31_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp12, tmp13, tmp14), TqDebugNewMutableSlice_HashTable_0(accessor, p_o, tmp5, tmp11));
  goto block6;

  block6:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp12, tmp13, tmp14)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoLocalsBlockList

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoModuleInfo
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoModuleInfo
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoModuleInfo(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  int31_t tmp11{}; USE(tmp11);
  int31_t tmp12{}; USE(tmp12);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp13{}; USE(tmp13);
  uintptr_t tmp14{}; USE(tmp14);
  intptr_t tmp15{}; USE(tmp15);
  intptr_t tmp16{}; USE(tmp16);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoLocalsBlockList(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(Internals::SmiValue(tmp7)));
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_uint32_constexpr_uint32_0(accessor, ScopeType::MODULE_SCOPE));
  ASSIGN_OR_RETURN(tmp10, TorqueDebugMacroShims::CodeStubAssembler::Word32Equal(accessor, tmp8, tmp9));
  if (tmp10) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(tmp11, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 1));
  phi_bb4_8 = tmp11;
  goto block4;

  block3:
  ASSIGN_OR_RETURN(tmp12, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp12;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp13, TqDebugConvert_intptr_int31_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp14, tmp15, tmp16), TqDebugNewMutableSlice_SourceTextModuleInfo_0(accessor, p_o, tmp5, tmp13));
  goto block6;

  block6:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp14, tmp15, tmp16)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoModuleInfo

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoModuleVariableCount
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoModuleVariableCount
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoModuleVariableCount(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  int31_t tmp11{}; USE(tmp11);
  int31_t tmp12{}; USE(tmp12);
  int31_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp13{}; USE(tmp13);
  uintptr_t tmp14{}; USE(tmp14);
  intptr_t tmp15{}; USE(tmp15);
  intptr_t tmp16{}; USE(tmp16);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoModuleInfo(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(Internals::SmiValue(tmp7)));
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_uint32_constexpr_uint32_0(accessor, ScopeType::MODULE_SCOPE));
  ASSIGN_OR_RETURN(tmp10, TorqueDebugMacroShims::CodeStubAssembler::Word32Equal(accessor, tmp8, tmp9));
  if (tmp10) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(tmp11, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 1));
  phi_bb4_8 = tmp11;
  goto block4;

  block3:
  ASSIGN_OR_RETURN(tmp12, TqDebugFromConstexpr_int31_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp12;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp13, TqDebugConvert_intptr_int31_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp14, tmp15, tmp16), TqDebugNewConstSlice_Smi_0(accessor, p_o, tmp5, tmp13));
  goto block6;

  block6:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp14, tmp15, tmp16)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoModuleVariableCount

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoModuleVariables
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoModuleVariables
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceScopeInfoModuleVariables(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  uint32_t tmp8{}; USE(tmp8);
  uint32_t tmp9{}; USE(tmp9);
  bool tmp10{}; USE(tmp10);
  uintptr_t tmp11{}; USE(tmp11);
  intptr_t tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  uintptr_t tmp15{}; USE(tmp15);
  uintptr_t tmp16{}; USE(tmp16);
  bool tmp17{}; USE(tmp17);
  intptr_t tmp18{}; USE(tmp18);
  intptr_t tmp19{}; USE(tmp19);
  uintptr_t tmp20{}; USE(tmp20);
  intptr_t tmp21{}; USE(tmp21);
  uintptr_t tmp22{}; USE(tmp22);
  uintptr_t tmp23{}; USE(tmp23);
  uintptr_t phi_bb4_8{}; USE(phi_bb4_8);
  intptr_t tmp24{}; USE(tmp24);
  uintptr_t tmp25{}; USE(tmp25);
  intptr_t tmp26{}; USE(tmp26);
  intptr_t tmp27{}; USE(tmp27);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceScopeInfoModuleVariableCount(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  tmp8 = CastToUnderlyingTypeIfEnum(base::BitField<ScopeType, 0, 4, uint32_t>::decode(Internals::SmiValue(tmp7)));
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_uint32_constexpr_uint32_0(accessor, ScopeType::MODULE_SCOPE));
  ASSIGN_OR_RETURN(tmp10, TorqueDebugMacroShims::CodeStubAssembler::Word32Equal(accessor, tmp8, tmp9));
  if (tmp10) {
    goto block2;
  } else {
    goto block3;
  }

  block2:
  ASSIGN_OR_RETURN(std::tie(tmp11, tmp12, tmp13), TqDebugFieldSliceScopeInfoModuleVariableCount(accessor, p_o));
  ASSIGN_OR_RETURN(tmp14, TqDebugConvert_intptr_constexpr_int31_0(accessor, 0));
  ASSIGN_OR_RETURN(tmp15, TqDebugConvert_uintptr_intptr_0(accessor, tmp14));
  ASSIGN_OR_RETURN(tmp16, TqDebugConvert_uintptr_intptr_0(accessor, tmp13));
  ASSIGN_OR_RETURN(tmp17, TorqueDebugMacroShims::CodeStubAssembler::UintPtrLessThan(accessor, tmp15, tmp16));
  if (tmp17) {
    goto block10;
  } else {
    goto block11;
  }

  block10:
  ASSIGN_OR_RETURN(tmp18, TqDebugTimesSizeOf_Smi_0(accessor, tmp14));
  ASSIGN_OR_RETURN(tmp19, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp12, tmp18));
  ASSIGN_OR_RETURN(std::tie(tmp20, tmp21), TqDebugNewReference_Smi_0(accessor, tmp11, tmp19));
  READ_TAGGED_FIELD_OR_FAIL(tmp22, accessor, tmp20, static_cast<int>(tmp21));
  phi_bb4_8 = tmp22;
  goto block4;

  block11:
  UNREACHABLE();

  block3:
  ASSIGN_OR_RETURN(tmp23, TqDebugFromConstexpr_Smi_constexpr_int31_0(accessor, 0));
  phi_bb4_8 = tmp23;
  goto block4;

  block4:
  ASSIGN_OR_RETURN(tmp24, TqDebugConvert_intptr_Smi_0(accessor, phi_bb4_8));
  ASSIGN_OR_RETURN(std::tie(tmp25, tmp26, tmp27), TqDebugNewMutableSlice_ModuleVariable_0(accessor, p_o, tmp5, tmp24));
  goto block13;

  block13:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp25, tmp26, tmp27)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceScopeInfoModuleVariables

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceFixedDoubleArrayFloats
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceFixedDoubleArrayFloats
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceFixedDoubleArrayFloats(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_float64_or_hole_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceFixedDoubleArrayFloats

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceCoverageInfoSlots
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceCoverageInfoSlots
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceCoverageInfoSlots(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  int32_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_FIELD_OR_FAIL(int32_t, tmp1, accessor, p_o, tmp0);
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_int32_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_CoverageInfoSlot_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceCoverageInfoSlots

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceDescriptorArrayDescriptors
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceDescriptorArrayDescriptors
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceDescriptorArrayDescriptors(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uint16_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_FIELD_OR_FAIL(uint16_t, tmp1, accessor, p_o, tmp0);
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_uint16_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 16));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_DescriptorEntry_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceDescriptorArrayDescriptors

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceFeedbackVectorRawFeedbackSlots
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceFeedbackVectorRawFeedbackSlots
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceFeedbackVectorRawFeedbackSlots(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  int32_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_FIELD_OR_FAIL(int32_t, tmp1, accessor, p_o, tmp0);
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_int32_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 32));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_MaybeObject_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceFeedbackVectorRawFeedbackSlots

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceWeakArrayListObjects
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceWeakArrayListObjects
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceWeakArrayListObjects(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 12));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_MaybeObject_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceWeakArrayListObjects

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetPadding
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetPadding
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedHashSetPadding(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  uintptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugConvert_intptr_constexpr_int31_0(accessor, 1));
  ASSIGN_OR_RETURN(tmp1, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 7));
  ASSIGN_OR_RETURN(std::tie(tmp2, tmp3, tmp4), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp1, tmp0));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp2, tmp3, tmp4)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetPadding

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetDataTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetDataTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedHashSetDataTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uint8_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 6));
  READ_FIELD_OR_FAIL(uint8_t, tmp1, accessor, p_o, tmp0);
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_uint8_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, SmallOrderedHashTable<int>::kLoadFactor));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp2, tmp3));
  ASSIGN_OR_RETURN(tmp5, TqDebugConvert_intptr_intptr_0(accessor, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(std::tie(tmp7, tmp8, tmp9), TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(accessor, p_o, tmp6, tmp5));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp7, tmp8, tmp9)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetDataTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetHashTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetHashTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedHashSetHashTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  uintptr_t tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceSmallOrderedHashSetDataTable(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 6));
  READ_FIELD_OR_FAIL(uint8_t, tmp7, accessor, p_o, tmp6);
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_uint8_0(accessor, tmp7));
  ASSIGN_OR_RETURN(std::tie(tmp9, tmp10, tmp11), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp5, tmp8));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp9, tmp10, tmp11)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetHashTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetChainTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetChainTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedHashSetChainTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
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
  uintptr_t tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceSmallOrderedHashSetHashTable(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 1));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 6));
  READ_FIELD_OR_FAIL(uint8_t, tmp7, accessor, p_o, tmp6);
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_uint8_0(accessor, tmp7));
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, SmallOrderedHashTable<int>::kLoadFactor));
  ASSIGN_OR_RETURN(tmp10, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp8, tmp9));
  ASSIGN_OR_RETURN(tmp11, TqDebugConvert_intptr_intptr_0(accessor, tmp10));
  ASSIGN_OR_RETURN(std::tie(tmp12, tmp13, tmp14), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp5, tmp11));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp12, tmp13, tmp14)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashSetChainTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapPadding
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapPadding
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedHashMapPadding(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  uintptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugConvert_intptr_constexpr_int31_0(accessor, 1));
  ASSIGN_OR_RETURN(tmp1, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 7));
  ASSIGN_OR_RETURN(std::tie(tmp2, tmp3, tmp4), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp1, tmp0));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp2, tmp3, tmp4)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapPadding

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapDataTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapDataTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedHashMapDataTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uint8_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 6));
  READ_FIELD_OR_FAIL(uint8_t, tmp1, accessor, p_o, tmp0);
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_uint8_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, SmallOrderedHashTable<int>::kLoadFactor));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp2, tmp3));
  ASSIGN_OR_RETURN(tmp5, TqDebugConvert_intptr_intptr_0(accessor, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(std::tie(tmp7, tmp8, tmp9), TqDebugNewMutableSlice_HashMapEntry_0(accessor, p_o, tmp6, tmp5));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp7, tmp8, tmp9)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapDataTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapHashTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapHashTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedHashMapHashTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  uintptr_t tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceSmallOrderedHashMapDataTable(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 6));
  READ_FIELD_OR_FAIL(uint8_t, tmp7, accessor, p_o, tmp6);
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_uint8_0(accessor, tmp7));
  ASSIGN_OR_RETURN(std::tie(tmp9, tmp10, tmp11), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp5, tmp8));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp9, tmp10, tmp11)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapHashTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapChainTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapChainTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedHashMapChainTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
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
  uintptr_t tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceSmallOrderedHashMapHashTable(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 1));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 6));
  READ_FIELD_OR_FAIL(uint8_t, tmp7, accessor, p_o, tmp6);
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_uint8_0(accessor, tmp7));
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, SmallOrderedHashTable<int>::kLoadFactor));
  ASSIGN_OR_RETURN(tmp10, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp8, tmp9));
  ASSIGN_OR_RETURN(tmp11, TqDebugConvert_intptr_intptr_0(accessor, tmp10));
  ASSIGN_OR_RETURN(std::tie(tmp12, tmp13, tmp14), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp5, tmp11));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp12, tmp13, tmp14)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedHashMapChainTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryPadding1
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryPadding1
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedNameDictionaryPadding1(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  uintptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugConvert_intptr_constexpr_int31_0(accessor, 1));
  ASSIGN_OR_RETURN(tmp1, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 11));
  ASSIGN_OR_RETURN(std::tie(tmp2, tmp3, tmp4), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp1, tmp0));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp2, tmp3, tmp4)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryPadding1

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryDataTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryDataTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedNameDictionaryDataTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uint8_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 10));
  READ_FIELD_OR_FAIL(uint8_t, tmp1, accessor, p_o, tmp0);
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_uint8_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, SmallOrderedHashTable<int>::kLoadFactor));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp2, tmp3));
  ASSIGN_OR_RETURN(tmp5, TqDebugConvert_intptr_intptr_0(accessor, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 12));
  ASSIGN_OR_RETURN(std::tie(tmp7, tmp8, tmp9), TqDebugNewMutableSlice_NameDictionaryEntry_0(accessor, p_o, tmp6, tmp5));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp7, tmp8, tmp9)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryDataTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryHashTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryHashTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedNameDictionaryHashTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  uintptr_t tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceSmallOrderedNameDictionaryDataTable(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 12));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 10));
  READ_FIELD_OR_FAIL(uint8_t, tmp7, accessor, p_o, tmp6);
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_uint8_0(accessor, tmp7));
  ASSIGN_OR_RETURN(std::tie(tmp9, tmp10, tmp11), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp5, tmp8));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp9, tmp10, tmp11)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryHashTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryChainTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryChainTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSmallOrderedNameDictionaryChainTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uint8_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  uintptr_t tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceSmallOrderedNameDictionaryHashTable(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 1));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 10));
  READ_FIELD_OR_FAIL(uint8_t, tmp7, accessor, p_o, tmp6);
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_uint8_0(accessor, tmp7));
  ASSIGN_OR_RETURN(std::tie(tmp9, tmp10, tmp11), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp5, tmp8));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp9, tmp10, tmp11)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSmallOrderedNameDictionaryChainTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSeqOneByteStringChars
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSeqOneByteStringChars
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSeqOneByteStringChars(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  int32_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  READ_FIELD_OR_FAIL(int32_t, tmp1, accessor, p_o, tmp0);
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_int32_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 12));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewConstSlice_char8_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSeqOneByteStringChars

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSeqTwoByteStringChars
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSeqTwoByteStringChars
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSeqTwoByteStringChars(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  int32_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  READ_FIELD_OR_FAIL(int32_t, tmp1, accessor, p_o, tmp0);
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_int32_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 12));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewConstSlice_char16_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSeqTwoByteStringChars

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSwissNameDictionaryDataTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSwissNameDictionaryDataTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSwissNameDictionaryDataTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  int32_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  READ_FIELD_OR_FAIL(int32_t, tmp1, accessor, p_o, tmp0);
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_int32_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 2));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp2, tmp3));
  ASSIGN_OR_RETURN(tmp5, TqDebugConvert_intptr_intptr_0(accessor, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 16));
  ASSIGN_OR_RETURN(std::tie(tmp7, tmp8, tmp9), TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(accessor, p_o, tmp6, tmp5));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp7, tmp8, tmp9)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSwissNameDictionaryDataTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSwissNameDictionaryCtrlTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSwissNameDictionaryCtrlTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSwissNameDictionaryCtrlTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
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
  uintptr_t tmp12{}; USE(tmp12);
  intptr_t tmp13{}; USE(tmp13);
  intptr_t tmp14{}; USE(tmp14);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceSwissNameDictionaryDataTable(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  READ_FIELD_OR_FAIL(int32_t, tmp7, accessor, p_o, tmp6);
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_int32_0(accessor, tmp7));
  ASSIGN_OR_RETURN(tmp9, TqDebugFromConstexpr_intptr_constexpr_int32_0(accessor, swiss_table::Group::kWidth));
  ASSIGN_OR_RETURN(tmp10, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp8, tmp9));
  ASSIGN_OR_RETURN(tmp11, TqDebugConvert_intptr_intptr_0(accessor, tmp10));
  ASSIGN_OR_RETURN(std::tie(tmp12, tmp13, tmp14), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp5, tmp11));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp12, tmp13, tmp14)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSwissNameDictionaryCtrlTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceSwissNameDictionaryPropertyDetailsTable
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceSwissNameDictionaryPropertyDetailsTable
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceSwissNameDictionaryPropertyDetailsTable(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  int32_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  intptr_t tmp9{}; USE(tmp9);
  uintptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  intptr_t tmp12{}; USE(tmp12);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceSwissNameDictionaryCtrlTable(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 1));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 8));
  READ_FIELD_OR_FAIL(int32_t, tmp7, accessor, p_o, tmp6);
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_int32_0(accessor, tmp7));
  ASSIGN_OR_RETURN(tmp9, TqDebugConvert_intptr_intptr_0(accessor, tmp8));
  ASSIGN_OR_RETURN(std::tie(tmp10, tmp11, tmp12), TqDebugNewMutableSlice_uint8_0(accessor, p_o, tmp5, tmp9));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp10, tmp11, tmp12)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceSwissNameDictionaryPropertyDetailsTable

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceInternalClassWithSmiElementsEntries
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceInternalClassWithSmiElementsEntries
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceInternalClassWithSmiElementsEntries(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 16));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_Smi_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceInternalClassWithSmiElementsEntries

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceInternalClassWithStructElementsEntries
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceInternalClassWithStructElementsEntries
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceInternalClassWithStructElementsEntries(d::MemoryAccessor accessor, uintptr_t p_o) {
  intptr_t tmp0{}; USE(tmp0);
  uintptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  uintptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 12));
  READ_TAGGED_FIELD_OR_FAIL(tmp1, accessor, p_o, static_cast<int>(tmp0));
  ASSIGN_OR_RETURN(tmp2, TqDebugConvert_intptr_Smi_0(accessor, tmp1));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 24));
  ASSIGN_OR_RETURN(std::tie(tmp4, tmp5, tmp6), TqDebugNewMutableSlice_Smi_0(accessor, p_o, tmp3, tmp2));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp4, tmp5, tmp6)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceInternalClassWithStructElementsEntries

#ifndef V8_INTERNAL_DEFINED_TqDebugFieldSliceInternalClassWithStructElementsMoreEntries
#define V8_INTERNAL_DEFINED_TqDebugFieldSliceInternalClassWithStructElementsMoreEntries
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugFieldSliceInternalClassWithStructElementsMoreEntries(d::MemoryAccessor accessor, uintptr_t p_o) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  intptr_t tmp3{}; USE(tmp3);
  intptr_t tmp4{}; USE(tmp4);
  intptr_t tmp5{}; USE(tmp5);
  intptr_t tmp6{}; USE(tmp6);
  uintptr_t tmp7{}; USE(tmp7);
  intptr_t tmp8{}; USE(tmp8);
  uintptr_t tmp9{}; USE(tmp9);
  intptr_t tmp10{}; USE(tmp10);
  intptr_t tmp11{}; USE(tmp11);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(std::tie(tmp0, tmp1, tmp2), TqDebugFieldSliceInternalClassWithStructElementsEntries(accessor, p_o));
  ASSIGN_OR_RETURN(tmp3, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 4));
  ASSIGN_OR_RETURN(tmp4, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, tmp3, tmp2));
  ASSIGN_OR_RETURN(tmp5, TorqueDebugMacroShims::CodeStubAssembler::IntPtrAdd(accessor, tmp1, tmp4));
  ASSIGN_OR_RETURN(tmp6, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, 12));
  READ_TAGGED_FIELD_OR_FAIL(tmp7, accessor, p_o, static_cast<int>(tmp6));
  ASSIGN_OR_RETURN(tmp8, TqDebugConvert_intptr_Smi_0(accessor, tmp7));
  ASSIGN_OR_RETURN(std::tie(tmp9, tmp10, tmp11), TqDebugNewMutableSlice_InternalClassStructElement_0(accessor, p_o, tmp5, tmp8));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp9, tmp10, tmp11)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFieldSliceInternalClassWithStructElementsMoreEntries

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
Value<intptr_t> TqDebugConvert_intptr_Smi_0(d::MemoryAccessor accessor, uintptr_t p_s) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::SmiUntag(accessor, p_s));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Object_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Object_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_Object_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Object_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
Value<intptr_t> TqDebugConvert_intptr_Smi_0(d::MemoryAccessor accessor, uintptr_t p_s) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::SmiUntag(accessor, p_s));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Object_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Object_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_Object_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Object_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_uint8_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_uint8_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_uint8_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_uint8_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_MaybeObject_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_MaybeObject_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_MaybeObject_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_MaybeObject_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
Value<intptr_t> TqDebugConvert_intptr_Smi_0(d::MemoryAccessor accessor, uintptr_t p_s) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::SmiUntag(accessor, p_s));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Smi_OR_TheHole_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Smi_OR_TheHole_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_Smi_OR_TheHole_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Smi_OR_TheHole_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
Value<intptr_t> TqDebugConvert_intptr_Smi_0(d::MemoryAccessor accessor, uintptr_t p_s) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::SmiUntag(accessor, p_s));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_String_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_String_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_String_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_String_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_SmiTagged_VariableProperties_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_SmiTagged_VariableProperties_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_SmiTagged_VariableProperties_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_SmiTagged_VariableProperties_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_int31_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_int31_constexpr_int31_0
Value<int31_t> TqDebugFromConstexpr_int31_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  int31_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_int31_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int31_0
Value<intptr_t> TqDebugConvert_intptr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::ChangeInt32ToIntPtr(accessor, p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Smi_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_Smi_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_VariableAllocationInfo_constexpr_STACK_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_VariableAllocationInfo_constexpr_STACK_0
Value<uint32_t> TqDebugFromConstexpr_VariableAllocationInfo_constexpr_STACK_0(d::MemoryAccessor accessor, VariableAllocationInfo p_o) {
  uint32_t tmp0{}; USE(tmp0);
  uint32_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_o));
  tmp1 = (tmp0);
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp1};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_VariableAllocationInfo_constexpr_STACK_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_bool_constexpr_bool_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_bool_constexpr_bool_0
Value<bool> TqDebugFromConstexpr_bool_constexpr_bool_0(d::MemoryAccessor accessor, bool p_b) {
  bool tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::BoolConstant(accessor, p_b));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_bool_constexpr_bool_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0
Value<uint32_t> TqDebugFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0(d::MemoryAccessor accessor, VariableAllocationInfo p_o) {
  uint32_t tmp0{}; USE(tmp0);
  uint32_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_o));
  tmp1 = (tmp0);
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp1};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_VariableAllocationInfo_constexpr_CONTEXT_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_VariableAllocationInfo_constexpr_NONE_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_VariableAllocationInfo_constexpr_NONE_0
Value<uint32_t> TqDebugFromConstexpr_VariableAllocationInfo_constexpr_NONE_0(d::MemoryAccessor accessor, VariableAllocationInfo p_o) {
  uint32_t tmp0{}; USE(tmp0);
  uint32_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_o));
  tmp1 = (tmp0);
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp1};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_VariableAllocationInfo_constexpr_NONE_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_FunctionVariableInfo_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_FunctionVariableInfo_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_FunctionVariableInfo_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_FunctionVariableInfo_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_String_OR_Undefined_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_String_OR_Undefined_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_String_OR_Undefined_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_String_OR_Undefined_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_uint32_constexpr_uint32_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_uint32_constexpr_uint32_0
Value<uint32_t> TqDebugFromConstexpr_uint32_constexpr_uint32_0(d::MemoryAccessor accessor, uint32_t p_i) {
  int32_t tmp0{}; USE(tmp0);
  uint32_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  ASSIGN_OR_RETURN(tmp1, TorqueDebugMacroShims::CodeStubAssembler::Unsigned(accessor, tmp0));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp1};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_uint32_constexpr_uint32_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_PositionInfo_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_PositionInfo_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_PositionInfo_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_PositionInfo_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_TheHole_OR_ScopeInfo_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_TheHole_OR_ScopeInfo_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_TheHole_OR_ScopeInfo_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_TheHole_OR_ScopeInfo_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_HashTable_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_HashTable_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_HashTable_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_HashTable_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_SourceTextModuleInfo_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_SourceTextModuleInfo_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_SourceTextModuleInfo_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_SourceTextModuleInfo_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewConstSlice_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugNewConstSlice_Smi_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewConstSlice_Smi_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewConstSlice_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_constexpr_int31_0
Value<intptr_t> TqDebugConvert_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_uintptr_intptr_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_uintptr_intptr_0
Value<uintptr_t> TqDebugConvert_uintptr_intptr_0(d::MemoryAccessor accessor, intptr_t p_i) {
  uintptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::Unsigned(accessor, p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_uintptr_intptr_0

#ifndef V8_INTERNAL_DEFINED_TqDebugTimesSizeOf_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugTimesSizeOf_Smi_0
Value<intptr_t> TqDebugTimesSizeOf_Smi_0(d::MemoryAccessor accessor, intptr_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, kTaggedSize));
  ASSIGN_OR_RETURN(tmp1, TorqueDebugMacroShims::CodeStubAssembler::IntPtrMul(accessor, p_i, tmp0));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp1};
}

#endif // V8_INTERNAL_DEFINED_TqDebugTimesSizeOf_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewReference_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugNewReference_Smi_0
Value<std::tuple<uintptr_t, intptr_t>> TqDebugNewReference_Smi_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  std::tie(tmp0, tmp1) = (std::make_tuple(p_object, p_offset));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewReference_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_Smi_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_Smi_constexpr_int31_0
Value<uintptr_t> TqDebugFromConstexpr_Smi_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  uintptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = Internals::IntToSmi(CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_Smi_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_ModuleVariable_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_ModuleVariable_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_ModuleVariable_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_ModuleVariable_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_float64_or_hole_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_float64_or_hole_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_float64_or_hole_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_float64_or_hole_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0
Value<intptr_t> TqDebugConvert_intptr_int32_0(d::MemoryAccessor accessor, int32_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::ChangeInt32ToIntPtr(accessor, p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_CoverageInfoSlot_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_CoverageInfoSlot_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_CoverageInfoSlot_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_CoverageInfoSlot_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_uint16_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_uint16_0
Value<intptr_t> TqDebugConvert_intptr_uint16_0(d::MemoryAccessor accessor, uint16_t p_ui) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::ChangeUint32ToWord(accessor, p_ui));
  ASSIGN_OR_RETURN(tmp1, TorqueDebugMacroShims::CodeStubAssembler::Signed(accessor, tmp0));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp1};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_uint16_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_DescriptorEntry_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_DescriptorEntry_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_DescriptorEntry_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_DescriptorEntry_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0
Value<intptr_t> TqDebugConvert_intptr_int32_0(d::MemoryAccessor accessor, int32_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::ChangeInt32ToIntPtr(accessor, p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_MaybeObject_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_MaybeObject_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_MaybeObject_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_MaybeObject_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_constexpr_int31_0
Value<intptr_t> TqDebugConvert_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TqDebugFromConstexpr_intptr_constexpr_int31_0(accessor, p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_uint8_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_uint8_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_uint8_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_uint8_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_uint8_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_uint8_0
Value<intptr_t> TqDebugConvert_intptr_uint8_0(d::MemoryAccessor accessor, uint8_t p_ui) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::ChangeUint32ToWord(accessor, p_ui));
  ASSIGN_OR_RETURN(tmp1, TorqueDebugMacroShims::CodeStubAssembler::Signed(accessor, tmp0));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp1};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_uint8_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_intptr_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_intptr_0
Value<intptr_t> TqDebugConvert_intptr_intptr_0(d::MemoryAccessor accessor, intptr_t p_i) {
  goto block0;

  block0:
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, p_i};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_intptr_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_HashMapEntry_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_HashMapEntry_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_HashMapEntry_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_HashMapEntry_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_NameDictionaryEntry_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_NameDictionaryEntry_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_NameDictionaryEntry_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_NameDictionaryEntry_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0
Value<intptr_t> TqDebugConvert_intptr_int32_0(d::MemoryAccessor accessor, int32_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::ChangeInt32ToIntPtr(accessor, p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewConstSlice_char8_0
#define V8_INTERNAL_DEFINED_TqDebugNewConstSlice_char8_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewConstSlice_char8_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewConstSlice_char8_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewConstSlice_char16_0
#define V8_INTERNAL_DEFINED_TqDebugNewConstSlice_char16_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewConstSlice_char16_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewConstSlice_char16_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0
Value<intptr_t> TqDebugConvert_intptr_int32_0(d::MemoryAccessor accessor, int32_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::ChangeInt32ToIntPtr(accessor, p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_int32_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_intptr_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_intptr_0
Value<intptr_t> TqDebugConvert_intptr_intptr_0(d::MemoryAccessor accessor, intptr_t p_i) {
  goto block0;

  block0:
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, p_i};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_intptr_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_JSReceiver_OR_Smi_OR_HeapNumber_OR_BigInt_OR_String_OR_Symbol_OR_True_OR_False_OR_Null_OR_Undefined_OR_TheHole_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int32_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int32_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int32_0(d::MemoryAccessor accessor, int32_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int32_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_uint8_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_uint8_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_uint8_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_uint8_0

#ifndef V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
#define V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0
Value<intptr_t> TqDebugFromConstexpr_intptr_constexpr_int31_0(d::MemoryAccessor accessor, int31_t p_i) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  tmp0 = (CastToUnderlyingTypeIfEnum(p_i));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugFromConstexpr_intptr_constexpr_int31_0

#ifndef V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0
Value<intptr_t> TqDebugConvert_intptr_Smi_0(d::MemoryAccessor accessor, uintptr_t p_s) {
  intptr_t tmp0{}; USE(tmp0);
  goto block0;

  block0:
  ASSIGN_OR_RETURN(tmp0, TorqueDebugMacroShims::CodeStubAssembler::SmiUntag(accessor, p_s));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, tmp0};
}

#endif // V8_INTERNAL_DEFINED_TqDebugConvert_intptr_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Smi_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Smi_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_Smi_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_Smi_0

#ifndef V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_InternalClassStructElement_0
#define V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_InternalClassStructElement_0
Value<std::tuple<uintptr_t, intptr_t, intptr_t>> TqDebugNewMutableSlice_InternalClassStructElement_0(d::MemoryAccessor accessor, uintptr_t p_object, intptr_t p_offset, intptr_t p_length) {
  uintptr_t tmp0{}; USE(tmp0);
  intptr_t tmp1{}; USE(tmp1);
  intptr_t tmp2{}; USE(tmp2);
  goto block0;

  block0:
  std::tie(tmp0, tmp1, tmp2) = (std::make_tuple(p_object, p_offset, p_length));
  goto block2;

  block2:
  return {d::MemoryAccessResult::kOk, std::make_tuple(tmp0, tmp1, tmp2)};
}

#endif // V8_INTERNAL_DEFINED_TqDebugNewMutableSlice_InternalClassStructElement_0

}  // namespace internal
}  // namespace v8
}  // namespace debug_helper_internal

