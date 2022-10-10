package com.telerik.metadata.parsing.java.fields

import com.telerik.metadata.parsing.bytecode.fields.NativeFieldBytecodeDescriptor
import org.apache.bcel.classfile.Field

class JavaFieldDescriptor(field: Field, override val isPackagePrivate: Boolean) : NativeFieldBytecodeDescriptor(field) {
    override val isPublic = field.isPublic
    override val isInternal = false // java has no internal modifier
    override val isProtected = field.isProtected
    override val isPrivate = field.isPrivate
}