/*
 * nativescriptassert.h
 *
 *  Created on: 12.11.2013
 *      Author: blagoev
 */

#ifndef NATIVESCRIPTASSERT_H_
#define NATIVESCRIPTASSERT_H_

#include <android/log.h>

namespace tns {
extern bool LogEnabled;

#define DEBUG_WRITE(fmt, args...) if (tns::LogEnabled) __android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", fmt, ##args)
#define DEBUG_WRITE_FORCE(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", fmt, ##args)
#define DEBUG_WRITE_FATAL(fmt, args...) __android_log_print(ANDROID_LOG_FATAL, "TNS.Native", fmt, ##args)

/*
 * Reports a failed NS_CHECK and kills the process. The message is left in the
 * crash breadcrumb file and claimed as the abort message, so a check that
 * fires on a user's device names itself instead of arriving as an unattributed
 * SIGABRT.
 */
[[noreturn]] void OnCheckFailed(const char* expression, const char* file,
                                int line);
}

/*
 * Aborts unless the expression holds, in every build configuration.
 *
 * For invariants whose violation leaves no way to continue -- the alternative
 * at these sites is undefined behaviour a few statements later, which surfaces
 * as a tombstone pointing at whatever happened to run next.
 */
#define NS_CHECK(...)                                              \
  do {                                                             \
    if (!(__VA_ARGS__)) {                                          \
      ::tns::OnCheckFailed(#__VA_ARGS__, __FILE__, __LINE__);      \
    }                                                              \
  } while (false)

/*
 * NS_CHECK in debug builds, nothing in released ones.
 *
 * Shipped builds are compiled RelWithDebInfo, which defines NDEBUG, so the
 * expression never runs there. It must be free of side effects -- anything the
 * program depends on has to be evaluated outside the macro:
 *
 *     bool success = obj->Set(context, key, value).FromMaybe(false);
 *     NS_DCHECK(success);
 *
 * The expression is still compiled when NDEBUG is defined, so one that stops
 * making sense is a build failure rather than something only a debug build
 * notices.
 */
#ifdef NDEBUG
#define NS_DCHECK(...) \
  (true ? static_cast<void>(0) : static_cast<void>((__VA_ARGS__)))
#else
#define NS_DCHECK(...) NS_CHECK(__VA_ARGS__)
#endif

#endif /* NATIVESCRIPTASSERT_H_ */
