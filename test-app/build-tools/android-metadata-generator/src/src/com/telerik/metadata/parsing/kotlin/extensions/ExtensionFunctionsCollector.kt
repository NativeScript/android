package com.telerik.metadata.parsing.kotlin.extensions

import com.telerik.metadata.parsing.NativeClassDescriptor
import com.telerik.metadata.parsing.kotlin.classes.KotlinClassDescriptor

/**
 * Handles collection of Kotlin extension functions
 */
interface ExtensionFunctionsCollector {
    /**
     * Collects Kotlin extension functions for a given native class descriptor
     *
     * @param kotlinClassDescriptor Kotlin class descriptor to use
     * @return collection of pairs consisting of class name and extension function
     */
    fun collectExtensionFunctions(kotlinClassDescriptor: KotlinClassDescriptor): Collection<ClassNameAndFunctionPair<KotlinExtensionFunctionDescriptor>>
}
