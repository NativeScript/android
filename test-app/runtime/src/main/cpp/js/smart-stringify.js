function smartStringify(object) {
    const seen = [];
    var replacer = function (key, value) {
        if (value != null && typeof value == "object") {
            if (seen.indexOf(value) >= 0) {
                if (key) {
                    return "[Circular]";
                }
                return;
            }
            seen.push(value);
        }
        return value;
    };
    return JSON.stringify(object, replacer, 2);
}
module.exports = smartStringify;
