package com.telerik.metadata.parsing.kotlin.metadata

import com.telerik.metadata.parsing.NativeClassDescriptor

import kotlin.metadata.KmFunction
import kotlin.metadata.KmProperty
import kotlin.metadata.jvm.KotlinClassMetadata
import java.util.stream.Stream

interface ClassMetadataParser {
    fun wasKotlinCompanionObject(clazz: NativeClassDescriptor, possibleCompanion: NativeClassDescriptor): Boolean
    fun getKotlinProperties(kotlinMetadata: KotlinClassMetadata): Stream<KmProperty>
    fun getKotlinExtensionFunctions(kotlinMetadata: KotlinClassMetadata): Stream<KmFunction>
}
