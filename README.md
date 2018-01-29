# Android Runtime for NativeScript
[![Build Status](https://travis-ci.org/NativeScript/android-runtime.svg?branch=master)](https://travis-ci.org/NativeScript/android-runtime)
[![Waffle.io - NativeScript Android Runtime](https://badge.waffle.io/NativeScript/android-runtime.svg?columns=In%20Progress)](https://waffle.io/NativeScript/android-runtime)

Contains the source code for the NativeScript's Android Runtime. [NativeScript](https://www.nativescript.org/) is a framework which enables developers to write truly native mobile applications for Android and iOS using JavaScript and CSS. Each mobile platform has its own ecosystem and offers completely different development tools and language(s) - Java for Android and Objective C (Swift) for iOS. In order to translate JavaScript code to the corresponding native APIs some kind of proxy mechanism is needed. This is exactly what the "Runtime" parts of NativeScript are responsible for. The Android Runtime may be thought of as "The Bridge" between the JavaScript and Android worlds. A NativeScript application for Android is a standard native package (apk) which besides the JavaScript files embed the runtime as well.

<!-- TOC depthFrom:2 -->

- [Main Projects](#main-projects)
- [Helper Projects](#helper-projects)
- [Architecture Diagram](#architecture-diagram)
- [Build Prerequisites](#build-prerequisites)
- [How to build](#how-to-build)
- [How to run tests](#how-to-run-tests)
- [Get Help](#get-help)

<!-- /TOC -->

## Main Projects
The repo is structured in the following projects (ordered by dependencies):

* [**android-metadata-generator**](android-metadata-generator) - generates metadata necessary for the Android Runtime.
* [**android-binding-generator**](test-app/runtime-binding-generator) - enables Java & Android types to be dynamically created at runtime. Needed by the `extend` routine.
* [**android-runtime**](test-app/runtime) - contains the core logic behind the NativeScript's Android Runtime. This project contains native C++ code and needs the Android NDK to build properly.
* [**android-runtime-testapp**](test-app/app) - this is a vanilla Android Application, which contains the tests for the runtime project.

## Helper Projects

* [**android-static-binding-generator**](android-static-binding-generator) - build tool that generates bindings based on the user's javascript code.
* [**project-template**](build-artifacts/project-template-gradle) - this is an empty placeholder Android Application project, used by the [NativeScript CLI](https://github.com/NativeScript/nativescript-cli) when building an Android project.

## Architecture Diagram
The NativeScript Android Runtime architecture can be summarized in the following diagram. 

![Android Runtime diagram](https://github.com/NativeScript/docs/blob/master/docs/img/ns-runtime-android.png)

For more details on how it works, read the [documentation](https://docs.nativescript.org/runtimes/android/overview). 

## Build Prerequisites
Following are the minimal prerequisites to build the runtime package.

* Install the latest [Android Studio ](https://developer.android.com/studio/index.html).
* Install Android API Level 22, 23, 24, 25 from Android Studio -> Tools -> Android -> SDK Manager
* Android NDK 15c - [NDK Archives](https://developer.android.com/ndk/downloads/older_releases.html) or [NDK Downloads](https://developer.android.com/ndk/downloads/index.html) if you can't find it in the archives.
* Download Android Support Repository through the Android SDK Manager
* Download Build Tools 25.0.2 through the Android SDK Manager.

## How to Build

* Run command 

  Windows: 

  ```Shell
  gradlew
  ```

  Mac/Linux:

  ```Shell
  ./gradlew
  ```

* The build process includes building of the runtime package (both optimized and with unstripped v8 symbol table), as well as all supplementary tools used for the android builds: metadata-generator, binding-generator, metadata-generator, static-binding-generator
* The result of the build will be in the dist folder.

  `Note:` To cut the build time in half and package only the optimized (stripped) version of the runtime package comment out 'tasks.generateRuntimeAar.execute()' in the [build.gradle](https://github.com/NativeScript/android-runtime/blob/v3.0.0-rc.1/build.gradle#L114) script.

## How to Run Tests

* Go to subfolder test-app after you built the runtime.
* Start an emulator or connect a device.

  ``Note: Keep in mind the device or emulator needs to have an sdcard mounted.``
* Run command
```Shell
gradlew runtest
```

## Get Help
Please, use [github issues](https://github.com/NativeScript/android-runtime/issues) strictly for [reporting bugs](CONTRIBUTING.md#reporting-bugs) or [requesting features](CONTRIBUTING.md#requesting-new-features). For general questions and support, check out the [NativeScript community forum](https://discourse.nativescript.org/) or ask our experts in [NativeScript community Slack channel](http://developer.telerik.com/wp-login.php?action=slack-invitation).
