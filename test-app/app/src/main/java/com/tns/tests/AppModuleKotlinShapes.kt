package com.tns.tests

// Fixtures for testAppModuleBindings.js: app-module Kotlin types whose bindings need more
// than the generator's Java view of them.

// Kotlin adds a synthetic constructor taking a DefaultConstructorMarker for the default.
abstract class AppModuleKotlinDefaultArgument(val label: String = "kotlin default") {
    abstract fun describe(): String
}

abstract class AppModuleKotlinOverloads @JvmOverloads constructor(val label: String = "kotlin overloads", val count: Int = 1) {
    abstract fun describe(): String
}

abstract class AppModuleKotlinSelfTyped<T : AppModuleKotlinSelfTyped<T>> {
    abstract fun self(): T

    abstract fun describe(): String
}

interface AppModuleKotlinTwoDefaults {
    fun name(): String

    fun first(): String = "kotlin first"

    fun second(): String = "kotlin second"
}

// Java callers, which report an exception a call throws by its class name.
object AppModuleKotlinShapesCaller {
    @JvmStatic
    fun first(target: AppModuleKotlinTwoDefaults): String = try {
        target.first()
    } catch (e: Throwable) {
        e.javaClass.name
    }

    @JvmStatic
    fun second(target: AppModuleKotlinTwoDefaults): String = try {
        target.second()
    } catch (e: Throwable) {
        e.javaClass.name
    }
}
