package com.telerik.metadata.parsing.classes.kotlin.extensions.bytecode

import com.telerik.metadata.parsing.classes.MetadataInfoAnnotationDescriptor
import com.telerik.metadata.parsing.classes.kotlin.extensions.KotlinExtensionFunctionDescriptor
import com.telerik.metadata.parsing.classes.NativeMethodDescriptor

class KotlinExtensionFunctionBytecodeDescriptor(private val m: NativeMethodDescriptor) : KotlinExtensionFunctionDescriptor {

    override val isPublic = m.isPublic

    override val isProtected = m.isProtected

    override val isSynthetic = m.isSynthetic

    override val isStatic = m.isStatic

    override val isAbstract = m.isAbstract

    override val declaringClass = m.declaringClass

    override val name = m.name

    override val signature = m.signature

    override val argumentTypes = m.argumentTypes

    override val returnType = m.returnType

    override val metadataInfoAnnotation: MetadataInfoAnnotationDescriptor? = null

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other == null || javaClass != other.javaClass) return false

        val that = other as KotlinExtensionFunctionBytecodeDescriptor?

        return m == that!!.m
    }

    override fun hashCode(): Int {
        return m.hashCode()
    }
}
