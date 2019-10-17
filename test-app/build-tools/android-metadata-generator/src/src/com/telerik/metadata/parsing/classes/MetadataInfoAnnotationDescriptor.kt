package com.telerik.metadata.parsing.classes

interface MetadataInfoAnnotationDescriptor {
    val superClassname: String
    fun skip(): Boolean
}
