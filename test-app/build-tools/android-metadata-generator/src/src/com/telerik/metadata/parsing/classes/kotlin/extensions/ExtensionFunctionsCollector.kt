package com.telerik.metadata.parsing.classes.kotlin.extensions

import com.telerik.metadata.parsing.classes.NativeClassDescriptor

/**
 * Handles collection of Kotlin extension functions
 */
interface ExtensionFunctionsCollector {
    /**
     * Collects Kotlin extension functions for a given native class descriptor
     *
     * @param classDescriptor class descriptor to use
     * @return collection of pairs consisting of class name and extension function
     */
    fun collectExtensionFunctions(classDescriptor: NativeClassDescriptor): Collection<ClassNameAndFunctionPair<KotlinExtensionFunctionDescriptor>>
}
