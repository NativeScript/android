# Android Runtime for NativeScript
[![Build Status](https://travis-ci.org/NativeScript/android-runtime.svg?branch=master)](https://travis-ci.org/NativeScript/android-runtime)

Contains the source code for the NativeScript's Android Runtime. [NativeScript](https://www.nativescript.org/) is a framework which enables developers to write truly native mobile applications for Android and iOS using JavaScript and CSS. Each mobile platform has its own ecosystem and offers completely different development tools and language(s) - Java for Android and Objective C (Swift) for iOS. In order to translate JavaScript code to the corresponding native APIs some kind of proxy mechanism is needed. This is exactly what the "Runtime" parts of NativeScript are responsible for. The Android Runtime may be thought of as "The Bridge" between the JavaScript and Android worlds. A NativeScript application for Android is a standard native package (apk) which besides the JavaScript files embed the runtime as well.

<!-- TOC depthFrom:2 -->

- [Main Projects](#main-projects)
- [Helper Projects](#helper-projects)
- [SBG vs Runtime Dex Generation](#sbg-vs-runtime-dex-generation)
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

* [**android-metadata-generator**](test-app/build-tools/android-metadata-generator) - generates metadata necessary for the Android Runtime.
* [**android-binding-generator**](test-app/runtime-binding-generator) - enables Java & Android types to be dynamically created at runtime. Needed by the `extend` routine.
* [**android-runtime**](test-app/runtime) - contains the core logic behind the NativeScript's Android Runtime. This project contains native C++ code and needs the Android NDK to build properly.
* [**android-runtime-testapp**](test-app/app) - this is a vanilla Android Application, which contains the tests for the runtime project.

## Helper Projects

* [**android-static-binding-generator**](android-static-binding-generator) - build tool that generates bindings based on the user's javascript code. See [SBG vs Runtime Dex Generation](#sbg-vs-runtime-dex-generation) for the production vs HMR-dev split.
* [**project-template**](build-artifacts/project-template-gradle) - this is an empty placeholder Android Application project, used by the [NativeScript CLI](https://github.com/NativeScript/nativescript-cli) when building an Android project.

## SBG vs Runtime Dex Generation

The Android runtime turns every JavaScript `.extend('com.tns.Foo', Bar, { ... })`
(or `Bar.extend({ ... })`) call into a real Java subclass with a dispatching
proxy. There are **two** code paths that produce that subclass — a build-time
path and a runtime path — and which one runs depends on whether the
`.extend(...)` call site was visible to the Static Binding Generator (SBG)
when the APK was built.

### Build-time path (production / classic CLI)

[**android-static-binding-generator**](android-static-binding-generator) (SBG)
runs over the bundled JS once during `nativescript build android`:

1. SBG parses every JS file the bundle includes and finds every
   `.extend('com.tns.X', BaseClass, { ... })` call (and the modern
   `BaseClass.extend({ ... })` shorthand).
2. For each call it asks
   [**android-binding-generator**](test-app/runtime-binding-generator)'s
   `ProxyGenerator`/`Dump` to emit a `.dex` for a Java class named
   `com.tns.gen.<BaseFqn-with-$-as-_>` (or, for `@JavaProxy(...)`-style
   explicit names, the user-chosen name).
3. The resulting dex files are packaged into the APK alongside the JS
   bundle and listed in metadata so the runtime can find them via the
   classloader.

In production this means the Java class is already present and loadable
the very first time the JS `.extend(...)` call runs. The runtime never
generates a fresh dex.

### Runtime path (HMR-dev / dynamic `.extend`)

When SBG can't see the call at build time, the runtime generates the dex
on demand. This happens in two common shapes:

* **HMR/Vite dev workflow** — the build-time bundle (`bundle.mjs`) is just
  the HMR bootstrap; modules like
  `@nativescript/core/ui/frame/fragment.android.ts` are fetched over HTTP
  at runtime. SBG never sees those `.extend(...)` calls, so no pre-baked
  dex exists.
* **`unnamed-extend` use cases** — `eval`-generated extends, or extends
  whose first argument is computed at runtime, escape the SBG scan even
  in production builds.

The runtime path is wired through
[`com.tns.ClassResolver`](test-app/runtime/src/main/java/com/tns/ClassResolver.java)
→ [`com.tns.DexFactory`](test-app/runtime/src/main/java/com/tns/DexFactory.java):

1. `ClassResolver.resolveClass` first tries `classStorageService.retrieveClass(name)`.
   In production this hits the SBG-generated dex and we're done.
2. On `LookedUpClassNotFound` (typical for HMR), if a `baseClassName` is
   present, `ClassResolver` falls back to `DexFactory.resolveClass(...)`
   which runs the same `ProxyGenerator`/`Dump` pipeline SBG uses — only
   it does it at runtime, writes the dex into the app's per-thumb cache
   under `<dexDir>/<name>.dex`, wraps it in a `.jar`, and loads it via
   `DexClassLoader` (or `BaseDexClassLoader` injection when the
   `injectIntoParentClassLoader` flag is on).
3. Generated proxy class names normalize JVM inner-class `$` to `_`
   (both in `Dump`'s class signature and in `DexFactory`'s
   `loadClass(...)` arg). The actual JVM lookup name is always
   `com.tns.gen.<base-with-$-as-_>` — never `com.tns.gen.<base>$<nested>`.

### Edge cases worth knowing about

* **`$` vs `_` mismatches** — `Class.forName(baseClassName)` requires JVM
  `$` inner-class syntax (`android.app.Application$ActivityLifecycleCallbacks`),
  but `classLoader.loadClass(generatedName)` requires the `_`-normalized
  sibling (`com.tns.gen.android.app.Application_ActivityLifecycleCallbacks`).
  Both `DexFactory.resolveClass` and `DexFactory.findClass` apply the
  normalization in the loadable-name path while preserving `$` for the
  reflective base-class lookup. Removing either normalization
  reintroduces the "Didn't find class
  `com.tns.gen.android.app.Application$ActivityLifecycleCallbacks`"
  ClassNotFoundException for runtime-generated proxies.
* **Cache invalidation** — the runtime dex cache is keyed on a per-build
  `dexThumb` (the runtime regenerates it whenever the JS bundle changes).
  Stale dex from a previous boot is purged in
  `DexFactory.updateDexThumbAndPurgeCache`. Don't bypass the thumb — a
  dex generated against an older base class can crash at method dispatch
  time when the base API drifts.
* **Parent-classloader injection** — Android framework code that calls
  `Class.forName(name)` searches the app's `PathClassLoader`, *not* an
  isolated `DexClassLoader`. When a generated proxy needs to be visible
  to framework reflection (e.g. `FragmentFactory`, `Activity` resolution
  from `AndroidManifest.xml`), construct the `DexFactory` with
  `injectIntoParentClassLoader=true` so its `injectDexIntoClassLoader`
  helper splices the generated jar's `dexElements` onto the parent's
  `pathList`.
* **Don't normalize synthetic module keys** — module registry keys like
  `ns-vendor://...`, `optional:...`, `node:...`, `blob:...` are not
  filesystem paths and must be preserved verbatim through invalidation
  + reload. They are NOT the same kind of "synthetic name" as the
  `com.tns.gen.<...>` class names above and don't go through this dex
  pipeline.

### What to look at when this breaks

* `ClassResolver.java` — the fallback decision between
  `classStorageService` and `DexFactory`.
* `DexFactory.java` — runtime dex generation, cache, and the
  `$`→`_` normalization.
* `ProxyGenerator.java` + `Dump.java` (under `runtime-binding-generator`)
  — what the generated class actually looks like in bytecode.
* `android-static-binding-generator` — what SBG sees (and crucially
  what it *doesn't* see) at build time.

## Architecture Diagram
The NativeScript Android Runtime architecture can be summarized in the following diagram. 

![Android Runtime diagram](https://github.com/NativeScript/docs/blob/master/docs/img/ns-runtime-android.png)

For more details on how it works, read the [documentation](https://v7.docs.nativescript.org/core-concepts/android-runtime/overview). 

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

* Install dependencies from NPM:

  ```Shell
  cd test-app/build-tools/jsparser
  npm install
  ```

* Set environment variables:
  - `JAVA_HOME` such that `$JAVA_HOME/bin/java` points to your Java executable
  - `ANDROID_HOME` pointing to where you have installed the Android SDK
  - `ANDROID_NDK_HOME` pointing to the version of the Android NDK needed for this version of NativeScript

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
gradlew runtests
```

## How to Connect the Inspector

* Install dependencies: run command
```Shell
cd test-app
npm install
```
* Run the test app in Android Studio, or manually in the emulator.
* Run command
```Shell
npx ns debug android --start
```
  (If you instead have the `nativescript` NPM module installed globally,
  you can omit the `npm install` and the `npx` prefix.)
* This will print out a URL, such as
  `devtools://devtools/bundled/inspector.html?ws=localhost:40000`.
  Open this URL in Chrome or Chromium.

## Contribute
We love PRs! Check out the [contributing guidelines](CONTRIBUTING.md). If you want to contribute, but you are not sure where to start - look for [issues labeled `help wanted`](https://github.com/NativeScript/android-runtime/issues?q=is%3Aopen+is%3Aissue+label%3A%22help+wanted%22).


## Misc

* [Implementing additional Chrome DevTools protocol Domains](docs/extending-inspector.md)

## Get Help
Please, use [github issues](https://github.com/NativeScript/android-runtime/issues) strictly for [reporting bugs](CONTRIBUTING.md#reporting-bugs) or [requesting features](CONTRIBUTING.md#requesting-new-features). For general questions and support, check out [Stack Overflow](https://stackoverflow.com/questions/tagged/nativescript) or ask our experts in [NativeScript community Slack channel](http://developer.telerik.com/wp-login.php?action=slack-invitation).
