#ifndef APPFILES_H_
#define APPFILES_H_

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct AAssetManager;

namespace tns {
namespace AppFiles {

// What a map layer answers for one app-relative path.
struct LayerEntry {
    enum class Kind {
        File,
        Asset,
        Whiteout,
    };
    Kind kind = Kind::Whiteout;
    // An absolute file path or an asset path.
    std::string target;
};

struct Layer {
    enum class Kind {
        // The files directory, laid out like the files root.
        Extracted,
        // The APK-served set, straight from the APK.
        Apk,
        // From customizers; these answer only paths under app/.
        Directory,
        ApkAssets,
        Map,
    };
    Kind kind = Kind::Extracted;
    // Directory: a directory laid out like the app root. ApkAssets: an asset
    // directory laid out like the app root.
    std::string root;
    // Map: app-relative path -> entry.
    std::vector<std::pair<std::string, LayerEntry>> entries;
};

struct Config {
    // The canonical files directory: the logical root V8 sees.
    std::string filesRoot;
    // Other spellings of filesRoot.
    std::vector<std::string> aliases;
    // The APK's assets; needed by loadCodeFromApk and by asset layers.
    AAssetManager* assets = nullptr;
    bool loadCodeFromApk = false;
    // Globs relative to the app root that are extracted even when they are code.
    std::vector<std::string> alwaysExtract;
    // Searched top first. Empty means the built-in order: the extracted files,
    // then the APK. An Apk layer is inert without loadCodeFromApk.
    std::vector<Layer> layers;
};

/*
 * Only the first call takes effect, so every runtime may call it; the roots
 * below are empty until then. Must run before any read and before assets are
 * extracted, since the extractor consults IsApkServed.
 */
void Init(const Config& config);

// Files directory only, every app file read from disk.
void Init(const std::string& filesRoot);

// The logical "<files>", "<files>/app", "<files>/internal" and "<files>/metadata",
// with no trailing slash.
const std::string& FilesRoot();
const std::string& AppRoot();
const std::string& InternalRoot();
const std::string& MetadataRoot();

// "<files>/app/", for prefix checks and joining relative names.
const std::string& AppRootPrefix();

bool LoadsCodeFromApk();

/*
 * The one rule for which files are served from the APK instead of being
 * extracted: with loadCodeFromApk, .js/.mjs/.cjs/.map under app/ (minus the
 * alwaysExtract globs) and everything under internal/ and metadata/.
 * `relative` is relative to the files root, e.g. "app/main.js".
 */
bool IsApkServed(const std::string& relative);

/*
 * True when `path` names something inside the files directory under any of
 * its spellings. `logical` is then that path with "//", "." and ".."
 * collapsed and the directory spelled as FilesRoot().
 */
bool ToLogical(const std::string& path, std::string& logical);

/*
 * Reads of app files. A path under the app, internal or metadata root is
 * resolved through the app file sources; anything else reads the real file
 * system as-is. Safe to call from any thread.
 */
bool IsFile(const std::string& path);

// False when the file cannot be opened; a short read truncates `out`.
bool ReadText(const std::string& path, std::string& out);

class Reader {
public:
    virtual ~Reader() = default;
    virtual size_t size() const = 0;
    // Fills `dst` with up to `n` bytes; fewer only at end of file or on error.
    virtual size_t read(void* dst, size_t n) = 0;
};

// Null when the file cannot be opened, with errno describing why.
// A Reader belongs to one thread at a time.
std::unique_ptr<Reader> Open(const std::string& path);

/*
 * The real file that serves `path`, for consumers that need one (dlopen).
 * False when `path` is served from the APK or does not exist.
 */
bool RealFilePath(const std::string& path, std::string& realPath);

}  // namespace AppFiles
}  // namespace tns

#endif /* APPFILES_H_ */
