package com.telerik.metadata.security.filtering

interface MetadataFilter {
    fun isAllowed(packageName: String, className: String): MetadataFilterResult
}