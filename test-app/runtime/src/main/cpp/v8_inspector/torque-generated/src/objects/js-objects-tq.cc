#include "src/objects/js-objects-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsJSObject_NonInline(HeapObject o) {
  return o.IsJSObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSObject<JSObject, JSReceiver>::JSObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSObjectVerify(JSObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSCustomElementsObject_NonInline(HeapObject o) {
  return o.IsJSCustomElementsObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSCustomElementsObject<JSCustomElementsObject, JSObject>::JSCustomElementsObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSCustomElementsObjectVerify(JSCustomElementsObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSSpecialObject_NonInline(HeapObject o) {
  return o.IsJSSpecialObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSSpecialObject<JSSpecialObject, JSCustomElementsObject>::JSSpecialObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSSpecialObjectVerify(JSSpecialObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSGlobalProxy_NonInline(HeapObject o) {
  return o.IsJSGlobalProxy();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSGlobalProxy<JSGlobalProxy, JSSpecialObject>::JSGlobalProxyVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSGlobalProxyVerify(JSGlobalProxy::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSPrimitiveWrapper_NonInline(HeapObject o) {
  return o.IsJSPrimitiveWrapper();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSPrimitiveWrapper<JSPrimitiveWrapper, JSCustomElementsObject>::JSPrimitiveWrapperVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSPrimitiveWrapperVerify(JSPrimitiveWrapper::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSDate_NonInline(HeapObject o) {
  return o.IsJSDate();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSDate<JSDate, JSObject>::JSDateVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSDateVerify(JSDate::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSAsyncFromSyncIterator_NonInline(HeapObject o) {
  return o.IsJSAsyncFromSyncIterator();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSAsyncFromSyncIterator<JSAsyncFromSyncIterator, JSObject>::JSAsyncFromSyncIteratorVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSAsyncFromSyncIteratorVerify(JSAsyncFromSyncIterator::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSStringIterator_NonInline(HeapObject o) {
  return o.IsJSStringIterator();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSStringIterator<JSStringIterator, JSObject>::JSStringIteratorVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSStringIteratorVerify(JSStringIterator::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
