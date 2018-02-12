#include "v8.h"
#include <string>
#include <vector>

#ifndef UTIL_H_
#define UTIL_H_

namespace tns {
class Util {
    public:
        static std::string JniClassPathToCanonicalName(const std::string& jniClassPath);

        static void SplitString(const std::string& str, const std::string& delimiters, std::vector<std::string>& tokens);

        static void JoinString(const std::vector<std::string>& list, const std::string& delimiter, std::string& out);

        static bool EndsWith(const std::string& str, const std::string& suffix);

        static std::string ConvertFromJniToCanonicalName(const std::string& name);

        static std::string ConvertFromCanonicalToJniName(const std::string& name);

        static std::string ReplaceAll(std::string& str, const std::string& from, const std::string& to);

        static std::u16string ConvertFromUtf8ToUtf16(const std::string& str);

        static std::uint16_t* ConvertFromUtf8ToProtocolUtf16(const std::string& str);
};
}

#endif /* UTIL_H_ */