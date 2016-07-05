var transition_1 = require("ui/transition");
var FlipTransition = (function (_super) {
    __extends(FlipTransition, _super);
    function FlipTransition(direction, duration, curve) {
        _super.call(this, duration, curve);
        this._direction = direction;
    }
    FlipTransition.prototype.createAndroidAnimator = function (transitionType) {
        var objectAnimators;
        var values;
        var animator;
        var animatorSet = new android.animation.AnimatorSet();
        var fullDuration = this.getDuration() || 300;
        var interpolator = this.getCurve();
        var rotationY = this._direction === "right" ? 180 : -180;
        switch (transitionType) {
            case transition_1.AndroidTransitionType.enter:
                objectAnimators = Array.create(android.animation.Animator, 3);
                values = Array.create("float", 2);
                values[0] = 1.0;
                values[1] = 0.0;
                animator = android.animation.ObjectAnimator.ofFloat(null, "alpha", values);
                animator.setDuration(0);
                objectAnimators[0] = animator;
                values = Array.create("float", 2);
                values[0] = rotationY;
                values[1] = 0.0;
                animator = android.animation.ObjectAnimator.ofFloat(null, "rotationY", values);
                animator.setInterpolator(interpolator);
                animator.setDuration(fullDuration);
                objectAnimators[1] = animator;
                values = Array.create("float", 2);
                values[0] = 0.0;
                values[1] = 1.0;
                animator = android.animation.ObjectAnimator.ofFloat(null, "alpha", values);
                animator.setStartDelay(fullDuration / 2);
                animator.setDuration(1);
                objectAnimators[2] = animator;
                break;
            case transition_1.AndroidTransitionType.exit:
                objectAnimators = Array.create(android.animation.Animator, 2);
                values = Array.create("float", 2);
                values[0] = 0.0;
                values[1] = -rotationY;
                animator = android.animation.ObjectAnimator.ofFloat(null, "rotationY", values);
                animator.setInterpolator(interpolator);
                animator.setDuration(fullDuration);
                objectAnimators[0] = animator;
                values = Array.create("float", 2);
                values[0] = 1.0;
                values[1] = 0.0;
                animator = android.animation.ObjectAnimator.ofFloat(null, "alpha", values);
                animator.setStartDelay(fullDuration / 2);
                animator.setDuration(1);
                objectAnimators[1] = animator;
                break;
            case transition_1.AndroidTransitionType.popEnter:
                objectAnimators = Array.create(android.animation.Animator, 3);
                values = Array.create("float", 2);
                values[0] = 1.0;
                values[1] = 0.0;
                animator = android.animation.ObjectAnimator.ofFloat(null, "alpha", values);
                animator.setDuration(0);
                objectAnimators[0] = animator;
                values = Array.create("float", 2);
                values[0] = -rotationY;
                values[1] = 0.0;
                animator = android.animation.ObjectAnimator.ofFloat(null, "rotationY", values);
                animator.setInterpolator(interpolator);
                animator.setDuration(fullDuration);
                objectAnimators[1] = animator;
                values = Array.create("float", 2);
                values[0] = 0.0;
                values[1] = 1.0;
                animator = android.animation.ObjectAnimator.ofFloat(null, "alpha", values);
                animator.setStartDelay(fullDuration / 2);
                animator.setDuration(1);
                objectAnimators[2] = animator;
                break;
            case transition_1.AndroidTransitionType.popExit:
                objectAnimators = Array.create(android.animation.Animator, 2);
                values = Array.create("float", 2);
                values[0] = 0.0;
                values[1] = rotationY;
                animator = android.animation.ObjectAnimator.ofFloat(null, "rotationY", values);
                animator.setInterpolator(interpolator);
                animator.setDuration(fullDuration);
                objectAnimators[0] = animator;
                values = Array.create("float", 2);
                values[0] = 1.0;
                values[1] = 0.0;
                animator = android.animation.ObjectAnimator.ofFloat(null, "alpha", values);
                animator.setStartDelay(fullDuration / 2);
                animator.setDuration(1);
                objectAnimators[1] = animator;
                break;
        }
        animatorSet.playTogether(objectAnimators);
        return animatorSet;
    };
    return FlipTransition;
}(transition_1.Transition));
exports.FlipTransition = FlipTransition;
