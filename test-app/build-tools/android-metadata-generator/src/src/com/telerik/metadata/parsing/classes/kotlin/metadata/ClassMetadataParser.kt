package com.telerik.metadata.parsing.classes.kotlin.metadata

import com.telerik.metadata.parsing.classes.NativeClassDescriptor

import kotlinx.metadata.KmFunction
import kotlinx.metadata.KmProperty

interface ClassMetadataParser {
    fun wasKotlinClass(clazz: NativeClassDescriptor): Boolean
    fun wasKotlinCompanionObject(clazz: NativeClassDescriptor, possibleCompanion: NativeClassDescriptor): Boolean
    fun getKotlinProperties(clazz: NativeClassDescriptor): List<KmProperty>
    fun getKotlinExtensionFunctions(clazz: NativeClassDescriptor): List<KmFunction>
}
