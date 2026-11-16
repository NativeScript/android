#include "ScriptCache.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <mutex>
#include <sys/stat.h>
#include <unistd.h>
#include <zlib.h>

#include "AppFiles.h"
#include "Constants.h"
#include "ManualInstrumentation.h"

namespace tns {

namespace {

constexpr char kMagic[4] = {'N', 'S', 'C', 'C'};
constexpr uint32_t kFormatVersion = 1;

struct Header {
    char magic[4];
    uint32_t formatVersion;
    uint32_t sourceLength;
    uint32_t sourceCrc;
    uint32_t payloadLength;
    uint32_t payloadCrc;
};
static_assert(sizeof(Header) == 24, "the header is written as raw bytes");

std::string s_parentDir;
std::string s_dir;
std::once_flag s_initOnce;

uint32_t Crc32(const void* data, size_t length) {
    uLong crc = crc32(0L, Z_NULL, 0);
    const auto* bytes = static_cast<const Bytef*>(data);
    while (length > 0) {
        uInt chunk = length > 0x40000000 ? 0x40000000 : static_cast<uInt>(length);
        crc = crc32(crc, bytes, chunk);
        bytes += chunk;
        length -= chunk;
    }
    return static_cast<uint32_t>(crc);
}

// FNV-1a: stable across builds and processes, unlike std::hash.
uint64_t PathHash(const std::string& path) {
    uint64_t hash = 0xcbf29ce484222325ULL;
    for (unsigned char c : path) {
        hash ^= c;
        hash *= 0x100000001b3ULL;
    }
    return hash;
}

bool ReadFully(int fd, void* dst, size_t length) {
    auto* out = static_cast<char*>(dst);
    while (length > 0) {
        ssize_t r = read(fd, out, length);
        if (r < 0 && errno == EINTR) {
            continue;
        }
        if (r <= 0) {
            return false;
        }
        out += r;
        length -= static_cast<size_t>(r);
    }
    return true;
}

bool WriteFully(int fd, const void* src, size_t length) {
    const auto* in = static_cast<const char*>(src);
    while (length > 0) {
        ssize_t w = write(fd, in, length);
        if (w < 0 && errno == EINTR) {
            continue;
        }
        if (w <= 0) {
            return false;
        }
        in += w;
        length -= static_cast<size_t>(w);
    }
    return true;
}

}  // namespace

void ScriptCache::Init(const std::string& codeCacheDir) {
    std::call_once(s_initOnce, [&codeCacheDir]() {
        if (!codeCacheDir.empty()) {
            s_parentDir = codeCacheDir;
            s_dir = codeCacheDir + "/v8";
        }
    });
}

ScriptCache::ScriptCache(const std::string& path, Kind kind, const std::string& source)
    : m_enabled(Constants::V8_CACHE_COMPILED_CODE && !s_dir.empty() &&
                source.size() <= UINT32_MAX) {
    if (!m_enabled) {
        return;
    }
    std::string logical;
    if (!AppFiles::ToLogical(path, logical)) {
        logical = path;
    }
    char name[17];
    snprintf(name, sizeof(name), "%016llx", static_cast<unsigned long long>(PathHash(logical)));
    m_file = s_dir + "/" + name + (kind == Kind::Module ? ".mcache" : ".cache");
    m_sourceLength = static_cast<uint32_t>(source.size());
    m_sourceCrc = Crc32(source.data(), source.size());
}

v8::ScriptCompiler::CachedData* ScriptCache::Load() const {
    if (!m_enabled) {
        return nullptr;
    }
    tns::instrumentation::Frame frame("ScriptCache::Load");

    int fd = open(m_file.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        return nullptr;
    }

    Header header;
    struct stat st;
    bool valid = ReadFully(fd, &header, sizeof(header)) && fstat(fd, &st) == 0 &&
                 memcmp(header.magic, kMagic, sizeof(kMagic)) == 0 &&
                 header.formatVersion == kFormatVersion &&
                 header.sourceLength == m_sourceLength && header.sourceCrc == m_sourceCrc &&
                 header.payloadLength > 0 && header.payloadLength <= INT32_MAX &&
                 static_cast<uint64_t>(st.st_size) == sizeof(header) + header.payloadLength;

    std::unique_ptr<uint8_t[]> payload;
    if (valid) {
        payload.reset(new uint8_t[header.payloadLength]);
        valid = ReadFully(fd, payload.get(), header.payloadLength) &&
                Crc32(payload.get(), header.payloadLength) == header.payloadCrc;
    }
    close(fd);

    if (!valid) {
        return nullptr;
    }
    return new v8::ScriptCompiler::CachedData(payload.release(),
                                              static_cast<int>(header.payloadLength),
                                              v8::ScriptCompiler::CachedData::BufferOwned);
}

void ScriptCache::Save(const v8::ScriptCompiler::CachedData* data) const {
    std::unique_ptr<const v8::ScriptCompiler::CachedData> owned(data);
    if (!m_enabled || data == nullptr || data->length <= 0) {
        return;
    }
    tns::instrumentation::Frame frame("ScriptCache::Save");

    // Either can fail (direct boot, a full disk); the write below then fails too.
    mkdir(s_parentDir.c_str(), 0700);
    mkdir(s_dir.c_str(), 0700);

    Header header;
    memcpy(header.magic, kMagic, sizeof(kMagic));
    header.formatVersion = kFormatVersion;
    header.sourceLength = m_sourceLength;
    header.sourceCrc = m_sourceCrc;
    header.payloadLength = static_cast<uint32_t>(data->length);
    header.payloadCrc = Crc32(data->data, static_cast<size_t>(data->length));

    // A unique temp name, then rename: runtimes on other threads may be
    // writing the same entry, and readers must never see a partial file.
    std::string temp = m_file + ".XXXXXX";
    int fd = mkstemp(&temp[0]);
    if (fd < 0) {
        return;
    }
    bool written = WriteFully(fd, &header, sizeof(header)) &&
                   WriteFully(fd, data->data, static_cast<size_t>(data->length));
    written = close(fd) == 0 && written;
    if (!written || rename(temp.c_str(), m_file.c_str()) != 0) {
        unlink(temp.c_str());
    }
}

}  // namespace tns
