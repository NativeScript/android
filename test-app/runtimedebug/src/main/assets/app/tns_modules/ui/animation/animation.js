var common = require("./animation-common");
var utils = require("utils/utils");
var color = require("color");
var trace = require("trace");
var types = require("utils/types");
var enums = require("ui/enums");
var styleModule = require("ui/styling/style");
var lazy_1 = require("utils/lazy");
var dependencyObservable = require("ui/core/dependency-observable");
global.moduleMerge(common, exports);
var argbEvaluator;
function ensureArgbEvaluator() {
    if (!argbEvaluator) {
        argbEvaluator = new android.animation.ArgbEvaluator();
    }
}
var keyPrefix = "ui.animation.";
var propertyKeys = {};
propertyKeys[common.Properties.backgroundColor] = Symbol(keyPrefix + common.Properties.backgroundColor);
propertyKeys[common.Properties.opacity] = Symbol(keyPrefix + common.Properties.opacity);
propertyKeys[common.Properties.rotate] = Symbol(keyPrefix + common.Properties.rotate);
propertyKeys[common.Properties.scale] = Symbol(keyPrefix + common.Properties.scale);
propertyKeys[common.Properties.translate] = Symbol(keyPrefix + common.Properties.translate);
var Animation = (function (_super) {
    __extends(Animation, _super);
    function Animation(animationDefinitions, playSequentially) {
        _super.call(this, animationDefinitions, playSequentially);
        if (animationDefinitions.length > 0 && animationDefinitions[0].valueSource !== undefined) {
            this._valueSource = animationDefinitions[0].valueSource;
        }
        var that = this;
        this._animatorListener = new android.animation.Animator.AnimatorListener({
            onAnimationStart: function (animator) {
                if (trace.enabled) {
                    trace.write("MainAnimatorListener.onAndroidAnimationStart(" + animator + ")", trace.categories.Animation);
                }
            },
            onAnimationRepeat: function (animator) {
                if (trace.enabled) {
                    trace.write("MainAnimatorListener.onAnimationRepeat(" + animator + ")", trace.categories.Animation);
                }
            },
            onAnimationEnd: function (animator) {
                if (trace.enabled) {
                    trace.write("MainAnimatorListener.onAnimationEnd(" + animator + ")", trace.categories.Animation);
                }
                that._onAndroidAnimationEnd();
            },
            onAnimationCancel: function (animator) {
                if (trace.enabled) {
                    trace.write("MainAnimatorListener.onAnimationCancel(" + animator + ")", trace.categories.Animation);
                }
                that._onAndroidAnimationCancel();
            }
        });
    }
    Animation.prototype.play = function () {
        var animationFinishedPromise = _super.prototype.play.call(this);
        this._animators = new Array();
        this._propertyUpdateCallbacks = new Array();
        this._propertyResetCallbacks = new Array();
        for (var i = 0, length_1 = this._propertyAnimations.length; i < length_1; i++) {
            this._createAnimators(this._propertyAnimations[i]);
        }
        this._nativeAnimatorsArray = Array.create(android.animation.Animator, this._animators.length);
        for (var i = 0, length_2 = this._animators.length; i < length_2; i++) {
            this._nativeAnimatorsArray[i] = this._animators[i];
        }
        this._animatorSet = new android.animation.AnimatorSet();
        this._animatorSet.addListener(this._animatorListener);
        if (this._animators.length > 0) {
            if (this._playSequentially) {
                this._animatorSet.playSequentially(this._nativeAnimatorsArray);
            }
            else {
                this._animatorSet.playTogether(this._nativeAnimatorsArray);
            }
        }
        this._enableHardwareAcceleration();
        if (trace.enabled) {
            trace.write("Starting " + this._nativeAnimatorsArray.length + " animations " + (this._playSequentially ? "sequentially." : "together."), trace.categories.Animation);
        }
        this._animatorSet.setupStartValues();
        this._animatorSet.start();
        return animationFinishedPromise;
    };
    Animation.prototype.cancel = function () {
        _super.prototype.cancel.call(this);
        if (trace.enabled) {
            trace.write("Cancelling AnimatorSet.", trace.categories.Animation);
        }
        this._animatorSet.cancel();
    };
    Animation.prototype._onAndroidAnimationEnd = function () {
        if (!this.isPlaying) {
            return;
        }
        var i = 0;
        var length = this._propertyUpdateCallbacks.length;
        for (; i < length; i++) {
            this._propertyUpdateCallbacks[i]();
        }
        this._disableHardwareAcceleration();
        this._resolveAnimationFinishedPromise();
    };
    Animation.prototype._onAndroidAnimationCancel = function () {
        var i = 0;
        var length = this._propertyResetCallbacks.length;
        for (; i < length; i++) {
            this._propertyResetCallbacks[i]();
        }
        this._disableHardwareAcceleration();
        this._rejectAnimationFinishedPromise();
    };
    Animation.prototype._createAnimators = function (propertyAnimation) {
        if (!propertyAnimation.target._nativeView) {
            return;
        }
        if (trace.enabled) {
            trace.write("Creating ObjectAnimator(s) for animation: " + common.Animation._getAnimationInfo(propertyAnimation) + "...", trace.categories.Animation);
        }
        if (types.isNullOrUndefined(propertyAnimation.target)) {
            throw new Error("Animation target cannot be null or undefined!");
        }
        if (types.isNullOrUndefined(propertyAnimation.property)) {
            throw new Error("Animation property cannot be null or undefined!");
        }
        if (types.isNullOrUndefined(propertyAnimation.value)) {
            throw new Error("Animation value cannot be null or undefined!");
        }
        var nativeArray;
        var nativeView = propertyAnimation.target._nativeView;
        var animators = new Array();
        var propertyUpdateCallbacks = new Array();
        var propertyResetCallbacks = new Array();
        var originalValue1;
        var originalValue2;
        var density = utils.layout.getDisplayDensity();
        var xyObjectAnimators;
        var animatorSet;
        var key = propertyKeys[propertyAnimation.property];
        if (key) {
            propertyAnimation.target[key] = propertyAnimation;
        }
        function checkAnimation(cb) {
            return function () {
                if (propertyAnimation.target[key] === propertyAnimation) {
                    delete propertyAnimation.target[key];
                    cb();
                }
            };
        }
        var valueSource = this._valueSource !== undefined ? this._valueSource : dependencyObservable.ValueSource.Local;
        switch (propertyAnimation.property) {
            case common.Properties.opacity:
                originalValue1 = nativeView.getAlpha();
                nativeArray = Array.create("float", 1);
                nativeArray[0] = propertyAnimation.value;
                propertyUpdateCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.opacityProperty, propertyAnimation.value, valueSource);
                }));
                propertyResetCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.opacityProperty, originalValue1, valueSource);
                }));
                animators.push(android.animation.ObjectAnimator.ofFloat(nativeView, "alpha", nativeArray));
                break;
            case common.Properties.backgroundColor:
                ensureArgbEvaluator();
                originalValue1 = propertyAnimation.target.backgroundColor;
                nativeArray = Array.create(java.lang.Object, 2);
                nativeArray[0] = propertyAnimation.target.backgroundColor ? java.lang.Integer.valueOf(propertyAnimation.target.backgroundColor.argb) : java.lang.Integer.valueOf(-1);
                nativeArray[1] = java.lang.Integer.valueOf(propertyAnimation.value.argb);
                var animator = android.animation.ValueAnimator.ofObject(argbEvaluator, nativeArray);
                animator.addUpdateListener(new android.animation.ValueAnimator.AnimatorUpdateListener({
                    onAnimationUpdate: function (animator) {
                        var argb = animator.getAnimatedValue().intValue();
                        propertyAnimation.target.style._setValue(styleModule.backgroundColorProperty, new color.Color(argb), valueSource);
                    }
                }));
                propertyUpdateCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.backgroundColorProperty, propertyAnimation.value, valueSource);
                }));
                propertyResetCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.backgroundColorProperty, originalValue1, valueSource);
                }));
                animators.push(animator);
                break;
            case common.Properties.translate:
                xyObjectAnimators = Array.create(android.animation.Animator, 2);
                nativeArray = Array.create("float", 1);
                nativeArray[0] = propertyAnimation.value.x * density;
                xyObjectAnimators[0] = android.animation.ObjectAnimator.ofFloat(nativeView, "translationX", nativeArray);
                xyObjectAnimators[0].setRepeatCount(Animation._getAndroidRepeatCount(propertyAnimation.iterations));
                nativeArray = Array.create("float", 1);
                nativeArray[0] = propertyAnimation.value.y * density;
                xyObjectAnimators[1] = android.animation.ObjectAnimator.ofFloat(nativeView, "translationY", nativeArray);
                xyObjectAnimators[1].setRepeatCount(Animation._getAndroidRepeatCount(propertyAnimation.iterations));
                originalValue1 = nativeView.getTranslationX();
                originalValue2 = nativeView.getTranslationY();
                propertyUpdateCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.translateXProperty, propertyAnimation.value.x, valueSource);
                    propertyAnimation.target.style._setValue(styleModule.translateYProperty, propertyAnimation.value.y, valueSource);
                }));
                propertyResetCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.translateXProperty, originalValue1, valueSource);
                    propertyAnimation.target.style._setValue(styleModule.translateYProperty, originalValue2, valueSource);
                }));
                animatorSet = new android.animation.AnimatorSet();
                animatorSet.playTogether(xyObjectAnimators);
                animatorSet.setupStartValues();
                animators.push(animatorSet);
                break;
            case common.Properties.scale:
                xyObjectAnimators = Array.create(android.animation.Animator, 2);
                nativeArray = Array.create("float", 1);
                nativeArray[0] = propertyAnimation.value.x;
                xyObjectAnimators[0] = android.animation.ObjectAnimator.ofFloat(nativeView, "scaleX", nativeArray);
                xyObjectAnimators[0].setRepeatCount(Animation._getAndroidRepeatCount(propertyAnimation.iterations));
                nativeArray = Array.create("float", 1);
                nativeArray[0] = propertyAnimation.value.y;
                xyObjectAnimators[1] = android.animation.ObjectAnimator.ofFloat(nativeView, "scaleY", nativeArray);
                xyObjectAnimators[1].setRepeatCount(Animation._getAndroidRepeatCount(propertyAnimation.iterations));
                originalValue1 = nativeView.getScaleX();
                originalValue2 = nativeView.getScaleY();
                propertyUpdateCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.scaleXProperty, propertyAnimation.value.x, valueSource);
                    propertyAnimation.target.style._setValue(styleModule.scaleYProperty, propertyAnimation.value.y, valueSource);
                }));
                propertyResetCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.scaleXProperty, originalValue1, valueSource);
                    propertyAnimation.target.style._setValue(styleModule.scaleYProperty, originalValue2, valueSource);
                }));
                animatorSet = new android.animation.AnimatorSet();
                animatorSet.playTogether(xyObjectAnimators);
                animatorSet.setupStartValues();
                animators.push(animatorSet);
                break;
            case common.Properties.rotate:
                originalValue1 = nativeView.getRotation();
                nativeArray = Array.create("float", 1);
                nativeArray[0] = propertyAnimation.value;
                propertyUpdateCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.rotateProperty, propertyAnimation.value, valueSource);
                }));
                propertyResetCallbacks.push(checkAnimation(function () {
                    propertyAnimation.target.style._setValue(styleModule.rotateProperty, originalValue1, valueSource);
                }));
                animators.push(android.animation.ObjectAnimator.ofFloat(nativeView, "rotation", nativeArray));
                break;
            default:
                throw new Error("Cannot animate " + propertyAnimation.property);
        }
        var i = 0;
        var length = animators.length;
        for (; i < length; i++) {
            if (propertyAnimation.duration !== undefined) {
                animators[i].setDuration(propertyAnimation.duration);
            }
            if (propertyAnimation.delay !== undefined) {
                animators[i].setStartDelay(propertyAnimation.delay);
            }
            if (propertyAnimation.iterations !== undefined && animators[i] instanceof android.animation.ValueAnimator) {
                animators[i].setRepeatCount(Animation._getAndroidRepeatCount(propertyAnimation.iterations));
            }
            if (propertyAnimation.curve !== undefined) {
                animators[i].setInterpolator(propertyAnimation.curve);
            }
            if (trace.enabled) {
                trace.write("Animator created: " + animators[i], trace.categories.Animation);
            }
        }
        this._animators = this._animators.concat(animators);
        this._propertyUpdateCallbacks = this._propertyUpdateCallbacks.concat(propertyUpdateCallbacks);
        this._propertyResetCallbacks = this._propertyResetCallbacks.concat(propertyResetCallbacks);
    };
    Animation._getAndroidRepeatCount = function (iterations) {
        return (iterations === Number.POSITIVE_INFINITY) ? android.view.animation.Animation.INFINITE : iterations - 1;
    };
    Animation.prototype._enableHardwareAcceleration = function () {
        for (var i = 0, length_3 = this._propertyAnimations.length; i < length_3; i++) {
            var cache = this._propertyAnimations[i].target._nativeView;
            if (cache) {
                var layerType = cache.getLayerType();
                if (layerType !== android.view.View.LAYER_TYPE_HARDWARE) {
                    cache.layerType = layerType;
                    cache.setLayerType(android.view.View.LAYER_TYPE_HARDWARE, null);
                }
            }
        }
    };
    Animation.prototype._disableHardwareAcceleration = function () {
        for (var i = 0, length_4 = this._propertyAnimations.length; i < length_4; i++) {
            var cache = this._propertyAnimations[i].target._nativeView;
            if (cache && cache.layerType !== undefined) {
                cache.setLayerType(cache.layerType, null);
                cache.layerType = undefined;
            }
        }
    };
    return Animation;
}(common.Animation));
exports.Animation = Animation;
var easeIn = lazy_1.default(function () { return new android.view.animation.AccelerateInterpolator(1); });
var easeOut = lazy_1.default(function () { return new android.view.animation.DecelerateInterpolator(1); });
var easeInOut = lazy_1.default(function () { return new android.view.animation.AccelerateDecelerateInterpolator(); });
var linear = lazy_1.default(function () { return new android.view.animation.LinearInterpolator(); });
var bounce = lazy_1.default(function () { return new android.view.animation.BounceInterpolator(); });
function _resolveAnimationCurve(curve) {
    switch (curve) {
        case enums.AnimationCurve.easeIn:
            if (trace.enabled) {
                trace.write("Animation curve resolved to android.view.animation.AccelerateInterpolator(1).", trace.categories.Animation);
            }
            return easeIn();
        case enums.AnimationCurve.easeOut:
            if (trace.enabled) {
                trace.write("Animation curve resolved to android.view.animation.DecelerateInterpolator(1).", trace.categories.Animation);
            }
            return easeOut();
        case enums.AnimationCurve.easeInOut:
            if (trace.enabled) {
                trace.write("Animation curve resolved to android.view.animation.AccelerateDecelerateInterpolator().", trace.categories.Animation);
            }
            return easeInOut();
        case enums.AnimationCurve.linear:
            if (trace.enabled) {
                trace.write("Animation curve resolved to android.view.animation.LinearInterpolator().", trace.categories.Animation);
            }
            return linear();
        case enums.AnimationCurve.spring:
            if (trace.enabled) {
                trace.write("Animation curve resolved to android.view.animation.BounceInterpolator().", trace.categories.Animation);
            }
            return bounce();
        case enums.AnimationCurve.ease:
            return android.support.v4.view.animation.PathInterpolatorCompat.create(0.25, 0.1, 0.25, 1.0);
        default:
            if (trace.enabled) {
                trace.write("Animation curve resolved to original: " + curve, trace.categories.Animation);
            }
            if (curve instanceof common.CubicBezierAnimationCurve) {
                var animationCurve = curve;
                var interpolator = android.support.v4.view.animation.PathInterpolatorCompat.create(animationCurve.x1, animationCurve.y1, animationCurve.x2, animationCurve.y2);
                return interpolator;
            }
            return curve;
    }
}
exports._resolveAnimationCurve = _resolveAnimationCurve;
