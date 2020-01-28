package com.telerik.metadata.security.filtering.input.user

import com.telerik.metadata.security.filtering.input.PatternEntry
import com.telerik.metadata.security.filtering.input.PatternsCollection
import java.net.URI
import java.nio.file.Files
import java.nio.file.Paths

object UserPatternsCollection : PatternsCollection {

    override var whitelistEntries: Collection<PatternEntry> = listOf(PatternEntry("*", "*"))
        private set

    override var blacklistEntries: Collection<PatternEntry> = emptyList()
        private set


    override fun populateWhitelistEntriesFromFile(filePath: String) {
        whitelistEntries = parseFile(filePath) + PatternEntry("java.lang", "Object")
    }

    override fun populateBlacklistEntriesFromFile(filePath: String) {
        blacklistEntries = parseFile(filePath)
    }

    private fun parseFile(path: String): Collection<PatternEntry> {
        val fileLines = Files.readAllLines(Paths.get(URI(path)))
        val lineSplitter = UserLineSplitter()
        val commentFilter = UserLineCommentFilter()

        return fileLines
                .filter { !commentFilter.isCommentLine(it) }
                .map { lineSplitter.splitLine(it) }
    }
}