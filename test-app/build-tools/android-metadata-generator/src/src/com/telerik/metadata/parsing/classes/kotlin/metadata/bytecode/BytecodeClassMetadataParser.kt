package com.telerik.metadata.parsing.classes.kotlin.metadata.bytecode

import com.telerik.metadata.parsing.classes.NativeClassDescriptor
import com.telerik.metadata.parsing.classes.kotlin.metadata.ClassMetadataParser
import com.telerik.metadata.parsing.classes.kotlin.metadata.MetadataAnnotation
import kotlinx.metadata.KmFunction
import kotlinx.metadata.KmProperty
import kotlinx.metadata.jvm.KotlinClassHeader
import kotlinx.metadata.jvm.KotlinClassMetadata
import java.lang.reflect.Modifier
import java.util.stream.Collectors

class BytecodeClassMetadataParser : ClassMetadataParser {

    override fun wasKotlinClass(clazz: NativeClassDescriptor): Boolean {
        return clazz.kotlinClassMetadataAnnotation.isPresent
    }

    override fun wasKotlinCompanionObject(clazz: NativeClassDescriptor, possibleCompanion: NativeClassDescriptor): Boolean {
        val kotlinClassMetadataAnnotationOptional = clazz.kotlinClassMetadataAnnotation
        if (!kotlinClassMetadataAnnotationOptional.isPresent) {
            return false
        }

        val kotlinClassMetadataAnnotation = kotlinClassMetadataAnnotationOptional.get()
        val kotlinClassMetadata = buildKotlinClassMetadata(kotlinClassMetadataAnnotation)

        if (kotlinClassMetadata is KotlinClassMetadata.Class) {
            val kmClass = kotlinClassMetadata.toKmClass()
            val companion = kmClass.companionObject
            val fullCompanionName = clazz.className + "$" + companion
            return possibleCompanion.className == fullCompanionName
        }

        return false
    }

    private fun buildKotlinClassMetadata(kotlinClassMetadataAnnotation: MetadataAnnotation): KotlinClassMetadata? {
        val kotlinClassHeader = KotlinClassHeader(
                kotlinClassMetadataAnnotation.kind,
                kotlinClassMetadataAnnotation.metadataVersion,
                kotlinClassMetadataAnnotation.bytecodeVersion,
                kotlinClassMetadataAnnotation.data1,
                kotlinClassMetadataAnnotation.data2,
                kotlinClassMetadataAnnotation.extraString,
                kotlinClassMetadataAnnotation.packageName,
                kotlinClassMetadataAnnotation.extraInt)

        return KotlinClassMetadata.read(kotlinClassHeader)
    }

    override fun getKotlinProperties(clazz: NativeClassDescriptor): List<KmProperty> {
        val kotlinClassMetadataAnnotationOptional = clazz.kotlinClassMetadataAnnotation
        if (!kotlinClassMetadataAnnotationOptional.isPresent) {
            return emptyList()
        }

        val kotlinClassMetadataAnnotation = kotlinClassMetadataAnnotationOptional.get()
        val kotlinClassMetadata = buildKotlinClassMetadata(kotlinClassMetadataAnnotation)

        if (kotlinClassMetadata is KotlinClassMetadata.Class) {
            val kmClass = kotlinClassMetadata.toKmClass()
            return kmClass.properties
                    .stream()
                    .filter { p ->
                        ((Modifier.isPublic(p.getterFlags) || Modifier.isProtected(p.getterFlags))
                                && (Modifier.isPublic(p.setterFlags) || Modifier.isProtected(p.setterFlags))
                                && !p.name.startsWith("is"))
                    }
                    .collect(Collectors.toList())
        }

        return emptyList()
    }

    override fun getKotlinExtensionFunctions(clazz: NativeClassDescriptor): List<KmFunction> {
        val kotlinClassMetadataAnnotationOptional = clazz.kotlinClassMetadataAnnotation
        if (!kotlinClassMetadataAnnotationOptional.isPresent) {
            return emptyList()
        }

        val kotlinClassMetadataAnnotation = kotlinClassMetadataAnnotationOptional.get()
        val kotlinClassMetadata = buildKotlinClassMetadata(kotlinClassMetadataAnnotation)

        if (kotlinClassMetadata is KotlinClassMetadata.Class) {
            val kmClass = kotlinClassMetadata.toKmClass()

            return kmClass.functions
                    .stream()
                    .filter { isVisibleExtensionFunction(it) }
                    .collect(Collectors.toList())
        } else if (kotlinClassMetadata is KotlinClassMetadata.FileFacade) {
            val kmClass = kotlinClassMetadata.toKmPackage()

            return kmClass.functions
                    .stream()
                    .filter { isVisibleExtensionFunction(it) }
                    .collect(Collectors.toList())
        }

        return emptyList()
    }

    private fun isVisibleExtensionFunction(function: KmFunction): Boolean {
        return function.receiverParameterType != null
    }
}
