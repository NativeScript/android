package com.telerik.metadata.parsing.classes.kotlin.properties

import com.telerik.metadata.parsing.classes.NativeMethodDescriptor

/**
 * Describes the properties of a Kotlin property
 */
interface KotlinPropertyDescriptor {
    val name: String
    val getterMethod: NativeMethodDescriptor?
    val setterMethod: NativeMethodDescriptor?
}
