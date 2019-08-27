package com.tns.tests.kotlin.companions

abstract class BaseKotlinClassWithCompanion {
    companion object {
        public fun getString() = "someString"
    }
}