function require_factory(requireInternal, dirName) {
    return function require(modulePath) {
        if (global.__requireOverride) {
            var result = global.__requireOverride(modulePath, dirName);
            if (result) {
                return result;
            }
        }
        return requireInternal(modulePath, dirName);
    }
}
module.exports = require_factory;
