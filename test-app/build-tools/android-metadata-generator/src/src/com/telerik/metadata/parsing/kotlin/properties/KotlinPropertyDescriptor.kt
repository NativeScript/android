package com.telerik.metadata.parsing.kotlin.properties

import com.telerik.metadata.parsing.NativeMethodDescriptor
import com.telerik.metadata.parsing.NativePropertyDescriptor

class KotlinPropertyDescriptor(override val name: String,
                               override val getterMethod: NativeMethodDescriptor?,
                               override val setterMethod: NativeMethodDescriptor?, val duplicate: Boolean = false ) : NativePropertyDescriptor
