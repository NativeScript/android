package com.telerik.metadata.security.filtering.input

interface InputParser {
    fun parseFile(pathToFile: String): List<PatternEntry>
}