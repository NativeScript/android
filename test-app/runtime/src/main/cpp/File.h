/*
 * File.h
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */

#ifndef JNI_FILE_H_
#define JNI_FILE_H_

#include <string>

namespace tns {
class File {
    public:
        static std::string ReadText(const std::string& filePath);
        /*
         * `ok` distinguishes a file that could not be opened from one that is
         * genuinely empty — the plain overload renders both as "".
         */
        static std::string ReadText(const std::string& filePath, bool& ok);
        static bool WriteBinary(const std::string& filePath, const void* inData, int length);
        static void* ReadBinary(const std::string& filePath, int& length);
    private:
        static const char* WRITE_BINARY;
        static const char* READ_BINARY;
};
}

#endif /* JNI_FILE_H_ */
