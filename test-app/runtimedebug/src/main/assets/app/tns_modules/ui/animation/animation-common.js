var definition = require("ui/animation");
var colorModule = require("color");
var types = require("utils/types");
var trace;
function ensureTrace() {
    if (!trace) {
        trace = require("trace");
    }
}
var Properties;
(function (Properties) {
    Properties.opacity = "opacity";
    Properties.backgroundColor = "backgroundColor";
    Properties.translate = "translate";
    Properties.rotate = "rotate";
    Properties.scale = "scale";
})(Properties = exports.Properties || (exports.Properties = {}));
var CubicBezierAnimationCurve = (function () {
    function CubicBezierAnimationCurve(x1, y1, x2, y2) {
        this.x1 = x1;
        this.y1 = y1;
        this.x2 = x2;
        this.y2 = y2;
    }
    return CubicBezierAnimationCurve;
}());
exports.CubicBezierAnimationCurve = CubicBezierAnimationCurve;
var AnimationPromise = (function () {
    function AnimationPromise() {
    }
    AnimationPromise.prototype.cancel = function () { };
    AnimationPromise.prototype.then = function (onFulfilled, onRejected) { return new AnimationPromise(); };
    AnimationPromise.prototype.catch = function (onRejected) { return new AnimationPromise(); };
    return AnimationPromise;
}());
exports.AnimationPromise = AnimationPromise;
var Animation = (function () {
    function Animation(animationDefinitions, playSequentially) {
        if (!animationDefinitions || animationDefinitions.length === 0) {
            throw new Error("No animation definitions specified");
        }
        ensureTrace();
        if (trace.enabled) {
            trace.write("Analyzing " + animationDefinitions.length + " animation definitions...", trace.categories.Animation);
        }
        this._propertyAnimations = new Array();
        var i = 0;
        var length = animationDefinitions.length;
        for (; i < length; i++) {
            animationDefinitions[i].curve = definition._resolveAnimationCurve(animationDefinitions[i].curve);
            this._propertyAnimations = this._propertyAnimations.concat(Animation._createPropertyAnimations(animationDefinitions[i]));
        }
        if (this._propertyAnimations.length === 0) {
            throw new Error("Nothing to animate.");
        }
        if (trace.enabled) {
            trace.write("Created " + this._propertyAnimations.length + " individual property animations.", trace.categories.Animation);
        }
        this._playSequentially = playSequentially;
    }
    Animation.prototype.play = function () {
        var _this = this;
        if (this.isPlaying) {
            throw new Error("Animation is already playing.");
        }
        var animationFinishedPromise = new Promise(function (resolve, reject) {
            _this._resolve = resolve;
            _this._reject = reject;
        });
        this.fixupAnimationPromise(animationFinishedPromise);
        this._isPlaying = true;
        return animationFinishedPromise;
    };
    Animation.prototype.fixupAnimationPromise = function (promise) {
        var _this = this;
        promise.cancel = function () {
            _this.cancel();
        };
        var _then = promise.then;
        promise.then = function () {
            var r = _then.apply(promise, arguments);
            _this.fixupAnimationPromise(r);
            return r;
        };
        var _catch = promise.catch;
        promise.catch = function () {
            var r = _catch.apply(promise, arguments);
            _this.fixupAnimationPromise(r);
            return r;
        };
    };
    Animation.prototype.cancel = function () {
        if (!this.isPlaying) {
            throw new Error("Animation is not currently playing.");
        }
    };
    Object.defineProperty(Animation.prototype, "isPlaying", {
        get: function () {
            return this._isPlaying;
        },
        enumerable: true,
        configurable: true
    });
    Animation.prototype._resolveAnimationFinishedPromise = function () {
        this._isPlaying = false;
        this._resolve();
    };
    Animation.prototype._rejectAnimationFinishedPromise = function () {
        this._isPlaying = false;
        this._reject(new Error("Animation cancelled."));
    };
    Animation._createPropertyAnimations = function (animationDefinition) {
        if (!animationDefinition.target) {
            throw new Error("No animation target specified.");
        }
        for (var item in animationDefinition) {
            if (!types.isDefined(animationDefinition[item])) {
                continue;
            }
            if ((item === Properties.opacity ||
                item === Properties.rotate ||
                item === "duration" ||
                item === "delay" ||
                item === "iterations") && !types.isNumber(animationDefinition[item])) {
                throw new Error("Property " + item + " must be valid number. Value: " + animationDefinition[item]);
            }
            else if ((item === Properties.scale ||
                item === Properties.translate) &&
                (!types.isNumber(animationDefinition[item].x) ||
                    !types.isNumber(animationDefinition[item].y))) {
                throw new Error("Property " + item + " must be valid Pair. Value: " + animationDefinition[item]);
            }
            else if (item === Properties.backgroundColor && !colorModule.Color.isValid(animationDefinition.backgroundColor)) {
                throw new Error("Property " + item + " must be valid color. Value: " + animationDefinition[item]);
            }
        }
        var propertyAnimations = new Array();
        if (animationDefinition.opacity !== undefined) {
            propertyAnimations.push({
                target: animationDefinition.target,
                property: Properties.opacity,
                value: animationDefinition.opacity,
                duration: animationDefinition.duration,
                delay: animationDefinition.delay,
                iterations: animationDefinition.iterations,
                curve: animationDefinition.curve
            });
        }
        if (animationDefinition.backgroundColor !== undefined) {
            propertyAnimations.push({
                target: animationDefinition.target,
                property: Properties.backgroundColor,
                value: types.isString(animationDefinition.backgroundColor) ?
                    new colorModule.Color(animationDefinition.backgroundColor) : animationDefinition.backgroundColor,
                duration: animationDefinition.duration,
                delay: animationDefinition.delay,
                iterations: animationDefinition.iterations,
                curve: animationDefinition.curve
            });
        }
        if (animationDefinition.translate !== undefined) {
            propertyAnimations.push({
                target: animationDefinition.target,
                property: Properties.translate,
                value: animationDefinition.translate,
                duration: animationDefinition.duration,
                delay: animationDefinition.delay,
                iterations: animationDefinition.iterations,
                curve: animationDefinition.curve
            });
        }
        if (animationDefinition.scale !== undefined) {
            propertyAnimations.push({
                target: animationDefinition.target,
                property: Properties.scale,
                value: animationDefinition.scale,
                duration: animationDefinition.duration,
                delay: animationDefinition.delay,
                iterations: animationDefinition.iterations,
                curve: animationDefinition.curve
            });
        }
        if (animationDefinition.rotate !== undefined) {
            propertyAnimations.push({
                target: animationDefinition.target,
                property: Properties.rotate,
                value: animationDefinition.rotate,
                duration: animationDefinition.duration,
                delay: animationDefinition.delay,
                iterations: animationDefinition.iterations,
                curve: animationDefinition.curve
            });
        }
        if (propertyAnimations.length === 0) {
            throw new Error("No animation property specified.");
        }
        return propertyAnimations;
    };
    Animation._getAnimationInfo = function (animation) {
        return JSON.stringify({
            target: animation.target.id,
            property: animation.property,
            value: animation.value,
            duration: animation.duration,
            delay: animation.delay,
            iterations: animation.iterations,
            curve: animation.curve
        });
    };
    return Animation;
}());
exports.Animation = Animation;
