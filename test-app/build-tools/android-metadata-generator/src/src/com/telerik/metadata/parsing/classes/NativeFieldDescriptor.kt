package com.telerik.metadata.parsing.classes

/**
 * Describes the properties of a native field
 */
interface NativeFieldDescriptor {
    val isPublic: Boolean
    val isProtected: Boolean
    val isFinal: Boolean
    val isStatic: Boolean
    val name: String
    val type: NativeTypeDescriptor
}
