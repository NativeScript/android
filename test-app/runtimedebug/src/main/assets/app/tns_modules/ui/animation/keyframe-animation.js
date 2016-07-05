var animationModule = require("ui/animation");
var enums = require("ui/enums");
var style = require("ui/styling/style");
var KeyframeDeclaration = (function () {
    function KeyframeDeclaration() {
    }
    return KeyframeDeclaration;
}());
exports.KeyframeDeclaration = KeyframeDeclaration;
var KeyframeInfo = (function () {
    function KeyframeInfo() {
    }
    return KeyframeInfo;
}());
exports.KeyframeInfo = KeyframeInfo;
var KeyframeAnimationInfo = (function () {
    function KeyframeAnimationInfo() {
        this.name = "";
        this.duration = 0.3;
        this.delay = 0;
        this.iterations = 1;
        this.curve = enums.AnimationCurve.ease;
        this.isForwards = false;
        this.isReverse = false;
    }
    return KeyframeAnimationInfo;
}());
exports.KeyframeAnimationInfo = KeyframeAnimationInfo;
var KeyframeAnimation = (function () {
    function KeyframeAnimation() {
        this.delay = 0;
        this.iterations = 1;
    }
    KeyframeAnimation.keyframeAnimationFromInfo = function (info, valueSourceModifier) {
        var animations = new Array();
        var length = info.keyframes.length;
        var startDuration = 0;
        if (info.isReverse) {
            for (var index_1 = length - 1; index_1 >= 0; index_1--) {
                var keyframe = info.keyframes[index_1];
                startDuration = KeyframeAnimation.parseKeyframe(info, keyframe, animations, startDuration, valueSourceModifier);
            }
        }
        else {
            for (var index_2 = 0; index_2 < length; index_2++) {
                var keyframe = info.keyframes[index_2];
                startDuration = KeyframeAnimation.parseKeyframe(info, keyframe, animations, startDuration, valueSourceModifier);
            }
            for (var index_3 = length - 1; index_3 > 0; index_3--) {
                var a1 = animations[index_3];
                var a2 = animations[index_3 - 1];
                if (a2["curve"] !== undefined) {
                    a1["curve"] = a2["curve"];
                    a2["curve"] = undefined;
                }
            }
        }
        for (var index_4 = 1; index_4 < length; index_4++) {
            var a = animations[index_4];
            if (a["curve"] === undefined) {
                a["curve"] = info.curve;
            }
        }
        var animation = new KeyframeAnimation();
        animation.delay = info.delay;
        animation.iterations = info.iterations;
        animation.animations = animations;
        animation._isForwards = info.isForwards;
        return animation;
    };
    KeyframeAnimation.parseKeyframe = function (info, keyframe, animations, startDuration, valueSourceModifier) {
        var animation = {};
        for (var _i = 0, _a = keyframe.declarations; _i < _a.length; _i++) {
            var declaration = _a[_i];
            animation[declaration.property] = declaration.value;
        }
        var duration = keyframe.duration;
        if (duration === 0) {
            duration = 0.01;
        }
        else {
            duration = (info.duration * duration) - startDuration;
            startDuration += duration;
        }
        animation["duration"] = info.isReverse ? info.duration - duration : duration;
        animation["curve"] = keyframe.curve;
        animation["forceLayer"] = true;
        animation["valueSource"] = valueSourceModifier;
        animations.push(animation);
        return startDuration;
    };
    Object.defineProperty(KeyframeAnimation.prototype, "isPlaying", {
        get: function () {
            return this._isPlaying;
        },
        enumerable: true,
        configurable: true
    });
    KeyframeAnimation.prototype.cancel = function () {
        if (this._isPlaying) {
            this._isPlaying = false;
            for (var i = this._nativeAnimations.length - 1; i >= 0; i--) {
                var animation = this._nativeAnimations[i];
                if (animation.isPlaying) {
                    animation.cancel();
                }
            }
            if (this._nativeAnimations.length > 0) {
                var animation = this._nativeAnimations[0];
                this._resetAnimationValues(this._target, animation);
            }
            this._rejectAnimationFinishedPromise();
        }
    };
    KeyframeAnimation.prototype.play = function (view) {
        var _this = this;
        if (this._isPlaying) {
            throw new Error("Animation is already playing.");
        }
        var animationFinishedPromise = new Promise(function (resolve, reject) {
            _this._resolve = resolve;
            _this._reject = reject;
        });
        this._isPlaying = true;
        this._nativeAnimations = new Array();
        this._target = view;
        if (this.delay !== 0) {
            var that_1 = this;
            setTimeout(function () { that_1.animate(view, 0, that_1.iterations); }, that_1.delay, that_1);
        }
        else {
            this.animate(view, 0, this.iterations);
        }
        return animationFinishedPromise;
    };
    KeyframeAnimation.prototype.animate = function (view, index, iterations) {
        var _this = this;
        if (!this._isPlaying) {
            return;
        }
        if (index === 0) {
            var animation = this.animations[0];
            var modifier = animation["valueSource"];
            if ("backgroundColor" in animation) {
                view.style._setValue(style.backgroundColorProperty, animation["backgroundColor"], modifier);
            }
            if ("scale" in animation) {
                view.style._setValue(style.scaleXProperty, animation["scale"].x, modifier);
                view.style._setValue(style.scaleYProperty, animation["scale"].y, modifier);
            }
            if ("translate" in animation) {
                view.style._setValue(style.translateXProperty, animation["translate"].x, modifier);
                view.style._setValue(style.translateYProperty, animation["translate"].y, modifier);
            }
            if ("rotate" in animation) {
                view.style._setValue(style.rotateProperty, animation["rotate"], modifier);
            }
            if ("opacity" in animation) {
                view.style._setValue(style.opacityProperty, animation["opacity"], modifier);
            }
            var that_2 = this;
            setTimeout(function () { that_2.animate(view, 1, iterations); }, 1, that_2);
        }
        else if (index < 0 || index >= this.animations.length) {
            iterations -= 1;
            if (iterations > 0) {
                this.animate(view, 0, iterations);
            }
            else {
                if (this._isForwards === false) {
                    var animation = this.animations[this.animations.length - 1];
                    this._resetAnimationValues(view, animation);
                }
                this._resolveAnimationFinishedPromise();
            }
        }
        else {
            var animationDef = this.animations[index];
            animationDef.target = view;
            var animation = new animationModule.Animation([animationDef]);
            animation.play().then(function () {
                _this.animate(view, index + 1, iterations);
            });
            this._nativeAnimations.push(animation);
        }
    };
    KeyframeAnimation.prototype._resolveAnimationFinishedPromise = function () {
        this._nativeAnimations = new Array();
        this._isPlaying = false;
        this._target = null;
        this._resolve();
    };
    KeyframeAnimation.prototype._rejectAnimationFinishedPromise = function () {
        this._nativeAnimations = new Array();
        this._isPlaying = false;
        this._target = null;
        this._reject(new Error("Animation cancelled."));
    };
    KeyframeAnimation.prototype._resetAnimationValues = function (view, animation) {
        var modifier = animation["valueSource"];
        if ("backgroundColor" in animation) {
            view.style._resetValue(style.backgroundColorProperty, modifier);
        }
        if ("scale" in animation) {
            view.style._resetValue(style.scaleXProperty, modifier);
            view.style._resetValue(style.scaleYProperty, modifier);
        }
        if ("translate" in animation) {
            view.style._resetValue(style.translateXProperty, modifier);
            view.style._resetValue(style.translateYProperty, modifier);
        }
        if ("rotate" in animation) {
            view.style._resetValue(style.rotateProperty, modifier);
        }
        if ("opacity" in animation) {
            view.style._resetValue(style.opacityProperty, modifier);
        }
    };
    return KeyframeAnimation;
}());
exports.KeyframeAnimation = KeyframeAnimation;
