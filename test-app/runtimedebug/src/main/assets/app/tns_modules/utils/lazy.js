function lazy(action) {
    var _value;
    return function () { return _value || (_value = action()); };
}
Object.defineProperty(exports, "__esModule", { value: true });
exports.default = lazy;
