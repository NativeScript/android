# Android Runtime Changelog

1.2.0
==

## What's New

 - [Support http cookies in build-in http client (#159)](https://github.com/NativeScript/android-runtime/issues/159)
 - [Implement support for CLI live sync feature (#137)](https://github.com/NativeScript/android-runtime/issues/137)
 - [Rethink 0 day support for new android versions (#86)](https://github.com/NativeScript/android-runtime/issues/86)

## Bug Fixes

 - [Fix ErrorActivity not displayed on uncaught exceptions (#158)](https://github.com/NativeScript/android-runtime/issues/158)
 - [Fix JNI memory leak when resolving classes in metadata reader (#157)](https://github.com/NativeScript/android-runtime/issues/157)
 - [Application crash with JNI ERROR (app bug): local reference table overflow (max=512) (#149)](https://github.com/NativeScript/android-runtime/issues/149)
 - [Cannot set float field (#148)](https://github.com/NativeScript/android-runtime/issues/148)
 - [IndexedPropertySetter not called (#127)](https://github.com/NativeScript/android-runtime/issues/127)
 - [Fix GetDbgPort intent (#117)](https://github.com/NativeScript/android-runtime/issues/117)
 - [Recreating an Activity with fragments on same process crashes the runtime (#96)](https://github.com/NativeScript/android-runtime/issues/96)
 - [Grunt tasks fail on Windows (#61)](https://github.com/NativeScript/android-runtime/issues/61)

## Performance

 - [Optimize Strings marshaling between Java and V8 (#160)](https://github.com/NativeScript/android-runtime/issues/160)
 - [Improve required module loading (#156)](https://github.com/NativeScript/android-runtime/issues/156)
 - [Improve JNI String marshalling (#126)](https://github.com/NativeScript/android-runtime/issues/126)
 - [Cache folder-as-module resolved path (#121)](https://github.com/NativeScript/android-runtime/issues/121) 


## 1.1.0 (2015, June 10)

### New

* Implemented [#58](https://github.com/NativeScript/android-runtime/issues/60) to remove the 3-seconds initial timeout for Debug builds.
* Implemented [#118](https://github.com/NativeScript/android-runtime/issues/118) to replace the MultiDex library with DexClassLoader.
* Started [#103](https://github.com/NativeScript/android-runtime/issues/103) AOT proxy generation to improve startup time and to enable new scenarios like BroadcastReceivers, BackgroundServices and arbitrary Activity types declared in the manifest.

### Fixed

* [#63](https://github.com/NativeScript/android-runtime/issues/63). An issue which prevented users to extend overloaded methods.
* [#64](https://github.com/NativeScript/android-runtime/issues/64). A JNI Crash when calling JS method with char.
* [#113](https://github.com/NativeScript/android-runtime/issues/113). Fixes the extend routine for an Activity.
* [#114](https://github.com/NativeScript/android-runtime/issues/114). Removes the redundant setNativeScriptOverrides method.

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
