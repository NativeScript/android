/*
 * nativescriptassert.h
 *
 *  Created on: 12.11.2013
 *      Author: blagoev
 */

#ifndef NATIVESCRIPTASSERT_H_
#define NATIVESCRIPTASSERT_H_

#include <android/log.h>

namespace tns
{
	extern bool LogEnabled;

	#define DEBUG_WRITE(fmt, args...) if (tns::LogEnabled) __android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", fmt, ##args)
	#define DEBUG_WRITE_FORCE(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", fmt, ##args)
	#define DEBUG_WRITE_FATAL(fmt, args...) __android_log_print(ANDROID_LOG_FATAL, "TNS.Native", fmt, ##args)
	#define ASSERT(cond) ((cond)?(void)0:__android_log_assert("##cond", "TNS.Native", ""))
	#define ASSERT_MESSAGE(cond, fmt, args...) ((cond)?(void)0:__android_log_assert("##cond", "TNS.Native", fmt, ##args))
	#define ASSERT_FAIL(fmt, args...) (__android_log_assert(fmt, "TNS.Native", fmt, ##args))
	void APP_FAIL(const char *message);
}

#endif /* NATIVESCRIPTASSERT_H_ */
