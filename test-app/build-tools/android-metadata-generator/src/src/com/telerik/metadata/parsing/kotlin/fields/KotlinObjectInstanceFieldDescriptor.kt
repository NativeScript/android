package com.telerik.metadata.parsing.kotlin.fields

import com.telerik.metadata.parsing.bytecode.fields.NativeFieldBytecodeDescriptor
import com.telerik.metadata.parsing.kotlin.classes.KotlinClassDescriptor
import org.apache.bcel.classfile.Field

class KotlinObjectInstanceFieldDescriptor(field: Field, companion: KotlinClassDescriptor) : NativeFieldBytecodeDescriptor(field) {
    override val isPublic = companion.isPublic
    override val isInternal = companion.isInternal
    override val isProtected = companion.isProtected
}