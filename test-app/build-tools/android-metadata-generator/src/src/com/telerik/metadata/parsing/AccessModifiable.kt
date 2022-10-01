package com.telerik.metadata.parsing

interface AccessModifiable {
    val isPublic: Boolean

    val isInternal: Boolean

    val isProtected: Boolean

    val isPackagePrivate: Boolean

    val isPrivate: Boolean
}