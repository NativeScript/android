var frame_1 = require("ui/frame");
var FragmentClass = (function (_super) {
    __extends(FragmentClass, _super);
    function FragmentClass() {
        _super.call(this);
        return global.__native(this);
    }
    FragmentClass.prototype.onHiddenChanged = function (hidden) {
        this._callbacks.onHiddenChanged(this, hidden, _super.prototype.onHiddenChanged);
    };
    FragmentClass.prototype.onCreateAnimator = function (transit, enter, nextAnim) {
        var result = this._callbacks.onCreateAnimator(this, transit, enter, nextAnim, _super.prototype.onCreateAnimator);
        return result;
    };
    FragmentClass.prototype.onCreate = function (savedInstanceState) {
        if (!this._callbacks) {
            frame_1.setFragmentCallbacks(this);
        }
        this.setHasOptionsMenu(true);
        this._callbacks.onCreate(this, savedInstanceState, _super.prototype.onCreate);
    };
    FragmentClass.prototype.onCreateView = function (inflater, container, savedInstanceState) {
        var result = this._callbacks.onCreateView(this, inflater, container, savedInstanceState, _super.prototype.onCreateView);
        return result;
    };
    FragmentClass.prototype.onSaveInstanceState = function (outState) {
        this._callbacks.onSaveInstanceState(this, outState, _super.prototype.onSaveInstanceState);
    };
    FragmentClass.prototype.onDestroyView = function () {
        this._callbacks.onDestroyView(this, _super.prototype.onDestroyView);
    };
    FragmentClass.prototype.onDestroy = function () {
        this._callbacks.onDestroy(this, _super.prototype.onDestroy);
    };
    FragmentClass.prototype.toString = function () {
        return this._callbacks.toStringOverride(this, _super.prototype.toString);
    };
    FragmentClass = __decorate([
        JavaProxy("com.tns.FragmentClass")
    ], FragmentClass);
    return FragmentClass;
}(android.app.Fragment));
frame_1.setFragmentClass(FragmentClass);
