# Android Runtime for NativeScript
[![Build Status](https://travis-ci.org/NativeScript/android-runtime.svg?branch=master)](https://travis-ci.org/NativeScript/android-runtime)

Contains the source code for the NativeScript's Android Runtime. [NativeScript](https://www.nativescript.org/) is a framework which enables developers to write truly native mobile applications for Android and iOS using JavaScript and CSS. Each mobile platform has its own ecosystem and offers completely different development tools and language(s) - Java for Android and Objective C (Swift) for iOS. In order to translate JavaScript code to the corresponding native APIs some kind of proxy mechanism is needed. This is exactly what the "Runtime" parts of NativeScript are responsible for. The Android Runtime may be thought of as "The Bridge" between the JavaScript and Android worlds. A NativeScript application for Android is a standard native package (apk) which besides the JavaScript files embed the runtime as well.

<!-- TOC depthFrom:2 -->

- [Main Projects](#main-projects)
- [Helper Projects](#helper-projects)
- [Architecture Diagram](#architecture-diagram)
- [Build Prerequisites](#build-prerequisites)
- [How to build](#how-to-build)
- [How to run tests](#how-to-run-tests)
- [Misc](#misc)
- [Get Help](#get-help)

<!-- /TOC -->

## Additional docs
Several Wiki pages describe some internal topics about the runtime [here](https://github.com/NativeScript/android-runtime/wiki)

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

* Install the latest [Android Studio](https://developer.android.com/studio/index.html).
* From the SDK Manager (Android Studio -> Tools -> Android -> SDK Manager) install the following components:
    * Android API Level 23, 24, 25, 26, 27
    * Android NDK 
    * Android Support Repository
    * Download Build Tools 
    * CMake
    * LLDB
* Clone this repository as well as the submodules: `git clone --recurse-submodules git@github.com:NativeScript/android-runtime.git`

## Working with the Runtime in Android Studio

* Open the test-app folder in Android Studio. It represents a valid Android project and you are able to build and run a test application working with the Runtime from the source.

Note: You might need to run the Android Studio from the command line in order to preserve the environment variables. This is in case you get errors like "missing npm" if starting the studio the usual way.

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
## Contribute
We love PRs! Check out the [contributing guidelines](CONTRIBUTING.md). If you want to contribute, but you are not sure where to start - look for [issues labeled `help wanted`](https://github.com/NativeScript/android-runtime/issues?q=is%3Aopen+is%3Aissue+label%3A%22help+wanted%22).


## Misc

* [Implementing additional Chrome DevTools protocol Domains](docs/extending-inspector.md)

## Get Help
Please, use [github issues](https://github.com/NativeScript/android-runtime/issues) strictly for [reporting bugs](CONTRIBUTING.md#reporting-bugs) or [requesting features](CONTRIBUTING.md#requesting-new-features). For general questions and support, check out [Stack Overflow](https://stackoverflow.com/questions/tagged/nativescript) or ask our experts in [NativeScript community Slack channel](http://developer.telerik.com/wp-login.php?action=slack-invitation).
