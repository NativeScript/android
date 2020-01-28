package com.telerik.metadata.security.filtering.input.user

import com.telerik.metadata.security.filtering.input.PatternEntry
import com.telerik.metadata.security.filtering.input.InputParser
import java.net.URI
import java.nio.file.Files
import java.nio.file.Paths

class UserInputParser : InputParser {
    override fun parseFile(pathToFile: String): List<PatternEntry> {
        val fileLines = Files.readAllLines(Paths.get(URI(pathToFile)))
        val lineSplitter = UserLineSplitter()
        val commentFilter = UserLineCommentFilter()

        return fileLines
                .filter { !commentFilter.isCommentLine(it) }
                .map { lineSplitter.splitLine(it) }

    }
}