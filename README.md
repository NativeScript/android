[![Build Status](https://travis-ci.org/NativeScript/android-runtime.svg?branch=master)](https://travis-ci.org/NativeScript/android-runtime)

# Android-Runtime
Contains the source code for the NativeScript's Android runtime. The android-runtime is a main component used in [NativeScript](https://www.nativescript.org/).

# Main Projects
The repo is structured in the following projects (ordered by dependencies):

* **android-metadata-generator** - generates metadata necessary for the Android Runtime. Located in __android-metadata-generator__.
* **android-binding-generator** - enables Java & Android types to be dynamically created at runtime. Needed by the `extend` routine. Located in __test-app/runtime-binding-generator__.
* **android-runtime** - contains the core logic behind the NativeScript's Android Runtime. This project contains native C++ code and needs the Android NDK to build properly. Located in __test-app/runtime__.
* **android-runtime-testapp** - this is a vanilla Android Application, which contains the tests for the runtime project.  Located in __test-app/app__.

# Helper Projects

* **android-static-binding-generator** - build tool that generates bindings based on the user's javascript code. Located in __android-static-binding-generator__.
* **project-template** - this is an empty placeholder Android Application project, used by the [NativeScript CLI](https://github.com/NativeScript/nativescript-cli) when building an Android project. Located in __build-artifacts/project-template-gradle/__.

# Build Prerequisites
Following are the minimal prerequisites to build the runtime package.

* Install the latest [Android Studio ](https://developer.android.com/studio/index.html).
* Install Android API Level 22, 23, 24, 25 from Android Studio -> Tools -> Android -> SDK Manager
* Android NDK 15c - [NDK Archives](https://developer.android.com/ndk/downloads/older_releases.html) or [NDK Downloads](https://developer.android.com/ndk/downloads/index.html) if you can't find it in the archives.
* Download Android Support Repository through the Android SDK Manager
* Download Build Tools 25.0.2 through the Android SDK Manager.

# How to build

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

# How to run tests

* Go to subfolder test-app after you built the runtime.
* Start an emulator or connect a device.

  ``Note: Keep in mind the device or emulator needs to have an sdcard mounted.``
* Run command
```Shell
gradlew runtest
```
