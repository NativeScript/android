package com.telerik.metadata.security.classes

import com.telerik.metadata.parsing.NativeClassDescriptor
import com.telerik.metadata.security.MetadataSecurityViolationException
import com.telerik.metadata.security.SecuredNativeDescriptor

class SecuredNativeClassDescriptor(isUsageAllowed: Boolean, private val nativeDescriptorInput: NativeClassDescriptor) : SecuredNativeDescriptor {

    override val isUsageAllowed: Boolean = isUsageAllowed
        get() = field && nativeDescriptorInput !== NativeClassDescriptor.Missing

    override val nativeDescriptor: NativeClassDescriptor = nativeDescriptorInput
        get() = if (isUsageAllowed) field else throw MetadataSecurityViolationException("Usage of this class (${field.className}) is not allowed!")
}