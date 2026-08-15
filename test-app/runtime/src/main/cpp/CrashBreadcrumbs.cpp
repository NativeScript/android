#include "CrashBreadcrumbs.h"

#include <android/log.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <atomic>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <mutex>

namespace {

constexpr size_t kMaxRuntimes = 16;
constexpr size_t kFieldMax = 160;
constexpr size_t kBufferMax = 8192;
constexpr size_t kHeaderMax = 128;
constexpr size_t kFatalMax = 256;

struct Slot {
  bool used;
  bool isWorker;
  int runtimeId;
  int tid;
  char script[kFieldMax];
  char module[kFieldMax];
};

Slot g_slots[kMaxRuntimes];
std::mutex g_mutex;

/*
 * Rendered in two buffers alternately, so the signal handler never reads the
 * one a running thread is part way through writing.
 */
char g_rendered[2][kBufferMax];
size_t g_renderedLength[2];
std::atomic<int> g_active{-1};

std::atomic<int> g_storeFd{-1};
std::atomic_flag g_recorded = ATOMIC_FLAG_INIT;
struct sigaction g_previous[NSIG];

/*
 * Written by whichever thread is on its way to abort(), read by the signal
 * handler. Kept out of the rendered buffers so that recording it needs no
 * lock -- the thread may be aborting from under one.
 */
char g_fatalMessage[kFatalMax];
std::atomic<size_t> g_fatalLength{0};

thread_local Slot* t_slot = nullptr;

int CurrentTid() { return static_cast<int>(syscall(__NR_gettid)); }

void CopyField(char* dst, const char* src) {
  if (src == nullptr) {
    dst[0] = '\0';
    return;
  }
  size_t length = strlen(src);
  if (length < kFieldMax) {
    memcpy(dst, src, length + 1);
    return;
  }
  // Keep the tail: the file name identifies a module, the leading directories
  // are shared by every module in the app.
  memcpy(dst, "...", 3);
  memcpy(dst + 3, src + length - (kFieldMax - 4), kFieldMax - 4);
  dst[kFieldMax - 1] = '\0';
}

void Append(char* out, size_t& length, const char* format, ...)
    __attribute__((format(printf, 3, 4)));

void Append(char* out, size_t& length, const char* format, ...) {
  if (length >= kBufferMax) {
    return;
  }
  va_list args;
  va_start(args, format);
  int written = vsnprintf(out + length, kBufferMax - length, format, args);
  va_end(args);
  if (written > 0) {
    length += static_cast<size_t>(written);
    if (length > kBufferMax - 1) {
      length = kBufferMax - 1;
    }
  }
}

void RenderLocked() {
  // Once a crash is recorded the handler may be reading either buffer; a
  // second flip after that point would rewrite the one it is copying out.
  if (g_recorded.test(std::memory_order_acquire)) {
    return;
  }
  int next = g_active.load(std::memory_order_relaxed) == 0 ? 1 : 0;
  char* out = g_rendered[next];
  size_t length = 0;

  Append(out, length, "NativeScript runtime state (pid %d):\n", getpid());
  for (const Slot& slot : g_slots) {
    if (!slot.used) {
      continue;
    }
    Append(out, length, "  runtime=%d tid=%d %s", slot.runtimeId, slot.tid,
           slot.isWorker ? "worker" : "main");
    if (slot.script[0] != '\0') {
      Append(out, length, " script=%s", slot.script);
    }
    Append(out, length, " module=%s\n",
           slot.module[0] != '\0' ? slot.module : "<none>");
  }

  g_renderedLength[next] = length;
  g_active.store(next, std::memory_order_release);
}

Slot* FindLocked(int runtimeId) {
  for (Slot& slot : g_slots) {
    if (slot.used && slot.runtimeId == runtimeId) {
      return &slot;
    }
  }
  return nullptr;
}

/* Async-signal-safe integer formatting; snprintf is not usable here. */
void AppendRaw(char* out, size_t capacity, size_t& length, const char* text) {
  while (*text != '\0' && length < capacity) {
    out[length++] = *text++;
  }
}

void AppendRawInt(char* out, size_t capacity, size_t& length, int value) {
  char digits[16];
  size_t count = 0;
  unsigned int magnitude = static_cast<unsigned int>(value);
  do {
    digits[count++] = static_cast<char>('0' + magnitude % 10);
    magnitude /= 10;
  } while (magnitude != 0 && count < sizeof(digits));
  while (count > 0 && length < capacity) {
    out[length++] = digits[--count];
  }
}

void Handler(int signalNumber, siginfo_t* info, void* context) {
  // Only the first thread to fault records; the rest are already doomed.
  if (!g_recorded.test_and_set()) {
    int fd = g_storeFd.load(std::memory_order_acquire);
    if (fd >= 0) {
      char header[kHeaderMax];
      size_t length = 0;
      AppendRaw(header, sizeof(header), length, "fatal signal ");
      AppendRawInt(header, sizeof(header), length, signalNumber);
      AppendRaw(header, sizeof(header), length, " on tid ");
      AppendRawInt(header, sizeof(header), length, CurrentTid());
      AppendRaw(header, sizeof(header), length, "\n");

      ssize_t written = pwrite(fd, header, length, 0);
      if (written > 0) {
        off_t offset = written;

        size_t fatalLength = g_fatalLength.load(std::memory_order_acquire);
        if (fatalLength > 0) {
          ssize_t fatalWritten =
              pwrite(fd, g_fatalMessage, fatalLength, offset);
          if (fatalWritten > 0) {
            offset += fatalWritten;
          }
        }

        int active = g_active.load(std::memory_order_acquire);
        if (active >= 0) {
          pwrite(fd, g_rendered[active], g_renderedLength[active], offset);
        }
      }
    }
  }

  /*
   * Hand the signal to whoever owned it before us -- on Android that is
   * debuggerd, which writes the tombstone.
   *
   * A signal the kernel raised for a real fault arrives again on its own once
   * this returns and the faulting instruction re-executes, so debuggerd is
   * entered with the kernel's original siginfo instead of anything
   * synthesised here. One that was delivered by abort() or kill() (si_code
   * <= 0) will not come back, so it has to be re-raised explicitly.
   */
  sigaction(signalNumber, &g_previous[signalNumber], nullptr);
  if (info == nullptr || info->si_code <= 0) {
    raise(signalNumber);
  }
}

}  // namespace

namespace tns {

void CrashBreadcrumbs::Install() {
  static std::once_flag once;
  std::call_once(once, [] {
    struct sigaction action = {};
    action.sa_sigaction = Handler;
    // SA_ONSTACK matters for a stack-overflow SIGSEGV, which has no room left
    // on the faulting stack to run a handler. bionic already gives every
    // thread an alternate signal stack, so the flag is all that is needed.
    action.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&action.sa_mask);
    for (int signalNumber : {SIGSEGV, SIGABRT, SIGBUS, SIGILL, SIGFPE}) {
      sigaction(signalNumber, &action, &g_previous[signalNumber]);
    }
  });
}

void CrashBreadcrumbs::OpenStore(const std::string& filesRoot) {
  static std::once_flag once;
  std::call_once(once, [&filesRoot] {
    std::string path = filesRoot + "/.ns-crash-breadcrumb";
    int fd = open(path.c_str(), O_RDWR | O_CREAT | O_CLOEXEC, 0600);
    if (fd < 0) {
      return;
    }

    char previous[kBufferMax + kHeaderMax];
    ssize_t length = read(fd, previous, sizeof(previous) - 1);
    if (length > 0) {
      previous[length] = '\0';
      // Deliberately not ANDROID_LOG_FATAL: liblog feeds a fatal record to
      // android_set_abort_message, and bionic keeps the first message it is
      // given for the life of the process. Claiming that slot here would
      // describe the *previous* process in this one's tombstone, and would
      // shut out the abort message libc or ART writes for the real fault.
      __android_log_print(
          ANDROID_LOG_ERROR, "TNS.Native",
          "The previous process was killed by a fatal signal. Runtime state "
          "recorded at that moment (match tid against the tombstone in "
          "/data/tombstones):\n%s",
          previous);
      ftruncate(fd, 0);
    }

    g_storeFd.store(fd, std::memory_order_release);
  });
}

void CrashBreadcrumbs::RegisterRuntime(int runtimeId) {
  std::lock_guard<std::mutex> lock(g_mutex);
  Slot* slot = FindLocked(runtimeId);
  if (slot == nullptr) {
    for (Slot& candidate : g_slots) {
      if (!candidate.used) {
        slot = &candidate;
        break;
      }
    }
  }
  if (slot == nullptr) {
    // Table full. Keep the runtimes already tracked rather than evicting one.
    return;
  }

  slot->used = true;
  slot->isWorker = false;
  slot->runtimeId = runtimeId;
  slot->tid = CurrentTid();
  slot->script[0] = '\0';
  slot->module[0] = '\0';
  t_slot = slot;
  RenderLocked();
}

void CrashBreadcrumbs::UnregisterRuntime(int runtimeId) {
  std::lock_guard<std::mutex> lock(g_mutex);
  Slot* slot = FindLocked(runtimeId);
  if (slot == nullptr) {
    return;
  }
  if (t_slot == slot) {
    t_slot = nullptr;
  }
  slot->used = false;
  RenderLocked();
}

void CrashBreadcrumbs::SetWorkerScript(int runtimeId, const char* script) {
  std::lock_guard<std::mutex> lock(g_mutex);
  Slot* slot = FindLocked(runtimeId);
  if (slot == nullptr) {
    return;
  }
  slot->isWorker = true;
  CopyField(slot->script, script);
  RenderLocked();
}

void CrashBreadcrumbs::RecordFatal(const char* message) {
  if (message == nullptr) {
    return;
  }
  // Room is reserved for the newline and the terminator.
  size_t length = strnlen(message, kFatalMax - 2);
  memcpy(g_fatalMessage, message, length);
  g_fatalMessage[length] = '\n';
  g_fatalMessage[length + 1] = '\0';
  g_fatalLength.store(length + 1, std::memory_order_release);
}

CrashBreadcrumbs::ModuleScope::ModuleScope(const char* modulePath) {
  Slot* slot = t_slot;
  if (slot == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lock(g_mutex);
  previous_ = slot->module;
  restore_ = true;
  CopyField(slot->module, modulePath);
  RenderLocked();
}

CrashBreadcrumbs::ModuleScope::~ModuleScope() {
  Slot* slot = t_slot;
  if (!restore_ || slot == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lock(g_mutex);
  CopyField(slot->module, previous_.c_str());
  RenderLocked();
}

}  // namespace tns
