#include "AppFiles.h"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <mutex>
#include <sys/stat.h>
#include <unistd.h>

namespace tns {
namespace AppFiles {

namespace {

// One place app files can come from. `relative` is relative to the files
// root, e.g. "app/main.js" or "metadata/treeNodeStream.dat".
class Source {
public:
    virtual ~Source() = default;
    virtual bool IsFile(const std::string& relative) const = 0;
    // Null with errno ENOENT or ENOTDIR when this source has no such file.
    virtual std::unique_ptr<Reader> Open(const std::string& relative) const = 0;
};

class FileReader final : public Reader {
public:
    FileReader(int fd, size_t size) : m_fd(fd), m_size(size) {}
    ~FileReader() override { close(m_fd); }

    size_t size() const override { return m_size; }

    size_t read(void* dst, size_t n) override {
        auto* out = static_cast<char*>(dst);
        size_t total = 0;
        while (total < n) {
            ssize_t r = ::read(m_fd, out + total, n - total);
            if (r < 0 && errno == EINTR) {
                continue;
            }
            if (r <= 0) {
                break;
            }
            total += static_cast<size_t>(r);
        }
        return total;
    }

private:
    int m_fd;
    size_t m_size;
};

bool IsRegularFile(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

std::unique_ptr<Reader> OpenRegularFile(const std::string& path) {
    int fd = open(path.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        return nullptr;
    }
    struct stat st;
    int error = 0;
    if (fstat(fd, &st) != 0) {
        error = errno;
    } else if (!S_ISREG(st.st_mode)) {
        error = S_ISDIR(st.st_mode) ? EISDIR : EINVAL;
    }
    if (error != 0) {
        close(fd);
        errno = error;
        return nullptr;
    }
    return std::unique_ptr<Reader>(new FileReader(fd, static_cast<size_t>(st.st_size)));
}

class DirectorySource final : public Source {
public:
    explicit DirectorySource(std::string root) : m_root(std::move(root)) {}

    bool IsFile(const std::string& relative) const override {
        return IsRegularFile(m_root + "/" + relative);
    }

    std::unique_ptr<Reader> Open(const std::string& relative) const override {
        return OpenRegularFile(m_root + "/" + relative);
    }

private:
    std::string m_root;
};

struct State {
    std::string files;
    std::string app;
    std::string appPrefix;
    std::string internal;
    std::string metadata;
    // Spellings of `files` other than itself.
    std::vector<std::string> aliases;
    // Searched in order; the first source that has a file serves it.
    std::vector<std::unique_ptr<Source>> sources;
};

State s_state;
std::once_flag s_initOnce;

// Collapses "//", "." and ".." in an absolute path; ".." never climbs above "/".
std::string CollapseDotSegments(const std::string& path) {
    std::vector<std::string> segments;
    size_t i = 0;
    while (i <= path.size()) {
        size_t j = path.find('/', i);
        if (j == std::string::npos) {
            j = path.size();
        }
        std::string segment = path.substr(i, j - i);
        if (segment == "..") {
            if (!segments.empty()) {
                segments.pop_back();
            }
        } else if (!segment.empty() && segment != ".") {
            segments.push_back(std::move(segment));
        }
        i = j + 1;
    }
    std::string result;
    for (const auto& segment : segments) {
        result += "/";
        result += segment;
    }
    return result.empty() ? "/" : result;
}

bool HasDirPrefix(const std::string& path, const std::string& dir) {
    return path.size() > dir.size() && path.compare(0, dir.size(), dir) == 0 &&
           path[dir.size()] == '/';
}

// /data/data/<pkg> and /data/user/0/<pkg> name the same directory.
std::string SwappedDataSpelling(const std::string& path) {
    static const std::string kDataData = "/data/data/";
    static const std::string kDataUser0 = "/data/user/0/";
    if (path.compare(0, kDataData.size(), kDataData) == 0) {
        return kDataUser0 + path.substr(kDataData.size());
    }
    if (path.compare(0, kDataUser0.size(), kDataUser0) == 0) {
        return kDataData + path.substr(kDataUser0.size());
    }
    return std::string();
}

void AddAlias(const std::string& alias) {
    if (alias.empty() || alias[0] != '/') {
        return;
    }
    std::string collapsed = CollapseDotSegments(alias);
    if (collapsed == s_state.files) {
        return;
    }
    for (const auto& existing : s_state.aliases) {
        if (existing == collapsed) {
            return;
        }
    }
    s_state.aliases.push_back(std::move(collapsed));
}

// The path relative to the files root when `path` names something under one
// of the layered roots (app, internal, metadata).
bool LayeredRelativePath(const std::string& path, std::string& relative) {
    if (s_state.files.empty()) {
        return false;
    }
    std::string logical;
    if (!ToLogical(path, logical) || !HasDirPrefix(logical, s_state.files)) {
        return false;
    }
    std::string rest = logical.substr(s_state.files.size() + 1);
    for (const char* root : {"app/", "internal/", "metadata/"}) {
        if (rest.compare(0, strlen(root), root) == 0) {
            relative = std::move(rest);
            return true;
        }
    }
    return false;
}

}  // namespace

void Init(const std::string& filesRoot, const std::vector<std::string>& aliases) {
    std::call_once(s_initOnce, [&]() {
        s_state.files = filesRoot.empty() || filesRoot[0] != '/'
                ? filesRoot
                : CollapseDotSegments(filesRoot);
        s_state.app = s_state.files + "/app";
        s_state.appPrefix = s_state.app + "/";
        s_state.internal = s_state.files + "/internal";
        s_state.metadata = s_state.files + "/metadata";

        AddAlias(SwappedDataSpelling(s_state.files));
        for (const auto& alias : aliases) {
            AddAlias(alias);
            AddAlias(SwappedDataSpelling(alias));
        }

        s_state.sources.emplace_back(new DirectorySource(s_state.files));
    });
}

const std::string& FilesRoot() {
    return s_state.files;
}

const std::string& AppRoot() {
    return s_state.app;
}

const std::string& InternalRoot() {
    return s_state.internal;
}

const std::string& MetadataRoot() {
    return s_state.metadata;
}

const std::string& AppRootPrefix() {
    return s_state.appPrefix;
}

bool ToLogical(const std::string& path, std::string& logical) {
    if (s_state.files.empty() || path.empty() || path[0] != '/') {
        return false;
    }
    std::string collapsed = CollapseDotSegments(path);
    if (collapsed == s_state.files || HasDirPrefix(collapsed, s_state.files)) {
        logical = std::move(collapsed);
        return true;
    }
    for (const auto& alias : s_state.aliases) {
        if (collapsed == alias || HasDirPrefix(collapsed, alias)) {
            logical = s_state.files + collapsed.substr(alias.size());
            return true;
        }
    }
    return false;
}

bool IsFile(const std::string& path) {
    std::string relative;
    if (!LayeredRelativePath(path, relative)) {
        return IsRegularFile(path);
    }
    for (const auto& source : s_state.sources) {
        if (source->IsFile(relative)) {
            return true;
        }
    }
    return false;
}

std::unique_ptr<Reader> Open(const std::string& path) {
    std::string relative;
    if (!LayeredRelativePath(path, relative)) {
        return OpenRegularFile(path);
    }
    errno = ENOENT;
    for (const auto& source : s_state.sources) {
        auto reader = source->Open(relative);
        if (reader != nullptr || (errno != ENOENT && errno != ENOTDIR)) {
            return reader;
        }
    }
    return nullptr;
}

bool ReadText(const std::string& path, std::string& out) {
    auto reader = Open(path);
    if (reader == nullptr) {
        out.clear();
        return false;
    }
    out.resize(reader->size());
    out.resize(reader->read(&out[0], out.size()));
    return true;
}

}  // namespace AppFiles
}  // namespace tns
