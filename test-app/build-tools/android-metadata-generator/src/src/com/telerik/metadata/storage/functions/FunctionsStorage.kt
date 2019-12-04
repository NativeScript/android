package com.telerik.metadata.storage.functions

import com.telerik.metadata.parsing.NativeMethodDescriptor
import com.telerik.metadata.parsing.kotlin.extensions.ClassNameAndFunctionPair

interface FunctionsStorage<T : NativeMethodDescriptor> {
    fun storeFunctions(extensionFunctions: Collection<ClassNameAndFunctionPair<T>>)
    fun retrieveFunctions(className: String): Collection<T>
}
