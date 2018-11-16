//
// Created by Vladimir Mutafov on 16.11.18.
//

#ifndef TEST_APP_DESUGAREDINTERFACECOMPANIONCLASSNAMERESOLVER_H
#define TEST_APP_DESUGAREDINTERFACECOMPANIONCLASSNAMERESOLVER_H


#include <string>

class DesugaredInterfaceCompanionClassNameResolver {

public:
    std::string resolveD8InterfaceCompanionClassName(std::string interfaceName);

    std::string resolveBazelInterfaceCompanionClassName(std::string interfaceName);

private:
    const std::string BAZEL_COMPANION_CLASS_SUFFIX = "$$CC";
    const std::string D8_COMPANION_CLASS_SUFFIX = "$-CC";


};


#endif //TEST_APP_DESUGAREDINTERFACECOMPANIONCLASSNAMERESOLVER_H
