var appModule = require("./application-common");
var observable = require("data/observable");
var enums;
global.moduleMerge(appModule, exports);
var typedExports = exports;
function initLifecycleCallbacks() {
    var lifecycleCallbacks = new android.app.Application.ActivityLifecycleCallbacks({
        onActivityCreated: function (activity, bundle) {
            if (!androidApp.startActivity) {
                androidApp.startActivity = activity;
                androidApp.notify({ eventName: "activityCreated", object: androidApp, activity: activity, bundle: bundle });
                if (androidApp.onActivityCreated) {
                    androidApp.onActivityCreated(activity, bundle);
                }
            }
            androidApp.currentContext = activity;
        },
        onActivityDestroyed: function (activity) {
            if (activity === androidApp.foregroundActivity) {
                androidApp.foregroundActivity = undefined;
            }
            if (activity === androidApp.currentContext) {
                androidApp.currentContext = undefined;
            }
            if (activity === androidApp.startActivity) {
                if (typedExports.onExit) {
                    typedExports.onExit();
                }
                typedExports.notify({ eventName: typedExports.exitEvent, object: androidApp, android: activity });
                androidApp.startActivity = undefined;
            }
            androidApp.notify({ eventName: "activityDestroyed", object: androidApp, activity: activity });
            if (androidApp.onActivityDestroyed) {
                androidApp.onActivityDestroyed(activity);
            }
            gc();
        },
        onActivityPaused: function (activity) {
            androidApp.paused = true;
            if (activity === androidApp.foregroundActivity) {
                if (typedExports.onSuspend) {
                    typedExports.onSuspend();
                }
                typedExports.notify({ eventName: typedExports.suspendEvent, object: androidApp, android: activity });
            }
            androidApp.notify({ eventName: "activityPaused", object: androidApp, activity: activity });
            if (androidApp.onActivityPaused) {
                androidApp.onActivityPaused(activity);
            }
        },
        onActivityResumed: function (activity) {
            androidApp.paused = false;
            androidApp.foregroundActivity = activity;
            if (typedExports.onResume) {
                typedExports.onResume();
            }
            typedExports.notify({ eventName: typedExports.resumeEvent, object: androidApp, android: activity });
            androidApp.notify({ eventName: "activityResumed", object: androidApp, activity: activity });
            if (androidApp.onActivityResumed) {
                androidApp.onActivityResumed(activity);
            }
        },
        onActivitySaveInstanceState: function (activity, bundle) {
            androidApp.notify({ eventName: "saveActivityState", object: androidApp, activity: activity, bundle: bundle });
            if (androidApp.onSaveActivityState) {
                androidApp.onSaveActivityState(activity, bundle);
            }
        },
        onActivityStarted: function (activity) {
            androidApp.foregroundActivity = activity;
            androidApp.notify({ eventName: "activityStarted", object: androidApp, activity: activity });
            if (androidApp.onActivityStarted) {
                androidApp.onActivityStarted(activity);
            }
        },
        onActivityStopped: function (activity) {
            androidApp.notify({ eventName: "activityStopped", object: androidApp, activity: activity });
            if (androidApp.onActivityStopped) {
                androidApp.onActivityStopped(activity);
            }
        }
    });
    return lifecycleCallbacks;
}
var currentOrientation;
function initComponentCallbacks() {
    var componentCallbacks = new android.content.ComponentCallbacks2({
        onLowMemory: function () {
            gc();
            java.lang.System.gc();
            typedExports.notify({ eventName: typedExports.lowMemoryEvent, object: this, android: this });
        },
        onTrimMemory: function (level) {
        },
        onConfigurationChanged: function (newConfig) {
            var newOrientation = newConfig.orientation;
            if (newOrientation === currentOrientation) {
                return;
            }
            if (!enums) {
                enums = require("ui/enums");
            }
            currentOrientation = newOrientation;
            var newValue;
            switch (newOrientation) {
                case android.content.res.Configuration.ORIENTATION_LANDSCAPE:
                    newValue = enums.DeviceOrientation.landscape;
                    break;
                case android.content.res.Configuration.ORIENTATION_PORTRAIT:
                    newValue = enums.DeviceOrientation.portrait;
                    break;
                default:
                    newValue = enums.DeviceOrientation.unknown;
                    break;
            }
            typedExports.notify({
                eventName: typedExports.orientationChangedEvent,
                android: androidApp.nativeApp,
                newValue: newValue,
                object: typedExports.android,
            });
        }
    });
    return componentCallbacks;
}
var AndroidApplication = (function (_super) {
    __extends(AndroidApplication, _super);
    function AndroidApplication() {
        _super.apply(this, arguments);
        this._registeredReceivers = {};
        this._pendingReceiverRegistrations = new Array();
    }
    AndroidApplication.prototype.init = function (nativeApp) {
        if (this.nativeApp) {
            throw new Error("application.android already initialized.");
        }
        this.nativeApp = nativeApp;
        this.packageName = nativeApp.getPackageName();
        this.context = nativeApp.getApplicationContext();
        var lifecycleCallbacks = initLifecycleCallbacks();
        var componentCallbacks = initComponentCallbacks();
        this.nativeApp.registerActivityLifecycleCallbacks(lifecycleCallbacks);
        this.nativeApp.registerComponentCallbacks(componentCallbacks);
        this._registerPendingReceivers();
    };
    AndroidApplication.prototype._registerPendingReceivers = function () {
        if (this._pendingReceiverRegistrations) {
            var i = 0;
            var length = this._pendingReceiverRegistrations.length;
            for (; i < length; i++) {
                var registerFunc = this._pendingReceiverRegistrations[i];
                registerFunc(this.context);
            }
            this._pendingReceiverRegistrations = new Array();
        }
    };
    AndroidApplication.prototype.registerBroadcastReceiver = function (intentFilter, onReceiveCallback) {
        ensureBroadCastReceiverClass();
        var that = this;
        var registerFunc = function (context) {
            var receiver = new BroadcastReceiverClass(onReceiveCallback);
            context.registerReceiver(receiver, new android.content.IntentFilter(intentFilter));
            that._registeredReceivers[intentFilter] = receiver;
        };
        if (this.context) {
            registerFunc(this.context);
        }
        else {
            this._pendingReceiverRegistrations.push(registerFunc);
        }
    };
    AndroidApplication.prototype.unregisterBroadcastReceiver = function (intentFilter) {
        var receiver = this._registeredReceivers[intentFilter];
        if (receiver) {
            this.context.unregisterReceiver(receiver);
            this._registeredReceivers[intentFilter] = undefined;
            delete this._registeredReceivers[intentFilter];
        }
    };
    AndroidApplication.activityCreatedEvent = "activityCreated";
    AndroidApplication.activityDestroyedEvent = "activityDestroyed";
    AndroidApplication.activityStartedEvent = "activityStarted";
    AndroidApplication.activityPausedEvent = "activityPaused";
    AndroidApplication.activityResumedEvent = "activityResumed";
    AndroidApplication.activityStoppedEvent = "activityStopped";
    AndroidApplication.saveActivityStateEvent = "saveActivityState";
    AndroidApplication.activityResultEvent = "activityResult";
    AndroidApplication.activityBackPressedEvent = "activityBackPressed";
    AndroidApplication.activityRequestPermissionsEvent = "activityRequestPermissions";
    return AndroidApplication;
}(observable.Observable));
exports.AndroidApplication = AndroidApplication;
var androidApp = new AndroidApplication();
typedExports.android = androidApp;
var BroadcastReceiverClass;
function ensureBroadCastReceiverClass() {
    if (BroadcastReceiverClass) {
        return;
    }
    var BroadcastReceiver = (function (_super) {
        __extends(BroadcastReceiver, _super);
        function BroadcastReceiver(onReceiveCallback) {
            _super.call(this);
            this._onReceiveCallback = onReceiveCallback;
            return global.__native(this);
        }
        BroadcastReceiver.prototype.onReceive = function (context, intent) {
            if (this._onReceiveCallback) {
                this._onReceiveCallback(context, intent);
            }
        };
        return BroadcastReceiver;
    }(android.content.BroadcastReceiver));
    BroadcastReceiverClass = BroadcastReceiver;
}
var started = false;
function start(entry) {
    if (started) {
        throw new Error("Application is already started.");
    }
    if (!androidApp.nativeApp) {
        var utils = require("utils/utils");
        var nativeApp = utils.ad.getApplication();
        androidApp.init(nativeApp);
    }
    started = true;
    if (entry) {
        typedExports.mainEntry = entry;
    }
    loadCss();
}
exports.start = start;
function loadCss() {
    typedExports.appSelectors = typedExports.loadCss(typedExports.cssFile) || [];
    if (typedExports.appSelectors.length > 0) {
        typedExports.mergeCssSelectors(typedExports);
    }
}
function addCss(cssText) {
    var parsed = typedExports.parseCss(cssText);
    if (parsed) {
        typedExports.additionalSelectors.push.apply(typedExports.additionalSelectors, parsed);
        typedExports.mergeCssSelectors(typedExports);
    }
}
exports.addCss = addCss;
global.__onLiveSync = function () {
    if (typedExports.android && typedExports.android.paused) {
        return;
    }
    appModule.__onLiveSync();
    loadCss();
};
global.__onUncaughtError = function (error) {
    var types = require("utils/types");
    if (types.isFunction(typedExports.onUncaughtError)) {
        typedExports.onUncaughtError(error);
    }
    typedExports.notify({ eventName: typedExports.uncaughtErrorEvent, object: appModule.android, android: error });
};
