var common = require("./utils-common");
var enums = require("ui/enums");
global.moduleMerge(common, exports);
var trace;
function ensureTrace() {
    if (!trace) {
        trace = require("trace");
    }
}
var layout;
(function (layout) {
    var density = -1;
    var metrics;
    var MODE_SHIFT = 30;
    var MODE_MASK = 0x3 << MODE_SHIFT;
    var sdkVersion = -1;
    var useOldMeasureSpec = false;
    function makeMeasureSpec(size, mode) {
        if (sdkVersion === -1) {
            sdkVersion = ad.getApplicationContext().getApplicationInfo().targetSdkVersion;
            useOldMeasureSpec = sdkVersion <= android.os.Build.VERSION_CODES.JELLY_BEAN_MR1;
        }
        if (useOldMeasureSpec) {
            return size + mode;
        }
        return (size & ~MODE_MASK) | (mode & MODE_MASK);
    }
    layout.makeMeasureSpec = makeMeasureSpec;
    function getDisplayMetrics() {
        if (!metrics) {
            metrics = ad.getApplicationContext().getResources().getDisplayMetrics();
        }
        return metrics;
    }
    layout.getDisplayMetrics = getDisplayMetrics;
    function getDisplayDensity() {
        if (density === -1) {
            density = getDisplayMetrics().density;
        }
        return density;
    }
    layout.getDisplayDensity = getDisplayDensity;
    function toDevicePixels(value) {
        return value * getDisplayDensity();
    }
    layout.toDevicePixels = toDevicePixels;
    function toDeviceIndependentPixels(value) {
        return value / getDisplayDensity();
    }
    layout.toDeviceIndependentPixels = toDeviceIndependentPixels;
})(layout = exports.layout || (exports.layout = {}));
var ad;
(function (ad) {
    function setTextDecoration(view, value) {
        var flags = 0;
        var values = (value + "").split(" ");
        if (values.indexOf(enums.TextDecoration.underline) !== -1) {
            flags = flags | android.graphics.Paint.UNDERLINE_TEXT_FLAG;
        }
        if (values.indexOf(enums.TextDecoration.lineThrough) !== -1) {
            flags = flags | android.graphics.Paint.STRIKE_THRU_TEXT_FLAG;
        }
        if (values.indexOf(enums.TextDecoration.none) === -1) {
            view.setPaintFlags(flags);
        }
        else {
            view.setPaintFlags(0);
        }
    }
    ad.setTextDecoration = setTextDecoration;
    function setTextTransform(v, value) {
        var view = v._nativeView;
        var str = view.getText() + "";
        var result = getTransformedString(value, view, str);
        if (v.formattedText) {
            for (var i = 0; i < v.formattedText.spans.length; i++) {
                var span = v.formattedText.spans.getItem(i);
                span.text = getTransformedString(value, view, span.text);
            }
        }
        else {
            view.setText(result);
        }
    }
    ad.setTextTransform = setTextTransform;
    function getTransformedString(textTransform, view, stringToTransform) {
        var result;
        switch (textTransform) {
            case enums.TextTransform.none:
            default:
                result = view["originalString"] || stringToTransform;
                if (view["transformationMethod"]) {
                    view.setTransformationMethod(view["transformationMethod"]);
                }
                break;
            case enums.TextTransform.uppercase:
                view.setTransformationMethod(null);
                result = stringToTransform.toUpperCase();
                break;
            case enums.TextTransform.lowercase:
                view.setTransformationMethod(null);
                result = stringToTransform.toLowerCase();
                break;
            case enums.TextTransform.capitalize:
                view.setTransformationMethod(null);
                result = getCapitalizedString(stringToTransform);
                break;
        }
        if (!view["originalString"]) {
            view["originalString"] = stringToTransform;
            view["transformationMethod"] = view.getTransformationMethod();
        }
        return result;
    }
    function getCapitalizedString(str) {
        var words = str.split(" ");
        var newWords = [];
        for (var i = 0; i < words.length; i++) {
            var word = words[i].toLowerCase();
            newWords.push(word.substr(0, 1).toUpperCase() + word.substring(1));
        }
        return newWords.join(" ");
    }
    function setWhiteSpace(view, value) {
        view.setSingleLine(value === enums.WhiteSpace.nowrap);
        view.setEllipsize(value === enums.WhiteSpace.nowrap ? android.text.TextUtils.TruncateAt.END : null);
    }
    ad.setWhiteSpace = setWhiteSpace;
    var nativeApp;
    function getApplication() {
        if (!nativeApp) {
            if (com.tns.NativeScriptApplication) {
                nativeApp = com.tns.NativeScriptApplication.getInstance();
            }
            if (!nativeApp) {
                var application = require("application");
                nativeApp = application.android.nativeApp;
                if (!nativeApp) {
                    var clazz = java.lang.Class.forName("android.app.ActivityThread");
                    if (clazz) {
                        var method = clazz.getMethod("currentApplication", null);
                        if (method) {
                            nativeApp = method.invoke(null, null);
                        }
                    }
                }
            }
            if (!nativeApp) {
                throw new Error("Failed to retrieve native Android Application object. If you have a custom android.app.Application type implemented make sure that you've called the '<application-module>.android.init' method.");
            }
        }
        return nativeApp;
    }
    ad.getApplication = getApplication;
    function getApplicationContext() {
        var app = getApplication();
        return app.getApplicationContext();
    }
    ad.getApplicationContext = getApplicationContext;
    var inputMethodManager;
    function getInputMethodManager() {
        if (!inputMethodManager) {
            inputMethodManager = getApplicationContext().getSystemService(android.content.Context.INPUT_METHOD_SERVICE);
        }
        return inputMethodManager;
    }
    ad.getInputMethodManager = getInputMethodManager;
    function showSoftInput(nativeView) {
        var imm = getInputMethodManager();
        if (imm && nativeView instanceof android.view.View) {
            imm.showSoftInput(nativeView, android.view.inputmethod.InputMethodManager.SHOW_IMPLICIT);
        }
    }
    ad.showSoftInput = showSoftInput;
    function dismissSoftInput(nativeView) {
        var imm = getInputMethodManager();
        if (imm && nativeView instanceof android.view.View) {
            imm.hideSoftInputFromWindow(nativeView.getWindowToken(), 0);
        }
    }
    ad.dismissSoftInput = dismissSoftInput;
    var collections;
    (function (collections) {
        function stringArrayToStringSet(str) {
            var hashSet = new java.util.HashSet();
            if ("undefined" !== typeof str) {
                for (var element in str) {
                    hashSet.add('' + str[element]);
                }
            }
            return hashSet;
        }
        collections.stringArrayToStringSet = stringArrayToStringSet;
        function stringSetToStringArray(stringSet) {
            var arr = [];
            if ("undefined" !== typeof stringSet) {
                var it = stringSet.iterator();
                while (it.hasNext()) {
                    var element = '' + it.next();
                    arr.push(element);
                }
            }
            return arr;
        }
        collections.stringSetToStringArray = stringSetToStringArray;
    })(collections = ad.collections || (ad.collections = {}));
    var resources;
    (function (resources_1) {
        var attr;
        var attrCache = new Map();
        function getDrawableId(name) {
            return getId(":drawable/" + name);
        }
        resources_1.getDrawableId = getDrawableId;
        function getStringId(name) {
            return getId(":string/" + name);
        }
        resources_1.getStringId = getStringId;
        function getId(name) {
            var resources = getApplicationContext().getResources();
            var packageName = getApplicationContext().getPackageName();
            var uri = packageName + name;
            return resources.getIdentifier(uri, null, null);
        }
        resources_1.getId = getId;
        function getPalleteColor(name, context) {
            if (attrCache.has(name)) {
                return attrCache.get(name);
            }
            var result = 0;
            try {
                if (!attr) {
                    attr = java.lang.Class.forName("android.support.v7.appcompat.R$attr");
                }
                var colorID = 0;
                var field = attr.getField(name);
                if (field) {
                    colorID = field.getInt(null);
                }
                if (colorID) {
                    var typedValue = new android.util.TypedValue();
                    context.getTheme().resolveAttribute(colorID, typedValue, true);
                    result = typedValue.data;
                }
            }
            catch (ex) {
                ensureTrace();
                trace.write("Cannot get pallete color: " + name, trace.categories.Error, trace.messageType.error);
            }
            attrCache.set(name, result);
            return result;
        }
        resources_1.getPalleteColor = getPalleteColor;
    })(resources = ad.resources || (ad.resources = {}));
})(ad = exports.ad || (exports.ad = {}));
function GC() {
    gc();
}
exports.GC = GC;
function openUrl(location) {
    var context = ad.getApplicationContext();
    try {
        var intent = new android.content.Intent(android.content.Intent.ACTION_VIEW, android.net.Uri.parse(location.trim()));
        intent.addFlags(android.content.Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
    }
    catch (e) {
        ensureTrace();
        trace.write("Error in OpenURL", trace.categories.Error, trace.messageType.error);
        return false;
    }
    return true;
}
exports.openUrl = openUrl;
