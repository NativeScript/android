package com.telerik.metadata.parsing

interface NativePropertyDescriptor : NativeDescriptor {
    val name: String
    val getterMethod: NativeMethodDescriptor?
    val setterMethod: NativeMethodDescriptor?
}