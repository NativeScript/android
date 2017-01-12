#include "JniSignatureParser.h"

#include <assert.h>

using namespace std;
using namespace tns;

JniSignatureParser::JniSignatureParser(const string& signature)
    : m_signature(signature) {
}

vector<string> JniSignatureParser::Parse() {
    size_t startIdx = m_signature.find_first_of('(');

    assert(startIdx != string::npos);

    size_t endIdx = m_signature.find_first_of(')');

    assert(endIdx != string::npos);

    vector<string> tokens = ParseParams(startIdx + 1, endIdx);

    return tokens;
}

vector<string> JniSignatureParser::ParseParams(int stardIdx, int endIdx) {
    vector<string> tokens;

    m_pos = stardIdx;

    while (m_pos < endIdx) {
        string token = ReadNextToken(endIdx);
        tokens.push_back(token);
    }

    return tokens;
}

string JniSignatureParser::ReadNextToken(int endIdx) {
    string token;

    char currChar = m_signature[m_pos];

    int idx;
    bool endFound;
    bool testNextChar = true;

    switch (currChar) {
    case 'Z':
    case 'B':
    case 'C':
    case 'S':
    case 'I':
    case 'J':
    case 'F':
    case 'D':
        ++m_pos;
        token.push_back(currChar);
        break;

    case 'L':
        idx = m_signature.find(';', m_pos);
        assert(idx != string::npos);
        token = m_signature.substr(m_pos, idx - m_pos + 1);
        m_pos = idx + 1;
        break;

    case '[':
        idx = m_pos;
        endFound = false;
        while (!endFound && (idx < endIdx)) {
            currChar = m_signature[idx++];
            if (testNextChar) {
                switch (currChar) {
                case 'Z':
                case 'B':
                case 'C':
                case 'S':
                case 'I':
                case 'J':
                case 'F':
                case 'D':
                    endFound = true;
                    break;
                }
                testNextChar = currChar == '[';
            } else {
                endFound = currChar == ';';
            }
        }
        assert(endFound);
        token = m_signature.substr(m_pos, idx - m_pos);
        m_pos = idx;
        break;

    default:
        assert(false);
        break;
    }

    return token;
}
