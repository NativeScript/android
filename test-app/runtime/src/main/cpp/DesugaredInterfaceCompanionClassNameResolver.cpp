#include "DesugaredInterfaceCompanionClassNameResolver.h"

std::string DesugaredInterfaceCompanionClassNameResolver::resolveD8InterfaceCompanionClassName(
        const std::string& interfaceName) {
    return interfaceName + D8_COMPANION_CLASS_SUFFIX;
}

std::string DesugaredInterfaceCompanionClassNameResolver::resolveBazelInterfaceCompanionClassName(
        const std::string& interfaceName) {
    return interfaceName + BAZEL_COMPANION_CLASS_SUFFIX;
}
