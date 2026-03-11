package com.telerik.metadata.parsing.kotlin.methods

import com.telerik.metadata.parsing.bytecode.methods.NativeMethodBytecodeDescriptor
import com.telerik.metadata.parsing.kotlin.classes.KotlinClassDescriptor
import kotlin.metadata.KmDeclarationContainer
import kotlin.metadata.Visibility
import kotlin.metadata.jvm.KotlinClassMetadata
import kotlin.metadata.jvm.signature
import kotlin.metadata.visibility
import org.apache.bcel.classfile.Method

class KotlinMethodDescriptor(private val method: Method, private val originClass: KotlinClassDescriptor,
                             override val isPackagePrivate: Boolean
) : NativeMethodBytecodeDescriptor(method, originClass) {

    override val isPublic by lazy {
        method.isPublic && !isInternal
    }

    override val isProtected by lazy {
        method.isProtected && !isInternal
    }

    override val isPrivate by lazy {
        method.isPrivate && !isInternal
    }

    override val isInternal by lazy {
        return@lazy when (val kotlinMetadata = originClass.kotlinMetadata) {
            is KotlinClassMetadata.Class -> checkIfMethodIsInternal(method, kotlinMetadata.kmClass)
            is KotlinClassMetadata.FileFacade -> checkIfMethodIsInternal(method,
                kotlinMetadata.kmPackage
            )
            is KotlinClassMetadata.SyntheticClass -> false
            is KotlinClassMetadata.MultiFileClassFacade -> false
            is KotlinClassMetadata.MultiFileClassPart -> checkIfMethodIsInternal(method,
                kotlinMetadata.kmPackage
            )
            is KotlinClassMetadata.Unknown -> false
            null -> false
        }
    }

    private fun checkIfMethodIsInternal(method: Method, kotlinDeclarationContainer: KmDeclarationContainer): Boolean {
        val function = kotlinDeclarationContainer
                .functions
                .firstOrNull {
                    it.signature != null && it.signature!!.name == method.name && it.signature!!.descriptor == method.signature
                }
        return if (function != null) function.visibility == Visibility.INTERNAL else false
    }
}