package com.telerik.metadata.security.filtering.input

interface PatternsCollection {
    val whitelistEntries: Collection<PatternEntry>
    val blacklistEntries: Collection<PatternEntry>

    fun populateWhitelistEntriesFromFile(filePath: String)
    fun populateBlacklistEntriesFromFile(filePath: String)
}