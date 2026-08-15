#include "NativeScriptAssert.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "CrashBreadcrumbs.h"

namespace tns {

void OnCheckFailed(const char* expression, const char* file, int line) {
  // The build compiles with absolute paths, and the leading directories are
  // shared by every file in the runtime.
  const char* separator = strrchr(file, '/');
  if (separator != nullptr) {
    file = separator + 1;
  }

  char message[256];
  snprintf(message, sizeof(message), "NS_CHECK failed: %s, at %s:%d",
           expression, file, line);

  CrashBreadcrumbs::RecordFatal(message);
  // ANDROID_LOG_FATAL rather than an error: liblog hands a fatal record to
  // android_set_abort_message, which is what puts this line in the tombstone
  // beside the abort. Nothing has claimed that slot yet on this path.
  DEBUG_WRITE_FATAL("%s", message);

  abort();
}

}  // namespace tns
