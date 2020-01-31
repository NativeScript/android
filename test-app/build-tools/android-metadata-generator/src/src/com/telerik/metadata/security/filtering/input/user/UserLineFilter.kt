package com.telerik.metadata.security.filtering.input.user

class UserLineFilter {
    fun isCommentLine(line: String) = line.startsWith("//") || line.startsWith("#")
    fun isEmptyLine(line: String) = line.isBlank()
}