# Android Runtime Changelog

## 1.0.0 (2015, April 29)

### New

* Updated the V8 version to 4.1.0.27.
* Re-implemented debugger support (no more spontaneous dead locks).

### Fixed

* An issue with the error reporting routine.

### Breaking Changes

* Renamed global functions:
	* `__log` (was `Log`)
	* `__debugbreak` (was `waitForDebugger`)
	* `__enableVerboseLogging` (was `enableVerboseLogging`)
	* `__disableVerboseLogging` (was `disableVerboseLogging`)
	* `__exit` (was `fail`)

## 0.10.0 (2015, April 17)

### New

* Added Dynamic Generator for binding proxies. This boosts the initial loading time, especially on Android 5.0+ devices.
* Added several optimization techniques, which further optimize the loading time and the overall performance.
* Improved the error reporting mechanism for Debug builds.
* Added support for package.json and index.js for bootstrapping an application.

### Breaking Changes

  * Removed the simulated property-like support for Android types. E.g. the `android.content.Intent.getAction()` previously was accessible like `android.content.Intent.Action`. This is no longer valid as it contradicts with the Android APIs.
  * Changed the way `extend` constructs work
```javascript
// WRONG
var handler = new android.os.Handler.extend({...})();

// CORRECT
var handlerType = android.os.Handler.extend({...});
var handler = new handlerType();
```
  * The directory structure in the `assets` folder has changed. The `tns_modules` directory is now within the `assets/app` one. To migrate older CLI projects to the new structure simply move the content of the inner app folder one level up:

####Previous structure:
```
|--app
|--|--app
|--|--|--bootstrap.js
|--|--|--myFile.js
|--|--tns_modules
```

####New structure:
```
|--app
|--|--bootstrap.js
|--|--myFile.js
|--|--tns_modules
```
