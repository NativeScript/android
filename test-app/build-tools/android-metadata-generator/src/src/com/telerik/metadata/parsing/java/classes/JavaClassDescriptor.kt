package com.telerik.metadata.parsing.java.classes

import com.telerik.metadata.parsing.NativeFieldDescriptor
import com.telerik.metadata.parsing.NativeMethodDescriptor
import com.telerik.metadata.parsing.NativePropertyDescriptor
import com.telerik.metadata.parsing.bytecode.classes.NativeClassBytecodeDescriptor
import com.telerik.metadata.parsing.java.fields.JavaFieldDescriptor
import com.telerik.metadata.parsing.java.methods.JavaMethodDescriptor
import org.apache.bcel.classfile.JavaClass

class JavaClassDescriptor(nativeClass: JavaClass, override val isPackagePrivate: Boolean) : NativeClassBytecodeDescriptor(nativeClass) {

    override val methods: Array<JavaMethodDescriptor>
        get() {
            val ms = clazz.methods
            return Array(ms.size) {
                JavaMethodDescriptor(ms[it], this, isPackagePrivate)
            }
        }

    override val fields: Array<JavaFieldDescriptor>
        get() {
            val fs = clazz.fields
            return Array(fs.size) {
                JavaFieldDescriptor(fs[it], isPackagePrivate)
            }
        }

    override val properties: Array<out NativePropertyDescriptor> = emptyArray() // java classes have no properties
    override val isPublic = nativeClass.isPublic
    override val isInternal = false
    override val isProtected = nativeClass.isProtected
    override val isPrivate = nativeClass.isPrivate
}