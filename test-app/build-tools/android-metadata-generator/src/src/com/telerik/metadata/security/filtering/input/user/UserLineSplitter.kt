package com.telerik.metadata.security.filtering.input.user

import com.telerik.metadata.security.filtering.input.PatternEntry

class UserLineSplitter {

    fun splitLine(line: String): PatternEntry {
        val splits = line.split(':')

        if (splits.size == 1) {
            val split = splits[0].trim()

            assertNotEmptySplit(split, line)

            return PatternEntry(split, "")
        }

        if (splits.size == 2) {
            val firstSplit = splits[0].trim()
            val secondSplit = splits[1].trim()

            return PatternEntry(firstSplit, secondSplit)
        }

        throw InvalidUserInputException("Invalid entry line: $line")
    }

    private fun assertNotEmptySplit(split: String, line: String) {
        if (split == "") {
            throw InvalidUserInputException("Invalid entry line: $line")
        }
    }
}