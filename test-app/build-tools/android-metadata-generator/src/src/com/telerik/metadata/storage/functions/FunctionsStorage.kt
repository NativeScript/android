package com.telerik.metadata.storage.functions

import com.telerik.metadata.parsing.classes.NativeMethodDescriptor
import com.telerik.metadata.parsing.classes.kotlin.extensions.ClassNameAndFunctionPair

interface FunctionsStorage<T : NativeMethodDescriptor> {
    fun storeFunctions(extensionFunctions: Collection<ClassNameAndFunctionPair<T>>)
    fun retrieveFunctions(className: String): Collection<T>
}
