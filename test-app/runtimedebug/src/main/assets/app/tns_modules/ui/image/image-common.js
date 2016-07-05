var dependencyObservable = require("ui/core/dependency-observable");
var view = require("ui/core/view");
var proxy = require("ui/core/proxy");
var imageSource = require("image-source");
var enums = require("ui/enums");
var platform = require("platform");
var utils = require("utils/utils");
var types = require("utils/types");
var SRC = "src";
var IMAGE_SOURCE = "imageSource";
var LOAD_MODE = "loadMode";
var SYNC = "sync";
var ASYNC = "async";
var IMAGE = "Image";
var ISLOADING = "isLoading";
var STRETCH = "stretch";
var AffectsLayout = platform.device.os === platform.platformNames.android ? dependencyObservable.PropertyMetadataSettings.None : dependencyObservable.PropertyMetadataSettings.AffectsLayout;
function onSrcPropertyChanged(data) {
    var image = data.object;
    image._createImageSourceFromSrc();
}
var Image = (function (_super) {
    __extends(Image, _super);
    function Image() {
        _super.apply(this, arguments);
    }
    Object.defineProperty(Image.prototype, "imageSource", {
        get: function () {
            return this._getValue(Image.imageSourceProperty);
        },
        set: function (value) {
            this._setValue(Image.imageSourceProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Image.prototype, "src", {
        get: function () {
            return this._getValue(Image.srcProperty);
        },
        set: function (value) {
            this._setValue(Image.srcProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Image.prototype, "isLoading", {
        get: function () {
            return this._getValue(Image.isLoadingProperty);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Image.prototype, "stretch", {
        get: function () {
            return this._getValue(Image.stretchProperty);
        },
        set: function (value) {
            this._setValue(Image.stretchProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Image.prototype, "loadMode", {
        get: function () {
            return this._getValue(Image.loadModeProperty);
        },
        set: function (value) {
            this._setValue(Image.loadModeProperty, value);
        },
        enumerable: true,
        configurable: true
    });
    Image.prototype._setNativeImage = function (nativeImage) {
    };
    Image.prototype._createImageSourceFromSrc = function () {
        var _this = this;
        var value = this.src;
        if (types.isString(value)) {
            value = value.trim();
            this.imageSource = null;
            this["_url"] = value;
            this._setValue(Image.isLoadingProperty, true);
            var source = new imageSource.ImageSource();
            var imageLoaded = function () {
                var currentValue = _this.src;
                if (!types.isString(_this.src) || value !== currentValue.trim()) {
                    return;
                }
                _this.imageSource = source;
                _this._setValue(Image.isLoadingProperty, false);
            };
            if (utils.isDataURI(value)) {
                var base64Data = value.split(",")[1];
                if (types.isDefined(base64Data)) {
                    if (this.loadMode === SYNC) {
                        source.loadFromBase64(base64Data);
                        imageLoaded();
                    }
                    else if (this.loadMode === ASYNC) {
                        source.fromBase64(base64Data).then(imageLoaded);
                    }
                }
            }
            else if (imageSource.isFileOrResourcePath(value)) {
                if (value.indexOf(utils.RESOURCE_PREFIX) === 0) {
                    var resPath = value.substr(utils.RESOURCE_PREFIX.length);
                    if (this.loadMode === SYNC) {
                        source.loadFromResource(resPath);
                        imageLoaded();
                    }
                    else if (this.loadMode === ASYNC) {
                        this.imageSource = null;
                        source.fromResource(resPath).then(imageLoaded);
                    }
                }
                else {
                    if (this.loadMode === SYNC) {
                        source.loadFromFile(value);
                        imageLoaded();
                    }
                    else if (this.loadMode === ASYNC) {
                        this.imageSource = null;
                        source.fromFile(value).then(imageLoaded);
                    }
                }
            }
            else {
                this.imageSource = null;
                imageSource.fromUrl(value).then(function (r) {
                    if (_this["_url"] === value) {
                        _this.imageSource = r;
                        _this._setValue(Image.isLoadingProperty, false);
                    }
                });
            }
        }
        else if (value instanceof imageSource.ImageSource) {
            this.imageSource = value;
            this._setValue(Image.isLoadingProperty, false);
        }
        else {
            this.imageSource = imageSource.fromNativeSource(value);
            this._setValue(Image.isLoadingProperty, false);
        }
    };
    Image.srcProperty = new dependencyObservable.Property(SRC, IMAGE, new proxy.PropertyMetadata(undefined, dependencyObservable.PropertyMetadataSettings.None, onSrcPropertyChanged));
    Image.imageSourceProperty = new dependencyObservable.Property(IMAGE_SOURCE, IMAGE, new proxy.PropertyMetadata(undefined, dependencyObservable.PropertyMetadataSettings.None));
    Image.isLoadingProperty = new dependencyObservable.Property(ISLOADING, IMAGE, new proxy.PropertyMetadata(false, dependencyObservable.PropertyMetadataSettings.None));
    Image.stretchProperty = new dependencyObservable.Property(STRETCH, IMAGE, new proxy.PropertyMetadata(enums.Stretch.aspectFit, AffectsLayout));
    Image.loadModeProperty = new dependencyObservable.Property(LOAD_MODE, IMAGE, new proxy.PropertyMetadata(SYNC, 0, null, function (value) { return value === SYNC || value === ASYNC; }, null));
    return Image;
}(view.View));
exports.Image = Image;
