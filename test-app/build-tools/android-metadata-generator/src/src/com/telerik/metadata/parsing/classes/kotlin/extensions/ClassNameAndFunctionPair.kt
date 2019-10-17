package com.telerik.metadata.parsing.classes.kotlin.extensions

import com.telerik.metadata.parsing.classes.NativeMethodDescriptor

class ClassNameAndFunctionPair<T : NativeMethodDescriptor>(val className: String, val function: T)
