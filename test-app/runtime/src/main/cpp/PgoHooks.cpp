#include "PgoHooks.h"

using namespace v8;

#ifdef NS_PGO_GENERATE
extern "C" {
int __llvm_profile_write_file(void);
void __llvm_profile_set_filename(const char* name);
}

namespace {
void PgoDump(const FunctionCallbackInfo<Value>& info) {
  auto isolate = info.GetIsolate();
  if (info.Length() < 1 || !info[0]->IsString()) {
    info.GetReturnValue().Set(-1);
    return;
  }
  String::Utf8Value path(isolate, info[0]);
  __llvm_profile_set_filename(*path);
  info.GetReturnValue().Set(__llvm_profile_write_file());
}
}  // namespace
#endif

void tns::InstallPgoHooks(Isolate* isolate, Local<ObjectTemplate> globalTemplate) {
#ifdef NS_PGO_GENERATE
  globalTemplate->Set(String::NewFromUtf8Literal(isolate, "__pgoDump"),
                      FunctionTemplate::New(isolate, PgoDump));
#endif
}
