var KeyboardType;
(function (KeyboardType) {
    KeyboardType.datetime = "datetime";
    KeyboardType.phone = "phone";
    KeyboardType.number = "number";
    KeyboardType.url = "url";
    KeyboardType.email = "email";
})(KeyboardType = exports.KeyboardType || (exports.KeyboardType = {}));
var ReturnKeyType;
(function (ReturnKeyType) {
    ReturnKeyType.done = "done";
    ReturnKeyType.next = "next";
    ReturnKeyType.go = "go";
    ReturnKeyType.search = "search";
    ReturnKeyType.send = "send";
})(ReturnKeyType = exports.ReturnKeyType || (exports.ReturnKeyType = {}));
var TextAlignment;
(function (TextAlignment) {
    TextAlignment.left = "left";
    TextAlignment.center = "center";
    TextAlignment.right = "right";
})(TextAlignment = exports.TextAlignment || (exports.TextAlignment = {}));
var TextDecoration;
(function (TextDecoration) {
    TextDecoration.none = "none";
    TextDecoration.underline = "underline";
    TextDecoration.lineThrough = "line-through";
})(TextDecoration = exports.TextDecoration || (exports.TextDecoration = {}));
var TextTransform;
(function (TextTransform) {
    TextTransform.none = "none";
    TextTransform.capitalize = "capitalize";
    TextTransform.uppercase = "uppercase";
    TextTransform.lowercase = "lowercase";
})(TextTransform = exports.TextTransform || (exports.TextTransform = {}));
var WhiteSpace;
(function (WhiteSpace) {
    WhiteSpace.normal = "normal";
    WhiteSpace.nowrap = "nowrap";
})(WhiteSpace = exports.WhiteSpace || (exports.WhiteSpace = {}));
var Orientation;
(function (Orientation) {
    Orientation.horizontal = "horizontal";
    Orientation.vertical = "vertical";
})(Orientation = exports.Orientation || (exports.Orientation = {}));
var DeviceOrientation;
(function (DeviceOrientation) {
    DeviceOrientation.portrait = "portrait";
    DeviceOrientation.landscape = "landscape";
    DeviceOrientation.unknown = "unknown";
})(DeviceOrientation = exports.DeviceOrientation || (exports.DeviceOrientation = {}));
var HorizontalAlignment;
(function (HorizontalAlignment) {
    HorizontalAlignment.left = "left";
    HorizontalAlignment.center = "center";
    HorizontalAlignment.right = "right";
    HorizontalAlignment.stretch = "stretch";
})(HorizontalAlignment = exports.HorizontalAlignment || (exports.HorizontalAlignment = {}));
var VerticalAlignment;
(function (VerticalAlignment) {
    VerticalAlignment.top = "top";
    VerticalAlignment.center = "center";
    VerticalAlignment.middle = "middle";
    VerticalAlignment.bottom = "bottom";
    VerticalAlignment.stretch = "stretch";
})(VerticalAlignment = exports.VerticalAlignment || (exports.VerticalAlignment = {}));
var Stretch;
(function (Stretch) {
    Stretch.none = "none";
    Stretch.aspectFill = "aspectFill";
    Stretch.aspectFit = "aspectFit";
    Stretch.fill = "fill";
})(Stretch = exports.Stretch || (exports.Stretch = {}));
var Visibility;
(function (Visibility) {
    Visibility.visible = "visible";
    Visibility.collapse = "collapse";
    Visibility.collapsed = "collapsed";
})(Visibility = exports.Visibility || (exports.Visibility = {}));
var FontAttributes;
(function (FontAttributes) {
    FontAttributes.Normal = 0;
    FontAttributes.Bold = 1;
    FontAttributes.Italic = 1 << 1;
})(FontAttributes = exports.FontAttributes || (exports.FontAttributes = {}));
var DeviceType;
(function (DeviceType) {
    DeviceType.Phone = "Phone";
    DeviceType.Tablet = "Tablet";
})(DeviceType = exports.DeviceType || (exports.DeviceType = {}));
var UpdateTextTrigger;
(function (UpdateTextTrigger) {
    UpdateTextTrigger.focusLost = "focusLost";
    UpdateTextTrigger.textChanged = "textChanged";
})(UpdateTextTrigger = exports.UpdateTextTrigger || (exports.UpdateTextTrigger = {}));
var Accuracy;
(function (Accuracy) {
    Accuracy.any = 300;
    Accuracy.high = 3;
})(Accuracy = exports.Accuracy || (exports.Accuracy = {}));
var Dock;
(function (Dock) {
    Dock.left = "left";
    Dock.top = "top";
    Dock.right = "right";
    Dock.bottom = "bottom";
})(Dock = exports.Dock || (exports.Dock = {}));
var AutocapitalizationType;
(function (AutocapitalizationType) {
    AutocapitalizationType.none = "none";
    AutocapitalizationType.words = "words";
    AutocapitalizationType.sentences = "sentences";
    AutocapitalizationType.allCharacters = "allCharacters";
})(AutocapitalizationType = exports.AutocapitalizationType || (exports.AutocapitalizationType = {}));
var NavigationBarVisibility;
(function (NavigationBarVisibility) {
    NavigationBarVisibility.auto = "auto";
    NavigationBarVisibility.never = "never";
    NavigationBarVisibility.always = "always";
})(NavigationBarVisibility = exports.NavigationBarVisibility || (exports.NavigationBarVisibility = {}));
var AndroidActionBarIconVisibility;
(function (AndroidActionBarIconVisibility) {
    AndroidActionBarIconVisibility.auto = "auto";
    AndroidActionBarIconVisibility.never = "never";
    AndroidActionBarIconVisibility.always = "always";
})(AndroidActionBarIconVisibility = exports.AndroidActionBarIconVisibility || (exports.AndroidActionBarIconVisibility = {}));
var AndroidActionItemPosition;
(function (AndroidActionItemPosition) {
    AndroidActionItemPosition.actionBar = "actionBar";
    AndroidActionItemPosition.actionBarIfRoom = "actionBarIfRoom";
    AndroidActionItemPosition.popup = "popup";
})(AndroidActionItemPosition = exports.AndroidActionItemPosition || (exports.AndroidActionItemPosition = {}));
var IOSActionItemPosition;
(function (IOSActionItemPosition) {
    IOSActionItemPosition.left = "left";
    IOSActionItemPosition.right = "right";
})(IOSActionItemPosition = exports.IOSActionItemPosition || (exports.IOSActionItemPosition = {}));
var ImageFormat;
(function (ImageFormat) {
    ImageFormat.png = "png";
    ImageFormat.jpeg = "jpeg";
})(ImageFormat = exports.ImageFormat || (exports.ImageFormat = {}));
var FontStyle;
(function (FontStyle) {
    FontStyle.normal = "normal";
    FontStyle.italic = "italic";
})(FontStyle = exports.FontStyle || (exports.FontStyle = {}));
var FontWeight;
(function (FontWeight) {
    FontWeight.thin = "100";
    FontWeight.extraLight = "200";
    FontWeight.light = "300";
    FontWeight.normal = "normal";
    FontWeight.medium = "500";
    FontWeight.semiBold = "600";
    FontWeight.bold = "bold";
    FontWeight.extraBold = "800";
    FontWeight.black = "900";
})(FontWeight = exports.FontWeight || (exports.FontWeight = {}));
var BackgroundRepeat;
(function (BackgroundRepeat) {
    BackgroundRepeat.repeat = "repeat";
    BackgroundRepeat.repeatX = "repeat-x";
    BackgroundRepeat.repeatY = "repeat-y";
    BackgroundRepeat.noRepeat = "no-repeat";
})(BackgroundRepeat = exports.BackgroundRepeat || (exports.BackgroundRepeat = {}));
var animationModule;
var AnimationCurve;
(function (AnimationCurve) {
    AnimationCurve.ease = "ease";
    AnimationCurve.easeIn = "easeIn";
    AnimationCurve.easeOut = "easeOut";
    AnimationCurve.easeInOut = "easeInOut";
    AnimationCurve.linear = "linear";
    AnimationCurve.spring = "spring";
    function cubicBezier(x1, y1, x2, y2) {
        animationModule = animationModule || require("ui/animation");
        return new animationModule.CubicBezierAnimationCurve(x1, y1, x2, y2);
    }
    AnimationCurve.cubicBezier = cubicBezier;
})(AnimationCurve = exports.AnimationCurve || (exports.AnimationCurve = {}));
