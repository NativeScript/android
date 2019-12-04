package com.telerik.metadata.parsing

/**
 * Describes the properties of a native class
 */
interface NativeClassDescriptor : AccessModifiable {
    val isClass: Boolean

    val isInterface: Boolean

    val isEnum: Boolean

    val isStatic: Boolean

    val methods: Array<out NativeMethodDescriptor>

    val properties: Array<out NativePropertyDescriptor>

    val fields: Array<out NativeFieldDescriptor>

    val metadataInfoAnnotation: MetadataInfoAnnotationDescriptor?

    val interfaceNames: Array<String>

    val packageName: String

    val className: String

    val superclassName: String
}
