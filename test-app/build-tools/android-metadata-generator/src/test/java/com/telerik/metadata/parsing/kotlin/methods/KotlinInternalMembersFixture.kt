package com.telerik.metadata.parsing.kotlin.methods

@Suppress("unused")
class KotlinInternalMembersFixture {

    var publicProperty: String = ""

    internal var internalProperty: String = ""

    var internalSetterProperty: String = ""
        internal set

    fun publicFunction(): String = publicProperty

    internal fun internalFunction(): String = internalProperty
}
