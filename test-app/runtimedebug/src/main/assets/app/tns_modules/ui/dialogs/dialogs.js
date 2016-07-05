var dialogsCommon = require("./dialogs-common");
var appmodule = require("application");
var types = require("utils/types");
global.moduleMerge(dialogsCommon, exports);
function createAlertDialog(options) {
    var alert = new android.app.AlertDialog.Builder(appmodule.android.currentContext);
    alert.setTitle(options && types.isString(options.title) ? options.title : "");
    alert.setMessage(options && types.isString(options.message) ? options.message : "");
    if (options && options.cancelable === false) {
        alert.setCancelable(false);
    }
    return alert;
}
function showDialog(builder) {
    var dlg = builder.show();
    var labelColor = dialogsCommon.getLabelColor();
    if (labelColor) {
        var textViewId = dlg.getContext().getResources().getIdentifier("android:id/alertTitle", null, null);
        if (textViewId) {
            var tv = dlg.findViewById(textViewId);
            if (tv) {
                tv.setTextColor(labelColor.android);
            }
        }
        var messageTextViewId = dlg.getContext().getResources().getIdentifier("android:id/message", null, null);
        if (messageTextViewId) {
            var messageTextView = dlg.findViewById(messageTextViewId);
            if (messageTextView) {
                messageTextView.setTextColor(labelColor.android);
            }
        }
    }
}
function addButtonsToAlertDialog(alert, options, callback) {
    if (!options) {
        return;
    }
    if (options.okButtonText) {
        alert.setPositiveButton(options.okButtonText, new android.content.DialogInterface.OnClickListener({
            onClick: function (dialog, id) {
                dialog.cancel();
                callback(true);
            }
        }));
    }
    if (options.cancelButtonText) {
        alert.setNegativeButton(options.cancelButtonText, new android.content.DialogInterface.OnClickListener({
            onClick: function (dialog, id) {
                dialog.cancel();
                callback(false);
            }
        }));
    }
    if (options.neutralButtonText) {
        alert.setNeutralButton(options.neutralButtonText, new android.content.DialogInterface.OnClickListener({
            onClick: function (dialog, id) {
                dialog.cancel();
                callback(undefined);
            }
        }));
    }
    alert.setOnDismissListener(new android.content.DialogInterface.OnDismissListener({
        onDismiss: function () {
            callback(false);
        }
    }));
}
function alert(arg) {
    return new Promise(function (resolve, reject) {
        try {
            var options = !dialogsCommon.isDialogOptions(arg) ? { title: dialogsCommon.ALERT, okButtonText: dialogsCommon.OK, message: arg + "" } : arg;
            var alert = createAlertDialog(options);
            alert.setPositiveButton(options.okButtonText, new android.content.DialogInterface.OnClickListener({
                onClick: function (dialog, id) {
                    dialog.cancel();
                    resolve();
                }
            }));
            alert.setOnDismissListener(new android.content.DialogInterface.OnDismissListener({
                onDismiss: function () {
                    resolve();
                }
            }));
            showDialog(alert);
        }
        catch (ex) {
            reject(ex);
        }
    });
}
exports.alert = alert;
function confirm(arg) {
    return new Promise(function (resolve, reject) {
        try {
            var options = !dialogsCommon.isDialogOptions(arg) ? { title: dialogsCommon.CONFIRM, okButtonText: dialogsCommon.OK, cancelButtonText: dialogsCommon.CANCEL, message: arg + "" } : arg;
            var alert = createAlertDialog(options);
            addButtonsToAlertDialog(alert, options, function (result) { resolve(result); });
            showDialog(alert);
        }
        catch (ex) {
            reject(ex);
        }
    });
}
exports.confirm = confirm;
function prompt(arg) {
    var options;
    var defaultOptions = {
        title: dialogsCommon.PROMPT,
        okButtonText: dialogsCommon.OK,
        cancelButtonText: dialogsCommon.CANCEL,
        inputType: dialogsCommon.inputType.text,
    };
    if (arguments.length === 1) {
        if (types.isString(arg)) {
            options = defaultOptions;
            options.message = arg;
        }
        else {
            options = arg;
        }
    }
    else if (arguments.length === 2) {
        if (types.isString(arguments[0]) && types.isString(arguments[1])) {
            options = defaultOptions;
            options.message = arguments[0];
            options.defaultText = arguments[1];
        }
    }
    return new Promise(function (resolve, reject) {
        try {
            var alert = createAlertDialog(options);
            var input = new android.widget.EditText(appmodule.android.currentContext);
            if (options && options.inputType === dialogsCommon.inputType.password) {
                input.setInputType(android.text.InputType.TYPE_CLASS_TEXT | android.text.InputType.TYPE_TEXT_VARIATION_PASSWORD);
            }
            input.setText(options && options.defaultText || "");
            alert.setView(input);
            var getText = function () { return input.getText().toString(); };
            addButtonsToAlertDialog(alert, options, function (r) { resolve({ result: r, text: getText() }); });
            showDialog(alert);
        }
        catch (ex) {
            reject(ex);
        }
    });
}
exports.prompt = prompt;
function login(arg) {
    var options;
    var defaultOptions = { title: dialogsCommon.LOGIN, okButtonText: dialogsCommon.OK, cancelButtonText: dialogsCommon.CANCEL };
    if (arguments.length === 1) {
        if (types.isString(arguments[0])) {
            options = defaultOptions;
            options.message = arguments[0];
        }
        else {
            options = arguments[0];
        }
    }
    else if (arguments.length === 2) {
        if (types.isString(arguments[0]) && types.isString(arguments[1])) {
            options = defaultOptions;
            options.message = arguments[0];
            options.userName = arguments[1];
        }
    }
    else if (arguments.length === 3) {
        if (types.isString(arguments[0]) && types.isString(arguments[1]) && types.isString(arguments[2])) {
            options = defaultOptions;
            options.message = arguments[0];
            options.userName = arguments[1];
            options.password = arguments[2];
        }
    }
    return new Promise(function (resolve, reject) {
        try {
            var context = appmodule.android.currentContext;
            var alert = createAlertDialog(options);
            var userNameInput = new android.widget.EditText(context);
            userNameInput.setText(options.userName ? options.userName : "");
            var passwordInput = new android.widget.EditText(context);
            passwordInput.setInputType(android.text.InputType.TYPE_CLASS_TEXT | android.text.InputType.TYPE_TEXT_VARIATION_PASSWORD);
            passwordInput.setText(options.password ? options.password : "");
            var layout = new android.widget.LinearLayout(context);
            layout.setOrientation(1);
            layout.addView(userNameInput);
            layout.addView(passwordInput);
            alert.setView(layout);
            addButtonsToAlertDialog(alert, options, function (r) {
                resolve({
                    result: r,
                    userName: userNameInput.getText().toString(),
                    password: passwordInput.getText().toString()
                });
            });
            showDialog(alert);
        }
        catch (ex) {
            reject(ex);
        }
    });
}
exports.login = login;
function action(arg) {
    var options;
    var defaultOptions = { title: null, cancelButtonText: dialogsCommon.CANCEL };
    if (arguments.length === 1) {
        if (types.isString(arguments[0])) {
            options = defaultOptions;
            options.message = arguments[0];
        }
        else {
            options = arguments[0];
        }
    }
    else if (arguments.length === 2) {
        if (types.isString(arguments[0]) && types.isString(arguments[1])) {
            options = defaultOptions;
            options.message = arguments[0];
            options.cancelButtonText = arguments[1];
        }
    }
    else if (arguments.length === 3) {
        if (types.isString(arguments[0]) && types.isString(arguments[1]) && types.isDefined(arguments[2])) {
            options = defaultOptions;
            options.message = arguments[0];
            options.cancelButtonText = arguments[1];
            options.actions = arguments[2];
        }
    }
    return new Promise(function (resolve, reject) {
        try {
            var activity = appmodule.android.foregroundActivity || appmodule.android.startActivity;
            var alert = new android.app.AlertDialog.Builder(activity);
            var message = options && types.isString(options.message) ? options.message : "";
            var title = options && types.isString(options.title) ? options.title : "";
            if (options && options.cancelable === false) {
                alert.setCancelable(false);
            }
            if (title) {
                alert.setTitle(title);
                if (!options.actions) {
                    alert.setMessage(message);
                }
            }
            else {
                alert.setTitle(message);
            }
            if (options.actions) {
                alert.setItems(options.actions, new android.content.DialogInterface.OnClickListener({
                    onClick: function (dialog, which) {
                        resolve(options.actions[which]);
                    }
                }));
            }
            if (types.isString(options.cancelButtonText)) {
                alert.setNegativeButton(options.cancelButtonText, new android.content.DialogInterface.OnClickListener({
                    onClick: function (dialog, id) {
                        dialog.cancel();
                        resolve(options.cancelButtonText);
                    }
                }));
            }
            alert.setOnDismissListener(new android.content.DialogInterface.OnDismissListener({
                onDismiss: function () {
                    if (types.isString(options.cancelButtonText)) {
                        resolve(options.cancelButtonText);
                    }
                    else {
                        resolve("");
                    }
                }
            }));
            showDialog(alert);
        }
        catch (ex) {
            reject(ex);
        }
    });
}
exports.action = action;
