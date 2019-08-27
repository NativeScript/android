package com.tns.tests.kotlin.companions

import com.tns.tests.kotlin.SimpleKotlinObject

class KotlinClassWithNamedCompanion {
    companion object NamedCompanion {
        fun getStringFromNamedCompanion(): String = "testCompanion"
        fun getProvidedStringFromNamedCompanion(providedString: String) = providedString

        fun getSimpleObjectFromNamedCompanion(): SimpleKotlinObject = SimpleKotlinObject()
        fun getProvidedSimpleObjectFromNamedCompanion(providedSimpleObject: SimpleKotlinObject) = providedSimpleObject

        @JvmStatic fun getStringJvmStaticFromNamedCompanion(): String = "testCompanion"
        @JvmStatic fun getProvidedStringJvmStaticFromNamedCompanion(providedString: String) = providedString
    }
}