var transition = require("ui/transition");
var platform = require("platform");
var lazy_1 = require("utils/lazy");
var screenWidth = lazy_1.default(function () { return platform.screen.mainScreen.widthPixels; });
var screenHeight = lazy_1.default(function () { return platform.screen.mainScreen.heightPixels; });
var SlideTransition = (function (_super) {
    __extends(SlideTransition, _super);
    function SlideTransition(direction, duration, curve) {
        _super.call(this, duration, curve);
        this._direction = direction;
    }
    SlideTransition.prototype.createAndroidAnimator = function (transitionType) {
        var translationValues = Array.create("float", 2);
        switch (this._direction) {
            case "left":
                switch (transitionType) {
                    case transition.AndroidTransitionType.enter:
                        translationValues[0] = screenWidth();
                        translationValues[1] = 0;
                        break;
                    case transition.AndroidTransitionType.exit:
                        translationValues[0] = 0;
                        translationValues[1] = -screenWidth();
                        break;
                    case transition.AndroidTransitionType.popEnter:
                        translationValues[0] = -screenWidth();
                        translationValues[1] = 0;
                        break;
                    case transition.AndroidTransitionType.popExit:
                        translationValues[0] = 0;
                        translationValues[1] = screenWidth();
                        break;
                }
                break;
            case "right":
                switch (transitionType) {
                    case transition.AndroidTransitionType.enter:
                        translationValues[0] = -screenWidth();
                        translationValues[1] = 0;
                        break;
                    case transition.AndroidTransitionType.exit:
                        translationValues[0] = 0;
                        translationValues[1] = screenWidth();
                        break;
                    case transition.AndroidTransitionType.popEnter:
                        translationValues[0] = screenWidth();
                        translationValues[1] = 0;
                        break;
                    case transition.AndroidTransitionType.popExit:
                        translationValues[0] = 0;
                        translationValues[1] = -screenWidth();
                        break;
                }
                break;
            case "top":
                switch (transitionType) {
                    case transition.AndroidTransitionType.enter:
                        translationValues[0] = screenHeight();
                        translationValues[1] = 0;
                        break;
                    case transition.AndroidTransitionType.exit:
                        translationValues[0] = 0;
                        translationValues[1] = -screenHeight();
                        break;
                    case transition.AndroidTransitionType.popEnter:
                        translationValues[0] = -screenHeight();
                        translationValues[1] = 0;
                        break;
                    case transition.AndroidTransitionType.popExit:
                        translationValues[0] = 0;
                        translationValues[1] = screenHeight();
                        break;
                }
                break;
            case "bottom":
                switch (transitionType) {
                    case transition.AndroidTransitionType.enter:
                        translationValues[0] = -screenHeight();
                        translationValues[1] = 0;
                        break;
                    case transition.AndroidTransitionType.exit:
                        translationValues[0] = 0;
                        translationValues[1] = screenHeight();
                        break;
                    case transition.AndroidTransitionType.popEnter:
                        translationValues[0] = screenHeight();
                        translationValues[1] = 0;
                        break;
                    case transition.AndroidTransitionType.popExit:
                        translationValues[0] = 0;
                        translationValues[1] = -screenHeight();
                        break;
                }
                break;
        }
        var prop;
        if (this._direction === "left" || this._direction === "right") {
            prop = "translationX";
        }
        else {
            prop = "translationY";
        }
        var animator = android.animation.ObjectAnimator.ofFloat(null, prop, translationValues);
        var duration = this.getDuration();
        if (duration !== undefined) {
            animator.setDuration(duration);
        }
        animator.setInterpolator(this.getCurve());
        return animator;
    };
    SlideTransition.prototype.toString = function () {
        return _super.prototype.toString.call(this) + " " + this._direction;
    };
    return SlideTransition;
}(transition.Transition));
exports.SlideTransition = SlideTransition;
