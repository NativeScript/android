var observable = require("ui/core/dependency-observable");
var styleProperty = require("ui/styling/style-property");
var visualStateConstants = require("ui/styling/visual-state-constants");
var VisualState = (function () {
    function VisualState() {
        this._setters = {};
        this._animatedSelectors = [];
    }
    Object.defineProperty(VisualState.prototype, "setters", {
        get: function () {
            return this._setters;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(VisualState.prototype, "animatedSelectors", {
        get: function () {
            return this._animatedSelectors;
        },
        enumerable: true,
        configurable: true
    });
    return VisualState;
}());
exports.VisualState = VisualState;
function goToState(view, state) {
    var root = view.page;
    if (!root) {
        return undefined;
    }
    var allStates = root._getStyleScope().getVisualStates(view);
    if (!allStates) {
        return undefined;
    }
    if (!(state in allStates)) {
        state = visualStateConstants.Normal;
    }
    if (state !== view.visualState) {
        var newState = allStates[state];
        var oldState = allStates[view.visualState];
        resetProperties(view, oldState, newState);
        applyProperties(view, newState);
    }
    return state;
}
exports.goToState = goToState;
function resetProperties(view, oldState, newState) {
    if (!oldState) {
        return;
    }
    var property;
    for (var name_1 in oldState.setters) {
        if (newState && (name_1 in newState.setters)) {
            continue;
        }
        property = styleProperty.getPropertyByName(name_1);
        if (property) {
            view.style._resetValue(property, observable.ValueSource.VisualState);
        }
    }
    view._unregisterAllAnimations();
    for (var _i = 0, _a = oldState.animatedSelectors; _i < _a.length; _i++) {
        var selector = _a[_i];
        for (var _b = 0, _c = selector.animations; _b < _c.length; _b++) {
            var animationInfo = _c[_b];
            for (var _d = 0, _e = animationInfo.keyframes; _d < _e.length; _d++) {
                var keyframe = _e[_d];
                for (var _f = 0, _g = keyframe.declarations; _f < _g.length; _f++) {
                    var declaration = _g[_f];
                    property = styleProperty.getPropertyByName(declaration.property);
                    if (property) {
                        view.style._resetValue(property, observable.ValueSource.VisualState);
                    }
                }
            }
        }
    }
}
function applyProperties(view, state) {
    if (!state) {
        return;
    }
    var property;
    for (var name_2 in state.setters) {
        property = styleProperty.getPropertyByName(name_2);
        if (property) {
            view.style._setValue(property, state.setters[name_2], observable.ValueSource.VisualState);
        }
    }
    for (var _i = 0, _a = state.animatedSelectors; _i < _a.length; _i++) {
        var selector = _a[_i];
        selector.apply(view, observable.ValueSource.VisualState);
    }
}
