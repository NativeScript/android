package com.tns.tests.kotlin.properties

import com.tns.tests.kotlin.SimpleKotlinObject

open class KotlinClassWithProperties {

    var mutableProperty = "someMutableProperty"
    val immutableProperty = "someImmutableProperty"

    private var privateMutableProperty = "somePrivateMutableProperty"
    private val privateImmutableProperty = "somePrivateImmutableProperty"

    internal var internalMutableProperty = "someInternalMutableProperty"
    internal val internalImmutableProperty = "someInternalImmutableProperty"

    protected var protectedMutableProperty = "someProtectedMutableProperty"
    protected val protectedImmutableProperty = "someProtectedImmutableProperty"

    var privateSetterProperty = "somePrivateSetterProperty"
        private set(value) {
            field = value
        }

    var mutablePropertyWithComplexType: SimpleKotlinObject = SimpleKotlinObject()

    var isMutableBooleanProperty = true
    var hasSomeData = true;
    var isSomeShit = 2;
}