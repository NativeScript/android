package com.telerik.metadata.parsing.classes

/**
 * Describes the signature of a native type
 */
interface NativeTypeDescriptor {
    val signature: String

    class PrimitiveType constructor(override val signature: String) : NativeTypeDescriptor

    companion object {
        val VOID: NativeTypeDescriptor = PrimitiveType("V")
        val CHAR: NativeTypeDescriptor = PrimitiveType("C")
        val BOOLEAN: NativeTypeDescriptor = PrimitiveType("Z")
        val BYTE: NativeTypeDescriptor = PrimitiveType("B")
        val SHORT: NativeTypeDescriptor = PrimitiveType("S")
        val INT: NativeTypeDescriptor = PrimitiveType("I")
        val LONG: NativeTypeDescriptor = PrimitiveType("J")
        val FLOAT: NativeTypeDescriptor = PrimitiveType("F")
        val DOUBLE: NativeTypeDescriptor = PrimitiveType("D")
    }
}
