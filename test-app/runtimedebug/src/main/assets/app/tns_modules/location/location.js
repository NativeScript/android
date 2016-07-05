var enums = require("ui/enums");
var common = require("./location-common");
var utils = require("utils/utils");
global.moduleMerge(common, exports);
var LocationManager = (function () {
    function LocationManager() {
        this.desiredAccuracy = enums.Accuracy.any;
        this.updateDistance = 0;
        var alm = utils.ad.getApplicationContext().getSystemService(android.content.Context.LOCATION_SERVICE);
        this.androidLocationManager = new AndroidLocationManager(alm);
        this.androidLocationManager.minimumUpdateTime = 200;
    }
    Object.defineProperty(LocationManager.prototype, "android", {
        get: function () {
            return this.androidLocationManager;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(LocationManager.prototype, "ios", {
        get: function () {
            return undefined;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(LocationManager.prototype, "locationListener", {
        get: function () {
            if (!this._locationListener) {
                this._locationListener = new android.location.LocationListener({
                    onLocationChanged: function (location1) {
                        if (this._onLocation) {
                            var location = LocationManager.locationFromAndroidLocation(location1);
                            if (this.maximumAge) {
                                if (location.timestamp.valueOf() + this.maximumAge > new Date().valueOf()) {
                                    this._onLocation(location);
                                }
                            }
                            else {
                                this._onLocation(location);
                            }
                        }
                    },
                    onProviderDisabled: function (provider) {
                    },
                    onProviderEnabled: function (provider) {
                    },
                    onStatusChanged: function (arg1, arg2, arg3) {
                    }
                });
            }
            return this._locationListener;
        },
        enumerable: true,
        configurable: true
    });
    LocationManager.locationFromAndroidLocation = function (androidLocation) {
        var location = new common.Location();
        location.latitude = androidLocation.getLatitude();
        location.longitude = androidLocation.getLongitude();
        location.altitude = androidLocation.getAltitude();
        location.horizontalAccuracy = androidLocation.getAccuracy();
        location.verticalAccuracy = androidLocation.getAccuracy();
        location.speed = androidLocation.getSpeed();
        location.direction = androidLocation.getBearing();
        location.timestamp = new Date(androidLocation.getTime());
        location.android = androidLocation;
        return location;
    };
    LocationManager.androidLocationFromLocation = function (location) {
        var androidLocation = new android.location.Location('custom');
        androidLocation.setLatitude(location.latitude);
        androidLocation.setLongitude(location.longitude);
        if (location.altitude) {
            androidLocation.setAltitude(location.altitude);
        }
        if (location.speed) {
            androidLocation.setSpeed(float(location.speed));
        }
        if (location.direction) {
            androidLocation.setBearing(float(location.direction));
        }
        if (location.timestamp) {
            try {
                androidLocation.setTime(long(location.timestamp.getTime()));
            }
            catch (e) {
                console.error('invalid location timestamp');
            }
        }
        return androidLocation;
    };
    LocationManager.isEnabled = function () {
        var criteria = new android.location.Criteria();
        criteria.setAccuracy(android.location.Criteria.ACCURACY_COARSE);
        var lm = utils.ad.getApplicationContext().getSystemService(android.content.Context.LOCATION_SERVICE);
        var enabledProviders = lm.getProviders(criteria, true);
        return (enabledProviders.size() > 0) ? true : false;
    };
    LocationManager.distance = function (loc1, loc2) {
        if (!loc1.android) {
            loc1.android = LocationManager.androidLocationFromLocation(loc1);
        }
        if (!loc2.android) {
            loc2.android = LocationManager.androidLocationFromLocation(loc2);
        }
        return loc1.android.distanceTo(loc2.android);
    };
    LocationManager.prototype.startLocationMonitoring = function (onLocation, onError, options) {
        var criteria = new android.location.Criteria();
        if (options) {
            if (options.desiredAccuracy) {
                this.desiredAccuracy = options.desiredAccuracy;
            }
            if (options.updateDistance) {
                this.updateDistance = options.updateDistance;
            }
            if (options.minimumUpdateTime) {
                this.androidLocationManager.minimumUpdateTime = options.minimumUpdateTime;
            }
        }
        criteria.setAccuracy((this.desiredAccuracy === enums.Accuracy.high) ? android.location.Criteria.ACCURACY_FINE : android.location.Criteria.ACCURACY_COARSE);
        this.locationListener._onLocation = onLocation;
        this.locationListener._onError = onError;
        this.locationListener.maximumAge = (options && ("number" === typeof options.maximumAge)) ? options.maximumAge : undefined;
        try {
            this.androidLocationManager.manager.requestLocationUpdates(this.androidLocationManager.minimumUpdateTime, this.updateDistance, criteria, this.locationListener, null);
            this.isStarted = true;
        }
        catch (e) {
            if (onError) {
                onError(e);
            }
        }
    };
    LocationManager.prototype.stopLocationMonitoring = function () {
        this.androidLocationManager.manager.removeUpdates(this.locationListener);
        this.isStarted = false;
    };
    Object.defineProperty(LocationManager.prototype, "lastKnownLocation", {
        get: function () {
            var criteria = new android.location.Criteria();
            criteria.setAccuracy((this.desiredAccuracy === enums.Accuracy.high) ? android.location.Criteria.ACCURACY_FINE : android.location.Criteria.ACCURACY_COARSE);
            try {
                var providers = this.androidLocationManager.manager.getProviders(criteria, false);
                var it = providers.iterator();
                var location;
                var tempLocation;
                while (it.hasNext()) {
                    var element = it.next();
                    tempLocation = this.androidLocationManager.manager.getLastKnownLocation(element);
                    if (!location) {
                        location = tempLocation;
                    }
                    else {
                        if (tempLocation.getTime() < location.getTime()) {
                            location = tempLocation;
                        }
                    }
                }
                if (location) {
                    return LocationManager.locationFromAndroidLocation(location);
                }
            }
            catch (e) {
                console.error(e.message);
            }
            return null;
        },
        enumerable: true,
        configurable: true
    });
    LocationManager = __decorate([
        Deprecated
    ], LocationManager);
    return LocationManager;
}());
exports.LocationManager = LocationManager;
var AndroidLocationManager = (function () {
    function AndroidLocationManager(manager) {
        this._manager = manager;
    }
    Object.defineProperty(AndroidLocationManager.prototype, "minimumUpdateTime", {
        get: function () {
            return this._minimumUpdateTime;
        },
        set: function (value) {
            this._minimumUpdateTime = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(AndroidLocationManager.prototype, "manager", {
        get: function () {
            return this._manager;
        },
        enumerable: true,
        configurable: true
    });
    AndroidLocationManager = __decorate([
        Deprecated
    ], AndroidLocationManager);
    return AndroidLocationManager;
}());
exports.AndroidLocationManager = AndroidLocationManager;
