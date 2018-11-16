//
// Created by Vladimir Mutafov on 16.11.18.
//

#include "DesugaredInterfaceCompanionClassNameResolver.h"

std::string DesugaredInterfaceCompanionClassNameResolver::resolveD8InterfaceCompanionClassName(
        std::string interfaceName) {
    return interfaceName + D8_COMPANION_CLASS_SUFFIX;
}

std::string DesugaredInterfaceCompanionClassNameResolver::resolveBazelInterfaceCompanionClassName(
        std::string interfaceName) {
    return interfaceName + BAZEL_COMPANION_CLASS_SUFFIX;
}
