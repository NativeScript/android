#include "JSIRuntime.h"

#include "Runtime.h"
#include "RuntimeState.h"
#include "V8Runtime.h"

namespace {

struct JSIRuntimeState {
  std::shared_ptr<facebook::jsi::Runtime> runtime =
      std::make_shared<rnv8::V8Runtime>();
};

}  // namespace

namespace tns {

std::shared_ptr<facebook::jsi::Runtime> GetJSIRuntime() {
  Runtime* currentRuntime = Runtime::GetCurrentRuntime();
  if (currentRuntime == nullptr) {
    return nullptr;
  }
  v8::Isolate* isolate = currentRuntime->GetIsolate();
  if (isolate == nullptr) {
    return nullptr;
  }
  auto* state = RuntimeState::For<JSIRuntimeState>(isolate);
  if (state == nullptr) {
    return nullptr;
  }
  return state->runtime;
}

}  // namespace tns

extern "C" facebook::jsi::Runtime* NativeScriptJSIRuntime(void) {
  auto rt = tns::GetJSIRuntime();
  return rt ? rt.get() : nullptr;
}
