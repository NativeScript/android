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
    ok = false;

    FILE* file = fopen(filePath.c_str(), READ_BINARY);
    if (file == nullptr) {
        // A path that never existed, or one deleted between a caller's stat and
        // this open. Callers surface their own error; reading on regardless
        // aborts the process on a null FILE*.
        DEBUG_WRITE_FORCE("File::ReadText: cannot open %s", filePath.c_str());
        return string();
    }

    fseek(file, 0, SEEK_END);
    auto size = ftell(file);
    if (size < 0) {
        DEBUG_WRITE_FORCE("File::ReadText: cannot size %s", filePath.c_str());
        fclose(file);
        return string();
    }
    rewind(file);

    // Read straight into the string that is returned: main and worker runtimes
    // read modules concurrently from their own threads, so there is no scratch
    // buffer to share, and the caller needs a string either way.
    string content(static_cast<size_t>(size), '\0');
    auto bytesRead = fread(content.data(), 1, static_cast<size_t>(size), file);
    fclose(file);

    // A short read is a truncated file, not a failure to open; `size` is only
    // what the directory entry claimed.
    content.resize(bytesRead);

    ok = true;
    return content;
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

const char* File::WRITE_BINARY = "wb";
const char* File::READ_BINARY = "rb";
}
