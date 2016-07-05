var defModule = require("location");
var defaultGetLocationTimeout = 20000;
var Location = (function () {
    function Location() {
    }
    Location = __decorate([
        Deprecated
    ], Location);
    return Location;
}());
exports.Location = Location;
exports.getLocation = function (options) {
    var timerId;
    var locationManager = new defModule.LocationManager();
    if (options && (0 === options.timeout)) {
        return new Promise(function (resolve, reject) {
            var location = locationManager.lastKnownLocation;
            if (location) {
                if (options && ("number" === typeof options.maximumAge)) {
                    if (location.timestamp.valueOf() + options.maximumAge > new Date().valueOf()) {
                        resolve(location);
                    }
                    else {
                        reject(new Error("timeout is 0 and last known location is older than maximumAge"));
                    }
                }
                else {
                    resolve(location);
                }
            }
            else {
                reject(new Error("timeout is 0 and no known location found"));
            }
        });
    }
    return new Promise(function (resolve, reject) {
        if (!defModule.LocationManager.isEnabled()) {
            return reject(new Error("Location service is disabled"));
        }
        var timer = require("timer");
        locationManager.startLocationMonitoring(function (location) {
            if (options && ("number" === typeof options.maximumAge)) {
                if (location.timestamp.valueOf() + options.maximumAge > new Date().valueOf()) {
                    locationManager.stopLocationMonitoring();
                    if ("undefined" !== typeof timerId) {
                        timer.clearTimeout(timerId);
                    }
                    resolve(location);
                }
            }
            else {
                locationManager.stopLocationMonitoring();
                if ("undefined" !== typeof timerId) {
                    timer.clearTimeout(timerId);
                }
                resolve(location);
            }
        }, function (error) {
            console.error('Location error received: ' + error);
            locationManager.stopLocationMonitoring();
            if ("undefined" !== typeof timerId) {
                timer.clearTimeout(timerId);
            }
            reject(error);
        }, options);
        if (options && ("number" === typeof options.timeout)) {
            timerId = timer.setTimeout(function () {
                locationManager.stopLocationMonitoring();
                reject(new Error("timeout searching for location"));
            }, options.timeout || defaultGetLocationTimeout);
        }
    });
};
