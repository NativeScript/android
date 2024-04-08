# [8.7.0](https://github.com/NativeScript/android/compare/v8.7.0-rc.3...v8.7.0) (2024-04-08)



# [8.7.0-rc.3](https://github.com/NativeScript/android/compare/v8.6.2...v8.7.0-rc.3) (2024-04-08)


### Bug Fixes

* devtools namespace usage ([#1810](https://github.com/NativeScript/android/issues/1810)) ([5aaac57](https://github.com/NativeScript/android/commit/5aaac5788ff9abf1c043817e87c8e03eb61907c0))
* dts-generator.jar path ([1120a32](https://github.com/NativeScript/android/commit/1120a3258d53f83b7b4dfe7e505234e2b0d6cd2b))
* inspector and globals ([#1811](https://github.com/NativeScript/android/issues/1811)) ([79ebd18](https://github.com/NativeScript/android/commit/79ebd18f308cd86fa98784f14b5c3f5ac39d8c5f))


### Features

* bump ndk to r23c ([#1803](https://github.com/NativeScript/android/issues/1803)) ([3894959](https://github.com/NativeScript/android/commit/3894959e0b4fe31f61cfd9fa70d5e2b04a0f36ac))
* devtools element/network inspectors ([#1808](https://github.com/NativeScript/android/issues/1808)) ([1470796](https://github.com/NativeScript/android/commit/1470796dc506f0d01e94fe117119dc217ff8c909))
* migrate to faster maps and use runtime context ([#1793](https://github.com/NativeScript/android/issues/1793)) ([b248dc4](https://github.com/NativeScript/android/commit/b248dc4038d0c1a6af420447c713bc968431f97e))
* update libzip to 1.10.1 ([#1805](https://github.com/NativeScript/android/issues/1805)) ([ee2e3e0](https://github.com/NativeScript/android/commit/ee2e3e0b87caf3cff4784f1464dd51b2923c6861))
* use node module bindings like the iOS runtime ([#1795](https://github.com/NativeScript/android/issues/1795)) ([643958b](https://github.com/NativeScript/android/commit/643958b6a4c3698567edde3fd03052873b2644dc))
* **WinterCG:** URL & URLSearchParams ([#1801](https://github.com/NativeScript/android/issues/1801)) ([4f3a0d7](https://github.com/NativeScript/android/commit/4f3a0d7f2de5f899779bd0fe9081390e6c4d24b2))


### Reverts

* Version.h changes ([9faa25d](https://github.com/NativeScript/android/commit/9faa25dda197d3da4f694ea59208309bb02e529c))



## [8.6.2](https://github.com/NativeScript/android/compare/v8.6.1...v8.6.2) (2023-10-10)



## [8.6.1](https://github.com/NativeScript/android/compare/v8.6.0...v8.6.1) (2023-10-10)


### Bug Fixes

* copy drawables ([4ff92cb](https://github.com/NativeScript/android/commit/4ff92cb32a954be4c3d32c302e301cef0a4b72a6))



# [8.6.0](https://github.com/NativeScript/android/compare/v8.5.3...v8.6.0) (2023-10-06)


### Bug Fixes

* make jar files readonly prior to loading ([#1790](https://github.com/NativeScript/android/issues/1790)) ([2bcdaf0](https://github.com/NativeScript/android/commit/2bcdaf01fb850db4a982c22c2d792f9493a2a7fa))
* only use project jar files if they are linked ([d23ca94](https://github.com/NativeScript/android/commit/d23ca94ba7c660b26224c57ba6f22085aa99f95c))
* revert namespace change as to not break existing projects ([8b7b59d](https://github.com/NativeScript/android/commit/8b7b59d23d926b696bde3c1031cf3a842a24133d))


### Features

* improved error activity ui ([#1776](https://github.com/NativeScript/android/issues/1776)) ([ee3e354](https://github.com/NativeScript/android/commit/ee3e354f1bec89268daf93086aa6dd24898677b9))
* upgrade client gradle version ([c778c0d](https://github.com/NativeScript/android/commit/c778c0d238c4ba44390f786ba06ab8e51ffb2c97))

## [8.5.4](https://github.com/NativeScript/android/compare/v8.5.3...v8.5.4) (2023-09-27)


### Bug Fixes

* make jar files readonly prior to loading ([#1790](https://github.com/NativeScript/android/issues/1790)) ([14a932a](https://github.com/NativeScript/android/commit/14a932ad2d62c94f2f4e139125835da760dcdd58))


## [8.5.3](https://github.com/NativeScript/android/compare/v8.5.2...v8.5.3) (2023-09-22)


### Bug Fixes

* resolve __postFrameCallback crash on re-scheduling ([6a533ce](https://github.com/NativeScript/android/commit/6a533ce58b22b163d888c51f203be2ef3aa98347))



## [8.5.2](https://github.com/NativeScript/android/compare/v8.5.1...v8.5.2) (2023-08-31)


### Bug Fixes

* __runOnMainThread erase iterator before it can be invalidated ([e811484](https://github.com/NativeScript/android/commit/e81148409046957fceb07b14394baa5bc055286d))
* pull js stack trace from wrapped NativeScriptExceptions ([#1774](https://github.com/NativeScript/android/issues/1774)) ([52b7fa2](https://github.com/NativeScript/android/commit/52b7fa242cb4582701dcebf97aa0f6e0400bcb1a))



## [8.5.1](https://github.com/NativeScript/android/compare/v8.5.0...v8.5.1) (2023-07-24)


### Bug Fixes

* Avoid setting the same property on an ObjectTemplate twice ([9e610c8](https://github.com/NativeScript/android/commit/9e610c8b61a721c04b0a7b8f5167f5b070c5419f))
* Don't access `super` property on implementation object ([d8b8bc0](https://github.com/NativeScript/android/commit/d8b8bc02cf51de021855bb6a39ccd39ce1287304))
* Don't iterate properties in GetImplementedInterfaces ([9dfae65](https://github.com/NativeScript/android/commit/9dfae6589caad31ee50a5946e7369f12505cd955))
* intermediary fix for https://github.com/NativeScript/android/pull/1771 ([32c7abb](https://github.com/NativeScript/android/commit/32c7abb3418b20d1ac2d80c18c5e25a402937628))
* Leave context after Runtime::PrepareV8Runtime() ([cd1d285](https://github.com/NativeScript/android/commit/cd1d2850e5c28d89339a90d783db1f1623957be7))
* memory leak on accessing static interface methods ([88ce2d8](https://github.com/NativeScript/android/commit/88ce2d8b2c2ccf5568d1b95f0bf9df47179f658a))
* memory leak on saving code cache ([6d416a1](https://github.com/NativeScript/android/commit/6d416a11e5b304d8ac2bfb4c48931a22e353df72))
* Update common-runtime-tests-app ([c8db3ca](https://github.com/NativeScript/android/commit/c8db3cab6b3e3ddd41ab94adb8edac46a704cbe9))
* update legacy android version in package.json ([#1744](https://github.com/NativeScript/android/issues/1744)) ([b4ad8e5](https://github.com/NativeScript/android/commit/b4ad8e541ff211055dc5197c7e0b350b6666771f))
* Use Isolate::TryGetCurrent() ([afe026a](https://github.com/NativeScript/android/commit/afe026a29f3e9a1a5e533e4c66fd013f5f7f163c))


* Remove weak callback from __postFrameCallback cache (#1755) ([ff1b979](https://github.com/NativeScript/android/commit/ff1b97975c7fdcd6b2acfb6153ba635d4f420eff)), closes [#1755](https://github.com/NativeScript/android/issues/1755)


### Features

* add support for kotlin 1.8 ([#1765](https://github.com/NativeScript/android/issues/1765)) ([1a928e4](https://github.com/NativeScript/android/commit/1a928e47e733b1d4ba2bb83b4f541e1cac835155))


### Performance Improvements

* avoid unnecessary string copying when calling static properties of interfaces ([8b53d02](https://github.com/NativeScript/android/commit/8b53d022315d4a7fa9d617f431e0364030942d24))


### BREAKING CHANGES

* __startNDKProfiler() and __stopNDKProfiler() global
bindings no longer available.

The NDK profiler was not functional, since nothing in the build process
defined the NDK_PROFILER_ENABLED preprocessor symbol. The start and stop
functions were already no-ops.

* chore: Remove outdated comment

RunMicrotasks no longer exists, it's already been replaced in the code
with PerformMicrotaskCheckpoint.

* chore: Use unique_ptr for NewBackingStore in byte buffers

V8 doc: https://docs.google.com/document/d/1sTc_jRL87Fu175Holm5SV0kajkseGl2r8ifGY76G35k/edit

The V8 usage examples show unique_ptr here; it probably doesn't matter
much, but we don't need the backing store after creating the ArrayBuffer,
and I believe shared_ptr is slightly more overhead than unique_ptr.

For convenience, replace the manual empty deleter for direct buffers with
v8::BackingStore::EmptyDeleter.

* chore: Remove weak finalizer callback from __postFrameCallback()

Weak finalizer callbacks are going away in V8 11.x, so we have to remove
this one. Luckily, a finalizer callback is not necessary - it's never
needed to prevent the frame callback from being collected.

However, a weak callback is not necessary in the first place. We can just
clean up the cache entry after the callback is executed, since it is only
executed once.

Note that the call to erase() destructs the FrameCallbackCacheEntry
instance, making `entry` a dangling pointer, so don't use it after the
erase(). There's probably a safer way to do this, although the way that
first occurred to me (pass the key to the callback instead of the entry,
and then use std::unordered_map::extract()) is not available on
robin_hood::unordered_map.

* fix: Make sure frame callbacks are not ignored

There was a bug where __postFrameCallback() would not always cause the
callback to be called. Without initializing `removed`, sometimes it would
have a nonzero value, so the callback would be ignored.

* chore: Clear callback caches' persistent handles in destructor

Clearing the persistent handles in the destructor makes it a bit easier to
deal with the cache entry's lifetime: they are cleared whenever the entry
is removed from the cache.

We do this for both the main thread callback cache and the frame callback
cache.

Adding a destructor makes the cache entries non-movable. But the only
place where they were moved was when inserting them into the cache anyway.
We can use C++17's try_emplace() method to insert them without having to
move them.

* chore: Construct FrameCallbackCacheEntry with ID

This avoids the situation of forgetting to add an ID to the cache entry.

* chore: Improve usage of unordered_map APIs in CallbackHandlers

This fixes a few places where we can avoid double lookups:

- In RunOnMainThreadFdCallback, we already have a valid iterator, so no
  need to look up the same key again in RemoveKey (this is the only usage
  of RemoveKey, so we can remove it.) (Additionally, we probably want to
  remove the cache entry before throwing a NativeScript exception.)

- In PostFrameCallback and RemoveFrameCallback, we should not do
  contains() immediately followed by find().

* chore: Fix runtime typo

* chore: Ignore main thread and frame callback return values

We don't do anything with the return value from these callbacks, so it's
OK to ignore them and not convert them to a local handle.



# [8.5.0](https://github.com/NativeScript/android/compare/v8.4.0...v8.5.0) (2023-06-27)


### Bug Fixes

* add semicolon after console type ([32259a9](https://github.com/NativeScript/android/commit/32259a90607bdd282d855c3701c7b7c2b203439d))
* always log console messages and uncomment live sync ([c0f5514](https://github.com/NativeScript/android/commit/c0f5514686decc111d8b2e2f93c434141788f298))
* Compile as C++17 ([221a9c2](https://github.com/NativeScript/android/commit/221a9c2c3747c2698238841ea56cf4d837206c20))
* Correctly initialize context in inspector client init() ([0bc0480](https://github.com/NativeScript/android/commit/0bc0480035d01c681765a01b794b48db003be050))
* drain microtasks after devtools message ([4834a2b](https://github.com/NativeScript/android/commit/4834a2b8e3d979323753d1da265ad1faaabbd655))
* Implement console.log inspector with Runtime protocol ([2c4337b](https://github.com/NativeScript/android/commit/2c4337bf5b7c2c00bbcd5b34da1e8c08478fd37a))
* memcpy array data for non-direct java bytebuffers ([#1740](https://github.com/NativeScript/android/issues/1740)) ([1c0214a](https://github.com/NativeScript/android/commit/1c0214a4785d425dc83f9612169fad7f70a28d41))
* multi threading on MethodResolver ([bc8bc52](https://github.com/NativeScript/android/commit/bc8bc5253ff8abecf97bf98462d21fda15757a6a))
* possible infinite loop and memory leak in metadata reader ([#1749](https://github.com/NativeScript/android/issues/1749)) ([c2c8aa8](https://github.com/NativeScript/android/commit/c2c8aa8a5f81c10aee293e14af797e5d1e3fbb4c))
* Re-enable inspector code ([f357ce6](https://github.com/NativeScript/android/commit/f357ce6b9836be109ea5717ba0c09ee01a3f9876))
* refactor console.log implementation a bit ([#1741](https://github.com/NativeScript/android/issues/1741)) ([d3c52cb](https://github.com/NativeScript/android/commit/d3c52cbaae8620d1ba1ed43d61da6df6f9df0f9c))
* remove free of non-owned jni buffer ([81806b3](https://github.com/NativeScript/android/commit/81806b399f42a981a0cf54ab8a4be5712333e938))
* Remove use of DISALLOW_COPY_AND_ASSIGN macro in inspector ([6da1a6b](https://github.com/NativeScript/android/commit/6da1a6bba0d9808568e8d6ae68fb0c71c6271ff1))
* Remove use of V8InspectorPlatform ([1f2b202](https://github.com/NativeScript/android/commit/1f2b202263207de2045bafad5afedf786da788e1))
* Restore DOM and Network callback handlers ([35689a7](https://github.com/NativeScript/android/commit/35689a7462ea86a760705c949dea6f4de4192cf8))
* uncomment abifilters ([f5a5434](https://github.com/NativeScript/android/commit/f5a5434f41d764352de89133d2068b109bd3a54c))
* update js arg conversions ([0640fce](https://github.com/NativeScript/android/commit/0640fcee3bd5429a637f3811fd7f8b4dc8ddf508))
* use min sdk 17 ([2f2358c](https://github.com/NativeScript/android/commit/2f2358c0daf4c476110367432889740c18b49694))


### Features

* add console message type prefixes ([9a10e2b](https://github.com/NativeScript/android/commit/9a10e2b89410c6757bceba705a902060f37d97a1))
* initial support for __inspectorSendEvent ([233b7c3](https://github.com/NativeScript/android/commit/233b7c39c2910e01adc261c4d0d3ddc1d92ef9d6))
* native timer polyfills ([#1733](https://github.com/NativeScript/android/issues/1733)) ([3415e5c](https://github.com/NativeScript/android/commit/3415e5c515b3d73360a87f4f7aec03c44bd2c63c))
* remove old WeakRef polyfill ([97d7465](https://github.com/NativeScript/android/commit/97d7465ca46df7240c4887fffb3167e469179a7a))
* update all archs to use v8 10.3.22 ([a68b057](https://github.com/NativeScript/android/commit/a68b057da208338a83a620e64c0f4fecd90cff59))
* use v8 static ([ee8a521](https://github.com/NativeScript/android/commit/ee8a5213a54d40046993ee61ac652842ccacfbe4))


### Performance Improvements

* refactor arrays marshaling ([84e7ddb](https://github.com/NativeScript/android/commit/84e7ddbe7fd503a3a5f11eed55246138068ca28f))
* remove instantiations and cache runtime in isolate ([f23c1bb](https://github.com/NativeScript/android/commit/f23c1bb5b61c2fcaf7c6961450055bfd4fa4d385))
* update old args converter with array marshaling optimizations and use this converter again tempporarily ([b5218c9](https://github.com/NativeScript/android/commit/b5218c9f5dce1e7c603887f4c9b65f3bd5284aa8))
* use vector pointers for passing parsed method signatures ([aa623b7](https://github.com/NativeScript/android/commit/aa623b795ef38fb023cdef60d15641b8d8bf0338))



# [8.4.0](https://github.com/NativeScript/android/compare/v8.3.0...v8.4.0) (2022-11-30)


### Bug Fixes

* don't assert for sub projects ([b5efdbc](https://github.com/NativeScript/android/commit/b5efdbc5ea11a7f5c73703ddae7485c4782bd8d7))
* handle missing child classes when querying native classes ([#1718](https://github.com/NativeScript/android/issues/1718)) ([c238166](https://github.com/NativeScript/android/commit/c238166af3ce5205b65f2d79a23cfb963c3d60f9))
* JvmField annotated fields ([#1726](https://github.com/NativeScript/android/issues/1726)) ([59da1cb](https://github.com/NativeScript/android/commit/59da1cbc8db888833fc24ea207b7d2f9195985b8)), closes [#1604](https://github.com/NativeScript/android/issues/1604)
* JvmName annotation & Kotlin building from App Resources ([1ba30be](https://github.com/NativeScript/android/commit/1ba30becfed7ba775412d722912a5e4db8c51e51)), closes [#1682](https://github.com/NativeScript/android/issues/1682)
* null data ([3b4a56d](https://github.com/NativeScript/android/commit/3b4a56df86f36a7738cc0089195d20239ce67273))
* RunOnMainThreadCallback & PostFrameCallback ([#1721](https://github.com/NativeScript/android/issues/1721)) ([1ccd033](https://github.com/NativeScript/android/commit/1ccd033194d498b7614f16cc5d9cdcfb6a414f44))
* typed array handling ([#1738](https://github.com/NativeScript/android/issues/1738)) ([00509ee](https://github.com/NativeScript/android/commit/00509eec6b929353a313530932f00757a3ccadb6))
* typedarray & raf ([#1729](https://github.com/NativeScript/android/issues/1729)) ([796b9b1](https://github.com/NativeScript/android/commit/796b9b1fbba1e4cc81cb2098035ea38b3620a983))


### Features

* add class name to native objects ([#1723](https://github.com/NativeScript/android/issues/1723)) ([0c601ae](https://github.com/NativeScript/android/commit/0c601ae4680dcb6bea95f3292f6b0c3b9c1baeb1))
* settings.gradle plugin ([#1731](https://github.com/NativeScript/android/issues/1731)) ([1759ecd](https://github.com/NativeScript/android/commit/1759ecd4071ccbeb60ea129af46e7f019c7523fe))
* upgrade libzip to 1.9.2 ([#1724](https://github.com/NativeScript/android/issues/1724)) ([1dfd38f](https://github.com/NativeScript/android/commit/1dfd38ff0ea3a98b6d35f6f7c53f10ac1a788770))



# [8.3.0](https://github.com/NativeScript/android/compare/v8.2.4...v8.3.0) (2022-07-14)


### Bug Fixes

* apply before-plugins before checking versions ([0df7362](https://github.com/NativeScript/android/commit/0df73625b72adb2fd654091f74e527dd61e3029b))
* Set package version before preReleaseBuild ([12acd7f](https://github.com/NativeScript/android/commit/12acd7f3c1a265c0d8cddef8cb936825d2af7877))


### Features

* runOnMain, postFrameCallback & removeFrameCallback ([#1713](https://github.com/NativeScript/android/issues/1713)) ([bdd0313](https://github.com/NativeScript/android/commit/bdd031317c15d5e7eaa0dfeab2ef599b4eeddc4c))



## [8.2.3](https://github.com/NativeScript/android/compare/v8.2.2...v8.2.3) (2022-06-01)


### Bug Fixes

* direct boot should not crash/ANR ([d2b18d7](https://github.com/NativeScript/android/commit/d2b18d7a8e23af6b27a8e2281e43bf72d715af66))



## [8.2.2](https://github.com/NativeScript/android/compare/v8.2.1...v8.2.2) (2022-03-09)


### Bug Fixes

* app freezing on splash screen ([a61d3c1](https://github.com/NativeScript/android/commit/a61d3c1932ca11b67c88bc1b9a0eb97c9847ee19))



## [8.2.1](https://github.com/NativeScript/android/compare/v8.2.0...v8.2.1) (2022-03-08)


### Bug Fixes

* add back gradle key in package json for the time being ([af925d5](https://github.com/NativeScript/android/commit/af925d50990820c9ca92a9a1a869518dbfd43fad))



# [8.2.0](https://github.com/NativeScript/android/compare/v7.0.1...v8.2.0) (2022-03-08)


### Bug Fixes

* **chrome-devtools:** Elements tab ([e935a61](https://github.com/NativeScript/android/commit/e935a61de5043ae2febd76b34963bcaba7049791)), closes [#1641](https://github.com/NativeScript/android/issues/1641) [#1640](https://github.com/NativeScript/android/issues/1640)
* ensure buildMetadata is done before R8  ([01ad92e](https://github.com/NativeScript/android/commit/01ad92e53d6fcd6448b89004e1d49a66dd2b3254))
* js -> buffer handling ([b49801c](https://github.com/NativeScript/android/commit/b49801cb7df3acd65ecb0a7e688afe2a25be908e))
* load `before-plugins.gradle` in build.gradle to allow to override `androidBuildToolsVersion` and other vars ([d926dd4](https://github.com/NativeScript/android/commit/d926dd4340539ee9223ab9090ab20dd3c253af2a))
* loop ([95ba9f5](https://github.com/NativeScript/android/commit/95ba9f5bd382cbffef2469fca632b69729c5837c))
* material lib as debugImplementation ([c6946fe](https://github.com/NativeScript/android/commit/c6946fe6633ebb851fec35c19c2a64632628396e))
* pending intent api31 flags ([d8781fd](https://github.com/NativeScript/android/commit/d8781fdcbffcf44d49d2df1055421eeecf8316ea))
* update to jsparser ([ea1517c](https://github.com/NativeScript/android/commit/ea1517cbcd3b62037440c42fdcedf54eb2bee035))
* **workers:** invalidate cached isolates on dispose ([#1704](https://github.com/NativeScript/android/issues/1704)) ([67e9daf](https://github.com/NativeScript/android/commit/67e9daf709722b608ef1fb6110d5b708f1819c41))


### Features

* commonGradleProperties ([77585ad](https://github.com/NativeScript/android/commit/77585ad58cceba74d8df69e5838b9f80dc947b49))
* expose `PerformMicrotaskCheckpoint` ([4b58570](https://github.com/NativeScript/android/commit/4b58570fa3c43fc0a0ab73790030a414412f7c19))
* log used androidX lib versions ([#1651](https://github.com/NativeScript/android/issues/1651)) ([528c46f](https://github.com/NativeScript/android/commit/528c46f99dd00859ba38c2671b7a5372d34f6a63))
* make console.time use 3 decimals ([7d9f90b](https://github.com/NativeScript/android/commit/7d9f90bd6c45c9f1fd94904e6b17dd028fff1a11))
* support passing typedArrays as nio buffers ([38230e5](https://github.com/NativeScript/android/commit/38230e5b282cf5ad04d3d655cf0671bc7e576a19))
* working gradle7 ([501f884](https://github.com/NativeScript/android/commit/501f884120d6a884a4e2bb6d4d60312aa7efacff))



7.0.1
==

- [Gradle not respecting kotlinVersion (#1642)](https://github.com/NativeScript/android-runtime/pull/1642)
- [--debug-brk crashing android runtime (#1641)](https://github.com/NativeScript/android-runtime/pull/1641)


7.0.0
==

## Bug Fixes

- [appPath & appResourcePath now passed in (#1635)](https://github.com/NativeScript/android-runtime/pull/1635)
- [Kotlin extension overwriting metadata & metadata duplication (#1633)](https://github.com/NativeScript/android-runtime/pull/1633)
- [Update gradle & kotlin dependancies (#1629)](https://github.com/NativeScript/android-runtime/pull/1629)


6.5.3
==
- [Java 8 compatibility when build with Java 9 (#1625)](https://github.com/NativeScript/android-runtime/pull/1625)
- [metadata crash on startup with Local Notification plugin (#1621)](https://github.com/NativeScript/android-runtime/pull/1621)


6.5.1
==

## Bug Fixes

- [Cleanup of warnings / possible not checked nullpointers (#1610)](https://github.com/NativeScript/android-runtime/pull/1610)

- [[metadata] not rebuilding on json api usage change (#1589)](https://github.com/NativeScript/android-runtime/issues/1589)


6.5.0
==

### No changes

6.4.1
==

## What's New

- [[FR] Customize metadata generation to avoid adding unnecessary unused JS wrappers (#1485)](https://github.com/NativeScript/android-runtime/issues/1485)
- [Provide verbose metadata filtering output (#1583)](https://github.com/NativeScript/android-runtime/issues/1583)
- [Upgrade v8 to 8.0.426.16 (#1579)](https://github.com/NativeScript/android-runtime/issues/1579)
- [Upgrade android gradle plugin to the latest 3.5.3 version (#1564)](https://github.com/NativeScript/android-runtime/issues/1564)

## Bug Fixes

- [Using Android KTX is causing NativeScript app to crash (#1571)](https://github.com/NativeScript/android-runtime/issues/1571)
- [SBG generating conflicting class definitions (#1569)](https://github.com/NativeScript/android-runtime/issues/1569)
- [WebAssembly won't initialize without the debugger (#1558)](https://github.com/NativeScript/android-runtime/issues/1558)
- [Clean up allocations from finalized object links (#1566)](https://github.com/NativeScript/android-runtime/pull/1566)
- [Fix Kotlin Object issue (#1562)](https://github.com/NativeScript/android-runtime/pull/1562)

6.3.1
==

## Bug Fixes

- [Kotlin enum values not visible at runtime (#1560)](https://github.com/NativeScript/android-runtime/issues/1560)
- [Code cache breaks HMR (#1554)](https://github.com/NativeScript/android-runtime/issues/1554)
- [Worker memory leak in android (#1550)](https://github.com/NativeScript/android-runtime/issues/1550)

6.3.0
==

## What's New

- [Upgrade v8 to 7.8.279.19 (#1526)](https://github.com/NativeScript/android-runtime/issues/1526)
- [Restrict Kotlin internal modifier from metadata (#1551)](https://github.com/NativeScript/android-runtime/pull/1551)
- [Restrict Kotlin properties with non-public type (#1552)](https://github.com/NativeScript/android-runtime/issues/1552)

## Bug Fixes

- [Android 9: signal 11 (SIGSEGV), code 1 (SEGV_MAPERR) in libNativeScript.so (#1413)](https://github.com/NativeScript/android-runtime/issues/1413)

6.2.0
==

## What's New
 - [Add Kotlin extension functions support (#1515)](https://github.com/NativeScript/android-runtime/issues/1515)
 - [update dts generator to the latest version (#1506)](https://github.com/NativeScript/android-runtime/pull/1506)
 - [Unify exceptions information in try/catch and __onUncaughtError (#1445)](https://github.com/NativeScript/android-runtime/issues/1445)
 - [Interop between JS Objects and JSONObjects (#1500)](https://github.com/NativeScript/android-runtime/issues/1500)
 - [Collect build analytics (#1501)](https://github.com/NativeScript/android-runtime/issues/1501)
 - [expose the NDK revision used to build the V8 and the Runtime (#1498)](https://github.com/NativeScript/android-runtime/pull/1498)
 - [Upgrade android gradle plugin to the latest 3.5.1 version (#1502)](https://github.com/NativeScript/android-runtime/issues/1502)
 - [support snapshot libs out of the box (#1496)](https://github.com/NativeScript/android-runtime/pull/1496)
 - [Upgrade v8 to 7.7.299.11 (#1478)](https://github.com/NativeScript/android-runtime/issues/1478)

## Bug Fixes
 - [Console.log in worker makes chrome debugging crash (#1511)](https://github.com/NativeScript/android-runtime/issues/1511)
 - [fix searching for merge assets folder on windows (#1503)](https://github.com/NativeScript/android-runtime/pull/1503)
 - [Background job with WorkManager (#1488)](https://github.com/NativeScript/android-runtime/issues/1488)
 - [Deprecated API used in the ErrorActivity is crashing the app (when latest support library is used) (#1494)](https://github.com/NativeScript/android-runtime/issues/1494)

6.1.2
==

## Bug Fixes

- [UI freezes with tns-android 6.1.0 (Android only) (#1479)](https://github.com/NativeScript/android-runtime/issues/1479)

6.1.1
==

## Bug Fixes

- [When using kotlin sometimes the metadata is not existing in the result apk (#1476)](https://github.com/NativeScript/android-runtime/issues/1476)


6.1.0
==

## What's New
 - [Runtime Binding Generator depends on deprecated APIs #1441)](https://github.com/NativeScript/android-runtime/issues/1441)
 - [The runtime depends on accessing a hidden value field (#1458)](https://github.com/NativeScript/android-runtime/issues/1458)
 - [Upgrade v8 to 7.6.303.28 (#1439)](https://github.com/NativeScript/android-runtime/issues/1439)
 - [Unify JS stack trace when exception is thrown #1443](https://github.com/NativeScript/android-runtime/issues/1443)
 - [Update androidSdk, targetSdk and build tools to 29 (#1452)](https://github.com/NativeScript/android-runtime/issues/1452)
 - [Upgrade android gradle plugin to the latest 3.5.0 version (#1456)](https://github.com/NativeScript/android-runtime/issues/1456)
 - [Add initial Kotlin support (#1459)](https://github.com/NativeScript/android-runtime/issues/1459)
 - [Add support for user defined gradle.properties (#1463)](https://github.com/NativeScript/android-runtime/issues/1463)


## Bug Fixes

 - [SIGSEGV in libNativeScript.so on callback from java with console.log when displaying an object. #1366](https://github.com/NativeScript/android-runtime/issues/1366)

6.0.2
==

## What's New

 - [Include x86_64 architecture](https://github.com/NativeScript/android-runtime/issues/1419)

6.0.1
==

## Bug Fixes

- [Arabic and Kurdish characters show as gibberish in console.log() (#1302)](https://github.com/NativeScript/android-runtime/issues/1302)
- [IntentService extending fails at runtime when service is started (#1426)](https://github.com/NativeScript/android-runtime/issues/1426)
- [SBG may fail when parsing big JS files (#1430)](https://github.com/NativeScript/android-runtime/issues/1430)
- [Upgrade android gradle plugin to the latest 3.4.2 version (#1425)](https://github.com/NativeScript/android-runtime/issues/1425)


6.0.0
==

## Breaking Changes

- Exception information in onDiscarderError and onUnhandledError is changed so that `message` contains the exception message and `stackTrace` contains only the stackTrace. In the previous implementation `stackTrace` contained some additional details (including the exception message) and the `message` was something like:

    ```
    The application crashed because of an uncaught exception. You can look at "stackTrace" or "nativeException" for more detailed information about the exception.
    ```

- [The built-in `JSON.stringify` method is used for cross workers communication](https://github.com/NativeScript/android-runtime/issues/1408). Circular object references are no longer supported and attempting to send such object will throw an exception.


## What's New

- [Use the built-in JSON.stringify for cross workers communication (#1411)](https://github.com/NativeScript/android-runtime/pull/1411)
- [Enable AndroidX and Jetifier(#1370)](https://github.com/NativeScript/android-runtime/issues/1370)
- [Upgrade v8 to 7.5.288.22(#1387)](https://github.com/NativeScript/android-runtime/issues/1387)
- [Upgrade android gradle plugin to the latest 3.4.1 version(#1390)](https://github.com/NativeScript/android-runtime/issues/1390)
- [Remove printStackTrace method calls from the source code(#1359)](https://github.com/NativeScript/android-runtime/issues/1359)

## Bug Fixes

- [Improve package.json parsing in SBG (#1407)](https://github.com/NativeScript/android-runtime/pull/1407)
- [Improve error message in SBG class parsing (#1401)](https://github.com/NativeScript/android-runtime/pull/1401)
- [java.lang.NullPointerException in Metadata generator(#13795)](https://github.com/NativeScript/android-runtime/issues/1379)
- [Buffer() is deprecated(#1392)](https://github.com/NativeScript/android-runtime/pull/1392)
- [Warnings when building android(#1396)](https://github.com/NativeScript/android-runtime/issues/1396)
- [No JS stack on discardedError and unhandledError(#1354)](https://github.com/NativeScript/android-runtime/issues/1354)


5.4.0
==

## What's New

- [Upgrade v8 to 7.4.288.25(#1356)](https://github.com/NativeScript/android-runtime/issues/1356)
- [Upgrade the android gradle plugin to the latest 3.4.0 version(#1360)](https://github.com/NativeScript/android-runtime/issues/1360)
- [Enable V8 symbols using from application's package.json file(#1368)](https://github.com/NativeScript/android-runtime/issues/1368)

## Bug Fixes

- [HashMaps improvements - fix for "Attempt to use cleared object reference"(#1345)](https://github.com/NativeScript/android-runtime/issues/1345)
- [Unable to start Service when the application process is killed by the OS(#1347)](https://github.com/NativeScript/android-runtime/pull/1347)
- [gradlew not compatible with sh(#1349)](https://github.com/NativeScript/android-runtime/issues/1349)
- [Memory leak in global.postMessage(#1358)](https://github.com/NativeScript/android-runtime/issues/1358)
- [memory leak java <-> javascript when java returns [] array(#1363)](https://github.com/NativeScript/android-runtime/issues/1363)
- [Bug when reifying some generic classes in the SBG(#1372)](https://github.com/NativeScript/android-runtime/issues/1372)
- [Extending an Android service fails if `onCreate` hasn't been overridden()](https://github.com/NativeScript/android-runtime/issues/1373)

5.3.1
==
## Bug Fixes
 - [5.3 build failing (Android) (#1329)](https://github.com/NativeScript/android-runtime/issues/1329)
 - [fix(build): Correct dependencies of `cleanupAllJars` gradle task (#1338)](https://github.com/NativeScript/android-runtime/pull/1338)

5.3.0
==

## What's New
 - [Upgrade v8 to 7.3.492.25(#1301)](https://github.com/NativeScript/android-runtime/issues/1301)
 - [Upgrade the android gradle plugin to the latest 3.3.2 version(#1304)](https://github.com/NativeScript/android-runtime/issues/1304)
 - [Fail SBG when there's no sbg-bindings.txt file generated(#1286)](https://github.com/NativeScript/android-runtime/issues/1286)
 - [Enable arm64-v8 in app.gradle(#1284)](https://github.com/NativeScript/android-runtime/issues/1284)
 - [Support external buildscript configurations(#1279)](https://github.com/NativeScript/android-runtime/issues/1279)
 - [Refactor SBG to support generics and proper handling of overridable methods(#1322)](https://github.com/NativeScript/android-runtime/issues/1322)

## Bug Fixes
 - [The minSdk version should not be declared in the android manifest file(#1316)](https://github.com/NativeScript/android-runtime/issues/1316)
 - ["Unable to resolve dependency" error when runtime is not build (#1309)](https://github.com/NativeScript/android-runtime/issues/1309)
 - [App crashes after tns debug android --debug-brk and trying to debug with "Step into/over"(#892)](https://github.com/NativeScript/android-runtime/issues/892)
 - [Unable to call plugin's native code if application has been build before adding the plugin(#1293)](https://github.com/NativeScript/android-runtime/issues/1293)
 - [Android build fails when tns-core-modules is updated(#1257)](https://github.com/NativeScript/android-runtime/issues/1257)
 - [Generate better code(#689)](https://github.com/NativeScript/android-runtime/issues/689)

5.2.1
==

## Bug Fixes
 - [Breakpoint stop to hit when you have two open tabs and close one(#1247)](https://github.com/NativeScript/android-runtime/issues/1247)

5.2.0
==

## What's New
 - [Upgrade v8 to 7.1.302.32(#1237)](https://github.com/NativeScript/android-runtime/issues/1237)
 - [Add OnDiscardedError handler(#1245)](https://github.com/NativeScript/android-runtime/issues/1245)
 - [Upgrade the android gradle plugin to the latest 3.3.1 version(#1251)](https://github.com/NativeScript/android-runtime/issues/1251)
 - [Add android X support(#1226)](https://github.com/NativeScript/android-runtime/issues/1226)
 - [Provide a JS helper function on the global object to release the native object wrapped by a JS instance(#1254)](https://github.com/NativeScript/android-runtime/issues/1254)

## Bug Fixes

 - [ClassNotFound exception when calling nested static class with correct argument(#1195)](https://github.com/NativeScript/android-runtime/issues/1195)
 - [If you refresh or close the chrome dev tools window an error will be log in the console (#1202)](https://github.com/NativeScript/android-runtime/issues/1202)
 - [Debug on Android fails when stopped on breakpoint and change in .xml/.css/.html is applied(#1243)](https://github.com/NativeScript/android-runtime/issues/1243)
 - [Upgrade V8 to v7 to fix unstable sort() method(#1176)](https://github.com/NativeScript/android-runtime/issues/1176)
 - [CodeCache option is broken since Android Runtime 4.1.0(#1235)](https://github.com/NativeScript/android-runtime/issues/1235)
 - [Snapshots with ABI splits do not work since Android Runtime 4.1.0(#1234)](https://github.com/NativeScript/android-runtime/issues/1234)

5.1.0
==

## What's New
 - [Add a setting to wrap calls to CallJSMethod in try catch(#1223)](https://github.com/NativeScript/android-runtime/issues/1223)
 - [Support for abstract interface with static methods(#1157)](https://github.com/NativeScript/android-runtime/issues/1157)
 - [Use gradle plugin 3.2.1 instead of 3.2.0(#1209)](https://github.com/NativeScript/android-runtime/pull/1209)
 - [Add a concrete exception when the runtime cannot be found(#1201)](https://github.com/NativeScript/android-runtime/pull/1201)

## Bug Fixes

 - [__extends not working as expected for non native inheritance(#1181)](https://github.com/NativeScript/android-runtime/issues/1181)

5.0.0
==

## What's New
 - [Upgrade v8 to 6.9.427.23(#1168)](https://github.com/NativeScript/android-runtime/issues/1168)
 - [Added support for before-plugins.gradle file applied before plugin(#1183)](https://github.com/NativeScript/android-runtime/pull/1185)
 - [Make JSParser in SBG fail the build when failing(#1152)](https://github.com/NativeScript/android-runtime/issues/1152)
 - [Generate interface names list in SBG in parallel(#1132)](https://github.com/NativeScript/android-runtime/issues/1132)
 - [Upgrade android gradle plugin to 3.2.0(#1147)](https://github.com/NativeScript/android-runtime/issues/1147)

## Bug Fixes

 - [Static Binding Generator fails if class has static properties that are used within the class(#1160)](https://github.com/NativeScript/android-runtime/issues/1160)
 - [Fixing NoClassDefFoundError when using older API(#1164)](https://github.com/NativeScript/android-runtime/pull/1164)

4.2.0
==

## What's New
 - [Upgrade v8 to 6.7.288.46(#1130)](https://github.com/NativeScript/android-runtime/issues/1130)
 - [Static binding generator now uses bundled npm packages(#1096)](https://github.com/NativeScript/android-runtime/issues/1096)
 - [Add gradle dependencies versions in package.json(#1102)](https://github.com/NativeScript/android-runtime/issues/1102)
 - [Introduce a setting for auto catching exceptions when calling JS method native(#1119)](https://github.com/NativeScript/android-runtime/issues/1119)
 - [Make livesync work entirely through named sockets(#932)](https://github.com/NativeScript/android-runtime/issues/932)

## Bug Fixes

 - [Unable to increase minSdk version by modifying AndroidManifest.xml(#1104)](https://github.com/NativeScript/android-runtime/issues/1104)
 - [UTF8 symbols in inspector protocol are not properly encoded(#1116)](https://github.com/NativeScript/android-runtime/issues/1116)
 - [ChromeDevTools: If you close the socket the app will crash on the device/emulator(#1122)](https://github.com/NativeScript/android-runtime/issues/1122)
 - [App tries to write in /data/local/tmp(#828)](https://github.com/NativeScript/android-runtime/issues/828)
 - [Rewrite livesync implementation(#929)](https://github.com/NativeScript/android-runtime/pull/929)

4.1.3
==

## Bug Fixes

 - Use google repository as primary gradle repository

4.1.2
==

## Bug Fixes

 - [Webview Crash With Android 8.1.0 / Chromium 67.0.3396.68 (#1075)](https://github.com/NativeScript/android-runtime/issues/1075)

4.1.1
==

## What's New

 - [Add support for java.nio.HeapByteBuffer to ArrayBuffer conversion(#1060)](https://github.com/NativeScript/android-runtime/issues/1060)
 - [Upgrade Gradle version (wrapper and plugin) to latest(#1054)](https://github.com/NativeScript/android-runtime/issues/1054)
 - [Accessing Native Packages starting with 'in'(#1046)](https://github.com/NativeScript/android-runtime/issues/1046)
 - [tns-android 4.0.1: undefined objects are dumped with console.log making the log unreadable(#1026)](https://github.com/NativeScript/android-runtime/issues/1026)
 - [Support new gradle dependency configuration(#993)](https://github.com/NativeScript/android-runtime/issues/993)
 - [new Date() does not work as expected after time zone change(#961)](https://github.com/NativeScript/android-runtime/issues/961)
 - [Update V8 to latest stable version(#808)](https://github.com/NativeScript/android-runtime/issues/808)
 - [Checking if java class implements java interface(#739)](https://github.com/NativeScript/android-runtime/issues/739)
 - [How to disable console.logs in release builds(#1024)](https://github.com/NativeScript/android-runtime/issues/1024)

## Bug Fixes

 - [Calling java method with incorrect params crashes with a JNI exception (#844)](https://github.com/NativeScript/android-runtime/issues/844)
 - [Chrome DevTools: App crashes after adding new view element(#1051)](https://github.com/NativeScript/android-runtime/issues/1051)
 - [Extraneous classes generated by SBG are not cleaned on rebuild(#904)](https://github.com/NativeScript/android-runtime/issues/904)
 - [Source files have different roots, while using debug command(896)](https://github.com/NativeScript/android-runtime/issues/896)

4.0.1
==

## Bug Fixes

 - [App crash on startup on Android 4.4](https://github.com/NativeScript/android-runtime/issues/999)

4.0.0
==

## Breaking Changes

 - [Rewrite the build script routine to no longer use flavors as the primary mechanism to apply nativescript plugin Android configurations (#890)](https://github.com/NativeScript/android-runtime/issues/890) - **If you see the `All flavors must now belong to a named flavor dimension.` build error, ensure that you are using the latest CLI. Plugins will be built implicitly by the CLI, version 4.0.0-rc or newer.**
 - [Application package outputs are now located at `platforms/android/app/build/outputs/<build-type>/<flavor - if any>/app-<build-type>.apk` (#938)](https://github.com/NativeScript/android-runtime/issues/938)

## What's New

 - [Respect `.jar` and `.aar` libraries from `App_Resources/Android/libs` (#899)](https://github.com/NativeScript/android-runtime/issues/899)
 - [Add user-defined Android project files - `.java`, resources, assets in `App_Resources/Android` (#700)](https://github.com/NativeScript/android-runtime/issues/700) - **Enabled after updating the App_Resources/Android subdirectory structure. Use `tns resources update android` with CLI 4.0.0-rc or newer.**
 - [Console API improvements - file name, line, column support in Chrome DevTools; Objects are expanded to JSON representations (#894) (#884)](https://github.com/NativeScript/android-runtime/pull/894) - **Replaces the console API in the `tns-core-modules`.**
 - [Update Gradle to 4.1 and Android plugin for Gradle to 3.0.1 (#938)](https://github.com/NativeScript/android-runtime/issues/938)

## Bug Fixes

 - [fix: static binding generator creating wrong files, when two extended classes have the same name (#692)](https://github.com/NativeScript/android-runtime/issues/692)

3.4.2
==

## Bug Fixes

 - [fix include gradle flavor generation for plugins with incomplete include.gradle scripts (#937)](https://github.com/NativeScript/android-runtime/pull/937)

3.4.1
==

## Bug Fixes

 - [Want help to access webview document height and cookies (#5243)](https://github.com/NativeScript/NativeScript/issues/5243)
 - [Question about plugin using native lib NS 3.4 (#5254)](https://github.com/NativeScript/NativeScript/issues/5254)

3.4.0
==

## What's New

 - [Android Studio Integration (#876)](https://github.com/NativeScript/android-runtime/issues/876)

## Bug Fixes

 - [Faulty Java class name when extending a class inside a file containing dots (#761)](https://github.com/NativeScript/android-runtime/issues/761)

3.3.1
==

## Bug Fixes

 - [app.gradle applies before other plugin gradle scripts (#878)](https://github.com/NativeScript/android-runtime/issues/878)

3.3.0
==

## Bug Fixes

 - [Provide better error message while parsing js files (#833)](https://github.com/NativeScript/android-runtime/issues/833)
 - [Improve JavaScript Metadata generation (#832)](https://github.com/NativeScript/android-runtime/issues/832)
 - [Improve Error handling incorrectly implementing Java interface (#836)](https://github.com/NativeScript/android-runtime/issues/836)

3.2.0
==

## Bug Fixes

 - [Static binding generator fails when using Webpack + Workers (#778)](https://github.com/NativeScript/android-runtime/issues/778)

3.1.1
==

## Bug Fixes
 - [APKS with ABI split crash on start up (#785)](https://github.com/NativeScript/android-runtime/issues/785)

3.1.0
==

## What's New
 - [Chrome DevTools Elements Tab Support (#746)](https://github.com/NativeScript/android-runtime/issues/746)

3.0.1
==

## Bug Fixes

 - [--debug-brk flag not working (#2741)](https://github.com/NativeScript/nativescript-cli/issues/2741)
 - [Clean app between different versions of application package. Ensure Android 6's AutoBackup feature doesn't restore files for NS apps](https://github.com/NativeScript/android-runtime/pull/771#issue-232247925)

3.0.0
==

## What's New

 - [Chrome DevTools Network Domain (#715)](https://github.com/NativeScript/android-runtime/issues/715)
 - [Chrome DevTools Scope Tab (#713)](https://github.com/NativeScript/android-runtime/issues/713)
 - [Enabling java source code or direct dex generation #663)](https://github.com/NativeScript/android-runtime/issues/663)
 - [Improve Gradle incremental build (#562)](https://github.com/NativeScript/android-runtime/issues/562)


## Bug Fixes

 - [Javascript array not marshalling to Java long[] properly  (#696)](https://github.com/NativeScript/android-runtime/issues/696)

2.5.0-RC
==

## What's New

 - [Error when running on real android device (#628)](https://github.com/NativeScript/android-runtime/issues/628)
 - [Updating v8 to 5.4/5.5 #631)](https://github.com/NativeScript/android-runtime/issues/631)

## Bug Fixes

 - [Can't use npm packages ending with ".js" (#666)](https://github.com/NativeScript/android-runtime/issues/666)
 - [Static binding generator crash build-time: clazz is null causes app to crash (#665)](https://github.com/NativeScript/android-runtime/issues/665)
 - [Decorators aren't respected when extending classes with TypeScript 2.1.4+   (#651)](https://github.com/NativeScript/android-runtime/issues/651)
 - [Samples SDK app crashes (#632)](https://github.com/NativeScript/android-runtime/issues/632)
 - [Missing stack trace on worker errors (#629)](https://github.com/NativeScript/android-runtime/issues/629)
 - [Classes using fields from compileSdk > Platform Sdk on device cause crash when extended in TS (#626)](https://github.com/NativeScript/android-runtime/issues/626)
 - [A failure building in debug and release in series (#649)](https://github.com/NativeScript/android-runtime/issues/649)

2.4.0
==

## What's New

 - [Update the V8 JavaScript Engine to 5.2.361 (97% ES6 support)](http://v8project.blogspot.bg/2016/06/release-52.html)
 - [[Experimental] Generate typings for android.jar and android support libs (--androidTypings) (#605)](https://github.com/NativeScript/android-runtime/pull/605)
 - [[Experimental] Multithreading support enabled with Web Workers API (#532)](https://github.com/NativeScript/android-runtime/issues/532)
 - [Enable enableProguardInReleaseBuilds in build.gradle (#567)](https://github.com/NativeScript/android-runtime/issues/567)
 - [Optimized default apk size (#529)](https://github.com/NativeScript/android-runtime/issues/529)
 - [Improved debug Error Activity (#293)](https://github.com/NativeScript/android-runtime/issues/293)

## Bug Fixes

 - [Build for armv7 and x86 only by default (#614)](https://github.com/NativeScript/android-runtime/issues/614)
 - [Make javascript parsing during build incremental (#572)](https://github.com/NativeScript/android-runtime/issues/572)
 - [App won't launch on Android 22 device (#592)](https://github.com/NativeScript/android-runtime/issues/592)
 - [Metadata isn't created for generated dex files (#552)](https://github.com/NativeScript/android-runtime/issues/552)
 - [App doesn't load with the new custom Application\Activity support (#546)](https://github.com/NativeScript/android-runtime/issues/546)

## Performance

 - [Enable enableProguardInReleaseBuilds in build.gradle (#567)](https://github.com/NativeScript/android-runtime/issues/567)
 - [Optimize default apk size  (#529)](https://github.com/NativeScript/android-runtime/issues/529)

2.3.0
==

## What's New

 - [Extend is not working as previous versions in all cases. (#514)](https://github.com/NativeScript/android-runtime/issues/514)
 - [JS: Binding: Run-time error occured in file: undefined at line: undefined and column: undefined (#443)](https://github.com/NativeScript/android-runtime/issues/443)

## Bug Fixes

 - [Crash when invoking a second (different) signature of an overloaded method (meta generator cache?) (#555)](https://github.com/NativeScript/android-runtime/issues/555)
 - [App crashes when set undefined to reference type field (#306)](https://github.com/NativeScript/android-runtime/issues/306)

2.2.0
==

## What's New

 - [Classes can implement multiple interfaces (#501)](https://github.com/NativeScript/android-runtime/pull/501)

## Performance

 - [Update gradle wrapper version to 2.10, and gradle plugin to 2.1.2 (#516)](https://github.com/NativeScript/android-runtime/pull/516)

2.1
==

## What's New

 - [Implement custom gradle clean (#459)](https://github.com/NativeScript/android-runtime/issues/459)

## Bug Fixes

 - [App crash (#476)](https://github.com/NativeScript/android-runtime/issues/476)
 - [The static binding generator should clean redundant files (#467)](https://github.com/NativeScript/android-runtime/issues/467)
 - [Android builds fail on nativescript 2.0  (#460)](https://github.com/NativeScript/android-runtime/issues/460)
 - [Need to add a Gradle Android.defaultConfig (#454)](https://github.com/NativeScript/android-runtime/issues/454)

## Performance

 - [Initial builds of ng2 apps are slow (#436)](https://github.com/NativeScript/android-runtime/issues/436)

2.0.0
==

## What's New

 - [Android N early developer preview (#378)](https://github.com/NativeScript/android-runtime/issues/378)
 - [[Proposal] Static binding generator specification. (#363)](https://github.com/NativeScript/android-runtime/issues/363)
 - [Android Runtime Support for older Android versions (#357)](https://github.com/NativeScript/android-runtime/issues/357)
 - [Data Marshalling: Support for typed arrays (#65)](https://github.com/NativeScript/android-runtime/issues/65)
 - [Support Android Widgets](https://github.com/NativeScript/android-runtime/issues/69)
 - [Add support for caching already compiled JS code](https://github.com/NativeScript/android-runtime/issues/257)
 - [Additional Intents Crashes app](https://github.com/NativeScript/android-runtime/issues/218)
 - [Enable Multidex support](https://github.com/NativeScript/android-runtime/issues/344)

## Bug Fixes

 - [Wrong object lifecycle management (#382)](https://github.com/NativeScript/android-runtime/issues/382)
 - [CLI can easily fail and blow project up on windows when you have multiple plugins. (#369)](https://github.com/NativeScript/android-runtime/issues/369)
 - [Provide method implementations for partially implemented interfaces. (#259)](https://github.com/NativeScript/android-runtime/issues/259)
 - [Generate metadata for protected interfaces (#236)](https://github.com/NativeScript/android-runtime/issues/236)
 - [Cannot resolve method/constructor signatures when null is passed (#90)](https://github.com/NativeScript/android-runtime/issues/90)
 - [App seems to load up with a white screen on run, but works in debug mode #397](https://github.com/NativeScript/android-runtime/issues/397)

## Performance

 - [Wrong object lifecycle management (#382)](https://github.com/NativeScript/android-runtime/issues/382)

1.7.1
==

## Bug Fixes

 - [Fix application initialization](https://github.com/NativeScript/android-runtime/issues/396)
 - [Fix error activity](https://github.com/NativeScript/android-runtime/issues/395)

1.7.0
==

## What's New
 - [Extendind Application and Activity classes](https://github.com/NativeScript/android-runtime/issues/226)
 - Gradle script improvements

## Bug Fixes

 - [App crashes with "NativeScriptApplication already initialized"](https://github.com/NativeScript/android-runtime/issues/362)
 - [Upgrade to Gradle 1.5.0](https://github.com/NativeScript/android-runtime/issues/375)


1.6.0
==

## What's New

 - [Build common test infrastructure for Android and iOS](https://github.com/NativeScript/android-runtime/issues/68)
 - [New syntax for Java arrays](https://github.com/NativeScript/android-runtime/issues/70)
 - [Improved debugger](https://github.com/NativeScript/android-runtime/issues/112)
 - [Log in the debugger console](https://github.com/NativeScript/android-runtime/issues/145)
 - [Update documentation](https://github.com/NativeScript/android-runtime/issues/290)
 - [Provide support for ARMv8a](https://github.com/NativeScript/android-runtime/issues/297)
 - [Imroved exception handling](https://github.com/NativeScript/android-runtime/issues/300)
 - Gradle script improvements

## Bug Fixes

 - [Fix app crash during debugging (#270)](https://github.com/NativeScript/android-runtime/issues/270)
 - [Fix app hang during array marshalling](https://github.com/NativeScript/android-runtime/issues/320)
 - [Fix incorrect module resolution](https://github.com/NativeScript/android-runtime/issues/334)
 - [Fix app crash during debugging (#338)](https://github.com/NativeScript/android-runtime/issues/338)

1.5.1
==

## What's New

 - [Enable requiring of JSON files (like in Node) (#217)](https://github.com/NativeScript/android-runtime/issues/217)
 - [Revisit the "assert" routine in the JNI part (#221)](https://github.com/NativeScript/android-runtime/issues/221)
 - [Android CallStack  (#228)](https://github.com/NativeScript/android-runtime/issues/228)
 - [error handling introducing c++ exceptions (#277)](https://github.com/NativeScript/android-runtime/pull/277)
 - [Simplify require errors (#287)](https://github.com/NativeScript/android-runtime/issues/287)
 - [Experimental: Support native modules (#291)](https://github.com/NativeScript/android-runtime/issues/291)
 - Gradle script improvements

## Bug Fixes

 - [Print meaningful error when metadata generator fails to reflect a class (#245)](https://github.com/NativeScript/android-runtime/issues/245)

1.5.0
==

## What's New

 - Improved LiveSync
 - [Improved error handling](https://github.com/NativeScript/android-runtime/issues/221)
 - Use Gradle Wrapper
 - Use V8 code cache (experimental)

 ## Bug Fixes

 - Proper handling of HTTP 401 status code
 - [Generate metadata for protected interfaces](https://github.com/NativeScript/android-runtime/issues/236)
 - [Fix loading module with NULL char in it](https://github.com/NativeScript/android-runtime/issues/271)

1.4.0
==

## Bug Fixes

 - [Generated metadata is not updated after initial build until after 'gradle clean' is called (#227)](https://github.com/NativeScript/android-runtime/issues/227)
 - [Incorrect behavior when getting or setting java fields from javascript (#219)](https://github.com/NativeScript/android-runtime/issues/219)
 - [Better handling of package.json main configuration (#190)](https://github.com/NativeScript/android-runtime/issues/190)
 - [Calling non existen ctor crashesh the runtime (#180)](https://github.com/NativeScript/android-runtime/issues/180)
 - [ClassCastException when tries to convert numeric return value of overridden methods (#139)](https://github.com/NativeScript/android-runtime/issues/139)

1.3.0
==

## What's New

 - [Expose public API for NativeScript Companion App for deleting old *.dex files (#187)](https://github.com/NativeScript/android-runtime/issues/187)
 - [Add support for AppBuilder LiveSync (#186)](https://github.com/NativeScript/android-runtime/issues/186)
 - [Create a new template project for Gradle build (#182)](https://github.com/NativeScript/android-runtime/issues/182)
 - [Support ~ path syntax in require (#177)](https://github.com/NativeScript/android-runtime/issues/177)
 - [Enable using the Google Design library (and alike) with "library add" command (#140)](https://github.com/NativeScript/android-runtime/issues/140)
 - [Implement support for CLI live sync feature (#137)](https://github.com/NativeScript/android-runtime/issues/137)
 - [Ahead-of-time generation of binding proxies (#103)](https://github.com/NativeScript/android-runtime/issues/103)
 - [Use pool of arrays for marshalling (#33)](https://github.com/NativeScript/android-runtime/issues/33)

## Bug Fixes

 - [Run after LiveSync starts the last synced app on the device/emulator (#214)](https://github.com/NativeScript/android-runtime/issues/214)
 - [Cannot load module with relative path on Android 6 (#206)](https://github.com/NativeScript/android-runtime/issues/206)
 - [App crashes when call overloaded method of a base class (#203)](https://github.com/NativeScript/android-runtime/issues/203)
 - [Fix file is external to application error on Android M (#185)](https://github.com/NativeScript/android-runtime/issues/185)
 - [App crash during GC (#184)](https://github.com/NativeScript/android-runtime/issues/184)
 - [JNI reference leaks when passing JavaScript arrays (#167)](https://github.com/NativeScript/android-runtime/issues/167)
 - [ArrayBuffer broken (#164)](https://github.com/NativeScript/android-runtime/issues/164)
 - [ClassCastException when tries to convert numeric return value of overridden methods (#139)](https://github.com/NativeScript/android-runtime/issues/139)
 - [Allow debugger reconnects (#136)](https://github.com/NativeScript/android-runtime/issues/136)
 - [__onUncaughtError is not called. (#108)](https://github.com/NativeScript/android-runtime/issues/108)

## Performance

 - [Cache parsed method signature (#181)](https://github.com/NativeScript/android-runtime/issues/181)
 - [Use pool of arrays for marshalling (#33)](https://github.com/NativeScript/android-runtime/issues/33)

# Android Runtime Changelog

1.2.1
==

## What's New

 - Updated android widgets library
 - [Allow verbose logging system property to enable debug messages early in engine bootstrap](https://github.com/NativeScript/android-runtime/issues/111)

## Bug Fixes

 - [Additional null checks in V8 to handle certain possible garbage collection issues](https://github.com/NativeScript/android-runtime/issues/111)
 - Fix sync support in runtime for specific (Samsung) Android devices where run-as is not working
 - [Fix JNI memory leak](https://github.com/NativeScript/android-runtime/issues/167)
 - Fix copy of correct android.jar referenced in project properties


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
