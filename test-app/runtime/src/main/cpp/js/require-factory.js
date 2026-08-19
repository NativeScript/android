function require_factory(requireInternal, dirName, policy, deadlineSeconds, throwOnTimeout,
                         pumpRunLoop) {
    return function require(modulePath) {
        if (global.__requireOverride) {
            var result = global.__requireOverride(modulePath, dirName);
            if (result) {
                return result;
            }
        }
        // `policy` and the three evaluate options are opaque native tokens,
        // resolved once when this require was minted; undefined means the
        // strict default.
        return requireInternal(modulePath, dirName, policy, deadlineSeconds, throwOnTimeout,
                               pumpRunLoop);
    }
}
module.exports = require_factory;
