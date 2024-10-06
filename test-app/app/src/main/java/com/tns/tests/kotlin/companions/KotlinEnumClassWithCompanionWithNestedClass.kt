package com.tns.tests.kotlin.companions

import com.tns.tests.kotlin.SimpleKotlinObject


enum class KotlinEnumClassWithCompanionWithNestedClass {
    TEST_ENTRY;

    companion object {
        fun getStringFromCompanion(): String = "testCompanion"
        fun getProvidedStringFromCompanion(providedString: String) = providedString

        fun getSimpleObjectFromCompanion(): SimpleKotlinObject = SimpleKotlinObject()
        fun getProvidedSimpleObjectFromCompanion(providedSimpleObject: SimpleKotlinObject) =
            providedSimpleObject

        @JvmStatic
        fun getStringJvmStaticFromCompanion(): String = "testCompanion"

        @JvmStatic
        fun getProvidedStringJvmStaticFromCompanion(providedString: String) = providedString

        class TestClass {

        }
    }

}
