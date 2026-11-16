#ifndef SCRIPTCACHE_H_
#define SCRIPTCACHE_H_

#include <cstdint>
#include <string>

#include "v8.h"

namespace tns {

/*
 * The V8 code cache of one script, kept as <code cache dir>/v8/<hash of the
 * logical path>.{cache,mcache}. An entry is used only when it was produced
 * from byte-identical source, and its payload checksum holds: release V8
 * skips its own checksum. Every failure to read or write one is silent; the
 * script simply compiles without it.
 */
class ScriptCache {
public:
    /*
     * A classic-script cache and a module cache are not interchangeable, and a
     * `.js` file is reachable both ways (require() compiles it classic, an
     * `import` as a module), so the kind is part of the file name.
     */
    enum class Kind {
        Classic,
        Module,
    };

    // Only the first call takes effect; caching stays off until then.
    static void Init(const std::string& codeCacheDir);

    // `source` is exactly the text handed to V8.
    ScriptCache(const std::string& path, Kind kind, const std::string& source);

    // Whether the `codeCache` option is on and there is a directory to use.
    bool Enabled() const { return m_enabled; }

    // Null when there is no valid entry; ownership passes to the caller.
    v8::ScriptCompiler::CachedData* Load() const;

    // Takes ownership of `data`.
    void Save(const v8::ScriptCompiler::CachedData* data) const;

private:
    bool m_enabled;
    std::string m_file;
    uint32_t m_sourceLength = 0;
    uint32_t m_sourceCrc = 0;
};

}  // namespace tns

#endif /* SCRIPTCACHE_H_ */
