var common = require("./application-settings-common");
var utils = require("utils/utils");
var sharedPreferences;
function ensureSharedPreferences() {
    if (!sharedPreferences) {
        sharedPreferences = utils.ad.getApplicationContext().getSharedPreferences("prefs.db", 0);
    }
}
function verify(key) {
    common.checkKey(key);
    ensureSharedPreferences();
}
exports.hasKey = function (key) {
    verify(key);
    return sharedPreferences.contains(key);
};
exports.getBoolean = function (key, defaultValue) {
    verify(key);
    if (exports.hasKey(key)) {
        return sharedPreferences.getBoolean(key, false);
    }
    return defaultValue;
};
exports.getString = function (key, defaultValue) {
    verify(key);
    if (exports.hasKey(key)) {
        return sharedPreferences.getString(key, "");
    }
    return defaultValue;
};
exports.getNumber = function (key, defaultValue) {
    verify(key);
    if (exports.hasKey(key)) {
        return sharedPreferences.getFloat(key, float(0.0));
    }
    return defaultValue;
};
exports.setBoolean = function (key, value) {
    verify(key);
    common.ensureValidValue(value, "boolean");
    var editor = sharedPreferences.edit();
    editor.putBoolean(key, value);
    editor.commit();
};
exports.setString = function (key, value) {
    verify(key);
    common.ensureValidValue(value, "string");
    var editor = sharedPreferences.edit();
    editor.putString(key, value);
    editor.commit();
};
exports.setNumber = function (key, value) {
    verify(key);
    common.ensureValidValue(value, "number");
    var editor = sharedPreferences.edit();
    editor.putFloat(key, float(value));
    editor.commit();
};
exports.remove = function (key) {
    verify(key);
    var editor = sharedPreferences.edit();
    editor.remove(key);
    editor.commit();
};
exports.clear = function () {
    ensureSharedPreferences();
    sharedPreferences.edit().clear().commit();
};
