package com.tns.tests

// Fixture for testAppModuleBindings.js. It only compiles with the rest of the build, as
// NativeScriptApplication is generated from JS, and has a file of its own because a Kotlin
// source that does not compile on its own is left out whole.
open class AppModuleKotlinNeedsBuild {
    open fun application(): com.tns.NativeScriptApplication? = null

    open fun describe(): String = "needs build"
}
