"use strict";
var application = require('application');
var storeSvc = require('./appStoreSvc');
application.mainModule = 'main-page';
application.cssFile = './app.css';
application.start();

var buttonName = "a.B.C.ButtonThatWillAlwaysBeExtended";

android.widget.Button.extend("a.B.C.ButtonThatWillAlwaysBeExtended", {
    toString: function() {
        return buttonName;
    }
});