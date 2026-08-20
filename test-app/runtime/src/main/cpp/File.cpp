/*
 * File.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */

#include "File.h"
#include "NativeScriptAssert.h"
#include <sstream>
#include <fstream>
#include <sys/mman.h>

using namespace std;

namespace tns {

string File::ReadText(const string& filePath) {
    bool ok;
    return ReadText(filePath, ok);
}

string File::ReadText(const string& filePath, bool& ok) {
    int len;
    bool isNew;
    const char* content = ReadText(filePath, len, isNew);

    ok = content != nullptr;

    if (content == nullptr) {
        return string();
    }

    string s(content, len);

    if (isNew) {
        delete[] content;
    }

    return s;
}

void* File::ReadBinary(const string& filePath, int& length) {
    length = 0;

    auto file = fopen(filePath.c_str(), READ_BINARY);
    if (!file) {
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    rewind(file);

    uint8_t* data = new uint8_t[length];
    fread(data, sizeof(uint8_t), length, file);
    fclose(file);

    return data;
}

bool File::WriteBinary(const string& filePath, const void* data, int length) {
    auto file = fopen(filePath.c_str(), WRITE_BINARY);
    if (!file) {
        return false;
    }

    auto writtenBytes = fwrite(data, sizeof(uint8_t), length, file);
    fclose(file);

    return writtenBytes == length;
}

const char* File::ReadText(const string& filePath, int& charLength, bool& isNew) {
    charLength = 0;
    isNew = false;

    FILE* file = fopen(filePath.c_str(), "rb");
    if (file == nullptr) {
        // A path that never existed, or one deleted between a caller's stat and
        // this open. Callers surface their own error; reading on regardless
        // aborts the process on a null FILE*.
        DEBUG_WRITE_FORCE("File::ReadText: cannot open %s", filePath.c_str());
        return nullptr;
    }
    fseek(file, 0, SEEK_END);

    charLength = ftell(file);
    isNew = charLength > BUFFER_SIZE;

    rewind(file);

    if (isNew) {
        char* newBuffer = new char[charLength];
        fread(newBuffer, 1, charLength, file);
        fclose(file);

        return newBuffer;
    }

    fread(Buffer, 1, charLength, file);
    fclose(file);

    return Buffer;
}

char* File::Buffer = new char[BUFFER_SIZE];

const char* File::WRITE_BINARY = "wb";
const char* File::READ_BINARY = "rb";
}
