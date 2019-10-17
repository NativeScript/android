package com.telerik.metadata.parsing.classes.bytecode

import com.telerik.metadata.parsing.classes.NativeFieldDescriptor
import com.telerik.metadata.parsing.classes.NativeTypeDescriptor

import org.apache.bcel.classfile.Field

class NativeFieldBytecodeDescriptor(f: Field) : NativeFieldDescriptor {

    override val isPublic = f.isPublic

    override val isProtected = f.isProtected

    override val isFinal = f.isFinal

    override val isStatic = f.isStatic

    override val name: String = f.name

    override val type = NativeTypeBytecodeDescriptor(f.type)
}
