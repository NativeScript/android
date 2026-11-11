#include "AppFiles.h"

#include <mutex>

namespace tns {
namespace AppFiles {

namespace {

struct Roots {
    std::string app;
    std::string appPrefix;
};

Roots s_roots;
std::once_flag s_initOnce;

}  // namespace

void Init(const std::string& filesRoot) {
    std::call_once(s_initOnce, [&filesRoot]() {
        s_roots.app = filesRoot + "/app";
        s_roots.appPrefix = s_roots.app + "/";
    });
}

const std::string& AppRoot() {
    return s_roots.app;
}

const std::string& AppRootPrefix() {
    return s_roots.appPrefix;
}

}  // namespace AppFiles
}  // namespace tns
