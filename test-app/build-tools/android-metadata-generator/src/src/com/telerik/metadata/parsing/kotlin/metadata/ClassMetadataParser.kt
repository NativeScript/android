package com.telerik.metadata.parsing.kotlin.metadata

import com.telerik.metadata.parsing.NativeClassDescriptor

import kotlinx.metadata.KmFunction
import kotlinx.metadata.KmProperty
import kotlinx.metadata.jvm.KotlinClassMetadata
import java.util.stream.Stream

interface ClassMetadataParser {
    fun wasKotlinCompanionObject(clazz: NativeClassDescriptor, possibleCompanion: NativeClassDescriptor): Boolean
    fun getKotlinProperties(kotlinMetadata: KotlinClassMetadata): Stream<KmProperty>
    fun getKotlinExtensionFunctions(kotlinMetadata: KotlinClassMetadata): Stream<KmFunction>
}
