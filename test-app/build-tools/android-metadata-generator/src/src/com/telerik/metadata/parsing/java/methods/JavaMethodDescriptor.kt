package com.telerik.metadata.parsing.java.methods

import com.telerik.metadata.parsing.NativeClassDescriptor
import com.telerik.metadata.parsing.bytecode.methods.NativeMethodBytecodeDescriptor
import org.apache.bcel.classfile.Method

class JavaMethodDescriptor(method: Method, originClass: NativeClassDescriptor) : NativeMethodBytecodeDescriptor(method, originClass) {
    override val isPublic = method.isPublic
    override val isInternal = false // java has no internal modifier
    override val isProtected = method.isProtected
}