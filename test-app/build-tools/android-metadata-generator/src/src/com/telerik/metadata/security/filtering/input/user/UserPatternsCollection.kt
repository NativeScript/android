package com.telerik.metadata.security.filtering.input.user

import com.telerik.metadata.security.filtering.input.PatternEntry
import com.telerik.metadata.security.filtering.input.PatternsCollection
import java.nio.file.Files
import java.nio.file.Path
import java.nio.file.Paths

object UserPatternsCollection : PatternsCollection {

    override var whitelistEntries: Collection<PatternEntry> = listOf(PatternEntry("*", "*"))
        private set

    override var blacklistEntries: Collection<PatternEntry> = emptyList()
        private set


    override fun populateWhitelistEntriesFromFile(filePath: String) {
        val path = Paths.get(filePath)

        if (!Files.exists(path)) {
            return
        }

        whitelistEntries = parseFile(path) +
                PatternEntry("java.lang", "Object") +
                PatternEntry("com.tns.gen*", "*")
    }

    override fun populateBlacklistEntriesFromFile(filePath: String) {
        val path = Paths.get(filePath)

        if (!Files.exists(path)) {
            return
        }

        blacklistEntries = parseFile(path)
    }

    private fun parseFile(path: Path): Collection<PatternEntry> {
        val fileLines = Files.readAllLines(path)
        val lineSplitter = UserLineSplitter()
        val lineFilter = UserLineFilter()

        return fileLines
                .asSequence()
                .filter { !lineFilter.isCommentLine(it) }
                .filter { !lineFilter.isEmptyLine(it) }
                .map { lineSplitter.splitLine(it) }
                .toList()
    }
}