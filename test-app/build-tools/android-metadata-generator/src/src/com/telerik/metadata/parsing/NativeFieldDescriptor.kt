package com.telerik.metadata.parsing

/**
 * Describes the properties of a native field
 */
interface NativeFieldDescriptor : AccessModifiable, NativeDescriptor {
    val isFinal: Boolean
    val isStatic: Boolean
    val name: String
    val type: NativeTypeDescriptor
}
