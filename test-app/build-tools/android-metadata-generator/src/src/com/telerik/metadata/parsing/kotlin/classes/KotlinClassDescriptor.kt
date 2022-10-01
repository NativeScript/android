package com.telerik.metadata.parsing.kotlin.classes

import com.telerik.metadata.ClassUtil
import com.telerik.metadata.ClassUtil.getCanonicalName
import com.telerik.metadata.parsing.NativeFieldDescriptor
import com.telerik.metadata.parsing.NativeMethodDescriptor
import com.telerik.metadata.parsing.NativePropertyDescriptor
import com.telerik.metadata.parsing.bytecode.classes.NativeClassBytecodeDescriptor
import com.telerik.metadata.parsing.kotlin.fields.KotlinCompanionFieldDescriptor
import com.telerik.metadata.parsing.kotlin.fields.KotlinEnumFieldDescriptor
import com.telerik.metadata.parsing.kotlin.fields.KotlinJvmFieldDescriptor
import com.telerik.metadata.parsing.kotlin.fields.KotlinObjectInstanceFieldDescriptor
import com.telerik.metadata.parsing.kotlin.metadata.MetadataAnnotation
import com.telerik.metadata.parsing.kotlin.metadata.bytecode.BytecodeClassMetadataParser
import com.telerik.metadata.parsing.kotlin.methods.KotlinMethodDescriptor
import com.telerik.metadata.parsing.kotlin.properties.KotlinPropertyDescriptor
import com.telerik.metadata.security.classes.SecuredClassRepository
import kotlinx.metadata.Flag
import kotlinx.metadata.KmClass
import kotlinx.metadata.KmProperty
import kotlinx.metadata.jvm.KotlinClassHeader
import kotlinx.metadata.jvm.KotlinClassMetadata
import kotlinx.metadata.jvm.getterSignature
import kotlinx.metadata.jvm.setterSignature
import org.apache.bcel.classfile.JavaClass
import java.io.IOException
import java.nio.file.Files
import java.nio.file.Paths
import java.nio.file.StandardOpenOption
import java.util.*
import java.util.stream.Collectors

class KotlinClassDescriptor(nativeClass: JavaClass, private val metadataAnnotation: MetadataAnnotation,
                            override val isPackagePrivate: Boolean
) : NativeClassBytecodeDescriptor(nativeClass) {


    override val methods: Array<KotlinMethodDescriptor> by lazy {
        val ms = clazz.methods
        return@lazy Array(ms.size) {
            KotlinMethodDescriptor(ms[it], this, isPackagePrivate)
        }
    }

    override val fields: Array<NativeFieldDescriptor> by lazy {
        val fields = ArrayList<NativeFieldDescriptor>()
        val meta = kotlinMetadata
        var kotlinMetadataProperties: Collection<KmProperty> = emptyList()

        if (meta is KotlinClassMetadata.Class) {
            val metaClass = meta.toKmClass()
            kotlinMetadataProperties = metaClass.properties

            val possibleCompanionField = getCompanionFieldIfAny(nativeClass, metaClass)
            if (possibleCompanionField.isPresent) {
                fields.add(possibleCompanionField.get())
            }

            val possibleObjectInstanceField = getObjectFieldIfAny(nativeClass)
            if (possibleObjectInstanceField.isPresent) {
                fields.add(possibleObjectInstanceField.get())
            }

            if (metaClass.enumEntries.isNotEmpty()) {

                val enumFields = getEnumEntriesAsFields(nativeClass, metaClass.enumEntries)
                fields.addAll(enumFields)
            }

        } else if (meta is KotlinClassMetadata.FileFacade) {
            kotlinMetadataProperties = meta.toKmPackage().properties
        } else if (meta is KotlinClassMetadata.MultiFileClassPart) {
            kotlinMetadataProperties = meta.toKmPackage().properties
        }


        val jvmFields = getJvmFieldAnnotatedFields(nativeClass, kotlinMetadataProperties)
        fields.addAll(jvmFields)

        return@lazy fields.toTypedArray()
    }

    private fun getJvmFieldAnnotatedFields(nativeClass: JavaClass, metadataProperties: Collection<KmProperty>): Collection<KotlinJvmFieldDescriptor> {
        val jvmFields = nativeClass
                .fields
                .filter {
                    it.annotationEntries.any { annotationEntry -> annotationEntry.annotationType == "Lkotlin/jvm/JvmField;" }
                }


        val kotlinFields = ArrayList<KotlinJvmFieldDescriptor>()

        for (field in jvmFields) {
            for (prop in metadataProperties) {
                if (field.name == prop.name) {
                    val kotlinField = KotlinJvmFieldDescriptor(
                            field = field,
                            isPublic = Flag.IS_PUBLIC(prop.flags),
                            isInternal = Flag.IS_INTERNAL(prop.flags),
                            isProtected = Flag.IS_PROTECTED(prop.flags),
                            isPackagePrivate,
                            isPrivate = Flag.IS_PRIVATE(prop.flags),
                    )

                    kotlinFields.add(kotlinField)
                }
            }
        }

        return kotlinFields
    }

    private fun getCompanionFieldIfAny(nativeClass: JavaClass, kmClass: KmClass): Optional<KotlinCompanionFieldDescriptor> {
        val companionName = kmClass.companionObject
        if (!companionName.isNullOrEmpty()) {
            val fullCompanionName = "$className$$companionName"
            val canonicalName = ClassUtil.getCanonicalName(fullCompanionName)
            val possibleCompanionClass = SecuredClassRepository.findClass(canonicalName)
            if (possibleCompanionClass.isUsageAllowed) {
                val companionClass = possibleCompanionClass.nativeDescriptor
                if (companionClass is KotlinClassDescriptor) {
                    val companionField = nativeClass
                            .fields
                            .single { it.name == companionName }
                    val companionFieldDescriptor = KotlinCompanionFieldDescriptor(companionField, companionClass)
                    return Optional.of(companionFieldDescriptor)
                }
            }
        }

        return Optional.empty()
    }

    private fun getObjectFieldIfAny(nativeClass: JavaClass): Optional<KotlinObjectInstanceFieldDescriptor> {
        return nativeClass
                .fields
                .singleOrNull { field ->
                    field.name == "INSTANCE" && getCanonicalName(field.type.signature) == nativeClass.className
                }.run {
                    if (this != null) {
                        val objectInstanceField = KotlinObjectInstanceFieldDescriptor(this@run, this@KotlinClassDescriptor)
                        Optional.of(objectInstanceField)
                    } else {
                        Optional.empty()
                    }
                }
    }

    private fun getEnumEntriesAsFields(nativeClass: JavaClass, metadataEnumEntries: Collection<String>): Collection<KotlinEnumFieldDescriptor> {
        val bytecodeFields = nativeClass.fields

        val matchingEnumFields = bytecodeFields
                .filter {
                    metadataEnumEntries.contains(it.name)
                }
                .map {
                    KotlinEnumFieldDescriptor(it, isPublic, isInternal, isProtected, isPackagePrivate, isPrivate)
                }

        return matchingEnumFields
    }

    override val properties: Array<out NativePropertyDescriptor> by lazy {
        // checking if prop is also jvm field if it is remove to avoid the runtime using the invalid data
        val fields = this.fields
        val metadata = kotlinMetadata
        if (metadata is KotlinClassMetadata.Class) {
            val props = metadataParser
                .getKotlinProperties(metadata)
                .map {
                    val propertyName = it.name
                    var duplicate = false

                    if(fields.find { field ->  field.name == propertyName} != null){
                        duplicate = true
                    }

                    // Ignoring kotlin fields with the @JvmName annotation since they will be included through the java method passed to the annotation
                   val hasJvmNameMethod =  nativeClass
                        .methods
                        .find { field ->  field.name == propertyName }
                        ?.annotationEntries?.any {
                                annotationEntry -> annotationEntry.annotationType == "Lkotlin/jvm/JvmName;"
                        } ?: false


                    if (hasJvmNameMethod){
                        duplicate = true
                    }

                    var getter: NativeMethodDescriptor? = null
                    val getterSignature = it.getterSignature
                    if (getterSignature != null) {
                        getter = getMethodDescriptorWithSignature(getterSignature.name, getterSignature.desc)
                    }

                    var setter: NativeMethodDescriptor? = null
                    val setterSignature = it.setterSignature
                    if (setterSignature != null) {
                        setter = getMethodDescriptorWithSignature(setterSignature.name, setterSignature.desc)
                    }

                    KotlinPropertyDescriptor(propertyName, getter, setter, duplicate)
                }.filter {
                    !it.duplicate
                }.collect(Collectors.toList())
            return@lazy props.toTypedArray()
        }

        return@lazy emptyArray<KotlinPropertyDescriptor>()
    }

    private fun getMethodDescriptorWithSignature(name: String, signature: String): NativeMethodDescriptor? {
        for (method in clazz.methods) {
            val methodSignature = method.signature
            val methodName = method.name

            if (methodSignature == signature && methodName == name) {
                return KotlinMethodDescriptor(method, this, isPackagePrivate)
            }
        }

        return null
    }

    override val isPublic by lazy {
        when (val metadata = kotlinMetadata) {
            is KotlinClassMetadata.Class -> Flag.IS_PUBLIC(metadata.toKmClass().flags)
            else -> clazz.isPublic
        }
    }

    override val isInternal by lazy {
        when (val metadata = kotlinMetadata) {
            is KotlinClassMetadata.Class -> Flag.IS_INTERNAL(metadata.toKmClass().flags)
            else -> false
        }
    }

    override val isProtected by lazy {
        when (val metadata = kotlinMetadata) {
            is KotlinClassMetadata.Class -> Flag.IS_PROTECTED(metadata.toKmClass().flags)
            else -> clazz.isProtected
        }
    }

    override val isPrivate by lazy {
        when (val metadata = kotlinMetadata) {
            is KotlinClassMetadata.Class -> Flag.IS_PRIVATE(metadata.toKmClass().flags)
            else -> clazz.isPrivate
        }
    }

    private val metadataParser = BytecodeClassMetadataParser()
    internal val kotlinMetadata: KotlinClassMetadata? by lazy {
        val kotlinClassHeader = KotlinClassHeader(
                metadataAnnotation.kind,
                metadataAnnotation.metadataVersion,
                metadataAnnotation.data1,
                metadataAnnotation.data2,
                metadataAnnotation.extraString,
                metadataAnnotation.packageName,
                metadataAnnotation.extraInt)

        KotlinClassMetadata.read(kotlinClassHeader)
    }


}