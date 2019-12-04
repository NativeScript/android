package com.telerik.metadata.parsing.kotlin.extensions.bytecode

import com.telerik.metadata.ClassUtil
import com.telerik.metadata.parsing.kotlin.classes.KotlinClassDescriptor
import com.telerik.metadata.parsing.kotlin.extensions.ClassNameAndFunctionPair
import com.telerik.metadata.parsing.kotlin.extensions.KotlinExtensionFunctionDescriptor
import com.telerik.metadata.parsing.kotlin.extensions.ExtensionFunctionsCollector
import com.telerik.metadata.parsing.kotlin.metadata.ClassMetadataParser
import com.telerik.metadata.parsing.kotlin.methods.KotlinMethodDescriptor
import kotlinx.metadata.jvm.signature
import java.util.*

class BytecodeExtensionFunctionsCollector(private val kotlinClassMetadataParser: ClassMetadataParser) : ExtensionFunctionsCollector {

    override fun collectExtensionFunctions(kotlinClassDescriptor: KotlinClassDescriptor): Collection<ClassNameAndFunctionPair<KotlinExtensionFunctionDescriptor>> {
        val kotlinMetadata = kotlinClassDescriptor.kotlinMetadata ?: return emptyList()

        val extensionFunctionsDescriptors = ArrayList<ClassNameAndFunctionPair<KotlinExtensionFunctionDescriptor>>()

        val extensionFunctions = kotlinClassMetadataParser.getKotlinExtensionFunctions(kotlinMetadata)

        for (extensionFunction in extensionFunctions) {
            val signature = extensionFunction.signature

            if (signature != null) {
                val functionName = signature.name
                val functionSignature = signature.desc

                val extensionFunctionDescriptor: KotlinMethodDescriptor = Arrays
                        .stream(kotlinClassDescriptor.methods)
                        .filter { x -> x.name == functionName && x.signature == functionSignature }
                        .findFirst()
                        .get()

                if (extensionFunctionDescriptor.isStatic) {
                    val receiverType = extensionFunctionDescriptor.argumentTypes[0] // kotlin extension functions' first argument is the receiver type
                    val receiverClassName = ClassUtil.getCanonicalName(receiverType.signature)
                    val classNameAndFunctionPair: ClassNameAndFunctionPair<KotlinExtensionFunctionDescriptor> = ClassNameAndFunctionPair(receiverClassName, KotlinExtensionFunctionBytecodeDescriptor(extensionFunctionDescriptor))
                    extensionFunctionsDescriptors.add(classNameAndFunctionPair)
                }
            }
        }

        return extensionFunctionsDescriptors
    }
}
