package com.telerik.metadata.security.filtering.input

data class PatternEntry(val packagePattern: String, val classPattern: String) {
    companion object {
        fun empty(): PatternEntry {
            return PatternEntry("", "")
        }
    }

    override fun toString() = "$packagePattern:$classPattern"
}