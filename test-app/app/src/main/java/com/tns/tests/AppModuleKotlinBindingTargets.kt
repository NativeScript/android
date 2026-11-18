package com.tns.tests

// Fixtures for testAppModuleBindings.js: Kotlin types of the app module.

interface AppModuleKotlinBindingTarget {
    fun describe(): String

    interface Nested {
        fun describe(): String
    }
}

open class AppModuleKotlinBase {
    open fun describe(): String = "kotlin base"
}

interface AppModuleKotlinOnJava : AppModuleBindingTarget

// The JVM name of the internal member carries the build variant's module name, so JS
// subclasses of this class are never prebuilt.
abstract class AppModuleKotlinAbstract {
    internal abstract fun internalDescribe(): String

    abstract fun describe(): String
}

@JvmInline
value class AppModuleKotlinCount(val value: Int)

// Functions taking a value class get JVM names that are not Java identifiers, so a binding
// cannot implement the abstract ones: JS implementations of these types are never prebuilt.
abstract class AppModuleKotlinValueAbstract {
    abstract fun count(count: AppModuleKotlinCount): Int

    abstract fun describe(): String
}

interface AppModuleKotlinValueTarget {
    fun count(count: AppModuleKotlinCount): Int

    fun describe(): String
}

// An open member with such a name is not overridden, so JS subclasses are prebuilt.
open class AppModuleKotlinValueOpen {
    open fun count(count: AppModuleKotlinCount): Int = count.value

    open fun describe(): String = "kotlin value open"
}
