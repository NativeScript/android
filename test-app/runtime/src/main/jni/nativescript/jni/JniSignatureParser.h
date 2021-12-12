#ifndef JNISIGNATUREPARSER_H_
#define JNISIGNATUREPARSER_H_

#include <string>
#include <vector>

namespace tns {
class JniSignatureParser {
    public:
        JniSignatureParser(const std::string& signature);

        std::vector<std::string> Parse();

    private:

        std::vector<std::string> ParseParams(int stardIdx, int endIdx);

        std::string ReadNextToken(int endIdx);

        int m_pos;

        std::string m_signature;
};
}

#endif /* JNISIGNATUREPARSER_H_ */
