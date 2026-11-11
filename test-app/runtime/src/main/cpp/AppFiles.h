#ifndef APPFILES_H_
#define APPFILES_H_

#include <string>

namespace tns {
namespace AppFiles {

/*
 * Sets the app's files directory. Only the first call takes effect, so every
 * runtime may call it; the roots below are empty until then.
 */
void Init(const std::string& filesRoot);

// "<files>/app", no trailing slash.
const std::string& AppRoot();

// "<files>/app/", for prefix checks and joining relative names.
const std::string& AppRootPrefix();

}  // namespace AppFiles
}  // namespace tns

#endif /* APPFILES_H_ */
