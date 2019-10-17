package com.telerik.metadata.parsing.classes

/**
 * Describes the properties of a native method
 */
interface NativeMethodDescriptor {
    val isPublic: Boolean
    val isProtected: Boolean
    val isSynthetic: Boolean
    val isStatic: Boolean
    val isAbstract: Boolean

    val declaringClass: NativeClassDescriptor

    val name: String
    val signature: String

    val argumentTypes: Array<NativeTypeDescriptor>
    val returnType: NativeTypeDescriptor

    val metadataInfoAnnotation: MetadataInfoAnnotationDescriptor?
}
