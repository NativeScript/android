package com.telerik.metadata.parsing.kotlin.metadata.bytecode

import com.telerik.metadata.parsing.NativeClassDescriptor
import com.telerik.metadata.parsing.kotlin.classes.KotlinClassDescriptor
import com.telerik.metadata.parsing.kotlin.metadata.ClassMetadataParser
import kotlinx.metadata.Flag
import kotlinx.metadata.KmFunction
import kotlinx.metadata.KmProperty
import kotlinx.metadata.jvm.KotlinClassMetadata
import java.lang.reflect.Modifier
import java.util.stream.Stream

class BytecodeClassMetadataParser : ClassMetadataParser {

    override fun wasKotlinCompanionObject(clazz: NativeClassDescriptor, possibleCompanion: NativeClassDescriptor): Boolean {
        if (clazz !is KotlinClassDescriptor) {
            return false
        }

        val kotlinMetadata = clazz.kotlinMetadata
        if (kotlinMetadata is KotlinClassMetadata.Class) {
            val kmClass = kotlinMetadata.toKmClass()
            kmClass.companionObject
            val companion = kmClass.companionObject
            val fullCompanionName = clazz.className + "$" + companion
            return possibleCompanion.className == fullCompanionName
        }

        return false
    }

    override fun getKotlinProperties(kotlinMetadata: KotlinClassMetadata): Stream<KmProperty> {
        if (kotlinMetadata is KotlinClassMetadata.Class) {
            val kmClass = kotlinMetadata.toKmClass()
            return kmClass.properties
                    .stream()
                    .filter {
                        Flag.IS_PUBLIC(it.flags) || Flag.IS_PROTECTED(it.flags)
                    }
                    .filter { p ->
                        ((Modifier.isPublic(p.getterFlags) || Modifier.isProtected(p.getterFlags))
                                && (Modifier.isPublic(p.setterFlags) || Modifier.isProtected(p.setterFlags))
                                && !p.name.startsWith("is"))
                    }
        }

        return Stream.empty()
    }

    override fun getKotlinExtensionFunctions(kotlinMetadata: KotlinClassMetadata): Stream<KmFunction> {
        if (kotlinMetadata is KotlinClassMetadata.Class) {
            val kmClass = kotlinMetadata.toKmClass()

            return kmClass.functions
                    .stream()
                    .filter { isVisibleExtensionFunction(it) }
        } else if (kotlinMetadata is KotlinClassMetadata.FileFacade) {
            val kmClass = kotlinMetadata.toKmPackage()

            return kmClass.functions
                    .stream()
                    .filter { isVisibleExtensionFunction(it) }
        }

        return Stream.empty()
    }

    private fun isVisibleExtensionFunction(function: KmFunction): Boolean {
        return function.receiverParameterType != null
    }
}
