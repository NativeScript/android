package com.telerik.metadata.parsing

interface MetadataInfoAnnotationDescriptor {
    val superClassname: String
    fun skip(): Boolean
}
