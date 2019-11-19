package com.telerik.metadata.parsing.classes.bytecode

import com.telerik.metadata.ClassUtil
import com.telerik.metadata.parsing.classes.MetadataInfoAnnotationDescriptor
import com.telerik.metadata.parsing.classes.NativeClassDescriptor
import com.telerik.metadata.parsing.classes.NativeFieldDescriptor
import com.telerik.metadata.parsing.classes.NativeMethodDescriptor
import com.telerik.metadata.parsing.classes.kotlin.metadata.MetadataAnnotation
import com.telerik.metadata.parsing.classes.kotlin.metadata.bytecode.BytecodeClassMetadataParser
import com.telerik.metadata.parsing.classes.kotlin.metadata.bytecode.BytecodeMetadataAnnotation
import com.telerik.metadata.parsing.classes.kotlin.properties.KotlinPropertyDescriptor
import com.telerik.metadata.parsing.classes.kotlin.properties.bytecode.KotlinPropertyBytecodeDescriptor
import kotlinx.metadata.jvm.getterSignature
import kotlinx.metadata.jvm.setterSignature
import org.apache.bcel.classfile.ConstantClass
import org.apache.bcel.classfile.ConstantUtf8
import org.apache.bcel.classfile.InnerClasses
import org.apache.bcel.classfile.JavaClass
import java.util.*

class NativeClassBytecodeDescriptor(private val clazz: JavaClass) : NativeClassDescriptor {

    override val isClass = clazz.isClass

    override val isInterface = clazz.isInterface

    override val isEnum = clazz.isEnum

    override val isStatic = clazz.isStatic

    override val isPublic = clazz.isPublic

    override val isProtected = clazz.isProtected

    override val methods: Array<NativeMethodDescriptor>
        get() {
            val ms = clazz.methods
            return Array(ms.size) {
                NativeMethodBytecodeDescriptor(ms[it], this)
            }
        }

    override val properties: Array<KotlinPropertyDescriptor>
        get() {
            val propertyDescriptors = ArrayList<KotlinPropertyDescriptor>()
            val kotlinClassMetadataParser = BytecodeClassMetadataParser()
            val kotlinProperties = kotlinClassMetadataParser.getKotlinProperties(this)
            for (kmProperty in kotlinProperties) {
                val propertyName = kmProperty.name

                var getter: NativeMethodDescriptor? = null
                val getterSignature = kmProperty.getterSignature
                if (getterSignature != null) {
                    getter = getMethodDescriptorWithSignature(getterSignature.name, getterSignature.desc)
                }

                var setter: NativeMethodDescriptor? = null
                val setterSignature = kmProperty.setterSignature
                if (setterSignature != null) {
                    setter = getMethodDescriptorWithSignature(setterSignature.name, setterSignature.desc)
                }

                propertyDescriptors.add(KotlinPropertyBytecodeDescriptor(propertyName, getter, setter))

            }

            return propertyDescriptors.toTypedArray()
        }

    override val fields: Array<NativeFieldDescriptor>
        get() {
            val fs = clazz.fields
            return Array(fs.size) {
                NativeFieldBytecodeDescriptor(fs[it])
            }
        }

    override val metadataInfoAnnotation: MetadataInfoAnnotationDescriptor? = null

    override val kotlinClassMetadataAnnotation: Optional<MetadataAnnotation>
        get() {
            val annotationEntries = clazz.annotationEntries
            if (annotationEntries != null) {
                for (annotationEntry in annotationEntries) {
                    val annotationType = annotationEntry.annotationType
                    if ("Lkotlin/Metadata;" == annotationType) {
                        val kotlinClassMetadataAnnotation = BytecodeMetadataAnnotation(annotationEntry)
                        return Optional.of(kotlinClassMetadataAnnotation)
                    }
                }
            }

            return Optional.empty()
        }

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

    private fun getMethodDescriptorWithSignature(name: String, signature: String): NativeMethodDescriptor? {
        for (method in clazz.methods) {
            val methodSignature = method.signature
            val methodName = method.name

            if (methodSignature == signature && methodName == name) {
                return NativeMethodBytecodeDescriptor(method, this)
            }
        }

        return null
    }
}
