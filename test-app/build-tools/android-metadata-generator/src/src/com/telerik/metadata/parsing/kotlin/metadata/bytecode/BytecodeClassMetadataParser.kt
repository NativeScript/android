package com.telerik.metadata.parsing.kotlin.metadata.bytecode

import com.telerik.metadata.parsing.NativeClassDescriptor
import com.telerik.metadata.parsing.kotlin.classes.KotlinClassDescriptor
import com.telerik.metadata.parsing.kotlin.metadata.ClassMetadataParser
import kotlin.metadata.KmFunction
import kotlin.metadata.KmProperty
import kotlin.metadata.Visibility
import kotlin.metadata.jvm.KotlinClassMetadata
import kotlin.metadata.visibility
import java.lang.reflect.Modifier
import java.util.stream.Stream

class BytecodeClassMetadataParser : ClassMetadataParser {

    override fun wasKotlinCompanionObject(clazz: NativeClassDescriptor, possibleCompanion: NativeClassDescriptor): Boolean {
        if (clazz !is KotlinClassDescriptor) {
            return false
        }

        val kotlinMetadata = clazz.kotlinMetadata
        if (kotlinMetadata is KotlinClassMetadata.Class) {
            val kmClass = kotlinMetadata.kmClass
            kmClass.companionObject
            val companion = kmClass.companionObject
            val fullCompanionName = clazz.className + "$" + companion
            return possibleCompanion.className == fullCompanionName
        }

        return false
    }

    override fun getKotlinProperties(kotlinMetadata: KotlinClassMetadata): Stream<KmProperty> {
        if (kotlinMetadata is KotlinClassMetadata.Class) {
            val kmClass = kotlinMetadata.kmClass
            return kmClass.properties
                    .stream()
                    .filter {
                        it.visibility == Visibility.PUBLIC || it.visibility == Visibility.PROTECTED
                    }
                    .filter { p ->
                        ((p.getter.visibility == Visibility.PUBLIC || p.getter.visibility == Visibility.PROTECTED)
                                && (p.setter?.visibility == Visibility.PUBLIC || p.setter?.visibility == Visibility.PROTECTED)
                                && !p.name.startsWith("is"))
                    }
        }

        return Stream.empty()
    }

    override fun getKotlinExtensionFunctions(kotlinMetadata: KotlinClassMetadata): Stream<KmFunction> {
        if (kotlinMetadata is KotlinClassMetadata.Class) {
            val kmClass = kotlinMetadata.kmClass

            return kmClass.functions
                    .stream()
                    .filter { isVisibleExtensionFunction(it) }
        } else if (kotlinMetadata is KotlinClassMetadata.FileFacade) {
            val kmClass = kotlinMetadata.kmPackage

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
