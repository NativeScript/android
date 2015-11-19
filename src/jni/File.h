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
			static bool Exists(const std::string& filePath);
			static bool WriteBinary(const std::string& filePath, const void* inData, int length);
			static void* ReadBinary(const std::string& filePath, int& length);
		private:
			static const int BUFFER_SIZE = 1024 * 1024;
			static char* Buffer;
			static const char* WRITE_BINARY;
			static const char* READ_BINARY;
	};
}


#endif /* JNI_FILE_H_ */
