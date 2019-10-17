package com.telerik.metadata.parsing.classes

import com.telerik.metadata.parsing.classes.kotlin.metadata.MetadataAnnotation
import com.telerik.metadata.parsing.classes.kotlin.properties.KotlinPropertyDescriptor

import java.util.Optional

/**
 * Describes the properties of a native class
 */
interface NativeClassDescriptor {
    val isClass: Boolean

    val isInterface: Boolean

    val isEnum: Boolean

    val isStatic: Boolean

    val isPublic: Boolean

    val isProtected: Boolean

    val methods: Array<NativeMethodDescriptor>

    val properties: Array<KotlinPropertyDescriptor>

    val fields: Array<NativeFieldDescriptor>

    val metadataInfoAnnotation: MetadataInfoAnnotationDescriptor?

    val kotlinClassMetadataAnnotation: Optional<MetadataAnnotation>

    val interfaceNames: Array<String>

    val packageName: String

    val className: String

    val superclassName: String
}
