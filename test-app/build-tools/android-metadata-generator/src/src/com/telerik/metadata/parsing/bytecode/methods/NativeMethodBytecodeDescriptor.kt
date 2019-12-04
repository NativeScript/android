package com.telerik.metadata.parsing.bytecode.methods

import com.telerik.metadata.parsing.MetadataInfoAnnotationDescriptor
import com.telerik.metadata.parsing.NativeClassDescriptor
import com.telerik.metadata.parsing.NativeMethodDescriptor
import com.telerik.metadata.parsing.NativeTypeDescriptor
import com.telerik.metadata.parsing.bytecode.types.NativeTypeBytecodeDescriptor
import org.apache.bcel.classfile.Method

abstract class NativeMethodBytecodeDescriptor(private val m: Method, override val declaringClass: NativeClassDescriptor) : NativeMethodDescriptor {

    override val isSynthetic = m.isSynthetic

    override val isStatic = m.isStatic

    override val isAbstract = m.isAbstract

    override val name: String = m.name

    override val signature: String = m.signature

    override val argumentTypes: Array<NativeTypeDescriptor>
        get() {
            val ts = m.argumentTypes
            return Array(ts.size) {
                NativeTypeBytecodeDescriptor(ts[it])
            }
        }

    override val returnType = NativeTypeBytecodeDescriptor(m.returnType)

    override val metadataInfoAnnotation: MetadataInfoAnnotationDescriptor? = null

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other == null || javaClass != other.javaClass) return false

        return other is NativeMethodBytecodeDescriptor && m == other.m
    }

    override fun hashCode(): Int {
        return m.hashCode()
    }
}
