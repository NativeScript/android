var textBase = require("ui/text-base");
var proxy = require("ui/core/proxy");
var dependencyObservable = require("ui/core/dependency-observable");
var enums = require("ui/enums");
var keyboardTypeProperty = new dependencyObservable.Property("keyboardType", "EditableTextBase", new proxy.PropertyMetadata(undefined, dependencyObservable.PropertyMetadataSettings.None));
var returnKeyTypeProperty = new dependencyObservable.Property("returnKeyType", "EditableTextBase", new proxy.PropertyMetadata(undefined, dependencyObservable.PropertyMetadataSettings.None));
var editableProperty = new dependencyObservable.Property("editable", "EditableTextBase", new proxy.PropertyMetadata(true, dependencyObservable.PropertyMetadataSettings.None));
var updateTextTriggerProperty = new dependencyObservable.Property("updateTextTrigger", "EditableTextBase", new proxy.PropertyMetadata(enums.UpdateTextTrigger.textChanged, dependencyObservable.PropertyMetadataSettings.None));
var autocapitalizationTypeProperty = new dependencyObservable.Property("autocapitalizationType", "EditableTextBase", new proxy.PropertyMetadata(enums.AutocapitalizationType.sentences, dependencyObservable.PropertyMetadataSettings.None));
var autocorrectProperty = new dependencyObservable.Property("autocorrect", "EditableTextBase", new proxy.PropertyMetadata(undefined, dependencyObservable.PropertyMetadataSettings.None));
exports.hintProperty = new dependencyObservable.Property("hint", "EditableTextBase", new proxy.PropertyMetadata(""));
function onKeyboardTypePropertyChanged(data) {
    var editableTextBase = data.object;
    editableTextBase._onKeyboardTypePropertyChanged(data);
}
keyboardTypeProperty.metadata.onSetNativeValue = onKeyboardTypePropertyChanged;
function onReturnKeyTypePropertyChanged(data) {
    var editableTextBase = data.object;
    editableTextBase._onReturnKeyTypePropertyChanged(data);
}
returnKeyTypeProperty.metadata.onSetNativeValue = onReturnKeyTypePropertyChanged;
function onEditablePropertyChanged(data) {
    var editableTextBase = data.object;
    editableTextBase._onEditablePropertyChanged(data);
}
editableProperty.metadata.onSetNativeValue = onEditablePropertyChanged;
function onAutocapitalizationTypePropertyChanged(data) {
    var editableTextBase = data.object;
    editableTextBase._onAutocapitalizationTypePropertyChanged(data);
}
autocapitalizationTypeProperty.metadata.onSetNativeValue = onAutocapitalizationTypePropertyChanged;
function onAutocorrectPropertyChanged(data) {
    var editableTextBase = data.object;
    editableTextBase._onAutocorrectPropertyChanged(data);
}
autocorrectProperty.metadata.onSetNativeValue = onAutocorrectPropertyChanged;
function onHintPropertyChanged(data) {
    var editableTextBase = data.object;
    editableTextBase._onHintPropertyChanged(data);
}
exports.hintProperty.metadata.onSetNativeValue = onHintPropertyChanged;
var EditableTextBase = (function (_super) {
    __extends(EditableTextBase, _super);
    function EditableTextBase() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(EditableTextBase.prototype, "keyboardType", {
        get: function () {
            return this._getValue(EditableTextBase.keyboardTypeProperty);
        },
        set: function (value) {
            this._setValue(EditableTextBase.keyboardTypeProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(EditableTextBase.prototype, "returnKeyType", {
        get: function () {
            return this._getValue(EditableTextBase.returnKeyTypeProperty);
        },
        set: function (value) {
            this._setValue(EditableTextBase.returnKeyTypeProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(EditableTextBase.prototype, "editable", {
        get: function () {
            return this._getValue(EditableTextBase.editableProperty);
        },
        set: function (value) {
            this._setValue(EditableTextBase.editableProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(EditableTextBase.prototype, "updateTextTrigger", {
        get: function () {
            return this._getValue(EditableTextBase.updateTextTriggerProperty);
        },
        set: function (value) {
            this._setValue(EditableTextBase.updateTextTriggerProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(EditableTextBase.prototype, "autocapitalizationType", {
        get: function () {
            return this._getValue(EditableTextBase.autocapitalizationTypeProperty);
        },
        set: function (value) {
            this._setValue(EditableTextBase.autocapitalizationTypeProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(EditableTextBase.prototype, "autocorrect", {
        get: function () {
            return this._getValue(EditableTextBase.autocorrectProperty);
        },
        set: function (value) {
            this._setValue(EditableTextBase.autocorrectProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(EditableTextBase.prototype, "hint", {
        get: function () {
            return this._getValue(EditableTextBase.hintProperty);
        },
        set: function (value) {
            this._setValue(EditableTextBase.hintProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    EditableTextBase.prototype.dismissSoftInput = function () {
    };
    EditableTextBase.prototype._onKeyboardTypePropertyChanged = function (data) {
    };
    EditableTextBase.prototype._onReturnKeyTypePropertyChanged = function (data) {
    };
    EditableTextBase.prototype._onEditablePropertyChanged = function (data) {
    };
    EditableTextBase.prototype._onAutocapitalizationTypePropertyChanged = function (data) {
    };
    EditableTextBase.prototype._onAutocorrectPropertyChanged = function (data) {
    };
    EditableTextBase.prototype._onHintPropertyChanged = function (data) {
    };
    EditableTextBase.keyboardTypeProperty = keyboardTypeProperty;
    EditableTextBase.returnKeyTypeProperty = returnKeyTypeProperty;
    EditableTextBase.editableProperty = editableProperty;
    EditableTextBase.updateTextTriggerProperty = updateTextTriggerProperty;
    EditableTextBase.autocapitalizationTypeProperty = autocapitalizationTypeProperty;
    EditableTextBase.autocorrectProperty = autocorrectProperty;
    EditableTextBase.hintProperty = exports.hintProperty;
    return EditableTextBase;
}(textBase.TextBase));
exports.EditableTextBase = EditableTextBase;
