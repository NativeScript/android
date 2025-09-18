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
import org.apache.bcel.classfile.JavaClass
import java.util.*
import java.util.stream.Collectors
import kotlin.metadata.KmClass
import kotlin.metadata.KmProperty
import kotlin.metadata.Visibility
import kotlin.metadata.jvm.KotlinClassMetadata
import kotlin.metadata.jvm.Metadata
import kotlin.metadata.jvm.getterSignature
import kotlin.metadata.jvm.setterSignature
import kotlin.metadata.visibility

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
            val metaClass = meta.kmClass
            kotlinMetadataProperties = metaClass.properties

            val possibleCompanionField = getCompanionFieldIfAny(nativeClass, metaClass)
            if (possibleCompanionField.isPresent) {
                fields.add(possibleCompanionField.get())
            }

            val possibleObjectInstanceField = getObjectFieldIfAny(nativeClass)
            if (possibleObjectInstanceField.isPresent) {
                fields.add(possibleObjectInstanceField.get())
            }

            if (metaClass.kmEnumEntries.isNotEmpty()) {
                val enums: Collection<String> = metaClass.kmEnumEntries.map { it.name }
                val enumFields = getEnumEntriesAsFields(nativeClass, enums)
                fields.addAll(enumFields)
            }

        } else if (meta is KotlinClassMetadata.FileFacade) {
            kotlinMetadataProperties = meta.kmPackage.properties
        } else if (meta is KotlinClassMetadata.MultiFileClassPart) {
            kotlinMetadataProperties = meta.kmPackage.properties
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
                            isPublic = prop.visibility == Visibility.PUBLIC,
                            isInternal = prop.visibility == Visibility.INTERNAL,
                            isProtected = prop.visibility == Visibility.PROTECTED,
                            isPackagePrivate,
                            isPrivate = prop.visibility == Visibility.PRIVATE,
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
                        getter = getMethodDescriptorWithSignature(getterSignature.name,
                            getterSignature.descriptor
                        )
                    }

                    var setter: NativeMethodDescriptor? = null
                    val setterSignature = it.setterSignature
                    if (setterSignature != null) {
                        setter = getMethodDescriptorWithSignature(setterSignature.name,
                            setterSignature.descriptor
                        )
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
            is KotlinClassMetadata.Class -> metadata.kmClass.visibility == Visibility.PUBLIC
            else -> clazz.isPublic
        }
    }

    override val isInternal by lazy {
        when (val metadata = kotlinMetadata) {
            is KotlinClassMetadata.Class -> metadata.kmClass.visibility == Visibility.INTERNAL
            else -> false
        }
    }

    override val isProtected by lazy {
        when (val metadata = kotlinMetadata) {
            is KotlinClassMetadata.Class -> metadata.kmClass.visibility == Visibility.PROTECTED
            else -> clazz.isProtected
        }
    }

    override val isPrivate by lazy {
        when (val metadata = kotlinMetadata) {
            is KotlinClassMetadata.Class -> metadata.kmClass.visibility == Visibility.PRIVATE
            else -> clazz.isPrivate
        }
    }

    private val metadataParser = BytecodeClassMetadataParser()
    internal val kotlinMetadata: KotlinClassMetadata? by lazy {
        val metadata = Metadata(
                metadataAnnotation.kind,
                metadataAnnotation.metadataVersion,
                metadataAnnotation.data1,
                metadataAnnotation.data2,
                metadataAnnotation.extraString,
                metadataAnnotation.packageName,
                metadataAnnotation.extraInt)

        KotlinClassMetadata.readStrict(metadata)
    }


}