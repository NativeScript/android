var observable = require("data/observable");
var observableArray = require("data/observable-array");
var types = require("utils/types");
var colorModule = require("color");
var knownCollections;
(function (knownCollections) {
    knownCollections.spans = "spans";
})(knownCollections = exports.knownCollections || (exports.knownCollections = {}));
var CHILD_SPAN = "Span";
var CHILD_FORMATTED_TEXT = "formattedText";
var CHILD_FORMATTED_STRING = "FormattedString";
var FormattedString = (function (_super) {
    __extends(FormattedString, _super);
    function FormattedString() {
        _super.call(this);
        this._spans = new observableArray.ObservableArray();
        this._spans.addEventListener(observableArray.ObservableArray.changeEvent, this.onSpansCollectionChanged, this);
        this._isDirty = true;
    }
    Object.defineProperty(FormattedString.prototype, "parent", {
        get: function () {
            return this._parent;
        },
        set: function (value) {
            if (this._parent !== value) {
                this._parent = value;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FormattedString.prototype, "fontFamily", {
        get: function () {
            return this._fontFamily;
        },
        set: function (value) {
            if (this._fontFamily !== value) {
                this._fontFamily = value;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FormattedString.prototype, "fontSize", {
        get: function () {
            return this._fontSize;
        },
        set: function (value) {
            var fSize;
            if (types.isString(value)) {
                fSize = parseInt(value);
            }
            else {
                fSize = value;
            }
            if (this._fontSize !== fSize) {
                this._fontSize = fSize;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FormattedString.prototype, "foregroundColor", {
        get: function () {
            return this._foregroundColor;
        },
        set: function (value) {
            var foreColor;
            if (types.isString(value)) {
                foreColor = new colorModule.Color(value);
            }
            else {
                foreColor = value;
            }
            if (this._foregroundColor !== foreColor) {
                this._foregroundColor = foreColor;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FormattedString.prototype, "backgroundColor", {
        get: function () {
            return this._backgroundColor;
        },
        set: function (value) {
            var backColor;
            if (types.isString(value)) {
                backColor = new colorModule.Color(value);
            }
            else {
                backColor = value;
            }
            if (this._backgroundColor !== backColor) {
                this._backgroundColor = backColor;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FormattedString.prototype, "underline", {
        get: function () {
            return this._underline;
        },
        set: function (value) {
            var underlineIntValue;
            if (types.isString(value)) {
                underlineIntValue = parseInt(value);
            }
            else {
                underlineIntValue = value;
            }
            if (this._underline !== underlineIntValue) {
                this._underline = underlineIntValue;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FormattedString.prototype, "strikethrough", {
        get: function () {
            return this._strikethrough;
        },
        set: function (value) {
            var strikethroughIntValue;
            if (types.isString(value)) {
                strikethroughIntValue = parseInt(value);
            }
            else {
                strikethroughIntValue = value;
            }
            if (this._strikethrough !== strikethroughIntValue) {
                this._strikethrough = strikethroughIntValue;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FormattedString.prototype, "fontAttributes", {
        get: function () {
            return this._fontAttributes;
        },
        set: function (value) {
            if (this._fontAttributes !== value) {
                this._fontAttributes = value;
            }
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(FormattedString.prototype, "spans", {
        get: function () {
            if (!this._spans) {
                this._spans = new observableArray.ObservableArray();
            }
            return this._spans;
        },
        enumerable: true,
        configurable: true
    });
    FormattedString.prototype.onSpansCollectionChanged = function (eventData) {
        var i;
        if (eventData.addedCount > 0) {
            for (i = 0; i < eventData.addedCount; i++) {
                var addedSpan = eventData.object.getItem(eventData.index + i);
                addedSpan.parentFormattedString = this;
                addedSpan.addEventListener(observable.Observable.propertyChangeEvent, this.onSpanChanged, this);
            }
        }
        if (eventData.removed && eventData.removed.length > 0) {
            var p;
            for (p = 0; p < eventData.removed.length; p++) {
                var removedSpan = eventData.removed[p];
                removedSpan.removeEventListener(observable.Observable.propertyChangeEvent, this.onSpanChanged, this);
            }
        }
        this.updateFormattedText(true);
    };
    FormattedString.prototype.onSpanChanged = function (eventData) {
        this.updateFormattedText(true);
    };
    FormattedString.prototype.updateFormattedText = function (isDirty) {
        var shouldUpdate = isDirty || this._isDirty;
        if (shouldUpdate) {
            this.createFormattedStringCore();
            this._isDirty = false;
            this.notify(this._createPropertyChangeData("", this));
        }
    };
    FormattedString.prototype.createFormattedStringCore = function () {
    };
    FormattedString.prototype.toString = function () {
        var result = "";
        var i;
        for (i = 0; i < this._spans.length; i++) {
            result += this._spans.getItem(i).text;
        }
        return result;
    };
    FormattedString.prototype._addArrayFromBuilder = function (name, value) {
        var i;
        var span;
        if (name === knownCollections.spans) {
            for (i = 0; i < value.length; i++) {
                span = value[i];
                this.spans.push(span);
            }
        }
    };
    FormattedString.prototype.updateSpansBindingContext = function (newBindingContext) {
        var i;
        for (i = 0; i < this.spans.length; i++) {
            var span = this.spans.getItem(i);
            span.bindingContext = newBindingContext;
        }
    };
    FormattedString.prototype._addChildFromBuilder = function (name, value) {
        if (name === CHILD_SPAN) {
            this.spans.push(value);
        }
    };
    FormattedString.addFormattedStringToView = function (view, name, value) {
        if (name === CHILD_SPAN) {
            if (!view.formattedText) {
                view.formattedText = new FormattedString();
            }
            view.formattedText.spans.push(value);
        }
        else if (name === CHILD_FORMATTED_TEXT || name === CHILD_FORMATTED_STRING) {
            view.formattedText = value;
        }
    };
    return FormattedString;
}(observable.Observable));
exports.FormattedString = FormattedString;
