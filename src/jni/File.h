/*
 * File.h
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */

#ifndef JNI_FILE_H_
#define JNI_FILE_H_

#include <string>

namespace tns
{
	class File
	{
		public:
			static const char* ReadText(const std::string& filePath, int& length, bool& isNew);
			static std::string ReadText(const std::string& filePath);
		private:
			static const int BUFFER_SIZE = 1024 * 1024;
			static char* Buffer;
	};
}


#endif /* JNI_FILE_H_ */
