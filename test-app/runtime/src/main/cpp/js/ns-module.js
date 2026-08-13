"use strict";

// The `ns:module` builtin: the dev-loader control surface the runtime
// exposes to development tooling (docs/ns-builtin-modules.md). Every member
// is a native function handed in through `binding`; this file only shapes
// and freezes the exports.
//
// Membership varies by build:
//   - `canonicalizeHttpUrlKey` exists only in debug builds (test diagnostic).
// Missing members are simply absent — never present-but-throwing — so
// feature checks work.

const { ObjectFreeze } = primordials;

const surface = {
  configureLoader: binding.configureLoader,
  invalidateModules: binding.invalidateModules,
  getLoadedModuleUrls: binding.getLoadedModuleUrls,
  setDevBootComplete: binding.setDevBootComplete,
};
if (binding.canonicalizeHttpUrlKey !== undefined) {
  surface.canonicalizeHttpUrlKey = binding.canonicalizeHttpUrlKey;
}

module.exports = ObjectFreeze(surface);
