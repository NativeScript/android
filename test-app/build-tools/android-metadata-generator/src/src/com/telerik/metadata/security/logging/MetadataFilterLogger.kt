package com.telerik.metadata.security.logging

import com.telerik.metadata.security.filtering.MetadataFilterResult

interface MetadataFilterLogger {
    fun logAllowed(packageName: String, className: String, whitelistFilterResult: MetadataFilterResult, blacklistFilterResult: MetadataFilterResult)
    fun logDisallowed(packageName: String, className: String, whitelistFilterResult: MetadataFilterResult, blacklistFilterResult: MetadataFilterResult)
}