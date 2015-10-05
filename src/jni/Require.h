/*
 * Require.h
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */

#ifndef JNI_REQUIRE_H_
#define JNI_REQUIRE_H_

#include "v8.h"
#include <string>

namespace tns
{
	class Require
	{
		public:
			static v8::Local<v8::String> LoadModule(const std::string& path);

		private:
			static const char* MODULE_PART_1;
			static const char* MODULE_PART_2;
			static const char* MODULE_PART_3;
			static const char* MODULE_PART_4;
	};
}


#endif /* JNI_REQUIRE_H_ */
