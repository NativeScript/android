// Provides the ability to read object properties in
// postmortem or remote scenarios, where the debuggee's
// memory is not part of the current process's address
// space and must be read using a callback function.

#ifndef V8_GEN_TORQUE_GENERATED_CLASS_DEBUG_READERS_H_
#define V8_GEN_TORQUE_GENERATED_CLASS_DEBUG_READERS_H_

#include <cstdint>
#include <vector>

#include "tools/debug_helper/debug-helper-internal.h"

// Unset a wingdi.h macro that causes conflicts.
#ifdef GetBValue
#undef GetBValue
#endif

namespace v8 {
namespace internal {
namespace debug_helper_internal {

class TqHeapObject : public TqObject {
 public:
  inline TqHeapObject(uintptr_t address) : TqObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetMapAddress() const;
  Value<uintptr_t> GetMapValue(d::MemoryAccessor accessor ) const;
};

class TqContext : public TqHeapObject {
 public:
  inline TqContext(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t> GetLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetElementsAddress() const;
  Value<uintptr_t> GetElementsValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqJSReceiver : public TqHeapObject {
 public:
  inline TqJSReceiver(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetPropertiesOrHashAddress() const;
  Value<uintptr_t> GetPropertiesOrHashValue(d::MemoryAccessor accessor ) const;
};

class TqPrimitiveHeapObject : public TqHeapObject {
 public:
  inline TqPrimitiveHeapObject(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqHeapNumber : public TqPrimitiveHeapObject {
 public:
  inline TqHeapNumber(uintptr_t address) : TqPrimitiveHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetValueAddress() const;
  Value<double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetValueValue(d::MemoryAccessor accessor ) const;
};

class TqBigIntBase : public TqPrimitiveHeapObject {
 public:
  inline TqBigIntBase(uintptr_t address) : TqPrimitiveHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqName : public TqPrimitiveHeapObject {
 public:
  inline TqName(uintptr_t address) : TqPrimitiveHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetRawHashFieldAddress() const;
  Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetRawHashFieldValue(d::MemoryAccessor accessor ) const;
};

class TqString : public TqName {
 public:
  inline TqString(uintptr_t address) : TqName(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetLengthValue(d::MemoryAccessor accessor ) const;
};

class TqSymbol : public TqName {
 public:
  inline TqSymbol(uintptr_t address) : TqName(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetFlagsAddress() const;
  Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDescriptionAddress() const;
  Value<uintptr_t> GetDescriptionValue(d::MemoryAccessor accessor ) const;
};

class TqOddball : public TqPrimitiveHeapObject {
 public:
  inline TqOddball(uintptr_t address) : TqPrimitiveHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetToNumberRawAddress() const;
  Value<double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetToNumberRawValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetToStringAddress() const;
  Value<uintptr_t> GetToStringValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetToNumberAddress() const;
  Value<uintptr_t> GetToNumberValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetTypeOfAddress() const;
  Value<uintptr_t> GetTypeOfValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetKindAddress() const;
  Value<uintptr_t> GetKindValue(d::MemoryAccessor accessor ) const;
};

class TqFixedArrayBase : public TqHeapObject {
 public:
  inline TqFixedArrayBase(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t> GetLengthValue(d::MemoryAccessor accessor ) const;
};

class TqFixedArray : public TqFixedArrayBase {
 public:
  inline TqFixedArray(uintptr_t address) : TqFixedArrayBase(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetObjectsAddress() const;
  Value<uintptr_t> GetObjectsValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqJSObject : public TqJSReceiver {
 public:
  inline TqJSObject(uintptr_t address) : TqJSReceiver(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetElementsAddress() const;
  Value<uintptr_t> GetElementsValue(d::MemoryAccessor accessor ) const;
};

class TqByteArray : public TqFixedArrayBase {
 public:
  inline TqByteArray(uintptr_t address) : TqFixedArrayBase(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetBytesAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetBytesValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqWeakFixedArray : public TqHeapObject {
 public:
  inline TqWeakFixedArray(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t> GetLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetObjectsAddress() const;
  Value<uintptr_t> GetObjectsValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqForeign : public TqHeapObject {
 public:
  inline TqForeign(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetForeignAddressAddress() const;
  Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetForeignAddressValue(d::MemoryAccessor accessor ) const;
};

class TqJSProxy : public TqJSReceiver {
 public:
  inline TqJSProxy(uintptr_t address) : TqJSReceiver(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetTargetAddress() const;
  Value<uintptr_t> GetTargetValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetHandlerAddress() const;
  Value<uintptr_t> GetHandlerValue(d::MemoryAccessor accessor ) const;
};

class TqJSFunctionOrBoundFunction : public TqJSObject {
 public:
  inline TqJSFunctionOrBoundFunction(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqJSFunction : public TqJSFunctionOrBoundFunction {
 public:
  inline TqJSFunction(uintptr_t address) : TqJSFunctionOrBoundFunction(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSharedFunctionInfoAddress() const;
  Value<uintptr_t> GetSharedFunctionInfoValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContextAddress() const;
  Value<uintptr_t> GetContextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFeedbackCellAddress() const;
  Value<uintptr_t> GetFeedbackCellValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCodeAddress() const;
  Value<uintptr_t> GetCodeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPrototypeOrInitialMapAddress() const;
  Value<uintptr_t> GetPrototypeOrInitialMapValue(d::MemoryAccessor accessor ) const;
};

class TqJSBoundFunction : public TqJSFunctionOrBoundFunction {
 public:
  inline TqJSBoundFunction(uintptr_t address) : TqJSFunctionOrBoundFunction(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetBoundTargetFunctionAddress() const;
  Value<uintptr_t> GetBoundTargetFunctionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBoundThisAddress() const;
  Value<uintptr_t> GetBoundThisValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBoundArgumentsAddress() const;
  Value<uintptr_t> GetBoundArgumentsValue(d::MemoryAccessor accessor ) const;
};

class TqJSCustomElementsObject : public TqJSObject {
 public:
  inline TqJSCustomElementsObject(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqJSSpecialObject : public TqJSCustomElementsObject {
 public:
  inline TqJSSpecialObject(uintptr_t address) : TqJSCustomElementsObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqMap : public TqHeapObject {
 public:
  inline TqMap(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetInstanceSizeInWordsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetInstanceSizeInWordsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInObjectPropertiesStartOrConstructorFunctionIndexAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetInObjectPropertiesStartOrConstructorFunctionIndexValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetUsedOrUnusedInstanceSizeInWordsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetUsedOrUnusedInstanceSizeInWordsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetVisitorIdAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetVisitorIdValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInstanceTypeAddress() const;
  Value<InstanceType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetInstanceTypeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBitFieldAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetBitFieldValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBitField2Address() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetBitField2Value(d::MemoryAccessor accessor ) const;
  uintptr_t GetBitField3Address() const;
  Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetBitField3Value(d::MemoryAccessor accessor ) const;
  uintptr_t GetPrototypeAddress() const;
  Value<uintptr_t> GetPrototypeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetConstructorOrBackPointerOrNativeContextAddress() const;
  Value<uintptr_t> GetConstructorOrBackPointerOrNativeContextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInstanceDescriptorsAddress() const;
  Value<uintptr_t> GetInstanceDescriptorsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDependentCodeAddress() const;
  Value<uintptr_t> GetDependentCodeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPrototypeValidityCellAddress() const;
  Value<uintptr_t> GetPrototypeValidityCellValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetTransitionsOrPrototypeInfoAddress() const;
  Value<uintptr_t> GetTransitionsOrPrototypeInfoValue(d::MemoryAccessor accessor ) const;
};

class TqWeakCell : public TqHeapObject {
 public:
  inline TqWeakCell(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetFinalizationRegistryAddress() const;
  Value<uintptr_t> GetFinalizationRegistryValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetTargetAddress() const;
  Value<uintptr_t> GetTargetValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetUnregisterTokenAddress() const;
  Value<uintptr_t> GetUnregisterTokenValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetHoldingsAddress() const;
  Value<uintptr_t> GetHoldingsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPrevAddress() const;
  Value<uintptr_t> GetPrevValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNextAddress() const;
  Value<uintptr_t> GetNextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetKeyListPrevAddress() const;
  Value<uintptr_t> GetKeyListPrevValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetKeyListNextAddress() const;
  Value<uintptr_t> GetKeyListNextValue(d::MemoryAccessor accessor ) const;
};

class TqJSPromise : public TqJSObject {
 public:
  inline TqJSPromise(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetReactionsOrResultAddress() const;
  Value<uintptr_t> GetReactionsOrResultValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
};

class TqStruct : public TqHeapObject {
 public:
  inline TqStruct(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqPromiseCapability : public TqStruct {
 public:
  inline TqPromiseCapability(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetPromiseAddress() const;
  Value<uintptr_t> GetPromiseValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetResolveAddress() const;
  Value<uintptr_t> GetResolveValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRejectAddress() const;
  Value<uintptr_t> GetRejectValue(d::MemoryAccessor accessor ) const;
};

class TqJSArrayBufferView : public TqJSObject {
 public:
  inline TqJSArrayBufferView(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetBufferAddress() const;
  Value<uintptr_t> GetBufferValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetByteOffsetAddress() const;
  Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetByteOffsetValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetByteLengthAddress() const;
  Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetByteLengthValue(d::MemoryAccessor accessor ) const;
};

class TqJSTypedArray : public TqJSArrayBufferView {
 public:
  inline TqJSTypedArray(uintptr_t address) : TqJSArrayBufferView(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetExternalPointerAddress() const;
  Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetExternalPointerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBasePointerAddress() const;
  Value<uintptr_t> GetBasePointerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBitFieldAddress() const;
  Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetBitFieldValue(d::MemoryAccessor accessor ) const;
};

class TqDataHandler : public TqStruct {
 public:
  inline TqDataHandler(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSmiHandlerAddress() const;
  Value<uintptr_t> GetSmiHandlerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetValidityCellAddress() const;
  Value<uintptr_t> GetValidityCellValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetData1Address() const;
  Value<uintptr_t> GetData1Value(d::MemoryAccessor accessor ) const;
  uintptr_t GetData2Address() const;
  Value<uintptr_t> GetData2Value(d::MemoryAccessor accessor ) const;
  uintptr_t GetData3Address() const;
  Value<uintptr_t> GetData3Value(d::MemoryAccessor accessor ) const;
};

class TqAllocationMemento : public TqStruct {
 public:
  inline TqAllocationMemento(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetAllocationSiteAddress() const;
  Value<uintptr_t> GetAllocationSiteValue(d::MemoryAccessor accessor ) const;
};

class TqCallHandlerInfo : public TqStruct {
 public:
  inline TqCallHandlerInfo(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCallbackAddress() const;
  Value<uintptr_t> GetCallbackValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetJsCallbackAddress() const;
  Value<uintptr_t> GetJsCallbackValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
};

class TqInterceptorInfo : public TqStruct {
 public:
  inline TqInterceptorInfo(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetGetterAddress() const;
  Value<uintptr_t> GetGetterValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSetterAddress() const;
  Value<uintptr_t> GetSetterValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetQueryAddress() const;
  Value<uintptr_t> GetQueryValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDescriptorAddress() const;
  Value<uintptr_t> GetDescriptorValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDeleterAddress() const;
  Value<uintptr_t> GetDeleterValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEnumeratorAddress() const;
  Value<uintptr_t> GetEnumeratorValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDefinerAddress() const;
  Value<uintptr_t> GetDefinerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
};

class TqAccessCheckInfo : public TqStruct {
 public:
  inline TqAccessCheckInfo(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCallbackAddress() const;
  Value<uintptr_t> GetCallbackValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNamedInterceptorAddress() const;
  Value<uintptr_t> GetNamedInterceptorValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetIndexedInterceptorAddress() const;
  Value<uintptr_t> GetIndexedInterceptorValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
};

class TqAccessorInfo : public TqStruct {
 public:
  inline TqAccessorInfo(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNameAddress() const;
  Value<uintptr_t> GetNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetExpectedReceiverTypeAddress() const;
  Value<uintptr_t> GetExpectedReceiverTypeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSetterAddress() const;
  Value<uintptr_t> GetSetterValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetGetterAddress() const;
  Value<uintptr_t> GetGetterValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetJsGetterAddress() const;
  Value<uintptr_t> GetJsGetterValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
};

class TqJSArgumentsObject : public TqJSObject {
 public:
  inline TqJSArgumentsObject(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqJSSloppyArgumentsObject : public TqJSArgumentsObject {
 public:
  inline TqJSSloppyArgumentsObject(uintptr_t address) : TqJSArgumentsObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t> GetLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCalleeAddress() const;
  Value<uintptr_t> GetCalleeValue(d::MemoryAccessor accessor ) const;
};

class TqJSStrictArgumentsObject : public TqJSArgumentsObject {
 public:
  inline TqJSStrictArgumentsObject(uintptr_t address) : TqJSArgumentsObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t> GetLengthValue(d::MemoryAccessor accessor ) const;
};

class TqSloppyArgumentsElements : public TqFixedArrayBase {
 public:
  inline TqSloppyArgumentsElements(uintptr_t address) : TqFixedArrayBase(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetContextAddress() const;
  Value<uintptr_t> GetContextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetArgumentsAddress() const;
  Value<uintptr_t> GetArgumentsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetMappedEntriesAddress() const;
  Value<uintptr_t> GetMappedEntriesValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqAliasedArgumentsEntry : public TqStruct {
 public:
  inline TqAliasedArgumentsEntry(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetAliasedContextSlotAddress() const;
  Value<uintptr_t> GetAliasedContextSlotValue(d::MemoryAccessor accessor ) const;
};

class TqMutableBigInt : public TqBigIntBase {
 public:
  inline TqMutableBigInt(uintptr_t address) : TqBigIntBase(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqCell : public TqHeapObject {
 public:
  inline TqCell(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetValueAddress() const;
  Value<uintptr_t> GetValueValue(d::MemoryAccessor accessor ) const;
};

class TqBytecodeArray : public TqFixedArrayBase {
 public:
  inline TqBytecodeArray(uintptr_t address) : TqFixedArrayBase(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetConstantPoolAddress() const;
  Value<uintptr_t> GetConstantPoolValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetHandlerTableAddress() const;
  Value<uintptr_t> GetHandlerTableValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSourcePositionTableAddress() const;
  Value<uintptr_t> GetSourcePositionTableValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFrameSizeAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetFrameSizeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetParameterSizeAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetParameterSizeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetIncomingNewTargetOrGeneratorRegisterAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetIncomingNewTargetOrGeneratorRegisterValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetOsrNestingLevelAddress() const;
  Value<int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetOsrNestingLevelValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBytecodeAgeAddress() const;
  Value<int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetBytecodeAgeValue(d::MemoryAccessor accessor ) const;
};

class TqScopeInfo : public TqHeapObject {
 public:
  inline TqScopeInfo(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetParameterCountAddress() const;
  Value<uintptr_t> GetParameterCountValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContextLocalCountAddress() const;
  Value<uintptr_t> GetContextLocalCountValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContextLocalNamesAddress() const;
  Value<uintptr_t> GetContextLocalNamesValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqFixedDoubleArray : public TqFixedArrayBase {
 public:
  inline TqFixedDoubleArray(uintptr_t address) : TqFixedArrayBase(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetFloatsAddress() const;
};

class TqRegExpMatchInfo : public TqFixedArray {
 public:
  inline TqRegExpMatchInfo(uintptr_t address) : TqFixedArray(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqBreakPoint : public TqStruct {
 public:
  inline TqBreakPoint(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetIdAddress() const;
  Value<uintptr_t> GetIdValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetConditionAddress() const;
  Value<uintptr_t> GetConditionValue(d::MemoryAccessor accessor ) const;
};

class TqBreakPointInfo : public TqStruct {
 public:
  inline TqBreakPointInfo(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSourcePositionAddress() const;
  Value<uintptr_t> GetSourcePositionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBreakPointsAddress() const;
  Value<uintptr_t> GetBreakPointsValue(d::MemoryAccessor accessor ) const;
};

class TqDebugInfo : public TqStruct {
 public:
  inline TqDebugInfo(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSharedAddress() const;
  Value<uintptr_t> GetSharedValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDebuggerHintsAddress() const;
  Value<uintptr_t> GetDebuggerHintsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetScriptAddress() const;
  Value<uintptr_t> GetScriptValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetOriginalBytecodeArrayAddress() const;
  Value<uintptr_t> GetOriginalBytecodeArrayValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDebugBytecodeArrayAddress() const;
  Value<uintptr_t> GetDebugBytecodeArrayValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBreakPointsAddress() const;
  Value<uintptr_t> GetBreakPointsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCoverageInfoAddress() const;
  Value<uintptr_t> GetCoverageInfoValue(d::MemoryAccessor accessor ) const;
};

class TqCoverageInfo : public TqHeapObject {
 public:
  inline TqCoverageInfo(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSlotCountAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetSlotCountValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSlotsAddress() const;
};

class TqEnumCache : public TqStruct {
 public:
  inline TqEnumCache(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetKeysAddress() const;
  Value<uintptr_t> GetKeysValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetIndicesAddress() const;
  Value<uintptr_t> GetIndicesValue(d::MemoryAccessor accessor ) const;
};

class TqAccessorPair : public TqStruct {
 public:
  inline TqAccessorPair(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetGetterAddress() const;
  Value<uintptr_t> GetGetterValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSetterAddress() const;
  Value<uintptr_t> GetSetterValue(d::MemoryAccessor accessor ) const;
};

class TqClassPositions : public TqStruct {
 public:
  inline TqClassPositions(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetStartAddress() const;
  Value<uintptr_t> GetStartValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEndAddress() const;
  Value<uintptr_t> GetEndValue(d::MemoryAccessor accessor ) const;
};

class TqDescriptorArray : public TqHeapObject {
 public:
  inline TqDescriptorArray(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNumberOfAllDescriptorsAddress() const;
  Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfAllDescriptorsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfDescriptorsAddress() const;
  Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfDescriptorsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRawNumberOfMarkedDescriptorsAddress() const;
  Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetRawNumberOfMarkedDescriptorsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFiller16BitsAddress() const;
  Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetFiller16BitsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEnumCacheAddress() const;
  Value<uintptr_t> GetEnumCacheValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDescriptorsAddress() const;
};

class TqStrongDescriptorArray : public TqDescriptorArray {
 public:
  inline TqStrongDescriptorArray(uintptr_t address) : TqDescriptorArray(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqEmbedderDataArray : public TqHeapObject {
 public:
  inline TqEmbedderDataArray(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t> GetLengthValue(d::MemoryAccessor accessor ) const;
};

class TqFeedbackCell : public TqStruct {
 public:
  inline TqFeedbackCell(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetValueAddress() const;
  Value<uintptr_t> GetValueValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInterruptBudgetAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetInterruptBudgetValue(d::MemoryAccessor accessor ) const;
};

class TqFeedbackVector : public TqHeapObject {
 public:
  inline TqFeedbackVector(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInvocationCountAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetInvocationCountValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetProfilerTicksAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetProfilerTicksValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSharedFunctionInfoAddress() const;
  Value<uintptr_t> GetSharedFunctionInfoValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetMaybeOptimizedCodeAddress() const;
  Value<uintptr_t> GetMaybeOptimizedCodeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetClosureFeedbackCellArrayAddress() const;
  Value<uintptr_t> GetClosureFeedbackCellArrayValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRawFeedbackSlotsAddress() const;
  Value<uintptr_t> GetRawFeedbackSlotsValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqArrayList : public TqFixedArray {
 public:
  inline TqArrayList(uintptr_t address) : TqFixedArray(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqTemplateList : public TqFixedArray {
 public:
  inline TqTemplateList(uintptr_t address) : TqFixedArray(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqWeakArrayList : public TqHeapObject {
 public:
  inline TqWeakArrayList(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCapacityAddress() const;
  Value<uintptr_t> GetCapacityValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t> GetLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetObjectsAddress() const;
  Value<uintptr_t> GetObjectsValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqFreeSpace : public TqHeapObject {
 public:
  inline TqFreeSpace(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSizeAddress() const;
  Value<uintptr_t> GetSizeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNextAddress() const;
  Value<uintptr_t> GetNextValue(d::MemoryAccessor accessor ) const;
};

class TqJSArrayBuffer : public TqJSObject {
 public:
  inline TqJSArrayBuffer(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetByteLengthAddress() const;
  Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetByteLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetMaxByteLengthAddress() const;
  Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetMaxByteLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBackingStoreAddress() const;
  Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetBackingStoreValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetExtensionAddress() const;
  Value<Address /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetExtensionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBitFieldAddress() const;
  Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetBitFieldValue(d::MemoryAccessor accessor ) const;
};

class TqJSDataView : public TqJSArrayBufferView {
 public:
  inline TqJSDataView(uintptr_t address) : TqJSArrayBufferView(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetDataPointerAddress() const;
  Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetDataPointerValue(d::MemoryAccessor accessor ) const;
};

class TqJSArrayIterator : public TqJSObject {
 public:
  inline TqJSArrayIterator(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetIteratedObjectAddress() const;
  Value<uintptr_t> GetIteratedObjectValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNextIndexAddress() const;
  Value<uintptr_t> GetNextIndexValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetKindAddress() const;
  Value<uintptr_t> GetKindValue(d::MemoryAccessor accessor ) const;
};

class TqJSArray : public TqJSObject {
 public:
  inline TqJSArray(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t> GetLengthValue(d::MemoryAccessor accessor ) const;
};

class TqJSCollectionIterator : public TqJSObject {
 public:
  inline TqJSCollectionIterator(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetTableAddress() const;
  Value<uintptr_t> GetTableValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetIndexAddress() const;
  Value<uintptr_t> GetIndexValue(d::MemoryAccessor accessor ) const;
};

class TqJSCollection : public TqJSObject {
 public:
  inline TqJSCollection(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetTableAddress() const;
  Value<uintptr_t> GetTableValue(d::MemoryAccessor accessor ) const;
};

class TqJSSet : public TqJSCollection {
 public:
  inline TqJSSet(uintptr_t address) : TqJSCollection(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqJSMap : public TqJSCollection {
 public:
  inline TqJSMap(uintptr_t address) : TqJSCollection(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqJSWeakCollection : public TqJSObject {
 public:
  inline TqJSWeakCollection(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetTableAddress() const;
  Value<uintptr_t> GetTableValue(d::MemoryAccessor accessor ) const;
};

class TqJSWeakSet : public TqJSWeakCollection {
 public:
  inline TqJSWeakSet(uintptr_t address) : TqJSWeakCollection(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqJSWeakMap : public TqJSWeakCollection {
 public:
  inline TqJSWeakMap(uintptr_t address) : TqJSWeakCollection(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqJSGeneratorObject : public TqJSObject {
 public:
  inline TqJSGeneratorObject(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetFunctionAddress() const;
  Value<uintptr_t> GetFunctionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContextAddress() const;
  Value<uintptr_t> GetContextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetReceiverAddress() const;
  Value<uintptr_t> GetReceiverValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInputOrDebugPosAddress() const;
  Value<uintptr_t> GetInputOrDebugPosValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetResumeModeAddress() const;
  Value<uintptr_t> GetResumeModeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContinuationAddress() const;
  Value<uintptr_t> GetContinuationValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetParametersAndRegistersAddress() const;
  Value<uintptr_t> GetParametersAndRegistersValue(d::MemoryAccessor accessor ) const;
};

class TqJSAsyncFunctionObject : public TqJSGeneratorObject {
 public:
  inline TqJSAsyncFunctionObject(uintptr_t address) : TqJSGeneratorObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetPromiseAddress() const;
  Value<uintptr_t> GetPromiseValue(d::MemoryAccessor accessor ) const;
};

class TqJSAsyncGeneratorObject : public TqJSGeneratorObject {
 public:
  inline TqJSAsyncGeneratorObject(uintptr_t address) : TqJSGeneratorObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetQueueAddress() const;
  Value<uintptr_t> GetQueueValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetIsAwaitingAddress() const;
  Value<uintptr_t> GetIsAwaitingValue(d::MemoryAccessor accessor ) const;
};

class TqAsyncGeneratorRequest : public TqStruct {
 public:
  inline TqAsyncGeneratorRequest(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNextAddress() const;
  Value<uintptr_t> GetNextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetResumeModeAddress() const;
  Value<uintptr_t> GetResumeModeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetValueAddress() const;
  Value<uintptr_t> GetValueValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPromiseAddress() const;
  Value<uintptr_t> GetPromiseValue(d::MemoryAccessor accessor ) const;
};

class TqJSGlobalProxy : public TqJSSpecialObject {
 public:
  inline TqJSGlobalProxy(uintptr_t address) : TqJSSpecialObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNativeContextAddress() const;
  Value<uintptr_t> GetNativeContextValue(d::MemoryAccessor accessor ) const;
};

class TqJSGlobalObject : public TqJSSpecialObject {
 public:
  inline TqJSGlobalObject(uintptr_t address) : TqJSSpecialObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNativeContextAddress() const;
  Value<uintptr_t> GetNativeContextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetGlobalProxyAddress() const;
  Value<uintptr_t> GetGlobalProxyValue(d::MemoryAccessor accessor ) const;
};

class TqJSPrimitiveWrapper : public TqJSCustomElementsObject {
 public:
  inline TqJSPrimitiveWrapper(uintptr_t address) : TqJSCustomElementsObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetValueAddress() const;
  Value<uintptr_t> GetValueValue(d::MemoryAccessor accessor ) const;
};

class TqJSMessageObject : public TqJSObject {
 public:
  inline TqJSMessageObject(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetMessageTypeAddress() const;
  Value<uintptr_t> GetMessageTypeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetArgumentsAddress() const;
  Value<uintptr_t> GetArgumentsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetScriptAddress() const;
  Value<uintptr_t> GetScriptValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetStackFramesAddress() const;
  Value<uintptr_t> GetStackFramesValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSharedInfoAddress() const;
  Value<uintptr_t> GetSharedInfoValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBytecodeOffsetAddress() const;
  Value<uintptr_t> GetBytecodeOffsetValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetStartPositionAddress() const;
  Value<uintptr_t> GetStartPositionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEndPositionAddress() const;
  Value<uintptr_t> GetEndPositionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetErrorLevelAddress() const;
  Value<uintptr_t> GetErrorLevelValue(d::MemoryAccessor accessor ) const;
};

class TqJSDate : public TqJSObject {
 public:
  inline TqJSDate(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetValueAddress() const;
  Value<uintptr_t> GetValueValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetYearAddress() const;
  Value<uintptr_t> GetYearValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetMonthAddress() const;
  Value<uintptr_t> GetMonthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDayAddress() const;
  Value<uintptr_t> GetDayValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetWeekdayAddress() const;
  Value<uintptr_t> GetWeekdayValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetHourAddress() const;
  Value<uintptr_t> GetHourValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetMinAddress() const;
  Value<uintptr_t> GetMinValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSecAddress() const;
  Value<uintptr_t> GetSecValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCacheStampAddress() const;
  Value<uintptr_t> GetCacheStampValue(d::MemoryAccessor accessor ) const;
};

class TqJSAsyncFromSyncIterator : public TqJSObject {
 public:
  inline TqJSAsyncFromSyncIterator(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSyncIteratorAddress() const;
  Value<uintptr_t> GetSyncIteratorValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNextAddress() const;
  Value<uintptr_t> GetNextValue(d::MemoryAccessor accessor ) const;
};

class TqJSStringIterator : public TqJSObject {
 public:
  inline TqJSStringIterator(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetStringAddress() const;
  Value<uintptr_t> GetStringValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetIndexAddress() const;
  Value<uintptr_t> GetIndexValue(d::MemoryAccessor accessor ) const;
};

class TqJSProxyRevocableResult : public TqJSObject {
 public:
  inline TqJSProxyRevocableResult(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetProxyAddress() const;
  Value<uintptr_t> GetProxyValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRevokeAddress() const;
  Value<uintptr_t> GetRevokeValue(d::MemoryAccessor accessor ) const;
};

class TqJSRegExpStringIterator : public TqJSObject {
 public:
  inline TqJSRegExpStringIterator(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetIteratingRegExpAddress() const;
  Value<uintptr_t> GetIteratingRegExpValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetIteratedStringAddress() const;
  Value<uintptr_t> GetIteratedStringValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
};

class TqJSRegExp : public TqJSObject {
 public:
  inline TqJSRegExp(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSourceAddress() const;
  Value<uintptr_t> GetSourceValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
};

class TqJSRegExpResult : public TqJSArray {
 public:
  inline TqJSRegExpResult(uintptr_t address) : TqJSArray(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetIndexAddress() const;
  Value<uintptr_t> GetIndexValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInputAddress() const;
  Value<uintptr_t> GetInputValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetGroupsAddress() const;
  Value<uintptr_t> GetGroupsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNamesAddress() const;
  Value<uintptr_t> GetNamesValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRegexpInputAddress() const;
  Value<uintptr_t> GetRegexpInputValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRegexpLastIndexAddress() const;
  Value<uintptr_t> GetRegexpLastIndexValue(d::MemoryAccessor accessor ) const;
};

class TqJSRegExpResultWithIndices : public TqJSRegExpResult {
 public:
  inline TqJSRegExpResultWithIndices(uintptr_t address) : TqJSRegExpResult(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetIndicesAddress() const;
  Value<uintptr_t> GetIndicesValue(d::MemoryAccessor accessor ) const;
};

class TqJSRegExpResultIndices : public TqJSArray {
 public:
  inline TqJSRegExpResultIndices(uintptr_t address) : TqJSArray(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetGroupsAddress() const;
  Value<uintptr_t> GetGroupsValue(d::MemoryAccessor accessor ) const;
};

class TqJSFinalizationRegistry : public TqJSObject {
 public:
  inline TqJSFinalizationRegistry(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNativeContextAddress() const;
  Value<uintptr_t> GetNativeContextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCleanupAddress() const;
  Value<uintptr_t> GetCleanupValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetActiveCellsAddress() const;
  Value<uintptr_t> GetActiveCellsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetClearedCellsAddress() const;
  Value<uintptr_t> GetClearedCellsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetKeyMapAddress() const;
  Value<uintptr_t> GetKeyMapValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNextDirtyAddress() const;
  Value<uintptr_t> GetNextDirtyValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
};

class TqJSWeakRef : public TqJSObject {
 public:
  inline TqJSWeakRef(uintptr_t address) : TqJSObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetTargetAddress() const;
  Value<uintptr_t> GetTargetValue(d::MemoryAccessor accessor ) const;
};

class TqArrayBoilerplateDescription : public TqStruct {
 public:
  inline TqArrayBoilerplateDescription(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetConstantElementsAddress() const;
  Value<uintptr_t> GetConstantElementsValue(d::MemoryAccessor accessor ) const;
};

class TqRegExpBoilerplateDescription : public TqStruct {
 public:
  inline TqRegExpBoilerplateDescription(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSourceAddress() const;
  Value<uintptr_t> GetSourceValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
};

class TqMegaDomHandler : public TqHeapObject {
 public:
  inline TqMegaDomHandler(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetAccessorAddress() const;
  Value<uintptr_t> GetAccessorValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContextAddress() const;
  Value<uintptr_t> GetContextValue(d::MemoryAccessor accessor ) const;
};

class TqMicrotask : public TqStruct {
 public:
  inline TqMicrotask(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqCallbackTask : public TqMicrotask {
 public:
  inline TqCallbackTask(uintptr_t address) : TqMicrotask(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCallbackAddress() const;
  Value<uintptr_t> GetCallbackValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
};

class TqCallableTask : public TqMicrotask {
 public:
  inline TqCallableTask(uintptr_t address) : TqMicrotask(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCallableAddress() const;
  Value<uintptr_t> GetCallableValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContextAddress() const;
  Value<uintptr_t> GetContextValue(d::MemoryAccessor accessor ) const;
};

class TqModule : public TqHeapObject {
 public:
  inline TqModule(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetExportsAddress() const;
  Value<uintptr_t> GetExportsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetHashAddress() const;
  Value<uintptr_t> GetHashValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetStatusAddress() const;
  Value<uintptr_t> GetStatusValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetModuleNamespaceAddress() const;
  Value<uintptr_t> GetModuleNamespaceValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetExceptionAddress() const;
  Value<uintptr_t> GetExceptionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetTopLevelCapabilityAddress() const;
  Value<uintptr_t> GetTopLevelCapabilityValue(d::MemoryAccessor accessor ) const;
};

class TqJSModuleNamespace : public TqJSSpecialObject {
 public:
  inline TqJSModuleNamespace(uintptr_t address) : TqJSSpecialObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetModuleAddress() const;
  Value<uintptr_t> GetModuleValue(d::MemoryAccessor accessor ) const;
};

class TqSmallOrderedHashTable : public TqHeapObject {
 public:
  inline TqSmallOrderedHashTable(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqSmallOrderedHashSet : public TqSmallOrderedHashTable {
 public:
  inline TqSmallOrderedHashSet(uintptr_t address) : TqSmallOrderedHashTable(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNumberOfElementsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfElementsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfDeletedElementsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfDeletedElementsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfBucketsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfBucketsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPaddingAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetPaddingValue(d::MemoryAccessor accessor , size_t offset) const;
  uintptr_t GetDataTableAddress() const;
  Value<uintptr_t> GetDataTableValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqSmallOrderedHashMap : public TqSmallOrderedHashTable {
 public:
  inline TqSmallOrderedHashMap(uintptr_t address) : TqSmallOrderedHashTable(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNumberOfElementsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfElementsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfDeletedElementsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfDeletedElementsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfBucketsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfBucketsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPaddingAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetPaddingValue(d::MemoryAccessor accessor , size_t offset) const;
  uintptr_t GetDataTableAddress() const;
};

class TqSmallOrderedNameDictionary : public TqSmallOrderedHashTable {
 public:
  inline TqSmallOrderedNameDictionary(uintptr_t address) : TqSmallOrderedHashTable(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetHashAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetHashValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfElementsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfElementsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfDeletedElementsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfDeletedElementsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfBucketsAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetNumberOfBucketsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPadding1Address() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetPadding1Value(d::MemoryAccessor accessor , size_t offset) const;
  uintptr_t GetDataTableAddress() const;
};

class TqPromiseReaction : public TqStruct {
 public:
  inline TqPromiseReaction(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNextAddress() const;
  Value<uintptr_t> GetNextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRejectHandlerAddress() const;
  Value<uintptr_t> GetRejectHandlerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFulfillHandlerAddress() const;
  Value<uintptr_t> GetFulfillHandlerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPromiseOrCapabilityAddress() const;
  Value<uintptr_t> GetPromiseOrCapabilityValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContinuationPreservedEmbedderDataAddress() const;
  Value<uintptr_t> GetContinuationPreservedEmbedderDataValue(d::MemoryAccessor accessor ) const;
};

class TqPromiseReactionJobTask : public TqMicrotask {
 public:
  inline TqPromiseReactionJobTask(uintptr_t address) : TqMicrotask(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetArgumentAddress() const;
  Value<uintptr_t> GetArgumentValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContextAddress() const;
  Value<uintptr_t> GetContextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetHandlerAddress() const;
  Value<uintptr_t> GetHandlerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPromiseOrCapabilityAddress() const;
  Value<uintptr_t> GetPromiseOrCapabilityValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContinuationPreservedEmbedderDataAddress() const;
  Value<uintptr_t> GetContinuationPreservedEmbedderDataValue(d::MemoryAccessor accessor ) const;
};

class TqPromiseFulfillReactionJobTask : public TqPromiseReactionJobTask {
 public:
  inline TqPromiseFulfillReactionJobTask(uintptr_t address) : TqPromiseReactionJobTask(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqPromiseRejectReactionJobTask : public TqPromiseReactionJobTask {
 public:
  inline TqPromiseRejectReactionJobTask(uintptr_t address) : TqPromiseReactionJobTask(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqPromiseResolveThenableJobTask : public TqMicrotask {
 public:
  inline TqPromiseResolveThenableJobTask(uintptr_t address) : TqMicrotask(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetContextAddress() const;
  Value<uintptr_t> GetContextValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPromiseToResolveAddress() const;
  Value<uintptr_t> GetPromiseToResolveValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetThenableAddress() const;
  Value<uintptr_t> GetThenableValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetThenAddress() const;
  Value<uintptr_t> GetThenValue(d::MemoryAccessor accessor ) const;
};

class TqPropertyArray : public TqHeapObject {
 public:
  inline TqPropertyArray(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetLengthAndHashAddress() const;
  Value<uintptr_t> GetLengthAndHashValue(d::MemoryAccessor accessor ) const;
};

class TqPropertyCell : public TqHeapObject {
 public:
  inline TqPropertyCell(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNameAddress() const;
  Value<uintptr_t> GetNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPropertyDetailsRawAddress() const;
  Value<uintptr_t> GetPropertyDetailsRawValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetValueAddress() const;
  Value<uintptr_t> GetValueValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDependentCodeAddress() const;
  Value<uintptr_t> GetDependentCodeValue(d::MemoryAccessor accessor ) const;
};

class TqPropertyDescriptorObject : public TqStruct {
 public:
  inline TqPropertyDescriptorObject(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetValueAddress() const;
  Value<uintptr_t> GetValueValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetGetAddress() const;
  Value<uintptr_t> GetGetValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSetAddress() const;
  Value<uintptr_t> GetSetValue(d::MemoryAccessor accessor ) const;
};

class TqPrototypeInfo : public TqStruct {
 public:
  inline TqPrototypeInfo(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetModuleNamespaceAddress() const;
  Value<uintptr_t> GetModuleNamespaceValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPrototypeUsersAddress() const;
  Value<uintptr_t> GetPrototypeUsersValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPrototypeChainEnumCacheAddress() const;
  Value<uintptr_t> GetPrototypeChainEnumCacheValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRegistrySlotAddress() const;
  Value<uintptr_t> GetRegistrySlotValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetObjectCreateMapAddress() const;
  Value<uintptr_t> GetObjectCreateMapValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBitFieldAddress() const;
  Value<uintptr_t> GetBitFieldValue(d::MemoryAccessor accessor ) const;
};

class TqScript : public TqStruct {
 public:
  inline TqScript(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSourceAddress() const;
  Value<uintptr_t> GetSourceValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNameAddress() const;
  Value<uintptr_t> GetNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetLineOffsetAddress() const;
  Value<uintptr_t> GetLineOffsetValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetColumnOffsetAddress() const;
  Value<uintptr_t> GetColumnOffsetValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetContextDataAddress() const;
  Value<uintptr_t> GetContextDataValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetScriptTypeAddress() const;
  Value<uintptr_t> GetScriptTypeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetLineEndsAddress() const;
  Value<uintptr_t> GetLineEndsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetIdAddress() const;
  Value<uintptr_t> GetIdValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEvalFromSharedOrWrappedArgumentsOrSfiTableAddress() const;
  Value<uintptr_t> GetEvalFromSharedOrWrappedArgumentsOrSfiTableValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEvalFromPositionAddress() const;
  Value<uintptr_t> GetEvalFromPositionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSharedFunctionInfosAddress() const;
  Value<uintptr_t> GetSharedFunctionInfosValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSourceUrlAddress() const;
  Value<uintptr_t> GetSourceUrlValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSourceMappingUrlAddress() const;
  Value<uintptr_t> GetSourceMappingUrlValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetHostDefinedOptionsAddress() const;
  Value<uintptr_t> GetHostDefinedOptionsValue(d::MemoryAccessor accessor ) const;
};

class TqPreparseData : public TqHeapObject {
 public:
  inline TqPreparseData(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetDataLengthAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetDataLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetChildrenLengthAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetChildrenLengthValue(d::MemoryAccessor accessor ) const;
};

class TqInterpreterData : public TqStruct {
 public:
  inline TqInterpreterData(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetBytecodeArrayAddress() const;
  Value<uintptr_t> GetBytecodeArrayValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInterpreterTrampolineAddress() const;
  Value<uintptr_t> GetInterpreterTrampolineValue(d::MemoryAccessor accessor ) const;
};

class TqBaselineData : public TqStruct {
 public:
  inline TqBaselineData(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetBaselineCodeAddress() const;
  Value<uintptr_t> GetBaselineCodeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
};

class TqSharedFunctionInfo : public TqHeapObject {
 public:
  inline TqSharedFunctionInfo(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetFunctionDataAddress() const;
  Value<uintptr_t> GetFunctionDataValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNameOrScopeInfoAddress() const;
  Value<uintptr_t> GetNameOrScopeInfoValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetOuterScopeInfoOrFeedbackMetadataAddress() const;
  Value<uintptr_t> GetOuterScopeInfoOrFeedbackMetadataValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetScriptOrDebugInfoAddress() const;
  Value<uintptr_t> GetScriptOrDebugInfoValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetLengthAddress() const;
  Value<int16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFormalParameterCountAddress() const;
  Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetFormalParameterCountValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFunctionTokenOffsetAddress() const;
  Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetFunctionTokenOffsetValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetExpectedNofPropertiesAddress() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetExpectedNofPropertiesValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlags2Address() const;
  Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetFlags2Value(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFunctionLiteralIdAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetFunctionLiteralIdValue(d::MemoryAccessor accessor ) const;
};

class TqUncompiledData : public TqHeapObject {
 public:
  inline TqUncompiledData(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetInferredNameAddress() const;
  Value<uintptr_t> GetInferredNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetStartPositionAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetStartPositionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEndPositionAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetEndPositionValue(d::MemoryAccessor accessor ) const;
};

class TqUncompiledDataWithoutPreparseData : public TqUncompiledData {
 public:
  inline TqUncompiledDataWithoutPreparseData(uintptr_t address) : TqUncompiledData(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqUncompiledDataWithPreparseData : public TqUncompiledData {
 public:
  inline TqUncompiledDataWithPreparseData(uintptr_t address) : TqUncompiledData(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetPreparseDataAddress() const;
  Value<uintptr_t> GetPreparseDataValue(d::MemoryAccessor accessor ) const;
};

class TqOnHeapBasicBlockProfilerData : public TqHeapObject {
 public:
  inline TqOnHeapBasicBlockProfilerData(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetBlockIdsAddress() const;
  Value<uintptr_t> GetBlockIdsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCountsAddress() const;
  Value<uintptr_t> GetCountsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNameAddress() const;
  Value<uintptr_t> GetNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetScheduleAddress() const;
  Value<uintptr_t> GetScheduleValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCodeAddress() const;
  Value<uintptr_t> GetCodeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetHashAddress() const;
  Value<uintptr_t> GetHashValue(d::MemoryAccessor accessor ) const;
};

class TqSourceTextModule : public TqModule {
 public:
  inline TqSourceTextModule(uintptr_t address) : TqModule(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCodeAddress() const;
  Value<uintptr_t> GetCodeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRegularExportsAddress() const;
  Value<uintptr_t> GetRegularExportsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRegularImportsAddress() const;
  Value<uintptr_t> GetRegularImportsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRequestedModulesAddress() const;
  Value<uintptr_t> GetRequestedModulesValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetImportMetaAddress() const;
  Value<uintptr_t> GetImportMetaValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCycleRootAddress() const;
  Value<uintptr_t> GetCycleRootValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetAsyncParentModulesAddress() const;
  Value<uintptr_t> GetAsyncParentModulesValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDfsIndexAddress() const;
  Value<uintptr_t> GetDfsIndexValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDfsAncestorIndexAddress() const;
  Value<uintptr_t> GetDfsAncestorIndexValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPendingAsyncDependenciesAddress() const;
  Value<uintptr_t> GetPendingAsyncDependenciesValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
};

class TqModuleRequest : public TqStruct {
 public:
  inline TqModuleRequest(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSpecifierAddress() const;
  Value<uintptr_t> GetSpecifierValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetImportAssertionsAddress() const;
  Value<uintptr_t> GetImportAssertionsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPositionAddress() const;
  Value<uintptr_t> GetPositionValue(d::MemoryAccessor accessor ) const;
};

class TqSourceTextModuleInfoEntry : public TqStruct {
 public:
  inline TqSourceTextModuleInfoEntry(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetExportNameAddress() const;
  Value<uintptr_t> GetExportNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetLocalNameAddress() const;
  Value<uintptr_t> GetLocalNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetImportNameAddress() const;
  Value<uintptr_t> GetImportNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetModuleRequestAddress() const;
  Value<uintptr_t> GetModuleRequestValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCellIndexAddress() const;
  Value<uintptr_t> GetCellIndexValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBegPosAddress() const;
  Value<uintptr_t> GetBegPosValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEndPosAddress() const;
  Value<uintptr_t> GetEndPosValue(d::MemoryAccessor accessor ) const;
};

class TqStackFrameInfo : public TqStruct {
 public:
  inline TqStackFrameInfo(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetReceiverOrInstanceAddress() const;
  Value<uintptr_t> GetReceiverOrInstanceValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFunctionAddress() const;
  Value<uintptr_t> GetFunctionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCodeObjectAddress() const;
  Value<uintptr_t> GetCodeObjectValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCodeOffsetOrSourcePositionAddress() const;
  Value<uintptr_t> GetCodeOffsetOrSourcePositionValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagsAddress() const;
  Value<uintptr_t> GetFlagsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetParametersAddress() const;
  Value<uintptr_t> GetParametersValue(d::MemoryAccessor accessor ) const;
};

class TqConsString : public TqString {
 public:
  inline TqConsString(uintptr_t address) : TqString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetFirstAddress() const;
  Value<uintptr_t> GetFirstValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSecondAddress() const;
  Value<uintptr_t> GetSecondValue(d::MemoryAccessor accessor ) const;
};

class TqExternalString : public TqString {
 public:
  inline TqExternalString(uintptr_t address) : TqString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetResourceAddress() const;
  Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetResourceValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetResourceDataAddress() const;
  Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetResourceDataValue(d::MemoryAccessor accessor ) const;
};

class TqExternalOneByteString : public TqExternalString {
 public:
  inline TqExternalOneByteString(uintptr_t address) : TqExternalString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqExternalTwoByteString : public TqExternalString {
 public:
  inline TqExternalTwoByteString(uintptr_t address) : TqExternalString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqInternalizedString : public TqString {
 public:
  inline TqInternalizedString(uintptr_t address) : TqString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqSeqString : public TqString {
 public:
  inline TqSeqString(uintptr_t address) : TqString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqSeqOneByteString : public TqSeqString {
 public:
  inline TqSeqOneByteString(uintptr_t address) : TqSeqString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCharsAddress() const;
  Value<char /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetCharsValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqSeqTwoByteString : public TqSeqString {
 public:
  inline TqSeqTwoByteString(uintptr_t address) : TqSeqString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCharsAddress() const;
  Value<char16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetCharsValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqSlicedString : public TqString {
 public:
  inline TqSlicedString(uintptr_t address) : TqString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetParentAddress() const;
  Value<uintptr_t> GetParentValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetOffsetAddress() const;
  Value<uintptr_t> GetOffsetValue(d::MemoryAccessor accessor ) const;
};

class TqThinString : public TqString {
 public:
  inline TqThinString(uintptr_t address) : TqString(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetActualAddress() const;
  Value<uintptr_t> GetActualValue(d::MemoryAccessor accessor ) const;
};

class TqTuple2 : public TqStruct {
 public:
  inline TqTuple2(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetValue1Address() const;
  Value<uintptr_t> GetValue1Value(d::MemoryAccessor accessor ) const;
  uintptr_t GetValue2Address() const;
  Value<uintptr_t> GetValue2Value(d::MemoryAccessor accessor ) const;
};

class TqSwissNameDictionary : public TqHeapObject {
 public:
  inline TqSwissNameDictionary(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetHashAddress() const;
  Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetHashValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCapacityAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetCapacityValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetMetaTableAddress() const;
  Value<uintptr_t> GetMetaTableValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDataTableAddress() const;
  Value<uintptr_t> GetDataTableValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqSyntheticModule : public TqModule {
 public:
  inline TqSyntheticModule(uintptr_t address) : TqModule(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetNameAddress() const;
  Value<uintptr_t> GetNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetExportNamesAddress() const;
  Value<uintptr_t> GetExportNamesValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEvaluationStepsAddress() const;
  Value<uintptr_t> GetEvaluationStepsValue(d::MemoryAccessor accessor ) const;
};

class TqCachedTemplateObject : public TqStruct {
 public:
  inline TqCachedTemplateObject(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetSlotIdAddress() const;
  Value<uintptr_t> GetSlotIdValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetTemplateObjectAddress() const;
  Value<uintptr_t> GetTemplateObjectValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNextAddress() const;
  Value<uintptr_t> GetNextValue(d::MemoryAccessor accessor ) const;
};

class TqTemplateObjectDescription : public TqStruct {
 public:
  inline TqTemplateObjectDescription(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetRawStringsAddress() const;
  Value<uintptr_t> GetRawStringsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCookedStringsAddress() const;
  Value<uintptr_t> GetCookedStringsValue(d::MemoryAccessor accessor ) const;
};

class TqTemplateInfo : public TqStruct {
 public:
  inline TqTemplateInfo(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetTagAddress() const;
  Value<uintptr_t> GetTagValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSerialNumberAddress() const;
  Value<uintptr_t> GetSerialNumberValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfPropertiesAddress() const;
  Value<uintptr_t> GetNumberOfPropertiesValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPropertyListAddress() const;
  Value<uintptr_t> GetPropertyListValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPropertyAccessorsAddress() const;
  Value<uintptr_t> GetPropertyAccessorsValue(d::MemoryAccessor accessor ) const;
};

class TqFunctionTemplateRareData : public TqStruct {
 public:
  inline TqFunctionTemplateRareData(uintptr_t address) : TqStruct(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetPrototypeTemplateAddress() const;
  Value<uintptr_t> GetPrototypeTemplateValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPrototypeProviderTemplateAddress() const;
  Value<uintptr_t> GetPrototypeProviderTemplateValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetParentTemplateAddress() const;
  Value<uintptr_t> GetParentTemplateValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNamedPropertyHandlerAddress() const;
  Value<uintptr_t> GetNamedPropertyHandlerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetIndexedPropertyHandlerAddress() const;
  Value<uintptr_t> GetIndexedPropertyHandlerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInstanceTemplateAddress() const;
  Value<uintptr_t> GetInstanceTemplateValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInstanceCallHandlerAddress() const;
  Value<uintptr_t> GetInstanceCallHandlerValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetAccessCheckInfoAddress() const;
  Value<uintptr_t> GetAccessCheckInfoValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCFunctionOverloadsAddress() const;
  Value<uintptr_t> GetCFunctionOverloadsValue(d::MemoryAccessor accessor ) const;
};

class TqFunctionTemplateInfo : public TqTemplateInfo {
 public:
  inline TqFunctionTemplateInfo(uintptr_t address) : TqTemplateInfo(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCallCodeAddress() const;
  Value<uintptr_t> GetCallCodeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetClassNameAddress() const;
  Value<uintptr_t> GetClassNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSignatureAddress() const;
  Value<uintptr_t> GetSignatureValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetRareDataAddress() const;
  Value<uintptr_t> GetRareDataValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSharedFunctionInfoAddress() const;
  Value<uintptr_t> GetSharedFunctionInfoValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetFlagAddress() const;
  Value<uintptr_t> GetFlagValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetLengthAddress() const;
  Value<uintptr_t> GetLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCachedPropertyNameAddress() const;
  Value<uintptr_t> GetCachedPropertyNameValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInstanceTypeAddress() const;
  Value<uintptr_t> GetInstanceTypeValue(d::MemoryAccessor accessor ) const;
};

class TqObjectTemplateInfo : public TqTemplateInfo {
 public:
  inline TqObjectTemplateInfo(uintptr_t address) : TqTemplateInfo(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetConstructorAddress() const;
  Value<uintptr_t> GetConstructorValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
};

class TqInternalClass : public TqHeapObject {
 public:
  inline TqInternalClass(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetAAddress() const;
  Value<uintptr_t> GetAValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBAddress() const;
  Value<uintptr_t> GetBValue(d::MemoryAccessor accessor ) const;
};

class TqSmiPair : public TqHeapObject {
 public:
  inline TqSmiPair(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetAAddress() const;
  Value<uintptr_t> GetAValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBAddress() const;
  Value<uintptr_t> GetBValue(d::MemoryAccessor accessor ) const;
};

class TqSmiBox : public TqHeapObject {
 public:
  inline TqSmiBox(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetValueAddress() const;
  Value<uintptr_t> GetValueValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetUnrelatedAddress() const;
  Value<uintptr_t> GetUnrelatedValue(d::MemoryAccessor accessor ) const;
};

class TqExportedSubClassBase : public TqHeapObject {
 public:
  inline TqExportedSubClassBase(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetAAddress() const;
  Value<uintptr_t> GetAValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetBAddress() const;
  Value<uintptr_t> GetBValue(d::MemoryAccessor accessor ) const;
};

class TqExportedSubClass : public TqExportedSubClassBase {
 public:
  inline TqExportedSubClass(uintptr_t address) : TqExportedSubClassBase(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetCFieldAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetCFieldValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDFieldAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetDFieldValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEFieldAddress() const;
  Value<uintptr_t> GetEFieldValue(d::MemoryAccessor accessor ) const;
};

class TqAbstractInternalClass : public TqHeapObject {
 public:
  inline TqAbstractInternalClass(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqAbstractInternalClassSubclass1 : public TqAbstractInternalClass {
 public:
  inline TqAbstractInternalClassSubclass1(uintptr_t address) : TqAbstractInternalClass(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqAbstractInternalClassSubclass2 : public TqAbstractInternalClass {
 public:
  inline TqAbstractInternalClassSubclass2(uintptr_t address) : TqAbstractInternalClass(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
};

class TqInternalClassWithSmiElements : public TqFixedArrayBase {
 public:
  inline TqInternalClassWithSmiElements(uintptr_t address) : TqFixedArrayBase(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetObjectAddress() const;
  Value<uintptr_t> GetObjectValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEntriesAddress() const;
  Value<uintptr_t> GetEntriesValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqInternalClassWithStructElements : public TqHeapObject {
 public:
  inline TqInternalClassWithStructElements(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetDummy1Address() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetDummy1Value(d::MemoryAccessor accessor ) const;
  uintptr_t GetDummy2Address() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetDummy2Value(d::MemoryAccessor accessor ) const;
  uintptr_t GetCountAddress() const;
  Value<uintptr_t> GetCountValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDataAddress() const;
  Value<uintptr_t> GetDataValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetObjectAddress() const;
  Value<uintptr_t> GetObjectValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetEntriesAddress() const;
  Value<uintptr_t> GetEntriesValue(d::MemoryAccessor accessor , size_t offset) const;
};

class TqExportedSubClass2 : public TqExportedSubClassBase {
 public:
  inline TqExportedSubClass2(uintptr_t address) : TqExportedSubClassBase(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetXFieldAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetXFieldValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetYFieldAddress() const;
  Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> GetYFieldValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetZFieldAddress() const;
  Value<uintptr_t> GetZFieldValue(d::MemoryAccessor accessor ) const;
};

class TqSortState : public TqHeapObject {
 public:
  inline TqSortState(uintptr_t address) : TqHeapObject(address) {}
  std::vector<std::unique_ptr<ObjectProperty>> GetProperties(
      d::MemoryAccessor accessor) const override;
  const char* GetName() const override;
  void Visit(TqObjectVisitor* visitor) const override;
  bool IsSuperclassOf(const TqObject* other) const override;
  uintptr_t GetReceiverAddress() const;
  Value<uintptr_t> GetReceiverValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInitialReceiverMapAddress() const;
  Value<uintptr_t> GetInitialReceiverMapValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetInitialReceiverLengthAddress() const;
  Value<uintptr_t> GetInitialReceiverLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetUserCmpFnAddress() const;
  Value<uintptr_t> GetUserCmpFnValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSortComparePtrAddress() const;
  Value<uintptr_t> GetSortComparePtrValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetLoadFnAddress() const;
  Value<uintptr_t> GetLoadFnValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetStoreFnAddress() const;
  Value<uintptr_t> GetStoreFnValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetDeleteFnAddress() const;
  Value<uintptr_t> GetDeleteFnValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetCanUseSameAccessorFnAddress() const;
  Value<uintptr_t> GetCanUseSameAccessorFnValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetMinGallopAddress() const;
  Value<uintptr_t> GetMinGallopValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPendingRunsSizeAddress() const;
  Value<uintptr_t> GetPendingRunsSizeValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetPendingRunsAddress() const;
  Value<uintptr_t> GetPendingRunsValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetWorkArrayAddress() const;
  Value<uintptr_t> GetWorkArrayValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetTempArrayAddress() const;
  Value<uintptr_t> GetTempArrayValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetSortLengthAddress() const;
  Value<uintptr_t> GetSortLengthValue(d::MemoryAccessor accessor ) const;
  uintptr_t GetNumberOfUndefinedAddress() const;
  Value<uintptr_t> GetNumberOfUndefinedValue(d::MemoryAccessor accessor ) const;
};

class TqObjectVisitor {
 public:
  virtual void VisitObject(const TqObject* object) {}
  virtual void VisitHeapObject(const TqHeapObject* object) {
    VisitObject(object);
  }
  virtual void VisitContext(const TqContext* object) {
    VisitHeapObject(object);
  }
  virtual void VisitJSReceiver(const TqJSReceiver* object) {
    VisitHeapObject(object);
  }
  virtual void VisitPrimitiveHeapObject(const TqPrimitiveHeapObject* object) {
    VisitHeapObject(object);
  }
  virtual void VisitHeapNumber(const TqHeapNumber* object) {
    VisitPrimitiveHeapObject(object);
  }
  virtual void VisitBigIntBase(const TqBigIntBase* object) {
    VisitPrimitiveHeapObject(object);
  }
  virtual void VisitName(const TqName* object) {
    VisitPrimitiveHeapObject(object);
  }
  virtual void VisitString(const TqString* object) {
    VisitName(object);
  }
  virtual void VisitSymbol(const TqSymbol* object) {
    VisitName(object);
  }
  virtual void VisitOddball(const TqOddball* object) {
    VisitPrimitiveHeapObject(object);
  }
  virtual void VisitFixedArrayBase(const TqFixedArrayBase* object) {
    VisitHeapObject(object);
  }
  virtual void VisitFixedArray(const TqFixedArray* object) {
    VisitFixedArrayBase(object);
  }
  virtual void VisitJSObject(const TqJSObject* object) {
    VisitJSReceiver(object);
  }
  virtual void VisitByteArray(const TqByteArray* object) {
    VisitFixedArrayBase(object);
  }
  virtual void VisitWeakFixedArray(const TqWeakFixedArray* object) {
    VisitHeapObject(object);
  }
  virtual void VisitForeign(const TqForeign* object) {
    VisitHeapObject(object);
  }
  virtual void VisitJSProxy(const TqJSProxy* object) {
    VisitJSReceiver(object);
  }
  virtual void VisitJSFunctionOrBoundFunction(const TqJSFunctionOrBoundFunction* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSFunction(const TqJSFunction* object) {
    VisitJSFunctionOrBoundFunction(object);
  }
  virtual void VisitJSBoundFunction(const TqJSBoundFunction* object) {
    VisitJSFunctionOrBoundFunction(object);
  }
  virtual void VisitJSCustomElementsObject(const TqJSCustomElementsObject* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSSpecialObject(const TqJSSpecialObject* object) {
    VisitJSCustomElementsObject(object);
  }
  virtual void VisitMap(const TqMap* object) {
    VisitHeapObject(object);
  }
  virtual void VisitWeakCell(const TqWeakCell* object) {
    VisitHeapObject(object);
  }
  virtual void VisitJSPromise(const TqJSPromise* object) {
    VisitJSObject(object);
  }
  virtual void VisitStruct(const TqStruct* object) {
    VisitHeapObject(object);
  }
  virtual void VisitPromiseCapability(const TqPromiseCapability* object) {
    VisitStruct(object);
  }
  virtual void VisitJSArrayBufferView(const TqJSArrayBufferView* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSTypedArray(const TqJSTypedArray* object) {
    VisitJSArrayBufferView(object);
  }
  virtual void VisitDataHandler(const TqDataHandler* object) {
    VisitStruct(object);
  }
  virtual void VisitAllocationMemento(const TqAllocationMemento* object) {
    VisitStruct(object);
  }
  virtual void VisitCallHandlerInfo(const TqCallHandlerInfo* object) {
    VisitStruct(object);
  }
  virtual void VisitInterceptorInfo(const TqInterceptorInfo* object) {
    VisitStruct(object);
  }
  virtual void VisitAccessCheckInfo(const TqAccessCheckInfo* object) {
    VisitStruct(object);
  }
  virtual void VisitAccessorInfo(const TqAccessorInfo* object) {
    VisitStruct(object);
  }
  virtual void VisitJSArgumentsObject(const TqJSArgumentsObject* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSSloppyArgumentsObject(const TqJSSloppyArgumentsObject* object) {
    VisitJSArgumentsObject(object);
  }
  virtual void VisitJSStrictArgumentsObject(const TqJSStrictArgumentsObject* object) {
    VisitJSArgumentsObject(object);
  }
  virtual void VisitSloppyArgumentsElements(const TqSloppyArgumentsElements* object) {
    VisitFixedArrayBase(object);
  }
  virtual void VisitAliasedArgumentsEntry(const TqAliasedArgumentsEntry* object) {
    VisitStruct(object);
  }
  virtual void VisitMutableBigInt(const TqMutableBigInt* object) {
    VisitBigIntBase(object);
  }
  virtual void VisitCell(const TqCell* object) {
    VisitHeapObject(object);
  }
  virtual void VisitBytecodeArray(const TqBytecodeArray* object) {
    VisitFixedArrayBase(object);
  }
  virtual void VisitScopeInfo(const TqScopeInfo* object) {
    VisitHeapObject(object);
  }
  virtual void VisitFixedDoubleArray(const TqFixedDoubleArray* object) {
    VisitFixedArrayBase(object);
  }
  virtual void VisitRegExpMatchInfo(const TqRegExpMatchInfo* object) {
    VisitFixedArray(object);
  }
  virtual void VisitBreakPoint(const TqBreakPoint* object) {
    VisitStruct(object);
  }
  virtual void VisitBreakPointInfo(const TqBreakPointInfo* object) {
    VisitStruct(object);
  }
  virtual void VisitDebugInfo(const TqDebugInfo* object) {
    VisitStruct(object);
  }
  virtual void VisitCoverageInfo(const TqCoverageInfo* object) {
    VisitHeapObject(object);
  }
  virtual void VisitEnumCache(const TqEnumCache* object) {
    VisitStruct(object);
  }
  virtual void VisitAccessorPair(const TqAccessorPair* object) {
    VisitStruct(object);
  }
  virtual void VisitClassPositions(const TqClassPositions* object) {
    VisitStruct(object);
  }
  virtual void VisitDescriptorArray(const TqDescriptorArray* object) {
    VisitHeapObject(object);
  }
  virtual void VisitStrongDescriptorArray(const TqStrongDescriptorArray* object) {
    VisitDescriptorArray(object);
  }
  virtual void VisitEmbedderDataArray(const TqEmbedderDataArray* object) {
    VisitHeapObject(object);
  }
  virtual void VisitFeedbackCell(const TqFeedbackCell* object) {
    VisitStruct(object);
  }
  virtual void VisitFeedbackVector(const TqFeedbackVector* object) {
    VisitHeapObject(object);
  }
  virtual void VisitArrayList(const TqArrayList* object) {
    VisitFixedArray(object);
  }
  virtual void VisitTemplateList(const TqTemplateList* object) {
    VisitFixedArray(object);
  }
  virtual void VisitWeakArrayList(const TqWeakArrayList* object) {
    VisitHeapObject(object);
  }
  virtual void VisitFreeSpace(const TqFreeSpace* object) {
    VisitHeapObject(object);
  }
  virtual void VisitJSArrayBuffer(const TqJSArrayBuffer* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSDataView(const TqJSDataView* object) {
    VisitJSArrayBufferView(object);
  }
  virtual void VisitJSArrayIterator(const TqJSArrayIterator* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSArray(const TqJSArray* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSCollectionIterator(const TqJSCollectionIterator* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSCollection(const TqJSCollection* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSSet(const TqJSSet* object) {
    VisitJSCollection(object);
  }
  virtual void VisitJSMap(const TqJSMap* object) {
    VisitJSCollection(object);
  }
  virtual void VisitJSWeakCollection(const TqJSWeakCollection* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSWeakSet(const TqJSWeakSet* object) {
    VisitJSWeakCollection(object);
  }
  virtual void VisitJSWeakMap(const TqJSWeakMap* object) {
    VisitJSWeakCollection(object);
  }
  virtual void VisitJSGeneratorObject(const TqJSGeneratorObject* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSAsyncFunctionObject(const TqJSAsyncFunctionObject* object) {
    VisitJSGeneratorObject(object);
  }
  virtual void VisitJSAsyncGeneratorObject(const TqJSAsyncGeneratorObject* object) {
    VisitJSGeneratorObject(object);
  }
  virtual void VisitAsyncGeneratorRequest(const TqAsyncGeneratorRequest* object) {
    VisitStruct(object);
  }
  virtual void VisitJSGlobalProxy(const TqJSGlobalProxy* object) {
    VisitJSSpecialObject(object);
  }
  virtual void VisitJSGlobalObject(const TqJSGlobalObject* object) {
    VisitJSSpecialObject(object);
  }
  virtual void VisitJSPrimitiveWrapper(const TqJSPrimitiveWrapper* object) {
    VisitJSCustomElementsObject(object);
  }
  virtual void VisitJSMessageObject(const TqJSMessageObject* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSDate(const TqJSDate* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSAsyncFromSyncIterator(const TqJSAsyncFromSyncIterator* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSStringIterator(const TqJSStringIterator* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSProxyRevocableResult(const TqJSProxyRevocableResult* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSRegExpStringIterator(const TqJSRegExpStringIterator* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSRegExp(const TqJSRegExp* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSRegExpResult(const TqJSRegExpResult* object) {
    VisitJSArray(object);
  }
  virtual void VisitJSRegExpResultWithIndices(const TqJSRegExpResultWithIndices* object) {
    VisitJSRegExpResult(object);
  }
  virtual void VisitJSRegExpResultIndices(const TqJSRegExpResultIndices* object) {
    VisitJSArray(object);
  }
  virtual void VisitJSFinalizationRegistry(const TqJSFinalizationRegistry* object) {
    VisitJSObject(object);
  }
  virtual void VisitJSWeakRef(const TqJSWeakRef* object) {
    VisitJSObject(object);
  }
  virtual void VisitArrayBoilerplateDescription(const TqArrayBoilerplateDescription* object) {
    VisitStruct(object);
  }
  virtual void VisitRegExpBoilerplateDescription(const TqRegExpBoilerplateDescription* object) {
    VisitStruct(object);
  }
  virtual void VisitMegaDomHandler(const TqMegaDomHandler* object) {
    VisitHeapObject(object);
  }
  virtual void VisitMicrotask(const TqMicrotask* object) {
    VisitStruct(object);
  }
  virtual void VisitCallbackTask(const TqCallbackTask* object) {
    VisitMicrotask(object);
  }
  virtual void VisitCallableTask(const TqCallableTask* object) {
    VisitMicrotask(object);
  }
  virtual void VisitModule(const TqModule* object) {
    VisitHeapObject(object);
  }
  virtual void VisitJSModuleNamespace(const TqJSModuleNamespace* object) {
    VisitJSSpecialObject(object);
  }
  virtual void VisitSmallOrderedHashTable(const TqSmallOrderedHashTable* object) {
    VisitHeapObject(object);
  }
  virtual void VisitSmallOrderedHashSet(const TqSmallOrderedHashSet* object) {
    VisitSmallOrderedHashTable(object);
  }
  virtual void VisitSmallOrderedHashMap(const TqSmallOrderedHashMap* object) {
    VisitSmallOrderedHashTable(object);
  }
  virtual void VisitSmallOrderedNameDictionary(const TqSmallOrderedNameDictionary* object) {
    VisitSmallOrderedHashTable(object);
  }
  virtual void VisitPromiseReaction(const TqPromiseReaction* object) {
    VisitStruct(object);
  }
  virtual void VisitPromiseReactionJobTask(const TqPromiseReactionJobTask* object) {
    VisitMicrotask(object);
  }
  virtual void VisitPromiseFulfillReactionJobTask(const TqPromiseFulfillReactionJobTask* object) {
    VisitPromiseReactionJobTask(object);
  }
  virtual void VisitPromiseRejectReactionJobTask(const TqPromiseRejectReactionJobTask* object) {
    VisitPromiseReactionJobTask(object);
  }
  virtual void VisitPromiseResolveThenableJobTask(const TqPromiseResolveThenableJobTask* object) {
    VisitMicrotask(object);
  }
  virtual void VisitPropertyArray(const TqPropertyArray* object) {
    VisitHeapObject(object);
  }
  virtual void VisitPropertyCell(const TqPropertyCell* object) {
    VisitHeapObject(object);
  }
  virtual void VisitPropertyDescriptorObject(const TqPropertyDescriptorObject* object) {
    VisitStruct(object);
  }
  virtual void VisitPrototypeInfo(const TqPrototypeInfo* object) {
    VisitStruct(object);
  }
  virtual void VisitScript(const TqScript* object) {
    VisitStruct(object);
  }
  virtual void VisitPreparseData(const TqPreparseData* object) {
    VisitHeapObject(object);
  }
  virtual void VisitInterpreterData(const TqInterpreterData* object) {
    VisitStruct(object);
  }
  virtual void VisitBaselineData(const TqBaselineData* object) {
    VisitStruct(object);
  }
  virtual void VisitSharedFunctionInfo(const TqSharedFunctionInfo* object) {
    VisitHeapObject(object);
  }
  virtual void VisitUncompiledData(const TqUncompiledData* object) {
    VisitHeapObject(object);
  }
  virtual void VisitUncompiledDataWithoutPreparseData(const TqUncompiledDataWithoutPreparseData* object) {
    VisitUncompiledData(object);
  }
  virtual void VisitUncompiledDataWithPreparseData(const TqUncompiledDataWithPreparseData* object) {
    VisitUncompiledData(object);
  }
  virtual void VisitOnHeapBasicBlockProfilerData(const TqOnHeapBasicBlockProfilerData* object) {
    VisitHeapObject(object);
  }
  virtual void VisitSourceTextModule(const TqSourceTextModule* object) {
    VisitModule(object);
  }
  virtual void VisitModuleRequest(const TqModuleRequest* object) {
    VisitStruct(object);
  }
  virtual void VisitSourceTextModuleInfoEntry(const TqSourceTextModuleInfoEntry* object) {
    VisitStruct(object);
  }
  virtual void VisitStackFrameInfo(const TqStackFrameInfo* object) {
    VisitStruct(object);
  }
  virtual void VisitConsString(const TqConsString* object) {
    VisitString(object);
  }
  virtual void VisitExternalString(const TqExternalString* object) {
    VisitString(object);
  }
  virtual void VisitExternalOneByteString(const TqExternalOneByteString* object) {
    VisitExternalString(object);
  }
  virtual void VisitExternalTwoByteString(const TqExternalTwoByteString* object) {
    VisitExternalString(object);
  }
  virtual void VisitInternalizedString(const TqInternalizedString* object) {
    VisitString(object);
  }
  virtual void VisitSeqString(const TqSeqString* object) {
    VisitString(object);
  }
  virtual void VisitSeqOneByteString(const TqSeqOneByteString* object) {
    VisitSeqString(object);
  }
  virtual void VisitSeqTwoByteString(const TqSeqTwoByteString* object) {
    VisitSeqString(object);
  }
  virtual void VisitSlicedString(const TqSlicedString* object) {
    VisitString(object);
  }
  virtual void VisitThinString(const TqThinString* object) {
    VisitString(object);
  }
  virtual void VisitTuple2(const TqTuple2* object) {
    VisitStruct(object);
  }
  virtual void VisitSwissNameDictionary(const TqSwissNameDictionary* object) {
    VisitHeapObject(object);
  }
  virtual void VisitSyntheticModule(const TqSyntheticModule* object) {
    VisitModule(object);
  }
  virtual void VisitCachedTemplateObject(const TqCachedTemplateObject* object) {
    VisitStruct(object);
  }
  virtual void VisitTemplateObjectDescription(const TqTemplateObjectDescription* object) {
    VisitStruct(object);
  }
  virtual void VisitTemplateInfo(const TqTemplateInfo* object) {
    VisitStruct(object);
  }
  virtual void VisitFunctionTemplateRareData(const TqFunctionTemplateRareData* object) {
    VisitStruct(object);
  }
  virtual void VisitFunctionTemplateInfo(const TqFunctionTemplateInfo* object) {
    VisitTemplateInfo(object);
  }
  virtual void VisitObjectTemplateInfo(const TqObjectTemplateInfo* object) {
    VisitTemplateInfo(object);
  }
  virtual void VisitInternalClass(const TqInternalClass* object) {
    VisitHeapObject(object);
  }
  virtual void VisitSmiPair(const TqSmiPair* object) {
    VisitHeapObject(object);
  }
  virtual void VisitSmiBox(const TqSmiBox* object) {
    VisitHeapObject(object);
  }
  virtual void VisitExportedSubClassBase(const TqExportedSubClassBase* object) {
    VisitHeapObject(object);
  }
  virtual void VisitExportedSubClass(const TqExportedSubClass* object) {
    VisitExportedSubClassBase(object);
  }
  virtual void VisitAbstractInternalClass(const TqAbstractInternalClass* object) {
    VisitHeapObject(object);
  }
  virtual void VisitAbstractInternalClassSubclass1(const TqAbstractInternalClassSubclass1* object) {
    VisitAbstractInternalClass(object);
  }
  virtual void VisitAbstractInternalClassSubclass2(const TqAbstractInternalClassSubclass2* object) {
    VisitAbstractInternalClass(object);
  }
  virtual void VisitInternalClassWithSmiElements(const TqInternalClassWithSmiElements* object) {
    VisitFixedArrayBase(object);
  }
  virtual void VisitInternalClassWithStructElements(const TqInternalClassWithStructElements* object) {
    VisitHeapObject(object);
  }
  virtual void VisitExportedSubClass2(const TqExportedSubClass2* object) {
    VisitExportedSubClassBase(object);
  }
  virtual void VisitSortState(const TqSortState* object) {
    VisitHeapObject(object);
  }
};
}  // namespace debug_helper_internal
}  // namespace internal
}  // namespace v8
#endif  // V8_GEN_TORQUE_GENERATED_CLASS_DEBUG_READERS_H_
