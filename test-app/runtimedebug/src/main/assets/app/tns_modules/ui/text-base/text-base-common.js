var view = require("ui/core/view");
var observable = require("data/observable");
var dependencyObservable = require("ui/core/dependency-observable");
var proxy = require("ui/core/proxy");
var formattedString = require("text/formatted-string");
var tbs = require("ui/text-base/text-base-styler");
var weakEvents;
function ensureWeakEvents() {
    if (!weakEvents) {
        weakEvents = require("ui/core/weak-event-listener");
    }
}
var textProperty = new dependencyObservable.Property("text", "TextBase", new proxy.PropertyMetadata("", dependencyObservable.PropertyMetadataSettings.None));
var formattedTextProperty = new dependencyObservable.Property("formattedText", "TextBase", new proxy.PropertyMetadata("", dependencyObservable.PropertyMetadataSettings.AffectsLayout));
function onTextPropertyChanged(data) {
    var textBase = data.object;
    textBase._onTextPropertyChanged(data);
    textBase.style._updateTextTransform();
    textBase.style._updateTextDecoration();
}
textProperty.metadata.onSetNativeValue = onTextPropertyChanged;
function onFormattedTextPropertyChanged(data) {
    var textBase = data.object;
    textBase._onFormattedTextPropertyChanged(data);
}
formattedTextProperty.metadata.onSetNativeValue = onFormattedTextPropertyChanged;
var TextBase = (function (_super) {
    __extends(TextBase, _super);
    function TextBase() {
        _super.apply(this, arguments);
    }
    TextBase.prototype._onBindingContextChanged = function (oldValue, newValue) {
        _super.prototype._onBindingContextChanged.call(this, oldValue, newValue);
        if (this.formattedText) {
            this.formattedText.updateSpansBindingContext(newValue);
        }
        this.style._updateTextTransform();
        this.style._updateTextDecoration();
    };
    Object.defineProperty(TextBase.prototype, "text", {
        get: function () {
            return this._getValue(TextBase.textProperty);
        },
        set: function (value) {
            this._setValue(TextBase.textProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TextBase.prototype, "fontSize", {
        get: function () {
            return this.style.fontSize;
        },
        set: function (value) {
            this.style.fontSize = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TextBase.prototype, "textAlignment", {
        get: function () {
            return this.style.textAlignment;
        },
        set: function (value) {
            this.style.textAlignment = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(TextBase.prototype, "formattedText", {
        get: function () {
            return this._getValue(TextBase.formattedTextProperty);
        },
        set: function (value) {
            if (this.formattedText !== value) {
                ensureWeakEvents();
                if (this.formattedText) {
                    weakEvents.removeWeakEventListener(this.formattedText, observable.Observable.propertyChangeEvent, this.onFormattedTextChanged, this);
                }
                this._setValue(TextBase.formattedTextProperty, value);
                if (value) {
                    weakEvents.addWeakEventListener(value, observable.Observable.propertyChangeEvent, this.onFormattedTextChanged, this);
                }
            }
        },
        enumerable: true,
        configurable: true
    });
    TextBase.prototype.onFormattedTextChanged = function (eventData) {
        var value = eventData.value;
        this._setFormattedTextPropertyToNative(value);
        this._onPropertyChangedFromNative(TextBase.textProperty, value.toString());
    };
    TextBase.prototype._onTextPropertyChanged = function (data) {
    };
    TextBase.prototype._setFormattedTextPropertyToNative = function (value) {
    };
    TextBase.prototype._onFormattedTextPropertyChanged = function (data) {
        var newValue = data.newValue;
        if (newValue) {
            newValue.parent = this;
        }
        this._setFormattedTextPropertyToNative(newValue);
        var newText = newValue ? newValue.toString() : "";
        this._onPropertyChangedFromNative(TextBase.textProperty, newText);
    };
    TextBase.prototype._addChildFromBuilder = function (name, value) {
        formattedString.FormattedString.addFormattedStringToView(this, name, value);
    };
    TextBase.prototype._requestLayoutOnTextChanged = function () {
        this.requestLayout();
    };
    TextBase.textProperty = textProperty;
    TextBase.formattedTextProperty = formattedTextProperty;
    return TextBase;
}(view.View));
exports.TextBase = TextBase;
tbs.TextBaseStyler.registerHandlers();
