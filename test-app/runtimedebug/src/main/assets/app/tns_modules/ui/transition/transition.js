var frame_1 = require("ui/frame");
var types_1 = require("utils/types");
var platform_1 = require("platform");
var lazy_1 = require("utils/lazy");
var trace = require("trace");
var slideTransition;
function ensureSlideTransition() {
    if (!slideTransition) {
        slideTransition = require("ui/transition/slide-transition");
    }
}
var fadeTransition;
function ensureFadeTransition() {
    if (!fadeTransition) {
        fadeTransition = require("ui/transition/fade-transition");
    }
}
var flipTransition;
function ensureFlipTransition() {
    if (!flipTransition) {
        flipTransition = require("ui/transition/flip-transition");
    }
}
var animation;
function ensureAnimationModule() {
    if (!animation) {
        animation = require("ui/animation");
    }
}
var _sdkVersion = lazy_1.default(function () { return parseInt(platform_1.device.sdkVersion); });
var _defaultInterpolator = lazy_1.default(function () { return new android.view.animation.AccelerateDecelerateInterpolator(); });
var enterFakeResourceId = -10;
var exitFakeResourceId = -20;
var popEnterFakeResourceId = -30;
var popExitFakeResourceId = -40;
var AndroidTransitionType;
(function (AndroidTransitionType) {
    AndroidTransitionType.enter = "enter";
    AndroidTransitionType.exit = "exit";
    AndroidTransitionType.popEnter = "popEnter";
    AndroidTransitionType.popExit = "popExit";
})(AndroidTransitionType = exports.AndroidTransitionType || (exports.AndroidTransitionType = {}));
function _clearBackwardTransitions(fragment) {
    var expandedFragment = fragment;
    if (expandedFragment.enterPopExitTransition) {
        if (trace.enabled) {
            trace.write("Cleared enterPopExitTransition " + expandedFragment.enterPopExitTransition + " for " + fragment, trace.categories.Transition);
        }
        expandedFragment.enterPopExitTransition = undefined;
    }
    if (_sdkVersion() >= 21) {
        var enterTransition = fragment.getEnterTransition();
        if (enterTransition) {
            if (trace.enabled) {
                trace.write("Cleared Enter " + enterTransition.getClass().getSimpleName() + " transition for " + fragment, trace.categories.Transition);
            }
            fragment.setEnterTransition(null);
        }
        var returnTransition = fragment.getReturnTransition();
        if (returnTransition) {
            if (trace.enabled) {
                trace.write("Cleared Pop Exit " + returnTransition.getClass().getSimpleName() + " transition for " + fragment, trace.categories.Transition);
            }
            fragment.setReturnTransition(null);
        }
    }
}
exports._clearBackwardTransitions = _clearBackwardTransitions;
function _clearForwardTransitions(fragment) {
    var expandedFragment = fragment;
    if (expandedFragment.exitPopEnterTransition) {
        if (trace.enabled) {
            trace.write("Cleared exitPopEnterTransition " + expandedFragment.exitPopEnterTransition + " for " + fragment, trace.categories.Transition);
        }
        expandedFragment.exitPopEnterTransition = undefined;
    }
    if (_sdkVersion() >= 21) {
        var exitTransition = fragment.getExitTransition();
        if (exitTransition) {
            if (trace.enabled) {
                trace.write("Cleared Exit " + exitTransition.getClass().getSimpleName() + " transition for " + fragment, trace.categories.Transition);
            }
            fragment.setExitTransition(null);
        }
        var reenterTransition = fragment.getReenterTransition();
        if (reenterTransition) {
            if (trace.enabled) {
                trace.write("Cleared Pop Enter " + reenterTransition.getClass().getSimpleName() + " transition for " + fragment, trace.categories.Transition);
            }
            fragment.setReenterTransition(null);
        }
    }
}
exports._clearForwardTransitions = _clearForwardTransitions;
function _setAndroidFragmentTransitions(navigationTransition, currentFragment, newFragment, fragmentTransaction) {
    trace.write("Setting Android Fragment Transitions...", trace.categories.Transition);
    var name;
    if (navigationTransition.name) {
        name = navigationTransition.name.toLowerCase();
    }
    var useLollipopTransition = name && (name.indexOf("slide") === 0 || name === "fade" || name === "explode") && _sdkVersion() >= 21;
    if (frame_1.topmost().android.cachePagesOnNavigate && _sdkVersion() === 23) {
        useLollipopTransition = false;
    }
    if (useLollipopTransition) {
        newFragment.setAllowEnterTransitionOverlap(true);
        newFragment.setAllowReturnTransitionOverlap(true);
        if (currentFragment) {
            currentFragment.setAllowEnterTransitionOverlap(true);
            currentFragment.setAllowReturnTransitionOverlap(true);
        }
        if (name.indexOf("slide") === 0) {
            var direction = name.substr("slide".length) || "left";
            switch (direction) {
                case "left":
                    var rightEdge = new android.transition.Slide(android.view.Gravity.RIGHT);
                    _setUpNativeTransition(navigationTransition, rightEdge);
                    _addNativeTransitionListener(newFragment, rightEdge);
                    newFragment.setEnterTransition(rightEdge);
                    if (currentFragment) {
                        var leftEdge_1 = new android.transition.Slide(android.view.Gravity.LEFT);
                        _setUpNativeTransition(navigationTransition, leftEdge_1);
                        _addNativeTransitionListener(currentFragment, leftEdge_1);
                        currentFragment.setExitTransition(leftEdge_1);
                    }
                    break;
                case "right":
                    var leftEdge = new android.transition.Slide(android.view.Gravity.LEFT);
                    _setUpNativeTransition(navigationTransition, leftEdge);
                    _addNativeTransitionListener(newFragment, leftEdge);
                    newFragment.setEnterTransition(leftEdge);
                    if (currentFragment) {
                        var rightEdge_1 = new android.transition.Slide(android.view.Gravity.RIGHT);
                        _setUpNativeTransition(navigationTransition, rightEdge_1);
                        _addNativeTransitionListener(currentFragment, rightEdge_1);
                        currentFragment.setExitTransition(rightEdge_1);
                    }
                    break;
                case "top":
                    var bottomEdge = new android.transition.Slide(android.view.Gravity.BOTTOM);
                    _setUpNativeTransition(navigationTransition, bottomEdge);
                    _addNativeTransitionListener(newFragment, bottomEdge);
                    newFragment.setEnterTransition(bottomEdge);
                    if (currentFragment) {
                        var topEdge_1 = new android.transition.Slide(android.view.Gravity.TOP);
                        _setUpNativeTransition(navigationTransition, topEdge_1);
                        _addNativeTransitionListener(currentFragment, topEdge_1);
                        currentFragment.setExitTransition(topEdge_1);
                    }
                    break;
                case "bottom":
                    var topEdge = new android.transition.Slide(android.view.Gravity.TOP);
                    _setUpNativeTransition(navigationTransition, topEdge);
                    _addNativeTransitionListener(newFragment, topEdge);
                    newFragment.setEnterTransition(topEdge);
                    if (currentFragment) {
                        var bottomEdge_1 = new android.transition.Slide(android.view.Gravity.BOTTOM);
                        _setUpNativeTransition(navigationTransition, bottomEdge_1);
                        _addNativeTransitionListener(currentFragment, bottomEdge_1);
                        currentFragment.setExitTransition(bottomEdge_1);
                    }
                    break;
            }
        }
        else if (name === "fade") {
            var fadeEnter = new android.transition.Fade(android.transition.Fade.IN);
            _setUpNativeTransition(navigationTransition, fadeEnter);
            _addNativeTransitionListener(newFragment, fadeEnter);
            newFragment.setEnterTransition(fadeEnter);
            var fadeReturn = new android.transition.Fade(android.transition.Fade.OUT);
            _setUpNativeTransition(navigationTransition, fadeReturn);
            _addNativeTransitionListener(newFragment, fadeReturn);
            newFragment.setReturnTransition(fadeReturn);
            if (currentFragment) {
                var fadeExit = new android.transition.Fade(android.transition.Fade.OUT);
                _setUpNativeTransition(navigationTransition, fadeExit);
                _addNativeTransitionListener(currentFragment, fadeExit);
                currentFragment.setExitTransition(fadeExit);
                var fadeReenter = new android.transition.Fade(android.transition.Fade.IN);
                _setUpNativeTransition(navigationTransition, fadeReenter);
                _addNativeTransitionListener(currentFragment, fadeReenter);
                currentFragment.setReenterTransition(fadeReenter);
            }
        }
        else if (name === "explode") {
            var explodeEnter = new android.transition.Explode();
            _setUpNativeTransition(navigationTransition, explodeEnter);
            _addNativeTransitionListener(newFragment, explodeEnter);
            newFragment.setEnterTransition(explodeEnter);
            if (currentFragment) {
                var explodeExit = new android.transition.Explode();
                _setUpNativeTransition(navigationTransition, explodeExit);
                _addNativeTransitionListener(currentFragment, explodeExit);
                currentFragment.setExitTransition(explodeExit);
            }
        }
    }
    else {
        var transition = void 0;
        if (name) {
            if (name.indexOf("slide") === 0) {
                var direction = name.substr("slide".length) || "left";
                ensureSlideTransition();
                transition = new slideTransition.SlideTransition(direction, navigationTransition.duration, navigationTransition.curve);
            }
            else if (name === "fade") {
                ensureFadeTransition();
                transition = new fadeTransition.FadeTransition(navigationTransition.duration, navigationTransition.curve);
            }
            else if (name.indexOf("flip") === 0) {
                var direction = name.substr("flip".length) || "right";
                ensureFlipTransition();
                transition = new flipTransition.FlipTransition(direction, navigationTransition.duration, navigationTransition.curve);
            }
        }
        else {
            transition = navigationTransition.instance;
        }
        if (transition) {
            var newExpandedFragment = newFragment;
            newExpandedFragment.enterPopExitTransition = transition;
            if (currentFragment) {
                var currentExpandedFragment = currentFragment;
                currentExpandedFragment.exitPopEnterTransition = transition;
            }
            fragmentTransaction.setCustomAnimations(enterFakeResourceId, exitFakeResourceId, popEnterFakeResourceId, popExitFakeResourceId);
        }
    }
    _printTransitions(currentFragment);
    _printTransitions(newFragment);
}
exports._setAndroidFragmentTransitions = _setAndroidFragmentTransitions;
function _setUpNativeTransition(navigationTransition, nativeTransition) {
    if (navigationTransition.duration) {
        nativeTransition.setDuration(navigationTransition.duration);
    }
    if (navigationTransition.curve) {
        ensureAnimationModule();
        var interpolator = animation._resolveAnimationCurve(navigationTransition.curve);
        nativeTransition.setInterpolator(interpolator);
    }
    else {
        nativeTransition.setInterpolator(_defaultInterpolator());
    }
}
function _onFragmentShown(fragment, isBack) {
    if (trace.enabled) {
        trace.write("_onFragmentShown(" + fragment + ", isBack: " + isBack + ")", trace.categories.Transition);
    }
    var expandedFragment = fragment;
    var transitionType = isBack ? "Pop Enter" : "Enter";
    var relevantTransition = isBack ? expandedFragment.exitPopEnterTransition : expandedFragment.enterPopExitTransition;
    if (relevantTransition) {
        if (trace.enabled) {
            trace.write(fragment + " has been shown when going " + (isBack ? "back" : "forward") + ", but there is " + transitionType + " " + relevantTransition + ". Will complete page addition when transition ends.", trace.categories.Transition);
        }
        expandedFragment.completePageAdditionWhenTransitionEnds = { isBack: isBack };
    }
    else if (_sdkVersion() >= 21) {
        var nativeTransition = isBack ? fragment.getReenterTransition() : fragment.getEnterTransition();
        if (nativeTransition) {
            if (trace.enabled) {
                trace.write(fragment + " has been shown when going " + (isBack ? "back" : "forward") + ", but there is " + transitionType + " " + nativeTransition.getClass().getSimpleName() + " transition. Will complete page addition when transition ends.", trace.categories.Transition);
            }
            expandedFragment.completePageAdditionWhenTransitionEnds = { isBack: isBack };
        }
    }
    if (!expandedFragment.completePageAdditionWhenTransitionEnds) {
        _completePageAddition(fragment, isBack);
    }
}
exports._onFragmentShown = _onFragmentShown;
function _onFragmentHidden(fragment, isBack, destroyed) {
    if (trace.enabled) {
        trace.write("_onFragmentHidden(" + fragment + ", isBack: " + isBack + ", destroyed: " + destroyed + ")", trace.categories.Transition);
    }
    var expandedFragment = fragment;
    var transitionType = isBack ? "Pop Exit" : "Exit";
    var relevantTransition = isBack ? expandedFragment.enterPopExitTransition : expandedFragment.exitPopEnterTransition;
    if (relevantTransition) {
        if (trace.enabled) {
            trace.write(fragment + " has been hidden when going " + (isBack ? "back" : "forward") + ", but there is " + transitionType + " " + relevantTransition + ". Will complete page removal when transition ends.", trace.categories.Transition);
        }
        expandedFragment.completePageRemovalWhenTransitionEnds = { isBack: isBack };
    }
    else if (_sdkVersion() >= 21) {
        var nativeTransition = isBack ? fragment.getReturnTransition() : fragment.getExitTransition();
        if (nativeTransition) {
            if (trace.enabled) {
                trace.write(fragment + " has been hidden when going " + (isBack ? "back" : "forward") + ", but there is " + transitionType + " " + nativeTransition.getClass().getSimpleName() + " transition. Will complete page removal when transition ends.", trace.categories.Transition);
            }
            expandedFragment.completePageRemovalWhenTransitionEnds = { isBack: isBack };
        }
    }
    expandedFragment.isDestroyed = destroyed;
    if (expandedFragment.completePageRemovalWhenTransitionEnds === undefined) {
        _completePageRemoval(fragment, isBack);
    }
}
exports._onFragmentHidden = _onFragmentHidden;
function _completePageAddition(fragment, isBack) {
    var expandedFragment = fragment;
    expandedFragment.completePageAdditionWhenTransitionEnds = undefined;
    var frame = fragment._callbacks.frame;
    var entry = fragment._callbacks.entry;
    var page = entry.resolvedPage;
    if (trace.enabled) {
        trace.write("STARTING ADDITION of " + page + "...", trace.categories.Transition);
    }
    frame._currentEntry = entry;
    page.onNavigatedTo(isBack);
    frame._processNavigationQueue(page);
    entry.isNavigation = undefined;
    if (trace.enabled) {
        trace.write("ADDITION of " + page + " completed", trace.categories.Transition);
    }
}
function _completePageRemoval(fragment, isBack) {
    var expandedFragment = fragment;
    expandedFragment.completePageRemovalWhenTransitionEnds = undefined;
    var frame = fragment._callbacks.frame;
    var entry = fragment._callbacks.entry;
    var page = entry.resolvedPage;
    if (trace.enabled) {
        trace.write("STARTING REMOVAL of " + page + "...", trace.categories.Transition);
    }
    if (page.frame) {
        frame._removeView(page);
        if (entry.isNavigation) {
            page.onNavigatedFrom(isBack);
        }
        if (trace.enabled) {
            trace.write("REMOVAL of " + page + " completed", trace.categories.Transition);
        }
    }
    else {
        if (trace.enabled) {
            trace.write("REMOVAL of " + page + " has already been done", trace.categories.Transition);
        }
    }
    if (expandedFragment.isDestroyed) {
        expandedFragment.isDestroyed = undefined;
        if (page._context) {
            page._onDetached(true);
            if (trace.enabled) {
                trace.write("DETACHMENT of " + page + " completed", trace.categories.Transition);
            }
        }
        else {
            if (trace.enabled) {
                trace.write("DETACHMENT of " + page + " has already been done", trace.categories.Transition);
            }
            _removePageNativeViewFromAndroidParent(page);
        }
    }
    entry.isNavigation = undefined;
}
function _removePageNativeViewFromAndroidParent(page) {
    if (page._nativeView && page._nativeView.getParent) {
        var androidParent = page._nativeView.getParent();
        if (androidParent && androidParent.removeView) {
            if (trace.enabled) {
                trace.write("REMOVED " + page + "._nativeView from its Android parent", trace.categories.Transition);
            }
            page._onDetached(true);
            androidParent.removeView(page._nativeView);
        }
    }
}
exports._removePageNativeViewFromAndroidParent = _removePageNativeViewFromAndroidParent;
function _toShortString(nativeTransition) {
    return nativeTransition.getClass().getSimpleName() + "@" + nativeTransition.hashCode().toString(16);
}
function _addNativeTransitionListener(fragment, nativeTransition) {
    var expandedFragment = fragment;
    var transitionListener = new android.transition.Transition.TransitionListener({
        onTransitionCancel: function (transition) {
            if (trace.enabled) {
                trace.write("CANCEL " + _toShortString(nativeTransition) + " transition for " + fragment, trace.categories.Transition);
            }
            if (expandedFragment.completePageRemovalWhenTransitionEnds) {
                _completePageRemoval(fragment, expandedFragment.completePageRemovalWhenTransitionEnds.isBack);
            }
            if (expandedFragment.completePageAdditionWhenTransitionEnds) {
                _completePageAddition(fragment, expandedFragment.completePageAdditionWhenTransitionEnds.isBack);
            }
        },
        onTransitionEnd: function (transition) {
            if (trace.enabled) {
                trace.write("END " + _toShortString(nativeTransition) + " transition for " + fragment, trace.categories.Transition);
            }
            if (expandedFragment.completePageRemovalWhenTransitionEnds) {
                _completePageRemoval(fragment, expandedFragment.completePageRemovalWhenTransitionEnds.isBack);
            }
            if (expandedFragment.completePageAdditionWhenTransitionEnds) {
                _completePageAddition(fragment, expandedFragment.completePageAdditionWhenTransitionEnds.isBack);
            }
        },
        onTransitionPause: function (transition) {
            if (trace.enabled) {
                trace.write("PAUSE " + _toShortString(nativeTransition) + " transition for " + fragment, trace.categories.Transition);
            }
        },
        onTransitionResume: function (transition) {
            if (trace.enabled) {
                trace.write("RESUME " + _toShortString(nativeTransition) + " transition for " + fragment, trace.categories.Transition);
            }
        },
        onTransitionStart: function (transition) {
            if (trace.enabled) {
                trace.write("START " + _toShortString(nativeTransition) + " transition for " + fragment, trace.categories.Transition);
            }
        }
    });
    nativeTransition.addListener(transitionListener);
}
function _onFragmentCreateAnimator(fragment, nextAnim) {
    var expandedFragment = fragment;
    var transitionType;
    switch (nextAnim) {
        case enterFakeResourceId:
            transitionType = AndroidTransitionType.enter;
            break;
        case exitFakeResourceId:
            transitionType = AndroidTransitionType.exit;
            break;
        case popEnterFakeResourceId:
            transitionType = AndroidTransitionType.popEnter;
            break;
        case popExitFakeResourceId:
            transitionType = AndroidTransitionType.popExit;
            break;
    }
    if ((nextAnim === popExitFakeResourceId || !nextAnim) && expandedFragment.exitHack) {
        trace.write("HACK EXIT FOR " + fragment, trace.categories.Transition);
        transitionType = AndroidTransitionType.exit;
    }
    var transition;
    switch (transitionType) {
        case AndroidTransitionType.enter:
        case AndroidTransitionType.popExit:
            transition = expandedFragment.enterPopExitTransition;
            break;
        case AndroidTransitionType.exit:
        case AndroidTransitionType.popEnter:
            transition = expandedFragment.exitPopEnterTransition;
            break;
    }
    var animator;
    if (transition) {
        animator = transition.createAndroidAnimator(transitionType);
        trace.write(transition + ".createAndroidAnimator(" + transitionType + "): " + animator, trace.categories.Transition);
        var transitionListener = new android.animation.Animator.AnimatorListener({
            onAnimationStart: function (animator) {
                if (trace.enabled) {
                    trace.write("START " + transitionType + " " + transition + " for " + fragment, trace.categories.Transition);
                }
            },
            onAnimationRepeat: function (animator) {
                if (trace.enabled) {
                    trace.write("REPEAT " + transitionType + " " + transition + " for " + fragment, trace.categories.Transition);
                }
            },
            onAnimationEnd: function (animator) {
                if (trace.enabled) {
                    trace.write("END " + transitionType + " " + transition + " for " + fragment, trace.categories.Transition);
                }
                if (expandedFragment.completePageRemovalWhenTransitionEnds) {
                    _completePageRemoval(fragment, expandedFragment.completePageRemovalWhenTransitionEnds.isBack);
                }
                if (expandedFragment.completePageAdditionWhenTransitionEnds) {
                    _completePageAddition(fragment, expandedFragment.completePageAdditionWhenTransitionEnds.isBack);
                }
            },
            onAnimationCancel: function (animator) {
                if (trace.enabled) {
                    trace.write("CANCEL " + transitionType + " " + transition + " for " + fragment, trace.categories.Transition);
                }
                if (expandedFragment.completePageRemovalWhenTransitionEnds) {
                    _completePageRemoval(fragment, expandedFragment.completePageRemovalWhenTransitionEnds.isBack);
                }
                if (expandedFragment.completePageAdditionWhenTransitionEnds) {
                    _completePageAddition(fragment, expandedFragment.completePageAdditionWhenTransitionEnds.isBack);
                }
            }
        });
        animator.addListener(transitionListener);
    }
    if (transitionType && !animator) {
        animator = _createDummyZeroDurationAnimator();
    }
    return animator;
}
exports._onFragmentCreateAnimator = _onFragmentCreateAnimator;
function _prepareCurrentFragmentForClearHistory(fragment) {
    trace.write("Preparing " + fragment + " transitions fro clear history...", trace.categories.Transition);
    var expandedFragment = fragment;
    expandedFragment.exitHack = true;
    if (_sdkVersion() >= 21) {
        var exitTransition = fragment.getExitTransition();
        fragment.setReturnTransition(exitTransition);
    }
    _printTransitions(fragment);
}
exports._prepareCurrentFragmentForClearHistory = _prepareCurrentFragmentForClearHistory;
var intEvaluator;
function ensureIntEvaluator() {
    if (!intEvaluator) {
        intEvaluator = new android.animation.IntEvaluator();
    }
}
function _createDummyZeroDurationAnimator() {
    if (trace.enabled) {
        trace.write("_createDummyZeroDurationAnimator()", trace.categories.Transition);
    }
    ensureIntEvaluator();
    var nativeArray = Array.create(java.lang.Object, 2);
    nativeArray[0] = java.lang.Integer.valueOf(0);
    nativeArray[1] = java.lang.Integer.valueOf(1);
    var animator = android.animation.ValueAnimator.ofObject(intEvaluator, nativeArray);
    animator.setDuration(0);
    return animator;
}
function _printTransitions(f) {
    if (f && trace.enabled) {
        var ef = f;
        var result = ef + " Transitions:";
        result += "" + (ef.enterPopExitTransition ? " enterPopExit=" + ef.enterPopExitTransition : "");
        result += "" + (ef.exitPopEnterTransition ? " exitPopEnter=" + ef.exitPopEnterTransition : "");
        if (_sdkVersion() >= 21) {
            result += "" + (f.getEnterTransition() ? " enter=" + _toShortString(f.getEnterTransition()) : "");
            result += "" + (f.getExitTransition() ? " exit=" + _toShortString(f.getExitTransition()) : "");
            result += "" + (f.getReenterTransition() ? " popEnter=" + _toShortString(f.getReenterTransition()) : "");
            result += "" + (f.getReturnTransition() ? " popExit=" + _toShortString(f.getReturnTransition()) : "");
        }
        trace.write(result, trace.categories.Transition);
    }
}
var Transition = (function () {
    function Transition(duration, curve) {
        this._duration = duration;
        if (curve) {
            var animation_1 = require("ui/animation");
            this._interpolator = animation_1._resolveAnimationCurve(curve);
        }
        else {
            this._interpolator = _defaultInterpolator();
        }
        this._id = Transition.transitionId++;
    }
    Transition.prototype.getDuration = function () {
        return this._duration;
    };
    Transition.prototype.getCurve = function () {
        return this._interpolator;
    };
    Transition.prototype.animateIOSTransition = function (containerView, fromView, toView, operation, completion) {
        throw new Error("Abstract method call");
    };
    Transition.prototype.createAndroidAnimator = function (transitionType) {
        throw new Error("Abstract method call");
    };
    Transition.prototype.toString = function () {
        return types_1.getClass(this) + "@" + this._id;
    };
    Transition.transitionId = 0;
    return Transition;
}());
exports.Transition = Transition;
