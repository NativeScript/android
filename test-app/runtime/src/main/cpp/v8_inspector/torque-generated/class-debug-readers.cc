#include "torque-generated/class-debug-readers.h"

#include "src/objects/all-objects-inl.h"
#include "torque-generated/debug-macros.h"

// Unset a wingdi.h macro that causes conflicts.
#ifdef GetBValue
#undef GetBValue
#endif

namespace i = v8::internal;

namespace v8 {
namespace internal {
namespace debug_helper_internal {

const char* TqHeapObject::GetName() const {
  return "v8::internal::HeapObject";
}

void TqHeapObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitHeapObject(this);
}

bool TqHeapObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqHeapObject*>(other) != nullptr;
}

uintptr_t TqHeapObject::GetMapAddress() const {
  return address_ - i::kHeapObjectTag + 0;
}

Value<uintptr_t> TqHeapObject::GetMapValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetMapAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqHeapObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> map_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("map", "v8::internal::TaggedValue", "v8::internal::Map", GetMapAddress(), 1, 4, std::move(map_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqContext::GetName() const {
  return "v8::internal::Context";
}

void TqContext::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitContext(this);
}

bool TqContext::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqContext*>(other) != nullptr;
}

uintptr_t TqContext::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqContext::GetLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqContext::GetElementsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqContext::GetElementsValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetElementsAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqContext::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> elements_struct_field_list;
  auto indexed_field_slice_elements = TqDebugFieldSliceContextElements(accessor, address_);
  if (indexed_field_slice_elements.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("elements", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_elements.value), std::get<2>(indexed_field_slice_elements.value), 4, std::move(elements_struct_field_list), GetArrayKind(indexed_field_slice_elements.validity)));
  }
  return result;
}

const char* TqJSReceiver::GetName() const {
  return "v8::internal::JSReceiver";
}

void TqJSReceiver::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSReceiver(this);
}

bool TqJSReceiver::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSReceiver*>(other) != nullptr;
}

uintptr_t TqJSReceiver::GetPropertiesOrHashAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqJSReceiver::GetPropertiesOrHashValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPropertiesOrHashAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSReceiver::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> properties_or_hash_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("properties_or_hash", "v8::internal::TaggedValue", "v8::internal::Object", GetPropertiesOrHashAddress(), 1, 4, std::move(properties_or_hash_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqPrimitiveHeapObject::GetName() const {
  return "v8::internal::PrimitiveHeapObject";
}

void TqPrimitiveHeapObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPrimitiveHeapObject(this);
}

bool TqPrimitiveHeapObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPrimitiveHeapObject*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqPrimitiveHeapObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  return result;
}

const char* TqHeapNumber::GetName() const {
  return "v8::internal::HeapNumber";
}

void TqHeapNumber::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitHeapNumber(this);
}

bool TqHeapNumber::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqHeapNumber*>(other) != nullptr;
}

uintptr_t TqHeapNumber::GetValueAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqHeapNumber::GetValueValue(d::MemoryAccessor accessor) const {
  double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetValueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqHeapNumber::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqPrimitiveHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> value_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value", CheckTypeName<double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("double"), CheckTypeName<double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("double"), GetValueAddress(), 1, 8, std::move(value_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqBigIntBase::GetName() const {
  return "v8::internal::BigIntBase";
}

void TqBigIntBase::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitBigIntBase(this);
}

bool TqBigIntBase::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqBigIntBase*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqBigIntBase::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqPrimitiveHeapObject::GetProperties(accessor);
  return result;
}

const char* TqName::GetName() const {
  return "v8::internal::Name";
}

void TqName::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitName(this);
}

bool TqName::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqName*>(other) != nullptr;
}

uintptr_t TqName::GetRawHashFieldAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqName::GetRawHashFieldValue(d::MemoryAccessor accessor) const {
  uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetRawHashFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqName::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqPrimitiveHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> raw_hash_field_struct_field_list;
  raw_hash_field_struct_field_list.push_back(std::make_unique<StructProperty>("hash_not_computed", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 0));
  raw_hash_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_not_integer_index_mask", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  raw_hash_field_struct_field_list.push_back(std::make_unique<StructProperty>("array_index_value", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), 0, 24, 2));
  raw_hash_field_struct_field_list.push_back(std::make_unique<StructProperty>("array_index_length", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), 0, 6, 26));
  result.push_back(std::make_unique<ObjectProperty>("raw_hash_field", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), GetRawHashFieldAddress(), 1, 4, std::move(raw_hash_field_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqString::GetName() const {
  return "v8::internal::String";
}

void TqString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitString(this);
}

bool TqString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqString*>(other) != nullptr;
}

uintptr_t TqString::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqString::GetLengthValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqName::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSymbol::GetName() const {
  return "v8::internal::Symbol";
}

void TqSymbol::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSymbol(this);
}

bool TqSymbol::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSymbol*>(other) != nullptr;
}

uintptr_t TqSymbol::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSymbol::GetFlagsValue(d::MemoryAccessor accessor) const {
  uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSymbol::GetDescriptionAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqSymbol::GetDescriptionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDescriptionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSymbol::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqName::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_private", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 0));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_well_known_symbol", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_in_public_symbol_table", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_interesting_symbol", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_private_name", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_private_brand", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  result.push_back(std::make_unique<ObjectProperty>("flags", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> description_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("description", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", GetDescriptionAddress(), 1, 4, std::move(description_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqOddball::GetName() const {
  return "v8::internal::Oddball";
}

void TqOddball::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitOddball(this);
}

bool TqOddball::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqOddball*>(other) != nullptr;
}

uintptr_t TqOddball::GetToNumberRawAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqOddball::GetToNumberRawValue(d::MemoryAccessor accessor) const {
  double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetToNumberRawAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqOddball::GetToStringAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqOddball::GetToStringValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetToStringAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqOddball::GetToNumberAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqOddball::GetToNumberValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetToNumberAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqOddball::GetTypeOfAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqOddball::GetTypeOfValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTypeOfAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqOddball::GetKindAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqOddball::GetKindValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetKindAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqOddball::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqPrimitiveHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> to_number_raw_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("to_number_raw", CheckTypeName<double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("double"), CheckTypeName<double /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("double"), GetToNumberRawAddress(), 1, 8, std::move(to_number_raw_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> to_string_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("to_string", "v8::internal::TaggedValue", "v8::internal::String", GetToStringAddress(), 1, 4, std::move(to_string_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> to_number_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("to_number", "v8::internal::TaggedValue", "v8::internal::Object", GetToNumberAddress(), 1, 4, std::move(to_number_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> type_of_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("type_of", "v8::internal::TaggedValue", "v8::internal::String", GetTypeOfAddress(), 1, 4, std::move(type_of_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> kind_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("kind", "v8::internal::TaggedValue", "v8::internal::Object", GetKindAddress(), 1, 4, std::move(kind_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqFixedArrayBase::GetName() const {
  return "v8::internal::FixedArrayBase";
}

void TqFixedArrayBase::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitFixedArrayBase(this);
}

bool TqFixedArrayBase::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqFixedArrayBase*>(other) != nullptr;
}

uintptr_t TqFixedArrayBase::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqFixedArrayBase::GetLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqFixedArrayBase::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqFixedArray::GetName() const {
  return "v8::internal::FixedArray";
}

void TqFixedArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitFixedArray(this);
}

bool TqFixedArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqFixedArray*>(other) != nullptr;
}

uintptr_t TqFixedArray::GetObjectsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqFixedArray::GetObjectsValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetObjectsAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqFixedArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqFixedArrayBase::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> objects_struct_field_list;
  auto indexed_field_slice_objects = TqDebugFieldSliceFixedArrayObjects(accessor, address_);
  if (indexed_field_slice_objects.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("objects", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_objects.value), std::get<2>(indexed_field_slice_objects.value), 4, std::move(objects_struct_field_list), GetArrayKind(indexed_field_slice_objects.validity)));
  }
  return result;
}

const char* TqJSObject::GetName() const {
  return "v8::internal::JSObject";
}

void TqJSObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSObject(this);
}

bool TqJSObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSObject*>(other) != nullptr;
}

uintptr_t TqJSObject::GetElementsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqJSObject::GetElementsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetElementsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSReceiver::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> elements_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("elements", "v8::internal::TaggedValue", "v8::internal::FixedArrayBase", GetElementsAddress(), 1, 4, std::move(elements_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqByteArray::GetName() const {
  return "v8::internal::ByteArray";
}

void TqByteArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitByteArray(this);
}

bool TqByteArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqByteArray*>(other) != nullptr;
}

uintptr_t TqByteArray::GetBytesAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqByteArray::GetBytesValue(d::MemoryAccessor accessor, size_t offset) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetBytesAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqByteArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqFixedArrayBase::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> bytes_struct_field_list;
  auto indexed_field_slice_bytes = TqDebugFieldSliceByteArrayBytes(accessor, address_);
  if (indexed_field_slice_bytes.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("bytes", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_bytes.value), std::get<2>(indexed_field_slice_bytes.value), 1, std::move(bytes_struct_field_list), GetArrayKind(indexed_field_slice_bytes.validity)));
  }
  return result;
}

const char* TqWeakFixedArray::GetName() const {
  return "v8::internal::WeakFixedArray";
}

void TqWeakFixedArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitWeakFixedArray(this);
}

bool TqWeakFixedArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqWeakFixedArray*>(other) != nullptr;
}

uintptr_t TqWeakFixedArray::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqWeakFixedArray::GetLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakFixedArray::GetObjectsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqWeakFixedArray::GetObjectsValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetObjectsAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqWeakFixedArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> objects_struct_field_list;
  auto indexed_field_slice_objects = TqDebugFieldSliceWeakFixedArrayObjects(accessor, address_);
  if (indexed_field_slice_objects.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("objects", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_objects.value), std::get<2>(indexed_field_slice_objects.value), 4, std::move(objects_struct_field_list), GetArrayKind(indexed_field_slice_objects.validity)));
  }
  return result;
}

const char* TqForeign::GetName() const {
  return "v8::internal::Foreign";
}

void TqForeign::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitForeign(this);
}

bool TqForeign::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqForeign*>(other) != nullptr;
}

uintptr_t TqForeign::GetForeignAddressAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqForeign::GetForeignAddressValue(d::MemoryAccessor accessor) const {
  ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetForeignAddressAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqForeign::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> foreign_address_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("foreign_address", CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), GetForeignAddressAddress(), 1, 8, std::move(foreign_address_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSProxy::GetName() const {
  return "v8::internal::JSProxy";
}

void TqJSProxy::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSProxy(this);
}

bool TqJSProxy::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSProxy*>(other) != nullptr;
}

uintptr_t TqJSProxy::GetTargetAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqJSProxy::GetTargetValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTargetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSProxy::GetHandlerAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSProxy::GetHandlerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetHandlerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSProxy::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSReceiver::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> target_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("target", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetTargetAddress(), 1, 4, std::move(target_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> handler_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("handler", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetHandlerAddress(), 1, 4, std::move(handler_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSFunctionOrBoundFunction::GetName() const {
  return "v8::internal::JSFunctionOrBoundFunction";
}

void TqJSFunctionOrBoundFunction::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSFunctionOrBoundFunction(this);
}

bool TqJSFunctionOrBoundFunction::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSFunctionOrBoundFunction*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSFunctionOrBoundFunction::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  return result;
}

const char* TqJSFunction::GetName() const {
  return "v8::internal::JSFunction";
}

void TqJSFunction::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSFunction(this);
}

bool TqJSFunction::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSFunction*>(other) != nullptr;
}

uintptr_t TqJSFunction::GetSharedFunctionInfoAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSFunction::GetSharedFunctionInfoValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSharedFunctionInfoAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFunction::GetContextAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSFunction::GetContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFunction::GetFeedbackCellAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSFunction::GetFeedbackCellValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFeedbackCellAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFunction::GetCodeAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqJSFunction::GetCodeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCodeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFunction::GetPrototypeOrInitialMapAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqJSFunction::GetPrototypeOrInitialMapValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPrototypeOrInitialMapAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSFunction::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSFunctionOrBoundFunction::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> shared_function_info_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("shared_function_info", "v8::internal::TaggedValue", "v8::internal::SharedFunctionInfo", GetSharedFunctionInfoAddress(), 1, 4, std::move(shared_function_info_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("context", "v8::internal::TaggedValue", "v8::internal::Context", GetContextAddress(), 1, 4, std::move(context_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> feedback_cell_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("feedback_cell", "v8::internal::TaggedValue", "v8::internal::FeedbackCell", GetFeedbackCellAddress(), 1, 4, std::move(feedback_cell_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> code_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("code", "v8::internal::TaggedValue", "v8::internal::Code", GetCodeAddress(), 1, 4, std::move(code_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> prototype_or_initial_map_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("prototype_or_initial_map", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPrototypeOrInitialMapAddress(), 1, 4, std::move(prototype_or_initial_map_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSBoundFunction::GetName() const {
  return "v8::internal::JSBoundFunction";
}

void TqJSBoundFunction::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSBoundFunction(this);
}

bool TqJSBoundFunction::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSBoundFunction*>(other) != nullptr;
}

uintptr_t TqJSBoundFunction::GetBoundTargetFunctionAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSBoundFunction::GetBoundTargetFunctionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBoundTargetFunctionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSBoundFunction::GetBoundThisAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSBoundFunction::GetBoundThisValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBoundThisAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSBoundFunction::GetBoundArgumentsAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSBoundFunction::GetBoundArgumentsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBoundArgumentsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSBoundFunction::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSFunctionOrBoundFunction::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> bound_target_function_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("bound_target_function", "v8::internal::TaggedValue", "v8::internal::JSReceiver", GetBoundTargetFunctionAddress(), 1, 4, std::move(bound_target_function_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bound_this_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("bound_this", "v8::internal::TaggedValue", "v8::internal::Object", GetBoundThisAddress(), 1, 4, std::move(bound_this_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bound_arguments_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("bound_arguments", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetBoundArgumentsAddress(), 1, 4, std::move(bound_arguments_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSCustomElementsObject::GetName() const {
  return "v8::internal::JSCustomElementsObject";
}

void TqJSCustomElementsObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSCustomElementsObject(this);
}

bool TqJSCustomElementsObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSCustomElementsObject*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSCustomElementsObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  return result;
}

const char* TqJSSpecialObject::GetName() const {
  return "v8::internal::JSSpecialObject";
}

void TqJSSpecialObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSSpecialObject(this);
}

bool TqJSSpecialObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSSpecialObject*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSSpecialObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSCustomElementsObject::GetProperties(accessor);
  return result;
}

const char* TqMap::GetName() const {
  return "v8::internal::Map";
}

void TqMap::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitMap(this);
}

bool TqMap::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqMap*>(other) != nullptr;
}

uintptr_t TqMap::GetInstanceSizeInWordsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqMap::GetInstanceSizeInWordsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetInstanceSizeInWordsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqMap::GetInObjectPropertiesStartOrConstructorFunctionIndexAddress() const {
  return address_ - i::kHeapObjectTag + 5;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqMap::GetInObjectPropertiesStartOrConstructorFunctionIndexValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetInObjectPropertiesStartOrConstructorFunctionIndexAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqMap::GetUsedOrUnusedInstanceSizeInWordsAddress() const {
  return address_ - i::kHeapObjectTag + 6;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqMap::GetUsedOrUnusedInstanceSizeInWordsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetUsedOrUnusedInstanceSizeInWordsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqMap::GetVisitorIdAddress() const {
  return address_ - i::kHeapObjectTag + 7;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqMap::GetVisitorIdValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetVisitorIdAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqMap::GetInstanceTypeAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<InstanceType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqMap::GetInstanceTypeValue(d::MemoryAccessor accessor) const {
  InstanceType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetInstanceTypeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqMap::GetBitFieldAddress() const {
  return address_ - i::kHeapObjectTag + 10;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqMap::GetBitFieldValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetBitFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqMap::GetBitField2Address() const {
  return address_ - i::kHeapObjectTag + 11;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqMap::GetBitField2Value(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetBitField2Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqMap::GetBitField3Address() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqMap::GetBitField3Value(d::MemoryAccessor accessor) const {
  uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetBitField3Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqMap::GetPrototypeAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqMap::GetPrototypeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPrototypeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqMap::GetConstructorOrBackPointerOrNativeContextAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqMap::GetConstructorOrBackPointerOrNativeContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetConstructorOrBackPointerOrNativeContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqMap::GetInstanceDescriptorsAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqMap::GetInstanceDescriptorsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInstanceDescriptorsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqMap::GetDependentCodeAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqMap::GetDependentCodeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDependentCodeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqMap::GetPrototypeValidityCellAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqMap::GetPrototypeValidityCellValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPrototypeValidityCellAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqMap::GetTransitionsOrPrototypeInfoAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqMap::GetTransitionsOrPrototypeInfoValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTransitionsOrPrototypeInfoAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqMap::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> instance_size_in_words_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("instance_size_in_words", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetInstanceSizeInWordsAddress(), 1, 1, std::move(instance_size_in_words_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> in_object_properties_start_or_constructor_function_index_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("in_object_properties_start_or_constructor_function_index", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetInObjectPropertiesStartOrConstructorFunctionIndexAddress(), 1, 1, std::move(in_object_properties_start_or_constructor_function_index_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> used_or_unused_instance_size_in_words_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("used_or_unused_instance_size_in_words", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetUsedOrUnusedInstanceSizeInWordsAddress(), 1, 1, std::move(used_or_unused_instance_size_in_words_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> visitor_id_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("visitor_id", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetVisitorIdAddress(), 1, 1, std::move(visitor_id_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> instance_type_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("instance_type", CheckTypeName<InstanceType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("InstanceType"), CheckTypeName<InstanceType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("InstanceType"), GetInstanceTypeAddress(), 1, 2, std::move(instance_type_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bit_field_struct_field_list;
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("has_non_instance_prototype", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 0));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_callable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("has_named_interceptor", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("has_indexed_interceptor", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_undetectable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_access_check_needed", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_constructor", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 6));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("has_prototype_slot", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 7));
  result.push_back(std::make_unique<ObjectProperty>("bit_field", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetBitFieldAddress(), 1, 1, std::move(bit_field_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bit_field2_struct_field_list;
  bit_field2_struct_field_list.push_back(std::make_unique<StructProperty>("new_target_is_base", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 0));
  bit_field2_struct_field_list.push_back(std::make_unique<StructProperty>("is_immutable_prototype", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  bit_field2_struct_field_list.push_back(std::make_unique<StructProperty>("elements_kind", CheckTypeName<ElementsKind /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ElementsKind"), CheckTypeName<ElementsKind /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ElementsKind"), 0, 6, 2));
  result.push_back(std::make_unique<ObjectProperty>("bit_field2", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetBitField2Address(), 1, 1, std::move(bit_field2_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bit_field3_struct_field_list;
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("enum_length", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 10, 0));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("number_of_own_descriptors", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 10, 10));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("is_prototype_map", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 20));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("is_dictionary_map", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 21));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("owns_descriptors", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 22));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("is_in_retained_map_list", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 23));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("is_deprecated", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 24));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("is_unstable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 25));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("is_migration_target", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 26));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("is_extensible", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 27));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("may_have_interesting_symbols", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 28));
  bit_field3_struct_field_list.push_back(std::make_unique<StructProperty>("construction_counter", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 3, 29));
  result.push_back(std::make_unique<ObjectProperty>("bit_field3", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), GetBitField3Address(), 1, 4, std::move(bit_field3_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> prototype_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("prototype", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPrototypeAddress(), 1, 4, std::move(prototype_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> constructor_or_back_pointer_or_native_context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("constructor_or_back_pointer_or_native_context", "v8::internal::TaggedValue", "v8::internal::Object", GetConstructorOrBackPointerOrNativeContextAddress(), 1, 4, std::move(constructor_or_back_pointer_or_native_context_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> instance_descriptors_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("instance_descriptors", "v8::internal::TaggedValue", "v8::internal::DescriptorArray", GetInstanceDescriptorsAddress(), 1, 4, std::move(instance_descriptors_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> dependent_code_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("dependent_code", "v8::internal::TaggedValue", "v8::internal::WeakFixedArray", GetDependentCodeAddress(), 1, 4, std::move(dependent_code_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> prototype_validity_cell_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("prototype_validity_cell", "v8::internal::TaggedValue", "v8::internal::Object", GetPrototypeValidityCellAddress(), 1, 4, std::move(prototype_validity_cell_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> transitions_or_prototype_info_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("transitions_or_prototype_info", "v8::internal::TaggedValue", "v8::internal::Object", GetTransitionsOrPrototypeInfoAddress(), 1, 4, std::move(transitions_or_prototype_info_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqWeakCell::GetName() const {
  return "v8::internal::WeakCell";
}

void TqWeakCell::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitWeakCell(this);
}

bool TqWeakCell::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqWeakCell*>(other) != nullptr;
}

uintptr_t TqWeakCell::GetFinalizationRegistryAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqWeakCell::GetFinalizationRegistryValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFinalizationRegistryAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakCell::GetTargetAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqWeakCell::GetTargetValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTargetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakCell::GetUnregisterTokenAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqWeakCell::GetUnregisterTokenValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetUnregisterTokenAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakCell::GetHoldingsAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqWeakCell::GetHoldingsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetHoldingsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakCell::GetPrevAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqWeakCell::GetPrevValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPrevAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakCell::GetNextAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqWeakCell::GetNextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakCell::GetKeyListPrevAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqWeakCell::GetKeyListPrevValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetKeyListPrevAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakCell::GetKeyListNextAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqWeakCell::GetKeyListNextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetKeyListNextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqWeakCell::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> finalization_registry_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("finalization_registry", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetFinalizationRegistryAddress(), 1, 4, std::move(finalization_registry_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> target_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("target", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetTargetAddress(), 1, 4, std::move(target_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> unregister_token_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("unregister_token", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetUnregisterTokenAddress(), 1, 4, std::move(unregister_token_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> holdings_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("holdings", "v8::internal::TaggedValue", "v8::internal::Object", GetHoldingsAddress(), 1, 4, std::move(holdings_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> prev_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("prev", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPrevAddress(), 1, 4, std::move(prev_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> next_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("next", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetNextAddress(), 1, 4, std::move(next_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> key_list_prev_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("key_list_prev", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetKeyListPrevAddress(), 1, 4, std::move(key_list_prev_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> key_list_next_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("key_list_next", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetKeyListNextAddress(), 1, 4, std::move(key_list_next_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSPromise::GetName() const {
  return "v8::internal::JSPromise";
}

void TqJSPromise::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSPromise(this);
}

bool TqJSPromise::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSPromise*>(other) != nullptr;
}

uintptr_t TqJSPromise::GetReactionsOrResultAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSPromise::GetReactionsOrResultValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetReactionsOrResultAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSPromise::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSPromise::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSPromise::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> reactions_or_result_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("reactions_or_result", "v8::internal::TaggedValue", "v8::internal::Object", GetReactionsOrResultAddress(), 1, 4, std::move(reactions_or_result_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("status", CheckTypeName<Promise::PromiseState /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("Promise::PromiseState"), CheckTypeName<Promise::PromiseState /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("Promise::PromiseState"), 0, 2, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_handler", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("handled_hint", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_silent", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("async_task_id", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 22, 6));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqStruct::GetName() const {
  return "v8::internal::Struct";
}

void TqStruct::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitStruct(this);
}

bool TqStruct::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqStruct*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqStruct::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  return result;
}

const char* TqPromiseCapability::GetName() const {
  return "v8::internal::PromiseCapability";
}

void TqPromiseCapability::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPromiseCapability(this);
}

bool TqPromiseCapability::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPromiseCapability*>(other) != nullptr;
}

uintptr_t TqPromiseCapability::GetPromiseAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqPromiseCapability::GetPromiseValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPromiseAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseCapability::GetResolveAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqPromiseCapability::GetResolveValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetResolveAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseCapability::GetRejectAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqPromiseCapability::GetRejectValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRejectAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqPromiseCapability::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> promise_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("promise", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPromiseAddress(), 1, 4, std::move(promise_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> resolve_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("resolve", "v8::internal::TaggedValue", "v8::internal::Object", GetResolveAddress(), 1, 4, std::move(resolve_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> reject_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("reject", "v8::internal::TaggedValue", "v8::internal::Object", GetRejectAddress(), 1, 4, std::move(reject_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSArrayBufferView::GetName() const {
  return "v8::internal::JSArrayBufferView";
}

void TqJSArrayBufferView::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSArrayBufferView(this);
}

bool TqJSArrayBufferView::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSArrayBufferView*>(other) != nullptr;
}

uintptr_t TqJSArrayBufferView::GetBufferAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSArrayBufferView::GetBufferValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBufferAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSArrayBufferView::GetByteOffsetAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSArrayBufferView::GetByteOffsetValue(d::MemoryAccessor accessor) const {
  uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetByteOffsetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqJSArrayBufferView::GetByteLengthAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSArrayBufferView::GetByteLengthValue(d::MemoryAccessor accessor) const {
  uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetByteLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSArrayBufferView::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> buffer_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("buffer", "v8::internal::TaggedValue", "v8::internal::JSArrayBuffer", GetBufferAddress(), 1, 4, std::move(buffer_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> byte_offset_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("byte_offset", CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), GetByteOffsetAddress(), 1, 8, std::move(byte_offset_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> byte_length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("byte_length", CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), GetByteLengthAddress(), 1, 8, std::move(byte_length_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSTypedArray::GetName() const {
  return "v8::internal::JSTypedArray";
}

void TqJSTypedArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSTypedArray(this);
}

bool TqJSTypedArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSTypedArray*>(other) != nullptr;
}

uintptr_t TqJSTypedArray::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSTypedArray::GetLengthValue(d::MemoryAccessor accessor) const {
  uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqJSTypedArray::GetExternalPointerAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSTypedArray::GetExternalPointerValue(d::MemoryAccessor accessor) const {
  ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetExternalPointerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqJSTypedArray::GetBasePointerAddress() const {
  return address_ - i::kHeapObjectTag + 48;
}

Value<uintptr_t> TqJSTypedArray::GetBasePointerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBasePointerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSTypedArray::GetBitFieldAddress() const {
  return address_ - i::kHeapObjectTag + 52;
}

Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSTypedArray::GetBitFieldValue(d::MemoryAccessor accessor) const {
  uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetBitFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSTypedArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSArrayBufferView::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), GetLengthAddress(), 1, 8, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> external_pointer_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("external_pointer", CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), GetExternalPointerAddress(), 1, 8, std::move(external_pointer_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> base_pointer_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("base_pointer", "v8::internal::TaggedValue", "v8::internal::Object", GetBasePointerAddress(), 1, 4, std::move(base_pointer_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bit_field_struct_field_list;
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_length_tracking", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 0));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_backed_by_rab", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  result.push_back(std::make_unique<ObjectProperty>("bit_field", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), GetBitFieldAddress(), 1, 4, std::move(bit_field_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqDataHandler::GetName() const {
  return "v8::internal::DataHandler";
}

void TqDataHandler::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitDataHandler(this);
}

bool TqDataHandler::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqDataHandler*>(other) != nullptr;
}

uintptr_t TqDataHandler::GetSmiHandlerAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqDataHandler::GetSmiHandlerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSmiHandlerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDataHandler::GetValidityCellAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqDataHandler::GetValidityCellValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValidityCellAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDataHandler::GetData1Address() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqDataHandler::GetData1Value(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetData1Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDataHandler::GetData2Address() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqDataHandler::GetData2Value(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetData2Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDataHandler::GetData3Address() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqDataHandler::GetData3Value(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetData3Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqDataHandler::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> smi_handler_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("smi_handler", "v8::internal::TaggedValue", "v8::internal::Object", GetSmiHandlerAddress(), 1, 4, std::move(smi_handler_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> validity_cell_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("validity_cell", "v8::internal::TaggedValue", "v8::internal::Object", GetValidityCellAddress(), 1, 4, std::move(validity_cell_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_1_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data_1", "v8::internal::TaggedValue", "v8::internal::Object", GetData1Address(), 1, 4, std::move(data_1_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_2_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data_2", "v8::internal::TaggedValue", "v8::internal::Object", GetData2Address(), 1, 4, std::move(data_2_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_3_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data_3", "v8::internal::TaggedValue", "v8::internal::Object", GetData3Address(), 1, 4, std::move(data_3_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqAllocationMemento::GetName() const {
  return "v8::internal::AllocationMemento";
}

void TqAllocationMemento::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitAllocationMemento(this);
}

bool TqAllocationMemento::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqAllocationMemento*>(other) != nullptr;
}

uintptr_t TqAllocationMemento::GetAllocationSiteAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqAllocationMemento::GetAllocationSiteValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetAllocationSiteAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqAllocationMemento::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> allocation_site_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("allocation_site", "v8::internal::TaggedValue", "v8::internal::AllocationSite", GetAllocationSiteAddress(), 1, 4, std::move(allocation_site_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqCallHandlerInfo::GetName() const {
  return "v8::internal::CallHandlerInfo";
}

void TqCallHandlerInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitCallHandlerInfo(this);
}

bool TqCallHandlerInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqCallHandlerInfo*>(other) != nullptr;
}

uintptr_t TqCallHandlerInfo::GetCallbackAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqCallHandlerInfo::GetCallbackValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCallbackAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqCallHandlerInfo::GetJsCallbackAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqCallHandlerInfo::GetJsCallbackValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetJsCallbackAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqCallHandlerInfo::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqCallHandlerInfo::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqCallHandlerInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> callback_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("callback", "v8::internal::TaggedValue", "v8::internal::Object", GetCallbackAddress(), 1, 4, std::move(callback_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> js_callback_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("js_callback", "v8::internal::TaggedValue", "v8::internal::Object", GetJsCallbackAddress(), 1, 4, std::move(js_callback_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::Object", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqInterceptorInfo::GetName() const {
  return "v8::internal::InterceptorInfo";
}

void TqInterceptorInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitInterceptorInfo(this);
}

bool TqInterceptorInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqInterceptorInfo*>(other) != nullptr;
}

uintptr_t TqInterceptorInfo::GetGetterAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqInterceptorInfo::GetGetterValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetGetterAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInterceptorInfo::GetSetterAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqInterceptorInfo::GetSetterValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSetterAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInterceptorInfo::GetQueryAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqInterceptorInfo::GetQueryValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetQueryAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInterceptorInfo::GetDescriptorAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqInterceptorInfo::GetDescriptorValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDescriptorAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInterceptorInfo::GetDeleterAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqInterceptorInfo::GetDeleterValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDeleterAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInterceptorInfo::GetEnumeratorAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqInterceptorInfo::GetEnumeratorValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEnumeratorAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInterceptorInfo::GetDefinerAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqInterceptorInfo::GetDefinerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDefinerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInterceptorInfo::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqInterceptorInfo::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInterceptorInfo::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqInterceptorInfo::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqInterceptorInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> getter_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("getter", "v8::internal::TaggedValue", "v8::internal::Object", GetGetterAddress(), 1, 4, std::move(getter_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> setter_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("setter", "v8::internal::TaggedValue", "v8::internal::Object", GetSetterAddress(), 1, 4, std::move(setter_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> query_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("query", "v8::internal::TaggedValue", "v8::internal::Object", GetQueryAddress(), 1, 4, std::move(query_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> descriptor_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("descriptor", "v8::internal::TaggedValue", "v8::internal::Object", GetDescriptorAddress(), 1, 4, std::move(descriptor_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> deleter_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("deleter", "v8::internal::TaggedValue", "v8::internal::Object", GetDeleterAddress(), 1, 4, std::move(deleter_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> enumerator_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("enumerator", "v8::internal::TaggedValue", "v8::internal::Object", GetEnumeratorAddress(), 1, 4, std::move(enumerator_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> definer_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("definer", "v8::internal::TaggedValue", "v8::internal::Object", GetDefinerAddress(), 1, 4, std::move(definer_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::Object", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("can_intercept_symbols", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("all_can_read", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("non_masking", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("named", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_no_side_effect", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqAccessCheckInfo::GetName() const {
  return "v8::internal::AccessCheckInfo";
}

void TqAccessCheckInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitAccessCheckInfo(this);
}

bool TqAccessCheckInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqAccessCheckInfo*>(other) != nullptr;
}

uintptr_t TqAccessCheckInfo::GetCallbackAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqAccessCheckInfo::GetCallbackValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCallbackAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessCheckInfo::GetNamedInterceptorAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqAccessCheckInfo::GetNamedInterceptorValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNamedInterceptorAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessCheckInfo::GetIndexedInterceptorAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqAccessCheckInfo::GetIndexedInterceptorValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIndexedInterceptorAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessCheckInfo::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqAccessCheckInfo::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqAccessCheckInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> callback_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("callback", "v8::internal::TaggedValue", "v8::internal::Object", GetCallbackAddress(), 1, 4, std::move(callback_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> named_interceptor_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("named_interceptor", "v8::internal::TaggedValue", "v8::internal::Object", GetNamedInterceptorAddress(), 1, 4, std::move(named_interceptor_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> indexed_interceptor_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("indexed_interceptor", "v8::internal::TaggedValue", "v8::internal::Object", GetIndexedInterceptorAddress(), 1, 4, std::move(indexed_interceptor_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::Object", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqAccessorInfo::GetName() const {
  return "v8::internal::AccessorInfo";
}

void TqAccessorInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitAccessorInfo(this);
}

bool TqAccessorInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqAccessorInfo*>(other) != nullptr;
}

uintptr_t TqAccessorInfo::GetNameAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqAccessorInfo::GetNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessorInfo::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqAccessorInfo::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessorInfo::GetExpectedReceiverTypeAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqAccessorInfo::GetExpectedReceiverTypeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetExpectedReceiverTypeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessorInfo::GetSetterAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqAccessorInfo::GetSetterValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSetterAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessorInfo::GetGetterAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqAccessorInfo::GetGetterValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetGetterAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessorInfo::GetJsGetterAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqAccessorInfo::GetJsGetterValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetJsGetterAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessorInfo::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqAccessorInfo::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqAccessorInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("name", "v8::internal::TaggedValue", "v8::internal::Name", GetNameAddress(), 1, 4, std::move(name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("all_can_read", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("all_can_write", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_special_data_property", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_sloppy", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("replace_on_access", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("getter_side_effect_type", CheckTypeName<SideEffectType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("SideEffectType"), CheckTypeName<SideEffectType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("SideEffectType"), 0, 2, 6));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("setter_side_effect_type", CheckTypeName<SideEffectType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("SideEffectType"), CheckTypeName<SideEffectType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("SideEffectType"), 0, 2, 8));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("initial_attributes", CheckTypeName<PropertyAttributes /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("PropertyAttributes"), CheckTypeName<PropertyAttributes /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("PropertyAttributes"), 0, 3, 10));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> expected_receiver_type_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("expected_receiver_type", "v8::internal::TaggedValue", "v8::internal::Object", GetExpectedReceiverTypeAddress(), 1, 4, std::move(expected_receiver_type_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> setter_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("setter", "v8::internal::TaggedValue", "v8::internal::Object", GetSetterAddress(), 1, 4, std::move(setter_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> getter_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("getter", "v8::internal::TaggedValue", "v8::internal::Object", GetGetterAddress(), 1, 4, std::move(getter_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> js_getter_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("js_getter", "v8::internal::TaggedValue", "v8::internal::Object", GetJsGetterAddress(), 1, 4, std::move(js_getter_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::Object", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSArgumentsObject::GetName() const {
  return "v8::internal::JSArgumentsObject";
}

void TqJSArgumentsObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSArgumentsObject(this);
}

bool TqJSArgumentsObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSArgumentsObject*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSArgumentsObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  return result;
}

const char* TqJSSloppyArgumentsObject::GetName() const {
  return "v8::internal::JSSloppyArgumentsObject";
}

void TqJSSloppyArgumentsObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSSloppyArgumentsObject(this);
}

bool TqJSSloppyArgumentsObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSSloppyArgumentsObject*>(other) != nullptr;
}

uintptr_t TqJSSloppyArgumentsObject::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSSloppyArgumentsObject::GetLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSSloppyArgumentsObject::GetCalleeAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSSloppyArgumentsObject::GetCalleeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCalleeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSSloppyArgumentsObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSArgumentsObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> callee_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("callee", "v8::internal::TaggedValue", "v8::internal::Object", GetCalleeAddress(), 1, 4, std::move(callee_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSStrictArgumentsObject::GetName() const {
  return "v8::internal::JSStrictArgumentsObject";
}

void TqJSStrictArgumentsObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSStrictArgumentsObject(this);
}

bool TqJSStrictArgumentsObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSStrictArgumentsObject*>(other) != nullptr;
}

uintptr_t TqJSStrictArgumentsObject::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSStrictArgumentsObject::GetLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSStrictArgumentsObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSArgumentsObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSloppyArgumentsElements::GetName() const {
  return "v8::internal::SloppyArgumentsElements";
}

void TqSloppyArgumentsElements::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSloppyArgumentsElements(this);
}

bool TqSloppyArgumentsElements::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSloppyArgumentsElements*>(other) != nullptr;
}

uintptr_t TqSloppyArgumentsElements::GetContextAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqSloppyArgumentsElements::GetContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSloppyArgumentsElements::GetArgumentsAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqSloppyArgumentsElements::GetArgumentsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetArgumentsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSloppyArgumentsElements::GetMappedEntriesAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqSloppyArgumentsElements::GetMappedEntriesValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetMappedEntriesAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSloppyArgumentsElements::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqFixedArrayBase::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("context", "v8::internal::TaggedValue", "v8::internal::Context", GetContextAddress(), 1, 4, std::move(context_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> arguments_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("arguments", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetArgumentsAddress(), 1, 4, std::move(arguments_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> mapped_entries_struct_field_list;
  auto indexed_field_slice_mapped_entries = TqDebugFieldSliceSloppyArgumentsElementsMappedEntries(accessor, address_);
  if (indexed_field_slice_mapped_entries.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("mapped_entries", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_mapped_entries.value), std::get<2>(indexed_field_slice_mapped_entries.value), 4, std::move(mapped_entries_struct_field_list), GetArrayKind(indexed_field_slice_mapped_entries.validity)));
  }
  return result;
}

const char* TqAliasedArgumentsEntry::GetName() const {
  return "v8::internal::AliasedArgumentsEntry";
}

void TqAliasedArgumentsEntry::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitAliasedArgumentsEntry(this);
}

bool TqAliasedArgumentsEntry::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqAliasedArgumentsEntry*>(other) != nullptr;
}

uintptr_t TqAliasedArgumentsEntry::GetAliasedContextSlotAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqAliasedArgumentsEntry::GetAliasedContextSlotValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetAliasedContextSlotAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqAliasedArgumentsEntry::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> aliased_context_slot_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("aliased_context_slot", "v8::internal::TaggedValue", "v8::internal::Object", GetAliasedContextSlotAddress(), 1, 4, std::move(aliased_context_slot_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqMutableBigInt::GetName() const {
  return "v8::internal::MutableBigInt";
}

void TqMutableBigInt::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitMutableBigInt(this);
}

bool TqMutableBigInt::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqMutableBigInt*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqMutableBigInt::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqBigIntBase::GetProperties(accessor);
  return result;
}

const char* TqCell::GetName() const {
  return "v8::internal::Cell";
}

void TqCell::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitCell(this);
}

bool TqCell::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqCell*>(other) != nullptr;
}

uintptr_t TqCell::GetValueAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqCell::GetValueValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqCell::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> value_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", GetValueAddress(), 1, 4, std::move(value_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqBytecodeArray::GetName() const {
  return "v8::internal::BytecodeArray";
}

void TqBytecodeArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitBytecodeArray(this);
}

bool TqBytecodeArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqBytecodeArray*>(other) != nullptr;
}

uintptr_t TqBytecodeArray::GetConstantPoolAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqBytecodeArray::GetConstantPoolValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetConstantPoolAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqBytecodeArray::GetHandlerTableAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqBytecodeArray::GetHandlerTableValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetHandlerTableAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqBytecodeArray::GetSourcePositionTableAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqBytecodeArray::GetSourcePositionTableValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSourcePositionTableAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqBytecodeArray::GetFrameSizeAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqBytecodeArray::GetFrameSizeValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetFrameSizeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqBytecodeArray::GetParameterSizeAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqBytecodeArray::GetParameterSizeValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetParameterSizeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqBytecodeArray::GetIncomingNewTargetOrGeneratorRegisterAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqBytecodeArray::GetIncomingNewTargetOrGeneratorRegisterValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetIncomingNewTargetOrGeneratorRegisterAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqBytecodeArray::GetOsrNestingLevelAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqBytecodeArray::GetOsrNestingLevelValue(d::MemoryAccessor accessor) const {
  int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetOsrNestingLevelAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqBytecodeArray::GetBytecodeAgeAddress() const {
  return address_ - i::kHeapObjectTag + 33;
}

Value<int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqBytecodeArray::GetBytecodeAgeValue(d::MemoryAccessor accessor) const {
  int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetBytecodeAgeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqBytecodeArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqFixedArrayBase::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> constant_pool_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("constant_pool", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetConstantPoolAddress(), 1, 4, std::move(constant_pool_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> handler_table_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("handler_table", "v8::internal::TaggedValue", "v8::internal::ByteArray", GetHandlerTableAddress(), 1, 4, std::move(handler_table_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> source_position_table_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("source_position_table", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetSourcePositionTableAddress(), 1, 4, std::move(source_position_table_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> frame_size_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("frame_size", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetFrameSizeAddress(), 1, 4, std::move(frame_size_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> parameter_size_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("parameter_size", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetParameterSizeAddress(), 1, 4, std::move(parameter_size_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> incoming_new_target_or_generator_register_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("incoming_new_target_or_generator_register", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetIncomingNewTargetOrGeneratorRegisterAddress(), 1, 4, std::move(incoming_new_target_or_generator_register_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> osr_nesting_level_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("osr_nesting_level", CheckTypeName<int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int8_t"), CheckTypeName<int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int8_t"), GetOsrNestingLevelAddress(), 1, 1, std::move(osr_nesting_level_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bytecode_age_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("bytecode_age", CheckTypeName<int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int8_t"), CheckTypeName<int8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int8_t"), GetBytecodeAgeAddress(), 1, 1, std::move(bytecode_age_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqScopeInfo::GetName() const {
  return "v8::internal::ScopeInfo";
}

void TqScopeInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitScopeInfo(this);
}

bool TqScopeInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqScopeInfo*>(other) != nullptr;
}

uintptr_t TqScopeInfo::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqScopeInfo::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScopeInfo::GetParameterCountAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqScopeInfo::GetParameterCountValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetParameterCountAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScopeInfo::GetContextLocalCountAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqScopeInfo::GetContextLocalCountValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextLocalCountAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScopeInfo::GetContextLocalNamesAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqScopeInfo::GetContextLocalNamesValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextLocalNamesAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqScopeInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("scope_type", CheckTypeName<ScopeType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ScopeType"), CheckTypeName<ScopeType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ScopeType"), 0, 4, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("sloppy_eval_can_extend_vars", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("language_mode", CheckTypeName<LanguageMode /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("LanguageMode"), CheckTypeName<LanguageMode /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("LanguageMode"), 0, 1, 6));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("declaration_scope", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 7));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("receiver_variable", CheckTypeName<VariableAllocationInfo /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("VariableAllocationInfo"), CheckTypeName<VariableAllocationInfo /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("VariableAllocationInfo"), 0, 2, 8));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_class_brand", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 10));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_saved_class_variable_index", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 11));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_new_target", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 12));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("function_variable", CheckTypeName<VariableAllocationInfo /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("VariableAllocationInfo"), CheckTypeName<VariableAllocationInfo /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("VariableAllocationInfo"), 0, 2, 13));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_inferred_function_name", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 15));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_asm_module", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 16));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_simple_parameters", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 17));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("function_kind", CheckTypeName<FunctionKind /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("FunctionKind"), CheckTypeName<FunctionKind /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("FunctionKind"), 0, 5, 18));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_outer_scope_info", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 23));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_debug_evaluate_scope", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 24));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("force_context_allocation", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 25));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("private_name_lookup_skips_outer_class", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 26));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_context_extension_slot", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 27));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_repl_mode_scope", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 28));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_locals_block_list", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 29));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_empty", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 30));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> parameter_count_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("parameter_count", "v8::internal::TaggedValue", "v8::internal::Object", GetParameterCountAddress(), 1, 4, std::move(parameter_count_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> context_local_count_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("context_local_count", "v8::internal::TaggedValue", "v8::internal::Object", GetContextLocalCountAddress(), 1, 4, std::move(context_local_count_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> context_local_names_struct_field_list;
  auto indexed_field_slice_context_local_names = TqDebugFieldSliceScopeInfoContextLocalNames(accessor, address_);
  if (indexed_field_slice_context_local_names.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("context_local_names", "v8::internal::TaggedValue", "v8::internal::String", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_context_local_names.value), std::get<2>(indexed_field_slice_context_local_names.value), 4, std::move(context_local_names_struct_field_list), GetArrayKind(indexed_field_slice_context_local_names.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> context_local_infos_struct_field_list;
  context_local_infos_struct_field_list.push_back(std::make_unique<StructProperty>("variable_mode", CheckTypeName<VariableMode /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("VariableMode"), CheckTypeName<VariableMode /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("VariableMode"), 0, 4, 1));
  context_local_infos_struct_field_list.push_back(std::make_unique<StructProperty>("init_flag", CheckTypeName<InitializationFlag /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("InitializationFlag"), CheckTypeName<InitializationFlag /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("InitializationFlag"), 0, 1, 5));
  context_local_infos_struct_field_list.push_back(std::make_unique<StructProperty>("maybe_assigned_flag", CheckTypeName<MaybeAssignedFlag /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("MaybeAssignedFlag"), CheckTypeName<MaybeAssignedFlag /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("MaybeAssignedFlag"), 0, 1, 6));
  context_local_infos_struct_field_list.push_back(std::make_unique<StructProperty>("parameter_number", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), 0, 16, 7));
  context_local_infos_struct_field_list.push_back(std::make_unique<StructProperty>("is_static_flag", CheckTypeName<IsStaticFlag /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("IsStaticFlag"), CheckTypeName<IsStaticFlag /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("IsStaticFlag"), 0, 1, 23));
  auto indexed_field_slice_context_local_infos = TqDebugFieldSliceScopeInfoContextLocalInfos(accessor, address_);
  if (indexed_field_slice_context_local_infos.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("context_local_infos", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_context_local_infos.value), std::get<2>(indexed_field_slice_context_local_infos.value), 4, std::move(context_local_infos_struct_field_list), GetArrayKind(indexed_field_slice_context_local_infos.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> saved_class_variable_info_struct_field_list;
  auto indexed_field_slice_saved_class_variable_info = TqDebugFieldSliceScopeInfoSavedClassVariableInfo(accessor, address_);
  if (indexed_field_slice_saved_class_variable_info.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("saved_class_variable_info", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_saved_class_variable_info.value), std::get<2>(indexed_field_slice_saved_class_variable_info.value), 4, std::move(saved_class_variable_info_struct_field_list), GetArrayKind(indexed_field_slice_saved_class_variable_info.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> receiver_info_struct_field_list;
  auto indexed_field_slice_receiver_info = TqDebugFieldSliceScopeInfoReceiverInfo(accessor, address_);
  if (indexed_field_slice_receiver_info.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("receiver_info", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_receiver_info.value), std::get<2>(indexed_field_slice_receiver_info.value), 4, std::move(receiver_info_struct_field_list), GetArrayKind(indexed_field_slice_receiver_info.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> function_variable_info_struct_field_list;
  function_variable_info_struct_field_list.push_back(std::make_unique<StructProperty>("name", "v8::internal::TaggedValue", "v8::internal::Object", 0, 0, 0));
  function_variable_info_struct_field_list.push_back(std::make_unique<StructProperty>("context_or_stack_slot_index", "v8::internal::TaggedValue", "v8::internal::Object", 4, 0, 0));
  auto indexed_field_slice_function_variable_info = TqDebugFieldSliceScopeInfoFunctionVariableInfo(accessor, address_);
  if (indexed_field_slice_function_variable_info.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("function_variable_info", "", "", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_function_variable_info.value), std::get<2>(indexed_field_slice_function_variable_info.value), 8, std::move(function_variable_info_struct_field_list), GetArrayKind(indexed_field_slice_function_variable_info.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> inferred_function_name_struct_field_list;
  auto indexed_field_slice_inferred_function_name = TqDebugFieldSliceScopeInfoInferredFunctionName(accessor, address_);
  if (indexed_field_slice_inferred_function_name.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("inferred_function_name", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_inferred_function_name.value), std::get<2>(indexed_field_slice_inferred_function_name.value), 4, std::move(inferred_function_name_struct_field_list), GetArrayKind(indexed_field_slice_inferred_function_name.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> position_info_struct_field_list;
  position_info_struct_field_list.push_back(std::make_unique<StructProperty>("start", "v8::internal::TaggedValue", "v8::internal::Object", 0, 0, 0));
  position_info_struct_field_list.push_back(std::make_unique<StructProperty>("end", "v8::internal::TaggedValue", "v8::internal::Object", 4, 0, 0));
  auto indexed_field_slice_position_info = TqDebugFieldSliceScopeInfoPositionInfo(accessor, address_);
  if (indexed_field_slice_position_info.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("position_info", "", "", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_position_info.value), std::get<2>(indexed_field_slice_position_info.value), 8, std::move(position_info_struct_field_list), GetArrayKind(indexed_field_slice_position_info.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> outer_scope_info_struct_field_list;
  auto indexed_field_slice_outer_scope_info = TqDebugFieldSliceScopeInfoOuterScopeInfo(accessor, address_);
  if (indexed_field_slice_outer_scope_info.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("outer_scope_info", "v8::internal::TaggedValue", "v8::internal::HeapObject", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_outer_scope_info.value), std::get<2>(indexed_field_slice_outer_scope_info.value), 4, std::move(outer_scope_info_struct_field_list), GetArrayKind(indexed_field_slice_outer_scope_info.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> locals_block_list_struct_field_list;
  auto indexed_field_slice_locals_block_list = TqDebugFieldSliceScopeInfoLocalsBlockList(accessor, address_);
  if (indexed_field_slice_locals_block_list.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("locals_block_list", "v8::internal::TaggedValue", "v8::internal::FixedArray", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_locals_block_list.value), std::get<2>(indexed_field_slice_locals_block_list.value), 4, std::move(locals_block_list_struct_field_list), GetArrayKind(indexed_field_slice_locals_block_list.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> module_info_struct_field_list;
  auto indexed_field_slice_module_info = TqDebugFieldSliceScopeInfoModuleInfo(accessor, address_);
  if (indexed_field_slice_module_info.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("module_info", "v8::internal::TaggedValue", "v8::internal::FixedArray", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_module_info.value), std::get<2>(indexed_field_slice_module_info.value), 4, std::move(module_info_struct_field_list), GetArrayKind(indexed_field_slice_module_info.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> module_variable_count_struct_field_list;
  auto indexed_field_slice_module_variable_count = TqDebugFieldSliceScopeInfoModuleVariableCount(accessor, address_);
  if (indexed_field_slice_module_variable_count.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("module_variable_count", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_module_variable_count.value), std::get<2>(indexed_field_slice_module_variable_count.value), 4, std::move(module_variable_count_struct_field_list), GetArrayKind(indexed_field_slice_module_variable_count.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> module_variables_struct_field_list;
  module_variables_struct_field_list.push_back(std::make_unique<StructProperty>("name", "v8::internal::TaggedValue", "v8::internal::String", 0, 0, 0));
  module_variables_struct_field_list.push_back(std::make_unique<StructProperty>("index", "v8::internal::TaggedValue", "v8::internal::Object", 4, 0, 0));
  module_variables_struct_field_list.push_back(std::make_unique<StructProperty>("properties", "v8::internal::TaggedValue", "v8::internal::Object", 8, 0, 0));
  auto indexed_field_slice_module_variables = TqDebugFieldSliceScopeInfoModuleVariables(accessor, address_);
  if (indexed_field_slice_module_variables.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("module_variables", "", "", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_module_variables.value), std::get<2>(indexed_field_slice_module_variables.value), 12, std::move(module_variables_struct_field_list), GetArrayKind(indexed_field_slice_module_variables.validity)));
  }
  return result;
}

const char* TqFixedDoubleArray::GetName() const {
  return "v8::internal::FixedDoubleArray";
}

void TqFixedDoubleArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitFixedDoubleArray(this);
}

bool TqFixedDoubleArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqFixedDoubleArray*>(other) != nullptr;
}

uintptr_t TqFixedDoubleArray::GetFloatsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

std::vector<std::unique_ptr<ObjectProperty>> TqFixedDoubleArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqFixedArrayBase::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> floats_struct_field_list;
  auto indexed_field_slice_floats = TqDebugFieldSliceFixedDoubleArrayFloats(accessor, address_);
  if (indexed_field_slice_floats.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("floats", "", "", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_floats.value), std::get<2>(indexed_field_slice_floats.value), 8, std::move(floats_struct_field_list), GetArrayKind(indexed_field_slice_floats.validity)));
  }
  return result;
}

const char* TqRegExpMatchInfo::GetName() const {
  return "v8::internal::RegExpMatchInfo";
}

void TqRegExpMatchInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitRegExpMatchInfo(this);
}

bool TqRegExpMatchInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqRegExpMatchInfo*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqRegExpMatchInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqFixedArray::GetProperties(accessor);
  return result;
}

const char* TqBreakPoint::GetName() const {
  return "v8::internal::BreakPoint";
}

void TqBreakPoint::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitBreakPoint(this);
}

bool TqBreakPoint::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqBreakPoint*>(other) != nullptr;
}

uintptr_t TqBreakPoint::GetIdAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqBreakPoint::GetIdValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIdAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqBreakPoint::GetConditionAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqBreakPoint::GetConditionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetConditionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqBreakPoint::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> id_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("id", "v8::internal::TaggedValue", "v8::internal::Object", GetIdAddress(), 1, 4, std::move(id_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> condition_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("condition", "v8::internal::TaggedValue", "v8::internal::String", GetConditionAddress(), 1, 4, std::move(condition_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqBreakPointInfo::GetName() const {
  return "v8::internal::BreakPointInfo";
}

void TqBreakPointInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitBreakPointInfo(this);
}

bool TqBreakPointInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqBreakPointInfo*>(other) != nullptr;
}

uintptr_t TqBreakPointInfo::GetSourcePositionAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqBreakPointInfo::GetSourcePositionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSourcePositionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqBreakPointInfo::GetBreakPointsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqBreakPointInfo::GetBreakPointsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBreakPointsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqBreakPointInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> source_position_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("source_position", "v8::internal::TaggedValue", "v8::internal::Object", GetSourcePositionAddress(), 1, 4, std::move(source_position_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> break_points_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("break_points", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetBreakPointsAddress(), 1, 4, std::move(break_points_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqDebugInfo::GetName() const {
  return "v8::internal::DebugInfo";
}

void TqDebugInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitDebugInfo(this);
}

bool TqDebugInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqDebugInfo*>(other) != nullptr;
}

uintptr_t TqDebugInfo::GetSharedAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqDebugInfo::GetSharedValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSharedAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDebugInfo::GetDebuggerHintsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqDebugInfo::GetDebuggerHintsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDebuggerHintsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDebugInfo::GetScriptAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqDebugInfo::GetScriptValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetScriptAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDebugInfo::GetOriginalBytecodeArrayAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqDebugInfo::GetOriginalBytecodeArrayValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetOriginalBytecodeArrayAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDebugInfo::GetDebugBytecodeArrayAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqDebugInfo::GetDebugBytecodeArrayValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDebugBytecodeArrayAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDebugInfo::GetBreakPointsAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqDebugInfo::GetBreakPointsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBreakPointsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDebugInfo::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqDebugInfo::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDebugInfo::GetCoverageInfoAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqDebugInfo::GetCoverageInfoValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCoverageInfoAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqDebugInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> shared_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("shared", "v8::internal::TaggedValue", "v8::internal::SharedFunctionInfo", GetSharedAddress(), 1, 4, std::move(shared_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> debugger_hints_struct_field_list;
  debugger_hints_struct_field_list.push_back(std::make_unique<StructProperty>("side_effect_state", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 2, 1));
  debugger_hints_struct_field_list.push_back(std::make_unique<StructProperty>("debug_is_blackboxed", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  debugger_hints_struct_field_list.push_back(std::make_unique<StructProperty>("computed_debug_is_blackboxed", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  debugger_hints_struct_field_list.push_back(std::make_unique<StructProperty>("debugging_id", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 20, 5));
  result.push_back(std::make_unique<ObjectProperty>("debugger_hints", "v8::internal::TaggedValue", "v8::internal::Object", GetDebuggerHintsAddress(), 1, 4, std::move(debugger_hints_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> script_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("script", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetScriptAddress(), 1, 4, std::move(script_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> original_bytecode_array_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("original_bytecode_array", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetOriginalBytecodeArrayAddress(), 1, 4, std::move(original_bytecode_array_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> debug_bytecode_array_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("debug_bytecode_array", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetDebugBytecodeArrayAddress(), 1, 4, std::move(debug_bytecode_array_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> break_points_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("break_points", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetBreakPointsAddress(), 1, 4, std::move(break_points_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_break_info", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("prepared_for_debug_execution", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_coverage_info", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("break_at_entry", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("can_break_at_entry", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("debug_execution_mode", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 6));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> coverage_info_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("coverage_info", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetCoverageInfoAddress(), 1, 4, std::move(coverage_info_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqCoverageInfo::GetName() const {
  return "v8::internal::CoverageInfo";
}

void TqCoverageInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitCoverageInfo(this);
}

bool TqCoverageInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqCoverageInfo*>(other) != nullptr;
}

uintptr_t TqCoverageInfo::GetSlotCountAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqCoverageInfo::GetSlotCountValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetSlotCountAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqCoverageInfo::GetSlotsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

std::vector<std::unique_ptr<ObjectProperty>> TqCoverageInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> slot_count_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("slot_count", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetSlotCountAddress(), 1, 4, std::move(slot_count_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> slots_struct_field_list;
  slots_struct_field_list.push_back(std::make_unique<StructProperty>("start_source_position", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 0, 0));
  slots_struct_field_list.push_back(std::make_unique<StructProperty>("end_source_position", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 4, 0, 0));
  slots_struct_field_list.push_back(std::make_unique<StructProperty>("block_count", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 8, 0, 0));
  slots_struct_field_list.push_back(std::make_unique<StructProperty>("padding", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 12, 0, 0));
  auto indexed_field_slice_slots = TqDebugFieldSliceCoverageInfoSlots(accessor, address_);
  if (indexed_field_slice_slots.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("slots", "", "", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_slots.value), std::get<2>(indexed_field_slice_slots.value), 16, std::move(slots_struct_field_list), GetArrayKind(indexed_field_slice_slots.validity)));
  }
  return result;
}

const char* TqEnumCache::GetName() const {
  return "v8::internal::EnumCache";
}

void TqEnumCache::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitEnumCache(this);
}

bool TqEnumCache::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqEnumCache*>(other) != nullptr;
}

uintptr_t TqEnumCache::GetKeysAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqEnumCache::GetKeysValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetKeysAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqEnumCache::GetIndicesAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqEnumCache::GetIndicesValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIndicesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqEnumCache::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> keys_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("keys", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetKeysAddress(), 1, 4, std::move(keys_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> indices_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("indices", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetIndicesAddress(), 1, 4, std::move(indices_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqAccessorPair::GetName() const {
  return "v8::internal::AccessorPair";
}

void TqAccessorPair::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitAccessorPair(this);
}

bool TqAccessorPair::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqAccessorPair*>(other) != nullptr;
}

uintptr_t TqAccessorPair::GetGetterAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqAccessorPair::GetGetterValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetGetterAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAccessorPair::GetSetterAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqAccessorPair::GetSetterValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSetterAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqAccessorPair::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> getter_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("getter", "v8::internal::TaggedValue", "v8::internal::Object", GetGetterAddress(), 1, 4, std::move(getter_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> setter_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("setter", "v8::internal::TaggedValue", "v8::internal::Object", GetSetterAddress(), 1, 4, std::move(setter_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqClassPositions::GetName() const {
  return "v8::internal::ClassPositions";
}

void TqClassPositions::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitClassPositions(this);
}

bool TqClassPositions::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqClassPositions*>(other) != nullptr;
}

uintptr_t TqClassPositions::GetStartAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqClassPositions::GetStartValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetStartAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqClassPositions::GetEndAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqClassPositions::GetEndValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEndAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqClassPositions::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> start_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("start", "v8::internal::TaggedValue", "v8::internal::Object", GetStartAddress(), 1, 4, std::move(start_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> end_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("end", "v8::internal::TaggedValue", "v8::internal::Object", GetEndAddress(), 1, 4, std::move(end_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqDescriptorArray::GetName() const {
  return "v8::internal::DescriptorArray";
}

void TqDescriptorArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitDescriptorArray(this);
}

bool TqDescriptorArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqDescriptorArray*>(other) != nullptr;
}

uintptr_t TqDescriptorArray::GetNumberOfAllDescriptorsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqDescriptorArray::GetNumberOfAllDescriptorsValue(d::MemoryAccessor accessor) const {
  uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfAllDescriptorsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqDescriptorArray::GetNumberOfDescriptorsAddress() const {
  return address_ - i::kHeapObjectTag + 6;
}

Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqDescriptorArray::GetNumberOfDescriptorsValue(d::MemoryAccessor accessor) const {
  uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfDescriptorsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqDescriptorArray::GetRawNumberOfMarkedDescriptorsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqDescriptorArray::GetRawNumberOfMarkedDescriptorsValue(d::MemoryAccessor accessor) const {
  uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetRawNumberOfMarkedDescriptorsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqDescriptorArray::GetFiller16BitsAddress() const {
  return address_ - i::kHeapObjectTag + 10;
}

Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqDescriptorArray::GetFiller16BitsValue(d::MemoryAccessor accessor) const {
  uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetFiller16BitsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqDescriptorArray::GetEnumCacheAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqDescriptorArray::GetEnumCacheValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEnumCacheAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqDescriptorArray::GetDescriptorsAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

std::vector<std::unique_ptr<ObjectProperty>> TqDescriptorArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> number_of_all_descriptors_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_all_descriptors", CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), GetNumberOfAllDescriptorsAddress(), 1, 2, std::move(number_of_all_descriptors_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> number_of_descriptors_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_descriptors", CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), GetNumberOfDescriptorsAddress(), 1, 2, std::move(number_of_descriptors_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> raw_number_of_marked_descriptors_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("raw_number_of_marked_descriptors", CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), GetRawNumberOfMarkedDescriptorsAddress(), 1, 2, std::move(raw_number_of_marked_descriptors_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> filler16_bits_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("filler16_bits", CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), GetFiller16BitsAddress(), 1, 2, std::move(filler16_bits_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> enum_cache_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("enum_cache", "v8::internal::TaggedValue", "v8::internal::EnumCache", GetEnumCacheAddress(), 1, 4, std::move(enum_cache_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> descriptors_struct_field_list;
  descriptors_struct_field_list.push_back(std::make_unique<StructProperty>("key", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", 0, 0, 0));
  descriptors_struct_field_list.push_back(std::make_unique<StructProperty>("details", "v8::internal::TaggedValue", "v8::internal::Object", 4, 0, 0));
  descriptors_struct_field_list.push_back(std::make_unique<StructProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", 8, 0, 0));
  auto indexed_field_slice_descriptors = TqDebugFieldSliceDescriptorArrayDescriptors(accessor, address_);
  if (indexed_field_slice_descriptors.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("descriptors", "", "", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_descriptors.value), std::get<2>(indexed_field_slice_descriptors.value), 12, std::move(descriptors_struct_field_list), GetArrayKind(indexed_field_slice_descriptors.validity)));
  }
  return result;
}

const char* TqStrongDescriptorArray::GetName() const {
  return "v8::internal::StrongDescriptorArray";
}

void TqStrongDescriptorArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitStrongDescriptorArray(this);
}

bool TqStrongDescriptorArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqStrongDescriptorArray*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqStrongDescriptorArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqDescriptorArray::GetProperties(accessor);
  return result;
}

const char* TqEmbedderDataArray::GetName() const {
  return "v8::internal::EmbedderDataArray";
}

void TqEmbedderDataArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitEmbedderDataArray(this);
}

bool TqEmbedderDataArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqEmbedderDataArray*>(other) != nullptr;
}

uintptr_t TqEmbedderDataArray::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqEmbedderDataArray::GetLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqEmbedderDataArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqFeedbackCell::GetName() const {
  return "v8::internal::FeedbackCell";
}

void TqFeedbackCell::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitFeedbackCell(this);
}

bool TqFeedbackCell::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqFeedbackCell*>(other) != nullptr;
}

uintptr_t TqFeedbackCell::GetValueAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqFeedbackCell::GetValueValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFeedbackCell::GetInterruptBudgetAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqFeedbackCell::GetInterruptBudgetValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetInterruptBudgetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqFeedbackCell::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> value_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetValueAddress(), 1, 4, std::move(value_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> interrupt_budget_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("interrupt_budget", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetInterruptBudgetAddress(), 1, 4, std::move(interrupt_budget_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqFeedbackVector::GetName() const {
  return "v8::internal::FeedbackVector";
}

void TqFeedbackVector::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitFeedbackVector(this);
}

bool TqFeedbackVector::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqFeedbackVector*>(other) != nullptr;
}

uintptr_t TqFeedbackVector::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqFeedbackVector::GetLengthValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqFeedbackVector::GetInvocationCountAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqFeedbackVector::GetInvocationCountValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetInvocationCountAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqFeedbackVector::GetProfilerTicksAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqFeedbackVector::GetProfilerTicksValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetProfilerTicksAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqFeedbackVector::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqFeedbackVector::GetFlagsValue(d::MemoryAccessor accessor) const {
  uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqFeedbackVector::GetSharedFunctionInfoAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqFeedbackVector::GetSharedFunctionInfoValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSharedFunctionInfoAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFeedbackVector::GetMaybeOptimizedCodeAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqFeedbackVector::GetMaybeOptimizedCodeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetMaybeOptimizedCodeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFeedbackVector::GetClosureFeedbackCellArrayAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqFeedbackVector::GetClosureFeedbackCellArrayValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetClosureFeedbackCellArrayAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFeedbackVector::GetRawFeedbackSlotsAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqFeedbackVector::GetRawFeedbackSlotsValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRawFeedbackSlotsAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqFeedbackVector::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> invocation_count_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("invocation_count", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetInvocationCountAddress(), 1, 4, std::move(invocation_count_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> profiler_ticks_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("profiler_ticks", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetProfilerTicksAddress(), 1, 4, std::move(profiler_ticks_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("optimization_marker", CheckTypeName<OptimizationMarker /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("OptimizationMarker"), CheckTypeName<OptimizationMarker /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("OptimizationMarker"), 0, 3, 0));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("optimization_tier", CheckTypeName<OptimizationTier /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("OptimizationTier"), CheckTypeName<OptimizationTier /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("OptimizationTier"), 0, 2, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("global_ticks_at_last_runtime_profiler_interrupt", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), 0, 24, 5));
  result.push_back(std::make_unique<ObjectProperty>("flags", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> shared_function_info_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("shared_function_info", "v8::internal::TaggedValue", "v8::internal::SharedFunctionInfo", GetSharedFunctionInfoAddress(), 1, 4, std::move(shared_function_info_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> maybe_optimized_code_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("maybe_optimized_code", "v8::internal::TaggedValue", "v8::internal::Object", GetMaybeOptimizedCodeAddress(), 1, 4, std::move(maybe_optimized_code_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> closure_feedback_cell_array_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("closure_feedback_cell_array", "v8::internal::TaggedValue", "v8::internal::ClosureFeedbackCellArray", GetClosureFeedbackCellArrayAddress(), 1, 4, std::move(closure_feedback_cell_array_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> raw_feedback_slots_struct_field_list;
  auto indexed_field_slice_raw_feedback_slots = TqDebugFieldSliceFeedbackVectorRawFeedbackSlots(accessor, address_);
  if (indexed_field_slice_raw_feedback_slots.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("raw_feedback_slots", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_raw_feedback_slots.value), std::get<2>(indexed_field_slice_raw_feedback_slots.value), 4, std::move(raw_feedback_slots_struct_field_list), GetArrayKind(indexed_field_slice_raw_feedback_slots.validity)));
  }
  return result;
}

const char* TqArrayList::GetName() const {
  return "v8::internal::ArrayList";
}

void TqArrayList::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitArrayList(this);
}

bool TqArrayList::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqArrayList*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqArrayList::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqFixedArray::GetProperties(accessor);
  return result;
}

const char* TqTemplateList::GetName() const {
  return "v8::internal::TemplateList";
}

void TqTemplateList::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitTemplateList(this);
}

bool TqTemplateList::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqTemplateList*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqTemplateList::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqFixedArray::GetProperties(accessor);
  return result;
}

const char* TqWeakArrayList::GetName() const {
  return "v8::internal::WeakArrayList";
}

void TqWeakArrayList::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitWeakArrayList(this);
}

bool TqWeakArrayList::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqWeakArrayList*>(other) != nullptr;
}

uintptr_t TqWeakArrayList::GetCapacityAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqWeakArrayList::GetCapacityValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCapacityAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakArrayList::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqWeakArrayList::GetLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqWeakArrayList::GetObjectsAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqWeakArrayList::GetObjectsValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetObjectsAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqWeakArrayList::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> capacity_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("capacity", "v8::internal::TaggedValue", "v8::internal::Object", GetCapacityAddress(), 1, 4, std::move(capacity_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> objects_struct_field_list;
  auto indexed_field_slice_objects = TqDebugFieldSliceWeakArrayListObjects(accessor, address_);
  if (indexed_field_slice_objects.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("objects", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_objects.value), std::get<2>(indexed_field_slice_objects.value), 4, std::move(objects_struct_field_list), GetArrayKind(indexed_field_slice_objects.validity)));
  }
  return result;
}

const char* TqFreeSpace::GetName() const {
  return "v8::internal::FreeSpace";
}

void TqFreeSpace::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitFreeSpace(this);
}

bool TqFreeSpace::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqFreeSpace*>(other) != nullptr;
}

uintptr_t TqFreeSpace::GetSizeAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqFreeSpace::GetSizeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSizeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFreeSpace::GetNextAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqFreeSpace::GetNextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqFreeSpace::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> size_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("size", "v8::internal::TaggedValue", "v8::internal::Object", GetSizeAddress(), 1, 4, std::move(size_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> next_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("next", "v8::internal::TaggedValue", "v8::internal::Object", GetNextAddress(), 1, 4, std::move(next_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSArrayBuffer::GetName() const {
  return "v8::internal::JSArrayBuffer";
}

void TqJSArrayBuffer::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSArrayBuffer(this);
}

bool TqJSArrayBuffer::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSArrayBuffer*>(other) != nullptr;
}

uintptr_t TqJSArrayBuffer::GetByteLengthAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSArrayBuffer::GetByteLengthValue(d::MemoryAccessor accessor) const {
  uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetByteLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqJSArrayBuffer::GetMaxByteLengthAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSArrayBuffer::GetMaxByteLengthValue(d::MemoryAccessor accessor) const {
  uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetMaxByteLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqJSArrayBuffer::GetBackingStoreAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSArrayBuffer::GetBackingStoreValue(d::MemoryAccessor accessor) const {
  ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetBackingStoreAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqJSArrayBuffer::GetExtensionAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<Address /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSArrayBuffer::GetExtensionValue(d::MemoryAccessor accessor) const {
  Address /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetExtensionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqJSArrayBuffer::GetBitFieldAddress() const {
  return address_ - i::kHeapObjectTag + 44;
}

Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSArrayBuffer::GetBitFieldValue(d::MemoryAccessor accessor) const {
  uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetBitFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSArrayBuffer::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> byte_length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("byte_length", CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), GetByteLengthAddress(), 1, 8, std::move(byte_length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> max_byte_length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("max_byte_length", CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), CheckTypeName<uintptr_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uintptr_t"), GetMaxByteLengthAddress(), 1, 8, std::move(max_byte_length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> backing_store_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("backing_store", CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), GetBackingStoreAddress(), 1, 8, std::move(backing_store_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> extension_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("extension", CheckTypeName<Address /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("Address"), CheckTypeName<Address /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("Address"), GetExtensionAddress(), 1, 8, std::move(extension_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bit_field_struct_field_list;
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_external", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 0));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_detachable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("was_detached", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_asm_js_memory", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_shared", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("is_resizable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  result.push_back(std::make_unique<ObjectProperty>("bit_field", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), GetBitFieldAddress(), 1, 4, std::move(bit_field_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSDataView::GetName() const {
  return "v8::internal::JSDataView";
}

void TqJSDataView::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSDataView(this);
}

bool TqJSDataView::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSDataView*>(other) != nullptr;
}

uintptr_t TqJSDataView::GetDataPointerAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqJSDataView::GetDataPointerValue(d::MemoryAccessor accessor) const {
  ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetDataPointerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSDataView::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSArrayBufferView::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> data_pointer_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data_pointer", CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), GetDataPointerAddress(), 1, 8, std::move(data_pointer_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSArrayIterator::GetName() const {
  return "v8::internal::JSArrayIterator";
}

void TqJSArrayIterator::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSArrayIterator(this);
}

bool TqJSArrayIterator::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSArrayIterator*>(other) != nullptr;
}

uintptr_t TqJSArrayIterator::GetIteratedObjectAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSArrayIterator::GetIteratedObjectValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIteratedObjectAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSArrayIterator::GetNextIndexAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSArrayIterator::GetNextIndexValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNextIndexAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSArrayIterator::GetKindAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSArrayIterator::GetKindValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetKindAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSArrayIterator::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> iterated_object_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("iterated_object", "v8::internal::TaggedValue", "v8::internal::JSReceiver", GetIteratedObjectAddress(), 1, 4, std::move(iterated_object_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> next_index_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("next_index", "v8::internal::TaggedValue", "v8::internal::Object", GetNextIndexAddress(), 1, 4, std::move(next_index_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> kind_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("kind", "v8::internal::TaggedValue", "v8::internal::Object", GetKindAddress(), 1, 4, std::move(kind_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSArray::GetName() const {
  return "v8::internal::JSArray";
}

void TqJSArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSArray(this);
}

bool TqJSArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSArray*>(other) != nullptr;
}

uintptr_t TqJSArray::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSArray::GetLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSCollectionIterator::GetName() const {
  return "v8::internal::JSCollectionIterator";
}

void TqJSCollectionIterator::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSCollectionIterator(this);
}

bool TqJSCollectionIterator::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSCollectionIterator*>(other) != nullptr;
}

uintptr_t TqJSCollectionIterator::GetTableAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSCollectionIterator::GetTableValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTableAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSCollectionIterator::GetIndexAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSCollectionIterator::GetIndexValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIndexAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSCollectionIterator::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> table_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("table", "v8::internal::TaggedValue", "v8::internal::Object", GetTableAddress(), 1, 4, std::move(table_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> index_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("index", "v8::internal::TaggedValue", "v8::internal::Object", GetIndexAddress(), 1, 4, std::move(index_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSCollection::GetName() const {
  return "v8::internal::JSCollection";
}

void TqJSCollection::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSCollection(this);
}

bool TqJSCollection::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSCollection*>(other) != nullptr;
}

uintptr_t TqJSCollection::GetTableAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSCollection::GetTableValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTableAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSCollection::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> table_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("table", "v8::internal::TaggedValue", "v8::internal::Object", GetTableAddress(), 1, 4, std::move(table_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSSet::GetName() const {
  return "v8::internal::JSSet";
}

void TqJSSet::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSSet(this);
}

bool TqJSSet::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSSet*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSSet::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSCollection::GetProperties(accessor);
  return result;
}

const char* TqJSMap::GetName() const {
  return "v8::internal::JSMap";
}

void TqJSMap::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSMap(this);
}

bool TqJSMap::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSMap*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSMap::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSCollection::GetProperties(accessor);
  return result;
}

const char* TqJSWeakCollection::GetName() const {
  return "v8::internal::JSWeakCollection";
}

void TqJSWeakCollection::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSWeakCollection(this);
}

bool TqJSWeakCollection::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSWeakCollection*>(other) != nullptr;
}

uintptr_t TqJSWeakCollection::GetTableAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSWeakCollection::GetTableValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTableAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSWeakCollection::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> table_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("table", "v8::internal::TaggedValue", "v8::internal::Object", GetTableAddress(), 1, 4, std::move(table_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSWeakSet::GetName() const {
  return "v8::internal::JSWeakSet";
}

void TqJSWeakSet::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSWeakSet(this);
}

bool TqJSWeakSet::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSWeakSet*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSWeakSet::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSWeakCollection::GetProperties(accessor);
  return result;
}

const char* TqJSWeakMap::GetName() const {
  return "v8::internal::JSWeakMap";
}

void TqJSWeakMap::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSWeakMap(this);
}

bool TqJSWeakMap::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSWeakMap*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSWeakMap::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSWeakCollection::GetProperties(accessor);
  return result;
}

const char* TqJSGeneratorObject::GetName() const {
  return "v8::internal::JSGeneratorObject";
}

void TqJSGeneratorObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSGeneratorObject(this);
}

bool TqJSGeneratorObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSGeneratorObject*>(other) != nullptr;
}

uintptr_t TqJSGeneratorObject::GetFunctionAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSGeneratorObject::GetFunctionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFunctionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSGeneratorObject::GetContextAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSGeneratorObject::GetContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSGeneratorObject::GetReceiverAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSGeneratorObject::GetReceiverValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetReceiverAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSGeneratorObject::GetInputOrDebugPosAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqJSGeneratorObject::GetInputOrDebugPosValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInputOrDebugPosAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSGeneratorObject::GetResumeModeAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqJSGeneratorObject::GetResumeModeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetResumeModeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSGeneratorObject::GetContinuationAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqJSGeneratorObject::GetContinuationValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContinuationAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSGeneratorObject::GetParametersAndRegistersAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqJSGeneratorObject::GetParametersAndRegistersValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetParametersAndRegistersAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSGeneratorObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> function_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("function", "v8::internal::TaggedValue", "v8::internal::JSFunction", GetFunctionAddress(), 1, 4, std::move(function_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("context", "v8::internal::TaggedValue", "v8::internal::Context", GetContextAddress(), 1, 4, std::move(context_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> receiver_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("receiver", "v8::internal::TaggedValue", "v8::internal::Object", GetReceiverAddress(), 1, 4, std::move(receiver_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> input_or_debug_pos_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("input_or_debug_pos", "v8::internal::TaggedValue", "v8::internal::Object", GetInputOrDebugPosAddress(), 1, 4, std::move(input_or_debug_pos_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> resume_mode_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("resume_mode", "v8::internal::TaggedValue", "v8::internal::Object", GetResumeModeAddress(), 1, 4, std::move(resume_mode_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> continuation_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("continuation", "v8::internal::TaggedValue", "v8::internal::Object", GetContinuationAddress(), 1, 4, std::move(continuation_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> parameters_and_registers_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("parameters_and_registers", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetParametersAndRegistersAddress(), 1, 4, std::move(parameters_and_registers_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSAsyncFunctionObject::GetName() const {
  return "v8::internal::JSAsyncFunctionObject";
}

void TqJSAsyncFunctionObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSAsyncFunctionObject(this);
}

bool TqJSAsyncFunctionObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSAsyncFunctionObject*>(other) != nullptr;
}

uintptr_t TqJSAsyncFunctionObject::GetPromiseAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<uintptr_t> TqJSAsyncFunctionObject::GetPromiseValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPromiseAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSAsyncFunctionObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSGeneratorObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> promise_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("promise", "v8::internal::TaggedValue", "v8::internal::JSPromise", GetPromiseAddress(), 1, 4, std::move(promise_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSAsyncGeneratorObject::GetName() const {
  return "v8::internal::JSAsyncGeneratorObject";
}

void TqJSAsyncGeneratorObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSAsyncGeneratorObject(this);
}

bool TqJSAsyncGeneratorObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSAsyncGeneratorObject*>(other) != nullptr;
}

uintptr_t TqJSAsyncGeneratorObject::GetQueueAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<uintptr_t> TqJSAsyncGeneratorObject::GetQueueValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetQueueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSAsyncGeneratorObject::GetIsAwaitingAddress() const {
  return address_ - i::kHeapObjectTag + 44;
}

Value<uintptr_t> TqJSAsyncGeneratorObject::GetIsAwaitingValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIsAwaitingAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSAsyncGeneratorObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSGeneratorObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> queue_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("queue", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetQueueAddress(), 1, 4, std::move(queue_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> is_awaiting_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("is_awaiting", "v8::internal::TaggedValue", "v8::internal::Object", GetIsAwaitingAddress(), 1, 4, std::move(is_awaiting_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqAsyncGeneratorRequest::GetName() const {
  return "v8::internal::AsyncGeneratorRequest";
}

void TqAsyncGeneratorRequest::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitAsyncGeneratorRequest(this);
}

bool TqAsyncGeneratorRequest::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqAsyncGeneratorRequest*>(other) != nullptr;
}

uintptr_t TqAsyncGeneratorRequest::GetNextAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqAsyncGeneratorRequest::GetNextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAsyncGeneratorRequest::GetResumeModeAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqAsyncGeneratorRequest::GetResumeModeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetResumeModeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAsyncGeneratorRequest::GetValueAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqAsyncGeneratorRequest::GetValueValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqAsyncGeneratorRequest::GetPromiseAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqAsyncGeneratorRequest::GetPromiseValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPromiseAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqAsyncGeneratorRequest::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> next_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("next", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetNextAddress(), 1, 4, std::move(next_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> resume_mode_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("resume_mode", "v8::internal::TaggedValue", "v8::internal::Object", GetResumeModeAddress(), 1, 4, std::move(resume_mode_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> value_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", GetValueAddress(), 1, 4, std::move(value_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> promise_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("promise", "v8::internal::TaggedValue", "v8::internal::JSPromise", GetPromiseAddress(), 1, 4, std::move(promise_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSGlobalProxy::GetName() const {
  return "v8::internal::JSGlobalProxy";
}

void TqJSGlobalProxy::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSGlobalProxy(this);
}

bool TqJSGlobalProxy::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSGlobalProxy*>(other) != nullptr;
}

uintptr_t TqJSGlobalProxy::GetNativeContextAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSGlobalProxy::GetNativeContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNativeContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSGlobalProxy::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSSpecialObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> native_context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("native_context", "v8::internal::TaggedValue", "v8::internal::Object", GetNativeContextAddress(), 1, 4, std::move(native_context_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSGlobalObject::GetName() const {
  return "v8::internal::JSGlobalObject";
}

void TqJSGlobalObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSGlobalObject(this);
}

bool TqJSGlobalObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSGlobalObject*>(other) != nullptr;
}

uintptr_t TqJSGlobalObject::GetNativeContextAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSGlobalObject::GetNativeContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNativeContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSGlobalObject::GetGlobalProxyAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSGlobalObject::GetGlobalProxyValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetGlobalProxyAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSGlobalObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSSpecialObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> native_context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("native_context", "v8::internal::TaggedValue", "v8::internal::NativeContext", GetNativeContextAddress(), 1, 4, std::move(native_context_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> global_proxy_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("global_proxy", "v8::internal::TaggedValue", "v8::internal::JSGlobalProxy", GetGlobalProxyAddress(), 1, 4, std::move(global_proxy_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSPrimitiveWrapper::GetName() const {
  return "v8::internal::JSPrimitiveWrapper";
}

void TqJSPrimitiveWrapper::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSPrimitiveWrapper(this);
}

bool TqJSPrimitiveWrapper::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSPrimitiveWrapper*>(other) != nullptr;
}

uintptr_t TqJSPrimitiveWrapper::GetValueAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSPrimitiveWrapper::GetValueValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSPrimitiveWrapper::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSCustomElementsObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> value_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", GetValueAddress(), 1, 4, std::move(value_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSMessageObject::GetName() const {
  return "v8::internal::JSMessageObject";
}

void TqJSMessageObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSMessageObject(this);
}

bool TqJSMessageObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSMessageObject*>(other) != nullptr;
}

uintptr_t TqJSMessageObject::GetMessageTypeAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSMessageObject::GetMessageTypeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetMessageTypeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSMessageObject::GetArgumentsAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSMessageObject::GetArgumentsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetArgumentsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSMessageObject::GetScriptAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSMessageObject::GetScriptValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetScriptAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSMessageObject::GetStackFramesAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqJSMessageObject::GetStackFramesValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetStackFramesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSMessageObject::GetSharedInfoAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqJSMessageObject::GetSharedInfoValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSharedInfoAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSMessageObject::GetBytecodeOffsetAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqJSMessageObject::GetBytecodeOffsetValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBytecodeOffsetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSMessageObject::GetStartPositionAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqJSMessageObject::GetStartPositionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetStartPositionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSMessageObject::GetEndPositionAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<uintptr_t> TqJSMessageObject::GetEndPositionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEndPositionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSMessageObject::GetErrorLevelAddress() const {
  return address_ - i::kHeapObjectTag + 44;
}

Value<uintptr_t> TqJSMessageObject::GetErrorLevelValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetErrorLevelAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSMessageObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> message_type_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("message_type", "v8::internal::TaggedValue", "v8::internal::Object", GetMessageTypeAddress(), 1, 4, std::move(message_type_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> arguments_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("arguments", "v8::internal::TaggedValue", "v8::internal::Object", GetArgumentsAddress(), 1, 4, std::move(arguments_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> script_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("script", "v8::internal::TaggedValue", "v8::internal::Script", GetScriptAddress(), 1, 4, std::move(script_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> stack_frames_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("stack_frames", "v8::internal::TaggedValue", "v8::internal::Object", GetStackFramesAddress(), 1, 4, std::move(stack_frames_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> shared_info_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("shared_info", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetSharedInfoAddress(), 1, 4, std::move(shared_info_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bytecode_offset_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("bytecode_offset", "v8::internal::TaggedValue", "v8::internal::Object", GetBytecodeOffsetAddress(), 1, 4, std::move(bytecode_offset_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> start_position_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("start_position", "v8::internal::TaggedValue", "v8::internal::Object", GetStartPositionAddress(), 1, 4, std::move(start_position_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> end_position_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("end_position", "v8::internal::TaggedValue", "v8::internal::Object", GetEndPositionAddress(), 1, 4, std::move(end_position_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> error_level_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("error_level", "v8::internal::TaggedValue", "v8::internal::Object", GetErrorLevelAddress(), 1, 4, std::move(error_level_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSDate::GetName() const {
  return "v8::internal::JSDate";
}

void TqJSDate::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSDate(this);
}

bool TqJSDate::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSDate*>(other) != nullptr;
}

uintptr_t TqJSDate::GetValueAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSDate::GetValueValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSDate::GetYearAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSDate::GetYearValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetYearAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSDate::GetMonthAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSDate::GetMonthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetMonthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSDate::GetDayAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqJSDate::GetDayValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDayAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSDate::GetWeekdayAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqJSDate::GetWeekdayValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetWeekdayAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSDate::GetHourAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqJSDate::GetHourValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetHourAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSDate::GetMinAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqJSDate::GetMinValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetMinAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSDate::GetSecAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<uintptr_t> TqJSDate::GetSecValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSecAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSDate::GetCacheStampAddress() const {
  return address_ - i::kHeapObjectTag + 44;
}

Value<uintptr_t> TqJSDate::GetCacheStampValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCacheStampAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSDate::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> value_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", GetValueAddress(), 1, 4, std::move(value_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> year_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("year", "v8::internal::TaggedValue", "v8::internal::Object", GetYearAddress(), 1, 4, std::move(year_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> month_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("month", "v8::internal::TaggedValue", "v8::internal::Object", GetMonthAddress(), 1, 4, std::move(month_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> day_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("day", "v8::internal::TaggedValue", "v8::internal::Object", GetDayAddress(), 1, 4, std::move(day_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> weekday_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("weekday", "v8::internal::TaggedValue", "v8::internal::Object", GetWeekdayAddress(), 1, 4, std::move(weekday_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> hour_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("hour", "v8::internal::TaggedValue", "v8::internal::Object", GetHourAddress(), 1, 4, std::move(hour_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> min_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("min", "v8::internal::TaggedValue", "v8::internal::Object", GetMinAddress(), 1, 4, std::move(min_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> sec_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("sec", "v8::internal::TaggedValue", "v8::internal::Object", GetSecAddress(), 1, 4, std::move(sec_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> cache_stamp_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("cache_stamp", "v8::internal::TaggedValue", "v8::internal::Object", GetCacheStampAddress(), 1, 4, std::move(cache_stamp_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSAsyncFromSyncIterator::GetName() const {
  return "v8::internal::JSAsyncFromSyncIterator";
}

void TqJSAsyncFromSyncIterator::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSAsyncFromSyncIterator(this);
}

bool TqJSAsyncFromSyncIterator::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSAsyncFromSyncIterator*>(other) != nullptr;
}

uintptr_t TqJSAsyncFromSyncIterator::GetSyncIteratorAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSAsyncFromSyncIterator::GetSyncIteratorValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSyncIteratorAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSAsyncFromSyncIterator::GetNextAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSAsyncFromSyncIterator::GetNextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSAsyncFromSyncIterator::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> sync_iterator_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("sync_iterator", "v8::internal::TaggedValue", "v8::internal::JSReceiver", GetSyncIteratorAddress(), 1, 4, std::move(sync_iterator_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> next_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("next", "v8::internal::TaggedValue", "v8::internal::Object", GetNextAddress(), 1, 4, std::move(next_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSStringIterator::GetName() const {
  return "v8::internal::JSStringIterator";
}

void TqJSStringIterator::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSStringIterator(this);
}

bool TqJSStringIterator::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSStringIterator*>(other) != nullptr;
}

uintptr_t TqJSStringIterator::GetStringAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSStringIterator::GetStringValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetStringAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSStringIterator::GetIndexAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSStringIterator::GetIndexValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIndexAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSStringIterator::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> string_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("string", "v8::internal::TaggedValue", "v8::internal::String", GetStringAddress(), 1, 4, std::move(string_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> index_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("index", "v8::internal::TaggedValue", "v8::internal::Object", GetIndexAddress(), 1, 4, std::move(index_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSProxyRevocableResult::GetName() const {
  return "v8::internal::JSProxyRevocableResult";
}

void TqJSProxyRevocableResult::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSProxyRevocableResult(this);
}

bool TqJSProxyRevocableResult::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSProxyRevocableResult*>(other) != nullptr;
}

uintptr_t TqJSProxyRevocableResult::GetProxyAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSProxyRevocableResult::GetProxyValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetProxyAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSProxyRevocableResult::GetRevokeAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSProxyRevocableResult::GetRevokeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRevokeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSProxyRevocableResult::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> proxy_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("proxy", "v8::internal::TaggedValue", "v8::internal::Object", GetProxyAddress(), 1, 4, std::move(proxy_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> revoke_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("revoke", "v8::internal::TaggedValue", "v8::internal::Object", GetRevokeAddress(), 1, 4, std::move(revoke_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSRegExpStringIterator::GetName() const {
  return "v8::internal::JSRegExpStringIterator";
}

void TqJSRegExpStringIterator::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSRegExpStringIterator(this);
}

bool TqJSRegExpStringIterator::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSRegExpStringIterator*>(other) != nullptr;
}

uintptr_t TqJSRegExpStringIterator::GetIteratingRegExpAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSRegExpStringIterator::GetIteratingRegExpValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIteratingRegExpAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSRegExpStringIterator::GetIteratedStringAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSRegExpStringIterator::GetIteratedStringValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIteratedStringAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSRegExpStringIterator::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSRegExpStringIterator::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSRegExpStringIterator::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> iterating_reg_exp_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("iterating_reg_exp", "v8::internal::TaggedValue", "v8::internal::JSReceiver", GetIteratingRegExpAddress(), 1, 4, std::move(iterating_reg_exp_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> iterated_string_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("iterated_string", "v8::internal::TaggedValue", "v8::internal::String", GetIteratedStringAddress(), 1, 4, std::move(iterated_string_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("done", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("global", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("unicode", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSRegExp::GetName() const {
  return "v8::internal::JSRegExp";
}

void TqJSRegExp::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSRegExp(this);
}

bool TqJSRegExp::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSRegExp*>(other) != nullptr;
}

uintptr_t TqJSRegExp::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSRegExp::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSRegExp::GetSourceAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSRegExp::GetSourceValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSourceAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSRegExp::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSRegExp::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSRegExp::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> source_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("source", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", GetSourceAddress(), 1, 4, std::move(source_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSRegExpResult::GetName() const {
  return "v8::internal::JSRegExpResult";
}

void TqJSRegExpResult::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSRegExpResult(this);
}

bool TqJSRegExpResult::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSRegExpResult*>(other) != nullptr;
}

uintptr_t TqJSRegExpResult::GetIndexAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSRegExpResult::GetIndexValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIndexAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSRegExpResult::GetInputAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSRegExpResult::GetInputValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInputAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSRegExpResult::GetGroupsAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqJSRegExpResult::GetGroupsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetGroupsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSRegExpResult::GetNamesAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqJSRegExpResult::GetNamesValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNamesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSRegExpResult::GetRegexpInputAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqJSRegExpResult::GetRegexpInputValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRegexpInputAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSRegExpResult::GetRegexpLastIndexAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqJSRegExpResult::GetRegexpLastIndexValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRegexpLastIndexAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSRegExpResult::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSArray::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> index_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("index", "v8::internal::TaggedValue", "v8::internal::Object", GetIndexAddress(), 1, 4, std::move(index_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> input_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("input", "v8::internal::TaggedValue", "v8::internal::Object", GetInputAddress(), 1, 4, std::move(input_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> groups_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("groups", "v8::internal::TaggedValue", "v8::internal::Object", GetGroupsAddress(), 1, 4, std::move(groups_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> names_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("names", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetNamesAddress(), 1, 4, std::move(names_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> regexp_input_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("regexp_input", "v8::internal::TaggedValue", "v8::internal::String", GetRegexpInputAddress(), 1, 4, std::move(regexp_input_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> regexp_last_index_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("regexp_last_index", "v8::internal::TaggedValue", "v8::internal::Object", GetRegexpLastIndexAddress(), 1, 4, std::move(regexp_last_index_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSRegExpResultWithIndices::GetName() const {
  return "v8::internal::JSRegExpResultWithIndices";
}

void TqJSRegExpResultWithIndices::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSRegExpResultWithIndices(this);
}

bool TqJSRegExpResultWithIndices::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSRegExpResultWithIndices*>(other) != nullptr;
}

uintptr_t TqJSRegExpResultWithIndices::GetIndicesAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<uintptr_t> TqJSRegExpResultWithIndices::GetIndicesValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIndicesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSRegExpResultWithIndices::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSRegExpResult::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> indices_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("indices", "v8::internal::TaggedValue", "v8::internal::Object", GetIndicesAddress(), 1, 4, std::move(indices_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSRegExpResultIndices::GetName() const {
  return "v8::internal::JSRegExpResultIndices";
}

void TqJSRegExpResultIndices::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSRegExpResultIndices(this);
}

bool TqJSRegExpResultIndices::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSRegExpResultIndices*>(other) != nullptr;
}

uintptr_t TqJSRegExpResultIndices::GetGroupsAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSRegExpResultIndices::GetGroupsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetGroupsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSRegExpResultIndices::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSArray::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> groups_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("groups", "v8::internal::TaggedValue", "v8::internal::Object", GetGroupsAddress(), 1, 4, std::move(groups_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSFinalizationRegistry::GetName() const {
  return "v8::internal::JSFinalizationRegistry";
}

void TqJSFinalizationRegistry::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSFinalizationRegistry(this);
}

bool TqJSFinalizationRegistry::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSFinalizationRegistry*>(other) != nullptr;
}

uintptr_t TqJSFinalizationRegistry::GetNativeContextAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSFinalizationRegistry::GetNativeContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNativeContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFinalizationRegistry::GetCleanupAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqJSFinalizationRegistry::GetCleanupValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCleanupAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFinalizationRegistry::GetActiveCellsAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqJSFinalizationRegistry::GetActiveCellsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetActiveCellsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFinalizationRegistry::GetClearedCellsAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqJSFinalizationRegistry::GetClearedCellsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetClearedCellsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFinalizationRegistry::GetKeyMapAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqJSFinalizationRegistry::GetKeyMapValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetKeyMapAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFinalizationRegistry::GetNextDirtyAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqJSFinalizationRegistry::GetNextDirtyValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNextDirtyAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqJSFinalizationRegistry::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqJSFinalizationRegistry::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSFinalizationRegistry::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> native_context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("native_context", "v8::internal::TaggedValue", "v8::internal::NativeContext", GetNativeContextAddress(), 1, 4, std::move(native_context_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> cleanup_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("cleanup", "v8::internal::TaggedValue", "v8::internal::JSReceiver", GetCleanupAddress(), 1, 4, std::move(cleanup_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> active_cells_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("active_cells", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetActiveCellsAddress(), 1, 4, std::move(active_cells_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> cleared_cells_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("cleared_cells", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetClearedCellsAddress(), 1, 4, std::move(cleared_cells_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> key_map_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("key_map", "v8::internal::TaggedValue", "v8::internal::Object", GetKeyMapAddress(), 1, 4, std::move(key_map_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> next_dirty_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("next_dirty", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetNextDirtyAddress(), 1, 4, std::move(next_dirty_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("scheduled_for_cleanup", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSWeakRef::GetName() const {
  return "v8::internal::JSWeakRef";
}

void TqJSWeakRef::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSWeakRef(this);
}

bool TqJSWeakRef::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSWeakRef*>(other) != nullptr;
}

uintptr_t TqJSWeakRef::GetTargetAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSWeakRef::GetTargetValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTargetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSWeakRef::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> target_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("target", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetTargetAddress(), 1, 4, std::move(target_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqArrayBoilerplateDescription::GetName() const {
  return "v8::internal::ArrayBoilerplateDescription";
}

void TqArrayBoilerplateDescription::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitArrayBoilerplateDescription(this);
}

bool TqArrayBoilerplateDescription::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqArrayBoilerplateDescription*>(other) != nullptr;
}

uintptr_t TqArrayBoilerplateDescription::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqArrayBoilerplateDescription::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqArrayBoilerplateDescription::GetConstantElementsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqArrayBoilerplateDescription::GetConstantElementsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetConstantElementsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqArrayBoilerplateDescription::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> constant_elements_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("constant_elements", "v8::internal::TaggedValue", "v8::internal::FixedArrayBase", GetConstantElementsAddress(), 1, 4, std::move(constant_elements_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqRegExpBoilerplateDescription::GetName() const {
  return "v8::internal::RegExpBoilerplateDescription";
}

void TqRegExpBoilerplateDescription::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitRegExpBoilerplateDescription(this);
}

bool TqRegExpBoilerplateDescription::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqRegExpBoilerplateDescription*>(other) != nullptr;
}

uintptr_t TqRegExpBoilerplateDescription::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqRegExpBoilerplateDescription::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqRegExpBoilerplateDescription::GetSourceAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqRegExpBoilerplateDescription::GetSourceValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSourceAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqRegExpBoilerplateDescription::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqRegExpBoilerplateDescription::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqRegExpBoilerplateDescription::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> source_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("source", "v8::internal::TaggedValue", "v8::internal::String", GetSourceAddress(), 1, 4, std::move(source_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("global", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("ignore_case", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("multiline", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("sticky", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("unicode", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("dot_all", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 6));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("linear", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 7));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_indices", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 8));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqMegaDomHandler::GetName() const {
  return "v8::internal::MegaDomHandler";
}

void TqMegaDomHandler::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitMegaDomHandler(this);
}

bool TqMegaDomHandler::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqMegaDomHandler*>(other) != nullptr;
}

uintptr_t TqMegaDomHandler::GetAccessorAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqMegaDomHandler::GetAccessorValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetAccessorAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqMegaDomHandler::GetContextAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqMegaDomHandler::GetContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqMegaDomHandler::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> accessor_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("accessor", "v8::internal::TaggedValue", "v8::internal::Object", GetAccessorAddress(), 1, 4, std::move(accessor_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("context", "v8::internal::TaggedValue", "v8::internal::Object", GetContextAddress(), 1, 4, std::move(context_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqMicrotask::GetName() const {
  return "v8::internal::Microtask";
}

void TqMicrotask::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitMicrotask(this);
}

bool TqMicrotask::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqMicrotask*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqMicrotask::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  return result;
}

const char* TqCallbackTask::GetName() const {
  return "v8::internal::CallbackTask";
}

void TqCallbackTask::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitCallbackTask(this);
}

bool TqCallbackTask::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqCallbackTask*>(other) != nullptr;
}

uintptr_t TqCallbackTask::GetCallbackAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqCallbackTask::GetCallbackValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCallbackAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqCallbackTask::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqCallbackTask::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqCallbackTask::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqMicrotask::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> callback_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("callback", "v8::internal::TaggedValue", "v8::internal::Foreign", GetCallbackAddress(), 1, 4, std::move(callback_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::Foreign", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqCallableTask::GetName() const {
  return "v8::internal::CallableTask";
}

void TqCallableTask::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitCallableTask(this);
}

bool TqCallableTask::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqCallableTask*>(other) != nullptr;
}

uintptr_t TqCallableTask::GetCallableAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqCallableTask::GetCallableValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCallableAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqCallableTask::GetContextAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqCallableTask::GetContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqCallableTask::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqMicrotask::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> callable_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("callable", "v8::internal::TaggedValue", "v8::internal::JSReceiver", GetCallableAddress(), 1, 4, std::move(callable_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("context", "v8::internal::TaggedValue", "v8::internal::Context", GetContextAddress(), 1, 4, std::move(context_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqModule::GetName() const {
  return "v8::internal::Module";
}

void TqModule::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitModule(this);
}

bool TqModule::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqModule*>(other) != nullptr;
}

uintptr_t TqModule::GetExportsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqModule::GetExportsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetExportsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqModule::GetHashAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqModule::GetHashValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetHashAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqModule::GetStatusAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqModule::GetStatusValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetStatusAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqModule::GetModuleNamespaceAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqModule::GetModuleNamespaceValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetModuleNamespaceAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqModule::GetExceptionAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqModule::GetExceptionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetExceptionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqModule::GetTopLevelCapabilityAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqModule::GetTopLevelCapabilityValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTopLevelCapabilityAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqModule::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> exports_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("exports", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetExportsAddress(), 1, 4, std::move(exports_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> hash_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("hash", "v8::internal::TaggedValue", "v8::internal::Object", GetHashAddress(), 1, 4, std::move(hash_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> status_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("status", "v8::internal::TaggedValue", "v8::internal::Object", GetStatusAddress(), 1, 4, std::move(status_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> module_namespace_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("module_namespace", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetModuleNamespaceAddress(), 1, 4, std::move(module_namespace_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> exception_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("exception", "v8::internal::TaggedValue", "v8::internal::Object", GetExceptionAddress(), 1, 4, std::move(exception_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> top_level_capability_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("top_level_capability", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetTopLevelCapabilityAddress(), 1, 4, std::move(top_level_capability_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqJSModuleNamespace::GetName() const {
  return "v8::internal::JSModuleNamespace";
}

void TqJSModuleNamespace::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitJSModuleNamespace(this);
}

bool TqJSModuleNamespace::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqJSModuleNamespace*>(other) != nullptr;
}

uintptr_t TqJSModuleNamespace::GetModuleAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqJSModuleNamespace::GetModuleValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetModuleAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqJSModuleNamespace::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqJSSpecialObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> module_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("module", "v8::internal::TaggedValue", "v8::internal::Module", GetModuleAddress(), 1, 4, std::move(module_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSmallOrderedHashTable::GetName() const {
  return "v8::internal::SmallOrderedHashTable";
}

void TqSmallOrderedHashTable::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSmallOrderedHashTable(this);
}

bool TqSmallOrderedHashTable::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSmallOrderedHashTable*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqSmallOrderedHashTable::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  return result;
}

const char* TqSmallOrderedHashSet::GetName() const {
  return "v8::internal::SmallOrderedHashSet";
}

void TqSmallOrderedHashSet::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSmallOrderedHashSet(this);
}

bool TqSmallOrderedHashSet::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSmallOrderedHashSet*>(other) != nullptr;
}

uintptr_t TqSmallOrderedHashSet::GetNumberOfElementsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedHashSet::GetNumberOfElementsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfElementsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedHashSet::GetNumberOfDeletedElementsAddress() const {
  return address_ - i::kHeapObjectTag + 5;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedHashSet::GetNumberOfDeletedElementsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfDeletedElementsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedHashSet::GetNumberOfBucketsAddress() const {
  return address_ - i::kHeapObjectTag + 6;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedHashSet::GetNumberOfBucketsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfBucketsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedHashSet::GetPaddingAddress() const {
  return address_ - i::kHeapObjectTag + 7;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedHashSet::GetPaddingValue(d::MemoryAccessor accessor, size_t offset) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetPaddingAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedHashSet::GetDataTableAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqSmallOrderedHashSet::GetDataTableValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataTableAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSmallOrderedHashSet::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqSmallOrderedHashTable::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> number_of_elements_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_elements", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetNumberOfElementsAddress(), 1, 1, std::move(number_of_elements_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> number_of_deleted_elements_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_deleted_elements", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetNumberOfDeletedElementsAddress(), 1, 1, std::move(number_of_deleted_elements_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> number_of_buckets_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_buckets", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetNumberOfBucketsAddress(), 1, 1, std::move(number_of_buckets_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> padding_struct_field_list;
  auto indexed_field_slice_padding = TqDebugFieldSliceSmallOrderedHashSetPadding(accessor, address_);
  if (indexed_field_slice_padding.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("padding", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_padding.value), std::get<2>(indexed_field_slice_padding.value), 1, std::move(padding_struct_field_list), GetArrayKind(indexed_field_slice_padding.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> data_table_struct_field_list;
  auto indexed_field_slice_data_table = TqDebugFieldSliceSmallOrderedHashSetDataTable(accessor, address_);
  if (indexed_field_slice_data_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("data_table", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_data_table.value), std::get<2>(indexed_field_slice_data_table.value), 4, std::move(data_table_struct_field_list), GetArrayKind(indexed_field_slice_data_table.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> hash_table_struct_field_list;
  auto indexed_field_slice_hash_table = TqDebugFieldSliceSmallOrderedHashSetHashTable(accessor, address_);
  if (indexed_field_slice_hash_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("hash_table", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_hash_table.value), std::get<2>(indexed_field_slice_hash_table.value), 1, std::move(hash_table_struct_field_list), GetArrayKind(indexed_field_slice_hash_table.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> chain_table_struct_field_list;
  auto indexed_field_slice_chain_table = TqDebugFieldSliceSmallOrderedHashSetChainTable(accessor, address_);
  if (indexed_field_slice_chain_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("chain_table", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_chain_table.value), std::get<2>(indexed_field_slice_chain_table.value), 1, std::move(chain_table_struct_field_list), GetArrayKind(indexed_field_slice_chain_table.validity)));
  }
  return result;
}

const char* TqSmallOrderedHashMap::GetName() const {
  return "v8::internal::SmallOrderedHashMap";
}

void TqSmallOrderedHashMap::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSmallOrderedHashMap(this);
}

bool TqSmallOrderedHashMap::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSmallOrderedHashMap*>(other) != nullptr;
}

uintptr_t TqSmallOrderedHashMap::GetNumberOfElementsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedHashMap::GetNumberOfElementsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfElementsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedHashMap::GetNumberOfDeletedElementsAddress() const {
  return address_ - i::kHeapObjectTag + 5;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedHashMap::GetNumberOfDeletedElementsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfDeletedElementsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedHashMap::GetNumberOfBucketsAddress() const {
  return address_ - i::kHeapObjectTag + 6;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedHashMap::GetNumberOfBucketsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfBucketsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedHashMap::GetPaddingAddress() const {
  return address_ - i::kHeapObjectTag + 7;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedHashMap::GetPaddingValue(d::MemoryAccessor accessor, size_t offset) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetPaddingAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedHashMap::GetDataTableAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

std::vector<std::unique_ptr<ObjectProperty>> TqSmallOrderedHashMap::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqSmallOrderedHashTable::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> number_of_elements_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_elements", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetNumberOfElementsAddress(), 1, 1, std::move(number_of_elements_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> number_of_deleted_elements_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_deleted_elements", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetNumberOfDeletedElementsAddress(), 1, 1, std::move(number_of_deleted_elements_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> number_of_buckets_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_buckets", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetNumberOfBucketsAddress(), 1, 1, std::move(number_of_buckets_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> padding_struct_field_list;
  auto indexed_field_slice_padding = TqDebugFieldSliceSmallOrderedHashMapPadding(accessor, address_);
  if (indexed_field_slice_padding.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("padding", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_padding.value), std::get<2>(indexed_field_slice_padding.value), 1, std::move(padding_struct_field_list), GetArrayKind(indexed_field_slice_padding.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> data_table_struct_field_list;
  data_table_struct_field_list.push_back(std::make_unique<StructProperty>("key", "v8::internal::TaggedValue", "v8::internal::Object", 0, 0, 0));
  data_table_struct_field_list.push_back(std::make_unique<StructProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", 4, 0, 0));
  auto indexed_field_slice_data_table = TqDebugFieldSliceSmallOrderedHashMapDataTable(accessor, address_);
  if (indexed_field_slice_data_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("data_table", "", "", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_data_table.value), std::get<2>(indexed_field_slice_data_table.value), 8, std::move(data_table_struct_field_list), GetArrayKind(indexed_field_slice_data_table.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> hash_table_struct_field_list;
  auto indexed_field_slice_hash_table = TqDebugFieldSliceSmallOrderedHashMapHashTable(accessor, address_);
  if (indexed_field_slice_hash_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("hash_table", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_hash_table.value), std::get<2>(indexed_field_slice_hash_table.value), 1, std::move(hash_table_struct_field_list), GetArrayKind(indexed_field_slice_hash_table.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> chain_table_struct_field_list;
  auto indexed_field_slice_chain_table = TqDebugFieldSliceSmallOrderedHashMapChainTable(accessor, address_);
  if (indexed_field_slice_chain_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("chain_table", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_chain_table.value), std::get<2>(indexed_field_slice_chain_table.value), 1, std::move(chain_table_struct_field_list), GetArrayKind(indexed_field_slice_chain_table.validity)));
  }
  return result;
}

const char* TqSmallOrderedNameDictionary::GetName() const {
  return "v8::internal::SmallOrderedNameDictionary";
}

void TqSmallOrderedNameDictionary::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSmallOrderedNameDictionary(this);
}

bool TqSmallOrderedNameDictionary::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSmallOrderedNameDictionary*>(other) != nullptr;
}

uintptr_t TqSmallOrderedNameDictionary::GetHashAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedNameDictionary::GetHashValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetHashAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedNameDictionary::GetNumberOfElementsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedNameDictionary::GetNumberOfElementsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfElementsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedNameDictionary::GetNumberOfDeletedElementsAddress() const {
  return address_ - i::kHeapObjectTag + 9;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedNameDictionary::GetNumberOfDeletedElementsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfDeletedElementsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedNameDictionary::GetNumberOfBucketsAddress() const {
  return address_ - i::kHeapObjectTag + 10;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedNameDictionary::GetNumberOfBucketsValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfBucketsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedNameDictionary::GetPadding1Address() const {
  return address_ - i::kHeapObjectTag + 11;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSmallOrderedNameDictionary::GetPadding1Value(d::MemoryAccessor accessor, size_t offset) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetPadding1Address() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSmallOrderedNameDictionary::GetDataTableAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

std::vector<std::unique_ptr<ObjectProperty>> TqSmallOrderedNameDictionary::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqSmallOrderedHashTable::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> hash_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("hash", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetHashAddress(), 1, 4, std::move(hash_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> number_of_elements_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_elements", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetNumberOfElementsAddress(), 1, 1, std::move(number_of_elements_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> number_of_deleted_elements_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_deleted_elements", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetNumberOfDeletedElementsAddress(), 1, 1, std::move(number_of_deleted_elements_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> number_of_buckets_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_buckets", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetNumberOfBucketsAddress(), 1, 1, std::move(number_of_buckets_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> padding_1_struct_field_list;
  auto indexed_field_slice_padding_1 = TqDebugFieldSliceSmallOrderedNameDictionaryPadding1(accessor, address_);
  if (indexed_field_slice_padding_1.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("padding_1", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_padding_1.value), std::get<2>(indexed_field_slice_padding_1.value), 1, std::move(padding_1_struct_field_list), GetArrayKind(indexed_field_slice_padding_1.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> data_table_struct_field_list;
  data_table_struct_field_list.push_back(std::make_unique<StructProperty>("key", "v8::internal::TaggedValue", "v8::internal::Object", 0, 0, 0));
  data_table_struct_field_list.push_back(std::make_unique<StructProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", 4, 0, 0));
  data_table_struct_field_list.push_back(std::make_unique<StructProperty>("property_details", "v8::internal::TaggedValue", "v8::internal::Object", 8, 0, 0));
  auto indexed_field_slice_data_table = TqDebugFieldSliceSmallOrderedNameDictionaryDataTable(accessor, address_);
  if (indexed_field_slice_data_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("data_table", "", "", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_data_table.value), std::get<2>(indexed_field_slice_data_table.value), 12, std::move(data_table_struct_field_list), GetArrayKind(indexed_field_slice_data_table.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> hash_table_struct_field_list;
  auto indexed_field_slice_hash_table = TqDebugFieldSliceSmallOrderedNameDictionaryHashTable(accessor, address_);
  if (indexed_field_slice_hash_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("hash_table", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_hash_table.value), std::get<2>(indexed_field_slice_hash_table.value), 1, std::move(hash_table_struct_field_list), GetArrayKind(indexed_field_slice_hash_table.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> chain_table_struct_field_list;
  auto indexed_field_slice_chain_table = TqDebugFieldSliceSmallOrderedNameDictionaryChainTable(accessor, address_);
  if (indexed_field_slice_chain_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("chain_table", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_chain_table.value), std::get<2>(indexed_field_slice_chain_table.value), 1, std::move(chain_table_struct_field_list), GetArrayKind(indexed_field_slice_chain_table.validity)));
  }
  return result;
}

const char* TqPromiseReaction::GetName() const {
  return "v8::internal::PromiseReaction";
}

void TqPromiseReaction::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPromiseReaction(this);
}

bool TqPromiseReaction::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPromiseReaction*>(other) != nullptr;
}

uintptr_t TqPromiseReaction::GetNextAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqPromiseReaction::GetNextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseReaction::GetRejectHandlerAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqPromiseReaction::GetRejectHandlerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRejectHandlerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseReaction::GetFulfillHandlerAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqPromiseReaction::GetFulfillHandlerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFulfillHandlerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseReaction::GetPromiseOrCapabilityAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqPromiseReaction::GetPromiseOrCapabilityValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPromiseOrCapabilityAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseReaction::GetContinuationPreservedEmbedderDataAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqPromiseReaction::GetContinuationPreservedEmbedderDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContinuationPreservedEmbedderDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqPromiseReaction::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> next_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("next", "v8::internal::TaggedValue", "v8::internal::Object", GetNextAddress(), 1, 4, std::move(next_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> reject_handler_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("reject_handler", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetRejectHandlerAddress(), 1, 4, std::move(reject_handler_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> fulfill_handler_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("fulfill_handler", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetFulfillHandlerAddress(), 1, 4, std::move(fulfill_handler_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> promise_or_capability_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("promise_or_capability", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPromiseOrCapabilityAddress(), 1, 4, std::move(promise_or_capability_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> continuation_preserved_embedder_data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("continuation_preserved_embedder_data", "v8::internal::TaggedValue", "v8::internal::Object", GetContinuationPreservedEmbedderDataAddress(), 1, 4, std::move(continuation_preserved_embedder_data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqPromiseReactionJobTask::GetName() const {
  return "v8::internal::PromiseReactionJobTask";
}

void TqPromiseReactionJobTask::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPromiseReactionJobTask(this);
}

bool TqPromiseReactionJobTask::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPromiseReactionJobTask*>(other) != nullptr;
}

uintptr_t TqPromiseReactionJobTask::GetArgumentAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqPromiseReactionJobTask::GetArgumentValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetArgumentAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseReactionJobTask::GetContextAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqPromiseReactionJobTask::GetContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseReactionJobTask::GetHandlerAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqPromiseReactionJobTask::GetHandlerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetHandlerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseReactionJobTask::GetPromiseOrCapabilityAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqPromiseReactionJobTask::GetPromiseOrCapabilityValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPromiseOrCapabilityAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseReactionJobTask::GetContinuationPreservedEmbedderDataAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqPromiseReactionJobTask::GetContinuationPreservedEmbedderDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContinuationPreservedEmbedderDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqPromiseReactionJobTask::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqMicrotask::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> argument_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("argument", "v8::internal::TaggedValue", "v8::internal::Object", GetArgumentAddress(), 1, 4, std::move(argument_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("context", "v8::internal::TaggedValue", "v8::internal::Context", GetContextAddress(), 1, 4, std::move(context_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> handler_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("handler", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetHandlerAddress(), 1, 4, std::move(handler_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> promise_or_capability_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("promise_or_capability", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPromiseOrCapabilityAddress(), 1, 4, std::move(promise_or_capability_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> continuation_preserved_embedder_data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("continuation_preserved_embedder_data", "v8::internal::TaggedValue", "v8::internal::Object", GetContinuationPreservedEmbedderDataAddress(), 1, 4, std::move(continuation_preserved_embedder_data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqPromiseFulfillReactionJobTask::GetName() const {
  return "v8::internal::PromiseFulfillReactionJobTask";
}

void TqPromiseFulfillReactionJobTask::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPromiseFulfillReactionJobTask(this);
}

bool TqPromiseFulfillReactionJobTask::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPromiseFulfillReactionJobTask*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqPromiseFulfillReactionJobTask::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqPromiseReactionJobTask::GetProperties(accessor);
  return result;
}

const char* TqPromiseRejectReactionJobTask::GetName() const {
  return "v8::internal::PromiseRejectReactionJobTask";
}

void TqPromiseRejectReactionJobTask::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPromiseRejectReactionJobTask(this);
}

bool TqPromiseRejectReactionJobTask::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPromiseRejectReactionJobTask*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqPromiseRejectReactionJobTask::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqPromiseReactionJobTask::GetProperties(accessor);
  return result;
}

const char* TqPromiseResolveThenableJobTask::GetName() const {
  return "v8::internal::PromiseResolveThenableJobTask";
}

void TqPromiseResolveThenableJobTask::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPromiseResolveThenableJobTask(this);
}

bool TqPromiseResolveThenableJobTask::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPromiseResolveThenableJobTask*>(other) != nullptr;
}

uintptr_t TqPromiseResolveThenableJobTask::GetContextAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqPromiseResolveThenableJobTask::GetContextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseResolveThenableJobTask::GetPromiseToResolveAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqPromiseResolveThenableJobTask::GetPromiseToResolveValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPromiseToResolveAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseResolveThenableJobTask::GetThenableAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqPromiseResolveThenableJobTask::GetThenableValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetThenableAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPromiseResolveThenableJobTask::GetThenAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqPromiseResolveThenableJobTask::GetThenValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetThenAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqPromiseResolveThenableJobTask::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqMicrotask::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> context_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("context", "v8::internal::TaggedValue", "v8::internal::Context", GetContextAddress(), 1, 4, std::move(context_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> promise_to_resolve_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("promise_to_resolve", "v8::internal::TaggedValue", "v8::internal::JSPromise", GetPromiseToResolveAddress(), 1, 4, std::move(promise_to_resolve_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> thenable_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("thenable", "v8::internal::TaggedValue", "v8::internal::JSReceiver", GetThenableAddress(), 1, 4, std::move(thenable_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> then_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("then", "v8::internal::TaggedValue", "v8::internal::JSReceiver", GetThenAddress(), 1, 4, std::move(then_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqPropertyArray::GetName() const {
  return "v8::internal::PropertyArray";
}

void TqPropertyArray::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPropertyArray(this);
}

bool TqPropertyArray::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPropertyArray*>(other) != nullptr;
}

uintptr_t TqPropertyArray::GetLengthAndHashAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqPropertyArray::GetLengthAndHashValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAndHashAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqPropertyArray::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> length_and_hash_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length_and_hash", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAndHashAddress(), 1, 4, std::move(length_and_hash_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqPropertyCell::GetName() const {
  return "v8::internal::PropertyCell";
}

void TqPropertyCell::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPropertyCell(this);
}

bool TqPropertyCell::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPropertyCell*>(other) != nullptr;
}

uintptr_t TqPropertyCell::GetNameAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqPropertyCell::GetNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPropertyCell::GetPropertyDetailsRawAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqPropertyCell::GetPropertyDetailsRawValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPropertyDetailsRawAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPropertyCell::GetValueAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqPropertyCell::GetValueValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPropertyCell::GetDependentCodeAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqPropertyCell::GetDependentCodeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDependentCodeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqPropertyCell::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("name", "v8::internal::TaggedValue", "v8::internal::Name", GetNameAddress(), 1, 4, std::move(name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> property_details_raw_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("property_details_raw", "v8::internal::TaggedValue", "v8::internal::Object", GetPropertyDetailsRawAddress(), 1, 4, std::move(property_details_raw_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> value_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", GetValueAddress(), 1, 4, std::move(value_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> dependent_code_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("dependent_code", "v8::internal::TaggedValue", "v8::internal::WeakFixedArray", GetDependentCodeAddress(), 1, 4, std::move(dependent_code_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqPropertyDescriptorObject::GetName() const {
  return "v8::internal::PropertyDescriptorObject";
}

void TqPropertyDescriptorObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPropertyDescriptorObject(this);
}

bool TqPropertyDescriptorObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPropertyDescriptorObject*>(other) != nullptr;
}

uintptr_t TqPropertyDescriptorObject::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqPropertyDescriptorObject::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPropertyDescriptorObject::GetValueAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqPropertyDescriptorObject::GetValueValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPropertyDescriptorObject::GetGetAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqPropertyDescriptorObject::GetGetValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetGetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPropertyDescriptorObject::GetSetAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqPropertyDescriptorObject::GetSetValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqPropertyDescriptorObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_enumerable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_enumerable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_configurable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_configurable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_writable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_writable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 6));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_value", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 7));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_get", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 8));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_set", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 9));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> value_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", GetValueAddress(), 1, 4, std::move(value_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> get_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("get", "v8::internal::TaggedValue", "v8::internal::Object", GetGetAddress(), 1, 4, std::move(get_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> set_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("set", "v8::internal::TaggedValue", "v8::internal::Object", GetSetAddress(), 1, 4, std::move(set_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqPrototypeInfo::GetName() const {
  return "v8::internal::PrototypeInfo";
}

void TqPrototypeInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPrototypeInfo(this);
}

bool TqPrototypeInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPrototypeInfo*>(other) != nullptr;
}

uintptr_t TqPrototypeInfo::GetModuleNamespaceAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqPrototypeInfo::GetModuleNamespaceValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetModuleNamespaceAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPrototypeInfo::GetPrototypeUsersAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqPrototypeInfo::GetPrototypeUsersValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPrototypeUsersAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPrototypeInfo::GetPrototypeChainEnumCacheAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqPrototypeInfo::GetPrototypeChainEnumCacheValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPrototypeChainEnumCacheAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPrototypeInfo::GetRegistrySlotAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqPrototypeInfo::GetRegistrySlotValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRegistrySlotAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPrototypeInfo::GetObjectCreateMapAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqPrototypeInfo::GetObjectCreateMapValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetObjectCreateMapAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqPrototypeInfo::GetBitFieldAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqPrototypeInfo::GetBitFieldValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBitFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqPrototypeInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> module_namespace_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("module_namespace", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetModuleNamespaceAddress(), 1, 4, std::move(module_namespace_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> prototype_users_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("prototype_users", "v8::internal::TaggedValue", "v8::internal::Object", GetPrototypeUsersAddress(), 1, 4, std::move(prototype_users_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> prototype_chain_enum_cache_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("prototype_chain_enum_cache", "v8::internal::TaggedValue", "v8::internal::Object", GetPrototypeChainEnumCacheAddress(), 1, 4, std::move(prototype_chain_enum_cache_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> registry_slot_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("registry_slot", "v8::internal::TaggedValue", "v8::internal::Object", GetRegistrySlotAddress(), 1, 4, std::move(registry_slot_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> object_create_map_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("object_create_map", "v8::internal::TaggedValue", "v8::internal::Object", GetObjectCreateMapAddress(), 1, 4, std::move(object_create_map_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> bit_field_struct_field_list;
  bit_field_struct_field_list.push_back(std::make_unique<StructProperty>("should_be_fast", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  result.push_back(std::make_unique<ObjectProperty>("bit_field", "v8::internal::TaggedValue", "v8::internal::Object", GetBitFieldAddress(), 1, 4, std::move(bit_field_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqScript::GetName() const {
  return "v8::internal::Script";
}

void TqScript::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitScript(this);
}

bool TqScript::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqScript*>(other) != nullptr;
}

uintptr_t TqScript::GetSourceAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqScript::GetSourceValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSourceAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetNameAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqScript::GetNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetLineOffsetAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqScript::GetLineOffsetValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLineOffsetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetColumnOffsetAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqScript::GetColumnOffsetValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetColumnOffsetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetContextDataAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqScript::GetContextDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetContextDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetScriptTypeAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqScript::GetScriptTypeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetScriptTypeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetLineEndsAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqScript::GetLineEndsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLineEndsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetIdAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqScript::GetIdValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIdAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetEvalFromSharedOrWrappedArgumentsOrSfiTableAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqScript::GetEvalFromSharedOrWrappedArgumentsOrSfiTableValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEvalFromSharedOrWrappedArgumentsOrSfiTableAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetEvalFromPositionAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<uintptr_t> TqScript::GetEvalFromPositionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEvalFromPositionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetSharedFunctionInfosAddress() const {
  return address_ - i::kHeapObjectTag + 44;
}

Value<uintptr_t> TqScript::GetSharedFunctionInfosValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSharedFunctionInfosAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 48;
}

Value<uintptr_t> TqScript::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetSourceUrlAddress() const {
  return address_ - i::kHeapObjectTag + 52;
}

Value<uintptr_t> TqScript::GetSourceUrlValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSourceUrlAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetSourceMappingUrlAddress() const {
  return address_ - i::kHeapObjectTag + 56;
}

Value<uintptr_t> TqScript::GetSourceMappingUrlValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSourceMappingUrlAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqScript::GetHostDefinedOptionsAddress() const {
  return address_ - i::kHeapObjectTag + 60;
}

Value<uintptr_t> TqScript::GetHostDefinedOptionsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetHostDefinedOptionsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqScript::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> source_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("source", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", GetSourceAddress(), 1, 4, std::move(source_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("name", "v8::internal::TaggedValue", "v8::internal::Object", GetNameAddress(), 1, 4, std::move(name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> line_offset_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("line_offset", "v8::internal::TaggedValue", "v8::internal::Object", GetLineOffsetAddress(), 1, 4, std::move(line_offset_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> column_offset_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("column_offset", "v8::internal::TaggedValue", "v8::internal::Object", GetColumnOffsetAddress(), 1, 4, std::move(column_offset_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> context_data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("context_data", "v8::internal::TaggedValue", "v8::internal::Object", GetContextDataAddress(), 1, 4, std::move(context_data_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> script_type_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("script_type", "v8::internal::TaggedValue", "v8::internal::Object", GetScriptTypeAddress(), 1, 4, std::move(script_type_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> line_ends_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("line_ends", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetLineEndsAddress(), 1, 4, std::move(line_ends_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> id_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("id", "v8::internal::TaggedValue", "v8::internal::Object", GetIdAddress(), 1, 4, std::move(id_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> eval_from_shared_or_wrapped_arguments_or_sfi_table_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("eval_from_shared_or_wrapped_arguments_or_sfi_table", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetEvalFromSharedOrWrappedArgumentsOrSfiTableAddress(), 1, 4, std::move(eval_from_shared_or_wrapped_arguments_or_sfi_table_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> eval_from_position_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("eval_from_position", "v8::internal::TaggedValue", "v8::internal::Object", GetEvalFromPositionAddress(), 1, 4, std::move(eval_from_position_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> shared_function_infos_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("shared_function_infos", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetSharedFunctionInfosAddress(), 1, 4, std::move(shared_function_infos_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("compilation_type", CheckTypeName<Script::CompilationType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("Script::CompilationType"), CheckTypeName<Script::CompilationType /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("Script::CompilationType"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("compilation_state", CheckTypeName<Script::CompilationState /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("Script::CompilationState"), CheckTypeName<Script::CompilationState /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("Script::CompilationState"), 0, 1, 2));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_repl_mode", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("origin_options", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 4, 4));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("break_on_entry", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 8));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> source_url_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("source_url", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", GetSourceUrlAddress(), 1, 4, std::move(source_url_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> source_mapping_url_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("source_mapping_url", "v8::internal::TaggedValue", "v8::internal::Object", GetSourceMappingUrlAddress(), 1, 4, std::move(source_mapping_url_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> host_defined_options_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("host_defined_options", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetHostDefinedOptionsAddress(), 1, 4, std::move(host_defined_options_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqPreparseData::GetName() const {
  return "v8::internal::PreparseData";
}

void TqPreparseData::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitPreparseData(this);
}

bool TqPreparseData::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqPreparseData*>(other) != nullptr;
}

uintptr_t TqPreparseData::GetDataLengthAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqPreparseData::GetDataLengthValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetDataLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqPreparseData::GetChildrenLengthAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqPreparseData::GetChildrenLengthValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetChildrenLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqPreparseData::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> data_length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data_length", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetDataLengthAddress(), 1, 4, std::move(data_length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> children_length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("children_length", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetChildrenLengthAddress(), 1, 4, std::move(children_length_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqInterpreterData::GetName() const {
  return "v8::internal::InterpreterData";
}

void TqInterpreterData::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitInterpreterData(this);
}

bool TqInterpreterData::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqInterpreterData*>(other) != nullptr;
}

uintptr_t TqInterpreterData::GetBytecodeArrayAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqInterpreterData::GetBytecodeArrayValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBytecodeArrayAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInterpreterData::GetInterpreterTrampolineAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqInterpreterData::GetInterpreterTrampolineValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInterpreterTrampolineAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqInterpreterData::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> bytecode_array_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("bytecode_array", "v8::internal::TaggedValue", "v8::internal::BytecodeArray", GetBytecodeArrayAddress(), 1, 4, std::move(bytecode_array_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> interpreter_trampoline_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("interpreter_trampoline", "v8::internal::TaggedValue", "v8::internal::Code", GetInterpreterTrampolineAddress(), 1, 4, std::move(interpreter_trampoline_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqBaselineData::GetName() const {
  return "v8::internal::BaselineData";
}

void TqBaselineData::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitBaselineData(this);
}

bool TqBaselineData::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqBaselineData*>(other) != nullptr;
}

uintptr_t TqBaselineData::GetBaselineCodeAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqBaselineData::GetBaselineCodeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBaselineCodeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqBaselineData::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqBaselineData::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqBaselineData::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> baseline_code_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("baseline_code", "v8::internal::TaggedValue", "v8::internal::Code", GetBaselineCodeAddress(), 1, 4, std::move(baseline_code_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSharedFunctionInfo::GetName() const {
  return "v8::internal::SharedFunctionInfo";
}

void TqSharedFunctionInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSharedFunctionInfo(this);
}

bool TqSharedFunctionInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSharedFunctionInfo*>(other) != nullptr;
}

uintptr_t TqSharedFunctionInfo::GetFunctionDataAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqSharedFunctionInfo::GetFunctionDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFunctionDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSharedFunctionInfo::GetNameOrScopeInfoAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqSharedFunctionInfo::GetNameOrScopeInfoValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNameOrScopeInfoAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSharedFunctionInfo::GetOuterScopeInfoOrFeedbackMetadataAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqSharedFunctionInfo::GetOuterScopeInfoOrFeedbackMetadataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetOuterScopeInfoOrFeedbackMetadataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSharedFunctionInfo::GetScriptOrDebugInfoAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqSharedFunctionInfo::GetScriptOrDebugInfoValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetScriptOrDebugInfoAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSharedFunctionInfo::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<int16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSharedFunctionInfo::GetLengthValue(d::MemoryAccessor accessor) const {
  int16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSharedFunctionInfo::GetFormalParameterCountAddress() const {
  return address_ - i::kHeapObjectTag + 22;
}

Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSharedFunctionInfo::GetFormalParameterCountValue(d::MemoryAccessor accessor) const {
  uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetFormalParameterCountAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSharedFunctionInfo::GetFunctionTokenOffsetAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSharedFunctionInfo::GetFunctionTokenOffsetValue(d::MemoryAccessor accessor) const {
  uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetFunctionTokenOffsetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSharedFunctionInfo::GetExpectedNofPropertiesAddress() const {
  return address_ - i::kHeapObjectTag + 26;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSharedFunctionInfo::GetExpectedNofPropertiesValue(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetExpectedNofPropertiesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSharedFunctionInfo::GetFlags2Address() const {
  return address_ - i::kHeapObjectTag + 27;
}

Value<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSharedFunctionInfo::GetFlags2Value(d::MemoryAccessor accessor) const {
  uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetFlags2Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSharedFunctionInfo::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSharedFunctionInfo::GetFlagsValue(d::MemoryAccessor accessor) const {
  uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSharedFunctionInfo::GetFunctionLiteralIdAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSharedFunctionInfo::GetFunctionLiteralIdValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetFunctionLiteralIdAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSharedFunctionInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> function_data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("function_data", "v8::internal::TaggedValue", "v8::internal::Object", GetFunctionDataAddress(), 1, 4, std::move(function_data_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> name_or_scope_info_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("name_or_scope_info", "v8::internal::TaggedValue", "v8::internal::Object", GetNameOrScopeInfoAddress(), 1, 4, std::move(name_or_scope_info_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> outer_scope_info_or_feedback_metadata_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("outer_scope_info_or_feedback_metadata", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetOuterScopeInfoOrFeedbackMetadataAddress(), 1, 4, std::move(outer_scope_info_or_feedback_metadata_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> script_or_debug_info_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("script_or_debug_info", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetScriptOrDebugInfoAddress(), 1, 4, std::move(script_or_debug_info_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", CheckTypeName<int16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int16_t"), CheckTypeName<int16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int16_t"), GetLengthAddress(), 1, 2, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> formal_parameter_count_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("formal_parameter_count", CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), GetFormalParameterCountAddress(), 1, 2, std::move(formal_parameter_count_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> function_token_offset_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("function_token_offset", CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), CheckTypeName<uint16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint16_t"), GetFunctionTokenOffsetAddress(), 1, 2, std::move(function_token_offset_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> expected_nof_properties_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("expected_nof_properties", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetExpectedNofPropertiesAddress(), 1, 1, std::move(expected_nof_properties_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags2_struct_field_list;
  flags2_struct_field_list.push_back(std::make_unique<StructProperty>("class_scope_has_private_brand", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 0));
  flags2_struct_field_list.push_back(std::make_unique<StructProperty>("has_static_private_methods_or_accessors", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  result.push_back(std::make_unique<ObjectProperty>("flags2", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), GetFlags2Address(), 1, 1, std::move(flags2_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("function_kind", CheckTypeName<FunctionKind /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("FunctionKind"), CheckTypeName<FunctionKind /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("FunctionKind"), 0, 5, 0));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_native", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_strict", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 6));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("function_syntax_kind", CheckTypeName<FunctionSyntaxKind /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("FunctionSyntaxKind"), CheckTypeName<FunctionSyntaxKind /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("FunctionSyntaxKind"), 0, 3, 7));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_class_constructor", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 10));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_duplicate_parameters", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 11));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("allow_lazy_compilation", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 12));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_asm_wasm_broken", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 13));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("function_map_index", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), 0, 5, 14));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("disabled_optimization_reason", CheckTypeName<BailoutReason /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("BailoutReason"), CheckTypeName<BailoutReason /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("BailoutReason"), 0, 4, 19));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("requires_instance_members_initializer", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 23));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("construct_as_builtin", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 24));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("name_should_print_as_anonymous", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 25));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("has_reported_binary_coverage", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 26));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_top_level", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 27));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("properties_are_final", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 28));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("private_name_lookup_skips_outer_class", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 29));
  result.push_back(std::make_unique<ObjectProperty>("flags", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> function_literal_id_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("function_literal_id", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetFunctionLiteralIdAddress(), 1, 4, std::move(function_literal_id_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqUncompiledData::GetName() const {
  return "v8::internal::UncompiledData";
}

void TqUncompiledData::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitUncompiledData(this);
}

bool TqUncompiledData::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqUncompiledData*>(other) != nullptr;
}

uintptr_t TqUncompiledData::GetInferredNameAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqUncompiledData::GetInferredNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInferredNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqUncompiledData::GetStartPositionAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqUncompiledData::GetStartPositionValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetStartPositionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqUncompiledData::GetEndPositionAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqUncompiledData::GetEndPositionValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetEndPositionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqUncompiledData::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> inferred_name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("inferred_name", "v8::internal::TaggedValue", "v8::internal::String", GetInferredNameAddress(), 1, 4, std::move(inferred_name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> start_position_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("start_position", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetStartPositionAddress(), 1, 4, std::move(start_position_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> end_position_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("end_position", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetEndPositionAddress(), 1, 4, std::move(end_position_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqUncompiledDataWithoutPreparseData::GetName() const {
  return "v8::internal::UncompiledDataWithoutPreparseData";
}

void TqUncompiledDataWithoutPreparseData::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitUncompiledDataWithoutPreparseData(this);
}

bool TqUncompiledDataWithoutPreparseData::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqUncompiledDataWithoutPreparseData*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqUncompiledDataWithoutPreparseData::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqUncompiledData::GetProperties(accessor);
  return result;
}

const char* TqUncompiledDataWithPreparseData::GetName() const {
  return "v8::internal::UncompiledDataWithPreparseData";
}

void TqUncompiledDataWithPreparseData::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitUncompiledDataWithPreparseData(this);
}

bool TqUncompiledDataWithPreparseData::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqUncompiledDataWithPreparseData*>(other) != nullptr;
}

uintptr_t TqUncompiledDataWithPreparseData::GetPreparseDataAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqUncompiledDataWithPreparseData::GetPreparseDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPreparseDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqUncompiledDataWithPreparseData::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqUncompiledData::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> preparse_data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("preparse_data", "v8::internal::TaggedValue", "v8::internal::PreparseData", GetPreparseDataAddress(), 1, 4, std::move(preparse_data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqOnHeapBasicBlockProfilerData::GetName() const {
  return "v8::internal::OnHeapBasicBlockProfilerData";
}

void TqOnHeapBasicBlockProfilerData::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitOnHeapBasicBlockProfilerData(this);
}

bool TqOnHeapBasicBlockProfilerData::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqOnHeapBasicBlockProfilerData*>(other) != nullptr;
}

uintptr_t TqOnHeapBasicBlockProfilerData::GetBlockIdsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqOnHeapBasicBlockProfilerData::GetBlockIdsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBlockIdsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqOnHeapBasicBlockProfilerData::GetCountsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqOnHeapBasicBlockProfilerData::GetCountsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCountsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqOnHeapBasicBlockProfilerData::GetNameAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqOnHeapBasicBlockProfilerData::GetNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqOnHeapBasicBlockProfilerData::GetScheduleAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqOnHeapBasicBlockProfilerData::GetScheduleValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetScheduleAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqOnHeapBasicBlockProfilerData::GetCodeAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqOnHeapBasicBlockProfilerData::GetCodeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCodeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqOnHeapBasicBlockProfilerData::GetHashAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqOnHeapBasicBlockProfilerData::GetHashValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetHashAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqOnHeapBasicBlockProfilerData::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> block_ids_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("block_ids", "v8::internal::TaggedValue", "v8::internal::ByteArray", GetBlockIdsAddress(), 1, 4, std::move(block_ids_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> counts_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("counts", "v8::internal::TaggedValue", "v8::internal::ByteArray", GetCountsAddress(), 1, 4, std::move(counts_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("name", "v8::internal::TaggedValue", "v8::internal::String", GetNameAddress(), 1, 4, std::move(name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> schedule_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("schedule", "v8::internal::TaggedValue", "v8::internal::String", GetScheduleAddress(), 1, 4, std::move(schedule_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> code_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("code", "v8::internal::TaggedValue", "v8::internal::String", GetCodeAddress(), 1, 4, std::move(code_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> hash_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("hash", "v8::internal::TaggedValue", "v8::internal::Object", GetHashAddress(), 1, 4, std::move(hash_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSourceTextModule::GetName() const {
  return "v8::internal::SourceTextModule";
}

void TqSourceTextModule::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSourceTextModule(this);
}

bool TqSourceTextModule::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSourceTextModule*>(other) != nullptr;
}

uintptr_t TqSourceTextModule::GetCodeAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqSourceTextModule::GetCodeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCodeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetRegularExportsAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqSourceTextModule::GetRegularExportsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRegularExportsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetRegularImportsAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqSourceTextModule::GetRegularImportsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRegularImportsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetRequestedModulesAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<uintptr_t> TqSourceTextModule::GetRequestedModulesValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRequestedModulesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetImportMetaAddress() const {
  return address_ - i::kHeapObjectTag + 44;
}

Value<uintptr_t> TqSourceTextModule::GetImportMetaValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetImportMetaAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetCycleRootAddress() const {
  return address_ - i::kHeapObjectTag + 48;
}

Value<uintptr_t> TqSourceTextModule::GetCycleRootValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCycleRootAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetAsyncParentModulesAddress() const {
  return address_ - i::kHeapObjectTag + 52;
}

Value<uintptr_t> TqSourceTextModule::GetAsyncParentModulesValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetAsyncParentModulesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetDfsIndexAddress() const {
  return address_ - i::kHeapObjectTag + 56;
}

Value<uintptr_t> TqSourceTextModule::GetDfsIndexValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDfsIndexAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetDfsAncestorIndexAddress() const {
  return address_ - i::kHeapObjectTag + 60;
}

Value<uintptr_t> TqSourceTextModule::GetDfsAncestorIndexValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDfsAncestorIndexAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetPendingAsyncDependenciesAddress() const {
  return address_ - i::kHeapObjectTag + 64;
}

Value<uintptr_t> TqSourceTextModule::GetPendingAsyncDependenciesValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPendingAsyncDependenciesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModule::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 68;
}

Value<uintptr_t> TqSourceTextModule::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSourceTextModule::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqModule::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> code_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("code", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetCodeAddress(), 1, 4, std::move(code_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> regular_exports_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("regular_exports", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetRegularExportsAddress(), 1, 4, std::move(regular_exports_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> regular_imports_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("regular_imports", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetRegularImportsAddress(), 1, 4, std::move(regular_imports_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> requested_modules_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("requested_modules", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetRequestedModulesAddress(), 1, 4, std::move(requested_modules_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> import_meta_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("import_meta", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetImportMetaAddress(), 1, 4, std::move(import_meta_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> cycle_root_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("cycle_root", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetCycleRootAddress(), 1, 4, std::move(cycle_root_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> async_parent_modules_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("async_parent_modules", "v8::internal::TaggedValue", "v8::internal::ArrayList", GetAsyncParentModulesAddress(), 1, 4, std::move(async_parent_modules_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> dfs_index_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("dfs_index", "v8::internal::TaggedValue", "v8::internal::Object", GetDfsIndexAddress(), 1, 4, std::move(dfs_index_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> dfs_ancestor_index_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("dfs_ancestor_index", "v8::internal::TaggedValue", "v8::internal::Object", GetDfsAncestorIndexAddress(), 1, 4, std::move(dfs_ancestor_index_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> pending_async_dependencies_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("pending_async_dependencies", "v8::internal::TaggedValue", "v8::internal::Object", GetPendingAsyncDependenciesAddress(), 1, 4, std::move(pending_async_dependencies_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("async", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("async_evaluating_ordinal", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), 0, 30, 2));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqModuleRequest::GetName() const {
  return "v8::internal::ModuleRequest";
}

void TqModuleRequest::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitModuleRequest(this);
}

bool TqModuleRequest::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqModuleRequest*>(other) != nullptr;
}

uintptr_t TqModuleRequest::GetSpecifierAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqModuleRequest::GetSpecifierValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSpecifierAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqModuleRequest::GetImportAssertionsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqModuleRequest::GetImportAssertionsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetImportAssertionsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqModuleRequest::GetPositionAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqModuleRequest::GetPositionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPositionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqModuleRequest::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> specifier_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("specifier", "v8::internal::TaggedValue", "v8::internal::String", GetSpecifierAddress(), 1, 4, std::move(specifier_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> import_assertions_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("import_assertions", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetImportAssertionsAddress(), 1, 4, std::move(import_assertions_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> position_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("position", "v8::internal::TaggedValue", "v8::internal::Object", GetPositionAddress(), 1, 4, std::move(position_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSourceTextModuleInfoEntry::GetName() const {
  return "v8::internal::SourceTextModuleInfoEntry";
}

void TqSourceTextModuleInfoEntry::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSourceTextModuleInfoEntry(this);
}

bool TqSourceTextModuleInfoEntry::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSourceTextModuleInfoEntry*>(other) != nullptr;
}

uintptr_t TqSourceTextModuleInfoEntry::GetExportNameAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqSourceTextModuleInfoEntry::GetExportNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetExportNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModuleInfoEntry::GetLocalNameAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqSourceTextModuleInfoEntry::GetLocalNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLocalNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModuleInfoEntry::GetImportNameAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqSourceTextModuleInfoEntry::GetImportNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetImportNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModuleInfoEntry::GetModuleRequestAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqSourceTextModuleInfoEntry::GetModuleRequestValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetModuleRequestAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModuleInfoEntry::GetCellIndexAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqSourceTextModuleInfoEntry::GetCellIndexValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCellIndexAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModuleInfoEntry::GetBegPosAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqSourceTextModuleInfoEntry::GetBegPosValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBegPosAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSourceTextModuleInfoEntry::GetEndPosAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqSourceTextModuleInfoEntry::GetEndPosValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEndPosAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSourceTextModuleInfoEntry::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> export_name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("export_name", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", GetExportNameAddress(), 1, 4, std::move(export_name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> local_name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("local_name", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", GetLocalNameAddress(), 1, 4, std::move(local_name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> import_name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("import_name", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", GetImportNameAddress(), 1, 4, std::move(import_name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> module_request_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("module_request", "v8::internal::TaggedValue", "v8::internal::Object", GetModuleRequestAddress(), 1, 4, std::move(module_request_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> cell_index_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("cell_index", "v8::internal::TaggedValue", "v8::internal::Object", GetCellIndexAddress(), 1, 4, std::move(cell_index_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> beg_pos_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("beg_pos", "v8::internal::TaggedValue", "v8::internal::Object", GetBegPosAddress(), 1, 4, std::move(beg_pos_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> end_pos_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("end_pos", "v8::internal::TaggedValue", "v8::internal::Object", GetEndPosAddress(), 1, 4, std::move(end_pos_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqStackFrameInfo::GetName() const {
  return "v8::internal::StackFrameInfo";
}

void TqStackFrameInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitStackFrameInfo(this);
}

bool TqStackFrameInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqStackFrameInfo*>(other) != nullptr;
}

uintptr_t TqStackFrameInfo::GetReceiverOrInstanceAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqStackFrameInfo::GetReceiverOrInstanceValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetReceiverOrInstanceAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqStackFrameInfo::GetFunctionAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqStackFrameInfo::GetFunctionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFunctionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqStackFrameInfo::GetCodeObjectAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqStackFrameInfo::GetCodeObjectValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCodeObjectAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqStackFrameInfo::GetCodeOffsetOrSourcePositionAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqStackFrameInfo::GetCodeOffsetOrSourcePositionValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCodeOffsetOrSourcePositionAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqStackFrameInfo::GetFlagsAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqStackFrameInfo::GetFlagsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqStackFrameInfo::GetParametersAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqStackFrameInfo::GetParametersValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetParametersAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqStackFrameInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> receiver_or_instance_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("receiver_or_instance", "v8::internal::TaggedValue", "v8::internal::Object", GetReceiverOrInstanceAddress(), 1, 4, std::move(receiver_or_instance_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> function_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("function", "v8::internal::TaggedValue", "v8::internal::Object", GetFunctionAddress(), 1, 4, std::move(function_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> code_object_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("code_object", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetCodeObjectAddress(), 1, 4, std::move(code_object_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> code_offset_or_source_position_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("code_offset_or_source_position", "v8::internal::TaggedValue", "v8::internal::Object", GetCodeOffsetOrSourcePositionAddress(), 1, 4, std::move(code_offset_or_source_position_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flags_struct_field_list;
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_wasm", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_asm_js_wasm", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_strict", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_constructor", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_asm_js_at_number_conversion", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_async", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 6));
  flags_struct_field_list.push_back(std::make_unique<StructProperty>("is_source_position_computed", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 7));
  result.push_back(std::make_unique<ObjectProperty>("flags", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagsAddress(), 1, 4, std::move(flags_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> parameters_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("parameters", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetParametersAddress(), 1, 4, std::move(parameters_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqConsString::GetName() const {
  return "v8::internal::ConsString";
}

void TqConsString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitConsString(this);
}

bool TqConsString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqConsString*>(other) != nullptr;
}

uintptr_t TqConsString::GetFirstAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqConsString::GetFirstValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFirstAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqConsString::GetSecondAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqConsString::GetSecondValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSecondAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqConsString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqString::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> first_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("first", "v8::internal::TaggedValue", "v8::internal::String", GetFirstAddress(), 1, 4, std::move(first_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> second_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("second", "v8::internal::TaggedValue", "v8::internal::String", GetSecondAddress(), 1, 4, std::move(second_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqExternalString::GetName() const {
  return "v8::internal::ExternalString";
}

void TqExternalString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitExternalString(this);
}

bool TqExternalString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqExternalString*>(other) != nullptr;
}

uintptr_t TqExternalString::GetResourceAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqExternalString::GetResourceValue(d::MemoryAccessor accessor) const {
  ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetResourceAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqExternalString::GetResourceDataAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqExternalString::GetResourceDataValue(d::MemoryAccessor accessor) const {
  ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetResourceDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqExternalString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqString::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> resource_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("resource", CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), GetResourceAddress(), 1, 8, std::move(resource_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> resource_data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("resource_data", CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), CheckTypeName<ExternalPointer_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("ExternalPointer_t"), GetResourceDataAddress(), 1, 8, std::move(resource_data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqExternalOneByteString::GetName() const {
  return "v8::internal::ExternalOneByteString";
}

void TqExternalOneByteString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitExternalOneByteString(this);
}

bool TqExternalOneByteString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqExternalOneByteString*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqExternalOneByteString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqExternalString::GetProperties(accessor);
  return result;
}

const char* TqExternalTwoByteString::GetName() const {
  return "v8::internal::ExternalTwoByteString";
}

void TqExternalTwoByteString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitExternalTwoByteString(this);
}

bool TqExternalTwoByteString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqExternalTwoByteString*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqExternalTwoByteString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqExternalString::GetProperties(accessor);
  return result;
}

const char* TqInternalizedString::GetName() const {
  return "v8::internal::InternalizedString";
}

void TqInternalizedString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitInternalizedString(this);
}

bool TqInternalizedString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqInternalizedString*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqInternalizedString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqString::GetProperties(accessor);
  return result;
}

const char* TqSeqString::GetName() const {
  return "v8::internal::SeqString";
}

void TqSeqString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSeqString(this);
}

bool TqSeqString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSeqString*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqSeqString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqString::GetProperties(accessor);
  return result;
}

const char* TqSeqOneByteString::GetName() const {
  return "v8::internal::SeqOneByteString";
}

void TqSeqOneByteString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSeqOneByteString(this);
}

bool TqSeqOneByteString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSeqOneByteString*>(other) != nullptr;
}

uintptr_t TqSeqOneByteString::GetCharsAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<char /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSeqOneByteString::GetCharsValue(d::MemoryAccessor accessor, size_t offset) const {
  char /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetCharsAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSeqOneByteString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqSeqString::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> chars_struct_field_list;
  auto indexed_field_slice_chars = TqDebugFieldSliceSeqOneByteStringChars(accessor, address_);
  if (indexed_field_slice_chars.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("chars", CheckTypeName<char /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("char"), CheckTypeName<char /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("char"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_chars.value), std::get<2>(indexed_field_slice_chars.value), 1, std::move(chars_struct_field_list), GetArrayKind(indexed_field_slice_chars.validity)));
  }
  return result;
}

const char* TqSeqTwoByteString::GetName() const {
  return "v8::internal::SeqTwoByteString";
}

void TqSeqTwoByteString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSeqTwoByteString(this);
}

bool TqSeqTwoByteString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSeqTwoByteString*>(other) != nullptr;
}

uintptr_t TqSeqTwoByteString::GetCharsAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<char16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSeqTwoByteString::GetCharsValue(d::MemoryAccessor accessor, size_t offset) const {
  char16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetCharsAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSeqTwoByteString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqSeqString::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> chars_struct_field_list;
  auto indexed_field_slice_chars = TqDebugFieldSliceSeqTwoByteStringChars(accessor, address_);
  if (indexed_field_slice_chars.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("chars", CheckTypeName<char16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("char16_t"), CheckTypeName<char16_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("char16_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_chars.value), std::get<2>(indexed_field_slice_chars.value), 2, std::move(chars_struct_field_list), GetArrayKind(indexed_field_slice_chars.validity)));
  }
  return result;
}

const char* TqSlicedString::GetName() const {
  return "v8::internal::SlicedString";
}

void TqSlicedString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSlicedString(this);
}

bool TqSlicedString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSlicedString*>(other) != nullptr;
}

uintptr_t TqSlicedString::GetParentAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqSlicedString::GetParentValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetParentAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSlicedString::GetOffsetAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqSlicedString::GetOffsetValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetOffsetAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSlicedString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqString::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> parent_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("parent", "v8::internal::TaggedValue", "v8::internal::String", GetParentAddress(), 1, 4, std::move(parent_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> offset_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("offset", "v8::internal::TaggedValue", "v8::internal::Object", GetOffsetAddress(), 1, 4, std::move(offset_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqThinString::GetName() const {
  return "v8::internal::ThinString";
}

void TqThinString::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitThinString(this);
}

bool TqThinString::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqThinString*>(other) != nullptr;
}

uintptr_t TqThinString::GetActualAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqThinString::GetActualValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetActualAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqThinString::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqString::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> actual_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("actual", "v8::internal::TaggedValue", "v8::internal::String", GetActualAddress(), 1, 4, std::move(actual_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqTuple2::GetName() const {
  return "v8::internal::Tuple2";
}

void TqTuple2::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitTuple2(this);
}

bool TqTuple2::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqTuple2*>(other) != nullptr;
}

uintptr_t TqTuple2::GetValue1Address() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqTuple2::GetValue1Value(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValue1Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqTuple2::GetValue2Address() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqTuple2::GetValue2Value(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValue2Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqTuple2::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> value1_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value1", "v8::internal::TaggedValue", "v8::internal::Object", GetValue1Address(), 1, 4, std::move(value1_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> value2_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value2", "v8::internal::TaggedValue", "v8::internal::Object", GetValue2Address(), 1, 4, std::move(value2_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSwissNameDictionary::GetName() const {
  return "v8::internal::SwissNameDictionary";
}

void TqSwissNameDictionary::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSwissNameDictionary(this);
}

bool TqSwissNameDictionary::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSwissNameDictionary*>(other) != nullptr;
}

uintptr_t TqSwissNameDictionary::GetHashAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSwissNameDictionary::GetHashValue(d::MemoryAccessor accessor) const {
  uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetHashAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSwissNameDictionary::GetCapacityAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqSwissNameDictionary::GetCapacityValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetCapacityAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqSwissNameDictionary::GetMetaTableAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqSwissNameDictionary::GetMetaTableValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetMetaTableAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSwissNameDictionary::GetDataTableAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqSwissNameDictionary::GetDataTableValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataTableAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSwissNameDictionary::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> hash_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("hash", CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), CheckTypeName<uint32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint32_t"), GetHashAddress(), 1, 4, std::move(hash_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> capacity_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("capacity", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetCapacityAddress(), 1, 4, std::move(capacity_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> meta_table_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("meta_table", "v8::internal::TaggedValue", "v8::internal::ByteArray", GetMetaTableAddress(), 1, 4, std::move(meta_table_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_table_struct_field_list;
  auto indexed_field_slice_data_table = TqDebugFieldSliceSwissNameDictionaryDataTable(accessor, address_);
  if (indexed_field_slice_data_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("data_table", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_data_table.value), std::get<2>(indexed_field_slice_data_table.value), 4, std::move(data_table_struct_field_list), GetArrayKind(indexed_field_slice_data_table.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> ctrl_table_struct_field_list;
  auto indexed_field_slice_ctrl_table = TqDebugFieldSliceSwissNameDictionaryCtrlTable(accessor, address_);
  if (indexed_field_slice_ctrl_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("ctrl_table", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_ctrl_table.value), std::get<2>(indexed_field_slice_ctrl_table.value), 1, std::move(ctrl_table_struct_field_list), GetArrayKind(indexed_field_slice_ctrl_table.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> property_details_table_struct_field_list;
  auto indexed_field_slice_property_details_table = TqDebugFieldSliceSwissNameDictionaryPropertyDetailsTable(accessor, address_);
  if (indexed_field_slice_property_details_table.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("property_details_table", CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), CheckTypeName<uint8_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("uint8_t"), address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_property_details_table.value), std::get<2>(indexed_field_slice_property_details_table.value), 1, std::move(property_details_table_struct_field_list), GetArrayKind(indexed_field_slice_property_details_table.validity)));
  }
  return result;
}

const char* TqSyntheticModule::GetName() const {
  return "v8::internal::SyntheticModule";
}

void TqSyntheticModule::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSyntheticModule(this);
}

bool TqSyntheticModule::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSyntheticModule*>(other) != nullptr;
}

uintptr_t TqSyntheticModule::GetNameAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqSyntheticModule::GetNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSyntheticModule::GetExportNamesAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqSyntheticModule::GetExportNamesValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetExportNamesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSyntheticModule::GetEvaluationStepsAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqSyntheticModule::GetEvaluationStepsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEvaluationStepsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSyntheticModule::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqModule::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("name", "v8::internal::TaggedValue", "v8::internal::String", GetNameAddress(), 1, 4, std::move(name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> export_names_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("export_names", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetExportNamesAddress(), 1, 4, std::move(export_names_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> evaluation_steps_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("evaluation_steps", "v8::internal::TaggedValue", "v8::internal::Foreign", GetEvaluationStepsAddress(), 1, 4, std::move(evaluation_steps_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqCachedTemplateObject::GetName() const {
  return "v8::internal::CachedTemplateObject";
}

void TqCachedTemplateObject::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitCachedTemplateObject(this);
}

bool TqCachedTemplateObject::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqCachedTemplateObject*>(other) != nullptr;
}

uintptr_t TqCachedTemplateObject::GetSlotIdAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqCachedTemplateObject::GetSlotIdValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSlotIdAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqCachedTemplateObject::GetTemplateObjectAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqCachedTemplateObject::GetTemplateObjectValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTemplateObjectAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqCachedTemplateObject::GetNextAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqCachedTemplateObject::GetNextValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNextAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqCachedTemplateObject::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> slot_id_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("slot_id", "v8::internal::TaggedValue", "v8::internal::Object", GetSlotIdAddress(), 1, 4, std::move(slot_id_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> template_object_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("template_object", "v8::internal::TaggedValue", "v8::internal::JSArray", GetTemplateObjectAddress(), 1, 4, std::move(template_object_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> next_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("next", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetNextAddress(), 1, 4, std::move(next_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqTemplateObjectDescription::GetName() const {
  return "v8::internal::TemplateObjectDescription";
}

void TqTemplateObjectDescription::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitTemplateObjectDescription(this);
}

bool TqTemplateObjectDescription::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqTemplateObjectDescription*>(other) != nullptr;
}

uintptr_t TqTemplateObjectDescription::GetRawStringsAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqTemplateObjectDescription::GetRawStringsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRawStringsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqTemplateObjectDescription::GetCookedStringsAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqTemplateObjectDescription::GetCookedStringsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCookedStringsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqTemplateObjectDescription::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> raw_strings_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("raw_strings", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetRawStringsAddress(), 1, 4, std::move(raw_strings_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> cooked_strings_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("cooked_strings", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetCookedStringsAddress(), 1, 4, std::move(cooked_strings_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqTemplateInfo::GetName() const {
  return "v8::internal::TemplateInfo";
}

void TqTemplateInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitTemplateInfo(this);
}

bool TqTemplateInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqTemplateInfo*>(other) != nullptr;
}

uintptr_t TqTemplateInfo::GetTagAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqTemplateInfo::GetTagValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTagAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqTemplateInfo::GetSerialNumberAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqTemplateInfo::GetSerialNumberValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSerialNumberAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqTemplateInfo::GetNumberOfPropertiesAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqTemplateInfo::GetNumberOfPropertiesValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfPropertiesAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqTemplateInfo::GetPropertyListAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqTemplateInfo::GetPropertyListValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPropertyListAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqTemplateInfo::GetPropertyAccessorsAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqTemplateInfo::GetPropertyAccessorsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPropertyAccessorsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqTemplateInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> tag_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("tag", "v8::internal::TaggedValue", "v8::internal::Object", GetTagAddress(), 1, 4, std::move(tag_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> serial_number_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("serial_number", "v8::internal::TaggedValue", "v8::internal::Object", GetSerialNumberAddress(), 1, 4, std::move(serial_number_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> number_of_properties_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("number_of_properties", "v8::internal::TaggedValue", "v8::internal::Object", GetNumberOfPropertiesAddress(), 1, 4, std::move(number_of_properties_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> property_list_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("property_list", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPropertyListAddress(), 1, 4, std::move(property_list_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> property_accessors_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("property_accessors", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPropertyAccessorsAddress(), 1, 4, std::move(property_accessors_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqFunctionTemplateRareData::GetName() const {
  return "v8::internal::FunctionTemplateRareData";
}

void TqFunctionTemplateRareData::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitFunctionTemplateRareData(this);
}

bool TqFunctionTemplateRareData::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqFunctionTemplateRareData*>(other) != nullptr;
}

uintptr_t TqFunctionTemplateRareData::GetPrototypeTemplateAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqFunctionTemplateRareData::GetPrototypeTemplateValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPrototypeTemplateAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateRareData::GetPrototypeProviderTemplateAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqFunctionTemplateRareData::GetPrototypeProviderTemplateValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPrototypeProviderTemplateAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateRareData::GetParentTemplateAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqFunctionTemplateRareData::GetParentTemplateValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetParentTemplateAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateRareData::GetNamedPropertyHandlerAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqFunctionTemplateRareData::GetNamedPropertyHandlerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNamedPropertyHandlerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateRareData::GetIndexedPropertyHandlerAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqFunctionTemplateRareData::GetIndexedPropertyHandlerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetIndexedPropertyHandlerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateRareData::GetInstanceTemplateAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqFunctionTemplateRareData::GetInstanceTemplateValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInstanceTemplateAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateRareData::GetInstanceCallHandlerAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqFunctionTemplateRareData::GetInstanceCallHandlerValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInstanceCallHandlerAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateRareData::GetAccessCheckInfoAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqFunctionTemplateRareData::GetAccessCheckInfoValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetAccessCheckInfoAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateRareData::GetCFunctionOverloadsAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqFunctionTemplateRareData::GetCFunctionOverloadsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCFunctionOverloadsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqFunctionTemplateRareData::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqStruct::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> prototype_template_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("prototype_template", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPrototypeTemplateAddress(), 1, 4, std::move(prototype_template_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> prototype_provider_template_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("prototype_provider_template", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetPrototypeProviderTemplateAddress(), 1, 4, std::move(prototype_provider_template_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> parent_template_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("parent_template", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetParentTemplateAddress(), 1, 4, std::move(parent_template_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> named_property_handler_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("named_property_handler", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetNamedPropertyHandlerAddress(), 1, 4, std::move(named_property_handler_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> indexed_property_handler_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("indexed_property_handler", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetIndexedPropertyHandlerAddress(), 1, 4, std::move(indexed_property_handler_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> instance_template_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("instance_template", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetInstanceTemplateAddress(), 1, 4, std::move(instance_template_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> instance_call_handler_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("instance_call_handler", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetInstanceCallHandlerAddress(), 1, 4, std::move(instance_call_handler_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> access_check_info_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("access_check_info", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetAccessCheckInfoAddress(), 1, 4, std::move(access_check_info_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> c_function_overloads_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("c_function_overloads", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetCFunctionOverloadsAddress(), 1, 4, std::move(c_function_overloads_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqFunctionTemplateInfo::GetName() const {
  return "v8::internal::FunctionTemplateInfo";
}

void TqFunctionTemplateInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitFunctionTemplateInfo(this);
}

bool TqFunctionTemplateInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqFunctionTemplateInfo*>(other) != nullptr;
}

uintptr_t TqFunctionTemplateInfo::GetCallCodeAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqFunctionTemplateInfo::GetCallCodeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCallCodeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateInfo::GetClassNameAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqFunctionTemplateInfo::GetClassNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetClassNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateInfo::GetSignatureAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqFunctionTemplateInfo::GetSignatureValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSignatureAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateInfo::GetRareDataAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqFunctionTemplateInfo::GetRareDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetRareDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateInfo::GetSharedFunctionInfoAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<uintptr_t> TqFunctionTemplateInfo::GetSharedFunctionInfoValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSharedFunctionInfoAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateInfo::GetFlagAddress() const {
  return address_ - i::kHeapObjectTag + 44;
}

Value<uintptr_t> TqFunctionTemplateInfo::GetFlagValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetFlagAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateInfo::GetLengthAddress() const {
  return address_ - i::kHeapObjectTag + 48;
}

Value<uintptr_t> TqFunctionTemplateInfo::GetLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateInfo::GetCachedPropertyNameAddress() const {
  return address_ - i::kHeapObjectTag + 52;
}

Value<uintptr_t> TqFunctionTemplateInfo::GetCachedPropertyNameValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCachedPropertyNameAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqFunctionTemplateInfo::GetInstanceTypeAddress() const {
  return address_ - i::kHeapObjectTag + 56;
}

Value<uintptr_t> TqFunctionTemplateInfo::GetInstanceTypeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInstanceTypeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqFunctionTemplateInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqTemplateInfo::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> call_code_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("call_code", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetCallCodeAddress(), 1, 4, std::move(call_code_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> class_name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("class_name", "v8::internal::TaggedValue", "v8::internal::PrimitiveHeapObject", GetClassNameAddress(), 1, 4, std::move(class_name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> signature_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("signature", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetSignatureAddress(), 1, 4, std::move(signature_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> rare_data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("rare_data", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetRareDataAddress(), 1, 4, std::move(rare_data_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> shared_function_info_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("shared_function_info", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetSharedFunctionInfoAddress(), 1, 4, std::move(shared_function_info_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> flag_struct_field_list;
  flag_struct_field_list.push_back(std::make_unique<StructProperty>("undetectable", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  flag_struct_field_list.push_back(std::make_unique<StructProperty>("needs_access_check", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  flag_struct_field_list.push_back(std::make_unique<StructProperty>("read_only_prototype", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 3));
  flag_struct_field_list.push_back(std::make_unique<StructProperty>("remove_prototype", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 4));
  flag_struct_field_list.push_back(std::make_unique<StructProperty>("accept_any_receiver", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 5));
  flag_struct_field_list.push_back(std::make_unique<StructProperty>("published", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 6));
  flag_struct_field_list.push_back(std::make_unique<StructProperty>("allowed_receiver_range_start", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 12, 7));
  flag_struct_field_list.push_back(std::make_unique<StructProperty>("allowed_receiver_range_end", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 12, 19));
  result.push_back(std::make_unique<ObjectProperty>("flag", "v8::internal::TaggedValue", "v8::internal::Object", GetFlagAddress(), 1, 4, std::move(flag_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> length_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("length", "v8::internal::TaggedValue", "v8::internal::Object", GetLengthAddress(), 1, 4, std::move(length_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> cached_property_name_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("cached_property_name", "v8::internal::TaggedValue", "v8::internal::Object", GetCachedPropertyNameAddress(), 1, 4, std::move(cached_property_name_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> instance_type_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("instance_type", "v8::internal::TaggedValue", "v8::internal::Object", GetInstanceTypeAddress(), 1, 4, std::move(instance_type_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqObjectTemplateInfo::GetName() const {
  return "v8::internal::ObjectTemplateInfo";
}

void TqObjectTemplateInfo::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitObjectTemplateInfo(this);
}

bool TqObjectTemplateInfo::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqObjectTemplateInfo*>(other) != nullptr;
}

uintptr_t TqObjectTemplateInfo::GetConstructorAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqObjectTemplateInfo::GetConstructorValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetConstructorAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqObjectTemplateInfo::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqObjectTemplateInfo::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqObjectTemplateInfo::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqTemplateInfo::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> constructor_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("constructor", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetConstructorAddress(), 1, 4, std::move(constructor_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  data_struct_field_list.push_back(std::make_unique<StructProperty>("is_immutable_prototype", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 1));
  data_struct_field_list.push_back(std::make_unique<StructProperty>("is_code_kind", CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), CheckTypeName<bool /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("bool"), 0, 1, 2));
  data_struct_field_list.push_back(std::make_unique<StructProperty>("embedder_field_count", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), 0, 28, 3));
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::Object", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqInternalClass::GetName() const {
  return "v8::internal::InternalClass";
}

void TqInternalClass::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitInternalClass(this);
}

bool TqInternalClass::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqInternalClass*>(other) != nullptr;
}

uintptr_t TqInternalClass::GetAAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqInternalClass::GetAValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetAAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInternalClass::GetBAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqInternalClass::GetBValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqInternalClass::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> a_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("a", "v8::internal::TaggedValue", "v8::internal::Object", GetAAddress(), 1, 4, std::move(a_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> b_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("b", "v8::internal::TaggedValue", "v8::internal::Object", GetBAddress(), 1, 4, std::move(b_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSmiPair::GetName() const {
  return "v8::internal::SmiPair";
}

void TqSmiPair::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSmiPair(this);
}

bool TqSmiPair::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSmiPair*>(other) != nullptr;
}

uintptr_t TqSmiPair::GetAAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqSmiPair::GetAValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetAAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSmiPair::GetBAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqSmiPair::GetBValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSmiPair::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> a_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("a", "v8::internal::TaggedValue", "v8::internal::Object", GetAAddress(), 1, 4, std::move(a_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> b_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("b", "v8::internal::TaggedValue", "v8::internal::Object", GetBAddress(), 1, 4, std::move(b_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSmiBox::GetName() const {
  return "v8::internal::SmiBox";
}

void TqSmiBox::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSmiBox(this);
}

bool TqSmiBox::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSmiBox*>(other) != nullptr;
}

uintptr_t TqSmiBox::GetValueAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqSmiBox::GetValueValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetValueAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSmiBox::GetUnrelatedAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqSmiBox::GetUnrelatedValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetUnrelatedAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSmiBox::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> value_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("value", "v8::internal::TaggedValue", "v8::internal::Object", GetValueAddress(), 1, 4, std::move(value_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> unrelated_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("unrelated", "v8::internal::TaggedValue", "v8::internal::Object", GetUnrelatedAddress(), 1, 4, std::move(unrelated_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqExportedSubClassBase::GetName() const {
  return "v8::internal::ExportedSubClassBase";
}

void TqExportedSubClassBase::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitExportedSubClassBase(this);
}

bool TqExportedSubClassBase::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqExportedSubClassBase*>(other) != nullptr;
}

uintptr_t TqExportedSubClassBase::GetAAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqExportedSubClassBase::GetAValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetAAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqExportedSubClassBase::GetBAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqExportedSubClassBase::GetBValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetBAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqExportedSubClassBase::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> a_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("a", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetAAddress(), 1, 4, std::move(a_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> b_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("b", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetBAddress(), 1, 4, std::move(b_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqExportedSubClass::GetName() const {
  return "v8::internal::ExportedSubClass";
}

void TqExportedSubClass::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitExportedSubClass(this);
}

bool TqExportedSubClass::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqExportedSubClass*>(other) != nullptr;
}

uintptr_t TqExportedSubClass::GetCFieldAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqExportedSubClass::GetCFieldValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetCFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqExportedSubClass::GetDFieldAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqExportedSubClass::GetDFieldValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetDFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqExportedSubClass::GetEFieldAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqExportedSubClass::GetEFieldValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqExportedSubClass::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqExportedSubClassBase::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> c_field_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("c_field", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetCFieldAddress(), 1, 4, std::move(c_field_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> d_field_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("d_field", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetDFieldAddress(), 1, 4, std::move(d_field_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> e_field_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("e_field", "v8::internal::TaggedValue", "v8::internal::Object", GetEFieldAddress(), 1, 4, std::move(e_field_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqAbstractInternalClass::GetName() const {
  return "v8::internal::AbstractInternalClass";
}

void TqAbstractInternalClass::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitAbstractInternalClass(this);
}

bool TqAbstractInternalClass::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqAbstractInternalClass*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqAbstractInternalClass::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  return result;
}

const char* TqAbstractInternalClassSubclass1::GetName() const {
  return "v8::internal::AbstractInternalClassSubclass1";
}

void TqAbstractInternalClassSubclass1::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitAbstractInternalClassSubclass1(this);
}

bool TqAbstractInternalClassSubclass1::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqAbstractInternalClassSubclass1*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqAbstractInternalClassSubclass1::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqAbstractInternalClass::GetProperties(accessor);
  return result;
}

const char* TqAbstractInternalClassSubclass2::GetName() const {
  return "v8::internal::AbstractInternalClassSubclass2";
}

void TqAbstractInternalClassSubclass2::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitAbstractInternalClassSubclass2(this);
}

bool TqAbstractInternalClassSubclass2::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqAbstractInternalClassSubclass2*>(other) != nullptr;
}

std::vector<std::unique_ptr<ObjectProperty>> TqAbstractInternalClassSubclass2::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqAbstractInternalClass::GetProperties(accessor);
  return result;
}

const char* TqInternalClassWithSmiElements::GetName() const {
  return "v8::internal::InternalClassWithSmiElements";
}

void TqInternalClassWithSmiElements::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitInternalClassWithSmiElements(this);
}

bool TqInternalClassWithSmiElements::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqInternalClassWithSmiElements*>(other) != nullptr;
}

uintptr_t TqInternalClassWithSmiElements::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqInternalClassWithSmiElements::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInternalClassWithSmiElements::GetObjectAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqInternalClassWithSmiElements::GetObjectValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetObjectAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInternalClassWithSmiElements::GetEntriesAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqInternalClassWithSmiElements::GetEntriesValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEntriesAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqInternalClassWithSmiElements::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqFixedArrayBase::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::Object", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> object_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("object", "v8::internal::TaggedValue", "v8::internal::Oddball", GetObjectAddress(), 1, 4, std::move(object_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> entries_struct_field_list;
  auto indexed_field_slice_entries = TqDebugFieldSliceInternalClassWithSmiElementsEntries(accessor, address_);
  if (indexed_field_slice_entries.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("entries", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_entries.value), std::get<2>(indexed_field_slice_entries.value), 4, std::move(entries_struct_field_list), GetArrayKind(indexed_field_slice_entries.validity)));
  }
  return result;
}

const char* TqInternalClassWithStructElements::GetName() const {
  return "v8::internal::InternalClassWithStructElements";
}

void TqInternalClassWithStructElements::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitInternalClassWithStructElements(this);
}

bool TqInternalClassWithStructElements::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqInternalClassWithStructElements*>(other) != nullptr;
}

uintptr_t TqInternalClassWithStructElements::GetDummy1Address() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqInternalClassWithStructElements::GetDummy1Value(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetDummy1Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqInternalClassWithStructElements::GetDummy2Address() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqInternalClassWithStructElements::GetDummy2Value(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetDummy2Address(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqInternalClassWithStructElements::GetCountAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqInternalClassWithStructElements::GetCountValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCountAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInternalClassWithStructElements::GetDataAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqInternalClassWithStructElements::GetDataValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDataAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInternalClassWithStructElements::GetObjectAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqInternalClassWithStructElements::GetObjectValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetObjectAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqInternalClassWithStructElements::GetEntriesAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqInternalClassWithStructElements::GetEntriesValue(d::MemoryAccessor accessor, size_t offset) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetEntriesAddress() + offset * sizeof(value), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqInternalClassWithStructElements::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> dummy1_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("dummy1", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetDummy1Address(), 1, 4, std::move(dummy1_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> dummy2_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("dummy2", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetDummy2Address(), 1, 4, std::move(dummy2_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> count_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("count", "v8::internal::TaggedValue", "v8::internal::Object", GetCountAddress(), 1, 4, std::move(count_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> data_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("data", "v8::internal::TaggedValue", "v8::internal::Object", GetDataAddress(), 1, 4, std::move(data_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> object_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("object", "v8::internal::TaggedValue", "v8::internal::Object", GetObjectAddress(), 1, 4, std::move(object_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> entries_struct_field_list;
  auto indexed_field_slice_entries = TqDebugFieldSliceInternalClassWithStructElementsEntries(accessor, address_);
  if (indexed_field_slice_entries.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("entries", "v8::internal::TaggedValue", "v8::internal::Object", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_entries.value), std::get<2>(indexed_field_slice_entries.value), 4, std::move(entries_struct_field_list), GetArrayKind(indexed_field_slice_entries.validity)));
  }
  std::vector<std::unique_ptr<StructProperty>> more_entries_struct_field_list;
  more_entries_struct_field_list.push_back(std::make_unique<StructProperty>("a", "v8::internal::TaggedValue", "v8::internal::Object", 0, 0, 0));
  more_entries_struct_field_list.push_back(std::make_unique<StructProperty>("b", "v8::internal::TaggedValue", "v8::internal::Object", 4, 0, 0));
  auto indexed_field_slice_more_entries = TqDebugFieldSliceInternalClassWithStructElementsMoreEntries(accessor, address_);
  if (indexed_field_slice_more_entries.validity == d::MemoryAccessResult::kOk) {
    result.push_back(std::make_unique<ObjectProperty>("more_entries", "", "", address_ - i::kHeapObjectTag + std::get<1>(indexed_field_slice_more_entries.value), std::get<2>(indexed_field_slice_more_entries.value), 8, std::move(more_entries_struct_field_list), GetArrayKind(indexed_field_slice_more_entries.validity)));
  }
  return result;
}

const char* TqExportedSubClass2::GetName() const {
  return "v8::internal::ExportedSubClass2";
}

void TqExportedSubClass2::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitExportedSubClass2(this);
}

bool TqExportedSubClass2::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqExportedSubClass2*>(other) != nullptr;
}

uintptr_t TqExportedSubClass2::GetXFieldAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqExportedSubClass2::GetXFieldValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetXFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqExportedSubClass2::GetYFieldAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/> TqExportedSubClass2::GetYFieldValue(d::MemoryAccessor accessor) const {
  int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/ value{};
  d::MemoryAccessResult validity = accessor(GetYFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, value};
}

uintptr_t TqExportedSubClass2::GetZFieldAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqExportedSubClass2::GetZFieldValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetZFieldAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqExportedSubClass2::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqExportedSubClassBase::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> x_field_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("x_field", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetXFieldAddress(), 1, 4, std::move(x_field_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> y_field_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("y_field", CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), CheckTypeName<int32_t /*Failing? Ensure constexpr type name is correct, and the necessary #include is in any .tq file*/>("int32_t"), GetYFieldAddress(), 1, 4, std::move(y_field_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> z_field_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("z_field", "v8::internal::TaggedValue", "v8::internal::Object", GetZFieldAddress(), 1, 4, std::move(z_field_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* TqSortState::GetName() const {
  return "v8::internal::SortState";
}

void TqSortState::Visit(TqObjectVisitor* visitor) const {
  visitor->VisitSortState(this);
}

bool TqSortState::IsSuperclassOf(const TqObject* other) const {
  return GetName() != other->GetName() && dynamic_cast<const TqSortState*>(other) != nullptr;
}

uintptr_t TqSortState::GetReceiverAddress() const {
  return address_ - i::kHeapObjectTag + 4;
}

Value<uintptr_t> TqSortState::GetReceiverValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetReceiverAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetInitialReceiverMapAddress() const {
  return address_ - i::kHeapObjectTag + 8;
}

Value<uintptr_t> TqSortState::GetInitialReceiverMapValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInitialReceiverMapAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetInitialReceiverLengthAddress() const {
  return address_ - i::kHeapObjectTag + 12;
}

Value<uintptr_t> TqSortState::GetInitialReceiverLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetInitialReceiverLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetUserCmpFnAddress() const {
  return address_ - i::kHeapObjectTag + 16;
}

Value<uintptr_t> TqSortState::GetUserCmpFnValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetUserCmpFnAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetSortComparePtrAddress() const {
  return address_ - i::kHeapObjectTag + 20;
}

Value<uintptr_t> TqSortState::GetSortComparePtrValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSortComparePtrAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetLoadFnAddress() const {
  return address_ - i::kHeapObjectTag + 24;
}

Value<uintptr_t> TqSortState::GetLoadFnValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetLoadFnAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetStoreFnAddress() const {
  return address_ - i::kHeapObjectTag + 28;
}

Value<uintptr_t> TqSortState::GetStoreFnValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetStoreFnAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetDeleteFnAddress() const {
  return address_ - i::kHeapObjectTag + 32;
}

Value<uintptr_t> TqSortState::GetDeleteFnValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetDeleteFnAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetCanUseSameAccessorFnAddress() const {
  return address_ - i::kHeapObjectTag + 36;
}

Value<uintptr_t> TqSortState::GetCanUseSameAccessorFnValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetCanUseSameAccessorFnAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetMinGallopAddress() const {
  return address_ - i::kHeapObjectTag + 40;
}

Value<uintptr_t> TqSortState::GetMinGallopValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetMinGallopAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetPendingRunsSizeAddress() const {
  return address_ - i::kHeapObjectTag + 44;
}

Value<uintptr_t> TqSortState::GetPendingRunsSizeValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPendingRunsSizeAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetPendingRunsAddress() const {
  return address_ - i::kHeapObjectTag + 48;
}

Value<uintptr_t> TqSortState::GetPendingRunsValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetPendingRunsAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetWorkArrayAddress() const {
  return address_ - i::kHeapObjectTag + 52;
}

Value<uintptr_t> TqSortState::GetWorkArrayValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetWorkArrayAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetTempArrayAddress() const {
  return address_ - i::kHeapObjectTag + 56;
}

Value<uintptr_t> TqSortState::GetTempArrayValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetTempArrayAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetSortLengthAddress() const {
  return address_ - i::kHeapObjectTag + 60;
}

Value<uintptr_t> TqSortState::GetSortLengthValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetSortLengthAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

uintptr_t TqSortState::GetNumberOfUndefinedAddress() const {
  return address_ - i::kHeapObjectTag + 64;
}

Value<uintptr_t> TqSortState::GetNumberOfUndefinedValue(d::MemoryAccessor accessor) const {
  i::Tagged_t value{};
  d::MemoryAccessResult validity = accessor(GetNumberOfUndefinedAddress(), reinterpret_cast<uint8_t*>(&value), sizeof(value));
  return {validity, EnsureDecompressed(value, address_)};
}

std::vector<std::unique_ptr<ObjectProperty>> TqSortState::GetProperties(d::MemoryAccessor accessor) const {
  std::vector<std::unique_ptr<ObjectProperty>> result = TqHeapObject::GetProperties(accessor);
  std::vector<std::unique_ptr<StructProperty>> receiver_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("receiver", "v8::internal::TaggedValue", "v8::internal::JSReceiver", GetReceiverAddress(), 1, 4, std::move(receiver_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> initialReceiverMap_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("initialReceiverMap", "v8::internal::TaggedValue", "v8::internal::Map", GetInitialReceiverMapAddress(), 1, 4, std::move(initialReceiverMap_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> initialReceiverLength_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("initialReceiverLength", "v8::internal::TaggedValue", "v8::internal::Object", GetInitialReceiverLengthAddress(), 1, 4, std::move(initialReceiverLength_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> userCmpFn_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("userCmpFn", "v8::internal::TaggedValue", "v8::internal::HeapObject", GetUserCmpFnAddress(), 1, 4, std::move(userCmpFn_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> sortComparePtr_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("sortComparePtr", "v8::internal::TaggedValue", "v8::internal::Object", GetSortComparePtrAddress(), 1, 4, std::move(sortComparePtr_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> loadFn_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("loadFn", "v8::internal::TaggedValue", "v8::internal::Object", GetLoadFnAddress(), 1, 4, std::move(loadFn_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> storeFn_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("storeFn", "v8::internal::TaggedValue", "v8::internal::Object", GetStoreFnAddress(), 1, 4, std::move(storeFn_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> deleteFn_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("deleteFn", "v8::internal::TaggedValue", "v8::internal::Object", GetDeleteFnAddress(), 1, 4, std::move(deleteFn_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> canUseSameAccessorFn_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("canUseSameAccessorFn", "v8::internal::TaggedValue", "v8::internal::Object", GetCanUseSameAccessorFnAddress(), 1, 4, std::move(canUseSameAccessorFn_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> minGallop_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("minGallop", "v8::internal::TaggedValue", "v8::internal::Object", GetMinGallopAddress(), 1, 4, std::move(minGallop_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> pendingRunsSize_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("pendingRunsSize", "v8::internal::TaggedValue", "v8::internal::Object", GetPendingRunsSizeAddress(), 1, 4, std::move(pendingRunsSize_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> pendingRuns_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("pendingRuns", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetPendingRunsAddress(), 1, 4, std::move(pendingRuns_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> workArray_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("workArray", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetWorkArrayAddress(), 1, 4, std::move(workArray_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> tempArray_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("tempArray", "v8::internal::TaggedValue", "v8::internal::FixedArray", GetTempArrayAddress(), 1, 4, std::move(tempArray_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> sortLength_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("sortLength", "v8::internal::TaggedValue", "v8::internal::Object", GetSortLengthAddress(), 1, 4, std::move(sortLength_struct_field_list), d::PropertyKind::kSingle));
  std::vector<std::unique_ptr<StructProperty>> numberOfUndefined_struct_field_list;
  result.push_back(std::make_unique<ObjectProperty>("numberOfUndefined", "v8::internal::TaggedValue", "v8::internal::Object", GetNumberOfUndefinedAddress(), 1, 4, std::move(numberOfUndefined_struct_field_list), d::PropertyKind::kSingle));
  return result;
}

const char* kObjectClassNames[] {
  "v8::internal::HeapObject",
  "v8::internal::Context",
  "v8::internal::JSReceiver",
  "v8::internal::PrimitiveHeapObject",
  "v8::internal::HeapNumber",
  "v8::internal::BigIntBase",
  "v8::internal::Name",
  "v8::internal::String",
  "v8::internal::Symbol",
  "v8::internal::Oddball",
  "v8::internal::FixedArrayBase",
  "v8::internal::FixedArray",
  "v8::internal::JSObject",
  "v8::internal::ByteArray",
  "v8::internal::WeakFixedArray",
  "v8::internal::Foreign",
  "v8::internal::JSProxy",
  "v8::internal::JSFunctionOrBoundFunction",
  "v8::internal::JSFunction",
  "v8::internal::JSBoundFunction",
  "v8::internal::JSCustomElementsObject",
  "v8::internal::JSSpecialObject",
  "v8::internal::Map",
  "v8::internal::WeakCell",
  "v8::internal::JSPromise",
  "v8::internal::Struct",
  "v8::internal::PromiseCapability",
  "v8::internal::JSArrayBufferView",
  "v8::internal::JSTypedArray",
  "v8::internal::DataHandler",
  "v8::internal::AllocationMemento",
  "v8::internal::CallHandlerInfo",
  "v8::internal::InterceptorInfo",
  "v8::internal::AccessCheckInfo",
  "v8::internal::AccessorInfo",
  "v8::internal::JSArgumentsObject",
  "v8::internal::JSSloppyArgumentsObject",
  "v8::internal::JSStrictArgumentsObject",
  "v8::internal::SloppyArgumentsElements",
  "v8::internal::AliasedArgumentsEntry",
  "v8::internal::MutableBigInt",
  "v8::internal::Cell",
  "v8::internal::BytecodeArray",
  "v8::internal::ScopeInfo",
  "v8::internal::FixedDoubleArray",
  "v8::internal::RegExpMatchInfo",
  "v8::internal::BreakPoint",
  "v8::internal::BreakPointInfo",
  "v8::internal::DebugInfo",
  "v8::internal::CoverageInfo",
  "v8::internal::EnumCache",
  "v8::internal::AccessorPair",
  "v8::internal::ClassPositions",
  "v8::internal::DescriptorArray",
  "v8::internal::StrongDescriptorArray",
  "v8::internal::EmbedderDataArray",
  "v8::internal::FeedbackCell",
  "v8::internal::FeedbackVector",
  "v8::internal::ArrayList",
  "v8::internal::TemplateList",
  "v8::internal::WeakArrayList",
  "v8::internal::FreeSpace",
  "v8::internal::JSArrayBuffer",
  "v8::internal::JSDataView",
  "v8::internal::JSArrayIterator",
  "v8::internal::JSArray",
  "v8::internal::JSCollectionIterator",
  "v8::internal::JSCollection",
  "v8::internal::JSSet",
  "v8::internal::JSMap",
  "v8::internal::JSWeakCollection",
  "v8::internal::JSWeakSet",
  "v8::internal::JSWeakMap",
  "v8::internal::JSGeneratorObject",
  "v8::internal::JSAsyncFunctionObject",
  "v8::internal::JSAsyncGeneratorObject",
  "v8::internal::AsyncGeneratorRequest",
  "v8::internal::JSGlobalProxy",
  "v8::internal::JSGlobalObject",
  "v8::internal::JSPrimitiveWrapper",
  "v8::internal::JSMessageObject",
  "v8::internal::JSDate",
  "v8::internal::JSAsyncFromSyncIterator",
  "v8::internal::JSStringIterator",
  "v8::internal::JSProxyRevocableResult",
  "v8::internal::JSRegExpStringIterator",
  "v8::internal::JSRegExp",
  "v8::internal::JSRegExpResult",
  "v8::internal::JSRegExpResultWithIndices",
  "v8::internal::JSRegExpResultIndices",
  "v8::internal::JSFinalizationRegistry",
  "v8::internal::JSWeakRef",
  "v8::internal::ArrayBoilerplateDescription",
  "v8::internal::RegExpBoilerplateDescription",
  "v8::internal::MegaDomHandler",
  "v8::internal::Microtask",
  "v8::internal::CallbackTask",
  "v8::internal::CallableTask",
  "v8::internal::Module",
  "v8::internal::JSModuleNamespace",
  "v8::internal::SmallOrderedHashTable",
  "v8::internal::SmallOrderedHashSet",
  "v8::internal::SmallOrderedHashMap",
  "v8::internal::SmallOrderedNameDictionary",
  "v8::internal::PromiseReaction",
  "v8::internal::PromiseReactionJobTask",
  "v8::internal::PromiseFulfillReactionJobTask",
  "v8::internal::PromiseRejectReactionJobTask",
  "v8::internal::PromiseResolveThenableJobTask",
  "v8::internal::PropertyArray",
  "v8::internal::PropertyCell",
  "v8::internal::PropertyDescriptorObject",
  "v8::internal::PrototypeInfo",
  "v8::internal::Script",
  "v8::internal::PreparseData",
  "v8::internal::InterpreterData",
  "v8::internal::BaselineData",
  "v8::internal::SharedFunctionInfo",
  "v8::internal::UncompiledData",
  "v8::internal::UncompiledDataWithoutPreparseData",
  "v8::internal::UncompiledDataWithPreparseData",
  "v8::internal::OnHeapBasicBlockProfilerData",
  "v8::internal::SourceTextModule",
  "v8::internal::ModuleRequest",
  "v8::internal::SourceTextModuleInfoEntry",
  "v8::internal::StackFrameInfo",
  "v8::internal::ConsString",
  "v8::internal::ExternalString",
  "v8::internal::ExternalOneByteString",
  "v8::internal::ExternalTwoByteString",
  "v8::internal::InternalizedString",
  "v8::internal::SeqString",
  "v8::internal::SeqOneByteString",
  "v8::internal::SeqTwoByteString",
  "v8::internal::SlicedString",
  "v8::internal::ThinString",
  "v8::internal::Tuple2",
  "v8::internal::SwissNameDictionary",
  "v8::internal::SyntheticModule",
  "v8::internal::CachedTemplateObject",
  "v8::internal::TemplateObjectDescription",
  "v8::internal::TemplateInfo",
  "v8::internal::FunctionTemplateRareData",
  "v8::internal::FunctionTemplateInfo",
  "v8::internal::ObjectTemplateInfo",
  "v8::internal::InternalClass",
  "v8::internal::SmiPair",
  "v8::internal::SmiBox",
  "v8::internal::ExportedSubClassBase",
  "v8::internal::ExportedSubClass",
  "v8::internal::AbstractInternalClass",
  "v8::internal::AbstractInternalClassSubclass1",
  "v8::internal::AbstractInternalClassSubclass2",
  "v8::internal::InternalClassWithSmiElements",
  "v8::internal::InternalClassWithStructElements",
  "v8::internal::ExportedSubClass2",
  "v8::internal::SortState",
};

const d::ClassList kObjectClassList {
  sizeof(kObjectClassNames) / sizeof(const char*),
  kObjectClassNames,
};
}  // namespace debug_helper_internal
}  // namespace internal
}  // namespace v8
