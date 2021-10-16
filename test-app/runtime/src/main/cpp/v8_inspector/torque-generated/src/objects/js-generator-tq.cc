#include "src/objects/js-generator-inl.h"

#include "torque-generated/class-verifiers.h"
#include "src/objects/instance-type-inl.h"

namespace v8 {
namespace internal {
bool IsJSGeneratorObject_NonInline(HeapObject o) {
  return o.IsJSGeneratorObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSGeneratorObject<JSGeneratorObject, JSObject>::JSGeneratorObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSGeneratorObjectVerify(JSGeneratorObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSAsyncFunctionObject_NonInline(HeapObject o) {
  return o.IsJSAsyncFunctionObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSAsyncFunctionObject<JSAsyncFunctionObject, JSGeneratorObject>::JSAsyncFunctionObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSAsyncFunctionObjectVerify(JSAsyncFunctionObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsJSAsyncGeneratorObject_NonInline(HeapObject o) {
  return o.IsJSAsyncGeneratorObject();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedJSAsyncGeneratorObject<JSAsyncGeneratorObject, JSGeneratorObject>::JSAsyncGeneratorObjectVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::JSAsyncGeneratorObjectVerify(JSAsyncGeneratorObject::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
bool IsAsyncGeneratorRequest_NonInline(HeapObject o) {
  return o.IsAsyncGeneratorRequest();}

#ifdef VERIFY_HEAP

template <>
void TorqueGeneratedAsyncGeneratorRequest<AsyncGeneratorRequest, Struct>::AsyncGeneratorRequestVerify(Isolate* isolate) {
  TorqueGeneratedClassVerifiers::AsyncGeneratorRequestVerify(AsyncGeneratorRequest::cast(*this), isolate);
}
#endif  // VERIFY_HEAP
} // namespace internal
} // namespace v8
