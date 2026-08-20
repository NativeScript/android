// A static import is the third route to a built-in module, alongside require()
// and import(); this fixture exists to fail on it.
import "node:path";

export const unreachable = true;
