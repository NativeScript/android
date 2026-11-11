#ifndef EXTENDFILEIDENTIFIER_H_
#define EXTENDFILEIDENTIFIER_H_

#include <string>

namespace tns {

/**
 * The file part of an anonymous extended class's name, for a script path relative to the
 * app root with its extension removed.
 *
 * Must stay identical to the static binding generator (the file name built in
 * jsparser's es5-visitors.js, then Generator.generateUniqueFileIdentifier and
 * Generator.getNormalizedName). A different name makes the runtime miss the prebuilt
 * class and generate one at runtime.
 */
std::string ExtendFileIdentifier(const std::string& relativePathWithoutExtension);

}

#endif /* EXTENDFILEIDENTIFIER_H_ */
