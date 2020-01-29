package com.telerik.metadata.security.filtering.matching.impl

import com.telerik.metadata.security.filtering.matching.PatternMatcher

class PatternMatcherImpl : PatternMatcher {

    override fun match(pattern: String, input: String): Boolean {
        /*
        Algorithm is borrowed from: https://www.geeksforgeeks.org/wildcard-character-matching/
         */


        // Short circuit if pattern is '*'
        if (pattern == "*" && input.isNotEmpty()) {
            return true
        }

        // If we reach at the end of both strings,
        // we are done
        if (pattern.isEmpty() && input.isEmpty())
            return true

        // Make sure that the characters after '*'
        // are present in second string.
        // This function assumes that the first
        // string will not contain two consecutive '*'
        if (pattern.length > 1 && pattern[0] == '*' &&
                input.isEmpty())
            return false

        // If the first string contains '?',
        // or current characters of both strings match
        if (pattern.length > 1 && pattern[0] == '?' || pattern.isNotEmpty() && input.isNotEmpty() &&
                pattern[0] == input[0])
            return match(pattern.substring(1),
                    input.substring(1))

        // If there is *, then there are two possibilities
        // a) We consider current character of second string
        // b) We ignore current character of second string.
        return if (pattern.isNotEmpty() && pattern[0] == '*') match(pattern.substring(1), input) || match(pattern, input.substring(1)) else false
    }

}