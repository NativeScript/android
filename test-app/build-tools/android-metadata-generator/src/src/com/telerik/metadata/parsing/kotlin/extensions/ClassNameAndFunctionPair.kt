package com.telerik.metadata.parsing.kotlin.extensions

import com.telerik.metadata.parsing.NativeMethodDescriptor

class ClassNameAndFunctionPair<T : NativeMethodDescriptor>(val className: String, val function: T)
