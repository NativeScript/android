package com.telerik.metadata.parsing.classes.kotlin.extensions.bytecode

import com.telerik.metadata.ClassUtil
import com.telerik.metadata.parsing.classes.NativeClassDescriptor
import com.telerik.metadata.parsing.classes.kotlin.extensions.ClassNameAndFunctionPair
import com.telerik.metadata.parsing.classes.kotlin.extensions.KotlinExtensionFunctionDescriptor
import com.telerik.metadata.parsing.classes.kotlin.extensions.ExtensionFunctionsCollector
import com.telerik.metadata.parsing.classes.kotlin.metadata.ClassMetadataParser
import kotlinx.metadata.jvm.signature
import java.util.*

class BytecodeExtensionFunctionsCollector(private val kotlinClassMetadataParser: ClassMetadataParser) : ExtensionFunctionsCollector {

    override fun collectExtensionFunctions(classDescriptor: NativeClassDescriptor): Collection<ClassNameAndFunctionPair<KotlinExtensionFunctionDescriptor>> {
        val extensionFunctionsDescriptors = ArrayList<ClassNameAndFunctionPair<KotlinExtensionFunctionDescriptor>>()

        val extensionFunctions = kotlinClassMetadataParser.getKotlinExtensionFunctions(classDescriptor)

        for (extensionFunction in extensionFunctions) {
            val signature = extensionFunction.signature

            if (signature != null) {
                val functionName = signature.name
                val functionSignature = signature.desc

                val extensionFunctionDescriptor = Arrays
                        .stream(classDescriptor.methods)
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
