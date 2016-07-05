var frameCommon = require("./frame-common");
var pages = require("ui/page");
var transitionModule = require("ui/transition");
var trace = require("trace");
var observable_1 = require("data/observable");
var application = require("application");
var types = require("utils/types");
global.moduleMerge(frameCommon, exports);
var HIDDEN = "_hidden";
var INTENT_EXTRA = "com.tns.activity";
var FRAMEID = "_frameId";
var navDepth = -1;
var fragmentId = -1;
var activityInitialized = false;
var CALLBACKS = "_callbacks";
function onFragmentShown(fragment) {
    if (trace.enabled) {
        trace.write("SHOWN " + fragment, trace.categories.NativeLifecycle);
    }
    var callbacks = fragment[CALLBACKS];
    if (callbacks.clearHistory) {
        if (trace.enabled) {
            trace.write(fragment + " has been shown, but it is being cleared from history. Returning.", trace.categories.NativeLifecycle);
        }
        return null;
    }
    var frame = callbacks.frame;
    var entry = callbacks.entry;
    var page = entry.resolvedPage;
    page._fragmentTag = entry.fragmentTag;
    var currentNavigationContext;
    var navigationQueue = frame._navigationQueue;
    for (var i = 0; i < navigationQueue.length; i++) {
        if (navigationQueue[i].entry === entry) {
            currentNavigationContext = navigationQueue[i];
            break;
        }
    }
    var isBack = currentNavigationContext ? currentNavigationContext.isBackNavigation : false;
    frame._addView(page);
    if (!frame.isLoaded) {
        frame._currentEntry = entry;
        frame.onLoaded();
    }
    transitionModule._onFragmentShown(fragment, isBack);
}
function onFragmentHidden(fragment, destroyed) {
    if (trace.enabled) {
        trace.write("HIDDEN " + fragment + "; destroyed: " + destroyed, trace.categories.NativeLifecycle);
    }
    var callbacks = fragment[CALLBACKS];
    var isBack = callbacks.entry.isBack;
    callbacks.entry.isBack = undefined;
    callbacks.entry.resolvedPage._fragmentTag = undefined;
    transitionModule._onFragmentHidden(fragment, isBack, destroyed);
}
var Frame = (function (_super) {
    __extends(Frame, _super);
    function Frame() {
        _super.call(this);
        this._containerViewId = -1;
        this._android = new AndroidFrame(this);
        this._listener = new android.view.View.OnAttachStateChangeListener({
            onViewAttachedToWindow: this.onNativeViewAttachedToWindow.bind(this),
            onViewDetachedFromWindow: this.onNativeViewDetachedToWindow.bind(this)
        });
    }
    Object.defineProperty(Frame, "defaultAnimatedNavigation", {
        get: function () {
            return frameCommon.Frame.defaultAnimatedNavigation;
        },
        set: function (value) {
            frameCommon.Frame.defaultAnimatedNavigation = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Frame, "defaultTransition", {
        get: function () {
            return frameCommon.Frame.defaultTransition;
        },
        set: function (value) {
            frameCommon.Frame.defaultTransition = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Frame.prototype, "containerViewId", {
        get: function () {
            return this._containerViewId;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Frame.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Frame.prototype, "_nativeView", {
        get: function () {
            return this._android.rootViewGroup;
        },
        enumerable: true,
        configurable: true
    });
    Frame.prototype._navigateCore = function (backstackEntry) {
        _super.prototype._navigateCore.call(this, backstackEntry);
        var activity = this._android.activity;
        if (!activity) {
            var currentActivity = this._android.currentActivity;
            if (currentActivity) {
                startActivity(currentActivity, this._android.frameId);
            }
            this._delayedNavigationEntry = backstackEntry;
            return;
        }
        var manager = activity.getFragmentManager();
        var currentFragment;
        if (this._currentEntry) {
            this._currentEntry.isNavigation = true;
            currentFragment = manager.findFragmentByTag(this._currentEntry.fragmentTag);
        }
        var clearHistory = backstackEntry.entry.clearHistory;
        if (clearHistory) {
            navDepth = -1;
        }
        navDepth++;
        fragmentId++;
        var newFragmentTag = "fragment" + fragmentId + "[" + navDepth + "]";
        ensureFragmentClass();
        var newFragment = new fragmentClass();
        var args = new android.os.Bundle();
        args.putInt(FRAMEID, this._android.frameId);
        newFragment.setArguments(args);
        setFragmentCallbacks(newFragment);
        var callbacks = newFragment[CALLBACKS];
        callbacks.frame = this;
        callbacks.entry = backstackEntry;
        backstackEntry.isNavigation = true;
        backstackEntry.fragmentTag = newFragmentTag;
        backstackEntry.navDepth = navDepth;
        var fragmentTransaction = manager.beginTransaction();
        if (trace.enabled) {
            trace.write("BEGIN TRANSACTION " + fragmentTransaction, trace.categories.Navigation);
        }
        var animated = this._getIsAnimatedNavigation(backstackEntry.entry);
        var navigationTransition = this._getNavigationTransition(backstackEntry.entry);
        if (currentFragment) {
            transitionModule._clearForwardTransitions(currentFragment);
            if (animated && navigationTransition) {
                transitionModule._setAndroidFragmentTransitions(navigationTransition, currentFragment, newFragment, fragmentTransaction);
            }
        }
        var length = manager.getBackStackEntryCount();
        var emptyNativeBackStack = clearHistory && length > 0;
        if (emptyNativeBackStack) {
            for (var i = 0; i < length; i++) {
                var fragmentToRemove = manager.findFragmentByTag(manager.getBackStackEntryAt(i).getName());
                Frame._clearHistory(fragmentToRemove);
            }
            if (currentFragment) {
                transitionModule._prepareCurrentFragmentForClearHistory(currentFragment);
            }
            var firstEntryName = manager.getBackStackEntryAt(0).getName();
            if (trace.enabled) {
                trace.write("POP BACK STACK " + firstEntryName, trace.categories.Navigation);
            }
            manager.popBackStackImmediate(firstEntryName, android.app.FragmentManager.POP_BACK_STACK_INCLUSIVE);
        }
        if (currentFragment && !emptyNativeBackStack) {
            if (this.android.cachePagesOnNavigate && !clearHistory) {
                if (trace.enabled) {
                    trace.write("\tHIDE " + currentFragment, trace.categories.Navigation);
                }
                fragmentTransaction.hide(currentFragment);
            }
            else {
                if (trace.enabled) {
                    trace.write("\tREMOVE " + currentFragment, trace.categories.Navigation);
                }
                fragmentTransaction.remove(currentFragment);
            }
        }
        if (trace.enabled) {
            trace.write("\tADD " + newFragmentTag + "<" + callbacks.entry.resolvedPage + ">", trace.categories.Navigation);
        }
        fragmentTransaction.add(this.containerViewId, newFragment, newFragmentTag);
        if (this.backStack.length > 0 && currentFragment && !clearHistory) {
            if (trace.enabled) {
                trace.write("\tADD TO BACK STACK " + currentFragment, trace.categories.Navigation);
            }
            fragmentTransaction.addToBackStack(this._currentEntry.fragmentTag);
        }
        if (currentFragment) {
            ensureAnimationFixed();
            var trans = void 0;
            if (this.android.cachePagesOnNavigate && animationFixed < 0 && !navigationTransition) {
                trans = android.app.FragmentTransaction.TRANSIT_NONE;
            }
            else {
                trans = animated ? android.app.FragmentTransaction.TRANSIT_FRAGMENT_OPEN : android.app.FragmentTransaction.TRANSIT_NONE;
            }
            if (trace.enabled) {
                trace.write("\tSET TRANSITION " + (trans === 0 ? "NONE" : "OPEN"), trace.categories.Navigation);
            }
            fragmentTransaction.setTransition(trans);
        }
        fragmentTransaction.commit();
        if (trace.enabled) {
            trace.write("END TRANSACTION " + fragmentTransaction, trace.categories.Navigation);
        }
    };
    Frame._clearHistory = function (fragment) {
        if (trace.enabled) {
            trace.write("CLEAR HISTORY FOR " + fragment, trace.categories.Navigation);
        }
        var callbacks = fragment[CALLBACKS];
        callbacks.clearHistory = true;
        transitionModule._clearBackwardTransitions(fragment);
        transitionModule._clearForwardTransitions(fragment);
        transitionModule._removePageNativeViewFromAndroidParent(callbacks.entry.resolvedPage);
    };
    Frame.prototype._goBackCore = function (backstackEntry) {
        _super.prototype._goBackCore.call(this, backstackEntry);
        navDepth = backstackEntry.navDepth;
        backstackEntry.isNavigation = true;
        if (this._currentEntry) {
            this._currentEntry.isBack = true;
            this._currentEntry.isNavigation = true;
        }
        var manager = this._android.activity.getFragmentManager();
        if (manager.getBackStackEntryCount() > 0) {
            manager.popBackStack(backstackEntry.fragmentTag, android.app.FragmentManager.POP_BACK_STACK_INCLUSIVE);
        }
    };
    Frame.prototype._createUI = function () {
        var root = new org.nativescript.widgets.ContentLayout(this._context);
        if (this._containerViewId < 0) {
            this._containerViewId = android.view.View.generateViewId();
        }
        this._android.rootViewGroup = root;
        this._android.rootViewGroup.setId(this._containerViewId);
        this._android.rootViewGroup.addOnAttachStateChangeListener(this._listener);
    };
    Frame.prototype.onNativeViewAttachedToWindow = function (view) {
        if (this._delayedNavigationEntry) {
            this._navigateCore(this._delayedNavigationEntry);
            this._delayedNavigationEntry = undefined;
        }
    };
    Frame.prototype.onNativeViewDetachedToWindow = function (view) {
    };
    Frame.prototype._popFromFrameStack = function () {
        if (!this._isInFrameStack) {
            return;
        }
        _super.prototype._popFromFrameStack.call(this);
        if (this._android.hasOwnActivity) {
            this._android.activity.finish();
        }
    };
    Frame.prototype._clearAndroidReference = function () {
        this._android.rootViewGroup.removeOnAttachStateChangeListener(this._listener);
        this._android.rootViewGroup = null;
    };
    Frame.prototype._printNativeBackStack = function () {
        if (!this._android.activity) {
            return;
        }
        var manager = this._android.activity.getFragmentManager();
        var length = manager.getBackStackEntryCount();
        var i = length - 1;
        console.log("Fragment Manager Back Stack: ");
        while (i >= 0) {
            var fragment = manager.findFragmentByTag(manager.getBackStackEntryAt(i--).getName());
            console.log("\t" + fragment);
        }
    };
    Frame.prototype._getNavBarVisible = function (page) {
        if (types.isDefined(page.actionBarHidden)) {
            return !page.actionBarHidden;
        }
        if (this._android && types.isDefined(this._android.showActionBar)) {
            return this._android.showActionBar;
        }
        return true;
    };
    Frame.prototype._processNavigationContext = function (navigationContext) {
        var _this = this;
        var activity = this._android.activity;
        if (activity) {
            var isForegroundActivity = activity === application.android.foregroundActivity;
            var isPaused = application.android.paused;
            if (activity && !isForegroundActivity || (isForegroundActivity && isPaused)) {
                var weakActivity_1 = new WeakRef(activity);
                var resume_1 = function (args) {
                    var weakActivityInstance = weakActivity_1.get();
                    var isCurrent = args.activity === weakActivityInstance;
                    if (!weakActivityInstance) {
                        if (trace.enabled) {
                            trace.write("Frame _processNavigationContext: Drop For Activity GC-ed", trace.categories.Navigation);
                        }
                        unsubscribe_1();
                        return;
                    }
                    if (isCurrent) {
                        if (trace.enabled) {
                            trace.write("Frame _processNavigationContext: Activity.Resumed, Continue", trace.categories.Navigation);
                        }
                        _super.prototype._processNavigationContext.call(_this, navigationContext);
                        unsubscribe_1();
                    }
                };
                var unsubscribe_1 = function () {
                    if (trace.enabled) {
                        trace.write("Frame _processNavigationContext: Unsubscribe from Activity.Resumed", trace.categories.Navigation);
                    }
                    application.android.off(application.AndroidApplication.activityResumedEvent, resume_1);
                    application.android.off(application.AndroidApplication.activityStoppedEvent, unsubscribe_1);
                    application.android.off(application.AndroidApplication.activityDestroyedEvent, unsubscribe_1);
                };
                if (trace.enabled) {
                    trace.write("Frame._processNavigationContext: Subscribe for Activity.Resumed", trace.categories.Navigation);
                }
                application.android.on(application.AndroidApplication.activityResumedEvent, resume_1);
                application.android.on(application.AndroidApplication.activityStoppedEvent, unsubscribe_1);
                application.android.on(application.AndroidApplication.activityDestroyedEvent, unsubscribe_1);
                return;
            }
        }
        _super.prototype._processNavigationContext.call(this, navigationContext);
    };
    return Frame;
}(frameCommon.Frame));
exports.Frame = Frame;
var framesCounter = 0;
var framesCache = new Array();
var AndroidFrame = (function (_super) {
    __extends(AndroidFrame, _super);
    function AndroidFrame(owner) {
        _super.call(this);
        this.hasOwnActivity = false;
        this._showActionBar = true;
        this._owner = owner;
        this.frameId = framesCounter++;
        framesCache.push(new WeakRef(this));
    }
    Object.defineProperty(AndroidFrame.prototype, "showActionBar", {
        get: function () {
            return this._showActionBar;
        },
        set: function (value) {
            if (this._showActionBar !== value) {
                this._showActionBar = value;
                if (this.owner.currentPage) {
                    this.owner.currentPage.actionBar.update();
                }
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(AndroidFrame.prototype, "activity", {
        get: function () {
            var activity = this.owner._context;
            if (activity) {
                return activity;
            }
            var currView = this._owner.parent;
            while (currView) {
                if (currView instanceof Frame) {
                    return currView.android.activity;
                }
                currView = currView.parent;
            }
            return undefined;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(AndroidFrame.prototype, "actionBar", {
        get: function () {
            var activity = this.currentActivity;
            if (!activity) {
                return undefined;
            }
            var bar = activity.getActionBar();
            if (!bar) {
                return undefined;
            }
            return bar;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(AndroidFrame.prototype, "currentActivity", {
        get: function () {
            var activity = this.activity;
            if (activity) {
                return activity;
            }
            var stack = frameCommon.stack(), length = stack.length, i = length - 1, frame;
            for (i; i >= 0; i--) {
                frame = stack[i];
                activity = frame.android.activity;
                if (activity) {
                    return activity;
                }
            }
            return undefined;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(AndroidFrame.prototype, "owner", {
        get: function () {
            return this._owner;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(AndroidFrame.prototype, "cachePagesOnNavigate", {
        get: function () {
            return this._cachePagesOnNavigate;
        },
        set: function (value) {
            if (this._cachePagesOnNavigate !== value) {
                if (this._owner.backStack.length > 0) {
                    this._owner._printFrameBackStack();
                    this._owner._printNativeBackStack();
                    console.log("currentPage: " + this._owner.currentPage);
                    throw new Error("Cannot set cachePagesOnNavigate if there are items in the back stack.");
                }
                this._cachePagesOnNavigate = value;
            }
        },
        enumerable: true,
        configurable: true
    });
    AndroidFrame.prototype.canGoBack = function () {
        if (!this.activity) {
            return false;
        }
        return this.activity.getIntent().getAction() !== android.content.Intent.ACTION_MAIN;
    };
    AndroidFrame.prototype.fragmentForPage = function (page) {
        if (!page) {
            return undefined;
        }
        var tag = page._fragmentTag;
        if (tag) {
            var manager = this.activity.getFragmentManager();
            return manager.findFragmentByTag(tag);
        }
        return undefined;
    };
    return AndroidFrame;
}(observable_1.Observable));
function findPageForFragment(fragment, frame) {
    var fragmentTag = fragment.getTag();
    var page;
    var entry;
    if (trace.enabled) {
        trace.write("Finding page for " + fragmentTag + ".", trace.categories.NativeLifecycle);
    }
    if (fragmentTag === pages.DIALOG_FRAGMENT_TAG) {
        if (trace.enabled) {
            trace.write("No need to find page for dialog fragment.", trace.categories.NativeLifecycle);
        }
        return;
    }
    if (frame._currentEntry && frame._currentEntry.fragmentTag === fragmentTag) {
        page = frame.currentPage;
        entry = frame._currentEntry;
        if (trace.enabled) {
            trace.write("Current page matches fragment " + fragmentTag + ".", trace.categories.NativeLifecycle);
        }
    }
    else {
        var backStack = frame.backStack;
        for (var i = 0; i < backStack.length; i++) {
            if (backStack[i].fragmentTag === fragmentTag) {
                entry = backStack[i];
                break;
            }
        }
        if (entry) {
            page = entry.resolvedPage;
            if (trace.enabled) {
                trace.write("Found " + page + " for " + fragmentTag, trace.categories.NativeLifecycle);
            }
        }
    }
    if (page) {
        var callbacks = fragment[CALLBACKS];
        callbacks.frame = frame;
        callbacks.entry = entry;
    }
    else {
        throw new Error("Could not find a page for " + fragmentTag + ".");
    }
}
function startActivity(activity, frameId) {
    var intent = new android.content.Intent(activity, activity.getClass());
    intent.setAction(android.content.Intent.ACTION_DEFAULT);
    intent.putExtra(INTENT_EXTRA, frameId);
    activity.startActivity(intent);
}
function getFrameById(frameId) {
    for (var i = 0; i < framesCache.length; i++) {
        var aliveFrame = framesCache[i].get();
        if (aliveFrame && aliveFrame.frameId === frameId) {
            return aliveFrame.owner;
        }
    }
    return null;
}
var animationFixed;
function ensureAnimationFixed() {
    if (!animationFixed) {
        animationFixed = android.os.Build.VERSION.SDK_INT >= 19 ? 1 : -1;
    }
}
function ensureFragmentClass() {
    if (fragmentClass) {
        return;
    }
    require("ui/frame/fragment");
    if (!fragmentClass) {
        throw new Error("Failed to initialize the extended android.app.Fragment class");
    }
}
var fragmentClass;
function setFragmentClass(clazz) {
    if (fragmentClass) {
        throw new Error("Fragment class already initialized");
    }
    fragmentClass = clazz;
}
exports.setFragmentClass = setFragmentClass;
var FragmentCallbacksImplementation = (function () {
    function FragmentCallbacksImplementation() {
    }
    FragmentCallbacksImplementation.prototype.onHiddenChanged = function (fragment, hidden, superFunc) {
        if (trace.enabled) {
            trace.write(fragment + ".onHiddenChanged(" + hidden + ")", trace.categories.NativeLifecycle);
        }
        superFunc.call(fragment, hidden);
        if (hidden) {
            onFragmentHidden(fragment, false);
        }
        else {
            onFragmentShown(fragment);
        }
    };
    FragmentCallbacksImplementation.prototype.onCreateAnimator = function (fragment, transit, enter, nextAnim, superFunc) {
        var nextAnimString;
        switch (nextAnim) {
            case -10:
                nextAnimString = "enter";
                break;
            case -20:
                nextAnimString = "exit";
                break;
            case -30:
                nextAnimString = "popEnter";
                break;
            case -40:
                nextAnimString = "popExit";
                break;
        }
        var animator = transitionModule._onFragmentCreateAnimator(fragment, nextAnim);
        if (!animator) {
            animator = superFunc.call(fragment, transit, enter, nextAnim);
        }
        if (trace.enabled) {
            trace.write(fragment + ".onCreateAnimator(" + transit + ", " + (enter ? "enter" : "exit") + ", " + nextAnimString + "): " + animator, trace.categories.NativeLifecycle);
        }
        return animator;
    };
    FragmentCallbacksImplementation.prototype.onCreate = function (fragment, savedInstanceState, superFunc) {
        if (trace.enabled) {
            trace.write(fragment + ".onCreate(" + savedInstanceState + ")", trace.categories.NativeLifecycle);
        }
        superFunc.call(fragment, savedInstanceState);
        if (!this.entry) {
            var frameId = fragment.getArguments().getInt(FRAMEID);
            var frame = getFrameById(frameId);
            if (frame) {
                this.frame = frame;
            }
            else {
                throw new Error("Cannot find Frame for " + fragment);
            }
            findPageForFragment(fragment, this.frame);
        }
    };
    FragmentCallbacksImplementation.prototype.onCreateView = function (fragment, inflater, container, savedInstanceState, superFunc) {
        if (trace.enabled) {
            trace.write(fragment + ".onCreateView(inflater, container, " + savedInstanceState + ")", trace.categories.NativeLifecycle);
        }
        var entry = this.entry;
        var page = entry.resolvedPage;
        if (savedInstanceState && savedInstanceState.getBoolean(HIDDEN, false)) {
            fragment.getFragmentManager().beginTransaction().hide(fragment).commit();
            page._onAttached(fragment.getActivity());
        }
        else {
            onFragmentShown(fragment);
        }
        return page._nativeView;
    };
    FragmentCallbacksImplementation.prototype.onSaveInstanceState = function (fragment, outState, superFunc) {
        if (trace.enabled) {
            trace.write(fragment + ".onSaveInstanceState(" + outState + ")", trace.categories.NativeLifecycle);
        }
        superFunc.call(fragment, outState);
        if (fragment.isHidden()) {
            outState.putBoolean(HIDDEN, true);
        }
    };
    FragmentCallbacksImplementation.prototype.onDestroyView = function (fragment, superFunc) {
        if (trace.enabled) {
            trace.write(fragment + ".onDestroyView()", trace.categories.NativeLifecycle);
        }
        superFunc.call(fragment);
        onFragmentHidden(fragment, true);
    };
    FragmentCallbacksImplementation.prototype.onDestroy = function (fragment, superFunc) {
        if (trace.enabled) {
            trace.write(fragment + ".onDestroy()", trace.categories.NativeLifecycle);
        }
        superFunc.call(fragment);
    };
    FragmentCallbacksImplementation.prototype.toStringOverride = function (fragment, superFunc) {
        return fragment.getTag() + "<" + (this.entry ? this.entry.resolvedPage : "") + ">";
    };
    return FragmentCallbacksImplementation;
}());
var ActivityCallbacksImplementation = (function () {
    function ActivityCallbacksImplementation() {
    }
    ActivityCallbacksImplementation.prototype.onCreate = function (activity, savedInstanceState, superFunc) {
        if (trace.enabled) {
            trace.write("Activity.onCreate(" + savedInstanceState + ")", trace.categories.NativeLifecycle);
        }
        var app = application.android;
        var intent = activity.getIntent();
        if (application.onLaunch) {
            application.onLaunch(intent);
        }
        var launchArgs = { eventName: application.launchEvent, object: app, android: intent };
        application.notify(launchArgs);
        var frameId = -1;
        var rootView = launchArgs.root;
        var extras = intent.getExtras();
        if (extras) {
            frameId = extras.getInt(INTENT_EXTRA, -1);
        }
        if (savedInstanceState && frameId < 0) {
            frameId = savedInstanceState.getInt(INTENT_EXTRA, -1);
        }
        var frame;
        var navParam;
        if (frameId >= 0) {
            rootView = getFrameById(frameId);
        }
        if (!rootView) {
            navParam = application.mainEntry;
            if (!navParam) {
                navParam = application.mainModule;
            }
            if (navParam) {
                frame = new Frame();
            }
            else {
                throw new Error("A Frame must be used to navigate to a Page.");
            }
            rootView = frame;
        }
        var isRestart = !!savedInstanceState && activityInitialized;
        superFunc.call(activity, isRestart ? savedInstanceState : null);
        this._rootView = rootView;
        rootView._onAttached(activity);
        activity.setContentView(rootView._nativeView, new org.nativescript.widgets.CommonLayoutParams());
        if (frame) {
            frame.navigate(navParam);
        }
        activityInitialized = true;
    };
    ActivityCallbacksImplementation.prototype.onSaveInstanceState = function (activity, outState, superFunc) {
        superFunc.call(activity, outState);
        var view = this._rootView;
        if (view instanceof Frame) {
            outState.putInt(INTENT_EXTRA, view.android.frameId);
        }
    };
    ActivityCallbacksImplementation.prototype.onStart = function (activity, superFunc) {
        superFunc.call(activity);
        if (trace.enabled) {
            trace.write("NativeScriptActivity.onStart();", trace.categories.NativeLifecycle);
        }
        var rootView = this._rootView;
        if (rootView && !rootView.isLoaded) {
            rootView.onLoaded();
        }
    };
    ActivityCallbacksImplementation.prototype.onStop = function (activity, superFunc) {
        superFunc.call(activity);
        if (trace.enabled) {
            trace.write("NativeScriptActivity.onStop();", trace.categories.NativeLifecycle);
        }
        var rootView = this._rootView;
        if (rootView && rootView.isLoaded) {
            rootView.onUnloaded();
        }
    };
    ActivityCallbacksImplementation.prototype.onDestroy = function (activity, superFunc) {
        var rootView = this._rootView;
        if (rootView && rootView._context) {
            rootView._onDetached(true);
        }
        superFunc.call(activity);
        if (trace.enabled) {
            trace.write("NativeScriptActivity.onDestroy();", trace.categories.NativeLifecycle);
        }
    };
    ActivityCallbacksImplementation.prototype.onBackPressed = function (activity, superFunc) {
        if (trace.enabled) {
            trace.write("NativeScriptActivity.onBackPressed;", trace.categories.NativeLifecycle);
        }
        var args = {
            eventName: "activityBackPressed",
            object: application.android,
            activity: activity,
            cancel: false,
        };
        application.android.notify(args);
        if (args.cancel) {
            return;
        }
        if (!frameCommon.goBack()) {
            superFunc.call(activity);
        }
    };
    ActivityCallbacksImplementation.prototype.onRequestPermissionsResult = function (activity, requestCode, permissions, grantResults, superFunc) {
        if (trace.enabled) {
            trace.write("NativeScriptActivity.onRequestPermissionsResult;", trace.categories.NativeLifecycle);
        }
        application.android.notify({
            eventName: "activityRequestPermissions",
            object: application.android,
            activity: activity,
            requestCode: requestCode,
            permissions: permissions,
            grantResults: grantResults
        });
    };
    ActivityCallbacksImplementation.prototype.onActivityResult = function (activity, requestCode, resultCode, data, superFunc) {
        superFunc.call(activity, requestCode, resultCode, data);
        if (trace.enabled) {
            trace.write("NativeScriptActivity.onActivityResult(" + requestCode + ", " + resultCode + ", " + data + ")", trace.categories.NativeLifecycle);
        }
        var result = application.android.onActivityResult;
        if (result) {
            result(requestCode, resultCode, data);
        }
        application.android.notify({
            eventName: "activityResult",
            object: application.android,
            activity: activity,
            requestCode: requestCode,
            resultCode: resultCode,
            intent: data
        });
    };
    return ActivityCallbacksImplementation;
}());
function setActivityCallbacks(activity) {
    activity[CALLBACKS] = new ActivityCallbacksImplementation();
}
exports.setActivityCallbacks = setActivityCallbacks;
function setFragmentCallbacks(fragment) {
    fragment[CALLBACKS] = new FragmentCallbacksImplementation();
}
exports.setFragmentCallbacks = setFragmentCallbacks;
