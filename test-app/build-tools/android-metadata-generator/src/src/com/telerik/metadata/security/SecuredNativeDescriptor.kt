package com.telerik.metadata.security

import com.telerik.metadata.parsing.NativeDescriptor

interface SecuredNativeDescriptor {
    val isUsageAllowed: Boolean
    val nativeDescriptor: NativeDescriptor
}