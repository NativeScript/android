package com.telerik.metadata.parsing.kotlin.extensions.bytecode

import com.telerik.metadata.parsing.MetadataInfoAnnotationDescriptor
import com.telerik.metadata.parsing.kotlin.extensions.KotlinExtensionFunctionDescriptor
import com.telerik.metadata.parsing.kotlin.methods.KotlinMethodDescriptor

class KotlinExtensionFunctionBytecodeDescriptor(private val kotlinMethodDescriptor: KotlinMethodDescriptor) : KotlinExtensionFunctionDescriptor {

    override val isSynthetic = kotlinMethodDescriptor.isSynthetic

    override val isStatic = kotlinMethodDescriptor.isStatic

    override val isAbstract = kotlinMethodDescriptor.isAbstract

    override val declaringClass = kotlinMethodDescriptor.declaringClass

    override val name = kotlinMethodDescriptor.name

    override val signature = kotlinMethodDescriptor.signature

    override val argumentTypes = kotlinMethodDescriptor.argumentTypes

    override val returnType = kotlinMethodDescriptor.returnType

    override val metadataInfoAnnotation: MetadataInfoAnnotationDescriptor? = null

    override val isPublic = kotlinMethodDescriptor.isPublic

    override val isInternal = kotlinMethodDescriptor.isInternal

    override val isProtected = kotlinMethodDescriptor.isProtected

    override val isPackagePrivate = kotlinMethodDescriptor.isPackagePrivate

    override val isPrivate = kotlinMethodDescriptor.isPrivate

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other == null || javaClass != other.javaClass) return false

        val that = other as KotlinExtensionFunctionBytecodeDescriptor?

        return kotlinMethodDescriptor == that!!.kotlinMethodDescriptor
    }

    override fun hashCode(): Int {
        return kotlinMethodDescriptor.hashCode()
    }
}
