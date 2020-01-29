package com.telerik.metadata.security.filtering.input.user

class UserLineCommentFilter {
    fun isCommentLine(line: String) = line.startsWith("//") || line.startsWith("#")
}