const { ArrayPrototypeIndexOf, ArrayPrototypePush, JSONStringify } = primordials;

function smartStringify(object) {
    const seen = [];
    var replacer = function (key, value) {
        if (value != null && typeof value == "object") {
            if (ArrayPrototypeIndexOf(seen, value) >= 0) {
                if (key) {
                    return "[Circular]";
                }
                return;
            }
            ArrayPrototypePush(seen, value);
        }
        return value;
    };
    return JSONStringify(object, replacer, 2);
}
module.exports = smartStringify;
