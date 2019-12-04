package com.telerik.metadata.parsing.bytecode.fields

import com.telerik.metadata.parsing.NativeFieldDescriptor
import com.telerik.metadata.parsing.bytecode.types.NativeTypeBytecodeDescriptor

import org.apache.bcel.classfile.Field

abstract class NativeFieldBytecodeDescriptor(f: Field) : NativeFieldDescriptor {

    override val isFinal = f.isFinal

    override val isStatic = f.isStatic

    override val name: String = f.name

    override val type = NativeTypeBytecodeDescriptor(f.type)
}
