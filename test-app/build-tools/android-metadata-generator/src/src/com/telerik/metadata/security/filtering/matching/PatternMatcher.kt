package com.telerik.metadata.security.filtering.matching

interface PatternMatcher {
    fun match(pattern: String, input: String): Boolean
}