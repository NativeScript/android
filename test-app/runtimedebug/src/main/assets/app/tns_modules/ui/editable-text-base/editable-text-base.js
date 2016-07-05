var common = require("./editable-text-base-common");
var enums = require("ui/enums");
var utils = require("utils/utils");
var types = require("utils/types");
var EditableTextBase = (function (_super) {
    __extends(EditableTextBase, _super);
    function EditableTextBase() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(EditableTextBase.prototype, "android", {
        get: function () {
            return this._android;
        },
        enumerable: true,
        configurable: true
    });
    EditableTextBase.prototype._createUI = function () {
        this._android = new android.widget.EditText(this._context);
        this._configureEditText();
        this._keyListenerCache = this.android.getKeyListener();
        var that = new WeakRef(this);
        this._textWatcher = new android.text.TextWatcher({
            beforeTextChanged: function (text, start, count, after) {
            },
            onTextChanged: function (text, start, before, count) {
                var owner = that.get();
                if (!owner) {
                    return;
                }
                var selectionStart = owner.android.getSelectionStart();
                owner.android.removeTextChangedListener(owner._textWatcher);
                owner.style._updateTextTransform();
                owner.android.addTextChangedListener(owner._textWatcher);
                owner.android.setSelection(selectionStart);
            },
            afterTextChanged: function (editable) {
                var owner = that.get();
                if (!owner) {
                    return;
                }
                switch (owner.updateTextTrigger) {
                    case enums.UpdateTextTrigger.focusLost:
                        owner._dirtyTextAccumulator = editable.toString();
                        break;
                    case enums.UpdateTextTrigger.textChanged:
                        owner._onPropertyChangedFromNative(EditableTextBase.textProperty, editable.toString());
                        break;
                    default:
                        throw new Error("Invalid updateTextTrigger: " + owner.updateTextTrigger);
                }
            }
        });
        this._android.addTextChangedListener(this._textWatcher);
        var focusChangeListener = new android.view.View.OnFocusChangeListener({
            onFocusChange: function (view, hasFocus) {
                var owner = that.get();
                if (!owner) {
                    return;
                }
                if (!hasFocus) {
                    if (owner._dirtyTextAccumulator) {
                        owner._onPropertyChangedFromNative(EditableTextBase.textProperty, owner._dirtyTextAccumulator);
                        owner._dirtyTextAccumulator = undefined;
                    }
                    owner.dismissSoftInput();
                }
            }
        });
        this._android.setOnFocusChangeListener(focusChangeListener);
        var editorActionListener = new android.widget.TextView.OnEditorActionListener({
            onEditorAction: function (textView, actionId, event) {
                var owner = that.get();
                if (owner) {
                    if (actionId === android.view.inputmethod.EditorInfo.IME_ACTION_DONE ||
                        actionId === android.view.inputmethod.EditorInfo.IME_ACTION_GO ||
                        actionId === android.view.inputmethod.EditorInfo.IME_ACTION_SEARCH ||
                        actionId === android.view.inputmethod.EditorInfo.IME_ACTION_SEND ||
                        actionId === android.view.inputmethod.EditorInfo.IME_ACTION_NEXT ||
                        (event && event.getKeyCode() === android.view.KeyEvent.KEYCODE_ENTER)) {
                        owner.dismissSoftInput();
                        owner._onReturnPress();
                    }
                }
                return false;
            }
        });
        this._android.setOnEditorActionListener(editorActionListener);
    };
    EditableTextBase.prototype._configureEditText = function () {
    };
    EditableTextBase.prototype._onReturnPress = function () {
    };
    EditableTextBase.prototype._onDetached = function (force) {
        this._android = undefined;
        _super.prototype._onDetached.call(this, force);
    };
    EditableTextBase.prototype.dismissSoftInput = function () {
        utils.ad.dismissSoftInput(this._nativeView);
    };
    EditableTextBase.prototype.focus = function () {
        var result = _super.prototype.focus.call(this);
        if (result) {
            utils.ad.showSoftInput(this._nativeView);
        }
        return result;
    };
    EditableTextBase.prototype._onTextPropertyChanged = function (data) {
        if (this._android) {
            var newValue = types.toUIString(data.newValue);
            this.android.setText(newValue, android.widget.TextView.BufferType.EDITABLE);
        }
    };
    EditableTextBase.prototype._onKeyboardTypePropertyChanged = function (data) {
        if (!this._android) {
            return;
        }
        var newInputType;
        switch (data.newValue) {
            case enums.KeyboardType.datetime:
                newInputType = android.text.InputType.TYPE_CLASS_DATETIME | android.text.InputType.TYPE_DATETIME_VARIATION_NORMAL;
                break;
            case enums.KeyboardType.phone:
                newInputType = android.text.InputType.TYPE_CLASS_PHONE;
                break;
            case enums.KeyboardType.number:
                newInputType = android.text.InputType.TYPE_CLASS_NUMBER | android.text.InputType.TYPE_NUMBER_VARIATION_NORMAL | android.text.InputType.TYPE_NUMBER_FLAG_SIGNED | android.text.InputType.TYPE_NUMBER_FLAG_DECIMAL;
                break;
            case enums.KeyboardType.url:
                newInputType = android.text.InputType.TYPE_CLASS_TEXT | android.text.InputType.TYPE_TEXT_VARIATION_URI;
                break;
            case enums.KeyboardType.email:
                newInputType = android.text.InputType.TYPE_CLASS_TEXT | android.text.InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS;
                break;
            default:
                newInputType = android.text.InputType.TYPE_CLASS_TEXT | android.text.InputType.TYPE_TEXT_VARIATION_NORMAL;
                break;
        }
        this._setInputType(newInputType);
    };
    EditableTextBase.prototype._onReturnKeyTypePropertyChanged = function (data) {
        if (!this._android) {
            return;
        }
        var newImeOptions;
        switch (data.newValue) {
            case enums.ReturnKeyType.done:
                newImeOptions = android.view.inputmethod.EditorInfo.IME_ACTION_DONE;
                break;
            case enums.ReturnKeyType.go:
                newImeOptions = android.view.inputmethod.EditorInfo.IME_ACTION_GO;
                break;
            case enums.ReturnKeyType.next:
                newImeOptions = android.view.inputmethod.EditorInfo.IME_ACTION_NEXT;
                break;
            case enums.ReturnKeyType.search:
                newImeOptions = android.view.inputmethod.EditorInfo.IME_ACTION_SEARCH;
                break;
            case enums.ReturnKeyType.send:
                newImeOptions = android.view.inputmethod.EditorInfo.IME_ACTION_SEND;
                break;
            default:
                newImeOptions = android.view.inputmethod.EditorInfo.IME_ACTION_UNSPECIFIED;
                break;
        }
        this._android.setImeOptions(newImeOptions);
    };
    EditableTextBase.prototype._onEditablePropertyChanged = function (data) {
        if (!this._android) {
            return;
        }
        if (data.newValue) {
            this.android.setKeyListener(this._keyListenerCache);
        }
        else {
            this.android.setKeyListener(null);
        }
    };
    EditableTextBase.prototype._onAutocapitalizationTypePropertyChanged = function (data) {
        var editableTextBase = data.object;
        if (!editableTextBase.android) {
            return;
        }
        var inputType = editableTextBase.android.getInputType();
        inputType = inputType & ~28672;
        switch (data.newValue) {
            case enums.AutocapitalizationType.none:
                break;
            case enums.AutocapitalizationType.words:
                inputType = inputType | android.text.InputType.TYPE_TEXT_FLAG_CAP_WORDS;
                break;
            case enums.AutocapitalizationType.sentences:
                inputType = inputType | android.text.InputType.TYPE_TEXT_FLAG_CAP_SENTENCES;
                break;
            case enums.AutocapitalizationType.allCharacters:
                inputType = inputType | android.text.InputType.TYPE_TEXT_FLAG_CAP_CHARACTERS;
                break;
            default:
                inputType = inputType | android.text.InputType.TYPE_TEXT_FLAG_CAP_SENTENCES;
                break;
        }
        editableTextBase._setInputType(inputType);
    };
    EditableTextBase.prototype._onAutocorrectPropertyChanged = function (data) {
        var editableTextBase = data.object;
        if (!editableTextBase.android) {
            return;
        }
        var inputType = editableTextBase.android.getInputType();
        switch (data.newValue) {
            case true:
                inputType = inputType | android.text.InputType.TYPE_TEXT_FLAG_AUTO_COMPLETE;
                inputType = inputType | android.text.InputType.TYPE_TEXT_FLAG_AUTO_CORRECT;
                inputType = inputType & ~android.text.InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS;
                break;
            case false:
                inputType = inputType & ~android.text.InputType.TYPE_TEXT_FLAG_AUTO_COMPLETE;
                inputType = inputType & ~android.text.InputType.TYPE_TEXT_FLAG_AUTO_CORRECT;
                inputType = inputType | android.text.InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS;
                break;
            default:
                break;
        }
        editableTextBase._setInputType(inputType);
    };
    EditableTextBase.prototype._onHintPropertyChanged = function (data) {
        var editableTextBase = data.object;
        if (!editableTextBase.android) {
            return;
        }
        editableTextBase.android.setHint(data.newValue + "");
    };
    EditableTextBase.prototype._setInputType = function (inputType) {
        this.android.setInputType(inputType);
        var listener = this.android.getKeyListener();
        if (listener) {
            this._keyListenerCache = listener;
        }
        if (!this.editable) {
            this.android.setKeyListener(null);
        }
    };
    return EditableTextBase;
}(common.EditableTextBase));
exports.EditableTextBase = EditableTextBase;
