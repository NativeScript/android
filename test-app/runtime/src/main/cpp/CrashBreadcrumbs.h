#ifndef CRASHBREADCRUMBS_H_
#define CRASHBREADCRUMBS_H_

#include <string>

namespace tns {

/*
 * Records what each runtime thread was doing, so a process killed by a fatal
 * signal leaves behind more than a native backtrace.
 *
 * The state is rendered into a plain byte buffer as it changes, on ordinary
 * threads. At crash time the only work left is a write(2) of that buffer,
 * which is one of the few calls POSIX permits from a signal handler --
 * anything that allocates, takes a lock or formats has already happened.
 */
class CrashBreadcrumbs {
 public:
  /*
   * Installs SIGSEGV/SIGABRT/SIGBUS/SIGILL/SIGFPE handlers that record the
   * breadcrumb and then hand the signal back to the handler installed before
   * them, so debuggerd still writes the tombstone. Idempotent.
   */
  static void Install();

  /*
   * Points the store at the app's files directory and reports whatever a
   * previous process left behind. Idempotent, so every runtime may call it.
   */
  static void OpenStore(const std::string& filesRoot);

  /* Binds the calling thread to a runtime for that runtime's lifetime. */
  static void RegisterRuntime(int runtimeId);
  static void UnregisterRuntime(int runtimeId);

  /* Marks a registered runtime as a worker started from `script`. */
  static void SetWorkerScript(int runtimeId, const char* script);

  /*
   * Records a line to be written ahead of the runtime state should the process
   * die. Takes no lock, so it stays usable from a thread that is about to
   * abort and may already hold any of the runtime's own locks.
   */
  static void RecordFatal(const char* message);

  /*
   * Records the module the calling runtime is executing for the lifetime of
   * the scope. Module loads nest (`require` inside a module body), so the
   * enclosing module is restored on destruction, on throw paths included.
   */
  class ModuleScope {
   public:
    explicit ModuleScope(const char* modulePath);
    ~ModuleScope();
    ModuleScope(const ModuleScope&) = delete;
    ModuleScope& operator=(const ModuleScope&) = delete;

   private:
    std::string previous_;
    bool restore_ = false;
  };
};

}  // namespace tns

#endif /* CRASHBREADCRUMBS_H_ */
