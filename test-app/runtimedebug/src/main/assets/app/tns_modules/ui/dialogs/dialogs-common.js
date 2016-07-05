var frame = require("ui/frame");
var button = require("ui/button");
var textField = require("ui/text-field");
var label = require("ui/label");
var types = require("utils/types");
exports.STRING = "string", exports.PROMPT = "Prompt", exports.CONFIRM = "Confirm", exports.ALERT = "Alert", exports.LOGIN = "Login", exports.OK = "OK", exports.CANCEL = "Cancel";
var inputType;
(function (inputType) {
    inputType.text = "text";
    inputType.password = "password";
})(inputType = exports.inputType || (exports.inputType = {}));
function getCurrentPage() {
    var topMostFrame = frame.topmost();
    if (topMostFrame) {
        return topMostFrame.currentPage;
    }
    return undefined;
}
exports.getCurrentPage = getCurrentPage;
function applySelectors(view) {
    var currentPage = getCurrentPage();
    if (currentPage) {
        var styleScope = currentPage._getStyleScope();
        if (styleScope) {
            styleScope.applySelectors(view);
        }
    }
}
var buttonColor;
function getButtonColor() {
    if (!buttonColor) {
        var btn = new button.Button();
        applySelectors(btn);
        buttonColor = btn.color;
    }
    return buttonColor;
}
exports.getButtonColor = getButtonColor;
var textFieldColor;
function getTextFieldColor() {
    if (!textFieldColor) {
        var tf = new textField.TextField();
        applySelectors(tf);
        textFieldColor = tf.color;
    }
    return textFieldColor;
}
exports.getTextFieldColor = getTextFieldColor;
var labelColor;
function getLabelColor() {
    if (!labelColor) {
        var lbl = new label.Label();
        applySelectors(lbl);
        labelColor = lbl.color;
    }
    return labelColor;
}
exports.getLabelColor = getLabelColor;
function isDialogOptions(arg) {
    return !types.isNullOrUndefined(arg) && (arg.message || arg.title);
}
exports.isDialogOptions = isDialogOptions;
