package com.telerik.metadata.parsing

interface NativePropertyDescriptor {
    val name: String
    val getterMethod: NativeMethodDescriptor?
    val setterMethod: NativeMethodDescriptor?
}