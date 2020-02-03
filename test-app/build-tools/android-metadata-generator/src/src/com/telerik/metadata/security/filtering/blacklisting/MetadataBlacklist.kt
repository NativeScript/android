package com.telerik.metadata.security.filtering.blacklisting

import com.telerik.metadata.security.filtering.MetadataFilter
import com.telerik.metadata.security.filtering.MetadataFilterResult
import com.telerik.metadata.security.filtering.input.PatternEntry
import com.telerik.metadata.security.filtering.matching.PatternMatcher
import java.util.*

class MetadataBlacklist(private val userInput: Collection<PatternEntry>, private val patternMatcher: PatternMatcher) : MetadataFilter {

    override fun isAllowed(packageName: String, className: String): MetadataFilterResult {
        val responsiblePatternEntry = userInput.firstOrNull { inputEntry ->
            (inputEntry.packagePattern.isEmpty() || patternMatcher.match(inputEntry.packagePattern, packageName))
                    && (inputEntry.classPattern.isEmpty() || patternMatcher.match(inputEntry.classPattern, className))
        }

        return if (responsiblePatternEntry != null) MetadataFilterResult(false, Optional.of(responsiblePatternEntry)) else MetadataFilterResult(true)
    }
}