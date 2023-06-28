#ifndef TEST_APP_DESUGAREDINTERFACECOMPANIONCLASSNAMERESOLVER_H
#define TEST_APP_DESUGAREDINTERFACECOMPANIONCLASSNAMERESOLVER_H


#include <string>

class DesugaredInterfaceCompanionClassNameResolver {

public:
    std::string resolveD8InterfaceCompanionClassName(const std::string& interfaceName);

    std::string resolveBazelInterfaceCompanionClassName(const std::string& interfaceName);

private:
    const std::string BAZEL_COMPANION_CLASS_SUFFIX = "$$CC";
    const std::string D8_COMPANION_CLASS_SUFFIX = "$-CC";


};


#endif //TEST_APP_DESUGAREDINTERFACECOMPANIONCLASSNAMERESOLVER_H
