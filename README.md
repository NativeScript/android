# android-runtime
Contains the source code for the NativeScript's Android runtime.

# projects
The repo is structured in the following projects (ordered by dependencies):

* **android-binding-generator** - enables Java & Android types to be dynamically created at runtime. Needed by the `extend` routine.
* **android-runtime** - contains the core logic behind the NativeScript's Android Runtime. This project contains native C++ code and needs the Android NDK to build properly.
* **android-runtime-testapp** - this is a vanilla Android Application, which contains the tests for the runtime project.

# how to build it
The source in this repo is organized in Eclipse projects, using the ADT plugin. Following are the steps needed to build it locally:

* Install the latest [Android SDK](http://developer.android.com/sdk/index.html#Other).
* Install the [ADT Plugin](http://developer.android.com/tools/sdk/eclipse-adt.html) and the supported Eclipse version as described on the plugin page.
* Download Android API Level 21 through the [SDK Manager](http://developer.android.com/tools/help/sdk-manager.html).
* Download and install the [Android NDK](https://developer.android.com/tools/sdk/ndk/index.html).
