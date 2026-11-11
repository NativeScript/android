#include "ExtendFileIdentifier.h"

#include <cstdint>
#include <vector>

namespace {

void Md5(const std::string& input, uint8_t digest[16]) {
    static const uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
    };
    static const int S[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21,
    };

    std::vector<uint8_t> message(input.begin(), input.end());
    uint64_t bitLength = static_cast<uint64_t>(input.size()) * 8;
    message.push_back(0x80);
    while (message.size() % 64 != 56) {
        message.push_back(0);
    }
    for (int i = 0; i < 8; i++) {
        message.push_back(static_cast<uint8_t>(bitLength >> (8 * i)));
    }

    uint32_t state[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    for (size_t offset = 0; offset < message.size(); offset += 64) {
        uint32_t words[16];
        for (int i = 0; i < 16; i++) {
            const uint8_t* p = &message[offset + i * 4];
            words[i] = static_cast<uint32_t>(p[0]) | (static_cast<uint32_t>(p[1]) << 8) |
                       (static_cast<uint32_t>(p[2]) << 16) | (static_cast<uint32_t>(p[3]) << 24);
        }

        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        for (int i = 0; i < 64; i++) {
            uint32_t f;
            int g;
            if (i < 16) {
                f = (b & c) | (~b & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | (~d & c);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            } else {
                f = c ^ (b | ~d);
                g = (7 * i) % 16;
            }
            f = f + a + K[i] + words[g];
            a = d;
            d = c;
            c = b;
            b = b + ((f << S[i]) | (f >> (32 - S[i])));
        }
        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            digest[i * 4 + j] = static_cast<uint8_t>(state[i] >> (8 * j));
        }
    }
}

// Java's String.split("_"): keeps leading and inner empty parts, drops trailing ones.
std::vector<std::string> SplitLikeJava(const std::string& value) {
    std::vector<std::string> parts;
    size_t start = 0;
    while (true) {
        size_t end = value.find('_', start);
        if (end == std::string::npos) {
            parts.push_back(value.substr(start));
            break;
        }
        parts.push_back(value.substr(start, end - start));
        start = end + 1;
    }
    while (!parts.empty() && parts.back().empty()) {
        parts.pop_back();
    }
    return parts;
}

bool IsJavaIdentifierPart(unsigned char c) {
    // Non-ASCII bytes are kept: the generator keeps letters outside ASCII as they are.
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
           c == '_' || c == '$' || c >= 0x80;
}

}

namespace tns {

std::string ExtendFileIdentifier(const std::string& relativePathWithoutExtension) {
    std::string file = relativePathWithoutExtension;
    for (char& ch : file) {
        if (ch == '-' || ch == '\\' || ch == '/' || ch == '.' || ch == ' ') {
            ch = '_';
        }
    }

    if (file.empty()) {
        return "unknown";
    }

    std::vector<std::string> parts = SplitLikeJava(file);
    std::string identifier;
    size_t first = parts.size() > 3 ? parts.size() - 3 : 0;
    for (size_t i = first; i < parts.size(); i++) {
        if (!identifier.empty()) {
            identifier += '_';
        }
        identifier += parts[i];
    }

    uint8_t digest[16];
    Md5(file, digest);
    static const char hex[] = "0123456789abcdef";
    identifier += '_';
    for (int i = 0; i < 3; i++) {
        identifier += hex[digest[i] >> 4];
        identifier += hex[digest[i] & 0xf];
    }

    for (char& ch : identifier) {
        if (!IsJavaIdentifierPart(static_cast<unsigned char>(ch))) {
            ch = '_';
        }
    }
    return identifier;
}

}
