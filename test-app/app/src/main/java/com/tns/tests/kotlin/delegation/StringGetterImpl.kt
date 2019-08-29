package com.tns.tests.kotlin.delegation

class StringGetterImpl: StringGetter {
    override fun getString(): String = "some string"
}