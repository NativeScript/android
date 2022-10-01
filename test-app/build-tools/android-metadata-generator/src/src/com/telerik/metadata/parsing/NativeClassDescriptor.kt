package com.telerik.metadata.parsing

/**
 * Describes the properties of a native class
 */
interface NativeClassDescriptor : AccessModifiable, NativeDescriptor {
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

    companion object Missing : NativeClassDescriptor {
        override val isClass = false
        override val isInterface = false
        override val isEnum = false
        override val isStatic = false
        override val methods = emptyArray<NativeMethodDescriptor>()
        override val properties = emptyArray<NativePropertyDescriptor>()
        override val fields = emptyArray<NativeFieldDescriptor>()
        override val metadataInfoAnnotation: MetadataInfoAnnotationDescriptor? = null
        override val interfaceNames = emptyArray<String>()
        override val packageName = ""
        override val className = ""
        override val superclassName = ""
        override val isPublic = false
        override val isInternal = false
        override val isProtected = false
        override val isPackagePrivate = false
        override val isPrivate = false

    }
}
