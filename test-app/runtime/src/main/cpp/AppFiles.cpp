#include "AppFiles.h"

#include <android/asset_manager.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <mutex>
#include <sys/stat.h>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>

namespace tns {
namespace AppFiles {

namespace {

enum class Lookup {
    Miss,
    Hit,
};

/*
 * One place app files can come from. `relative` is relative to the files
 * root, e.g. "app/main.js" or "metadata/treeNodeStream.dat". Sources are
 * immutable after Init, apart from internal caches they guard themselves.
 */
class Source {
public:
    virtual ~Source() = default;
    virtual Lookup Find(const std::string& relative) const = 0;
    // Only called after Find hit; null with errno set when the open fails.
    virtual std::unique_ptr<Reader> Open(const std::string& relative) const = 0;
    virtual bool RealFilePath(const std::string& relative, std::string& realPath) const {
        return false;
    }
};

// Which part of the layered roots a built-in source answers for.
enum class Serves {
    Everything,
    ApkServedOnly,
};

bool InScope(Serves serves, const std::string& relative) {
    switch (serves) {
        case Serves::Everything:
            return true;
        case Serves::ApkServedOnly:
            return IsApkServed(relative);
    }
    return false;
}

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

class AssetReader final : public Reader {
public:
    explicit AssetReader(AAsset* asset) : m_asset(asset) {}
    ~AssetReader() override { AAsset_close(m_asset); }

    size_t size() const override { return static_cast<size_t>(AAsset_getLength64(m_asset)); }

    size_t read(void* dst, size_t n) override {
        auto* out = static_cast<char*>(dst);
        size_t total = 0;
        while (total < n) {
            int r = AAsset_read(m_asset, out + total, n - total);
            if (r <= 0) {
                break;
            }
            total += static_cast<size_t>(r);
        }
        return total;
    }

private:
    AAsset* m_asset;
};

class DirectorySource final : public Source {
public:
    DirectorySource(std::string root, Serves serves) : m_root(std::move(root)), m_serves(serves) {}

    Lookup Find(const std::string& relative) const override {
        return InScope(m_serves, relative) && IsRegularFile(m_root + "/" + relative)
                ? Lookup::Hit
                : Lookup::Miss;
    }

    std::unique_ptr<Reader> Open(const std::string& relative) const override {
        return OpenRegularFile(m_root + "/" + relative);
    }

    bool RealFilePath(const std::string& relative, std::string& realPath) const override {
        realPath = m_root + "/" + relative;
        return true;
    }

private:
    std::string m_root;
    Serves m_serves;
};

/*
 * The APK's assets, whose paths mirror the files root ("app/main.js" is the
 * asset "app/main.js"). Existence is answered from one AAssetManager_openDir
 * listing per directory, taken on first use; AAssets are never shared, every
 * Open gets its own.
 */
class ApkSource final : public Source {
public:
    ApkSource(AAssetManager* assets, Serves serves) : m_assets(assets), m_serves(serves) {}

    Lookup Find(const std::string& relative) const override {
        return InScope(m_serves, relative) && AssetExists(relative) ? Lookup::Hit : Lookup::Miss;
    }

    std::unique_ptr<Reader> Open(const std::string& relative) const override {
        AAsset* asset = AAssetManager_open(m_assets, relative.c_str(), AASSET_MODE_STREAMING);
        if (asset == nullptr) {
            errno = ENOENT;
            return nullptr;
        }
        return std::unique_ptr<Reader>(new AssetReader(asset));
    }

private:
    bool AssetExists(const std::string& assetPath) const {
        size_t slash = assetPath.find_last_of('/');
        std::string dir = slash == std::string::npos ? std::string() : assetPath.substr(0, slash);
        std::string name = slash == std::string::npos ? assetPath : assetPath.substr(slash + 1);

        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_listings.find(dir);
        if (it == m_listings.end()) {
            std::unordered_set<std::string> names;
            if (AAssetDir* listing = AAssetManager_openDir(m_assets, dir.c_str())) {
                while (const char* entry = AAssetDir_getNextFileName(listing)) {
                    names.insert(entry);
                }
                AAssetDir_close(listing);
            }
            it = m_listings.emplace(std::move(dir), std::move(names)).first;
        }
        return it->second.count(name) != 0;
    }

    AAssetManager* m_assets;
    Serves m_serves;
    mutable std::mutex m_mutex;
    // Directory -> the regular files directly in it.
    mutable std::unordered_map<std::string, std::unordered_set<std::string>> m_listings;
};

struct State {
    std::string files;
    std::string app;
    std::string appPrefix;
    std::string internal;
    std::string metadata;
    // Spellings of `files` other than itself.
    std::vector<std::string> aliases;
    bool loadCodeFromApk = false;
    std::vector<std::string> alwaysExtract;
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

bool HasPrefix(const std::string& s, const char* prefix) {
    return s.compare(0, strlen(prefix), prefix) == 0;
}

/*
 * Glob match where "*" and "?" stay within one path segment and "**" spans
 * segments; a "**" segment followed by a slash also matches zero directories.
 */
bool GlobMatch(const char* pattern, const char* path) {
    while (*pattern != '\0') {
        if (pattern[0] == '*' && pattern[1] == '*') {
            pattern += 2;
            if (*pattern == '/') {
                ++pattern;
                for (const char* p = path;; ++p) {
                    if ((p == path || p[-1] == '/') && GlobMatch(pattern, p)) {
                        return true;
                    }
                    if (*p == '\0') {
                        return false;
                    }
                }
            }
            for (const char* p = path;; ++p) {
                if (GlobMatch(pattern, p)) {
                    return true;
                }
                if (*p == '\0') {
                    return false;
                }
            }
        }
        if (*pattern == '*') {
            ++pattern;
            for (const char* p = path;; ++p) {
                if (GlobMatch(pattern, p)) {
                    return true;
                }
                if (*p == '\0' || *p == '/') {
                    return false;
                }
            }
        }
        if (*path == '\0') {
            return false;
        }
        if (*pattern == '?' ? *path == '/' : *pattern != *path) {
            return false;
        }
        ++pattern;
        ++path;
    }
    return *path == '\0';
}

bool IsCodeExtension(const std::string& relative) {
    size_t slash = relative.find_last_of('/');
    size_t dot = relative.find_last_of('.');
    if (dot == std::string::npos || (slash != std::string::npos && dot < slash)) {
        return false;
    }
    const char* ext = relative.c_str() + dot + 1;
    return strcmp(ext, "js") == 0 || strcmp(ext, "mjs") == 0 || strcmp(ext, "cjs") == 0 ||
           strcmp(ext, "map") == 0;
}

// The path relative to the files root when `path` names something under one
// of the layered roots (app, internal, metadata).
bool LayeredRelativePath(const std::string& path, std::string& relative) {
    std::string logical;
    if (!ToLogical(path, logical) || !HasDirPrefix(logical, s_state.files)) {
        return false;
    }
    std::string rest = logical.substr(s_state.files.size() + 1);
    if (HasPrefix(rest, "app/") || HasPrefix(rest, "internal/") || HasPrefix(rest, "metadata/")) {
        relative = std::move(rest);
        return true;
    }
    return false;
}

// The first source that has `relative`, or null.
const Source* FindSource(const std::string& relative) {
    for (const auto& source : s_state.sources) {
        if (source->Find(relative) == Lookup::Hit) {
            return source.get();
        }
    }
    return nullptr;
}

}  // namespace

void Init(const Config& config) {
    std::call_once(s_initOnce, [&config]() {
        const std::string& filesRoot = config.filesRoot;
        s_state.files = filesRoot.empty() || filesRoot[0] != '/'
                ? filesRoot
                : CollapseDotSegments(filesRoot);
        s_state.app = s_state.files + "/app";
        s_state.appPrefix = s_state.app + "/";
        s_state.internal = s_state.files + "/internal";
        s_state.metadata = s_state.files + "/metadata";

        AddAlias(SwappedDataSpelling(s_state.files));
        for (const auto& alias : config.aliases) {
            AddAlias(alias);
            AddAlias(SwappedDataSpelling(alias));
        }

        s_state.loadCodeFromApk = config.loadCodeFromApk && config.assets != nullptr;
        s_state.alwaysExtract = config.alwaysExtract;

        // Disk first: what LiveSync or the app itself writes into the files
        // directory wins over the APK's copy.
        s_state.sources.emplace_back(new DirectorySource(s_state.files, Serves::Everything));
        if (s_state.loadCodeFromApk) {
            s_state.sources.emplace_back(new ApkSource(config.assets, Serves::ApkServedOnly));
        }
    });
}

void Init(const std::string& filesRoot) {
    Config config;
    config.filesRoot = filesRoot;
    Init(config);
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

bool LoadsCodeFromApk() {
    return s_state.loadCodeFromApk;
}

bool IsApkServed(const std::string& relative) {
    if (!s_state.loadCodeFromApk) {
        return false;
    }
    if (HasPrefix(relative, "internal/") || HasPrefix(relative, "metadata/")) {
        return true;
    }
    if (!HasPrefix(relative, "app/") || !IsCodeExtension(relative)) {
        return false;
    }
    const char* appRelative = relative.c_str() + strlen("app/");
    for (const auto& glob : s_state.alwaysExtract) {
        if (GlobMatch(glob.c_str(), appRelative)) {
            return false;
        }
    }
    return true;
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
    return FindSource(relative) != nullptr;
}

std::unique_ptr<Reader> Open(const std::string& path) {
    std::string relative;
    if (!LayeredRelativePath(path, relative)) {
        return OpenRegularFile(path);
    }
    const Source* source = FindSource(relative);
    if (source == nullptr) {
        errno = ENOENT;
        return nullptr;
    }
    return source->Open(relative);
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

bool RealFilePath(const std::string& path, std::string& realPath) {
    std::string relative;
    if (!LayeredRelativePath(path, relative)) {
        realPath = path;
        return IsRegularFile(path);
    }
    const Source* source = FindSource(relative);
    return source != nullptr && source->RealFilePath(relative, realPath);
}

}  // namespace AppFiles
}  // namespace tns
