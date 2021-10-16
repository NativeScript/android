#include "src/wasm/wasm-objects-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsWasmFunctionData_NonInline(HeapObject o) {
  return o.IsWasmFunctionData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmFunctionData<WasmFunctionData, Foreign>::WasmFunctionDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmFunctionDataVerify(WasmFunctionData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmExportedFunctionData_NonInline(HeapObject o) {
  return o.IsWasmExportedFunctionData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmExportedFunctionData<WasmExportedFunctionData, WasmFunctionData>::WasmExportedFunctionDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmExportedFunctionDataVerify(WasmExportedFunctionData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmJSFunctionData_NonInline(HeapObject o) {
  return o.IsWasmJSFunctionData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmJSFunctionData<WasmJSFunctionData, WasmFunctionData>::WasmJSFunctionDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmJSFunctionDataVerify(WasmJSFunctionData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmCapiFunctionData_NonInline(HeapObject o) {
  return o.IsWasmCapiFunctionData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmCapiFunctionData<WasmCapiFunctionData, WasmFunctionData>::WasmCapiFunctionDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmCapiFunctionDataVerify(WasmCapiFunctionData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmIndirectFunctionTable_NonInline(HeapObject o) {
  return o.IsWasmIndirectFunctionTable();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmIndirectFunctionTable<WasmIndirectFunctionTable, Struct>::WasmIndirectFunctionTableVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmIndirectFunctionTableVerify(WasmIndirectFunctionTable::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmExceptionTag_NonInline(HeapObject o) {
  return o.IsWasmExceptionTag();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmExceptionTag<WasmExceptionTag, Struct>::WasmExceptionTagVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmExceptionTagVerify(WasmExceptionTag::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmModuleObject_NonInline(HeapObject o) {
  return o.IsWasmModuleObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmModuleObject<WasmModuleObject, JSObject>::WasmModuleObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmModuleObjectVerify(WasmModuleObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmTableObject_NonInline(HeapObject o) {
  return o.IsWasmTableObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmTableObject<WasmTableObject, JSObject>::WasmTableObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmTableObjectVerify(WasmTableObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmMemoryObject_NonInline(HeapObject o) {
  return o.IsWasmMemoryObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmMemoryObject<WasmMemoryObject, JSObject>::WasmMemoryObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmMemoryObjectVerify(WasmMemoryObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmGlobalObject_NonInline(HeapObject o) {
  return o.IsWasmGlobalObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmGlobalObject<WasmGlobalObject, JSObject>::WasmGlobalObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmGlobalObjectVerify(WasmGlobalObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmExceptionObject_NonInline(HeapObject o) {
  return o.IsWasmExceptionObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmExceptionObject<WasmExceptionObject, JSObject>::WasmExceptionObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmExceptionObjectVerify(WasmExceptionObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsAsmWasmData_NonInline(HeapObject o) {
  return o.IsAsmWasmData();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedAsmWasmData<AsmWasmData, Struct>::AsmWasmDataVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::AsmWasmDataVerify(AsmWasmData::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmTypeInfo_NonInline(HeapObject o) {
  return o.IsWasmTypeInfo();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmTypeInfo<WasmTypeInfo, Foreign>::WasmTypeInfoVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmTypeInfoVerify(WasmTypeInfo::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmObject_NonInline(HeapObject o) {
  return o.IsWasmObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmObject<WasmObject, JSReceiver>::WasmObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmObjectVerify(WasmObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmStruct_NonInline(HeapObject o) {
  return o.IsWasmStruct();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmStruct<WasmStruct, WasmObject>::WasmStructVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmStructVerify(WasmStruct::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsWasmArray_NonInline(HeapObject o) {
  return o.IsWasmArray();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedWasmArray<WasmArray, WasmObject>::WasmArrayVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::WasmArrayVerify(WasmArray::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
