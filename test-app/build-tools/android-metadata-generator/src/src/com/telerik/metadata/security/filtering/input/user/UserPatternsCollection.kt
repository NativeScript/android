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
        whitelistEntries = parseFile(filePath) +
                PatternEntry("java.lang", "Object") +
                PatternEntry("com.tns.gen*", "*")
    }

    override fun populateBlacklistEntriesFromFile(filePath: String) {
        blacklistEntries = parseFile(filePath)
    }

    private fun parseFile(filePath: String): Collection<PatternEntry> {
        val path = Paths.get(filePath)

        if (!Files.exists(path)) {
            return emptyList()
        }

        val fileLines = Files.readAllLines(path)
        val lineSplitter = UserLineSplitter()
        val commentFilter = UserLineCommentFilter()

        return fileLines
                .filter { !commentFilter.isCommentLine(it) }
                .map { lineSplitter.splitLine(it) }
    }
}