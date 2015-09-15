# Android-Runtime
Contains the source code for the NativeScript's Android runtime.

# Main Projects
The repo is structured in the following projects (ordered by dependencies):

* **android-metadata-generator** - generates metadata necessary for the Android Runtime.
* **android-binding-generator** - enables Java & Android types to be dynamically created at runtime. Needed by the `extend` routine.
* **android-runtime** - contains the core logic behind the NativeScript's Android Runtime. This project contains native C++ code and needs the Android NDK to build properly.
* **android-runtime-testapp** - this is a vanilla Android Application, which contains the tests for the runtime project.

# Helper Projects

* **android-static-binding-generator** - build tool that generates bindings based on the user's javascript code
* **android-binding-generator-tester** - contains the tests for the binding-generator project.
* **build/project-template** - this is an empty placeholder Android Application project, used by the [NativeScript CLI](https://github.com/NativeScript/nativescript-cli) when building an Android project.

# Build Prerequisites
The source in this repo is organized in Eclipse projects, using the ADT plugin. Following are the needed software if you need to build the project locally.

* Install the latest [Android SDK](http://developer.android.com/sdk/index.html#Other). Set path sdk.
* Install the [ADT Plugin](http://developer.android.com/tools/sdk/eclipse-adt.html) and the supported Eclipse version as described on the plugin page.
* Download Android API Level 22 through the [SDK Manager](http://developer.android.com/tools/help/sdk-manager.html).
* Download and install the [Android NDK](https://developer.android.com/tools/sdk/ndk/index.html). Set path to ndk.
* Download and install [Gradle](https://gradle.org/) (min version 2.3) Set path to gradle.
* Download Android Support Repository through the Android SDK Manager (needed by gradle)
* Download Build Tools 22 through the Android SDK Manager.

# How to build

* Get the widgets jar, either from [here](/build/project-template-gradle/libs/widgets.jar) or you can build it yourself from [android-widgets repo](https://github.com/NativeScript/android-widgets)
* Copy the android-widgets.jar in the root directory of the runtime.
* Run command 
```Shell
gradle packar -PwidgetsPath=./android-widgets.jar
```
or
```Shell
gradle packar -PwidgetsPath=./widgets.jar
```
depending on the jar you use.
* The result of the build will be in the dist folder.

# How to run tests

* Go to subfolder test-app
* Start an emulator or connect a device.

  ``Note: Keep in mind the device or emulator needs to have an sdcard mounted.``
* Run command
```Shell
gradle runtest
```
