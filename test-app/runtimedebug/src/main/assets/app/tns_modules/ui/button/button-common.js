var dependency_observable_1 = require("ui/core/dependency-observable");
var view = require("ui/core/view");
var proxy = require("ui/core/proxy");
var formattedString = require("text/formatted-string");
var observable = require("data/observable");
var enums_1 = require("ui/enums");
var platform_1 = require("platform");
var AffectsLayout = platform_1.isAndroid ? dependency_observable_1.PropertyMetadataSettings.None : dependency_observable_1.PropertyMetadataSettings.AffectsLayout;
var weakEvents;
function ensureWeakEvents() {
    if (!weakEvents) {
        weakEvents = require("ui/core/weak-event-listener");
    }
}
var textProperty = new dependency_observable_1.Property("text", "Button", new proxy.PropertyMetadata("", AffectsLayout));
var formattedTextProperty = new dependency_observable_1.Property("formattedText", "Button", new proxy.PropertyMetadata("", AffectsLayout));
var textWrapProperty = new dependency_observable_1.Property("textWrap", "Button", new proxy.PropertyMetadata(false, AffectsLayout));
function onTextPropertyChanged(data) {
    var button = data.object;
    button._onTextPropertyChanged(data);
    button.style._updateTextDecoration();
    button.style._updateTextTransform();
}
function onFormattedTextPropertyChanged(data) {
    var button = data.object;
    button._onFormattedTextPropertyChanged(data);
}
textProperty.metadata.onSetNativeValue = onTextPropertyChanged;
formattedTextProperty.metadata.onSetNativeValue = onFormattedTextPropertyChanged;
var Button = (function (_super) {
    __extends(Button, _super);
    function Button() {
        _super.apply(this, arguments);
    }
    Button.prototype._onBindingContextChanged = function (oldValue, newValue) {
        _super.prototype._onBindingContextChanged.call(this, oldValue, newValue);
        if (this.formattedText) {
            this.formattedText.updateSpansBindingContext(newValue);
        }
        this.style._updateTextDecoration();
        this.style._updateTextTransform();
    };
    Object.defineProperty(Button.prototype, "text", {
        get: function () {
            return this._getValue(Button.textProperty);
        },
        set: function (value) {
            this._setValue(Button.textProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Button.prototype, "formattedText", {
        get: function () {
            return this._getValue(Button.formattedTextProperty);
        },
        set: function (value) {
            if (this.formattedText !== value) {
                ensureWeakEvents();
                if (this.formattedText) {
                    weakEvents.removeWeakEventListener(this.formattedText, observable.Observable.propertyChangeEvent, this.onFormattedTextChanged, this);
                }
                this._setValue(Button.formattedTextProperty, value);
                if (value) {
                    weakEvents.addWeakEventListener(value, observable.Observable.propertyChangeEvent, this.onFormattedTextChanged, this);
                }
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Button.prototype, "textWrap", {
        get: function () {
            return this._getValue(Button.textWrapProperty);
        },
        set: function (value) {
            this._setValue(Button.textWrapProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Button.prototype, "whiteSpace", {
        get: function () {
            return this.style.whiteSpace;
        },
        set: function (value) {
            this.style.whiteSpace = value;
        },
        enumerable: true,
        configurable: true
    });
    Button.prototype.onFormattedTextChanged = function (eventData) {
        var value = eventData.value;
        this._setFormattedTextPropertyToNative(value);
        this._onPropertyChangedFromNative(Button.textProperty, value.toString());
    };
    Button.prototype._onTextPropertyChanged = function (data) {
    };
    Button.prototype._setFormattedTextPropertyToNative = function (value) {
    };
    Button.prototype._onFormattedTextPropertyChanged = function (data) {
        var newValue = data.newValue;
        if (newValue) {
            newValue.parent = this;
        }
        this._setFormattedTextPropertyToNative(newValue);
        var newText = newValue ? newValue.toString() : "";
        this._onPropertyChangedFromNative(Button.textProperty, newText);
    };
    Button.prototype._addChildFromBuilder = function (name, value) {
        formattedString.FormattedString.addFormattedStringToView(this, name, value);
    };
    Button.tapEvent = "tap";
    Button.textProperty = textProperty;
    Button.formattedTextProperty = formattedTextProperty;
    Button.textWrapProperty = textWrapProperty;
    return Button;
}(view.View));
exports.Button = Button;
function onTextWrapPropertyChanged(data) {
    var v = data.object;
    v.style.whiteSpace = data.newValue ? enums_1.WhiteSpace.normal : enums_1.WhiteSpace.nowrap;
}
Button.textWrapProperty.metadata.onSetNativeValue = onTextWrapPropertyChanged;
