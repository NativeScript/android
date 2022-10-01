package com.telerik.metadata.parsing.kotlin.fields

import com.telerik.metadata.parsing.bytecode.fields.NativeFieldBytecodeDescriptor
import org.apache.bcel.classfile.Field

class KotlinJvmFieldDescriptor(field: Field,
                               override val isPublic: Boolean,
                               override val isInternal: Boolean,
                               override val isProtected: Boolean,
                               override val isPackagePrivate: Boolean,
                               override val isPrivate: Boolean
) : NativeFieldBytecodeDescriptor(field)