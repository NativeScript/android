package com.telerik.metadata.parsing.bytecode.types

import com.telerik.metadata.parsing.NativeTypeDescriptor

import org.apache.bcel.generic.Type

class NativeTypeBytecodeDescriptor(t: Type) : NativeTypeDescriptor {

    override val signature: String = t.signature

    override fun hashCode() = signature.hashCode()

    override fun equals(other: Any?) = other is NativeTypeDescriptor && this.signature == other.signature
}
