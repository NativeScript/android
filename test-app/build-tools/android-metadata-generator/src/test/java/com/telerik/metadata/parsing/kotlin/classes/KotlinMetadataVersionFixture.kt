package com.telerik.metadata.parsing.kotlin.classes

class KotlinMetadataVersionFixture(val greeting: String) {

    var counter: Int = 0

    fun greet(name: String): String = "$greeting, $name"
}
