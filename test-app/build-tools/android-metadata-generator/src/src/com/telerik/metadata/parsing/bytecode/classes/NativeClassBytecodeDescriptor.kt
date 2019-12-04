package com.telerik.metadata.parsing.bytecode.classes

import com.telerik.metadata.ClassUtil
import com.telerik.metadata.parsing.MetadataInfoAnnotationDescriptor
import com.telerik.metadata.parsing.NativeClassDescriptor
import org.apache.bcel.classfile.ConstantClass
import org.apache.bcel.classfile.ConstantUtf8
import org.apache.bcel.classfile.InnerClasses
import org.apache.bcel.classfile.JavaClass

abstract class NativeClassBytecodeDescriptor(protected val clazz: JavaClass) : NativeClassDescriptor {

    override val isClass = clazz.isClass

    override val isInterface = clazz.isInterface

    override val isEnum = clazz.isEnum

    override val isStatic = clazz.isStatic

    override val metadataInfoAnnotation: MetadataInfoAnnotationDescriptor? = null

    override val interfaceNames: Array<String> = clazz.interfaceNames

    override val packageName: String = clazz.packageName

    override val className: String = clazz.className

    override val superclassName: String = clazz.superclassName

    init {
        init()
    }

    private fun init() {
        if (clazz.className.contains("$")) {
            val name = ClassUtil.getSimpleName(this)
            var isAnonymousClass: Boolean
            try {
                Integer.parseInt(name)
                isAnonymousClass = true
            } catch (e: Exception) {
                isAnonymousClass = false
            }

            if (isAnonymousClass) {
                return
            }
            var found = false
            val fullClassName = getClassName(clazz.classNameIndex) ?: return

            for (a in clazz.attributes) {
                if (a is InnerClasses) {
                    val i = a.innerClasses
                    for (ic in i) {
                        val innerClassName = getClassName(ic.innerClassIndex)

                        if (fullClassName == innerClassName) {
                            val flags = ic.innerAccessFlags
                            clazz.accessFlags = flags
                            found = true
                            break
                        }
                    }
                }
                if (found) {
                    break
                }
            }
        }
    }

    private fun getClassName(classIndex: Int): String? {
        val constantPool = clazz.constantPool
        val innerClassNameIndex = constantPool.getConstant(classIndex) as ConstantClass
        val className = constantPool.getConstant(innerClassNameIndex.nameIndex) as ConstantUtf8
        return className.bytes
    }


}
