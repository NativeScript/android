#ifndef APPFILES_H_
#define APPFILES_H_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace tns {
namespace AppFiles {

/*
 * Sets the app's files directory: the canonical path, which is the logical
 * root V8 sees, plus any other spellings of it. Only the first call takes
 * effect, so every runtime may call it; the roots below are empty until then.
 */
void Init(const std::string& filesRoot, const std::vector<std::string>& aliases = {});

// The logical "<files>", "<files>/app", "<files>/internal" and "<files>/metadata",
// with no trailing slash.
const std::string& FilesRoot();
const std::string& AppRoot();
const std::string& InternalRoot();
const std::string& MetadataRoot();

// "<files>/app/", for prefix checks and joining relative names.
const std::string& AppRootPrefix();

/*
 * The logical spelling of `path`: "." / ".." / "//" are collapsed and any
 * alias of the files directory becomes FilesRoot(). A path that does not end
 * up inside the files directory is returned unchanged.
 */
std::string Normalize(const std::string& path);

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

}  // namespace AppFiles
}  // namespace tns

#endif /* APPFILES_H_ */
