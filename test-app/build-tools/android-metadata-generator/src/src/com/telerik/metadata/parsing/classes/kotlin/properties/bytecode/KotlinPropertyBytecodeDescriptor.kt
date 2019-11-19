package com.telerik.metadata.parsing.classes.kotlin.properties.bytecode

import com.telerik.metadata.parsing.classes.NativeMethodDescriptor
import com.telerik.metadata.parsing.classes.kotlin.properties.KotlinPropertyDescriptor

class KotlinPropertyBytecodeDescriptor(override val name: String,
                                       override val getterMethod: NativeMethodDescriptor?,
                                       override val setterMethod: NativeMethodDescriptor?) : KotlinPropertyDescriptor
