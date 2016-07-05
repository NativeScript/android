var observable = require("data/observable");
var barcodescanner = require("nativescript-barcodescanner");
var dialogs = require("ui/dialogs");
var DemoAppModel = (function (_super) {
  __extends(DemoAppModel, _super);
  function DemoAppModel() {
    _super.call(this);
  }

  DemoAppModel.prototype.doCheckAvailable = function () {
    barcodescanner.available().then(
        function(avail) {
          dialogs.alert({
            title: "Scanning available?",
            message: avail ? "YES" : "NO",
            okButtonText: "OK"
          })
        }
    )
  };

  DemoAppModel.prototype.scan = function (front, flip) {
    barcodescanner.scan({
      cancelLabel: "Stop scanning", // iOS only, default 'Close'
      message: "Go scan something", // Android only, default is 'Place a barcode inside the viewfinder rectangle to scan it.'
      preferFrontCamera: front,     // Android only, default false
      showFlipCameraButton: flip    // Android only, default false (on iOS it's always available)
    }).then(
        function(result) {
          dialogs.alert({
            title: "Scan result",
            message: "Format: " + result.format + ",\nValue: " + result.text,
            okButtonText: "OK"
          })
        },
        function(errorMessage) {
          console.log("No scan. " + errorMessage);
        }
    )
  };

  DemoAppModel.prototype.doScanWithFrontCamera = function () {
    this.scan(true, false);
  };

  DemoAppModel.prototype.doScanWithBackCamera = function () {
    this.scan(false, true);
  };

  DemoAppModel.prototype.doCheckHasCameraPermission = function () {
    barcodescanner.hasCameraPermission().then(
        function(granted) {
          dialogs.alert({
            title: "Permission granted?",
            message: granted ? "YES" : "NO",
            okButtonText: "OK"
          })
        }
    )
  };

  DemoAppModel.prototype.doRequestCameraPermission = function () {
    barcodescanner.requestCameraPermission().then(
        function() {
          console.log("Camera permission requested");
        }
    )
  };

  return DemoAppModel;
})(observable.Observable);
exports.DemoAppModel = DemoAppModel;
exports.mainViewModel = new DemoAppModel();
