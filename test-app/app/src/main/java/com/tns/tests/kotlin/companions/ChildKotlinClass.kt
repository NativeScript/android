package com.tns.tests.kotlin.companions

class ChildKotlinClass : BaseKotlinClassWithCompanion() {
    fun getStringFromCompanion() = BaseKotlinClassWithCompanion.getString()
}