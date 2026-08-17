package com.telerik.metadata.parsing.kotlin.methods

import com.telerik.metadata.parsing.bytecode.methods.NativeMethodBytecodeDescriptor
import com.telerik.metadata.parsing.kotlin.classes.KotlinClassDescriptor
import kotlin.metadata.KmDeclarationContainer
import kotlin.metadata.Visibility
import kotlin.metadata.jvm.JvmMethodSignature
import kotlin.metadata.jvm.KotlinClassMetadata
import kotlin.metadata.jvm.getterSignature
import kotlin.metadata.jvm.setterSignature
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
                .firstOrNull { it.signature.matches(method) }
        if (function != null) {
            return function.visibility == Visibility.INTERNAL
        }

        // Property accessors are absent from `functions`, so without this an internal property's
        // getter and setter are treated as public and stay reachable from JS under their mangled
        // names. An accessor can also narrow the property's visibility on its own.
        for (property in kotlinDeclarationContainer.properties) {
            if (property.getterSignature.matches(method)) {
                return property.visibility == Visibility.INTERNAL || property.getter.visibility == Visibility.INTERNAL
            }

            if (property.setterSignature.matches(method)) {
                return property.visibility == Visibility.INTERNAL || property.setter?.visibility == Visibility.INTERNAL
            }
        }

        return false
    }

    private fun JvmMethodSignature?.matches(method: Method): Boolean {
        return this != null && name == method.name && descriptor == method.signature
    }
}