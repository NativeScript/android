var common = require("./search-bar-common");
var color = require("color");
var utils = require("utils/utils");
var style = require("ui/styling/style");
var types;
function ensureTypes() {
    if (!types) {
        types = require("utils/types");
    }
}
var SEARCHTEXT = "searchText";
var QUERY = "query";
var EMPTY = "";
function onTextPropertyChanged(data) {
    var bar = data.object;
    if (!bar.android) {
        return;
    }
    bar.android.setQuery(data.newValue, false);
}
common.SearchBar.textProperty.metadata.onSetNativeValue = onTextPropertyChanged;
function onTextFieldBackgroundColorPropertyChanged(data) {
    var bar = data.object;
    if (!bar.android) {
        return;
    }
    if (data.newValue instanceof color.Color) {
        _changeSearchViewBackgroundColor(bar.android, data.newValue.android);
    }
}
common.SearchBar.textFieldBackgroundColorProperty.metadata.onSetNativeValue = onTextFieldBackgroundColorPropertyChanged;
function onTextFieldHintColorPropertyChanged(data) {
    var bar = data.object;
    if (!bar.android) {
        return;
    }
    if (data.newValue instanceof color.Color) {
        _changeSearchViewHintColor(bar.android, data.newValue.android);
    }
}
common.SearchBar.textFieldHintColorProperty.metadata.onSetNativeValue = onTextFieldHintColorPropertyChanged;
function onHintPropertyChanged(data) {
    var bar = data.object;
    if (!bar.android) {
        return;
    }
    var newValue = data.newValue;
    ensureTypes();
    if (types.isString(newValue)) {
        bar.android.setQueryHint(newValue);
    }
}
common.SearchBar.hintProperty.metadata.onSetNativeValue = onHintPropertyChanged;
function getTextView(bar) {
    if (bar) {
        var id = bar.getContext().getResources().getIdentifier("android:id/search_src_text", null, null);
        if (id) {
            return bar.findViewById(id);
        }
    }
    return undefined;
}
function _changeSearchViewBackgroundColor(bar, color) {
    var textView = getTextView(bar);
    if (textView) {
        textView.setBackgroundColor(color);
    }
}
function _changeSearchViewHintColor(bar, color) {
    var textView = getTextView(bar);
    if (textView) {
        textView.setHintTextColor(color);
    }
}
global.moduleMerge(common, exports);
var SearchBar = (function (_super) {
    __extends(SearchBar, _super);
    function SearchBar() {
        _super.apply(this, arguments);
    }
    SearchBar.prototype.dismissSoftInput = function () {
        utils.ad.dismissSoftInput(this._nativeView);
    };
    SearchBar.prototype.focus = function () {
        var result = _super.prototype.focus.call(this);
        if (result) {
            utils.ad.showSoftInput(this._nativeView);
        }
        return result;
    };
    SearchBar.prototype._createUI = function () {
        this._android = new android.widget.SearchView(this._context);
        this._android.setIconified(false);
        var that = new WeakRef(this);
        this._android.setOnQueryTextListener(new android.widget.SearchView.OnQueryTextListener({
            get owner() {
                return that.get();
            },
            onQueryTextChange: function (newText) {
                if (this.owner) {
                    this.owner._onPropertyChangedFromNative(common.SearchBar.textProperty, newText);
                    if (newText === EMPTY && this[SEARCHTEXT] !== newText) {
                        this.owner._emit(common.SearchBar.clearEvent);
                    }
                    this[SEARCHTEXT] = newText;
                }
                return true;
            },
            onQueryTextSubmit: function (query) {
                if (this.owner) {
                    if (query !== EMPTY && this[QUERY] !== query) {
                        this.owner._emit(common.SearchBar.submitEvent);
                    }
                    this[QUERY] = query;
                }
                return true;
            }
        }));
        this._android.setOnCloseListener(new android.widget.SearchView.OnCloseListener({
            get owner() {
                return that.get();
            },
            onClose: function () {
                if (this.owner) {
                    this.owner._emit(common.SearchBar.clearEvent);
                }
                return true;
            }
        }));
        if (this.textFieldBackgroundColor instanceof color.Color) {
            _changeSearchViewBackgroundColor(this._android, this.textFieldBackgroundColor.android);
        }
        if (this.textFieldHintColor instanceof color.Color) {
            _changeSearchViewHintColor(this._android, this.textFieldHintColor.android);
        }
    };
    Object.defineProperty(SearchBar.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    return SearchBar;
}(common.SearchBar));
exports.SearchBar = SearchBar;
var SearchBarStyler = (function () {
    function SearchBarStyler() {
    }
    SearchBarStyler.getBackgroundColorProperty = function (v) {
        var bar = v._nativeView;
        return bar.getDrawingCacheBackgroundColor();
    };
    SearchBarStyler.setBackgroundColorProperty = function (v, newValue) {
        var bar = v._nativeView;
        bar.setBackgroundColor(newValue);
        SearchBarStyler._changeSearchViewPlateBackgroundColor(bar, newValue);
    };
    SearchBarStyler.resetBackgroundColorProperty = function (v, nativeValue) {
        var bar = v._nativeView;
        bar.setBackgroundColor(nativeValue);
        SearchBarStyler._changeSearchViewPlateBackgroundColor(bar, nativeValue);
    };
    SearchBarStyler.getColorProperty = function (v) {
        var bar = v._nativeView;
        var textView = SearchBarStyler._getSearchViewTextView(bar);
        if (textView) {
            return textView.getCurrentTextColor();
        }
        return undefined;
    };
    SearchBarStyler.setColorProperty = function (v, newValue) {
        var bar = v._nativeView;
        SearchBarStyler._changeSearchViewTextColor(bar, newValue);
    };
    SearchBarStyler.resetColorProperty = function (v, nativeValue) {
        var bar = v._nativeView;
        SearchBarStyler._changeSearchViewTextColor(bar, nativeValue);
    };
    SearchBarStyler.setFontInternalProperty = function (v, newValue, nativeValue) {
        var bar = v.android;
        var textView = SearchBarStyler._getSearchViewTextView(bar);
        var fontValue = newValue;
        var typeface = fontValue.getAndroidTypeface();
        if (typeface) {
            textView.setTypeface(typeface);
        }
        else {
            textView.setTypeface(nativeValue.typeface);
        }
        if (fontValue.fontSize) {
            textView.setTextSize(fontValue.fontSize);
        }
        else {
            textView.setTextSize(android.util.TypedValue.COMPLEX_UNIT_PX, nativeValue.size);
        }
    };
    SearchBarStyler.resetFontInternalProperty = function (v, nativeValue) {
        var bar = v.android;
        var textView = SearchBarStyler._getSearchViewTextView(bar);
        textView.setTypeface(nativeValue.typeface);
        textView.setTextSize(android.util.TypedValue.COMPLEX_UNIT_PX, nativeValue.size);
    };
    SearchBarStyler.getNativeFontInternalValue = function (v) {
        var bar = v.android;
        var textView = SearchBarStyler._getSearchViewTextView(bar);
        return {
            typeface: textView.getTypeface(),
            size: textView.getTextSize()
        };
    };
    SearchBarStyler.registerHandlers = function () {
        style.registerHandler(style.backgroundColorProperty, new style.StylePropertyChangedHandler(SearchBarStyler.setBackgroundColorProperty, SearchBarStyler.resetBackgroundColorProperty, SearchBarStyler.getBackgroundColorProperty), "SearchBar");
        style.registerHandler(style.colorProperty, new style.StylePropertyChangedHandler(SearchBarStyler.setColorProperty, SearchBarStyler.resetColorProperty, SearchBarStyler.getColorProperty), "SearchBar");
        style.registerHandler(style.fontInternalProperty, new style.StylePropertyChangedHandler(SearchBarStyler.setFontInternalProperty, SearchBarStyler.resetFontInternalProperty, SearchBarStyler.getNativeFontInternalValue), "SearchBar");
    };
    SearchBarStyler._getSearchViewTextView = function (bar) {
        var id = bar.getContext().getResources().getIdentifier("android:id/search_src_text", null, null);
        return bar.findViewById(id);
    };
    SearchBarStyler._changeSearchViewTextColor = function (bar, color) {
        var textView = SearchBarStyler._getSearchViewTextView(bar);
        if (textView) {
            textView.setTextColor(color);
        }
    };
    SearchBarStyler._changeSearchViewPlateBackgroundColor = function (bar, color) {
        var id = bar.getContext().getResources().getIdentifier("android:id/search_plate", null, null);
        var textView = bar.findViewById(id);
        if (textView) {
            textView.setBackgroundColor(color);
        }
    };
    return SearchBarStyler;
}());
exports.SearchBarStyler = SearchBarStyler;
SearchBarStyler.registerHandlers();
